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
#include "cert_tools.h"

namespace OHOS {
namespace SignatureTools {
class CertToolsTest : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
    };
    static void TearDownTestCase()
    {
    };
    void SetUp()
    {
    };
    void TearDown()
    {
    };
};

/**
 * @tc.name: cert_tools_test_001
 * @tc.desc: parameter basicConstraintsCritical connot convert bool
 * @tc.type: FUNC
 */
HWTEST_F(CertToolsTest, cert_tools_test_001, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::BASIC_CONSTRAINTS_CRITICAL] = std::string("a");
    options[Options::BASIC_CONSTRAINTS] = std::string("1");
    bool result = CertTools::SetBisicConstraints(&options, nullptr);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: cert_tools_test_002
 * @tc.desc: parameter basicConstraintsCa connot convert bool
 * @tc.type: FUNC
 */
HWTEST_F(CertToolsTest, cert_tools_test_002, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::BASIC_CONSTRAINTS_CRITICAL] = std::string("1");
    options[Options::BASIC_CONSTRAINTS] = std::string("1");
    options[Options::BASIC_CONSTRAINTS_CA] = std::string("a");
    bool result = CertTools::SetBisicConstraints(&options, nullptr);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: cert_tools_test_003
 * @tc.desc: This function tests success for interface SetBisicConstraints
 * @tc.type: FUNC
 */
HWTEST_F(CertToolsTest, cert_tools_test_003, testing::ext::TestSize.Level1)
{
    Options options;
    options[Options::BASIC_CONSTRAINTS_CRITICAL] = std::string("1");
    options[Options::BASIC_CONSTRAINTS] = std::string("1");
    options[Options::BASIC_CONSTRAINTS_CA] = std::string("1");
    X509* cert = X509_new();
    bool ret = CertTools::SetBisicConstraints(&options, cert);
    EXPECT_EQ(ret, true);
    X509_free(cert);
}

/**
 * @tc.name: cert_tools_test_004
 * @tc.desc: This function tests failed for interface SignForSubCert due to parameter caPrikey is null
 * @tc.type: FUNC
 */
HWTEST_F(CertToolsTest, cert_tools_test_004, testing::ext::TestSize.Level1)
{
    Options options;
    KeyStoreHelper stroe;
    EVP_PKEY* caPrikey = stroe.GenerateKeyPair("ECC", 256);
    X509* cert = X509_new();
    X509_REQ* subcsr = X509_REQ_new();
    X509_REQ_set_pubkey(subcsr, caPrikey);
    X509_REQ* rootcsr = X509_REQ_new();
    bool ret = CertTools::SignForSubCert(cert, subcsr, rootcsr, nullptr, &options);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: cert_tools_test_005
 * @tc.desc: This function tests failed for interface SetSubjectForCert due to parameter cert is null
 * @tc.type: FUNC
 */
HWTEST_F(CertToolsTest, cert_tools_test_005, testing::ext::TestSize.Level1)
{
    X509_REQ* certReq = X509_REQ_new();
    bool ret = CertTools::SetSubjectForCert(certReq, nullptr);
    EXPECT_EQ(ret, false);
    X509_REQ_free(certReq);
}

/**
 * @tc.name: cert_tools_test_006
 * @tc.desc: This function tests failed for interface GenerateSubCert due to parameter subject is error
 * @tc.type: FUNC
 */
HWTEST_F(CertToolsTest, cert_tools_test_006, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char isksPwd[] = "123456";
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    bool ret = CertTools::GenerateSubCert(keyPair, csr, params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: cert_tools_test_007
 * @tc.desc: This function tests failed for interface SetCertPublickKey due to parameter subjectCsr no set PublickKey
 * @tc.type: FUNC
 */
HWTEST_F(CertToolsTest, cert_tools_test_007, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    X509_REQ* certReq = X509_REQ_new();
    bool ret = CertTools::SetCertPublickKey(cert, certReq);
    EXPECT_EQ(ret, false);
    X509_REQ_free(certReq);
    X509_free(cert);
}

/**
 * @tc.name: cert_tools_test_008
 * @tc.desc: This function tests failed for interface SetCertPublickKey due to parameter cert is null
 * @tc.type: FUNC
 */
HWTEST_F(CertToolsTest, cert_tools_test_008, testing::ext::TestSize.Level1)
{
    X509_REQ* certReq = X509_REQ_new();
    KeyStoreHelper stroe;
    EVP_PKEY* Prikey = stroe.GenerateKeyPair("ECC", 256);
    X509_REQ_set_pubkey(certReq, Prikey);
    bool ret = CertTools::SetCertPublickKey(nullptr, certReq);
    EXPECT_EQ(ret, false);
    X509_REQ_free(certReq);
}
} // namespace SignatureTools
} // namespace OHOS