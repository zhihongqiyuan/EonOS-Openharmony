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
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void GenerateCaTest::SetUpTestCase(void)
{
    (void)rename("./generateCA/phone-default-signed.txt", "./generateCA/phone-default-signed.hap");
    (void)rename("./generateCA/bin_signed_package.txt", "./generateCA/bin_signed_package.bin");
    (void)rename("./generateCA/elf_signed_package.txt", "./generateCA/elf_signed_package.elf");
}

void GenerateCaTest::TearDownTestCase(void)
{
}

void GenerateCaTest::SetUp()
{
}

void GenerateCaTest::TearDown()
{
}

/**
 * @tc.name: generate_cert_test_017
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keyusagecritial.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_017, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    std::string issuerKeystoreFile = "/data/test/generateKeyPair/pp.p12";
    std::string signAlg = "";
    std::string subject = "";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string keyUsage = "digitalSignature";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
    char secret[] = "123456";
    char isksPwd[] = "123456";
    char keystorePwd[] = "123456";
    char issuerkeypwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["issuerKeyPwd"] = issuerkeypwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;

    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_cert_test_018
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keyusagecritial.
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_018, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string keyUsage = "digitalSignature";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
    char secret[] = "123456";
    char isksPwd[] = "123456";
    char keystorePwd[] = "123456";
    char issuerkeypwd[] = "123456";
    std::string outFile = "/datamt/test/generateCA/general-ca-test.cer";
    (*params)["keyPwd"] = secret;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["issuerKeyPwd"] = issuerkeypwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_cert_test_019
 * @tc.desc: Test function of GenerateCert() SUCCESS to generate a certificate.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_019, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Application Signature Service CA";
    std::string keyUsage = "digitalSignature";
    char secret[] = "123456";
    char keystorePwd[] = "123456";
    int validity = 365;
    std::string outfile = "/data/test/generateCA/single-app1.cer";
    (*params)["keyPwd"] = secret;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyUsage"] = keyUsage;
    (*params)["validity"] = validity;
    (*params)["outFile"] = outfile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: generate_cert_test_020
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a empty signalg.
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_020, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string signAlg = "";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Application Signature Service CA";
    std::string keyUsage = "digitalSignature";
    char secret[] = "123456";
    char keystorePwd[] = "123456";
    int validity = 365;
    std::string outfile = "/data/test/generateCA/single-app1.cer";
    (*params)["keyPwd"] = secret;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyUsage"] = keyUsage;
    (*params)["validity"] = validity;
    (*params)["outFile"] = outfile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_cert_test_021
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a empty subject.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_021, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Application Signature Service CA";
    std::string keyUsage = "digitalSignature";
    char secret[] = "123456";
    char keystorePwd[] = "123456";
    int validity = 365;
    std::string outfile = "/data/test/generateCA/single-app1.cer";
    (*params)["keyPwd"] = secret;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyUsage"] = keyUsage;
    (*params)["validity"] = validity;
    (*params)["outFile"] = outfile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: get_int_test_001
 * @tc.desc: Test function of GetInt()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_int_test_001, testing::ext::TestSize.Level1)
{
    Options option;
    std::string str = "test";
    int tmp = option.GetInt(str);
    EXPECT_EQ(std::to_string(tmp).size(), 1U);
}

/**
 * @tc.name: generate_app_cert_test_001
 * @tc.desc: Test function of GenerateAppCert()  interface for generate certChain SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_appcert_test_001, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string rootCaCertFile = "/data/test/generateCA/root-ca1.cer";
    std::string subCaCertFile = "/data/test/generateCA/app-sign-srv-ca1.cer";
    std::string outFile = "/data/test/generateCA/app-release1.pem";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    int validity = 365;
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::CA_CERT_FILE] = rootCaCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    bool result = api.GenerateAppCert(&params);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: generate_app_cert_test_002
 * @tc.desc: Test function of GenerateAppCert() interface for generate cert SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_appcert_test_002, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string outFile = "/data/test/generateCA/app-release1.cer";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    int validity = 365;
    std::string outForm = "cert";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    bool result = api.GenerateAppCert(&params);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: generate_app_cert_test_003
 * @tc.desc: Test function of GenerateAppCert() interface for generate certChain SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_appcert_test_003, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string rootCaCertFile = "/data/test/generateCA/root-ca1.cer";
    std::string subCaCertFile = "/data/test/generateCA/app-sign-srv-ca1.cer";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    int validity = 365;
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::CA_CERT_FILE] = rootCaCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    bool result = api.GenerateAppCert(&params);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: generate_app_cert_test_004
 * @tc.desc: Test function of GenerateAppCert() interface for generate certChain SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_appcert_test_004, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    int validity = 365;
    std::string outForm = "cert";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    bool result = api.GenerateAppCert(&params);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: generate_app_cert_test_005
 * @tc.desc: Test function of GenerateAppCert() interface for generate certChain with error keyalias FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_appcert_test_005, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/apptest.p12";
    std::string caCertFile = "/data/test/generateCA/root-ca1test.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1test.cer";
    std::string outFile = "/data/test/generateCA/test-profile-cert-v1.cer";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::CA_CERT_FILE] = caCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    bool result = api.GenerateAppCert(&params);
    EXPECT_EQ(result, false);
}
/**
 * @tc.name: generate_app_cert_test_006
 * @tc.desc: Test function of GenerateAppCert() interface for generate certChain with a error issuerKeyAliasFAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_appcert_test_006, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "45679887";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/apptest.p12";
    std::string caCertFile = "/data/test/generateCA/root-ca1test.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1test.cer";
    std::string outFile = "/data/test/generateCA/test-profile-cert-v1.cer";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::CA_CERT_FILE] = caCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    bool result = api.GenerateAppCert(&params);
    EXPECT_EQ(result, false);
}
/**
 * @tc.name: generate_app_cert_test_007
 * @tc.desc: Test function of GenerateAppCert() interface for generate certChain with a error keystoreFile FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_appcert_test_007, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string keystoreFile = "/data/test/generateCA/apptest.p12";
    std::string caCertFile = "/data/test/generateCA/root-ca1test.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1test.cer";
    std::string outFile = "/data/test/generateCA/test-profile-cert-v1.cer";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::CA_CERT_FILE] = caCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    bool result = api.GenerateAppCert(&params);
    EXPECT_EQ(result, false);
}
/**
 * @tc.name: generate_profile_cert_test_001
 * @tc.desc: Test function of GenerateProfileCert() interface for generate certChain SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_profilecert_test_001, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string outFile = "/data/test/generateCA/profile1.cer";
    int validity = 365;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    std::string outForm = "cert";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    bool result = api.GenerateProfileCert(&params);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: generate_profile_cert_test_002
 * @tc.desc: Test function of GenerateProfileCert() interface for generate cert SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_profilecert_test_002, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string rootCaCertFile = "/data/test/generateCA/root-ca1.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1.cer";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string outFile = "/data/test/generateCA/profile-release1.pem";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    std::string outForm = "certChain";
    int validity = 365;
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::CA_CERT_FILE] = rootCaCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    bool result = api.GenerateProfileCert(&params);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: generate_profile_cert_test_003
 * @tc.desc: Test function of GenerateProfileCert() interface for generate certChain SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_profilecert_test_003, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    int validity = 365;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    std::string outForm = "cert";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    bool result = api.GenerateProfileCert(&params);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: generate_profile_cert_test_004
 * @tc.desc: Test function of GenerateProfileCert() interface for generate certChain SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_profilecert_test_004, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string rootCaCertFile = "/data/test/generateCA/root-ca1.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1.cer";
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    std::string outForm = "certChain";
    int validity = 365;
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::CA_CERT_FILE] = rootCaCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FORM] = outForm;
    params[Options::VALIDITY] = validity;
    bool result = api.GenerateProfileCert(&params);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: generate_profile_cert_test_005
 * @tc.desc: Test function of GenerateProfileCert() interface for generate certChain with a error keyAlias FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_profilecert_test_005, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string caCertFile = "/data/test/generateCA/root-ca1test.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1test.cer";
    std::string outFile = "/data/test/generateCA/test-profile-cert-v1.cer";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::CA_CERT_FILE] = caCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    bool result = api.GenerateProfileCert(&params);
    EXPECT_EQ(result, false);
}
/**
 * @tc.name: generate_profile_cert_test_006
 * @tc.desc: Test function of GenerateProfileCert() interface for generate certChain with a issuerKeyAlias FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_profilecert_test_006, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "45679887";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string caCertFile = "/data/test/generateCA/root-ca1test.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1test.cer";
    std::string outFile = "/data/test/generateCA/test-profile-cert-v1.cer";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::CA_CERT_FILE] = caCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SUBJECT] = subject;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    bool result = api.GenerateProfileCert(&params);
    EXPECT_EQ(result, false);
}
/**
 * @tc.name: generate_profile_cert_test_007
 * @tc.desc: Test function of GenerateProfileCert() interface for generate certChain with a error cacertfile FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_profilecert_test_007, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Profile Signature Service CA";
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string caCertFile = "/data/test/generateCA/root-ca1test.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1test.cer";
    std::string outFile = "/data/test/generateCA/test-profile-cert-v1.cer";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    std::string outForm = "certChain";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::CA_CERT_FILE] = caCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::KEY_ALIAS] = keyAlias;
    params[Options::ISSUER] = issuer;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    params[Options::OUT_FILE] = outFile;
    params[Options::OUT_FORM] = outForm;
    bool result = api.GenerateProfileCert(&params);
    EXPECT_EQ(result, false);
}


/**
 * @tc.name: judge_alg_type_test_001
 * @tc.desc: Test function of JudgeAlgType() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, judge_alg_type_test_001, testing::ext::TestSize.Level1)
{
    std::string str = "ECC";
    bool ret = CmdUtil::JudgeAlgType(str);
    EXPECT_EQ(ret, true);
}
/**
 * @tc.name: judge_size_test_001
 * @tc.desc: Test function of JudgeSize() interface for FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, judge_size_test_001, testing::ext::TestSize.Level1)
{
    int size = 255;
    bool ret = CmdUtil::JudgeSize(size);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name:generate_root_cert_001
 * @tc.desc: Test function of :GenerateRootCertificate() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_root_cert_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    char keyPwd[] = "123456";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char keystorePwd[] = "123456";
    std::string algorithm = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["algorithm"] = algorithm;
    (*params)["signAlgorithm"] = signAlgorithm;
    (*params)["subject"] = subject;
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateRootCertificate(keyPair, csr, params.get());
    EXPECT_NE(cert, nullptr);
}
/**
 * @tc.name:generate_root_cert_002
 * @tc.desc: Test function of :GenerateRootCertificate() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_root_cert_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    char keyPwd[] = "123456";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char keystorePwd[] = "123456";
    std::string algorithm = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string basicConstraintsPathLen = "5";
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["algorithm"] = algorithm;
    (*params)["signAlgorithm"] = signAlgorithm;
    (*params)["subject"] = subject;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateRootCertificate(keyPair, csr, params.get());
    EXPECT_NE(cert, nullptr);
}

/**
 * @tc.name:generate_root_cert_003
 * @tc.desc: Test function of :GenerateRootCertificate() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_root_cert_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    char keyPwd[] = "123456";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char keystorePwd[] = "123456";
    std::string algorithm = "ECC";
    std::string signAlgorithm = "";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["algorithm"] = algorithm;
    (*params)["signAlgorithm"] = signAlgorithm;
    (*params)["subject"] = subject;
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_EQ(csr, nullptr);
    X509* cert = CertTools::GenerateRootCertificate(keyPair, csr, params.get());
    EXPECT_EQ(cert, nullptr);
}
/**
 * @tc.name: generate_end_cert_test_001
 * @tc.desc: Test function of GenerateEndCert()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_end_cert_test_001, testing::ext::TestSize.Level1)
{
    const char appSigningCapability[] = {0x30, 0x06, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00};
    Options params;
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char keystorePwd[] = "123456";
    std::string algorithm = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 3650;
    params["keyAlias"] = keyAlias;
    params["keyPwd"] = keyPwd;
    params["keySize"] = keySize;
    params["keystoreFile"] = keystoreFile;
    params["keystorePwd"] = keystorePwd;
    params["algorithm"] = algorithm;
    params["signAlgorithm"] = signAlgorithm;
    params["subject"] = subject;
    params["issuer"] = issuer;
    params["validity"] = validity;
    LocalizationAdapter adapter(&params);
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateEndCert(csr, keyPair, adapter, appSigningCapability,
                                            sizeof(appSigningCapability));
    EXPECT_NE(cert, nullptr);
}

/**
 * @tc.name: generate_end_cert_test_002
 * @tc.desc: Test function of GenerateEndCert()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_end_cert_test_002, testing::ext::TestSize.Level1)
{
    char appSigningCapability[] = {0x30, 0x06, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00};
    Options params;
    char keyPwd[] = "123456";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char keystorePwd[] = "123456";
    std::string algorithm = "ECC";
    std::string signAlgorithm = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    params["keyPwd"] = keyPwd;
    params["keySize"] = keySize;
    params["keystoreFile"] = keystoreFile;
    params["keystorePwd"] = keystorePwd;
    params["algorithm"] = algorithm;
    params["signAlgorithm"] = signAlgorithm;
    params["subject"] = subject;
    params["issuer"] = issuer;
    LocalizationAdapter adapter(&params);
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateEndCert(csr, keyPair, adapter, appSigningCapability,
                                            sizeof(appSigningCapability));
    EXPECT_NE(cert, nullptr);
}

/**
 * @tc.name: generate_end_cert_test_003
 * @tc.desc: Test function of GenerateEndCert()  interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_end_cert_test_003, testing::ext::TestSize.Level1)
{
    char appSigningCapability[] = {0x30, 0x06, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00};
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "alias";
    char keyPwd[] = "123456";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char keystorePwd[] = "123456";
    std::string algorithm = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["algorithm"] = algorithm;
    (*params)["signAlgorithm"] = signAlgorithm;
    (*params)["subject"] = subject;
    LocalizationAdapter adapter(params.get());
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateEndCert(csr, keyPair, adapter, appSigningCapability,
                                            sizeof(appSigningCapability));
    EXPECT_EQ(cert, nullptr);
}

/**
 * @tc.name: is_out_form_chain_test_001
 * @tc.desc: Test function of IsOutFormChain()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, is_out_form_chain_test_001, testing::ext::TestSize.Level1)
{
    Options params;
    std::string outForm = "certChain";
    params[Options::OUT_FORM] = outForm;
    LocalizationAdapter adapter(&params);
    bool ret = adapter.IsOutFormChain();
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: is_out_form_chain_test_002
 * @tc.desc: Test function of IsOutFormChain()  interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, is_out_form_chain_test_002, testing::ext::TestSize.Level1)
{
    Options params;
    std::string outForm = "cert";
    params[Options::OUT_FORM] = outForm;
    LocalizationAdapter adapter(&params);
    bool ret = adapter.IsOutFormChain();
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: get_cert_fromfile_test_001
 * @tc.desc: Test function of GetCertsFromFile()  interface for error path FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_cert_fromfile_test_001, testing::ext::TestSize.Level1)
{
    Options params;
    std::string path = "";
    LocalizationAdapter adapter(&params);
    auto ret = adapter.GetCertsFromFile(path, " ");
    EXPECT_EQ(ret.empty(), true);
}

/**
 * @tc.name: get_cert_fromfile_test_002
 * @tc.desc: Test function of GetCertsFromFile()  interface for error path FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_cert_fromfile_test_002, testing::ext::TestSize.Level1)
{
    Options params;
    std::string path = "certs";
    LocalizationAdapter adapter(&params);
    auto ret = adapter.GetCertsFromFile(path, " ");
    EXPECT_EQ(ret.empty(), true);
}

/**
 * @tc.name: output_cert_test_001
 * @tc.desc: Test function of OutPutCert() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, output_cert_test_001, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    char appSigningCapability[] = {0x30, 0x06, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00};
    Options params;
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char keystorePwd[] = "123456";
    std::string algorithm = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 3650;
    params["keyAlias"] = keyAlias;
    params["keyPwd"] = keyPwd;
    params["keySize"] = keySize;
    params["keystoreFile"] = keystoreFile;
    params["keystorePwd"] = keystorePwd;
    params["algorithm"] = algorithm;
    params["signAlgorithm"] = signAlgorithm;
    params["subject"] = subject;
    params["issuer"] = issuer;
    params["validity"] = validity;
    LocalizationAdapter adapter(&params);
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateEndCert(csr, keyPair, adapter, appSigningCapability,
                                            sizeof(appSigningCapability));
    EXPECT_NE(cert, nullptr);
    bool ret = api.OutPutCert(cert, "/data/test/generateCA/test.cer");
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: output_cert_test_002
 * @tc.desc: Test function of OutPutCert() interface write file FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, output_cert_test_002, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    char appSigningCapability[] = {0x30, 0x06, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00};
    Options params;
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char keystorePwd[] = "123456";
    std::string algorithm = "ECC";
    std::string signAlgorithm = "SHA256withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 3650;
    params["keyAlias"] = keyAlias;
    params["keyPwd"] = keyPwd;
    params["keySize"] = keySize;
    params["keystoreFile"] = keystoreFile;
    params["keystorePwd"] = keystorePwd;
    params["algorithm"] = algorithm;
    params["signAlgorithm"] = signAlgorithm;
    params["subject"] = subject;
    params["issuer"] = issuer;
    params["validity"] = validity;
    LocalizationAdapter adapter(&params);
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateEndCert(csr, keyPair, adapter, appSigningCapability,
                                            sizeof(appSigningCapability));
    EXPECT_NE(cert, nullptr);
    bool ret = api.OutPutCert(cert, "/datasgje/test/generateCA/test.cer ");
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: output_cert_chain_test_001
 * @tc.desc: Test function of OutPutCertChain() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, output_cert_chain_test_001, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    X509* cert1 = X509_new();
    X509* cert2 = X509_new();
    X509* cert3 = X509_new();
    std::string path = "/data/test/generateCA/test.pem";
    std::vector<X509*> certs;
    certs.push_back(cert1);
    certs.push_back(cert2);
    certs.push_back(cert3);
    bool ret = api.OutPutCertChain(certs, path);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: output_cert_chain_test_002
 * @tc.desc: Test function of OutPutCertChain() interface with a empty path FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, output_cert_chain_test_002, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    X509* cert1 = X509_new();
    X509* cert2 = X509_new();
    X509* cert3 = X509_new();
    std::string path = "";
    std::vector<X509*> certs;
    certs.push_back(cert1);
    certs.push_back(cert2);
    certs.push_back(cert3);
    bool ret = api.OutPutCertChain(certs, path);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: print_x509_test_001
 * @tc.desc: Test function of PrintX509CertFromMemory() interface for print cert SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, print_x509_test_001, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    X509* cert1 = X509_new();
    bool ret = api.PrintX509CertFromMemory(cert1);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: read_x509_test_001
 * @tc.desc: Test function of ReadfileToX509() interface read x509 cert file FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, read_x509_test_001, testing::ext::TestSize.Level1)
{
    std::string path = "";
    X509* cert1 = CertTools::ReadfileToX509(path);
    EXPECT_EQ(cert1, nullptr);
}

/**
 * @tc.name: read_x509_test_002
 * @tc.desc: Test function of ReadfileToX509 interface for read x509 cert FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, read_x509_test_002, testing::ext::TestSize.Level1)
{
    std::string path = "/data/test/generateCA/root-ca1.cer";
    X509* cert1 = CertTools::ReadfileToX509(path);
    EXPECT_NE(cert1, nullptr);
}

/**
 * @tc.name: get_and_output_cert_test_001
 * @tc.desc: Test function of GetAndOutPutCert() interface for output type certchain SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_and_output_cert_test_001, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string caCertFile = "/data/test/generateCA/root-ca1.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1.cer";
    std::string outForm = "certChain";
    params[Options::CA_CERT_FILE] = caCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::OUT_FORM] = outForm;
    LocalizationAdapter adapter(&params);
    std::string path = "/data/test/generateCA/root-ca1.cer";
    X509* cert1 = CertTools::ReadfileToX509(path);
    EXPECT_NE(cert1, nullptr);
    bool result = api.GetAndOutPutCert(adapter, cert1);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: get_and_output_cert_test_002
 * @tc.desc: Test function of GetAndOutPutCert() interface for output type cert SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_and_output_cert_test_002, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string outForm = "cert";
    params[Options::OUT_FORM] = outForm;
    LocalizationAdapter adapter(&params);
    std::string path = "/data/test/generateCA/root-ca1.cer";
    X509* cert1 = CertTools::ReadfileToX509(path);
    EXPECT_NE(cert1, nullptr);
    bool result = api.GetAndOutPutCert(adapter, cert1);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: get_and_output_cert_test_003
 * @tc.desc: Test function of GetAndOutPutCert() interface for certchain SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_and_output_cert_test_003, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string caCertFile = "/data/test/generateCA/root-ca1.cer";
    std::string subCaCertFile = "/data/test/generateCA/profile-sign-srv-ca1.cer";
    std::string outFile = "/data/test/generateCA/certChain";
    params[Options::CA_CERT_FILE] = caCertFile;
    params[Options::SUB_CA_CERT_FILE] = subCaCertFile;
    params[Options::OUT_FORM] = outFile;
    LocalizationAdapter adapter(&params);
    std::string path = "/data/test/generateCA/root-ca1.cer";
    X509* cert1 = CertTools::ReadfileToX509(path);
    EXPECT_NE(cert1, nullptr);
    bool result = api.GetAndOutPutCert(adapter, cert1);
    EXPECT_EQ(result, true);
}
/**
 * @tc.name: hap_verify_test_001
 * @tc.desc: Test function of VerifyHapSigner() interface for verify p7b FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, hap_verify_test_001, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string inFile = "/data/test/generateCA/phone1-default-signed.hap";
    std::string outCertChain = "/data/test/generateCA/hap-verify.cer";
    std::string outProfile = "/data/test/generateCA/hap-profile.p7b";
    params[Options::IN_FILE] = inFile;
    params[Options::OUT_CERT_CHAIN] = outCertChain;
    params[Options::OUT_PROFILE] = outProfile;
    bool result = api.VerifyHapSigner(&params);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: hap_verify_test_002
 * @tc.desc: Test function of VerifyHapSigner() interface for cert verify p7b FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, hap_verify_test_002, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string inFile = "";
    std::string outCertChain = "/data/test/generateCA/hap-verify.cer";
    std::string outProfile = "/data/test/generateCA/hap-profile.p7b";
    params[Options::IN_FILE] = inFile;
    params[Options::OUT_CERT_CHAIN] = outCertChain;
    params[Options::OUT_PROFILE] = outProfile;
    bool result = api.VerifyHapSigner(&params);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: hap_verify_test_003
 * @tc.desc: Test function of VerifyHapSigner() interface for verify hap SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, hap_verify_test_003, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string inFile = "./generateCA/phone-default-signed.hap";
    std::string outCertChain = "./generateCA/xxx.cer";
    std::string outProfile = "./generateCA/xxx.p7b";
    std::string inForm = "zip";
    params[Options::IN_FILE] = inFile;
    params[Options::OUT_CERT_CHAIN] = outCertChain;
    params[Options::OUT_PROFILE] = outProfile;
    params[Options::INFORM] = inForm;
    bool result = api.VerifyHapSigner(&params);
    EXPECT_EQ(result, true);
}

/**
 * @tc.name: hap_verify_test_004
 * @tc.desc: Test function of VerifyHapSigner() interface for verify elf SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, hap_verify_test_004, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string inFile = "./generateCA/elf_signed_package.elf";
    std::string outCertChain = "./generateCA/xxx.cer";
    std::string outProfile = "./generateCA/xxx.p7b";
    std::string inForm = "elf";
    params[Options::IN_FILE] = inFile;
    params[Options::OUT_CERT_CHAIN] = outCertChain;
    params[Options::OUT_PROFILE] = outProfile;
    params[Options::INFORM] = inForm;
    bool result = api.VerifyHapSigner(&params);
    EXPECT_EQ(result, true);
}

/**
 * @tc.name: hap_verify_test_005
 * @tc.desc: Test function of VerifyHapSigner() interface for verify bin SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, hap_verify_test_005, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string inFile = "./generateCA/bin_signed_package.bin";
    std::string outCertChain = "./generateCA/xxx.cer";
    std::string outProfile = "./generateCA/xxx.p7b";
    std::string inForm = "bin";
    params[Options::IN_FILE] = inFile;
    params[Options::OUT_CERT_CHAIN] = outCertChain;
    params[Options::OUT_PROFILE] = outProfile;
    params[Options::INFORM] = inForm;
    bool result = api.VerifyHapSigner(&params);
    EXPECT_EQ(result, true);
}

/**
 * @tc.name: generate_key_pair_test_001
 * @tc.desc: Test function of GetIssuerKeyByAlias() interface for generate key FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_key_pair_test_001, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string issuerkeyStroeFile = "/data/test/generateCA/ohtest.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_STORE_FILE] = issuerkeyStroeFile;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    LocalizationAdapter adapter(&params);
    adapter.SetIssuerKeyStoreFile(true);
    EVP_PKEY* issueKeyPair = adapter.GetIssuerKeyByAlias();
    EXPECT_NE(issueKeyPair, nullptr);
}
/**
 * @tc.name: generate_key_pair_test_002
 * @tc.desc: Test function of GetIssuerKeyByAlias() interface without a keyalg FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_key_pair_test_002, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string issuerkeyStroeFile = "/data/test/generateCA/ohtest.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_STORE_FILE] = issuerkeyStroeFile;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    LocalizationAdapter adapter(&params);
    adapter.SetIssuerKeyStoreFile(true);
    EVP_PKEY* issueKeyPair = nullptr;
    adapter.GetKeyPair(true, &issueKeyPair);
    EXPECT_NE(issueKeyPair, nullptr);
}
/**
 * @tc.name: generate_key_pair_test_003
 * @tc.desc: Test function of GenerateKeyPair() interface for read key FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_key_pair_test_003, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string issuerKeyAlias = "oh";
    std::string issuerkeyStroeFile = "/data/test/generateCA/ohtest.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    char isskeypwd[] = "123456";
    params[Options::KEY_RIGHTS] = secret;
    params[Options::KEY_STORE_RIGHTS] = ksPwd;
    params[Options::ISSUER_KEY_STORE_FILE] = issuerkeyStroeFile;
    params[Options::ISSUER_KEY_RIGHTS] = isksPwd;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    params[Options::SIGN_ALG] = signAlg;
    params[Options::KEY_STORE_FILE] = keystoreFile;
    LocalizationAdapter adapter(&params);
    adapter.SetIssuerKeyStoreFile(true);
    EVP_PKEY* issueKeyPair = adapter.GetIssuerKeyByAlias();
    EXPECT_EQ(issueKeyPair, nullptr);
}
/**
 * @tc.name: generate_key_pair_test_004
 * @tc.desc: Test function of GenerateKeyPair() interface whit empty issuer all info FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_key_pair_test_004, testing::ext::TestSize.Level1)
{
    SignToolServiceImpl api;
    Options params;
    std::string issuerKeyAlias = "";
    std::string issuerkeyStroeFile = "";
    char isskeypwd[] = {0};
    params[Options::ISSUER_KEY_STORE_FILE] = issuerkeyStroeFile;
    params[Options::ISSUER_KEY_STORE_RIGHTS] = isskeypwd;
    params[Options::ISSUER_KEY_ALIAS] = issuerKeyAlias;
    LocalizationAdapter adapter(&params);
    adapter.SetIssuerKeyStoreFile(true);
    EVP_PKEY* issueKeyPair = adapter.GetIssuerKeyByAlias();
    EXPECT_EQ(issueKeyPair, nullptr);
}

/**
 * @tc.name: set_expandedInf_ext_one_test_001
 * @tc.desc: Test function of SetKeyUsage() interface whit a empty keyusage FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_expandedInf_ext_one_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    X509* cert = X509_new();
    bool keyUsageCritical = true;
    std::string keyUsage = "";
    (*params)["keyUsage"] = keyUsage;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    bool cert1 = CertTools::SetKeyUsage(cert, params.get());
    EXPECT_NE(cert1, false);
}

/**
 * @tc.name: set_expandedInf_ext_one_test_002
 * @tc.desc: Test function of SetKeyUsage() interface for error branch FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_expandedInf_ext_one_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    X509* cert = X509_new();
    bool keyUsageCritical = false;
    std::string keyUsage = "digitalSignature,nonRepudiation";
    (*params)["keyUsage"] = keyUsage;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    bool cert1 = CertTools::SetKeyUsage(cert, params.get());
    EXPECT_NE(cert1, false);
}

/**
 * @tc.name: set_expandedInf_ext_two_test_001
 * @tc.desc: Test function of SetkeyUsageExt() interface for error branch FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_expandedInf_ext_two_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    X509* cert = X509_new();
    bool extKeyUsageCritical = true;
    std::string extKeyUsage = "clientAuthentication";
    (*params)["extKeyUsageCritical"] = extKeyUsageCritical;
    (*params)["extKeyUsage"] = extKeyUsage;
    bool cert1 = CertTools::SetkeyUsageExt(cert, params.get());
    EXPECT_NE(cert1, false);
}

/**
 * @tc.name: set_expandedInf_ext_two_test_002
 * @tc.desc: Test function of SetkeyUsageExt() interface for error branch FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, set_expandedInf_ext_two_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    X509* cert = X509_new();
    bool extKeyUsageCritical = false;
    std::string extKeyUsage = "";
    (*params)["extKeyUsageCritical"] = extKeyUsageCritical;
    (*params)["extKeyUsage"] = extKeyUsage;
    bool cert1 = CertTools::SetkeyUsageExt(cert, params.get());
    EXPECT_NE(cert1, false);
}

/**
 * @tc.name: generate_sub_cert_to_file_test_001
 * @tc.desc: Test function of GenerateCaToFile() interface with error keystorefile FAILED.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_to_file_test_001, testing::ext::TestSize.Level1)
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
 * @tc.name: generate_sub_cert_to_file_test_002
 * @tc.desc: Test function of GenerateCaToFile() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_to_file_test_002, testing::ext::TestSize.Level1)
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
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    EXPECT_NE(csr, nullptr);
    X509* cert = CertTools::GenerateRootCertificate(keyPair, csr, params.get());
    EXPECT_NE(cert, nullptr);
    bool ret = api->GenerateSubCertToFile(params.get(), keyPair);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: generate_sub_cert_to_file_test_003
 * @tc.desc: Test function of GenerateCaToFile() interface for SUCCESS .
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_to_file_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
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
    EXPECT_NE(cert, nullptr);
    bool ret = api->GenerateSubCertToFile(params.get(), keyPair);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: run_app_cert_err_test_001
 * @tc.desc: Test function of RunAppCert() interface for failed due to outFile is not exist.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, run_app_cert_err_test_001, testing::ext::TestSize.Level1)
{
    Options options;
    SignToolServiceImpl service;
    options[Options::ISSUER_KEY_STORE_FILE] = std::string("/data/test/generateCA/ohtest.p12");
    options[Options::CA_CERT_FILE] = std::string("/data/test/generateCA/root-ca1.cer");
    options[Options::SUB_CA_CERT_FILE] = std::string("/data/test/generateCA/app-sign-srv-ca1.cer");
    options[Options::KEY_STORE_FILE] = std::string("/data/test/generateCA/OpenHarmony.p12");

    options[Options::OUT_FILE] = "./nohave/test.cer";
    bool ret = ParamsRunTool::RunAppCert(&options, service);
    EXPECT_EQ(ret, false);
}
}
}