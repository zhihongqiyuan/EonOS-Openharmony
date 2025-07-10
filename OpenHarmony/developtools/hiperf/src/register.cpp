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

// for libunwind.h empty struct has size 0 in c, size 1 in c++
#include "register.h"
#if !is_mingw
#include <sys/utsname.h>
#endif
#include "debug_logger.h"


namespace OHOS {
namespace Developtools {
namespace HiPerf {
static ArchType g_deviceArchType = ArchType::ARCH_UNKNOWN;
// order is IP , SP for ut
static const std::map<size_t, const std::string> PERF_REG_NAME_MAP = {
#if defined(target_cpu_x86_64)
    {PERF_REG_X86_IP, "PERF_REG_X86_IP"},
    {PERF_REG_X86_SP, "PERF_REG_X86_SP"},
#elif defined(target_cpu_arm)
    {PERF_REG_ARM_PC, "PERF_REG_ARM_PC"},
    {PERF_REG_ARM_SP, "PERF_REG_ARM_SP"},
#elif defined(target_cpu_arm64)
    {PERF_REG_ARM64_PC, "PERF_REG_ARM64_PC"},
    {PERF_REG_ARM64_SP, "PERF_REG_ARM64_SP"},
#endif
};

// these copy from kerne uapi perf_regs.h
uint64_t GetSupportedRegMask(ArchType arch)
{
    uint64_t result = 0;
    switch (arch) {
        case ArchType::ARCH_X86:
            result = ((1ULL << PERF_REG_X86_32_MAX) - 1);
            break;
        case ArchType::ARCH_X86_64:
            result = ((1ULL << PERF_REG_X86_64_MAX) - 1);
            result &= ~((1ULL << PERF_REG_X86_DS) | (1ULL << PERF_REG_X86_ES) |
                        (1ULL << PERF_REG_X86_FS) | (1ULL << PERF_REG_X86_GS));
            break;
        case ArchType::ARCH_ARM:
            result = ((1ULL << PERF_REG_ARM_MAX) - 1);
            break;
        case ArchType::ARCH_ARM64:
            result = ((1ULL << PERF_REG_ARM64_MAX) - 1);
            break;
        default:
            result = std::numeric_limits<uint64_t>::max();
            HLOGE("unsupport arch %u", arch);
            break;
    }
    return result;
}

const std::string UpdatePerfContext(uint64_t addr, perf_callchain_context &perfCallchainContext)
{
    if (PERF_CONTEXT_NAME.count(addr) != 0) {
        perfCallchainContext = static_cast<perf_callchain_context>(addr);
        return StringPrintf("%s: %" PRIx64 "", PERF_CONTEXT_NAME.at(addr).c_str(), addr);
    } else {
        perfCallchainContext = PERF_CONTEXT_MAX;
        return StringPrintf("unknow context: %" PRIx64 "", addr);
    }
}

const std::string GetArchName(ArchType arch)
{
    switch (arch) {
        case ArchType::ARCH_X86:
            return "X86_32";
        case ArchType::ARCH_X86_64:
            return "X86_64";
        case ArchType::ARCH_ARM:
            return "ARM";
        case ArchType::ARCH_ARM64:
            return "ARM64";
        default:
            return "Unsupport";
    }
}

size_t RegisterGetIP(ArchType arch)
{
    switch (arch) {
        case ArchType::ARCH_X86:
        case ArchType::ARCH_X86_64:
            return PERF_REG_X86_IP;
        case ArchType::ARCH_ARM:
            return PERF_REG_ARM_PC;
        case ArchType::ARCH_ARM64:
            return PERF_REG_ARM64_PC;
        default:
            return std::numeric_limits<size_t>::max();
    }
}

size_t RegisterGetSP(ArchType arch)
{
    switch (arch) {
        case ArchType::ARCH_X86:
        case ArchType::ARCH_X86_64:
            return PERF_REG_X86_SP;
        case ArchType::ARCH_ARM:
            return PERF_REG_ARM_SP;
        case ArchType::ARCH_ARM64:
            return PERF_REG_ARM64_SP;
        default:
            return std::numeric_limits<size_t>::max();
    }
}

const std::string RegisterGetName(size_t registerIndex)
{
    std::string name;
    name.append("PerfReg[");
    name.append(std::to_string(registerIndex));
    if (PERF_REG_NAME_MAP.count(registerIndex) > 0) {
        name.append(":");
        name.append(PERF_REG_NAME_MAP.at(registerIndex));
    }
    name.append("]");
    return name;
}

bool RegisterGetValue(uint64_t &value, const u64 registers[], const size_t registerIndex,
                      const size_t registerNumber)
{
    if (registerIndex >= registerNumber) {
        HLOGE("registerIndex is %zu, max is %zu", registerIndex, registerNumber);
        return false;
    }
    value = registers[registerIndex];
    return true;
}

ArchType GetArchTypeFromUname(const std::string &machine)
{
    if (StringStartsWith(machine, "arm")) {
        if (machine == "armv8l") {
            // 32 bit elf run in 64 bit cpu
            return ArchType::ARCH_ARM64;
        }
        return ArchType::ARCH_ARM;
    } else if (machine == "aarch64") {
        return ArchType::ARCH_ARM64;
    } else if (machine == "x86_64") {
        return ArchType::ARCH_X86_64;
    } else if (machine == "x86" || machine == "i686") {
        return ArchType::ARCH_X86;
    } else {
        HLOGE("unsupport machine %s", machine.c_str());
        return ArchType::ARCH_UNKNOWN;
    }
}

ArchType GetArchTypeFromABI(bool abi32)
{
    if (g_deviceArchType == ArchType::ARCH_UNKNOWN) {
        g_deviceArchType = GetDeviceArch();
    }
    if (abi32) {
        if (g_deviceArchType == ArchType::ARCH_ARM64) {
            return ArchType::ARCH_ARM;
        } else if (g_deviceArchType == ArchType::ARCH_X86_64) {
            return ArchType::ARCH_X86;
        }
    }
    return g_deviceArchType;
}

ArchType SetDeviceArch(ArchType arch)
{
    HLOGD("g_deviceArchType change to  %s", GetArchName(arch).c_str());
    g_deviceArchType = arch;
    return g_deviceArchType;
}

ArchType GetDeviceArch()
{
#if defined(is_mingw) && is_mingw
    return g_deviceArchType;
#else
    if (g_deviceArchType != ArchType::ARCH_UNKNOWN) {
        return g_deviceArchType;
    } else {
        utsname systemName;
        if ((uname(&systemName)) != 0) {
            // fallback
            g_deviceArchType = BUILD_ARCH_TYPE;
        } else {
            g_deviceArchType = GetArchTypeFromUname(systemName.machine);
            HLOGD("machine arch is %s : %s", systemName.machine,
                  GetArchName(g_deviceArchType).c_str());
            if (g_deviceArchType == ArchType::ARCH_UNKNOWN) {
                g_deviceArchType = BUILD_ARCH_TYPE;
            }
        }
    }
    return g_deviceArchType;
#endif
}

void UpdateRegForABI(ArchType arch, u64 *regs)
{
    if (g_deviceArchType == ArchType::ARCH_ARM64 && arch == ArchType::ARCH_ARM) {
        // arm in arm64
        regs[PERF_REG_ARM_PC] = regs[PERF_REG_ARM64_PC];
    }
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
