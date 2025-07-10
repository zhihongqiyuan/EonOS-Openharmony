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
#define HILOG_TAG "PerfRecord"

#include "perf_event_record.h"
#include "spe_decoder.h"
#include <cinttypes>

#include "utilities.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {

bool PerfRecordSample::dumpRemoveStack_ = false;
std::vector<u64> PerfRecordSample::ips_ = {};
std::vector<DfxFrame> PerfRecordSample::callFrames_ = {};
std::vector<pid_t> PerfRecordSample::serverPidMap_ = {};
thread_local std::unordered_map<PerfRecordType, PerfEventRecord*> PerfEventRecordFactory::recordMap_ = {};

static PerfEventRecord* CreatePerfEventRecord(PerfRecordType type)
{
    switch (type) {
        case PERF_RECORD_SAMPLE:
            return new PerfRecordSample();
        case PERF_RECORD_MMAP:
            return new PerfRecordMmap();
        case PERF_RECORD_MMAP2:
            return new PerfRecordMmap2();
        case PERF_RECORD_LOST:
            return new PerfRecordLost();
        case PERF_RECORD_COMM:
            return new PerfRecordComm();
        case PERF_RECORD_EXIT:
            return new PerfRecordExit();
        case PERF_RECORD_THROTTLE:
            return new PerfRecordThrottle();
        case PERF_RECORD_UNTHROTTLE:
            return new PerfRecordUnthrottle();
        case PERF_RECORD_FORK:
            return new PerfRecordFork();
        case PERF_RECORD_READ:
            return new PerfRecordRead();
        case PERF_RECORD_AUX:
            return new PerfRecordAux();
        case PERF_RECORD_AUXTRACE:
            return new PerfRecordAuxtrace();
        case PERF_RECORD_AUXTRACE_INFO:
            return new PerfRecordAuxTraceInfo();
        case PERF_RECORD_TIME_CONV:
            return new PerfRecordTimeConv();
        case PERF_RECORD_CPU_MAP:
            return new PerfRecordCpuMap();
        case PERF_RECORD_ITRACE_START:
            return new PerfRecordItraceStart();
        case PERF_RECORD_LOST_SAMPLES:
            return new PerfRecordLostSamples();
        case PERF_RECORD_SWITCH:
            return new PerfRecordSwitch();
        case PERF_RECORD_SWITCH_CPU_WIDE:
            return new PerfRecordSwitchCpuWide();
        default:
            HLOGE("unknown record type %d\n", type);
            return new PerfRecordNull();
    }
}

template<typename T>
inline void PushToBinary(bool condition, uint8_t *&p, const T &v)
{
    if (condition) {
        *(reinterpret_cast<T *>(p)) = v;
        p += sizeof(T);
    }
}

template<typename T1, typename T2>
inline void PushToBinary2(bool condition, uint8_t *&p, const T1 &v1, const T2 &v2)
{
    if (condition) {
        *(reinterpret_cast<T1 *>(p)) = v1;
        p += sizeof(T1);
        *(reinterpret_cast<T2 *>(p)) = v2;
        p += sizeof(T2);
    }
}

template<typename T>
inline void PopFromBinary(bool condition, uint8_t*& p, T& v, u64& size)
{
    HIPERF_ASSERT(sizeof(T) <= size, "PopFromBinary error\n");
    if (condition) {
        v = *(reinterpret_cast<const T *>(p));
        p += sizeof(T);
        size -= sizeof(T);
    }
}

template<typename T1, typename T2>
inline void PopFromBinary2(bool condition, uint8_t*& p, T1& v1, T2& v2, u64& size)
{
    HIPERF_ASSERT(sizeof(T1) + sizeof(T2) <= size, "PopFromBinary2 error\n");
    if (condition) {
        v1 = *(reinterpret_cast<const T1 *>(p));
        p += sizeof(T1);
        v2 = *(reinterpret_cast<const T2 *>(p));
        p += sizeof(T2);
        size -= (sizeof(T1) + sizeof(T2));
    }
}

inline void SetPointerOffset(uint8_t*& p, u64 offset, u64& size)
{
    HIPERF_ASSERT(offset <= size && offset <= RECORD_SIZE_LIMIT, "SetPointerOffset error\n");
    size -= offset;
    p += offset;
}

void PerfEventRecord::GetHeaderBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetHeaderSize()) {
        buf.resize(GetHeaderSize());
    }
    uint8_t *p = buf.data();
    *(reinterpret_cast<perf_event_header*>(p)) = header_;
}

void PerfEventRecord::Dump(int indent, std::string outputFilename, FILE *outputDump) const
{
    if (outputDump != nullptr) {
        g_outputDump = outputDump;
    } else if (!outputFilename.empty() && g_outputDump == nullptr) {
        std::string resolvedPath = CanonicalizeSpecPath(outputFilename.c_str());
        g_outputDump = fopen(resolvedPath.c_str(), "w");
        if (g_outputDump == nullptr) {
            printf("unable open file to '%s' because '%d'\n", outputFilename.c_str(), errno);
            return;
        }
    }
    PRINT_INDENT(indent, "\n");
    PRINT_INDENT(indent, "record %s: type %u, misc %u, size %zu\n", GetName(), GetType(),
                 GetMisc(), GetSize());
    DumpData(indent + 1);
}

void PerfEventRecord::DumpLog(const std::string &prefix) const
{
    HLOGV("%s: record %s: type %u, misc %u, size %zu\n", prefix.c_str(), GetName(),
          GetType(), GetMisc(), GetSize());
}

void PerfEventRecord::Init(perf_event_type type, bool inKernel)
{
    header_.type = type;
    header_.misc = inKernel ? PERF_RECORD_MISC_KERNEL : PERF_RECORD_MISC_USER;
    header_.size = sizeof(header_);
};

void PerfEventRecord::Init(perf_event_hiperf_ext_type type)
{
    header_.type = type;
    header_.misc = PERF_RECORD_MISC_USER;
    header_.size = sizeof(header_);
}

void PerfEventRecord::InitHeader(uint8_t* p)
{
    if (p == nullptr) {
        header_.type = PERF_RECORD_MMAP;
        header_.misc = PERF_RECORD_MISC_USER;
        header_.size = 0;
        return;
    }
    header_ = *(reinterpret_cast<perf_event_header*>(p));
}

void PerfRecordAuxtrace::Init(uint8_t* data, const perf_event_attr& attr)
{
    data_ = {};
    rawData_ = nullptr;
    if (data == nullptr) {
        PerfEventRecord::InitHeader(data);
        HLOGE("Init failed");
        return;
    }
    PerfEventRecordTemplate::Init(data);
    if (header_.size != sizeof(header_) + sizeof(data_)) {
        HLOGE("header_.size invalid");
        return;
    }
    rawData_ = data + header_.size;
}

PerfRecordAuxtrace::PerfRecordAuxtrace(u64 size, u64 offset, u64 reference, u32 idx, u32 tid, u32 cpu, u32 pid)
{
    PerfEventRecord::Init(PERF_RECORD_AUXTRACE);
    data_.size = size;
    data_.offset = offset;
    data_.reference = reference;
    data_.idx = idx;
    data_.tid = tid;
    data_.cpu = cpu;
    data_.reserved__ = pid;

    header_.size = sizeof(header_) + sizeof(data_);
}

bool PerfRecordAuxtrace::GetBinary1(std::vector<uint8_t> &buf) const
{
    if (buf.size() < header_.size) {
        buf.resize(header_.size);
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    size_t copySize = header_.size - GetHeaderSize();
    if (memcpy_s(p, buf.size() - GetHeaderSize(), reinterpret_cast<const uint8_t *>(&data_), copySize) != 0) {
        HLOGE("memcpy_s return failed in GetBinary1");
        return false;
    }
    return true;
}

bool PerfRecordAuxtrace::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    size_t copySize = header_.size - GetHeaderSize();
    if (memcpy_s(p, buf.size() - GetHeaderSize(), reinterpret_cast<const uint8_t *>(&data_), copySize) != 0) {
        HLOGE("memcpy_s return failed in GetBinary with data_");
        return false;
    }
    p += header_.size - GetHeaderSize();
    if (memcpy_s(p, buf.size() - header_.size, static_cast<uint8_t *>(rawData_), data_.size) != 0) {
        HLOGE("memcpy_s return failed in GetBinary with rawData_");
        return false;
    }
    return true;
}

void PerfRecordAuxtrace::DumpData(int indent) const
{
    PRINT_INDENT(indent, "size 0x%llx, offset 0x%llx, reference 0x%llx, idx %u, tid %u, cpu %u, pid %u\n",
                 data_.size, data_.offset, data_.reference, data_.idx, data_.tid, data_.cpu, data_.reserved__);
#if defined(is_ohos) && is_ohos
    if (!SpeDumpRawData(rawData_, data_.size, indent, g_outputDump)) {
        HLOGE("SpeDumpRawData failed");
    }
#endif
}

void PerfRecordAuxtrace::DumpLog(const std::string &prefix) const
{
    HLOGV("size %llu, offset 0x%llx, reference 0x%llx, idx %u, tid %u, cpu %u\n",
          data_.size, data_.offset, data_.reference, data_.idx, data_.tid, data_.cpu);
}

size_t PerfRecordAuxtrace::GetSize() const
{
    return header_.size + data_.size;
}

void PerfRecordMmap::Init(uint8_t* data, const perf_event_attr&)
{
    PerfEventRecordTemplate::Init(data);
    data_.filename[KILO - 1] = '\0';
}

PerfRecordMmap::PerfRecordMmap(bool inKernel, u32 pid, u32 tid, u64 addr, u64 len, u64 pgoff,
                               const std::string &filename)
{
    PerfEventRecord::Init(PERF_RECORD_MMAP, inKernel);

    data_.pid = pid;
    data_.tid = tid;
    data_.addr = addr;
    data_.len = len;
    data_.pgoff = pgoff;
    if (strncpy_s(data_.filename, KILO, filename.c_str(), filename.size()) != 0) {
        HLOGE("strncpy_s failed");
    }

    header_.size = sizeof(header_) + sizeof(data_) - KILO + filename.size() + 1;
}

bool PerfRecordMmap::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    // data_.filename[] is variable-length
    std::copy(reinterpret_cast<const uint8_t *>(&data_),
              reinterpret_cast<const uint8_t *>(&data_) + GetSize() - GetHeaderSize(), p);
    return true;
}

void PerfRecordMmap::DumpData(int indent) const
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        PRINT_INDENT(indent, "pid %u, tid %u, addr 0x%llx, len 0x%llx\n", data_.pid, data_.tid,
                     data_.addr, data_.len);
        PRINT_INDENT(indent, "pgoff 0x%llx, filename %s\n", data_.pgoff, data_.filename);
    }
#endif
}

void PerfRecordMmap::DumpLog(const std::string &prefix) const
{
    HLOGV("%s:  MMAP: size %d pid %u tid %u dso '%s' (0x%llx-0x%llx)@0x%llx", prefix.c_str(),
          header_.size, data_.pid, data_.tid, data_.filename, data_.addr, data_.addr + data_.len, data_.pgoff);
}

void PerfRecordMmap2::Init(uint8_t* data, const perf_event_attr&)
{
    PerfEventRecordTemplate::Init(data);
    discard_ = false;
}

PerfRecordMmap2::PerfRecordMmap2(bool inKernel, u32 pid, u32 tid, u64 addr, u64 len, u64 pgoff,
                                 u32 maj, u32 min, u64 ino, u32 prot, u32 flags,
                                 const std::string &filename)
{
    PerfEventRecord::Init(PERF_RECORD_MMAP2, inKernel);
    data_.pid = pid;
    data_.tid = tid;
    data_.addr = addr;
    data_.len = len;
    data_.pgoff = pgoff;
    data_.maj = maj;
    data_.min = min;
    data_.ino = ino;
    data_.ino_generation = 0;
    data_.prot = prot;
    data_.flags = flags;
    if (strncpy_s(data_.filename, KILO, filename.c_str(), filename.size()) != 0) {
        HLOGE("strncpy_s failed");
    }

    header_.size = sizeof(header_) + sizeof(data_) - KILO + filename.size() + 1;
}

PerfRecordMmap2::PerfRecordMmap2(bool inKernel, u32 pid, u32 tid, std::shared_ptr<DfxMap> item)
{
    PerfEventRecord::Init(PERF_RECORD_MMAP2, inKernel);
    data_.pid = pid;
    data_.tid = tid;
    if (item != nullptr) {
        data_.addr = item->begin;
        data_.len = item->end - item->begin;
        data_.pgoff = item->offset;
        data_.maj = item->major;
        data_.min = item->minor;
        data_.ino = item->inode;
        data_.ino_generation = 0;
        // r--p 00000000 103:3e 12307                         /data/storage/el1/bundle/entry.hap
        // why prot get from this is 7. rwxp
        DfxMap::PermsToProts(item->perms, data_.prot, data_.flags);
        if (strncpy_s(data_.filename, KILO, item->name.c_str(), item->name.size()) != 0) {
            HLOGE("strncpy_s failed");
        }

        header_.size = sizeof(header_) + sizeof(data_) - KILO + item->name.size() + 1;
    } else {
        data_.addr = 0;
        data_.len = 0;
        data_.pgoff = 0;
        data_.maj = 0;
        data_.min = 0;
        data_.ino = 0;
        data_.ino_generation = 0;
        if (memset_s(data_.filename, KILO, 0, KILO) != EOK) {
            HLOGE("memset_s failed");
        }
    }
}

bool PerfRecordMmap2::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    // data_.filename[] is variable-length
    std::copy(reinterpret_cast<const uint8_t *>(&data_),
              reinterpret_cast<const uint8_t *>(&data_) + GetSize() - GetHeaderSize(), p);
    return true;
}

void PerfRecordMmap2::DumpData(int indent) const
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        PRINT_INDENT(indent, "pid %u, tid %u, addr 0x%llx, len 0x%llx\n", data_.pid, data_.tid,
                     data_.addr, data_.len);
        PRINT_INDENT(indent, "pgoff 0x%llx, maj %u, min %u, ino %llu, ino_generation %llu\n",
                     data_.pgoff, data_.maj, data_.min, data_.ino, data_.ino_generation);
        PRINT_INDENT(indent, "prot %u, flags %u, filename %s\n", data_.prot, data_.flags,
                     data_.filename);
    }
#endif
}

void PerfRecordMmap2::DumpLog(const std::string &prefix) const
{
    HLOGV("%s:  MMAP2: size %d pid %u tid %u dso '%s' (0x%llx-0x%llx)@0x%llx", prefix.c_str(),
          header_.size, data_.pid, data_.tid, data_.filename, data_.addr, data_.addr + data_.len,
          data_.pgoff);
}

bool PerfRecordLost::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordLostData *>(p);
    *pDest = data_;

    return true;
}

void PerfRecordLost::DumpData(int indent) const
{
    PRINT_INDENT(indent, "id %llu, lost %llu\n", data_.id, data_.lost);
}

void PerfRecordComm::Init(uint8_t* data, const perf_event_attr&)
{
    PerfEventRecordTemplate::Init(data);
    data_.comm[KILO - 1] = '\0';
}

PerfRecordComm::PerfRecordComm(bool inKernel, u32 pid, u32 tid, const std::string &comm)
{
    PerfEventRecord::Init(PERF_RECORD_COMM, inKernel);
    data_.pid = pid;
    data_.tid = tid;
    if (strncpy_s(data_.comm, KILO, comm.c_str(), comm.size()) != 0) {
        HLOGE("strncpy_s failed !!!");
    }

    header_.size = sizeof(header_) + sizeof(data_) - KILO + comm.size() + 1;
}

bool PerfRecordComm::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    // data_.comm[] is variable-length
    std::copy(reinterpret_cast<const uint8_t *>(&data_),
              reinterpret_cast<const uint8_t *>(&data_) + GetSize() - GetHeaderSize(), p);

    return true;
}

void PerfRecordComm::DumpData(int indent) const
{
    PRINT_INDENT(indent, "pid %u, tid %u, comm %s\n", data_.pid, data_.tid, data_.comm);
}

void PerfRecordComm::DumpLog(const std::string &prefix) const
{
    HLOGV("pid %u, tid %u, comm %s\n", data_.pid, data_.tid, data_.comm);
}

PerfRecordSample::PerfRecordSample(const PerfRecordSample& sample)
{
    header_ = sample.header_;
    data_ = sample.data_;

    sampleType_ = sample.sampleType_;
    skipKernel_ = sample.skipKernel_;
    skipPid_ = sample.skipPid_;

    ips_ = sample.ips_;
    callFrames_ = sample.callFrames_;
    serverPidMap_ = sample.serverPidMap_;

    stackId_ = sample.stackId_;
    removeStack_ = sample.removeStack_;
}

void PerfRecordSample::Init(uint8_t *p, const perf_event_attr &attr)
{
    PerfEventRecord::InitHeader(p);

    HLOG_ASSERT(p != nullptr);
    // clear the vector data
    Clean();
    sampleType_ = attr.sample_type;
    skipKernel_ = 0;
    skipPid_ = 0;
    stackId_ = {0};
    removeStack_ = false;
    data_ = {};
    uint8_t *start = p;
    u64 dataSize = static_cast<u64>(RECORD_SIZE_LIMIT);
    SetPointerOffset(p, sizeof(header_), dataSize);

    // parse record according SAMPLE_TYPE
    PopFromBinary(sampleType_ & PERF_SAMPLE_IDENTIFIER, p, data_.sample_id, dataSize);
    PopFromBinary(sampleType_ & PERF_SAMPLE_IP, p, data_.ip, dataSize);
    PopFromBinary2(sampleType_ & PERF_SAMPLE_TID, p, data_.pid, data_.tid, dataSize);
    PopFromBinary(sampleType_ & PERF_SAMPLE_TIME, p, data_.time, dataSize);
    PopFromBinary(sampleType_ & PERF_SAMPLE_ADDR, p, data_.addr, dataSize);
    PopFromBinary(sampleType_ & PERF_SAMPLE_ID, p, data_.id, dataSize);
    PopFromBinary(sampleType_ & PERF_SAMPLE_STREAM_ID, p, data_.stream_id, dataSize);
    PopFromBinary2(sampleType_ & PERF_SAMPLE_CPU, p, data_.cpu, data_.res, dataSize);
    PopFromBinary(sampleType_ & PERF_SAMPLE_PERIOD, p, data_.period, dataSize);
    PopFromBinary(sampleType_ & PERF_SAMPLE_CALLCHAIN, p, data_.nr, dataSize);
    if (data_.nr > 0) {
        // the pointer is from input(p), require caller keep input(p) with *this together
        // think it in next time
        data_.ips = reinterpret_cast<u64 *>(p);
        SetPointerOffset(p, data_.nr * sizeof(u64), dataSize);
    }
    PopFromBinary(sampleType_ & PERF_SAMPLE_RAW, p, data_.raw_size, dataSize);
    if (data_.raw_size > 0) {
        data_.raw_data = p;
        SetPointerOffset(p, data_.raw_size * sizeof(u8), dataSize);
    }
    PopFromBinary(sampleType_ & PERF_SAMPLE_BRANCH_STACK, p, data_.bnr, dataSize);
    if (data_.bnr > 0) {
        data_.lbr = reinterpret_cast<PerfBranchEntry *>(p);
        SetPointerOffset(p, data_.bnr * sizeof(PerfBranchEntry), dataSize);
    }
    PopFromBinary(sampleType_ & PERF_SAMPLE_REGS_USER, p, data_.user_abi, dataSize);
    if (data_.user_abi > 0) {
        data_.reg_mask = attr.sample_regs_user;
        data_.reg_nr = __builtin_popcountll(data_.reg_mask);
        data_.user_regs = reinterpret_cast<u64 *>(p);
        SetPointerOffset(p, data_.reg_nr * sizeof(u64), dataSize);
    }
    PopFromBinary(sampleType_ & PERF_SAMPLE_SERVER_PID, p, data_.server_nr, dataSize);
    if (data_.server_nr > 0) {
        data_.server_pids = reinterpret_cast<u64 *>(p);
        SetPointerOffset(p, data_.server_nr * sizeof(u64), dataSize);
    }
    PopFromBinary(sampleType_ & PERF_SAMPLE_STACK_USER, p, data_.stack_size, dataSize);
    if (data_.stack_size > 0) {
        data_.stack_data = p;
        SetPointerOffset(p, data_.stack_size, dataSize);
        PopFromBinary(true, p, data_.dyn_size, dataSize);
    }
    uint32_t remain = header_.size - (p - start);
    if (data_.nr == 0 && dumpRemoveStack_ && remain == sizeof(stackId_)) {
        PopFromBinary(true, p, stackId_.value, dataSize);
    }
}

void PerfRecordSample::SetDumpRemoveStack(bool dumpRemoveStack)
{
    dumpRemoveStack_ = dumpRemoveStack;
}

bool PerfRecordSample::IsDumpRemoveStack()
{
    return dumpRemoveStack_;
}

bool PerfRecordSample::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    PushToBinary(sampleType_ & PERF_SAMPLE_IDENTIFIER, p, data_.sample_id);
    PushToBinary(sampleType_ & PERF_SAMPLE_IP, p, data_.ip);
    PushToBinary2(sampleType_ & PERF_SAMPLE_TID, p, data_.pid, data_.tid);
    PushToBinary(sampleType_ & PERF_SAMPLE_TIME, p, data_.time);
    PushToBinary(sampleType_ & PERF_SAMPLE_ADDR, p, data_.addr);
    PushToBinary(sampleType_ & PERF_SAMPLE_ID, p, data_.id);
    PushToBinary(sampleType_ & PERF_SAMPLE_STREAM_ID, p, data_.stream_id);
    PushToBinary2(sampleType_ & PERF_SAMPLE_CPU, p, data_.cpu, data_.res);
    PushToBinary(sampleType_ & PERF_SAMPLE_PERIOD, p, data_.period);
    PushToBinary(sampleType_ & PERF_SAMPLE_CALLCHAIN, p, data_.nr);
    if (data_.nr > 0 && !removeStack_) {
        std::copy(data_.ips + skipKernel_, data_.ips + data_.nr + skipKernel_,
                  reinterpret_cast<u64 *>(p));
        p += data_.nr * sizeof(u64);
    }
    PushToBinary(sampleType_ & PERF_SAMPLE_RAW, p, data_.raw_size);
    if (data_.raw_size > 0) {
        std::copy(data_.raw_data, data_.raw_data + data_.raw_size, p);
        p += data_.raw_size * sizeof(u8);
    }
    PushToBinary(sampleType_ & PERF_SAMPLE_BRANCH_STACK, p, data_.bnr);
    if (data_.bnr > 0) {
        std::copy(data_.lbr, data_.lbr + data_.bnr, reinterpret_cast<PerfBranchEntry *>(p));
        p += data_.bnr * sizeof(PerfBranchEntry);
    }
    PushToBinary(sampleType_ & PERF_SAMPLE_REGS_USER, p, data_.user_abi);
    if (data_.user_abi > 0 && data_.reg_nr > 0) {
        std::copy(data_.user_regs, data_.user_regs + data_.reg_nr, reinterpret_cast<u64 *>(p));
        p += data_.reg_nr * sizeof(u64);
    }
    PushToBinary(sampleType_ & PERF_SAMPLE_SERVER_PID, p, data_.server_nr);
    if (data_.server_nr > 0) {
        std::copy(data_.server_pids + skipPid_, data_.server_pids + data_.server_nr + skipPid_,
                  reinterpret_cast<u64 *>(p));
        p += data_.server_nr * sizeof(u64);
    }
    PushToBinary(sampleType_ & PERF_SAMPLE_STACK_USER, p, data_.stack_size);
    if (data_.stack_size > 0) {
        std::copy(data_.stack_data, data_.stack_data + data_.stack_size, p);
        p += data_.stack_size * sizeof(u8);
        PushToBinary(true, p, data_.dyn_size);
    }
    PushToBinary(removeStack_, p, stackId_.value);
    return true;
}

void PerfRecordSample::DumpData(int indent) const
{
    PRINT_INDENT(indent, "sample_type: 0x%" PRIx64 "\n", sampleType_);

    // dump record according sampleType
    if (sampleType_ & (PERF_SAMPLE_ID | PERF_SAMPLE_IDENTIFIER)) {
        PRINT_INDENT(indent, "ID %" PRIu64 "\n", static_cast<uint64_t>(data_.sample_id));
    }
    if (sampleType_ & PERF_SAMPLE_IP) {
        PRINT_INDENT(indent, "ip %llx\n", data_.ip);
    }
    if (sampleType_ & PERF_SAMPLE_TID) {
        PRINT_INDENT(indent, "pid %u, tid %u\n", data_.pid, data_.tid);
    }
    if (sampleType_ & PERF_SAMPLE_TIME) {
        PRINT_INDENT(indent, "time %llu\n", data_.time);
    }
    if (sampleType_ & PERF_SAMPLE_ADDR) {
        PRINT_INDENT(indent, "addr %p\n", reinterpret_cast<void *>(data_.addr));
    }
    if (sampleType_ & PERF_SAMPLE_STREAM_ID) {
        PRINT_INDENT(indent, "stream_id %" PRIu64 "\n", static_cast<uint64_t>(data_.stream_id));
    }
    if (sampleType_ & PERF_SAMPLE_CPU) {
        PRINT_INDENT(indent, "cpu %u, res %u\n", data_.cpu, data_.res);
    }
    if (sampleType_ & PERF_SAMPLE_PERIOD) {
        PRINT_INDENT(indent, "period %" PRIu64 "\n", static_cast<uint64_t>(data_.period));
    }
    if (stackId_.section.id > 0) {
        PRINT_INDENT(indent, "stackid %" PRIu64 "\n", static_cast<uint64_t>(stackId_.section.id));
    }
    if (sampleType_ & PERF_SAMPLE_CALLCHAIN) {
        bool userContext = false;
        PRINT_INDENT(indent, "callchain nr=%lld\n", data_.nr);
        for (uint64_t i = 0; i < data_.nr; ++i) {
            std::string_view supplement = "";
            if ((sampleType_ & PERF_SAMPLE_STACK_USER) == 0 || data_.ips[i] != PERF_CONTEXT_USER) {
                PRINT_INDENT(indent + 1, "0x%llx%s\n", data_.ips[i], supplement.data());
                continue;
            }
            // is PERF_SAMPLE_STACK_USER type and is PERF_CONTEXT_USER
            if (!userContext) {
                userContext = true;
                supplement = " <unwind callstack>";
            } else {
                supplement = " <expand callstack>";
            }
            PRINT_INDENT(indent + 1, "0x%llx%s\n", data_.ips[i], supplement.data());
        }
    }
    if (sampleType_ & PERF_SAMPLE_RAW) {
        PRINT_INDENT(indent, "raw size=%u\n", data_.raw_size);
        const uint32_t *data = reinterpret_cast<const uint32_t *>(data_.raw_data);
        size_t size = data_.raw_size / sizeof(uint32_t);
        for (size_t i = 0; i < size; ++i) {
            PRINT_INDENT(indent + 1, "0x%08x (%x)\n", data[i], data[i]);
        }
    }
    if (sampleType_ & PERF_SAMPLE_BRANCH_STACK) {
        PRINT_INDENT(indent, "branch_stack nr=%lld\n", data_.bnr);
        for (uint64_t i = 0; i < data_.bnr; ++i) {
            auto &item = data_.lbr[i];
            PRINT_INDENT(indent + 1, "from 0x%llx, to 0x%llx, flags 0x%llx\n", item.from, item.to, item.flags);
        }
    }
    if (sampleType_ & PERF_SAMPLE_REGS_USER) {
        PRINT_INDENT(indent, "user regs: abi=%lld, reg_nr=%lld\n", data_.user_abi, data_.reg_nr);
#if defined(is_ohos) && is_ohos
        if (IsRoot()) {
            for (uint64_t i = 0; i < data_.reg_nr; ++i) {
                PRINT_INDENT(indent + 1, "0x%llx\n", data_.user_regs[i]);
            }
        }
#endif
    }
    if (sampleType_ & PERF_SAMPLE_SERVER_PID) {
        PRINT_INDENT(indent, "server nr=%lld\n", data_.server_nr);
        for (uint64_t i = 0; i < data_.server_nr; ++i) {
            PRINT_INDENT(indent + 1, "pid: %llu\n", data_.server_pids[i]);
        }
    }
    if (sampleType_ & PERF_SAMPLE_STACK_USER) {
        PRINT_INDENT(indent, "user stack: size %llu dyn_size %lld\n", data_.stack_size,
                     data_.dyn_size);
    }
}

void PerfRecordSample::DumpLog(const std::string &prefix) const
{
    HLOGV("%s: SAMPLE: id= %llu size %d pid %u tid %u ips %llu regs %llu, stacks %llu time %llu",
          prefix.c_str(), data_.sample_id, header_.size, data_.pid, data_.tid, data_.nr,
          data_.reg_nr, data_.dyn_size, data_.time);
}

void PerfRecordSample::RecoverCallStack()
{
    data_.ips = ips_.data();
    data_.nr = ips_.size();
    removeStack_ = true;
}

void PerfRecordSample::ReplaceWithCallStack(size_t originalSize)
{
    // first we check if we have some user unwind stack need to merge ?
    if (callFrames_.size() != 0) {
        // when we have some kernel ips , we cp it first
        // new size is user call frames + kernel call frames
        // + PERF_CONTEXT_USER(last + 1) + expand mark(also PERF_CONTEXT_USER)
        const unsigned int perfContextSize = 2;
        ips_.reserve(data_.nr + callFrames_.size() + perfContextSize);
        if (data_.nr > 0) {
            ips_.assign(data_.ips, data_.ips + data_.nr);
        }
        // add user context mark
        ips_.emplace_back(PERF_CONTEXT_USER);
        // we also need make a expand mark just for debug only
        const size_t beginIpsSize = ips_.size();
        bool ret = std::all_of(callFrames_.begin(), callFrames_.end(), [&](const DfxFrame &frame) {
            ips_.emplace_back(frame.pc);
            if (originalSize != 0 && (originalSize != callFrames_.size()) &&
                ips_.size() == (originalSize + beginIpsSize)) {
                // just for debug
                // so we can see which frame begin is expand call frames
                ips_.emplace_back(PERF_CONTEXT_USER);
            }
            return true;
        });
        if (ret) {
            HLOGV("combed %zu", callFrames_.size());
        } else {
            HLOGV("failed to combed %zu", callFrames_.size());
        }

        if (sampleType_ & PERF_SAMPLE_REGS_USER) {
            header_.size -= data_.reg_nr * sizeof(u64);
            data_.reg_nr = 0;
            data_.user_abi = 0;
        }

        if (sampleType_ & PERF_SAMPLE_STACK_USER) {
            // 1. remove the user stack
            header_.size -= data_.stack_size;
            header_.size -= sizeof(data_.dyn_size);

            // 2. clean the size
            data_.stack_size = 0;
            data_.dyn_size = 0;
        }

        if (sampleType_ & PERF_SAMPLE_CALLCHAIN) {
            HLOGV("ips change from %llu -> %zu", data_.nr, ips_.size());

            // 3. remove the nr size
            header_.size -= data_.nr * sizeof(u64);

            // 4. add new nr size
            data_.nr = ips_.size();
            header_.size += data_.nr * sizeof(u64);

            // 5. change ips potin to our ips array and hold it.
            data_.ips = ips_.data();
        }
    }
}

inline pid_t PerfRecordSample::GetPid() const
{
    return data_.pid;
}

uint64_t PerfRecordSample::GetTime() const
{
    return data_.time;
}

void PerfRecordSample::Clean()
{
    ips_.clear();
    callFrames_.clear();
    serverPidMap_.clear();
}

pid_t PerfRecordSample::GetUstackServerPid()
{
    if (!data_.server_nr) {
        return data_.pid;
    }

    size_t currServer = 0;
    // ipNr == 1...nr: server_pid of data_.ips[nr]
    for (size_t i = 0; i < data_.nr; i++) {
        // context change, use next server pid
        if (data_.ips[i] >= PERF_CONTEXT_MAX) {
            currServer++;
        }
    }
    // ipNr == nr + 1: server_pid of ustack
    if (currServer > 0) {
        currServer++;
    }
    if (currServer >= data_.server_nr) {
        HLOGE("ustack server pid nr %zu out of range", currServer);
        return data_.pid;
    }

    // return server pid
    return data_.server_pids[currServer];
}

pid_t PerfRecordSample::GetServerPidof(unsigned int ipNr)
{
    if (!data_.server_nr) {
        return data_.pid;
    }

    // init serverPidMap_
    if (!serverPidMap_.size()) {
        size_t currServer = 0;
        // ipNr == 0: server_pid of data_.ip
        serverPidMap_.emplace_back(data_.server_pids[currServer]);
        // ipNr == 1...nr: server_pid of data_.ips[nr]
        for (size_t i = 1; i < data_.nr; i++) {
            // context change, use next server pid
            if (data_.ips[i] >= PERF_CONTEXT_MAX) {
                currServer++;
            }
            if (currServer >= data_.server_nr) {
                HLOGE("callchain server pid nr %zu out of range", currServer);
                break;
            }
            serverPidMap_.emplace_back(data_.server_pids[currServer]);
        }
    }

    // return server pid
    if (ipNr >= serverPidMap_.size()) {
        return data_.pid;
    } else {
        return serverPidMap_[ipNr];
    }
}

bool PerfRecordExit::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordExitData *>(p);
    *pDest = data_;
    return true;
}

void PerfRecordExit::DumpData(int indent) const
{
    PRINT_INDENT(indent, "pid %u, ppid %u, tid %u, ptid %u time 0x%llx\n", data_.pid, data_.ppid,
                 data_.tid, data_.ptid, data_.time);
}

bool PerfRecordThrottle::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordThrottleData *>(p);
    *pDest = data_;
    return true;
}

void PerfRecordThrottle::DumpData(int indent) const
{
    PRINT_INDENT(indent, "time 0x%llx, id %llx, stream_id %llx\n", data_.time, data_.id,
                 data_.stream_id);
}

bool PerfRecordUnthrottle::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordThrottleData *>(p);
    *pDest = data_;
    return true;
}

void PerfRecordUnthrottle::DumpData(int indent) const
{
    PRINT_INDENT(indent, "time 0x%llx, id %llx, stream_id %llx\n", data_.time, data_.id,
                 data_.stream_id);
}

bool PerfRecordFork::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordForkData *>(p);
    *pDest = data_;
    return true;
}

void PerfRecordFork::DumpData(int indent) const
{
    PRINT_INDENT(indent, "pid %u, ppid %u, tid %u, ptid %u\n", data_.pid, data_.ppid, data_.tid,
                 data_.ptid);
}

bool PerfRecordRead::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordReadData *>(p);
    *pDest = data_;
    return true;
}

void PerfRecordRead::DumpData(int indent) const
{
    PRINT_INDENT(indent, "pid %u, tid %u\n", data_.pid, data_.tid);
    PRINT_INDENT(indent, "values: value %llx, timeEnabled %llx, timeRunning %llx, id %llx\n",
                 data_.values.value, data_.values.timeEnabled, data_.values.timeRunning, data_.values.id);
}

bool PerfRecordAux::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    PushToBinary(true, p, data_.aux_offset);
    PushToBinary(true, p, data_.aux_size);
    PushToBinary(true, p, data_.flags);
    PushToBinary2(sampleType_ & PERF_SAMPLE_TID, p, data_.sample_id.pid, data_.sample_id.tid);
    PushToBinary(sampleType_ & PERF_SAMPLE_TIME, p, data_.sample_id.time);
    PushToBinary(sampleType_ & PERF_SAMPLE_ID, p, data_.sample_id.id);
    PushToBinary(sampleType_ & PERF_SAMPLE_STREAM_ID, p, data_.sample_id.stream_id);

    PushToBinary2(sampleType_ & PERF_SAMPLE_CPU, p, data_.sample_id.cpu, data_.sample_id.res);
    PushToBinary(sampleType_ & PERF_SAMPLE_IDENTIFIER, p, data_.sample_id.id2);
    return true;
}

void PerfRecordAux::DumpData(int indent) const
{
    PRINT_INDENT(indent, "aux_offset 0x%llx aux_size 0x%llx flags 0x%llx pid %u tid %u time %llu",
                 data_.aux_offset, data_.aux_size, data_.flags, data_.sample_id.pid, data_.sample_id.tid,
                 data_.sample_id.time);
}

bool PerfRecordAuxTraceInfo::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();
    std::copy(reinterpret_cast<const uint8_t *>(&data_),
              reinterpret_cast<const uint8_t *>(&data_) + GetSize() - GetHeaderSize(), p);
    return true;
}

void PerfRecordAuxTraceInfo::DumpData(int indent) const
{
    PRINT_INDENT(indent, "aux_trace_event");
}

bool PerfRecordTimeConv::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();
    std::copy(reinterpret_cast<const uint8_t *>(&data_),
              reinterpret_cast<const uint8_t *>(&data_) + GetSize() - GetHeaderSize(), p);
    return true;
}

void PerfRecordTimeConv::DumpData(int indent) const
{
    PRINT_INDENT(indent, "aux_time_event");
}

bool PerfRecordCpuMap::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();
    std::copy(reinterpret_cast<const uint8_t *>(&data_),
              reinterpret_cast<const uint8_t *>(&data_) + GetSize() - GetHeaderSize(), p);
    return true;
}

void PerfRecordCpuMap::DumpData(int indent) const
{
    PRINT_INDENT(indent, "cpu_map_event");
}

bool PerfRecordItraceStart::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordItraceStartData *>(p);
    *pDest = data_;
    return true;
}

void PerfRecordItraceStart::DumpData(int indent) const
{
    PRINT_INDENT(indent, "pid %u, tid %u\n", data_.pid, data_.tid);
}

bool PerfRecordLostSamples::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordLostSamplesData *>(p);
    *pDest = data_;
    return true;
}

void PerfRecordLostSamples::DumpData(int indent) const
{
    PRINT_INDENT(indent, "lost %llu\n", data_.lost);
}

bool PerfRecordSwitch::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordSwitchData *>(p);
    *pDest = data_;
    return true;
}

bool PerfRecordSwitchCpuWide::GetBinary(std::vector<uint8_t> &buf) const
{
    if (buf.size() < GetSize()) {
        buf.resize(GetSize());
    }

    GetHeaderBinary(buf);
    uint8_t *p = buf.data() + GetHeaderSize();

    auto pDest = reinterpret_cast<PerfRecordSwitchCpuWideData *>(p);
    *pDest = data_;
    return true;
}

void PerfRecordSwitchCpuWide::DumpData(int indent) const
{
    PRINT_INDENT(indent, "next_prev_pid %u, next_prev_tid %u\n", data_.next_prev_pid,
                 data_.next_prev_tid);
}

PerfEventRecord& PerfEventRecordFactory::GetPerfEventRecord(PerfRecordType type, uint8_t* data,
                                                            const perf_event_attr &attr)
{
    HLOG_ASSERT(data != nullptr);
    PerfEventRecord* record = nullptr;
    auto it = recordMap_.find(type);
    if (it == recordMap_.end()) {
        record = CreatePerfEventRecord(type);
        if (record == nullptr) {
            record = new PerfRecordNull();
        }
        recordMap_.emplace(type, record);
    } else {
        record = it->second;
    }
    record->Init(data, attr);
    return *record;
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
