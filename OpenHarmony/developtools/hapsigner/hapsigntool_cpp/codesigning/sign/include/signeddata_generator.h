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
#ifndef SIGNATRUETOOLS_SIGNEDDATA_GENERATOR_H
#define SIGNATRUETOOLS_SIGNEDDATA_GENERATOR_H

#include <string>

#include "signer_config.h"

namespace OHOS {
namespace SignatureTools {

class SignedDataGenerator {
public:
    virtual ~SignedDataGenerator() = default;
    /**
     * Generate signature data with specific content and sign configuration.
     *
     * @param content      unsigned file digest content.
     * @param signerConfig sign configurations.
     * @param ret signed data.
     * @return 0:success <0:error
     */
    virtual int GenerateSignedData(const std::string& content, SignerConfig* signerConfig, std::string& ret) = 0;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_SIGNEDDATA_GENERATOR_H