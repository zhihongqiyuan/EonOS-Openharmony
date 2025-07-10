/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#ifndef SUBCOMMAND_REPORT_H
#define SUBCOMMAND_REPORT_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <tuple>
#include <linux/perf_event.h>

#include "perf_file_reader.h"
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
#include "report_protobuf_file.h"
#endif
#include "debug_logger.h"
#include "option.h"
#include "perf_event_record.h"
#include "report.h"
#include "report_json_file.h"
#include "subcommand.h"
#include "symbols_file.h"
#include "utilities.h"
#include "virtual_runtime.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
class SubCommandReport : public SubCommand {
public:
    SubCommandReport()
        // clang-format off
        : SubCommand("report", "report sampling information from perf.data format file",
        "Usage: hiperf report [options]\n"
        "       The default options are same as :\n"
        "       -i perf.data --sort comm,pid,tid,dso,func\n"
        "   --symbol-dir <dir>\n"
        "       use symbols path to find symbols.\n"
        "       separate the paths with commas.\n"
        "   --limit-percent <number>\n"
        "       only show heat percent limit content.\n"
        "   -s / --call-stack\n"
        "       show the unwind callstack\n"
        "   --call-stack-limit-percent <number>\n"
        "       only show the callstack heat percent limit content.\n"
        "   --proto\n"
        "       show protobuf content in the save file name.\n"
        "       default file name is perf.data.\n"
        "   --json\n"
        "       report in json format.\n"
        "       default file name is perf.data.\n"
        "   --diff <target file>\n"
        "       show the diff result from -i to -diff .\n"
        "       example: \"report -i a.data --diff b.data\"\n"
        "   --branch\n"
        "       show the branch from address instead of ip address\n"
        "   --<keys> <keyname1>[,keyname2][,...]\n"
        "       select able keys: comms,pids,tids,dsos,funcs,from_dsos,from_funcs\n"
        "           example: --comms hiperf\n"
        "   --sort <key1>[,key2][,...]\n"
        "       Choose some keywords.\n"
        "       These keywords will be used for sorting.\n"
        "       Only selected keywords will appear in the report.\n"
        "\n"
        "           pid             -- process id\n"
        "           tid             -- thread id\n"
        "           comm            -- thread name (can be changed for same thread)\n"
        "           dso             -- like dso name , or elf path\n"
        "           func            -- function name in the symbols\n"
        "\n"
        "       in branch mode:\n"
        "           from_dso        -- branched from dso or elf\n"
        "           from_func       -- branched from function\n"
        "           dso             -- means branched to dso or elf\n"
        "           func            -- means branched to function\n"
        "   -i <filename>\n"
        "           perf data file to report, default is perf.data\n"
        "   -o <filename>\n"
        "           report file name. if empty will use stdout print\n"
        "   --hide_count\n"
        "           will not show count in report\n"
        "   --dumpoptions\n"
        "           Dump command options.\n"
        "\n"
        ), recordFile_ {"perf.data", ""} // default file path is perf.data
    // clang-format on
    {
    }
    HiperfError OnSubCommand(std::vector<std::string>& args) override;
    bool ParseOption(std::vector<std::string> &args) override;
    void DumpOptions(void) const override;
    static bool RegisterSubCommandReport(void);
    bool RecordCallBack(PerfEventRecord& record);

    ~SubCommandReport();

    static SubCommand& GetInstance();
private:
    void ProcessSample(std::unique_ptr<PerfRecordSample> &);
    void BroadcastSample(std::unique_ptr<PerfRecordSample> &);

    bool VerifyOption();
    bool VerifyDisplayOption();

    // load
    bool PrepareOutput();
    bool LoadPerfData();
    void ProcessFeaturesData();
    void LoadEventConfigData();
    void LoadAttrSection();
    void LoadEventDesc();
    void ProcessSymbolsData();
    void LoadPerfDataCompleted();
    void ProcessUniStackTableData();

    bool OutputReport();
    bool OutputStd();

    // prefdata
    bool showCallStack_ = false;
    bool diffMode_ = false;

    // create record file reader pointer
    std::unique_ptr<PerfFileReader> recordFileReader_;
    std::vector<std::string> symbolsPaths_;

    // report file name , if empty will use stdout
    std::string reportFile_;

    // for diff report
    enum RecordIndex { FIRST = 0, SECOND = 1, MAX = 2, CURRENT = -1 } index_ = FIRST;
    std::string recordFile_[MAX];
    ReportOption reportOption_;
    Report report_[MAX] = {reportOption_, reportOption_};
    inline Report &GetReport(RecordIndex index = CURRENT, int configId = 0)
    {
        if (index == CURRENT) {
            return report_[index_];
        } else {
            return report_[index];
        }
    }

    std::vector<std::string> configNames_;
    std::set<uint64_t> cpuOffids_;

    const std::string cpuOffEventName = "sched:sched_switch";
    bool cpuOffMode_ = false;
    std::map<pid_t, std::unique_ptr<PerfRecordSample>> prevSampleCache_;
    void FlushCacheRecord();

    // in debug mode we will output some more debug info
    bool debug_ = false;
    bool branch_ = false;
    bool jsonFormat_ = false;

    FILE *output_ = nullptr;

    std::unique_ptr<ReportJsonFile> reportJsonFile_ = nullptr;

    bool protobufFormat_ = false;
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
    std::unique_ptr<ReportProtobufFileWriter> protobufOutputFileWriter_ = nullptr;
    void UpdateReportInfo();
#endif
    void SetHM();
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // SUBCOMMAND_REPORT_H
