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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_ENUM_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_ENUM_DECLARATION_BUILDER

#include "ir/ts/tsEnumDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSEnumDeclarationBuilder : public AstBuilder<ir::TSEnumDeclaration> {
public:
    explicit TSEnumDeclarationBuilder(ark::ArenaAllocator *allocator)
        : AstBuilder(allocator), members_(allocator->Adapter())
    {
    }

    TSEnumDeclarationBuilder &SetKey(Identifier *key)
    {
        key_ = key;
        return *this;
    }

    TSEnumDeclarationBuilder &AddMember(TSEnumMember *member)
    {
        members_.emplace_back(member);
        return *this;
    }

    TSEnumDeclarationBuilder &SetIsStatic(bool isStatic)
    {
        isStatic_ = isStatic;
        return *this;
    }

    TSEnumDeclarationBuilder &SetIsDeclare(bool isDeclare)
    {
        isDeclare_ = isDeclare;
        return *this;
    }

    TSEnumDeclarationBuilder &SetIsConst(bool isConst)
    {
        isConst_ = isConst;
        return *this;
    }

    TSEnumDeclaration *Build()
    {
        auto node = AllocNode(Allocator(), key_, std::move(members_),
                              ir::TSEnumDeclaration::ConstructorFlags {isConst_, isStatic_, isDeclare_});
        return node;
    }

private:
    Identifier *key_ {};
    ArenaVector<AstNode *> members_;
    bool isConst_ = false;
    bool isStatic_ = false;
    bool isDeclare_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_ENUM_DECLARATION_BUILDER