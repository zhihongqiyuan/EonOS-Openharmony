/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "compilerImpl.h"

#include <compiler/core/compileQueue.h>
#include <compiler/core/compilerContext.h>
#include <compiler/core/emitter/emitter.h>

namespace panda::es2panda::compiler {

CompilerImpl::~CompilerImpl()
{
    if (queue_ != nullptr) {
        delete queue_;
        queue_ = nullptr;
    }
}

panda::pandasm::Program *CompilerImpl::Compile(parser::Program *program, const es2panda::CompilerOptions &options,
    const std::string &debugInfoSourceFile, const std::string &pkgName)
{
    CompilerContext context(program->Binder(), options.isDebug, options.isDebuggerEvaluateExpressionMode,
                            options.mergeAbc, false, options.recordDebugSource,
                            debugInfoSourceFile, pkgName, program->RecordName(), patchFixHelper_);

    ArenaAllocator localAllocator(SpaceType::SPACE_TYPE_COMPILER, nullptr, true);

    queue_ = new CompileFuncQueue(threadCount_, &context);
    queue_->Schedule();

    /* Main thread can also be used instead of idling */
    queue_->Consume();
    queue_->Wait();

    if (context.IsMergeAbc()) {
        context.GetEmitter()->GenRecordNameInfo();
    }

    // For global optimization
    // Constant local export module variable slot info is recorded in the emitter for now,
    // emit it to the bytecode analysis result here.
    if (options.requireGlobalOptimization) {
        util::Helpers::SetConstantLocalExportSlots(std::string(program->RecordName()),
            context.GetEmitter()->GetConstantLocalExportSlots());
    }

    return context.GetEmitter()->Finalize(options.dumpDebugInfo, patchFixHelper_);
}

void CompilerImpl::DumpAsm(const panda::pandasm::Program *prog)
{
    Emitter::DumpAsm(prog);
}

}  // namespace panda::es2panda::compiler
