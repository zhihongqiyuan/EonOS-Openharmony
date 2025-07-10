/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EBPF_STD_TYPE_H
#define EBPF_STD_TYPE_H

namespace OHOS {
namespace EBPF_TOOLS {
struct HeaderDataItem {
    uint32_t headSize = 0;
    uint32_t version = 0;
    uint32_t clock = 0;
    uint32_t cmdLineLen = 0;
}__attribute__((packed));

struct EventMaps {
    uint32_t type = 0;
    uint32_t len = 0;
    uint64_t start = 0;
    uint64_t end = 0;
    uint32_t offset = 0;
    uint32_t pid = 0;
    uint32_t fileNameLen = 0;
    std::string fileName;
} __attribute__((packed));

struct SymbolInfo {
    uint32_t type = 0;
    uint32_t len = 0;
    uint64_t textVaddr = 0;
    uint32_t textOffset = 0;
    uint32_t strTabLen = 0;
    uint32_t symTabLen = 0;
    uint32_t fileNameSize = 0;
    uint32_t symEntlen = 0;
    char *strTab = nullptr;
    char *symTab = nullptr;
    std::string fileName;
}__attribute__((packed));

struct EventFs {
    static constexpr uint8_t MAX_TRACER_NAME_LEN = 8;
    static constexpr uint8_t MAX_TYPE_NAME_LEN = 16;
    static constexpr uint8_t MAX_COMM_LEN = 16;
    uint32_t tag = 0;
    uint32_t len = 0;
    uint32_t pid = 0;
    uint32_t tid = 0;
    char tracerName[MAX_TRACER_NAME_LEN];
    uint64_t start = 0;
    uint64_t end = 0;
    char typeName[MAX_TYPE_NAME_LEN];
    int32_t ret = 0;
    uint16_t nrUserIPs = 0;
    uint16_t type = 0;
    std::vector<uint64_t> args;
    char comm[MAX_COMM_LEN];
    std::vector<uint64_t> userIPs;
}__attribute__((packed));

struct EventMem {
    static constexpr uint8_t MAX_TRACER_NAME_LEN = 8;
    static constexpr uint8_t MAX_TYPE_NAME_LEN = 16;
    static constexpr uint8_t MAX_COMM_LEN = 16;
    uint32_t tag = 0;
    uint32_t len = 0;
    uint32_t pid = 0;
    uint32_t tid = 0;
    char tagName[MAX_TRACER_NAME_LEN];
    uint64_t start = 0;
    uint64_t end = 0;
    char typeName[MAX_TYPE_NAME_LEN];
    uint64_t addr = 0;
    uint32_t size = 0;
    uint16_t nips = 0;
    uint16_t type = 0;
    char comm[MAX_COMM_LEN];
    std::vector<uint64_t> userIPs;
}__attribute__((packed));

struct EventStr {
    static constexpr uint8_t MAX_STR_TRACE_NAME_LEN = 8;
    static constexpr uint8_t MAX_STR_TYPE_NAME_LEN = 16;
    uint32_t tag = 0;
    uint32_t len = 0;
    uint32_t pid = 0;
    uint32_t tid = 0;
    uint64_t start = 0;
    uint32_t srcTracer = 0;
    uint32_t srcType = 0;
    uint32_t strLen = 0;
    uint32_t padding = 0;
    std::string fileName;
}__attribute__((packed));

struct EventBIO {
    static constexpr uint8_t MAX_TYPE_NAME_LEN = 16;
    static constexpr uint8_t MAX_COMM_LEN = 16;
    uint32_t tag = 0;
    uint32_t len = 0;
    uint32_t pid = 0;
    uint32_t tid = 0;
    char comm[MAX_COMM_LEN];
    uint64_t start = 0;
    uint64_t end = 0;
    uint32_t prio = 0;
    uint32_t size = 0;
    uint64_t blkcnt = 0;
    uint32_t nips = 0;
    uint32_t type = 0;
    char typeName[MAX_TYPE_NAME_LEN];
    std::vector<uint64_t> userIPs;
}__attribute__((packed));

struct Record {
    Record() = default;
    ~Record() = default;
    Record(std::string name, uint64_t sym, uint64_t str)
    {
        fileName = name;
        symTabAddr = sym;
        strTabAddr = str;
    }
    std::string fileName;
    uint64_t symTabAddr = 0;
    uint64_t strTabAddr = 0;
};

struct Elf32_Sym {
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
}__attribute__((packed));

struct Elf64_Sym {
    uint32_t st_name;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
    uint64_t st_value;
    uint64_t st_size;
}__attribute__((packed));

enum TracerType:int {
    MAPSTRACE = 0,
    SYMBOLTRACE,
    FSTRACE,
    PFTRACE,
    BIOTRACE,
    STRTRACE,
    DLOPEN_TRACE,
    KERNEL_SYM = 0x10001,
    BADTRACE,
};
} // EBPF_TOOLS
} // OHOS

#endif // EBPF_STD_TYPE_H