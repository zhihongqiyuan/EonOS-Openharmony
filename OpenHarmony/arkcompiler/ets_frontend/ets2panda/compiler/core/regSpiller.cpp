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

#include "regSpiller.h"
#include "compiler/core/codeGen.h"
#include "checker/types/type.h"

namespace ark::es2panda::compiler {

CodeGen *RegSpiller::GetCodeGen() const noexcept
{
    return cg_;
}

std::pair<RegSpiller::SpillInfo, const checker::Type *> RegSpiller::New() noexcept
{
    const VReg origin {VReg::REG_START - spillIndex_++};
    const auto *const originType = cg_->GetVRegType(origin);
    const SpillInfo spillInfo {origin, cg_->AllocRegWithType(originType)};
    return std::make_pair(spillInfo, originType);
}

void RegSpiller::Adjust(const std::unordered_set<VReg> &regs) noexcept
{
    while (true) {
        const VReg origin {VReg::REG_START - spillIndex_};

        if (regs.count(origin) == 0) {
            break;
        }

        ++spillIndex_;
    }
}

void RegSpiller::SetCodeGen(CodeGen &cg) noexcept
{
    cg_ = &cg;
}

std::uint32_t RegSpiller::SpillIndex() const noexcept
{
    return spillIndex_;
}

std::uint32_t &RegSpiller::SpillIndex() noexcept
{
    return spillIndex_;
}

RegScope DynamicRegSpiller::Start(CodeGen &cg)
{
    SetCodeGen(cg);
    regEnd_ = GetCodeGen()->NextReg().GetIndex();
    return RegScope {&cg};
}

RegSpiller::SpillInfo DynamicRegSpiller::Restore()
{
    const auto newSpillIndex = --SpillIndex();
    return RegSpiller::SpillInfo(VReg {VReg::REG_START - newSpillIndex}, VReg {regEnd_ - SpillIndex()});
}

bool DynamicRegSpiller::Restored() const
{
    return SpillIndex() == 0;
}

IRNode *DynamicRegSpiller::MoveReg(const ir::AstNode *const node, const VReg vd, const VReg vs,
                                   [[maybe_unused]] const bool spillMov)
{
    return GetCodeGen()->AllocMov(node, vd, vs);
}

void DynamicRegSpiller::Finalize() noexcept
{
    ASSERT(SpillIndex() == 0);
}

RegScope StaticRegSpiller::Start(CodeGen &cg)
{
    SetCodeGen(cg);
    return RegScope {&cg};
}

RegSpiller::SpillInfo StaticRegSpiller::Restore()
{
    ASSERT(spills_.size() <= VReg::REG_START);
    ASSERT(!spills_.empty());
    const auto last = spills_.back().Reversed();
    spills_.pop_back();
    return last;
}

bool StaticRegSpiller::Restored() const
{
    return spills_.empty();
}

IRNode *StaticRegSpiller::MoveReg(const ir::AstNode *const node, const VReg vd, const VReg vs, const bool spillMov)
{
    if (vd == vs) {
        return nullptr;
    }

    const auto *const sourceType = GetCodeGen()->GetVRegType(vs);
    if (sourceType == nullptr) {
        return nullptr;
    }

    GetCodeGen()->SetVRegType(vd, sourceType);
    if (spillMov) {
        spills_.emplace_back(vd, vs);
    }

    return GetCodeGen()->AllocMov(node, vd, vs);
}

void StaticRegSpiller::Finalize() noexcept
{
    SpillIndex() = 0;
}

}  // namespace ark::es2panda::compiler
