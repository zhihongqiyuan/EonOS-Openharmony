/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
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

#ifndef HIEBPF_COMMON_H
#define HIEBPF_COMMON_H

#include "hiebpf_macros.h"

constexpr int ARGS_ARRAY_SIZE = 4;
constexpr int  COMMAND_LINE_MAX = 127;
const int MAX_BUFFER_SIZE = 256 * 1024 * 1024;

enum ITEM_TYPE {
    itemEventMaps,
    itemSymbolInfo,
    itemEventFs,
    itemEventMem,
};

enum CLOCK_TYPE {
    RealTime,
    BootTime,
    Monotonic,
    MonotonicRaw,
};

// event type
enum EVENT_TYPE {
    EBPF_OPEN,
    EBPF_CLOSE,
    EBPF_READ,
    EBPF_WRITE,
};

struct EBPFHead {
    int type;
    int size;
};

struct ArgStr {
    uint16_t argStrLen;
    char* argStr;
};

struct EbpfHeader {
    static constexpr uint32_t HEADER_SIZE = 1024;
    static constexpr uint32_t STR_SIZE = 127;
    struct HeaderData {
        char magic[8] = {'E', 'B', 'P', 'F', 'F', 'I', 'L', 'E'};
        uint32_t headSize = 1024;
        uint32_t version = 0;
        uint32_t clock = CLOCK_TYPE::BootTime;
        uint32_t cmdLineLen = 0;
        char cmdline[COMMAND_LINE_MAX] = { 0 };
    };
    HeaderData data_ = {};
    char padding[HEADER_SIZE - sizeof(data_)] = {};
};

struct SysOpenArgs {
    char fileName[];
};

struct SysCloseArgs {
    uint64_t fd;
};

struct SysReadWriteArgs {
    uint64_t fd;
    uint64_t size;
};

struct ItemEventFs {
    uint32_t tag;
    uint32_t len;
    uint32_t pid;
    uint32_t tid;
    uint64_t start;
    uint64_t end;
    int ret;
    uint16_t nrUserIPs;
    uint16_t type;
    uint64_t args[ARGS_ARRAY_SIZE] = {0};
    uint64_t* userIPs;
    uint16_t commLen;
    char* comm;
    uint16_t argStrTotalLen;
};

struct ItemEventMem {
    uint32_t tag;
    uint32_t len;
    uint32_t pid;
    uint32_t tid;
    char tagName[MAX_TRACER_NAME_LEN];
    uint64_t start;
    uint64_t end;
    char typeName[MAX_TYPE_NAME_LEN];
    uint64_t addr;
    uint32_t size;
    uint16_t nIPs;
    uint16_t type;
    char comm[MAX_COMM_LEN];
    uint64_t* userIPs;
};
#endif