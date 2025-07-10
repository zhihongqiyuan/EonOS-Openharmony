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
#ifndef SIGNATRUETOOLS_VERIFY_HAP_V2_H
#define SIGNATRUETOOLS_VERIFY_HAP_V2_H
#include <string>

#include "byte_buffer.h"
#include "random_access_file.h"
#include "profile_verify.h"
#include "verify_hap_openssl_utils.h"
#include "signature_info.h"
#include "options.h"
#include "file_utils.h"

namespace OHOS {
namespace SignatureTools {
class VerifyHap {
public:
    static const int32_t HEX_PRINT_LENGTH;
    static const int32_t DIGEST_BLOCK_LEN_OFFSET;
    static const int32_t DIGEST_ALGORITHM_OFFSET;
    static const int32_t DIGEST_LEN_OFFSET;
    static const int32_t DIGEST_OFFSET_IN_CONTENT;
    static const std::string HAP_APP_PATTERN;
    static const std::string HQF_APP_PATTERN;
    static const std::string HSP_APP_PATTERN;
    static const std::string APP_APP_PATTERN;
    static const int OFFSET_ZERO = 0;
    static const int OFFSET_FOUR = 4;
    static const int OFFSET_EIGHT = 8;

    VerifyHap();
    VerifyHap(bool isPrintCert);
    void setIsPrintCert(bool printCert);

    bool HapOutPutPkcs7(PKCS7* p7, const std::string& outPutPath);

    DLL_EXPORT bool CheckFilePath(const std::string& filePath, std::string& standardFilePath);

    bool outputOptionalBlocks(const std::string& outputProfileFile, const std::string& outputProofFile,
                                 const std::string& outputPropertyFile,
                                 const std::vector<OptionalBlock>& optionBlocks);
    bool writeOptionalBytesToFile(const OptionalBlock& optionalBlock, const std::string& path);

    bool HapOutPutCertChain(std::vector<X509*>& certs, const std::string& outPutPath);

    int32_t Verify(const std::string& filePath, Options* options);

    int32_t WriteVerifyOutput(Pkcs7Context& pkcs7Context, std::vector<int8_t>& profile, Options* options);

    int32_t Verify(RandomAccessFile& hapFile, Options* options, const std::string& filePath);

    bool CheckCodeSign(const std::string& hapFilePath, const std::vector<OptionalBlock>& optionalBlocks)const;
    static int GetProfileContent(const std::string profile, std::string& ret);

    bool VerifyAppPkcs7(Pkcs7Context& pkcs7Context, const ByteBuffer& hapSignatureBlock);
    DLL_EXPORT bool GetDigestAndAlgorithm(Pkcs7Context& digest);
    static bool PrintCertChainToCmd(std::vector<X509*>& certChain);

private:
    bool isPrintCert;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_VERIFY_HAP_V2_H