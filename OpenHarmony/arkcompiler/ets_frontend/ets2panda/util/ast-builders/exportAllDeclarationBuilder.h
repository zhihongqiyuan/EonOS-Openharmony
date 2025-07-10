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

#ifndef ES2PANDA_UTIL_INCLUDE_EXPORT_ALL_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_EXPORT_ALL_DECLARATION_BUILDER

#include "ir/module/exportAllDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class ExportAllDeclarationBuilder : public AstBuilder<ir::ExportAllDeclaration> {
public:
    explicit ExportAllDeclarationBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    ExportAllDeclarationBuilder &SetSource(StringLiteral *source)
    {
        source_ = source;
        return *this;
    }

    ExportAllDeclarationBuilder &SetExported(Identifier *exported)
    {
        exported_ = exported;
        return *this;
    }

    ExportAllDeclaration *Build()
    {
        auto *node = AllocNode(source_, exported_);
        return node;
    }

private:
    StringLiteral *source_ {};
    Identifier *exported_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_EXPORT_ALL_DECLARATION_BUILDER