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
#ifndef HIEPRF_UNIQUE_STACK_TABLE_H
#define HIEPRF_UNIQUE_STACK_TABLE_H
#include "utilities.h"
#include <cinttypes>
#include <linux/perf_event.h>
#include <string>
namespace OHOS {
namespace Developtools {
namespace HiPerf {

#define ADDR_BIT_LENGTH        40
#define IDX_BIT_LENGTH         23
#define KERNEL_FLAG_BIT_LENGTH 1
#define DECONFLICT_INCREASE_STEP  3
#define RESIZE_MULTIPLE          2
#define NR_BIT_LENGTH          41

constexpr uint32_t INITIAL_TABLE_SIZE = 4 * 1024 * 1024;
constexpr uint32_t MAX_NODES_CNT = 1 << IDX_BIT_LENGTH ;
constexpr uint64_t IP_IN_KERNEL = 1uLL << 63;
constexpr uint64_t HEAD_NODE_INDEX = 0;
// FFFFFF0000000000
constexpr uint64_t KERNEL_PREFIX = 0xFFFFFFull << 40;
constexpr uint8_t INIT_DECONFLICT_ALLOWED = 22;
constexpr uint8_t HASH_STEP_BASE_MULTIPLE = 2;
constexpr uint8_t HASH_STEP_BASE_NUM = 1;

// align
#pragma pack(push, 4)

union Node {
    uint64_t value;
    struct {
        uint64_t ip : ADDR_BIT_LENGTH;
        uint64_t prevIdx : IDX_BIT_LENGTH;
        uint64_t inKernel : KERNEL_FLAG_BIT_LENGTH;
    } section;
};

struct UniStackNode {
    uint32_t index;
    Node node;
};

struct UniStackTableInfo {
    uint32_t pid;
    uint32_t tableSize;
    uint32_t numNodes;
    std::vector<UniStackNode> nodes;
};

union StackId {
    uint64_t value;
    struct {
        uint64_t id : IDX_BIT_LENGTH;
        uint64_t nr : NR_BIT_LENGTH;
    } section;
};

#pragma pack(pop)
static_assert(sizeof(Node) == 8, "Node size must be 8 byte");

class UniqueStackTable {
public:
    bool Init();
    explicit UniqueStackTable(pid_t pid) : pid_(pid)
    {
        Init();
    }

    UniqueStackTable(pid_t pid, uint32_t size) : pid_(pid), tableSize_(size)
    {
        Init();
    }
    ~UniqueStackTable()
    {
        tableBuf_ = nullptr;
    }

    uint64_t PutIpsInTable(StackId *stackId, u64 *ips, u64 nr);
    bool GetIpsByStackId(const StackId stackId, std::vector<u64>& ips);
    bool ImportNode(uint32_t index, const Node& node);
    size_t GetWriteSize();

    bool Resize();

    uint32_t GetPid()
    {
        return pid_;
    }

    uint32_t GetTabelSize()
    {
        return tableSize_;
    }

    std::vector<uint32_t>& GetUsedIndexes()
    {
        return usedSlots_;
    }

    Node* GetHeadNode()
    {
        return reinterpret_cast<Node *>(tableBuf_.get());
    }

private:
    Node* GetFrame(uint64_t stackId);
    uint64_t PutIpInSlot(uint64_t thisIp, uint64_t prevIdx);
    u32 pid_;
    uint32_t tableSize_ = INITIAL_TABLE_SIZE;
    std::unique_ptr<uint8_t[]> tableBuf_ = nullptr;
    std::vector<uint32_t> usedSlots_;
    uint32_t totalNodes_ = 0;
    // current available node count, include index 0
    uint32_t availableNodes_ = 0;
    uint32_t hashModulus_ = 1;
    // 0 for reserved, start from 1
    uint32_t availableIndex_ = 1;
    // for de-conflict
    uint64_t hashStep_ = 0;
    uint8_t deconflictTimes_ = INIT_DECONFLICT_ALLOWED;
};

using ProcessStackMap = std::map<pid_t, std::shared_ptr<UniqueStackTable>>;
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIEPRF_UNIQUE_STACK_TABLE_H