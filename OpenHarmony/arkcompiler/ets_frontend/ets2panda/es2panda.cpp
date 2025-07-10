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

#include "es2panda.h"

#include "compiler/core/compilerImpl.h"
#include "util/options.h"

#include <iostream>
#include <thread>

namespace ark::es2panda {
constexpr size_t DEFAULT_THREAD_COUNT = 2;

template <class T>
T DirName(T const &path, T const &delims = ark::os::file::File::GetPathDelim())
{
    std::size_t pos = path.find_last_of(delims);
    if (pos == std::string::npos) {
        return "./";
    }

    if (pos == 0) {
        return delims;
    }

    std::string_view dirPath = path.substr(0, pos);
    if (dirPath.compare(".") == 0 || dirPath.compare("..") == 0) {
        return path.substr(0, pos + 1);
    }

    return path.substr(0, pos);
}

SourceFile::SourceFile(std::string_view fn, std::string_view s) : filePath(fn), fileFolder(DirName(fn)), source(s) {}

SourceFile::SourceFile(std::string_view fn, std::string_view s, bool m)
    : filePath(fn), fileFolder(DirName(fn)), source(s), isModule(m)
{
}

SourceFile::SourceFile(std::string_view fn, std::string_view s, std::string_view rp, bool m)
    : filePath(fn), fileFolder(DirName(fn)), source(s), resolvedPath(DirName(rp)), isModule(m)
{
}

Compiler::Compiler(ScriptExtension ext) : Compiler(ext, DEFAULT_THREAD_COUNT, {}) {}

Compiler::Compiler(ScriptExtension ext, size_t threadCount) : Compiler(ext, threadCount, {}) {}

Compiler::Compiler(ScriptExtension ext, size_t threadCount, std::vector<util::Plugin> &&plugins)
    : plugins_(std::move(plugins)), compiler_(new compiler::CompilerImpl(threadCount, &plugins_)), ext_(ext)
{
}

Compiler::~Compiler()
{
    delete compiler_;
}

pandasm::Program *Compiler::Compile(const SourceFile &input, const util::Options &options, uint32_t parseStatus)
{
    try {
        return compiler_->Compile(compiler::CompilationUnit {input, options, parseStatus, ext_});
    } catch (const class Error &e) {
        error_ = e;
        return nullptr;
    }
}

bool Compiler::IsAnyError() const noexcept
{
    return compiler_->IsAnyError();
}

std::string Compiler::GetPhasesList() const
{
    return compiler::CompilerImpl::GetPhasesList(ext_);
}

void Compiler::DumpAsm(const pandasm::Program *prog)
{
    compiler::CompilerImpl::DumpAsm(prog);
}
}  // namespace ark::es2panda
