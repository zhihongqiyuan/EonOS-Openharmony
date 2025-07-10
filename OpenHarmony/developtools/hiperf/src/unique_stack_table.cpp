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
#include "unique_stack_table.h"

#include "hiperf_hilog.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {

bool UniqueStackTable::Init()
{
    // index 0 for reserved
    availableIndex_ = 1;
    totalNodes_ = ((tableSize_ / sizeof(Node)) >> 1) << 1; // make it even.
    if (totalNodes_ > MAX_NODES_CNT) {
        HLOGE("Hashtable size limit, initial value too large!");
        return false;
    }

    availableNodes_ = totalNodes_;
    hashModulus_ = availableNodes_ > 1 ? availableNodes_ - 1 : 1;
    hashStep_ = (totalNodes_ / (deconflictTimes_ * HASH_STEP_BASE_MULTIPLE + HASH_STEP_BASE_NUM));
    tableBuf_ = std::make_unique<uint8_t[]>(tableSize_);

    HLOGI("Init totalNodes_: %u, availableNodes_: %u, availableIndex_: %u  hashStep_: %" PRIu64 ", hashModulus_: %u",
        totalNodes_, availableNodes_, availableIndex_, hashStep_, hashModulus_);
    return true;
}

bool UniqueStackTable::Resize()
{
    CHECK_TRUE(tableBuf_ == nullptr, 0, 1, "Hashtable not exist, fatal error!");
    uint32_t oldNumNodes = totalNodes_;

    HLOGI("Before resize, totalNodes_: %u, availableNodes_: %u, availableIndex_: %u  hashStep_: %" PRIu64 "",
        totalNodes_, availableNodes_, availableIndex_, hashStep_);

    if ((totalNodes_ << RESIZE_MULTIPLE) > MAX_NODES_CNT) {
        HLOGW("Hashtable size limit, resize failed current cnt: %u, max cnt: %u", totalNodes_, MAX_NODES_CNT);
        return false;
    }

    uint32_t newtableSize = tableSize_ << RESIZE_MULTIPLE;
    std::unique_ptr<uint8_t[]> newTable = std::make_unique<uint8_t[]>(newtableSize);
    if (newTable.get() == nullptr) {
        HLOGE("%s: malloc(%u) failed, errno: %d", __func__, newtableSize, errno);
        return false;
    }

    if (memcpy_s(newTable.get(), tableSize_, tableBuf_.get(), tableSize_) != 0) {
        HLOGE("%s: memcpy_s(%u) failed, errno: %d", __func__, tableSize_, errno);
        return false;
    }

    tableBuf_ = std::move(newTable);
    tableSize_ = newtableSize;
    deconflictTimes_ += DECONFLICT_INCREASE_STEP;
    availableIndex_ += availableNodes_;
    totalNodes_ = ((newtableSize / sizeof(Node)) >> 1) << 1; // make it even.
    availableNodes_ = totalNodes_ - oldNumNodes;
    hashModulus_ = availableNodes_ > 1 ? availableNodes_ - 1 : 1;
    hashStep_ = availableNodes_ / (deconflictTimes_ * HASH_STEP_BASE_MULTIPLE + HASH_STEP_BASE_NUM);
    HLOGI("After resize, totalNodes_: %u, availableNodes_: %u, availableIndex_: %u hashStep_: %" PRIu64 "",
        totalNodes_, availableNodes_, availableIndex_, hashStep_);
    return true;
}

uint64_t UniqueStackTable::PutIpInSlot(uint64_t thisIp, uint64_t prevIdx)
{
    Node *tableHead = reinterpret_cast<Node *>(tableBuf_.get());
    uint64_t curIpIdx = (((thisIp >> 2) ^ (prevIdx << 4)) % hashModulus_) + availableIndex_;
    uint8_t currentDeconflictTimes = deconflictTimes_;

    Node node;
    node.section.ip = thisIp;
    node.section.prevIdx = prevIdx;
    node.section.inKernel = !!(thisIp & IP_IN_KERNEL);
    while (currentDeconflictTimes--) {
        Node* tableNode = reinterpret_cast<Node *>(tableHead) + curIpIdx;
        if (tableNode == nullptr) {
            continue;
        }
        // empty case
        if (tableNode->value == 0) {
            tableNode->value = node.value;
            usedSlots_.emplace_back(uint32_t(curIpIdx));
            return curIpIdx;
        }
        // already inserted
        if (tableNode->value == node.value) {
            return curIpIdx;
        }

        curIpIdx += currentDeconflictTimes * hashStep_ + 1;
        if (curIpIdx >= totalNodes_) {
            // make sure index 0 do not occupy
            curIpIdx -= (availableNodes_ >= 1 ? availableNodes_ - 1 : 0);
        }
    }

    HLOGI("Collison unresolved, need resize, usedSlots_.size(): %zu, curIpIdx: %" PRIu64 "",
        usedSlots_.size(), curIpIdx);
    return 0;
}

uint64_t UniqueStackTable::PutIpsInTable(StackId *stackId, u64 *ips, u64 nr)
{
    if (tableBuf_ == nullptr) {
        HLOGE("Hashtable not exist, fatal error!");
        return 0;
    }
    int64_t reverseIndex = static_cast<int64_t>(nr);
    uint64_t prev = 0;
    while (--reverseIndex >= 0) {
        uint64_t pc = ips[reverseIndex];

        if (pc == 0) {
            continue;
        }
        prev = PutIpInSlot(pc, prev);
        CHECK_TRUE(prev == 0, 0, 0, "");
    }
    CHECK_TRUE(stackId == nullptr, 0, 0, "");
    stackId->section.id = prev;
    stackId->section.nr = nr;
    return prev;
}

size_t UniqueStackTable::GetWriteSize()
{
    CHECK_TRUE(tableBuf_ == nullptr, 0, 1, "Hashtable not exist, fatal error!");
    size_t size = 0;
    size += sizeof(pid_);
    size += sizeof(tableSize_);
    uint32_t usedNodes = usedSlots_.size();
    size += sizeof(usedNodes);
    size += usedNodes * sizeof(uint32_t); // key index
    size += usedNodes * sizeof(uint64_t); // node value
    return size;
}

Node* UniqueStackTable::GetFrame(uint64_t stackId)
{
    Node *tableHead = reinterpret_cast<Node *>(tableBuf_.get());
    // should not occur
    CHECK_TRUE(stackId >= totalNodes_, nullptr, 1, "Failed to find frame by index: %" PRIu64 "", stackId);

    return reinterpret_cast<Node *>(&tableHead[stackId]);
}

bool UniqueStackTable::GetIpsByStackId(StackId stackId, std::vector<u64>& ips)
{
    CHECK_TRUE(tableBuf_ == nullptr, false, 1, "Hashtable not exist, failed to find frame!");
    uint64_t nr = stackId.section.nr;
    uint64_t tailIdx = stackId.section.id;

    Node *node = GetFrame(tailIdx);
    while (node != nullptr && nr > 0) {
        ips.push_back(
            node->section.inKernel ? (node->section.ip | KERNEL_PREFIX) : node->section.ip);
        if (node->section.prevIdx == HEAD_NODE_INDEX) {
            break;
        }
        node = GetFrame(node->section.prevIdx);
        nr--;
    }
    return true;
}

bool UniqueStackTable::ImportNode(uint32_t index, const Node& node)
{
    CHECK_TRUE(index >= tableSize_, false, 0, "");
    Node *tableHead = reinterpret_cast<Node *>(tableBuf_.get());
    tableHead[index].value = node.value;
    return true;
}

} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS