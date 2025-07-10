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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_INSTANTIATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_INSTANTIATION_BUILDER

#include "ir/ts/tsTypeParameterInstantiation.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSTypeParameterInstantiationBuilder : public AstBuilder<ir::TSTypeParameterInstantiation> {
public:
    explicit TSTypeParameterInstantiationBuilder(ark::ArenaAllocator *allocator)
        : AstBuilder(allocator), params_(allocator->Adapter())
    {
    }

    TSTypeParameterInstantiationBuilder &SetParams(ArenaVector<TypeNode *> &&params)
    {
        params_ = std::move(params);
        return *this;
    }

    TSTypeParameterInstantiationBuilder &AddParams(TypeNode *param)
    {
        params_.emplace_back(param);
        return *this;
    }

    TSTypeParameterInstantiation *Build()
    {
        auto *node = AllocNode(std::move(params_));
        return node;
    }

private:
    ArenaVector<TypeNode *> params_;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_INSTANTIATION_BUILDER