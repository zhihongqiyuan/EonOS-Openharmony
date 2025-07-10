/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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

#include "segment_header.h"

namespace OHOS {
namespace SignatureTools {

SegmentHeader::SegmentHeader()
{
}

SegmentHeader::SegmentHeader(int32_t type, int32_t segmentSize) :type(type), segmentOffset(0), segmentSize(segmentSize)
{
}

SegmentHeader::SegmentHeader(int32_t type, int32_t segmentOffset,
                             int32_t segmentSize)
{
    this->type = type;
    this->segmentOffset = segmentOffset;
    this->segmentSize = segmentSize;
}

int32_t SegmentHeader::GetType()
{
    return type;
}

void SegmentHeader::SetSegmentOffset(int32_t offset)
{
    segmentOffset = offset;
}

int32_t SegmentHeader::GetSegmentOffset()
{
    return segmentOffset;
}

int32_t SegmentHeader::GetSegmentSize()
{
    return segmentSize;
}

void SegmentHeader::ToByteArray(std::vector<int8_t> &ret)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(SEGMENT_HEADER_LENGTH));
    bf->PutInt32(type);
    bf->PutInt32(segmentOffset);
    bf->PutInt32(segmentSize);
    bf->Flip();
    ret = std::vector<int8_t>(bf->GetBufferPtr(), bf->GetBufferPtr() + bf.get()->GetLimit());
    return;
}

std::unique_ptr<SegmentHeader> SegmentHeader::FromByteArray(std::vector<int8_t> bytes)
{
    if (bytes.size() != SEGMENT_HEADER_LENGTH) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "The flag of SegmentHeader is incorrect.");
        return std::unique_ptr<SegmentHeader>();
    }
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(SEGMENT_HEADER_LENGTH));
    bf->PutData(bytes.data(), bytes.size());
    bf->Flip();
    int32_t inType = 0;
    bf->GetInt32(inType);
    if ((inType != CSB_FSVERITY_INFO_SEG) && (inType != CSB_HAP_META_SEG)
        && (inType != CSB_NATIVE_LIB_INFO_SEG)) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "The type of SegmentHeader is incorrect.");
        return std::unique_ptr<SegmentHeader>();
    }
    int32_t inSegmentOffset = 0;
    bf->GetInt32(inSegmentOffset);
    // segment offset is always larger than the size of CodeSignBlockHeader
    int32_t inSegmentSize = 0;
    bf->GetInt32(inSegmentSize);
    if (inSegmentSize < 0) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The segment size of SegmentHeader is incorrect.");
        return std::unique_ptr<SegmentHeader>();
    }
    return std::make_unique<SegmentHeader>(inType, inSegmentOffset, inSegmentSize);
}

}
}