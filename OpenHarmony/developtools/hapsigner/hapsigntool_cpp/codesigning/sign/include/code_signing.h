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
#ifndef SIGNATURETOOLS_CODE_SIGNING_H
#define SIGNATURETOOLS_CODE_SIGNING_H

#include <vector>
#include <string>
#include <contrib/minizip/unzip.h>
#include <mutex>

#include "securec.h"
#include "thread_pool.h"
#include "hap_utils.h"
#include "zip_signer.h"
#include "file_utils.h"
#include "local_signer.h"
#include "signer_config.h"
#include "signature_tools_log.h"
#include "fs_verity_generator.h"
#include "merkle_tree_extension.h"
#include "bc_signeddata_generator.h"
#include "unzip_handle_param.h"
#include "code_sign_block.h"

namespace OHOS {
namespace SignatureTools {
class CodeSigning {
public:
    static const std::vector<std::string> SUPPORT_FILE_FORM;
    static const std::string HAP_SIGNATURE_ENTRY_NAME;
    static const std::string ENABLE_SIGN_CODE_VALUE;
    static const std::string LIBS_PATH_PREFIX;
    CodeSigning(SignerConfig* signConfig);
    CodeSigning();

    bool SignFile(std::istream& inputStream,
                  int64_t fileSize, bool storeTree,
                  int64_t fsvTreeOffset, const std::string &ownerID,
                  std::pair<SignInfo, std::vector<int8_t>>& ret);
    bool GetCodeSignBlock(const std::string &input, int64_t offset,
                          const std::string &inForm, const std::string &profileContent,
                          ZipSigner& zip, std::vector<int8_t>& ret);
    bool GetElfCodeSignBlock(const std::string &input, int64_t offset,
                             const std::string &inForm, const std::string &profileContent,
                             std::vector<int8_t> &codesignData);

public:
    static bool IsNativeFile(const std::string& input);
    int64_t ComputeDataSize(ZipSigner& zip);
    int64_t GetTimestamp();
    bool SignNativeLibs(const std::string &input, std::string &ownerID);
    void UpdateCodeSignBlock();
    bool GetNativeEntriesFromHap(const std::string& packageName, UnzipHandleParam& param);
    bool GetSingleFileStreamFromZip(unzFile& zFile, char fileName[],
                                    unz_file_info& zFileInfo,
                                    int& readFileSize, std::stringbuf& sb);
    bool RunParseZipInfo(const std::string& packageName, UnzipHandleParam& param, uLong index);
    bool GenerateSignature(const std::vector<int8_t>& signedData, const std::string&,
                           std::vector<int8_t>&);
    int64_t m_timestamp = 0;
    std::vector<std::string> m_extractedNativeLibSuffixs;
    SignerConfig* m_signConfig;
    CodeSignBlock m_codeSignBlock;
    std::mutex m_mutex;

private:
    static bool CheckUnzParam(unzFile& zFile, unz_file_info& zFileInfo, char fileName[], size_t* nameLen);
    static bool CheckFileName(char fileName[], size_t* nameLen);
    bool HandleZipGlobalInfo(const std::string& packageName, unzFile& zFile,
                             unz_global_info& zGlobalInfo, UnzipHandleParam& param);
    bool DoNativeLibVerify(std::string fileName, std::stringbuf& sb,
                           UnzipHandleParam& param, int readFileSize);
    bool DoNativeLibSignOrVerify(std::string fileName, std::stringbuf& sb,
                                 UnzipHandleParam& param, int readFileSize);
    static constexpr int MAX_PROCESSORS = 32;
    const int POOL_SIZE = std::min(MAX_PROCESSORS, static_cast<int>(std::thread::hardware_concurrency()));
    std::shared_ptr<Uscript::ThreadPool> mPools;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_CODE_SIGNING_H