/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "hiperf_libreport.h"

#include "debug_logger.h"
#include "elf_factory.h"
#include "perf_file_reader.h"
#include "subcommand_dump.h"
#include "subcommand_report.h"
#include "utilities.h"

using namespace OHOS::Developtools::HiPerf;
extern "C" {
// this is a demo function
const char *EchoLoopBack(const char *echo)
{
    HLOGD("EchoLoopBack:%s\n", echo);
    return echo;
}

int SetDebug(bool enable)
{
#ifdef HIPERF_DEBUG
    if (enable) {
        DebugLogger::GetInstance()->SetLogLevel(LEVEL_VERBOSE);
        DebugLogger::GetInstance()->Disable(false);
    } else {
        DebugLogger::GetInstance()->Disable(true);
    }
#endif
    return 0;
}

int Report(const char *perfFile, const char *reportFile, const char *reportOptions)
{
    std::unique_ptr<SubCommandReport> report = std::make_unique<SubCommandReport>();
    HLOGD("report the file %s to %s\n", perfFile, reportFile);
    if (perfFile != nullptr && reportFile != nullptr) {
        std::vector<std::string> args;
        args.emplace_back("-i");
        args.emplace_back(perfFile);
        args.emplace_back("-o");
        args.emplace_back(reportFile);
        if (reportOptions != nullptr) {
            std::vector<std::string> options = StringSplit(reportOptions);
            for (std::string &option : options) {
                args.emplace_back(option);
            }
        }
        if (report->ParseOption(args)) {
            return report->OnSubCommand(args) == HiperfError::NO_ERR ? 0 : -1;
        }
    } else {
        printf("path is nullptr\n");
    }
    return -1;
}

int ReportJson(const char *perfFile, const char *reportFile)
{
    return ReportUnwindJson(perfFile, reportFile, nullptr);
}

int ReportUnwindJson(const char *perfFile, const char *reportFile, const char *symbolsDir)
{
    std::unique_ptr<SubCommandReport> report = std::make_unique<SubCommandReport>();
    HLOGD("report the file %s to json file %s symbols from %s\n", perfFile, reportFile, symbolsDir);
    if (perfFile != nullptr && reportFile != nullptr) {
        std::vector<std::string> args;
        args.emplace_back("-i");
        args.emplace_back(perfFile);
        args.emplace_back("-o");
        args.emplace_back(reportFile);
        args.emplace_back("--json");
        if (symbolsDir != nullptr) {
            args.emplace_back("--symbol-dir");
            args.emplace_back(symbolsDir);
        }
        if (report->ParseOption(args)) {
            return report->OnSubCommand(args) == HiperfError::NO_ERR ? 0 : -1;
        }
    }
    return -1;
}

static std::unique_ptr<PerfFileReader> GetReader(const std::string &fileName)
{
    // check if file exist
    if (access(fileName.c_str(), F_OK) != 0) {
        // file not exists
        printf("Can not access data file %s\n", fileName.c_str());
        return nullptr;
    }

    auto reader = PerfFileReader::Instance(fileName);
    if (reader == nullptr) {
        printf("%s format not correct\n", fileName.c_str());
        return nullptr;
    } else {
        return reader;
    }
}

const char *ReportGetSymbolFiles(const char *perfFile)
{
    HLOGD("report the file %s for symbols \n", perfFile);
    static std::string result; // static for hold the c_str buffer
    result.clear();
    if (perfFile == nullptr) {
        HLOGW("perfFile is nullptr.");
        return result.c_str();
    }

    auto reader = GetReader(perfFile);
    if (reader == nullptr) {
        HLOGW("reader is nullptr.");
        return result.c_str();
    }
    // found symbols in file
    reader->ReadFeatureSection();
    for (auto &featureSection : reader->GetFeatureSections()) {
        if (featureSection->featureId_ == FEATURE::HIPERF_FILES_SYMBOL) {
            const PerfFileSectionSymbolsFiles *sectionSymbolsFiles =
                static_cast<const PerfFileSectionSymbolsFiles *>(featureSection.get());
            auto it = sectionSymbolsFiles->symbolFileStructs_.begin();
            while (it != sectionSymbolsFiles->symbolFileStructs_.end()) {
                HLOGD("%s buildId:%s\n", it->filePath_.c_str(), it->buildId_.c_str());
                result.append("[");
                result.append(it->filePath_.c_str());
                result.append(",");
                result.append(it->buildId_.c_str());
                result.append("]");
                result.append(",");
                it++;
            }
            result[result.size() >= 1 ? result.size() - 1 : 0] = '\0';
        }
    }
    return result.c_str();
}

const char *ReportGetBuildId(const char *elfPath)
{
    static std::string buildId; // static for hold the c_str buffer
    buildId.clear();
    std::string path(elfPath);
    RegularElfFactory elfFactory(path);
    std::shared_ptr<DfxElf> elfFile = elfFactory.Create();
    buildId = elfFile->GetBuildId();
    return buildId.c_str();
}

const char *ReportGetElfArch(const char *elfPath)
{
    RegularElfFactory elfFactory(elfPath);
    std::shared_ptr<DfxElf> elfFile = elfFactory.Create();
    const char *machineName = "unknown";
    switch (elfFile->GetArchType()) {
        case ArchType::ARCH_ARM:
            machineName = "arm";
            break;
        case ArchType::ARCH_ARM64:
            machineName = "arm64";
            break;
        case ArchType::ARCH_X86:
            machineName = "x86";
            break;
        case ArchType::ARCH_X86_64:
            machineName = "x86_64";
            break;
        default:
            break;
    }
    HLOGD("elf '%s' mache type is %s \n", elfPath, machineName);
    return machineName;
}

int Dump(const char *fileName)
{
    std::unique_ptr<SubCommandDump> dump = std::make_unique<SubCommandDump>();
    HLOGD("dump the file %s\n", fileName);
    if (fileName != nullptr) {
        std::vector<std::string> args;
        args.emplace_back(fileName);
        if (dump->ParseOption(args)) {
            return dump->OnSubCommand(args) == HiperfError::NO_ERR ? 0 : -1;
        }
    }
    return -1;
}

} // extern "C"
