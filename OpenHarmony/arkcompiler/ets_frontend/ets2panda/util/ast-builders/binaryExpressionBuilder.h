/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_UTIL_INCLUDE_BINARY_EXPRESSION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_BINARY_EXPRESSION_BUILDER

#include "mem/arena_allocator.h"
#include "astBuilder.h"
#include "ir/expressions/binaryExpression.h"
#include "ir/expressions/literals/numberLiteral.h"

namespace ark::es2panda::ir {

class BinaryExpressionBuilder : public AstBuilder<ir::BinaryExpression> {
public:
    explicit BinaryExpressionBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    BinaryExpressionBuilder &SetOperator(lexer::TokenType op)
    {
        operator_ = op;
        return *this;
    }

    BinaryExpressionBuilder &SetLeft(Expression *left)
    {
        left_ = left;
        return *this;
    }

    BinaryExpressionBuilder &SetRight(Expression *right)
    {
        right_ = right;
        return *this;
    }

    BinaryExpression *Build()
    {
        auto *node = AllocNode(left_, right_, operator_);
        return node;
    }

private:
    Expression *left_ = nullptr;
    Expression *right_ = nullptr;
    lexer::TokenType operator_ = lexer::TokenType::PUNCTUATOR_PLUS;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_BINARY_EXPRESSION_BUILDER