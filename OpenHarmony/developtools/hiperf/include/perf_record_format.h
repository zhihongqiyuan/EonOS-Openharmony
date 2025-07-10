/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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
#ifndef HIPERF_PERF_RECORD_FORMAT_H
#define HIPERF_PERF_RECORD_FORMAT_H

#include <string>
#include <linux/types.h>

#include "utilities.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
// description from https://man7.org/linux/man-pages/man2/perf_event_open.2.html

#define SAMPLE_ID_ALL 0
#define PERF_SAMPLE_SERVER_PID (1U << 31)

struct sample_id {
    u32 pid = 0;
    u32 tid = 0;       /* if PERF_SAMPLE_TID set */
    u64 time = 0;      /* if PERF_SAMPLE_TIME set */
    u64 id = 0;        /* if PERF_SAMPLE_ID set */
    u64 stream_id = 0; /* if PERF_SAMPLE_STREAM_ID set  */
    u32 cpu = 0;
    u32 res = 0;  /* if PERF_SAMPLE_CPU set */
    u64 id2 = 0;       /* if PERF_SAMPLE_IDENTIFIER set */
};

// If PERF_FORMAT_GROUP was not specified
struct read_format {
    __u64 value = 0;        /* The value of the event */
    __u64 timeEnabled = 0; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
    __u64 timeRunning = 0; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
    __u64 id = 0;           /* if PERF_FORMAT_ID */
};

struct PerfRecordAuxtraceData {
    u64 size = 0;
    u64 offset = 0;
    u64 reference = 0;
    u32 idx = 0;
    u32 tid = 0;
    u32 cpu = 0;
    u32 reserved__ = 0;
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    The MMAP events record the PROT_EXEC mappings so that
    we can correlate user-space IPs to code.  They have
    the following structure:
        pid     is the process ID.
        tid     is the thread ID.
        addr    is the address of the allocated memory.
        len     is the length of the allocated memory.
        pgoff   is the page offset of the allocated memory.
        filename
            is a string describing the backing of
            the allocated memory.
*/
struct PerfRecordMmapData {
    u32 pid = 0;
    u32 tid = 0;
    u64 addr = 0;
    u64 len = 0;
    u64 pgoff = 0;
    char filename[KILO] = {0};
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    This record includes extended information on mmap(2)
    calls returning executable mappings.  The format is
    similar to that of the PERF_RECORD_MMAP record, but
    includes extra values that allow uniquely identifying
    shared mappings.

    pid    is the process ID.
    tid    is the thread ID.
    addr   is the address of the allocated memory.
    len    is the length of the allocated memory.
    pgoff  is the page offset of the allocated memory.
    maj    is the major ID of the underlying device.
    min    is the minor ID of the underlying device.
    ino    is the inode number.
    ino_generation
            is the inode generation.
    prot   is the protection information.
    flags  is the flags information.
    filename
            is a string describing the backing of the
            allocated memory.
*/
struct PerfRecordMmap2Data {
    u32 pid = 0;
    u32 tid = 0;
    u64 addr = 0;
    u64 len = 0;
    u64 pgoff = 0;
    u32 maj = 0;
    u32 min = 0;
    u64 ino = 0;
    u64 ino_generation = 0;
    u32 prot = 0;
    u32 flags = 0;
    char filename[KILO] = {0};
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    This record indicates when events are lost.
    id     is the unique event ID for the samples that  were lost.
    lost   is the number of events that were lost.
*/
struct PerfRecordLostData {
    u64 id = 0;
    u64 lost = 0;
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    This record indicates a change in the process name.
    pid    is the process ID.
    tid    is the thread ID.
    comm   is a string containing the new name of the process.
*/
struct PerfRecordCommData {
    u32 pid = 0;
    u32 tid = 0;
    char comm[KILO] = {0};
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

struct PerfBranchEntry {
    u64 from = 0;
    u64 to = 0;
    u64 flags = 0;
};

// This record indicates a sample.
struct PerfRecordSampleData {
    u64 sample_id = 0; /* if PERF_SAMPLE_IDENTIFIER */
    u64 ip = 0;        /* if PERF_SAMPLE_IP */
    u32 pid = 0;
    u32 tid = 0;  /* if PERF_SAMPLE_TID */
    u64 time = 0;      /* if PERF_SAMPLE_TIME */
    u64 addr = 0;      /* if PERF_SAMPLE_ADDR */
    u64 id = 0;        /* if PERF_SAMPLE_ID */
    u64 stream_id = 0; /* if PERF_SAMPLE_STREAM_ID */
    u32 cpu = 0;
    u32 res = 0;  /* if PERF_SAMPLE_CPU */
    u64 period = 0;    /* if PERF_SAMPLE_PERIOD */
    struct read_format v;
    /* if PERF_SAMPLE_READ */
    u64 nr = 0;                        /* if PERF_SAMPLE_CALLCHAIN */
    u64 *ips = nullptr;                /* if PERF_SAMPLE_CALLCHAIN */
    u32 raw_size = 0;                  /* if PERF_SAMPLE_RAW */
    u8 *raw_data = nullptr;                  /* if PERF_SAMPLE_RAW */
    u64 bnr = 0;                       /* if PERF_SAMPLE_BRANCH_STACK */
    struct PerfBranchEntry *lbr = nullptr; /* if PERF_SAMPLE_BRANCH_STACK */
    u64 user_abi = 0;                  /* if PERF_SAMPLE_REGS_USER */
    u64 reg_mask = 0;
    u64 reg_nr = 0;
    u64 *user_regs = nullptr;   /* if PERF_SAMPLE_REGS_USER */
    u64 stack_size = 0;   /* if PERF_SAMPLE_STACK_USER */
    u8 *stack_data = nullptr;   /* if PERF_SAMPLE_STACK_USER */
    u64 dyn_size = 0;     /* if PERF_SAMPLE_STACK_USER && stack_size != 0 */
    u64 weight = 0;       /* if PERF_SAMPLE_WEIGHT */
    u64 data_src = 0;     /* if PERF_SAMPLE_DATA_SRC */
    u64 transaction = 0;  /* if PERF_SAMPLE_TRANSACTION */
    u64 intr_abi = 0;     /* if PERF_SAMPLE_REGS_INTR */
    u64 intr_regs[0]; /* if PERF_SAMPLE_REGS_INTR */
    u64 phys_addr = 0;    /* if PERF_SAMPLE_PHYS_ADDR */
    u64 cgroup = 0;       /* if PERF_SAMPLE_CGROUP */
    u64 server_nr = 0;    /* if PERF_SAMPLE_SERVER_PID */
    u64 *server_pids = 0; /* if PERF_SAMPLE_SERVER_PID */
};

/*
    This record indicates a process exit event.
*/
struct PerfRecordExitData {
    u32 pid = 0;
    u32 ppid = 0;
    u32 tid = 0;
    u32 ptid = 0;
    u64 time = 0;
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    This record indicates a throttle/unthrottle event.
*/
struct PerfRecordThrottleData {
    u64 time = 0;
    u64 id = 0;
    u64 stream_id = 0;
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    This record indicates a fork event.
*/
struct PerfRecordForkData {
    u32 pid = 0;
    u32 ppid = 0;
    u32 tid = 0;
    u32 ptid = 0;
    u64 time = 0;
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    When using hardware sampling (such as Intel PEBS) this
    record indicates some number of samples that may have
    been lost.
*/
struct PerfRecordLostSamplesData {
    u64 lost = 0;
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    This record indicates which process has initiated an
    instruction trace event, allowing tools to properly
    correlate the instruction addresses in the AUX buffer
    with the proper executable.

    pid    process ID of the thread starting an
            instruction trace.
    tid    thread ID of the thread starting an instruction
            trace.
*/
struct PerfRecordItraceStartData {
    u32 pid = 0;
    u32 tid = 0;
};

/*
    This record reports that new data is available in the
    separate AUX buffer region.

    aux_offset
            offset in the AUX mmap region where the new
            data begins.
    aux_size
            size of the data made available.
    flags  describes the AUX update.
            PERF_AUX_FLAG_TRUNCATED
                if set, then the data returned was
                truncated to fit the available buffer
                size.

            PERF_AUX_FLAG_OVERWRITE
                if set, then the data returned has
                overwritten previous data.
*/
struct PerfRecordAuxData {
    u64 aux_offset = 0;
    u64 aux_size = 0;
    u64 flags = 0;
    struct sample_id sample_id;
};

/*
    This record indicates a read event.
*/
struct PerfRecordReadData {
    u32 pid = 0;
    u32 tid = 0;
    read_format values;
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    This record indicates a context switch has happened.
    The PERF_RECORD_MISC_SWITCH_OUT bit in the misc field
    indicates whether it was a context switch into or away
    from the current process.
*/
struct PerfRecordSwitchData {
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    As with PERF_RECORD_SWITCH this record indicates a
    context switch has happened, but it only occurs when
    sampling in CPU-wide mode and provides additional
    information on the process being switched to/from.
    The PERF_RECORD_MISC_SWITCH_OUT bit in the misc field
    indicates whether it was a context switch into or away
    from the current process.

    next_prev_pid
            The process ID of the previous (if switching
            in) or next (if switching out) process on the
            CPU.

    next_prev_tid
            The thread ID of the previous (if switching in)
            or next (if switching out) thread on the CPU.
*/
struct PerfRecordSwitchCpuWideData {
    u32 next_prev_pid = 0;
    u32 next_prev_tid = 0;
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

/*
    This record includes various namespace information of
    a process.

    pid    is the process ID
    tid    is the thread ID

    nr_namespace
            is the number of namespaces in this record

    Each namespace has dev and inode fields and is
    recorded in the fixed position like below:

    NET_NS_INDEX=0
            Network namespace
    UTS_NS_INDEX=1
            UTS namespace
    IPC_NS_INDEX=2
            IPC namespace
    PID_NS_INDEX=3
            PID namespace
    USER_NS_INDEX=4
            User namespace
    MNT_NS_INDEX=5
            Mount namespace
    CGROUP_NS_INDEX=6
            Cgroup namespace
*/
struct PerfRecordNamespacesData {
    u32 pid = 0;
    u32 tid = 0;
    u64 nr_namespaces = 0;
    struct name_space {
        u64 dev = 0;
        u64 inode = 0;
    } namespaces[0];
#if SAMPLE_ID_ALL
    struct sample_id sample_id;
#endif
};

struct PerfRecordTtimeConvData {
    u64 time_shift = 0;
    u64 time_mult = 0;
    u64 time_zero = 0;
    u64 time_cycles = 0;
    u64 time_mask = 0;
    u8  cap_user_time_zero = 0;
    u8  cap_user_time_short = 0;
    u8  reserved[6] = {0}; // 6 : For alignment
};

struct PerfRecordAuxtraceInfoData {
    u32 type = 0;
    u32 reserved = 0;
    u64 priv[2] = {0};
};

struct PerfRecordCpuMapData {
    u16 type = 0;
    u16 nr = 0;
    u16 cpu[16] = {0};     // 16 : cpus
    u8  reserved[4] = {0}; // 4 : For alignment
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_PERF_RECORD_FORMAT_H
