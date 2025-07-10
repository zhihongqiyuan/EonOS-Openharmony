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

#ifndef ES2PANDA_UTIL_INCLUDE_ETS_WILDCARD_TYPE_BUILDER
#define ES2PANDA_UTIL_INCLUDE_ETS_WILDCARD_TYPE_BUILDER

#include "ir/ets/etsWildcardType.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class ETSWildcardTypeBuilder : public AstBuilder<ETSWildcardType> {
public:
    explicit ETSWildcardTypeBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    ETSWildcardTypeBuilder &SetETSTypeReference(ir::ETSTypeReference *typeReference)
    {
        typeReference_ = typeReference;
        return *this;
    }

    ETSWildcardTypeBuilder &SetFlags(ModifierFlags flags)
    {
        flags_ = flags;
        return *this;
    }

    ETSWildcardType *Build()
    {
        auto node = AllocNode(typeReference_, flags_);
        return node;
    }

private:
    ir::ETSTypeReference *typeReference_ {};
    ModifierFlags flags_ = ModifierFlags::NONE;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_ETS_WILDCARD_TYPE_BUILDER