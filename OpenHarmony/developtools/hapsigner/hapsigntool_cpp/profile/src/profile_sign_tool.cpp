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
#include "profile_sign_tool.h"
#include "signer_factory.h"
#include "local_signer.h"
#include "localization_adapter.h"
#include "file_utils.h"
#include "pkcs7_data.h"
#include "verify_hap_openssl_utils.h"
#include "signature_tools_errno.h"

namespace OHOS {
namespace SignatureTools {

int ProfileSignTool::GenerateP7b(LocalizationAdapter& adapter, const std::string& content, std::string& ret)
{
    std::unique_ptr<SignerFactory> signerFactory = std::make_unique<SignerFactory>();
    int result = RET_OK;
    std::shared_ptr<Signer> signer(signerFactory->GetSigner(adapter));
    if (signer == NULL) {
        SIGNATURE_TOOLS_LOGE("signer is NULL, get signer failed");
        return INVALIDPARAM_ERROR;
    }
    const std::string sigAlg = adapter.GetSignAlg();
    // ret is the generated p7b data
    result = SignProfile(content, signer, sigAlg, ret);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("generate p7b failed");
        return SIGN_ERROR;
    }
    PKCS7Data p7Data;
    result = p7Data.Parse(ret);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("parse p7b failed");
        return PARSE_ERROR;
    }
    result = p7Data.Verify();
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("verify p7b failed");
        return VERIFY_ERROR;
    }
    return result;
}
/**
* @param content content to sign
* @param signer signer
* @param sigAlg sign algorithm  only SHAwith256 or SHAwith384
* @param ret signed data
* @return 0:success <0:error
*/
int ProfileSignTool::SignProfile(const std::string& content, const std::shared_ptr<Signer>& signer,
                                 const std::string& sigAlg, std::string& ret)
{
    PKCS7Data p7Data;
    int result = RET_OK;
    result = p7Data.Sign(content, signer, sigAlg, ret);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("SignProfile faild!");
        return SIGN_ERROR;
    }
    return result;
}
} // namespace SignatureTools
} // namespace OHOS