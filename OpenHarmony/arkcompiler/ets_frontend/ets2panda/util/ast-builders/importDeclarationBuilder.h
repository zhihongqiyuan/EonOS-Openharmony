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

#ifndef ES2PANDA_UTIL_INCLUDE_IMPORT_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_IMPORT_DECLARATION_BUILDER

#include "ir/module/importDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class ImportDeclarationBuilder : public AstBuilder<ir::ImportDeclaration> {
public:
    explicit ImportDeclarationBuilder(ark::ArenaAllocator *allocator)
        : AstBuilder(allocator), specifiers_(allocator->Adapter())
    {
    }

    ImportDeclarationBuilder &SetSource(StringLiteral *source)
    {
        source_ = source;
        return *this;
    }

    ImportDeclarationBuilder &AddSpecifier(AstNode *spec)
    {
        specifiers_.emplace_back(spec);
        return *this;
    }

    ImportDeclarationBuilder &SetImportKind(ImportKinds kind)
    {
        importKind_ = kind;
        return *this;
    }

    ImportDeclaration *Build()
    {
        auto *node = AllocNode(source_, std::move(specifiers_), importKind_);
        return node;
    }

private:
    StringLiteral *source_ {};
    ArenaVector<AstNode *> specifiers_;
    ImportKinds importKind_ = ImportKinds::VALUE;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_IMPORT_DECLARATION_BUILDER