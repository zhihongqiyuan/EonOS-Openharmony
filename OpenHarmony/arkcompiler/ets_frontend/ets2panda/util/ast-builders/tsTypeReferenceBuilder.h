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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_TYPE_REFERENCE_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_TYPE_REFERENCE_BUILDER

#include "ir/ts/tsTypeReference.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSTypeReferenceBuilder : public AstBuilder<TSTypeReference> {
public:
    explicit TSTypeReferenceBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSTypeReferenceBuilder &SetTypeName(Identifier *typeName)
    {
        typeName_ = typeName;
        return *this;
    }

    TSTypeReferenceBuilder &SetTypeParams(TSTypeParameterInstantiation *typeParams)
    {
        typeParams_ = typeParams;
        return *this;
    }

    TSTypeReference *Build()
    {
        auto node = AllocNode(typeName_, typeParams_);
        return node;
    }

private:
    Expression *typeName_ {};
    TSTypeParameterInstantiation *typeParams_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_TYPE_REFERENCE_BUILDER