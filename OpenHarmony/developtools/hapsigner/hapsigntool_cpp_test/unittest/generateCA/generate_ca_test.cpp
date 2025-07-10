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


// rootCa
/**
 * @tc.name: generate_ca_test_001
 * @tc.desc: Test function of GenerateCa() generate a root certificate.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/profile.cer";
    int keySize = 384;
    int validity = 3650;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    (*params)["validity"] = validity;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, true);
}

// rootCa
/**
 * @tc.name: generate_ca_test_002
 * @tc.desc: Test function of GenerateCa() generate a root certificate.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/root-ca-test.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, true);
}

// rootCa
/**
 * @tc.name: generate_ca_test_003
 * @tc.desc: Test function of GenerateCa() FAILED to generate a root certificate.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/root-ca-test.cer";
    int keySize = 384;
    int validity = 365;
    int basicConstraintsPathLen = 0;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["validity"] = validity;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_004
 * @tc.desc: Test function of GenerateCa() FAILED to generate a root certificate without a subject.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/root-ca-test.cer";
    int keySize = 384;
    int validity = 365;
    int basicConstraintsPathLen = 0;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["validity"] = validity;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_005
 * @tc.desc: Test function of GenerateCa() FAILED to generate a root certificate without a keyalg.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_005, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/root-ca-test.cer";
    int keySize = 384;
    int validity = 365;
    int basicConstraintsPathLen = 0;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keySize"] = keySize;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["validity"] = validity;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_006
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error key password.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_006, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/profile.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "adhjkljasjhdk";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_007
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error skystore password.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_007, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/profile.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "adhjkljasjhdk";
    char ksPwd[] = "123456";
    char isksPwd[] = "554245";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_008
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error issuerkeystore password.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_008, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/profile.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.pp12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "adhjkljasjhdk";
    char ksPwd[] = "123456";
    char isksPwd[] = "554245";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_009
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error keystorefile path.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_009, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 5;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/profile.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "adhjkljasjhdk";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_010
 * @tc.desc: Test function of GenerateCa() SUCCESS to generate a root certificate.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_010, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 5;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/root-ca-test.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: generate_ca_test_011
 * @tc.desc: Test function of GenerateCa() FAILED to generate a root certificate without all pwd.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_011, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/root-ca-test.cer";
    int keySize = 384;
    int validity = 365;
    int basicConstraintsPathLen = 5;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["validity"] = validity;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_012
 * @tc.desc: Test function of GenerateCa() FAILED to generate a root certificate without a subject.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_012, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/root-ca-test.cer";
    int keySize = 384;
    int validity = 365;
    int basicConstraintsPathLen = 5;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["validity"] = validity;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_013
 * @tc.desc: Test function of GenerateCa() FAILED to generate a root certificate without a keyalg.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_013, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/root-ca-test.cer";
    int keySize = 384;
    int validity = 365;
    int basicConstraintsPathLen = 5;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keySize"] = keySize;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["validity"] = validity;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_014
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error outfile path.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_014, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    int basicConstraintsPathLen = 5;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/profile.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "adhjkljasjhdk";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;

    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_015
 * @tc.desc: Test function of GenerateCa() SUCCESS to generate a root certificate.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_015, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-root-ca-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string outFile = "/data/test/generateCA/root-ca1.cer";
    int validity = 365;
    int keySize = 384;
    char keypwd[] = "123456";
    char ksPwd[] = "123456";
    (*params)["keyPwd"] = keypwd;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    (*params)["validity"] = validity;
    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: generate_ca_test_016
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error keyalias.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_016, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string issuerkeyAlias = "oh-root-ca-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Application Signature Service CA";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string outFile = "/data/test/generateCA/app-sign-srv-ca1.cer";
    int validity = 365;
    int keySize = 384;
    char keypwd[] = "123456";
    char ksPwd[] = "123456";
    char issuerPwd[] = "123456";
    (*params)["keyPwd"] = keypwd;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeyPwd"] = issuerPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    (*params)["validity"] = validity;
    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_ca_test_017
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error issuerkeyalias.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_ca_test_017, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string issuerkeyAlias = "oh-root-ca-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Application Signature Service CA";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/OpenHarmony.p12";
    std::string outFile = "/data/test/generateCA/profile-sign-srv-ca1.cer";
    int validity = 365;
    int keySize = 384;
    char keypwd[] = "123456";
    char ksPwd[] = "123456";
    char issuerPwd[] = "123456";
    (*params)["keyPwd"] = keypwd;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeyPwd"] = issuerPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    (*params)["validity"] = validity;
    bool ret = api->GenerateCA(params.get());
    EXPECT_EQ(ret, false);
}


/**
 * @tc.name: generate_sub_cert_test_001
 * @tc.desc: Test function of GenerateCa() SUCCESS to generate a sub certificate.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_test_001, testing::ext::TestSize.Level1)
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
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateSubCert(keyPair, csr, params.get());
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: generate_sub_cert_test_002
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error keyalias.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/subca.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = EVP_PKEY_new();
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = X509_REQ_new();
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateSubCert(keyPair, csr, params.get());
    EXPECT_EQ(ret, false);
}


/**
 * @tc.name: generate_sub_cert_test_003
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a empty signalg.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/subca.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = EVP_PKEY_new();
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = X509_REQ_new();
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateSubCert(keyPair, csr, params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_sub_cert_test_004
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error issuerkeystorefile.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 5;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/subca.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = EVP_PKEY_new();
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = X509_REQ_new();
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateSubCert(keyPair, csr, params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_sub_cert_test_005
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error signalg.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_test_005, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "";
    int basicConstraintsPathLen = 5;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/subca.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = EVP_PKEY_new();
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = X509_REQ_new();
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateSubCert(keyPair, csr, params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_sub_cert_test_006
 * @tc.desc: Test function of GenerateCa() FAILED to generate a sub certificate with a error keyalias.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_sub_cert_test_006, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    int basicConstraintsPathLen = 0;
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string outFile = "/data/test/generateCA/subca.cer";
    std::string issuerKeystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int keySize = 384;
    char secret[] = "123456";
    char ksPwd[] = "123456";
    char isksPwd[] = "123456";
    int validity = 365;
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = ksPwd;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerkeyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;
    (*params)["basicConstraintsPathLen"] = basicConstraintsPathLen;
    (*params)["outFile"] = outFile;
    (*params)["validity"] = validity;
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = EVP_PKEY_new();
    EXPECT_NE(keyPair, nullptr);
    X509_REQ* csr = X509_REQ_new();
    EXPECT_NE(csr, nullptr);
    bool ret = CertTools::GenerateSubCert(keyPair, csr, params.get());
    EXPECT_EQ(ret, false);
}


/**
 * @tc.name: valid_file_type_test_001
 * @tc.desc: Test function of ValidFileType() SUCCESS check file suffix
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, valid_file_type_test_001, testing::ext::TestSize.Level1)
{
    std::string issuerKeystoreFile = "ab.p12";
    bool ret = FileUtils::ValidFileType(issuerKeystoreFile, { "p12", "jks" });
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: get_chars_test_001
 * @tc.desc: Test function of Options::GetChars() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_chars_test_001, testing::ext::TestSize.Level1)
{
    Options option;
    const std::string test = "test";
    char value[6] = "value";
    option[test] = value;
    char* tmp = option.GetChars(test);
    EXPECT_EQ(std::strcmp("value", tmp), 0);
}

/**
 * @tc.name: get_string_test_001
 * @tc.desc: Test function of Options::GetString() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, get_string_test_001, testing::ext::TestSize.Level1)
{
    Options option;
    std::string test = "test";
    std::string value = "value";
    option[test] = value;
    std::string str = option.GetString(test);
    EXPECT_EQ(std::strcmp("value", str.c_str()), 0);
}

/**
 * @tc.name: equals_test_001
 * @tc.desc: Test function of Options::Equals() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, equals_test_001, testing::ext::TestSize.Level1)
{
    Options option;
    const std::string test1 = "test1";
    const std::string test2 = "test2";
    bool ret = option.Equals(test1, test2);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: is_empty_test_001
 * @tc.desc: Test function of Options::IsEmpty() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, is_empty_test_001, testing::ext::TestSize.Level1)
{
    Options option;
    std::string test1 = "test1";
    bool ret = option.IsEmpty(test1);
    EXPECT_EQ(ret, false);
}

// general cert
/**
 * @tc.name: generate_cert_test_001
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keyalias.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
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
    int validity = 365;
    std::string outfile = "/data/test/generateCA/general.cer";
    (*params)["keyPwd"] = secret;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["issuerKeyPwd"] = issuerkeypwd;
    (*params)["keyAlias"] = keyAlias;
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
    (*params)["validity"] = validity;
    (*params)["outFile"] = outfile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

// general cert
/**
 * @tc.name: generate_cert_test_002
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keystorefile.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_002, testing::ext::TestSize.Level1)
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
    std::string outFile = "/data/test/generateCA/rootCa.cer";
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
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}


// general cert
/**
 * @tc.name: generate_cert_test_003
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keyusage.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_003, testing::ext::TestSize.Level1)
{
    SIGNATURE_TOOLS_LOGI(" welcome to  test space !!! ");
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CNA,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string keyUsage = "digitalSignature";
    std::string outFile = "general.cer";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
    char secret[] = "123456";
    char keystorePwd[] = "123456";
    int keySize = 384;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = keystorePwd;

    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

// general cert
/**
 * @tc.name: generate_cert_test_004
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a empty signalg.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    std::string signAlg = "";
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
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;

    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

// general cert
/**
 * @tc.name: generate_cert_test_005
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a empty subject.
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_005, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    int keySize = 256;
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
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
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["issuerkeyAlias"] = issuerkeyAlias;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}
// general cert
/**
 * @tc.name: generate_cert_test_006
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate without a keyalias.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_006, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string keyUsage = "digitalSignature";
    std::string outFile = "/datamsge/test/generateCA/general.cer";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
    char secret[] = "123456";
    char keystorePwd[] = "123456";
    int keySize = 384;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = keystorePwd;

    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_cert_test_007
 * @tc.desc: Test function of GenerateCert() FAILED to generate a sub certificate without a keyalias.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_007, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string subject = "";
    std::string signAlg = "";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string keyUsage = "digitalSignature";
    std::string outFile = "general.cer";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
    char secret[] = "123456";
    char keystorePwd[] = "123456";
    int keySize = 384;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = keystorePwd;

    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_cert_test_008
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keypwd.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_008, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CNA,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string signAlg = "SHA256withRSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string keyUsage = "digitalSignature";
    std::string outFile = "general.cer";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
    char secret[] = "123456";
    char keystorePwd[] = "123456";
    int keySize = 384;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = keystorePwd;

    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_cert_test_009
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error outfile.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_009, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "/data/test/generateCA/ohtest.p12";
    std::string keyUsage = "digitalSignature";
    std::string outFile = "/mjssngek/test/generateCA/general.cer";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
    char secret[] = "123456";
    char keystorePwd[] = "123456";
    int keySize = 384;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}

// general cert
/**
 * @tc.name: generate_cert_test_010
 * @tc.desc: Test function of GenerateCert() SUCCESS to generate a certificate.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_010, testing::ext::TestSize.Level1)
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
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string keyUsage = "digitalSignature";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
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
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlg, subject);
    X509 *cert = CertTools::GenerateCert(keyPair, csr, params.get());
    EXPECT_NE(cert, nullptr);
}

/**
 * @tc.name: generate_cert_test_015
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keyusagecritial.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_015, testing::ext::TestSize.Level1)
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
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string keyUsage = "digitalSignature";
    std::string basicConstraints = "false";
    std::string basicConstraintsCritical = "false";
    std::string basicConstraintsCa = "false";
    bool keyUsageCritical = true;
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
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    EVP_PKEY* keyPair = EVP_PKEY_new();
    X509_REQ* csr = X509_REQ_new();
    X509 *cert = CertTools::GenerateCert(keyPair, csr, params.get());
    EXPECT_EQ(cert, nullptr);
}

/**
 * @tc.name: generate_cert_test_016
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keyusagecritial.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_016, testing::ext::TestSize.Level1)
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
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string keyUsage = "digitalSignature";
    std::string basicConstraints = "false";
    std::string basicConstraintsCritical = "false";
    std::string basicConstraintsCa = "false";
    bool keyUsageCritical = true;
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
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EVP_PKEY* keyPair = nullptr;
    keyPair = adaptePtr->GetAliasKey(true);
    X509_REQ* csr = X509_REQ_new();
    X509 *cert = CertTools::GenerateCert(keyPair, csr, params.get());
    EXPECT_EQ(cert, nullptr);
}

/**
 * @tc.name: generate_cert_test_022
 * @tc.desc: Test function of GenerateCert() FAILED to generate a certificate with a error keyusagecritial.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCaTest, generate_cert_test_022, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "alias";
    std::string issuerkeyAlias = "oh-app1-key-v1";
    std::string keystoreFile = "/data/test/generateKeyPair/keypair.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string keyUsage = "digitalSignature";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    std::string keyUsageCritical = "true";
    char secret[] = "123456";
    char isksPwd[] = "123456";
    char keystorePwd[] = "123456";
    char issuerkeypwd[] = "123456";
    int validity = 365;
    std::string outfile = "/data/test/generateCA/general.cer";
    (*params)["keyPwd"] = secret;
    (*params)["issuerKeystorePwd"] = isksPwd;
    (*params)["issuerKeyPwd"] = issuerkeypwd;
    (*params)["keyAlias"] = keyAlias;
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
    (*params)["validity"] = validity;
    (*params)["outFile"] = outfile;
    bool ret = api->GenerateCert(params.get());
    EXPECT_EQ(ret, false);
}
}
}