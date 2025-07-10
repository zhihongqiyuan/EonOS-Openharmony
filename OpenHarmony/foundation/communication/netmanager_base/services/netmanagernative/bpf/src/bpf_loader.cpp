/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <cerrno>
#include <fcntl.h>
#include <filesystem>
#include <functional>
#include <iostream>
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <map>
#include <memory.h>
#include <string>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <vector>

#include "bpf_def.h"
#include "bpf_loader.h"
#include "elf_types.hpp"
#include "elfio.hpp"
#include "elfio_relocation.hpp"
#include "net_manager_constants.h"
#include "netnative_log_wrapper.h"
#include "securec.h"

#define DEFINE_SECTION_NAME(name) \
    {                             \
        name, strlen(name)        \
    }

#define DEFINE_PROG_TYPE(progName, progType) \
    {                                        \
        progName, progType                   \
    }
#define DEFINE_ATTACH_TYPE(progName, attachType, needExpectedAttach) \
    {                                                               \
        progName, attachType, needExpectedAttach                    \
    }

namespace OHOS::NetManagerStandard {
static constexpr const char *BPF_DIR = "/sys/fs/bpf";
static constexpr const char *CGROUP_DIR = "/sys/fs/cgroup";
static constexpr const char *MAPS_DIR = "/sys/fs/bpf/netsys/maps";
static constexpr const char *PROGS_DIR = "/sys/fs/bpf/netsys/progs";

// There is no limit to the size of SECTION_NAMES.
static const struct SectionName {
    const char *sectionName;
    size_t sectionNameLength;
} SECTION_NAMES[] = {
    DEFINE_SECTION_NAME("kprobe/"),
    DEFINE_SECTION_NAME("kretprobe/"),
    DEFINE_SECTION_NAME("tracepoint/"),
    DEFINE_SECTION_NAME("raw_tracepoint/"),
    DEFINE_SECTION_NAME("xdp"),
    DEFINE_SECTION_NAME("perf_event/"),
    DEFINE_SECTION_NAME("socket"),
    DEFINE_SECTION_NAME("cgroup/"),
    DEFINE_SECTION_NAME("sockops"),
    DEFINE_SECTION_NAME("sk_skb"),
    DEFINE_SECTION_NAME("sk_msg"),
    DEFINE_SECTION_NAME("cgroup_skb"),
    DEFINE_SECTION_NAME("xdp_packet_parser"),
    DEFINE_SECTION_NAME("schedcls"),
    DEFINE_SECTION_NAME("classifier"),
    DEFINE_SECTION_NAME("cgroup_sock"),
    DEFINE_SECTION_NAME("cgroup_addr"),
};

static const constexpr struct {
    const char *event;
    bpf_prog_type progType;
} PROG_TYPES[] = {
    DEFINE_PROG_TYPE("socket", BPF_PROG_TYPE_SOCKET_FILTER),
    DEFINE_PROG_TYPE("cgroup_skb", BPF_PROG_TYPE_CGROUP_SKB),
    DEFINE_PROG_TYPE("xdp", BPF_PROG_TYPE_XDP),
    DEFINE_PROG_TYPE("xdp_packet_parser", BPF_PROG_TYPE_XDP),
    DEFINE_PROG_TYPE("schedcls", BPF_PROG_TYPE_SCHED_CLS),
    DEFINE_PROG_TYPE("classifier", BPF_PROG_TYPE_SCHED_CLS),
    DEFINE_PROG_TYPE("cgroup_sock", BPF_PROG_TYPE_CGROUP_SOCK),
    DEFINE_PROG_TYPE("cgroup_addr", BPF_PROG_TYPE_CGROUP_SOCK_ADDR),
};

static const constexpr struct {
    const char *progName;
    bpf_attach_type attachType;
    bool needExpectedAttach;
} PROG_ATTACH_TYPES[] = {
    DEFINE_ATTACH_TYPE("cgroup_sock_inet_create_socket", BPF_CGROUP_INET_SOCK_CREATE, false),
    DEFINE_ATTACH_TYPE("cgroup_sock_inet_release_socket", BPF_CGROUP_INET_SOCK_RELEASE, true),
    DEFINE_ATTACH_TYPE("cgroup_skb_uid_ingress", BPF_CGROUP_INET_INGRESS, false),
    DEFINE_ATTACH_TYPE("cgroup_skb_uid_egress", BPF_CGROUP_INET_EGRESS, false),
    DEFINE_ATTACH_TYPE("cgroup_addr_bind4", BPF_CGROUP_INET4_BIND, true),
    DEFINE_ATTACH_TYPE("cgroup_addr_bind6", BPF_CGROUP_INET6_BIND, true),
    DEFINE_ATTACH_TYPE("cgroup_addr_connect4", BPF_CGROUP_INET4_CONNECT, true),
    DEFINE_ATTACH_TYPE("cgroup_addr_connect6", BPF_CGROUP_INET6_CONNECT, true),
    DEFINE_ATTACH_TYPE("cgroup_addr_sendmsg4", BPF_CGROUP_UDP4_SENDMSG, true),
    DEFINE_ATTACH_TYPE("cgroup_addr_sendmsg6", BPF_CGROUP_UDP6_SENDMSG, true),
};

int32_t g_sockFd = -1;

struct BpfMapData {
    BpfMapData() : fd(0)
    {
        if (memset_s(&def, sizeof(def), 0, sizeof(def)) != EOK) {
            NETNATIVE_LOGE("memset_s error");
        }
    }

    int32_t fd;
    std::string name;
    bpf_map_def def{};
};

template <typename type> inline uint64_t PtrToU64(const type ptr)
{
    return static_cast<uint64_t>(reinterpret_cast<uintptr_t>(ptr));
}

inline bool EndsWith(const std::string &str, const std::string &searchFor)
{
    if (searchFor.size() > str.size()) {
        return false;
    }

    std::string source = str.substr(str.size() - searchFor.size(), searchFor.size());
    return source == searchFor;
}

inline int32_t SysBpf(bpf_cmd cmd, bpf_attr *attr, uint32_t size)
{
    return static_cast<int32_t>(syscall(__NR_bpf, cmd, attr, size));
}

inline int32_t SysBpfObjGet(const std::string &pathName, uint32_t fileFlags)
{
    bpf_attr attr = {};
    if (memset_s(&attr, sizeof(attr), 0, sizeof(attr)) != EOK) {
        return NETMANAGER_ERROR;
    }
    attr.pathname = PtrToU64(pathName.c_str());
    attr.file_flags = fileFlags;
    return SysBpf(BPF_OBJ_GET, &attr, sizeof(attr));
}

inline int32_t SysBpfObjPin(int32_t fd, const std::string &pathName)
{
    bpf_attr attr = {};

    if (memset_s(&attr, sizeof(attr), 0, sizeof(attr)) != EOK) {
        return NETMANAGER_ERROR;
    }
    attr.pathname = PtrToU64(pathName.c_str());
    if (fd < 0) {
        return NETMANAGER_ERROR;
    }
    attr.bpf_fd = static_cast<uint32_t>(fd);

    return SysBpf(BPF_OBJ_PIN, &attr, sizeof(attr));
}

inline int32_t SysBpfProgLoad(bpf_attr *attr, uint32_t size)
{
    int32_t fd = SysBpf(BPF_PROG_LOAD, attr, size);
    while (fd < 0 && errno == EAGAIN) {
        fd = SysBpf(BPF_PROG_LOAD, attr, size);
    }

    return fd;
}

inline int32_t SysBpfObjDetach(bpf_attach_type type, const int progFd, const int cgFd)
{
    bpf_attr attr = {};

    if (memset_s(&attr, sizeof(attr), 0, sizeof(attr)) != EOK) {
        return NETMANAGER_ERROR;
    }
    attr.target_fd = cgFd;
    attr.attach_bpf_fd = progFd;
    attr.attach_type = type;

    return SysBpf(BPF_PROG_DETACH, &attr, sizeof(attr));
}

inline int32_t SysBpfObjAttach(bpf_attach_type type, const int progFd, const int cgFd)
{
    bpf_attr attr = {};

    if (memset_s(&attr, sizeof(attr), 0, sizeof(attr)) != EOK) {
        return NETMANAGER_ERROR;
    }
    attr.target_fd = cgFd;
    attr.attach_bpf_fd = progFd;
    attr.attach_type = type;
    attr.attach_flags = BPF_F_ALLOW_MULTI;

    return SysBpf(BPF_PROG_ATTACH, &attr, sizeof(attr));
}

inline bool MatchSecName(const std::string &name)
{
    auto matchFunc = [name](const SectionName &sec) -> bool {
        if (name.size() < sec.sectionNameLength) {
            return false;
        }
        return memcmp(name.c_str(), sec.sectionName, sec.sectionNameLength) == 0;
    };
    auto size = sizeof(SECTION_NAMES) / sizeof(SectionName);
    return std::any_of(SECTION_NAMES, SECTION_NAMES + size, matchFunc);
}

inline int32_t UnPin(const std::string &path)
{
    return unlink(path.c_str());
}

class ElfLoader {
public:
    explicit ElfLoader(std::string path) : path_(std::move(path)), kernVersion_(0) {}

    ElfLoadError Unload() const
    {
        const struct {
            uint32_t index;
            const char *infoMsg;
            std::function<ElfLoadError()> fun;
            const char *errorMsg;
        } funList[]{
            {1, "path is valid", isPathValid_, "path is not valid"},
            {2, "load elf file ok", loadElfFile_, "load elf file failed"},
            {3, "load elf map section ok", loadElfMapsSection_, "load elf map section failed"},
            {4, "delete maps ok", deleteMaps_, "delete maps failed"},
            {5, "unload progs ok", unloadProgs_, "unload progs ok"},
        };

        for (const auto &fun : funList) {
            auto ret = fun.fun();
            if (ret != ELF_LOAD_ERR_NONE) {
                NETNATIVE_LOGE("error msg is %{public}s", fun.errorMsg);
                return static_cast<ElfLoadError>(ret);
            }
            NETNATIVE_LOGI("the %{public}u step: %{public}s", fun.index, fun.infoMsg);
        }

        return ELF_LOAD_ERR_NONE;
    }

    ElfLoadError Load() const
    {
        const struct {
            uint32_t index;
            const char *infoMsg;
            std::function<ElfLoadError()> fun;
            const char *errorMsg;
        } funList[]{
            {1, "path is valid", isPathValid_, "path is not valid"},
            {2, "make directories fs ok", makeDirectories, "make directories fs failed"},
            {3, "load elf file ok", loadElfFile_, "load elf file failed"},
            {4, "version is valid", isVersionValid_, "version is not valid"},
            {5, "set license and version ok", setLicenseAndVersion_, "set license and version failed"},
            {6, "load elf map section ok", loadElfMapsSection_, "load elf map section failed"},
            {7, "set rlimit ok", setRlimit_, "set rlimit failed"},
            {8, "create maps ok", createMaps_, "create maps failed"},
            {9, "parse relocation ok", parseRelocation_, "parse relocation failed"},
            {10, "load progs ok", loadProgs_, "load progs failed"},
        };
        for (const auto &fun : funList) {
            auto ret = fun.fun();
            if (ret != ELF_LOAD_ERR_NONE) {
                NETNATIVE_LOGE("error msg is %{public}s", fun.errorMsg);
                return static_cast<ElfLoadError>(ret);
            }
            NETNATIVE_LOGI("the %{public}u step: %{public}s", fun.index, fun.infoMsg);
        }

        return ELF_LOAD_ERR_NONE;
    }

private:
    bool CheckPath()
    {
        if (path_.empty() || !std::filesystem::exists(path_) || std::filesystem::is_directory(path_)) {
            return false;
        }

        return true;
    }

    bool IsPathValid()
    {
        if (!CheckPath()) {
            return false;
        }
        return EndsWith(path_, ".o");
    }

    bool LoadElfFile()
    {
        return elfIo_.load(path_);
    }

    bool IsVersionValid()
    {
        return elfIo_.get_version() == ELFIO::EV_CURRENT;
    }

    static bool SetRlimit()
    {
        rlimit r = {RLIM_INFINITY, RLIM_INFINITY};
        return setrlimit(RLIMIT_MEMLOCK, &r) >= 0;
    }

    static bool IsMounted(const std::string &dir)
    {
        std::ifstream ifs("/proc/mounts", std::ios::in);
        if (!ifs.is_open()) {
            return false;
        }

        std::string s;
        while (std::getline(ifs, s)) {
            if (s.find(dir) != std::string::npos) {
                ifs.close();
                return true;
            }
        }
        ifs.close();
        return false;
    }

    static bool MakeDir(const std::string &dir)
    {
        if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
            return true;
        }
        if (!std::filesystem::exists(dir)) {
            if (!std::filesystem::create_directories(std::filesystem::path(dir))) {
                NETNATIVE_LOGE("filesystem make dir err %{public}d", errno);
                return false;
            }
            return true;
        }
        if (!std::filesystem::is_directory(dir)) {
            NETNATIVE_LOGE("%{public}s is a file", dir.c_str());
            return false;
        }
        return true;
    }

    static bool MakeDirectories()
    {
        if (IsMounted(BPF_DIR) && std::filesystem::exists(MAPS_DIR) && std::filesystem::is_directory(MAPS_DIR) &&
            std::filesystem::exists(PROGS_DIR) && std::filesystem::is_directory(PROGS_DIR)) {
            NETNATIVE_LOGE("%{public}s", "bpf directories are exists");
            return true;
        }
        if (!IsMounted(BPF_DIR) && mount(BPF_DIR, BPF_DIR, "bpf", MS_RELATIME, nullptr) < 0) {
            NETNATIVE_LOGE("mount bpf fs failed: errno = %{public}d", errno);
            return false;
        }
        if (!MakeDir(MAPS_DIR)) {
            NETNATIVE_LOGE("can not make dir: %{public}s", MAPS_DIR);
            return false;
        }
        if (!MakeDir(PROGS_DIR)) {
            NETNATIVE_LOGE("can not make dir: %{public}s", PROGS_DIR);
            return false;
        }

        if (!IsMounted(CGROUP_DIR) && mount(CGROUP_DIR, CGROUP_DIR, "cgroup2", MS_RELATIME, nullptr) < 0) {
            NETNATIVE_LOGE("mount cgroup fs failed: errno = %{public}d", errno);
            return false;
        }
        return true;
    }

    bool SetLicenseAndVersion()
    {
        return std::all_of(elfIo_.sections.begin(), elfIo_.sections.end(), [this](const auto &section) {
            if (section->get_name() == "license") {
                license_ = section->get_data();
                if (license_.empty()) {
                    return false;
                }
            } else if (section->get_name() == "version") {
                try {
                    kernVersion_ = std::stoi(section->get_data());
                } catch (const std::invalid_argument& e) {
                    NETNATIVE_LOGE("invalid_argument");
                    return false;
                } catch (const std::out_of_range& e) {
                    NETNATIVE_LOGE("out_of_range");
                    return false;
                }
                if (kernVersion_ == 0) {
                    return false;
                }
            }

            return true;
        });
    }

    std::map<ELFIO::Elf64_Addr, std::string> LoadElfMapSectionCore()
    {
        std::map<ELFIO::Elf64_Addr, std::string> mapName;
        for (const auto &section : elfIo_.sections) {
            if (section->get_type() != ELFIO::SHT_SYMTAB && section->get_type() != ELFIO::SHT_DYNSYM) {
                continue;
            }
            ELFIO::symbol_section_accessor symbols(elfIo_, section.get());
            for (ELFIO::Elf_Xword i = 0; i < symbols.get_symbols_num(); i++) {
                std::string name;
                ELFIO::Elf64_Addr value = 0;
                ELFIO::Elf_Xword size = 0;
                unsigned char bind = 0;
                unsigned char type = 0;
                ELFIO::Elf_Half elfSection = 0;
                unsigned char other = 0;
                symbols.get_symbol(i, name, value, size, bind, type, elfSection, other);
                if (type != ELFIO::STT_OBJECT || !EndsWith(name, "_map")) {
                    continue;
                }
                if (mapName.find(value) != mapName.end()) {
                    return {};
                }
                mapName[value] = name;
            }
        }
        return mapName;
    }

    bool LoadElfMapsSection()
    {
        if (elfIo_.sections.size() == 0) {
            return false;
        }

        auto it = std::find_if(elfIo_.sections.begin(), elfIo_.sections.end(),
                               [](const auto &section) { return section->get_name() == "maps"; });
        if (it == elfIo_.sections.end()) {
            return true;
        }

        ELFIO::section *mapsSection = it->get();
        auto defs = reinterpret_cast<const bpf_map_def *>(mapsSection->get_data());
        auto mapNum = mapsSection->get_size() / sizeof(bpf_map_def);
        for (size_t i = 0; i < static_cast<size_t>(mapNum); i++) {
            BpfMapData map;
            map.def = defs[i];
            maps_.emplace_back(map);
        }
        auto mapName = LoadElfMapSectionCore();
        if (mapName.size() != maps_.size()) {
            return false;
        }
        size_t mapIndex = 0;
        for (const auto &[addr, name] : mapName) {
            maps_[mapIndex].name = name;
            ++mapIndex;
        }

        return true;
    }

    static void PrintMapAttr(const bpf_attr &attr)
    {
        NETNATIVE_LOGI("%{public}s", "BPF_MAP_CREATE:");
        NETNATIVE_LOGI("  .map_type    = %{public}u", attr.map_type);
        NETNATIVE_LOGI("  .key_size    = %{public}u", attr.key_size);
        NETNATIVE_LOGI("  .value_size  = %{public}u", attr.value_size);
        NETNATIVE_LOGI("  .max_entries = %{public}u", attr.max_entries);
        NETNATIVE_LOGI("  .map_flags   = %{public}u", attr.map_flags);
        NETNATIVE_LOGI("  .map_name    = %{public}s", attr.map_name);
    }

    static int32_t BpfCreateMapNode(const BpfMapData &map)
    {
        bpf_attr attr = {};
        if (memset_s(&attr, sizeof(attr), 0, sizeof(attr)) != EOK) {
            return NETMANAGER_ERROR;
        }
        attr.map_type = map.def.type;
        attr.key_size = map.def.key_size;
        attr.value_size = map.def.value_size;
        attr.max_entries = map.def.max_entries;
        attr.map_flags = map.def.map_flags;
        if (!map.name.empty()) {
            if (memcpy_s(attr.map_name, sizeof(attr.map_name) - 1, map.name.c_str(),
                         std::min<size_t>(map.name.size(), sizeof(attr.map_name) - 1)) != EOK) {
                NETNATIVE_LOGE("Failed copy map name %{public}s", map.name.c_str());
                return NETMANAGER_ERROR;
            }
        }
        attr.numa_node = (map.def.map_flags & static_cast<unsigned int>(BPF_F_NUMA_NODE)) ? map.def.numa_node : 0;
        PrintMapAttr(attr);

        auto fd = SysBpf(BPF_MAP_CREATE, &attr, sizeof(attr));
        if (fd < 0) {
            NETNATIVE_LOGE("__NR_bpf, BPF_MAP_CREATE failed %{public}d %{public}d %{public}d", __NR_bpf, fd, errno);
        }
        return fd;
    }

    bool CreateMaps()
    {
        for (auto &map : maps_) {
            auto fd = BpfCreateMapNode(map);
            if (fd < 0) {
                NETNATIVE_LOGE("Failed create map (%{public}s): %{public}d", map.name.c_str(), fd);
                return false;
            }

            map.fd = fd;
            std::string mapPinLocation = std::string(MAPS_DIR) + "/" + map.name;
            if (access(mapPinLocation.c_str(), F_OK) == 0) {
                NETNATIVE_LOGI("map: %{public}s has already been pinned", mapPinLocation.c_str());
            } else {
                if (SysBpfObjPin(fd, mapPinLocation) < 0) {
                    NETNATIVE_LOGE("Failed to pin map: %{public}s, errno = %{public}d", mapPinLocation.c_str(), errno);
                    return false;
                }
            }
        }
        return true;
    }

    bool DeleteMaps()
    {
        return std::all_of(maps_.begin(), maps_.end(), [](const auto &map) {
            std::string mapPinLocation = std::string(MAPS_DIR) + "/" + map.name;
            if (access(mapPinLocation.c_str(), F_OK) == 0) {
                auto ret = UnPin(mapPinLocation);
                return ret >= 0;
            }
            return true;
        });
    }

    bool ApplyRelocation(bpf_insn *insn, ELFIO::section *section) const
    {
        if (insn == nullptr || section == nullptr || section->get_entry_size() == 0) {
            return false;
        }

        auto size = section->get_size() / section->get_entry_size();
        if (size == 0) {
            return false;
        }

        ELFIO::Elf64_Addr offset = 0;
        ELFIO::Elf64_Addr symbolValue = 0;
        std::string symbolName;
        ELFIO::Elf_Word type = 0;
        ELFIO::Elf_Sxword addend = 0;
        ELFIO::Elf_Sxword calcValue = 0;
        ELFIO::relocation_section_accessor relocation(elfIo_, section);
        for (size_t i = 0; i < size; i++) {
            relocation.get_entry(i, offset, symbolValue, symbolName, type, addend, calcValue);
            uint32_t index = offset / sizeof(bpf_insn);
            if (insn[index].code != (BPF_LD | BPF_IMM | BPF_DW)) {
                NETNATIVE_LOGE("Invalid relo for insn[%{public}u].code 0x%{public}x 0x%{public}x", index,
                               insn[index].code, (BPF_LD | BPF_IMM | BPF_DW));
                continue;
            }

            size_t mapIdx;
            bool match = false;
            for (mapIdx = 0; mapIdx < maps_.size(); mapIdx++) {
                if (maps_[mapIdx].name == symbolName) {
                    match = true;
                    break;
                }
            }
            if (!match) {
                NETNATIVE_LOGE("Invalid relo for insn[%{public}u] no map_data match %{public}s index %{public}zu",
                               index, section->get_name().c_str(), i);
                continue;
            }
            insn[index].src_reg = BPF_PSEUDO_MAP_FD;
            insn[index].imm = maps_[mapIdx].fd;
        }
        return true;
    }

    int32_t BpfLoadProgram(std::string &progName, bpf_prog_type type, const bpf_insn *insns, size_t insnsCnt)
    {
        if (insns == nullptr) {
            return NETMANAGER_ERROR;
        }

        bpf_attr attr = {};
        if (memset_s(&attr, sizeof(attr), 0, sizeof(attr)) != EOK) {
            return NETMANAGER_ERROR;
        }
        attr.prog_type = type;
        if (kernVersion_ < 0) {
            return NETMANAGER_ERROR;
        }
        attr.kern_version = static_cast<uint32_t>(kernVersion_);
        attr.insn_cnt = static_cast<uint32_t>(insnsCnt);
        attr.insns = PtrToU64(insns);
        attr.license = PtrToU64(license_.c_str());
        for (const auto &prog : PROG_ATTACH_TYPES) {
            if (prog.progName != nullptr && progName == prog.progName) {
                if (prog.needExpectedAttach) {
                    attr.expected_attach_type = prog.attachType;
                }
                NETNATIVE_LOGW(
                    "BpfLoadProgram progName[%{public}s] attrProgType[%{public}u], "
                    "attrExpectedAttachType[%{public}u]",
                    progName.c_str(), attr.prog_type, attr.expected_attach_type);
                break;
            }
        }

        return SysBpfProgLoad(&attr, sizeof(attr));
    }

    static bpf_prog_type ConvertEventToProgType(const std::string &event)
    {
        for (const auto &prog : PROG_TYPES) {
            size_t size = strlen(prog.event);
            if (event.size() < size) {
                continue;
            }
            if (memcmp(event.c_str(), prog.event, size) == 0) {
                return prog.progType;
            }
        }
        return static_cast<bpf_prog_type>(NETMANAGER_ERROR);
    }

    static bool DoAttach(int32_t progFd, const std::string &progName)
    {
        if (progName.size() < 1) {
            NETNATIVE_LOGE("progName is null");
            return false;
        }
        NETNATIVE_LOG_D("The progName = %{public}s", progName.c_str());

        for (const auto &prog : PROG_ATTACH_TYPES) {
            if (prog.progName != nullptr && progName == prog.progName) {
                int cgroupFd = open(CGROUP_DIR, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
                if (cgroupFd < 0) {
                    NETNATIVE_LOGE("open CGROUP_DIR failed: errno = %{public}d", errno);
                    return false;
                }

                if (SysBpfObjAttach(prog.attachType, progFd, cgroupFd) < NETSYS_SUCCESS) {
                    NETNATIVE_LOGE("attach %{public}s failed: errno = %{public}d", progName.c_str(), errno);
                    close(cgroupFd);
                    return false;
                }

                close(cgroupFd);
                return true;
            }
        }

        return true;
    }

    static void DoDetach(const std::string &progPinLocation, const std::string &progName)
    {
        if (progName.size() < 1) {
            NETNATIVE_LOGE("progName is null");
            return;
        }
        NETNATIVE_LOG_D("The progName = %{public}s", progName.c_str());

        for (const auto &prog : PROG_ATTACH_TYPES) {
            if (prog.progName != nullptr && progName == prog.progName) {
                int cgroupFd = open(CGROUP_DIR, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
                if (cgroupFd < NETSYS_SUCCESS) {
                    NETNATIVE_LOGE("open CGROUP_DIR failed: errno = %{public}d", errno);
                    return;
                }

                auto progFd = SysBpfObjGet(progPinLocation, 0);
                if (progFd < NETSYS_SUCCESS) {
                    close(cgroupFd);
                    return;
                }

                if (SysBpfObjDetach(prog.attachType, progFd, cgroupFd) < NETSYS_SUCCESS) {
                    NETNATIVE_LOGE("detach %{public}s failed: errno = %{public}d", progName.c_str(), errno);
                    close(cgroupFd);
                    return;
                }

                close(cgroupFd);
                return;
            }
        }
    }

    bool LoadProg(const std::string &event, const bpf_insn *insn, size_t insnCnt)
    {
        auto progType = ConvertEventToProgType(event);
        if (progType < NETSYS_SUCCESS) {
            NETNATIVE_LOGE("unsupported program type: %{public}s", event.c_str());
            return false;
        }

        if (insn == nullptr) {
            NETNATIVE_LOGE("insn is null");
            return false;
        }

        std::string progName = event;
        std::replace(progName.begin(), progName.end(), '/', '_');
        int32_t progFd = BpfLoadProgram(progName, progType, insn, insnCnt);
        if (progFd < NETSYS_SUCCESS) {
            NETNATIVE_LOGE("Failed to load bpf prog, error = %{public}d", errno);
            return false;
        }

        std::string progPinLocation = std::string(PROGS_DIR) + "/" + progName;
        if (access(progPinLocation.c_str(), F_OK) == 0) {
            NETNATIVE_LOGI("prog: %{public}s has already been pinned", progPinLocation.c_str());
        } else {
            if (SysBpfObjPin(progFd, progPinLocation) < NETSYS_SUCCESS) {
                NETNATIVE_LOGE("Failed to pin prog: %{public}s, errno = %{public}d", progPinLocation.c_str(), errno);
                close(progFd);
                return false;
            }
        }

        /* attach socket filter */
        if (progType == BPF_PROG_TYPE_SOCKET_FILTER) {
            if (g_sockFd < 0) {
                NETNATIVE_LOGE("create socket failed, %{public}d, err: %{public}d", g_sockFd, errno);
                close(progFd);
                /* return true to ignore this prog */
                return true;
            }
            if (setsockopt(g_sockFd, SOL_SOCKET, SO_ATTACH_BPF, &progFd, sizeof(progFd)) < 0) {
                NETNATIVE_LOGE("attach socket failed, err: %{public}d", errno);
                close(g_sockFd);
                g_sockFd = -1;
            }
            close(progFd);
            return true;
        } else {
            auto ret = DoAttach(progFd, progName);
            close(progFd);
            return ret;
        }
    }

    bool ParseRelocation()
    {
        return std::all_of(elfIo_.sections.begin(), elfIo_.sections.end(), [this](auto &section) -> bool {
            if (section->get_type() != ELFIO::SHT_REL) {
                return true;
            }

            auto info = section->get_info();
            auto progSec = elfIo_.sections[info];
            if (progSec == nullptr) {
                return true;
            }

            if (progSec->get_type() != ELFIO::SHT_PROGBITS || ((progSec->get_flags() & ELFIO::SHF_EXECINSTR) == 0)) {
                return true;
            }

            auto insn = reinterpret_cast<bpf_insn *>(const_cast<char *>(progSec->get_data()));
            if (insn == nullptr) {
                return false;
            }
            if (!ApplyRelocation(insn, section.get())) {
                return false;
            }
            return true;
        });
    }

    bool UnloadProgs()
    {
        if (g_sockFd > 0) {
            close(g_sockFd);
            g_sockFd = -1;
        }
        return std::all_of(elfIo_.sections.begin(), elfIo_.sections.end(), [this](const auto &section) -> bool {
            if (!MatchSecName(section->get_name())) {
                return true;
            }

            std::string progName = section->get_name();
            std::replace(progName.begin(), progName.end(), '/', '_');
            std::string progPinLocation = std::string(PROGS_DIR) + "/" + progName;
            if (access(progPinLocation.c_str(), F_OK) == 0) {
                DoDetach(progPinLocation, progName);

                return UnPin(progPinLocation) < NETSYS_SUCCESS ? false : true;
            }
            return true;
        });
    }

    bool LoadProgs()
    {
        if (g_sockFd > 0) {
            close(g_sockFd);
        }
        g_sockFd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        return std::all_of(elfIo_.sections.begin(), elfIo_.sections.end(), [this](const auto &section) -> bool {
            if (!MatchSecName(section->get_name())) {
                return true;
            }
            return LoadProg(section->get_name(), reinterpret_cast<const bpf_insn *>(section->get_data()),
                            section->get_size() / sizeof(bpf_insn));
        });
    }

    std::string path_;
    ELFIO::elfio elfIo_;
    std::string license_;
    int32_t kernVersion_;
    std::vector<BpfMapData> maps_;

    std::function<ElfLoadError()> isPathValid_ = [this]() -> ElfLoadError {
        if (!IsPathValid()) {
            return ELF_LOAD_ERR_PATH_INVALID;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> makeDirectories = []() -> ElfLoadError {
        if (!MakeDirectories()) {
            return ELF_LOAD_ERR_MAKE_DIR_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> loadElfFile_ = [this]() -> ElfLoadError {
        if (!LoadElfFile()) {
            return ELF_LOAD_ERR_LOAD_FILE_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> isVersionValid_ = [this]() -> ElfLoadError {
        if (!IsVersionValid()) {
            return ELF_LOAD_ERR_GET_VERSION_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> setLicenseAndVersion_ = [this]() -> ElfLoadError {
        if (!SetLicenseAndVersion()) {
            return ELF_LOAD_ERR_SELECT_LICENSE_AND_VERSION_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> loadElfMapsSection_ = [this]() -> ElfLoadError {
        if (!LoadElfMapsSection()) {
            return ELF_LOAD_ERR_LOAD_MAP_SECTION_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> setRlimit_ = []() -> ElfLoadError {
        if (!SetRlimit()) {
            return ELF_LOAD_ERR_SET_RLIMIT_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> createMaps_ = [this]() -> ElfLoadError {
        if (!CreateMaps()) {
            return ELF_LOAD_ERR_CREATE_MAP_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> parseRelocation_ = [this]() -> ElfLoadError {
        if (!ParseRelocation()) {
            return ELF_LOAD_ERR_PARSE_RELOCATION_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> loadProgs_ = [this]() -> ElfLoadError {
        if (!LoadProgs()) {
            return ELF_LOAD_ERR_LOAD_PROGS_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> deleteMaps_ = [this]() -> ElfLoadError {
        if (!DeleteMaps()) {
            return ELF_LOAD_ERR_DELETE_MAP_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };

    std::function<ElfLoadError()> unloadProgs_ = [this]() -> ElfLoadError {
        if (!UnloadProgs()) {
            return ELF_LOAD_ERR_UNLOAD_PROGS_FAIL;
        }
        return ELF_LOAD_ERR_NONE;
    };
};

ElfLoadError LoadElf(const std::string &elfPath)
{
    ElfLoader loader(elfPath);
    return loader.Load();
}

ElfLoadError UnloadElf(const std::string &elfPath)
{
    ElfLoader loader(elfPath);
    return loader.Unload();
}
} // namespace OHOS::NetManagerStandard
