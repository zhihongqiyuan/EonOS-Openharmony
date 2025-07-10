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

#include "etsLaunchExpression.h"

#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "checker/ETSchecker.h"
#include "checker/TSchecker.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
ETSLaunchExpression::ETSLaunchExpression(CallExpression *expr)
    : Expression(AstNodeType::ETS_LAUNCH_EXPRESSION), expr_(expr)
{
}

void ETSLaunchExpression::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(expr_); expr_ != transformedNode) {
        expr_->SetTransformedNode(transformationName, transformedNode);
        expr_ = transformedNode->AsCallExpression();
    }
}

void ETSLaunchExpression::Iterate(const NodeTraverser &cb) const
{
    cb(expr_);
}

void ETSLaunchExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSLaunchExpression"}, {"expr", expr_}});
}

void ETSLaunchExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("launch ");
    expr_->Dump(dumper);
}

void ETSLaunchExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ETSLaunchExpression::Compile([[maybe_unused]] compiler::ETSGen *etsg) const
{
#ifdef PANDA_WITH_ETS
    etsg->GetAstCompiler()->Compile(this);
#endif  // PANDA_WITH_ETS
}

checker::Type *ETSLaunchExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSLaunchExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

bool ETSLaunchExpression::IsStaticCall() const
{
    return expr_->Signature()->HasSignatureFlag(checker::SignatureFlags::STATIC);
}

ETSLaunchExpression *ETSLaunchExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const expr = expr_ != nullptr ? expr_->Clone(allocator, nullptr) : nullptr;

    if (auto *const clone = allocator->New<ETSLaunchExpression>(expr); clone != nullptr) {
        if (expr != nullptr) {
            expr->SetParent(clone);
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
