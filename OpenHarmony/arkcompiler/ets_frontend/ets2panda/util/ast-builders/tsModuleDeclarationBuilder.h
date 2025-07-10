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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_MODULE_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_MODULE_DECLARATION_BUILDER

#include "ir/ts/tsModuleDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSModuleDeclarationBuilder : public AstBuilder<TSModuleDeclaration> {
public:
    explicit TSModuleDeclarationBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSModuleDeclarationBuilder &SetName(Expression *name)
    {
        name_ = name;
        return *this;
    }

    TSModuleDeclarationBuilder &SetBody(Statement *body)
    {
        body_ = body;
        return *this;
    }

    TSModuleDeclarationBuilder &SetGlobal(bool global)
    {
        global_ = global;
        return *this;
    }

    TSModuleDeclarationBuilder &SetIsExternalAmbient(bool isExternalAmbient)
    {
        isExternalAmbient_ = isExternalAmbient;
        return *this;
    }

    TSModuleDeclaration *Build()
    {
        auto node =
            AllocNode(Allocator(), name_, body_, TSModuleDeclaration::ConstructorFlags {global_, isExternalAmbient_});
        return node;
    }

private:
    Expression *name_ {};
    Statement *body_ {};
    bool global_ = false;
    bool isExternalAmbient_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_MODULE_DECLARATION_BUILDER