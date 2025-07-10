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
#ifndef HIPERF_VIRTUAL_RUNTIME_H
#define HIPERF_VIRTUAL_RUNTIME_H

#include <functional>
#include <fstream>
#if defined(is_ohos) && is_ohos
#include "callstack.h"
#endif
#include "hashlist.h"
#include "perf_event_record.h"
#include "symbols_file.h"
#include "virtual_thread.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
/*
This Class contains userspace thread objects. and kernel space objects
It represents a virtual operating environment, mainly referring to the relationship between pid,
mmaps, and symbols.

It mainly receives data is ip pointer (virtual address), pid
According to these data, it will find the corresponding mmap and its corresponding elf (also called
DSO)

Then find the corresponding symbol in the corresponding elf symbol file according to the offset
recorded in the corresponding mmap.
*/
using kSymbolsHits = std::unordered_set<uint64_t>;
using uSymbolsHits = std::unordered_map<pid_t, std::unordered_set<uint64_t>>;

class VirtualRuntime {
public:
    explicit VirtualRuntime(bool onDevice = true);
    ~VirtualRuntime();
    // thread need hook the record
    // from the record , it will call back to write some Simulated Record
    // case 1. some mmap will be create when it read mmaps for each new process (from record sample)

    using RecordCallBack = std::function<bool(PerfEventRecord&)>;
    using CollectSymbolCallBack = std::function<void(PerfRecordSample*)>;

    void SetRecordMode(RecordCallBack recordCallBack);
    void SetCollectSymbolCallBack(CollectSymbolCallBack collectSymbolCallBack);

    // this both used in report and record follow
    // it process the record, and rebuild the trhread maps
    // It internally determines whether to go to the Record process (which will generate virtual
    // events) or the Report process by judging whether SetRecordMode has been passed.
    void UpdateFromRecord(PerfEventRecord &record);
    void NeedDropKernelCallChain(PerfRecordSample &sample);
    // in reocrd mode
    // we make a kernel symbols from some proc file
    void UpdateKernelSpaceMaps();
    void UpdateKernelModulesSpaceMaps();
    void UpdateServiceSpaceMaps();
    void UpdateDevhostSpaceMaps();
    // load vdso
    void LoadVdso();

    void UpdateKernelSymbols();
    void UpdateKernelModulesSymbols();
    void UpdateServiceSymbols();
    void UpdateDevhostSymbols();

    pid_t devhostPid_ = -1;
    void SetDevhostPid(pid_t devhost);
    void FixHMBundleMmap(char *filename, int pid, u16 &headerSize);

    // set symbols path , it will send to every symobile file for search
    bool SetSymbolsPaths(const std::vector<std::string> &symbolsPaths);

    // any mode
    static_assert(sizeof(pid_t) == sizeof(int));

    const std::vector<std::unique_ptr<SymbolsFile>> &GetSymbolsFiles() const
    {
        return symbolsFiles_;
    }

    const ProcessStackMap* GetUniStackTable()
    {
        return &processStackMap_;
    }

    void SetCallStackExpend(size_t mergeLevel = 0)
    {
        callstackMergeLevel_ = mergeLevel;
    }

    void SetDisableUnwind(bool disableUnwind)
    {
        HLOGV("disableUnwind change to %d", disableUnwind);
        disableUnwind_ = disableUnwind;
    }

    void EnableDebugInfoSymbolic(bool enable)
    {
        enableDebugInfoSymbolic_ = enable;
    }
    void SetDedupStack()
    {
        dedupStack_ = true;
    }

    void ImportUniqueStackNodes(const std::vector<UniStackTableInfo>&);

    bool isHM_ = false;
    void SetHM(bool isHM)
    {
        isHM_ = isHM;
    }

    void SetNeedKernelCallChain(bool kernelCallChain)
    {
        needkernelCallChain_ = kernelCallChain;
    }
    DfxSymbol GetSymbol(uint64_t ip, pid_t pid, pid_t tid,
                           const perf_callchain_context &context = PERF_CONTEXT_MAX);
    void ClearSymbolCache();

    VirtualThread &GetThread(pid_t pid, pid_t tid, const std::string name = "");
    const std::map<pid_t, VirtualThread> &GetThreads() const
    {
        return userSpaceThreadMap_;
    }
    void SymbolicRecord(PerfRecordSample &recordSample);
    void SymbolSpeRecord(PerfRecordAuxtrace &recordAuxTrace);

    // report use
    void UpdateFromPerfData(const std::vector<SymbolFileStruct> &);
    void UnwindFromRecord(PerfRecordSample &recordSample);
    std::string ReadThreadName(pid_t tid, bool isThread);
    std::string ReadFromSavedCmdLines(pid_t tid);
    bool IsKernelThread(pid_t pid);
    void CollectDedupSymbol(kSymbolsHits &kernelSymbolsHits,
                            uSymbolsHits &userSymbolsHits);
    // debug time
#ifdef HIPERF_DEBUG_TIME
    std::chrono::microseconds updateSymbolsTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds unwindFromRecordTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds unwindCallStackTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds symbolicRecordTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds updateThreadTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds processSampleRecordTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds processMmapRecordTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds processMmap2RecordTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds processCommRecordTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds threadParseMapsTimes_ = std::chrono::microseconds::zero();
    std::chrono::microseconds threadCreateMmapTimes_ = std::chrono::microseconds::zero();
#endif
    const bool loadSymboleWhenNeeded_ = true; // this is a feature config

private:
    bool needkernelCallChain_ = false;
    bool disableUnwind_ = true;
    bool enableDebugInfoSymbolic_ = false;
    bool dedupStack_ = false;
    const bool isRoot_ = IsRoot();
    size_t callstackMergeLevel_ = 1;
    std::ifstream savedCmdLines_;
#if defined(is_ohos) && is_ohos
    CallStack callstack_;
#endif
    // pid map with user space thread
    std::map<pid_t, VirtualThread> userSpaceThreadMap_;
    // not pid , just memmap
    std::vector<DfxMap> kernelSpaceMemMaps_;
    ProcessStackMap processStackMap_;
    RecordCallBack recordCallBack_;
    CollectSymbolCallBack collectSymbolCallBack_;
    std::vector<std::unique_ptr<SymbolsFile>> symbolsFiles_;
    enum SymbolCacheLimit : std::size_t {
        KERNEL_SYMBOL_CACHE_LIMIT = 4000,
        USER_SYMBOL_CACHE_LIMIT = 4000,
    };
    HashList<uint64_t, DfxSymbol> userSymbolCache_;
    HashList<uint64_t, DfxSymbol> kernelSymbolCache_ {KERNEL_SYMBOL_CACHE_LIMIT};
    HashList<uint64_t, DfxSymbol> kThreadSymbolCache_ {KERNEL_SYMBOL_CACHE_LIMIT};
    bool GetSymbolCache(uint64_t fileVaddr, DfxSymbol &symbol,
                        const perf_callchain_context &context);
    // find synbols function name
    void MakeCallFrame(DfxSymbol &symbol, DfxFrame &callFrame);
    void UpdateSymbols(std::shared_ptr<DfxMap> map, pid_t pid);
    // we don't know whether hap vma mapping is stand for a so
    // thus we need try to parse it first
    bool UpdateHapSymbols(std::shared_ptr<DfxMap> map);
    void UpdateFromRecord(PerfRecordSample &recordSample);
    void UpdateFromRecord(PerfRecordMmap &recordMmap);
    void UpdateFromRecord(PerfRecordMmap2 &recordMmap2);
    void UpdateFromRecord(PerfRecordComm &recordComm);
    void DedupFromRecord(PerfRecordSample *recordSample);
    void UpdateFromRecord(PerfRecordAuxtrace &recordAuxTrace);
    // threads
    VirtualThread &UpdateThread(pid_t pid, pid_t tid, const std::string name = "");
    VirtualThread &CreateThread(pid_t pid, pid_t tid, const std::string name = "");

    // maps
    std::shared_ptr<DfxMap> UpdateThreadMaps(pid_t pid, pid_t tid, const std::string filename, uint64_t begin,
                          uint64_t len, uint64_t offset, uint32_t prot = 0);
    void UpdatekernelMap(uint64_t begin, uint64_t end, uint64_t offset, std::string filename);

    const DfxSymbol GetKernelSymbol(uint64_t ip, const std::vector<DfxMap> &memMaps,
                                 const VirtualThread &thread);
    const DfxSymbol GetUserSymbol(uint64_t ip, const VirtualThread &thread);
    const DfxSymbol GetKernelThreadSymbol(uint64_t ip, const VirtualThread &thread);
#ifdef HIPERF_DEBUG
    std::unordered_set<uint64_t> missedRuntimeVaddr_;
#endif
    void SymbolicCallFrame(PerfRecordSample &recordSample, uint64_t ip,
                           pid_t serverPid, perf_callchain_context context);
    bool RecoverCallStack(PerfRecordSample &recordSample);
    std::vector<std::string> symbolsPaths_;

    // kernel thread
    void UpdateKernelThreadMap(pid_t pid, uint64_t begin, uint64_t len, uint64_t offset, std::string filename);
    bool CheckValidSandBoxMmap(PerfRecordMmap2 &recordMmap2);
    void ProcessKernelCallChain(PerfRecordSample &sample);
    void AdjustCallChain(PerfRecordSample &sample);
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_VIRTUAL_RUNTIME_H
