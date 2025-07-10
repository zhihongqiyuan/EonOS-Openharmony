/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_COMPILER_CORE_COMPILEQUEUE_H
#define ES2PANDA_COMPILER_CORE_COMPILEQUEUE_H

#include <aot/options.h>
#include <macros.h>
#include <os/thread.h>
#include <util/symbolTable.h>
#include <util/workerQueue.h>

#include <condition_variable>
#include <mutex>

#include <abc2program/abc2program_compiler.h>

namespace panda::es2panda::binder {
class FunctionScope;
}  // namespace panda::es2panda::binder

namespace panda::es2panda::compiler {

class CompilerContext;

class CompileFunctionJob : public util::WorkerJob {
public:
    explicit CompileFunctionJob(CompilerContext *context) : context_(context) {};
    NO_COPY_SEMANTIC(CompileFunctionJob);
    NO_MOVE_SEMANTIC(CompileFunctionJob);
    ~CompileFunctionJob() override = default;

    binder::FunctionScope *Scope() const
    {
        return scope_;
    }

    void SetFunctionScope(binder::FunctionScope *scope)
    {
        scope_ = scope;
    }

    void Run() override;

private:
    CompilerContext *context_ {};
    binder::FunctionScope *scope_ {};
};

class CompileModuleRecordJob : public util::WorkerJob {
public:
    explicit CompileModuleRecordJob(CompilerContext *context) : context_(context) {};
    NO_COPY_SEMANTIC(CompileModuleRecordJob);
    NO_MOVE_SEMANTIC(CompileModuleRecordJob);
    ~CompileModuleRecordJob() override = default;

    void Run() override;

private:
    CompilerContext *context_ {};
};

class CompileFileJob : public util::WorkerJob {
public:
    explicit CompileFileJob(es2panda::SourceFile *src, es2panda::CompilerOptions *options,
                            std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo,
                            std::unordered_set<std::string> &optimizationPendingProgs,
                            util::SymbolTable *symbolTable, panda::ArenaAllocator *allocator)
        : src_(src), options_(options), progsInfo_(progsInfo), optimizationPendingProgs_(optimizationPendingProgs),
          symbolTable_(symbolTable), allocator_(allocator) {};
    NO_COPY_SEMANTIC(CompileFileJob);
    NO_MOVE_SEMANTIC(CompileFileJob);
    ~CompileFileJob() override = default;

    void Run() override;

private:
    friend class CompileAbcClassJob;
    void CompileProgram();
    void OptimizeAndCacheProgram(panda::pandasm::Program *prog);
    bool RetrieveProgramFromCacheFiles(const std::string &buffer, bool isAbcFile = false);
    void CompileAbcFileJobInParallel(es2panda::Compiler &compiler);
    void InsertAbcCachePrograms(uint32_t hashCode,
                                std::map<std::string, panda::es2panda::util::ProgramCache *> &abcProgramsInfo);

    static std::mutex globalMutex_;
    es2panda::SourceFile *src_;
    es2panda::CompilerOptions *options_;
    std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo_;
    std::unordered_set<std::string> &optimizationPendingProgs_;
    util::SymbolTable *symbolTable_;
    panda::ArenaAllocator *allocator_;
};

class CompileAbcClassJob : public util::WorkerJob {
public:
    explicit CompileAbcClassJob(es2panda::SourceFile *src, const uint32_t classId,
                                const es2panda::CompilerOptions &options,
                                abc2program::Abc2ProgramCompiler &compiler,
                                std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo,
                                panda::ArenaAllocator *allocator,
                                std::string abcPkgName,
                                bool pkgVersionUpdateRequiredInAbc = true)
        : src_(src), classId_(classId), options_(options), compiler_(compiler), progsInfo_(progsInfo),
          allocator_(allocator), abcPkgName_(abcPkgName),
          pkgVersionUpdateRequiredInAbc_(pkgVersionUpdateRequiredInAbc) {};

    void SetOhmurlBeenChanged(bool hasOhmurlBeenChanged)
    {
        hasOhmurlBeenChanged_ = hasOhmurlBeenChanged;
    }
    void UpdatePkgNameOfImportOhmurl(panda::pandasm::Program *prog, const panda::es2panda::CompilerOptions &options);

    NO_COPY_SEMANTIC(CompileAbcClassJob);
    NO_MOVE_SEMANTIC(CompileAbcClassJob);
    ~CompileAbcClassJob() override = default;

    void Run() override;

private:
    void UpdateImportOhmurl(panda::pandasm::Program *prog, const CompilerOptions &options);
    void UpdateDynamicImport(panda::pandasm::Program *prog,
        const std::map<std::string, panda::es2panda::PkgInfo> &pkgContextInfo);
    void UpdateStaticImport(panda::pandasm::Program *prog,
        const std::map<std::string, panda::es2panda::PkgInfo> &pkgContextInfo);
    void UpdateBundleNameOfOhmurl(std::string &ohmurl);

    es2panda::SourceFile *src_;
    const uint32_t classId_;
    const es2panda::CompilerOptions &options_;
    abc2program::Abc2ProgramCompiler &compiler_;
    std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo_;
    panda::ArenaAllocator *allocator_;
    std::string abcPkgName_;
    bool pkgVersionUpdateRequiredInAbc_;
    bool hasOhmurlBeenChanged_ {false};
};

class PostAnalysisOptimizeFileJob : public util::WorkerJob {
public:
    explicit PostAnalysisOptimizeFileJob(const std::string &fileName, pandasm::Program *program)
        : fileName_(std::move(fileName)), program_(program) {}
    NO_COPY_SEMANTIC(PostAnalysisOptimizeFileJob);
    NO_MOVE_SEMANTIC(PostAnalysisOptimizeFileJob);
    ~PostAnalysisOptimizeFileJob() override = default;

    void Run() override;

private:
    std::string fileName_;
    pandasm::Program *program_;
};

class CompileFuncQueue : public util::WorkerQueue {
public:
    explicit CompileFuncQueue(size_t threadCount, CompilerContext *context)
        : util::WorkerQueue(threadCount), context_(context) {}

    NO_COPY_SEMANTIC(CompileFuncQueue);
    NO_MOVE_SEMANTIC(CompileFuncQueue);
    ~CompileFuncQueue() override = default;

    void Schedule() override;

private:
    CompilerContext *context_;
};

class CompileFileQueue : public util::WorkerQueue {
public:
    explicit CompileFileQueue(size_t threadCount, es2panda::CompilerOptions *options,
                              std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo,
                              std::unordered_set<std::string> &optimizationPendingProgs,
                              util::SymbolTable *symbolTable, panda::ArenaAllocator *allocator)
        : util::WorkerQueue(threadCount), options_(options), progsInfo_(progsInfo),
        optimizationPendingProgs_(optimizationPendingProgs), symbolTable_(symbolTable),
        allocator_(allocator) {}

    NO_COPY_SEMANTIC(CompileFileQueue);
    NO_MOVE_SEMANTIC(CompileFileQueue);
    ~CompileFileQueue() override = default;

    void Schedule() override;

private:
    es2panda::CompilerOptions *options_;
    std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo_;
    std::unordered_set<std::string> &optimizationPendingProgs_;
    util::SymbolTable *symbolTable_;
    panda::ArenaAllocator *allocator_;
};

class CompileAbcClassQueue : public util::WorkerQueue {
public:
    explicit CompileAbcClassQueue(size_t threadCount,
                                  const es2panda::CompilerOptions &options,
                                  abc2program::Abc2ProgramCompiler &compiler,
                                  std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo,
                                  panda::ArenaAllocator *allocator,
                                  es2panda::SourceFile *src)
        : util::WorkerQueue(threadCount), options_(options), compiler_(compiler), progsInfo_(progsInfo),
          allocator_(allocator), src_(src) {}

    NO_COPY_SEMANTIC(CompileAbcClassQueue);
    NO_MOVE_SEMANTIC(CompileAbcClassQueue);
    ~CompileAbcClassQueue() override = default;

    void Schedule() override;

private:
    bool NeedUpdateVersion();

    static std::mutex globalMutex_;
    const es2panda::CompilerOptions &options_;
    abc2program::Abc2ProgramCompiler &compiler_;
    std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo_;
    panda::ArenaAllocator *allocator_;
    es2panda::SourceFile *src_;
};

class PostAnalysisOptimizeFileQueue : public util::WorkerQueue {
public:
    explicit PostAnalysisOptimizeFileQueue(size_t threadCount,
                                           std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo,
                                           std::unordered_set<std::string> &optimizationPendingProgs)
        : util::WorkerQueue(threadCount), progsInfo_(progsInfo), optimizationPendingProgs_(optimizationPendingProgs) {}

    NO_COPY_SEMANTIC(PostAnalysisOptimizeFileQueue);
    NO_MOVE_SEMANTIC(PostAnalysisOptimizeFileQueue);
    ~PostAnalysisOptimizeFileQueue() override = default;

    void Schedule() override;

private:
    std::map<std::string, panda::es2panda::util::ProgramCache*> &progsInfo_;
    std::unordered_set<std::string> &optimizationPendingProgs_;
};

}  // namespace panda::es2panda::compiler

#endif
