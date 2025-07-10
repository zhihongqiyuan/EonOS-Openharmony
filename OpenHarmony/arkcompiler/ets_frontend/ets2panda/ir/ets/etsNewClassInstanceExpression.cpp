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

#include "etsNewClassInstanceExpression.h"

#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "checker/TSchecker.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ETSNewClassInstanceExpression::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(typeReference_); typeReference_ != transformedNode) {
        typeReference_->SetTransformedNode(transformationName, transformedNode);
        typeReference_ = transformedNode->AsExpression();
    }

    for (auto *&arg : arguments_) {
        if (auto *transformedNode = cb(arg); arg != transformedNode) {
            arg->SetTransformedNode(transformationName, transformedNode);
            arg = transformedNode->AsExpression();
        }
    }

    if (classDef_ != nullptr) {
        if (auto *transformedNode = cb(classDef_); classDef_ != transformedNode) {
            classDef_->SetTransformedNode(transformationName, transformedNode);
            classDef_ = transformedNode->AsClassDefinition();
        }
    }
}

void ETSNewClassInstanceExpression::Iterate([[maybe_unused]] const NodeTraverser &cb) const
{
    cb(typeReference_);

    for (auto *arg : arguments_) {
        cb(arg);
    }

    if (classDef_ != nullptr) {
        cb(classDef_);
    }
}

void ETSNewClassInstanceExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSNewClassInstanceExpression"},
                 {"typeReference", typeReference_},
                 {"arguments", arguments_},
                 {"classBody", AstDumper::Optional(classDef_)}});
}

void ETSNewClassInstanceExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("new ");
    if (typeReference_ != nullptr) {
        typeReference_->Dump(dumper);
    }
    dumper->Add("(");
    for (auto argument : arguments_) {
        argument->Dump(dumper);
        if (argument != arguments_.back()) {
            dumper->Add(", ");
        }
    }
    dumper->Add(")");
}

void ETSNewClassInstanceExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ETSNewClassInstanceExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSNewClassInstanceExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSNewClassInstanceExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

ETSNewClassInstanceExpression::ETSNewClassInstanceExpression(ETSNewClassInstanceExpression const &other,
                                                             ArenaAllocator *const allocator)
    : Expression(static_cast<Expression const &>(other)), arguments_(allocator->Adapter()), signature_(other.signature_)
{
    typeReference_ =
        other.typeReference_ != nullptr ? other.typeReference_->Clone(allocator, this)->AsExpression() : nullptr;
    classDef_ = other.classDef_ != nullptr ? other.classDef_->Clone(allocator, this)->AsClassDefinition() : nullptr;

    for (auto *const argument : other.arguments_) {
        arguments_.emplace_back(argument->Clone(allocator, this)->AsExpression());
    }
}

ETSNewClassInstanceExpression *ETSNewClassInstanceExpression::Clone(ArenaAllocator *const allocator,
                                                                    AstNode *const parent)
{
    if (auto *const clone = allocator->New<ETSNewClassInstanceExpression>(*this, allocator); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
