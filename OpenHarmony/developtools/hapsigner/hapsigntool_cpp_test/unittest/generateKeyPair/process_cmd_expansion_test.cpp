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

class ProcessCmdExpansionTest : public testing::Test {
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
 * @tc.name: processcmd_expansion_test_001
 * @tc.desc: Check whether basicConstraintsCa parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_001, testing::ext::TestSize.Level1)
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
    char arg23[] = "true";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_002
 * @tc.desc: Check whether basicConstraintsCa parameter for [ 1, 0, true, false, true, false ].
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_002, testing::ext::TestSize.Level1)
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
    char arg23[] = "TRUE";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_003
 * @tc.desc: Check that the -inFile argument is a valid path.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_003, testing::ext::TestSize.Level1)
{
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
    char arg11[] = "./abcd/profile11.json";
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_004
 * @tc.desc: The app-cert certificate is generated by the SHA256withECDSA signature algorithm.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_004, testing::ext::TestSize.Level1)
{
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
    char arg15[] = "SHA256withECDSA";
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_005
 * @tc.desc: The app-cert certificate is generated by the SHA384withECDSA signature algorithm.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_005, testing::ext::TestSize.Level1)
{
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_006
 * @tc.desc: Check if the signature algorithm is SHA384withECDSA or SHA256withECDSA.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_006, testing::ext::TestSize.Level1)
{
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
    char arg15[] = "abc";
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_007
 * @tc.desc: The verify-app module checks whether the -inForm parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_007, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "verify-app";
    char arg2[] = "-inFile";
    char arg3[] = "./generateKeyPair/OpenHarmonyDamage.p12";
    char arg4[] = "-outCertChain";
    char arg5[] = "./generateKeyPair/app-sign-srv-ca1.cer";
    char arg6[] = "-outProfile";
    char arg7[] = "./generateKeyPair/app-profile.p7b";
    char arg8[] = "-inForm";
    char arg9[] = "abc";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9 };
    int argc = 10;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_008
 * @tc.desc: The generate-app-cert module checks whether the -outForm parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_008, testing::ext::TestSize.Level1)
{
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
    char arg25[] = "abcd";
    char arg26[] = "-rootCaCertFile";
    char arg27[] = "./generateKeyPair/root-ca1.cer";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21,
                     arg22, arg23, arg24, arg25, arg26, arg27 };
    int argc = 28;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_009
 * @tc.desc: The generate-profile-cert module checks whether the -outForm parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_009, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "generate-profile-cert";
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
    char arg25[] = "abcd";
    char arg26[] = "-rootCaCertFile";
    char arg27[] = "./generateKeyPair/root-ca1.cer";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21,
                     arg22, arg23, arg24, arg25, arg26, arg27 };
    int argc = 28;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_010
 * @tc.desc: The generate-profile-cert module checks whether the -outForm parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_010, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "generate-profile-cert";
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
    char arg25[] = "cert";
    char arg26[] = "-rootCaCertFile";
    char arg27[] = "./generateKeyPair/root-ca1.cer";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21,
                     arg22, arg23, arg24, arg25, arg26, arg27 };
    int argc = 28;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_011
 * @tc.desc: The generate-profile-cert module checks whether the -outForm parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_011, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "generate-profile-cert";
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_012
 * @tc.desc: The generate-profile-cert module checks whether the -outForm parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_012, testing::ext::TestSize.Level1)
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
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_013
 * @tc.desc: Required parameter test of generate-cert module.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_013, testing::ext::TestSize.Level1)
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
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-keyUsageCritical";
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 22;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_014
 * @tc.desc: The signature algorithm SHA256withECDSA is used to generate the generic certificate.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_014, testing::ext::TestSize.Level1)
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
    char arg15[] = "SHA256withECDSA";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-keyUsageCritical";
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_015
 * @tc.desc: The signature algorithm SHA384withECDSA is used to generate the generic certificate.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_015, testing::ext::TestSize.Level1)
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
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_016
 * @tc.desc: Check that the generate-cert module -signAlg parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_016, testing::ext::TestSize.Level1)
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
    char arg15[] = "abcd";
    char arg16[] = "-keystoreFile";
    char arg17[] = "./generateKeyPair/OpenHarmony.p12";
    char arg18[] = "-keystorePwd";
    char arg19[] = "123456";
    char arg20[] = "-outFile";
    char arg21[] = "./generateKeyPair/general.cer";
    char arg22[] = "-keyUsageCritical";
    char arg23[] = "1";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23 };
    int argc = 24;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_017
 * @tc.desc: The sign-app module verifies that the -signAlg argument is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_017, testing::ext::TestSize.Level1)
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
 * @tc.name: processcmd_expansion_test_018
 * @tc.desc: The verify-app module verifies that the -signAlg argument is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_018, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "verify-app";
    char arg2[] = "-signAlg";
    char arg3[] = "SHA384withECDSA";
    char* argv[] = { arg0, arg1, arg2, arg3 };
    int argc = 4;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_019
 * @tc.desc: Remote signature requires parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_019, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "sign-profile";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-mode";
    char arg7[] = "remoteSign";
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

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_020
 * @tc.desc: sign-profile module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_020, testing::ext::TestSize.Level1)
{
    char arg0[] = "";
    char arg1[] = "sign-profile";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-profile1-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
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
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19 };
    int argc = 18;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_021
 * @tc.desc: sign-profile module parameter validation.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_021, testing::ext::TestSize.Level1)
{
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
    char arg15[] = "123k456";
    char arg16[] = "-outFile";
    char arg17[] = "./generateKeyPair/signed-profile.p7b";
    char arg18[] = "-profileCertFile";
    char arg19[] = "./generateKeyPair/signed-profile.p7b";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19 };
    int argc = 20;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);

    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_022
 * @tc.desc: sign-profile module signature algorithm verification.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_022, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA385withECDSA";
    std::string outFile = "./generateKeyPair/signed-profile.txt";
    std::string keyAlias = "abc";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
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
 * @tc.name: processcmd_expansion_test_023
 * @tc.desc: The sign-profile module verifies that the output directory is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_023, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string mode = "localSign";
    std::string inFile = "./generateKeyPair/profile.json";
    std::string signAlg = "SHA384withECDSA";
    std::string outFile = "./generateKeyPair/signed-profile.txt";
    std::string keyAlias = "abc";
    std::string keystoreFile = "./generateKeyPair/OpenHarmony.p12";
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
 * @tc.name: processcmd_expansion_test_024
 * @tc.desc: The verify-profile module verifies that the output directory is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_024, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/signed-profile.p7b";
    std::string outFile = "./generateKeyPair/abc/VerifyResult.json";

    (*params)["inFile"] = inFile;
    (*params)["outFile"] = outFile;

    bool ret = ParamsRunTool::RunVerifyProfile(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_025
 * @tc.desc: The verify-app module checks if the inForm parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_025, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outCertChain = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outProfile = "./generateKeyPair/signed-profile.p7b";
    std::string inForm = "zip";

    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    (*params)["inForm"] = inForm;
    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_026
 * @tc.desc: The verify-app module checks if the outProfile parameter is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_026, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outCertChain = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outProfile = "./generateKeyPair/abc/signed-profile.p7b";
    std::string inForm = "zip";

    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    (*params)["inForm"] = inForm;
    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_027
 * @tc.desc: The verify-app module checks that the inFile argument is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_027, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outCertChain = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outProfile = "./generateKeyPair/signed-profile.p7b";

    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_028
 * @tc.desc: The verify-app module checks that the outCertChain argument is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_028, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outCertChain = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outProfile = "./generateKeyPair/signed-profile.p7b";
    std::string inForm = "zip";

    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    (*params)["inForm"] = inForm;
    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}

/*
 * @tc.name: processcmd_expansion_test_029
 * @tc.desc: The verify-app module checks that the outProfile argument is valid.
 * @tc.type: FUNC
 * @tc.require:
 */
HWTEST_F(ProcessCmdExpansionTest, processcmd_expansion_test_029, testing::ext::TestSize.Level1)
{
    std::unique_ptr<SignToolServiceImpl> api = std::make_unique<SignToolServiceImpl>();
    std::shared_ptr<Options> params = std::make_shared<Options>();

    std::string inFile = "./generateKeyPair/OpenHarmonyDamage.p12";
    std::string outCertChain = "./generateKeyPair/app-sign-srv-ca1.cer";
    std::string outProfile = "./generateKeyPair/signed-profile.txt";
    std::string inForm = "zip";

    (*params)["inFile"] = inFile;
    (*params)["outCertChain"] = outCertChain;
    (*params)["outProfile"] = outProfile;
    (*params)["inForm"] = inForm;
    bool ret = ParamsRunTool::RunVerifyApp(params.get(), *api);
    EXPECT_EQ(ret, false);
}
} // namespace SignatureTools
} // namespace OHOS