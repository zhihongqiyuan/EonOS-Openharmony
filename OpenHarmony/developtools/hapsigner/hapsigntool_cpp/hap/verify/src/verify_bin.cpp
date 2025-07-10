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

#include "verify_bin.h"
#include "verify_hap.h"
#include "constant.h"

namespace OHOS {
namespace SignatureTools {
    
bool VerifyBin::Verify(Options* options)
{
    // check param
    if (options == nullptr) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "Param options is null.");
        return false;
    }
    if (!VerifyElf::CheckParams(options)) {
        SIGNATURE_TOOLS_LOGE("verify bin check params failed!");
        return false;
    }
    std::string filePath = options->GetString(Options::IN_FILE);
    bool checkSignFileFlag = VerifyElf::CheckSignFile(filePath);
    if (!checkSignFileFlag) {
        SIGNATURE_TOOLS_LOGE("check input bin file %s failed!", filePath.c_str());
        return false;
    }
    // verify bin
    std::vector<int8_t> profileVec;
    Pkcs7Context pkcs7Context;
    bool verifyBinFileFLag = VerifyBinFile(filePath, profileVec, options, pkcs7Context);
    if (!verifyBinFileFLag) {
        SIGNATURE_TOOLS_LOGE("verify bin file %s failed!", filePath.c_str());
        return false;
    }
    // write certificate and p7b file
    VerifyHap hapVerify(false);
    int32_t writeFlag = hapVerify.WriteVerifyOutput(pkcs7Context, profileVec, options);
    if (writeFlag != RET_OK) {
        SIGNATURE_TOOLS_LOGE("write elf output failed on verify bin!");
        return false;
    }
    return true;
}

bool VerifyBin::VerifyBinFile(const std::string& binFile, std::vector<int8_t>& profileVec,
                              Options* options, Pkcs7Context& pkcs7Context)
{
    SignBlockInfo signBlockInfo(true);
    bool getSignBlockInfoFlag = VerifyElf::GetSignBlockInfo(binFile, signBlockInfo, BIN);
    if (!getSignBlockInfoFlag) {
        SIGNATURE_TOOLS_LOGE("get signBlockInfo failed on verify bin %s", binFile.c_str());
        return false;
    }
    // verify profile
    std::string profileJson;
    bool verifyP7bFlag = VerifyElf::VerifyP7b(signBlockInfo.GetSignBlockMap(), options, pkcs7Context,
        profileVec, profileJson);
    if (!verifyP7bFlag) {
        SIGNATURE_TOOLS_LOGE("verify profile failed on verify bin %s", binFile.c_str());
        return false;
    }
    // verify signed data
    bool verifyBinDigestFlag = VerifyBinDigest(signBlockInfo);
    if (!verifyBinDigestFlag) {
        SIGNATURE_TOOLS_LOGE("verify digest failed on verify bin %s", binFile.c_str());
        return false;
    }
    return true;
}

bool VerifyBin::VerifyBinDigest(SignBlockInfo& signBlockInfo)
{
    std::vector<int8_t>& rawDigest = signBlockInfo.GetRawDigest();
    std::vector<int8_t>& generatedDig = signBlockInfo.GetFileDigest();
    bool isEqual = rawDigest.empty() || generatedDig.empty() ||
        !std::equal(rawDigest.begin(), rawDigest.end(), generatedDig.begin());
    if (isEqual) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "compare bin file raw digest and generated digest failed!");
        return false;
    }
    return true;
}

} // namespace SignatureTools
} // namespace OHOS