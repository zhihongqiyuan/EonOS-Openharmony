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

#ifndef ES2PANDA_UTIL_INCLUDE_UPDATE_EXPRESSION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_UPDATE_EXPRESSION_BUILDER

#include "mem/arena_allocator.h"
#include "astBuilder.h"
#include "ir/expressions/updateExpression.h"

namespace ark::es2panda::ir {

class UpdateExpressionBuilder : public AstBuilder<ir::UpdateExpression> {
public:
    explicit UpdateExpressionBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    UpdateExpressionBuilder &SetOperator(lexer::TokenType op)
    {
        operator_ = op;
        return *this;
    }

    UpdateExpressionBuilder &SetArgument(Expression *arg)
    {
        argument_ = arg;
        return *this;
    }

    UpdateExpressionBuilder &SetIsPrefix(bool isPrefix)
    {
        prefix_ = isPrefix;
        return *this;
    }

    UpdateExpression *Build()
    {
        auto *node = AllocNode(argument_, operator_, prefix_);
        return node;
    }

private:
    Expression *argument_ {};
    lexer::TokenType operator_ = lexer::TokenType::PUNCTUATOR_PLUS_PLUS;
    bool prefix_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_UPDATE_EXPRESSION_BUILDER