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

#include "verify_elf.h"
#include "block_data.h"
#include "sign_provider.h"
#include "verify_hap.h"

namespace OHOS {
namespace SignatureTools {

class VerifyElfTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void VerifyElfTest::SetUpTestCase(void)
{
    (void)rename("./elfVerify/elf_verify_data_err_package.txt", "./elfVerify/elf_verify_data_err_package.elf");
    (void)rename("./elfVerify/elf_verify_pkcs7_err_package.txt", "./elfVerify/elf_verify_pkcs7_err_package.elf");
    (void)rename("./elfVerify/elf_check_ownerid_err_package.txt", "./elfVerify/elf_check_ownerid_err_package.elf");
    (void)rename("./elfVerify/elf_signed_package.txt", "./elfVerify/elf_signed_package.elf");
    (void)rename("./elfVerify/elf_unsigned_package.txt", "./elfVerify/elf_unsigned_package.elf");
    (void)rename("./elfVerify/elf_check_file_err_package.txt", "./elfVerify/elf_check_file_err_package.elf");
    (void)rename("./elfVerify/elf_signed_with_profile_json.txt", "./elfVerify/elf_signed_with_profile_json.elf");
}

void VerifyElfTest::TearDownTestCase(void)
{
}

void VerifyElfTest::SetUp()
{
}

void VerifyElfTest::TearDown()
{
}

/**
 * @tc.name: Verify001
 * @tc.desc: Test function of VerifyElfTest::Verify() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify001, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: Verify002
 * @tc.desc: param not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify002, testing::ext::TestSize.Level1)
{
    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(nullptr);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify003
 * @tc.desc: param outCertChain not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify003, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package.elf"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify004
 * @tc.desc: param outProfile not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify004, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify005
 * @tc.desc: param inFile not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify005, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify006
 * @tc.desc: input file not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify006, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package111.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify007
 * @tc.desc: wrong input file
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify007, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_unsigned_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify008
 * @tc.desc: Test function of VerifyElfTest::Verify() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify008, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/readonly.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: Verify009
 * @tc.desc: check owner id error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify009, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_check_ownerid_err_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify010
 * @tc.desc: verify elf data error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify010, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_verify_data_err_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify011
 * @tc.desc: verify pkcs7 error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify011, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_verify_pkcs7_err_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: Verify013
 * @tc.desc: check elf file error
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify013, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_check_file_err_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: Verify014
 * @tc.desc: verify elf with unsigned profile
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, Verify014, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_with_profile_json.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    VerifyElf verifyElf;
    bool flag = verifyElf.Verify(&options);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: CheckParams
 * @tc.desc: Test function of VerifyElfTest::CheckParams() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, CheckParams, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    bool flag = VerifyElf::CheckParams(&options);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: CheckSignFile
 * @tc.desc: Test function of VerifyElfTest::CheckSignFile() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, CheckSignFile, testing::ext::TestSize.Level1)
{
    std::string file = "./elfVerify/elf_signed_package.elf";

    bool flag = VerifyElf::CheckSignFile(file);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: GetSignBlockData
 * @tc.desc: Test function of VerifyElfTest::GetSignBlockData() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, GetSignBlockData, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> bytes = { 1, 1, 1, 1, 1, 1, 1, 1 };
    BlockData blockData(0, 0);

    bool flag = VerifyElf::GetSignBlockData(bytes, blockData, "elf");

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: GetSignBlockInfo
 * @tc.desc: Test function of VerifyElfTest::GetSignBlockInfo() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, GetSignBlockInfo, testing::ext::TestSize.Level1)
{
    std::string file = "./elfVerify/elf_signed_package.elf";
    SignBlockInfo signBlockInfo(false);

    bool flag = VerifyElf::GetSignBlockInfo(file, signBlockInfo, "elf");

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: GetFileDigest
 * @tc.desc: Test function of VerifyElfTest::GetFileDigest() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, GetFileDigest, testing::ext::TestSize.Level1)
{
    std::string file = "./elfVerify/elf_signed_package.elf";
    SignBlockInfo signBlockInfo(false);
    VerifyElf::GetSignBlockInfo(file, signBlockInfo, "elf");
    std::vector<int8_t> fileBytes = { 1, 1, 1, 1, 1, 1, 1, 1 };
    std::vector<int8_t> signatrue = { 1, 1, 1, 1, 1, 1, 1, 1 };

    bool flag = VerifyElf::GetFileDigest(fileBytes, signatrue, signBlockInfo);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: GetRawContent
 * @tc.desc: Test function of VerifyElfTest::GetRawContent() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, GetRawContent, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> contentVec = { 1, 1, 1, 1, 1, 1, 1, 1 };
    std::string rawContent;

    bool flag = VerifyElf::GetRawContent(contentVec, rawContent);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: VerifyP7b
 * @tc.desc: Test function of VerifyElfTest::VerifyP7b() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, VerifyP7b, testing::ext::TestSize.Level1)
{
    std::unordered_map<int8_t, SigningBlock> signBlockMap;
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));
    Pkcs7Context pkcs7Context;
    std::vector<int8_t> profileVec;
    std::string profileJson;

    bool flag = VerifyElf::VerifyP7b(signBlockMap, &options, pkcs7Context, profileVec, profileJson);

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: SignElf001
 * @tc.desc: sign elf with code sign
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignElf001, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package.elf"));
    options.emplace("signCode", std::string("1"));
    options.emplace("inForm", std::string("elf"));

    SignProvider signProvider;
    bool flag = signProvider.SignElf(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: SignElf002
 * @tc.desc: sign elf without code sign
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignElf002, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package.elf"));
    options.emplace("signCode", std::string("0"));

    SignProvider signProvider;
    bool flag = signProvider.SignElf(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: SignElf003
 * @tc.desc: invalid param signCode
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignElf003, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package.elf"));
    options.emplace("signCode", std::string("2"));
    options.emplace("inForm", std::string("elf"));

    SignProvider signProvider;
    bool flag = signProvider.SignElf(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: SignElf004
 * @tc.desc: input file without suffix
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignElf004, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package"));
    options.emplace("signCode", std::string("1"));
    options.emplace("inForm", std::string("elf"));

    SignProvider signProvider;
    bool flag = signProvider.SignElf(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: SignElf005
 * @tc.desc: input file not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignElf005, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package111.elf"));
    options.emplace("signCode", std::string("1"));
    options.emplace("inForm", std::string("elf"));

    SignProvider signProvider;
    bool flag = signProvider.SignElf(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: SignElf006
 * @tc.desc: pem file not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignElf006, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA512withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package.elf"));
    options.emplace("signCode", std::string("1"));
    options.emplace("inForm", std::string("elf"));

    SignProvider signProvider;
    bool flag = signProvider.SignElf(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: SignElf007
 * @tc.desc: invalid compatible version
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignElf007, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package.elf"));
    options.emplace("signCode", std::string("1"));
    options.emplace("inForm", std::string("elf"));
    options.emplace("compatibleVersion", std::string("a"));

    SignProvider signProvider;
    bool flag = signProvider.SignElf(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: SignElf008
 * @tc.desc: p7b file not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignElf008, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile111.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package.elf"));
    options.emplace("signCode", std::string("1"));
    options.emplace("inForm", std::string("elf"));

    SignProvider signProvider;
    bool flag = signProvider.SignElf(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: SignBin001
 * @tc.desc: invalid alg SHA512withECDSA
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, SignBin001, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./hapSign/entry-default-signed.elf"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./hapSign/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA512withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./hapSign/ohtest.jks"));
    options.emplace("appCertFile", std::string("./hapSign/app-release1.pem"));
    options.emplace("inFile", std::string("./elfVerify/elf_unsigned_package.elf"));
    options.emplace("inForm", std::string("bin"));

    SignProvider signProvider;
    bool flag = signProvider.SignBin(&options);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: WriteVerifyOutput001
 * @tc.desc: cer file not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, WriteVerifyOutput001, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package.elf"));
    options.emplace(Options::OUT_PROFILE, std::string("./elfVerify/xx.p7b"));

    SignBlockInfo signBlockInfo(false);
    VerifyElf::GetSignBlockInfo("./elfVerify/elf_signed_package.elf", signBlockInfo, "elf");
    std::unordered_map<int8_t, SigningBlock> signBlockMap = signBlockInfo.GetSignBlockMap();
    SigningBlock profileSign = signBlockMap.find(2)->second;
    std::vector<int8_t> profileByte = profileSign.GetValue();
    std::vector<int8_t> profileVec;
    Pkcs7Context pkcs7Context;
    VerifyHap hapVerifyV2;
    std::unique_ptr<ByteBuffer> profileBuffer =
        std::make_unique<ByteBuffer>((char*)profileByte.data(), profileByte.size());
    hapVerifyV2.VerifyAppPkcs7(pkcs7Context, *profileBuffer);
    int32_t flag = hapVerifyV2.WriteVerifyOutput(pkcs7Context, profileVec, &options);

    EXPECT_NE(flag, 0);
}

/**
 * @tc.name: WriteVerifyOutput002
 * @tc.desc: p7b file not exist
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(VerifyElfTest, WriteVerifyOutput002, testing::ext::TestSize.Level1)
{
    Options options;
    options.emplace(Options::IN_FILE, std::string("./elfVerify/elf_signed_package.elf"));
    options.emplace(Options::OUT_CERT_CHAIN, std::string("./elfVerify/xx.cer"));

    SignBlockInfo signBlockInfo(false);
    VerifyElf::GetSignBlockInfo("./elfVerify/elf_signed_package.elf", signBlockInfo, "elf");
    std::unordered_map<int8_t, SigningBlock> signBlockMap = signBlockInfo.GetSignBlockMap();
    SigningBlock profileSign = signBlockMap.find(2)->second;
    std::vector<int8_t> profileByte = profileSign.GetValue();
    std::vector<int8_t> profileVec;
    Pkcs7Context pkcs7Context;
    VerifyHap hapVerifyV2;
    std::unique_ptr<ByteBuffer> profileBuffer =
        std::make_unique<ByteBuffer>((char*)profileByte.data(), profileByte.size());
    hapVerifyV2.VerifyAppPkcs7(pkcs7Context, *profileBuffer);
    int32_t flag = hapVerifyV2.WriteVerifyOutput(pkcs7Context, profileVec, &options);

    EXPECT_NE(flag, 0);
}
} // namespace SignatureTools
} // namespace OHOS