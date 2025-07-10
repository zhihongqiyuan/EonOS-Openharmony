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
#include <gtest/gtest.h>
#include "bc_signeddata_generator.h"
#include "code_signing.h"
#include "options.h"

namespace OHOS {
namespace SignatureTools {

class BCSignedDataGeneratorTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void BCSignedDataGeneratorTest::SetUpTestCase(void)
{
    (void)rename("./codeSigning/entry-default-unsigned-so.txt", "./codeSigning/entry-default-unsigned-so.hap");
}

void BCSignedDataGeneratorTest::TearDownTestCase(void)
{
}

void BCSignedDataGeneratorTest::SetUp()
{
}

void BCSignedDataGeneratorTest::TearDown()
{
}

/**
 * @tc.name: GenerateSignedData001
 * @tc.desc: Test function of BCSignedDataGenerator::GenerateSignedData() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BCSignedDataGeneratorTest, GenerateSignedData001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BCSignedDataGenerator> api = std::make_shared<BCSignedDataGenerator>();

    std::vector<int8_t> arr = { 70, 83, 86, 101, 114, 105, 116, 121, 1, 0, 32, 0, -61, -44,
        -75, -90, 72, 25, -110, -41, -90, 12, 102, -121, 99, 106, -25, -47, -120, 113, -32,
        30, 62, -64, 23, 114, 12, 25, 105, 121, 116, 88, -20, 4 };
    std::string signed_data(arr.begin(), arr.end());

    // 1.构造SignerConfig
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(9);

    std::map<std::string, std::string> params;
    params["keyPwd"] = "123456";
    params["mode"] = "localSign";
    params["keyAlias"] = "oh-app1-key-v1";
    params["signAlg"] = "SHA256withECDSA";
    params["appCertFile"] = "./codeSigning/app-release1.pem";
    params["signCode"] = "1";
    params["compatibleVersion"] = "9";
    params["outFile"] = "./codeSigning/entry-default-signed-so.hap";
    params["profileFile"] = "./codeSigning/signed-profile.p7b";
    params["keystorePwd"] = "123456";
    params["keystoreFile"] = "./codeSigning/ohtest.jks";
    params["inFile"] = "./codeSigning/entry-default-unsigned-so.hap";
    params["profileSigned"] = "1";
    signerConfig.FillParameters(params);

    ContentDigestAlgorithm contentDigestAlgorithm("SHA-256", 32);
    std::pair<std::string, void*> signatureAlgAndParams("SHA256withECDSA", nullptr);
    SignatureAlgorithmHelper signatureAlgorithm(SignatureAlgorithmId::DSA_WITH_SHA256, "ECDSA_WITH_SHA256",
                                               contentDigestAlgorithm, signatureAlgAndParams);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    signatureAlgorithms.push_back(signatureAlgorithm);
    signerConfig.SetSignatureAlgorithms(signatureAlgorithms);

    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./codeSigning/entry-default-signed-so.hap"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./codeSigning/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./codeSigning/ohtest.jks"));
    options.emplace("appCertFile", std::string("./codeSigning/app-release1.pem"));
    options.emplace("inFile", std::string("./codeSigning/entry-default-unsigned-so.hap"));
    signerConfig.SetOptions(&options);

    std::string ret;
    int result = api->GenerateSignedData(signed_data, &signerConfig, ret);

    EXPECT_NE(result, 0);
}

/**
 * @tc.name: GenerateSignedData002
 * @tc.desc: Test function of BCSignedDataGenerator::GenerateSignedData() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BCSignedDataGeneratorTest, GenerateSignedData002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BCSignedDataGenerator> api = std::make_shared<BCSignedDataGenerator>();

    std::string signed_data;
    SignerConfig signerConfig;
    std::string ret;
    int result = api->GenerateSignedData(signed_data, &signerConfig, ret);

    EXPECT_NE(result, 0);
}

/**
 * @tc.name: GenerateSignedData003
 * @tc.desc: Test function of BCSignedDataGenerator::GenerateSignedData() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BCSignedDataGeneratorTest, GenerateSignedData003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BCSignedDataGenerator> api = std::make_shared<BCSignedDataGenerator>();

    std::vector<int8_t> arr = { 70, 83, 86, 101, 114, 105, 116, 121, 1, 0, 32, 0, -61, -44,
        -75, -90, 72, 25, -110, -41, -90, 12, 102, -121, 99, 106, -25, -47, -120, 113, -32,
        30, 62, -64, 23, 114, 12, 25, 105, 121, 116, 88, -20, 4 };
    std::string signed_data(arr.begin(), arr.end());
    std::string ret;
    int result = api->GenerateSignedData(signed_data, NULL, ret);
    EXPECT_NE(result, 0);
}

/**
 * @tc.name: GetSigAlg001
 * @tc.desc: Test function of BCSignedDataGenerator::GetSigAlg() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BCSignedDataGeneratorTest, GetSigAlg001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BCSignedDataGenerator> api = std::make_shared<BCSignedDataGenerator>();

    // 1.构造SignerConfig
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(9);

    std::map<std::string, std::string> params;
    params["keyPwd"] = "123456";
    params["mode"] = "localSign";
    params["keyAlias"] = "oh-app1-key-v1";
    params["signAlg"] = "SHA256withECDSA";
    params["appCertFile"] = "./codeSigning/app-release1.pem";
    params["signCode"] = "1";
    params["compatibleVersion"] = "9";
    params["outFile"] = "./codeSigning/entry-default-signed-so.hap";
    params["profileFile"] = "./codeSigning/signed-profile.p7b";
    params["keystorePwd"] = "123456";
    params["keystoreFile"] = "./codeSigning/ohtest.jks";
    params["inFile"] = "./codeSigning/entry-default-unsigned-so.hap";
    params["profileSigned"] = "1";
    signerConfig.FillParameters(params);

    ContentDigestAlgorithm contentDigestAlgorithm("SHA-256", 32);
    std::pair<std::string, void*> signatureAlgAndParams("SHA256withECDSA", nullptr);
    SignatureAlgorithmHelper signatureAlgorithm(SignatureAlgorithmId::ECDSA_WITH_SHA256, "ECDSA_WITH_SHA256",
                                               contentDigestAlgorithm, signatureAlgAndParams);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    signatureAlgorithms.push_back(signatureAlgorithm);
    signerConfig.SetSignatureAlgorithms(signatureAlgorithms);

    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./codeSigning/entry-default-signed-so.hap"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./codeSigning/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./codeSigning/ohtest.jks"));
    options.emplace("appCertFile", std::string("./codeSigning/app-release1.pem"));
    options.emplace("inFile", std::string("./codeSigning/entry-default-unsigned-so.hap"));
    signerConfig.SetOptions(&options);

    std::string sigAlg;
    int ret = api->GetSigAlg(&signerConfig, sigAlg);
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: GetSigAlg002
 * @tc.desc: Test function of BCSignedDataGenerator::GetSigAlg() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BCSignedDataGeneratorTest, GetSigAlg002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BCSignedDataGenerator> api = std::make_shared<BCSignedDataGenerator>();

    // 1.构造SignerConfig
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(9);

    std::map<std::string, std::string> params;
    params["keyPwd"] = "123456";
    params["mode"] = "localSign";
    params["keyAlias"] = "oh-app1-key-v1";
    params["signAlg"] = "SHA256withECDSA";
    params["appCertFile"] = "./codeSigning/app-release1.pem";
    params["signCode"] = "1";
    params["compatibleVersion"] = "9";
    params["outFile"] = "./codeSigning/entry-default-signed-so.hap";
    params["profileFile"] = "./codeSigning/signed-profile.p7b";
    params["keystorePwd"] = "123456";
    params["keystoreFile"] = "./codeSigning/ohtest.jks";
    params["inFile"] = "./codeSigning/entry-default-unsigned-so.hap";
    params["profileSigned"] = "1";
    signerConfig.FillParameters(params);

    ContentDigestAlgorithm contentDigestAlgorithm("SHA-256", 32);
    std::pair<std::string, void*> signatureAlgAndParams("SHA256withECDSA", nullptr);
    SignatureAlgorithmHelper signatureAlgorithm(SignatureAlgorithmId::ECDSA_WITH_SHA256, "ECDSA_WITH_SHA256",
                                               contentDigestAlgorithm, signatureAlgAndParams);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    signatureAlgorithms.push_back(signatureAlgorithm);

    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./codeSigning/entry-default-signed-so.hap"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./codeSigning/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./codeSigning/ohtest.jks"));
    options.emplace("appCertFile", std::string("./codeSigning/app-release1.pem"));
    options.emplace("inFile", std::string("./codeSigning/entry-default-unsigned-so.hap"));
    signerConfig.SetOptions(&options);

    std::string sigAlg;
    int ret = api->GetSigAlg(&signerConfig, sigAlg);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: GetSigAlg003
 * @tc.desc: Test function of BCSignedDataGenerator::GetSigAlg() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BCSignedDataGeneratorTest, GetSigAlg003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BCSignedDataGenerator> api = std::make_shared<BCSignedDataGenerator>();

    // 1.构造SignerConfig
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(9);

    std::map<std::string, std::string> params;
    params["keyPwd"] = "123456";
    params["mode"] = "localSign";
    params["keyAlias"] = "oh-app1-key-v1";
    params["signAlg"] = "SHA256withECDSA";
    params["appCertFile"] = "./codeSigning/app-release1.pem";
    params["signCode"] = "1";
    params["compatibleVersion"] = "9";
    params["outFile"] = "./codeSigning/entry-default-signed-so.hap";
    params["profileFile"] = "./codeSigning/signed-profile.p7b";
    params["keystorePwd"] = "123456";
    params["keystoreFile"] = "./codeSigning/ohtest.jks";
    params["inFile"] = "./codeSigning/entry-default-unsigned-so.hap";
    params["profileSigned"] = "1";
    signerConfig.FillParameters(params);

    ContentDigestAlgorithm contentDigestAlgorithm("SHA-256", 32);
    std::pair<std::string, void*> signatureAlgAndParams("SHA256withECDSA", nullptr);
    SignatureAlgorithmHelper signatureAlgorithm(SignatureAlgorithmId::ECDSA_WITH_SHA384, "ECDSA_WITH_SHA384",
                                               contentDigestAlgorithm, signatureAlgAndParams);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    signatureAlgorithms.push_back(signatureAlgorithm);
    signerConfig.SetSignatureAlgorithms(signatureAlgorithms);

    Options options;
    options.emplace("mode", std::string("localSign"));
    options.emplace("keyPwd", std::string("123456"));
    options.emplace("outFile", std::string("./codeSigning/entry-default-signed-so.hap"));
    options.emplace("keyAlias", std::string("oh-app1-key-v1"));
    options.emplace("profileFile", std::string("./codeSigning/signed-profile.p7b"));
    options.emplace("signAlg", std::string("SHA256withECDSA"));
    options.emplace("keystorePwd", std::string("123456"));
    options.emplace("keystoreFile", std::string("./codeSigning/ohtest.jks"));
    options.emplace("appCertFile", std::string("./codeSigning/app-release1.pem"));
    options.emplace("inFile", std::string("./codeSigning/entry-default-unsigned-so.hap"));
    signerConfig.SetOptions(&options);

    std::string ownerID;
    api->SetOwnerId(ownerID);

    std::string sigAlg;
    int ret = api->GetSigAlg(&signerConfig, sigAlg);
    EXPECT_EQ(ret, 0);
}
} // namespace SignatureTools
} // namespace OHOS