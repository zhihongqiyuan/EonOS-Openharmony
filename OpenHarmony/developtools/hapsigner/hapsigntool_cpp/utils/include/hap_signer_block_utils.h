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
#ifndef SIGNATRUETOOLS_SIGNING_BLOCK_UTILS_H
#define SIGNATRUETOOLS_SIGNING_BLOCK_UTILS_H

#include <vector>

#include "data_source.h"
#include "export_define.h"
#include "byte_buffer.h"
#include "random_access_file.h"
#include "digest_parameter.h"
#include "pkcs7_context.h"
#include "signature_info.h"

namespace OHOS {
namespace SignatureTools {

constexpr int32_t ZIP_CHUNK_DIGEST_PRIFIX_LEN = 5;

struct HapSignBlockHead {
    int32_t version = 0;
    int32_t blockCount = 0;
    int64_t hapSignBlockSize;
    int64_t hapSignBlockMagicLo;
    int64_t hapSignBlockMagicHi;
};

enum HapBlobType {
    HAP_SIGN_BLOB = 0x20000000,
    PROOF_ROTATION_BLOB = 0x20000001,
    PROFILE_BLOB = 0x20000002,
    PROPERTY_BLOB = 0x20000003,
};

struct HapSubSignBlockHead {
    uint32_t type = 0;
    uint32_t length = 0;
    uint32_t offset = 0;
};

class HapSignerBlockUtils {
public:
    DLL_EXPORT static const int64_t HAP_SIG_BLOCK_MAGIC_HIGH_OLD;
    DLL_EXPORT static const int64_t HAP_SIG_BLOCK_MAGIC_LOW_OLD;
    DLL_EXPORT static const int64_t HAP_SIG_BLOCK_MAGIC_HIGH;
    DLL_EXPORT static const int64_t HAP_SIG_BLOCK_MAGIC_LOW;
    DLL_EXPORT static const int32_t ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH;
    DLL_EXPORT static const int32_t ZIP_EOCD_SEGMENT_FLAG;
    static const int64_t CHUNK_SIZE;
    static const int32_t HAP_SIG_BLOCK_MIN_SIZE;
    static const int32_t ZIP_EOCD_SEG_MIN_SIZE;
    static const int32_t ZIP_EOCD_COMMENT_LENGTH_OFFSET;
    static const int32_t ZIP_CD_OFFSET_IN_EOCD;
    static const int32_t ZIP_CD_SIZE_OFFSET_IN_EOCD;
    static const int32_t ZIP_BLOCKS_NUM_NEED_DIGEST;
    static const char ZIP_FIRST_LEVEL_CHUNK_PREFIX;
    static const char ZIP_SECOND_LEVEL_CHUNK_PREFIX;
    /* the specifications of hap sign block */
    static constexpr int64_t MAX_HAP_SIGN_BLOCK_SIZE = 1024 * 1024 * 1024LL; // 1024MB
    static constexpr int32_t MAX_BLOCK_COUNT = 10;
    static constexpr int32_t VERSION_FOR_NEW_MAGIC_NUM = 3;
    static constexpr int32_t TEST_FILE_BLOCK_LENGTH = 50;
    static constexpr int32_t TEST_FILE_BLOCK_COUNT = 3;

    HapSignerBlockUtils() = delete;
    DLL_EXPORT static bool FindHapSignature(RandomAccessFile& hapFile, SignatureInfo& signInfo);
    DLL_EXPORT static bool GetOptionalBlockIndex(std::vector<OptionalBlock>& optionBlocks,
                                                 int32_t type, int& index);
    DLL_EXPORT static bool VerifyHapIntegrity(Pkcs7Context& digestInfo, RandomAccessFile& hapFile,
                                              SignatureInfo& signInfo);
    DLL_EXPORT static int64_t CreatTestZipFile(const std::string& pathFile, SignatureInfo& signInfo);

    DLL_EXPORT static bool FindEocdInHap(RandomAccessFile& hapFile, std::pair<ByteBuffer, int64_t>& eocd);
    DLL_EXPORT static bool FindEocdInHap(RandomAccessFile& hapFile, unsigned short maxCommentSize,
                                         std::pair<ByteBuffer, int64_t>& eocd);
    DLL_EXPORT static bool FindEocdInSearchBuffer(ByteBuffer& zipContents, int& offset);
    DLL_EXPORT static bool GetCentralDirectoryOffset(ByteBuffer& eocd, int64_t eocdOffset,
                                                     int64_t& centralDirectoryOffset);
    DLL_EXPORT static bool GetCentralDirectorySize(ByteBuffer& eocd, long& centralDirectorySize);
    static bool FindHapSigningBlock(RandomAccessFile& hapFile, int64_t centralDirOffset,
                                    SignatureInfo& signInfo);
    static bool FindHapSubSigningBlock(RandomAccessFile& hapFile,
                                       int32_t blockCount,
                                       int64_t blockArrayLen,
                                       int64_t hapSignBlockOffset,
                                       SignatureInfo& signInfo);
    DLL_EXPORT static bool ClassifyHapSubSigningBlock(SignatureInfo& signInfo,
                                                      const ByteBuffer& subBlock, uint32_t type);
    DLL_EXPORT static bool SetUnsignedInt32(ByteBuffer& buffer, int32_t offset, int64_t value);
    DLL_EXPORT static bool ComputeDigestsWithOptionalBlock(const DigestParameter& digestParam,
                                                           const std::vector<OptionalBlock>& optionalBlocks,
                                                           const ByteBuffer& chunkDigest,
                                                           ByteBuffer& finalDigest);
    static bool ComputeDigestsForEachChunk(const DigestParameter& digestParam, DataSource* contents[],
                                           int32_t len, ByteBuffer& result);
    static int32_t GetChunkCount(int64_t inputSize, int64_t chunkSize);
    static bool InitDigestPrefix(const DigestParameter& digestParam,
                                 unsigned char(&chunkContentPrefix)[ZIP_CHUNK_DIGEST_PRIFIX_LEN],
                                 int32_t chunkLen);
    DLL_EXPORT static DigestParameter GetDigestParameter(int32_t nId);
    DLL_EXPORT static bool GetSumOfChunkDigestLen(DataSource* contents[], int32_t len, int32_t chunkDigestLen,
                                                  int& chunkCount, int& sumOfChunkDigestLen);
    static bool ParseSignBlockHead(HapSignBlockHead& hapSignBlockHead, ByteBuffer& hapBlockHead);
    static bool ParseSubSignBlockHead(HapSubSignBlockHead& subSignBlockHead, ByteBuffer& hapBlockHead);
    static bool CheckSignBlockHead(const HapSignBlockHead& hapSignBlockHead);
    static void CreateHapSubSignBlockHead(HapSubSignBlockHead& signBlob, HapSubSignBlockHead& profileBlob,
                                          HapSubSignBlockHead& propertyBlob);
};
} // namespace SignatureTools
} // namespace OHOS
#endif // HAP_SIGNING_BLOCK_UTILS_H
