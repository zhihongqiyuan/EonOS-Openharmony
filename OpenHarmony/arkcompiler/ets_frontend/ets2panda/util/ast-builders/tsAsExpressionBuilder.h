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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_AS_EXPRESSION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_AS_EXPRESSION_BUILDER

#include "ir/ts/tsAsExpression.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSAsExpressionBuilder : public AstBuilder<TSAsExpression> {
public:
    explicit TSAsExpressionBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSAsExpressionBuilder &SetExpression(Expression *expression)
    {
        expression_ = expression;
        return *this;
    }

    TSAsExpressionBuilder &SetTypeAnnotation(TypeNode *typeAnnotation)
    {
        typeAnnotation_ = typeAnnotation;
        return *this;
    }

    TSAsExpressionBuilder &SetIsCOnst(bool isConst)
    {
        isConst_ = isConst;
        return *this;
    }

    TSAsExpression *Build()
    {
        auto node = AllocNode(expression_, typeAnnotation_, isConst_);
        return node;
    }

private:
    Expression *expression_ {};
    TypeNode *typeAnnotation_ {};
    bool isConst_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_AS_EXPRESSION_BUILDER