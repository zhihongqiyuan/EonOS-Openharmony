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

class ProcessCmdTest : public testing::Test {
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
 * @tc.name: processcmd_test_001
 * @tc.desc: The ProcessCmd function is executed to generate the key pair.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_001, testing::ext::TestSize.Level1)
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
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_002
 * @tc.desc: Test typing empty characters and printing the help document.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_002, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "";
    char* argv[] = { arg0, arg1 };

    int argc = 2;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: processcmd_test_003
 * @tc.desc: View the help documentation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_003, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "-h";
    char* argv[] = { arg0, arg1 };

    int argc = 2;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: processcmd_test_004
 * @tc.desc: Print version number.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_004, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "-v";
    char* argv[] = { arg0, arg1 };

    int argc = 2;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: processcmd_test_005
 * @tc.desc: Check that the value command-line argument is empty.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_005, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "generate-keypair";
    char arg2[] = "-keyAlias";
    char arg3[] = "";
    char* argv[] = { arg0, arg1, arg2, arg3 };

    int argc = 4;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_006
 * @tc.desc: Checks whether the first argument of key has -.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_006, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "generate-keypair";
    char arg2[] = "";
    char arg3[] = "";
    char* argv[] = { arg0, arg1, arg2, arg3 };

    int argc = 4;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_007
 * @tc.desc: Check whether the key algorithm is NIST-P-384 or NIST-P-256.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_007, testing::ext::TestSize.Level1)
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
    char arg9[] = "NIST-P-385";
    char arg10[] = "-keystoreFile";
    char arg11[] = "./generateKeyPair/OpenHarmony.p12";
    char arg12[] = "-keystorePwd";
    char arg13[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6,
                     arg7, arg8, arg9, arg10, arg11, arg12, arg13 };

    int argc = 14;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_008
 * @tc.desc: Checks whether the argument is in the trusted list.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_008, testing::ext::TestSize.Level1)
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
    char arg9[] = "NIST-P-257";
    char arg10[] = "-keystoreFile";
    char arg11[] = "./generateKeyPair/OpenHarmony.p12";
    char arg12[] = "-keystorePwd";
    char arg13[] = "123456";
    char arg14[] = "-extKeyUsageCritical";
    char arg15[] = "false";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                     arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15 };

    int argc = 16;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_009
* @tc.desc: Checks whether the last value of the command argument is null.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_009, testing::ext::TestSize.Level1)
{
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
    char arg22[] = "-keyPwd";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22 };
    int argc = 23;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_010
* @tc.desc: Check that the required parameters of the generate-cert module are met.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_010, testing::ext::TestSize.Level1)
{
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
    char arg17[] = "/mnt/d/file/0613test/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "/mnt/d/file/0613test/general.cer";
    char arg22[] = "-basicConstraintsPathLen";
    char arg23[] = "0";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_011
* @tc.desc: Check that the required parameters of the sign-app module are met.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_011, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "sign-app";
    char arg2[] = "-profileSigned";
    char arg3[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3 };
    int argc = 4;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_012
* @tc.desc: Check if the -profileSigned parameter in sign-app is [1,0, true, false, TRUE, FALSE].
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_012, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "sign-app";
    char arg2[] = "-profileSigned";
    char arg3[] = "abcd";
    char* argv[] = { arg0, arg1, arg2, arg3 };
    int argc = 4;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_013
* @tc.desc: Check that the output file path exists.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_013, testing::ext::TestSize.Level1)
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
    char arg9[] = "NIST-P-256";
    char arg10[] = "-keystoreFile";
    char arg11[] = "./aabc123/OpenHarmony.p12";
    char arg12[] = "-keystorePwd";
    char arg13[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13 };
    int argc = 14;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_014
* @tc.desc: Check if the keystore file is in 'p12' or 'JKS' file format.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_014, testing::ext::TestSize.Level1)
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
    char arg9[] = "NIST-P-256";
    char arg10[] = "-keystoreFile";
    char arg11[] = "./generateKeyPair";
    char arg12[] = "-keystorePwd";
    char arg13[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13 };
    int argc = 14;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_015
* @tc.desc: Check if the signature algorithm is SHA3256withECDSA or SHA384withECDSA.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_015, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "sign-app";
    char arg2[] = "-signAlg";
    char arg3[] = "abcd";
    char* argv[] = { arg0, arg1, arg2, arg3 };
    int argc = 4;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_016
* @tc.desc: Check if the signature algorithm is SHA3256withECDSA or SHA384withECDSA.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_016, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "sign-app";
    char arg2[] = "-signAlg";
    char arg3[] = "SHA384withECDSA";
    char* argv[] = { arg0, arg1, arg2, arg3 };
    int argc = 4;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_017
* @tc.desc: Check that the -inForm value is formatted correct.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_017, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "verify-app";
    char arg2[] = "-inForm";
    char arg3[] = "abcd";
    char* argv[] = { arg0, arg1, arg2, arg3 };
    int argc = 4;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_018
* @tc.desc: Check that the -outForm argument is correctly formatted.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_018, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "generate-app-cert";
    char arg2[] = "-outForm";
    char arg3[] = "abcd";
    char* argv[] = { arg0, arg1, arg2, arg3 };
    int argc = 4;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
* @tc.name: processcmd_test_019
* @tc.desc: Check the generate-app-cert module for required parameters.
* @tc.type: FUNC
* @tc.require:
*/
HWTEST_F(ProcessCmdTest, processcmd_test_019, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "generate-app-cert";
    char* argv[] = { arg0, arg1 };
    int argc = 2;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_020
 * @tc.desc: Printing help documents.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_020, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "-help";
    char* argv[] = { arg0, arg1 };
    int argc = 2;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: processcmd_test_021
 * @tc.desc: Print version number.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_021, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "-version";
    char* argv[] = { arg0, arg1 };
    int argc = 2;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: processcmd_test_022
 * @tc.desc: Print help documentation when an empty command is passed.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_022, testing::ext::TestSize.Level1)
{
    bool ret = ParamsRunTool::ProcessCmd(nullptr, 5);
    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: processcmd_test_023
 * @tc.desc: Check whether keyUsageCritical parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_023, testing::ext::TestSize.Level1)
{
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_024
 * @tc.desc: Check whether keyUsageCritical parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_024, testing::ext::TestSize.Level1)
{
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
    char arg23[] = "true";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_025
 * @tc.desc: Check whether extKeyUsageCritical parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_025, testing::ext::TestSize.Level1)
{
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
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_026
 * @tc.desc: Check whether extKeyUsageCritical parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_026, testing::ext::TestSize.Level1)
{
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
    char arg23[] = "true";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_027
 * @tc.desc: Check whether extKeyUsageCritical parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_027, testing::ext::TestSize.Level1)
{
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_028
 * @tc.desc: Check whether basicConstraints parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_028, testing::ext::TestSize.Level1)
{
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_029
 * @tc.desc: Check whether basicConstraints parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_029, testing::ext::TestSize.Level1)
{
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
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_030
 * @tc.desc: Check whether basicConstraints parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_030, testing::ext::TestSize.Level1)
{
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
    char arg23[] = "true";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_031
 * @tc.desc: Check whether basicConstraintsCritical parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_031, testing::ext::TestSize.Level1)
{
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
    char arg23[] = "true";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_032
 * @tc.desc: Check whether basicConstraintsCritical parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_032, testing::ext::TestSize.Level1)
{
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_033
 * @tc.desc: Check whether basicConstraintsCritical parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_033, testing::ext::TestSize.Level1)
{
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
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_034
 * @tc.desc: Check whether basicConstraintsCa parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_034, testing::ext::TestSize.Level1)
{
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
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_035
 * @tc.desc: Check that the inFile argument is correct.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_035, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string inFile = "./generateKeyPair";
    (*params)["inFile"] = inFile;
    bool ret = CmdUtil::UpdateParamForCheckInFile(params.get(), {Options::IN_FILE});

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_test_036
 * @tc.desc: Check that the inFile argument is correct.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_036, testing::ext::TestSize.Level1)
{
    std::shared_ptr<Options> params = std::make_shared<Options>();
    std::string inFile = "generateKeyPair";
    (*params)["inFile"] = inFile;
    bool ret = CmdUtil::UpdateParamForCheckOutFile(params.get(), {Options::IN_FILE});

    EXPECT_EQ(ret, true);
}

/*
 * @tc.name: processcmd_test_037
 * @tc.desc: Check that the profileSigned argument is correct.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdTest, processcmd_test_037, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "sign-app";
    char arg2[] = "-profileSigned";
    char arg3[] = "0";
    char* argv[] = {arg0, arg1, arg2, arg3};
    int argc = 4;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}
} // namespace SignatureTools
} // namespace OHOS