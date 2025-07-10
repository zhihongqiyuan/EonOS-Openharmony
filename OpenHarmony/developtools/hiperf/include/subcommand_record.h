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
#ifndef SUBCOMMAND_RECORD_H
#define SUBCOMMAND_RECORD_H

// some debug feaure
#define HIDEBUG_RECORD_NOT_PROCESS       0
#define HIDEBUG_RECORD_NOT_PROCESS_VM    0
#define HIDEBUG_RECORD_NOT_SAVE          0
#define HIDEBUG_SKIP_PROCESS_SYMBOLS     0
#define HIDEBUG_SKIP_MATCH_SYMBOLS       0
#define HIDEBUG_SKIP_LOAD_KERNEL_SYMBOLS 0
#define HIDEBUG_SKIP_SAVE_SYMBOLS        0
#define USE_COLLECT_SYMBOLIC             1

#include <functional>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include "perf_event_record.h"
#include "perf_events.h"
#include "perf_file_writer.h"
#include "perf_pipe.h"
#include "subcommand.h"
#include "virtual_runtime.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
class SubCommandRecord : public SubCommand {
public:
    static constexpr int DEFAULT_CPU_PERCENT = 25;
    static constexpr int MIN_CPU_PERCENT = 1;
    static constexpr int MAX_CPU_PERCENT = 100;
    static constexpr int MIN_SAMPLE_FREQUENCY = 1;
    static constexpr int MAX_SAMPLE_FREQUENCY = 100000;
    static constexpr int DEFAULT_MMAP_PAGES = 256;
    static constexpr int MIN_PERF_MMAP_PAGE = 2;
    static constexpr int MAX_PERF_MMAP_PAGE = 1024;
    static constexpr int DEFAULT_CHECK_APP_MS = 10;
    static constexpr int MIN_CHECK_APP_MS = 1;
    static constexpr int MAX_CHECK_APP_MS = 200;
    static constexpr float MIN_STOP_SECONDS = 0.100;
    static constexpr float MAX_STOP_SECONDS = 10000.0;
    static constexpr int MIN_SAVED_CMDLINES_SIZE = 512;
    static constexpr int DEFAULT_SAVED_CMDLINES_SIZE = 2048;
    static constexpr int MAX_SAVED_CMDLINES_SIZE = 4096;
    static constexpr uint64_t MIN_BACKTRACK_TIME_SEC = 5;
    static constexpr uint64_t DEFAULT_BACKTRACK_TIME_SEC = 10;
    static constexpr uint64_t MAX_BACKTRACK_TIME_SEC = 30;

    SubCommandRecord()
        // clang-format off
        : SubCommand("record", "Collect performance sample information",
        "Usage: hiperf record [options] [command [command-args]]\n"
        "       Collect performance sampling information of running [command].\n"
        "       The default options are: -c <all cpu> --cpu-limit 25 -d 10000.0 -e hw-cpu-cycles\n"
        "       -f 4000 -m 1024 -o /data/local/tmp/perf.data.\n"
        "   -a\n"
        "         Collect system-wide information.\n"
        "         for measures all processes/threads\n"
        "         This requires CAP_PERFMON (since Linux 5.8) or CAP_SYS_ADMIN capability or a\n"
        "         /proc/sys/kernel/perf_event_paranoid value of less than 1.\n"
        "   --exclude-hiperf\n"
        "         Don't record events issued by hiperf itself.\n"
        "   -c <cpuid>[<,cpuid>]...\n"
        "         cpuid should be 0,1,2...\n"
        "         Limit the CPU that collects data.\n"
        "         0 means cpu0, 1 means cpu1 ...\n"
        "   --cpu-limit <percent>\n"
        "         Set the max percent of cpu time used for recording.\n"
        "         percent is in range [1-100], default is 25.\n"
        "   -d <sec>\n"
        "         stop in <sec> seconds. floating point number. seconds is in range [0.100-10000.0]\n"
        "         default is 10000.0\n"
        "   -f <freq>\n"
        "         Set event sampling frequency. default is 4000 samples every second.\n"
        "         check /proc/sys/kernel/perf_event_max_sample_rate for maximum allowed frequency\n"
        "   --period <num>\n"
        "         Set event sampling period for tracepoint events. recording one sample when <num> events happen.\n"
        "         The default <num> is 1\n"
        "   -e <event1[:<u|k>]>[,event1[:<u|k>]]...\n"
        "         Customize the name of the event that needs to be sampled.\n"
        "         The name can use the names listed in the list parameter.\n"
        "         It can also be represented by the value of 0x<hex>.\n"
        "           u - monitor user space events only\n"
        "           k - monitor kernel space events only\n"
        "   -g <event1[:<u|k>]>[,event1[:<u|k>]]...\n"
        "         Put the events into a group, can set multiple groups by multiple -g\n"
        "         PMU is required to report data in designated groups\n"
        "         limited by HW capability, too many events cannot be reported in the same sampling)\n"
        "   --no-inherit\n"
        "         Don't trace child processes.\n"
        "   -p <pid1>[,pid2]...\n"
        "         Limit the process id of the collection target. Conflicts with the -a option.\n"
        "   -t <tid1>[,tid2]...\n"
        "         Limit the thread id of the collection target. Conflicts with the -a option.\n"
        "   --exclude-tid <tid1>[,tid2]...\n"
        "         Exclude threads of the collection target by thread ids. Conflicts with the -a option.\n"
        "   --exclude-thread <tname1>[,tname2]...\n"
        "         Exclude threads of the collection target by thread names. Conflicts with the -a option.\n"
        "   --exclude-process <pname1>[,pname2]...\n"
        "         Exclude processes by process names. Must be used with -a.\n"
        "   --offcpu\n"
        "         Trace when threads are scheduled off cpu.\n"
        "   -j <branch_filter1>[,branch_filter2]...\n"
        "         taken branch stack sampling, filter can be:\n"
        "           any: any type of branch\n"
        "           any_call: any function call or system call\n"
        "           any_ret: any function return or system call return\n"
        "           ind_call: any indirect branch\n"
        "           ind_jmp: any indirect jump\n"
        "           cond: conditional branches\n"
        "           call: direct calls, including far (to/from kernel) calls\n"
        "           u: only when the branch target is at the user level\n"
        "           k: only when the branch target is in the kernel\n"
        "         requires at least one of any, any_call, any_ret, ind_call, ind_jmp, cond, call\n"
        "   -s / --call-stack <fp|dwarf[,size]>\n"
        "         Setup and enable call stack (stack chain/backtrace) recording, Default is 'fp'.\n"
        "           the value can be:\n"
        "             fp: frame pointer\n"
        "             dwarf: DWARF's CFI - Call Frame Information\n"
        "               'dwarf,size' set sample stack size, size should be in 8~65528 and 8 byte aligned. \n"
        "           as the method to collect the information used to show the call stacks.\n"
        "   --kernel-callchain\n"
        "         collect kernel callchain, must used with -s fp/dwarf simultaneously.\n"
        "   --callchain-useronly\n"
        "         collect only user callchain.\n"
        "   --delay-unwind\n"
        "         If '-s dwarf' used, stack will be unwind while recording, use this option to switch\n"
        "         to unwind after recording.\n"
        "   --disable-unwind\n"
        "         If '-s dwarf' is used, stack will be unwind while recording by default\n"
        "         use this option to disable unwinding.\n"
        "   --disable-callstack-expand\n"
        "         If '-s dwarf' is used, to break the 64k stack limit, callstack is merged by default\n"
        "         to build more complete call stack. that may not be correct sometimes.\n"
        "   --enable-debuginfo-symbolic\n"
        "         If '-s fp/dwarf' is used, symbols in .gnu_debugdata section of an elf, also called minidebuginfo\n"
        "         will be parsed, if not use this option, we will not parse minidebuginfo by default.\n"
        "   --clockid <clock_id>\n"
        "         Set the clock id to use for the various time fields in the perf_event_type records.\n"
        "         monotonic and monotonic_raw are supported,\n"
        "         some events might also allow boottime, realtime and clock_tai.\n"
        "   --symbol-dir <dir>\n"
        "         Set directory to look for symbol files, used for unwinding. \n"
        "   -m <mmap_pages>\n"
        "         Number of the mmap pages, used to receiving record data from kernel,\n"
        "         must be a power of two, rang[2,1024], default is 1024.\n"
        "   --app <package_name>\n"
        "         Collect profile info for an OHOS app, the app must be debuggable.\n"
        "         Record will exit if the process is not started within 20 seconds.\n"
        "   --chkms <millisec>\n"
        "         Set the interval of querying the <package_name>.\n"
        "         <millisec> is in range [1-200], default is 10.\n"
        "   --data-limit <SIZE[K|M|G]>\n"
        "         Stop recording after SIZE bytes of records. Default is unlimited.\n"
        "   -o <output_file_name>\n"
        "         Set output file name, default is /data/local/tmp/perf.data.\n"
        "   -z\n"
        "         Compress record data.\n"
        "   --restart\n"
        "         Collect performance counter information of application startup.\n"
        "         Record will exit if the process is not started within 30 seconds.\n"
        "   --verbose\n"
        "         Show more detailed reports.\n"
        "   --control <command>\n"
        "         Control sampling by <command>, the <command> can be:\n"
        "           prepare: set arguments and prepare sampling\n"
        "           start: start sampling\n"
        "           pause: pause sampling\n"
        "           resume: resume sampling\n"
        "           output: output sampling data\n"
        "           stop: stop sampling\n"
        "   --dedup_stack\n"
        "         Remove duplicated stacks in perf record, conflicts with -a, only restrain using with -p\n"
        "   --cmdline-size <size>\n"
        "         set value to /sys/kernel/tracing/saved_cmdlines_size\n"
        "         the value should be between 512 and 4096\n"
        "   --report\n"
        "         Report with callstack after record. Conflicts with the -a option.\n"
        "   --backtrack\n"
        "         Collect data of the previous period. only restrain using with --control.\n"
        "   --backtrack-sec\n"
        "         If '--backtrack' is used, stop in <sec> seconds. seconds is in range [5-30]\n"
        "         default is 10\n"
        "   --dumpoptions\n"
        "         Dump command options.\n"
        )
    // clang-format on
    {
    }

    ~SubCommandRecord();
    HiperfError OnSubCommand(std::vector<std::string>& args) override;
    bool ParseOption(std::vector<std::string> &args) override;
    void DumpOptions(void) const override;

    // add args for hisysevent
    void AddReportArgs(CommandReporter& reporter) override;

    static bool RegisterSubCommandRecord(void);
    std::map<const std::string, uint64_t> speOptMap_ = {
        {"branch_filter", 0},   {"load_filter", 0},
        {"store_filter", 0},    {"ts_enable", 0},
        {"pa_enable", 0},       {"jitter", 0},
        {"min_latency", 0},      {"event_filter", 0},
        {"pct_enable", 0},
    };

    static SubCommand& GetInstance();

private:
    PerfEvents perfEvents_;
    PerfPipe perfPipe_;

    bool targetSystemWide_ = false;
    bool compressData_ = false;
    bool noInherit_ = false;
    bool excludeHiperf_ = false;
    bool offCPU_ = false;
    bool delayUnwind_ = false;
    bool disableUnwind_ = false;
    bool disableCallstackExpend_ = false;
    bool enableDebugInfoSymbolic_ = false;
    bool verboseReport_ = false;
    bool kernelCallChain_ = true;
    bool callChainUserOnly_ = false;
    bool report_ = false;
    float timeStopSec_ = PerfEvents::DEFAULT_TIMEOUT;
    int frequency_ = 0;
    int period_ = 0;
    int cpuPercent_ = DEFAULT_CPU_PERCENT;
    int mmapPages_ = MAX_PERF_MMAP_PAGE;
    int cmdlinesSize_ = DEFAULT_SAVED_CMDLINES_SIZE;
    int oldCmdlinesSize_ = 0;
    std::vector<std::string> symbolDir_ = {};
    std::string outputFilename_ = "/data/local/tmp/perf.data";
    std::string appPackage_ = {};
    int checkAppMs_ = DEFAULT_CHECK_APP_MS;
    std::string clockId_ = {};
    std::string strLimit_ = {};
    std::string fifoFileC2S_;
    std::string fifoFileS2C_;
    std::vector<pid_t> selectCpus_ = {};
    std::vector<pid_t> selectPids_ = {};
    std::vector<pid_t> selectTids_ = {};
    std::vector<pid_t> inputPidTidArgs_ = {};
    bool restart_ = false;
    std::vector<std::string> selectEvents_ = {};
    std::vector<std::vector<std::string>> selectGroups_ = {};
    std::vector<std::string> callStackType_ = {};
    std::vector<std::string> vecBranchFilters_ = {};
    std::vector<std::string> trackedCommand_ = {};

    // for exclude process and thread
    std::vector<pid_t> excludeTidArgs_ = {};
    std::vector<std::string> excludeThreadNameArgs_ = {};
    std::vector<std::string> excludeProcessNameArgs_ = {};
    std::set<pid_t> excludePids_ = {};
    std::set<pid_t> excludeTids_ = {};
    void CollectExcludeThread();
    void SetExcludeHiperf();
    bool IsThreadExcluded(pid_t pid, pid_t tid);

    // for background track
    bool backtrack_ = false;
    uint64_t backtrackTime_ = DEFAULT_BACKTRACK_TIME_SEC;   // 10 seconds
    bool outputEnd_ = false;
    bool PreOutputRecordFile();
    void OutputRecordFile();
    bool PostOutputRecordFile(bool output);

#ifdef CONFIG_HAS_CCM
    static constexpr char PRODUCT_CONFIG_PATH[] = "etc/hiperf/hiperf_cfg.json";
    static constexpr char CFG_MAP_PAGES[] = "MmapPages";
    void GetMmapPagesCfg();
#endif

    bool GetOptions(std::vector<std::string> &args);
    bool CheckArgsRange();
    bool CheckExcludeArgs();
    bool CheckOptions();
    bool GetSpeOptions();
    bool CheckDataLimitOption();
    bool CheckSelectCpuPidOption();
    bool GetOptionFrequencyAndPeriod(std::vector<std::string> &args);

    bool isCallStackDwarf_ = false;
    bool isCallStackFp_ = false;
    uint32_t callStackDwarfSize_ = MAX_SAMPLE_STACK_SIZE;
    uint64_t branchSampleType_ = 0;
    uint64_t dataSizeLimit_ = 0;
    bool isDataSizeLimitStop_ = false;

    std::unique_ptr<PerfFileWriter> fileWriter_ = nullptr;

    // for client
    int clientPipeInput_ = -1;
    int clientPipeOutput_ = -1;
    int readFd_ = -1;
    int writeFd_ = -1;
    int nullFd_ = -1;
    std::thread clientCommandHandle_;
    std::thread replyCommandHandle_;
    bool clientRunning_ = true;
    bool isHiperfClient_ = false;
    struct ControlCommandHandler {
        std::function<bool()> preProcess = []() -> bool {
            return false;
        };
        std::function<void(bool)> postProcess = [](bool) {};
    };
    std::unordered_map<std::string, ControlCommandHandler> controlCommandHandlerMap_ = {};
    inline void CreateClientThread();
    inline void CreateReplyThread();
    void ClientCommandHandle();
    void ReplyCommandHandle();
    void InitControlCommandHandlerMap();
    void DispatchControlCommand(const std::string& command);
    bool ClientCommandResponse(bool response);
    bool ClientCommandResponse(const std::string& str);
    bool ChildResponseToMain(bool response);
    bool ChildResponseToMain(const std::string& str);
    bool IsSamplingRunning();

    // for cmdline client
    bool allowIpc_ = true;
    std::string controlCmd_ = {};
    bool isFifoServer_ = false;
    bool isFifoClient_ = false;
    bool dedupStack_ = false;
    std::map<pid_t, std::vector<pid_t>> mapPids_;
    bool ProcessControl();
    bool CreateFifoServer();
    bool MainRecvFromChild(int fd, std::string& reply);
    void CloseClientThread();
    void CloseReplyThread();

    bool PreparePerfEvent();
    bool PrepareSysKernel();
    void PrepareKernelMaps();
    bool PrepareVirtualRuntime();

    size_t recordSamples_ = 0;
    size_t recordNoSamples_ = 0;

    bool isNeedSetPerfHarden_ = false;
    bool isSpe_ = false;

    // callback to process record
    bool ProcessRecord(PerfEventRecord& record);
    bool SaveRecord(const PerfEventRecord& record);

    // file format like as 0,1-3,4-6,7,8
    uint32_t GetCountFromFile(const std::string &fileName);
    std::string GetCpuDescFromFile();
    bool AddCpuFeature();
    void AddMemTotalFeature();
    void AddEventDescFeature();
    void AddRecordTimeFeature();
    void AddWorkloadCmdFeature();
    void AddCommandLineFeature();
    void AddCpuOffFeature();
    void AddDevhostFeature();
    bool AddFeatureRecordFile();

    bool CreateInitRecordFile(bool compressData = false);
    bool FinishWriteRecordFile();
    bool PostProcessRecordFile();
    bool RecordCompleted();
#ifdef HIPERF_DEBUG_TIME
    void ReportTime();
#endif

    bool CollectionSymbol(PerfEventRecord& record);
    void CollectSymbol(PerfRecordSample *sample);
    bool SetPerfLimit(const std::string& file, int value, std::function<bool (int, int)> const& cmd,
        const std::string& param);
    bool SetPerfCpuMaxPercent();
    bool SetPerfMaxSampleRate();
    bool SetPerfEventMlock();
    bool SetPerfHarden();

    bool TraceOffCpu();
    bool ParseCallStackOption(const std::vector<std::string> &callStackType);
    bool ParseDataLimitOption(const std::string &str);
    bool ParseBranchSampleType(const std::vector<std::string> &vecBranchSampleTypes);
    bool ParseControlCmd(const std::string cmd);
    bool CheckTargetProcessOptions();
    bool CheckTargetPids();
    bool CheckReportOption();
    bool CheckBacktrackOption();
    bool CheckSpeOption();
    bool IsAppRestarted();
    bool CheckAppRestart();
    pid_t GetPidFromAppPackage(const pid_t oldPid, const uint64_t waitAppTimeOut);
    bool IsAppRunning();
    bool IsPidAndTidExist();
    void MsgPrintAndTrans(bool isTrans, const std::string& msg);
    void WriteCommEventBeforeSampling();
    void RemoveVdsoTmpFile();
    void RemoveFifoFile();

    VirtualRuntime virtualRuntime_;
#if USE_COLLECT_SYMBOLIC
    std::unordered_map<pid_t, std::unordered_set<uint64_t>> kernelThreadSymbolsHits_;
    kSymbolsHits kernelSymbolsHits_;
    uSymbolsHits userSymbolsHits_;
    void SymbolicHits();
#endif

#ifdef HIPERF_DEBUG_TIME
    std::chrono::microseconds prcessRecordTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds saveRecordTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds saveFeatureTimes_ = std::chrono::microseconds::zero();
#endif
    std::chrono::time_point<std::chrono::steady_clock> startSaveFileTimes_;

    void SetHM();
    void SetSavedCmdlinesSize();
    void RecoverSavedCmdlinesSize();
    bool OnlineReportData();

    // only used in UT
    using CheckRecordCallBack = std::function<void(const PerfEventRecord&)>;
    void SetCheckRecordCallback(CheckRecordCallBack callback);
    CheckRecordCallBack checkCallback_ = nullptr;
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // SUBCOMMAND_RECORD_H
