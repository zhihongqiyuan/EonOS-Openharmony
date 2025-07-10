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
#include <filesystem>
#include "params_run_tool.h"
#include "local_sign_provider.h"
#include "sign_hap.h"
#include "sign_provider.h"
#include "sign_tool_service_impl.h"
#include "remote_sign_provider.h"
#include "hap_sign_test.h"
#include "digest_common.h"
#include <unistd.h>

namespace OHOS {
namespace SignatureTools {

void GenUnvaildUnSignHap(const std::string& path)
{
    std::ofstream outfile(path);
    if (!outfile) {
        SIGNATURE_TOOLS_LOGE("Unable to open file: %s", path.c_str());
        return;
    }
    outfile << "Hello, this is a Unvaild un sign Hap.\n";
    outfile.flush();
    outfile.close();
    return;
}

void HapSignTest::SetUpTestCase(void)
{
    GenUnvaildUnSignHap("./hapSign/phone-default-unsigned");
    GenUnvaildUnSignHap("./hapSign/nohap.hap");

    (void)rename("./hapSign/phone-default-unsigned-test.txt", "./hapSign/phone-default-unsigned-test.hap");
    (void)rename("./hapSign/phone-default-unsigned.txt", "./hapSign/phone-default-unsigned.hap");
    (void)rename("./hapSign/unsigned_with_cd_and_eocd.txt", "./hapSign/unsigned_with_cd_and_eocd.hap");
    (void)rename("./hapSign/unsigned_with_eocd.txt", "./hapSign/unsigned_with_eocd.hap");
    sync();
}

void HapSignTest::TearDownTestCase(void)
{
}
/*
 * @tc.name: hap_sign_test_001
 * @tc.desc: This function tests success for interface SignHap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_001, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, true);
}
/*
 * @tc.name: hap_sign_test_002
 * @tc.desc: This function tests success for interface SignHap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_002, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, true);
}
/*
 * @tc.name: hap_sign_test_003
 * @tc.desc: This function tests success for interface SignHap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_003, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "0";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, false);
}
/*
 * @tc.name: hap_sign_test_004
 * @tc.desc: This function tests success for interface SignHap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_004, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, true);
}
/*
 * @tc.name: hap_sign_test_005
 * @tc.desc: This function tests success for interface Sign
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_005, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    signProvider->CheckInputCertMatchWithProfile(nullptr, nullptr);
    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, true);

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_test_006
 * @tc.desc: This function tests failure for interface CheckParams due to parameter signCode is 3
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_006, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "3";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, false);
}

/*
 * @tc.name: hap_sign_test_007
 * @tc.desc: This function tests failure for interface CheckParams due to parameter signCode and appCertFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_007, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    (*params)["signCode"] = std::string("3");
    bool retParamT = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParamT, false);

    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = std::string("");
    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_008
 * @tc.desc: This function tests sucess for interface Sign
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_008, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, true);

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_test_009
 * @tc.desc: This function tests failure for interface Sign due to parameter profileSigned
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_009, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "0";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, true);

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_010
 * @tc.desc: This function tests sucess for interface Sign
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_010, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, true);

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_test_011
 * @tc.desc: This function tests sucess for interface GetDigestOutputByteSize
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_011, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    ContentDigestAlgorithm alg_tmp;
    ContentDigestAlgorithm alg_1 = ContentDigestAlgorithm::SHA256;
    ContentDigestAlgorithm* alg_2 = &alg_1;
    *alg_2 = alg_1;
    alg_tmp = alg_1;
    ContentDigestAlgorithm alg = ContentDigestAlgorithm::SHA256;
    std::string algname = alg.GetDigestAlgorithm();
    EXPECT_EQ(algname, std::string("SHA-256"));
    int size = alg.GetDigestOutputByteSize();
    EXPECT_EQ(size, 256 / 8);

    ContentDigestAlgorithm alg_384 = ContentDigestAlgorithm::SHA384;
    std::string algname_384 = alg_384.GetDigestAlgorithm();
    EXPECT_EQ(algname_384, std::string("SHA-384"));
    int size384 = alg_384.GetDigestOutputByteSize();
    EXPECT_EQ(size384, 384 / 8);

    ContentDigestAlgorithm alg_512 = ContentDigestAlgorithm::SHA512;
    alg_512 = ContentDigestAlgorithm::SHA512;
    std::string algname_512 = alg_512.GetDigestAlgorithm();
    EXPECT_EQ(algname_512, std::string("SHA-512"));
    int size512 = alg_512.GetDigestOutputByteSize();
    EXPECT_EQ(size512, 512 / 8);
}
/*
 * @tc.name: hap_sign_test_012
 * @tc.desc: This function tests sucess for interface ComputeDigests
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_012, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    ByteBuffer bf1("123456789", 9);
    ByteBuffer bf2("123456789", 9);
    ByteBuffer bf3("123456789", 9);
    ByteBuffer bf4("123456789", 9);
    ByteBufferDataSource ds1(bf1);
    ByteBufferDataSource ds2(bf2);
    ByteBufferDataSource ds3(bf3);

    DataSource* contents[] = {&ds1, &ds2, &ds3};
    int32_t len = 3;

    std::vector<OptionalBlock> optionalBlocks;
    optionalBlocks.push_back({HapUtils::HAP_PROFILE_BLOCK_ID, bf4});
    ByteBuffer dig_context;

    SignatureAlgorithm algo = SignatureAlgorithm::ALGORITHM_SHA256_WITH_ECDSA;
    int32_t nId = DigestCommon::GetDigestAlgorithmId(algo);
    DigestParameter digestParam = HapSignerBlockUtils::GetDigestParameter(nId);

    bool ret = SignHap::ComputeDigests(digestParam, contents, len, optionalBlocks, dig_context);
    EXPECT_EQ(ret, true);
}
/*
 * @tc.name: hap_sign_test_013
 * @tc.desc: This function tests failure for interface Sign due to parameter appCertFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_013, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release-nohave.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}
/*
 * @tc.name: hap_sign_test_014
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_014, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/nohave.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, true);

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_015
 * @tc.desc: This function tests failure for interface Sign due to parameter appCertFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_015, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release-.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    signProvider->CheckParams(params.get());

    signProvider->GetCrl();

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_016
 * @tc.desc: This function tests failure for interface Sign due to parameter outFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_016, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "/d/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, true);

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_017
 * @tc.desc: This function tests failure for interface Sign due to parameter outFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_017, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-unsigned.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, true);

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_test_018
 * @tc.desc: This function tests failure for interface Sign due to parameter signAlg
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_018, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withEA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    std::optional<X509_CRL*> crl = signProvider->GetCrl();
    EXPECT_EQ(crl.has_value(), false);

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_019
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_019, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_020
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_020, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_021
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_021, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/nohap.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_022
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_022, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/unsigned_with_eocd.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_023
 * @tc.desc: This function tests failure for interface SignHap due to parameter mode not support
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_023, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_024
 * @tc.desc: This function tests failure for interface SignHap due to parameter mode not support
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_024, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_025
 * @tc.desc: This function tests success for interface SignHap
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_025, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_test_026
 * @tc.desc: This function tests success and failure for interface Sign & ComputeDigests
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_026, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    ByteBuffer bf1("123456789", 9);
    ByteBuffer bf2("123456789", 9);
    ByteBuffer bf3("123456789", 9);
    ByteBuffer bf4("123456789", 9);
    ByteBufferDataSource ds1(bf1);
    ByteBufferDataSource ds2(bf2);
    ByteBufferDataSource ds3(bf3);

    DataSource* contents[] = {&ds1, &ds2, &ds3};
    DataSource* contents_t[] = {nullptr, nullptr, nullptr};
    int32_t len = 3;

    std::vector<OptionalBlock> optionalBlocks;
    std::vector<OptionalBlock> optionalBlockSTest;
    optionalBlocks.push_back({HapUtils::HAP_PROFILE_BLOCK_ID, bf4});
    ByteBuffer dig_context;

    SignatureAlgorithm algo = SignatureAlgorithm::ALGORITHM_SHA256_WITH_ECDSA;
    int32_t nId = DigestCommon::GetDigestAlgorithmId(algo);
    DigestParameter digestParam = HapSignerBlockUtils::GetDigestParameter(nId);

    SignerConfig config;
    ByteBuffer result;
    ByteBuffer result1;
    bool ret1 = SignHap::Sign(contents, 2, config, optionalBlocks, result);
    EXPECT_EQ(ret1, false);

    ret1 = SignHap::Sign(contents_t, 3, config, optionalBlocks, result1);
    EXPECT_EQ(ret1, false);

    std::vector<SignatureAlgorithmHelper> sig{SignatureAlgorithmHelper::ECDSA_WITH_SHA256_INSTANCE};
    config.SetSignatureAlgorithms(sig);
    ret1 = SignHap::Sign(contents_t, 3, config, optionalBlocks, result1);
    EXPECT_EQ(ret1, false);
    bool ret = SignHap::ComputeDigests(digestParam, contents, len, optionalBlockSTest, dig_context);
    ret = SignHap::ComputeDigests(digestParam, contents_t, len, optionalBlocks, dig_context);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_027
 * @tc.desc: This function tests failure for interface SignHap due to parameter inFile not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_027, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_test_028
 * @tc.desc: This function tests failure for interface SignHap due to parameter inFile damage
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_028, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/unsigned_with_cd_and_eocd.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_test_029
 * @tc.desc: This function tests failure for interface SignHap due to parameter signAlg not suport
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_test_029, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256w";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/unsigned_with_cd_and_eocd.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->SignHap(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: get_options_test_001
 * @tc.desc: This function tests success for interface SetOptions
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapSignTest, get_options_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    (*params)["keyAlias"] = keyAlias;
    SignerConfig signerConfig;
    signerConfig.SetOptions(params.get());
    EXPECT_EQ(params.get(), signerConfig.GetOptions());
}

/**
 * @tc.name: get_certificates_test_001
 * @tc.desc: Test function of GetCertificates()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapSignTest, get_certificates_test_001, testing::ext::TestSize.Level1)
{
    X509* newCert = X509_new();
    EXPECT_TRUE(X509_set_version(newCert, 2));
    SignerConfig signerConfig;
    signerConfig.SetX509CRLs(nullptr);
    signerConfig.GetX509CRLs();
    signerConfig.SetCertificates(nullptr);
    signerConfig.GetCertificates();
    signerConfig.GetSignParamMap();
    STACK_OF(X509) *certs = sk_X509_new_null();
    sk_X509_push(certs, newCert);
    signerConfig.SetCertificates(certs);

    EXPECT_NE(signerConfig.GetCertificates(), nullptr);
}

/**
 * @tc.name: get_X509_CRLs_test_001
 * @tc.desc: Test function of GetX509CRLs()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapSignTest, get_X509_CRLs_test_001, testing::ext::TestSize.Level1)
{
    STACK_OF(X509_CRL)* x509CRLs = nullptr;
    SignerConfig signerConfig;
    signerConfig.SetX509CRLs(x509CRLs);
    EXPECT_EQ(x509CRLs, signerConfig.GetX509CRLs());
}

/**
 * @tc.name: get_X509_CRLs_test_002
 * @tc.desc: This function tests failure for interface GetX509CRLs
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapSignTest, get_X509_CRLs_test_002, testing::ext::TestSize.Level1)
{
    STACK_OF(X509_CRL)* x509CRLs = nullptr;
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256w";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/unsigned_with_cd_and_eocd.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    SignerConfig signerConfig;
    signerConfig.SetOptions(params.get());

    signerConfig.GetX509CRLs();
    STACK_OF(X509_CRL)* crls = sk_X509_CRL_new_null();
    signerConfig.SetX509CRLs(crls);
    signerConfig.GetX509CRLs();

    signerConfig.SetCertificates(nullptr);
    signerConfig.SetX509CRLs(x509CRLs);
    signerConfig.GetX509CRLs();

    X509* newCert = X509_new();
    X509_set_version(newCert, 2);
    signerConfig.SetCertificates((STACK_OF(X509)*)newCert);
    signerConfig.GetX509CRLs();

    signerConfig.GetSigner();
    signerConfig.SetX509CRLs(x509CRLs);
    signerConfig.SetCertificates(nullptr);
    signerConfig.GetCertificates();
    EXPECT_EQ(false, (signerConfig.GetX509CRLs() != nullptr));
}

/**
 * @tc.name: get_compatible_version_test_001
 * @tc.desc: Test function of GetCompatibleVersion()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapSignTest, get_compatible_version_test_001, testing::ext::TestSize.Level1)
{
    int n = 5;
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(n);
    EXPECT_EQ(5, signerConfig.GetCompatibleVersion());
}

/**
 * @tc.name: find_by_id_test_001
 * @tc.desc: Test function of FindById()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapSignTest, find_by_id_test_001, testing::ext::TestSize.Level1)
{
    const SignatureAlgorithmHelper* tmp = SignatureAlgorithmHelper::FindById(SignatureAlgorithmId::ECDSA_WITH_SHA256);
    EXPECT_EQ(&(SignatureAlgorithmHelper::ECDSA_WITH_SHA256_INSTANCE), tmp);
    const SignatureAlgorithmHelper* tmp1 = SignatureAlgorithmHelper::FindById(SignatureAlgorithmId::ECDSA_WITH_SHA384);
    EXPECT_EQ(&(SignatureAlgorithmHelper::ECDSA_WITH_SHA384_INSTANCE), tmp1);
}

/**
 * @tc.name: find_by_id_test_002
 * @tc.desc: Test function of FindById()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapSignTest, find_by_id_test_002, testing::ext::TestSize.Level1)
{
    SignatureAlgorithmHelper tmp1 = SignatureAlgorithmHelper::ECDSA_WITH_SHA256_INSTANCE;
    SignatureAlgorithmHelper tmp3 = SignatureAlgorithmHelper::ECDSA_WITH_SHA384_INSTANCE;
    SignatureAlgorithmHelper* tmp2 = &tmp1;
    *tmp2 = tmp1;
    tmp3 = tmp1;
    const SignatureAlgorithmHelper* tmp = SignatureAlgorithmHelper::FindById(SignatureAlgorithmId::DSA_WITH_SHA512);
    EXPECT_EQ(nullptr, tmp);
}

/*
 * @tc.name: remote_sign_provider_001
 * @tc.desc: This function tests failure for interface CheckParams due to signCode is 3
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_001, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<RemoteSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    std::string onlineAuthMode = "/mnt/d/testcpuls/OpenHarmony.p12";
    std::string compatibleVersion = "8";
    std::string signerPlugin = "/mnt/d/libRemoteSigner.so";
    std::string signServer = "/mnt/d/testcpuls/app-release1.pem";
    std::string username = "test";
    char userPwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["onlineAuthMode"] = onlineAuthMode;
    (*params)["compatibleVersion"] = compatibleVersion;
    (*params)["signerPlugin"] = signerPlugin;
    (*params)["signServer"] = signServer;
    (*params)["username"] = username;
    (*params)["userPwd"] = userPwd;

    (*params)["signCode"] = std::string("3");
    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, false);
}

/*
 * @tc.name: remote_sign_provider_002
 * @tc.desc: This function tests failure for interface CheckInputCertMatchWithProfile due to X509 error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_002, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    bool ret = signProvider->CheckInputCertMatchWithProfile(nullptr, nullptr);
    EXPECT_EQ(ret, false);

    X509* cert = nullptr;
    STACK_OF(X509)* skCert = sk_X509_new(nullptr);
    BIO* certBio = BIO_new_file("./hapSign/app-release1.pem", "rb");
    if (certBio == nullptr) {
        sk_X509_pop_free(skCert, X509_free);
        EXPECT_NE(certBio, nullptr);
    }
    // 读取
    while (1) {
        cert = PEM_read_bio_X509(certBio, nullptr, nullptr, nullptr);
        if (cert == nullptr) {
            break;
        }
        sk_X509_push(skCert, cert);
    }
    BIO_free(certBio);
    if (sk_X509_num(skCert) > 1) {
        X509* firstCert = sk_X509_value(skCert, 0);
        X509* secondCert = sk_X509_value(skCert, 1);
        bool val = signProvider->CheckInputCertMatchWithProfile(firstCert, firstCert);
        val = signProvider->CheckInputCertMatchWithProfile(firstCert, secondCert);
        EXPECT_EQ(val, false);
    }
    sk_X509_pop_free(skCert, X509_free);
}

/*
 * @tc.name: remote_sign_provider_003
 * @tc.desc: This function tests failure for interface CheckInputCertMatchWithProfile due to x509 error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_003, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    CertTools::SetCertIssuerName(cert, BuildDN(issuername, issuerReq));

    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_set_subject_name(cert, BuildDN(subjectname, issuerReq));

    X509* cert1 = X509_new();
    X509_REQ* issuerReq1 = X509_REQ_new();
    std::string issuername1 = "C=wc,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    CertTools::SetCertIssuerName(cert1, BuildDN(issuername1, issuerReq1));

    std::string subjectname1 = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_set_subject_name(cert1, BuildDN(subjectname1, issuerReq1));

    bool ret = signProvider->CheckInputCertMatchWithProfile(cert1, cert);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    X509_REQ_free(issuerReq1);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: remote_sign_provider_004
 * @tc.desc: This function tests failure for interface CheckInputCertMatchWithProfile due to subjectname error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_004, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    CertTools::SetCertIssuerName(cert, BuildDN(issuername, issuerReq));
    CertTools::SetCertSerialNum(cert);
    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_set_subject_name(cert, BuildDN(subjectname, issuerReq));

    X509* cert1 = X509_new();
    X509_REQ* issuerReq1 = X509_REQ_new();
    std::string issuername1 = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    CertTools::SetCertIssuerName(cert1, BuildDN(issuername1, issuerReq1));
    CertTools::SetCertSerialNum(cert1);
    std::string subjectname1 = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_set_subject_name(cert1, BuildDN(subjectname1, issuerReq1));

    bool ret = signProvider->CheckInputCertMatchWithProfile(cert1, cert);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    X509_REQ_free(issuerReq1);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: remote_sign_provider_005
 * @tc.desc: This function tests failure for interface CheckParams due to signCode is 3
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_005, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<RemoteSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    std::string onlineAuthMode = "/mnt/d/testcpuls/OpenHarmony.p12";
    std::string compatibleVersion = "8";
    std::string signerPlugin = "/mnt/d/libRemoteSigner.so";
    std::string signServer = "/mnt/d/testcpuls/app-release1.pem";
    std::string username = "test";
    char userPwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["onlineAuthMode"] = onlineAuthMode;
    (*params)["compatibleVersion"] = compatibleVersion;
    (*params)["signerPlugin"] = signerPlugin;
    (*params)["signServer"] = signServer;
    (*params)["username"] = username;
    (*params)["userPwd"] = userPwd;

    (*params)["signCode"] = std::string("3");
    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, false);
}

/*
 * @tc.name: remote_sign_provider_006
 * @tc.desc: This function tests success for interface CheckParams
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_006, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<RemoteSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    std::string onlineAuthMode = "/mnt/d/testcpuls/OpenHarmony.p12";
    std::string compatibleVersion = "8";
    std::string signerPlugin = "/mnt/d/libRemoteSigner.so";
    std::string signServer = "/mnt/d/testcpuls/app-release1.pem";
    std::string username = "test";
    char userPwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["onlineAuthMode"] = onlineAuthMode;
    (*params)["compatibleVersion"] = compatibleVersion;
    (*params)["signerPlugin"] = signerPlugin;
    (*params)["signServer"] = signServer;
    (*params)["username"] = username;
    (*params)["userPwd"] = userPwd;
    bool retParam = signProvider->CheckParams(params.get());
    EXPECT_EQ(retParam, true);
}

/*
 * @tc.name: remote_sign_provider_007
 * @tc.desc: This function tests failed  for interface CheckInputCertMatchWithProfile due to X509_NAME error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_007, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509* cert1 = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    CertTools::SetCertIssuerName(cert, BuildDN(issuername, issuerReq));

    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* subName = BuildDN(subjectname, issuerReq);
    X509_set_subject_name(cert, subName);
    X509_set_subject_name(cert1, subName);

    X509_REQ* issuerReq1 = X509_REQ_new();
    std::string issuername1 = "C=wc,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    CertTools::SetCertIssuerName(cert1, BuildDN(issuername1, issuerReq1));

    std::string subjectname1 = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";

    bool ret = signProvider->CheckInputCertMatchWithProfile(cert1, cert);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    X509_REQ_free(issuerReq1);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: remote_sign_provider_008
 * @tc.desc: This function tests failed  for interface CheckInputCertMatchWithProfile due to EVP_PKEY error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_008, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509* cert1 = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* issName = BuildDN(issuername, issuerReq);
    CertTools::SetCertIssuerName(cert, issName);
    CertTools::SetCertIssuerName(cert1, issName);

    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* subName = BuildDN(subjectname, issuerReq);
    X509_set_subject_name(cert, subName);
    X509_set_subject_name(cert1, subName);
    KeyStoreHelper stroe;
    EVP_PKEY* key1 = stroe.GenerateKeyPair("ECC", 256);
    EVP_PKEY* key2 = stroe.GenerateKeyPair("ECC", 256);

    X509_set_pubkey(cert, key1);
    X509_set_pubkey(cert1, key2);

    bool ret = signProvider->CheckInputCertMatchWithProfile(cert1, cert);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    EVP_PKEY_free(key1);
    EVP_PKEY_free(key2);
    EXPECT_EQ(ret, false);
}
/*
 * @tc.name: remote_sign_provider_009
 * @tc.desc: This function tests failed for interface CheckInputCertMatchWithProfile due to EVP_PKEY error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_009, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509* cert1 = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* issName = BuildDN(issuername, issuerReq);
    CertTools::SetCertIssuerName(cert, issName);
    CertTools::SetCertIssuerName(cert1, issName);

    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* subName = BuildDN(subjectname, issuerReq);
    X509_set_subject_name(cert, subName);
    X509_set_subject_name(cert1, subName);
    KeyStoreHelper stroe;
    EVP_PKEY* key1 = stroe.GenerateKeyPair("ECC", 256);
    EVP_PKEY* key2 = stroe.GenerateKeyPair("ECC", 256);

    X509_set_pubkey(cert, key1);
    X509_set_pubkey(cert1, key2);

    bool ret = signProvider->CheckInputCertMatchWithProfile(cert1, nullptr);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    EVP_PKEY_free(key1);
    EVP_PKEY_free(key2);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: remote_sign_provider_010
 * @tc.desc: This function tests failed for interface CheckInputCertMatchWithProfile due to EVP_PKEY error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_010, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509* cert1 = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* issName = BuildDN(issuername, issuerReq);
    CertTools::SetCertIssuerName(cert, issName);
    CertTools::SetCertIssuerName(cert1, issName);

    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* subName = BuildDN(subjectname, issuerReq);
    X509_set_subject_name(cert, subName);
    X509_set_subject_name(cert1, subName);
    KeyStoreHelper stroe;
    EVP_PKEY* key1 = stroe.GenerateKeyPair("ECC", 256);

    X509_set_pubkey(cert, key1);

    bool ret = signProvider->CheckInputCertMatchWithProfile(cert1, cert);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    EVP_PKEY_free(key1);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: remote_sign_provider_011
 * @tc.desc: This function tests failed for interface CheckInputCertMatchWithProfile due to EVP_PKEY error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_011, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509* cert1 = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* issName = BuildDN(issuername, issuerReq);
    CertTools::SetCertIssuerName(cert, issName);
    CertTools::SetCertIssuerName(cert1, issName);

    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* subName = BuildDN(subjectname, issuerReq);
    X509_set_subject_name(cert, subName);
    X509_set_subject_name(cert1, subName);
    KeyStoreHelper stroe;
    EVP_PKEY* key2 = stroe.GenerateKeyPair("ECC", 256);

    X509_set_pubkey(cert1, key2);

    bool ret = signProvider->CheckInputCertMatchWithProfile(cert1, cert);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    EVP_PKEY_free(key2);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: remote_sign_provider_012
 * @tc.desc: This function tests failed for interface CheckInputCertMatchWithProfile due to EVP_PKEY error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, remote_sign_provider_012, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<RemoteSignProvider> signProvider = std::make_unique<RemoteSignProvider>();

    X509* cert = X509_new();
    X509* cert1 = X509_new();
    X509_REQ* issuerReq = X509_REQ_new();
    std::string issuername = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* issName = BuildDN(issuername, issuerReq);
    CertTools::SetCertIssuerName(cert, issName);
    CertTools::SetCertIssuerName(cert1, issName);

    std::string subjectname = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    X509_NAME* subName = BuildDN(subjectname, issuerReq);
    X509_set_subject_name(cert, subName);
    X509_set_subject_name(cert1, subName);
    KeyStoreHelper stroe;

    bool ret = signProvider->CheckInputCertMatchWithProfile(cert1, cert);
    X509_free(cert);
    X509_free(cert1);
    X509_REQ_free(issuerReq);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: remote_sign_provider_013
 * @tc.desc: This function tests failed for interface Sign due to parameter signerPlugin error
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapSignTest, remote_sign_provider_013, testing::ext::TestSize.Level1)
{
    std::string mode = "remoteSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string compatibleVersion = "8";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string outFile = "./hapSign/signed.hap";
    std::string signServer = "./hapSign/app-release1.pem";
    std::string signerPlugin = "./hapSign/dummy.z.so";
    std::string onlineAuthMode = "./hapSign/ohtest.p12";
    std::string username = "123456";
    char userPwd[] = "123456";

    std::shared_ptr<Options> params = std::make_shared<Options>();
    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["profileFile"] = profileFile;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["compatibleVersion"] = compatibleVersion;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signServer"] = signServer;
    (*params)["onlineAuthMode"] = onlineAuthMode;
    (*params)["username"] = username;
    (*params)["userPwd"] = userPwd;
    // the signerPlugin dummy.z.so is not exist
    (*params)["signerPlugin"] = signerPlugin;

    auto remoteSignProvider = std::make_unique<RemoteSignProvider>();
    ASSERT_FALSE(remoteSignProvider->Sign(params.get()));
}

/*
 * @tc.name: hap_sign_error_001
 * @tc.desc: This function tests failed for interface Sign due to parameter path not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignTest, hap_sign_error_001, testing::ext::TestSize.Level1)
{
    ByteBuffer bf1("123456789", 9);
    ByteBuffer bf2("123456789", 9);
    ByteBuffer bf3("123456789", 9);
    ByteBuffer bf4(0x7FFFFFFF);
    ByteBufferDataSource ds1(bf1);
    ByteBufferDataSource ds2(bf2);
    ByteBufferDataSource ds3(bf3);
    DataSource* contents[] = {&ds1, &ds2, &ds3};
    std::vector<OptionalBlock> optionalBlocks;
    optionalBlocks.push_back({HapUtils::HAP_PROFILE_BLOCK_ID, bf4});
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "/mnt/d/testcpuls/app-release1.pem";
    std::string profileFile = "/mnt/d/testcpuls/signed-profile.p7b";
    std::string inFile = "/mnt/d/testcpuls/entry-default-unsigned-so.hap";
    std::string keystoreFile = "/mnt/d/testcpuls/OpenHarmony.p12";
    std::string outFile = "/mnt/d/testcpuls/entry-default-signed-so.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";
    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    SignerConfig config;
    std::vector<SignatureAlgorithmHelper> sig{SignatureAlgorithmHelper::ECDSA_WITH_SHA256_INSTANCE};
    config.SetSignatureAlgorithms(sig);
    config.SetOptions(params.get());
    ByteBuffer result;
    bool ret1 = SignHap::Sign(contents, 3, config, optionalBlocks, result);
    EXPECT_EQ(ret1, false);
}
} // namespace SignatureTools
} // namespace OHOS