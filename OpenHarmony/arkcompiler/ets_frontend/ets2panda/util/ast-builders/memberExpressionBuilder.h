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

#ifndef ES2PANDA_UTIL_INCLUDE_MEMBER_EXPRESSION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_MEMBER_EXPRESSION_BUILDER

#include "ir/expressions/memberExpression.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class MemberExpressionBuilder : public AstBuilder<ir::MemberExpression> {
public:
    explicit MemberExpressionBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    MemberExpressionBuilder &SetObject(Expression *obj)
    {
        object_ = obj;
        return *this;
    }

    MemberExpressionBuilder &SetProperty(Expression *prop)
    {
        property_ = prop;
        return *this;
    }

    MemberExpressionBuilder &SetKind(MemberExpressionKind kind)
    {
        kind_ = kind;
        return *this;
    }

    MemberExpression *Build()
    {
        auto node = AllocNode(object_, property_, kind_, false, false);
        return node;
    }

private:
    Expression *object_ {};
    Expression *property_ {};
    MemberExpressionKind kind_ = MemberExpressionKind::NONE;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_MEMBER_EXPRESSION_BUILDER