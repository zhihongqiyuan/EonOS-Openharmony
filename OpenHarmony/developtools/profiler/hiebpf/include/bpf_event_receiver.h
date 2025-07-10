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

#ifndef BPF_EVENT_RECEIVER_H
#define BPF_EVENT_RECEIVER_H

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>


#include "type_headers.h"
#include "ringbuffer.h"
#include "hiebpf_data_file.h"
#include "hhlog.h"
#include "hiebpf.skel.h"
#include "bpf.h"
#include "fstrace_args_converter.h"
#include "maps_info.h"
#include "elf_symbol_info.h"
#include <iostream>


#define FSTRACE_MAX_ARGS    4
#define MAX_TRACER_NAME_LEN 8
#define MAX_TYPE_NAME_LEN   16
#define MAX_BUSY_LOOPS      1000
#define PADDING(nbytes)     char reserve_[nbytes]

struct FixedFSTraceTLVItem {
    __u32 tracer_;
    __u32 itemLen_;
    __u32 pid_;
    __u32 tid_;
    char tracerName_[MAX_TRACER_NAME_LEN]; // MAX_TRACER_NAME_LEN = 8 bytes, which is 64 bits
    __u64 stime_;
    __u64 ctime_;
    char typeName_[MAX_TYPE_NAME_LEN];      // MAX_TYPE_NAME_LEN = 16 bytes, which is 128 bits
    int32_t  retval_;
    uint16_t nips_;
    uint16_t type_;
    __u64 args_[FSTRACE_MAX_ARGS];
    char comm_[MAX_COMM_LEN];           // MAX_COMM_LEN = 16 bytes, which 128 bits
    /* the above members have been carefully aligned */
};

struct FixedPFTraceTLVItem {
    __u32 tracer_;
    __u32 itemLen_;
    __u32 pid_;
    __u32 tid_;
    char tracerName_[MAX_TRACER_NAME_LEN];
    __u64 stime_;
    __u64 ctime_;
    char typeName_[MAX_TYPE_NAME_LEN];
    __u64 addr_;
    __u32 size_;
    uint16_t nips_;
    uint16_t type_;
    char comm_[MAX_COMM_LEN];
    /* the above members have been carefully aligned */
};

struct FixedBIOTraceTLVItem {
    __u32 tracer_;
    __u32 itemLen_;
    __u32 pid_;
    __u32 tid_;
    char comm_[MAX_COMM_LEN];
    __u64 stime_;
    __u64 ctime_;
    __u32 prio_;
    __u32 size_;
    __u64 blkcnt_;
    __u32 nips_;
    __u32 type_;
    char typeName_[MAX_TYPE_NAME_LEN];
};

struct FixedSTRTraceTLVItem {
    __u32 tracer_;
    __u32 itemLen_;
    __u32 pid_;
    __u32 tid_;
    __u64 stime_;
    __u32 srcTracer_;
    __u32 srcType_;
    __u32 strLen_;
    PADDING(sizeof(__u32));
    /* the above members have been carefully aligned */
};

struct FixedMapTLVItem {
    uint32_t type = 0;
    uint32_t len = 0;
    uint64_t start = 0;
    uint64_t end = 0;
    uint32_t offset = 0;
    uint32_t pid = 0;
    uint32_t fileNameLen = 0;
}__attribute__((packed));

struct FixedSymbolTLVItem {
    uint32_t type = 0;
    uint32_t len = 0;
    uint64_t textVaddr = 0;
    uint32_t textOffset = 0;
    uint32_t strTabLen = 0;
    uint32_t symTabLen = 0;
    uint32_t fileNameLen = 0;
    uint32_t symEntLen = 0;
}__attribute__((packed));

class BPFEventReceiver {
public:
    static inline std::shared_ptr<BPFEventReceiver> MakeShared(const std::weak_ptr<HiebpfDataFile> &file)
    {
        std::shared_ptr<BPFEventReceiver> obj {new(std::nothrow) BPFEventReceiver {file}};
        if (obj == nullptr) {
            return nullptr;
        }
        if (obj->InitBuffer() != 0) {
            return nullptr;
        }
        return obj;
    }

    ~BPFEventReceiver()
    {
        if (not stop_) {
            stop_ = true;
        }
        if (workLoop_.joinable()) {
            workLoop_.join();
        }
    }

    inline int Put(void *data, const size_t dataSize)
    {
        HHLOGF((data == nullptr or dataSize == 0), "invalid arguments: this should never happen");
        HHLOGF(buf_ == nullptr, "receiver has no buffer: this should never happen");
        int ret {-1};
        ret = buf_->Put((const char*)data, dataSize);
        if (ret == static_cast<int>(dataSize)) {
            NotifyOne();
        }
        HHLOGF((0 <= ret and ret < static_cast<int>(dataSize)), "incomplete data received: this should never happen");
        return ret;
    }

    inline int Start()
    {
        workLoop_ = std::thread([this] { this->WorkLoop(); });
        return 0;
    }

    inline void Stop()
    {
        if (Running()) {
            stop_ = true;
            NotifyOne();
        }
        if (workLoop_.joinable()) {
            workLoop_.join();
        }
    }

    inline bool Running() const
    {
        return (not stop_);
    }

private:
    enum MemAlign:std::size_t {
        ALIGNMENT_BYTES = 8,
        ALIGNMENT_MASK = 7,
    };
    void DoWork();
    void ReceiveFSTraceEvent();
    void ReceivePFTraceEvent();
    void ReceiveBIOTraceEvent();
    void ReceiveSTRTraceEvent();
    void ReceiveDlopenTraceEvent();
    void WriteEventMaps(uint32_t pid);
    void WriteSymbolInfo(const std::string &fileName);
    void ReverseStr(char* left, char* right);
    void DiscardEvent();
    int EncodeFSTraceEvent(
        const struct fstrace_cmplt_event_t *cmplt_event,
        void* tlvItem,
        const size_t itemLen);
    int EncodePFTraceEvent(
        const struct pftrace_cmplt_event_t *cmplt_event,
        void* tlvItem,
        const size_t itemLen);
    int EncodeBIOTraceEvent(
        const struct biotrace_cmplt_event_t *cmplt_event,
        void* tlvItem,
        const size_t itemLen);
    int EncodeSTRTraceEvent(
        const struct strtrace_cmplt_event_t *cmplt_event,
        void* tlvItem,
        const size_t itemLen);
    int ConvertFSTraceArgsToArray(__u64 *args, const struct fstrace_start_event_t *start_event)
    {
        int (*fn) (__u64*, const struct fstrace_start_event_t *) = g_argsConverterTable[start_event->type];
        if (fn) {
            return fn(args, start_event);
        }
        HHLOGE(true, "invalid converter for type = %u", start_event->type);
        return -1;
    }

    inline __u32 GetFSTraceTLVItemSize(const struct fstrace_cmplt_event_t &cmplt_event)
    {
        __u32 size {sizeof(struct FixedFSTraceTLVItem)};
        size += cmplt_event.nips * sizeof(__u64);
        return size;
    }

    inline __u32 GetPFTraceTLVItemSize(const struct pftrace_cmplt_event_t &cmplt_event)
    {
        __u32 size {sizeof(struct FixedPFTraceTLVItem)};
        size += cmplt_event.nips * sizeof(__u64);
        return size;
    }

    inline __u32 GetBIOTraceTLVItemSize(const struct biotrace_cmplt_event_t &cmplt_event)
    {
        __u32 size {sizeof(struct FixedBIOTraceTLVItem)};
        size += cmplt_event.nips * sizeof(__u64);
        return size;
    }

    inline __u32 GetSTRTraceTLVItemSize(const struct strtrace_cmplt_event_t &cmplt_event)
    {
        __u32 size {sizeof(FixedSTRTraceTLVItem)};
        size += cmplt_event.len;
        size &= ~ALIGNMENT_MASK;
        size += alignBytes_;
        return size;
    }

    inline void WorkLoop()
    {
        while (not stop_) {
            DoWork();
        }
    }

    inline BPFEventReceiver(const std::weak_ptr<HiebpfDataFile> &file) : file_ {file} {}

    inline void NotifyOne()
    {
        cond_.notify_one();
    }

    inline int InitBuffer()
    {
        constexpr std::size_t bufSize {1 << 20};
        constexpr enum RingBuffer::MemAlignShift memAlign {RingBuffer::MemAlignShift::W_ALIGN_SHIFT};
        buf_ = std::make_unique<RingBuffer>(bufSize, memAlign);
        if (buf_ == nullptr or (not (*buf_))) {
            return -1;
        }
        return 0;
    }

    std::vector<std::string> gTracerTable {
        "maptrace",
        "symtrace",
        "fstrace",
        "pftrace",
        "biotrace",
        "strtrace",
    };

    std::vector<std::string> gFSTraceTypeTable {
        "",
        "openat2",
        "read",
        "write",
        "pread64",
        "pwrite64",
        "readv",
        "writev",
        "preadv",
        "pwritev",
        "close",
    };

    std::vector<std::string> gPFTraceTypeTable {
        "",
        "FILE_BACKED_IN",
        "PAGE_CACHE_HIT",
        "SWAP_FROM_ZRAM",
        "SWAP_FROM_DISK",
        "ZERO_FILL_PAGE",
        "FAKE_ZERO_PAGE",
        "COPY_ON_WRITE",
        "MAX_EVENT_TYPE",
    };

    std::vector<std::string> gBIOTraceTypeTable {
        "",
        "DATA_READ",
        "DATA_WRITE",
        "METADATA_READ",
        "METADATA_WRITE",
        "PAGE_IN",
        "PAGE_OUT",
    };

    const std::size_t alignBytes_ {ALIGNMENT_BYTES};
    bool stop_ {false};
    std::mutex  mtx_ {};
    std::condition_variable cond_ {};
    std::thread workLoop_ {};
    std::unique_ptr<RingBuffer> buf_ {nullptr};
    std::weak_ptr<HiebpfDataFile> file_ {};
    OHOS::Developtools::Hiebpf::MapsInfo mapsInfo_;
    OHOS::Developtools::Hiebpf::ElfSymbolInfo elfSymbolInfo_;
};
#endif