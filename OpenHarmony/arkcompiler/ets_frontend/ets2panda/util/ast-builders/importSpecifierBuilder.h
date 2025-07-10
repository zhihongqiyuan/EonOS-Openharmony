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

#ifndef ES2PANDA_UTIL_INCLUDE_IMPORT_SPECIFIER_BUILDER
#define ES2PANDA_UTIL_INCLUDE_IMPORT_SPECIFIER_BUILDER

#include "ir/module/importSpecifier.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class ImportSpecifierBuilder : public AstBuilder<ir::ImportSpecifier> {
public:
    explicit ImportSpecifierBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    ImportSpecifierBuilder &SetImported(Identifier *imported)
    {
        imported_ = imported;
        return *this;
    }

    ImportSpecifierBuilder &SetLocal(Identifier *local)
    {
        local_ = local;
        return *this;
    }

    ImportSpecifier *Build()
    {
        auto node = AllocNode(imported_, local_);
        return node;
    }

private:
    Identifier *imported_ {};
    Identifier *local_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_IMPORT_SPECIFIER_BUILDER