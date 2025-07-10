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
#ifndef SUBCOMMAND_STAT_H_
#define SUBCOMMAND_STAT_H_

#include "option.h"
#include "perf_events.h"
#include "perf_pipe.h"
#include "subcommand.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
class SubCommandStat : public SubCommand {
public:
    static constexpr int DEFAULT_CHECK_APP_MS = 10;
    static constexpr int MIN_CHECK_APP_MS = 1;
    static constexpr int MAX_CHECK_APP_MS = 200;
    ~SubCommandStat();
    SubCommandStat()
        : SubCommand("stat", "Collect performance counter information",
                     // clang-format off
        "Usage: hiperf stat [options] [command [command-args]]\n"
        "       Collect performance counter information of running [command].\n"
        "       The default options are: -c -1 -d 10000.0\n"
        "   -a\n"
        "         Collect system-wide information.\n"
        "         for measures all processes/threads\n"
        "         This requires CAP_PERFMON (since Linux 5.8) or\n"
        "          CAP_SYS_ADMIN capability or a\n"
        "         /proc/sys/kernel/perf_event_paranoid value of less than 1.\n"
        "   -c <cpuid>[<,cpuid>]\n"
        "         cpuid should be 0,1,2...\n"
        "         Limit the CPU that collects data.\n"
        "         0 means cpu0, 1 means cpu1 ...\n"
        "   -d <sec>\n"
        "         stop in <sec> seconds.\n"
        "         floating point number.\n"
        "         default is 10000.0\n"
        "   -i <ms>\n"
        "         print stat info every <ms>.\n"
        "   -e event1[:<u|k>][,event1[:<u|k>]]...\n"
        "         Customize the name of the event that needs to be counted.\n"
        "         The name can use the names listed in the list parameter.\n"
        "         It can also be represented by the value of 0x<hex>.\n"
        "            u - monitor user space events only\n"
        "            k - monitor kernel space events only\n"
        "   -g <event1[:<u|k>]>[,event1[:<u|k>]]...\n"
        "         The grouping function is added on the basis of the function of the -e parameter\n"
        "         PMU is required to report data in designated groups\n"
        "         limited by HW capability, too many events cannot be reported in the same sampling)\n"
        "   --no-inherit\n"
        "         Don't track new processes/threads.\n"
        "   -p <pid1>[,pid2]...\n"
        "         Limit the process id of the collection target. Conflicts with the -a option.\n"
        "   -t <tid1>[,tid2]...\n"
        "         Limit the thread id of the collection target. Conflicts with the -a option.\n"
        "   --app <package_name>\n"
        "         Collect profile info for an OHOS app, the app must be debuggable.\n"
        "         Record will exit if the process is not started within 10 seconds.\n"
        "   --chkms <millisec>\n"
        "         Set the interval of querying the <package_name>.\n"
        "         <millisec> is in range [1-200], default is 10.\n"
        "   --per-core\n"
        "         Print counters for each cpu core.\n"
        "   --per-thread\n"
        "         Print counters for each thread.\n"
        "   --restart\n"
        "         Collect performance counter information of application startup.\n"
        "         Record will exit if the process is not started within 30 seconds.\n"
        "   --verbose\n"
        "         Show more detailed reports.\n"
        "   --dumpoptions\n"
        "         Dump command options.\n"
        "   --control <command>\n"
        "         Control counting by <command>, the <command> can be:\n"
        "           prepare: set arguments and prepare counting\n"
        "           start: start counting\n"
        "           stop: stop counting\n"
        "   -o <output_file_name>\n"
        "         Set output file name, default is /data/local/tmp/perf_stat.txt.\n"
        "         Only restrain using with --control prepare.\n"
                     // clang-format on
                     ),
          targetSystemWide_(false)
    {
    }
    HiperfError OnSubCommand(std::vector<std::string>& args) override;
    bool ParseOption(std::vector<std::string> &args) override;
    bool ParseSpecialOption(std::vector<std::string> &args);
    void DumpOptions(void) const override;

    // add args for hisysevent
    void AddReportArgs(CommandReporter& reporter) override;

    static SubCommand& GetInstance();

private:
    PerfEvents perfEvents_;
    PerfPipe perfPipe_;
    bool targetSystemWide_ {false};
    std::vector<int> selectCpus_ = {};
    float timeStopSec_ = PerfEvents::DEFAULT_TIMEOUT;
    int timeReportMs_ {0};
    std::vector<std::vector<std::string>> selectEvents_;
    std::vector<std::vector<std::string>> selectGroups_;
    bool restart_ {false};
    bool noCreateNew_ {false};
    std::string appPackage_ = {};
    std::string outputFilename_ = "";
    std::string fifoFileC2S_;
    std::string fifoFileS2C_;
    int checkAppMs_ = DEFAULT_CHECK_APP_MS;
    std::vector<pid_t> selectPids_;
    std::vector<pid_t> selectTids_;
    std::vector<pid_t> inputPidTidArgs_ = {};
    bool perCpus_ {false};
    bool perThreads_ {false};
    bool verboseReport_ {false};
    std::vector<std::string> trackedCommand_ {};
    bool helpOption_ {false};
    bool CheckOptionPidAndApp(std::vector<pid_t> pids);
    bool CheckOptionPid(std::vector<pid_t> pids);
    bool CheckOutPutFile();
    static bool FindEventCount(
        const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents,
        const std::string &configName, const __u64 group_id, __u64 &eventcount, double &scale);
    static void GetComments(
        const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents,
        std::map<std::string, std::string> &comments);
    static bool FindRunningTime(
        const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents,
        double &running_time_in_sec, __u64 &group_id, double &main_scale);
    static bool IsMonitoredAtAllTime(const double &scale);
    static std::string GetCommentConfigName(
        const std::unique_ptr<PerfEvents::CountEvent> &countEvent, std::string eventName);

    static void Report(const std::map<std::string,
                       std::unique_ptr<PerfEvents::CountEvent>> &countEvents, FILE* filePtr);
    static void PrintPerHead(FILE* filePtr);
    static void GetPerKey(std::string &perKey, const PerfEvents::Summary &summary);
    static void MakeComments(const std::unique_ptr<PerfEvents::ReportSum> &reportSum, std::string &commentStr);
    static void ReportNormal(const std::map<std::string,
                             std::unique_ptr<PerfEvents::CountEvent>> &countEvents, FILE* filePtr);
    static void ReportDetailInfos(const std::map<std::string,
                                  std::unique_ptr<PerfEvents::CountEvent>> &countEvents, FILE* filePtr);
    static void PrintPerValue(const std::unique_ptr<PerfEvents::ReportSum> &reportSum, const float &ratio,
                              std::string &configName, FILE* filePtr);
    static void InitPerMap(const std::unique_ptr<PerfEvents::ReportSum> &newPerMap,
                           const PerfEvents::Summary &summary, VirtualRuntime& virtualInstance);
    static bool FindPerCoreEventCount(PerfEvents::Summary &summary, __u64 &eventCount, double &scale);
    static bool FindPercoreRunningTime(PerfEvents::Summary &summary, double &running_time_in_sec, double &main_scale);
    static std::string GetDetailComments(const std::unique_ptr<PerfEvents::CountEvent> &countEvent, double &comment,
                                  PerfEvents::Summary &summary, std::string &configName);
    static std::string HandleOtherConfig(double &comment, PerfEvents::Summary &summary,
                                         double running_time_in_sec, double scale, bool findRunningTime);

    void PrintUsage();
    inline bool HelpOption()
    {
        return helpOption_;
    }
    bool PrepairEvents();
    bool CheckOptions(const std::vector<pid_t> &pids);
    bool CheckSelectCpuPidOption();
    void SetReportFlags(bool cpuFlag, bool threadFlag);
    void SetPerfEvent();
    HiperfError CheckStatOption();

    // for client
    int clientPipeInput_ = -1;
    int clientPipeOutput_ = -1;
    int nullFd_ = -1;
    FILE* filePtr_ = nullptr;
    std::thread clientCommandHandle_;
    bool clientRunning_ = true;
    struct ControlCommandHandler {
        std::function<bool()> preProcess = []() -> bool {
            return false;
        };
        std::function<void(bool)> postProcess = [](bool) {};
    };
    std::unordered_map<std::string, ControlCommandHandler> controlCommandHandlerMap_ = {};
    inline void CreateClientThread();
    void ClientCommandHandle();
    void InitControlCommandHandlerMap();
    void DispatchControlCommand(const std::string& command);
    bool ClientCommandResponse(bool response);
    bool ClientCommandResponse(const std::string& str);
    bool IsSamplingRunning();

    // for cmdline client
    bool allowIpc_ = true;
    std::string controlCmd_ = {};
    bool isFifoServer_ = false;
    bool isFifoClient_ = false;
    bool ProcessControl();
    bool CreateFifoServer();
    void CloseClientThread();
    bool ParseControlCmd(const std::string& cmd);
};

bool RegisterSubCommandStat(void);
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // SUBCOMMAND_STAT_H_
