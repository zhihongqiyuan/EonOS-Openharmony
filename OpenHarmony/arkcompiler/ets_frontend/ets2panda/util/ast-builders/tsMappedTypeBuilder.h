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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_MAPPED_TYPE_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_MAPPED_TYPE_BUILDER

#include "ir/ts/tsMappedType.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSMappedTypeBuilder : public AstBuilder<TSMappedType> {
public:
    explicit TSMappedTypeBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSMappedTypeBuilder &SetTypeParameter(TSTypeParameter *typeParameter)
    {
        typeParameter_ = typeParameter;
        return *this;
    }

    TSMappedTypeBuilder &SetTypeAnnotation(TypeNode *typeAnnotation)
    {
        typeAnnotation_ = typeAnnotation;
        return *this;
    }

    TSMappedTypeBuilder &SetReadonly(MappedOption readonly)
    {
        readonly_ = readonly;
        return *this;
    }

    TSMappedTypeBuilder &SetOptional(MappedOption optional)
    {
        optional_ = optional;
        return *this;
    }

    TSMappedType *Build()
    {
        auto node = AllocNode(typeParameter_, typeAnnotation_, readonly_, optional_);
        return node;
    }

private:
    TSTypeParameter *typeParameter_ {};
    TypeNode *typeAnnotation_ {};
    MappedOption readonly_ {};
    MappedOption optional_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_MAPPED_TYPE_BUILDER