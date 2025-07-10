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
#include <openssl/ssl.h>
#include <gtest/gtest.h>
#include "signature_tools_log.h"
#include "options.h"
#include "sign_tool_service_impl.h"
#include "cert_tools.h"
#include "params_run_tool.h"
#include "localization_adapter.h"
#include "fs_digest_utils.h"
#include "constant.h"
#include <cstdio>
#include <cstring>

namespace OHOS {
namespace SignatureTools {

class GenerateCaTest : public testing::Test {
public:
    static void SetUpTestCase()
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
 * @tc.name: generate_sub_cert_to_file_test_004
 * @tc.desc: Test function of GenerateSubCertToFile() interface for add error branch FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_to_file_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "";
    char secret[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateRootCertificate(keyPair, csr, params.get());
    EXPECT_NE(cert, nullptr);
    bool ret = api->GenerateSubCertToFile(params.get(), keyPair);
    EXPECT_EQ(ret, false);
}
/**
 * @tc.name: generate_sub_cert_to_file_test_005
 * @tc.desc: Test function of GenerateCaToFile() interface for error keyalias FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_to_file_test_005, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECD";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    X509* cert = CertTools::GenerateRootCertificate(keyPair, csr, params.get());
    EXPECT_EQ(cert, nullptr);
    bool ret = api->GenerateSubCertToFile(params.get(), keyPair);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_sub_cert_to_file_test_006
 * @tc.desc: Test function of GenerateCaToFile() interface for error keystoreFile path FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_to_file_test_006, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECD";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    EVP_PKEY* keyPair = nullptr;
    bool ret = api->GenerateSubCertToFile(params.get(), keyPair);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_sub_cert_to_file_test_007
 * @tc.desc: Test function of GenerateCaToFile() interface for error keystoreFile path FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_to_file_test_007, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECD";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    std::string outFile = "/datamt/test/generateCA/sub-ca-test.cer";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlgorithm"] = signAlgorithm;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    (*params)["outFile"] = outFile;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    bool ret = api->GenerateSubCertToFile(params.get(), keyPair);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: set_cert_version_test_001
 * @tc.desc: Test function of SetCertVersion() interface for set cert version FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_cert_version_test_001, testing::ext::TestSize.Level1)
{
    X509* cert = nullptr;
    bool res = CertTools::SetCertVersion(cert, 1);
    EXPECT_EQ(res, false);
}
/**
 * @tc.name: set_pubkey_and_sign_cert_test_001
 * @tc.desc: Test function of SetPubkeyAndSignCert() interface for set cert pubey FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_pubkey_and_sign_cert_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlgorithm"] = signAlgorithm;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlgorithm, issuer);
    EXPECT_NE(issuercsr, nullptr);
    X509* cert = CertTools::GenerateRootCertificate(keyPair, csr, params.get());
    EXPECT_NE(cert, nullptr);
    bool cert1 = CertTools::SetPubkeyAndSignCert(cert, issuercsr, csr, keyPair, params.get());
    EXPECT_NE(cert1, false);
}


/**
 * @tc.name: set_pubkey_and_sign_cert_test_002
 * @tc.desc: Test function of SetPubkeyAndSignCert() interface for sign cert FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_pubkey_and_sign_cert_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlgorithm"] = signAlgorithm;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlgorithm, issuer);
    EXPECT_NE(issuercsr, nullptr);
    X509* cert = nullptr;
    bool cert1 = CertTools::SetPubkeyAndSignCert(cert, issuercsr, csr, keyPair, params.get());
    EXPECT_EQ(cert1, false);
    EXPECT_EQ(cert, nullptr);
}


/**
 * @tc.name: set_pubkey_and_sign_cert_test_003
 * @tc.desc: Test function of SetPubkeyAndSignCert() interface with a empty csr FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_pubkey_and_sign_cert_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* csr = X509_REQ_new();
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlg, issuer);
    X509* cert = nullptr;
    bool cert1 = CertTools::SetPubkeyAndSignCert(cert, issuercsr, csr, keyPair, params.get());
    EXPECT_EQ(cert1, false);
}

/**
 * @tc.name: set_pubkey_and_sign_cert_test_004
 * @tc.desc: Test function of SetPubkeyAndSignCert() interface with a null x509 cert FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_pubkey_and_sign_cert_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/key.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* csr = X509_REQ_new();
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlg, issuer);
    X509* cert = nullptr;
    bool cert1 = CertTools::SetPubkeyAndSignCert(cert, issuercsr, csr, keyPair, params.get());
    EXPECT_EQ(cert1, false);
}

/**
 * @tc.name: set_pubkey_and_sign_cert_test_005
 * @tc.desc: Test function of SetPubkeyAndSignCert() interface with a error issuercsr FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_pubkey_and_sign_cert_test_005, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/key.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* csr = X509_REQ_new();
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlg, issuer);
    X509* cert = nullptr;
    bool cert1 = CertTools::SetPubkeyAndSignCert(cert, issuercsr, csr, keyPair, params.get());
    EXPECT_EQ(cert1, false);
}

/**
 * @tc.name: set_cert_serial_test_003
 * @tc.desc: Test function of SetCertSubjectName() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_cert_serial_test_001, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    X509_REQ* req = X509_REQ_new();
    bool res = CertTools::SetCertSubjectName(cert, req);
    EXPECT_EQ(res, true);
}

/**
 * @tc.name: set_cert_validity_test_001
 * @tc.desc: Test function of SetCertValidityStartAndEnd() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_cert_validity_test_001, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    bool res = CertTools::SetCertValidityStartAndEnd(cert, -1, 2);
    X509_free(cert);
    EXPECT_EQ(res, true);
}


/**
 * @tc.name: X509_certverify_test_001
 * @tc.desc: Test function of X509CertVerify() interface for verify cert SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, X509_certverify_test_001, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    bool ret = api->X509CertVerify(cert, keyPair);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: X509_certverify_test_002
 * @tc.desc: Test function of X509CertVerify() interface with a no match key verify cert FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */

HWTEST_F(GenerateCaTest, X509_certverify_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    X509* cert = X509_new();
    EVP_PKEY* keyPair = EVP_PKEY_new();
    bool ret = api->X509CertVerify(cert, keyPair);
    EXPECT_NE(ret, false);
}


/**
 * @tc.name: X509_certverify_test_003
 * @tc.desc: Test function of X509CertVerify() with a match key verify cert SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, X509_certverify_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
    X509* cert = X509_new();
    bool ret = api->X509CertVerify(cert, keyPair);
    X509_free(cert);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: isempty_test_001
 * @tc.desc: Test function of IsEmpty() interface for judge empty SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, isempty_test_001, testing::ext::TestSize.Level1)
{
    std::string cs = "";
    bool res = FileUtils::IsEmpty(cs);
    EXPECT_EQ(res, true);
}
/**
 * @tc.name: Read_File_By_Offset_And_Length_test_001
 * @tc.desc: Test function of ReadInputByOffsetAndLength() interface for read SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, Read_File_By_Offset_And_Length_test_001, testing::ext::TestSize.Level1)
{
    std::ifstream file("/data/test/generateKeyPair/keypair.p12");
    std::string ret("std::string& ret");
    int res = FileUtils::ReadFileByOffsetAndLength(file, 1, 50, ret);
    EXPECT_EQ(res, 0);
}
/**
 * @tc.name: Read_File_By_Offset_And_Length_test_002
 * @tc.desc: Test function of ReadInputByOffsetAndLength() interface for read FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, Read_Input_By_Offset_And_Length_test_002, testing::ext::TestSize.Level1)
{
    std::ifstream file("/data/test/");
    std::string ret("std::string& ret");
    int res = FileUtils::ReadInputByOffsetAndLength(file, 111, 2147483647, ret);
    EXPECT_EQ(res, -1);
}
/**
* @tc.name: Read_File_By_Length_test_001
* @tc.desc: Test function of AppendWriteFileByOffsetToFile() interface for read FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Read_Input_By_Length_test_001, testing::ext::TestSize.Level1)
{
    std::ifstream file("/data/test/");
    std::string ret("std::string& ret");
    int res = FileUtils::ReadInputByLength(file, 2147483647, ret);
    EXPECT_EQ(res, -1);
}
/**
* @tc.name: Append_Write_File_ByOffset_To_File_test_001
* @tc.desc: Test function of AppendWriteFileByOffsetToFile() interface for write FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Append_Write_File_ByOffset_To_File_test_001, testing::ext::TestSize.Level1)
{
    std::ifstream file("");
    std::ofstream out("");
    bool res = FileUtils::AppendWriteFileByOffsetToFile(file, out, 10000, 100000);
    EXPECT_EQ(res, false);
}
/**
* @tc.name: Append_Write_File_ByOffset_To_File_test_002
* @tc.desc: Test function of AppendWriteFileByOffsetToFile() interface for write SUCCESS.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Append_Write_File_ByOffset_To_File_test_002, testing::ext::TestSize.Level1)
{
    std::ifstream file("/data/test/generateKeyPair/keypair.p12");
    std::ofstream out("/data/test/generateKeyPair/test.txt");
    bool res = FileUtils::AppendWriteFileByOffsetToFile(file, out, 10, 100);
    EXPECT_EQ(res, true);
}
/**
* @tc.name: Append_Write_File_ByOffset_To_File_test_003
* @tc.desc: Test function of AppendWriteFileByOffsetToFile() interface for write FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Append_Write_File_ByOffset_To_File_test_003, testing::ext::TestSize.Level1)
{
    std::ifstream file("/data/test/generateKeyPair/keypair.p12");
    std::ofstream out("");
    bool res = FileUtils::AppendWriteFileByOffsetToFile(file, out, 10, 100);
    EXPECT_EQ(res, false);
}
/**
* @tc.name: Is_Runnable_File_test_001
* @tc.desc: Test function of IsRunnableFile() interface for judge file suffix FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Is_Runnable_File_test_001, testing::ext::TestSize.Level1)
{
    std::string name = "";
    bool res = FileUtils::IsRunnableFile(name);
    EXPECT_EQ(res, false);
}
/**
* @tc.name: WriteByteToOutFile_test_001
* @tc.desc: Test function of WriteByteToOutFile()  interface for write SUCCESS.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, WriteByteToOutFile_test_001, testing::ext::TestSize.Level1)
{
    std::string bytes = "587469";
    std::ofstream outFile("/data/test/test.txt");
    bool res = FileUtils::WriteByteToOutFile(bytes, outFile);
    EXPECT_EQ(res, true);
}
/**
* @tc.name: WriteByteToOutFile_test_002
* @tc.desc: Test function of WriteByteToOutFile() interface for write FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, WriteByteToOutFile_test_002, testing::ext::TestSize.Level1)
{
    std::string bytes = "";
    std::ofstream outFile("");
    bool res = FileUtils::WriteByteToOutFile(bytes, outFile);
    EXPECT_EQ(res, false);
}
/**
* @tc.name: ReadFile_test_002
* @tc.desc: Test function of ReadFile() interface for read file SUCCESS.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, ReadFile_test_002, testing::ext::TestSize.Level1)
{
    std::string path = "/data/test/";
    std::string ret;
    int res = FileUtils::ReadFile(path, ret);
    EXPECT_EQ(res, 0);
}
/**
* @tc.name: WriteByteToOutFile_test_003
* @tc.desc: Test function of WriteByteToOutFile() interface for write SUCCESS.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, WriteByteToOutFile_test_003, testing::ext::TestSize.Level1)
{
    std::string bytes = "111";
    std::string outFile = "/data/test/test.txt";
    bool res = FileUtils::WriteByteToOutFile(bytes, outFile);
    EXPECT_EQ(res, true);
}
/**
* @tc.name: WriteByteToOutFile_test_004
* @tc.desc: Test function of WriteByteToOutFile() interface for write FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, WriteByteToOutFile_test_004, testing::ext::TestSize.Level1)
{
    std::string bytes = "111";
    std::string outFile = "";
    int res = FileUtils::WriteByteToOutFile(bytes, outFile);
    EXPECT_EQ(res, false);
}
/**
* @tc.name: ParsePkcs7Package_test_001
* @tc.desc: Test function of ParsePkcs7Package() interface for parse null p7b FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, ParsePkcs7Package_test_001, testing::ext::TestSize.Level1)
{
    const unsigned char packageData[] = {0};
    Pkcs7Context pkcs7Context;
    bool res = VerifyHapOpensslUtils::ParsePkcs7Package(packageData, 10, pkcs7Context);
    EXPECT_EQ(res, false);
}
/**
* @tc.name: ParsePkcs7Package_test_002
* @tc.desc: Test function of ParsePkcs7Package() interface for parse error p7b FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, ParsePkcs7Package_test_002, testing::ext::TestSize.Level1)
{
    const unsigned char packageData[] = {1, 1, 1, 1, 1};
    Pkcs7Context pkcs7Context;
    bool res = VerifyHapOpensslUtils::ParsePkcs7Package(packageData, 10, pkcs7Context);
    EXPECT_EQ(res, false);
}
/**
* @tc.name: GetCertChains_test_001
* @tc.desc: Test function of GetCertChains() interface for get certchain in p7b FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, GetCertChains_test_001, testing::ext::TestSize.Level1)
{
    PKCS7* p7 = nullptr;
    Pkcs7Context pkcs7Context;
    bool ret = VerifyHapOpensslUtils::GetCertChains(p7, pkcs7Context);
    EXPECT_EQ(ret, false);
}
/**
* @tc.name: GetCertChains_test_002
* @tc.desc: Test function of GetCertChains() interface for get certchain in p7b FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, GetCertChains_test_002, testing::ext::TestSize.Level1)
{
    PKCS7* p7 = PKCS7_new();
    Pkcs7Context pkcs7Context;
    bool ret = VerifyHapOpensslUtils::GetCertChains(p7, pkcs7Context);
    PKCS7_free(p7);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: set_bisic_constraints_patchlen_test_001
 * @tc.desc: Test function of SetBisicConstraintsPathLen() interface for set ConstraintsPathLen in cert FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */

HWTEST_F(GenerateCaTest, set_bisic_constraints_patchlen_test_001, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    Options options;
    options[Options::BASIC_CONSTRAINTS_PATH_LEN] = 1;
    bool cert1 = CertTools::SetBisicConstraintsPathLen(&options, cert);
    EXPECT_NE(cert1, false);
}

/**
 * @tc.name: sign_for_subcert_test_001
 * @tc.desc: Test function of SignForSubCert() interface SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, sign_for_subcert_test_001, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlg, issuer);
    bool cert1 = CertTools::SignForSubCert(cert, csr, issuercsr, keyPair, params.get());
    EXPECT_EQ(cert1, true);
}


/**
 * @tc.name: sign_for_subcert_test_002
 * @tc.desc: Test function of SignForSubCert() interface with a error keystoreFile path FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, sign_for_subcert_test_002, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = X509_REQ_new();
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlg, issuer);
    bool cert1 = CertTools::SignForSubCert(cert, csr, issuercsr, keyPair, params.get());
    EXPECT_EQ(cert1, false);
}


/**
 * @tc.name: sign_for_subcert_test_003
 * @tc.desc: Test function of SignForSubCert() interface for with a secret FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, sign_for_subcert_test_003, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    X509_REQ* issuercsr = X509_REQ_new();
    bool cert1 = CertTools::SignForSubCert(cert, csr, issuercsr, keyPair, params.get());

    EXPECT_NE(cert1, false);
}

/**
 * @tc.name: sign_for_subcert_test_004
 * @tc.desc: Test function of SignForSubCert() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, sign_for_subcert_test_004, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlg, issuer);
    bool cert1 = CertTools::SignForSubCert(cert, csr, issuercsr, keyPair, params.get());

    EXPECT_EQ(cert1, true);
}

/**
 * @tc.name: sign_for_subcert_test_005
 * @tc.desc: Test function of SignForSubCert() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, sign_for_subcert_test_005, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, signAlg, issuer);
    bool cert1 = CertTools::SignForSubCert(cert, csr, issuercsr, keyPair, params.get());

    EXPECT_EQ(cert1, true);
}

/**
 * @tc.name: get_csr_test_001
 * @tc.desc: Test function of GetCsr() interface with empty params FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */

HWTEST_F(GenerateCaTest, get_csr_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string signAlg = "";
    std::string subject = "";
    EVP_PKEY* keyPair = nullptr;
    bool ret = api->GetCsr(keyPair, signAlg, subject);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: get_csr_test_002
 * @tc.desc: Test function of GetCsr()  interface with a null key FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_csr_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    EVP_PKEY* keyPair = nullptr;
    bool ret = api->GetCsr(keyPair, signAlg, subject);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: save_cert_to_file_test_002
 * @tc.desc: Test function of CertTools::SaveCertTofile()  interface with a empty rootfile FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, save_cert_to_file_test_002, testing::ext::TestSize.Level1)
{
    X509* cert = X509_new();
    std::string rootoutFile = "";
    bool ret = CertTools::SaveCertTofile(rootoutFile, cert);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: isempty_test_002
 * @tc.desc: Test function of IsEmpty()  interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, isempty_test_002, testing::ext::TestSize.Level1)
{
    std::string cs = "1234";
    bool res = FileUtils::IsEmpty(cs);
    EXPECT_EQ(res, false);
}
/**
 * @tc.name: write_test_001
 * @tc.desc: Test function of IsEmpty()  interface with a error path FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, write_test_001, testing::ext::TestSize.Level1)
{
    std::string cs = "1234";
    int res = FileUtils::Write(cs, "/data/test/");
    EXPECT_EQ(res, -103);
}
/**
 * @tc.name: read_test_001
 * @tc.desc: Test function of IsEmpty()  interface with a null stream FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, read_test_001, testing::ext::TestSize.Level1)
{
    std::string cs = "1234";
    std::ifstream input;

    int res = FileUtils::Read(input, cs);
    EXPECT_EQ(res, 0);
}
/**
* @tc.name: Append_Write_File_ByOffset_To_File_test_004
* @tc.desc: Test function of AppendWriteFileByOffsetToFile() interface with a error path FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Append_Write_File_ByOffset_To_File_test_004, testing::ext::TestSize.Level1)
{
    std::string file = "/data/test/";
    std::ifstream in(file);
    std::ofstream out("");
    bool res = FileUtils::AppendWriteFileByOffsetToFile(in, out, 10, 100);
    EXPECT_EQ(res, false);
    out.close();
    in.close();
}
/**
* @tc.name: Append_Write_File_ByOffset_To_File_test_005
* @tc.desc: Test function of AppendWriteFileByOffsetToFile() interface SUCCESS.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Append_Write_File_ByOffset_To_File_test_005, testing::ext::TestSize.Level1)
{
    std::string file = "/data/test/generateKeyPair/keypair.p12";
    std::ifstream in(file);
    std::ofstream out("/data/test/generateKeyPair/test.txt");
    bool res = FileUtils::AppendWriteFileByOffsetToFile(in, out, 10, 100);
    EXPECT_EQ(res, true);
    out.close();
    in.close();
}
/**
* @tc.name: Append_Write_File_ByOffset_To_File_test_006
* @tc.desc: Test function of AppendWriteFileByOffsetToFile()  interface with a error path FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Append_Write_File_ByOffset_To_File_test_006, testing::ext::TestSize.Level1)
{
    std::string file = "/data/test/";
    std::ifstream in(file);
    std::ofstream out("/data/test/generateKeyPair/test.txt");
    bool res = FileUtils::AppendWriteFileByOffsetToFile(in, out, 10, 100);
    EXPECT_EQ(res, false);
    out.close();
    in.close();
}

/**
* @tc.name: Verify_Hap_test_001
* @tc.desc: Test function of VerifyHapSigner()  interface for hap type FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Verify_Hap_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string inForm = "zip";
    (*params)["inForm"] = inForm;
    bool res = api->VerifyHapSigner(params.get());
    EXPECT_EQ(res, false);
}

/**
* @tc.name: Verify_Hap_test_002
* @tc.desc: Test function of VerifyHapSigner()  interface for hap type FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Verify_Hap_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string inForm = "elf";
    (*params)["inForm"] = inForm;
    bool res = api->VerifyHapSigner(params.get());
    EXPECT_EQ(res, false);
}

/**
* @tc.name: Verify_Hap_test_003
* @tc.desc: Test function of VerifyHapSigner()  interface for hap type FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Verify_Hap_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string inForm = "bin";
    (*params)["inForm"] = inForm;
    bool res = api->VerifyHapSigner(params.get());
    EXPECT_EQ(res, false);
}

/**
* @tc.name: Verify_Hap_test_004
* @tc.desc: Test function of VerifyHapSigner()  interface for error type FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, Verify_Hap_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string inForm = "mmmmm";
    (*params)["inForm"] = inForm;
    bool res = api->VerifyHapSigner(params.get());
    EXPECT_EQ(res, false);
}

/**
* @tc.name: generate_root_cert_to_flie_001
* @tc.desc: Test function of GenerateRootCertToFile()  interface with a null keypair FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_root_cert_to_flie_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    EVP_PKEY* keyPair = nullptr;
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    bool res = api->GenerateRootCertToFile(params.get(), keyPair);
    EXPECT_EQ(res, false);
}

/**
* @tc.name: generate_root_cert_to_flie_002
* @tc.desc: Test function of GenerateRootCertToFile()  interface with a empty subject FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_root_cert_to_flie_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "";
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
    bool res = api->GenerateRootCertToFile(params.get(), keyPair);
    EXPECT_EQ(res, false);
}
/**
* @tc.name: generate_root_cert_to_flie_003
* @tc.desc: Test function of GenerateRootCertToFile()  interface with error keystoreFile path FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_root_cert_to_flie_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA256withRSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
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
    bool res = api->GenerateRootCertToFile(params.get(), keyPair);
    EXPECT_EQ(res, false);
}

/**
* @tc.name: generate_root_cert_to_flie_004
* @tc.desc: Test function of GenerateRootCertToFile()  interface with a error issuerkeyAlias FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_root_cert_to_flie_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    char isksPwd[] = "123456";
    std::string outFile = "/datamt/test/generateCA/root-ca-test.cer";
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
    (*params)["outFile"] = outFile;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    bool res = api->GenerateRootCertToFile(params.get(), keyPair);
    EXPECT_EQ(res, false);
}

/**
* @tc.name: out_put_mode_of_cert_001
* @tc.desc: Test function of OutputModeOfCert()  interface with a empty cert FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, out_put_mode_of_cert_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    X509* cert = X509_new();
    std::string outFile = "/datamt/test/generateCA/sub-ca-test.cer";
    (*params)["outFile"] = outFile;
    bool res = api->OutputModeOfCert(cert, params.get());
    EXPECT_EQ(res, false);
}

/**
* @tc.name: generate_key_store_001
* @tc.desc: Test function of GenerateKeyStore()  interface with a empty keyalias FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_key_store_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "";
    (*params)["keyAlias"] = keyAlias;
    bool res = api->GenerateKeyStore(params.get());
    EXPECT_EQ(res, false);
}

/**
* @tc.name: handle_issuer_key_alias_empty_001
* @tc.desc: Test function of HandleIssuerKeyAliasEmpty()  interface for RET_FAILED.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, handle_issuer_key_alias_empty_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keystoreFile = "/data/test/generateCA/other.p12";
    std::string issuerKeystoreFile = "/data/test/generateCA/issuer.p12";
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["keystoreFile"] = keystoreFile;
    int res = api->HandleIssuerKeyAliasEmpty(params.get());
    EXPECT_EQ(res, RET_FAILED);
}

/**
* @tc.name: handle_issuer_key_alias_empty_002
* @tc.desc: Test function of HandleIssuerKeyAliasEmpty()  interface with a empty keystorepwd.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, handle_issuer_key_alias_empty_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    char keystorepwd[] = "";
    char issuerstorepwd[] = "123456";
    std::string keystoreFile = "/data/test/generateCA/other.p12";
    std::string issuerKeystoreFile = "/data/test/generateCA/other.p12";
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystorePwd"] = issuerstorepwd;
    (*params)["keystorePwd"] = keystorepwd;
    int res = api->HandleIssuerKeyAliasEmpty(params.get());
    EXPECT_EQ(res, RET_FAILED);
}

/**
* @tc.name: handle_issuer_key_alias_empty_003
* @tc.desc: Test function of HandleIssuerKeyAliasEmpty()  interface for RET_FAILED.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, handle_issuer_key_alias_empty_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    char keystorepwd[] = "234567";
    char issuerstorepwd[] = "123456";
    std::string keystoreFile = "/data/test/generateCA/other.p12";
    std::string issuerKeystoreFile = "/data/test/generateCA/other.p12";
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystorePwd"] = issuerstorepwd;
    (*params)["keystorePwd"] = keystorepwd;
    int res = api->HandleIssuerKeyAliasEmpty(params.get());
    EXPECT_EQ(res, RET_FAILED);
}

/**
* @tc.name: handle_issuer_key_alias_empty_004
* @tc.desc: Test function of HandleIssuerKeyAliasEmpty()  interface with a empty issuerKeystoreFile RET_OK.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, handle_issuer_key_alias_empty_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keystoreFile = "/data/test/generateCA/other.p12";
    std::string issuerKeystoreFile = "";
    char keystorepwd[] = "234567";
    char issuerstorepwd[] = "123456";
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystorePwd"] = issuerstorepwd;
    (*params)["keystorePwd"] = keystorepwd;
    int res = api->HandleIssuerKeyAliasEmpty(params.get());
    EXPECT_EQ(res, RET_OK);
}
/**
* @tc.name: handle_issuer_key_alias_not_empty_001
* @tc.desc: Test function of HandleIsserKeyAliasNotEmpty()  interface with a empty issuerKeystoreFile RET_OK.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, handle_issuer_key_alias_not_empty_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string issuerKeystoreFile = "";
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    int res = api->HandleIsserKeyAliasNotEmpty(params.get());
    EXPECT_EQ(res, RET_OK);
}

/**
* @tc.name: handle_issuer_key_alias_not_empty_002
* @tc.desc: Test function of HandleIsserKeyAliasNotEmpty()  interface with error path FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, handle_issuer_key_alias_not_empty_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string issuerKeystoreFile = "/data/test/generateCA/other.cer";
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    int res = api->HandleIsserKeyAliasNotEmpty(params.get());
    EXPECT_EQ(res, RET_FAILED);
}

/**
* @tc.name: generate_end_cert_001
* @tc.desc: Test function of GenerateEndCert()  interface for SUCCESS.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_end_cert_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app-sign-srv-ca-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char isksPwd[] = "123456";
    std::string outfile = "/data/test/generateCA/app-release.cer";
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
    (*params)["outFile"] = outfile;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EVP_PKEY* issuerkeyPair = nullptr;
    adaptePtr->SetIssuerKeyStoreFile(true);
    issuerkeyPair = adaptePtr->GetIssuerKeyByAlias();
    X509_REQ* csr = nullptr;
    csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateEndCert(csr, issuerkeyPair, *adaptePtr, PROFILE_SIGNING_CAPABILITY,
                                          sizeof(PROFILE_SIGNING_CAPABILITY));
    EXPECT_EQ(ret, true);
}

/**
* @tc.name: generate_end_cert_002
* @tc.desc: Test function of GenerateEndCert()  interface with error keyAlias FAILED.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_end_cert_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app-sign-srv-ca-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char isksPwd[] = "123456";
    std::string outfile = "/data/test/generateCA/app-release.cer";
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
    (*params)["outFile"] = outfile;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EVP_PKEY* issuerkeyPair = nullptr;
    X509_REQ* csr = nullptr;
    csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateEndCert(csr, issuerkeyPair, *adaptePtr, PROFILE_SIGNING_CAPABILITY,
                                          sizeof(PROFILE_SIGNING_CAPABILITY));
    EXPECT_EQ(ret, false);
}

/**
* @tc.name: generate_end_cert_003
* @tc.desc: Test function of GenerateEndCert()  interface for SUCCESS.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_end_cert_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuerkeyAlias = "oh-profile-sign-srv-ca-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    char isksPwd[] = "123456";
    std::string outfile = "/data/test/generateCA/app-release.cer";
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
    (*params)["outFile"] = outfile;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EVP_PKEY* issuerkeyPair = nullptr;
    adaptePtr->SetIssuerKeyStoreFile(true);
    issuerkeyPair = adaptePtr->GetIssuerKeyByAlias();
    X509_REQ* csr = nullptr;
    csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateEndCert(csr, issuerkeyPair, *adaptePtr, PROFILE_SIGNING_CAPABILITY,
                                          sizeof(PROFILE_SIGNING_CAPABILITY));
    EXPECT_EQ(ret, true);
}

/**
* @tc.name: generate_end_cert_004
* @tc.desc: Test function of GenerateEndCert()  interface with a error issuerkeyAlias FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, generate_end_cert_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuerkeyAlias = "oh-profile-sign-srv-ca-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    char isksPwd[] = "123456";
    std::string outfile = "/data/test/generateCA/app-release.cer";
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
    (*params)["outFile"] = outfile;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    EVP_PKEY* issuerkeyPair = nullptr;
    X509_REQ* csr = nullptr;
    csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateEndCert(csr, issuerkeyPair, *adaptePtr, PROFILE_SIGNING_CAPABILITY,
                                          sizeof(PROFILE_SIGNING_CAPABILITY));
    EXPECT_EQ(ret, false);
}

/**
* @tc.name: issuer_key_store_file_001
* @tc.desc: Test function of IssuerKeyStoreFile()  interface for auto generate key FAIL.
* @tc.type: FUNC
* @tc.require: SR000H63TL
*/
HWTEST_F(GenerateCaTest, issuer_key_store_file_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v123";
    std::string issuerkeyAlias = "oh-app1-key-v122222";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony1.p12";
    char keystorePwd[] = "123456";
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    int ret = adaptePtr->IssuerKeyStoreFile(&keyPair, true);
    EXPECT_EQ(ret, RET_FAILED);
}
}
}