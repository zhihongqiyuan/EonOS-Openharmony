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

#include "binaryExpression.h"

#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "checker/TSchecker.h"
#include "ir/astNode.h"
#include "ir/expression.h"
#include "ir/srcDump.h"
#include "ir/visitor/AstVisitor.h"

namespace ark::es2panda::ir {
void BinaryExpression::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(left_); left_ != transformedNode) {
        left_->SetTransformedNode(transformationName, transformedNode);
        left_ = transformedNode->AsExpression();
    }

    if (auto *transformedNode = cb(right_); right_ != transformedNode) {
        right_->SetTransformedNode(transformationName, transformedNode);
        right_ = transformedNode->AsExpression();
    }
}

void BinaryExpression::Iterate(const NodeTraverser &cb) const
{
    cb(left_);
    cb(right_);
}

void BinaryExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", IsLogical() ? "LogicalExpression" : "BinaryExpression"},
                 {"operator", operator_},
                 {"left", left_},
                 {"right", right_}});
}

void BinaryExpression::Dump(ir::SrcDumper *dumper) const
{
    ASSERT(left_ != nullptr);
    ASSERT(right_ != nullptr);
    dumper->Add("((");
    left_->Dump(dumper);
    dumper->Add(") ");
    dumper->Add(TokenToString(operator_));
    dumper->Add(" (");
    right_->Dump(dumper);
    dumper->Add("))");
}

void BinaryExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void BinaryExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

void BinaryExpression::CompileOperands(compiler::ETSGen *etsg, compiler::VReg lhs) const
{
    left_->Compile(etsg);

    if (operator_ == lexer::TokenType::KEYW_INSTANCEOF) {
        etsg->StoreAccumulator(left_, lhs);
    } else {
        etsg->ApplyConversionAndStoreAccumulator(left_, lhs, operationType_);
    }

    right_->Compile(etsg);
    etsg->ApplyConversion(right_, operationType_);
}

checker::Type *BinaryExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *BinaryExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

BinaryExpression *BinaryExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const left = left_ != nullptr ? left_->Clone(allocator, nullptr)->AsExpression() : nullptr;
    auto *const right = right_ != nullptr ? right_->Clone(allocator, nullptr)->AsExpression() : nullptr;

    if (auto *const clone = allocator->New<BinaryExpression>(left, right, operator_); clone != nullptr) {
        if (operationType_ != nullptr) {
            clone->SetOperationType(operationType_);
        }

        if (right != nullptr) {
            right->SetParent(clone);
        }

        if (left != nullptr) {
            left->SetParent(clone);
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
