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

#include <unistd.h>
#include <fstream>
#include <map>
#include <gtest/gtest.h>

#include "byte_buffer_data_source.h"
#include "random_access_file.h"
#include "hap_signer_block_utils.h"
#include "hap_signer_block_utils_test.h"

using namespace testing::ext;

namespace OHOS {
namespace SignatureTools {
void CreateHapSubSignBlockHead(HapSubSignBlockHead& signBlob, HapSubSignBlockHead& profileBlob,
                               HapSubSignBlockHead& propertyBlob)
{
    signBlob.type = HAP_SIGN_BLOB;
    signBlob.length = TEST_FILE_BLOCK_LENGTH;
    signBlob.offset = sizeof(HapSubSignBlockHead) * TEST_FILE_BLOCK_COUNT;
    profileBlob.type = PROFILE_BLOB;
    profileBlob.length = TEST_FILE_BLOCK_LENGTH;
    profileBlob.offset = signBlob.offset + signBlob.length;
    propertyBlob.type = PROPERTY_BLOB;
    propertyBlob.length = TEST_FILE_BLOCK_LENGTH;
    propertyBlob.offset = profileBlob.offset + profileBlob.length;
}

int64_t CreatTestZipFile(const std::string& pathFile, SignatureInfo& signInfo)
{
    std::ofstream hapFile(pathFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    if (!hapFile.is_open()) {
        return 0;
    }
    char block[TEST_FILE_BLOCK_LENGTH] = { 0 };
    /* input contents of ZIP entries */
    hapFile.seekp(0, std::ios_base::beg);
    hapFile.write(block, sizeof(block));
    /* input sign block */
    HapSubSignBlockHead signBlob;
    HapSubSignBlockHead profileBlob;
    HapSubSignBlockHead propertyBlob;
    CreateHapSubSignBlockHead(signBlob, profileBlob, propertyBlob);
    hapFile.write(reinterpret_cast<char*>(&signBlob), sizeof(signBlob));
    hapFile.write(reinterpret_cast<char*>(&profileBlob), sizeof(profileBlob));
    hapFile.write(reinterpret_cast<char*>(&propertyBlob), sizeof(propertyBlob));
    for (int32_t i = 0; i < TEST_FILE_BLOCK_COUNT; i++) {
        hapFile.write(block, sizeof(block));
    }
    int32_t blockCount = TEST_FILE_BLOCK_COUNT;
    hapFile.write(reinterpret_cast<char*>(&blockCount), sizeof(blockCount));
    int64_t signBlockSize = (sizeof(HapSubSignBlockHead) + sizeof(block)) *
        TEST_FILE_BLOCK_COUNT +
        HapSignerBlockUtils::ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH;
    hapFile.write(reinterpret_cast<char*>(&signBlockSize), sizeof(signBlockSize));
    int64_t magic = HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_LOW_OLD;
    hapFile.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    magic = HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_HIGH_OLD;
    hapFile.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    int32_t version = 1;
    hapFile.write(reinterpret_cast<char*>(&version), sizeof(version));
    /* input central direction */
    hapFile.write(block, sizeof(block));
    /* input end of central direction */
    int32_t zidEocdSign = HapSignerBlockUtils::ZIP_EOCD_SEGMENT_FLAG;
    hapFile.write(reinterpret_cast<char*>(&zidEocdSign), sizeof(zidEocdSign));
    hapFile.write(reinterpret_cast<char*>(&magic), sizeof(magic));
    uint32_t centralDirLen = sizeof(block);
    hapFile.write(reinterpret_cast<char*>(&centralDirLen), sizeof(centralDirLen));
    uint32_t centralDirOffset = TEST_FILE_BLOCK_LENGTH + signBlockSize;
    hapFile.write(reinterpret_cast<char*>(&centralDirOffset), sizeof(centralDirOffset));
    short eocdCommentLen = 0;
    hapFile.write(reinterpret_cast<char*>(&eocdCommentLen), sizeof(eocdCommentLen));
    hapFile.close();
    signInfo.hapCentralDirOffset = centralDirOffset;
    signInfo.hapEocdOffset = centralDirOffset + centralDirLen;
    signInfo.hapSignatureBlock.SetCapacity(TEST_FILE_BLOCK_LENGTH);
    signInfo.hapSignatureBlock.PutData(0, block, sizeof(block));
    int64_t sumLen = signInfo.hapEocdOffset + sizeof(zidEocdSign) + sizeof(centralDirLen) +
        sizeof(centralDirOffset) + sizeof(magic) + sizeof(eocdCommentLen);
    return sumLen;
}

int SetTestSignerInfoSignAlgor(PKCS7_SIGNER_INFO* info)
{
    int signNid = 0;
    int hashNid = 0;
    X509_ALGOR* dig;
    X509_ALGOR* sig;
    PKCS7_SIGNER_INFO_get0_algs(info, NULL, &dig, &sig);
    if (dig == NULL || dig->algorithm == NULL ||
        (hashNid = OBJ_obj2nid(dig->algorithm)) == NID_undef ||
        !OBJ_find_sigid_by_algs(&signNid, hashNid, NID_X9_62_id_ecPublicKey) ||
        X509_ALGOR_set0(sig, OBJ_nid2obj(signNid), V_ASN1_UNDEF, 0) != 1) {
        return 0;
    }
    return 1;
}

int Pkcs7TestSetSignerInfo(PKCS7_SIGNER_INFO* info, X509* cert, const EVP_MD* hash)
{
    if (!ASN1_INTEGER_set(info->version, 1) ||
        !X509_NAME_set(&info->issuer_and_serial->issuer, X509_get_issuer_name(cert))) {
        return 0;
    }

    ASN1_INTEGER_free(info->issuer_and_serial->serial);
    if (!(info->issuer_and_serial->serial =
        ASN1_INTEGER_dup(X509_get_serialNumber(cert)))) {
        return 0;
    }

    X509_ALGOR_set0(info->digest_alg, OBJ_nid2obj(EVP_MD_type(hash)),
                    V_ASN1_NULL, NULL);

    if (!SetTestSignerInfoSignAlgor(info)) {
        return 0;
    }
    return 1;
}

const std::string HAP_VERIFY_V2_PATH = "./hapVerify/hap_verify_v2.hap";
const std::string HAP_VERIFY_V3_PATH = "./hapVerify/hap_verify_v3.hap";
class HapSignerBlockUtilsTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
    static const int32_t TEST_ZIP_BLOCKS_NUM_NEED_DIGEST;
};

const int32_t HapSignerBlockUtilsTest::TEST_ZIP_BLOCKS_NUM_NEED_DIGEST = 3;

void HapSignerBlockUtilsTest::SetUpTestCase(void)
{
    (void)rename("./hapVerify/hap_verify_v2.txt", HAP_VERIFY_V2_PATH.c_str());
    (void)rename("./hapVerify/hap_verify_v3.txt", HAP_VERIFY_V3_PATH.c_str());
    sync();
}

void HapSignerBlockUtilsTest::TearDownTestCase(void)
{
}

void HapSignerBlockUtilsTest::SetUp()
{
}

void HapSignerBlockUtilsTest::TearDown()
{
}

/**
 * @tc.name: Test FindHapSignature function
 * @tc.desc: input one right file and some error files, The static function will return correct result;
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, FindHapSignatureTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. create a test zip file.
     */
    std::string pathFile = "./hapVerify/test_hap_verify.hap";
    SignatureInfo signInfo;
    int64_t sumLen = CreatTestZipFile(pathFile, signInfo);
    /*
     * @tc.steps: step2. test FindHapSignature function
     * @tc.expected: step2. the return will be true.
     */
    RandomAccessFile hapTestFile;
    ASSERT_TRUE(hapTestFile.Init(pathFile));
    ASSERT_EQ(hapTestFile.GetLength(), sumLen);
    SignatureInfo hapSignInfo;
    ASSERT_TRUE(HapSignerBlockUtils::FindHapSignature(hapTestFile, hapSignInfo));
    /*
     * @tc.steps: step3. make central offset error, and test FindHapSignature function
     * @tc.expected: step3. can not find central directory, the return will be false.
     */
    ByteBuffer eocd(TEST_ZIP_EOCD_SIZE);
    EXPECT_GT(hapTestFile.ReadFileFullyFromOffset(eocd, sumLen - TEST_ZIP_EOCD_SIZE), 0);
    ByteBuffer buff(eocd);
    buff.PutInt32(TEST_ZIP_ECD_OFFSET_FIELD_OFFSET, TEST_HAPBYTEBUFFER_INT32_DATA);
    EXPECT_GT(hapTestFile.WriteToFile(buff, sumLen - TEST_ZIP_EOCD_SIZE, buff.GetCapacity()), 0);
    ASSERT_FALSE(HapSignerBlockUtils::FindHapSignature(hapTestFile, hapSignInfo));
    /*
     * @tc.steps: step4. make eocd comment error, and test FindHapSignature function
     * @tc.expected: step4. can not find eocd, the return will be false.
     */
    ByteBuffer buff2(eocd);
    buff2.PutInt16(TEST_ZIP_EOCD_COMMENT_OFFSET, TEST_HAPBYTEBUFFER_UINT16_DATA);
    EXPECT_GT(hapTestFile.WriteToFile(buff2, sumLen - TEST_ZIP_EOCD_SIZE, buff2.GetCapacity()), 0);
    ASSERT_FALSE(HapSignerBlockUtils::FindHapSignature(hapTestFile, hapSignInfo));
    /*
     * @tc.steps: step5. make hap signing block error, and test FindHapSignature function
     * @tc.expected: step4. can not find hap signing block, the return will be false.
     */
    RandomAccessFile hapTestFile2;
    ASSERT_TRUE(hapTestFile2.Init(HAP_VERIFY_V2_PATH));
    ASSERT_FALSE(HapSignerBlockUtils::FindHapSignature(hapTestFile2, hapSignInfo));
}

/**
 * @tc.name: Test FindEocdInHap function
 * @tc.desc: create a file with invalid length, The function will return false;
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, FindEocdInHapTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. create a test file with invalid length.
     */
    std::string pathFile = "./hapVerify/test_hap_verify.hap";
    std::ofstream hapFile;
    hapFile.open(pathFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(hapFile.is_open());
    uint32_t centralDirLen = TEST_HAPBYTEBUFFER_UINT32_DATA;
    hapFile.write(reinterpret_cast<char*>(&centralDirLen), sizeof(centralDirLen));
    hapFile.close();
    /*
     * @tc.steps: step2. test FindEocdInHap function
     * @tc.expected: step2. the return will be false.
     */
    RandomAccessFile hapTestFile;
    hapTestFile.Init(pathFile);
    std::pair<ByteBuffer, int64_t> eocd;

    ASSERT_FALSE(HapSignerBlockUtils::FindEocdInHap(hapTestFile, eocd));
    /*
     * @tc.steps: step3. test FindEocdInHap function
     * @tc.expected: step3. make the file length is right, but the comment size is wrong, the return will be false.
     */
    ByteBuffer fileLen(TEST_FILE_BLOCK_LENGTH);
    EXPECT_GT(hapTestFile.WriteToFile(fileLen, 0, fileLen.GetCapacity()), 0);
    int32_t maxCommentSize = TEST_INVALID_MAX_COMMENT_SIZE;
    ASSERT_FALSE(HapSignerBlockUtils::FindEocdInHap(hapTestFile, maxCommentSize, eocd));
    maxCommentSize = TEST_MAX_COMMENT_SIZE;
    ASSERT_FALSE(HapSignerBlockUtils::FindEocdInHap(hapTestFile, maxCommentSize, eocd));
    /*
     * @tc.steps: step4. test FindEocdInSearchBuffer function
     * @tc.expected: step4. make the searchBuffer is wrong, the return will be false.
     */
    ByteBuffer testHapBuffer(TEST_HAPBYTEBUFFER_LENGTH);
    int32_t offset = 0;
    ASSERT_FALSE(HapSignerBlockUtils::FindEocdInSearchBuffer(testHapBuffer, offset));
    ByteBuffer eocdBuff(TEST_ZIP_EOCD_SIZE);
    eocdBuff.PutInt32(HapSignerBlockUtils::ZIP_EOCD_SEGMENT_FLAG);
    eocdBuff.Flip();
    ASSERT_FALSE(HapSignerBlockUtils::FindEocdInSearchBuffer(eocdBuff, offset));
}

/**
 * @tc.name: Test GetCentralDirectoryOffset function
 * @tc.desc: create an ecod with invalid central offset and length,
 *           The function will return TEST_NOT_FIND_TARGET_OFFSET;
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, GetCentralDirectoryOffsetTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. create a test eocd with invalid central offset and length.
     */
    ByteBuffer testEocd(TEST_ZIP_EOCD_SIZE);
    int32_t centralDirLen = TEST_FILE_BLOCK_LENGTH;
    testEocd.PutInt32(TEST_ZIP_ECD_SIZE_FIELD_OFFSET, centralDirLen);
    int32_t centralDirOffset = TEST_FILE_BLOCK_LENGTH;
    testEocd.PutInt32(TEST_ZIP_ECD_OFFSET_FIELD_OFFSET, centralDirOffset);

    ASSERT_FALSE(HapSignerBlockUtils::SetUnsignedInt32(testEocd, 0, TEST_INVALID_MAX_COMMENT_SIZE));
    ByteBuffer emptyEocd;
    /*
     * @tc.steps: step2. run function with error eocdoffset
     * @tc.expected: step2. the return will be NOT_FIND_TARGET_OFFSET.
     */
    int64_t offset;
    ASSERT_FALSE(HapSignerBlockUtils::GetCentralDirectoryOffset(emptyEocd, 0, offset));
    ASSERT_FALSE(HapSignerBlockUtils::GetCentralDirectoryOffset(testEocd, 0, offset));
    ASSERT_FALSE(HapSignerBlockUtils::GetCentralDirectoryOffset(testEocd, TEST_FILE_BLOCK_LENGTH, offset));
}

/**
 * @tc.name: Test GetCentralDirectorySize function
 * @tc.desc: create an wrong eocd, the function will return false;
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, GetCentralDirectorySizeTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. create a test eocd with invalid length;
     * @tc.expected: step1. the return will be false.
     */
    ByteBuffer testEocd(TEST_HAPBYTEBUFFER_LENGTH);
    long centralDirectorySize;

    ASSERT_FALSE(HapSignerBlockUtils::GetCentralDirectorySize(testEocd, centralDirectorySize));
}

/**
 * @tc.name: Test FindHapSigningBlock function
 * @tc.desc: input one right file and some error files, The static function will return correct result;
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, FindHapSigningBlockTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. create a test zip file.
     */
    std::string pathFile = "./hapVerify/test_hap_verify.hap";
    SignatureInfo signInfo;
    int64_t sumLen = CreatTestZipFile(pathFile, signInfo);
    /*
     * @tc.steps: step2. test FindHapSigningBlock function
     * @tc.expected: step2. the return will be true.
     */
    RandomAccessFile hapTestFile;
    ASSERT_TRUE(hapTestFile.Init(pathFile));
    ASSERT_EQ(hapTestFile.GetLength(), sumLen);

    SignatureInfo hapSignInfo;
    ASSERT_TRUE(HapSignerBlockUtils::FindHapSigningBlock(hapTestFile, signInfo.hapCentralDirOffset, hapSignInfo));
    /*
     * @tc.steps: step3. test FindHapSigningBlock function
     * @tc.expected: step3. can not find cd offset, the return will be false.
     */
    ASSERT_FALSE(HapSignerBlockUtils::FindHapSigningBlock(hapTestFile, 0, hapSignInfo));
    /*
     * @tc.steps: step4. test FindHapSigningBlock function
     * @tc.expected: step4. cd offset is out of range, the return will be false.
     */
    ASSERT_FALSE(HapSignerBlockUtils::FindHapSigningBlock(hapTestFile, TEST_HAPBYTEBUFFER_INT32_DATA, hapSignInfo));
    /*
     * @tc.steps: step5. test CheckSignBlockHead function
     * @tc.expected: step5. make hapSignBlockMagic is wrong, the return will be false.
     */
    RandomAccessFile hapTestFile2;
    ASSERT_TRUE(hapTestFile2.Init(HAP_VERIFY_V2_PATH));
    int64_t fileLength = hapTestFile2.GetLength();
    ByteBuffer eocd(TEST_ZIP_EOCD_SIZE);
    int32_t centralDirOffset;
    EXPECT_GT(hapTestFile2.ReadFileFullyFromOffset(eocd, fileLength - TEST_ZIP_EOCD_SIZE), 0);
    ASSERT_TRUE(eocd.GetInt32(TEST_ZIP_ECD_OFFSET_FIELD_OFFSET, centralDirOffset));
    ASSERT_FALSE(HapSignerBlockUtils::FindHapSigningBlock(hapTestFile2, centralDirOffset, hapSignInfo));
    /*
     * @tc.steps: step6. test CheckSignBlockHead function
     * @tc.expected: step6. make hapSignBlockOffset is wrong, the return will be false.
     */
    RandomAccessFile hapTestFile3;
    ASSERT_TRUE(hapTestFile3.Init(HAP_VERIFY_V3_PATH));
    fileLength = hapTestFile3.GetLength();
    ByteBuffer eocd2(TEST_ZIP_EOCD_SIZE);
    EXPECT_GT(hapTestFile3.ReadFileFullyFromOffset(eocd2, fileLength - TEST_ZIP_EOCD_SIZE), 0);
    ASSERT_TRUE(eocd2.GetInt32(TEST_ZIP_ECD_OFFSET_FIELD_OFFSET, centralDirOffset));
    ASSERT_FALSE(HapSignerBlockUtils::FindHapSigningBlock(hapTestFile3, centralDirOffset, hapSignInfo));
}

/**
 * @tc.name: Test CheckSignBlockHead function
 * @tc.desc: input one right file and some error files, The static function will return correct result;
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, CheckSignBlockHeadTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. test CheckSignBlockHead function
     * @tc.expected: step1. check the hapSignBlockMagicHi is wrong, the return will be false.
     */
    HapSignBlockHead hapSignBlockHead{
        .version = 3,
        .blockCount = 0,
        .hapSignBlockSize = 0,
        .hapSignBlockMagicLo = HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_LOW,
        .hapSignBlockMagicHi = 0,
    };

    ASSERT_FALSE(HapSignerBlockUtils::CheckSignBlockHead(hapSignBlockHead));
    /*
     * @tc.steps: step2. test CheckSignBlockHead function
     * @tc.expected: step2. check the hapSignBlockSize is less than ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH,
     * the return will be false.
     */
    hapSignBlockHead.hapSignBlockMagicHi = HapSignerBlockUtils::HAP_SIG_BLOCK_MAGIC_HIGH;
    ASSERT_FALSE(HapSignerBlockUtils::CheckSignBlockHead(hapSignBlockHead));
    /*
     * @tc.steps: step3. test CheckSignBlockHead function
     * @tc.expected: step3. check the hapSignBlockSize is greater than MAX_HAP_SIGN_BLOCK_SIZE,
     * the return will be false.
     */
    hapSignBlockHead.hapSignBlockSize = INT_MAX;
    ASSERT_FALSE(HapSignerBlockUtils::CheckSignBlockHead(hapSignBlockHead));
    /*
     * @tc.steps: step4. test CheckSignBlockHead function
     * @tc.expected: step4. check the blockCount is greater than MAX_BLOCK_COUNT, the return will be false.
     */
    hapSignBlockHead.hapSignBlockSize = HapSignerBlockUtils::ZIP_HEAD_OF_SIGNING_BLOCK_LENGTH;
    hapSignBlockHead.blockCount = INT_MAX;
    ASSERT_FALSE(HapSignerBlockUtils::CheckSignBlockHead(hapSignBlockHead));
}

/**
 * @tc.name: Test FindHapSubSigningBlock function
 * @tc.desc: input one right file and some error files, The static function will return correct result;
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, FindHapSubSigningBlockTest001, TestSize.Level1)
{
    RandomAccessFile hapTestFile;
    ASSERT_TRUE(hapTestFile.Init(HAP_VERIFY_V2_PATH));

    ByteBuffer hapSignatureBlock(1);
    ByteBuffer hapEocd(1);
    ByteBuffer optionalBlockValue(1);
    std::vector<OptionalBlock> optionalBlocks{
        {
            .optionalType = 0,
            .optionalBlockValue = optionalBlockValue
        }
    };
    SignatureInfo signatureInfo{
        .hapSignatureBlock = hapSignatureBlock,
        .hapSigningBlockOffset = 0,
        .hapCentralDirOffset = 0,
        .hapEocdOffset = 0,
        .hapEocd = hapEocd,
        .optionBlocks = optionalBlocks,
        .version = 0
    };

    ASSERT_FALSE(HapSignerBlockUtils::FindHapSubSigningBlock(hapTestFile, 0, INT_MAX, 0, signatureInfo));

    ASSERT_FALSE(HapSignerBlockUtils::FindHapSubSigningBlock(hapTestFile, 1, 0, INT_MAX, signatureInfo));

    ASSERT_FALSE(HapSignerBlockUtils::FindHapSubSigningBlock(hapTestFile, 1, 0, 0, signatureInfo));

    ASSERT_FALSE(HapSignerBlockUtils::FindHapSubSigningBlock(hapTestFile, 1, 128, 128, signatureInfo));
}

/**
 * @tc.name: Test ClassifyHapSubSigningBlock function
 * @tc.desc: test function of classify optional block
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, ClassifyHapSubSigningBlock001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. run function with input of property block
     * @tc.expected: step1. the return signInfo.optionBlocks has property block which inputed.
     */
    SignatureInfo signInfo;
    ByteBuffer subBlock;
    uint32_t type = PROPERTY_BLOB;

    HapSignerBlockUtils::ClassifyHapSubSigningBlock(signInfo, subBlock, type);
    ASSERT_FALSE(signInfo.optionBlocks.empty());
    ASSERT_TRUE(signInfo.optionBlocks[0].optionalType == PROPERTY_BLOB);
}

/**
 * @tc.name: Test ComputeDigestsWithOptionalBlock function
 * @tc.desc: use an error nid and a right nid to compute digest
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, ComputeDigestsWithOptionalBlock001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. run function with an error nid and a right nid
     * @tc.expected: step1. the return will be false and right respectively.
     */
    OptionalBlock testOptionalBlock;
    testOptionalBlock.optionalType = PROPERTY_BLOB;
    testOptionalBlock.optionalBlockValue.SetCapacity(TEST_HAPBYTEBUFFER_LENGTH);
    std::vector<OptionalBlock> optionalBlocks;
    optionalBlocks.push_back(testOptionalBlock);

    ByteBuffer chunkDigest(TEST_HAPBYTEBUFFER_LENGTH);
    ByteBuffer finalDigest;
    int32_t nid = TEST_NULL_NID;
    DigestParameter errorParam = HapSignerBlockUtils::GetDigestParameter(nid);
    int32_t ret = HapSignerBlockUtils::ComputeDigestsWithOptionalBlock(
        errorParam, optionalBlocks, chunkDigest, finalDigest);
    ASSERT_FALSE(ret);
    nid = TEST_SHA256_NID;
    DigestParameter digestParam = HapSignerBlockUtils::GetDigestParameter(nid);
    ret = HapSignerBlockUtils::ComputeDigestsWithOptionalBlock(digestParam, optionalBlocks, chunkDigest, finalDigest);
    ASSERT_TRUE(ret);
}

/**
 * @tc.name: Test GetSumOfChunkDigestLen function
 * @tc.desc: Test GetSumOfChunkDigestLen with some error inputs
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, GetSumOfChunkDigestLenTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. input some error input to test GetSumOfChunkDigestLen
     * @tc.expected: step1. the return will be false.
     */
    int32_t chunkCount = 0;
    int32_t sumOfChunkDigestLen = 0;
    DataSource* contents[TEST_ZIP_BLOCKS_NUM_NEED_DIGEST] = { nullptr, nullptr, nullptr };
    bool ret = HapSignerBlockUtils::GetSumOfChunkDigestLen(contents, 0, 0, chunkCount, sumOfChunkDigestLen);
    ASSERT_FALSE(ret);
    ret = HapSignerBlockUtils::GetSumOfChunkDigestLen(contents,
                                                      TEST_ZIP_BLOCKS_NUM_NEED_DIGEST, 0,
                                                      chunkCount, sumOfChunkDigestLen);
    ASSERT_FALSE(ret);
    ByteBuffer testBuffer(TEST_ZIP_EOCD_SIZE);
    ByteBufferDataSource testSource(testBuffer);
    for (int32_t i = 0; i < TEST_ZIP_BLOCKS_NUM_NEED_DIGEST; i++) {
        contents[i] = &testSource;
    }
    ret = HapSignerBlockUtils::GetSumOfChunkDigestLen(contents,
                                                      TEST_ZIP_BLOCKS_NUM_NEED_DIGEST,
                                                      INT_MAX, chunkCount,
                                                      sumOfChunkDigestLen);
    ASSERT_FALSE(ret);
}

/**
 * @tc.name: Test VerifyHapIntegrity function
 * @tc.desc: create a file and input error digest, The static function will return false;
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, VerifyHapIntegrityTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. create a test zip file without eocd.
     */
    std::string pathFile = "./hapVerify/test_hap_verify.hap";
    SignatureInfo signInfo;
    CreatTestZipFile(pathFile, signInfo);
    /*
     * @tc.steps: step2. create an error digest to test VerifyHapIntegrity function
     * @tc.expected: step2. the return will be false.
     */
    Pkcs7Context digestInfo;
    digestInfo.content.SetCapacity(TEST_FILE_BLOCK_LENGTH);
    RandomAccessFile hapTestFile;
    hapTestFile.Init(pathFile);

    ASSERT_FALSE(HapSignerBlockUtils::VerifyHapIntegrity(digestInfo, hapTestFile, signInfo));
}

/**
 * @tc.name: VerifyHapError001
 * @tc.desc: This function tests failure for interface GetCertsChain due to PKCS7_SIGNER_INFO have not cert
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, VerifyHapError001, TestSize.Level1)
{
    PKCS7* pkcs7 = PKCS7_new();
    X509* cert = X509_new();
    PKCS7_set_type(pkcs7, NID_pkcs7_signed);
    PKCS7_content_new(pkcs7, NID_pkcs7_data);
    const EVP_MD* md = EVP_sha384();
    PKCS7_SIGNER_INFO* info = PKCS7_SIGNER_INFO_new();
    Pkcs7TestSetSignerInfo(info, cert, md);
    PKCS7_add_signer(pkcs7, info);
    Pkcs7Context pkcs7Context;
    bool ret = VerifyHapOpensslUtils::GetCertChains(pkcs7, pkcs7Context);
    EXPECT_EQ(ret, false);
    PKCS7_free(pkcs7);
    X509_free(cert);
}

/**
 * @tc.name: VerifyHapError002
 * @tc.desc: This function tests failure for interface GetCrlStack due to pkcs7 not init
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, VerifyHapError002, TestSize.Level1)
{
    PKCS7* pkcs7 = PKCS7_new();
    bool ret = VerifyHapOpensslUtils::GetCrlStack(pkcs7, nullptr);
    EXPECT_EQ(ret, false);
    PKCS7_free(pkcs7);
}

/**
 * @tc.name: VerifyHapError003
 * @tc.desc: This function tests failure for interface VerifyPkcs7 due to cert1 not init
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, VerifyHapError003, TestSize.Level1)
{
    PKCS7* pkcs7 = PKCS7_new();
    X509* cert = X509_new();
    PKCS7_set_type(pkcs7, NID_pkcs7_signed);
    PKCS7_content_new(pkcs7, NID_pkcs7_data);
    const EVP_MD* md = EVP_sha384();
    PKCS7_SIGNER_INFO* info = PKCS7_SIGNER_INFO_new();
    Pkcs7TestSetSignerInfo(info, cert, md);
    char* t = new char[2];
    t[0] = 1;
    t[1] = 2;
    ASN1_STRING_set0(info->enc_digest, t, 2);
    PKCS7_add_signer(pkcs7, info);
    Pkcs7Context pkcs7Context;
    pkcs7Context.p7 = pkcs7;
    CertChain certs;
    X509* cert1 = X509_new();
    certs.push_back(cert1);
    pkcs7Context.certChain.push_back(certs);
    bool ret = VerifyHapOpensslUtils::VerifyPkcs7(pkcs7Context);
    EXPECT_EQ(ret, false);
    X509_free(cert);
    X509_free(cert1);
}

/**
 * @tc.name: VerifyHapError004
 * @tc.desc: This function tests failure for interface ParsePkcs7Package due to cert1 not init
 * @tc.type: FUNC
 */
HWTEST_F(HapSignerBlockUtilsTest, VerifyHapError004, TestSize.Level1)
{
    PKCS7* pkcs7 = PKCS7_new();
    PKCS7_set_type(pkcs7, NID_pkcs7_signed);
    PKCS7_content_new(pkcs7, NID_pkcs7_data);
    unsigned char* p = nullptr;
    int len = i2d_PKCS7(pkcs7, &p);
    Pkcs7Context pkcs7Context;
    bool ret = VerifyHapOpensslUtils::ParsePkcs7Package(p, len, pkcs7Context);
    EXPECT_EQ(ret, false);
    PKCS7_free(pkcs7);
}
} // namespace SignatureTools
} // namespace OHOS