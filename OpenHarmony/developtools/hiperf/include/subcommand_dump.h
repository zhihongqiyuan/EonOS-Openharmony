/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#ifndef SUBCOMMAND_DUMP_H
#define SUBCOMMAND_DUMP_H

#include "perf_file_reader.h"

#include <memory>

#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
#include "report_protobuf_file.h"
#endif
#include "subcommand.h"
#include "symbols_file.h"
#include "virtual_runtime.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {

class SubCommandDump : public SubCommand {
public:
    SubCommandDump()
        // clang-format off
        : SubCommand("dump", "Dump content of a perf data file, like perf.data",

        "Usage: hiperf dump [option] <filename>\n"
        "       Dump specific parts of specified file .\n"
        "   --head\n"
        "       Dump header and attrs only.\n"
        "   -d\n"
        "       Dump data section only.\n"
        "   -f\n"
        "       Dump addtional features only.\n"
        "   --sympath <symbols path>\n"
        "       use symbols path to find symbols.\n"
        "   -i <file name>\n"
        "       perf data file to dump, default is perf.data\n"
        "   --elf <elf file name>\n"
        "       dump elf not perf data.\n"
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
        "   --proto <protobuf file name>\n"
        "       dump perf data from protobuf file.\n"
#endif
        "   --export <sample index>\n"
        "       also export the user stack data to some split file,\n"
        "       use this command to produce ut data.\n"
        "       named with sample index(0 base):\n"
        "           hiperf_<pid>_<tid>_user_regs_<index>.dump\n"
        "           hiperf_<pid>_<tid>_user_data_<index>.dump\n"
        "   -o <filename>\n"
        "       dump file name. if empty will use stdout print\n"
        )
    // clang-format on
    {
    }
    ~SubCommandDump() override;

    HiperfError OnSubCommand(std::vector<std::string>& args) override;
    bool ParseOption(std::vector<std::string> &args) override;

    static bool RegisterSubCommandDump(void);

    static void DumpPrintEventAttr(const perf_event_attr &attr, int indent = 0);
    std::unique_ptr<PerfFileReader> reader_;

    static SubCommand& GetInstance();

private:
    static void DumpSampleType(uint64_t sampleType, int indent);
    bool PrepareDumpOutput();
    int exportSampleIndex_ = -1;
    int currectSampleIndex_ = 0;
    std::string dumpFileName_;
    std::string elfFileName_;
    std::string outputFilename_ = "";
    std::string protobufDumpFileName_;
    int indent_ = 0;
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
    std::unique_ptr<ReportProtobufFileReader> protobufInputFileReader_ = nullptr;
#endif

    std::vector<AttrWithId> attrIds_;

    bool dumpHeader_ = false;
    bool dumpFeatures_ = false;
    bool dumpData_ = false;
    bool dumpAll_ = true;

    std::vector<std::string> dumpSymbolsPaths_;

    bool CheckInputFile();
    bool DumpElfFile();
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
    bool DumpProtoFile();
#endif
    void DumpPrintFileHeader(int indent = 0);
    void DumpAttrPortion(int indent = 0);
    void DumpDataPortion(int indent = 0);
    void DumpCallChain(int indent, const PerfRecordSample& sample);
    void DumpFeaturePortion(int indent = 0);
    void DumpUniqueStackTableNode(int indent, const PerfFileSectionUniStackTable &uniStackTable);
    void ExportUserData(PerfEventRecord& record);
    void ExportUserStack(const PerfRecordSample &recordSample);
    void PrintHeaderInfo(const int &indent);
    void PrintSymbolFile(const int &indent, const SymbolFileStruct &symbolFileStruct);
    void PrintFeatureEventdesc(int indent, const PerfFileSectionEventDesc &sectionEventdesc);
    void DumpSpeReport();
    bool CheckOptions();
    bool CheckOutputFile();
    VirtualRuntime vr_;

    void SetHM();
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // SUBCOMMAND_DUMP_H
