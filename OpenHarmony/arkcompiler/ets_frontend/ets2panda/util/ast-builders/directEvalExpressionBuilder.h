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

#ifndef ES2PANDA_UTIL_INCLUDE_DIRECT_EVAL_EXPRESSION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_DIRECT_EVAL_EXPRESSION_BUILDER

#include "ir/expressions/directEvalExpression.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class DirectEvalExpressionBuilder : public AstBuilder<ir::DirectEvalExpression> {
public:
    explicit DirectEvalExpressionBuilder(ark::ArenaAllocator *allocator)
        : AstBuilder(allocator), arguments_(Allocator()->Adapter())
    {
    }

    DirectEvalExpressionBuilder &SetCallee(Expression *const callee)
    {
        callee_ = callee;
        return *this;
    }

    DirectEvalExpressionBuilder &SetArguments(ArenaVector<Expression *> &&arguments)
    {
        arguments_ = std::move(arguments);
        return *this;
    }

    DirectEvalExpressionBuilder &SetArguments(Expression *arguments)
    {
        arguments_.emplace_back(arguments);
        return *this;
    }

    DirectEvalExpressionBuilder &SetParserStatus(uint32_t parserStatus)
    {
        parserStatus_ = parserStatus;
        return *this;
    }

    DirectEvalExpression *Build()
    {
        auto *node = AllocNode(callee_, std::move(arguments_), nullptr, false, parserStatus_);
        return node;
    }

private:
    Expression *callee_ = nullptr;
    ArenaVector<Expression *> arguments_;
    uint32_t parserStatus_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_DIRECT_EVAL_EXPRESSION_BUILDER