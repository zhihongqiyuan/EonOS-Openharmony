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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_BUILDER

#include "ir/ts/tsTypeParameter.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSTypeParameterBuilder : public AstBuilder<TSTypeParameter> {
public:
    explicit TSTypeParameterBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSTypeParameterBuilder &SetName(Identifier *name)
    {
        name_ = name;
        return *this;
    }

    TSTypeParameterBuilder &SetConstraint(TypeNode *constraint)
    {
        constraint_ = constraint;
        return *this;
    }

    TSTypeParameterBuilder &SetDefaultType(TypeNode *defaultType)
    {
        defaultType_ = defaultType;
        return *this;
    }

    TSTypeParameter *Build()
    {
        auto node = AllocNode(name_, constraint_, defaultType_);
        return node;
    }

private:
    Identifier *name_ {};
    TypeNode *constraint_ {};
    TypeNode *defaultType_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_BUILDER