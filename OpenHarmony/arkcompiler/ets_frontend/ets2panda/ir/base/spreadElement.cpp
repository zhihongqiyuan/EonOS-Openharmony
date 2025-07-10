/**
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "spreadElement.h"
#include "es2panda.h"

#include "checker/TSchecker.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/base/decorator.h"
#include "ir/typeNode.h"
#include "ir/expressions/arrayExpression.h"
#include "ir/expressions/objectExpression.h"

namespace ark::es2panda::ir {
SpreadElement::SpreadElement([[maybe_unused]] Tag const tag, SpreadElement const &other,
                             ArenaAllocator *const allocator)
    : AnnotatedExpression(static_cast<AnnotatedExpression const &>(other), allocator), decorators_(allocator->Adapter())
{
    optional_ = other.optional_;

    if (other.argument_ != nullptr) {
        argument_ = other.argument_->Clone(allocator, this)->AsExpression();
    }

    for (auto *decorator : other.decorators_) {
        decorators_.emplace_back(decorator->Clone(allocator, this));
    }
}

SpreadElement *SpreadElement::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<SpreadElement>(Tag {}, *this, allocator); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

ValidationInfo SpreadElement::ValidateExpression()
{
    ValidationInfo info;

    switch (argument_->Type()) {
        case AstNodeType::OBJECT_EXPRESSION: {
            info = argument_->AsObjectExpression()->ValidateExpression();
            break;
        }
        case AstNodeType::ARRAY_EXPRESSION: {
            info = argument_->AsArrayExpression()->ValidateExpression();
            break;
        }
        default: {
            break;
        }
    }

    return info;
}

bool SpreadElement::ConvertibleToRest(bool isDeclaration, bool allowPattern)
{
    bool convResult = true;

    switch (argument_->Type()) {
        case AstNodeType::ARRAY_EXPRESSION: {
            convResult = allowPattern && argument_->AsArrayExpression()->ConvertibleToArrayPattern();
            break;
        }
        case AstNodeType::OBJECT_EXPRESSION: {
            convResult = allowPattern && argument_->AsObjectExpression()->ConvertibleToObjectPattern();
            break;
        }
        case AstNodeType::META_PROPERTY_EXPRESSION:
        case AstNodeType::CHAIN_EXPRESSION:
        case AstNodeType::ASSIGNMENT_EXPRESSION: {
            convResult = false;
            break;
        }
        case AstNodeType::MEMBER_EXPRESSION: {
            convResult = !isDeclaration;
            break;
        }
        default: {
            break;
        }
    }

    SetType(AstNodeType::REST_ELEMENT);
    return convResult;
}

void SpreadElement::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    for (auto *&it : decorators_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsDecorator();
        }
    }

    if (auto *transformedNode = cb(argument_); argument_ != transformedNode) {
        argument_->SetTransformedNode(transformationName, transformedNode);
        argument_ = transformedNode->AsExpression();
    }

    if (auto *const typeAnnotation = TypeAnnotation(); typeAnnotation != nullptr) {
        if (auto *transformedNode = cb(typeAnnotation); typeAnnotation != transformedNode) {
            typeAnnotation->SetTransformedNode(transformationName, transformedNode);
            SetTsTypeAnnotation(static_cast<TypeNode *>(transformedNode));
        }
    }
}

void SpreadElement::Iterate(const NodeTraverser &cb) const
{
    for (auto *it : decorators_) {
        cb(it);
    }

    cb(argument_);

    if (TypeAnnotation() != nullptr) {
        cb(TypeAnnotation());
    }
}

void SpreadElement::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", (type_ == AstNodeType::SPREAD_ELEMENT) ? "SpreadElement" : "RestElement"},
                 {"decorators", AstDumper::Optional(decorators_)},
                 {"argument", argument_},
                 {"typeAnnotation", AstDumper::Optional(TypeAnnotation())}});
}

void SpreadElement::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("...");
    argument_->Dump(dumper);
    auto type = TypeAnnotation();
    if (type != nullptr) {
        dumper->Add(": ");
        type->Dump(dumper);
    }
}

void SpreadElement::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void SpreadElement::Compile([[maybe_unused]] compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *SpreadElement::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *SpreadElement::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
