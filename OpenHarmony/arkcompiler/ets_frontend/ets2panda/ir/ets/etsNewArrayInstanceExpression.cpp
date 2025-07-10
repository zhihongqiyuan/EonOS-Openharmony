/*
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

#include "etsNewArrayInstanceExpression.h"

#include "checker/ETSchecker.h"
#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/typeNode.h"

namespace ark::es2panda::ir {
void ETSNewArrayInstanceExpression::TransformChildren(const NodeTransformer &cb,
                                                      std::string_view const transformationName)
{
    if (auto *transformedNode = cb(typeReference_); typeReference_ != transformedNode) {
        typeReference_->SetTransformedNode(transformationName, transformedNode);
        typeReference_ = static_cast<TypeNode *>(transformedNode);
    }

    if (auto *transformedNode = cb(dimension_); dimension_ != transformedNode) {
        dimension_->SetTransformedNode(transformationName, transformedNode);
        dimension_ = transformedNode->AsExpression();
    }
}

void ETSNewArrayInstanceExpression::Iterate(const NodeTraverser &cb) const
{
    cb(typeReference_);
    cb(dimension_);
}

void ETSNewArrayInstanceExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add(
        {{"type", "ETSNewArrayInstanceExpression"}, {"typeReference", typeReference_}, {"dimension", dimension_}});
}

void ETSNewArrayInstanceExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("new ");
    ASSERT(typeReference_);
    typeReference_->Dump(dumper);
    ASSERT(dimension_);
    dumper->Add("[");
    dimension_->Dump(dumper);
    dumper->Add("]");
}

void ETSNewArrayInstanceExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void ETSNewArrayInstanceExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSNewArrayInstanceExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSNewArrayInstanceExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

ETSNewArrayInstanceExpression *ETSNewArrayInstanceExpression::Clone(ArenaAllocator *const allocator,
                                                                    AstNode *const parent)
{
    auto *const typeRef = typeReference_ != nullptr ? typeReference_->Clone(allocator, nullptr) : nullptr;
    auto *const dimension = dimension_ != nullptr ? dimension_->Clone(allocator, nullptr)->AsExpression() : nullptr;

    if (auto *const clone = allocator->New<ETSNewArrayInstanceExpression>(typeRef, dimension); clone != nullptr) {
        if (typeRef != nullptr) {
            typeRef->SetParent(clone);
        }

        if (dimension != nullptr) {
            dimension->SetParent(clone);
        }

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->defaultConstructorSignature_ = defaultConstructorSignature_;
        clone->SetRange(Range());

        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
