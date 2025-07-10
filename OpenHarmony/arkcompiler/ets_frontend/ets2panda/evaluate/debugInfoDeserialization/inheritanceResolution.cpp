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

#include "checker/ETSchecker.h"
#include "evaluate/helpers.h"

#include "evaluate/debugInfoDeserialization/debugInfoDeserializer.h"
#include "evaluate/scopedDebugInfoPlugin.h"
#include "evaluate/debugInfoStorage.h"
#include "evaluate/proxyProgramsCache.h"
#include "evaluate/entityDeclarator-inl.h"

#include "libpandafile/class_data_accessor.h"
#include "libpandafile/file-inl.h"

namespace ark::es2panda::evaluate {

namespace {

constexpr std::string_view OBJECT_NAME = "std.core.Object";

std::string GetFullSuperClassName(panda_file::ClassDataAccessor &cda)
{
    return panda_file::ClassDataAccessor::DemangledName(cda.GetPandaFile().GetStringData(cda.GetSuperClassId()));
}

std::optional<std::pair<util::StringView, FileDebugInfo *>> GetSuperClassModuleAndClassName(
    panda_file::ClassDataAccessor &cda, DebugInfoStorage *debugInfoStorage)
{
    ASSERT(debugInfoStorage);

    auto fullSuperClassName = GetFullSuperClassName(cda);
    if (fullSuperClassName == OBJECT_NAME) {
        // Must stop iterating upon reaching std.core.Object.
        return {};
    }

    auto [moduleName, recordName] = helpers::SplitRecordName(fullSuperClassName);
    auto *debugInfo = debugInfoStorage->GetDebugInfoByModuleName(moduleName);
    if (UNLIKELY(debugInfo == nullptr)) {
        LOG(FATAL, ES2PANDA) << "Failed to find debug info for module '" << moduleName << "'";
    }
    // Allocate record name for later usage in compilation.
    return std::make_pair(util::UString(recordName, debugInfoStorage->Allocator()).View(), debugInfo);
}

}  // namespace

struct ChainEntryInfo final {
    explicit ChainEntryInfo(std::string_view filePath, std::string_view declName,
                            panda_file::ClassDataAccessor *accessor, parser::Program *prog)
        : sourceFilePath(filePath), entityDeclarationName(declName), cda(accessor), program(prog)
    {
        ASSERT(cda != nullptr);
    }

    DEFAULT_MOVE_SEMANTIC(ChainEntryInfo);
    DEFAULT_COPY_SEMANTIC(ChainEntryInfo);

    ~ChainEntryInfo() = default;

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    std::string_view sourceFilePath;
    std::string_view entityDeclarationName;
    // Pointer is required for move semantics.
    panda_file::ClassDataAccessor *cda {nullptr};
    parser::Program *program {nullptr};
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

ir::ETSTypeReference *DebugInfoDeserializer::GetSuperClass(panda_file::ClassDataAccessor &cda)
{
    auto optClassInfo = GetSuperClassModuleAndClassName(cda, debugInfoPlugin_.GetDebugInfoStorage());
    if (!optClassInfo) {
        return nullptr;
    }
    auto [superClassName, debugInfo] = *optClassInfo;
    return ResolveInheritanceChain(superClassName, debugInfo);
}

ir::ETSTypeReference *DebugInfoDeserializer::ResolveInheritanceChain(util::StringView abcSuperName,
                                                                     FileDebugInfo *debugInfo)
{
    ASSERT(debugInfo);

    auto *program = debugInfoPlugin_.GetProxyProgramsCache()->GetProgram(debugInfo->sourceFilePath);
    if (debugInfoPlugin_.GetEntityDeclarator()->IsEntityDeclared(program, abcSuperName)) {
        // If a superclass has already been declared, then we just need to create the TypeReference of it.
        // Note that a TypeReference is created for each of usage of the super class.
        return helpers::CreateETSTypeReference(debugInfoPlugin_.GetIrCheckHelper()->GetChecker(), abcSuperName);
    }
    return ResolveInheritanceChainImpl(abcSuperName, debugInfo);
}

ir::ETSTypeReference *DebugInfoDeserializer::ResolveInheritanceChainImpl(util::StringView abcSuperName,
                                                                         FileDebugInfo *debugInfo)
{
    auto *checker = debugInfoPlugin_.GetIrCheckHelper()->GetChecker();

    // List starting from the most derived to the base class.
    ArenaVector<ChainEntryInfo> chainEntryList(checker->Allocator()->Adapter());
    auto alreadyCreatedSuperClassName = CollectChainInfo(chainEntryList, abcSuperName, debugInfo);

    ir::ETSTypeReference *superClass = nullptr;
    if (!alreadyCreatedSuperClassName.Empty()) {
        superClass = helpers::CreateETSTypeReference(checker, util::StringView(alreadyCreatedSuperClassName));
    }

    auto *entityDeclarator = debugInfoPlugin_.GetEntityDeclarator();
    for (auto it = chainEntryList.rbegin(); it != chainEntryList.rend(); ++it) {
        // Create entity in the same file in that it destinded to be declared, so import declaration should not be
        // inserted in AST.
        std::string_view declarationName = it->entityDeclarationName;
        auto *cda = it->cda;
        ASSERT(cda != nullptr);
        // clang-format off
        entityDeclarator->ImportGlobalEntity(it->sourceFilePath, declarationName, it->program, declarationName,
            [this, superClass, cda](auto, auto *program, auto, auto name) {
                auto *classDecl = CreateClassDeclaration(name, *cda, superClass, program);
                return classDecl->Definition()->Ident()->Variable();
            });
        // clang-format on
        superClass = helpers::CreateETSTypeReference(checker, declarationName);
    }

    return superClass;
}

util::StringView DebugInfoDeserializer::CollectChainInfo(ArenaVector<ChainEntryInfo> &chainEntryList,
                                                         util::StringView abcSuperName, FileDebugInfo *debugInfo)
{
    ASSERT(debugInfo != nullptr);

    auto *proxyProgramsCache = debugInfoPlugin_.GetProxyProgramsCache();
    auto *debugInfoStorage = debugInfoPlugin_.GetDebugInfoStorage();
    auto *entityDeclarator = debugInfoPlugin_.GetEntityDeclarator();
    auto *allocator = debugInfoPlugin_.GetIrCheckHelper()->GetChecker()->Allocator();

    // CC-OFFNXT(G.CTL.03) false positive
    while (true) {
        auto *program = proxyProgramsCache->GetProgram(debugInfo->sourceFilePath);
        ASSERT(program != nullptr);
        if (entityDeclarator->IsEntityDeclared(program, abcSuperName)) {
            // Go until reach A_i, which has already been declared.
            // Object <-- A_1 <-- ... <-- A_i <-- ... <-- A_n
            //    |                        |
            //    +------------------------+
            //                |
            //             Declared
            return abcSuperName;
        }

        // Class must be created - save necessary information about it.
        auto classId = debugInfoStorage->FindClass(debugInfo->sourceFilePath, abcSuperName.Utf8());
        if (!classId.IsValid()) {
            LOG(FATAL, ES2PANDA) << "Can't find classId for " << abcSuperName;
        }
        auto *cda = allocator->New<panda_file::ClassDataAccessor>(*debugInfo->pf, classId);
        chainEntryList.emplace_back(debugInfo->sourceFilePath, abcSuperName.Utf8(), cda, program);

        // Update information for the next iteration.
        auto optClassInfo = GetSuperClassModuleAndClassName(*cda, debugInfoStorage);
        if (!optClassInfo) {
            // Go until reach Object:
            // std.core.Object <-- A1 <-- A2 <-- ... <-- An
            return "";
        }
        std::tie(abcSuperName, debugInfo) = *optClassInfo;
    }
}

}  // namespace ark::es2panda::evaluate
