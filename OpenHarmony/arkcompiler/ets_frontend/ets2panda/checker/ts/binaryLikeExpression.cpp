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

#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/memberExpression.h"

#include "checker/TSchecker.h"

namespace ark::es2panda::checker {

void TSChecker::CheckBooleanLikeType(Type *leftType, Type *rightType, ir::AstNode *expr, lexer::TokenType op)
{
    if (leftType->HasTypeFlag(TypeFlag::BOOLEAN_LIKE) && rightType->HasTypeFlag(TypeFlag::BOOLEAN_LIKE)) {
        lexer::TokenType suggestedOp;
        switch (op) {
            case lexer::TokenType::PUNCTUATOR_BITWISE_OR:
            case lexer::TokenType::PUNCTUATOR_BITWISE_OR_EQUAL: {
                suggestedOp = lexer::TokenType::PUNCTUATOR_LOGICAL_OR;
                break;
            }
            case lexer::TokenType::PUNCTUATOR_BITWISE_AND:
            case lexer::TokenType::PUNCTUATOR_BITWISE_AND_EQUAL: {
                suggestedOp = lexer::TokenType::PUNCTUATOR_LOGICAL_AND;
                break;
            }
            case lexer::TokenType::PUNCTUATOR_BITWISE_XOR:
            case lexer::TokenType::PUNCTUATOR_BITWISE_XOR_EQUAL: {
                suggestedOp = lexer::TokenType::PUNCTUATOR_NOT_STRICT_EQUAL;
                break;
            }
            default: {
                suggestedOp = lexer::TokenType::EOS;
                break;
            }
        }

        if (suggestedOp != lexer::TokenType::EOS) {
            ThrowTypeError(
                {"The ", op, " operator is not allowed for boolean types. Consider using ", suggestedOp, " instead"},
                expr->Start());
        }
    }
}
Type *TSChecker::CheckBinaryOperator(ExpressionTypeInfo *leftRightType, ir::Expression *leftExpr,
                                     ir::Expression *rightExpr, ir::AstNode *expr, lexer::TokenType op)
{
    CheckNonNullType(leftRightType->leftType, leftExpr->Start());
    CheckNonNullType(leftRightType->rightType, rightExpr->Start());

    CheckBooleanLikeType(leftRightType->leftType, leftRightType->rightType, expr, op);

    if (!leftRightType->leftType->HasTypeFlag(TypeFlag::VALID_ARITHMETIC_TYPE)) {
        ThrowTypeError(
            "The left-hand side of an arithmetic operation must be of type 'any', 'number', 'bigint' or an "
            "enum "
            "type.",
            expr->Start());
    }

    if (!leftRightType->rightType->HasTypeFlag(TypeFlag::VALID_ARITHMETIC_TYPE)) {
        ThrowTypeError(
            "The right-hand side of an arithmetic operation must be of type 'any', 'number', 'bigint' or an "
            "enum "
            "type.",
            rightExpr->Start());
    }

    Type *resultType = nullptr;
    if ((leftRightType->leftType->IsAnyType() && leftRightType->rightType->IsAnyType()) ||
        !(leftRightType->leftType->HasTypeFlag(TypeFlag::BIGINT_LIKE) ||
          leftRightType->rightType->HasTypeFlag(TypeFlag::BIGINT_LIKE))) {
        resultType = GlobalNumberType();
    } else if (leftRightType->leftType->HasTypeFlag(TypeFlag::BIGINT_LIKE) &&
               leftRightType->rightType->HasTypeFlag(TypeFlag::BIGINT_LIKE)) {
        if (op == lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT ||
            op == lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT_EQUAL) {
            ThrowTypeError({"operator ", op, " cannot be applied to types 'bigint' and 'bigint'"}, expr->Start());
        }
        resultType = GlobalBigintType();
    } else {
        ThrowBinaryLikeError(op, leftRightType->leftType, leftRightType->rightType, expr->Start());
    }

    CheckAssignmentOperator(op, leftExpr, leftRightType->leftType, resultType);
    return resultType;
}

Type *TSChecker::CheckPlusOperator(ExpressionTypeInfo *leftRightType, ir::Expression *leftExpr,
                                   ir::Expression *rightExpr, ir::AstNode *expr, lexer::TokenType op)
{
    if (!leftRightType->leftType->HasTypeFlag(TypeFlag::STRING_LIKE) &&
        !leftRightType->rightType->HasTypeFlag(TypeFlag::STRING_LIKE)) {
        CheckNonNullType(leftRightType->leftType, leftExpr->Start());
        CheckNonNullType(leftRightType->rightType, rightExpr->Start());
    }

    Type *resultType = nullptr;
    if (IsTypeAssignableTo(leftRightType->leftType, GlobalNumberType()) &&
        IsTypeAssignableTo(leftRightType->rightType, GlobalNumberType())) {
        resultType = GlobalNumberType();
    } else if (IsTypeAssignableTo(leftRightType->leftType, GlobalBigintType()) &&
               IsTypeAssignableTo(leftRightType->rightType, GlobalBigintType())) {
        resultType = GlobalBigintType();
    } else if (IsTypeAssignableTo(leftRightType->leftType, GlobalStringType()) ||
               IsTypeAssignableTo(leftRightType->rightType, GlobalStringType())) {
        resultType = GlobalStringType();
    } else if (MaybeTypeOfKind(leftRightType->leftType, TypeFlag::UNKNOWN)) {
        ThrowTypeError("object is of type 'unknown'", leftExpr->Start());
    } else if (MaybeTypeOfKind(leftRightType->rightType, TypeFlag::UNKNOWN)) {
        ThrowTypeError("object is of type 'unknown'", rightExpr->Start());
    } else if (leftRightType->leftType->IsAnyType() || leftRightType->rightType->IsAnyType()) {
        resultType = GlobalAnyType();
    } else {
        ThrowBinaryLikeError(op, leftRightType->leftType, leftRightType->rightType, expr->Start());
    }

    if (op == lexer::TokenType::PUNCTUATOR_PLUS_EQUAL) {
        CheckAssignmentOperator(op, leftExpr, leftRightType->leftType, resultType);
    }

    return resultType;
}

Type *TSChecker::CheckCompareOperator(ExpressionTypeInfo *leftRightType, ir::Expression *leftExpr,
                                      ir::Expression *rightExpr, ir::AstNode *expr, lexer::TokenType op)
{
    CheckNonNullType(leftRightType->leftType, leftExpr->Start());
    CheckNonNullType(leftRightType->rightType, rightExpr->Start());

    if (AreTypesComparable(leftRightType->leftType, leftRightType->rightType) ||
        (IsTypeAssignableTo(leftRightType->leftType, GlobalNumberOrBigintType()) &&
         IsTypeAssignableTo(leftRightType->rightType, GlobalNumberOrBigintType()))) {
        return GlobalBooleanType();
    }

    ThrowBinaryLikeError(op, leftRightType->leftType, leftRightType->rightType, expr->Start());

    return GlobalAnyType();
}

Type *TSChecker::CheckAndOperator(Type *leftType, Type *rightType, ir::Expression *leftExpr)
{
    CheckTruthinessOfType(leftType, leftExpr->Start());

    if ((static_cast<uint64_t>(leftType->GetTypeFacts()) & static_cast<uint64_t>(TypeFacts::TRUTHY)) != 0U) {
        Type *resultType = CreateUnionType({ExtractDefinitelyFalsyTypes(rightType), rightType});
        return resultType;
    }

    return leftType;
}

Type *TSChecker::CheckOrOperator(Type *leftType, Type *rightType, ir::Expression *leftExpr)
{
    CheckTruthinessOfType(leftType, leftExpr->Start());

    if ((static_cast<uint64_t>(leftType->GetTypeFacts()) & static_cast<uint64_t>(TypeFacts::FALSY)) != 0U) {
        // NOTE: aszilagyi. subtype reduction in the result union
        Type *resultType = CreateUnionType({RemoveDefinitelyFalsyTypes(leftType), rightType});
        return resultType;
    }

    return leftType;
}

static bool TypeHasCallOrConstructSignatures(Type *type)
{
    return type->IsObjectType() &&
           (!type->AsObjectType()->CallSignatures().empty() || !type->AsObjectType()->ConstructSignatures().empty());
}

Type *TSChecker::CheckInstanceofExpression(Type *leftType, Type *rightType, ir::Expression *rightExpr,
                                           ir::AstNode *expr)
{
    if (leftType->TypeFlags() != TypeFlag::ANY && IsAllTypesAssignableTo(leftType, GlobalPrimitiveType())) {
        ThrowTypeError({"The left-hand side of an 'instanceof' expression must be of type 'any',",
                        " an object type or a type parameter."},
                       expr->Start());
    }

    // NOTE: aszilagyi. Check if right type is subtype of globalFunctionType
    if (rightType->TypeFlags() != TypeFlag::ANY && !TypeHasCallOrConstructSignatures(rightType)) {
        ThrowTypeError({"The right-hand side of an 'instanceof' expression must be of type 'any'",
                        " or of a type assignable to the 'Function' interface type."},
                       rightExpr->Start());
    }

    return GlobalBooleanType();
}

Type *TSChecker::CheckInExpression(Type *leftType, Type *rightType, ir::Expression *leftExpr, ir::Expression *rightExpr,
                                   ir::AstNode *expr)
{
    CheckNonNullType(leftType, leftExpr->Start());
    CheckNonNullType(rightType, rightExpr->Start());

    // NOTE: aszilagyi. Check IsAllTypesAssignableTo with ESSymbol too
    if (leftType->TypeFlags() != TypeFlag::ANY && !IsAllTypesAssignableTo(leftType, GlobalStringOrNumberType())) {
        ThrowTypeError(
            {"The left-hand side of an 'in' expression must be of type 'any',", " 'string', 'number', or 'symbol'."},
            expr->Start());
    }

    // NOTE: aszilagyi. Handle type parameters
    if (!IsAllTypesAssignableTo(rightType, GlobalNonPrimitiveType())) {
        ThrowTypeError("The right-hand side of an 'in' expression must not be a primitive.", rightExpr->Start());
    }

    return GlobalBooleanType();
}

void TSChecker::CheckAssignmentOperator(lexer::TokenType op, ir::Expression *leftExpr, Type *leftType, Type *valueType)
{
    if (IsAssignmentOperator(op)) {
        CheckReferenceExpression(
            leftExpr, "the left hand side of an assignment expression must be a variable or a property access",
            "The left-hand side of an assignment expression may not be an optional property access.");

        if (!IsTypeAssignableTo(valueType, leftType)) {
            ThrowAssignmentError(valueType, leftType, leftExpr->Start(),
                                 leftExpr->Parent()->AsAssignmentExpression()->Right()->IsMemberExpression() ||
                                     leftExpr->Parent()->AsAssignmentExpression()->Right()->IsChainExpression());
        }
    }
}
}  // namespace ark::es2panda::checker
