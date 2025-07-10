/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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
#ifndef SIGNATRUETOOLS_PROFILESIGNTOOOL_H
#define SIGNATRUETOOLS_PROFILESIGNTOOOL_H

#include <memory>
#include <string>

#include "openssl/pkcs7.h"
#include "openssl/err.h"
#include "openssl/x509.h"
#include "signer.h"
#include "localization_adapter.h"
#include "pkcs7_data.h"

namespace OHOS {
namespace SignatureTools {
/**
 * To sign and verify profile.
 *
 * @since 2021/12/28
 */
class ProfileSignTool {
public:
    /**
     * generateP7b.
     *
     * @param adapter local adapter with params
     * @param content content to sign
     * @param ret signed content
     * @return 0:success <0:error
     */
    static int GenerateP7b(LocalizationAdapter& adapter, const std::string& content, std::string& ret);
    /**
      * signProfile.
      *
      * @param content content to sign
      * @param signer signer
      * @param sigAlg sign algorithm  only SHAwith256 or SHAwith384
      * @ret signed data
      * @return 0:success <0:error
      */
    static int SignProfile(const std::string& content,
                           const std::shared_ptr<Signer>& signer, const std::string& sigAlg, std::string& ret);
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_PROFILESIGNTOOOL_H