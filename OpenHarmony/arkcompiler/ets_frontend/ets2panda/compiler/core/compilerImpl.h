/**
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_COMPILER_INCLUDE_COMPILER_IMPL_H
#define ES2PANDA_COMPILER_INCLUDE_COMPILER_IMPL_H

#include "es2panda.h"
#include "compiler/core/compileQueue.h"
#include "macros.h"
#include "mem/arena_allocator.h"
#include "os/thread.h"

#include <string>

namespace ark::pandasm {
struct Program;
}  // namespace ark::pandasm

namespace ark::es2panda::compiler {
class CompileQueue;

class CompilationUnit {
public:
    explicit CompilationUnit(const SourceFile &i, const util::Options &o, uint32_t s, ScriptExtension e)
        : input(i), options(o), rawParserStatus(s), ext(e)
    {
    }

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    const SourceFile &input;
    const util::Options &options;
    uint32_t rawParserStatus;
    ScriptExtension ext;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

class CompilerImpl {
public:
    explicit CompilerImpl(size_t threadCount, std::vector<util::Plugin> const *plugins)
        : queue_(threadCount), plugins_(plugins)
    {
    }
    NO_COPY_SEMANTIC(CompilerImpl);
    NO_MOVE_SEMANTIC(CompilerImpl);
    ~CompilerImpl() = default;

    pandasm::Program *Compile(const CompilationUnit &unit);

    std::vector<util::Plugin> const &Plugins()
    {
        return *plugins_;
    }

    static void DumpAsm(const ark::pandasm::Program *prog);
    static std::string GetPhasesList(const ScriptExtension ext);

    ark::pandasm::Program *Emit(public_lib::Context *context);

    CompileQueue *Queue()
    {
        return &queue_;
    }

    bool IsAnyError()
    {
        return isAnyError_;
    }

    void SetIsAnyError(bool value)
    {
        isAnyError_ = value;
    }

private:
    static void HandleContextLiterals(public_lib::Context *context);

    CompileQueue queue_;
    std::vector<util::Plugin> const *plugins_;
    bool isAnyError_ = false;
};
}  // namespace ark::es2panda::compiler

#endif
