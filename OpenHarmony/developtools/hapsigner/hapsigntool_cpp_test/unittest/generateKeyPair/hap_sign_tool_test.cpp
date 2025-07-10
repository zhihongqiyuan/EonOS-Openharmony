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

class HapSignToolTest : public testing::Test {
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
 * @tc.name: hap_sign_tool_test_001
 * @tc.desc: Generates a key pair input check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_001, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string keyAlg = "ECC";
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
 * @tc.name: hap_sign_tool_test_002
 * @tc.desc: Generate a csr entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_002, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA256withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/oh-app1-key-v1.csr";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunCsr(params.get(), *api);
    EXPECT_EQ(ret, true);
}

/*
* @tc.name: hap_sign_tool_test_003
* @tc.desc: Generate the root certificate entry check.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolTest, hap_sign_tool_test_003, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/root-ca1.cer";
    std::string keyAlg = "ECC";
    int keySize = 384;

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["validity"] = validity;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, true);
}

/*
* @tc.name: hap_sign_tool_test_004
* @tc.desc: Generate an app debug certificate for entry checks.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolTest, hap_sign_tool_test_004, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
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

    bool ret = ParamsRunTool::RunProfileCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_005
 * @tc.desc: Generate profile debugging certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_005, testing::ext::TestSize.Level1)
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
 * @tc.name: hap_sign_tool_test_006
 * @tc.desc: Generate a universal certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_006, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/general.cer";

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

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_007
 * @tc.desc: Generate profile signature entry checks.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_007, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
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
 * @tc.name: hap_sign_tool_test_008
 * @tc.desc: Generate a profile check-in check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_008, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/signed-profile.p7b";
    std::string outFile = "./generateKeyPair/VerifyResult.json";

    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunVerifyProfile(params.get(), *api);
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_tool_test_009
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_009, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_010
 * @tc.desc: verify-app module inFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_010, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outCertChain = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outProfile = "./generateKeyPair/app-profile.p7b";

    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;

    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hap_sign_tool_test_011
* @tc.desc: Invoke the generate key pair interface.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolTest, hap_sign_tool_test_011, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "generate-keypair";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-app1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-keyAlg";
    char arg7[] = "ECC";
    char arg8[] = "-keySize";
    char arg9[] = "NIST-P-384";
    char arg10[] = "-keystoreFile";
    char arg11[] = "./generateKeyPair/OpenHarmony.p12";
    char arg12[] = "-keystorePwd";
    char arg13[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13 };
    int argc = 14;

    ParamsSharedPtr param = std::make_shared<Params>();
    std::shared_ptr<SignToolServiceImpl> service_api = std::make_shared<SignToolServiceImpl>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);
    bool ret = ParamsRunTool::DispatchParams(param, *service_api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_012
 * @tc.desc: Invoke to generate hap signature interface.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_012, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "sign-app";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-app1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-mode";
    char arg7[] = "localSign";
    char arg8[] = "-signCode";
    char arg9[] = "1";
    char arg10[] = "-signAlg";
    char arg11[] = "SHA384withECDSA";
    char arg12[] = "-appCertFile";
    char arg13[] = "./generateKeyPair/app-release1.pem";
    char arg14[] = "-profileFile";
    char arg15[] = "./generateKeyPair/signed-profile.p7b";
    char arg16[] = "-inFile";
    char arg17[] = "OpenHarmonyDamage.p12";
    char arg18[] = "-keystoreFile";
    char arg19[] = "./generateKeyPair/OpenHarmony.p12";
    char arg20[] = "-keystorePwd";
    char arg21[] = "123456";
    char arg22[] = "-outFile";
    char arg23[] = "./generateKeyPair/OpenHarmonyDamage.p12";
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
 * @tc.name: hap_sign_tool_test_013
 * @tc.desc: Invoke the generate profile signature interface.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_013, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "sign-profile";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-mode";
    char arg7[] = "localSign";
    char arg8[] = "-signAlg";
    char arg9[] = "SHA384withECDSA";
    char arg10[] = "-inFile";
    char arg11[] = "./generateKeyPair/profile.json";
    char arg12[] = "-keystoreFile";
    char arg13[] = "./generateKeyPair/OpenHarmony.p12";
    char arg14[] = "-keystorePwd";
    char arg15[] = "123456";
    char arg16[] = "-outFile";
    char arg17[] = "./generateKeyPair/signed-profile.p7b";
    char arg18[] = "-profileCertFile";
    char arg19[] = "./generateKeyPair/signed-profile.p7b";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19 };
    int argc = 20;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_014
 * @tc.desc: Invoke to generate hap check interface.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_014, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "verify-app";
    char arg2[] = "-inFile";
    char arg3[] = "./generateKeyPair/OpenHarmonyDamage.p12";
    char arg4[] = "-outCertChain";
    char arg5[] = "./generateKeyPair/app-sign-srv-ca1.cer";
    char arg6[] = "-outProfile";
    char arg7[] = "./generateKeyPair/app-profile.p7b";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7 };
    int argc = 8;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_015
 * @tc.desc: Invoke the generate profile check interface.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_015, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/signed-profile.p7b";
    std::string outFile = "./generateKeyPair/VerifyResult.json";

    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;

    char arg0[] = "";
    char arg1[] = "verify-profile";
    char arg2[] = "-inFile";
    char arg3[] = "./generateKeyPair/signed-profile.p7b";
    char arg4[] = "-outFile";
    char arg5[] = "./generateKeyPair/VerifyResult.json";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5 };
    int argc = 6;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_tool_test_016
 * @tc.desc: Invoke the Generate root certificate interface.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_016, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-ca";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-root-ca-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-subject";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-validity";
    char arg9[] = "365";
    char arg10[] = "-signAlg";
    char arg11[] = "SHA384withECDSA";
    char arg12[] = "-keystoreFile";
    char arg13[] = "./generateKeyPair/OpenHarmony.p12";
    char arg14[] = "-keystorePwd";
    char arg15[] = "123456";
    char arg16[] = "-outFile";
    char arg17[] = "./generateKeyPair/root-ca1.cer";
    char arg18[] = "-keyAlg";
    char arg19[] = "ECC";
    char arg20[] = "-keySize";
    char arg21[] = "NIST-P-384";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21 };
    int argc = 20;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_017
 * @tc.desc: Invoke the generate app certificate interface.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_017, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-app-cert";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-app1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-app-sign-srv-ca-key-v1";
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
    char arg21[] = "./generateKeyPair/app-release1.pem";
    char arg22[] = "-subCaCertFile";
    char arg23[] = "./generateKeyPair/app-sign-srv-ca1.cer";
    char arg24[] = "-outForm";
    char arg25[] = "certChain";
    char arg26[] = "-rootCaCertFile";
    char arg27[] = "./generateKeyPair/root-ca1.cer";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21,
                     arg22, arg23, arg24, arg25, arg26, arg27 };
    int argc = 28;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_018
 * @tc.desc: Invoke the Generate profile certificate interface.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_018, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-profile-cert";
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
    char arg21[] = "./generateKeyPair/profile-release1.pem";
    char arg22[] = "-subCaCertFile";
    char arg23[] = "./generateKeyPair/profile-sign-srv-ca1.cer";
    char arg24[] = "-outForm";
    char arg25[] = "certChain";
    char arg26[] = "-rootCaCertFile";
    char arg27[] = "./generateKeyPair/root-ca1.cer";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                     arg21, arg22, arg23, arg24, arg25, arg26, arg27 };
    int argc = 28;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_019
 * @tc.desc: Invoke the Generate generic certificate interface.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_019, testing::ext::TestSize.Level1)
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
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21 };
    int argc = 22;

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_020
 * @tc.desc: Command error, does not invoke any check interface.
 * @tc.type: FUNC
 * @tc.require:(generate-parameter)
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_020, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    char arg0[] = "";
    char arg1[] = "generate-parameter";
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
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21 };
    int argc = 22;
    ParamsRunTool::PrintHelp();

    ParamsSharedPtr param = std::make_shared<Params>();
    CmdUtil cmdUtil;

    cmdUtil.Convert2Params(argv, argc, param);

    bool ret = ParamsRunTool::DispatchParams(param, *api.get());
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hap_sign_tool_test_023
* @tc.desc: Generate the root certificate entry check.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolTest, hap_sign_tool_test_023, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    (*params)["keyAlias"] = keyAlias;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_024
 * @tc.desc: Generate the root certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_024, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/root-ca1.cer";
    std::string keyAlg = "RSA";
    int keySize = 384;

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["validity"] = validity;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_025
 * @tc.desc: Generate the root certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_025, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    int validity = 365;
    std::string signAlg = "SHA385withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/root-ca1.cer";
    std::string keyAlg = "ECC";
    int keySize = 999;

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["validity"] = validity;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_026
 * @tc.desc: Generate the root certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_026, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    int validity = 365;
    std::string signAlg = "SHA385withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/root-ca1.cer";
    std::string keyAlg = "ECC";
    int keySize = 999;

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["validity"] = validity;
    (*params)["issuer"] = issuer;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_027
 * @tc.desc: Generate a universal certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_027, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string issuerKeyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN= Openharmony Application CA";
    std::string issuer =
        "C=CNA,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    std::string signAlg = "SHA384withECDSA";
    std::string keyStoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string keyUsage = "digitalSignature";
    std::string outFile = "./generateKeyPair/general.cer";
    std::string basicConstraints = "true";
    std::string basicConstraintsCritical = "true";
    std::string basicConstraintsCa = "true";
    bool keyUsageCritical = true;
    char secret[] = "123456";
    int keysize = 384;
    (*params)["keyAlias"] = keyAlias;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["keysize"] = keysize;
    (*params)["subject"] = subject;
    (*params)["issuer"] = issuer;
    (*params)["signAlg"] = signAlg;
    (*params)["keyStoreFile"] = keyStoreFile;
    (*params)["keyUsage"] = keyUsage;
    (*params)["basicConstraints"] = basicConstraints;
    (*params)["basicConstraintsCritical"] = basicConstraintsCritical;
    (*params)["basicConstraintsCa"] = basicConstraintsCa;
    (*params)["keyUsageCritical"] = keyUsageCritical;
    (*params)["keyPwd"] = secret;
    (*params)["keystorePwd"] = secret;

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_028
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_028, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_029
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_029, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_030
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_030, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.txt";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_031
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_031, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA385withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_032
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_032, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_033
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_033, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.txt";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_034
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_034, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.txt";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string profileSigned = "0";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["profileSigned"] = profileSigned;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hap_sign_tool_test_035
* @tc.desc: Generate the root certificate entry check.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolTest, hap_sign_tool_test_035, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    int validity = 365;
    std::string signAlg = "SHA385withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/root-ca1.cer";
    std::string keyAlg = "ECC";
    int keySize = 384;

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["validity"] = validity;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
* @tc.name: hap_sign_tool_test_036
* @tc.desc: Generate the root certificate entry check.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(HapSignToolTest, hap_sign_tool_test_036, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/root-ca1.cer";
    std::string keyAlg = "ECC";
    int keySize = 384;

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["validity"] = validity;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_037
 * @tc.desc: Generate the root certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_037, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/root-ca1.cer";
    std::string keyAlg = "ECC";
    int keySize = 384;

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["keyAlg"] = keyAlg;
    (*params)["keySize"] = keySize;
    (*params)["validity"] = validity;
    (*params)["issuer"] = issuer;

    bool ret = ParamsRunTool::RunCa(params.get(), *api);
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: hap_sign_tool_test_038
 * @tc.desc: Generate a universal certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_038, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/general.cer";
    std::string keyUsage = "digitalSignature";

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
    (*params)["keyUsage"] = keyUsage;

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_039
 * @tc.desc: Generate a universal certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_039, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/general.cer";
    std::string keyUsage = "abcd";

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
    (*params)["keyUsage"] = keyUsage;

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_040
 * @tc.desc: Generate a universal certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_040, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/general.cer";
    std::string keyUsage = "digitalSignature";
    std::string extKeyUsage = "abcd";

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
    (*params)["keyUsage"] = keyUsage;
    (*params)["extKeyUsage"] = extKeyUsage;

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_041
 * @tc.desc: Generate a universal certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_041, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA385withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/general.cer";
    std::string keyUsage = "digitalSignature";

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
    (*params)["keyUsage"] = keyUsage;

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_042
 * @tc.desc: Generate a universal certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_042, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/general.cer";
    std::string keyUsage = "digitalSignature";

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
    (*params)["keyUsage"] = keyUsage;

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_043
 * @tc.desc: Generate app debugging certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_043, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

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
 * @tc.name: hap_sign_tool_test_044
 * @tc.desc: Generate app debugging certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_044, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    int validity = 365;
    std::string signAlg = "SHA385withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
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
 * @tc.name: hap_sign_tool_test_045
 * @tc.desc: Generate app debugging certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_045, testing::ext::TestSize.Level1)
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
    (*params)["rootCaCertFile"] = rootCaCertFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_046
 * @tc.desc: Generate app debugging certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_046, testing::ext::TestSize.Level1)
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
    std::string outForm = "123456";
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
 * @tc.name: hap_sign_tool_test_047
 * @tc.desc: Generate app debugging certificate entry check.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_047, testing::ext::TestSize.Level1)
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

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_048
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_048, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string profileSigned = "1";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["profileSigned"] = profileSigned;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_049
 * @tc.desc: The hap signature entry check is generated.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_049, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384wA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string profileSigned = "1";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;
    (*params)["profileSigned"] = profileSigned;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: hap_sign_tool_test_050
 * @tc.desc: verify-app module outProfile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(HapSignToolTest, hap_sign_tool_test_050, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outCertChain = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outProfile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

} // namespace SignatureTools
} // namespace OHOS