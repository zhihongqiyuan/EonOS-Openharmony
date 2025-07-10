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

#ifndef ES2PANDA_UTIL_INCLUDE_CONDITIONAL_EXPRESSION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_CONDITIONAL_EXPRESSION_BUILDER

#include "ir/expressions/conditionalExpression.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class ConditionalExpressionBuilder : public AstBuilder<ir::ConditionalExpression> {
public:
    explicit ConditionalExpressionBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    ConditionalExpressionBuilder &SetTest(Expression *test)
    {
        test_ = test;
        return *this;
    }

    ConditionalExpressionBuilder &SetConsequent(Expression *conseq)
    {
        consequent_ = conseq;
        return *this;
    }

    ConditionalExpressionBuilder &SetAlternate(Expression *alternate)
    {
        alternate_ = alternate;
        return *this;
    }

    ConditionalExpression *Build()
    {
        auto *node = AllocNode(test_, consequent_, alternate_);
        return node;
    }

private:
    Expression *test_ {};
    Expression *consequent_ {};
    Expression *alternate_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_CONDITIONAL_EXPRESSION_BUILDER