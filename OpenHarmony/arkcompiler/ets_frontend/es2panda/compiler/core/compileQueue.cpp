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

#include "compileQueue.h"

#include "utils/timers.h"

#include <compiler/core/compilerContext.h>
#include <compiler/core/emitter/emitter.h>
#include <compiler/core/function.h>
#include <compiler/core/pandagen.h>
#include <protobufSnapshotGenerator.h>
#include <util/commonUtil.h>

namespace panda::es2panda::compiler {

std::mutex CompileFileJob::globalMutex_;
std::mutex CompileAbcClassQueue::globalMutex_;

void CompileFunctionJob::Run()
{
    std::unique_lock<std::mutex> lock(m_);
    cond_.wait(lock, [this] { return dependencies_ == 0; });

    ArenaAllocator allocator(SpaceType::SPACE_TYPE_COMPILER, nullptr, true);
    PandaGen pg(&allocator, context_, scope_);

    Function::Compile(&pg);

    FunctionEmitter funcEmitter(&allocator, &pg);
    funcEmitter.Generate(context_->PatchFixHelper());

    context_->GetEmitter()->AddFunction(&funcEmitter, context_);

    for (auto *dependant : dependants_) {
        dependant->Signal();
    }
}

void CompileModuleRecordJob::Run()
{
    std::unique_lock<std::mutex> lock(m_);
    cond_.wait(lock, [this] { return dependencies_ == 0; });

    bool hasLazyImport = context_->Binder()->Program()->ModuleRecord()->HasLazyImport();
    ModuleRecordEmitter moduleEmitter(context_->Binder()->Program()->ModuleRecord(), context_->NewLiteralIndex(),
        hasLazyImport ? context_->NewLiteralIndex() : -1);
    moduleEmitter.Generate();

    context_->GetEmitter()->AddSourceTextModuleRecord(&moduleEmitter, context_);

    for (auto *dependant : dependants_) {
        dependant->Signal();
    }
}

bool CompileFileJob::RetrieveProgramFromCacheFiles(const std::string &buffer, bool isAbcFile)
{
    if (options_->requireGlobalOptimization) {
        return false;
    }
    auto cacheFileIter = options_->cacheFiles.find(src_->fileName);
    // Disable the use of file caching when cross-program optimization is required, to prevent cached files from
    // not being invalidated when their dependencies change, or from not being reanalyzed when their dependents
    // are updated
    if (cacheFileIter != options_->cacheFiles.end()) {
        // cache is invalid when any one of source file infos being changed
        auto bufToHash = buffer + src_->fileName + src_->recordName + src_->sourcefile + src_->pkgName;
        ArenaAllocator allocator(SpaceType::SPACE_TYPE_COMPILER, nullptr, true);
        if (!isAbcFile) {
            src_->hash = GetHash32String(reinterpret_cast<const uint8_t *>(bufToHash.c_str()));
            auto *cacheProgramInfo = proto::ProtobufSnapshotGenerator::GetCacheContext(cacheFileIter->second,
                                                                                       &allocator);
            if (cacheProgramInfo != nullptr && cacheProgramInfo->hashCode == src_->hash) {
                std::unique_lock<std::mutex> lock(globalMutex_);
                auto *cache = allocator_->New<util::ProgramCache>(src_->hash, std::move(cacheProgramInfo->program));
                progsInfo_.insert({src_->fileName, cache});
                return true;
            }
        } else {
            std::map<std::string, PkgInfo> updateVersionInfo =
                options_->compileContextInfo.updateVersionInfo[src_->pkgName];
            auto abcBufToHash = bufToHash;
            for (auto &[pkgName, pkgInfo]: updateVersionInfo) {
                /**
                 * When the bytecode har dependency package version changes, it needs to be recompiled, so the hash
                 * value needs to change
                 */
                abcBufToHash = abcBufToHash + pkgName + ":" + pkgInfo.version;
            }
            // An ABC file starts with '\0', but the 'GetHash32String' method does not support this format.
            src_->hash = GetHash32(reinterpret_cast<const uint8_t *>(abcBufToHash.c_str()), abcBufToHash.size());
            auto *cacheAbcProgramsInfo = proto::ProtobufSnapshotGenerator::GetAbcInputCacheContext(
                cacheFileIter->second, &allocator);
            if (cacheAbcProgramsInfo != nullptr && cacheAbcProgramsInfo->hashCode == src_->hash) {
                InsertAbcCachePrograms(src_->hash, cacheAbcProgramsInfo->programsCache);
                return true;
            }
        }
    }
    return false;
}

void CompileFileJob::InsertAbcCachePrograms(uint32_t hashCode,
    std::map<std::string, panda::es2panda::util::ProgramCache *> &abcProgramsInfo)
{
    std::unique_lock<std::mutex> lock(globalMutex_);
    Compiler::SetExpectedProgsCount(Compiler::GetExpectedProgsCount() + abcProgramsInfo.size() - 1);
    for (auto pair : abcProgramsInfo) {
        ASSERT(progsInfo_.find(pair.first) == progsInfo_.end());
        pair.second->program.isGeneratedFromMergedAbc = true;
        auto *cache = allocator_->New<util::ProgramCache>(hashCode, std::move(pair.second->program), false);
        progsInfo_.insert({pair.first, cache});
    }
}

void CompileFileJob::Run()
{
    std::stringstream ss;
    std::string buffer;
    panda::Timer::timerStart(panda::EVENT_READ_INPUT_AND_CACHE, src_->fileName);
    if (!src_->fileName.empty()) {
        if (!util::Helpers::ReadFileToBuffer(src_->fileName, ss)) {
            return;
        }
        buffer = ss.str();
        src_->source = buffer;
        if (RetrieveProgramFromCacheFiles(buffer, !src_->isSourceMode)) {
            panda::Timer::timerEnd(panda::EVENT_READ_INPUT_AND_CACHE, src_->fileName);
            return;
        }
    }
    panda::Timer::timerEnd(panda::EVENT_READ_INPUT_AND_CACHE, src_->fileName);

    CompileProgram();
}

void CompileFileJob::CompileProgram()
{
    es2panda::Compiler compiler(src_->scriptExtension, options_->functionThreadCount);
    panda::pandasm::Program *prog = nullptr;

    if (src_->isSourceMode) {
        panda::Timer::timerStart(panda::EVENT_COMPILE_FILE, src_->fileName);
        prog = compiler.CompileFile(*options_, src_, symbolTable_);
        panda::Timer::timerEnd(panda::EVENT_COMPILE_FILE, src_->fileName);
    } else if (!options_->mergeAbc) {
        // If input is an abc file, in non merge-abc mode, compile classes one by one.
        panda::Timer::timerStart(panda::EVENT_COMPILE_ABC_FILE, src_->fileName);
        prog = compiler.CompileAbcFile(src_->fileName, *options_);
        panda::Timer::timerEnd(panda::EVENT_COMPILE_ABC_FILE, src_->fileName);
    } else {
        // If input is an abc file, in merge-abc mode, compile each class parallelly.
        CompileAbcFileJobInParallel(compiler);
        return;
    }

    if (prog == nullptr) {
        return;
    }

    OptimizeAndCacheProgram(prog);
}

void CompileFileJob::CompileAbcFileJobInParallel(es2panda::Compiler &compiler)
{
    std::map<std::string, panda::es2panda::util::ProgramCache *> abcProgramsInfo {};

    panda::Timer::timerStart(panda::EVENT_COMPILE_ABC_FILE, src_->fileName);
    compiler.CompileAbcFileInParallel(src_, *options_, abcProgramsInfo, allocator_);
    panda::Timer::timerEnd(panda::EVENT_COMPILE_ABC_FILE, src_->fileName);

    panda::Timer::timerStart(panda::EVENT_UPDATE_ABC_PROG_CACHE, src_->fileName);
    auto outputCacheIter = options_->cacheFiles.find(src_->fileName);
    if (!options_->requireGlobalOptimization && outputCacheIter != options_->cacheFiles.end()) {
        auto *cache = allocator_->New<panda::es2panda::util::AbcProgramsCache>(src_->hash, abcProgramsInfo);
        CHECK_NOT_NULL(cache);
        panda::proto::ProtobufSnapshotGenerator::UpdateAbcCacheFile(cache, outputCacheIter->second);
    }
    InsertAbcCachePrograms(src_->hash, abcProgramsInfo);
    panda::Timer::timerEnd(panda::EVENT_UPDATE_ABC_PROG_CACHE, src_->fileName);
}

void CompileFileJob::OptimizeAndCacheProgram(panda::pandasm::Program *prog)
{
    bool requireOptimizationAfterAnalysis = false;
    // When cross-program optimizations are required, skip program-local optimization at this stage
    // and perform it later after the analysis of all programs has been completed
    if (src_->isSourceMode && options_->transformLib.empty()) {
        if (options_->requireGlobalOptimization) {
            panda::Timer::timerStart(panda::EVENT_OPTIMIZE_PROGRAM, src_->fileName);
            util::Helpers::AnalysisProgram(prog, src_->fileName);
            requireOptimizationAfterAnalysis = true;
        } else if (options_->optLevel != 0) {
            panda::Timer::timerStart(panda::EVENT_OPTIMIZE_PROGRAM, src_->fileName);
            util::Helpers::OptimizeProgram(prog, src_->fileName);
            panda::Timer::timerEnd(panda::EVENT_OPTIMIZE_PROGRAM, src_->fileName);
        }
    }

    {
        std::unique_lock<std::mutex> lock(globalMutex_);
        auto *cache = allocator_->New<util::ProgramCache>(src_->hash, std::move(*prog), src_->isSourceMode);
        progsInfo_.insert({src_->fileName, cache});
        if (requireOptimizationAfterAnalysis) {
            optimizationPendingProgs_.insert(src_->fileName);
        }
    }
}

void CompileAbcClassJob::Run()
{
    panda_file::File::EntityId recordId(classId_);
    auto *program = new panda::pandasm::Program();
    std::string record_name = "";
    compiler_.CompileAbcClass(recordId, *program, record_name);
    program->isGeneratedFromMergedAbc = true;

    if (!options_.modifiedPkgName.empty()) {
        UpdatePkgNameOfImportOhmurl(program, options_);
    }
    // Update ohmurl for abc input when needed
    if (options_.compileContextInfo.needModifyRecord ||
        (options_.updatePkgVersionForAbcInput && pkgVersionUpdateRequiredInAbc_)) {
        panda::Timer::timerStart(panda::EVENT_UPDATE_ABC_PKG_VERSION, record_name);
        UpdateImportOhmurl(program, options_);
        panda::Timer::timerEnd(panda::EVENT_UPDATE_ABC_PKG_VERSION, record_name);
        // Remove redundant strings created due to version replacement
        panda::Timer::timerStart(panda::EVENT_UPDATE_ABC_PROGRAM_STRING, record_name);
        if (options_.removeRedundantFile && hasOhmurlBeenChanged_) {
            program->strings.clear();
            for (const auto &[_, function] : program->function_table) {
                const auto &funcStringSet = function.CollectStringsFromFunctionInsns();
                program->strings.insert(funcStringSet.begin(), funcStringSet.end());
            }
        }
        panda::Timer::timerEnd(panda::EVENT_UPDATE_ABC_PROGRAM_STRING, record_name);
    }

    panda::Timer::timerStart(panda::EVENT_UPDATE_ABC_PROG_CACHE, record_name);
    {
        std::unique_lock<std::mutex> lock(CompileFileJob::globalMutex_);
        ASSERT(compiler_.GetAbcFile().GetFilename().find(util::CHAR_VERTICAL_LINE) == std::string::npos);
        ASSERT(program->record_table.size() == 1);
        ASSERT(util::RecordNotGeneratedFromBytecode(program->record_table.begin()->first));
        auto name = compiler_.GetAbcFile().GetFilename();
        name += util::CHAR_VERTICAL_LINE + program->record_table.begin()->first;
        auto *cache = allocator_->New<util::ProgramCache>(src_->hash, std::move(*program), true);
        progsInfo_.emplace(name, cache);
    }
    panda::Timer::timerEnd(panda::EVENT_UPDATE_ABC_PROG_CACHE, record_name);

    delete program;
    program = nullptr;
}

void CompileAbcClassJob::UpdateBundleNameOfOhmurl(std::string &ohmurl)
{
    const auto &newOhmurl = util::UpdateBundleNameIfNeeded(ohmurl, options_.compileContextInfo.bundleName,
                                                           options_.compileContextInfo.externalPkgNames);
    if (newOhmurl == ohmurl) {
        return;
    }
    SetOhmurlBeenChanged(true);
    ohmurl = newOhmurl;
}

void CompileAbcClassJob::UpdateDynamicImport(panda::pandasm::Program *prog,
    const std::map<std::string, panda::es2panda::PkgInfo> &pkgContextInfo)
{
    for (auto &[name, function] : prog->function_table) {
        util::VisitDyanmicImports<false>(function, [this, &prog, pkgContextInfo](std::string &ohmurl) {
            if (this->options_.compileContextInfo.needModifyRecord) {
                this->UpdateBundleNameOfOhmurl(ohmurl);
            }
            const auto &newOhmurl = util::UpdatePackageVersionIfNeeded(ohmurl, pkgContextInfo);
            if (newOhmurl == ohmurl) {
                return;
            }
            prog->strings.insert(newOhmurl);
            this->SetOhmurlBeenChanged(true);
            ohmurl = newOhmurl;
        });
    }
}

void CompileAbcClassJob::UpdateStaticImport(panda::pandasm::Program *prog,
    const std::map<std::string, panda::es2panda::PkgInfo> &pkgContextInfo)
{
    for (auto &[recordName, record] : prog->record_table) {
        util::VisitStaticImports<false>(*prog, record, [this, pkgContextInfo](std::string &ohmurl) {
            if (this->options_.compileContextInfo.needModifyRecord) {
                this->UpdateBundleNameOfOhmurl(ohmurl);
            }

            const auto &newOhmurl = util::UpdatePackageVersionIfNeeded(ohmurl, pkgContextInfo);
            if (newOhmurl == ohmurl) {
                return;
            }
            this->SetOhmurlBeenChanged(true);
            ohmurl = newOhmurl;
        });
    }
}

void CompileAbcClassJob::UpdateImportOhmurl(panda::pandasm::Program *prog,
                                            const panda::es2panda::CompilerOptions &options)
{
    bool isAccurateUpdateVersion = !options.compileContextInfo.updateVersionInfo.empty();
    const std::map<std::string, panda::es2panda::PkgInfo> &pkgContextInfo = isAccurateUpdateVersion ?
        options.compileContextInfo.updateVersionInfo.at(abcPkgName_) : options.compileContextInfo.pkgContextInfo;
    // Replace for esm module static import
    UpdateStaticImport(prog, pkgContextInfo);
    // Replace for dynamic import
    UpdateDynamicImport(prog, pkgContextInfo);
}
/**
 * Need to modify the package name of the original package to the package name of the target package when
 * you merging two packages.
 */
void CompileAbcClassJob::UpdatePkgNameOfImportOhmurl(panda::pandasm::Program *prog,
    const panda::es2panda::CompilerOptions &options)
{
    for (auto &[recordName, record] : prog->record_table) {
        util::VisitStaticImports<false>(*prog, record, [this, options](std::string &ohmurl) {
            const auto &newOhmurl = util::UpdatePackageNameIfNeeded(ohmurl, options.modifiedPkgName);
            if (newOhmurl == ohmurl) {
                return;
            }
            this->SetOhmurlBeenChanged(true);
            ohmurl = newOhmurl;
        });
    }
    for (auto &[name, function] : prog->function_table) {
        util::VisitDyanmicImports<false>(function, [this, options](std::string &ohmurl) {
            const auto &newOhmurl = util::UpdatePackageNameIfNeeded(ohmurl, options.modifiedPkgName);
            if (newOhmurl == ohmurl) {
                return;
            }
            this->SetOhmurlBeenChanged(true);
            ohmurl = newOhmurl;
        });
    }
    if (hasOhmurlBeenChanged_) {
        prog->strings.clear();
        for (const auto &[_, function] : prog->function_table) {
            const auto &funcStringSet = function.CollectStringsFromFunctionInsns();
            prog->strings.insert(funcStringSet.begin(), funcStringSet.end());
        }
    }
}

void PostAnalysisOptimizeFileJob::Run()
{
    util::Helpers::OptimizeProgram(program_, fileName_);
    panda::Timer::timerEnd(panda::EVENT_OPTIMIZE_PROGRAM, fileName_);
}

void CompileFuncQueue::Schedule()
{
    ASSERT(jobsCount_ == 0);
    std::unique_lock<std::mutex> lock(m_);
    const auto &functions = context_->Binder()->Functions();

    for (auto *function : functions) {
        auto *funcJob = new CompileFunctionJob(context_);
        funcJob->SetFunctionScope(function);
        jobs_.push_back(funcJob);
        jobsCount_++;
    }

    if (context_->Binder()->Program()->Kind() == parser::ScriptKind::MODULE) {
        auto *moduleRecordJob = new CompileModuleRecordJob(context_);
        jobs_.push_back(moduleRecordJob);
        jobsCount_++;
    }

    lock.unlock();
    jobsAvailable_.notify_all();
}

void CompileFileQueue::Schedule()
{
    ASSERT(jobsCount_ == 0);
    std::unique_lock<std::mutex> lock(m_);

    for (auto &input: options_->sourceFiles) {
        auto *fileJob = new CompileFileJob(&input, options_, progsInfo_, optimizationPendingProgs_,
                                           symbolTable_, allocator_);
        jobs_.push_back(fileJob);
        jobsCount_++;
    }

    lock.unlock();
    jobsAvailable_.notify_all();
}

bool CompileAbcClassQueue::NeedUpdateVersion()
{
    std::unordered_map<std::string, std::map<std::string, panda::es2panda::PkgInfo>> updateVersionInfo =
        options_.compileContextInfo.updateVersionInfo;
    auto iter = updateVersionInfo.find(src_->pkgName);
    return updateVersionInfo.empty() || (iter != updateVersionInfo.end() && !iter->second.empty());
}

void CompileAbcClassQueue::Schedule()
{
    std::unique_lock<std::mutex> lock(m_);

    auto classIds = compiler_.GetAbcFile().GetClasses();
    bool needUpdateVersion = NeedUpdateVersion();
    for (size_t i = 0; i != classIds.size(); ++i) {
        if (!compiler_.CheckClassId(classIds[i], i)) {
            continue;
        }

        auto *abcClassJob = new CompileAbcClassJob(src_, classIds[i], options_, compiler_, progsInfo_, allocator_,
                                                   src_->pkgName, needUpdateVersion);

        jobs_.push_back(abcClassJob);
        jobsCount_++;
    }

    lock.unlock();
    jobsAvailable_.notify_all();
}

void PostAnalysisOptimizeFileQueue::Schedule()
{
    ASSERT(jobsCount_ == 0);
    std::unique_lock<std::mutex> lock(m_);

    for (const auto &optimizationPendingProgName : optimizationPendingProgs_) {
        auto progInfo = progsInfo_.find(optimizationPendingProgName);
        if (progInfo == progsInfo_.end()) {
            continue;
        }
        auto *optimizeJob = new PostAnalysisOptimizeFileJob(progInfo->first, &progInfo->second->program);
        jobs_.push_back(optimizeJob);
        jobsCount_++;
    }
}

}  // namespace panda::es2panda::compiler
