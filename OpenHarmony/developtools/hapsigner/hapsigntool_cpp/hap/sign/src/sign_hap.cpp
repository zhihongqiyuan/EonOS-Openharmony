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
#include <numeric>

#include "signature_tools_log.h"
#include "signature_algorithm_helper.h"
#include "bc_pkcs7_generator.h"
#include "digest_common.h"
#include "sign_hap.h"

namespace OHOS {
namespace SignatureTools {
bool SignHap::Sign(DataSource* contents[], int32_t len, SignerConfig& config,
                   std::vector<OptionalBlock>& optionalBlocks, ByteBuffer& result)
{
    if (len != CONTENT_NUBER) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
                            "zip contents len must is 3, now is " + std::to_string(len));
        return false;
    }
    std::vector<SignatureAlgorithmHelper> algoClass = config.GetSignatureAlgorithms();
    if (algoClass.empty()) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Signature Algorithms is empty");
        return false;
    }
    SignatureAlgorithm algo = static_cast<SignatureAlgorithm>(algoClass[0].m_id);
    SIGNATURE_TOOLS_LOGI("[SignHap] Signature Algorithm  is %d", algo);
    int32_t nId = DigestCommon::GetDigestAlgorithmId(algo);
    DigestParameter digestParam = HapSignerBlockUtils::GetDigestParameter(nId);
    ByteBuffer digContext;
    std::vector<std::pair<int32_t, ByteBuffer>> nidAndcontentDigestsVec;
    // 1:Summarize corresponding content and optionalBlock
    if (!ComputeDigests(digestParam, contents, CONTENT_NUBER, optionalBlocks, digContext)) {
        SIGNATURE_TOOLS_LOGE("[SignHap] compute Digests failed");
        return false;
    }
    SIGNATURE_TOOLS_LOGI("[SignHap] ComputeDigests %d", digContext.GetCapacity());
    // 2:Encoding Summary Information
    ByteBuffer digMessage;
    std::pair<int32_t, ByteBuffer> nidAndcontentDigests = std::make_pair(algo, digContext);
    nidAndcontentDigestsVec.push_back(nidAndcontentDigests);
    EncodeListOfPairsToByteArray(digestParam, nidAndcontentDigestsVec, digMessage);

    SIGNATURE_TOOLS_LOGI("[SignHap] EncodeListOfPairsToByteArray %d", digMessage.GetCapacity());
    // 3:Encrypt the encoded summary information.
    std::shared_ptr<Pkcs7Generator> pkcs7Generator = std::make_shared<BCPkcs7Generator>();
    std::string digMessageData(digMessage.GetBufferPtr(), digMessage.GetCapacity());
    std::string ret;
    if (pkcs7Generator->GenerateSignedData(digMessageData, &config, ret) != 0) {
        SIGNATURE_TOOLS_LOGE("[SignHap] Generate Signed Data failed");
        return false;
    }
    SIGNATURE_TOOLS_LOGI("[SignHap] GenerateSignedData %lu", static_cast<unsigned long>(ret.size()));
    bool checkGenerateHapSigningBlockFlag = GenerateHapSigningBlock(ret, optionalBlocks,
                                                                    config.GetCompatibleVersion(), result);
    if (!checkGenerateHapSigningBlockFlag) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Generate Hap Signing Block failed");
        return false;
    }
    SIGNATURE_TOOLS_LOGI("[SignHap] GenerateHapSigningBlock %d", result.GetCapacity());
    return true;
}

bool SignHap::ComputeDigests(const DigestParameter& digestParam, DataSource* contents[], int32_t len,
                             const std::vector<OptionalBlock>& optionalBlocks, ByteBuffer& result)
{
    ByteBuffer chunkDigest;
    bool ret = HapSignerBlockUtils::ComputeDigestsForEachChunk(digestParam, contents, len, chunkDigest);
    if (!ret) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Compute Content Digests failed");
        return false;
    }
    bool checkComputeDigestsWithOptionalBlockFlag =
        HapSignerBlockUtils::ComputeDigestsWithOptionalBlock(digestParam, optionalBlocks, chunkDigest, result);
    if (!checkComputeDigestsWithOptionalBlockFlag) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Compute Final Digests failed");
        return false;
    }
    return true;
}

void SignHap::EncodeListOfPairsToByteArray(const DigestParameter& digestParam,
                                           const std::vector<std::pair<int32_t,
                                           ByteBuffer>>&nidAndcontentDigests, ByteBuffer& result)
{
    int encodeSize = INT_SIZE * 2  + INT_SIZE * 3 * nidAndcontentDigests.size();
    encodeSize += std::accumulate(nidAndcontentDigests.begin(), nidAndcontentDigests.end(), 0,
        [](int sum, const std::pair<int32_t, ByteBuffer>& pair) { return sum + pair.second.GetCapacity(); });
    result.SetCapacity(encodeSize);
    result.PutInt32(CONTENT_VERSION); // version
    result.PutInt32(BLOCK_NUMBER); // block number
    for (const auto& pair : nidAndcontentDigests) {
        auto second = pair.second;
        result.PutInt32(INT_SIZE + INT_SIZE + second.GetCapacity());
        result.PutInt32(pair.first);
        result.PutInt32(second.GetCapacity());
        result.Put(second);
    }
    return;
}

bool SignHap::GenerateHapSigningBlock(const std::string& hapSignatureSchemeBlock,
                                      std::vector<OptionalBlock>& optionalBlocks,
                                      int compatibleVersion, ByteBuffer& result)
{
    // FORMAT:
    // Proof-of-Rotation pairs(optional):
    // uint32:type
    // uint32:length
    // uint32:offset
    // Property pairs(optional):
    // uint32:type
    // uint32:length
    // uint32:offset
    // Profile capability pairs(optional):
    // uint32:type
    // uint32:length
    // uint32:offset
    // length bytes : app signing pairs
    // uint32:type
    // uint32:length
    // uint32:offset
    // repeated ID-value pairs(reserved extensions):
    // length bytes : Proof-of-Rotation values
    // length bytes : property values
    // length bytes : profile capability values
    // length bytes : signature schema values
    // uint64: size
    // uint128: magic
    // uint32: version
    long optionalBlockSize = std::accumulate(optionalBlocks.begin(), optionalBlocks.end(), 0L,
        [](int64_t sum, const auto& elem) { return sum + elem.optionalBlockValue.GetCapacity(); });
    long resultSize = ((OPTIONAL_TYPE_SIZE + OPTIONAL_LENGTH_SIZE + OPTIONAL_OFFSET_SIZE) *
                       (optionalBlocks.size() + 1)) + optionalBlockSize + hapSignatureSchemeBlock.size() +
        BLOCK_COUNT + HapUtils::BLOCK_SIZE + BLOCK_MAGIC + BLOCK_VERSION;
    if (resultSize > INT_MAX) {
        SIGNATURE_TOOLS_LOGE("Illegal Argument. HapSigningBlock out of range: %ld", resultSize);
        return false;
    }
    result.SetCapacity((int)resultSize);
    std::unordered_map<int, int> typeAndOffsetMap;
    int currentOffset = ((OPTIONAL_TYPE_SIZE + OPTIONAL_LENGTH_SIZE
                         + OPTIONAL_OFFSET_SIZE) * (optionalBlocks.size() + 1));
    int currentOffsetInBlockValue = 0;
    int blockValueSizes = (int)(optionalBlockSize + hapSignatureSchemeBlock.size());
    std::string blockValues(blockValueSizes, 0);
    for (const auto& elem : optionalBlocks) {
        if (memcpy_s(blockValues.data() + currentOffsetInBlockValue, blockValueSizes,
            elem.optionalBlockValue.GetBufferPtr(),
            elem.optionalBlockValue.GetCapacity()) != 0) {
            SIGNATURE_TOOLS_LOGE("GenerateHapSigningBlock memcpy_s failed\n");
            return false;
        }
        typeAndOffsetMap.insert({ elem.optionalType, currentOffset });
        currentOffset += elem.optionalBlockValue.GetCapacity();
        currentOffsetInBlockValue += elem.optionalBlockValue.GetCapacity();
    }
    if (memcpy_s(blockValues.data() + currentOffsetInBlockValue, blockValueSizes, hapSignatureSchemeBlock.data(),
        hapSignatureSchemeBlock.size()) != 0) {
        SIGNATURE_TOOLS_LOGE("GenerateHapSigningBlock memcpy_s failed\n");
        return false;
    }
    typeAndOffsetMap.insert({ HapUtils::HAP_SIGNATURE_SCHEME_V1_BLOCK_ID, currentOffset });
    ExtractedResult(optionalBlocks, result, typeAndOffsetMap);
    result.PutInt32(HapUtils::HAP_SIGNATURE_SCHEME_V1_BLOCK_ID); // type
    result.PutInt32(hapSignatureSchemeBlock.size()); // length
    int offset = typeAndOffsetMap.at(HapUtils::HAP_SIGNATURE_SCHEME_V1_BLOCK_ID);
    result.PutInt32(offset); // offset
    result.PutData(blockValues.c_str(), blockValueSizes);
    result.PutInt32(optionalBlocks.size() + 1); // Signing block count
    result.PutInt64(resultSize); // length of hap signing block
    std::vector<int8_t> signingBlockMagic = HapUtils::GetHapSigningBlockMagic(compatibleVersion);
    result.PutData(reinterpret_cast<const char*>(signingBlockMagic.data()), signingBlockMagic.size()); // magic
    result.PutInt32(HapUtils::GetHapSigningBlockVersion(compatibleVersion)); // version
    return true;
}

void SignHap::ExtractedResult(std::vector<OptionalBlock>& optionalBlocks, ByteBuffer& result,
                              std::unordered_map<int, int>& typeAndOffsetMap)
{
    int offset;
    for (const auto& elem : optionalBlocks) {
        result.PutInt32(elem.optionalType);  // type
        result.PutInt32(elem.optionalBlockValue.GetCapacity());  // length
        offset = typeAndOffsetMap.at(elem.optionalType);
        result.PutInt32(offset);  // offset
    }
}
} // namespace SignatureTools
} // namespace OHOS