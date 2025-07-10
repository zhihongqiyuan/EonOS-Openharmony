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
#include <chrono>
#include <thread>
#include <string>
#include <filesystem>

#include "gtest/gtest.h"
#include "options.h"
#include "sign_tool_service_impl.h"
#include "nlohmann/json.hpp"
#include "signer_factory.h"
#include "profile_sign_tool.h"
#include "params_run_tool.h"
#include "pkcs7_data.h"
#include "signer_config.h"
#include "local_signer.h"
#include "bc_pkcs7_generator.h"
#include "bc_signeddata_generator.h"
#include "profile_verify.h"
#include "constant.h"

using  nlohmann::json;

namespace OHOS {
namespace SignatureTools {

// sign profile使用的全局参数
static const std::string SIGN_PROFILE_MODE = "localSign";
static const std::string SIGN_PROFILE_KEY_ALIAS = "oh-profile1-key-v1";
static const std::string SIGN_PROFILE_PROFILE_CERT_FILE = "./signProfile/profile-release1.pem";
static const std::string SIGN_PROFILE_SIGN_ALG = "SHA384withECDSA";
static const std::string SIGN_PROFILE_KEY_STORE_FILE = "./signProfile/ohtest.p12";
static const std::string SIGN_PROFILE_OUT_FILE = "./signProfile/signed-profile.p7b";
static const std::string SIGN_PROFILE_IN_FILE = "./signProfile/profile.json";
// verify profile 使用的全局参数
static const std::string VERIFY_PROFILE_IN_FILE = "./signProfile/signed-profile.p7b";
static const std::string VERIFY_PROFILE_OUT_FILE = "./signProfile/VerifyResult.json";
// sign app 使用全局参数
static const std::string SIGN_APP_MODE = "localSign";
static const std::string SIGN_APP_KEY_ALIAS = "oh-app1-key-v1";
static const std::string SIGN_APP_APP_CERT_FILE = "./signProfile/app-release1.pem";
static const std::string SIGN_APP_PROFILE_FILE = "./signProfile/signed-profile.p7b";
static const std::string SIGN_APP_IN_FILE = "./signProfile/unsigned.hap";
static const std::string SIGN_APP_IN_FILE_TXT = "./signProfile/unsigned.txt";
static const std::string SIGN_APP_SIGN_ALG = "SHA256withECDSA";
static const std::string SIGN_APP_KEY_STORE_FILE = "./signProfile/ohtest.p12";
static const std::string SIGN_APP_OUT_FILE = "./signProfile/signed.hap";

class SignProfileTest : public testing::Test {
public:
    static void SetUpTestCase(void);

    static void TearDownTestCase(void);

    void SetUp()override;

    void TearDown()override;
};

void SignProfileTest::SetUpTestCase(void)
{
    std::filesystem::copy(SIGN_APP_IN_FILE_TXT, SIGN_APP_IN_FILE);
}

void SignProfileTest::TearDownTestCase(void)
{
    std::filesystem::remove(SIGN_APP_IN_FILE);
}

void SignProfileTest::SetUp()
{
}

void SignProfileTest::TearDown()
{
}

/**
    * @tc.name: run_sign_profile_test001
    * @tc.desc: Test function of HapSignTool::RunSignProfile() interface for SUCCESS.
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, run_sign_profile_test001, testing::ext::TestSize.Level1)
{
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = ParamsRunTool::RunSignProfile(&options, api);
    EXPECT_EQ(result, true);
}

/**
    * @tc.name: sign_profile_test001
    * @tc.desc: Test function of SignToolServiceImpl::SignProfile() interface for SUCCESS.
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, sign_profile_test001, testing::ext::TestSize.Level1)
{
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    char pwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignToolServiceImpl api;
    bool result = api.SignProfile(&options);
    EXPECT_EQ(result, true);
    // 密钥匿名错误 失败
    options[Options::KEY_ALIAS] = std::string("oh-profile1-key-v2");
    std::copy(pwd, pwd + 7, keypwd);
    std::copy(pwd, pwd + 7, keyStorePwd);
    result = api.SignProfile(&options);
    options[Options::KEY_ALIAS] = std::string("oh-profile1-key-v1");
    EXPECT_EQ(result, false);
    // 使用SHAwithECDSA256签名算法 成功
    options[Options::SIGN_ALG] = std::string("SHA256withECDSA");
    std::copy(pwd, pwd + 7, keypwd);
    std::copy(pwd, pwd + 7, keyStorePwd);
    result = api.SignProfile(&options);
    options[Options::SIGN_ALG] = std::string("SHA384withECDSA");
    EXPECT_EQ(result, true);
}

/**
    * @tc.name: get_provision_content_test001
    * @tc.desc: Test function of SignToolServiceImpl::GetProvisionContent() interface for SUCCESS.
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, get_provision_content_test001, testing::ext::TestSize.Level1)
{
    std::string ret;
    const std::string provisionFilePath = SIGN_PROFILE_IN_FILE;
    SignToolServiceImpl::GetProvisionContent(provisionFilePath, ret);
    EXPECT_TRUE(ret.size() > 0);
}

/**
* @tc.name: parse_provision_test001
* @tc.desc: Test function of ParseProvision() interface for SUCCESS.
* @tc.size: MEDIUM
* @tc.type: FUNC
* @tc.level Level 1
* @tc.require: SR000H63TL
*/
HWTEST_F(SignProfileTest, parse_provision_test001, testing::ext::TestSize.Level1)
{
    std::string bytes;
    FileUtils::ReadFile(SIGN_PROFILE_IN_FILE, bytes);
    ProfileInfo provision;
    AppProvisionVerifyResult result = ParseProvision(bytes, provision);
    EXPECT_TRUE(result == PROVISION_OK);
}

/**
    * @tc.name: generate_p7b_test001
    * @tc.desc: Test function of ProfileSignTool::GenerateP7b() interface for SUCCESS.
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, generate_p7b_test001, testing::ext::TestSize.Level1)
{
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    std::string content = "json content";
    int result = ProfileSignTool::GenerateP7b(adapter, content, content);
    EXPECT_TRUE(result == 0);
}

/**
    * @tc.name: print_cert_chain_sub_test001
    * @tc.desc: Test function of PKCS7Data::PrintCertChainSub() interface for SUCCESS.
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, print_cert_chain_sub_test001, testing::ext::TestSize.Level1)
{
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    STACK_OF(X509)* certs = signer->GetCertificates();
    PKCS7Data::PrintCertChainSub(certs);
    int ret = PKCS7Data::SortX509Stack(certs);
    EXPECT_EQ(ret, RET_OK);
}

/**
* @tc.name: run_verify_profile_test001
* @tc.desc: Test function of HapSignTool::RunVerifyProfile() interface for SUCCESS.
* @tc.size: MEDIUM
* @tc.type: FUNC
* @tc.level Level 1
* @tc.require: SR000H63TL
*/
HWTEST_F(SignProfileTest, run_verify_profile_test001, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::IN_FILE] = VERIFY_PROFILE_IN_FILE;
    options[Options::OUT_FILE] = VERIFY_PROFILE_OUT_FILE;
    SignToolServiceImpl api;
    bool ret = ParamsRunTool::RunVerifyProfile(&options, api);
    EXPECT_EQ(ret, true);
}

/**
* @tc.name: run_sign_profile_test001
* @tc.desc: Test function of SignToolServiceImpl::VerifyProfile() interface for SUCCESS json写入OUT_FILE.
* @tc.size: MEDIUM
* @tc.type: FUNC
* @tc.level Level 1
* @tc.require: SR000H63TL
*/
HWTEST_F(SignProfileTest, verify_profile_test001, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::IN_FILE] = VERIFY_PROFILE_IN_FILE;
    options[Options::OUT_FILE] = VERIFY_PROFILE_OUT_FILE;
    bool result = false;
    SignToolServiceImpl api;
    result = api.VerifyProfile(&options);
    EXPECT_TRUE(result);
}


/**
    * @tc.name: run_sign_profile_test001
    * @tc.desc: Test function of SignToolServiceImpl::VerifyProfile() interface for SUCCESS 验证成功打印json到控制台.
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, verify_profile_test002, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::IN_FILE] = VERIFY_PROFILE_IN_FILE;
    options[Options::OUT_FILE] = VERIFY_PROFILE_OUT_FILE;
    bool result = false;
    SignToolServiceImpl api;
    result = api.VerifyProfile(&options);
    EXPECT_TRUE(result);
}

/**
* @tc.name: parse_test001
* @tc.desc: Test function of PKCS7Data::Parse() interface for SUCCESS .
* @tc.size: MEDIUM
* @tc.type: FUNC
* @tc.level Level 1
* @tc.require: SR000H63TL
*/
HWTEST_F(SignProfileTest, parse_test001, testing::ext::TestSize.Level1)
{
    std::string p7b;
    FileUtils::ReadFile(VERIFY_PROFILE_IN_FILE, p7b);
    PKCS7Data p7;
    int result = p7.Parse(p7b);
    EXPECT_EQ(result, 0);
}

/**
    * @tc.name: parse_test001
    * @tc.desc: Test function of PKCS7Data::Parse() interface for SUCCESS .
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, parse_test002, testing::ext::TestSize.Level1)
{
    std::string p7b;
    FileUtils::ReadFile(VERIFY_PROFILE_IN_FILE, p7b);
    PKCS7Data p7;
    std::vector<int8_t> p7b_(p7b.begin(), p7b.end());
    int result = p7.Parse(p7b_);
    EXPECT_EQ(result, 0);
}

/**
    * @tc.name: parse_test001
    * @tc.desc: Test function of PKCS7Data::Sign() interface for SUCCESS .
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, sign_test001, testing::ext::TestSize.Level1)
{
    std::string content = "signed content data";
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    PKCS7Data p7;
    std::string p7b;
    int result = p7.Sign(content, signer, "SHA384withECDSA", p7b);
    EXPECT_EQ(result, 0);
}

/**
    * @tc.name: parse_test001
    * @tc.desc: Test function of PKCS7Data::Sign() interface for SUCCESS .数据分离
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, sign_test002, testing::ext::TestSize.Level1)
{
    std::string content = "signed content data";
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    PKCS7Data p7(PKCS7_DETACHED_FLAGS);
    std::string p7b;
    int result = p7.Sign(content, signer, "SHA384withECDSA", p7b);
    EXPECT_EQ(result, 0);
}

/**
    * @tc.name: parse_test001
    * @tc.desc: Test function of PKCS7Data::Verify() interface for SUCCESS .数据分离
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, verify_test001, testing::ext::TestSize.Level1)
{
    std::string p7b;
    FileUtils::ReadFile(VERIFY_PROFILE_IN_FILE, p7b);
    PKCS7Data p7;
    int result = p7.Parse(p7b);
    // 验证
    result = p7.Verify();
    EXPECT_EQ(result, 0);
}
/**
    * @tc.name: get_original_raw_data_test001
    * @tc.desc: Test function of PKCS7Data::GetOriginalRawData() interface for SUCCESS .
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, get_original_raw_data_test001, testing::ext::TestSize.Level1)
{
    std::string p7b;
    FileUtils::ReadFile(VERIFY_PROFILE_IN_FILE, p7b);
    PKCS7Data p7;
    int result = p7.Parse(p7b);
    std::string provision;
    result = p7.GetContent(provision);
    EXPECT_EQ(result, 0);
}

/**
    * @tc.name: get_signer_test001
    * @tc.desc: Test function of SignerFactory::GetSigner() interface for SUCCESS .
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, get_signer_test001, testing::ext::TestSize.Level1)
{
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    EXPECT_TRUE(signer != NULL);
}

/**
    * @tc.name: get_crls_test001
    * @tc.desc: Test function of LocalSigner::GetCrls() interface for SUCCESS .
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, get_crls_test001, testing::ext::TestSize.Level1)
{
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    STACK_OF(X509_CRL)* crls = signer->GetCrls();
    EXPECT_TRUE(crls == NULL);
}

/**
    * @tc.name: get_certificates_test001
    * @tc.desc: Test function of LocalSigner::GetCertificates() interface for SUCCESS .
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, get_certificates_test001, testing::ext::TestSize.Level1)
{
    Options options;
    std::string mode = SIGN_PROFILE_MODE;
    std::string keyAlias = SIGN_PROFILE_KEY_ALIAS;
    std::string profileCertFile = SIGN_PROFILE_PROFILE_CERT_FILE;
    std::string signAlg = SIGN_PROFILE_SIGN_ALG;
    std::string keystoreFile = SIGN_PROFILE_KEY_STORE_FILE;
    std::string outFile = SIGN_PROFILE_OUT_FILE;
    std::string inFile = SIGN_PROFILE_IN_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_ALIAS] = keyAlias;
    options[Options::MODE] = mode;
    options[Options::PROFILE_CERT_FILE] = profileCertFile;
    options[Options::SIGN_ALG] = signAlg;
    options[Options::KEY_STORE_FILE] = keystoreFile;
    options[Options::OUT_FILE] = outFile;
    options[Options::IN_FILE] = inFile;
    options[Options::KEY_RIGHTS] = keypwd;
    options[Options::KEY_STORE_RIGHTS] = keyStorePwd;

    LocalizationAdapter adapter(&options);
    SignerFactory factory;
    std::shared_ptr<Signer> signer = factory.GetSigner(adapter);
    STACK_OF(X509)* certs = signer->GetCertificates();
    EXPECT_TRUE(certs != NULL);
}

/**
    * @tc.name: generate_signeddata_test001
    * @tc.desc: Test function of BCSignedDataGenerator::GenerateSignedData() interface for SUCCESS .set ownerid and
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, generate_signeddata_test001, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::MODE] = SIGN_APP_MODE;
    options[Options::KEY_ALIAS] = SIGN_APP_KEY_ALIAS;
    options[Options::APP_CERT_FILE] = SIGN_APP_APP_CERT_FILE;
    options[Options::PROFILE_FILE] = SIGN_APP_PROFILE_FILE;
    options[Options::IN_FILE] = SIGN_APP_IN_FILE;
    options[Options::SIGN_ALG] = SIGN_APP_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_APP_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_APP_OUT_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_RIGHTS] = keyStorePwd;
    options[Options::KEY_STORE_RIGHTS] = keypwd;

    // config设置算法 signer
    SignerConfig config;
    config.SetOptions(&options);
    SignatureAlgorithmHelper algClass;
    std::vector<SignatureAlgorithmHelper> sigs;
    sigs.resize(1);
    sigs[0].m_id = SignatureAlgorithmId::ECDSA_WITH_SHA256;
    config.SetSignatureAlgorithms(sigs);

    std::string content = "digest content";
    std::string signedData;
    std::shared_ptr<BCSignedDataGenerator> signedDataGenerator =
        std::make_shared<BCSignedDataGenerator>();
    signedDataGenerator->SetOwnerId(OWNERID_OID);
    int result = signedDataGenerator->GenerateSignedData(content, &config, signedData);
    EXPECT_EQ(result, 0);
}

/**
    * @tc.name: generate_signeddata_test002
    * @tc.desc: Test function of BCPkcs7Generator::GenerateSignedData() interface for SUCCESS
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
    */
HWTEST_F(SignProfileTest, generate_signeddata_test002, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::MODE] = SIGN_APP_MODE;
    options[Options::KEY_ALIAS] = SIGN_APP_KEY_ALIAS;
    options[Options::APP_CERT_FILE] = SIGN_APP_APP_CERT_FILE;
    options[Options::PROFILE_FILE] = SIGN_APP_PROFILE_FILE;
    options[Options::IN_FILE] = SIGN_APP_IN_FILE;
    options[Options::SIGN_ALG] = SIGN_APP_SIGN_ALG;
    options[Options::KEY_STORE_FILE] = SIGN_APP_KEY_STORE_FILE;
    options[Options::OUT_FILE] = SIGN_APP_OUT_FILE;
    char keyStorePwd[] = "123456";
    char keypwd[] = "123456";
    options[Options::KEY_RIGHTS] = keyStorePwd;
    options[Options::KEY_STORE_RIGHTS] = keypwd;

    // config设置算法 signer
    SignerConfig config;
    config.SetOptions(&options);
    SignatureAlgorithmHelper algClass;
    std::vector<SignatureAlgorithmHelper> sigs;
    sigs.resize(1);
    sigs[0].m_id = SignatureAlgorithmId::ECDSA_WITH_SHA256;
    config.SetSignatureAlgorithms(sigs);

    std::string content = "digest content";
    std::string signedData;
    std::shared_ptr<Pkcs7Generator> pkcs7Generator = std::make_shared<BCPkcs7Generator>();
    int result = pkcs7Generator->GenerateSignedData(content, &config, signedData);
    EXPECT_EQ(result, 0);
}
/**
    * @tc.name: run_sign_app_test001
    * @tc.desc: Test function of HapSignTool::RunSignApp() interface for SUCCESS.
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
*/
HWTEST_F(SignProfileTest, run_sign_app_test001, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = SIGN_APP_MODE;
    (*params)["keyAlias"] = SIGN_APP_KEY_ALIAS;
    (*params)["signAlg"] = SIGN_APP_SIGN_ALG;
    (*params)["appCertFile"] = SIGN_APP_APP_CERT_FILE;
    (*params)["profileFile"] = SIGN_APP_PROFILE_FILE;
    (*params)["inFile"] = SIGN_APP_IN_FILE;
    (*params)["keystoreFile"] = SIGN_APP_KEY_STORE_FILE;
    (*params)["outFile"] = SIGN_APP_OUT_FILE;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["inForm"] = std::string("zip");
    (*params)["profileSigned"] = std::string("1");
    (*params)["signCode"] = std::string("1");

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, true);
}


/**
    * @tc.name: run_sign_app_test002
    * @tc.desc: Test function of HapSignTool::RunSignApp() interface for SUCCESS.
    * @tc.size: MEDIUM
    * @tc.type: FUNC
    * @tc.level Level 1
    * @tc.require: SR000H63TL
*/
HWTEST_F(SignProfileTest, run_sign_app_test002, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["mode"] = SIGN_APP_MODE;
    (*params)["keyAlias"] = SIGN_APP_KEY_ALIAS;
    (*params)["signAlg"] = SIGN_APP_SIGN_ALG;
    (*params)["appCertFile"] = SIGN_APP_APP_CERT_FILE;
    (*params)["profileFile"] = SIGN_APP_PROFILE_FILE;
    (*params)["inFile"] = SIGN_APP_IN_FILE;
    (*params)["keystoreFile"] = SIGN_APP_KEY_STORE_FILE;
    (*params)["outFile"] = SIGN_APP_OUT_FILE;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["inForm"] = std::string("zip");
    (*params)["profileSigned"] = std::string("0");
    (*params)["signCode"] = std::string("0");

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

}
}