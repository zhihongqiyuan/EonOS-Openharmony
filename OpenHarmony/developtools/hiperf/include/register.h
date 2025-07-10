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

#ifndef HIPERF_REGISTER_H
#define HIPERF_REGISTER_H

#include <map>
#include <linux/perf_event.h>

#include "utilities.h"
#include "unwind_define.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
using namespace OHOS::HiviewDFX;

// these define copy from kernel uapi
enum PerfEventX86Regs {
    PERF_REG_X86_AX,
    PERF_REG_X86_BX,
    PERF_REG_X86_CX,
    PERF_REG_X86_DX,
    PERF_REG_X86_SI,
    PERF_REG_X86_DI,
    PERF_REG_X86_BP,
    PERF_REG_X86_SP,
    PERF_REG_X86_IP,
    PERF_REG_X86_FLAGS,
    PERF_REG_X86_CS,
    PERF_REG_X86_SS,
    PERF_REG_X86_DS,
    PERF_REG_X86_ES,
    PERF_REG_X86_FS,
    PERF_REG_X86_GS,
    PERF_REG_X86_R8,
    PERF_REG_X86_R9,
    PERF_REG_X86_R10,
    PERF_REG_X86_R11,
    PERF_REG_X86_R12,
    PERF_REG_X86_R13,
    PERF_REG_X86_R14,
    PERF_REG_X86_R15,
    PERF_REG_X86_32_MAX = PERF_REG_X86_GS + 1,
    PERF_REG_X86_64_MAX = PERF_REG_X86_R15 + 1,
};

enum PerfEventArm64Regs {
    PERF_REG_ARM64_X0,
    PERF_REG_ARM64_X1,
    PERF_REG_ARM64_X2,
    PERF_REG_ARM64_X3,
    PERF_REG_ARM64_X4,
    PERF_REG_ARM64_X5,
    PERF_REG_ARM64_X6,
    PERF_REG_ARM64_X7,
    PERF_REG_ARM64_X8,
    PERF_REG_ARM64_X9,
    PERF_REG_ARM64_X10,
    PERF_REG_ARM64_X11,
    PERF_REG_ARM64_X12,
    PERF_REG_ARM64_X13,
    PERF_REG_ARM64_X14,
    PERF_REG_ARM64_X15,
    PERF_REG_ARM64_X16,
    PERF_REG_ARM64_X17,
    PERF_REG_ARM64_X18,
    PERF_REG_ARM64_X19,
    PERF_REG_ARM64_X20,
    PERF_REG_ARM64_X21,
    PERF_REG_ARM64_X22,
    PERF_REG_ARM64_X23,
    PERF_REG_ARM64_X24,
    PERF_REG_ARM64_X25,
    PERF_REG_ARM64_X26,
    PERF_REG_ARM64_X27,
    PERF_REG_ARM64_X28,
    PERF_REG_ARM64_X29,
    PERF_REG_ARM64_LR,
    PERF_REG_ARM64_SP,
    PERF_REG_ARM64_PC,
    PERF_REG_ARM64_MAX,
};

enum PerfEventArmRegs {
    PERF_REG_ARM_R0,
    PERF_REG_ARM_R1,
    PERF_REG_ARM_R2,
    PERF_REG_ARM_R3,
    PERF_REG_ARM_R4,
    PERF_REG_ARM_R5,
    PERF_REG_ARM_R6,
    PERF_REG_ARM_R7,
    PERF_REG_ARM_R8,
    PERF_REG_ARM_R9,
    PERF_REG_ARM_R10,
    PERF_REG_ARM_FP = 11,
    PERF_REG_ARM_IP = 12,
    PERF_REG_ARM_SP = 13,
    PERF_REG_ARM_LR = 14,
    PERF_REG_ARM_PC = 15,
    PERF_REG_ARM_MAX,
};

// context name
static const std::map<uint64_t, const std::string> PERF_CONTEXT_NAME = {
    {PERF_CONTEXT_HV, "PERF_CONTEXT_HV"},
    {PERF_CONTEXT_KERNEL, "PERF_CONTEXT_KERNEL"},
    {PERF_CONTEXT_USER, "PERF_CONTEXT_USER"},
    {PERF_CONTEXT_GUEST, "PERF_CONTEXT_GUEST"},
    {PERF_CONTEXT_GUEST_KERNEL, "PERF_CONTEXT_GUEST_KERNEL"},
    {PERF_CONTEXT_GUEST_USER, "PERF_CONTEXT_GUEST_USER"},
    {PERF_CONTEXT_MAX, "PERF_CONTEXT_MAX"},
};

#if defined(target_cpu_x86_64)
constexpr ArchType BUILD_ARCH_TYPE = ArchType::ARCH_X86_64;
#elif defined(target_cpu_arm64)
constexpr ArchType BUILD_ARCH_TYPE = ArchType::ARCH_ARM64;
#elif defined(target_cpu_arm)
constexpr ArchType BUILD_ARCH_TYPE = ArchType::ARCH_ARM;
#else
#error NOT SUPPORT ARCH
#endif

const std::string UpdatePerfContext(uint64_t addr, perf_callchain_context &perfCallchainContext);
const std::string GetArchName(ArchType arch);
uint64_t GetSupportedRegMask(ArchType arch);

// this is only for debug
const std::string RegisterGetName(size_t registerIndex);

bool RegisterGetValue(uint64_t &value, const u64 registers[], const size_t registerIndex,
                      const size_t registerNumber);

size_t RegisterGetSP(ArchType arch);
size_t RegisterGetIP(ArchType arch);

inline bool RegisterGetSPValue(uint64_t &value, ArchType arch, const u64 registers[],
                               const size_t registerNumber)
{
    return RegisterGetValue(value, registers, RegisterGetSP(arch), registerNumber);
}

inline bool RegisterGetIPValue(uint64_t &value, ArchType arch, const u64 registers[],
                               const size_t registerNumber)
{
    return RegisterGetValue(value, registers, RegisterGetIP(arch), registerNumber);
}

int LibunwindRegIdToPerfReg(int regnum);

ArchType GetDeviceArch();
ArchType SetDeviceArch(ArchType arch);
ArchType GetArchTypeFromUname(const std::string &machine);
ArchType GetArchTypeFromABI(bool abi32);
void UpdateRegForABI(ArchType arch, u64 *registers);
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_REGISTER_H
