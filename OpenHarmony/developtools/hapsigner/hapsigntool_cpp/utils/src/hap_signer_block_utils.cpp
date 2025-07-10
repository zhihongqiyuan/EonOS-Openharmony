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

#include "hap_signer_block_utils.h"
#include <cinttypes>
#include <climits>
#include <vector>

#include "signature_info.h"
#include "algorithm"
#include "openssl/evp.h"
#include "securec.h"
#include "byte_buffer_data_source.h"
#include "file_data_source.h"
#include "digest_common.h"
#include "signature_tools_log.h"
#include "signature_tools_errno.h"

namespace OHOS {
namespace SignatureTools {
const int64_t HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_LOW_OLD = 2334950737560224072LL;
const int64_t HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_HIGH_OLD = 3617552046287187010LL;
const int64_t HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_LOW = 7451613641622775868LL;
const int64_t HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_HIGH = 4497797983070462062LL;

/* 1MB = 1024 * 1024 Bytes */
const int64_t HapSignerBlockUtils::CHUNK_SIZE = 1048576LL;

const int32_t HapSignerBlockUtils::HAP_SIG_BLOCK_MIN_SIZE = 32;
const int32_t HapSignerBlockUtils::ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH = 32;

const int32_t HapSignerBlockUtils::ZIP_EOCD_SEG_MIN_SIZE = 22;
const int32_t HapSignerBlockUtils::ZIP_EOCD_SEGMENT_FLAG = 0x06054b50;
const int32_t HapSignerBlockUtils::ZIP_EOCD_COMMENT_LENGTH_OFFSET = 20;
const int32_t HapSignerBlockUtils::ZIP_CD_OFFSET_IN_EOCD = 16;
const int32_t HapSignerBlockUtils::ZIP_CD_SIZE_OFFSET_IN_EOCD = 12;
const int32_t HapSignerBlockUtils::ZIP_BLOCKS_NUM_NEED_DIGEST = 3;

const char HapSignerBlockUtils::ZIP_FIRST_LEVEL_CHUNK_PREFIX = 0x5a;
const char HapSignerBlockUtils::ZIP_SECOND_LEVEL_CHUNK_PREFIX = 0xa5;

/*
 * The package of hap is ZIP format, and contains four segments: contents of Zip entry,
 * hap signatures block, central directory and end of central directory.
 * The function will find the data segment of hap signature block from hap file.
 */
bool HapSignerBlockUtils::FindHapSignature(RandomAccessFile& hapFile, SignatureInfo& signInfo)
{
    std::pair<ByteBuffer, int64_t> eocdAndOffsetInFile;
    if (!FindEocdInHap(hapFile, eocdAndOffsetInFile)) {
        SIGNATURE_TOOLS_LOGE("find EoCD failed");
        return false;
    }

    signInfo.hapEocd = eocdAndOffsetInFile.first;
    signInfo.hapEocdOffset = eocdAndOffsetInFile.second;
    if (!GetCentralDirectoryOffset(signInfo.hapEocd, signInfo.hapEocdOffset, signInfo.hapCentralDirOffset)) {
        SIGNATURE_TOOLS_LOGE("get CD offset failed");
        PrintErrorNumberMsg("verify", VERIFY_ERROR, "ZIP End of Central Directory not found");
        return false;
    }

    if (!FindHapSigningBlock(hapFile, signInfo.hapCentralDirOffset, signInfo)) {
        SIGNATURE_TOOLS_LOGE("find signing block failed");
        return false;
    }
    return true;
}

bool HapSignerBlockUtils::FindEocdInHap(RandomAccessFile& hapFile, std::pair<ByteBuffer, int64_t>& eocd)
{
    /*
     * EoCD has an optional comment block. Most hap packages do not contain this block.
     * For hap packages without comment block, EoCD is the last 22 bytes of hap file.
     * Try as a hap without comment block first to avoid unnecessarily reading more data.
     */
    if (FindEocdInHap(hapFile, 0, eocd)) {
        SIGNATURE_TOOLS_LOGD("Find EoCD of Zip file");
        return true;
    }
    /*
     * If EoCD contain the comment block, we should find it from the offset of (fileLen - maxCommentSize - 22).
     * The max size of comment block is 65535, because the comment length is an unsigned 16-bit number.
     */
    return FindEocdInHap(hapFile, USHRT_MAX, eocd);
}

bool HapSignerBlockUtils::FindEocdInHap(RandomAccessFile& hapFile, unsigned short maxCommentSize,
                                        std::pair<ByteBuffer, int64_t>& eocd)
{
    int64_t fileLength = hapFile.GetLength();
    /* check whether has enough space for EoCD in the file. */
    if (fileLength < ZIP_EOCD_SEG_MIN_SIZE) {
        SIGNATURE_TOOLS_LOGE("file length %" PRId64 " is too smaller", fileLength);
        return false;
    }

    int32_t searchRange = static_cast<int>(maxCommentSize) + ZIP_EOCD_SEG_MIN_SIZE;
    if (fileLength < static_cast<int64_t>(searchRange)) {
        searchRange = static_cast<int>(fileLength);
    }

    ByteBuffer searchEocdBuffer(searchRange);
    int64_t searchRangeOffset = fileLength - searchEocdBuffer.GetCapacity();
    int64_t ret = hapFile.ReadFileFullyFromOffset(searchEocdBuffer, searchRangeOffset);
    if (ret < 0) {
        SIGNATURE_TOOLS_LOGE("read data from hap file error: %" PRId64, ret);
        return false;
    }

    int32_t eocdOffsetInSearchBuffer = 0;
    if (!FindEocdInSearchBuffer(searchEocdBuffer, eocdOffsetInSearchBuffer)) {
        SIGNATURE_TOOLS_LOGE("eocd is not found");
        return false;
    }

    searchEocdBuffer.SetPosition(eocdOffsetInSearchBuffer);
    searchEocdBuffer.Slice();
    eocd.first = searchEocdBuffer;
    eocd.second = searchRangeOffset + eocdOffsetInSearchBuffer;
    return true;
}


/*
* 4-bytes: End of central directory flag
* 2-bytes: Number of this disk
* 2-bytes: Number of the disk with the start of central directory
* 2-bytes: Total number of entries in the central directory on this disk
* 2-bytes: Total number of entries in the central directory
* 4-bytes: Size of central directory
* 4-bytes: offset of central directory in zip file
* 2-bytes: ZIP file comment length, the value n is in the range of [0, 65535]
* n-bytes: ZIP Comment block data
*
* This function find Eocd by searching Eocd flag from input buffer(searchBuffer) and
* making sure the comment length is equal to the expected value.
*/
bool HapSignerBlockUtils::FindEocdInSearchBuffer(ByteBuffer& searchBuffer, int& offset)
{
    int32_t searchBufferSize = searchBuffer.GetCapacity();
    if (searchBufferSize < ZIP_EOCD_SEG_MIN_SIZE) {
        SIGNATURE_TOOLS_LOGE("The size of searchBuffer %d is smaller than min size of Eocd",
                             searchBufferSize);
        return false;
    }

    int32_t calcCurrentOffset = searchBufferSize - ZIP_EOCD_SEG_MIN_SIZE;
    while (calcCurrentOffset >= 0) {
        int32_t hapEocdSegmentFlag;
        if (searchBuffer.GetInt32(calcCurrentOffset, hapEocdSegmentFlag) &&
            (hapEocdSegmentFlag == ZIP_EOCD_SEGMENT_FLAG)) {
            unsigned short commentLength;
            int32_t expectedCommentLength = searchBufferSize - ZIP_EOCD_SEG_MIN_SIZE - calcCurrentOffset;
            if (searchBuffer.GetUInt16(calcCurrentOffset + ZIP_EOCD_COMMENT_LENGTH_OFFSET, commentLength) &&
                static_cast<int>(commentLength) == expectedCommentLength) {
                offset = calcCurrentOffset;
                return true;
            }
        }
        calcCurrentOffset--;
    }
    return false;
}

bool HapSignerBlockUtils::GetCentralDirectoryOffset(ByteBuffer& eocd, int64_t eocdOffset,
                                                    int64_t& centralDirectoryOffset)
{
    uint32_t offsetValue;
    uint32_t sizeValue;
    if (!eocd.GetUInt32(ZIP_CD_OFFSET_IN_EOCD, offsetValue) ||
        !eocd.GetUInt32(ZIP_CD_SIZE_OFFSET_IN_EOCD, sizeValue)) {
        SIGNATURE_TOOLS_LOGE("GetUInt32 failed");
        return false;
    }

    centralDirectoryOffset = static_cast<int64_t>(offsetValue);
    if (centralDirectoryOffset > eocdOffset) {
        SIGNATURE_TOOLS_LOGE("centralDirOffset %" PRId64 " is larger than eocdOffset %" PRId64,
                             centralDirectoryOffset, eocdOffset);
        return false;
    }

    int64_t centralDirectorySize = static_cast<int64_t>(sizeValue);
    if (centralDirectoryOffset + centralDirectorySize != eocdOffset) {
        SIGNATURE_TOOLS_LOGE("centralDirOffset %" PRId64 " add centralDirSize %" PRId64 " is"
                             " not equal to eocdOffset %" PRId64, centralDirectoryOffset,
                             centralDirectorySize, eocdOffset);
        return false;
    }
    return true;
}

bool HapSignerBlockUtils::GetCentralDirectorySize(ByteBuffer& eocd, long& centralDirectorySize)
{
    uint32_t cdSize;
    if (!eocd.GetUInt32(ZIP_CD_SIZE_OFFSET_IN_EOCD, cdSize)) {
        SIGNATURE_TOOLS_LOGE("GetUInt32 failed");
        return false;
    }
    centralDirectorySize = (long)cdSize;
    return true;
}

bool HapSignerBlockUtils::SetUnsignedInt32(ByteBuffer& buffer, int32_t offset, int64_t value)
{
    if ((value < 0) || (value > static_cast<int64_t>(UINT_MAX))) {
        SIGNATURE_TOOLS_LOGE("uint32 value of out range: %" PRId64, value);
        return false;
    }
    buffer.PutInt32(offset, static_cast<int>(value));
    return true;
}

bool HapSignerBlockUtils::FindHapSigningBlock(RandomAccessFile& hapFile, int64_t centralDirOffset,
                                              SignatureInfo& signInfo)
{
    if (centralDirOffset < HAP_SIG_BLOCK_MIN_SIZE) {
        SIGNATURE_TOOLS_LOGE("HAP too small for HAP Signing Block: %" PRId64, centralDirOffset);
        return false;
    }
    /*
     * read hap signing block head, it's format:
     * int32: blockCount
     * int64: size
     * 16 bytes: magic
     * int32: version
     */
    ByteBuffer hapBlockHead(ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH);
    int64_t ret = hapFile.ReadFileFullyFromOffset(hapBlockHead,
                                                  centralDirOffset - hapBlockHead.GetCapacity());
    if (ret < 0) {
        SIGNATURE_TOOLS_LOGE("read hapBlockHead error: %" PRId64, ret);
        return false;
    }
    HapSignBlockHead hapSignBlockHead;
    if (!ParseSignBlockHead(hapSignBlockHead, hapBlockHead)) {
        SIGNATURE_TOOLS_LOGE("ParseSignBlockHead failed");
        return false;
    }

    if (!CheckSignBlockHead(hapSignBlockHead)) {
        SIGNATURE_TOOLS_LOGE("hapSignBlockHead is invalid");
        return false;
    }

    signInfo.version = hapSignBlockHead.version;
    int64_t blockArrayLen = hapSignBlockHead.hapSignBlockSize - ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH;
    int64_t hapSignBlockOffset = centralDirOffset - hapSignBlockHead.hapSignBlockSize;
    if (hapSignBlockOffset < 0) {
        SIGNATURE_TOOLS_LOGE("HAP Signing Block offset out of range %" PRId64, hapSignBlockOffset);
        return false;
    }
    signInfo.hapSigningBlockOffset = hapSignBlockOffset;
    return FindHapSubSigningBlock(hapFile, hapSignBlockHead.blockCount,
                                  blockArrayLen, hapSignBlockOffset, signInfo);
}

bool HapSignerBlockUtils::CheckSignBlockHead(const HapSignBlockHead& hapSignBlockHead)
{
    int64_t magicLow = HAP_SIG_BLOCK_MAGIC_LOW;
    int64_t magicHigh = HAP_SIG_BLOCK_MAGIC_HIGH;
    if (hapSignBlockHead.version < VERSION_FOR_NEW_MAGIC_NUM) {
        magicLow = HAP_SIG_BLOCK_MAGIC_LOW_OLD;
        magicHigh = HAP_SIG_BLOCK_MAGIC_HIGH_OLD;
    }

    if ((hapSignBlockHead.hapSignBlockMagicLo != magicLow) ||
        (hapSignBlockHead.hapSignBlockMagicHi != magicHigh)) {
        SIGNATURE_TOOLS_LOGE("No HAP Signing Block before ZIP Central Directory");
        return false;
    }

    if ((hapSignBlockHead.hapSignBlockSize < ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH) ||
        (hapSignBlockHead.hapSignBlockSize > MAX_HAP_SIGN_BLOCK_SIZE)) {
        SIGNATURE_TOOLS_LOGE("HAP Signing Block size out of range %" PRId64,
                             hapSignBlockHead.hapSignBlockSize);
        return false;
    }

    if (hapSignBlockHead.blockCount > MAX_BLOCK_COUNT) {
        SIGNATURE_TOOLS_LOGE("HAP Signing Block count out of range %d", hapSignBlockHead.blockCount);
        return false;
    }

    return true;
}

bool HapSignerBlockUtils::ParseSignBlockHead(HapSignBlockHead& hapSignBlockHead, ByteBuffer& hapBlockHead)
{
    return hapBlockHead.GetInt32(hapSignBlockHead.blockCount) &&
        hapBlockHead.GetInt64(hapSignBlockHead.hapSignBlockSize) &&
        hapBlockHead.GetInt64(hapSignBlockHead.hapSignBlockMagicLo) &&
        hapBlockHead.GetInt64(hapSignBlockHead.hapSignBlockMagicHi) &&
        hapBlockHead.GetInt32(hapSignBlockHead.version);
}

bool HapSignerBlockUtils::ParseSubSignBlockHead(HapSubSignBlockHead& subSignBlockHead,
                                                ByteBuffer& hapBlockHead)
{
    return hapBlockHead.GetUInt32(subSignBlockHead.type) &&
        hapBlockHead.GetUInt32(subSignBlockHead.length) &&
        hapBlockHead.GetUInt32(subSignBlockHead.offset);
}

/*
 * Hap Sign Block Format:
 * HapSubSignBlock1_Head
 * HapSubSignBlock2_Head
 * ...
 * HapSubSignBlockn_Head
 * HapSubSignBlock1_data
 * HapSubSignBlock2_data
 * ...
 * HapSubSignBlockn_data
 * hap signing block head
 *
 * This function reads the head of the HapSubSignBlocks,
 * and then reads the corresponding data of each block according to the offset provided by the head
 */
bool HapSignerBlockUtils::FindHapSubSigningBlock(RandomAccessFile& hapFile, int32_t blockCount, int64_t blockArrayLen,
                                                 int64_t hapSignBlockOffset, SignatureInfo& signInfo)
{
    int64_t offsetMax = hapSignBlockOffset + blockArrayLen;
    int64_t readLen = 0;
    int64_t readHeadOffset = hapSignBlockOffset;
    for (int32_t i = 0; i < blockCount; i++) {
        ByteBuffer hapBlockHead(ZIP_CD_SIZE_OFFSET_IN_EOCD);
        int64_t ret = hapFile.ReadFileFullyFromOffset(hapBlockHead, readHeadOffset);
        if (ret < 0) {
            SIGNATURE_TOOLS_LOGE("read %dst head error: %" PRId64, i, ret);
            return false;
        }
        HapSubSignBlockHead subSignBlockHead;
        if (!ParseSubSignBlockHead(subSignBlockHead, hapBlockHead)) {
            SIGNATURE_TOOLS_LOGE("ParseSubSignBlockHead failed");
            return false;
        }
        readLen += sizeof(HapSubSignBlockHead);
        readHeadOffset += sizeof(HapSubSignBlockHead);
        if (readHeadOffset > offsetMax) {
            SIGNATURE_TOOLS_LOGE("find %dst next head offset error", i);
            return false;
        }
        int64_t headOffset = static_cast<int64_t>(subSignBlockHead.offset);
        int64_t headLength = static_cast<int64_t>(subSignBlockHead.length);
        /* check subSignBlockHead */
        if ((offsetMax - headOffset) < hapSignBlockOffset || (blockArrayLen - headLength) < readLen) {
            SIGNATURE_TOOLS_LOGE("failed to find data offset or enough data for %dst subblock error", i);
            return false;
        }
        int64_t dataOffset = hapSignBlockOffset + headOffset;
        ByteBuffer signBuffer(subSignBlockHead.length);
        if ((ret = hapFile.ReadFileFullyFromOffset(signBuffer, dataOffset)) < 0) {
            SIGNATURE_TOOLS_LOGE("read %dst subblock error: %" PRId64, i, ret);
            return false;
        }
        readLen += headLength;
        if (!ClassifyHapSubSigningBlock(signInfo, signBuffer, subSignBlockHead.type)) {
            SIGNATURE_TOOLS_LOGE("subSigningBlock error, type is %d", subSignBlockHead.type);
            return false;
        }
    }
    /* size of block must be equal to the sum of all subblocks length */
    if (readLen != blockArrayLen) {
        SIGNATURE_TOOLS_LOGE("Len: %" PRId64 " is not equal blockArrayLen: %" PRId64, readLen, blockArrayLen);
        return false;
    }
    return true;
}

bool HapSignerBlockUtils::ClassifyHapSubSigningBlock(SignatureInfo& signInfo,
                                                     const ByteBuffer& subBlock, uint32_t type)
{
    bool ret = false;
    switch (type) {
        case HAP_SIGN_BLOB:
            {
                if (signInfo.hapSignatureBlock.GetCapacity() != 0) {
                    SIGNATURE_TOOLS_LOGE("find more than one hap sign block");
                    break;
                }
                signInfo.hapSignatureBlock = subBlock;
                ret = true;
                break;
            }
        case PROFILE_BLOB:
        case PROOF_ROTATION_BLOB:
        case PROPERTY_BLOB:
            {
                OptionalBlock optionalBlockObject;
                optionalBlockObject.optionalType = static_cast<int>(type);
                optionalBlockObject.optionalBlockValue = subBlock;
                signInfo.optionBlocks.push_back(optionalBlockObject);
                ret = true;
                break;
            }
        default:
            break;
    }
    return ret;
}

bool HapSignerBlockUtils::GetOptionalBlockIndex(std::vector<OptionalBlock>& optionBlocks,
                                                int32_t type,
                                                int& index)
{
    int32_t len = static_cast<int>(optionBlocks.size());
    for (int32_t i = 0; i < len; i++) {
        if (optionBlocks[i].optionalType == type) {
            index = i;
            return true;
        }
    }
    SIGNATURE_TOOLS_LOGE("get optional block type:%d failed.", type);
    return false;
}

bool HapSignerBlockUtils::VerifyHapIntegrity(
    Pkcs7Context& digestInfo, RandomAccessFile& hapFile, SignatureInfo& signInfo)
{
    if (!SetUnsignedInt32(signInfo.hapEocd, ZIP_CD_OFFSET_IN_EOCD, signInfo.hapSigningBlockOffset)) {
        SIGNATURE_TOOLS_LOGE("Set central dir offset failed");
        return false;
    }

    int64_t centralDirSize = signInfo.hapEocdOffset - signInfo.hapCentralDirOffset;
    FileDataSource contentsZip(hapFile, 0, signInfo.hapSigningBlockOffset, 0);
    FileDataSource centralDir(hapFile, signInfo.hapCentralDirOffset, centralDirSize, 0);
    ByteBufferDataSource eocd(signInfo.hapEocd);
    DataSource* content[ZIP_BLOCKS_NUM_NEED_DIGEST] = {&contentsZip, &centralDir, &eocd};
    int32_t nId = DigestCommon::GetDigestAlgorithmId(digestInfo.digestAlgorithm);
    DigestParameter digestParam = GetDigestParameter(nId);
    ByteBuffer chunkDigest;
    if (!ComputeDigestsForEachChunk(digestParam, content, ZIP_BLOCKS_NUM_NEED_DIGEST, chunkDigest)) {
        SIGNATURE_TOOLS_LOGE("Compute Content Digests failed, alg: %d", nId);
        return false;
    }

    ByteBuffer actualDigest;
    if (!ComputeDigestsWithOptionalBlock(digestParam, signInfo.optionBlocks, chunkDigest, actualDigest)) {
        SIGNATURE_TOOLS_LOGE("Compute Final Digests failed, alg: %d", nId);
        return false;
    }

    if (!digestInfo.content.IsEqual(actualDigest)) {
        SIGNATURE_TOOLS_LOGE("digest of contents verify failed, alg %d", nId);
        return false;
    }
    PrintMsg(std::string("Digest verify result: ") + "success" + ", DigestAlgorithm: "
             + DigestCommon::GetDigestAlgorithmString(digestInfo.digestAlgorithm));

    return true;
}

bool HapSignerBlockUtils::ComputeDigestsWithOptionalBlock(const DigestParameter& digestParam,
                                                          const std::vector<OptionalBlock>& optionalBlocks,
                                                          const ByteBuffer& chunkDigest,
                                                          ByteBuffer& finalDigest)
{
    unsigned char out[EVP_MAX_MD_SIZE];
    int32_t digestLen = DigestCommon::GetDigest(chunkDigest, optionalBlocks, digestParam, out);
    if (digestLen != digestParam.digestOutputSizeBytes) {
        SIGNATURE_TOOLS_LOGE("GetDigest failed, outLen is not right, %u, %d",
                             digestLen, digestParam.digestOutputSizeBytes);
        return false;
    }

    finalDigest.SetCapacity(digestParam.digestOutputSizeBytes);
    finalDigest.PutData(0, reinterpret_cast<char*>(out), digestParam.digestOutputSizeBytes);
    return true;
}

bool HapSignerBlockUtils::GetSumOfChunkDigestLen(DataSource* contents[], int32_t len,
                                                 int32_t chunkDigestLen, int& chunkCount,
                                                 int& sumOfChunkDigestLen)
{
    for (int32_t i = 0; i < len; i++) {
        if (contents[i] == nullptr) {
            SIGNATURE_TOOLS_LOGE("contents[%d] is nullptr", i);
            return false;
        }
        contents[i]->Reset();
        chunkCount += GetChunkCount(contents[i]->Remaining(), CHUNK_SIZE);
    }

    if (chunkCount <= 0) {
        SIGNATURE_TOOLS_LOGE("no content for digest");
        return false;
    }
    if (chunkCount == 0) {
        SIGNATURE_TOOLS_LOGE("no content for digest");
        return false;
    }
    if (chunkDigestLen < 0 || ((INT_MAX - ZIP_CHUNK_DIGEST_PRIFIX_LEN) / chunkCount) < chunkDigestLen) {
        SIGNATURE_TOOLS_LOGE("overflow chunkCount: %d, chunkDigestLen: %d",
                             chunkCount, chunkDigestLen);
        return false;
    }

    sumOfChunkDigestLen = ZIP_CHUNK_DIGEST_PRIFIX_LEN + chunkCount * chunkDigestLen;
    return true;
}

bool HapSignerBlockUtils::ComputeDigestsForEachChunk(const DigestParameter& digestParam,
                                                     DataSource* contents[], int32_t len, ByteBuffer& result)
{
    int32_t chunkCount = 0;
    int32_t sumOfChunksLen = 0;
    if (!GetSumOfChunkDigestLen(contents, len, digestParam.digestOutputSizeBytes, chunkCount, sumOfChunksLen)) {
        SIGNATURE_TOOLS_LOGE("GetSumOfChunkDigestLen failed");
        return false;
    }
    result.SetCapacity(sumOfChunksLen);
    result.PutByte(0, ZIP_FIRST_LEVEL_CHUNK_PREFIX);
    result.PutInt32(1, chunkCount);

    int32_t chunkIndex = 0;
    unsigned char outBlock[EVP_MAX_MD_SIZE];
    unsigned char zipChunkContentPrefix[ZIP_CHUNK_DIGEST_PRIFIX_LEN] = {
    (unsigned char)ZIP_SECOND_LEVEL_CHUNK_PREFIX, 0, 0, 0, 0};

    int32_t zipOffset = ZIP_CHUNK_DIGEST_PRIFIX_LEN;
    for (int32_t i = 0; i < len; i++) {
        while (contents[i]->HasRemaining()) {
            int32_t digestChunkSize = std::min(contents[i]->Remaining(), CHUNK_SIZE);
            if (!InitDigestPrefix(digestParam, zipChunkContentPrefix, digestChunkSize)) {
                SIGNATURE_TOOLS_LOGE("InitDigestPrefix failed");
                return false;
            }

            if (!contents[i]->ReadDataAndDigestUpdate(digestParam, digestChunkSize)) {
                SIGNATURE_TOOLS_LOGE("Copy Partial Buffer failed, count: %d", chunkIndex);
                return false;
            }

            int32_t digestLen = DigestCommon::GetDigest(digestParam, outBlock);
            if (digestLen != digestParam.digestOutputSizeBytes) {
                SIGNATURE_TOOLS_LOGE("GetDigest failed len: %d digestSizeBytes: %d",
                                     digestLen, digestParam.digestOutputSizeBytes);
                return false;
            }
            result.PutData(zipOffset, reinterpret_cast<char*>(outBlock), digestParam.digestOutputSizeBytes);
            zipOffset += digestLen;
            chunkIndex++;
        }
    }
    return true;
}

DigestParameter HapSignerBlockUtils::GetDigestParameter(int32_t nId)
{
    DigestParameter digestParam;
    digestParam.digestOutputSizeBytes = DigestCommon::GetDigestAlgorithmOutputSizeBytes(nId);
    digestParam.md = EVP_get_digestbynid(nId);
    digestParam.ctxPtr = EVP_MD_CTX_create();
    EVP_MD_CTX_init(digestParam.ctxPtr);
    return digestParam;
}

int32_t HapSignerBlockUtils::GetChunkCount(int64_t inputSize, int64_t chunkSize)
{
    if (chunkSize <= 0 || inputSize > LLONG_MAX - chunkSize) {
        return 0;
    }
    int64_t res = (inputSize + chunkSize - 1) / chunkSize;
    if (res > INT_MAX || res < 0) {
        return 0;
    }
    return static_cast<int>(res);
}

bool HapSignerBlockUtils::InitDigestPrefix(const DigestParameter& digestParam,
                                           unsigned char(&chunkContentPrefix)[ZIP_CHUNK_DIGEST_PRIFIX_LEN],
                                           int32_t chunkLen)
{
    if (memcpy_s((chunkContentPrefix + 1), ZIP_CHUNK_DIGEST_PRIFIX_LEN - 1,
                 (&chunkLen), sizeof(chunkLen)) != EOK) {
        SIGNATURE_TOOLS_LOGE("memcpy_s failed");
        return false;
    }

    if (!DigestCommon::DigestInit(digestParam)) {
        SIGNATURE_TOOLS_LOGE("DigestInit failed");
        return false;
    }

    if (!DigestCommon::DigestUpdate(digestParam, chunkContentPrefix, ZIP_CHUNK_DIGEST_PRIFIX_LEN)) {
        SIGNATURE_TOOLS_LOGE("DigestUpdate failed");
        return false;
    }
    return true;
}

int64_t HapSignerBlockUtils::CreatTestZipFile(const std::string& pathFile, SignatureInfo& signInfo)
{
    std::ofstream hapFileInfo(pathFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    if (!hapFileInfo.is_open()) {
        return 0;
    }
    char block[TEST_FILE_BLOCK_LENGTH] = {0};
    /* input contents of ZIP entries */
    hapFileInfo.seekp(0, std::ios_base::beg);
    hapFileInfo.write(block, sizeof(block));
    /* input sign block */
    HapSubSignBlockHead signBlob;
    HapSubSignBlockHead profileBlob;
    HapSubSignBlockHead propertyBlob;
    CreateHapSubSignBlockHead(signBlob, profileBlob, propertyBlob);
    hapFileInfo.write(reinterpret_cast<char*>(&signBlob), sizeof(signBlob));
    hapFileInfo.write(reinterpret_cast<char*>(&profileBlob), sizeof(profileBlob));
    hapFileInfo.write(reinterpret_cast<char*>(&propertyBlob), sizeof(propertyBlob));
    for (int32_t i = 0; i < TEST_FILE_BLOCK_COUNT; i++) {
        hapFileInfo.write(block, sizeof(block));
    }
    int32_t blockCount = TEST_FILE_BLOCK_COUNT;
    hapFileInfo.write(reinterpret_cast<char*>(&blockCount), sizeof(blockCount));
    int64_t signBlockSize = (sizeof(HapSubSignBlockHead) + sizeof(block)) * TEST_FILE_BLOCK_COUNT +
        HapSignerBlockUtils::ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH;
    hapFileInfo.write(reinterpret_cast<char*>(&signBlockSize), sizeof(signBlockSize));
    int64_t magic = HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_LOW_OLD;
    hapFileInfo.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    magic = HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_HIGH_OLD;
    hapFileInfo.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    int32_t version = 1;
    hapFileInfo.write(reinterpret_cast<char*>(&version), sizeof(version));
    /* input central direction */
    hapFileInfo.write(block, sizeof(block));
    /* input end of central direction */
    int32_t zidEocdSign = HapSignerBlockUtils::ZIP_EOCD_SEGMENT_FLAG;
    hapFileInfo.write(reinterpret_cast<char*>(&zidEocdSign), sizeof(zidEocdSign));
    hapFileInfo.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    uint32_t centralDirLen = sizeof(block);
    hapFileInfo.write(reinterpret_cast<char*>(&centralDirLen), sizeof(centralDirLen));
    uint32_t centralDirOffset = TEST_FILE_BLOCK_LENGTH + signBlockSize;
    hapFileInfo.write(reinterpret_cast<char*>(&centralDirOffset), sizeof(centralDirOffset));
    short eocdCommentLen = 0;
    hapFileInfo.write(reinterpret_cast<char*>(&eocdCommentLen), sizeof(eocdCommentLen));
    hapFileInfo.close();
    signInfo.hapCentralDirOffset = centralDirOffset;
    signInfo.hapEocdOffset = centralDirOffset + centralDirLen;
    signInfo.hapSignatureBlock.SetCapacity(TEST_FILE_BLOCK_LENGTH);
    signInfo.hapSignatureBlock.PutData(0, block, sizeof(block));
    int64_t sumLen = signInfo.hapEocdOffset + sizeof(zidEocdSign) + sizeof(centralDirLen) +
        sizeof(centralDirOffset) + sizeof(magic) + sizeof(eocdCommentLen);
    return sumLen;
}

void HapSignerBlockUtils::CreateHapSubSignBlockHead(HapSubSignBlockHead& signBlob,
                                                    HapSubSignBlockHead& profileBlob,
                                                    HapSubSignBlockHead& propertyBlob)
{
    signBlob.type = HAP_SIGN_BLOB;
    signBlob.length = TEST_FILE_BLOCK_LENGTH;
    signBlob.offset = sizeof(HapSubSignBlockHead) * TEST_FILE_BLOCK_COUNT;
    propertyBlob.type = PROPERTY_BLOB;
    propertyBlob.length = TEST_FILE_BLOCK_LENGTH;
    propertyBlob.offset = profileBlob.offset + profileBlob.length;
    profileBlob.type = PROFILE_BLOB;
    profileBlob.length = TEST_FILE_BLOCK_LENGTH;
    profileBlob.offset = signBlob.offset + signBlob.length;
}
} // namespace SignatureTools
} // namespace OHOS