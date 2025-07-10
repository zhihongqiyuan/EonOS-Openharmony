/*
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

#ifndef ES2PANDA_EVALUATE_DEBUG_INFO_DESERIALIZER_H
#define ES2PANDA_EVALUATE_DEBUG_INFO_DESERIALIZER_H

#include "libpandabase/utils/arena_containers.h"
#include "libpandafile/debug_info_extractor.h"
#include "libpandafile/file.h"

namespace ark::es2panda::varbinder {
class Variable;
}  // namespace ark::es2panda::varbinder

namespace ark::es2panda::parser {
class Program;
}  // namespace ark::es2panda::parser

namespace ark::es2panda::ir {
class ETSTypeReference;
}  // namespace ark::es2panda::ir

namespace ark::panda_file {
class ClassDataAccessor;
}  // namespace ark::panda_file

namespace ark::es2panda::evaluate {

struct ChainEntryInfo;
struct FileDebugInfo;
class ScopedDebugInfoPlugin;

class DebugInfoDeserializer final {
public:
    using RegisterNumber = int32_t;

public:
    NO_COPY_SEMANTIC(DebugInfoDeserializer);
    NO_MOVE_SEMANTIC(DebugInfoDeserializer);

    explicit DebugInfoDeserializer(ScopedDebugInfoPlugin &debugInfoPlugin);
    ~DebugInfoDeserializer() = default;

    varbinder::Variable *CreateIrClass(panda_file::File::EntityId classId, parser::Program *classProgram,
                                       util::StringView pathToSource, util::StringView classDeclName);

    varbinder::Variable *CreateIrLocalVariable(ir::Identifier *ident,
                                               const panda_file::LocalVariableTable &localVariableTable,
                                               size_t bytecodeOffset);

    varbinder::Variable *CreateIrGlobalVariable(parser::Program *program, util::StringView pathToSource,
                                                util::StringView varDeclName);

    varbinder::Variable *CreateIrGlobalMethods(ArenaVector<ir::AstNode *> &createdMethods, parser::Program *program,
                                               util::StringView pathToSource, util::StringView methodDeclName);

private:
    // Methods for resolving inheritance.
    ir::ETSTypeReference *GetSuperClass(panda_file::ClassDataAccessor &cda);

    /**
     * @brief Creates and checks all entries in inheritance chain.
     * @returns TypeReference to the last super class in resolved chain.
     */
    ir::ETSTypeReference *ResolveInheritanceChain(util::StringView abcSuperName, FileDebugInfo *debugInfo);

    ir::ETSTypeReference *ResolveInheritanceChainImpl(util::StringView abcSuperName, FileDebugInfo *debugInfo);

    /**
     * @brief Fills information required for inheritance chain recreation.
     * @returns declaration name of the first already created class in chain,
     * empty string if algorithm has reached std.core.Object.
     */
    util::StringView CollectChainInfo(ArenaVector<ChainEntryInfo> &chainEntryList, util::StringView abcSuperName,
                                      FileDebugInfo *debugInfo);

    varbinder::Variable *CreateLocalVarDecl(ir::Identifier *ident, RegisterNumber regNumber,
                                            const std::string &typeSignature);

    ir::ClassDeclaration *CreateClassDeclaration(util::StringView identName, panda_file::ClassDataAccessor &cda,
                                                 ir::ETSTypeReference *superClass, parser::Program *program);

private:
    ScopedDebugInfoPlugin &debugInfoPlugin_;
};

}  // namespace ark::es2panda::evaluate

#endif  // ES2PANDA_EVALUATE_DEBUG_INFO_DESERIALIZER_H
