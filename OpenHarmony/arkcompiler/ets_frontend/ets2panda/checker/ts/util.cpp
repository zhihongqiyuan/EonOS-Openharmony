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
#include "ir/expressions/binaryExpression.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/templateLiteral.h"
#include "ir/ts/tsQualifiedName.h"

#include "checker/TSchecker.h"

namespace ark::es2panda::checker {
ir::MemberExpression *TSChecker::ResolveLeftMostMemberExpression(ir::MemberExpression *expr)
{
    ir::MemberExpression *iter = expr;

    while (iter->Object()->IsMemberExpression()) {
        iter = iter->Object()->AsMemberExpression();
    }

    return iter;
}

bool TSChecker::InAssignment(ir::AstNode *node)
{
    ir::AstNode *parent = node;

    while (parent->Parent() != nullptr) {
        if (parent->Parent()->IsAssignmentExpression()) {
            return parent->Parent()->AsAssignmentExpression()->Left() == parent;
        }

        if (parent->Parent()->IsBinaryExpression()) {
            ir::BinaryExpression *binaryExpr = parent->Parent()->AsBinaryExpression();
            return IsAssignmentOperator(binaryExpr->OperatorType()) && binaryExpr->Left() == parent;
        }

        if (parent->Parent()->IsUpdateExpression()) {
            return true;
        }

        parent = parent->Parent();
    }
    return false;
}

bool TSChecker::IsAssignmentOperator(lexer::TokenType op)
{
    switch (op) {
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL:
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL:
        case lexer::TokenType::PUNCTUATOR_MULTIPLY_EQUAL:
        case lexer::TokenType::PUNCTUATOR_EXPONENTIATION_EQUAL:
        case lexer::TokenType::PUNCTUATOR_DIVIDE_EQUAL:
        case lexer::TokenType::PUNCTUATOR_MOD_EQUAL:
        case lexer::TokenType::PUNCTUATOR_MINUS_EQUAL:
        case lexer::TokenType::PUNCTUATOR_LEFT_SHIFT_EQUAL:
        case lexer::TokenType::PUNCTUATOR_RIGHT_SHIFT_EQUAL:
        case lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT_EQUAL:
        case lexer::TokenType::PUNCTUATOR_BITWISE_AND_EQUAL:
        case lexer::TokenType::PUNCTUATOR_BITWISE_OR_EQUAL:
        case lexer::TokenType::PUNCTUATOR_BITWISE_XOR_EQUAL:
        case lexer::TokenType::PUNCTUATOR_PLUS_EQUAL:
        case lexer::TokenType::PUNCTUATOR_SUBSTITUTION:
            return true;
        default:
            return false;
    }
}

bool TSChecker::IsLiteralType(const Type *type)
{
    if (type->IsBooleanType()) {
        return true;
    }

    if (type->IsUnionType()) {
        auto &constituentTypes = type->AsUnionType()->ConstituentTypes();
        bool result = true;
        for (auto *it : constituentTypes) {
            result &= it->HasTypeFlag(TypeFlag::UNIT);
        }
        return result;
    }

    return type->HasTypeFlag(TypeFlag::UNIT);
}

ir::AstNode *TSChecker::FindAncestorUntilGivenType(ir::AstNode *node, ir::AstNodeType stop)
{
    while (node->Parent()->Type() != stop) {
        if (node->Parent() != nullptr) {
            node = node->Parent();
            continue;
        }

        return nullptr;
    }

    return node;
}

bool TSChecker::MaybeTypeOfKind(const Type *type, TypeFlag flags)
{
    if (type->HasTypeFlag(flags)) {
        return true;
    }

    if (type->HasTypeFlag(TypeFlag::UNION_OR_INTERSECTION) && type->IsUnionType()) {
        const auto &constituentTypes = type->AsUnionType()->ConstituentTypes();
        for (auto *it : constituentTypes) {
            if (MaybeTypeOfKind(it, flags)) {
                return true;
            }
        }
    }

    return false;
}

bool TSChecker::MaybeTypeOfKind(const Type *type, ObjectType::ObjectTypeKind kind)
{
    if (type->IsObjectType() && type->AsObjectType()->Kind() == kind) {
        return true;
    }

    if (type->HasTypeFlag(TypeFlag::UNION_OR_INTERSECTION) && type->IsUnionType()) {
        const auto &constituentTypes = type->AsUnionType()->ConstituentTypes();
        for (auto *it : constituentTypes) {
            if (MaybeTypeOfKind(it, kind)) {
                return true;
            }
        }
    }

    return false;
}

bool TSChecker::IsConstantMemberAccess(ir::Expression *expr)
{
    switch (expr->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            return true;
        }
        case ir::AstNodeType::MEMBER_EXPRESSION: {
            bool res = IsConstantMemberAccess(expr->AsMemberExpression()->Object());
            return !expr->AsMemberExpression()->IsComputed()
                       ? res
                       : (res && IsStringLike(expr->AsMemberExpression()->Property()));
        }
        default:
            return false;
    }
}

bool TSChecker::IsStringLike(ir::Expression *expr)
{
    if (expr->IsStringLiteral()) {
        return true;
    }

    if (expr->IsTemplateLiteral() && expr->AsTemplateLiteral()->Quasis().empty()) {
        return true;
    }

    return false;
}
}  // namespace ark::es2panda::checker
