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
#include "sign_provider_test.h"
#include "params_run_tool.h"
#include "local_sign_provider.h"
#include "remote_sign_provider.h"
#include "sign_hap.h"
#include "sign_provider.h"
#include "sign_tool_service_impl.h"
#include <unistd.h>

namespace OHOS {
namespace SignatureTools {

void GenUnvaildUnProviderHap(const std::string& path)
{
    std::ofstream outfile(path);
    if (!outfile) {
        SIGNATURE_TOOLS_LOGE("Unable to open file: %s", path.c_str());
        return;
    }
    outfile << "Hello, this is a Unvaild un provider Hap.\n";
    outfile.flush();
    outfile.close();
    return;
}

void SignProviderTest::SetUpTestCase(void)
{
    GenUnvaildUnProviderHap("./hapSign/phone-default-unsigned");
    GenUnvaildUnProviderHap("./hapSign/phone-default-unsigned.txt");
    GenUnvaildUnProviderHap("./hapSign/nohap.hap");

    (void)rename("./hapSign/packages/phone-default-unsigned-test.txt", "./hapSign/phone-default-unsigned-test.hap");
    (void)rename("./hapSign/packages/phone-default-unsigned.txt", "./hapSign/phone-default-unsigned.hap");
    (void)rename("./hapSign/packages/unsigned_with_cd_and_eocd.txt", "./hapSign/unsigned_with_cd_and_eocd.hap");
    (void)rename("./hapSign/packages/unsigned_with_eocd.txt", "./hapSign/unsigned_with_eocd.hap");
    (void)rename("./codeSigning/unsigned-file.txt", "./codeSigning/unsigned-file.hap");
    sync();
}

void SignProviderTest::TearDownTestCase(void)
{
}

/*
 * @tc.name: sign_provider_test_001
 * @tc.desc: This function tests success for interface Sign
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_001, testing::ext::TestSize.Level1)
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
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned-test.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-unsigned-test.hap";
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

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, true);
}
/*
 * @tc.name: sign_provider_test_002
 * @tc.desc: This function tests failure for interface Sign due to parameter compatibleVersion is not int
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_002, testing::ext::TestSize.Level1)
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

    (*params)["compatibleVersion"] = std::string("a");
    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_003
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile format error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_003, testing::ext::TestSize.Level1)
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
 * @tc.name: sign_provider_test_004
 * @tc.desc: This function tests failure for interface Sign due to parameter compatibleVersion is not int
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_004, testing::ext::TestSize.Level1)
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
    std::string compatibleVersion = "a";

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
    (*params)["compatibleVersion"] = compatibleVersion;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_005
 * @tc.desc: This function tests failure for interface Sign due to parameter compatibleVersion is not int
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_005, testing::ext::TestSize.Level1)
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
    std::string property = "./hapSign/signed-profile.p7b";
    std::string proof = "./hapSign/signed-profile.p7b";

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
    (*params)["property"] = property;
    (*params)["proof"] = proof;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: sign_provider_test_006
 * @tc.desc: This function tests failure for interface Sign due to parameter property is not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_006, testing::ext::TestSize.Level1)
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
    std::string property = "./hapSign/signed-nohavaprofile.p7b";
    std::string proof = "./hapSign/signed-profile.p7b";

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
    (*params)["property"] = property;
    (*params)["proof"] = proof;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_007
 * @tc.desc: This function tests failure for interface Sign due to parameter property is not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_007, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-nohaveprofile.p7b";
    std::string inFile = "./hapSign/phone-default-unsigned.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";
    std::string property = "./hapSign/signed-profile.p7b";
    std::string proof = "./hapSign/signed-profile.p7b";

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
    (*params)["property"] = property;
    (*params)["proof"] = proof;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_008
 * @tc.desc: This function tests failure for interface Sign due to parameter proof is not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_008, testing::ext::TestSize.Level1)
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
    std::string property = "./hapSign/signed-profile.p7b";
    std::string proof = "./hapSign/signed-nohaveprofile.p7b";

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
    (*params)["property"] = property;
    (*params)["proof"] = proof;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: sign_provider_test_009
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile is not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_009, testing::ext::TestSize.Level1)
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
    std::string inFile = "./hapSign_test/phone-default-unsigned.hap";
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
 * @tc.name: sign_provider_test_010
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile is not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_010, testing::ext::TestSize.Level1)
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
    std::string inFile = "./hapSign";
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
 * @tc.name: sign_provider_test_011
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile is not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_011, testing::ext::TestSize.Level1)
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
    std::string inFile = "";
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
 * @tc.name: sign_provider_test_012
 * @tc.desc: This function tests failure for interface Sign due to parameter profileFile is not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_012, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-nohava-profile.p7b";
    std::string inFile = "./hapSign_test/phone-default-unsigned.hap";
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
 * @tc.name: sign_provider_test_013
 * @tc.desc: This function tests failure for interface Sign due to parameter compatibleVersion is not int
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_013, testing::ext::TestSize.Level1)
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
    std::string compatibleVersion = "a";

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
    (*params)["compatibleVersion"] = compatibleVersion;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_014
 * @tc.desc: This function tests failure for interface Sign due to parameter inFile is not exist
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_014, testing::ext::TestSize.Level1)
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
 * @tc.name: sign_provider_test_015
 * @tc.desc: This function tests failure for interface Sign due to parameter signAlg is not support
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_015, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA512withECDSA";
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

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_016
 * @tc.desc: This function tests failure for interface Sign due to parameter profileSigned error
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_016, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile-nohave.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned-test.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed-test.hap";
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

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_017
 * @tc.desc: This function tests failure for interface Sign due to parameter signAlg is not support
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_017, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA-nohave";
    std::string signCode = "0";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned-test.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed-test.hap";
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

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_018
 * @tc.desc: This function tests failure for interface Sign due to parameter signCode is 3
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_018, testing::ext::TestSize.Level1)
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

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_019
 * @tc.desc: This function tests failure for interface Sign due to parameter signAlg is empty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_019, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "";
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

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_020
 * @tc.desc: This function tests failure for interface Sign due to parameter profileFile is empty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_020, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "";
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

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: sign_provider_test_021
 * @tc.desc: This function tests failure for interface Sign due to parameter appCertFile is empty
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, sign_provider_test_021, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI("hello world !!!");
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "0";
    std::string appCertFile = "";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string profileSigned = "1";
    std::string inFile = "./hapSign/phone-default-unsigned-test.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/phone-default-signed-test.hap";
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

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: SignElf_001
 * @tc.desc: Test function result of SignElf_001 will be SUCCESS.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, SignElf_001, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./codeSigning/unsigned-file.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/entry-default-signed.elf";
    std::string inForm = "elf";
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
    (*params)["inForm"] = inForm;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    bool ret = signProvider->SignElf(params.get());

    EXPECT_NE(ret, -1);
}

/*
 * @tc.name: SignElf_002
 * @tc.desc: The return will be false, because the profileFile is null.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, SignElf_002, testing::ext::TestSize.Level1)
{
    // profileFile:p7b is null
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string inFile = "./codeSigning/unsigned-file.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/entry-default-signed.elf";
    std::string inForm = "elf";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["inForm"] = inForm;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    bool ret = signProvider->SignElf(params.get());
    EXPECT_NE(ret, -1);
}

/*
 * @tc.name: SignElf_003
 * @tc.desc: The return will be false, because the compatibleVersion is null.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, SignElf_003, testing::ext::TestSize.Level1)
{
    // compatibleVersion is null
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./codeSigning/unsigned-file.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/entry-default-signed.elf";
    std::string inForm = "elf";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";
    std::string compatibleVersion = "";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["inForm"] = inForm;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["compatibleVersion"] = compatibleVersion;
    bool ret = signProvider->SignElf(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: SignElf_004
 * @tc.desc: The return will be false, because the inFile is null.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, SignElf_004, testing::ext::TestSize.Level1)
{
    // inFile is null
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/entry-default-signed.elf";
    std::string inForm = "elf";
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
    (*params)["inForm"] = inForm;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = signProvider->SignElf(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: SignElf_005
 * @tc.desc: The return will be false, because the inFile path is error.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, SignElf_005, testing::ext::TestSize.Level1)
{
    // inFile path is false
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./hapSign_test/unsigned-file.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/entry-default-signed.elf";
    std::string inForm = "elf";
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
    (*params)["inForm"] = inForm;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    bool ret = signProvider->SignElf(params.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: SignElf_006
 * @tc.desc: The return will be false, because the signAlg format does not support.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(SignProviderTest, SignElf_006, testing::ext::TestSize.Level1)
{
    // signAlg format is false
    std::unique_ptr<SignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA512withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./hapSign/app-release1.pem";
    std::string profileFile = "./hapSign/signed-profile.p7b";
    std::string inFile = "./codeSigning/unsigned-file.hap";
    std::string keystoreFile = "./hapSign/ohtest.p12";
    std::string outFile = "./hapSign/entry-default-signed.elf";
    std::string inForm = "elf";
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
    (*params)["inForm"] = inForm;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    bool ret = signProvider->SignElf(params.get());
    EXPECT_EQ(ret, false);
}
} // namespace SignatureTools
} // namespace OHOS
