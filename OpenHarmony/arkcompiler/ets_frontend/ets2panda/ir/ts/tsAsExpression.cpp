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

#include "tsAsExpression.h"

#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"

namespace ark::es2panda::ir {
Expression *TSAsExpression::Expr() noexcept
{
    return expression_;
}

void TSAsExpression::SetExpr(Expression *expr) noexcept
{
    expression_ = expr;
    if (expression_ != nullptr) {
        SetStart(expression_->Start());
        expression_->SetParent(this);
    }
}

void TSAsExpression::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(expression_); expression_ != transformedNode) {
        expression_->SetTransformedNode(transformationName, transformedNode);
        expression_ = transformedNode->AsExpression();
    }

    if (auto *typeAnnotation = TypeAnnotation(); typeAnnotation != nullptr) {
        if (auto *transformedNode = cb(typeAnnotation); typeAnnotation != transformedNode) {
            typeAnnotation->SetTransformedNode(transformationName, transformedNode);
            SetTsTypeAnnotation(static_cast<TypeNode *>(transformedNode));
        }
    }
}

void TSAsExpression::Iterate(const NodeTraverser &cb) const
{
    cb(expression_);
    cb(TypeAnnotation());
}

void TSAsExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSAsExpression"}, {"expression", expression_}, {"typeAnnotation", TypeAnnotation()}});
}

void TSAsExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("(");
    expression_->Dump(dumper);
    dumper->Add(" as ");
    TypeAnnotation()->Dump(dumper);
    dumper->Add(")");
}

void TSAsExpression::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TSAsExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSAsExpression::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSAsExpression::Check(checker::ETSChecker *const checker)
{
    return checker->GetAnalyzer()->Check(this);
}

TSAsExpression *TSAsExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const expression = expression_ != nullptr ? expression_->Clone(allocator, nullptr)->AsExpression() : nullptr;
    auto *typeAnnotation = TypeAnnotation();
    if (typeAnnotation != nullptr) {
        typeAnnotation = typeAnnotation->Clone(allocator, nullptr);
    }

    if (auto *const clone = allocator->New<TSAsExpression>(expression, typeAnnotation, isConst_); clone != nullptr) {
        if (expression != nullptr) {
            expression->SetParent(clone);
        }

        if (typeAnnotation != nullptr) {
            typeAnnotation->SetParent(clone);
        }

        clone->SetTsType(TsType());
        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
