/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
#ifndef HIPERF_CALLSTACK_H
#define HIPERF_CALLSTACK_H

#include <map>
#include <optional>
#include <string>
#include <vector>

#if !is_mingw
#include <sys/mman.h>
#endif

#include "hashlist.h"
#include "register.h"
#include "utilities.h"
#include "unwinder.h"
#include "virtual_thread.h"

using ADDR_TYPE = uintptr_t;

namespace OHOS {
namespace Developtools {
namespace HiPerf {
using namespace OHOS::HiviewDFX;
const int MAX_CALL_FRAME_EXPAND_CYCLE = 10;
const size_t MAX_CALL_FRAME_EXPAND_CACHE_SIZE = 10;
const size_t MAX_CALL_FRAME_UNWIND_SIZE = 256;

struct UnwindInfo;

class CallStack {
public:
    CallStack();
    ~CallStack() = default;
    void ClearCache();
    bool UnwindCallStack(const VirtualThread &thread, bool abi32, u64 *regs, u64 regsNum,
                         const u8 *stack, u64 stackSize, std::vector<DfxFrame> &,
                         size_t maxStackLevel = MAX_CALL_FRAME_UNWIND_SIZE);
    size_t ExpandCallStack(pid_t tid, std::vector<DfxFrame> &callFrames, size_t expandLimit = 1u);

private:
    pid_t lastPid_ = -1;
    ADDR_TYPE lastAddr_ = 0;
    ADDR_TYPE lastData_ = 0;
    uint64_t stackPoint_ = 0;
    uint64_t stackEnd_ = 0;
    u64 *regs_ = nullptr; // not const , be cause we will fix it for arm64 cpu in UpdateRegForABI
    u64 regsNum_ = 0;
    const u8 *stack_ = nullptr;
    u64 stackSize_ = 0;

    void LogFrame(const std::string msg, const std::vector<DfxFrame> &frames);
    size_t DoExpandCallStack(std::vector<DfxFrame> &newCallFrames,
                           const std::vector<DfxFrame> &cachedCallFrames, size_t expandLimit);

    // we have a cache for all thread
    std::map<pid_t, HashList<uint64_t, std::vector<DfxFrame>>> cachedCallFramesMap_;
    bool GetIpSP(uint64_t &ip, uint64_t &sp, const u64 *regs, size_t regNum) const;
    ArchType arch_ = ArchType::ARCH_UNKNOWN;

    static bool ReadVirtualThreadMemory(UnwindInfo &unwindInfoPtr, ADDR_TYPE addr, ADDR_TYPE *data);
#if defined(HAVE_LIBUNWINDER) && HAVE_LIBUNWINDER
    bool DoUnwind2(const VirtualThread &thread, std::vector<DfxFrame> &callStack, size_t maxStackLevel);
    static void DumpTableInfo(UnwindTableInfo &outTableInfo);
    static int FillUnwindTable(SymbolsFile *symbolsFile, std::shared_ptr<DfxMap> map, UnwindInfo *unwindInfoPtr,
                               uintptr_t pc, UnwindTableInfo& outTableInfo);
    static int FindUnwindTable(uintptr_t pc, UnwindTableInfo& outTableInfo, void *arg);
    static int AccessMem2(uintptr_t addr, uintptr_t *val, void *arg);
    static int GetMapByPc(uintptr_t pc, std::shared_ptr<DfxMap>& map, void *arg);

    // pid->unwinder(acc/regs/maps) cache
    std::unordered_map<pid_t, std::shared_ptr<Unwinder>> pidUnwinder_;
    // pid->elf->unwindtable cache
    using DsoUnwindTableInfoMap = std::unordered_map<std::string, UnwindTableInfo>;
    std::unordered_map<pid_t, DsoUnwindTableInfoMap> unwindTableInfoMap_;

    std::shared_ptr<UnwindAccessors> accessor_;
#endif
};

struct UnwindInfo {
    const VirtualThread &thread;
    const u64 *regs;
    size_t regNumber;
    ArchType arch;
    CallStack &callStack;
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_CALLSTACK_H
