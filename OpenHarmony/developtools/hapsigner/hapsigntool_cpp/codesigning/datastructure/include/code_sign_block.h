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
#ifndef SIGNATRUETOOLS_CODE_SIGN_BLOCK_H
#define SIGNATRUETOOLS_CODE_SIGN_BLOCK_H

#include <map>
#include <string>
#include <vector>
#include <unordered_map>

#include "segment_header.h"
#include "hap_info_segment.h"
#include "code_sign_block_header.h"
#include "fs_verity_info_segment.h"
#include "native_lib_info_segment.h"

namespace OHOS {
namespace SignatureTools {

class CodeSignBlock {
public:
    static constexpr long PAGE_SIZE_4K = 4096;
    static constexpr int SEGMENT_HEADER_COUNT = 3;
    CodeSignBlock();
    virtual ~CodeSignBlock();
    void AddOneMerkleTree(const std::string& key, const std::vector<int8_t>& merkleTree);
    std::vector<int8_t> GetOneMerkleTreeByFileName(const std::string& key);
    void SetCodeSignBlockFlag();
    void SetSegmentNum();
    void AddToSegmentList(const SegmentHeader& sh);
    std::vector<SegmentHeader>& GetSegmentHeaderList();
    void SetSegmentHeaders();
    CodeSignBlockHeader& GetCodeSignBlockHeader();
    void SetCodeSignBlockHeader(const CodeSignBlockHeader& csbHeader);
    void SetFsVerityInfoSegment(const FsVerityInfoSegment& fsVeritySeg);
    HapInfoSegment& GetHapInfoSegment();
    void SetHapInfoSegment(const HapInfoSegment& hapSeg);
    NativeLibInfoSegment& GetSoInfoSegment();
    void SetSoInfoSegment(const NativeLibInfoSegment& soSeg);
    void ToByteArray(std::vector<int8_t>& ret);
    void ComputeSegmentOffset();
    int64_t ComputeMerkleTreeOffset(int64_t codeSignBlockOffset);
    void GenerateCodeSignBlockByte(int64_t fsvTreeOffset, std::vector<int8_t>& ret);

private:
    CodeSignBlockHeader codeSignBlockHeader;
    std::vector<SegmentHeader> segmentHeaderList;
    FsVerityInfoSegment fsVerityInfoSegment;
    HapInfoSegment hapInfoSegment;
    NativeLibInfoSegment nativeLibInfoSegment;
    std::vector<int8_t> zeroPadding;
    std::map<std::string, std::vector<int8_t>> merkleTreeMap;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_CODE_SIGN_BLOCK_H
