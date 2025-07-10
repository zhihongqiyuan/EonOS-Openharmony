/*
 * Copyright (c) 2021 - 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "regAllocator.h"

#include "compiler/core/codeGen.h"
#include "checker/types/type.h"

#include <algorithm>
#include <vector>

namespace ark::es2panda::compiler {
// AllocatorBase

AllocatorBase::AllocatorBase(CodeGen *const cg) noexcept : cg_(cg) {}

void AllocatorBase::PushBack(IRNode *const ins) const
{
    cg_->Insns().push_back(ins);
}

CodeGen &AllocatorBase::GetCodeGen() noexcept
{
    return *cg_;
}

const CodeGen &AllocatorBase::GetCodeGen() const noexcept
{
    return *cg_;
}

ArenaAllocator &AllocatorBase::Allocator() noexcept
{
    return *cg_->Allocator();
}

const ArenaAllocator &AllocatorBase::Allocator() const noexcept
{
    return *cg_->Allocator();
}

// SimpleAllocator

SimpleAllocator::SimpleAllocator(CodeGen *const cg) noexcept : AllocatorBase(cg) {}

Label *SimpleAllocator::AllocLabel(std::string &&id)
{
    const auto *lastInsNode =
        GetCodeGen().Insns().empty() ? FIRST_NODE_OF_FUNCTION : GetCodeGen().Insns().back()->Node();
    return Alloc<Label>(lastInsNode, std::move(id));
}

void SimpleAllocator::AddLabel(Label *const label) const
{
    PushBack(label);
}

// RegAllocatorBase

RegAllocatorBase::RegAllocatorBase(CodeGen *const cg, RegSpiller *const spiller) noexcept
    : AllocatorBase(cg), spiller_(spiller)
{
}

RegSpiller &RegAllocatorBase::Spiller() noexcept
{
    return *spiller_;
}

const RegSpiller &RegAllocatorBase::Spiller() const noexcept
{
    return *spiller_;
}

std::pair<bool, std::size_t> RegAllocatorBase::RegIndicesValid(const IRNode *const ins, const Span<VReg *> &registers)
{
    const auto &formats = ins->GetFormats();
    std::size_t limit = 0;

    for (const auto &format : formats) {
        for (const auto &formatItem : format.GetFormatItem()) {
            if (formatItem.IsVReg()) {
                limit = 1U << formatItem.BitWidth();
                break;
            }
        }

        if (std::all_of(registers.begin(), registers.end(),
                        [limit](const VReg *const reg) { return reg->IsValid(limit); })) {
            return {true, limit};
        }
    }

    return {false, limit};
}

VReg RegAllocatorBase::Spill(IRNode *const ins, const VReg reg) const
{
    const auto [spill_info, origin_type] = spiller_->New();

    if (origin_type != nullptr) {
        if (auto *const mov = spiller_->MoveReg(ins->Node(), spill_info.SpillReg(), spill_info.OriginReg(), true);
            mov != nullptr) {
            PushBack(mov);
        }
    }

    if (auto *const mov = spiller_->MoveReg(ins->Node(), spill_info.OriginReg(), reg, false); mov != nullptr) {
        PushBack(mov);
        spiller_->GetCodeGen()->SetVRegType(spill_info.OriginReg(), origin_type);
    }

    return spill_info.OriginReg();
}

void RegAllocatorBase::Restore(const IRNode *const ins) const
{
    const auto spillInfo = spiller_->Restore();
    if (spiller_->GetCodeGen()->GetVRegType(spillInfo.OriginReg()) == nullptr) {
        return;
    }

    if (auto *const mov = spiller_->MoveReg(ins->Node(), spillInfo.OriginReg(), spillInfo.SpillReg(), false);
        mov != nullptr) {
        PushBack(mov);
    }
}

// RegAllocator

RegAllocator::RegAllocator(CodeGen *const cg, RegSpiller *const spiller) noexcept : RegAllocatorBase(cg, spiller) {}

void RegAllocator::Run(IRNode *const ins, const int32_t spillMax)
{
    ASSERT(Spiller().Restored());
    std::array<VReg *, IRNode::MAX_REG_OPERAND> regs {};
    const auto regCnt = ins->Registers(&regs);
    const auto registers =
        Span<VReg *>(regs.data(), regs.data() + (spillMax == std::numeric_limits<int32_t>::max() ? regCnt : spillMax));

    std::array<OutVReg, IRNode::MAX_REG_OPERAND> dstRegs {};
    ins->OutRegisters(&dstRegs);

    const auto [indices_valid, limit] = RegIndicesValid(ins, registers);
    if (indices_valid) {
        PushBack(ins);
        return;
    }

    const auto rs = Spiller().Start(GetCodeGen());

    std::unordered_set<VReg> validRegs;
    for (auto *const reg : registers) {
        if (!reg->IsValid(limit)) {
            continue;
        }

        validRegs.insert(*reg);
    }

    std::vector<IRNode *> dstMoves;
    size_t i = 0;
    for (auto *const reg : registers) {
        auto dstInfo = dstRegs[i++];
        if (reg->IsValid(limit)) {
            continue;
        }

        Spiller().Adjust(validRegs);

        auto r = Spill(ins, *reg);

        if (dstInfo.reg != nullptr) {
            dstMoves.push_back(GetCodeGen().AllocMov(ins->Node(), dstInfo, r));
        }

        *reg = r;
    }

    PushBack(ins);

    for (auto *mov : dstMoves) {
        PushBack(mov);
    }

    while (!Spiller().Restored()) {
        Restore(ins);
    }

    Spiller().Finalize();
}

// RangeRegAllocator

RangeRegAllocator::RangeRegAllocator(CodeGen *const cg, RegSpiller *const spiller) noexcept
    : RegAllocatorBase(cg, spiller)
{
}

void RangeRegAllocator::Run(IRNode *const ins, VReg rangeStart, const std::size_t argCount)
{
    ASSERT(Spiller().Restored());
    const auto rangeEnd = rangeStart + argCount;

    std::array<VReg *, IRNode::MAX_REG_OPERAND> regs {};
    const auto regCnt = ins->Registers(&regs);
    const auto registers = Span<VReg *>(regs.data(), regs.data() + regCnt);
    if (RegIndicesValid(ins, registers).first) {
        PushBack(ins);
        return;
    }

    const auto rs = Spiller().Start(GetCodeGen());

    auto regIter = registers.begin();
    const auto regIterEnd = regIter + registers.size() - 1;  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    while (regIter != regIterEnd) {
        auto *const reg = *regIter;

        *reg = Spill(ins, *reg);
        regIter++;  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    auto *const regStartReg = *regIter;
    auto reg = rangeStart++;
    *regStartReg = Spill(ins, reg);

    while (rangeStart != rangeEnd) {
        reg = rangeStart++;
        Spill(ins, reg);
    }

    PushBack(ins);

    while (!Spiller().Restored()) {
        Restore(ins);
    }

    Spiller().Finalize();
}
}  // namespace ark::es2panda::compiler
