/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
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

#ifndef BPF_CONTROLLER_H
#define BPF_CONTROLLER_H

#include <iostream>

#include <memory>
#include <string>
#include <functional>
#include <vector>
#include <functional>
#include <set>

#include "type_headers.h"
#include "libbpf.h"
#include "bpf_log_reader.h"
#include "hhlog.h"
#include "hiebpf.skel.h"
#include "bpf_event_receiver.h"
#include "hiebpf_data_file.h"

enum HiebpfEventGroup:uint32_t {
    // file system events start from 0x000
    FS_GROUP_BASE = 0,
    FS_GROUP_ALL = FS_GROUP_BASE,
    FS_GROUP_OPEN,
    FS_GROUP_READ,
    FS_GROUP_WRITE,
    FS_GROUP_CLOSE,
    // memory events start from 0x100
    MEM_GROUP_BASE = 0x100,
    MEM_GROUP_ALL = MEM_GROUP_BASE,
    // BIO  events start from 0x200
    BIO_GROUP_BASE = 0x200,
    BIO_GROUP_ALL = BIO_GROUP_BASE,
};

struct BPFConfig {
    bool excludeTracer_ {true};
    bool unwindStack_ {true};
    __u32 dumpEvents_ {0};
    __u32 traceDuration_ {0};
    __u32 maxStackDepth_ {MAX_STACK_LIMIT};
    __u32 epollTimeout_ {100};
    __u32 pipelines_ {1}; // Numbers of pipeline channels that process data in user mode
    std::string cmd_;
    std::vector<pid_t> targetPids_ {};
    std::set<HiebpfEventGroup> selectEventGroups_ {};
    std::string outputFile_ {"/data/local/tmp/hiebpf.data"};
    // loggers configs
    __u32 BPFLogLevel_ {BPF_LOG_DEBUG};
    int LIBBPFLogLevel_ {LIBBPF_DEBUG};
    std::string BPFLogFile_ {"stdout"};
    std::string LIBBPFLogFile_ {"stdout"};
};

class BPFController {
public:
    ~BPFController();
    static std::unique_ptr<BPFController> MakeUnique(const struct BPFConfig& config);
    int Start();
    void Stop();

    static int HandleEvent(void *ctx, void *data, size_t dataSize);
    static int DumpEvent(void *ctx, void *data, size_t dataSize);

    enum ConfigurationLimit:__u32 {
        DUMP_EVENTS_LIMIT = 10000,
        TRACE_DURATION_LIMIT = 3600,
        MIN_PIPELINES_LIMIT = 1,
    };

private:
    BPFController(const struct BPFConfig& config):config_ {config} {}
    int VerifySelectEventGroups(const std::set<HiebpfEventGroup> &selectEventGroups);
    int VerifyConfigurations();
    int SetUpBPF();
    int FilterProgByEvents();
    void FilterFsGroup();
    int ConfigureBPF();
    int InitBPFVariables() const;
    int FillTargetPidMap() const;
    int ConfigBPFLogger();
    int ConfigLIBBPFLogger() const;
    int ConfigReceivers();
    int32_t ConfigDlopenBPFProg();
    uint64_t GetSymOffset(const std::string &path, const std::string &symbol);
    std::weak_ptr<BPFEventReceiver> NextActiveReceiver();

    int BPFEventLoopOnce() const
    {
        int err = ring_buffer__poll(rb_, config_.epollTimeout_);
        if (err < 0) {
            HHLOGE(true, "failed to poll BPF ring buffer: %s", strerror(-err));
            return err;
        }
        return 0;
    }

    inline int StartBPFLogReader()
    {
        if (bpfLogReader_) {
            return bpfLogReader_->Start();
        }
        return 0;
    }

    inline int StartReceivers()
    {
        if (receivers_.empty()) {
            return 0;
        }
        for (__u32 k = 0; k < receivers_.size(); ++k) {
            if (receivers_[k]->Start() != 0) {
                return -1;
            }
        }
        return 0;
    }

    static int LIBBPFPrintFunc(enum libbpf_print_level level, const char *format, va_list args);
    static int DumpFSTraceEvent(BPFController *bpfctlr, void *data, size_t dataSize);
    static int DumpPFTraceEvent(BPFController *bpfctlr, void *data, size_t dataSize);
    static int DumpBIOTraceEvent(BPFController *bpfctlr, void *data, size_t dataSize);
    static int DumpSTRTraceEvent(void *data, size_t dataSize);

    bool loopStop_ {false};
    struct hiebpf_bpf* skel_ {nullptr};
    std::unique_ptr<BPFLogReader> bpfLogReader_ {nullptr};
    struct BPFConfig config_;
    std::set<HiebpfEventGroup> selectEventGroups_ {};
    struct ring_buffer* rb_ {nullptr};
    std::shared_ptr<HiebpfDataFile> dataFile_ {nullptr};
    std::vector<std::shared_ptr<BPFEventReceiver>> receivers_ {};
    __u32 last_ {0};
    __u64 *ips_ {nullptr};
};

#endif