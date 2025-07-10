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

class OptionsCmdTest : public testing::Test {
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
 * @tc.name: Options_test_001
 * @tc.desc: get char* type value, and do type checking.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    char keyPwd[] = "123456";
    (*params)["keyPwd"] = keyPwd;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    char* strPrt = adaptePtr->options->GetChars(Options::KEY_RIGHTS);
    EXPECT_NE(strPrt, nullptr);
}

/*
 * @tc.name: Options_test_002
 * @tc.desc: get string type value, and do type checking.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    (*params)["keyAlias"] = keyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    std::string strPrt = adaptePtr->options->GetString(Options::KEY_ALIAS);

    EXPECT_NE(strPrt, "");
}

/*
 * @tc.name: Options_test_003
 * @tc.desc: get two-parameter string type value, and do type checking.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    std::string str = "test";
    (*params)["keyAlias"] = keyAlias;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    std::string strPrt = adaptePtr->options->GetString(Options::KEY_ALIAS, str);

    if (strPrt == keyAlias) {
        EXPECT_EQ(strPrt, keyAlias);
    } else if (strPrt == str) {
        EXPECT_EQ(strPrt, str);
    } else {
        EXPECT_EQ(strPrt, keyAlias);
    }
}

/*
 * @tc.name: Options_test_004
 * @tc.desc: get Int type value, and do type checking.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_004, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    int keySize = 256;
    (*params)["keySize"] = keySize;
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    int size = adaptePtr->options->GetInt(Options::KEY_SIZE);

    EXPECT_NE(size, 0);
}

/*
 * @tc.name: Options_test_005
 * @tc.desc: Check for equality.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_005, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string issuerkeystoreFile = "./generateKeyPair/OpenHarmony.p12";

    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerkeystoreFile"] = issuerkeystoreFile;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EXPECT_EQ(adaptePtr->options->Equals(Options::KEY_STORE_FILE, Options::ISSUER_KEY_STORE_FILE), false);
}

/*
 * @tc.name: Options_test_006
 * @tc.desc: Check for presence.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_006, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string issuerkeystoreFile = "./generateKeyPair/OpenHarmony.p12";

    (*params)["keystoreFile"] = keystoreFile;
    (*params)["issuerkeystoreFile"] = issuerkeystoreFile;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EXPECT_EQ(adaptePtr->options->Required({ Options::KEY_STORE_FILE, Options::ISSUER_KEY_STORE_FILE }), false);
}
/*
 * @tc.name: Options_test_007
 * @tc.desc: Check whether it is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_007, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string str = "";
    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EXPECT_EQ(adaptePtr->options->IsEmpty(str), true);
}

/*
* @tc.name: Options_test_008
* @tc.desc: get string type value, and do type checking.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(OptionsCmdTest, Options_test_008, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    (*params)["keyAlias"];

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    std::string strPrt = adaptePtr->options->GetString(Options::KEY_ALIAS);
    EXPECT_EQ(strPrt, "");
}

/*
 * @tc.name: Options_test_009
 * @tc.desc: get string type value, and do type checking.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_009, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    (*params)["keyAlias"] = keyAlias;

    std::string strPrt = params->GetString(Options::KEY_STORE_FILE);
    EXPECT_EQ(strPrt, "");
}

/*
 * @tc.name: Options_test_010
 * @tc.desc: get string type value, and do type checking.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_010, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    std::string strPrt = params->GetString(Options::KEY_RIGHTS);
    EXPECT_EQ(strPrt, "");
}

/*
 * @tc.name: Options_test_011
 * @tc.desc: get two-parameter string type value, and do type checking.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_011, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string str = "abcd";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    std::string strPrt = params->GetString(Options::KEY_RIGHTS, str);
    EXPECT_EQ(strPrt, str);
}

/*
 * @tc.name: Options_test_012
 * @tc.desc: get two-parameter string type value, and do type checking.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_012, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "";
    char keyPwd[] = "123456";
    std::string str = "abcd";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    std::string strPrt = params->GetString(Options::KEY_ALIAS, str);
    EXPECT_EQ(strPrt, str);
}

/*
 * @tc.name: Options_test_013
 * @tc.desc: Check if required parameters exist.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_013, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string signAlg = "SHA256withECDSA";
    std::string outForm = "cert";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";

    (*params)["keyAlias"] = keyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["outForm"] = outForm;
    (*params)["keystoreFile"] = keystoreFile;
    bool ret = params->Required({ Options::KEY_ALIAS, Options::SIGN_ALG, Options::OUT_FORM, Options::KEY_STORE_FILE });
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: Options_test_014
 * @tc.desc: Check if required parameters exist.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_014, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    bool ret = params->Required({ "" });
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: Options_test_015
 * @tc.desc: Check if required parameters exist.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_015, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keyAlias = "oh-app1-key-v1";

    (*params)["inFile"] = keyAlias;
    bool ret = params->Required({ Options::KEY_ALIAS });
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_016
 * @tc.desc: Check whether the algorithm is in ECC format.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_016, testing::ext::TestSize.Level1)
{
    std::string keyAlg = "ECC";
    EXPECT_EQ(CmdUtil::JudgeAlgType(keyAlg), true);
}

/*
 * @tc.name: Options_test_017
 * @tc.desc: Check whether the algorithm length is 256 or 384.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_017, testing::ext::TestSize.Level1)
{
    int size = 256;
    EXPECT_EQ(CmdUtil::JudgeSize(size), true);
}

/*
 * @tc.name: Options_test_018
 * @tc.desc: Write command line arguments to map.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_018, testing::ext::TestSize.Level1)
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

    CmdUtil cmdUtil;
    ParamsSharedPtr param = std::make_shared<Params>();
    bool ret = cmdUtil.Convert2Params(argv, argc, param);

    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: Options_test_019
 * @tc.desc: Gets command line arguments.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_019, testing::ext::TestSize.Level1)
{
    char argv[][100] = { "generate-keypair",
                     "-keyAlias", "oh-app1-key-v1",
                     "-keyPwd", "123456",
                     "-keyAlg", "ECC",
                     "-keySize", "NIST-P-384",
                     "-keystoreFile", "./generateKeyPair/OpenHarmony.p12",
                     "-keystorePwd", "123456"
    };

    ParamsSharedPtr param = std::make_shared<Params>();
    param->SetMethod(argv[1]);
    ParamsTrustList params_trust_list;
    std::vector<std::string> trustList = params_trust_list.GetTrustList(argv[1]);
    EXPECT_EQ(trustList.empty(), true);
}

/*
 * @tc.name: Options_test_020
 * @tc.desc: Check whether the file format is p12 or jks.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_020, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    (*params)["keystoreFile"] = keystoreFile;

    std::unique_ptr<LocalizationAdapter> adaptePtr = std::make_unique<LocalizationAdapter>(params.get());
    EXPECT_EQ(FileUtils::ValidFileType(adaptePtr->options->GetString(Options::KEY_STORE_FILE),
              { "p12", "jks" }), true);
}

/*
* @tc.name: Options_test_021
* @tc.desc: Checks whether the type is legal.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(OptionsCmdTest, Options_test_021, testing::ext::TestSize.Level1)
{
    std::string str = "";
    bool ret = CmdUtil::VerifyTypes(str);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_022
 * @tc.desc: The sign-app module checks whether the inFile module is a valid path.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_022, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "abcd";
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
 * @tc.name: Options_test_023
 * @tc.desc: The sign-app module checks if inform is a valid parameter.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_023, testing::ext::TestSize.Level1)
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
    std::string inform = "abcd";

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
    (*params)["inForm"] = inform;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_024
 * @tc.desc: The sign-app module checks if signAlg is a valid parameter.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_024, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384w";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "";
    std::string inFile = "OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string inform = "elf";

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
    (*params)["inForm"] = inform;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_025
 * @tc.desc: The sign-app module executes the branch with profileSigned = "1".
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_025, testing::ext::TestSize.Level1)
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
 * @tc.name: Options_test_026
 * @tc.desc: The sign-app module executes the branch with profileSigned = "0".
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_026, testing::ext::TestSize.Level1)
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
 * @tc.name: Options_test_027
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_027, testing::ext::TestSize.Level1)
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
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_028
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_028, testing::ext::TestSize.Level1)
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

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_029
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_029, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
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
 * @tc.name: Options_test_030
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_030, testing::ext::TestSize.Level1)
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
 * @tc.name: Options_test_031
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_031, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteSign";
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
 * @tc.name: Options_test_032
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_032, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
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
* @tc.name: Options_test_033
* @tc.desc: Checks whether the type is legal.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(OptionsCmdTest, Options_test_033, testing::ext::TestSize.Level1)
{
    std::string str = "clientAuthentication";
    bool ret = CmdUtil::VerifyType(str);

    EXPECT_EQ(ret, true);
}

/*
* @tc.name: Options_test_034
* @tc.desc: Checks whether the type is legal.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(OptionsCmdTest, Options_test_034, testing::ext::TestSize.Level1)
{
    std::string supportTypes = "abc,cba";
    std::string inputtype = "abc";
    bool ret = CmdUtil::VerifyType(inputtype, supportTypes);

    EXPECT_EQ(ret, true);
}

/*
* @tc.name: Options_test_035
* @tc.desc: Checks whether the type is legal.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(OptionsCmdTest, Options_test_035, testing::ext::TestSize.Level1)
{
    std::string supportTypes = "abc,cba";
    std::string inputtype = "cba";
    bool ret = CmdUtil::VerifyType(inputtype, supportTypes);

    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: Options_test_036
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_036, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
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
 * @tc.name: Options_test_037
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_037, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inFile"] = inFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_038
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_038, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_039
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_039, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["mode"] = mode;
    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["signAlg"] = signAlg;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_040
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_040, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["signCode"] = signCode;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_041
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_041, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "";

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
 * @tc.name: Options_test_042
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_042, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/signed-profile.p7b";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string inForm = "abcd";
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
    (*params)["inForm"] = inForm;
    (*params)["profileSigned"] = profileSigned;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_043
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_043, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string signCode = "1";
    std::string signAlg = "SHA384withECDSA";
    std::string appCertFile = "./generateKeyPair/app-release1.pem";
    std::string profileFile = "./generateKeyPair/profile.json";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string inForm = "abcd";
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
    (*params)["inForm"] = inForm;
    (*params)["profileSigned"] = profileSigned;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_044
 * @tc.desc: generate-app-cert module parameter inspection.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_044, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    char keyPwd[] = "123456";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "abcd";
    std::string subject = "abcd";
    std::string signAlg = "SHA256withECDSA";
    std::string outForm = "cert";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    char keystorePwd[] = "123456";
    std::string outFile = "./generateKeyPair/app1.txt";

    (*params)["keyAlias"] = keyAlias;
    (*params)["keyPwd"] = keyPwd;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["signAlg"] = signAlg;
    (*params)["subject"] = subject;
    (*params)["outForm"] = outForm;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keystorePwd"] = keystorePwd;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::CheckEndCertArguments(*params);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_045
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_045, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteResign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/test A/abc";
    std::string signAlg = "SHA384withECDSA";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_046
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_046, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "remoteSign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA384withECDSA";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_047
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_047, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "abc";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA384withECDSA";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_048
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_048, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA384withECDSA";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_049
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_049, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string keyAlias = "oh-app1-key-v1";
    std::string appCertFile = "./generateKeyPair/app-profile.p7b";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyAlias"] = keyAlias;
    (*params)["appCertFile"] = appCertFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_050
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_050, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA385withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string keyAlias = "oh-app1-key-v1";
    std::string appCertFile = "./generateKeyPair/app-profile.p7b";
    std::string profileFile = "./generateKeyPair/profile.json";
    std::string inForm = "";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyAlias"] = keyAlias;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileFile"] = profileFile;
    (*params)["inForm"] = inForm;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_051
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_051, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string keyAlias = "oh-app1-key-v1";
    std::string appCertFile = "./generateKeyPair/app-profile.p7b";
    std::string inForm = "elf";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyAlias"] = keyAlias;
    (*params)["appCertFile"] = appCertFile;
    (*params)["inForm"] = inForm;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_052
 * @tc.desc: sign-app module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_052, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string keyAlias = "oh-app1-key-v1";
    std::string appCertFile = "./generateKeyPair/app-profile.p7b";
    std::string profileFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string profileSigned = "1";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyAlias"] = keyAlias;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["profileFile"] = profileFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_053
 * @tc.desc: sign-app module keystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_053, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/app-profile.p7b";
    std::string keyAlias = "oh-app1-key-v1";
    std::string appCertFile = "./generateKeyPair/app-profile.p7b";
    std::string profileFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string profileSigned = "1";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyAlias"] = keyAlias;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["profileFile"] = profileFile;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_054
 * @tc.desc: sign-app module inForm parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_054, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string keyAlias = "oh-app1-key-v1";
    std::string appCertFile = "./generateKeyPair/app-profile.p7b";
    std::string profileFile = "./generateKeyPair/app-profile.p7b";
    std::string profileSigned = "1";
    std::string inForm = "abc";

    (*params)["mode"] = mode;
    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["keyAlias"] = keyAlias;
    (*params)["appCertFile"] = appCertFile;
    (*params)["profileSigned"] = profileSigned;
    (*params)["profileFile"] = profileFile;
    (*params)["inForm"] = inForm;

    bool ret = ParamsRunTool::RunSignApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_055
 * @tc.desc: generate-ca module keystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_055, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-root-ca-key-v1";
    char keyPwd[] = "123456";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Root CA";
    std::string issuer = "C=CN,O=OpenHarmony_test,OU=OpenHarmony Community,CN= Openharmony Application SUB  CA";
    int validity = 365;
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/app-profile.p7b";
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
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_056
 * @tc.desc: generate-cert module keystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_056, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string keyUsage = "digitalSignature";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/abc/OpenHarmonyDamage.p12";
    std::string outFile = "./generateKeyPair/general.cer";
    

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyUsage"] = keyUsage;

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_057
 * @tc.desc: generate-cert module keystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_057, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-profile1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-profile-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string keyUsage = "digitalSignature";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/app-profile.p7b";
    std::string outFile = "./generateKeyPair/general.cer";
    

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["keyUsage"] = keyUsage;

    bool ret = ParamsRunTool::RunCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_058
 * @tc.desc: generate-app-cert module keystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_058, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/app-profile.p7b";
    std::string outFile = "./generateKeyPair/app-release1.pem";

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_059
 * @tc.desc: generate-app-cert module issuerKeystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_059, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string issuer = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=Application Signature Service CA";
    std::string issuerKeyAlias = "oh-app-sign-srv-ca-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA384withECDSA";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
    std::string outFile = "./generateKeyPair/app-release1.pem";
    std::string issuerKeystoreFile = "./generateKeyPair/app-profile.p7b";

    (*params)["keyAlias"] = keyAlias;
    (*params)["issuer"] = issuer;
    (*params)["issuerKeyAlias"] = issuerKeyAlias;
    (*params)["subject"] = subject;
    (*params)["signAlg"] = signAlg;
    (*params)["keystoreFile"] = keystoreFile;
    (*params)["outFile"] = outFile;
    (*params)["issuerKeystoreFile"] = issuerKeystoreFile;

    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_060
 * @tc.desc: generate-app-cert module keystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_060, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string keystoreFile = "./generateKeyPair/abc/OpenHarmony.p12";
    (*params)["keystoreFile"] = keystoreFile;
    bool ret = ParamsRunTool::RunAppCert(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_061
 * @tc.desc: generate-csr module keystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_061, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string keyAlias = "oh-app1-key-v1";
    std::string subject = "C=CN,O=OpenHarmony,OU=OpenHarmony Community,CN=App1 Release";
    std::string signAlg = "SHA256withECDSA";
    std::string keystoreFile = "./generateKeyPair/app-profile.p7b";
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
 * @tc.name: Options_test_062
 * @tc.desc: sign-profile module keystoreFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_062, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA384withECDSA";
    std::string outFile = "./generateKeyPair/signed-profile.txt";
    std::string keyAlias = "abc";
    std::string keystoreFile = "./generateKeyPair/app-profile.p7b";
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

/*
 * @tc.name: Options_test_063
 * @tc.desc: verify-profile module inFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_063, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmony.p12";
    std::string outFile = "./generateKeyPair/VerifyResult.json";

    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunVerifyProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_064
 * @tc.desc: verify-app module inFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_064, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    (*params)["inFile"] = inFile;
    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_065
 * @tc.desc: verify-profile module inFile parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_065, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/abc/OpenHarmonyDamage.p12";
    std::string outCertChain = "abc";
    std::string outProfile = "abc";
    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: Options_test_067
 * @tc.desc: Remove the white space.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_067, testing::ext::TestSize.Level1)
{
    std::string str = "  123456  ";
    std::string params = StringUtils::Trim(str);
    EXPECT_EQ(params, "123456");
}

/*
 * @tc.name: Options_test_068
 * @tc.desc: Gets the first command line argument.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_068, testing::ext::TestSize.Level1)
{
    char argv[][100] = { "generate-keypair",
                     "-keyAlias", "oh-app1-key-v1",
                     "-keyPwd", "123456",
                     "-keyAlg", "ECC",
                     "-keySize", "NIST-P-384",
                     "-keystoreFile", "./generateKeyPair/OpenHarmony.p12",
                     "-keystorePwd", "123456"
    };

    ParamsSharedPtr param = std::make_shared<Params>();
    param->SetMethod(argv[1]);
    EXPECT_EQ(param->GetMethod().empty(), false);
}

/*
 * @tc.name: Options_test_069
 * @tc.desc: Gets the first command line argument.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_069, testing::ext::TestSize.Level1)
{
    std::string signatureAlgorithm = ParamConstants::HAP_SIG_ALGORITHM_SHA384_ECDSA;
    SignatureAlgorithmHelper out;
    bool ret = Params::GetSignatureAlgorithm(signatureAlgorithm, out);
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: Options_test_070
 * @tc.desc: Gets the first command line argument.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(OptionsCmdTest, Options_test_070, testing::ext::TestSize.Level1)
{
    std::string signatureAlgorithm = "123456";
    SignatureAlgorithmHelper out;
    bool ret = Params::GetSignatureAlgorithm(signatureAlgorithm, out);
    EXPECT_EQ(ret, false);
}
} // namespace SignatureTools
} // namespace OHOS