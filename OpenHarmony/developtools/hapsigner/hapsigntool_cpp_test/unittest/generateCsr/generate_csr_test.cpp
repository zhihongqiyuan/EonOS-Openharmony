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
#include <cstdlib>

#include "gtest/gtest.h"
#include "openssl/x509.h"
#include "options.h"
#include "sign_tool_service_impl.h"
#include "cert_tools.h"
#include "params_run_tool.h"
#include "file_utils.h"
#include "hash_utils.h"
#include "constant.h"
#include "securec.h"
#include "file_data_source.h"
#include "byte_buffer_data_source.h"

namespace OHOS {
namespace SignatureTools {

class GenerateCsrTest : public testing::Test {
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
 * @tc.name: generate_csr_test_001
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_001, testing::ext::TestSize.Level1) // 已有p12直接生成csr
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-1.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: generate_csr_test_002
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_002, testing::ext::TestSize.Level1) // 生成p12并用于生成csr
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    (*params)["keyAlias"] = std::string("oh-app1-key-v2");
    (*params)["keyAlg"] = std::string("ECC");
    (*params)["keySize"] = 256;
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest2.p12");

    {
        char keyPwd[] = "keyPwdChars";
        char keystorePwd[] = "keystorePwdChars";
        (*params)["keyPwd"] = keyPwd;
        (*params)["keystorePwd"] = keystorePwd;
        bool ret = api->GenerateKeyStore(params.get());
        EXPECT_EQ(ret, true);
    }

    {
        char keyPwd[] = "keyPwdChars";
        char keystorePwd[] = "keystorePwdChars";
        (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App2 Release");
        (*params)["signAlg"] = std::string(SIGN_ALG_SHA384);
        (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-2.csr");
        (*params)["keyPwd"] = keyPwd;
        (*params)["keystorePwd"] = keystorePwd;
        bool ret = api->GenerateCsr(params.get());
        EXPECT_EQ(ret, true);
    }
}

/**
 * @tc.name: generate_csr_test_003
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_003, testing::ext::TestSize.Level1) // 已有p12直接生成csr但必选参数缺失如subject
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-3.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_csr_test_004
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_004, testing::ext::TestSize.Level1) // 已有p12直接生成csr但subject参数字符串格式无效
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-4.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_csr_test_005
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_005, testing::ext::TestSize.Level1) // 已有p12直接生成csr但subject参数格式无效
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,ABC=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-5.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_csr_test_006
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_006, testing::ext::TestSize.Level1) // 已有p12直接生成csr但keyAlias错误
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v6");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-6.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_csr_test_007
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_007, testing::ext::TestSize.Level1) // 已有p12直接生成csr但keystorePwd错误
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "654321";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-7.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_csr_test_008
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_008, testing::ext::TestSize.Level1) // 已有p12直接生成csr但signAlg错误
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string("SHA256");
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-8.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_csr_test_009
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_009, testing::ext::TestSize.Level1) // 已有p12直接生成csr不输出到文件
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: generate_csr_test_010
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_010, testing::ext::TestSize.Level1) // 已有p12直接生成csr但路径错误
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/d/test/resource/oh-app1-key-10.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, false);
}


/**
 * @tc.name: generate_csr_test_011
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_011, testing::ext::TestSize.Level1) // 必选参数缺失keystoreFile, keystorePwd
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-11.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: generate_csr_test_012
 * @tc.desc: Test function of SignToolServiceImpl::GenerateCsr() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_012, testing::ext::TestSize.Level1) // 已有p12直接生成csr
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = std::string("/data/test/generateCsr/oh-app1-key-12.csr");

    bool ret = api->GenerateCsr(params.get());
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: generate_csr_test_013
 * @tc.desc: Test function of CertTools::GenerateCsr() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_013, testing::ext::TestSize.Level1)
{
    int keySize = 256;
    std::string algorithm = "ECC";
    std::string signAlgorithm = SIGN_ALG_SHA256;
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);
    EXPECT_NE(keyPair, nullptr);
    if (!keyPair) {
        X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
        EXPECT_NE(csr, nullptr);
        if (csr) {
            X509_REQ_free(csr);
        }
        EVP_PKEY_free(keyPair);
    }
}

/**
 * @tc.name: generate_csr_test_014
 * @tc.desc: Test function of CertTools::GenerateCsr() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, generate_csr_test_014, testing::ext::TestSize.Level1)
{
    std::string signAlgorithm = SIGN_ALG_SHA256;
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    X509_REQ* csr = CertTools::GenerateCsr(nullptr, signAlgorithm, subject);
    EXPECT_EQ(csr, nullptr);
}

/**
 * @tc.name: csr_to_string_test_001
 * @tc.desc: Test function of CertTools::CsrToString() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, csr_to_string_test_001, testing::ext::TestSize.Level1)
{
    int keySize = 256;
    std::string algorithm = "ECC";
    std::string signAlgorithm = SIGN_ALG_SHA256;
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::shared_ptr<KeyStoreHelper> keyStoreHelper = std::make_shared<KeyStoreHelper>();
    EVP_PKEY* keyPair = keyStoreHelper->GenerateKeyPair(algorithm, keySize);

    EXPECT_NE(keyPair, nullptr);

    if (keyPair) {
        X509_REQ* csr = CertTools::GenerateCsr(keyPair, signAlgorithm, subject);
        EXPECT_NE(csr, nullptr);
        if (csr) {
            std::string csrStr = CertTools::CsrToString(csr);
            EXPECT_NE(csrStr.size(), 0U);
            X509_REQ_free(csr);
        }
        EVP_PKEY_free(keyPair);
    }
}

/**
 * @tc.name: csr_to_string_test_002
 * @tc.desc: Test function of CertTools::CsrToString() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, csr_to_string_test_002, testing::ext::TestSize.Level1)
{
    std::string csrStr = CertTools::CsrToString(nullptr);
    EXPECT_EQ(csrStr, "");
}

/**
 * @tc.name: output_string_test_001
 * @tc.desc: Test function of SignToolServiceImpl::OutputString() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, output_string_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    EXPECT_EQ(api->OutputString("hello world", "/data/test/generateCsr/output_string.txt"), true);
}

/**
 * @tc.name: run_csr_test_001
 * @tc.desc: Test function of ParamsRunTool::RunCsr() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, run_csr_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: run_csr_test_002
 * @tc.desc: Test function of ParamsRunTool::RunCsr() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, run_csr_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string("SHA256");
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: run_csr_test_003
 * @tc.desc: Test function of ParamsRunTool::RunCsr() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, run_csr_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: run_csr_test_004
 * @tc.desc: Test function of ParamsRunTool::RunCsr() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, run_csr_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*params)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p11");
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: build_dn_test_001
 * @tc.desc: Test function of BuildDN() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, build_dn_test_001, testing::ext::TestSize.Level1)
{
    X509_REQ* csr = X509_REQ_new();
    EXPECT_NE(csr, nullptr);
    if (csr) {
        X509_NAME* name = BuildDN("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release", csr);
        EXPECT_NE(name, nullptr);
        if (name) {
            X509_NAME_free(name);
        }
    }
}

/**
 * @tc.name: check_dn_test_001
 * @tc.desc: Test function of g_checkDn() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, check_dn_test_001, testing::ext::TestSize.Level1)
{
    std::vector<pair<std::string, std::string>> pairs;
    EXPECT_EQ(g_checkDn("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release", pairs), 0);
}

/**
 * @tc.name: split_string_test_001
 * @tc.desc: Test function of StringUtils::SplitString() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, split_string_test_001, testing::ext::TestSize.Level1)
{
    std::string str = "0:0:0:0";
    std::vector<std::string> vec = StringUtils::SplitString(str, ':');
    EXPECT_EQ(vec.size(), 4U);
    if (vec.size() == 4) {
        EXPECT_EQ(vec[0].size(), 1U);
        if (vec[0].size() == 1U) {
            EXPECT_EQ(vec[0][0], '0');
        }
    }
}

/**
 * @tc.name: trim_test_001
 * @tc.desc: Test function of StringUtils::Trim() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, trim_test_001, testing::ext::TestSize.Level1)
{
    std::string str = " \n\r\f\va \n\r\f\v";
    std::string str1 = StringUtils::Trim(str);
    EXPECT_EQ(str1.size(), 1U);
}

/**
 * @tc.name: call_generators_test_001
 * @tc.desc: Test function of ParamsRunTool::CallGenerators() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, call_generators_test_001, testing::ext::TestSize.Level1)
{
    ParamsSharedPtr params = std::make_shared<Params>();
    params->SetMethod(GENERATE_CSR);
    Options* options = params->GetOptions();
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";
    (*options)["keyAlias"] = std::string("oh-app1-key-v1");
    (*options)["keyPwd"] = keyPwd;
    (*options)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*options)["signAlg"] = std::string(SIGN_ALG_SHA256);
    (*options)["keystoreFile"] = std::string("/data/test/generateCsr/ohtest.p12");
    (*options)["keystorePwd"] = keystorePwd;

    EXPECT_EQ(ParamsRunTool::CallGenerators(params, *api), true);
}

/**
 * @tc.name: process_cmd_test_001
 * @tc.desc: Test function of ParamsRunTool::ProcessCmd() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, process_cmd_test_001, testing::ext::TestSize.Level1)
{
    char arg0[] = "", arg1[] = "generate-csr", arg2[] = "-keyAlias", arg3[] = "oh-app1-key-v1",
        arg4[] = "-subject", arg5[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
        arg6[] = "-signAlg", arg8[] = "-keystoreFile",
        arg9[] = "/data/test/generateCsr/ohtest.p12", arg10[] = "-keystorePwd", arg11[] = "123456",
        arg12[] = "-keyPwd", arg13[] = "123456";
    char* arg7 = new char[SIGN_ALG_SHA256.size() + 1]();
    arg7[SIGN_ALG_SHA256.size()] = '\0';
    EXPECT_EQ(memcpy_s(arg7, SIGN_ALG_SHA256.size(), SIGN_ALG_SHA256.c_str(), SIGN_ALG_SHA256.size()), 0);
    char* args[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13 };
    EXPECT_EQ(ParamsRunTool::ProcessCmd(args, sizeof(args) / sizeof((char*)arg0)), true);
    delete[] arg7;
}

/**
 * @tc.name: process_cmd_test_003
 * @tc.desc: Test function of ParamsRunTool::ProcessCmd() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, process_cmd_test_003, testing::ext::TestSize.Level1)
{
    char arg0[] = "", arg1[] = "-h", arg2[] = "";
    char* args[] = { arg0, arg1 };
    EXPECT_EQ(ParamsRunTool::ProcessCmd(args, sizeof(args) / sizeof((char*)arg0)), true);
    char* args1[] = { arg0, arg2 };
    EXPECT_EQ(ParamsRunTool::ProcessCmd(args, sizeof(args1) / sizeof((char*)arg0)), true);
}

/**
 * @tc.name: process_cmd_test_004
 * @tc.desc: Test function of ParamsRunTool::ProcessCmd() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, process_cmd_test_004, testing::ext::TestSize.Level1)
{
    char arg0[] = "", arg1[] = "-v";
    char* args[] = { arg0, arg1 };
    ParamsRunTool::Version();
    EXPECT_EQ(ParamsRunTool::ProcessCmd(args, sizeof(args) / sizeof((char*)arg0)), true);
}

/**
 * @tc.name: is_valid_file_test_001
 * @tc.desc: Test function of FileUtils::IsValidFile() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, is_valid_file_test_001, testing::ext::TestSize.Level1)
{
    EXPECT_EQ(FileUtils::IsValidFile("/data/test/generateCsr/ohtest.p12"), true);
    EXPECT_EQ(FileUtils::IsValidFile("/data/test/generateCsr/ohtest.p11"), false);
    EXPECT_EQ(FileUtils::IsValidFile("/data/test/generateCsr/"), false);
}

/**
 * @tc.name: convert2params_test_001
 * @tc.desc: Test function of CmdUtil::Convert2Params() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, convert2params_test_001, testing::ext::TestSize.Level1)
{
    char arg0[] = "", arg1[] = "generate-csr", arg2[] = "-keyAlias", arg3[] = "oh-app1-key-v1",
        arg4[] = "-subject", arg5[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
        arg6[] = "-signAlg", arg8[] = "-keystoreFile",
        arg9[] = "/data/test/generateCsr/ohtest.p12", arg10[] = "-keystorePwd", arg11[] = "123456",
        arg12[] = "-keyPwd", arg13[] = "123456";
    char* arg7 = new char[SIGN_ALG_SHA256.size() + 1]();
    arg7[SIGN_ALG_SHA256.size()] = '\0';
    EXPECT_EQ(memcpy_s(arg7, SIGN_ALG_SHA256.size(), SIGN_ALG_SHA256.c_str(), SIGN_ALG_SHA256.size()), 0);
    char* args[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13 };
    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;
    EXPECT_EQ(cmdUtil.Convert2Params(args, sizeof(args) / sizeof((char*)arg0), param), true);
    delete[] arg7;
}

/**
 * @tc.name: convert2params_test_002
 * @tc.desc: Test function of CmdUtil::Convert2Params() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, convert2params_test_002, testing::ext::TestSize.Level1)
{
    char arg0[] = "", arg1[] = "generate", arg2[] = "-keyAlias", arg3[] = "oh-app1-key-v1",
        arg4[] = "-subject", arg5[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
        arg6[] = "-signAlg", arg8[] = "-keystoreFile",
        arg9[] = "/data/test/generateCsr/ohtest.p12", arg10[] = "-keystorePwd", arg11[] = "123456",
        arg12[] = "-keyPwd", arg13[] = "123456";
    char* arg7 = new char[SIGN_ALG_SHA256.size() + 1]();
    arg7[SIGN_ALG_SHA256.size()] = '\0';
    EXPECT_EQ(memcpy_s(arg7, SIGN_ALG_SHA256.size(), SIGN_ALG_SHA256.c_str(), SIGN_ALG_SHA256.size()), 0);
    char* args[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13 };
    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;
    EXPECT_EQ(cmdUtil.Convert2Params(args, sizeof(args) / sizeof((char*)arg0), param), false);
    delete[] arg7;
}

/**
 * @tc.name: convert2params_test_003
 * @tc.desc: Test function of CmdUtil::Convert2Params() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, convert2params_test_003, testing::ext::TestSize.Level1)
{
    char arg0[] = "", arg1[] = "generate-csr", arg2[] = "-keyAlias", arg3[] = "oh-app1-key-v1",
        arg4[] = "-subject", arg5[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
        arg6[] = "-signAlg", arg8[] = "-keystoreFile",
        arg9[] = "/data/test/generateCsr/ohtest.p12", arg10[] = "-keystorePwd", arg11[] = "123456",
        arg12[] = "-keyPwd", arg13[] = "123456", arg14[] = "-signcode";
    char* arg7 = new char[SIGN_ALG_SHA256.size() + 1]();
    arg7[SIGN_ALG_SHA256.size()] = '\0';
    EXPECT_EQ(memcpy_s(arg7, SIGN_ALG_SHA256.size(), SIGN_ALG_SHA256.c_str(), SIGN_ALG_SHA256.size()), 0);
    char* args[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14 };
    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;
    EXPECT_EQ(cmdUtil.Convert2Params(args, sizeof(args) / sizeof((char*)arg0), param), false);
    delete[] arg7;
}

/**
 * @tc.name: convert2params_test_004
 * @tc.desc: Test function of CmdUtil::Convert2Params() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, convert2params_test_004, testing::ext::TestSize.Level1)
{
    char arg0[] = "", arg1[] = "generate-csr", arg2[] = "-keyAlias", arg3[] = "oh-app1-key-v1",
        arg4[] = "-subject", arg5[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
        arg6[] = "-signAlg", arg8[] = "-keystoreFile",
        arg9[] = "/data/test/generateCsr/ohtest.p12", arg10[] = "-keystorePwd", arg11[] = "123456",
        arg12[] = "-keyPwd", arg13[] = "123456", arg14[] = "";
    char* arg7 = new char[SIGN_ALG_SHA256.size() + 1]();
    arg7[SIGN_ALG_SHA256.size()] = '\0';
    EXPECT_EQ(memcpy_s(arg7, SIGN_ALG_SHA256.size(), SIGN_ALG_SHA256.c_str(), SIGN_ALG_SHA256.size()), 0);
    char* args[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13, arg14 };
    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;
    EXPECT_EQ(cmdUtil.Convert2Params(args, sizeof(args) / sizeof((char*)arg0), param), false);
    delete[] arg7;
}

/**
 * @tc.name: get_method_test_001
 * @tc.desc: Test function of Params::GetMethod() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, get_method_test_001, testing::ext::TestSize.Level1)
{
    Params params;
    params.SetMethod("test");
    EXPECT_EQ(params.GetMethod(), "test");
}

/**
 * @tc.name: dispatch_params_test_001
 * @tc.desc: Test function of ParamsRunTool::DispatchParams() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, dispatch_params_test_001, testing::ext::TestSize.Level1)
{
    char arg0[] = "", arg1[] = "generate-csr", arg2[] = "-keyAlias", arg3[] = "oh-app1-key-v1",
        arg4[] = "-subject", arg5[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release",
        arg6[] = "-signAlg", arg8[] = "-keystoreFile",
        arg9[] = "/data/test/generateCsr/ohtest.p12", arg10[] = "-keystorePwd", arg11[] = "123456",
        arg12[] = "-keyPwd", arg13[] = "123456";
    char* arg7 = new char[SIGN_ALG_SHA256.size() + 1]();
    arg7[SIGN_ALG_SHA256.size()] = '\0';
    EXPECT_EQ(memcpy_s(arg7, SIGN_ALG_SHA256.size(), SIGN_ALG_SHA256.c_str(), SIGN_ALG_SHA256.size()), 0);
    char* args[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10,
        arg11, arg12, arg13 };
    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;
    EXPECT_EQ(cmdUtil.Convert2Params(args, sizeof(args) / sizeof((char*)arg0), param), true);
    std::shared_ptr<SignToolServiceImpl> service_api = std::make_shared<SignToolServiceImpl>();
    EXPECT_EQ(ParamsRunTool::DispatchParams(param, *service_api), true);
    delete[] arg7;
}

/**
 * @tc.name: get_hash_algs_id_test_001
 * @tc.desc: Test function of HashUtils::GetHashAlgsId() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, get_hash_algs_id_test_001, testing::ext::TestSize.Level1)
{
    HashUtils::GetHashAlgsId("SHA-224");
    HashUtils::GetHashAlgsId("SHA-384");
    HashUtils::GetHashAlgsId("SHA-512");
    int ret = HashUtils::GetHashAlgsId("SHA-1024");
    EXPECT_EQ(ret, 0);
}

/**
 * @tc.name: get_hash_algs_name_test_001
 * @tc.desc: Test function of HashUtils::GetHashAlgName() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, get_hash_algs_name_test_001, testing::ext::TestSize.Level1)
{
    HashUtils::GetHashAlgName(5);
    HashUtils::GetHashAlgName(7);
    HashUtils::GetHashAlgName(8);
    std::string ret = HashUtils::GetHashAlgName(9);
    EXPECT_EQ(ret, "");
}

/**
 * @tc.name: read_data_and_digest_update_test_001
 * @tc.desc: Test function of FileDataSource::ReadDataAndDigestUpdate() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, read_data_and_digest_update_test_001, testing::ext::TestSize.Level1)
{
    HashUtils::GetDigestFromBytes({}, 1, "");
    HashUtils::GetDigestFromBytes({ 'a' }, 0, "");

    RandomAccessFile file;
    FileDataSource src(file, 0, 0, 0);
    DigestParameter param;
    bool ret = src.ReadDataAndDigestUpdate(param, -1);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: read_data_and_digest_update_test_002
 * @tc.desc: Test function of ByteBufferDataSource::ReadDataAndDigestUpdate() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(GenerateCsrTest, read_data_and_digest_update_test_002, testing::ext::TestSize.Level1)
{
    ByteBuffer buffer;
    ByteBufferDataSource src(buffer);
    DigestParameter param;
    bool ret = src.ReadDataAndDigestUpdate(param, -1);
    EXPECT_EQ(ret, false);
}

} // namespace SignatureTools
} // namespace OHOS