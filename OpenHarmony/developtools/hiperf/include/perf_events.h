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
#ifndef HIPERF_PERF_EVENTS_H
#define HIPERF_PERF_EVENTS_H

#include <atomic>
#include <cassert>
#include <chrono>
#include <cinttypes>
#include <condition_variable>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <variant>
#include <vector>
#if !is_mingw
#include <poll.h>
#endif

#include <sys/types.h>
#include <unique_fd.h>
#include <linux/perf_event.h>

#include "debug_logger.h"
#include "perf_event_record.h"
#include "perf_record_format.h"
#include "ring_buffer.h"
#include "tracked_command.h"
#include "utilities.h"
#include "virtual_runtime.h"

// this for some performance debug
#define HIDEBUG_SKIP_CALLBACK 0
#define CALC_OFFSET(offset, size) ((offset) & ((size) - 1))
#ifndef CLOCK_MONOTONIC_RAW
#define CLOCK_MONOTONIC_RAW 4
#endif
#define PERF_AUXTRACE_RECORD_ALIGNMENT 8
#define PTR_TO_VOID(ptr) ((void *)(uintptr_t)(ptr))
#define PTR_ADD(b, o) ({ \
    uintptr_t _b = (uintptr_t)(b);  \
    uintptr_t _p = (uintptr_t)&(((uint8_t *)(_b))[o]);  \
    PTR_TO_VOID(_p);  \
})

namespace OHOS {
namespace Developtools {
namespace HiPerf {
using ConfigTable = std::map<__u64, const std::string>;
using SharedConfigTable = std::unique_ptr<ConfigTable>;

const std::string PERF_DISABLE_PARAM = "security.perf_harden";

// define convert from linux/perf_event.h
// description from https://man7.org/linux/man-pages/man2/perf_event_open.2.html

const ConfigTable PERF_HW_CONFIGS = {
    {PERF_COUNT_HW_CPU_CYCLES, "hw-cpu-cycles"},
    {PERF_COUNT_HW_INSTRUCTIONS, "hw-instructions"},
    {PERF_COUNT_HW_CACHE_REFERENCES, "hw-cache-references"},
    {PERF_COUNT_HW_CACHE_MISSES, "hw-cache-misses"},
    {PERF_COUNT_HW_BRANCH_INSTRUCTIONS, "hw-branch-instructions"},
    {PERF_COUNT_HW_BRANCH_MISSES, "hw-branch-misses"},
    {PERF_COUNT_HW_BUS_CYCLES, "hw-bus-cycles"},
    {PERF_COUNT_HW_STALLED_CYCLES_FRONTEND, "hw-stalled-cycles-frontend"},
    {PERF_COUNT_HW_STALLED_CYCLES_BACKEND, "hw-stalled-cycles-backend"},
    {PERF_COUNT_HW_REF_CPU_CYCLES, "hw-ref-cpu-cycles"},
};
const ConfigTable PERF_HW_CACHE_CONFIGS = {
    {PERF_COUNT_HW_CACHE_L1D, "hw-cache-l1d"},   {PERF_COUNT_HW_CACHE_L1I, "hw-cache-l1i"},
    {PERF_COUNT_HW_CACHE_LL, "hw-cache-ll"},     {PERF_COUNT_HW_CACHE_DTLB, "hw-cache-dtlb"},
    {PERF_COUNT_HW_CACHE_ITLB, "hw-cache-itlb"}, {PERF_COUNT_HW_CACHE_BPU, "hw-cache-bpu"},
    {PERF_COUNT_HW_CACHE_NODE, "hw-cache-node"},
};
const ConfigTable PERF_HW_CACHE_OP_CONFIGS = {
    {PERF_COUNT_HW_CACHE_OP_READ, "hw-cache-op-read"},
    {PERF_COUNT_HW_CACHE_OP_WRITE, "hw-cache-op-write"},
    {PERF_COUNT_HW_CACHE_OP_PREFETCH, "hw-cache-op-prefetch"},
};
const ConfigTable PERF_HW_CACHE_OP_RESULT_CONFIGS = {
    {PERF_COUNT_HW_CACHE_RESULT_ACCESS, "hw-cache-result-access"},
    {PERF_COUNT_HW_CACHE_RESULT_MISS, "hw-cache-result-miss"},
};
const ConfigTable PERF_SW_CONFIGS = {
    {PERF_COUNT_SW_CPU_CLOCK, "sw-cpu-clock"},
    {PERF_COUNT_SW_TASK_CLOCK, "sw-task-clock"},
    {PERF_COUNT_SW_PAGE_FAULTS, "sw-page-faults"},
    {PERF_COUNT_SW_CONTEXT_SWITCHES, "sw-context-switches"},
    {PERF_COUNT_SW_CPU_MIGRATIONS, "sw-cpu-migrations"},
    {PERF_COUNT_SW_PAGE_FAULTS_MIN, "sw-page-faults-min"},
    {PERF_COUNT_SW_PAGE_FAULTS_MAJ, "sw-page-faults-maj"},
    {PERF_COUNT_SW_ALIGNMENT_FAULTS, "sw-alignment-faults"},
    {PERF_COUNT_SW_EMULATION_FAULTS, "sw-emulation-faults"},
    {PERF_COUNT_SW_DUMMY, "sw-dummy"},
    {PERF_COUNT_SW_BPF_OUTPUT, "sw-bpf-output"},
};
const ConfigTable PERF_RAW_CONFIGS = {
    {0x0, "raw-sw-incr"},
    {0x1, "raw-l1-icache-refill"},
    {0x2, "raw-l1-itlb-refill"},
    {0x3, "raw-l1-dcache-refill"},
    {0x4, "raw-l1-dcache"},
    {0x5, "raw-l1-dtlb-refill"},
    {0x6, "raw-load-retired"},
    {0x7, "raw-store-retired"},
    {0x8, "raw-instruction-retired"},
    {0x9, "raw-exception-taken"},
    {0xa, "raw-exception-return"},
    {0xb, "raw-cid-write-retired"},
    {0xc, "raw-pc-write-retired"},
    {0xd, "raw-br-immed-retired"},
    {0xe, "raw-br-return-retired"},
    {0xf, "raw-unaligned-ldst-retired"},
    {0x10, "raw-br-mis-pred"},
    {0x11, "raw-cpu-cycles"},
    {0x12, "raw-br-pred"},
    {0x13, "raw-mem-access"},
    {0x14, "raw-l1-icache"},
    {0x15, "raw-l1-dcache-wb"},
    {0x16, "raw-l2-dcache"},
    {0x17, "raw-l2-dcache-refill"},
    {0x18, "raw-l2-dcache-wb"},
    {0x19, "raw-bus-access"},
    {0x1a, "raw-memory-error"},
    {0x1b, "raw-inst-spec"},
    {0x1c, "raw-ttbr-write-retired"},
    {0x1d, "raw-bus-cycles"},
    {0x1f, "raw-l1-dcache-allocate"},
    {0x20, "raw-l2-dcache-allocate"},
    {0x21, "raw-br-retired"},
    {0x22, "raw-br-mis-pred-retired"},
    {0x23, "raw-stall-frontend"},
    {0x24, "raw-stall-backend"},
    {0x25, "raw-l1-dtlb"},
    {0x26, "raw-l1-itlb"},
    {0x27, "raw-l2-icache"},
    {0x28, "raw-l2-icache-refill"},
    {0x29, "raw-l3-dcache-allocate"},
    {0x2a, "raw-l3-dcache-refill"},
    {0x2b, "raw-l3-dcache"},
    {0x2c, "raw-l3-dcache-wb"},
    {0x2d, "raw-l2-dtlb-refill"},
    {0x2e, "raw-l2-itlb-refill"},
    {0x2f, "raw-l2-dtlb"},
    {0x30, "raw-l2-itlb"},
    // newadd
    {0x31, "raw-remote-access"},
    {0x32, "raw-ll-cache"},
    {0x33, "raw-ll-cache-miss"},
    {0x34, "raw-dtlb-walk"},
    {0x35, "raw-itlb-walk"},
    {0x36, "raw-ll-cache-rd"},
    {0x37, "raw-ll-cache-miss-rd"},
    {0x38, "raw-remote-access-rd"},
    {0x39, "raw-l1d-cache-lmiss-rd"},
    {0x3A, "raw-op-retired"},
    {0x3B, "raw-op-spec"},
    {0x3C, "raw-stall"},
    {0x3D, "raw-stall-slot-backend"},
    {0x3E, "raw-stall-slot-frontend"},
    {0x3F, "raw-stall-slot"},
    {0x40, "raw-l1d-cache-rd"},
    {0x4000, "raw-sample-pop"},
    {0x4001, "raw-sample-feed"},
    {0x4002, "raw-sample-filtrate"},
    {0x4003, "raw-sample-collision"},
    {0x4004, "raw-cnt-cycles"},
    {0x4005, "raw-stall-backend-mem"},
    {0x4006, "raw-l1i-cache-lmiss"},
    {0x4009, "raw-l2d-cache-lmiss-rd"},
    {0x400A, "raw-l2i-cache-lmiss"},
    {0x400B, "raw-l3d-cache-lmiss-rd"},
    {0x8002, "raw-sve-inst-retired"},
    {0x8006, "raw-sve-inst-spec"},
    // Refer to "Table K3.1 ARM recommendations for IMPLEMENTATION DEFINED event numbers" in ARMv8 sp
    {0x0040, "raw-l1d-cache-rd"},
    {0x0041, "raw-l1d-cache-wr"},
    {0x0042, "raw-l1d-cache-refill-rd"},
    {0x0043, "raw-l1d-cache-refill-wr"},
    {0x0044, "raw-l1d-cache-refill-inner"},
    {0x0045, "raw-l1d-cache-refill-outer"},
    {0x0046, "raw-l1d-cache-wb-victim"},
    {0x0047, "raw-l1d-cache-wb-clean"},
    {0x0048, "raw-l1d-cache-inval"},
    // 0x0049-0x004B - Reserved
    {0x004C, "raw-l1d-tlb-refill-rd"},
    {0x004D, "raw-l1d-tlb-refill-wr"},
    {0x004E, "raw-l1d-tlb-rd"},
    {0x004F, "raw-l1d-tlb-wr"},
    {0x0050, "raw-l2d-cache-rd"},
    {0x0051, "raw-l2d-cache-wr"},
    {0x0052, "raw-l2d-cache-refill-rd"},
    {0x0053, "raw-l2d-cache-refill-wr"},
    // 0x0054-0x0055 - Reserved
    {0x0056, "raw-l2d-cache-wb-victim"},
    {0x0057, "raw-l2d-cache-wb-clean"},
    {0x0058, "raw-l2d-cache-inval"},
    // 0x0059-0x005B - Reserved
    {0x005C, "raw-l2d-tlb-refill-rd"},
    {0x005D, "raw-l2d-tlb-refill-wr"},
    {0x005E, "raw-l2d-tlb-rd"},
    {0x005F, "raw-l2d-tlb-wr"},
    {0x0060, "raw-bus-access-rd"},
    {0x0061, "raw-bus-access-wr"},
    {0x0062, "raw-bus-access-shared"},
    {0x0063, "raw-bus-access-not-shared"},
    {0x0064, "raw-bus-access-normal"},
    {0x0065, "raw-bus-access-periph"},
    {0x0066, "raw-mem-access-rd"},
    {0x0067, "raw-mem-access-wr"},
    {0x0068, "raw-unaligned-ld-spec"},
    {0x0069, "raw-unaligned-st-spec"},
    {0x006A, "raw-unaligned-ldst-spec"},
    // 0x006B - Reserved
    {0x006C, "raw-ldrex-spec"},
    {0x006D, "raw-strex-pass-spec"},
    {0x006E, "raw-strex-fail-spec"},
    {0x006F, "raw-strex-spec"},
    {0x0070, "raw-ld-spec"},
    {0x0071, "raw-st-spec"},
    {0x0072, "raw-ldst-spec"},
    {0x0073, "raw-dp-spec"},
    {0x0074, "raw-ase-spec"},
    {0x0075, "raw-vfp-spec"},
    {0x0076, "raw-pc-write-spec"},
    {0x0077, "raw-crypto-spec"},
    {0x0078, "raw-br-immed-spec"},
    {0x0079, "raw-br-return-spec"},
    {0x007A, "raw-br-indirect-spec"},
    // 0x007B - Reserved
    {0x007C, "raw-isb-spec"},
    {0x007D, "raw-dsb-spec"},
    {0x007E, "raw-dmb-spec"},
    // 0x007F-0x0080 - Reserved
    {0x0081, "raw-exc-undef"},
    {0x0082, "raw-exc-svc"},
    {0x0083, "raw-exc-pabort"},
    {0x0084, "raw-exc-dabort"},
    // 0x0085 - Reserved
    {0x0086, "raw-exc-irq"},
    {0x0087, "raw-exc-fiq"},
    {0x0088, "raw-exc-smc"},
    // 0x0089 - Reserved
    {0x008A, "raw-exc-hvc"},
    {0x008B, "raw-exc-trap-pabort"},
    {0x008C, "raw-exc-trap-dabort"},
    {0x008D, "raw-exc-trap-other"},
    {0x008E, "raw-exc-trap-irq"},
    {0x008F, "raw-exc-trap-fiq"},
    {0x0090, "raw-rc-ld-spec"},
    {0x0091, "raw-rc-st-spec"},
    // 0x0092-0x009F - Reserved
    {0x00A0, "raw-l3d-cache-rd"},
    {0x00A1, "raw-l3d-cache-wr"},
    {0x00A2, "raw-l3d-cache-refill-rd"},
    {0x00A3, "raw-l3d-cache-refill-wr"},
    // 0x00A4-0x00A5 - Reserved
    {0x00A6, "raw-l3d-cache-wb-victim"},
    {0x00A7, "raw-l3d-cache-wb-clean"},
    {0x00A8, "raw-l3d-cache-inval"},
};
static ConfigTable PERF_TRACEPOINT_CONFIGS = {

};

const std::map<perf_type_id, std::string> PERF_TYPES = {
    {PERF_TYPE_HARDWARE, "hardware"},
    {PERF_TYPE_SOFTWARE, "software"},
    {PERF_TYPE_TRACEPOINT, "tracepoint"},
    {PERF_TYPE_HW_CACHE, "hardware cache"},
    {PERF_TYPE_RAW, "raw"},
};

static std::map<perf_type_id, ConfigTable> TYPE_CONFIGS = {
    {PERF_TYPE_HARDWARE, (PERF_HW_CONFIGS)},           {PERF_TYPE_SOFTWARE, (PERF_SW_CONFIGS)},
    {PERF_TYPE_HW_CACHE, (PERF_HW_CACHE_CONFIGS)},     {PERF_TYPE_RAW, (PERF_RAW_CONFIGS)},
    {PERF_TYPE_TRACEPOINT, (PERF_TRACEPOINT_CONFIGS)},
};

// default config
const std::vector<__u64> DEFAULT_HW_CONFIGS = {
    PERF_COUNT_HW_CPU_CYCLES,
#if defined(__aarch64__)
    PERF_COUNT_HW_STALLED_CYCLES_FRONTEND,
    PERF_COUNT_HW_STALLED_CYCLES_BACKEND,
#endif
    PERF_COUNT_HW_INSTRUCTIONS,
    PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
    PERF_COUNT_HW_BRANCH_MISSES,
};
const std::vector<__u64> DEFAULT_SW_CONFIGS = {
    PERF_COUNT_SW_TASK_CLOCK,
    PERF_COUNT_SW_CONTEXT_SWITCHES,
    PERF_COUNT_SW_PAGE_FAULTS,
};
const std::map<perf_type_id, std::vector<__u64>> DEFAULT_TYPE_CONFIGS = {
    {PERF_TYPE_HARDWARE, DEFAULT_HW_CONFIGS},
    {PERF_TYPE_SOFTWARE, DEFAULT_SW_CONFIGS},
};

struct read_format_event {
    __u64 value; /* The value of the event */
    __u64 id;    /* if PERF_FORMAT_ID */
};

struct read_format_group {
    __u64 nr;           /* The number of events */
    __u64 timeEnabled; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
    __u64 timeRunning; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
    read_format_event events[1];
};

struct read_format_no_group {
    __u64 value;        /* The value of the event */
    __u64 timeEnabled; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
    __u64 timeRunning; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
    __u64 id;           /* if PERF_FORMAT_ID */
};

class PerfEvents {
public:
    static constexpr uint64_t DEFAULT_SAMPLE_FREQUNCY = 4000;
    static constexpr uint64_t DEFAULT_SAMPLE_PERIOD = 1;
    static constexpr uint64_t DEFAULT_TIMEOUT = 10 * 1000;
    static constexpr size_t MIN_BUFFER_SIZE = 64 * 1024 * 1024;
    static constexpr size_t BUFFER_LOW_LEVEL = 10 * 1024 * 1024;
    static constexpr size_t BUFFER_CRITICAL_LEVEL = 5 * 1024 * 1024;
    static constexpr size_t MAX_BUFFER_SIZE_LITTLE = 128 * 1024 * 1024;
    static constexpr size_t MAX_BUFFER_SIZE_LARGE = 256 * 1024 * 1024;
    static constexpr uint64_t DEFAULT_EVENT_MAX_SAMPLE_RATE = 8000;
    static constexpr uint64_t MIN_HM_TRACEPOINT_EVENT_ID = 32768;
    static constexpr size_t MAX_HEX_EVENT_NAME_LENGTH = 10;

    PerfEvents();
    ~PerfEvents();

    bool AddEvents(const std::vector<std::string> &eventStrings, bool group = false);
    bool PrepareTracking(void);
    bool StartTracking(bool immediately = true);
    bool StopTracking(void);
    bool PauseTracking(void);
    bool ResumeTracking(void);
    bool OutputTracking();
    /* call sequence
       1. setXXX
       2. AddEvents()
       3. PrepareTracking
       4. StartTracking (blocking...)
    */
    bool EnableTracking();
    bool IsTrackRunning();
    bool IsOutputTracking();
    void SetOutputTrackingStatus(bool status);

    void SetSystemTarget(bool systemTarget);
    void SetCpu(const std::vector<pid_t> cpus); // cpu id must be [0~N]
    void SetPid(const std::vector<pid_t> pids); // tis is same as pid in kernel
    void SetTimeOut(float timeOut);
    void SetTimeReport(int);
    void SetVerboseReport(bool);
    bool AddOffCpuEvent();

    inline void SetTrackedCommand(const std::vector<std::string> &trackedCommand)
    {
        if (!trackedCommand.empty()) {
            trackedCommand_ = TrackedCommand::CreateInstance(trackedCommand);
        }
    }

    void SetSampleFrequency(unsigned int frequency);
    void SetSamplePeriod(unsigned int period);

    // for background track
    void SetBackTrack(bool backtrack);
    void SetBackTrackTime(uint64_t backtrackTime);

    enum SampleStackType {
        NONE,
        FP,
        DWARF,
    };
    void SetSampleStackType(SampleStackType type);
    void SetDwarfSampleStackSize(uint32_t stackSize);
    void SetMmapPages(size_t mmapPages);
    std::vector<AttrWithId> GetAttrWithId() const;

    void SetInherit(bool inherit)
    {
        inherit_ = inherit;
    };
    void SetClockId(int clockId)
    {
        clockId_ = clockId;
    };
    void SetPerCpu(bool perCpu);
    void SetPerThread(bool perThread);
    bool SetBranchSampleType(uint64_t value);
    bool AddDefaultEvent(perf_type_id type);

    std::map<__u64, std::string> GetSupportEvents(perf_type_id type);

    struct Summary {
        int cpu;
        pid_t tid;
        __u64 eventCount = 0;
        __u64 timeEnabled = 0;
        __u64 timeRunning = 0;
        Summary(const int cpu, const pid_t tid, const __u64 eventCount,
                const __u64 timeEnabled, const __u64 timeRunning)
            : cpu(cpu), tid(tid), eventCount(eventCount), timeEnabled(timeEnabled), timeRunning(timeRunning)
        {
        }
    };

    struct ReportSum {
        int cpu;
        pid_t pid;
        pid_t tid;
        double scaleSum = 1.0;
        double commentSum = 0;
        __u64 eventCountSum = 0;
        __u64 enabledSum = 0;
        __u64 runningSum = 0;
        std::string configName = "";
        std::string threadName = "";
    };
    struct CountEvent {
        bool userOnly = false;
        bool kernelOnly = false;
        __u64 eventCount = 0;
        __u64 timeEnabled = 0;
        __u64 timeRunning = 0;
        __u64 id = 0;
        double usedCpus = 0;
        std::vector<Summary> summaries;
    };
    using StatCallBack =
        std::function<void(const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &, FILE*)>;
    using RecordCallBack = std::function<bool(PerfEventRecord&)>;

    void SetStatCallBack(StatCallBack reportCallBack);
    void SetRecordCallBack(RecordCallBack recordCallBack);
    void SetStatReportFd(FILE* reportPtr);
    void GetLostSamples(size_t &lostSamples, size_t &lostNonSamples)
    {
        lostSamples = lostSamples_;
        lostNonSamples = lostNonSamples_;
    }

    // review: remove this function.
    static const std::string GetStaticConfigName(perf_type_id type_id, __u64 config_id)
    {
        auto typeConfigs = TYPE_CONFIGS.find(type_id);
        if (typeConfigs != TYPE_CONFIGS.end()) {
            auto configs = typeConfigs->second;
            auto config = configs.find(config_id);
            if (config != configs.end()) {
                return config->second;
            } else {
                HLOGW("config not found for %u:%lld in %zu:%zu", type_id, config_id,
                      TYPE_CONFIGS.size(), configs.size());
                // dump all config size
                for (auto types : TYPE_CONFIGS) {
                    HLOGV("type id %d %zu", types.first, types.second.size());
                }
            }
        } else {
            HLOGW("type not found for %d  in %zu", type_id, TYPE_CONFIGS.size());
        }
        std::string configName = StringPrintf("0x%llx", config_id);
        return configName;
    };

    static const std::tuple<bool, perf_type_id, __u64> GetStaticConfigId(const std::string &event_name)
    {
        for (auto type : TYPE_CONFIGS) {
            for (auto config : (type.second)) {
                if (config.second == event_name) {
                    return std::make_tuple(true, type.first, config.first);
                }
            }
        }
        return std::make_tuple(false, PERF_TYPE_MAX, 0);
    };

    const std::string GetTraceConfigName(__u64 config_id)
    {
        auto config = traceConfigTable.find(config_id);
        if (config != traceConfigTable.end()) {
            return config->second;
        } else {
            HLOGW("config not found for %lld in traceConfigTable.", config_id);
        }
        return "<not found>";
    };

    static const std::string GetTypeName(perf_type_id type_id);
    bool ParseEventName(const std::string &nameStr, std::string &name, bool &excludeUser,
                        bool &excludeKernel, bool &isTracePoint);

    // mmap one fd for each cpu
    struct MmapFd {
        int fd;
        perf_event_mmap_page *mmapPage = nullptr;
        uint8_t *buf = nullptr;
        size_t bufSize = 0;
        size_t auxBufSize = 0;
        // for read and sort
        size_t dataSize = 0;
        perf_event_header header;
        uint64_t timestamp = 0;
        const perf_event_attr *attr = nullptr;
        size_t posCallChain = 0;
        int cpu = 0;
        void *auxBuf = nullptr;
        pid_t tid_ = 0;
    };

    bool isHM_ = false;
    bool isSpe_ = false;

    void SetHM(bool isHM);
    void SetConfig(std::map<const std::string, uint64_t> &speOptMaps);
private:
    size_t recordEventCount_ = 0; // only for debug time
#ifdef HIPERF_DEBUG_TIME
    std::chrono::microseconds recordCallBackTime_ = std::chrono::microseconds::zero();
    std::chrono::microseconds recordWaitDataTime_ = std::chrono::microseconds::zero();
    std::chrono::microseconds recordSleepTime_ = std::chrono::microseconds::zero();
    std::chrono::microseconds recordKernelReadTime_ = std::chrono::microseconds::zero();
#endif
    size_t lostSamples_ = 0;
    size_t lostNonSamples_ = 0;

    std::unique_ptr<RingBuffer> recordBuf_ {nullptr};
    bool recordBufReady_ = false;
    std::mutex mtxRrecordBuf_;
    std::condition_variable cvRecordBuf_;
    std::thread readRecordBufThread_;
    std::atomic_bool readRecordThreadRunning_ = false;
    bool startedTracking_ = false;
    bool isLowPriorityThread_ = false;
    void RecordLoop();
    void StatLoop();
    bool IsRecordInMmap(int timeout);
    void ReadRecordsFromMmaps();
    void ReadRecordsFromSpeMmaps(MmapFd& mmapFd, u64 auxOffset, u64 auxSize, u32 pid, u32 tid);
    void SpeReadData(void *dataPage, u64 *dataTail, uint8_t *buf, u32 size);
    bool GetRecordFromMmap(MmapFd &mmap);
    void GetRecordFieldFromMmap(MmapFd &mmap, void *dest, size_t pos, size_t size);
    void MoveRecordToBuf(MmapFd &mmap, bool &isAuxEvent, u64 &auxOffset, u64 &auxSize, u32 &pid, u32 &tid);
    size_t GetCallChainPosInSampleRecord(const perf_event_attr &attr);
    size_t GetStackSizePosInSampleRecord(MmapFd &mmap);
    bool CutStackAndMove(MmapFd &mmap);
    inline void WaitDataFromRingBuffer();
    inline bool ProcessRecord(const perf_event_attr* attr, uint8_t* data);
    void ReadRecordFromBuf();
    size_t CalcBufferSize();
    bool PrepareRecordThread();
    void WaitRecordThread();
    bool HaveTargetsExit(const std::chrono::steady_clock::time_point &startTime);
    void ExitReadRecordBufThread();

#ifdef CONFIG_HAS_CCM
    static constexpr char PRODUCT_CONFIG_PATH[] = "etc/hiperf/hiperf_cfg.json";
    static constexpr char CFG_MAX_BUFFER_SIZE[] = "MaxBufferSize";
    static constexpr char CFG_MIN_BUFFER_SIZE[] = "MinBufferSize";
    void GetBufferSizeCfg(size_t &maxBufferSize, size_t &minBufferSize);
#endif

    enum EventSpaceType {
        UNKNOW = 0,
        USER = 1,
        KERNEL = 2,
        USER_KERNEL = 3,
    };
    uint8_t eventSpaceType_ = EventSpaceType::UNKNOW;

    bool inherit_ = false;
    std::vector<pid_t> pids_;
    std::vector<pid_t> cpus_;
    std::vector<OHOS::UniqueFd> groups_;
    std::chrono::milliseconds timeOut_;    // milliseconds
    std::chrono::milliseconds timeReport_; // means same as timeOut
    bool perCpu_ = false;
    bool perThread_ = false;
    bool verboseReport_ = false;
    bool prepared_ = false;
    ConfigTable traceConfigTable;

    unsigned int samplePeriod_ = 0;
    unsigned int sampleFreq_ = 0;
    uint64_t config_ = 0;
    uint64_t config1_ = 0;
    uint64_t config2_ = 0;
    unsigned int speType_ = 0;

    struct FdItem {
        OHOS::UniqueFd fd;
        int cpu;
        pid_t pid;
        pid_t tid;
        __u64 eventCount;
        mutable uint64_t perfId = 0;
        uint64_t GetPrefId() const
        {
            if (perfId == 0) {
                read_format_no_group readNoGroupValue;
                if (read(fd, &readNoGroupValue, sizeof(readNoGroupValue)) > 0) {
                    perfId = readNoGroupValue.id;
                } else {
                    HLOGW("read failed with fd %d", fd.Get());
                }
            }
            return perfId;
        }
    };
    struct EventItem {
        std::string typeName;
        std::string configName;
        perf_event_attr attr = {};
        std::vector<FdItem> fdItems;
    };
    struct EventGroupItem {
        std::vector<EventItem> eventItems;
    };
    std::vector<EventGroupItem> eventGroupItem_;

    std::map<int, MmapFd> cpuMmap_;
    std::vector<MmapFd *> MmapRecordHeap_;

#if !is_mingw
    std::vector<struct pollfd> pollFds_;
#endif
    const int pollTimeOut_ = 500; // ms
    size_t pageSize_ = 4096;
    size_t auxMmapPages_ = 128;
    bool systemTarget_ = false;
    bool excludeHiperf_ = false;
    pid_t selfPid_ = -1;
    unsigned int mmapPages_ = 0;
    int clockId_ = -1;
    uint64_t branchSampleType_ = 0;
    FILE* reportPtr_ = nullptr;

    SampleStackType sampleStackType_ = SampleStackType::NONE;
    uint32_t dwarfSampleStackSize_ = MAX_SAMPLE_STACK_SIZE;

    // read records from the ring buffer singleton
    void ReadRecordFromBuffer();
    void ReadRecordFromBufferThread();

    std::unique_ptr<TrackedCommand> trackedCommand_ = {};

    StatCallBack reportCallBack_;
    RecordCallBack recordCallBack_;

    void LoadTracepointEventTypesFromSystem();
    bool PerfEventsEnable(bool);
    bool AddEvent(perf_type_id type, __u64 config, bool excludeUser = false,
                  bool excludeKernel = false, bool followGroup = false);
    bool AddEvent(const std::string &eventString, bool followGroup = false);
    bool AddSpeEvent(u32 type, bool followGroup = false);
    bool IsEventSupport(perf_type_id type, __u64 config);
    bool IsEventAttrSupport(perf_event_attr &attr);

    std::chrono::time_point<std::chrono::steady_clock> trackingStartTime_;
    std::chrono::time_point<std::chrono::steady_clock> trackingEndTime_;
    std::chrono::time_point<std::chrono::steady_clock> readingStartTime_;

    std::map<std::string, std::unique_ptr<CountEvent>> countEvents_;

    void PutAllCpus();
    bool PrepareFdEvents();
    bool CreateFdEvents();
    bool StatReport(const __u64 &durationInSec);
    bool CreateMmap(const FdItem &item, const perf_event_attr &attr);
    bool CreateSpeMmap(const FdItem &item, const perf_event_attr &attr);

    const perf_event_attr *GetDefaultAttr()
    {
        HLOG_ASSERT(eventGroupItem_.size() > 0);
        HLOG_ASSERT(eventGroupItem_[0].eventItems.size() > 0);
        return &(eventGroupItem_.at(0).eventItems.at(0).attr);
    };

    OHOS::UniqueFd Open(perf_event_attr &attr, pid_t pid = 0, int cpu = -1, int groupFd = -1,
                        unsigned long flags = 0);
    std::unique_ptr<perf_event_attr> CreateDefaultAttr(perf_type_id type, __u64 config);

    // for update time thread
    static bool updateTimeThreadRunning_;
    static std::atomic<uint64_t> currentTimeSecond_;
    static void UpdateCurrentTime();

    // for background track
    bool backtrack_ = false;
    bool outputTracking_ = false;
    uint64_t backtrackTime_ = 0;
    uint64_t outputEndTime_ = 0;
    bool IsSkipRecordForBacktrack(const PerfRecordSample& sample);
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_PERF_EVENTS_H
