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
#ifndef SIGNATRUETOOLS_SEGMENT_HEADER_H
#define SIGNATRUETOOLS_SEGMENT_HEADER_H

#include <vector>
#include <string>

#include "byte_buffer.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {

class SegmentHeader {
public:
    static const int32_t SEGMENT_HEADER_LENGTH = 12;
    static const int32_t CSB_FSVERITY_INFO_SEG = 0x1;
    static const int32_t CSB_HAP_META_SEG = 0x2;
    static const int32_t CSB_NATIVE_LIB_INFO_SEG = 0x3;
    SegmentHeader();
    SegmentHeader(int32_t type, int32_t segmentSize);
    SegmentHeader(int32_t type, int32_t segmentOffset,
                  int32_t segmentSize);
    static std::unique_ptr<SegmentHeader> FromByteArray(std::vector<int8_t> bytes);
    int32_t GetType();
    void SetSegmentOffset(int32_t offset);
    int32_t GetSegmentOffset();
    int32_t GetSegmentSize();
    void ToByteArray(std::vector<int8_t> &ret);

private:
    int32_t type;
    int32_t segmentOffset;
    int32_t segmentSize;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_SEGMENT_HEADER_H