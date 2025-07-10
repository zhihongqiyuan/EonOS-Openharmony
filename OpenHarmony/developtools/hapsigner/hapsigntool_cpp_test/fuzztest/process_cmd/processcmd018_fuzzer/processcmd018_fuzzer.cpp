/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <cstddef>
#include <cstdint>
#include <unistd.h>
#include <cstdio>
#include "params_run_tool.h"
#include "code_signing.h"

namespace OHOS {
namespace SignatureTools {
bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }
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
    char arg11[] = "SHA256withECDSA";
    char arg12[] = "-appCertFile";
    char arg13[] = "./generateKeyPair/app-release1.pem";
    char arg14[] = "-profileFile";
    char arg15[] = "./generateKeyPair/signed-profile.p7b";
    char arg16[] = "-inFile";
    char arg17[] = "./generateKeyPair/entry-default-unsigned-so.hap";
    char arg18[] = "-keystoreFile";
    char arg19[] = "./generateKeyPair/OpenHarmony.p12";
    char arg20[] = "-keystorePwd";
    char arg21[] = "123456";
    char arg22[] = "-outFile";
    char arg23[] = "./generateKeyPair/entry-default-signed-so.hap";
    char* argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                    arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23};
    int argc = 24;
    CodeSigning obj;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);
    return ret;
}

bool RemoteSignTest(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    std::ofstream dummySO("./generateKeyPair/dummySO.txt", std::ios::binary);
    dummySO << ' ';
    dummySO.close();

    char arg0[] = "";
    char arg1[] = "sign-app";
    char arg2[] = "-mode";
    char arg3[] = "remoteSign";
    char arg4[] = "-keyAlias";
    char arg5[] = "oh-app1-key-v1";
    char arg6[] = "-profileFile";
    char arg7[] = "./generateKeyPair/signed-profile.p7b";
    char arg8[] = "-inFile";
    char arg9[] = "./generateKeyPair/entry-default-unsigned-so.hap";
    char arg10[] = "-signAlg";
    char arg11[] = "SHA256withECDSA";
    char arg12[] = "-outFile";
    char arg13[] = "./generateKeyPair/entry-default-remote-signed-so.hap";
    char arg14[] = "-signServer";
    char arg15[] = "./generateKeyPair/app-release1.pem";
    char arg16[] = "-signerPlugin";
    char arg17[] = "./generateKeyPair/dummySO.txt";
    char arg18[] = "-onlineAuthMode";
    char arg19[] = "./generateKeyPair/OpenHarmony.p12";
    char arg20[] = "-username";
    char arg21[] = "123456";
    char arg22[] = "-userPwd";
    char arg23[] = "123456";
    char* argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                    arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23};
    int argc = 24;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);
    return ret;
}

bool SignElf(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }
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
    char arg11[] = "SHA256withECDSA";
    char arg12[] = "-appCertFile";
    char arg13[] = "./generateKeyPair/app-release1.pem";
    char arg14[] = "-profileFile";
    char arg15[] = "./generateKeyPair/signed-profile.p7b";
    char arg16[] = "-inFile";
    char arg17[] = "./generateKeyPair/entry-default-unsigned-so.hap";
    char arg18[] = "-keystoreFile";
    char arg19[] = "./generateKeyPair/OpenHarmony.p12";
    char arg20[] = "-keystorePwd";
    char arg21[] = "123456";
    char arg22[] = "-outFile";
    char arg23[] = "./generateKeyPair/entry-default-signed-so.elf";
    char arg24[] = "-inForm";
    char arg25[] = "elf";
    char* argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                    arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25};
    int argc = 26;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);
    return ret;
}

bool SignBin(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }
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
    char arg11[] = "SHA256withECDSA";
    char arg12[] = "-appCertFile";
    char arg13[] = "./generateKeyPair/app-release1.pem";
    char arg14[] = "-profileFile";
    char arg15[] = "./generateKeyPair/signed-profile.p7b";
    char arg16[] = "-inFile";
    char arg17[] = "./generateKeyPair/entry-default-unsigned-so.hap";
    char arg18[] = "-keystoreFile";
    char arg19[] = "./generateKeyPair/OpenHarmony.p12";
    char arg20[] = "-keystorePwd";
    char arg21[] = "123456";
    char arg22[] = "-outFile";
    char arg23[] = "./generateKeyPair/entry-default-signed-so.bin";
    char arg24[] = "-inForm";
    char arg25[] = "bin";
    char* argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                    arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23, arg24, arg25};
    int argc = 26;
    bool ret = ParamsRunTool::ProcessCmd(argv, argc);
    return ret;
}
} // namespace SignatureTools
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    (void)rename("./generateKeyPair/entry-default-unsigned-so.txt", "./generateKeyPair/entry-default-unsigned-so.hap");
    sync();
    /* Run your code on data */
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    OHOS::SignatureTools::RemoteSignTest(data, size);
    OHOS::SignatureTools::SignElf(data, size);
    OHOS::SignatureTools::SignBin(data, size);
    return 0;
}