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

#include "resolveIdentifiers.h"

#include "varbinder/ETSBinder.h"

namespace ark::es2panda::compiler {
bool ResolveIdentifiers::Perform(public_lib::Context *ctx, [[maybe_unused]] parser::Program *program)
{
    auto const &options = ctx->config->options->CompilerOptions();
    auto *varbinder = ctx->parserProgram->VarBinder()->AsETSBinder();

    if (options.dumpAst) {
        std::cout << varbinder->Program()->Dump() << std::endl;
    }

    if (options.opDumpAstOnlySilent) {
        varbinder->Program()->DumpSilent();
        return false;
    }

    if (options.parseOnly) {
        return false;
    }

    varbinder->SetGenStdLib(options.compilationMode == CompilationMode::GEN_STD_LIB);
    varbinder->IdentifierAnalysis();

    return true;
}

}  // namespace ark::es2panda::compiler
