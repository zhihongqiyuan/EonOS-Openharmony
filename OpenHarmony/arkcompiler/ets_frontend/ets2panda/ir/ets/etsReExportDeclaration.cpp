/**
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include "ir/ets/etsReExportDeclaration.h"
#include "ir/astDump.h"

namespace ark::es2panda::ir {

ETSReExportDeclaration::ETSReExportDeclaration(ETSImportDeclaration *etsImportDeclarations,
                                               const std::vector<std::string> &userPaths, util::StringView programPath,
                                               ArenaAllocator *allocator)
    : Statement(AstNodeType::REEXPORT_STATEMENT),
      etsImportDeclarations_(etsImportDeclarations),
      userPaths_(allocator->Adapter()),
      programPath_(programPath)
{
    for (const auto &path : userPaths) {
        userPaths_.emplace_back(util::UString(path, allocator).View());
    }
}

void ETSReExportDeclaration::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (etsImportDeclarations_ != nullptr) {
        if (auto *transformedNode = cb(etsImportDeclarations_); etsImportDeclarations_ != transformedNode) {
            etsImportDeclarations_->SetTransformedNode(transformationName, transformedNode);
            etsImportDeclarations_ = transformedNode->AsETSImportDeclaration();
        }
    }
}

void ETSReExportDeclaration::Iterate(const NodeTraverser &cb) const
{
    etsImportDeclarations_->Iterate(cb);
}

void ETSReExportDeclaration::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSReExportDeclaration"}, {"ets_import_declarations", etsImportDeclarations_}});
}

}  // namespace ark::es2panda::ir
