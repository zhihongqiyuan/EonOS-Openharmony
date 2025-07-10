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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_ENUM_MEMBER_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_ENUM_MEMBER_BUILDER

#include "ir/ts/tsEnumMember.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSEnumMemberBuilder : public AstBuilder<ir::TSEnumMember> {
public:
    explicit TSEnumMemberBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSEnumMemberBuilder &SetKey(Expression *key)
    {
        key_ = key;
        return *this;
    }

    TSEnumMemberBuilder &SetInit(Expression *init)
    {
        init_ = init;
        return *this;
    }

    TSEnumMember *Build()
    {
        auto *node = AllocNode(key_, init_);
        return node;
    }

private:
    Expression *key_ {};
    Expression *init_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_ENUM_MEMBER_BUILDER