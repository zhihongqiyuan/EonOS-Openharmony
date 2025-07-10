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

#include "condition.h"

#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/binaryExpression.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/unaryExpression.h"

namespace ark::es2panda::compiler {
bool Condition::CompileBinaryExpr(PandaGen *pg, const ir::BinaryExpression *binExpr, Label *falseLabel)
{
    switch (binExpr->OperatorType()) {
        case lexer::TokenType::PUNCTUATOR_EQUAL:
        case lexer::TokenType::PUNCTUATOR_NOT_EQUAL:
        case lexer::TokenType::PUNCTUATOR_STRICT_EQUAL:
        case lexer::TokenType::PUNCTUATOR_NOT_STRICT_EQUAL:
        case lexer::TokenType::PUNCTUATOR_LESS_THAN:
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL:
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN:
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL: {
            // This is a special case
            // These operators are expressed via cmp instructions and the following
            // if-else branches. Condition also expressed via cmp instruction and
            // the following if-else.
            // the goal of this method is to merge these two sequences of instructions.
            RegScope rs(pg);
            VReg lhs = pg->AllocReg();

            binExpr->Left()->Compile(pg);
            pg->StoreAccumulator(binExpr, lhs);
            binExpr->Right()->Compile(pg);
            pg->Condition(binExpr, binExpr->OperatorType(), lhs, falseLabel);
            return true;
        }
        case lexer::TokenType::PUNCTUATOR_LOGICAL_AND: {
            binExpr->Left()->Compile(pg);
            pg->ToBoolean(binExpr);
            pg->BranchIfFalse(binExpr, falseLabel);

            binExpr->Right()->Compile(pg);
            pg->ToBoolean(binExpr);
            pg->BranchIfFalse(binExpr, falseLabel);
            return true;
        }
        case lexer::TokenType::PUNCTUATOR_LOGICAL_OR: {
            auto *endLabel = pg->AllocLabel();

            binExpr->Left()->Compile(pg);
            pg->ToBoolean(binExpr);
            pg->BranchIfTrue(binExpr, endLabel);

            binExpr->Right()->Compile(pg);
            pg->ToBoolean(binExpr);
            pg->BranchIfFalse(binExpr, falseLabel);
            pg->SetLabel(binExpr, endLabel);
            return true;
        }
        default: {
            break;
        }
    }
    return false;
}

void Condition::Compile(PandaGen *pg, const ir::Expression *expr, Label *falseLabel)
{
    if (expr->IsBinaryExpression()) {
        if (CompileBinaryExpr(pg, expr->AsBinaryExpression(), falseLabel)) {
            return;
        }
    } else if (expr->IsUnaryExpression() &&
               expr->AsUnaryExpression()->OperatorType() == lexer::TokenType::PUNCTUATOR_EXCLAMATION_MARK) {
        expr->AsUnaryExpression()->Argument()->Compile(pg);

        pg->Negate(expr);
        pg->BranchIfFalse(expr, falseLabel);
        return;
    }

    // General case including some binExpr i.E.(a+b)
    expr->Compile(pg);
    pg->ToBoolean(expr);
    pg->BranchIfFalse(expr, falseLabel);
}

Condition::Result Condition::CheckConstantExpr(ETSGen *etsg, const ir::Expression *expr)
{
    const auto resultingExpression = [](const ir::Expression *e) {
        if (e->IsBinaryExpression() && e->AsBinaryExpression()->IsLogicalExtended()) {
            return e->AsBinaryExpression()->Result();
        }
        if (e->IsAssignmentExpression() && e->AsAssignmentExpression()->IsLogicalExtended()) {
            return e->AsAssignmentExpression()->Result();
        }
        return e;
    }(expr);
    if (resultingExpression == nullptr) {
        return Result::UNKNOWN;
    }

    if (etsg->Checker()->IsNullLikeOrVoidExpression(resultingExpression)) {
        return Result::CONST_FALSE;
    }

    auto exprRes = resultingExpression->TsType()->ResolveConditionExpr();
    if (std::get<0>(exprRes)) {
        return std::get<1>(exprRes) ? Result::CONST_TRUE : Result::CONST_FALSE;
    }

    return Result::UNKNOWN;
}

void Condition::CompileLogicalOrExpr(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel)
{
    auto ttctx = TargetTypeContext(etsg, binExpr->OperationType());
    RegScope rs(etsg);
    VReg lhs = etsg->AllocReg();
    VReg rhs = etsg->AllocReg();
    auto *returnLeftLabel = etsg->AllocLabel();
    auto *returnRightTrueLabel = etsg->AllocLabel();
    auto *returnRightFalseLabel = etsg->AllocLabel();

    binExpr->Left()->Compile(etsg);
    etsg->ApplyConversionAndStoreAccumulator(binExpr->Left(), lhs, binExpr->OperationType());
    etsg->ResolveConditionalResultIfTrue(binExpr->Left(), returnLeftLabel);
    etsg->BranchIfTrue(binExpr, returnLeftLabel);

    binExpr->Right()->Compile(etsg);
    etsg->ApplyConversionAndStoreAccumulator(binExpr->Right(), rhs, binExpr->OperationType());
    etsg->ResolveConditionalResultIfFalse(binExpr->Right(), returnRightFalseLabel);
    etsg->BranchIfFalse(binExpr, returnRightFalseLabel);
    etsg->LoadAccumulator(binExpr, rhs);
    etsg->Branch(binExpr, returnRightTrueLabel);

    etsg->SetLabel(binExpr, returnRightFalseLabel);
    etsg->LoadAccumulator(binExpr, rhs);
    etsg->Branch(binExpr, falseLabel);
    etsg->SetLabel(binExpr, returnLeftLabel);
    etsg->LoadAccumulator(binExpr, lhs);
    etsg->SetLabel(binExpr, returnRightTrueLabel);
}

void Condition::CompileLogicalAndExpr(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel)
{
    auto ttctx = TargetTypeContext(etsg, binExpr->OperationType());
    RegScope rs(etsg);
    VReg lhs = etsg->AllocReg();
    VReg rhs = etsg->AllocReg();
    auto *returnLeftLabel = etsg->AllocLabel();
    auto *returnRightTrueLabel = etsg->AllocLabel();
    auto *returnRightFalseLabel = etsg->AllocLabel();

    binExpr->Left()->Compile(etsg);
    etsg->ApplyConversionAndStoreAccumulator(binExpr->Left(), lhs, binExpr->OperationType());
    etsg->ResolveConditionalResultIfFalse(binExpr->Left(), returnLeftLabel);
    etsg->BranchIfFalse(binExpr, returnLeftLabel);

    binExpr->Right()->Compile(etsg);
    etsg->ApplyConversionAndStoreAccumulator(binExpr->Right(), rhs, binExpr->OperationType());
    etsg->ResolveConditionalResultIfFalse(binExpr->Right(), returnRightFalseLabel);
    etsg->BranchIfFalse(binExpr, returnRightFalseLabel);
    etsg->LoadAccumulator(binExpr, rhs);
    etsg->Branch(binExpr, returnRightTrueLabel);

    etsg->SetLabel(binExpr, returnLeftLabel);
    etsg->LoadAccumulator(binExpr, lhs);
    etsg->Branch(binExpr, falseLabel);
    etsg->SetLabel(binExpr, returnRightFalseLabel);
    etsg->LoadAccumulator(binExpr, rhs);
    etsg->Branch(binExpr, falseLabel);
    etsg->SetLabel(binExpr, returnRightTrueLabel);
}

bool Condition::CompileBinaryExprForBigInt(ETSGen *etsg, const ir::BinaryExpression *expr, Label *falseLabel)
{
    if ((expr->Left()->TsType() == nullptr) || (expr->Right()->TsType() == nullptr)) {
        return false;
    }

    if (!expr->Left()->TsType()->IsETSBigIntType()) {
        return false;
    }

    if (!expr->Right()->TsType()->IsETSBigIntType()) {
        return false;
    }

    std::string_view signature = compiler::Signatures::ANY;
    switch (expr->OperatorType()) {
        case lexer::TokenType::PUNCTUATOR_LESS_THAN:
            signature = compiler::Signatures::BUILTIN_BIGINT_OPERATOR_LESS_THAN;
            break;
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL:
            signature = compiler::Signatures::BUILTIN_BIGINT_OPERATOR_LESS_THAN_EQUAL;
            break;
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN:
            signature = compiler::Signatures::BUILTIN_BIGINT_OPERATOR_GREATER_THAN;
            break;
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL:
            signature = compiler::Signatures::BUILTIN_BIGINT_OPERATOR_GREATER_THAN_EQUAL;
            break;
        default:
            // Other operations are handled in the CompileBinaryExpr function
            return false;
    }

    auto ttctx = TargetTypeContext(etsg, expr->OperationType());
    RegScope rs(etsg);
    VReg lhs = etsg->AllocReg();
    expr->Left()->Compile(etsg);
    etsg->ApplyConversionAndStoreAccumulator(expr->Left(), lhs, expr->OperationType());
    expr->Right()->Compile(etsg);
    etsg->ApplyConversion(expr->Right(), expr->OperationType());
    compiler::VReg rhs = etsg->AllocReg();
    etsg->StoreAccumulator(expr, rhs);
    etsg->CallBigIntBinaryComparison(expr, lhs, rhs, signature);
    etsg->BranchIfFalse(expr, falseLabel);

    return true;
}

void Condition::CompileInstanceofExpr(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel)
{
    ASSERT(binExpr->OperatorType() == lexer::TokenType::KEYW_INSTANCEOF);
    binExpr->Compile(etsg);
    etsg->BranchIfFalse(binExpr, falseLabel);
}

bool Condition::CompileBinaryExpr(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel)
{
    if (CompileBinaryExprForBigInt(etsg, binExpr, falseLabel)) {
        return true;
    }

    switch (binExpr->OperatorType()) {
        case lexer::TokenType::PUNCTUATOR_EQUAL:
        case lexer::TokenType::PUNCTUATOR_NOT_EQUAL:
        case lexer::TokenType::PUNCTUATOR_LESS_THAN:
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL:
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN:
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL: {
            auto ttctx = TargetTypeContext(etsg, binExpr->OperationType());

            RegScope rs(etsg);
            VReg lhs = etsg->AllocReg();

            binExpr->CompileOperands(etsg, lhs);
            etsg->Condition(binExpr, binExpr->OperatorType(), lhs, falseLabel);
            return true;
        }
        case lexer::TokenType::PUNCTUATOR_LOGICAL_AND: {
            CompileLogicalAndExpr(etsg, binExpr, falseLabel);
            return true;
        }
        case lexer::TokenType::PUNCTUATOR_LOGICAL_OR: {
            CompileLogicalOrExpr(etsg, binExpr, falseLabel);
            return true;
        }
        case lexer::TokenType::KEYW_INSTANCEOF: {
            CompileInstanceofExpr(etsg, binExpr, falseLabel);
            return true;
        }
        default: {
            break;
        }
    }
    return false;
}

void Condition::Compile(ETSGen *etsg, const ir::Expression *expr, Label *falseLabel)
{
    if (expr->IsBinaryExpression()) {
        if (CompileBinaryExpr(etsg, expr->AsBinaryExpression(), falseLabel)) {
            return;
        }
    } else if (expr->IsUnaryExpression() &&
               expr->AsUnaryExpression()->OperatorType() == lexer::TokenType::PUNCTUATOR_EXCLAMATION_MARK) {
        expr->AsUnaryExpression()->Argument()->Compile(etsg);
        etsg->ApplyConversion(expr->AsUnaryExpression()->Argument(), etsg->Checker()->GlobalETSBooleanType());
        etsg->ResolveConditionalResultIfTrue(expr, falseLabel);
        etsg->BranchIfTrue(expr, falseLabel);
        return;
    }
    ASSERT(expr->TsType()->IsConditionalExprType());
    expr->Compile(etsg);
    etsg->ApplyConversion(expr, etsg->Checker()->GlobalETSBooleanType());
    etsg->ResolveConditionalResultIfFalse(expr, falseLabel);
    etsg->BranchIfFalse(expr, falseLabel);
}
}  // namespace ark::es2panda::compiler
