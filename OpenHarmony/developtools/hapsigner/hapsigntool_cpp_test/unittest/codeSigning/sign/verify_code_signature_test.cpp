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

#include <memory>
#include <fstream>
#include <gtest/gtest.h>
#include "verify_code_signature.h"
#include "verify_hap.h"
#include "hap_utils.h"

namespace OHOS {
namespace SignatureTools {

/*
 * 测试套件,固定写法
 */
class VerifyCodeSignatureTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void VerifyCodeSignatureTest::SetUpTestCase(void)
{
    (void)rename("./codeSigning/hap_no_suffix_err.txt", "./codeSigning/hap_no_suffix_err");
    (void)rename("./codeSigning/elf_parse_sign_block_err.txt", "./codeSigning/elf_parse_sign_block_err.elf");
    (void)rename("./codeSigning/hap_merkle_tree_err.txt", "./codeSigning/hap_merkle_tree_err.elf");
    (void)rename("./codeSigning/hap_profile_is_null_err.txt", "./codeSigning/hap_profile_is_null_err.hap");
    (void)rename("./codeSigning/hap_invalid_block_header_err.txt", "./codeSigning/hap_invalid_block_header_err.hap");
    (void)rename("./codeSigning/hap_offset_align_err.txt", "./codeSigning/hap_offset_align_err.hap");
    (void)rename("./codeSigning/hap_magic_number_err.txt", "./codeSigning/hap_magic_number_err.hap");
    (void)rename("./codeSigning/hap_segment_header_err.txt", "./codeSigning/hap_segment_header_err.hap");
    (void)rename("./codeSigning/entry-default-signed-so.txt", "./codeSigning/entry-default-signed-so.hap");
    (void)rename("./codeSigning/hap_file_type_err.txt", "./codeSigning/hap_file_type_err.elf");
    (void)rename("./codeSigning/hap_native_libs_err.txt", "./codeSigning/hap_native_libs_err.hap");
}

void VerifyCodeSignatureTest::TearDownTestCase(void)
{
}

void VerifyCodeSignatureTest::SetUp()
{
}

void VerifyCodeSignatureTest::TearDown()
{
}

/**
 * @tc.name: AreVectorsEqual001
 * @tc.desc: Test function of VerifyCodeSignatureTest::AreVectorsEqual() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, AreVectorsEqual001, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> vec1{ 21, 53, 29, 18, -17, 56, 92, 60, 29, 10, 28, 18, 19, 52, 59, 92 };
    std::vector<int8_t> vec2{ 21, 53, 29, 18, -17, 56, 92, 60, 29, 10, 28, 18, 19, 52, 59, 92 };
    bool flag = VerifyCodeSignature::AreVectorsEqual(vec1, vec2);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: AreVectorsEqual002
 * @tc.desc: Test function of VerifyCodeSignatureTest::AreVectorsEqual() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, AreVectorsEqual002, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> vec1{ 21, 53, 29, 19, -17, 56, 92, 60, 29, 10, 28, 18, 19 };
    std::vector<int8_t> vec2{ 21, 53, 29, 18, -17, 56, 92, 60, 29, 10, 28, 18, 19, 52, 59, 92 };
    bool flag = VerifyCodeSignature::AreVectorsEqual(vec1, vec2);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyCodeSign
 * @tc.desc: verify code sign without profile content
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyCodeSign, testing::ext::TestSize.Level1)
{
    std::pair<std::string, std::string> pairResult = std::make_pair("", "debug");
    std::string file = "./codeSigning/entry-default-signed-so.hap";
    int64_t offset = 1397151;
    int64_t length = 23193;
    std::string fileFormat = "hap";
    std::string profileContent = "";
    VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);
    CodeSignBlock csb;
    bool flag = VerifyCodeSignature::VerifyCodeSign(file, pairResult, csb);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyHap001
 * @tc.desc: verify hap without profile content
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyHap001, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/entry-default-signed-so.hap";
    int64_t offset = 1397151;
    int64_t length = 23193;
    std::string fileFormat = "hap";
    std::string profileContent = "";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyHap002
 * @tc.desc: Test function of VerifyCodeSignatureTest::VerifyHap() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyHap002, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/entry-default-signed-so.hap";
    int64_t offset = 1397151;
    int64_t length = 23193;
    std::string fileFormat = "";
    std::string profileContent = "";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: VerifyHap003
 * @tc.desc: verify hap with invalid offset
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyHap003, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/entry-default-signed-so.hap";
    int64_t offset = 1397151;
    int64_t length = 23194;
    std::string fileFormat = "hap";
    std::string profileContent = "";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyHap004
 * @tc.desc: verify hap without input file
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyHap004, testing::ext::TestSize.Level1)
{
    std::string file = "";
    int64_t offset = 1397151;
    int64_t length = 23193;
    std::string fileFormat = "hap";
    std::string profileContent = "";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyHap005
 * @tc.desc: verify hap with invalid block header
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyHap005, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_invalid_block_header_err.hap";
    int64_t offset = 1397151;
    int64_t length = 23221;
    std::string fileFormat = "hap";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyHap006
 * @tc.desc: hap offset align error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyHap006, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_offset_align_err.hap";
    int64_t offset = 1397151;
    int64_t length = 23193;
    std::string fileFormat = "hap";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyHap007
 * @tc.desc: hap magic number error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyHap007, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_magic_number_err.hap";
    int64_t offset = 1397151;
    int64_t length = 23193;
    std::string fileFormat = "hap";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyHap008
 * @tc.desc: hap segment header error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyHap008, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_segment_header_err.hap";
    int64_t offset = 1397151;
    int64_t length = 23193;
    std::string fileFormat = "hap";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifySingleFile
 * @tc.desc: verify single file with invalid merkle tree offset
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifySingleFile, testing::ext::TestSize.Level1)
{
    std::ifstream input;
    input.open("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    int64_t length = 4096;
    std::vector<int8_t> signature;
    int64_t merkleTreeOffset = 64;
    std::vector<int8_t> inMerkleTreeBytes;
    bool flag = VerifyCodeSignature::VerifySingleFile(input, length, signature, merkleTreeOffset, inMerkleTreeBytes);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyElf001
 * @tc.desc: elf merkle tree error with elf
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyElf001, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_merkle_tree_err.elf";
    int64_t offset = 8216;
    int64_t length = 10516;
    std::string fileFormat = "elf";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyElf(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyElf002
 * @tc.desc: elf merkle tree error with hap
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyElf002, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_merkle_tree_err.elf";
    int64_t offset = 8216;
    int64_t length = 10516;
    std::string fileFormat = "hap";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyElf(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: VerifyElf003
 * @tc.desc: verify elf with invalid input file
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyElf003, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_merkle_tree_err111.elf";
    int64_t offset = 8216;
    int64_t length = 10516;
    std::string fileFormat = "elf";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyElf(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyElf004
 * @tc.desc: verify elf with invalid length
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyElf004, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_merkle_tree_err.elf";
    int64_t offset = 8216;
    int64_t length = 2;
    std::string fileFormat = "elf";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyElf(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyElf005
 * @tc.desc: verify elf with invalid app-identifier
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyElf005, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_merkle_tree_err.elf";
    int64_t offset = 8216;
    int64_t length = 10516;
    std::string fileFormat = "elf";
    std::string profileContent = "{\"version-code\":1,\"app-identifier\":\"111\"}";
    bool flag = VerifyCodeSignature::VerifyElf(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyElf006
 * @tc.desc: hap file type error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyElf006, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_file_type_err.elf";
    int64_t offset = 8216;
    int64_t length = 10516;
    std::string fileFormat = "elf";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyElf(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyElf007
 * @tc.desc: parse elf sign block error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyElf007, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/elf_parse_sign_block_err.elf";
    int64_t offset = 8216;
    int64_t length = 10516;
    std::string fileFormat = "elf";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyElf(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyNativeLib001
 * @tc.desc: verify hap native libs error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyNativeLib001, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_native_libs_err.hap";
    unzFile zFile = unzOpen(file.c_str());
    std::pair<std::string, std::string> pairResult;
    pairResult.first = "111";
    pairResult.second = "222";
    CodeSignBlock csb;
    bool flag = VerifyCodeSignature::VerifyNativeLib(csb, file, zFile, pairResult);
    unzClose(zFile);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyNativeLib002
 * @tc.desc: hap profile is null
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, VerifyNativeLib002, testing::ext::TestSize.Level1)
{
    std::string file = "./codeSigning/hap_profile_is_null_err.hap";
    int64_t offset = 1397151;
    int64_t length = 23221;
    std::string fileFormat = "hap";
    std::string profileContent = "{\"version-code\":1,\"version-name\":\"1.0.0\"}";
    bool flag = VerifyCodeSignature::VerifyHap(file, offset, length, fileFormat, profileContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: CheckCodeSign001
 * @tc.desc: Test function of VerifyCodeSignatureTest::CheckCodeSign() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, CheckCodeSign001, testing::ext::TestSize.Level1)
{
    std::string file = "hap";
    std::vector<OptionalBlock> optionalBlocks;
    OptionalBlock block;
    block.optionalType = HapUtils::HAP_PROPERTY_BLOCK_ID;
    ByteBuffer bf(8);
    bf.PutByte('a');
    block.optionalBlockValue = bf;
    optionalBlocks.push_back(block);

    VerifyHap hapVerify;
    int32_t ret = hapVerify.CheckCodeSign(file, optionalBlocks);

    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: CheckCodeSign002
 * @tc.desc: Test function of VerifyCodeSignatureTest::CheckCodeSign() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, CheckCodeSign002, testing::ext::TestSize.Level1)
{
    std::string file = "test.hap";
    std::vector<OptionalBlock> optionalBlocks;
    OptionalBlock block;
    block.optionalType = HapUtils::HAP_PROPERTY_BLOCK_ID;
    ByteBuffer bf(8);
    bf.PutByte('a');
    block.optionalBlockValue = bf;
    optionalBlocks.push_back(block);

    VerifyHap hapVerify;
    int32_t ret = hapVerify.CheckCodeSign(file, optionalBlocks);

    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: CheckCodeSign003
 * @tc.desc: Test function of VerifyCodeSignatureTest::CheckCodeSign() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyCodeSignatureTest, CheckCodeSign003, testing::ext::TestSize.Level1)
{
    std::string file = "test.hap";
    std::vector<OptionalBlock> optionalBlocks;
    OptionalBlock block;
    block.optionalType = HapUtils::HAP_PROPERTY_BLOCK_ID;
    ByteBuffer bf(16);
    bf.PutByte('a');
    block.optionalBlockValue = bf;
    optionalBlocks.push_back(block);

    VerifyHap hapVerify;
    int32_t ret = hapVerify.CheckCodeSign(file, optionalBlocks);

    EXPECT_EQ(ret, 0);
}
} // namespace SignatureTools
} // namespace OHOS