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
#include "extension.h"
#include "merkle_tree_extension.h"
#include "segment_header.h"
#include "code_signing.h"
#include "file_utils.h"
#include "verify_hap.h"

namespace OHOS {
namespace SignatureTools {
bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    char arg0[] = "";
    char arg1[] = "verify-app";
    char arg2[] = "-inFile";
    char arg3[] = "./generateKeyPair/entry-default-signed-so.hap";
    char arg4[] = "-outCertChain";
    char arg5[] = "./generateKeyPair/app-sign-srv-ca1.cer";
    char arg6[] = "-outProfile";
    char arg7[] = "./generateKeyPair/app-profile.p7b";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7 };
    int argc = 8;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);
    return ret;
}

bool VerifyElf(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    char arg0[] = "";
    char arg1[] = "verify-app";
    char arg2[] = "-inFile";
    char arg3[] = "./generateKeyPair/entry-default-signed-so.elf";
    char arg4[] = "-outCertChain";
    char arg5[] = "./generateKeyPair/app-sign-srv-ca1.cer";
    char arg6[] = "-outProfile";
    char arg7[] = "./generateKeyPair/app-profile.p7b";
    char arg8[] = "-inForm";
    char arg9[] = "elf";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9 };
    int argc = 10;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);
    return ret;
}

bool VerifyBin(const uint8_t* data, size_t size)
{
    if (!data || !size) {
        return true;
    }

    char arg0[] = "";
    char arg1[] = "verify-app";
    char arg2[] = "-inFile";
    char arg3[] = "./generateKeyPair/entry-default-signed-so.bin";
    char arg4[] = "-outCertChain";
    char arg5[] = "./generateKeyPair/app-sign-srv-ca1.cer";
    char arg6[] = "-outProfile";
    char arg7[] = "./generateKeyPair/app-profile.p7b";
    char arg8[] = "-inForm";
    char arg9[] = "bin";
    char* argv[] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9 };
    int argc = 10;

    bool ret = ParamsRunTool::ProcessCmd(argv, argc);
    return ret;
}

bool TestDatastructure(const uint8_t* data, size_t size)
{
    Extension extension;
    int32_t sizet = extension.GetSize();
    MerkleTreeExtension merkleTreeExtension;
    SegmentHeader segmentHeader;
    CodeSigning codeSigning;
    VerifyHap verifyHap;
    verifyHap.setIsPrintCert(true);
    return sizet != 0;
}

bool TestFileUtils(const uint8_t* data, size_t size)
{
    std::string bytes;
    std::string file;
    bool flag = FileUtils::WriteByteToOutFile(bytes, file);
    FileUtils::DelDir(file);
    return flag;
}

bool TestNativeLibs(const uint8_t* data, size_t size)
{
    NativeLibInfoSegment nativeLibInfoSegment;
    std::vector<std::pair<std::string, SignInfo>> soInfoList;
    nativeLibInfoSegment.SetSoInfoList(soInfoList);
    return true;
}
} // namespace SignatureTools
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    (void)rename("./generateKeyPair/entry-default-signed-so.txt", "./generateKeyPair/entry-default-signed-so.hap");
    (void)rename("./generateKeyPair/signed-elf.txt", "./generateKeyPair/entry-default-signed-so.elf");
    (void)rename("./generateKeyPair/signed-bin.txt", "./generateKeyPair/entry-default-signed-so.bin");
    sync();
    /* Run your code on data */
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    OHOS::SignatureTools::VerifyElf(data, size);
    OHOS::SignatureTools::VerifyBin(data, size);
    OHOS::SignatureTools::TestDatastructure(data, size);
    OHOS::SignatureTools::TestFileUtils(data, size);
    sync();
    return 0;
}