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

#include "etsNewMultiDimArrayInstanceExpression.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ETSNewMultiDimArrayInstanceExpression::TransformChildren(const NodeTransformer &cb,
                                                              std::string_view const transformationName)
{
    if (auto *transformedNode = cb(typeReference_); typeReference_ != transformedNode) {
        typeReference_->SetTransformedNode(transformationName, transformedNode);
        typeReference_ = static_cast<TypeNode *>(transformedNode);
    }

    for (auto *&dim : dimensions_) {
        if (auto *transformedNode = cb(dim); dim != transformedNode) {
            dim->SetTransformedNode(transformationName, transformedNode);
            dim = transformedNode->AsExpression();
        }
    }
}

void ETSNewMultiDimArrayInstanceExpression::Iterate(const NodeTraverser &cb) const
{
    cb(typeReference_);
    for (auto *dim : dimensions_) {
        cb(dim);
    }
}

void ETSNewMultiDimArrayInstanceExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSNewMultiDimArrayInstanceExpression"},
                 {"typeReference", typeReference_},
                 {"dimensions", dimensions_}});
}

void ETSNewMultiDimArrayInstanceExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("new ");
    ASSERT(typeReference_);
    typeReference_->Dump(dumper);
    for (auto dim : dimensions_) {
        dumper->Add("[");
        dim->Dump(dumper);
        dumper->Add("]");
    }
}

void ETSNewMultiDimArrayInstanceExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void ETSNewMultiDimArrayInstanceExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSNewMultiDimArrayInstanceExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSNewMultiDimArrayInstanceExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

ETSNewMultiDimArrayInstanceExpression::ETSNewMultiDimArrayInstanceExpression(
    ETSNewMultiDimArrayInstanceExpression const &other, ArenaAllocator *const allocator)
    : Expression(static_cast<Expression const &>(other)),
      dimensions_(allocator->Adapter()),
      signature_(other.signature_)
{
    typeReference_ = other.typeReference_->Clone(allocator, this);

    for (auto *const dimension : other.dimensions_) {
        dimensions_.emplace_back(dimension->Clone(allocator, this)->AsExpression());
    }
}

ETSNewMultiDimArrayInstanceExpression *ETSNewMultiDimArrayInstanceExpression::Clone(ArenaAllocator *const allocator,
                                                                                    AstNode *const parent)
{
    if (auto *const clone = allocator->New<ETSNewMultiDimArrayInstanceExpression>(*this, allocator); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
