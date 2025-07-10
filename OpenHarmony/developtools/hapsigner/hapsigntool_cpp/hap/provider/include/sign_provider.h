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
#ifndef SIGNATRUETOOLS_SIGN_PROVIDER_H
#define SIGNATRUETOOLS_SIGN_PROVIDER_H

#include <sstream>
#include <set>
#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <utility>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>

#include "options.h"
#include "signature_tools_errno.h"
#include "hap_utils.h"
#include "hap_signer_block_utils.h"
#include "sign_hap.h"
#include "signature_tools_log.h"
#include "signer_config.h"
#include "param_constants.h"
#include "byte_buffer.h"
#include "data_source.h"
#include "random_access_file_input.h"
#include "random_access_file_output.h"
#include "random_access_file.h"
#include "zip_entry_header.h"
#include "zip_signer.h"
#include "zip_data_input.h"
#include "zip_utils.h"
#include "code_signing.h"
#include "byte_buffer_data_source.h"
#include "pkcs7_data.h"
#include "profile_verify.h"

typedef std::tuple<std::shared_ptr<std::ifstream>, std::shared_ptr<std::ofstream>, std::string> fileIOTuple;
namespace OHOS {
namespace SignatureTools {
class SignProvider {
public:
    SignProvider() = default;
    virtual ~SignProvider() = default;
    bool Sign(Options* options);
    bool SignElf(Options* options);
    bool SignBin(Options* options);
    bool SetSignParams(Options* options, std::unordered_set<std::string>& paramSet);
    virtual std::optional<X509_CRL*> GetCrl();
    virtual bool CheckParams(Options* options);
    virtual bool CheckInputCertMatchWithProfile(X509* inputCert, X509* certInProfile)const;

protected:
    struct DataSourceContents {
        DataSource* beforeCentralDir = nullptr;
        ByteBufferDataSource* centralDir = nullptr;
        ByteBufferDataSource* endOfCentralDir = nullptr;
        ByteBuffer cDByteBuffer;
        std::pair<ByteBuffer, int64_t> eocdPair;
        int64_t cDOffset = 0LL;
        ~DataSourceContents()
        {
            delete beforeCentralDir;
            delete centralDir;
            delete endOfCentralDir;
        }
    };

    void CheckSignAlignment();
    X509* GetCertificate(const std::string& certificate)const;
    std::string GetCertificateCN(X509* cert)const;
    std::string FindProfileFromOptionalBlocks()const;
    int CheckProfileValid(STACK_OF(X509)* inputCerts);
    int CheckProfileInfo(const ProfileInfo& info, STACK_OF(X509)* inputCerts)const;
    bool CheckSignCode();
    int LoadOptionalBlocks();
    bool CheckCompatibleVersion();
    std::vector<OptionalBlock> optionalBlocks;
    std::map<std::string, std::string> signParams = std::map<std::string, std::string>();

private:
    int CheckParmaAndInitConfig(SignerConfig& config, Options* options, std::string& suffix);

    fileIOTuple PrepareIOStreams(const std::string& inputPath, const std::string& outputPath, bool& ret);

    bool InitZipOutput(std::shared_ptr<RandomAccessFile> outputHap, std::shared_ptr<ZipSigner> zip,
                       std::shared_ptr<std::ifstream>, std::shared_ptr<std::ofstream>tmpOutput,
                       const std::string& path);

    bool PrintErrorLog(const std::string& log, const int& errorCode, std::string path = "");

    bool InitSigerConfig(SignerConfig& signerConfig, STACK_OF(X509)* publicCerts, Options* options);

    bool DoAfterSign(bool isPathOverlap, const std::string& tmpOutputFile, const std::string& inputFilePath);

    bool CreateSignerConfigs(STACK_OF(X509)* certificates, const std::optional<X509_CRL*>& crl,
                             Options* options, SignerConfig&);

    bool CopyFileAndAlignment(std::ifstream& input, std::ofstream& tmpOutput, int alignment, ZipSigner& zip);

    bool CheckSignatureAlg();

    int LoadOptionalBlock(const std::string& file, int type);
    bool CheckFile(const std::string& filePath);

    int GetX509Certificates(Options* options, STACK_OF(X509)** ret);
    int GetPublicCerts(Options* options, STACK_OF(X509)** ret);
    int GetCertificateChainFromFile(const std::string& certChianFile, STACK_OF(X509)** ret);
    int GetCertListFromFile(const std::string& certsFile, STACK_OF(X509)** ret);

    bool AppendCodeSignBlock(SignerConfig* signerConfig, std::string outputFilePath,
                             const std::string& suffix, int64_t centralDirectoryOffset, ZipSigner& zip);
    bool OutputSignedFile(RandomAccessFile* outputHap, long centralDirectoryOffset,
                          ByteBuffer& signingBlock, ByteBufferDataSource* centralDirectory, ByteBuffer& eocdBuffer);

    bool InitDataSourceContents(RandomAccessFile& outputHap, DataSourceContents& dataSrcContents);

private:
    static std::vector<std::string> VALID_SIGN_ALG_NAME;
    static constexpr int FOUR_BYTE = 4;
    std::string profileContent;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_SIGN_PROVIDER_H