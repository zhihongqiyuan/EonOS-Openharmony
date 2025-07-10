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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_TYPE_ALIAS_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_TYPE_ALIAS_DECLARATION_BUILDER

#include "ir/ts/tsTypeAliasDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSTypeAliasDeclarationBuilder : public AstBuilder<TSTypeAliasDeclaration> {
public:
    explicit TSTypeAliasDeclarationBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSTypeAliasDeclarationBuilder &SetId(Identifier *id)
    {
        id_ = id;
        return *this;
    }

    TSTypeAliasDeclarationBuilder &SetTypeParams(TSTypeParameterDeclaration *typeParams)
    {
        typeParams_ = typeParams;
        return *this;
    }

    TSTypeAliasDeclarationBuilder &SetTypeAnnotation(TypeNode *typeAnnotation)
    {
        typeAnnotation_ = typeAnnotation;
        return *this;
    }

    TSTypeAliasDeclaration *Build()
    {
        auto node = AllocNode(Allocator(), id_, typeParams_, typeAnnotation_);
        return node;
    }

private:
    Identifier *id_ {};
    TSTypeParameterDeclaration *typeParams_ {};
    TypeNode *typeAnnotation_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_TYPE_ALIAS_DECLARATION_BUILDER