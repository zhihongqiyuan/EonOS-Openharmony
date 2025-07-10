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
#include "code_signing.h"
#include "zip_signer.h"
#include "sign_provider.h"
#include "local_sign_provider.h"
#include "unzip_handle_param.h"

namespace OHOS {
namespace SignatureTools {

class CodeSigningTest : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
        (void)rename("./codeSigning/entry-default-unsigned-so.txt", "./codeSigning/entry-default-unsigned-so.hap");
        (void)rename("./codeSigning/entry-default-unsigned-so.txt", "./codeSigning/entry-default-unsigned-so.elf");
        (void)rename("./codeSigning/unsigned-file.txt", "./codeSigning/unsigned-file.hap");
    }
    static void TearDownTestCase()
    {
    }
    void SetUp()
    {
    };
    void TearDown()
    {
    };
};

/**
 * @tc.name: generateSignature001
 * @tc.desc: generate signature with signed data
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, generateSignature001, testing::ext::TestSize.Level1)
{
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

    CodeSigning codeSigning(&signerConfig);

    std::vector<int8_t> signedData = {70, 83, 86, 101, 114, 105, 116, 121, 1,
        0, 32, 0, -82, 98, 15, 102, 95, -26, -90, 88, 83, 8, -42, -65, -121,
        117, -43, -95, -102, -56, 109, 93, 25, -9, -88, 44, -25, 119, -39, -68,
        -15, 11, 123, -80};
    std::string ownerID;
    std::vector<int8_t> ret;
    bool flag = codeSigning.GenerateSignature(signedData, ownerID, ret);
    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: generateSignature002
 * @tc.desc: generate signature without signed data
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, generateSignature002, testing::ext::TestSize.Level1)
{
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

    CodeSigning codeSigning(&signerConfig);

    std::vector<int8_t> signedData;
    std::string ownerID;
    std::vector<int8_t> ret;
    bool flag = codeSigning.GenerateSignature(signedData, ownerID, ret);
    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: GetNativeEntriesFromHap
 * @tc.desc: Test function of CodeSigningTest::GetNativeEntriesFromHap() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, GetNativeEntriesFromHap, testing::ext::TestSize.Level1)
{
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

    CodeSigning codeSigning(&signerConfig);

    std::string packageName = "./codeSigning/entry-default-unsigned-so.hap";
    std::vector<std::pair<std::string, SignInfo>> ret;
    std::string ownerID;
    UnzipHandleParam param(ret, ownerID, true);
    bool flag = codeSigning.GetNativeEntriesFromHap(packageName, param);
    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: getTimestamp
 * @tc.desc: Test function of CodeSigningTest::getTimestamp() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, getTimestamp, testing::ext::TestSize.Level1)
{
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

    CodeSigning codeSigning(&signerConfig);
    codeSigning.UpdateCodeSignBlock();
    int64_t timeStamp = codeSigning.GetTimestamp();
    EXPECT_NE(timeStamp, 0);
}

/**
 * @tc.name: isNativeFile
 * @tc.desc: Test function of CodeSigningTest::isNativeFile() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, isNativeFile, testing::ext::TestSize.Level1)
{
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

    CodeSigning codeSigning(&signerConfig);
    std::string input = "libs/arm64-v8a/libc++_shared.so";
    bool flag = codeSigning.IsNativeFile(input);
    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: signFile
 * @tc.desc: Test function of CodeSigningTest::signFile() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, signFile, testing::ext::TestSize.Level1)
{
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

    CodeSigning codeSigning(&signerConfig);

    std::ifstream inputStream;
    inputStream.open("./codeSigning/entry-default-unsigned-so.hap", std::ios::binary);
    int64_t fileSize = 3479976;
    bool storeTree = true;
    int64_t fsvTreeOffset = 1024;
    std::string ownerID;
    std::pair<SignInfo, std::vector<int8_t>> ret;
    bool flag = codeSigning.SignFile(inputStream, fileSize, storeTree, fsvTreeOffset, ownerID, ret);
    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: signNativeLibs
 * @tc.desc: Test function of CodeSigningTest::signNativeLibs() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, signNativeLibs, testing::ext::TestSize.Level1)
{
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

    CodeSigning codeSigning(&signerConfig);
    std::string input = "./codeSigning/entry-default-unsigned-so.hap";
    std::string ownerID;
    bool flag = codeSigning.SignNativeLibs(input, ownerID);
    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: AppendCodeSignBlock
 * @tc.desc: Test function of CodeSigningTest::AppendCodeSignBlock() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, AppendCodeSignBlock, testing::ext::TestSize.Level1)
{
    std::shared_ptr<LocalSignProvider> signProvider = std::make_unique<LocalSignProvider>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string signCode = "1";
    std::string appCertFile = "./codeSigning/app-release1.pem";
    std::string profileFile = "./codeSigning/signed-profile.p7b";
    std::string inFile = "./codeSigning/entry-default-unsigned-so.elf";
    std::string keystoreFile = "./codeSigning/ohtest.p12";
    std::string outFile = "./codeSigning/entry-default-signed-so.hap";
    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";
    std::string inForm = "elf";

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
    (*params)["inForm"] = inForm;

    bool ret = signProvider->Sign(params.get());
    EXPECT_EQ(ret, false);
}

void SetParamsMap(std::map<std::string, std::string>& params)
{
    params["keyPwd"] = "123456";
    params["mode"] = "localSign";
    params["keyAlias"] = "oh-app1-key-v1";
    params["signAlg"] = "SHA256withECDSA";
    params["appCertFile"] = "./hapSign/app-release1.pem";
    params["signCode"] = "1";
    params["compatibleVersion"] = "9";
    params["outFile"] = "./hapSign/entry-default-signed.elf";
    params["profileFile"] = "./hapSign/signed-profile.p7b";
    params["keystorePwd"] = "123456";
    params["keystoreFile"] = "./hapSign/ohtest.jks";
    params["inFile"] = "./codeSigning/unsigned-file.hap";
    params["profileSigned"] = "1";
    params["inForm"] = "elf";
        std::string  provision = "{\"app-distribution-type\": \"app_gallery\",\"bundle-info\":{\"app-"
        "feature\":\"hos_system_app\",\"bundle-name\":\"com.OpenHarmony.app.test\",\"developer-id\":\"O"
        "penHarmony\",\"distribution-certificate\":\"-----BEGIN CERTIFICATE-----\\n"
        "MIICMzCCAbegAwIBAgIEaOC/zDAMBggqhkjOPQQDAwUAMGMxCzAJBgNVBAYTAkNO\\n"
        "MRQwEgYDVQQKEwtPcGVuSGFybW9ueTEZMBcGA1UECxMQT3Blbkhhcm1vbnkgVGVh\\n"
        "bTEjMCEGA1UEAxMaT3Blbkhhcm1vbnkgQXBwbGljYXRpb24gQ0EwHhcNMjEwMjAy\\n"
        "MTIxOTMxWhcNNDkxMjMxMTIxOTMxWjBoMQswCQYDVQQGEwJDTjEUMBIGA1UEChML\\n"
        "T3Blbkhhcm1vbnkxGTAXBgNVBAsTEE9wZW5IYXJtb255IFRlYW0xKDAmBgNVBAMT\\n"
        "H09wZW5IYXJtb255IEFwcGxpY2F0aW9uIFJlbGVhc2UwWTATBgcqhkjOPQIBBggq\\n"
        "hkjOPQMBBwNCAATbYOCQQpW5fdkYHN45v0X3AHax12jPBdEDosFRIZ1eXmxOYzSG\\n"
        "JwMfsHhUU90E8lI0TXYZnNmgM1sovubeQqATo1IwUDAfBgNVHSMEGDAWgBTbhrci\\n"
        "FtULoUu33SV7ufEFfaItRzAOBgNVHQ8BAf8EBAMCB4AwHQYDVR0OBBYEFPtxruhl\\n"
        "cRBQsJdwcZqLu9oNUVgaMAwGCCqGSM49BAMDBQADaAAwZQIxAJta0PQ2p4DIu/ps\\n"
        "LMdLCDgQ5UH1l0B4PGhBlMgdi2zf8nk9spazEQI/0XNwpft8QAIwHSuA2WelVi/o\\n"
        "zAlF08DnbJrOOtOnQq5wHOPlDYB4OtUzOYJk9scotrEnJxJzGsh/\\n"
        "-----END CERTIFICATE-----\\n"
        "\"},\"debug-info\":{\"device-id-type\":\"udid\",\"device-ids\":[\"69C7505BE341BDA5948C3C0CB"
        "44ABCD530296054159EFE0BD16A16CD0129CC42\",\"7EED06506FCE6325EB2E2FAA019458B856AB10493A6718C76"
        "79A73F958732865\"]},\"issuer\":\"pki_internal\",\"permissions\":{\"restricted-permissions\":"
        "[\"\"]},\"type\":\"release\",\"uuid\":\"fe686e1b-3770-4824-a938-961b140a7c98\",\"validity\":"
        "{\"not-after\":1705127532,\"not-before\":1610519532},\"version-code\":1,\"version-name\":\"1.0.0\"}";
    params["profileContent"] = provision;
}

void SetOptions(Options* options)
{
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
    static char keyPwd[] = "123456";
    static char keystorePwd[] = "123456";

    (*options)["mode"] = "localSign";
    (*options)["keyAlias"] = keyAlias;
    (*options)["signAlg"] = signAlg;
    (*options)["signCode"] = signCode;
    (*options)["appCertFile"] = appCertFile;
    (*options)["profileFile"] = profileFile;
    (*options)["inFile"] = inFile;
    (*options)["keystoreFile"] = keystoreFile;
    (*options)["outFile"] = outFile;
    (*options)["inForm"] = inForm;
    (*options)["keyPwd"] = keyPwd;
    (*options)["keystorePwd"] = keystorePwd;
}

/**
 * @tc.name: GetElfCodeSignBlock
 * @tc.desc: get elf code sign block without inForm
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, GetElfCodeSignBlock001, testing::ext::TestSize.Level1)
{
    // success
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(9);

    std::map<std::string, std::string> params;
    SetParamsMap(params);
    signerConfig.FillParameters(params);

    ContentDigestAlgorithm contentDigestAlgorithm("SHA-256", 32);
    std::pair<std::string, void*> signatureAlgAndParams("SHA256withECDSA", nullptr);
    SignatureAlgorithmHelper signatureAlgorithm(SignatureAlgorithmId::ECDSA_WITH_SHA256, "ECDSA_WITH_SHA256",
                                                contentDigestAlgorithm, signatureAlgAndParams);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    signatureAlgorithms.push_back(signatureAlgorithm);
    signerConfig.SetSignatureAlgorithms(signatureAlgorithms);

    std::shared_ptr<Options> options = std::make_shared<Options>();
    SetOptions(options.get());
    signerConfig.SetOptions(options.get());
    signerConfig.GetSigner();

    CodeSigning codeSigning(&signerConfig);
    std::string inputFile = "./hapSign/tmpFile1718344274";
    int64_t offset = 29184024;
    std::vector<int8_t> codesignData;
    std::string inForm = "elf";
    bool ret = codeSigning.GetElfCodeSignBlock(inputFile, offset, inForm, params.at("profileContent"), codesignData);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: GetElfCodeSignBlock002
 * @tc.desc: get elf code sign block with bin inForm
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, GetElfCodeSignBlock002, testing::ext::TestSize.Level1)
{
    // inForm error
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(9);

    std::map<std::string, std::string> params;
    SetParamsMap(params);
    params["inForm"] = "bin";
    signerConfig.FillParameters(params);

    ContentDigestAlgorithm contentDigestAlgorithm("SHA-256", 32);
    std::pair<std::string, void*> signatureAlgAndParams("SHA256withECDSA", nullptr);
    SignatureAlgorithmHelper signatureAlgorithm(SignatureAlgorithmId::ECDSA_WITH_SHA256, "ECDSA_WITH_SHA256",
                                                contentDigestAlgorithm, signatureAlgAndParams);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    signatureAlgorithms.push_back(signatureAlgorithm);
    signerConfig.SetSignatureAlgorithms(signatureAlgorithms);

    std::shared_ptr<Options> options = std::make_shared<Options>();
    SetOptions(options.get());
    (*options)["inForm"] = "bin";
    signerConfig.SetOptions(options.get());
    signerConfig.GetSigner();

    CodeSigning codeSigning(&signerConfig);
    std::string inputFile = "./hapSign/tmpFile1718344274";
    int64_t offset = 29184024;
    std::vector<int8_t> codesignData;
    std::string inForm = "bin";
    bool ret = codeSigning.GetElfCodeSignBlock(inputFile, offset, inForm, params.at("profileContent"), codesignData);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: GetElfCodeSignBlock003
 * @tc.desc: get elf code sign block with elf inForm
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, GetElfCodeSignBlock003, testing::ext::TestSize.Level1)
{
    // inFile error
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(9);

    std::map<std::string, std::string> params;
    SetParamsMap(params);
    params["inFile"] = "./hapSign_test/tmpFile1718344274";
    signerConfig.FillParameters(params);

    ContentDigestAlgorithm contentDigestAlgorithm("SHA-256", 32);
    std::pair<std::string, void*> signatureAlgAndParams("SHA256withECDSA", nullptr);
    SignatureAlgorithmHelper signatureAlgorithm(SignatureAlgorithmId::ECDSA_WITH_SHA256, "ECDSA_WITH_SHA256",
                                                contentDigestAlgorithm, signatureAlgAndParams);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    signatureAlgorithms.push_back(signatureAlgorithm);
    signerConfig.SetSignatureAlgorithms(signatureAlgorithms);

    std::shared_ptr<Options> options = std::make_shared<Options>();
    SetOptions(options.get());
    (*options)["inFile"] = "./hapSign_test/tmpFile1718344274";
    signerConfig.SetOptions(options.get());
    signerConfig.GetSigner();

    CodeSigning codeSigning(&signerConfig);
    std::string inputFile = params.at("inFile");
    int64_t offset = 29184024;
    std::vector<int8_t> codesignData;
    std::string inForm = "elf";
    bool ret = codeSigning.GetElfCodeSignBlock(inputFile, offset, inForm, params.at("profileContent"), codesignData);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: GetElfCodeSignBlock004
 * @tc.desc: get elf code sign block without input file
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSigningTest, GetElfCodeSignBlock004, testing::ext::TestSize.Level1)
{
    // signatureAlgorithm set DSA_WITH_SHA256 error
    SignerConfig signerConfig;
    signerConfig.SetCompatibleVersion(9);

    std::map<std::string, std::string> params;
    SetParamsMap(params);
    signerConfig.FillParameters(params);

    ContentDigestAlgorithm contentDigestAlgorithm("SHA-256", 32);
    std::pair<std::string, void*> signatureAlgAndParams("SHA256withECDSA", nullptr);
    SignatureAlgorithmHelper signatureAlgorithm(SignatureAlgorithmId::DSA_WITH_SHA256, "ECDSA_WITH_SHA256",
                                                contentDigestAlgorithm, signatureAlgAndParams);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    signatureAlgorithms.push_back(signatureAlgorithm);
    signerConfig.SetSignatureAlgorithms(signatureAlgorithms);

    std::shared_ptr<Options> options = std::make_shared<Options>();
    SetOptions(options.get());
    signerConfig.SetOptions(options.get());
    signerConfig.GetSigner();

    CodeSigning codeSigning(&signerConfig);
    std::string inputFile = params.at("inFile");
    int64_t offset = 29184024;
    std::vector<int8_t> codesignData;
    std::string inForm = "elf";
    bool ret = codeSigning.GetElfCodeSignBlock(inputFile, offset, inForm, params.at("profileContent"), codesignData);
    EXPECT_EQ(ret, false);
}
} // namespace SignatureTools
} // namespace OHOS