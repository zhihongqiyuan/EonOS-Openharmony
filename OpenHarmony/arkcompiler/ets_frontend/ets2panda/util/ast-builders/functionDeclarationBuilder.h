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

#ifndef ES2PANDA_UTIL_INCLUDE_FUNCTION_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_FUNCTION_DECLARATION_BUILDER

#include "ir/statements/functionDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class FunctionDeclarationBuilder : public AstBuilder<ir::FunctionDeclaration> {
public:
    explicit FunctionDeclarationBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    FunctionDeclarationBuilder &SetFunction(ScriptFunction *func)
    {
        func_ = func;
        return *this;
    }

    FunctionDeclarationBuilder &SetIsAnonymous(bool isAnonymous)
    {
        isAnonymous_ = isAnonymous;
        return *this;
    }

    FunctionDeclaration *Build()
    {
        auto *node = AllocNode(Allocator(), func_, isAnonymous_);
        return node;
    }

private:
    ScriptFunction *func_ {};
    bool isAnonymous_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_FUNCTION_DECLARATION_BUILDER