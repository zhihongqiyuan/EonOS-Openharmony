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

#include "expressionStatement.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ExpressionStatement::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(expression_); expression_ != transformedNode) {
        expression_->SetTransformedNode(transformationName, transformedNode);
        expression_ = transformedNode->AsExpression();
    }
}

void ExpressionStatement::Iterate(const NodeTraverser &cb) const
{
    cb(expression_);
}

void ExpressionStatement::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ExpressionStatement"}, {"expression", expression_}});
}

void ExpressionStatement::Dump(ir::SrcDumper *dumper) const
{
    ASSERT(expression_ != nullptr);
    expression_->Dump(dumper);
    if ((parent_ != nullptr) && (parent_->IsBlockStatement() || parent_->IsSwitchCaseStatement())) {
        dumper->Add(";");
    }
}

ExpressionStatement *ExpressionStatement::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const expression = expression_->Clone(allocator, nullptr)->AsExpression();

    if (auto *const clone = allocator->New<ExpressionStatement>(expression); clone != nullptr) {
        expression->SetParent(clone);
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

void ExpressionStatement::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ExpressionStatement::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ExpressionStatement::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ExpressionStatement::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
