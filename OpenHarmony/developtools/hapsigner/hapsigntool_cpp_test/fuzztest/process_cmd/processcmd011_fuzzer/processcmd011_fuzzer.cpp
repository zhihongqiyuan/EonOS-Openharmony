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
#include <cstdio>
#include "params_run_tool.h"

namespace OHOS {
namespace SignatureTools {
bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    char arg0[] = "";
    char arg1[] = "generate-ca";
    char arg2[] = "-keyAlias";
    char arg3[] = "oh-app-sign-srv-ca-key-v1";
    char arg4[] = "-keyPwd";
    char arg5[] = "123456";
    char arg6[] = "-issuer";
    char arg7[] = "C=CN;O=OpenHarmony;OU=OpenHarmony Community;CN=Root CA";
    char arg8[] = "-issuerKeyAlias";
    char arg9[] = "oh-root-ca-key-v1";
    char arg10[] = "-subject";
    char arg11[] = "C=CN;O=OpenHarmony;OU=OpenHarmony Community;CN= Application Signature Service CA";
    char arg12[] = "-keystoreFile";
    char arg13[] = "./generateKeyPair/OpenHarmony.p12";
    char arg14[] = "-keystorePwd";
    char arg15[] = "123456";
    char arg16[] = "-outFile";
    char arg17[] = "./generateKeyPair/app-sign-srv-ca1.cer";
    char arg18[] = "-validity";
    char arg19[] = "365";
    char arg20[] = "-keySize";
    char arg21[] = "NIST-P-384";
    char arg22[] = "-keyAlg";
    char arg23[] = "ECC";
    char arg24[] = "-signAlg";
    char arg25[] = "SHA384withECDSA";
    char arg26[] = "-issuerKeyPwd";
    char arg27[] = "123456";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12,
                     arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                     arg21, arg22, arg23, arg24, arg25, arg26, arg27 };
    int argc = 28;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);
    return ret;
}
} // namespace SignatureTools
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}