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
#include <vector>

#include "signature_tools_log.h"
#include "pkcs7_data.h"
#include "signature_algorithm_helper.h"
#include "bc_signeddata_generator.h"
#include "signer_config.h"
#include "signature_tools_errno.h"
#include "bc_pkcs7_generator.h"

namespace OHOS {
namespace SignatureTools {
BCPkcs7Generator::~BCPkcs7Generator()
{
}
int BCPkcs7Generator::GenerateSignedData(const std::string& content,
                                         SignerConfig* signerConfig, std::string& ret)
{
    int result = RET_OK;
    std::string sigAlg;
    if (content.empty()) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "digest content is empty, generate signed data failed");
        return INVALIDPARAM_ERROR;
    }
    if (signerConfig == NULL) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR, "signerConfig is NULL");
        return INVALIDPARAM_ERROR;
    }
    std::shared_ptr<Signer> signer(signerConfig->GetSigner());
    if (signer == NULL) {
        SIGNATURE_TOOLS_LOGE("signer is NULL");
        return INVALIDPARAM_ERROR;
    }
    result = BCSignedDataGenerator::GetSigAlg(signerConfig, sigAlg);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("get sigAlg from signerConfig failed");
        return result;
    }
    result = PackagePKCS7(content, signer, sigAlg, ret);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("package pkcs7 failed!");
        return result;
    }
    return result;
}
int BCPkcs7Generator::PackagePKCS7(const std::string& content, const std::shared_ptr<Signer>& signer,
                                   const std::string& sigAlg, std::string& ret)
{
    PKCS7Data p7Data;
    int result = RET_OK;
    result = p7Data.Sign(content, signer, sigAlg, ret);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("generate pkcs7 block failed");
        return SIGN_ERROR;
    }
    result = p7Data.Parse(ret);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("parse pkcs7 bytes failed");
        return PARSE_ERROR;
    }
    result = p7Data.Verify();
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("verify pkcs7 block failed");
        return VERIFY_ERROR;
    }
    return result;
}
} // namespace SignatureTools
} // namespace OHOS