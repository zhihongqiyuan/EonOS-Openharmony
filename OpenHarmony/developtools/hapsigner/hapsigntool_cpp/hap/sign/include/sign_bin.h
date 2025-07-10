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

#ifndef SIGNATRUETOOLS_SIGN_BIN_H
#define SIGNATRUETOOLS_SIGN_BIN_H

#include "signer_config.h"

namespace OHOS {
namespace SignatureTools {
class SignBin {
public:
    static bool Sign(SignerConfig& signerConfig, const std::map<std::string, std::string>& signParams);
    static std::vector<int8_t> GenerateFileDigest(const std::string& outputFile,
                                                  const std::string& signAlg);

private:
    static bool WriteBlockDataToFile(const std::string& inputFile, const std::string& outputFile,
                                     const std::string& profileFile, const std::string& profileSigned);
    static bool WriteSignDataToOutputFile(SignerConfig& SignerConfig, const std::string& outputFile,
                                          const std::string& signAlg);
    static bool WriteSignHeadDataToOutputFile(const std::string& inputFile, const std::string& outputFile);
    static bool CheckBinAndProfileLengthIsValid(int64_t binFileLen, int64_t profileDataLen);
    static bool IsLongOverflowInteger(int64_t num);
    static bool IsLongOverflowShort(int64_t num);
    static bool WriteSignedBin(const std::string& inputFile, const std::string& proBlockByte,
                               const std::string& signBlockByte, const std::string& profileFile,
                               const std::string& outputFile);
};
} // namespace SignatureTools
} // namespace OHOS
#endif