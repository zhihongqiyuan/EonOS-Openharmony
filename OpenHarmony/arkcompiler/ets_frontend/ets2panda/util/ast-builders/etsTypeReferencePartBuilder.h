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

#ifndef ES2PANDA_UTIL_INCLUDE_ETS_TYPE_REFERENCE_PART_BUILDER
#define ES2PANDA_UTIL_INCLUDE_ETS_TYPE_REFERENCE_PART_BUILDER

#include "ir/ets/etsTypeReferencePart.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class ETSTypeReferencePartBuilder : public AstBuilder<ir::ETSTypeReferencePart> {
public:
    explicit ETSTypeReferencePartBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    ETSTypeReferencePartBuilder &SetName(Expression *name)
    {
        name_ = name;
        return *this;
    }

    ETSTypeReferencePartBuilder &SetTypeParams(TSTypeParameterInstantiation *typeParams)
    {
        typeParams_ = typeParams;
        return *this;
    }

    ETSTypeReferencePartBuilder &SetPrev(ETSTypeReferencePart *prev)
    {
        prev_ = prev;
        return *this;
    }

    ETSTypeReferencePart *Build()
    {
        auto node = AllocNode(name_, typeParams_, prev_);
        return node;
    }

private:
    ir::Expression *name_ {};
    ir::TSTypeParameterInstantiation *typeParams_ {};
    ir::ETSTypeReferencePart *prev_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_ETS_TYPE_REFERENCE_PART_BUILDER