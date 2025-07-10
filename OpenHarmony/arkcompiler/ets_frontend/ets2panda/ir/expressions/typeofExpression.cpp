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

#include "typeofExpression.h"

#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void TypeofExpression::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(argument_); argument_ != transformedNode) {
        argument_->SetTransformedNode(transformationName, transformedNode);
        argument_ = transformedNode->AsExpression();
    }
}

void TypeofExpression::Iterate([[maybe_unused]] const NodeTraverser &cb) const
{
    cb(argument_);
}

void TypeofExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TypeofExpression"}, {"argument", argument_}});
}

void TypeofExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("(typeof ");
    argument_->Dump(dumper);
    dumper->Add(")");
}

void TypeofExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TypeofExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TypeofExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TypeofExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

// NOLINTNEXTLINE(default-arguments)
TypeofExpression *TypeofExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const argument = argument_->Clone(allocator, nullptr)->AsExpression();

    if (auto *const clone = allocator->New<TypeofExpression>(argument); clone != nullptr) {
        argument->SetParent(clone);

        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
