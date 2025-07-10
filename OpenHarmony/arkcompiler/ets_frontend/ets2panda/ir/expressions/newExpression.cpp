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

#include "newExpression.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "util/helpers.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
NewExpression::NewExpression([[maybe_unused]] Tag const tag, NewExpression const &other,
                             ArenaAllocator *const allocator)
    : Expression(static_cast<Expression const &>(other)), arguments_(allocator->Adapter())
{
    if (other.callee_ != nullptr) {
        callee_ = other.callee_->Clone(allocator, this)->AsExpression();
    }

    for (auto *argument : other.arguments_) {
        arguments_.emplace_back(argument->Clone(allocator, this)->AsExpression());
    }
}

NewExpression *NewExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<NewExpression>(Tag {}, *this, allocator); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

void NewExpression::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(callee_); callee_ != transformedNode) {
        callee_->SetTransformedNode(transformationName, transformedNode);
        callee_ = transformedNode->AsExpression();
    }

    for (auto *&it : arguments_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsExpression();
        }
    }
}

void NewExpression::Iterate(const NodeTraverser &cb) const
{
    cb(callee_);

    for (auto *it : arguments_) {
        cb(it);
    }
}

void NewExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "NewExpression"}, {"callee", callee_}, {"arguments", arguments_}});
}

void NewExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("NewExpression");
}

void NewExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void NewExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *NewExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *NewExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
