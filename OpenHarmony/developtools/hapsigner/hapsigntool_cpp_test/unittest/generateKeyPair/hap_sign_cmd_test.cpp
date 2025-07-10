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

#include <gtest/gtest.h>
#include "signature_tools_log.h"
#include "options.h"
#include "sign_tool_service_impl.h"
#include "localization_adapter.h"
#include "openssl/ssl.h"
#include "openssl/pem.h"
#include "openssl/err.h"
#include "p12_local.h"
#include "cmd_util.h"
#include "file_utils.h"
#include "params_run_tool.h"
#include "constant.h"
#include "params.h"
#include "params_trust_list.h"
#include "param_constants.h"

namespace OHOS {
namespace SignatureTools {

class HapSignToolCmdTest : public testing::Test {
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

/*
 * @tc.name: hapSign_tool_cmd_test_001
 * @tc.desc: The generate-app-cert module checks whether the subCaCertFile parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_001, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/app-release1.pem";
    std::string subCaCertFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outForm = "certChain";
    std::string rootCaCertFile = "./generateKeyPair/root-ca1.cer";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["validity"] = validity;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["subCaCertFile"] = subCaCertFile;
    (*params)["outForm"] = outForm;
    (*params)["rootCaCertFile"] = rootCaCertFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_002
 * @tc.desc: The generate-app-cert module checks whether the keystoreFile parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_002, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/app-release1.pem";
    std::string subCaCertFile = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outForm = "certChain";
    std::string rootCaCertFile = "./generateKeyPair/root-ca1.cer";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["validity"] = validity;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["subCaCertFile"] = subCaCertFile;
    (*params)["outForm"] = outForm;
    (*params)["rootCaCertFile"] = rootCaCertFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_003
 * @tc.desc: The generate-app-cert module checks whether the issuerKeystoreFile parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_003, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/app-release1.pem";
    std::string subCaCertFile = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outForm = "certChain";
    std::string rootCaCertFile = "./generateKeyPair/root-ca1.cer";
    std::string issuerKeystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["validity"] = validity;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["subCaCertFile"] = subCaCertFile;
    (*params)["outForm"] = outForm;
    (*params)["rootCaCertFile"] = rootCaCertFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hapSign_tool_cmd_test_004
* @tc.desc: generate-profile-cert module required parameter validation.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_004, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/profile-release1.pem";
    std::string subCaCertFile = "./generateKeyPair/profile-sign-srv-ca1.cer";
    std::string outForm = "certChain";
    std::string rootCaCertFile = "./generateKeyPair/root-ca1.cer";

    (*params)["keyPwd"] = keyPwd;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["validity"] = validity;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["subCaCertFile"] = subCaCertFile;
    (*params)["outForm"] = outForm;
    (*params)["rootCaCertFile"] = rootCaCertFile;

    bool ret = ParamsRunTool::RunProfileCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_005
 * @tc.desc: generate-keypair module required parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_005, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 384;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunKeypair(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_006
 * @tc.desc: generate-keypair module algorithm validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_006, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "RSA";
    int keySize = 384;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunKeypair(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_007
 * @tc.desc: generate-keypair module algorithm size verification.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_007, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 999;
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunKeypair(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_008
 * @tc.desc: The generate-keypair module checks whether the keystoreFile parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_008, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
    int keySize = 384;
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunKeypair(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/**
 * @tc.name: hapSign_tool_cmd_test_009
 * @tc.desc: generate-csr module required parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_009, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignToolServiceImpl> api = std::make_shared<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char keyPwd[] = "123456";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = std::string("oh-app1-key-v1");
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = std::string("C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release");
    (*params)["keystoreFile"] = std::string("./generateKeyPair/OpenHarmony.p12");
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_010
 * @tc.desc: verify-profile module required parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_010, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string outFile = "./generateKeyPair/VerifyResult.json";
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunVerifyProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_011
 * @tc.desc: verify-profile module inFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_011, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/signed-profile.txt";
    std::string outFile = "./generateKeyPair/VerifyResult.json";

    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunVerifyProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_012
 * @tc.desc: verify-profile module outFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_012, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/signed-profile.p7b";
    std::string outFile = "./generateKeyPair/VerifyResult.txt";

    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunVerifyProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hapSign_tool_cmd_test_013
* @tc.desc: generate-cert module required parameter validation.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_013, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraintsPathLen";
    char arg23[] = "0";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_014
 * @tc.desc: generate-cert module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_014, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraintsPathLen";
    char arg23[] = "1000000000000000000000000000000000000000000000000000000";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_015
 * @tc.desc: generate-cert module two-parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_015, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-validity";
    char arg23[] = "558g22";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_016
 * @tc.desc: The validity parameter of the generate-cert module checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_016, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-validity";
    char arg23[] = "558g2hhhsss1111111111111111111111111111111111112";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10,
                     arg11, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 22;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_017
 * @tc.desc: generate-cert module keyUsageCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_017, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-keyUsageCritical";
    char arg23[] = "TRUE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_018
 * @tc.desc: generate-cert module keyUsageCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_018, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-keyUsageCritical";
    char arg23[] = "false";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_019
 * @tc.desc: generate-cert module keyUsageCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_019, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-keyUsageCritical";
    char arg23[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_020
 * @tc.desc: generate-cert module extKeyUsageCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_020, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-extKeyUsageCritical";
    char arg23[] = "TRUE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;
    cmdUtil.Convert2Params(argv, argc, param);
    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_021
 * @tc.desc: generate-cert module extKeyUsageCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_021, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-extKeyUsageCritical";
    char arg23[] = "FALSE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_022
 * @tc.desc: generate-cert module extKeyUsageCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_022, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-extKeyUsageCritical";
    char arg23[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_023
 * @tc.desc: generate-cert module basicConstraints parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_023, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraints";
    char arg23[] = "TRUE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_024
 * @tc.desc: generate-cert module basicConstraints parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_024, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraints";
    char arg23[] = "FALSE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_025
 * @tc.desc: generate-cert module basicConstraints parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_025, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraints";
    char arg23[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_026
 * @tc.desc: generate-cert module basicConstraintsCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_026, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraintsCritical";
    char arg23[] = "TRUE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_027
 * @tc.desc: generate-cert module basicConstraintsCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_027, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraintsCritical";
    char arg23[] = "FALSE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_028
 * @tc.desc: generate-cert module basicConstraintsCritical parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_028, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraintsCritical";
    char arg23[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_029
 * @tc.desc: generate-cert module basicConstraintsCa parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_029, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraintsCa";
    char arg23[] = "TRUE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_030
 * @tc.desc: generate-cert module basicConstraintsCa parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_030, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraintsCa";
    char arg23[] = "FALSE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_031
 * @tc.desc: generate-cert module basicConstraintsCa parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_031, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-profile-sign-srv-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    char arg12[] = "-validity";
    char arg13[] = "365";
    char arg14[] = "-signAlg";
    char arg15[] = "SHA384withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-basicConstraintsCa";
    char arg23[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_032
 * @tc.desc: sign-profile module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_032, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string profileCertFile = "./generateKeyPair/profile-release1.pem";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/signed-profile.p7b";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["profileCertFile"] = profileCertFile;
    (*params)["inFile"] = inFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_033
 * @tc.desc: sign-profile module mode parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_033, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "abcd";
    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string profileCertFile = "./generateKeyPair/profile-release1.pem";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/signed-profile.p7b";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["profileCertFile"] = profileCertFile;
    (*params)["inFile"] = inFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_034
 * @tc.desc: The sign-profile module profileCertFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_034, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string profileCertFile = "./generateKeyPair/profile-release1.pem";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA384withECDSA";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/signed-profile.p7b";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["profileCertFile"] = profileCertFile;
    (*params)["inFile"] = inFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_035
 * @tc.desc: The sign-profile module signAlg parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_035, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string profileCertFile = "./generateKeyPair/profile-release1.pem";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA384acd";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/signed-profile.p7b";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["profileCertFile"] = profileCertFile;
    (*params)["inFile"] = inFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_036
 * @tc.desc: The verify-app module inform parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_036, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outCertChain = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outProfile = "./generateKeyPair/app-profile.p7b";
    std::string inform = "abcd";

    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    (*params)["inForm"] = inform;

    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hapSign_tool_cmd_test_037
* @tc.desc: The generate-ca module outFile parameter checks whether it is valid.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_037, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    std::string keyAlg = "ECC";
    int keySize = 384;
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string outFile = "./generateKeyPair/abc/rootca";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_038
 * @tc.desc: The generate-cert module outFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_038, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string keyUsage = "digitalSignature";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string outFile = "./generateKeyPair/abc/general.cer";

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["keyUsage"] = keyUsage;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    
    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_039
 * @tc.desc: The generate-cert module keystoreFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_039, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string keyUsage = "digitalSignature";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/general.cer";

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["keyUsage"] = keyUsage;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    
    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_040
 * @tc.desc: The generate-cert module subCaCertFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_040, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string outForm = "certChain";
    std::string subCaCertFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string rootCaCertFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outForm"] = outForm;
    (*params)["subCaCertFile"] = subCaCertFile;
    (*params)["rootCaCertFile"] = rootCaCertFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_041
 * @tc.desc: The generate-cert module keystoreFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_041, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outForm = "certChain";
    std::string subCaCertFile = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string rootCaCertFile = "./generateKeyPair/app-sign-srv-ca1.cer";

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outForm"] = outForm;
    (*params)["subCaCertFile"] = subCaCertFile;
    (*params)["rootCaCertFile"] = rootCaCertFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_042
 * @tc.desc: The generate-cert module issuerKeystoreFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_042, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string outForm = "certChain";
    std::string subCaCertFile = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string rootCaCertFile = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string issuerKeystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outForm"] = outForm;
    (*params)["subCaCertFile"] = subCaCertFile;
    (*params)["rootCaCertFile"] = rootCaCertFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hapSign_tool_cmd_test_043
* @tc.desc: The generate-profile-cert module keystoreFile parameter checks whether it is valid.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_043, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keystoreFile = "./generateKeyPair/abc/OpenHarmonyDamage.p12";
    (*params)["keystoreFile"] = keystoreFile;
    bool ret = ParamsRunTool::RunProfileCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hapSign_tool_cmd_test_044
* @tc.desc: The generate-profile-cert module outFile parameter checks whether it is valid.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_044, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string outFile = "./generateKeyPair/abc/OpenHarmonyDamage.p12";
    (*params)["outFile"] = outFile;
    bool ret = ParamsRunTool::RunProfileCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_045
 * @tc.desc: The generate-csr module outFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_045, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA256withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string outFile = "./generateKeyPair/abc/oh-app1-key-v1.csr";

    (*params)["keyAlias"] = keyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_046
 * @tc.desc: The generate-csr module keystoreFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_046, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA256withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/oh-app1-key-v1.csr";

    (*params)["keyAlias"] = keyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_047
 * @tc.desc: The generate-csr module outFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_047, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA256withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["keyAlias"] = keyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_048
 * @tc.desc: The sign-profile module outFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_048, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA384withECDSA";
    std::string outFile = "./generateKeyPair/abc/signed-profile.txt";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["signAlg"] = signAlg;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hapSign_tool_cmd_test_049
 * @tc.desc: The sign-profile module outFile parameter checks whether it is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolCmdTest, hapSign_tool_cmd_test_049, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA384withECDSA";
    std::string outFile = "./generateKeyPair/signed-profile.txt";
    std::string keyAlias = "abc";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string profileCertFile = "./generateKeyPair/OpenHarmony.p12";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["signAlg"] = signAlg;
    (*params)["outFile"] = outFile;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["profileCertFile"] = profileCertFile;

    bool ret = ParamsRunTool::RunSignProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}
} // namespace SignatureTools
} // namespace OHOS