/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
#define HILOG_TAG "CallStack"

#include "callstack.h"

#include <dlfcn.h>
#include <pthread.h>
#include <iostream>

#include <string>
#include <utility>

#include "dfx_regs.h"
#include "hiperf_hilog.h"
#include "register.h"

#ifdef target_cpu_arm
// reg size is int (unw_word_t)
#define UNW_WORD_PFLAG "x"
#else
// reg size is long (unw_word_t)
#define UNW_WORD_PFLAG "zx"
#endif
namespace OHOS {
namespace Developtools {
namespace HiPerf {
using namespace OHOS::HiviewDFX;

bool CallStack::ReadVirtualThreadMemory(UnwindInfo &unwindInfoPtr, ADDR_TYPE vaddr, ADDR_TYPE *data)
{
    if (__builtin_expect(unwindInfoPtr.thread.pid_ == unwindInfoPtr.callStack.lastPid_ &&
        vaddr == unwindInfoPtr.callStack.lastAddr_, true)) {
        *data = unwindInfoPtr.callStack.lastData_;
        return true;
    }

    if (unwindInfoPtr.thread.ReadRoMemory(vaddr, reinterpret_cast<uint8_t*>(data), sizeof(ADDR_TYPE))) {
        unwindInfoPtr.callStack.lastPid_ = unwindInfoPtr.thread.pid_;
        unwindInfoPtr.callStack.lastAddr_ = vaddr;
        unwindInfoPtr.callStack.lastData_ = *data;
        return true;
    } else {
        unwindInfoPtr.callStack.lastPid_ = -1;
        unwindInfoPtr.callStack.lastAddr_ = 0;
        return false;
    }
}

bool CallStack::GetIpSP(uint64_t &ip, uint64_t &sp, const u64 *regs, size_t regNum) const
{
    if (regNum > 0) {
        CHECK_TRUE(!RegisterGetSPValue(sp, arch_, regs, regNum), false, 1, "unable get sp");
        CHECK_TRUE(!RegisterGetIPValue(ip, arch_, regs, regNum), false, 1, "unable get ip");
        if (ip != 0) {
            return true;
        }
    } else {
        HLOGW("reg size is 0");
        return false;
    }
    return false;
}

bool CallStack::UnwindCallStack(const VirtualThread &thread, bool abi32, u64 *regs, u64 regsNum,
                                const u8 *stack, u64 stackSize, std::vector<DfxFrame> &callStack,
                                size_t maxStackLevel)
{
    regs_ = regs;
    regsNum_ = regsNum;
    stack_ = stack;
    stackSize_ = stackSize;

    arch_ = GetArchTypeFromABI(abi32);
    UpdateRegForABI(arch_, regs_);
    if (!RegisterGetSPValue(stackPoint_, arch_, regs_, regsNum_)) {
        HLOGE("RegisterGetSPValue failed");
        return false;
    } else {
        stackEnd_ = stackPoint_ + stackSize_;
    }

    uint64_t ip;
    uint64_t sp;
    if (!GetIpSP(ip, sp, regs_, regsNum_)) {
        HLOGW("unable get sp or sp , unable unwind");
        return false;
    } else {
        if (ip != 0) {
            HLOGV("unwind:%zu: ip 0x%" PRIx64 " sp 0x%" PRIx64 "", callStack.size(), ip, sp);
            callStack.emplace_back(ip, sp);
        }
    }

    /*
     * If we need more than one entry, do the DWARF
     * unwind itself.
     */
    if (maxStackLevel - 1 > 0) {
        return DoUnwind2(thread, callStack, maxStackLevel);
    }
    return true;
}

void CallStack::LogFrame(const std::string msg, const std::vector<DfxFrame> &frames)
{
    HLOGM("%s", msg.c_str());
    int level = 0;
    for (auto& frame : frames) {
        HLOGM("%d:%s", level++, frame.ToString().c_str());
    }
}

/*
we should have CallStack cache for each thread
end                    begin
0. A -> B -> C -> E -> F
1.           C -> E -> F
2.      B -> C
3. A -> B -> C
4.      B -> G -> H
5.      J -> C

0 is our cache
1 2 3... is from record

use expandLimit to setup how may frame match is needs

*/
size_t CallStack::DoExpandCallStack(std::vector<DfxFrame> &newCallFrames,
                                    const std::vector<DfxFrame> &cachedCallFrames,
                                    size_t expandLimit)
{
    int maxCycle = 0;

    if (expandLimit == 0 || newCallFrames.size() < expandLimit ||
        cachedCallFrames.size() < expandLimit ||
        cachedCallFrames.size() >= MAX_CALL_FRAME_UNWIND_SIZE) {
        HLOGM("expandLimit %zu not match new %zu cache %zu", expandLimit, newCallFrames.size(),
              cachedCallFrames.size());
        return 0; // size not enough
    }

    // called (Stack Bottom) , this will NOT change when compare
    // in case1 newIt -> C
    // in case2 newIt -> B
    const auto newIt = newCallFrames.end() - expandLimit;
    if (newIt != newCallFrames.end()) {
        HLOGM("try find new call chain bottom %s for limit %zu", newIt->ToString().c_str(),
            expandLimit);
    }

    // first frame search, from called - > caller
    // for case 2 it should found B
    size_t distances = expandLimit - 1;
    auto cachedIt = find(cachedCallFrames.begin(), cachedCallFrames.end(), *newIt);
    if (cachedIt == cachedCallFrames.end()) {
        HLOGM("not found in first search");
    }

    // cache frame found
    while (std::distance(cachedIt, cachedCallFrames.end()) >= signed(expandLimit)) {
        HLOG_ASSERT_MESSAGE(maxCycle++ < MAX_CALL_FRAME_EXPAND_CYCLE, "MAX_UNWIND_CYCLE = %d reach",
                            MAX_CALL_FRAME_EXPAND_CYCLE);

        if (std::equal(newIt, newIt + expandLimit, cachedIt)) {
            HLOGM("match %s + %zu", newIt->ToString().c_str(), expandLimit);
            cachedIt += expandLimit; // in while we check the boundary safe
            if (cachedIt == cachedCallFrames.end()) {
                // same but no more need expand
                break;
            }

            // expand the frame and make some log ?
            LogFrame("newCallStack:", newCallFrames);
            LogFrame("cachedCallStack:", cachedCallFrames);

            newCallFrames.insert(newCallFrames.end(), cachedIt, cachedCallFrames.end());
            auto expands = std::distance(cachedIt, cachedCallFrames.end());
            HLOGV("merge callstack increse to %zu (+%zd) ", newCallFrames.size(), expands);
            // we done the deal
            return expands;
        } else {
            // quick search next same farme again
            cachedIt++;
            if (cachedIt != cachedCallFrames.end()) {
                HLOGM("search next");
                cachedIt = find(cachedIt, cachedCallFrames.end(), *newIt);
            }
        }
    }
    HLOGM("cachedIt distance %zd , need %zd", std::distance(cachedCallFrames.begin(), cachedIt),
          distances);
    return 0u; // nothing expand
}

size_t CallStack::ExpandCallStack(pid_t tid, std::vector<DfxFrame> &callFrames, size_t expandLimit)
{
    size_t expand = 0u;
    if (expandLimit == 0) {
        return expand; // nothing need to do
    } else if (callFrames.size() < expandLimit) {
        HLOGM("new callstack is too small, skip it");
        return expand;
    }
    if (!cachedCallFramesMap_.count(tid)) {
        cachedCallFramesMap_[tid].reserve(MAX_CALL_FRAME_EXPAND_CACHE_SIZE);
    }
    if (callFrames.size() >= 1u) {
        // get top  (Earliest caller)
        HashList<uint64_t, std::vector<DfxFrame>> &cachedCallFrames = cachedCallFramesMap_[tid];
        HLOGV("find call stack frames in cache size %zu", cachedCallFrames.size());
        // compare
        using namespace std::rel_ops; // enable complement comparing operators
        for (auto itr = cachedCallFrames.begin(); itr < cachedCallFrames.end(); ++itr) {
            // each cached callstack
            /*
                stack 2    1    0
                cache A -> B -> C
                new        B -> C
                check:
                1 if new B == cache C
                2 if new B == cache B
                3 if new C == new C (if limit > 0)
                4 insert A after B in new stack
            */
            const std::vector<DfxFrame> &cachedCallStack = *itr;
            if (cachedCallStack.size() < expandLimit) {
                HLOGM("cache callstack is too small, skip it");
                continue; // check next
            }
            expand = DoExpandCallStack(callFrames, cachedCallStack, expandLimit);
            if (expand > 0) {
                break;
            }
        }
        // add new one in to cache cachedCallFrames.
        // further optimization can be done by caching pointer which avoids copying
        // vector
        cachedCallFrames[callFrames[0].pc] = callFrames;
    }
    HLOGM("expand %zu", expand);
    return expand;
}

#if defined(HAVE_LIBUNWINDER) && HAVE_LIBUNWINDER
bool CallStack::DoUnwind2(const VirtualThread &thread, std::vector<DfxFrame> &callStack,
                          size_t maxStackLevel)
{
#ifdef target_cpu_x86_64
    return false;
#else
    UnwindInfo unwindInfo = {
        .thread = thread,
        .callStack = *this,
    };

    if (pidUnwinder_.count(thread.pid_) == 0) {
        pidUnwinder_.emplace(thread.pid_, std::make_shared<Unwinder>(accessor_));
    }
    auto unwinder = pidUnwinder_[thread.pid_];

#ifdef target_cpu_arm
    static std::shared_ptr<DfxRegs> regs = std::make_shared<DfxRegsArm>();
    std::vector<uintptr_t> tempRegs;
    for (u64 i = 0; i < regsNum_; ++i) {
        tempRegs.push_back(static_cast<uintptr_t>(regs_[i]));
    }
    regs->SetRegsData(tempRegs);
#else
    static std::shared_ptr<DfxRegs> regs = std::make_shared<DfxRegsArm64>();
    regs->SetRegsData(reinterpret_cast<uintptr_t*>(regs_), regsNum_);
#endif
    CHECK_TRUE(unwinder == nullptr, false, 0, "");
    unwinder->SetRegs(regs);
    unwinder->Unwind(&unwindInfo);
    callStack = unwinder->GetFrames();
    HLOGD("callStack size:%zu", callStack.size());
    for (auto frame: callStack) {
        HLOGD("pc 0x%" PRIx64 " sp 0x%" PRIx64 "", frame.pc, frame.sp);
    }
    auto lastIt = callStack.end() - 1;
    auto preIt = lastIt - 1;
    if (lastIt != callStack.end() && preIt != callStack.end() &&
        callStack.size() > 1 && lastIt->pc == preIt->pc && lastIt->sp == preIt->sp) {
        callStack.erase(lastIt);
        HLOGD("remove last callframe");
    }
    return true;
#endif
}

void CallStack::DumpTableInfo(UnwindTableInfo &outTableInfo)
{
    HLOGV("unwind_table info: ");
    HLOGV(" start_ip:            0x%016" UNW_WORD_PFLAG "", outTableInfo.startPc);
    HLOGV(" end_ip:              0x%016" UNW_WORD_PFLAG "", outTableInfo.endPc);
    HLOGV(" segbase:             0x%016" UNW_WORD_PFLAG "", outTableInfo.segbase);
    HLOGV(" table_data:          0x%016" UNW_WORD_PFLAG "", outTableInfo.tableData);
    HLOGV(" table_len:           0x%016" UNW_WORD_PFLAG "", outTableInfo.tableLen);
}

int CallStack::FillUnwindTable(SymbolsFile *symbolsFile, std::shared_ptr<DfxMap> map, UnwindInfo *unwindInfoPtr,
                               uintptr_t pc, UnwindTableInfo& outTableInfo)
{
    HLOGM("try search debug info at %s", symbolsFile->filePath_.c_str());
    CHECK_TRUE(unwindInfoPtr == nullptr, -1, 0, "");
    auto &tableInfoMap = unwindInfoPtr->callStack.unwindTableInfoMap_;
    // all the thread in same process have same mmap and symbols
    if (tableInfoMap.find(unwindInfoPtr->thread.pid_) == tableInfoMap.end()) {
        tableInfoMap.emplace(unwindInfoPtr->thread.pid_, DsoUnwindTableInfoMap {});
    }
    DsoUnwindTableInfoMap &unwTabMap = tableInfoMap[unwindInfoPtr->thread.pid_];
    // find use dso name as key
    if (unwTabMap.find(symbolsFile->filePath_) == unwTabMap.end()) {
        UnwindTableInfo uti;
        auto elf = symbolsFile->GetElfFile();
        if (elf == nullptr) {
            return -1;
        }
        if (elf->FindUnwindTableInfo(pc, map, uti) == 0) {
            CHECK_TRUE(uti.format == -1, -1, 1, "parse unwind table failed.");
            unwTabMap[symbolsFile->filePath_] = uti;
            outTableInfo = unwTabMap[symbolsFile->filePath_];
            DumpTableInfo(uti);
            return 0;
        } else {
            HLOGV("FillUnwindTable failed");
            return -1;
        }
    } else {
        outTableInfo = unwTabMap[symbolsFile->filePath_];
        return 0;
    }
    return -1;
}

int CallStack::FindUnwindTable(uintptr_t pc, UnwindTableInfo& outTableInfo, void *arg)
{
    UnwindInfo *unwindInfoPtr = static_cast<UnwindInfo *>(arg);
    CHECK_TRUE(unwindInfoPtr == nullptr, -1, 0, "");
    int64_t mapIndex = unwindInfoPtr->thread.FindMapIndexByAddr(pc);
    if (mapIndex >= 0) {
        auto map = unwindInfoPtr->thread.GetMaps()[mapIndex];
        if (map != nullptr) {
            SymbolsFile *symbolsFile = unwindInfoPtr->thread.FindSymbolsFileByMap(map);
            if (symbolsFile != nullptr) {
                return FillUnwindTable(symbolsFile, map, unwindInfoPtr, pc, outTableInfo);
            } else {
                HLOGD("no symbols file found for thread %d:%s", unwindInfoPtr->thread.tid_,
                    unwindInfoPtr->thread.name_.c_str());
            }
        } else {
            HLOGD("pc 0x%016" UNW_WORD_PFLAG " not found in thread %d:%s", pc,
                unwindInfoPtr->thread.tid_, unwindInfoPtr->thread.name_.c_str());
        }
    } else {
        HLOGD("map index is -1");
    }
    return -1;
}

int CallStack::AccessMem2(uintptr_t addr, uintptr_t *val, void *arg)
{
    UnwindInfo *unwindInfoPtr = static_cast<UnwindInfo *>(arg);
    *val = 0;

    /* Check overflow. */
    CHECK_TRUE(unwindInfoPtr == nullptr || (addr + sizeof(uintptr_t) < addr), -1, 1,
               "unwindInfoPtr is null or address overflow at 0x%" UNW_WORD_PFLAG " increase 0x%zu",
               addr, sizeof(uintptr_t));

    if (addr < unwindInfoPtr->callStack.stackPoint_ ||
        addr + sizeof(uintptr_t) >= unwindInfoPtr->callStack.stackEnd_) {
        if (ReadVirtualThreadMemory(*unwindInfoPtr, addr, val)) {
            HLOGM("access_mem addr get val 0x%" UNW_WORD_PFLAG ", from mmap", *val);
        } else {
            HLOGW("access_mem mmap 0x%" PRIx64 " failed, STACK RANGE 0x%" PRIx64 "- 0x%" PRIx64 "(0x%" PRIx64 ")",
                  (uint64_t)addr,
                  unwindInfoPtr->callStack.stackPoint_, unwindInfoPtr->callStack.stackEnd_,
                  unwindInfoPtr->callStack.stackEnd_ - unwindInfoPtr->callStack.stackPoint_);
            return -1;
        }
    } else {
        size_t stackOffset = addr - unwindInfoPtr->callStack.stackPoint_;
        *val = *(uintptr_t *)&unwindInfoPtr->callStack.stack_[stackOffset];
        HLOGM("access_mem addr %p val %" UNW_WORD_PFLAG ", from stack offset %zu",
              reinterpret_cast<void *>(addr), *val, stackOffset);
    }

    return 0;
}
int CallStack::GetMapByPc(uintptr_t pc, std::shared_ptr<DfxMap>& map, void *arg)
{
    UnwindInfo *unwindInfoPtr = static_cast<UnwindInfo *>(arg);
    int64_t mapIndex = unwindInfoPtr->thread.FindMapIndexByAddr(pc);
    if (mapIndex >= 0) {
        map = unwindInfoPtr->thread.GetMaps()[mapIndex];
        if (map != nullptr) {
            return 0;
        }
    }
    HLOGD("pc 0x%016" UNW_WORD_PFLAG " not found in thread %d:%s", pc,
          unwindInfoPtr->thread.tid_, unwindInfoPtr->thread.name_.c_str());
    return -1;
}
#endif

CallStack::CallStack()
{
#if defined(HAVE_LIBUNWINDER) && HAVE_LIBUNWINDER
    accessor_ = std::make_shared<OHOS::HiviewDFX::UnwindAccessors>();
    accessor_->FindUnwindTable = &CallStack::FindUnwindTable;
    accessor_->AccessMem = &CallStack::AccessMem2;
    accessor_->AccessReg = nullptr;
    accessor_->GetMapByPc = &CallStack::GetMapByPc;
#endif
}

void CallStack::ClearCache()
{
    cachedCallFramesMap_.clear();
    lastPid_ = -1;
    lastAddr_ = 0;
    lastData_ = 0;
    stackPoint_ = 0;
    stackEnd_ = 0;
    regs_ = nullptr;
    regsNum_ = 0;
    stack_ = nullptr;
    stackSize_ = 0;

#if defined(HAVE_LIBUNWINDER) && HAVE_LIBUNWINDER
    pidUnwinder_.clear();
    unwindTableInfoMap_.clear();
#endif
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
