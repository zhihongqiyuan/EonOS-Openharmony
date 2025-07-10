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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_ARRAY_TYPE_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_ARRAY_TYPE_BUILDER

#include "ir/ts/tsArrayType.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSArrayTypeBuilder : public AstBuilder<TSArrayType> {
public:
    explicit TSArrayTypeBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSArrayTypeBuilder &SetExpression(TypeNode *elementType)
    {
        elementType_ = elementType;
        return *this;
    }

    TSArrayType *Build()
    {
        auto node = AllocNode(elementType_);
        return node;
    }

private:
    TypeNode *elementType_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_ARRAY_TYPE_BUILDER