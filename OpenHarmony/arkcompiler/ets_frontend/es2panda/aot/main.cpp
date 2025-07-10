/**
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

#include <abc2program/program_dump.h>
#include <mem/pool_manager.h>
#include "utils/timers.h"

#include <emitFiles.h>
#include <protobufSnapshotGenerator.h>
#include <resolveDepsRelation.h>
#include <util/commonUtil.h>
#include <util/dumper.h>
#include <util/moduleHelpers.h>

namespace panda::es2panda::aot {
using mem::MemConfig;
class MemManager {
public:
    explicit MemManager()
    {
        constexpr auto COMPILER_SIZE = 8192_MB;

        MemConfig::Initialize(0, 0, COMPILER_SIZE, 0);
        PoolManager::Initialize(PoolType::MMAP);
    }

    NO_COPY_SEMANTIC(MemManager);
    NO_MOVE_SEMANTIC(MemManager);

    ~MemManager()
    {
        PoolManager::Finalize();
        MemConfig::Finalize();
    }
};

static void GenerateBase64Output(panda::pandasm::Program *prog,
                                 const std::unique_ptr<panda::es2panda::aot::Options> &options)
{
    auto pandaFile = panda::pandasm::AsmEmitter::Emit(*prog, nullptr, options->CompilerOptions().targetApiVersion,
        options->CompilerOptions().targetApiSubVersion);
    const uint8_t *buffer = pandaFile->GetBase();
    size_t size = pandaFile->GetPtr().GetSize();
    std::string content(reinterpret_cast<const char*>(buffer), size);
    std::string base64Output = util::Base64Encode(content);
    std::cout << base64Output << std::endl;
}

static void DumpPandaFileSizeStatistic(std::map<std::string, size_t> &stat)
{
    size_t totalSize = 0;
    std::cout << "Panda file size statistic:" << std::endl;
    constexpr std::array<std::string_view, 2> INFO_STATS = {"instructions_number", "codesize"};

    for (const auto &[name, size] : stat) {
        if (find(INFO_STATS.begin(), INFO_STATS.end(), name) != INFO_STATS.end()) {
            continue;
        }
        std::cout << name << " section: " << size << std::endl;
        totalSize += size;
    }

    for (const auto &name : INFO_STATS) {
        std::cout << name << ": " << stat.at(std::string(name)) << std::endl;
    }

    std::cout << "total: " << totalSize << std::endl;
}

static void DumpPandaFileSizePctStatistic(std::map<std::string, size_t> &stat)
{
    size_t totalSize = 0;
    std::cout << "Panda file size statistic:" << std::endl;
    constexpr std::array<std::string_view, 2> INFO_STATS = {"instructions_number", "codesize"};

    for (const auto &[name, size] : stat) {
        if (find(INFO_STATS.begin(), INFO_STATS.end(), name) != INFO_STATS.end()) {
            continue;
        }
        totalSize += size;
    }

    const int itemWidth = 32;   // The width of each item name is 32 bit. (item name is a data structure in abc file)
    const int sectionWidth = 8; // The width of each section size is 8 bit. (section size is the size of each item)
    const int precision = 2;    // Control the output precision of the size percentage.
    const double percentFlag = 100.0f;  // Conversion of percentage output.
    for (const auto &[name, size] : stat) {
        if (find(INFO_STATS.begin(), INFO_STATS.end(), name) != INFO_STATS.end()) {
            continue;
        }
        std::cout << std::left << std::setw(itemWidth) << name << " section: " << \
        std::setw(sectionWidth) << size << ", percent: " << \
        std::fixed << std::setprecision(precision) << (size * percentFlag / totalSize) << "%" << std::endl;
    }

    std::cout << "total: " << totalSize << std::endl;
}

static bool GenerateProgramsByWorkers(const std::map<std::string, panda::es2panda::util::ProgramCache*> &programsInfo,
    const std::unique_ptr<panda::es2panda::aot::Options> &options, std::map<std::string, size_t> *statp)
{
    auto queue = new panda::es2panda::aot::EmitFileQueue(options, statp, programsInfo);

    bool emitResult = true;
    try {
        queue->Schedule();
        queue->Consume();
        queue->Wait();
    } catch (const class Error &e) {
        emitResult = false;
        std::cerr << e.Message() << std::endl;
    }

    delete queue;
    queue = nullptr;

    return emitResult;
}

static void DumpProgramInfos(const std::map<std::string, panda::es2panda::util::ProgramCache*> &programsInfo,
    const std::unique_ptr<panda::es2panda::aot::Options> &options)
{
    const es2panda::CompilerOptions &compilerOptions = options->CompilerOptions();
    if (compilerOptions.dumpAsm || compilerOptions.dumpLiteralBuffer || compilerOptions.dumpAsmProgram ||
        compilerOptions.dumpNormalizedAsmProgram) {
        for (const auto &progInfo : programsInfo) {
            if (compilerOptions.dumpAsmProgram || compilerOptions.dumpNormalizedAsmProgram) {
                panda::abc2program::PandasmProgramDumper dumper(compilerOptions.dumpNormalizedAsmProgram,
                                                                compilerOptions.isDebug);
                dumper.Dump(std::cout, progInfo.second->program);
            }

            if (compilerOptions.dumpAsm) {
                es2panda::Compiler::DumpAsm(&(progInfo.second->program));
            }

            if (compilerOptions.dumpLiteralBuffer) {
                panda::es2panda::util::Dumper::DumpLiterals(progInfo.second->program.literalarray_table);
            }

            if (compilerOptions.dumpString) {
                panda::es2panda::util::Dumper::DumpStrings(progInfo.second->program.strings);
            }
        }
    }
}

static bool GenerateProgram(std::map<std::string, panda::es2panda::util::ProgramCache*> &programsInfo,
                            const std::unique_ptr<panda::es2panda::aot::Options> &options,
                            const std::map<std::string, std::unordered_set<std::string>> &resolvedDepsRelation)
{
    DumpProgramInfos(programsInfo, options);

    if (programsInfo.size() == 1) {
        auto *prog = &(programsInfo.begin()->second->program);
        if (options->OutputFiles().empty() && options->CompilerOutput().empty()) {
            GenerateBase64Output(prog, options);
            return true;
        }

        // Disable generating cached files when cross-program optimization is required, to prevent cached files from
        // not being invalidated when their dependencies are changed
        if (options->CompilerProtoOutput().size() > 0 && !options->CompilerOptions().requireGlobalOptimization) {
            panda::proto::ProtobufSnapshotGenerator::GenerateSnapshot(*prog, options->CompilerProtoOutput());
            return true;
        }
    }

    if (options->NeedRemoveRedundantRecord()) {
        util::Helpers::RemoveProgramsRedundantData(programsInfo, resolvedDepsRelation);
        DumpProgramInfos(programsInfo, options);
    }

    bool dumpSize = options->SizeStat();
    bool dumpSizePct = options->SizePctStat();
    std::map<std::string, size_t> stat;
    std::map<std::string, size_t> *statp = (dumpSize || dumpSizePct) ? &stat : nullptr;

    if (!GenerateProgramsByWorkers(programsInfo, options, statp)) {
        return false;
    }

    if (dumpSize) {
        DumpPandaFileSizeStatistic(stat);
    }

    if (dumpSizePct) {
        DumpPandaFileSizePctStatistic(stat);
    }

    return true;
}

static bool CheckMergeModeConsistency(bool mergeAbc,
                                      std::map<std::string, panda::es2panda::util::ProgramCache*> programsInfo)
{
    std::unordered_map<std::string, std::unordered_set<std::string>> recordNameMap;

    // Names of these program records generated from abc input all follow such format: abcName|recordName
    for (auto &[name, _] : programsInfo) {
        if (util::RecordNotGeneratedFromBytecode(name)) {
            continue;
        }

        auto nameVec = util::Split(name, util::CHAR_VERTICAL_LINE);
        auto abcFileName = nameVec[0];
        auto recordName = nameVec.back();

        if (mergeAbc) {
            if (recordName == util::GLOBAL_TYPE_NAME) {
                std::cerr << "Current compile mode is merge-abc, all input abc files must be merged mode. "
                          << "But file '" << abcFileName << "' is not a merged abc." << std::endl;
                return false;
            }
        } else {
            if (recordNameMap.find(abcFileName) != recordNameMap.end()) {
                recordNameMap.find(abcFileName)->second.insert(recordName);
            } else {
                recordNameMap.insert({abcFileName, {recordName}});
            }
        }
    }

    if (!mergeAbc) {
        for (auto &[abcFileName, recordNameSet] : recordNameMap) {
            if (!recordNameSet.count(util::GLOBAL_TYPE_NAME)) {
                std::cerr << "Current compile mode is non merge-abc, all input abc files must be unmerged mode. "
                          << "But file '" << abcFileName << "' is a merged abc." << std::endl;
                return false;
            }
        }
    }

    return true;
}

static bool GenerateAbcFiles(std::map<std::string, panda::es2panda::util::ProgramCache*> &programsInfo,
                             const std::unique_ptr<panda::es2panda::aot::Options> &options, size_t expectedProgsCount,
                             const std::map<std::string, std::unordered_set<std::string>> &resolvedDepsRelation)
{
    if (programsInfo.size() != expectedProgsCount) {
        std::cerr << "The size of programs is expected to be " << expectedProgsCount
                  << ", but is " << programsInfo.size() << std::endl;
        return false;
    }

    if (!GenerateProgram(programsInfo, options, resolvedDepsRelation)) {
        std::cerr << "GenerateProgram Failed!" << std::endl;
        return false;
    }

    return true;
}

static bool ResolveDepsRelations(const std::map<std::string, panda::es2panda::util::ProgramCache *> &programsInfo,
                                 const std::unique_ptr<panda::es2panda::aot::Options> &options,
                                 std::map<std::string, std::unordered_set<std::string>> &resolvedDepsRelation)
{
    panda::es2panda::aot::DepsRelationResolver depsRelationResolver(programsInfo, options, resolvedDepsRelation);
    return depsRelationResolver.Resolve();
}

static bool ResolveAndGenerate(std::map<std::string, panda::es2panda::util::ProgramCache*> &programsInfo,
                               const std::unique_ptr<panda::es2panda::aot::Options> &options)
{
    panda::Timer::timerStart(panda::EVENT_RESOLVE_DEPS, "");
    // A mapping of program to its records which are resolved and collected as valid dependencies.
    std::map<std::string, std::unordered_set<std::string>> resolvedDepsRelation {};

    if (options->NeedCollectDepsRelation() &&
        !ResolveDepsRelations(programsInfo, options, resolvedDepsRelation)) {
        return true;
    }
    panda::Timer::timerEnd(panda::EVENT_RESOLVE_DEPS, "");

    panda::Timer::timerStart(panda::EVENT_EMIT_ABC, "");
    if (!GenerateAbcFiles(programsInfo, options, Compiler::GetExpectedProgsCount(), resolvedDepsRelation)) {
        return true;
    }
    panda::Timer::timerEnd(panda::EVENT_EMIT_ABC, "");

    return false;
}

int Run(int argc, const char **argv)
{
    auto options = std::make_unique<Options>();
    if (!options->Parse(argc, argv)) {
        std::cerr << options->ErrorMsg() << std::endl;
        return 1;
    }

    if (options->CompilerOptions().targetBcVersion) {
        auto bcVersionByApi = panda::panda_file::GetVersionByApi(options->CompilerOptions().targetApiVersion,
            options->CompilerOptions().targetApiSubVersion);
        std::cout << panda::panda_file::GetVersion(bcVersionByApi.value());
        return 0;
    }

    panda::Timer::timerStart(panda::EVENT_TOTAL, "");
    if (options->CompilerOptions().bcVersion || options->CompilerOptions().bcMinVersion) {
        std::string version = options->CompilerOptions().bcVersion ?
            panda::panda_file::GetVersion(panda::panda_file::version) :
            panda::panda_file::GetVersion(panda::panda_file::minVersion);
        std::cout << version;
        return 0;
    }

    std::map<std::string, panda::es2panda::util::ProgramCache*> programsInfo;
    panda::ArenaAllocator allocator(panda::SpaceType::SPACE_TYPE_COMPILER, nullptr, true);

    Compiler::SetExpectedProgsCount(options->CompilerOptions().sourceFiles.size());
    panda::Timer::timerStart(panda::EVENT_COMPILE, "");
    int ret = Compiler::CompileFiles(options->CompilerOptions(), programsInfo, &allocator);

    if (!CheckMergeModeConsistency(options->CompilerOptions().mergeAbc, programsInfo)) {
        return 1;
    }

    if (options->ParseOnly()) {
        return ret;
    }

    if (!options->NpmModuleEntryList().empty()) {
        es2panda::util::ModuleHelpers::CompileNpmModuleEntryList(options->NpmModuleEntryList(),
            options->CompilerOptions(), programsInfo, &allocator);
        Compiler::SetExpectedProgsCount(Compiler::GetExpectedProgsCount() + 1);
    }
    panda::Timer::timerEnd(panda::EVENT_COMPILE, "");

    if (ResolveAndGenerate(programsInfo, options)) {
        return 1;
    }
    panda::Timer::timerEnd(panda::EVENT_TOTAL, "");
    if (!options->PerfFile().empty()) {
        panda::Timer::PrintTimers();
    }
    return 0;
}
}  // namespace panda::es2panda::aot

int main(int argc, const char **argv)
{
    panda::es2panda::aot::MemManager mm;
    return panda::es2panda::aot::Run(argc, argv);
}
