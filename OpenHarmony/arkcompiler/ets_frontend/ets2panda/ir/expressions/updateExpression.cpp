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

#include "updateExpression.h"

#include "macros.h"
#include "varbinder/variable.h"
#include "compiler/base/lreference.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/regScope.h"
#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/expressions/unaryExpression.h"

namespace ark::es2panda::ir {
void UpdateExpression::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(argument_); argument_ != transformedNode) {
        argument_->SetTransformedNode(transformationName, transformedNode);
        argument_ = transformedNode->AsExpression();
    }
}

void UpdateExpression::Iterate(const NodeTraverser &cb) const
{
    cb(argument_);
}

void UpdateExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "UpdateExpression"}, {"operator", operator_}, {"prefix", prefix_}, {"argument", argument_}});
}

void UpdateExpression::Dump(ir::SrcDumper *dumper) const
{
    ASSERT(argument_);
    dumper->Add("(");
    if (prefix_) {
        dumper->Add(TokenToString(operator_));
        argument_->Dump(dumper);
    } else {
        argument_->Dump(dumper);
        dumper->Add(TokenToString(operator_));
    }
    dumper->Add(")");
}

void UpdateExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void UpdateExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *UpdateExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *UpdateExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

UpdateExpression *UpdateExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const argument = argument_ != nullptr ? argument_->Clone(allocator, nullptr)->AsExpression() : nullptr;

    if (auto *const clone = allocator->New<UpdateExpression>(argument, operator_, prefix_); clone != nullptr) {
        if (argument != nullptr) {
            argument->SetParent(clone);
        }

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
