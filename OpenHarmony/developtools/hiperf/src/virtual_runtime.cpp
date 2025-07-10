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
#define HILOG_TAG "Runtime"

#include "virtual_runtime.h"

#include <cinttypes>
#include <iostream>
#include <sstream>
#include <unistd.h>
#if !is_mingw
#include <sys/mman.h>
#endif

#include "dfx_map.h"
#include "hiperf_hilog.h"
#include "register.h"
#include "spe_decoder.h"
#include "symbols_file.h"
#include "utilities.h"

using namespace std::chrono;
namespace OHOS {
namespace Developtools {
namespace HiPerf {
namespace {
// if ip is 0 , 1 both not useful
const uint64_t BAD_IP_ADDRESS = 2;
}
// we unable to access 'swapper' from /proc/0/
VirtualRuntime::VirtualRuntime(bool onDevice)
{
    UpdateThread(0, 0, "swapper");
}

VirtualRuntime::~VirtualRuntime()
{
    if (savedCmdLines_.is_open()) {
        savedCmdLines_.close();
    }
}

std::string VirtualRuntime::ReadFromSavedCmdLines(pid_t tid)
{
    if (!savedCmdLines_.is_open()) {
        savedCmdLines_.open(SAVED_CMDLINES, std::ios::in);
    }
    if (!savedCmdLines_.is_open()) {
        return EMPTY_STRING;
    }
    savedCmdLines_.seekg(0, std::ios::beg);
    std::string line;
    std::string threadid = std::to_string(tid);
    while (getline(savedCmdLines_, line)) {
        if (line.find(threadid) != std::string::npos) {
            constexpr size_t sizeLimit {2};
            std::vector<std::string> linesToken = StringSplit(line, " ");
            if (linesToken.size() < sizeLimit) {
                return EMPTY_STRING;
            }
            if (threadid != linesToken[0]) {
                continue;
            }
            return linesToken[1];
        }
    }
    return EMPTY_STRING;
}

std::string VirtualRuntime::ReadThreadName(pid_t tid, bool isThread)
{
    std::string comm = "";
    if (tid == SYSMGR_PID) {
        comm = SYSMGR_NAME;
    } else if (tid == devhostPid_) {
        comm = DEVHOST_FILE_NAME;
    } else if (isThread) {
        comm = ReadFileToString(StringPrintf("/proc/%d/comm", tid));
    } else {
        comm = ReadFileToString(StringPrintf("/proc/%d/cmdline", tid));
    }
    if (comm == EMPTY_STRING) {
        comm = ReadFromSavedCmdLines(tid);
    }
    comm.erase(std::remove(comm.begin(), comm.end(), '\r'), comm.end());
    comm.erase(std::remove(comm.begin(), comm.end(), '\n'), comm.end());
    return comm;
}

VirtualThread &VirtualRuntime::UpdateThread(pid_t pid, pid_t tid, const std::string name)
{
#ifdef HIPERF_DEBUG_TIME
    const auto startTime = steady_clock::now();
#endif
    VirtualThread &thread = GetThread(pid, tid, name);
    if (!name.empty() && (thread.name_.empty() || !StringEndsWith(thread.name_, name))) {
        thread.name_ = name;
    }
#ifdef HIPERF_DEBUG_TIME
    updateThreadTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif
    return thread;
}

VirtualThread &VirtualRuntime::CreateThread(pid_t pid, pid_t tid, const std::string name)
{
    // make a new one
    if (pid == tid) {
        userSpaceThreadMap_.emplace(std::piecewise_construct, std::forward_as_tuple(tid),
                                    std::forward_as_tuple(pid, symbolsFiles_));
    } else {
        // for thread we need give it process info( for same mmap)
        userSpaceThreadMap_.emplace(
            std::piecewise_construct, std::forward_as_tuple(tid),
            std::forward_as_tuple(pid, tid, GetThread(pid, pid), symbolsFiles_));
    }
    VirtualThread &thread = userSpaceThreadMap_.at(tid);
    if (recordCallBack_) {
        if (pid == tid && !IsKernelThread(pid)) {
#ifdef HIPERF_DEBUG_TIME
            const auto startTime = steady_clock::now();
#endif
            thread.ParseMap();
#ifdef HIPERF_DEBUG_TIME
            threadParseMapsTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif
        }
#ifdef HIPERF_DEBUG_TIME
        const auto startCreateMmapTime = steady_clock::now();
#endif
        thread.name_ = name;
        if (thread.name_.empty()) {
            thread.name_ = ReadThreadName(tid, pid != tid);
        }
        HLOGD("create a new thread record for %u:%u:%s with %zu dso", pid, tid,
              thread.name_.c_str(), thread.GetMaps().size());
        // we need make a PerfRecordComm
        auto commRecord = std::make_unique<PerfRecordComm>(IsKernelThread(pid), pid, tid, thread.name_);
        recordCallBack_(*commRecord);
        // only work for pid
        if (pid == tid) {
            if (isHM_) {
                thread.FixHMBundleMap();
            }
            std::shared_ptr<DfxMap> prevMap = nullptr;
            for (auto &map : thread.GetMaps()) {
                // so in hap is load before start perf record
                // dynamic load library should be treat in the same way
                bool updateNormalSymbol = true;
                if (map->name.find(".hap") != std::string::npos && (map->prots & PROT_EXEC)) {
                    map->prevMap = prevMap;
                    updateNormalSymbol = !UpdateHapSymbols(map);
                    HLOGD("UpdateHapSymbols");
                }
                auto mmapRecord =
                    std::make_unique<PerfRecordMmap2>(false, thread.pid_, thread.tid_, map);
                HLOGD("make PerfRecordMmap2 %d:%d:%s:%s(0x%" PRIx64 "-0x%" PRIx64 ")@%" PRIx64 " ",
                      thread.pid_, thread.tid_, thread.name_.c_str(), map->name.c_str(),
                      map->begin, map->end, map->offset);
                recordCallBack_(*mmapRecord);
                if (updateNormalSymbol) {
                    UpdateSymbols(map, pid);
                }
                prevMap = map;
            }
        }
        HLOGV("thread created");
#ifdef HIPERF_DEBUG_TIME
        threadCreateMmapTimes_ +=
            duration_cast<microseconds>(steady_clock::now() - startCreateMmapTime);
#endif
    }
    return thread;
}

bool VirtualRuntime::UpdateHapSymbols(std::shared_ptr<DfxMap> map)
{
    if (map == nullptr) {
        return false;
    }
    HLOGV("hap name:%s", map->name.c_str());
    // found it by name
    auto symbolsFile = SymbolsFile::CreateSymbolsFile(map->name);
    CHECK_TRUE(symbolsFile == nullptr, false, 1,
               "Failed to load CreateSymbolsFile for exec section in hap(%s)", map->name.c_str());
    symbolsFile->SetMapsInfo(map);
    // update maps name if load debuginfo successfully
    CHECK_TRUE(!symbolsFile->LoadDebugInfo(map), false, 1,
               "Failed to load debuginfo for exec section in hap(%s)", map->name.c_str());

    if (!loadSymboleWhenNeeded_) { // todo misspelling
        symbolsFile->LoadSymbols(map);
    }
    symbolsFiles_.emplace_back(std::move(symbolsFile));
    return true;
}

VirtualThread &VirtualRuntime::GetThread(pid_t pid, pid_t tid, const std::string name)
{
    if (userSpaceThreadMap_.find(pid) == userSpaceThreadMap_.end()) {
        // no pid found
        // create process first
        CreateThread(pid, pid);
    }

    auto it = userSpaceThreadMap_.find(tid);
    if (it == userSpaceThreadMap_.end()) {
        // we also need thread
        return CreateThread(pid, tid, name);
    } else {
        return it->second;
    }
}

std::shared_ptr<DfxMap> VirtualRuntime::UpdateThreadMaps(pid_t pid, pid_t tid, const std::string filename,
                                                         uint64_t begin, uint64_t len, uint64_t offset, uint32_t prot)
{
    VirtualThread &thread = GetThread(pid, tid);
    std::shared_ptr<DfxMap> map = thread.CreateMapItem(filename, begin, len, offset, prot);
    if (isHM_) {
        thread.FixHMBundleMap();
    }
    return map;
}

void VirtualRuntime::UpdateKernelModulesSpaceMaps()
{
    // found the kernel modules
    std::vector<DfxMap> koMaps;
    std::ifstream ifs("/proc/modules", std::ifstream::in);
    if (!ifs.is_open()) {
        perror("kernel modules read failed(/proc/modules)\n");
        return;
    }
    std::string line;
    while (getline(ifs, line)) {
        uint64_t addr = 0;
        uint64_t size = 0;
        uint64_t lineSize = line.size();
        if (lineSize > 4096) { // 4096: line length
            continue;
        }
        char *module = new char[lineSize + 1];
        /*
        name       size  load     map
        hi_mipi_rx 53248 0 - Live 0xbf109000 (O)
        hi3516cv500_hdmi 237568 0 - Live 0xbf0bb000 (O)
        hifb 143360 0 - Live 0xbf089000 (O)
        hi3516cv500_vo_dev 98304 0 - Live 0xbf070000 (O)
        hi3516cv500_tde 110592 0 - Live 0xbf04a000 (O)
        hi3516cv500_sys 36864 0 - Live 0xbf03a000 (O)
        hi3516cv500_base 20480 5
        hi_mipi_rx,hi3516cv500_hdmi,hifb,hi3516cv500_vo_dev,hi3516cv500_tde,hi3516cv500_sys,
        hi3516cv500_base,sys_config,hi_proc,hi_irq,Live 0xbf000000 (O)
        */
        int ret = sscanf_s(line.c_str(), "%s%" PRIu64 "%*u%*s%*s 0x%" PRIx64 "", module,
                           lineSize, &size, &addr);
        constexpr int numSlices {3};
        if (ret == numSlices) {
            auto &map = koMaps.emplace_back(addr, addr + size, 0, "", std::string(module));
            HLOGV("add ko map %s", map.ToString().c_str());
        } else {
            HLOGE("unknown line %d: '%s'", ret, line.c_str());
        }
        delete []module;
    }

    if (std::all_of(koMaps.begin(), koMaps.end(),
                    [](const DfxMap &item) { return item.begin == 0; })) {
        koMaps.clear();
        HLOGW("no addr found in /proc/modules. remove all the ko");
    }
    if (recordCallBack_) {
        for (const auto &map : koMaps) {
            auto record = std::make_unique<PerfRecordMmap>(true, 0, 0, map.begin,
                                                           map.end - map.begin, 0, map.name);
            recordCallBack_(*record);
        }
    }
    std::move(koMaps.begin(), koMaps.end(), std::back_inserter(kernelSpaceMemMaps_));
}

void VirtualRuntime::UpdateKernelSpaceMaps()
{
    // add kernel first
    auto &map = kernelSpaceMemMaps_.emplace_back(0, std::numeric_limits<uint64_t>::max(), 0, "", KERNEL_MMAP_NAME);
    if (recordCallBack_) {
        auto record = std::make_unique<PerfRecordMmap>(true, 0, 0, map.begin,
                                                       map.end - map.begin, 0, map.name);
        recordCallBack_(*record);
    }
}

void VirtualRuntime::UpdateKernelModulesSymbols()
{
    HLOGD("load ko symbol and build id");
    for (auto &map : kernelSpaceMemMaps_) {
        if (map.name == KERNEL_MMAP_NAME) {
            continue;
        }
        auto kernelModuleFile = SymbolsFile::CreateSymbolsFile(SYMBOL_KERNEL_MODULE_FILE, map.name);
        if (symbolsPaths_.size() > 0) {
            kernelModuleFile->setSymbolsFilePath(symbolsPaths_); // also load from search path
        }
        kernelModuleFile->LoadSymbols();
        symbolsFiles_.emplace_back(std::move(kernelModuleFile));
    }
}

void VirtualRuntime::UpdateKernelSymbols()
{
    HLOGD("create a kernel mmap record");
    // found kernel source
    auto kernelFile = SymbolsFile::CreateSymbolsFile(KERNEL_MMAP_NAME);
    // set symbol path If it exists
    if (symbolsPaths_.size() > 0) {
        kernelFile->setSymbolsFilePath(symbolsPaths_); // also load from search path
    }
    if (!IsRoot()) {
        HLOGD("user mode do not load kernel syms");
        printf("Hiperf is not running as root mode. Do not need load kernel syms\n");
    }
    if (kernelFile->LoadSymbols()) {
        auto record = std::make_unique<PerfRecordMmap>(
            true, 0, 0, kernelFile->textExecVaddr_, kernelFile->textExecVaddrRange_,
            kernelFile->textExecVaddrFileOffset_, KERNEL_MMAP_NAME);

        if (recordCallBack_) {
            recordCallBack_(*record);
        }
        symbolsFiles_.emplace_back(std::move(kernelFile));
    } else {
        HLOGW("kernel symbol not found.\n");
    }
}

void VirtualRuntime::UpdatekernelMap(uint64_t begin, uint64_t end, uint64_t offset,
                                     std::string filename)
{
    HLOGV("update kernel map name:'%s' 0x%" PRIx64 " - 0x%" PRIx64 "@0x%08" PRIx64 "",
          filename.c_str(), begin, end, offset);

    HLOG_ASSERT(!filename.empty());
    auto it = find(kernelSpaceMemMaps_.begin(), kernelSpaceMemMaps_.end(), filename);
    if (it == kernelSpaceMemMaps_.end()) {
        kernelSpaceMemMaps_.emplace_back(begin, end, offset, "", filename);
    } else {
        it->begin = begin;
        it->end = end;
        it->offset = offset;
        it->name = filename;
    }
}

void VirtualRuntime::DedupFromRecord(PerfRecordSample *recordSample)
{
    CHECK_TRUE(recordSample == nullptr, NO_RETVAL, 0, "");
    u64 nr = recordSample->data_.nr;
    if (nr == 0) {
        collectSymbolCallBack_(recordSample);
        return;
    }
    u32 pid = recordSample->data_.pid;
    u64 *ips = recordSample->data_.ips;
    StackId stackId;
    stackId.value = 0;
    auto entry = processStackMap_.find(pid);
    std::shared_ptr<UniqueStackTable> table = nullptr;
    if (entry != processStackMap_.end()) {
        table = entry->second;
    } else {
        table = std::make_shared<UniqueStackTable>(pid);
        processStackMap_[pid] = table;
    }
    CHECK_TRUE(table == nullptr, NO_RETVAL, 0, "");
    while (table->PutIpsInTable(&stackId, ips, nr) == 0) {
        // try expand hashtable if collison can not resolved
        if (!table->Resize()) {
            HLOGW("Hashtable size limit, ip compress failed!");
            collectSymbolCallBack_(recordSample);
            return;
        }
    }
    // callstack dedup success
    recordSample->stackId_.value = stackId.value;
    recordSample->header_.size -= (sizeof(u64) * nr - sizeof(stackId));
    recordSample->data_.nr = 0;
    recordSample->data_.ips = nullptr;
    recordSample->removeStack_ = true;
}

void VirtualRuntime::CollectDedupSymbol(kSymbolsHits &kernelSymbolsHits,
                                        uSymbolsHits &userSymbolsHits)
{
    Node *node = nullptr;
    Node *head = nullptr;
    u32 pid;
    for (const auto &tableEntry : processStackMap_) {
        const auto &table = tableEntry.second;
        if (table == nullptr) {
            continue;
        }
        pid = table->GetPid();
        head = table->GetHeadNode();
        const auto &idxes = table->GetUsedIndexes();
        for (const auto idx : idxes) {
            node = head + idx;
            if (node == nullptr) {
                continue;
            }
            if (node->value != 0) {
                if (node->section.inKernel) {
                    uint64_t ip = node->section.ip | KERNEL_PREFIX;
                    if (ip == PERF_CONTEXT_KERNEL || ip == PERF_CONTEXT_USER) {
                        continue;
                    }
                    kernelSymbolsHits.insert(ip);
                } else {
                    userSymbolsHits[pid].insert(node->section.ip);
                }
            } else {
                HLOGD("node value error 0x%x", idx);
            }
        }
    }
}

void VirtualRuntime::UpdateFromRecord(PerfEventRecord &record)
{
#ifdef HIPERF_DEBUG_TIME
    const auto startTime = steady_clock::now();
#endif
    if (record.GetType() == PERF_RECORD_SAMPLE) {
        auto recordSample = static_cast<PerfRecordSample *>(&record);
        UpdateFromRecord(*recordSample);
#ifdef HIPERF_DEBUG_TIME
        processSampleRecordTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif
    } else if (record.GetType() == PERF_RECORD_MMAP) {
        auto recordMmap = static_cast<PerfRecordMmap *>(&record);
        UpdateFromRecord(*recordMmap);
#ifdef HIPERF_DEBUG_TIME
        processMmapRecordTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif
    } else if (record.GetType() == PERF_RECORD_MMAP2) {
        auto recordMmap2 = static_cast<PerfRecordMmap2 *>(&record);
        UpdateFromRecord(*recordMmap2);
#ifdef HIPERF_DEBUG_TIME
        processMmap2RecordTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif
    } else if (record.GetType() == PERF_RECORD_COMM) {
        auto recordComm = static_cast<PerfRecordComm *>(&record);
        UpdateFromRecord(*recordComm);
#ifdef HIPERF_DEBUG_TIME
        processCommRecordTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif
    } else if (record.GetType() == PERF_RECORD_AUXTRACE) {
        auto recordAuxTrace = static_cast<PerfRecordAuxtrace *>(&record);
        UpdateFromRecord(*recordAuxTrace);
#ifdef HIPERF_DEBUG_TIME
        processCommRecordTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif
    } else {
        HLOGW("skip record type %d", record.GetType());
    }
}

void VirtualRuntime::MakeCallFrame(DfxSymbol &symbol, DfxFrame &callFrame)
{
    callFrame.funcOffset = symbol.funcVaddr_;
    callFrame.mapOffset = symbol.offsetToVaddr_;
    callFrame.symbolFileIndex = symbol.symbolFileIndex_;
    callFrame.funcName = symbol.GetName();
    if (callFrame.funcName.empty()) {
        HLOGD("callFrame.funcName:%s, GetName:%s\n", callFrame.funcName.c_str(), symbol.GetName().data());
    }

    callFrame.index = static_cast<size_t>(symbol.index_);
    callFrame.mapName = symbol.module_.empty() ? symbol.comm_ : symbol.module_;
    HLOG_ASSERT_MESSAGE(!callFrame.funcName.empty(), "%s", symbol.ToDebugString().c_str());
}

void VirtualRuntime::SymbolicCallFrame(PerfRecordSample &recordSample, uint64_t ip,
                                       pid_t serverPid, perf_callchain_context context)
{
    pid_t pid = static_cast<pid_t>(recordSample.data_.pid);
    pid_t tid = static_cast<pid_t>(recordSample.data_.tid);
    if (serverPid != pid) {
        pid = tid = serverPid;
    }
    auto symbol = GetSymbol(ip, pid, tid, context);
    MakeCallFrame(symbol, recordSample.callFrames_.emplace_back(ip, 0));
    HLOGV(" (%zu)unwind symbol: %*s%s", recordSample.callFrames_.size(),
          static_cast<int>(recordSample.callFrames_.size()), "",
          recordSample.callFrames_.back().ToSymbolString().c_str());
}

bool VirtualRuntime::RecoverCallStack(PerfRecordSample &recordSample)
{
    auto StackTable = processStackMap_.find(recordSample.data_.pid);
    CHECK_TRUE(StackTable == processStackMap_.end(), false, 1, "not found %" PRIu32 " pid", recordSample.data_.pid);
    recordSample.ips_.clear();
    if (StackTable->second != nullptr) {
        StackTable->second->GetIpsByStackId(recordSample.stackId_, recordSample.ips_);
    }
    recordSample.RecoverCallStack();
    return true;
}

void VirtualRuntime::SymbolicRecord(PerfRecordSample &recordSample)
{
#ifdef HIPERF_DEBUG_TIME
    const auto startTime = steady_clock::now();
#endif
    // Symbolic the Call Stack
    recordSample.callFrames_.clear();
    perf_callchain_context context = PERF_CONTEXT_MAX;
    pid_t serverPid;
    if (recordSample.data_.nr == 0) {
        serverPid = recordSample.GetServerPidof(0);
        SymbolicCallFrame(recordSample, recordSample.data_.ip, serverPid, PERF_CONTEXT_MAX);
    }
    for (u64 i = 0; i < recordSample.data_.nr; i++) {
        uint64_t ip = recordSample.data_.ips[i];
        if (ip >= PERF_CONTEXT_MAX) {
            std::string contextName = UpdatePerfContext(ip, context);
            HLOGV("context switch to %s", contextName.c_str());
            continue;
        } else if (ip < BAD_IP_ADDRESS) {
            // ip 0 or 1 or less than 0
            continue;
        }
        serverPid = recordSample.GetServerPidof(i);
        SymbolicCallFrame(recordSample, ip, serverPid, context);
    }
#ifdef HIPERF_DEBUG_TIME
    auto usedTime = duration_cast<microseconds>(steady_clock::now() - startTime);
    if (usedTime.count() != 0) {
        HLOGV("cost %0.3f ms to symbolic ", usedTime.count() / MS_DURATION);
    }
    symbolicRecordTimes_ += usedTime;
#endif
}

void VirtualRuntime::NeedDropKernelCallChain(PerfRecordSample &sample)
{
    // only do this in record mode.
    if (recordCallBack_ == nullptr || needkernelCallChain_ ||
        !sample.InKernel() || sample.data_.nr == 0) {
        return;
    }

    u64 skip = 0;
    u64 skipPid = 0;
    u64 *ips = sample.data_.ips;
    for (; skip < sample.data_.nr; skip++) {
        if (ips[skip] == PERF_CONTEXT_KERNEL) {
            skipPid++;
        }
        if (ips[skip] == PERF_CONTEXT_USER) {
            break;
        }
    }
    sample.skipKernel_ = skip;
    sample.data_.nr -= skip;
    sample.header_.size -= sizeof(u64) * skip;
    if (sample.data_.server_nr > 0) {
        sample.skipPid_ = skipPid;
        sample.data_.server_nr -= skipPid;
        sample.header_.size -= sizeof(u64) * skipPid;
    }
}

void VirtualRuntime::AdjustCallChain(PerfRecordSample &sample)
{
#if defined(is_ohos) && is_ohos
    if (!isHM_ || recordCallBack_ == nullptr) {
        return;
    }
    constexpr uint64_t minValue = 0x5;
    constexpr uint64_t offset = 0x4;
    for (u64 i = 0; i < sample.data_.nr; i++) {
        if (sample.data_.ips[i] >= PERF_CONTEXT_MAX) {
            i++;
            continue;
        }
        if (i >= 1 && sample.data_.ips[i] >= minValue) {
            sample.data_.ips[i] -= offset;
        }
    }
#endif
}

void VirtualRuntime::ProcessKernelCallChain(PerfRecordSample &sample)
{
#if defined(is_ohos) && is_ohos
    if (isRoot_) {
        return;
    }
    if (recordCallBack_ != nullptr) {
        if (sample.data_.ip >= 0xffff000000000000) {
            sample.data_.ip = sample.data_.ip & 0xffffff0000000fff;
        }
        for (u64 i = 0; i < sample.data_.nr; i++) {
            if (sample.data_.ips[i] >= PERF_CONTEXT_MAX) {
                continue;
            }
            if (sample.data_.ips[i] >= 0xffff000000000000) {
                sample.data_.ips[i] = sample.data_.ips[i] & 0xffffff0000000fff;
            }
        }
    }
#endif
}

void VirtualRuntime::UnwindFromRecord(PerfRecordSample &recordSample)
{
#if defined(is_ohos) && is_ohos
#ifdef HIPERF_DEBUG_TIME
    const auto startTime = steady_clock::now();
#endif
    HLOGV("unwind record (time:%llu)", recordSample.data_.time);
    // if we have userstack ?
    if (recordSample.data_.stack_size > 0) {
        pid_t serverPid = recordSample.GetUstackServerPid();
        pid_t pid = static_cast<pid_t>(recordSample.data_.pid);
        pid_t tid = static_cast<pid_t>(recordSample.data_.tid);
        if (serverPid != pid) {
            pid = tid = serverPid;
        }
        auto &thread = UpdateThread(pid, tid);
        callstack_.UnwindCallStack(thread, recordSample.data_.user_abi == PERF_SAMPLE_REGS_ABI_32,
                                   recordSample.data_.user_regs, recordSample.data_.reg_nr,
                                   recordSample.data_.stack_data, recordSample.data_.dyn_size,
                                   recordSample.callFrames_);
#ifdef HIPERF_DEBUG_TIME
        unwindCallStackTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif
        size_t oldSize = recordSample.callFrames_.size();
        HLOGV("unwind %zu", recordSample.callFrames_.size());
        callstack_.ExpandCallStack(thread.tid_, recordSample.callFrames_, callstackMergeLevel_);
        HLOGV("expand %zu (+%zu)", recordSample.callFrames_.size(),
              recordSample.callFrames_.size() - oldSize);

        recordSample.ReplaceWithCallStack(oldSize);
    }

#ifdef HIPERF_DEBUG_TIME
    unwindFromRecordTimes_ += duration_cast<microseconds>(steady_clock::now() - startTime);
#endif

    NeedDropKernelCallChain(recordSample);
    // we will not do this in non record mode.
    if (dedupStack_ && recordCallBack_ != nullptr) {
        DedupFromRecord(&recordSample);
    }
#endif

    // we will not do this in record mode
    if (recordCallBack_ == nullptr) {
        if (dedupStack_ && recordSample.stackId_.section.id > 0 && recordSample.data_.nr == 0) {
            RecoverCallStack(recordSample);
        }
        // find the symbols , reabuild frame info
        SymbolicRecord(recordSample);
    }
}

void VirtualRuntime::SetCollectSymbolCallBack(CollectSymbolCallBack collectSymbolCallBack)
{
    collectSymbolCallBack_ = collectSymbolCallBack;
}

void VirtualRuntime::UpdateFromRecord(PerfRecordSample &recordSample)
{
    UpdateThread(recordSample.data_.pid, recordSample.data_.tid);
    if (recordSample.data_.server_nr) {
        // update all server threads
        for (size_t i = 0; i < recordSample.data_.server_nr; i++) {
            pid_t pid = static_cast<pid_t>(recordSample.data_.server_pids[i]);
            UpdateThread(pid, pid);
        }
    }
    AdjustCallChain(recordSample);
    ProcessKernelCallChain(recordSample);
    // unwind
    if (disableUnwind_) {
        return;
    } else {
        UnwindFromRecord(recordSample);
    }
}

void VirtualRuntime::UpdateFromRecord(PerfRecordMmap &recordMmap)
{
    HLOGV("  MMAP: size %d pid %u tid %u", recordMmap.header_.size, recordMmap.data_.pid,
          recordMmap.data_.tid);
    HLOGV("  MMAP: %s dso '%s' (0x%llx-0x%llx)@0x%llx", recordMmap.InKernel() ? "kernel" : "user",
          recordMmap.data_.filename, recordMmap.data_.addr,
          recordMmap.data_.addr + recordMmap.data_.len, recordMmap.data_.pgoff);
    // kernel mmap
    // don't overwrite the vailed mmap , so we also check the recordMmap.data_.len
    if (IsKernelThread(recordMmap.data_.pid)) {
        UpdateKernelThreadMap(recordMmap.data_.pid, recordMmap.data_.addr,
                              recordMmap.data_.len, recordMmap.data_.pgoff, recordMmap.data_.filename);
    } else if (recordMmap.InKernel()) {
        UpdatekernelMap(recordMmap.data_.addr, recordMmap.data_.addr + recordMmap.data_.len,
                        recordMmap.data_.pgoff, recordMmap.data_.filename);
    } else {
        NeedAdaptSandboxPath(recordMmap.data_.filename, recordMmap.data_.pid, recordMmap.header_.size);
        FixHMBundleMmap(recordMmap.data_.filename, recordMmap.data_.pid, recordMmap.header_.size);
        auto map = UpdateThreadMaps(recordMmap.data_.pid, recordMmap.data_.tid, recordMmap.data_.filename,
                                    recordMmap.data_.addr, recordMmap.data_.len, recordMmap.data_.pgoff);
        UpdateSymbols(map, recordMmap.data_.pid);
    }
}

bool VirtualRuntime::CheckValidSandBoxMmap(PerfRecordMmap2 &recordMmap2)
{
    static std::shared_ptr<DfxMap> prevMap;
    if ((recordMmap2.data_.prot & PROT_EXEC) != 0) {
        // fake first segment, when second segment come.
        auto symFile = SymbolsFile::CreateSymbolsFile(SYMBOL_ELF_FILE, recordMmap2.data_.filename);
        CHECK_TRUE(symFile == nullptr, false, 1, "CheckValidSandBoxMmap Failed to create symbolFile!");

        std::shared_ptr<DfxMap> curMap;
        if (strstr(recordMmap2.data_.filename, ".hap") != nullptr) {
            curMap = std::make_shared<DfxMap>(
                recordMmap2.data_.addr,
                recordMmap2.data_.addr + recordMmap2.data_.len,
                recordMmap2.data_.pgoff,
                "", // prot
                recordMmap2.data_.filename
            );
            curMap->prevMap = prevMap;
        }

        CHECK_TRUE(!symFile->LoadDebugInfo(curMap), false, 1, "CheckValidSandBoxMmap Failed to load debuginfo!");

        if (!loadSymboleWhenNeeded_) {
            symFile->LoadSymbols(curMap);
        }

        if (strstr(recordMmap2.data_.filename, ".hap") == nullptr) {
            auto elfLoadInfoMap = symFile->GetPtLoads();
            u64 begin = recordMmap2.data_.addr - elfLoadInfoMap[0].mmapLen;
            u64 len = elfLoadInfoMap[0].mmapLen;
            u64 pgoff = elfLoadInfoMap[0].offset & (~(elfLoadInfoMap[0].align >= 1 ? elfLoadInfoMap[0].align - 1 : 0));
            std::unique_ptr<PerfRecordMmap2> mmap2FirstSeg =
                std::make_unique<PerfRecordMmap2>(recordMmap2.InKernel(), recordMmap2.data_.pid, recordMmap2.data_.tid,
                begin, len, pgoff, 0, 0, 0, PROT_READ, 0, std::string(recordMmap2.data_.filename));
            UpdateThreadMaps(mmap2FirstSeg->data_.pid, mmap2FirstSeg->data_.tid, mmap2FirstSeg->data_.filename,
                mmap2FirstSeg->data_.addr, mmap2FirstSeg->data_.len, mmap2FirstSeg->data_.pgoff);
            recordCallBack_(*mmap2FirstSeg);
        } else {
            auto elfLoadInfoMap = symFile->GetPtLoads();
            u64 begin = recordMmap2.data_.addr - elfLoadInfoMap[0].mmapLen;
            u64 len = elfLoadInfoMap[0].mmapLen;
            u64 pgoff = elfLoadInfoMap[0].offset &
                        (~(elfLoadInfoMap[0].align >= 1 ? elfLoadInfoMap[0].align - 1 : 0));
            std::unique_ptr<PerfRecordMmap2> mmap2FirstSeg =
                std::make_unique<PerfRecordMmap2>(recordMmap2.InKernel(), recordMmap2.data_.pid, recordMmap2.data_.tid,
                begin, len, pgoff, 0, 0, 0, PROT_READ, 0, curMap->name);
            UpdateThreadMaps(mmap2FirstSeg->data_.pid, mmap2FirstSeg->data_.tid, curMap->name,
                mmap2FirstSeg->data_.addr, mmap2FirstSeg->data_.len, mmap2FirstSeg->data_.pgoff);
            recordCallBack_(*mmap2FirstSeg);

            std::unique_ptr<PerfRecordMmap2> mmap2SecondSegment =
                std::make_unique<PerfRecordMmap2>(recordMmap2.InKernel(), recordMmap2.data_.pid, recordMmap2.data_.tid,
                recordMmap2.data_.addr,
                recordMmap2.data_.len,
                recordMmap2.data_.pgoff - prevMap->offset, // minus load offset of hap
                0, 0, 0, recordMmap2.data_.prot, 0, curMap->name);
            UpdateThreadMaps(mmap2SecondSegment->data_.pid, mmap2SecondSegment->data_.tid, curMap->name,
                mmap2SecondSegment->data_.addr, mmap2SecondSegment->data_.len, mmap2SecondSegment->data_.pgoff);
            recordCallBack_(*mmap2SecondSegment);
            recordMmap2.discard_ = true;
        }
        symbolsFiles_.emplace_back(std::move(symFile));
        return true;
    } else if (recordMmap2.data_.pgoff == 0) {
        recordMmap2.discard_ = true;
    }

    if (strstr(recordMmap2.data_.filename, ".hap") != nullptr) {
        prevMap = std::make_shared<DfxMap>(
            recordMmap2.data_.addr,
            recordMmap2.data_.addr + recordMmap2.data_.len,
            recordMmap2.data_.pgoff,
            "", // prot
            recordMmap2.data_.filename
        );
        HLOGD("CheckValidSandBoxMmap Update prev map!");
    }
    return !recordMmap2.discard_;
}

void VirtualRuntime::UpdateFromRecord(PerfRecordMmap2 &recordMmap2)
{
    if (!OHOS::HiviewDFX::DfxMaps::IsLegalMapItem(recordMmap2.data_.filename)) {
        return;
    }

    HLOGV("  MMAP2: size %d pid %u tid %u", recordMmap2.header_.size, recordMmap2.data_.pid,
          recordMmap2.data_.tid);
    HLOGV("  MMAP2: %s dso '%s' (0x%llx-0x%llx)@0x%llx prot:%u", recordMmap2.InKernel() ? "kernel" : "user",
          recordMmap2.data_.filename, recordMmap2.data_.addr,
          recordMmap2.data_.addr + recordMmap2.data_.len, recordMmap2.data_.pgoff, recordMmap2.data_.prot);

    if (recordCallBack_) {
        if (NeedAdaptSandboxPath(recordMmap2.data_.filename, recordMmap2.data_.pid, recordMmap2.header_.size)) {
            FixHMBundleMmap(recordMmap2.data_.filename, recordMmap2.data_.pid, recordMmap2.header_.size);
            CHECK_TRUE(!CheckValidSandBoxMmap(recordMmap2), NO_RETVAL, 0, "");
        }
    }
    auto map = UpdateThreadMaps(recordMmap2.data_.pid, recordMmap2.data_.tid, recordMmap2.data_.filename,
                                recordMmap2.data_.addr, recordMmap2.data_.len, recordMmap2.data_.pgoff,
                                recordMmap2.data_.prot);
    UpdateSymbols(map, recordMmap2.data_.pid);
}

void VirtualRuntime::UpdateFromRecord(PerfRecordComm &recordComm)
{
    recordComm.DumpLog(__FUNCTION__);
    UpdateThread(recordComm.data_.pid, recordComm.data_.tid, recordComm.data_.comm);
}

void VirtualRuntime::UpdateFromRecord(PerfRecordAuxtrace &recordAuxTrace)
{
    if (recordCallBack_ == nullptr) {
#if defined(is_ohos) && is_ohos
        recordAuxTrace.DumpLog(__FUNCTION__);
        SpeDecoder *decoder = SpeDecoderDataNew(recordAuxTrace.rawData_, recordAuxTrace.data_.size);
        CHECK_TRUE(decoder == nullptr, NO_RETVAL, 0, "");
        std::vector<SpeRecord> records;
        while (true) {
            int ret = SpeDecode(decoder);
            if (ret <= 0) {
                break;
            }
            struct SpeRecord record = SpeRecord(decoder->record);
            records.emplace_back(record);
        }
        std::vector<ReportItemAuxRawData> auxRawData;
        for (auto rec: records) {
            u64 pc = 0;
            if (rec.from_ip) {
                pc = rec.from_ip;
            } else if (rec.to_ip) {
                pc = rec.to_ip;
            } else {
                continue;
            }
            DfxSymbol symbol = GetSymbol(pc, recordAuxTrace.data_.reserved__, recordAuxTrace.data_.tid);
            HLOGV("pc 0x%llx symbol %s", pc, symbol.ToDebugString().c_str());
            struct ReportItemAuxRawData reportItem = {rec.type, 0.0f, 1, symbol.comm_.data(), pc,
                                                      symbol.module_.data(), symbol.GetName().data(),
                                                      symbol.fileVaddr_};
            auxRawData.emplace_back(reportItem);
            HLOGV("type %u, from_ip: 0x%llx, to_ip: 0x%llx, timestamp: %llu, virt_addr: 0x%llx, phys_addr: 0x%llx",
                  rec.type, rec.from_ip, rec.to_ip, rec.timestamp, rec.virt_addr, rec.phys_addr);
        }
        AddReportItems(auxRawData);
        SpeDecoderFree(decoder);
#endif
    }
}

void VirtualRuntime::SymbolSpeRecord(PerfRecordAuxtrace &recordAuxTrace)
{
#if defined(is_ohos) && is_ohos
    recordAuxTrace.DumpLog(__FUNCTION__);
    SpeDecoder *decoder = SpeDecoderDataNew(recordAuxTrace.rawData_, recordAuxTrace.data_.size);
    CHECK_TRUE(decoder == nullptr, NO_RETVAL, 0, "");
    while (true) {
        int ret = SpeDecode(decoder);
        if (ret <= 0) {
            break;
        }
        struct SpeRecord record = SpeRecord(decoder->record);
        u64 pc = 0;
        if (record.from_ip) {
            pc = record.from_ip;
        } else if (record.to_ip) {
            pc = record.to_ip;
        } else {
            continue;
        }

        DfxSymbol symbol = GetSymbol(pc, recordAuxTrace.data_.reserved__, recordAuxTrace.data_.tid);
        HLOGV("pc 0x%llx symbol %s", pc, symbol.ToDebugString().c_str());
    }
    SpeDecoderFree(decoder);
#endif
}

void VirtualRuntime::SetRecordMode(RecordCallBack recordCallBack)
{
    recordCallBack_ = recordCallBack;
}

void VirtualRuntime::UpdateSymbols(std::shared_ptr<DfxMap> map, pid_t pid)
{
    CHECK_TRUE(map == nullptr || map->symbolFileIndex != -1, NO_RETVAL, 0, "");
    HLOGD("try to find symbols for file: %s", map->name.c_str());
    for (size_t i = 0; i < symbolsFiles_.size(); ++i) {
        if (symbolsFiles_[i]->filePath_ == map->name) {
            map->symbolFileIndex = static_cast<int32_t>(i);
            HLOGV("already have '%s', symbol index:%zu", map->name.c_str(), i);
            return;
        }
    }

#ifdef HIPERF_DEBUG_TIME
    const auto startTime = steady_clock::now();
#endif
    /**
     * map[]     map.name = SymbolsFile.filePath_         prot    SymbolsFileType
     * seg1      /data/storage/el1/bundle/entry.hap       r--p    ABC
     * seg2      /data/storage/el1/bundle/entry.hap       r-xp    ELF
     * seg3      /data/storage/el1/bundle/entry.hap       r--p    ABC
     * seg4      /data/storage/el1/bundle/entry.hap       r--p    ABC
     * seg1      /system/app/SceneBoard/SceneBoard.hap    r--p    ABC
     * seg2      /system/app/SceneBoard/SceneBoard.hap    r--p    ABC
     * seg3      /system/app/SceneBoard/SceneBoard.hap    r--p    ABC
     * segN      .hap                                     r--p    .an/jit/etc
     * 1.map.name == symbolsFile.filePath_
     * 2.map.FileType == symbolsFiles_[map.symbolFileIndex]
     * 3.cache pc->map->symbolsFiles[map.symbolFileIndex]
     * 4.must ensure map.mapType assigned with SymbolsFile constructions at the same time.
    */
    auto symbolsFile = SymbolsFile::CreateSymbolsFile(map->name, pid);
    symbolsFile->SetMapsInfo(map);
    if (enableDebugInfoSymbolic_ && symbolsFile->symbolFileType_ == SymbolsFileType::SYMBOL_ELF_FILE) {
        symbolsFile->EnableMiniDebugInfo();
    }
    // set symbol path If it exists
    if (symbolsPaths_.size() > 0) {
        // also load from search path
        symbolsFile->setSymbolsFilePath(symbolsPaths_);
    }
    if (loadSymboleWhenNeeded_) {
        // load it when we need it
        symbolsFiles_.emplace_back(std::move(symbolsFile));
    } else if (symbolsFile->LoadSymbols()) {
        symbolsFiles_.emplace_back(std::move(symbolsFile));
    } else {
        HLOGW("symbols file for '%s' not found.", map->name.c_str());
    }
#ifdef HIPERF_DEBUG_TIME
    auto usedTime = duration_cast<microseconds>(steady_clock::now() - startTime);
    if (usedTime.count() != 0) {
        HLOGV("cost %0.3f ms to load '%s'", usedTime.count() / MS_DURATION, map->name.c_str());
    }
    updateSymbolsTimes_ += usedTime;
#endif
}

const DfxSymbol VirtualRuntime::GetKernelSymbol(uint64_t ip, const std::vector<DfxMap> &memMaps,
                                                const VirtualThread &thread)
{
    DfxSymbol vaddrSymbol(ip, thread.name_);
    for (auto &map : memMaps) {
        if (ip > map.begin && ip < map.end) {
            HLOGM("found addr 0x%" PRIx64 " in kernel map 0x%" PRIx64 " - 0x%" PRIx64 " from %s",
                  ip, map.begin, map.end, map.name.c_str());
            vaddrSymbol.module_ = map.name;
            // found symbols by file name
            for (auto &symbolsFile : symbolsFiles_) {
                if (symbolsFile->filePath_ == map.name) {
                    vaddrSymbol.symbolFileIndex_ = symbolsFile->id_;
                    vaddrSymbol.fileVaddr_ =
                        symbolsFile->GetVaddrInSymbols(ip, map.begin, map.offset);
                    perf_callchain_context context = PERF_CONTEXT_KERNEL;
                    if (GetSymbolCache(vaddrSymbol.fileVaddr_, vaddrSymbol, context)) {
                        return vaddrSymbol;
                    }
                    HLOGV("found symbol vaddr 0x%" PRIx64 " for runtime vaddr 0x%" PRIx64
                          " at '%s'",
                          vaddrSymbol.fileVaddr_, ip, map.name.c_str());
                    if (!symbolsFile->SymbolsLoaded()) {
                        symbolsFile->LoadSymbols();
                    }
                    DfxSymbol foundSymbols = symbolsFile->GetSymbolWithVaddr(vaddrSymbol.fileVaddr_);
                    foundSymbols.taskVaddr_ = ip;
                    if (!foundSymbols.IsValid()) {
                        HLOGW("addr 0x%" PRIx64 " vaddr  0x%" PRIx64 " NOT found in symbol file %s",
                              ip, vaddrSymbol.fileVaddr_, map.name.c_str());
                        return vaddrSymbol;
                    } else {
                        return foundSymbols;
                    }
                }
            }
            HLOGW("addr 0x%" PRIx64 " in map but NOT found the symbol file %s", ip,
                  map.name.c_str());
        } else {
            HLOGM("addr 0x%" PRIx64 " not in map 0x%" PRIx64 " - 0x%" PRIx64 " from %s", ip,
                  map.begin, map.end, map.name.c_str());
        }
    }
    return vaddrSymbol;
}

const DfxSymbol VirtualRuntime::GetKernelThreadSymbol(uint64_t ip, const VirtualThread &thread)
{
    DfxSymbol vaddrSymbol(ip, thread.name_);
    int64_t mapIndex = thread.FindMapIndexByAddr(ip);
    if (mapIndex < 0) {
        HLOGV("not found in any map");
        return vaddrSymbol;
    }

    auto map = thread.GetMaps()[mapIndex];
    CHECK_TRUE(map == nullptr, vaddrSymbol, 0, "");
    HLOGM("found addr 0x%" PRIx64 " in kthread map 0x%" PRIx64 " - 0x%" PRIx64 " from %s",
            ip, map->begin, map->end, map->name.c_str());
    // found symbols by file name
    for (auto &symbolsFile : symbolsFiles_) {
        if (symbolsFile->filePath_ == map->name) {
            vaddrSymbol.symbolFileIndex_ = symbolsFile->id_;
            vaddrSymbol.module_ = map->name;
            vaddrSymbol.fileVaddr_ =
                symbolsFile->GetVaddrInSymbols(ip, map->begin, map->offset);
            perf_callchain_context context = PERF_CONTEXT_MAX;
            if (GetSymbolCache(vaddrSymbol.fileVaddr_, vaddrSymbol, context)) {
                return vaddrSymbol;
            }
            HLOGV("found symbol vaddr 0x%" PRIx64 " for runtime vaddr 0x%" PRIx64 " at '%s'",
                    vaddrSymbol.fileVaddr_, ip, map->name.c_str());
            if (!symbolsFile->SymbolsLoaded()) {
                symbolsFile->LoadDebugInfo();
                symbolsFile->LoadSymbols(map);
            }
            auto foundSymbols = symbolsFile->GetSymbolWithVaddr(vaddrSymbol.fileVaddr_);
            foundSymbols.taskVaddr_ = ip;
            if (!foundSymbols.IsValid()) {
                HLOGW("addr 0x%" PRIx64 " vaddr  0x%" PRIx64 " NOT found in symbol file %s",
                        ip, vaddrSymbol.fileVaddr_, map->name.c_str());
                return vaddrSymbol;
            } else {
                return foundSymbols;
            }
        }
    }
    HLOGW("addr 0x%" PRIx64 " in map but NOT found the symbol file %s", ip,
            map->name.c_str());
    return vaddrSymbol;
}

const DfxSymbol VirtualRuntime::GetUserSymbol(uint64_t ip, const VirtualThread &thread)
{
    DfxSymbol vaddrSymbol(ip, thread.name_);
    int64_t mapIndex = thread.FindMapIndexByAddr(ip);
    if (mapIndex >= 0) {
        auto map = thread.GetMaps()[mapIndex];
        SymbolsFile *symbolsFile = thread.FindSymbolsFileByMap(map);
        if (symbolsFile != nullptr) {
            vaddrSymbol.symbolFileIndex_ = symbolsFile->id_;
            vaddrSymbol.module_ = map->name;
            vaddrSymbol.fileVaddr_ = symbolsFile->GetVaddrInSymbols(ip, map->begin, map->offset);
            perf_callchain_context context = PERF_CONTEXT_USER;
            if (GetSymbolCache(vaddrSymbol.fileVaddr_, vaddrSymbol, context)) {
                return vaddrSymbol;
            }
            HLOGV("found symbol vaddr 0x%" PRIx64 " for runtime vaddr 0x%" PRIx64 " at '%s'",
                  vaddrSymbol.fileVaddr_, ip, map->name.c_str());
            if (!symbolsFile->SymbolsLoaded()) {
                symbolsFile->LoadDebugInfo(map);
                symbolsFile->LoadSymbols(map);
            }
            DfxSymbol foundSymbols;
            if (!symbolsFile->IsAbc()) {
                foundSymbols = symbolsFile->GetSymbolWithVaddr(vaddrSymbol.fileVaddr_);
            } else {
                HLOGD("symbolsFile:%s is ABC :%d", symbolsFile->filePath_.c_str(), symbolsFile->IsAbc());
                foundSymbols = symbolsFile->GetSymbolWithPcAndMap(ip, map);
            }

            if (foundSymbols.IsValid()) {
                return foundSymbols;
            } else {
                HLOGW("addr 0x%" PRIx64 " vaddr  0x%" PRIx64 " NOT found in symbol file %s", ip,
                      vaddrSymbol.fileVaddr_, map->name.c_str());
                if (symbolsFile->IsAbc()) {
                    symbolsFile->symbolsMap_.insert(std::make_pair(ip, vaddrSymbol));
                }
                return vaddrSymbol;
            }
        } else {
            HLOGW("addr 0x%" PRIx64 " in map but NOT found the symbol file %s", ip, map->name.c_str());
        }
    } else {
        HLOGV("not found in any map");
    }
    return vaddrSymbol;
}

bool VirtualRuntime::GetSymbolCache(uint64_t fileVaddr, DfxSymbol &symbol,
                                    const perf_callchain_context &context)
{
    if (context == PERF_CONTEXT_MAX && kThreadSymbolCache_.count(fileVaddr)) {
        CHECK_TRUE(kThreadSymbolCache_.find(symbol.fileVaddr_) == kThreadSymbolCache_.end(), false, 0, "");
        symbol = kThreadSymbolCache_[symbol.fileVaddr_];
        symbol.hit_++;
        HLOGV("hit kernel thread cache 0x%" PRIx64 " %d", fileVaddr, symbol.hit_);
        return true;
    } else if (context != PERF_CONTEXT_USER && kernelSymbolCache_.count(fileVaddr)) {
        CHECK_TRUE(kernelSymbolCache_.find(symbol.fileVaddr_) == kernelSymbolCache_.end(), false, 0, "");
        symbol = kernelSymbolCache_[symbol.fileVaddr_];
        symbol.hit_++;
        HLOGV("hit kernel cache 0x%" PRIx64 " %d", fileVaddr, symbol.hit_);
        return true;
    } else if (userSymbolCache_.count(symbol.fileVaddr_) != 0) {
        const DfxSymbol &cachedSymbol = userSymbolCache_[symbol.fileVaddr_];
        // must be the same file
        if (cachedSymbol.module_ != symbol.module_) {
            return false;
        }
        symbol = cachedSymbol;
        symbol.hit_++;
        HLOGV("hit user cache 0x%" PRIx64 " %d %s", fileVaddr, symbol.hit_,
            symbol.ToDebugString().c_str());
        return true;
    } else {
        HLOGM("cache miss k %zu u %zu kt %zu", kernelSymbolCache_.size(),
              userSymbolCache_.size(), kThreadSymbolCache_.size());
    }
    return false;
}

DfxSymbol VirtualRuntime::GetSymbol(uint64_t ip, pid_t pid, pid_t tid, const perf_callchain_context &context)
{
    HLOGV("try find tid %u ip 0x%" PRIx64 " in %zu symbolsFiles\n", tid, ip, symbolsFiles_.size());
    DfxSymbol symbol;

    if (IsKernelThread(pid)) {
        VirtualThread &kthread = GetThread(pid, tid);
        HLOGM("try found addr in kernel thread %u with %zu maps", pid,
              kthread.GetMaps().size());
        symbol = GetKernelThreadSymbol(ip, kthread);
        HLOGM("add addr to kernel thread cache 0x%" PRIx64 " cache size %zu", ip,
              kThreadSymbolCache_.size());
        kThreadSymbolCache_[symbol.fileVaddr_] = symbol;
        if (symbol.IsValid()) {
            return symbol;
        }
    }

    if (context == PERF_CONTEXT_USER || (context == PERF_CONTEXT_MAX && !symbol.IsValid())) {
        // check userspace memmap
        symbol = GetUserSymbol(ip, GetThread(pid, tid));
        if (userSymbolCache_.find(symbol.fileVaddr_) == userSymbolCache_.end()) {
            userSymbolCache_.reserve(USER_SYMBOL_CACHE_LIMIT);
        }
        userSymbolCache_[symbol.fileVaddr_] = symbol;
        HLOGV("cache ip 0x%" PRIx64 " to %s", ip,
              userSymbolCache_[symbol.fileVaddr_].ToDebugString().c_str());
    }

    if (context == PERF_CONTEXT_KERNEL || (context == PERF_CONTEXT_MAX && !symbol.IsValid())) {
        // check kernelspace
        HLOGM("try found addr in kernelspace %zu maps", kernelSpaceMemMaps_.size());
        symbol = GetKernelSymbol(ip, kernelSpaceMemMaps_, GetThread(pid, tid));
        HLOGM("add addr to kernel cache 0x%" PRIx64 " cache size %zu", ip,
              kernelSymbolCache_.size());
        kernelSymbolCache_[symbol.fileVaddr_] = symbol;
    }
    return symbol;
}

bool VirtualRuntime::SetSymbolsPaths(const std::vector<std::string> &symbolsPaths)
{
    std::unique_ptr<SymbolsFile> symbolsFile = SymbolsFile::CreateSymbolsFile(SYMBOL_UNKNOW_FILE);
    CHECK_TRUE(symbolsFile == nullptr, false, 0, "");
    // we need check if the path is accessible
    bool accessible = symbolsFile->setSymbolsFilePath(symbolsPaths);
    if (accessible) {
        symbolsPaths_ = symbolsPaths;
    } else {
        if (!symbolsPaths.empty()) {
            HLOGE("some symbols path unable access");
        }
    }
    return accessible;
}

void VirtualRuntime::UpdateFromPerfData(const std::vector<SymbolFileStruct> &symbolFileStructs)
{
    // review: if we need move to some other place ?
    HLOG_ASSERT_MESSAGE(symbolsFiles_.size() == 0, " symbolsFiles_ size is %zu",
                        symbolsFiles_.size());
    for (const auto &symbolFileStruct : symbolFileStructs) {
        HLOGV("symbolFileStruct.filePath_:'%s'", symbolFileStruct.filePath_.c_str());
        HLOGV("symbolFileStruct.buildId_:'%s'", symbolFileStruct.buildId_.c_str());
        HLOGV("process symbols file:'%s':'%s'", symbolFileStruct.filePath_.c_str(),
              symbolFileStruct.buildId_.c_str());

        // load from symbolFileStruct (perf.data)
        std::unique_ptr<SymbolsFile> symbolsFile = SymbolsFile::LoadSymbolsFromSaved(symbolFileStruct);
        if (symbolsFile == nullptr) {
            continue;
        }
        // reaload from sybol path If it exists
        if (symbolsPaths_.size() > 0) {
            HLOGV("try again with symbolsPaths setup");
            symbolsFile->setSymbolsFilePath(symbolsPaths_); // also load from search path
            // use give us path ,we must reload it.
            symbolsFile->LoadSymbols();
        }
        symbolsFile->id_ = static_cast<int32_t>(symbolsFiles_.size());
        symbolsFiles_.emplace_back(std::move(symbolsFile));
    }
}

void VirtualRuntime::ImportUniqueStackNodes(const std::vector<UniStackTableInfo>& uniStackTableInfos)
{
    for (const UniStackTableInfo& item : uniStackTableInfos) {
        auto stackTable = std::make_shared<UniqueStackTable>(item.pid, item.tableSize);
        for (const UniStackNode& node : item.nodes) {
            stackTable->ImportNode(node.index, node.node);
        }
        processStackMap_[item.pid] = std::move(stackTable);
    }
}

/*
   ARM functions
       The table below lists the symbols exported by the vDSO.

       symbol                 version
       ────────────────────────────────────────────────────────────
       __vdso_gettimeofday    LINUX_2.6 (exported since Linux 4.1)
       __vdso_clock_gettime   LINUX_2.6 (exported since Linux 4.1)

       Additionally, the ARM port has a code page full of utility
       functions.  Since it's just a raw page of code, there is no ELF
       information for doing symbol lookups or versioning.  It does
       provide support for different versions though.

       For information on this code page, it's best to refer to the
       kernel documentation as it's extremely detailed and covers
       everything you need to know:
       Documentation/arm/kernel_user_helpers.txt.

   aarch64 functions
       The table below lists the symbols exported by the vDSO.

       symbol                   version
       ──────────────────────────────────────
       __kernel_rt_sigreturn    LINUX_2.6.39
       __kernel_gettimeofday    LINUX_2.6.39
       __kernel_clock_gettime   LINUX_2.6.39
       __kernel_clock_getres    LINUX_2.6.39
*/
void VirtualRuntime::LoadVdso()
{
#if defined(is_ohos) && is_ohos
    VirtualThread myThread(getpid(), symbolsFiles_);
    myThread.ParseMap();
    for (const auto &map : myThread.GetMaps()) {
        if (map->IsVdsoMap()) {
            std::string memory(map->end - map->begin, '\0');
            std::copy(reinterpret_cast<char *>((map->begin)), reinterpret_cast<char *>((map->end)),
                      &memory[0]);
            std::string tempPath("/data/log/hiperflog/");
            if (!IsDirectoryExists(tempPath)) {
                HIPERF_HILOGI(MODULE_DEFAULT, "%{public}s not exist.", tempPath.c_str());
                CHECK_TRUE(!CreateDirectory(tempPath, HIPERF_FILE_PERM_770), NO_RETVAL,
                           LOG_TYPE_WITH_HILOG, "Create hiperflog path failed");
            }
            std::string tempFileName = tempPath + map->name;
            if (!WriteStringToFile(tempFileName, memory)) {
                printf("vdso temp file create fail at %s\n", tempFileName.c_str());
            } else {
                HLOGD("vdso temp file create at %s:%zu", tempFileName.c_str(), memory.size());
                auto symbolsFile = SymbolsFile::CreateSymbolsFile(map->name);
                symbolsFile->setSymbolsFilePath(tempPath); // also load from search path
                symbolsFiles_.emplace_back(std::move(symbolsFile));
                return;
            }
        }
    }
    HLOGD("no vdso found");
#endif
}

void VirtualRuntime::UpdateServiceSpaceMaps()
{
    VirtualThread &kthread = GetThread(SYSMGR_PID, SYSMGR_PID);
    kthread.ParseServiceMap(SYSMGR_FILE_NAME);
    if (recordCallBack_) {
        if (isRoot_) {
            for (const auto &map : kthread.GetMaps()) {
                PerfRecordMmap record(true, SYSMGR_PID, SYSMGR_PID,
                                      map->begin, map->end - map->begin,
                                      0, SYSMGR_FILE_NAME);
                recordCallBack_(record);
            }
        }
    }
}

void VirtualRuntime::UpdateServiceSymbols()
{
    HLOGD("try to update kernel thread symbols for kernel service");
    std::string fileName = SYSMGR_FILE_NAME;
    auto symbolsFile = SymbolsFile::CreateSymbolsFile(SYMBOL_KERNEL_THREAD_FILE, fileName);

    HLOGD("add kernel service symbol file: %s", fileName.c_str());
    if (symbolsFile->LoadSymbols()) {
        symbolsFiles_.emplace_back(std::move(symbolsFile));
    } else {
        HLOGW("symbols file for '%s' not found.", fileName.c_str());
    }
}

void VirtualRuntime::UpdateKernelThreadMap(pid_t pid, uint64_t begin, uint64_t len, uint64_t offset,
                                           std::string filename)
{
    HLOGV("update kernel thread map pid %u offset 0x%" PRIx64 " name:'%s'", pid, offset, filename.c_str());

    VirtualThread &thread = GetThread(pid, pid);
    thread.CreateMapItem(filename, begin, len, offset);
}

void VirtualRuntime::UpdateDevhostSpaceMaps()
{
    VirtualThread &kthread = GetThread(devhostPid_, devhostPid_);
    kthread.ParseDevhostMap(devhostPid_);
    if (recordCallBack_) {
        for (const auto &map : kthread.GetMaps()) {
            auto record =
            std::make_unique<PerfRecordMmap>(false, devhostPid_, devhostPid_,
                                             map->begin, map->end - map->begin,
                                             map->offset, map->name);
            recordCallBack_(*record);
        }
    }
}

void VirtualRuntime::UpdateDevhostSymbols()
{
    HLOGD("try to update kernel thread symbols for devhost");
    auto kallsyms = SymbolsFile::CreateSymbolsFile(SYMBOL_KERNEL_THREAD_FILE, DEVHOST_FILE_NAME);
    // file name of devhost.ko
    std::map<std::string_view, std::unique_ptr<SymbolsFile>> koMaps;
    koMaps[DEVHOST_FILE_NAME] =
        SymbolsFile::CreateSymbolsFile(SYMBOL_KERNEL_THREAD_FILE, DEVHOST_LINUX_FILE_NAME);

    if (kallsyms->LoadSymbols()) {
        for (auto &symbol : kallsyms->GetSymbols()) {
            if (koMaps.find(symbol.module_) == koMaps.end()) {
                std::string filename = std::string(symbol.module_);
                // [devhost] to /liblinux/devhost.ko
                filename.erase(filename.begin());
                filename.erase(filename.end() - 1);
                filename = DEVHOST_LINUX_PREFIX + filename + KERNEL_MODULES_EXT_NAME;
                koMaps[symbol.module_] =
                    SymbolsFile::CreateSymbolsFile(SYMBOL_KERNEL_THREAD_FILE, filename);
            }
            if (koMaps[symbol.module_] == nullptr) {
                continue;
            }
            koMaps[symbol.module_]->AddSymbol(std::move(symbol));
        }

        HLOGD("devhost loaded %zu symbolfiles", koMaps.size());
        for (auto &it : koMaps) {
            if (it.second == nullptr) {
                continue;
            }
            HLOGD("Load %zu symbols to %s", it.second->GetSymbols().size(),
                  it.second->filePath_.c_str());
            symbolsFiles_.emplace_back(std::move(it.second));
        }
    } else {
        HLOGW("symbols file for devhost parse failed.");
    }

    // update normal symbole files
    VirtualThread &kthread = GetThread(devhostPid_, devhostPid_);
    for (const auto &map : kthread.GetMaps()) {
        UpdateSymbols(map, devhostPid_);
    }
}

void VirtualRuntime::FixHMBundleMmap(char *filename, int pid, u16 &headerSize)
{
    if (!isHM_) {
        return;
    }
    // fix bundle path in mmap
    std::string newFilename = filename;
    VirtualThread &thread = GetThread(pid, pid);
    if (NeedAdaptHMBundlePath(newFilename, thread.name_)) {
        size_t oldSize = strlen(filename);
        if (memset_s(filename, KILO, '\0', KILO) != EOK) {
            HLOGD("memset_s failed in FixHMBundleMmap.");
        }
        if (strncpy_s(filename, KILO, newFilename.c_str(), newFilename.size()) != 0) {
            HLOGD("strncpy_s recordMmap2 failed!");
        }
        headerSize += newFilename.size() - oldSize;
    }
}

void VirtualRuntime::SetDevhostPid(pid_t devhost)
{
    HLOGD("Set devhost pid: %d", devhost);
    devhostPid_ = devhost;
}

bool VirtualRuntime::IsKernelThread(pid_t pid)
{
    if (!isHM_) {
        return false;
    }
    return pid == SYSMGR_PID || pid == devhostPid_;
}

void VirtualRuntime::ClearSymbolCache()
{
    userSpaceThreadMap_.clear();
    kernelSpaceMemMaps_.clear();
    processStackMap_.clear();
    symbolsFiles_.clear();
    userSymbolCache_.clear();
    kernelSymbolCache_.clear();
    kThreadSymbolCache_.clear();
    symbolsPaths_.clear();

#if defined(is_ohos) && is_ohos
    callstack_.ClearCache();
#endif
}

} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
