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

#include "hap_verify_test.h"
#include <string>
#include <gtest/gtest.h>
#include <filesystem>
#include "profile_info.h"
#include "verify_hap.h"
#include "test_hap_file_data.h"
#include "hap_cert_verify_openssl_utils_test.h"
#include "test_const.h"
#include "test_hap_file_data.h"
#include "file_utils.h"
#include "random_access_file.h"
#include "signature_tools_log.h"
#include "verify_hap.h"
#include "hap_signer_block_utils.h"
#include "openssl/pem.h"
#include "options.h"
#include "signature_tools_errno.h"
#include "hap_utils.h"
#include "cert_dn_utils.h"
#include "signer_config.h"
#include "digest_common.h"

using namespace testing::ext;

namespace OHOS {
namespace SignatureTools {
const std::string ERROR_CERTIFICATE = "errorCertificate";
const std::string TEST_CERTIFICATE = "-----BEGIN CERTIFICATE-----\n\
MIICMzCCAbegAwIBAgIEaOC/zDAMBggqhkjOPQQDAwUAMGMxCzAJBgNVBAYTAkNO\n\
MRQwEgYDVQQKEwtPcGVuSGFybW9ueTEZMBcGA1UECxMQT3Blbkhhcm1vbnkgVGVh\n\
bTEjMCEGA1UEAxMaT3Blbkhhcm1vbnkgQXBwbGljYXRpb24gQ0EwHhcNMjEwMjAy\n\
MTIxOTMxWhcNNDkxMjMxMTIxOTMxWjBoMQswCQYDVQQGEwJDTjEUMBIGA1UEChML\n\
T3Blbkhhcm1vbnkxGTAXBgNVBAsTEE9wZW5IYXJtb255IFRlYW0xKDAmBgNVBAMT\n\
H09wZW5IYXJtb255IEFwcGxpY2F0aW9uIFJlbGVhc2UwWTATBgcqhkjOPQIBBggq\n\
hkjOPQMBBwNCAATbYOCQQpW5fdkYHN45v0X3AHax12jPBdEDosFRIZ1eXmxOYzSG\n\
JwMfsHhUU90E8lI0TXYZnNmgM1sovubeQqATo1IwUDAfBgNVHSMEGDAWgBTbhrci\n\
FtULoUu33SV7ufEFfaItRzAOBgNVHQ8BAf8EBAMCB4AwHQYDVR0OBBYEFPtxruhl\n\
cRBQsJdwcZqLu9oNUVgaMAwGCCqGSM49BAMDBQADaAAwZQIxAJta0PQ2p4DIu/ps\n\
LMdLCDgQ5UH1l0B4PGhBlMgdi2zf8nk9spazEQI/0XNwpft8QAIwHSuA2WelVi/o\n\
zAlF08DnbJrOOtOnQq5wHOPlDYB4OtUzOYJk9scotrEnJxJzGsh/\n\
-----END CERTIFICATE-----\n";

class VerifyHapTest : public testing::Test {
public:
    static void SetUpTestCase(void);

    static void TearDownTestCase(void);

    void SetUp();

    void TearDown();
};

void GenUnvaildVerfyHap(const std::string& path)
{
    std::ofstream outfile(path);
    if (!outfile) {
        SIGNATURE_TOOLS_LOGE("Unable to open file: %s", path.c_str());
        return;
    }
    outfile << "Hello, this is a Unvaild verfy Hap.\n";
    outfile.flush();
    outfile.close();
    return;
}

void VerifyHapTest::SetUpTestCase(void)
{
    GenUnvaildVerfyHap("./hapVerify/unvaild.app");
    GenUnvaildVerfyHap("./hapVerify/unvaild.hap");
    GenUnvaildVerfyHap("./hapVerify/unvaild.hqf");
    GenUnvaildVerfyHap("./hapVerify/unvaild.hsp");
    GenUnvaildVerfyHap("./hapVerify/unvaild.txt");
    (void)rename("./hapVerify/packet_diget_algo_error.txt", "./hapVerify/packet_diget_algo_error.hap");
    (void)rename("./hapVerify/packet_hap_integrity_error.txt", "./hapVerify/packet_hap_integrity_error.hap");
    (void)rename("./hapVerify/packet_verify_app_error.txt", "./hapVerify/packet_verify_app_error.hap");
    (void)rename("./hapVerify/packet_get_certchain_error.txt", "./hapVerify/packet_get_certchain_error.hap");
    (void)rename("./hapVerify/packet_parse_profile_error.txt", "./hapVerify/packet_parse_profile_error.hap");
    (void)rename("./hapVerify/phone-default-signed.txt", "./hapVerify/phone-default-signed.hap");
    sync();
}

void VerifyHapTest::TearDownTestCase(void)
{
}

void VerifyHapTest::SetUp()
{
}

void VerifyHapTest::TearDown()
{
}

/**
 * @tc.name: CheckFilePathTest001
 * @tc.desc: The static function test whether input is a valid filepath;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, CheckFilePathTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. input an too long filepath.
     * @tc.expected: step1. the return will be false.
     */
    VerifyHap v2;
    std::string filePath = HAP_FILE_ECC_SIGN_BASE64;
    std::string standardFilePath;
    ASSERT_FALSE(v2.CheckFilePath(filePath, standardFilePath));
}

/**
 * @tc.name: GetDigestAndAlgorithmTest001
 * @tc.desc: The static function will return result of GetDigestAndAlgorithm;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, GetDigestAndAlgorithmTest001, TestSize.Level1)
{
    /*
     * @tc.steps: step1. input an error pkcs7 content.
     * @tc.expected: step1. the return will be false.
     */
    VerifyHap v2;
    Pkcs7Context digest;
    digest.content.SetCapacity(TEST_FILE_BLOCK_LENGTH);
    ASSERT_FALSE(v2.GetDigestAndAlgorithm(digest));
}

/**
 * @tc.name: Verify001
 * @tc.desc: The static function will return verify result;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify001, TestSize.Level0)
{
    /*
        * @tc.steps: step1. input a invalid path to function of HapVerify.
        * @tc.expected: step1. the return will be FILE_PATH_INVALID.
        */
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";

    std::string errorFile = "./hapVerify/signed_test.app";

    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    ASSERT_TRUE(resultCode != OHOS::SignatureTools::RET_OK);
    std::ofstream appFile;
    appFile.open(errorFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(appFile.is_open());
    appFile.close();
    resultCode = verify.Verify(errorFile, &options);
    ASSERT_TRUE(resultCode != OHOS::SignatureTools::RET_OK);
}

/**
 * @tc.name: HapVerifyOsApp001
 * @tc.desc: The static function will return verify result of signed file;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, HapVerifyOsApp001, TestSize.Level0)
{
    /*
        * @tc.steps: step1. input a signed file to verify.
        * @tc.expected: step1. the return will be RET_OK.
        */

    std::string filePath = "./hapVerify/phone-default-signed.hap";
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";

    VerifyHap verify;
    int32_t ret = verify.Verify(filePath, &options);
    ASSERT_EQ(ret, OHOS::SignatureTools::RET_OK);
}

/**
 * @tc.name: Verify002
 * @tc.desc: The static function will return verify result of signed file;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify002, TestSize.Level0)
{
    std::string filePath = "./hapVerify/phone-default-signed.hap";
    std::string errorfilePath = "./hapVerify/phone-default-signed_error.hap";
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";

    OHOS::SignatureTools::ByteBuffer byteBuffer;
    std::ifstream hapFile;
    hapFile.open(filePath, std::ifstream::binary);
    ASSERT_TRUE(hapFile.is_open());
    std::stringstream hapFileStr;
    hapFileStr << hapFile.rdbuf();
    size_t strSize = hapFileStr.str().size();
    byteBuffer.SetCapacity(strSize);
    byteBuffer.PutData(hapFileStr.str().c_str(), hapFileStr.str().size());
    hapFile.close();
    /*
        * @tc.steps: step1. input a signed file to verify.
        * @tc.expected: step1. the return will be RET_OK.
        */
    VerifyHap verify;
    ASSERT_TRUE(verify.Verify(filePath, &options) == OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step3. change comment data.
        * @tc.expected: step3. the return will be VERIFY_INTEGRITY_FAIL.
        */
    OHOS::SignatureTools::ByteBuffer errorCommentFile = byteBuffer;
    char tmp = TEST_HAPBYTEBUFFER_CHAR_DATA;
    errorCommentFile.PutByte(0, tmp);
    std::ofstream errorFile;
    errorFile.open(errorfilePath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(errorFile.is_open());
    errorFile.seekp(0, std::ios_base::beg);
    errorFile.write(errorCommentFile.GetBufferPtr(), errorCommentFile.GetCapacity());
    errorFile.close();
    ASSERT_NE(verify.Verify(errorfilePath, &options), OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step4. change profile pkcs7 data.
        * @tc.expected: step4. the return will be APP_SOURCE_NOT_TRUSTED.
        */
    errorCommentFile.PutByte(TEST_PFOFILE_PKCS7_DATA_INDEX, tmp);
    errorFile.open(errorfilePath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(errorFile.is_open());
    errorFile.seekp(0, std::ios_base::beg);
    errorFile.write(errorCommentFile.GetBufferPtr(), errorCommentFile.GetCapacity());
    errorFile.close();
    ASSERT_NE(verify.Verify(errorfilePath, &options), OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step5. change app pkcs7 data.
        * @tc.expected: step5. the return will be VERIFY_APP_PKCS7_FAIL.
        */
    errorCommentFile.PutByte(TEST_APP_PKCS7_DATA_INDEX, tmp);
    errorFile.open(errorfilePath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(errorFile.is_open());
    errorFile.seekp(0, std::ios_base::beg);
    errorFile.write(errorCommentFile.GetBufferPtr(), errorCommentFile.GetCapacity());
    errorFile.close();
    ASSERT_NE(verify.Verify(errorfilePath, &options), OHOS::SignatureTools::RET_OK);
}

/**
 * @tc.name: Verify003
 * @tc.desc: The static function will return verify result of signed file;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify003, TestSize.Level0)
{
    /*
        * @tc.steps: step1. input a invalid path to function of HapVerify.
        * @tc.expected: step1. the return will be FILE_PATH_INVALID.
        */
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";

    VerifyHap verify;
    /*
        * @tc.steps: step2. create a hapfile and run HapVerify.
        * @tc.expected: step2. the return will be SIGNATURE_NOT_FOUND.
        */
    std::string rightFile = "./hapVerify/signed.hap";
    std::ofstream hapFile;
    hapFile.open(rightFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(hapFile.is_open());
    hapFile.seekp(0, std::ios_base::beg);
    hapFile.write(MINIHAPFILE, TEST_MINI_HAP_FILE_LENGTH);
    hapFile.close();
    int32_t resultCode = verify.Verify(rightFile, &options);
    ASSERT_NE(resultCode, OHOS::SignatureTools::RET_OK);

    /*
        * @tc.steps: step3. create an error hapfile and run HapVerify.
        * @tc.expected: step3. the return will be SIGNATURE_NOT_FOUND.
        */
    std::string rightFile1 = "./hapVerify/signed1.hap";
    std::ofstream hapFile1;
    hapFile1.open(rightFile1.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(hapFile1.is_open());
    hapFile1.seekp(0, std::ios_base::beg);
    hapFile1.write(MINIHAPFILE, sizeof(MINIHAPFILE));
    hapFile1.seekp(TEST_MINI_HAP_FILE_LENGTH - sizeof(short), std::ios_base::beg);
    hapFile1.close();
    resultCode = verify.Verify(rightFile1, &options);
    ASSERT_NE(resultCode, OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step4. use an empty file to run HapVerify.
        * @tc.expected: step4. the return will be SIGNATURE_NOT_FOUND.
        */
    std::string invalidFile = "./hapVerify/signed2.hap";
    std::ofstream hapFile2;
    hapFile2.open(invalidFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(hapFile2.is_open());
    hapFile2.close();
    resultCode = verify.Verify(invalidFile, &options);
    ASSERT_NE(resultCode, OHOS::SignatureTools::RET_OK);
}

/**
 * @tc.name: Verify004
 * @tc.desc: This function tests failure for interface HapOutPutPkcs7 due to PKCS7 error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify004, TestSize.Level0)
{
    std::string outPutPath = "./test.log";
    PKCS7* p7 = nullptr;
    VerifyHap verify;
    bool ret = verify.HapOutPutPkcs7(p7, outPutPath);
    EXPECT_EQ(ret, false);
}

/**
* @tc.name: Verify005
* @tc.desc: This function tests failure for interface GetProfileContent due to profile error
* @tc.type: FUNC
*/
HWTEST_F(VerifyHapTest, Verify005, TestSize.Level0)
{
    std::string profile = "";
    std::string ret = "111";
    VerifyHap verify;
    verify.setIsPrintCert(true);
    int rets = verify.GetProfileContent(profile, ret);
    EXPECT_EQ(rets, -1);
}

/**
 * @tc.name: Verify006
 * @tc.desc: This function tests failure for interface GetProfileContent due to profile error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify006, TestSize.Level0)
{
    VerifyHap verify;
    std::string profile = "{version-name: 1.0.0,version-code: 1,uuid: fe686e1b-3770-4824-a938-961b140a7c98}";
    std::string ret = "111";
    int rets = verify.GetProfileContent(profile, ret);
    EXPECT_EQ(rets, -1);
}

/**
 * @tc.name: Verify007
 * @tc.desc: This function tests failure for interface VerifyAppPkcs7 due to context error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify007, TestSize.Level0)
{
    VerifyHap verify;
    Pkcs7Context pkcs7Context;
    ByteBuffer hapSignatureBlock;
    bool ret = verify.VerifyAppPkcs7(pkcs7Context, hapSignatureBlock);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: Verify008
 * @tc.desc: The static function will return verify result;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify008, TestSize.Level0)
{
    /*
        * @tc.steps: step1. input a invalid path to function of HapVerify.
        * @tc.expected: step1. the return will be FILE_PATH_INVALID.
        */
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/signed_test.app";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    ASSERT_TRUE(resultCode != OHOS::SignatureTools::RET_OK);
    std::ofstream appFile;
    appFile.open(errorFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(appFile.is_open());
    appFile.close();
    resultCode = verify.Verify(errorFile, &options);
    ASSERT_TRUE(resultCode != OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step2. create a hapfile and run HapVerify.
        * @tc.expected: step2. the return will be SIGNATURE_NOT_FOUND.
        */
    std::string rightFile = "./hapVerify/signed.hap";
    std::ofstream hapFile;
    hapFile.open(rightFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(hapFile.is_open());
    hapFile.seekp(0, std::ios_base::beg);
    hapFile.write(MINIHAPFILE, TEST_MINI_HAP_FILE_LENGTH);
    hapFile.close();
    resultCode = verify.Verify(rightFile, &options);
    ASSERT_TRUE(resultCode != OHOS::SignatureTools::RET_OK);

    /*
        * @tc.steps: step3. create an error hapfile and run HapVerify.
        * @tc.expected: step3. the return will be SIGNATURE_NOT_FOUND.
        */
    std::string rightFile1 = "./hapVerify/signed1.hap";
    std::ofstream hapFile1;
    hapFile1.open(rightFile1.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(hapFile1.is_open());
    hapFile1.seekp(0, std::ios_base::beg);
    hapFile1.write(MINIHAPFILE, sizeof(MINIHAPFILE));
    hapFile1.seekp(TEST_MINI_HAP_FILE_LENGTH - sizeof(short), std::ios_base::beg);
    hapFile1.close();
    resultCode = verify.Verify(rightFile1, &options);
    ASSERT_TRUE(resultCode != OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step4. use an empty file to run HapVerify.
        * @tc.expected: step4. the return will be SIGNATURE_NOT_FOUND.
        */
    std::string invalidFile = "./hapVerify/signed2.hap";
    std::ofstream hapFile2;
    hapFile2.open(invalidFile.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(hapFile2.is_open());
    hapFile2.close();
    resultCode = verify.Verify(invalidFile, &options);
    ASSERT_TRUE(resultCode != OHOS::SignatureTools::RET_OK);
}

/**
 * @tc.name: Verify009
 * @tc.desc: The static function will return verify result of signed file;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify009, TestSize.Level0)
{
    /*
        * @tc.steps: step1. input a signed file to verify.
        * @tc.expected: step1. the return will be RET_OK.
        */
    std::string filePath = "./hapVerify/phone-default-signed.hap";
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";

    VerifyHap verify;
    int32_t ret = verify.Verify(filePath, &options);
    ASSERT_EQ(ret, OHOS::SignatureTools::RET_OK);
}

/**
 * @tc.name: Verify010
 * @tc.desc: The static function will return verify result of signed file;
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify010, TestSize.Level0)
{
    std::string filePath = "./hapVerify/phone-default-signed.hap";
    std::string errorfilePath = "./hapVerify/phone-default-signed_error.hap";
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";

    OHOS::SignatureTools::ByteBuffer byteBuffer;
    std::ifstream hapFile;
    hapFile.open(filePath, std::ifstream::binary);
    ASSERT_TRUE(hapFile.is_open());
    std::stringstream hapFileStr;
    hapFileStr << hapFile.rdbuf();
    size_t strSize = hapFileStr.str().size();
    byteBuffer.SetCapacity(strSize);
    byteBuffer.PutData(hapFileStr.str().c_str(), hapFileStr.str().size());
    hapFile.close();
    /*
        * @tc.steps: step1. input a signed file to verify.
        * @tc.expected: step1. the return will be RET_OK.
        */
    VerifyHap verify;
    ASSERT_TRUE(verify.Verify(filePath, &options) == OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step3. change comment data.
        * @tc.expected: step3. the return will be VERIFY_INTEGRITY_FAIL.
        */
    OHOS::SignatureTools::ByteBuffer errorCommentFile = byteBuffer;
    char tmp = TEST_HAPBYTEBUFFER_CHAR_DATA;
    errorCommentFile.PutByte(0, tmp);
    std::ofstream errorFile;
    errorFile.open(errorfilePath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(errorFile.is_open());
    errorFile.seekp(0, std::ios_base::beg);
    errorFile.write(errorCommentFile.GetBufferPtr(), errorCommentFile.GetCapacity());
    errorFile.close();
    ASSERT_NE(verify.Verify(errorfilePath, &options), OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step4. change profile pkcs7 data.
        * @tc.expected: step4. the return will be APP_SOURCE_NOT_TRUSTED.
        */
    errorCommentFile.PutByte(TEST_PFOFILE_PKCS7_DATA_INDEX, tmp);
    errorFile.open(errorfilePath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(errorFile.is_open());
    errorFile.seekp(0, std::ios_base::beg);
    errorFile.write(errorCommentFile.GetBufferPtr(), errorCommentFile.GetCapacity());
    errorFile.close();
    ASSERT_NE(verify.Verify(errorfilePath, &options), OHOS::SignatureTools::RET_OK);
    /*
        * @tc.steps: step5. change app pkcs7 data.
        * @tc.expected: step5. the return will be VERIFY_APP_PKCS7_FAIL.
        */
    errorCommentFile.PutByte(TEST_APP_PKCS7_DATA_INDEX, tmp);
    errorFile.open(errorfilePath.c_str(), std::ios::binary | std::ios::out | std::ios::trunc);
    ASSERT_TRUE(errorFile.is_open());
    errorFile.seekp(0, std::ios_base::beg);
    errorFile.write(errorCommentFile.GetBufferPtr(), errorCommentFile.GetCapacity());
    errorFile.close();
    ASSERT_NE(verify.Verify(errorfilePath, &options), OHOS::SignatureTools::RET_OK);
}

/**
 * @tc.name: Verify011
 * @tc.desc: This function tests failure for interface HapOutPutPkcs7 due to PKCS7 error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify011, TestSize.Level0)
{
    std::string outPutPath = "./test.log";
    PKCS7* p7 = nullptr;
    VerifyHap verify;
    bool ret = verify.HapOutPutPkcs7(p7, outPutPath);
    EXPECT_EQ(ret, false);
}

/**
* @tc.name: Verify012
* @tc.desc: This function tests failure for interface GetProfileContent due to profile error
* @tc.type: FUNC
*/
HWTEST_F(VerifyHapTest, Verify012, TestSize.Level0)
{
    std::string profile = "";
    std::string ret = "111";
    VerifyHap verify;
    int rets = verify.GetProfileContent(profile, ret);
    EXPECT_EQ(rets, -1);
}

/**
 * @tc.name: Verify013
 * @tc.desc: This function tests failure for interface GetProfileContent due to profile error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify013, TestSize.Level0)
{
    VerifyHap verify;
    std::string profile = "{version-name: 1.0.0,version-code: 1,uuid: fe686e1b-3770-4824-a938-961b140a7c98}";
    std::string ret = "111";
    int rets = verify.GetProfileContent(profile, ret);
    EXPECT_EQ(rets, -1);
}

/**
 * @tc.name: Verify014
 * @tc.desc: This function tests failure for interface VerifyAppPkcs7 due to context error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, Verify014, TestSize.Level0)
{
    VerifyHap verify;
    Pkcs7Context pkcs7Context;
    ByteBuffer hapSignatureBlock;
    bool ret = verify.VerifyAppPkcs7(pkcs7Context, hapSignatureBlock);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: VerifyHapError001
 * @tc.desc: This function tests failure for interface Verify due to unvaild file
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError001, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/unvaild.hqf";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError002
 * @tc.desc: This function tests failure for interface Verify due to unvaild file
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError002, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/unvaild.hap";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError003
 * @tc.desc: This function tests failure for interface Verify due to unvaild file
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError003, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/unvaild.app";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError004
 * @tc.desc: This function tests failure for interface Verify due to unvaild file
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError004, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/unvaild.hsp";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError005
 * @tc.desc: This function tests failure for interface Verify due to unvaild file
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError005, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";

    std::string errorFile = "./hapVerify/unvaild.txt";

    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError006
 * @tc.desc: This function tests failure for interface Verify due to parameter outCertChain
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError006, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify_nohave/certchain.pem";
    options[Options::OUT_PROFILE] = "./hapVerify_nohava/profile.p7b";
    std::string errorFile = "./hapVerify/phone-default-signed.hap";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError007
 * @tc.desc: This function tests failure for interface Verify due to parameter outCertChain
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError007, TestSize.Level0)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string outCertChain = "./hapVerify_nohave/certchain.pem";
    std::string outProfile = "./hapVerify/profile.p7b";
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    std::string errorFile = "./hapVerify/phone-default-signed.hap";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, params.get());
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError008
 * @tc.desc: This function tests failure for interface Verify due to parameter outProfile
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError008, TestSize.Level0)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string outCertChain = "./hapVerify/certchain.pem";
    std::string outProfile = "./hapVerify_nohave/profile.p7b";
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    std::string errorFile = "./hapVerify/phone-default-signed.hap";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, params.get());
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError009
 * @tc.desc: This function tests failure for interface GetDigestAndAlgorithm due to content error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError009, TestSize.Level0)
{
    VerifyHap verify;
    Pkcs7Context digest;
    ByteBuffer content("1", 1);
    digest.content = content;
    bool ret = verify.GetDigestAndAlgorithm(digest);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: VerifyHapError010
 * @tc.desc: This function tests failure for interface GetDigestAndAlgorithm due to content error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError010, TestSize.Level0)
{
    VerifyHap verify;
    Pkcs7Context digest;
    ByteBuffer content("12345678912345", 14);
    digest.content = content;
    bool ret = verify.GetDigestAndAlgorithm(digest);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: VerifyHapError011
 * @tc.desc: This function tests failure for interface GetDigestAndAlgorithm due to content error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError011, TestSize.Level0)
{
    VerifyHap verify;
    Pkcs7Context digest;
    ByteBuffer content("123456789123456789123456789", 27);
    digest.content = content;
    bool ret = verify.GetDigestAndAlgorithm(digest);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: VerifyHapError012
 * @tc.desc: This function tests failure for interface GetDigestAndAlgorithm due to content error
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError012, TestSize.Level0)
{
    VerifyHap verify;
    Pkcs7Context digest;
    ByteBuffer content("123456789123456789123456789", 19);
    digest.content = content;
    bool ret = verify.GetDigestAndAlgorithm(digest);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: VerifyHapError013
 * @tc.desc: This function tests failure for interface Verify due to diget algo
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError013, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.cer";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";

    std::string errorFile = "./hapVerify/packet_diget_algo_error.hap";

    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError014
 * @tc.desc: This function tests failure for interface Verify due to hap integrity
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError014, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.cer";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/packet_hap_integrity_error.hap";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError015
 * @tc.desc: This function tests failure for interface Verify due to verify app
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError015, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.cer";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/packet_verify_app_error.hap";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError016
 * @tc.desc: This function tests failure for interface Verify due to get certchain
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError016, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.cer";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/packet_get_certchain_error.hap";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_NE(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError017
 * @tc.desc: This function tests failure for interface Verify due to parse profle
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError017, TestSize.Level0)
{
    Options options;
    options[Options::OUT_CERT_CHAIN] = "./hapVerify/certchain.cer";
    options[Options::OUT_PROFILE] = "./hapVerify/profile.p7b";
    std::string errorFile = "./hapVerify/packet_parse_profile_error.hap";
    VerifyHap verify;
    int32_t resultCode = verify.Verify(errorFile, &options);
    EXPECT_EQ(resultCode, 0);
}

/**
 * @tc.name: VerifyHapError018
 * @tc.desc: This function tests two scenarios of success and failure for interface outputOptionalBlocks
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError018, TestSize.Level0)
{
    std::string outputProfileFile = "./hapVerify/outputProfileFile.txt";
    std::string outputProofFile = "./hapVerify/outputProfileFile.txt";
    std::string outputPropertyFile = "./hapVerify/outputProfileFile.txt";
    std::string errorfile = "";
    ByteBuffer bf1("123456789", 9);
    ByteBuffer bf2("123456789", 9);
    ByteBuffer bf3("123456789", 9);
    ByteBuffer bf4("123456789", 9);
    std::vector<OptionalBlock> optionBlocks;
    optionBlocks.push_back({HapUtils::HAP_PROFILE_BLOCK_ID, bf1});
    optionBlocks.push_back({HapUtils::HAP_PROPERTY_BLOCK_ID, bf2});
    optionBlocks.push_back({HapUtils::HAP_PROOF_OF_ROTATION_BLOCK_ID, bf3});
    VerifyHap verify;
    bool resultCode = verify.outputOptionalBlocks(outputProfileFile, outputProofFile, outputPropertyFile, optionBlocks);
    EXPECT_EQ(resultCode, true);
    
    optionBlocks.push_back({HapUtils::HAP_CODE_SIGN_BLOCK_ID, bf4});
    resultCode = verify.outputOptionalBlocks(outputProfileFile, outputProofFile, outputPropertyFile, optionBlocks);
    EXPECT_EQ(resultCode, false);
    resultCode = verify.outputOptionalBlocks(errorfile, outputProofFile, outputPropertyFile, optionBlocks);
    EXPECT_EQ(resultCode, false);
    resultCode = verify.outputOptionalBlocks(outputProfileFile, errorfile, outputPropertyFile, optionBlocks);
    EXPECT_EQ(resultCode, false);
    resultCode = verify.outputOptionalBlocks(outputProfileFile, outputProofFile, errorfile, optionBlocks);
    EXPECT_EQ(resultCode, false);
}

/**
 * @tc.name: VerifyHapError019
 * @tc.desc: This function tests two scenarios of success and failure for interface GetDigestAlgorithmString
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError019, TestSize.Level0)
{
    int32_t signAlgorithm = ALGORITHM_SHA256_WITH_ECDSA;
    std::string ret = DigestCommon::GetDigestAlgorithmString(signAlgorithm);

    signAlgorithm = ALGORITHM_SHA384_WITH_ECDSA;
    ret = DigestCommon::GetDigestAlgorithmString(signAlgorithm);
    signAlgorithm = ALGORITHM_SHA512_WITH_ECDSA;
    ret = DigestCommon::GetDigestAlgorithmString(signAlgorithm);

    signAlgorithm = ALGORITHM_SHA512_WITH_DSA;
    ret = DigestCommon::GetDigestAlgorithmString(signAlgorithm);

    EXPECT_EQ(ret, "");
}

/**
 * @tc.name: VerifyHapError021
 * @tc.desc: This function tests failure for interface GetCertsChain due to certsChain and certVisitSign are empty
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError021, TestSize.Level0)
{
    CertSign certVisitSign;
    CertChain certsChain;
    STACK_OF(X509)* certs = sk_X509_new_null();
    sk_X509_push(certs, nullptr);
    CertSign certVisitSign1;
    VerifyCertOpensslUtils::GenerateCertSignFromCertStack(certs, certVisitSign1);
    sk_X509_pop_free(certs, X509_free);
    bool ret = VerifyCertOpensslUtils::GetCertsChain(certsChain, certVisitSign);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: VerifyHapError022
 * @tc.desc: This function tests failure for interface GetCertsChain due to certVisitSign is empty
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError022, TestSize.Level0)
{
    CertSign certVisitSign;
    CertChain certsChain;
    certsChain.push_back(nullptr);
    bool ret = VerifyCertOpensslUtils::GetCertsChain(certsChain, certVisitSign);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: VerifyHapError023
 * @tc.desc: This function tests failure for interface GetCrlBySignedCertIssuer due to crls is empty
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError023, TestSize.Level0)
{
    STACK_OF(X509_CRL)* crls = sk_X509_CRL_new_null();
    X509* cert = X509_new();
    X509_CRL* ret = VerifyCertOpensslUtils::GetCrlBySignedCertIssuer(crls, cert);
    EXPECT_EQ(ret, nullptr);
    sk_X509_CRL_pop_free(crls, X509_CRL_free);
    X509_free(cert);
}

/**
 * @tc.name: VerifyHapError024
 * @tc.desc: This function tests failure for interface GetCrlBySignedCertIssuer due to crls all empty element
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError024, TestSize.Level0)
{
    STACK_OF(X509_CRL)* crls = sk_X509_CRL_new_null();
    sk_X509_CRL_push(crls, nullptr);
    X509* cert = X509_new();
    X509_CRL* ret = VerifyCertOpensslUtils::GetCrlBySignedCertIssuer(crls, cert);
    EXPECT_EQ(ret, nullptr);
    sk_X509_CRL_pop_free(crls, X509_CRL_free);
    X509_free(cert);
}

/**
 * @tc.name: VerifyHapError025
 * @tc.desc: This function tests success for interface GetCrlBySignedCertIssuer
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError025, TestSize.Level0)
{
    STACK_OF(X509_CRL)* crls = sk_X509_CRL_new_null();
    sk_X509_CRL_push(crls, X509_CRL_new());
    X509* cert = X509_new();
    X509_CRL* ret = VerifyCertOpensslUtils::GetCrlBySignedCertIssuer(crls, cert);
    EXPECT_NE(ret, nullptr);
    sk_X509_CRL_pop_free(crls, X509_CRL_free);
    X509_free(cert);
}

/**
 * @tc.name: VerifyHapError026
 * @tc.desc: This function tests failed for interface GetCrlBySignedCertIssuer dut issuer name different
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError026, TestSize.Level0)
{
    STACK_OF(X509_CRL)* crls = sk_X509_CRL_new_null();
    sk_X509_CRL_push(crls, X509_CRL_new());
    X509* cert = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* subName = BuildDN(subjectname, issuerReq);
    X509_set_issuer_name(cert, subName);
    X509_CRL* ret = VerifyCertOpensslUtils::GetCrlBySignedCertIssuer(crls, cert);
    EXPECT_EQ(ret, nullptr);
    sk_X509_CRL_pop_free(crls, X509_CRL_free);
    X509_free(cert);
}

/**
 * @tc.name: VerifyHapError027
 * @tc.desc: This function tests failed for interface VerifyCrl dut certsChain no have public key
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError027, TestSize.Level0)
{
    STACK_OF(X509_CRL)* crls = sk_X509_CRL_new_null();
    sk_X509_CRL_push(crls, X509_CRL_new());
    X509* cert = X509_new();
    X509* cert1 = X509_new();
    CertChain certsChain;
    certsChain.push_back(cert);
    certsChain.push_back(cert1);
    Pkcs7Context pkcs7Context;
    bool ret = VerifyCertOpensslUtils::VerifyCrl(certsChain, crls, pkcs7Context);
    EXPECT_EQ(ret, false);
    SignerConfig config;
    config.SetX509CRLs(crls);
    X509_free(cert);
    X509_free(cert1);
}

/**
 * @tc.name: VerifyHapError028
 * @tc.desc: This function tests failed for interface outputOptionalBlocks dut  errorfile is not exit
 * @tc.type: FUNC
 */
HWTEST_F(VerifyHapTest, VerifyHapError028, TestSize.Level0)
{
    std::string outputProfileFile = "outputProfileFile.txt";
    std::string outputProofFile = "outputProfileFile.txt";
    std::string outputPropertyFile = "outputProfileFile.txt";
    std::string errorfile = "./nohave/path.txt";
    ByteBuffer bf1("123456789", 9);
    ByteBuffer bf2("123456789", 9);
    ByteBuffer bf3("123456789", 9);
    std::vector<OptionalBlock> optionBlocks;
    optionBlocks.push_back({ HapUtils::HAP_PROFILE_BLOCK_ID, bf1 });
    optionBlocks.push_back({ HapUtils::HAP_PROPERTY_BLOCK_ID, bf2 });
    optionBlocks.push_back({ HapUtils::HAP_PROOF_OF_ROTATION_BLOCK_ID, bf3 });
    ProfileInfo info1;
    ProfileInfo info2(info1);
    ProfileInfo info3;
    info1.profileBlockLength = 1;
    info1.profileBlock = std::make_unique<unsigned char[]>(2);
    info3 = info1;
    VerifyHap verify;
    bool resultCode = verify.outputOptionalBlocks(errorfile, outputProofFile, outputPropertyFile, optionBlocks);
    EXPECT_EQ(resultCode, false);
    resultCode = verify.outputOptionalBlocks(outputProfileFile, errorfile, outputPropertyFile, optionBlocks);
    EXPECT_EQ(resultCode, false);
    resultCode = verify.outputOptionalBlocks(outputProfileFile, outputProofFile, errorfile, optionBlocks);
    EXPECT_EQ(resultCode, false);
}
}
}
