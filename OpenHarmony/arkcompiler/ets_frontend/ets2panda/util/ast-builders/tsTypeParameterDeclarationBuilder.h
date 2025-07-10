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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_DECLARATION_BUILDER

#include "ir/ts/tsTypeParameterDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSTypeParameterDeclarationBuilder : public AstBuilder<TSTypeParameterDeclaration> {
public:
    explicit TSTypeParameterDeclarationBuilder(ark::ArenaAllocator *allocator)
        : AstBuilder(allocator), params_(allocator->Adapter())
    {
    }

    TSTypeParameterDeclarationBuilder &AddParam(TSTypeParameter *param)
    {
        params_.emplace_back(param);
        return *this;
    }

    TSTypeParameterDeclarationBuilder &SetRequiredParams(size_t requiredParams)
    {
        requiredParams_ = requiredParams;
        return *this;
    }

    TSTypeParameterDeclaration *Build()
    {
        auto node = AllocNode(std::move(params_), requiredParams_);
        return node;
    }

private:
    ArenaVector<TSTypeParameter *> params_;
    size_t requiredParams_ = 0;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_TYPE_PARAMETER_DECLARATION_BUILDER