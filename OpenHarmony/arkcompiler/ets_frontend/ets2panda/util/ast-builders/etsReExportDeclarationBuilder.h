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

#ifndef ES2PANDA_UTIL_INCLUDE_ETS_RE_EXPORT_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_ETS_RE_EXPORT_DECLARATION_BUILDER

#include "ir/ets/etsReExportDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class ETSReExportDeclarationBuilder : public AstBuilder<ir::ETSReExportDeclaration> {
public:
    explicit ETSReExportDeclarationBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    ETSReExportDeclarationBuilder &SetETSImportDeclaration(ETSImportDeclaration *etsImportDeclarations)
    {
        etsImportDeclarations_ = etsImportDeclarations;
        return *this;
    }

    ETSReExportDeclarationBuilder &AddUserPaths(util::StringView userPath)
    {
        userPaths_.emplace_back(userPath);
        return *this;
    }

    ETSReExportDeclarationBuilder &SetProgramPath(util::StringView programPath)
    {
        programPath_ = programPath;
        return *this;
    }

    ETSReExportDeclaration *Build()
    {
        auto node = AllocNode(etsImportDeclarations_, userPaths_, programPath_, Allocator());
        return node;
    }

private:
    ETSImportDeclaration *etsImportDeclarations_ {};
    std::vector<std::string> userPaths_;
    util::StringView programPath_;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_ETS_RE_EXPORT_DECLARATION_BUILDER