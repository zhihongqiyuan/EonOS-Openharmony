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
#include "sign_provider.h"

#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <cstdio>
#include <cinttypes>
#include <filesystem>
#include <algorithm>

#include "nlohmann/json.hpp"
#include "string_utils.h"
#include "file_utils.h"
#include "sign_elf.h"
#include "sign_bin.h"
#include "params.h"
#include "constant.h"

using namespace nlohmann;
namespace OHOS {
namespace SignatureTools {
std::vector<std::string> SignProvider::VALID_SIGN_ALG_NAME = {
    ParamConstants::HAP_SIG_ALGORITHM_SHA256_ECDSA,
    ParamConstants::HAP_SIG_ALGORITHM_SHA384_ECDSA,
    ParamConstants::HAP_SIG_ALGORITHM_SHA512_ECDSA
};

bool SignProvider::PrintErrorLog(const std::string& log, const int& errorCode, std::string path)
{
    SIGNATURE_TOOLS_LOGE("%s", std::string("(Error Code: " + std::to_string(errorCode) + ")" + log).c_str());
    if (path != "") {
        remove(path.c_str());
    }
    return false;
}

bool SignProvider::InitSigerConfig(SignerConfig& signerConfig, STACK_OF(X509)* publicCerts, Options* options)
{
    std::optional<X509_CRL*> crl = GetCrl();
    if (!CreateSignerConfigs(publicCerts, crl, options, signerConfig)) {
        SIGNATURE_TOOLS_LOGE("[SignHap] create Signer Configs failed");
        return false;
    }
    int CompatibleVersion;
    if (!StringUtils::CheckStringToint(signParams.at(ParamConstants::PARAM_BASIC_COMPATIBLE_VERSION),
        CompatibleVersion)) {
        SIGNATURE_TOOLS_LOGE("[SignHap] CompatibleVersion String To int failed");
        return false;
    }
    signerConfig.SetCompatibleVersion(CompatibleVersion);
    return true;
}

int SignProvider::CheckParmaAndInitConfig(SignerConfig& signerConfig, Options* options, std::string& suffix)
{
    STACK_OF(X509)* publicCerts = nullptr;
    int ret = GetX509Certificates(options, &publicCerts);
    if (ret != RET_OK) {
        sk_X509_pop_free(publicCerts, X509_free);
        SIGNATURE_TOOLS_LOGE("SIGNHAP_ERROR get X509 Certificates failed");
        return ret;
    }
    if (!CheckCompatibleVersion()) {
        sk_X509_pop_free(publicCerts, X509_free);
        SIGNATURE_TOOLS_LOGE("check Compatible Version failed!!");
        return COMMAND_PARAM_ERROR;
    }
    if (!InitSigerConfig(signerConfig, publicCerts, options)) {
        SIGNATURE_TOOLS_LOGE("create Signer Configs failed");
        return COMMAND_PARAM_ERROR;
    }
    std::string inputFilePath = signParams.at(ParamConstants::PARAM_BASIC_INPUT_FILE);
    suffix = FileUtils::GetSuffix(inputFilePath);
    if (suffix == "") {
        SIGNATURE_TOOLS_LOGE("hap format error pleass check!!");
        return COMMAND_PARAM_ERROR;
    }
    return RET_OK;
}

fileIOTuple SignProvider::PrepareIOStreams(const std::string& inputPath,
                                           const std::string& outputPath, bool& ret)
{
    std::shared_ptr<std::ifstream> inputFile = nullptr;
    std::shared_ptr<std::ofstream> outputFile = nullptr;
    std::string tmpOutputFilePath;
    ret = false;
    inputFile = std::make_shared<std::ifstream>(inputPath, std::ios::binary);
    if (!inputFile->good()) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                            inputPath + " open failed");
        return {nullptr, nullptr, ""};
    }
    if (inputPath == outputPath) {
        std::filesystem::path filePath = outputPath;
        std::filesystem::path directory = filePath.parent_path();
        std::string strDirectory = directory;
        tmpOutputFilePath = strDirectory + '/' + std::string("signedHap") + "." + "hap";
        outputFile = std::make_shared<std::ofstream>(tmpOutputFilePath, std::ios::binary | std::ios::trunc);
        if (!outputFile->good()) {
            PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                                tmpOutputFilePath + " open failed");
            return {nullptr, nullptr, ""};
        }
        ret = true;
    } else {
        outputFile = std::make_shared<std::ofstream>(outputPath, std::ios::binary | std::ios::trunc);
        if (!outputFile->good()) {
            PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                                outputPath + " open failed");
            return {nullptr, nullptr, ""};
        }
        tmpOutputFilePath = outputPath;
    }
    return {inputFile, outputFile, tmpOutputFilePath};
}

bool SignProvider::InitZipOutput(std::shared_ptr<RandomAccessFile> outputHap,
                                 std::shared_ptr<ZipSigner> zip,
                                 std::shared_ptr<std::ifstream> inputStream,
                                 std::shared_ptr<std::ofstream>tmpOutput,
                                 const std::string& Path)
{
    int alignment;
    if (!StringUtils::CheckStringToint(signParams.at(ParamConstants::PARAM_BASIC_ALIGNMENT), alignment)) {
        SIGNATURE_TOOLS_LOGE("[signHap] alignment String To int failed");
        inputStream->close();
        tmpOutput->close();
        remove(Path.c_str());
        return false;
    }

    if (!CopyFileAndAlignment(*inputStream, *tmpOutput, alignment, *zip)) {
        SIGNATURE_TOOLS_LOGE("[signHap] copy File And Alignment failed");
        inputStream->close();
        tmpOutput->close();
        remove(Path.c_str());
        return false;
    }

    inputStream->close();
    tmpOutput->flush();
    tmpOutput->close();
    if (!outputHap->Init(Path)) {
        SIGNATURE_TOOLS_LOGE("[signHap] init outputFile failed %s", Path.c_str());
        remove(Path.c_str());
        return false;
    }
    return true;
}

bool SignProvider::InitDataSourceContents(RandomAccessFile& outputHap, DataSourceContents& dataSrcContents)
{
    std::shared_ptr<ZipDataInput> outputHapIn = std::make_shared<RandomAccessFileInput>(outputHap);
    // get eocd bytebuffer and eocd offset
    if (!HapSignerBlockUtils::FindEocdInHap(outputHap, dataSrcContents.eocdPair)) {
        PrintErrorNumberMsg("ZIP_ERROR", ZIP_ERROR, "eocd is not found in hap");
        return false;
    }
    dataSrcContents.endOfCentralDir = new ByteBufferDataSource(dataSrcContents.eocdPair.first);
    if (!dataSrcContents.endOfCentralDir) {
        return false;
    }

    // get cd offset
    if (!HapSignerBlockUtils::GetCentralDirectoryOffset(dataSrcContents.eocdPair.first,
                                                        dataSrcContents.eocdPair.second, dataSrcContents.cDOffset)) {
        PrintErrorNumberMsg("ZIP_ERROR", ZIP_ERROR, "get central directory offset failed");
        return false;
    }

    SIGNATURE_TOOLS_LOGI("Central Directory Offset is %" PRId64, dataSrcContents.cDOffset);

    // get beforeCentralDir
    dataSrcContents.beforeCentralDir = outputHapIn->Slice(0, dataSrcContents.cDOffset);
    if (!dataSrcContents.beforeCentralDir) {
        return false;
    }

    // get cd size
    long cDSize;
    if (!HapSignerBlockUtils::GetCentralDirectorySize(dataSrcContents.eocdPair.first, cDSize)) {
        PrintErrorNumberMsg("ZIP_ERROR", ZIP_ERROR, "get central directory size failed");
        return false;
    }

    // get cd buffer
    dataSrcContents.cDByteBuffer = outputHapIn->CreateByteBuffer(dataSrcContents.cDOffset, cDSize);
    if (dataSrcContents.cDByteBuffer.GetCapacity() == 0) {
        return false;
    }
    dataSrcContents.centralDir = new ByteBufferDataSource(dataSrcContents.cDByteBuffer);
    if (!dataSrcContents.centralDir) {
        return false;
    }
    return true;
}

bool SignProvider::Sign(Options* options)
{
    bool isPathOverlap = false;
    SignerConfig signerConfig;
    std::string suffix;
    if (CheckParmaAndInitConfig(signerConfig, options, suffix) != RET_OK) {
        return PrintErrorLog("Check Parma And Init Config failed", COMMAND_PARAM_ERROR);
    }
    // Since CheckParmaAndInitConfig has already validated all parameters, it is possible to directly use at
    std::string inputFilePath = signParams.at(ParamConstants::PARAM_BASIC_INPUT_FILE);
    auto [inputStream, tmpOutput, tmpOutputFilePath] = PrepareIOStreams(
        inputFilePath,
        signParams.at(ParamConstants::PARAM_BASIC_OUTPUT_FILE), isPathOverlap);

    if (!inputStream || !tmpOutput) {
        return PrintErrorLog("[signHap] Prepare IO Streams failed", IO_ERROR);
    }

    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    if (!InitZipOutput(outputHap, zip, inputStream, tmpOutput, tmpOutputFilePath)) {
        return PrintErrorLog("[signHap] Init Zip Output failed", IO_ERROR);
    }

    DataSourceContents dataSrcContents;
    if (!InitDataSourceContents(*outputHap, dataSrcContents)) {
        return PrintErrorLog("[signHap] Init Data Source Contents failed", ZIP_ERROR, tmpOutputFilePath);
    }

    DataSource* contents[] = {dataSrcContents.beforeCentralDir,
        dataSrcContents.centralDir, dataSrcContents.endOfCentralDir
    };
    if (!AppendCodeSignBlock(&signerConfig, tmpOutputFilePath, suffix, dataSrcContents.cDOffset, *zip)) {
        return PrintErrorLog("[SignCode] AppendCodeSignBlock failed", SIGN_ERROR, tmpOutputFilePath);
    }

    ByteBuffer signingBlock;
    if (!SignHap::Sign(contents, sizeof(contents) / sizeof(contents[0]), signerConfig, optionalBlocks,
                       signingBlock)) {
        return PrintErrorLog("[SignHap] SignHap Sign failed.", SIGN_ERROR, tmpOutputFilePath);
    }

    int64_t newCentralDirectoryOffset = dataSrcContents.cDOffset + signingBlock.GetCapacity();
    SIGNATURE_TOOLS_LOGI("new Central Directory Offset is %" PRId64, newCentralDirectoryOffset);
    dataSrcContents.eocdPair.first.SetPosition(0);
    if (!ZipUtils::SetCentralDirectoryOffset(dataSrcContents.eocdPair.first, newCentralDirectoryOffset)) {
        return PrintErrorLog("[SignHap] Set Central Directory Offset.", ZIP_ERROR, tmpOutputFilePath);
    }

    if (!OutputSignedFile(outputHap.get(), dataSrcContents.cDOffset, signingBlock, dataSrcContents.centralDir,
                          dataSrcContents.eocdPair.first)) {
        return PrintErrorLog("[SignHap] write output signed file failed.", ZIP_ERROR, tmpOutputFilePath);
    }
    return DoAfterSign(isPathOverlap, tmpOutputFilePath, inputFilePath);
}

bool SignProvider::SignElf(Options* options)
{
    bool isPathOverlap = false;
    STACK_OF(X509)* publicCerts = nullptr;
    int ret = GetX509Certificates(options, &publicCerts);
    if (ret != RET_OK) {
        sk_X509_pop_free(publicCerts, X509_free);
        SIGNATURE_TOOLS_LOGE("[SignElf] get X509 Certificates failed! errorCode:%d", ret);
        return false;
    }
    if (!CheckCompatibleVersion()) {
        sk_X509_pop_free(publicCerts, X509_free);
        SIGNATURE_TOOLS_LOGE("[SignElf] check Compatible Version failed!!");
        return false;
    }

    std::string inputFilePath = signParams.at(ParamConstants::PARAM_BASIC_INPUT_FILE);

    auto [inputStream, tmpOutput, tmpOutputFilePath] =
        PrepareIOStreams(inputFilePath,
                         signParams.at(ParamConstants::PARAM_BASIC_OUTPUT_FILE),
                         isPathOverlap);

    if (!inputStream || !tmpOutput) {
        sk_X509_pop_free(publicCerts, X509_free);
        SIGNATURE_TOOLS_LOGE("[signElf] Prepare IO Streams failed");
        return false;
    }

    SignerConfig signerConfig;
    if (!InitSigerConfig(signerConfig, publicCerts, options)) {
        SIGNATURE_TOOLS_LOGE("SignElf] create Signer Configs failed");
        return false;
    }

    if (!profileContent.empty()) {
        signParams.insert(std::make_pair(ParamConstants::PARAM_PROFILE_JSON_CONTENT, profileContent));
    }

    if (!SignElf::Sign(signerConfig, signParams)) {
        SIGNATURE_TOOLS_LOGE("[SignElf] sign elf failed");
        return false;
    }

    return true;
}

bool SignProvider::SignBin(Options* options)
{
    STACK_OF(X509)* x509Certificates = nullptr;
    int ret = GetX509Certificates(options, &x509Certificates);
    if (ret != RET_OK) {
        sk_X509_pop_free(x509Certificates, X509_free);
        SIGNATURE_TOOLS_LOGE("[SignBin] get X509 Certificates failed! errorCode:%d", ret);
        return false;
    }
    if (!CheckCompatibleVersion()) {
        sk_X509_pop_free(x509Certificates, X509_free);
        SIGNATURE_TOOLS_LOGE("check Compatible Version failed!");
        return false;
    }

    SignerConfig signerConfig;
    if (!InitSigerConfig(signerConfig, x509Certificates, options)) {
        SIGNATURE_TOOLS_LOGE("[SignBin] create Signer Configs failed");
        return false;
    }

    bool signFlag = SignBin::Sign(signerConfig, signParams);
    if (!signFlag) {
        SIGNATURE_TOOLS_LOGE("sign bin internal failed");
        return false;
    }

    SIGNATURE_TOOLS_LOGI("sign bin success");
    return true;
}

bool SignProvider::AppendCodeSignBlock(SignerConfig* signerConfig, std::string outputFilePath,
                                       const std::string& suffix, int64_t centralDirectoryOffset, ZipSigner& zip)
{
    if (signParams.at(ParamConstants::PARAM_SIGN_CODE) == CodeSigning::ENABLE_SIGN_CODE_VALUE) {
        SIGNATURE_TOOLS_LOGI("start code signing.");
        std::string suffixTmp = suffix;
        std::transform(suffixTmp.begin(), suffixTmp.end(), suffixTmp.begin(), ::tolower);
        if (std::find(CodeSigning::SUPPORT_FILE_FORM.begin(), CodeSigning::SUPPORT_FILE_FORM.end(),
                      suffixTmp) == CodeSigning::SUPPORT_FILE_FORM.end()) {
            SIGNATURE_TOOLS_LOGI("no need to sign code for %s", suffixTmp.c_str());
            return true;
        }
        // 4 means hap format occupy 4 byte storage location,2 means optional blocks reserve 2 storage location
        int64_t codeSignOffset = centralDirectoryOffset + ((4 + 4 + 4) * (optionalBlocks.size() + 2 + 1));
        // create CodeSigning Object
        CodeSigning codeSigning(signerConfig);
        std::vector<int8_t> codeSignArray;
        if (!codeSigning.GetCodeSignBlock(outputFilePath, codeSignOffset, suffixTmp, profileContent, zip,
                                          codeSignArray)) {
            SIGNATURE_TOOLS_LOGE("Codesigning getCodeSignBlock Fail.");
            return false;
        }
        SIGNATURE_TOOLS_LOGI("generate codeSignArray finished.");
        std::unique_ptr<ByteBuffer> result =
            std::make_unique<ByteBuffer>(codeSignArray.size()
                                         + (FOUR_BYTE + FOUR_BYTE + FOUR_BYTE));
        result->PutInt32(HapUtils::HAP_CODE_SIGN_BLOCK_ID);
        result->PutInt32(codeSignArray.size()); // length
        result->PutInt32((int32_t)codeSignOffset); // offset
        result->PutData(codeSignArray.data(), codeSignArray.size());

        OptionalBlock tmp = {HapUtils::HAP_PROPERTY_BLOCK_ID, *result};
        optionalBlocks.insert(optionalBlocks.begin(), tmp);
    }
    return true;
}

bool SignProvider::CreateSignerConfigs(STACK_OF(X509)* certificates, const std::optional<X509_CRL*>& crl,
                                       Options* options, SignerConfig& inOut)
{
    inOut.FillParameters(signParams);
    inOut.SetCertificates(certificates);
    inOut.SetOptions(options);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    SignatureAlgorithmHelper alg;
    // Since CheckParmaAndInitConfig has already validated all parameters, it is possible to directly use at
    if (!Params::GetSignatureAlgorithm(signParams.at(ParamConstants::PARAM_BASIC_SIGANTURE_ALG),
                                       alg)) {
        SIGNATURE_TOOLS_LOGE("[SignHap] get Signature Algorithm failed");
        return false;
    }
    signatureAlgorithms.push_back(alg);
    inOut.SetSignatureAlgorithms(signatureAlgorithms);
    if (crl.has_value()) {
        SIGNATURE_TOOLS_LOGE("not support crl");
        return false;
    }
    return true;
}

int SignProvider::LoadOptionalBlocks()
{
    int ret = RET_OK;
    if (auto property = signParams.find(ParamConstants::PARAM_BASIC_PROPERTY);
        property != signParams.end()) {
        if ((ret = LoadOptionalBlock(property->second, HapUtils::HAP_PROPERTY_BLOCK_ID)) != RET_OK)
            return ret;
    }
    if (auto profile = signParams.find(ParamConstants::PARAM_BASIC_PROFILE); profile != signParams.end()) {
        if ((ret = LoadOptionalBlock(profile->second, HapUtils::HAP_PROFILE_BLOCK_ID)) != RET_OK)
            return ret;
    }
    if (auto proofOfRotation = signParams.find(ParamConstants::PARAM_BASIC_PROOF);
        proofOfRotation != signParams.end()) {
        if ((LoadOptionalBlock(proofOfRotation->second, HapUtils::HAP_PROOF_OF_ROTATION_BLOCK_ID)) != RET_OK)
            return ret;
    }
    return ret;
}

int SignProvider::LoadOptionalBlock(const std::string& file, int type)
{
    if (file.empty())
        return RET_OK;
    if (!CheckFile(file)) {
        SIGNATURE_TOOLS_LOGE("check file failed. Invalid file: %s, file type: %d",
                             file.c_str(), type);
        return FILE_NOT_FOUND;
    }
    ByteBuffer optionalBlockBuffer;
    if (!HapUtils::ReadFileToByteBuffer(file, optionalBlockBuffer))
        return IO_ERROR;
    if (optionalBlockBuffer.GetCapacity() == 0) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, file + " is empty!");
        return IO_ERROR;
    }
    optionalBlocks.push_back({type, optionalBlockBuffer});
    return RET_OK;
}

std::optional<X509_CRL*> SignProvider::GetCrl()
{
    return std::nullopt;
}

bool SignProvider::CheckFile(const std::string& filePath)
{
    if (filePath.empty()) {
        PrintErrorNumberMsg("FILE_NOT_FOUND", FILE_NOT_FOUND, "file name is null.");
        return false;
    }
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath)) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, filePath + " not exist or can not read!");
        return false;
    }
    return true;
}

int SignProvider::GetX509Certificates(Options* options, STACK_OF(X509)** X509Vec)
{
    int ret = RET_OK;
    // 1.check the parameters
    if (!CheckParams(options)) {
        SIGNATURE_TOOLS_LOGE("Check Params failed please check");
        return COMMAND_ERROR;
    }
    // 2.get x509 verify certificate
    ret = GetPublicCerts(options, X509Vec);
    if (ret != RET_OK) {
        SIGNATURE_TOOLS_LOGE("Get Public Certs please check");
        return ret;
    }
    // 3. load optionalBlocks
    ret = LoadOptionalBlocks();
    if (ret != RET_OK) {
        SIGNATURE_TOOLS_LOGE("Load Optional Blocks please check");
        return ret;
    }
    std::string inForm = options->GetString(Options::INFORM);
    std::string profileFile = options->GetString(Options::PROFILE_FILE);
    if (StringUtils::CaseCompare(inForm, ELF) && FileUtils::IsEmpty(profileFile)) {
        return ret;
    }
    // 4. check Profile Valid
    if ((ret = CheckProfileValid(*X509Vec)) < 0) {
        SIGNATURE_TOOLS_LOGE("profile check error");
        sk_X509_pop_free(*X509Vec, X509_free);
        *X509Vec = nullptr;
        return ret;
    }
    return ret;
}

int SignProvider::GetPublicCerts(Options* options, STACK_OF(X509)** ret)
{
    std::string appCertFileName = options->GetString(Options::APP_CERT_FILE);
    if (appCertFileName.empty()) {
        SIGNATURE_TOOLS_LOGI("appCertFile param can not find,may be is RemoteSigner");
        return RET_OK;
    }
    return GetCertificateChainFromFile(appCertFileName, ret);
}

int SignProvider::GetCertificateChainFromFile(const std::string& certChianFile, STACK_OF(X509)** ret)
{
    return GetCertListFromFile(certChianFile, ret);
}

int SignProvider::GetCertListFromFile(const std::string& certsFile, STACK_OF(X509)** ret)
{
    X509* cert = nullptr;
    *ret = sk_X509_new(nullptr);
    if (*ret == nullptr) {
        PrintErrorNumberMsg("RET_FAILED", RET_FAILED, "get CertList FromFile [sk_X509_new] failed");
        return RET_FAILED;
    }
    BIO* certBio = BIO_new_file(certsFile.c_str(), "rb");
    if (!certBio) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                            std::string("get CertList from file ") + certsFile + " failed");
        sk_X509_free(*ret);
        return IO_ERROR;
    }
    while (1) {
        cert = PEM_read_bio_X509(certBio, NULL, NULL, NULL);
        if (cert == nullptr)
            break;
        sk_X509_push(*ret, cert);
    }
    BIO_free(certBio);
    return RET_OK;
}

bool SignProvider::DoAfterSign(bool isPathOverlap, const std::string& tmpOutputFile, const std::string& inputFilePath)
{
    if (isPathOverlap) {
        remove(inputFilePath.c_str());
        if (rename(tmpOutputFile.c_str(), inputFilePath.c_str()) != 0) {
            PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                                "File name " + tmpOutputFile + " rename to " + inputFilePath + " failed!");
            return false;
        }
    }
    return true;
}

bool SignProvider::CopyFileAndAlignment(std::ifstream& input, std::ofstream& tmpOutput, int alignment, ZipSigner& zip)
{
    if (!zip.Init(input)) {
        PrintErrorNumberMsg("ZIP_ERROR", ZIP_ERROR, "zip init failed");
        return false;
    }
    zip.Alignment(alignment);
    zip.RemoveSignBlock();
    if (!zip.ToFile(input, tmpOutput)) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "zip write to file failed");
        return false;
    }
    return true;
}

bool SignProvider::SetSignParams(Options* options, std::unordered_set<std::string>& paramSet)
{
    for (auto it = options->begin(); it != options->end(); it++) {
        if (paramSet.find(it->first) != paramSet.end()) {
            size_t size = it->first.size();
            std::string str = it->first.substr(size - 3);
            if (str != "Pwd") {
                this->signParams.insert(std::make_pair(it->first, options->GetString(it->first)));
            }
        }
    }
    return true;
}

bool SignProvider::CheckParams(Options* options)
{
    std::vector<std::string> paramFileds;
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_ALIGNMENT);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_SIGANTURE_ALG);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_INPUT_FILE);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_OUTPUT_FILE);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PRIVATE_KEY);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PROFILE);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PROOF);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PROPERTY);
    paramFileds.emplace_back(ParamConstants::PARAM_REMOTE_SERVER);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PROFILE_SIGNED);
    paramFileds.emplace_back(ParamConstants::PARAM_LOCAL_PUBLIC_CERT);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_COMPATIBLE_VERSION);
    paramFileds.emplace_back(ParamConstants::PARAM_SIGN_CODE);
    paramFileds.emplace_back(ParamConstants::PARAM_IN_FORM);

    std::unordered_set<std::string> paramSet = Params::InitParamField(paramFileds);
    for (auto it = options->begin(); it != options->end(); it++) {
        if (paramSet.find(it->first) != paramSet.end()) {
            signParams.insert(std::make_pair(it->first, options->GetString(it->first)));
        }
    }

    if (signParams.find(ParamConstants::PARAM_BASIC_PROFILE_SIGNED) == signParams.end()
        || signParams.at(ParamConstants::PARAM_BASIC_PROFILE_SIGNED).empty()) {
        signParams[ParamConstants::PARAM_BASIC_PROFILE_SIGNED] = "1";
    }
    if (!CheckSignCode()) {
        SIGNATURE_TOOLS_LOGE("signCode Parameter check error, signCode must is 0 or 1");
        return false;
    }
    if (!CheckSignatureAlg()) {
        SIGNATURE_TOOLS_LOGE("signAlg Parameter is not support");
        return false;
    }
    CheckSignAlignment();
    return true;
}

bool SignProvider::CheckSignCode()
{
    if (signParams.find(ParamConstants::PARAM_SIGN_CODE) == signParams.end()) {
        signParams.insert(std::make_pair(ParamConstants::PARAM_SIGN_CODE, ParamConstants::ENABLE_SIGN_CODE));
        return true;
    }
    std::string codeSign = signParams[ParamConstants::PARAM_SIGN_CODE];
    if ((codeSign != ParamConstants::ENABLE_SIGN_CODE) && (codeSign != ParamConstants::DISABLE_SIGN_CODE)) {
        PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                            "signCode Parameter must 0 or 1, you put is " + codeSign);
        return false;
    }
    return true;
}

bool SignProvider::CheckSignatureAlg()
{
    std::string signAlg = signParams[ParamConstants::PARAM_BASIC_SIGANTURE_ALG];
    // Remove leading spaces
    size_t start = signAlg.find_first_not_of(" ");
    if (start != std::string::npos) {
        signAlg = signAlg.substr(start);
    }
    // Remove trailing spaces
    size_t end = signAlg.find_last_not_of(" ");
    if (end != std::string::npos) {
        signAlg.resize(end + 1);
    }
    for (auto it = VALID_SIGN_ALG_NAME.begin(); it != VALID_SIGN_ALG_NAME.end(); it++) {
        if (StringUtils::CaseCompare(*it, signAlg)) {
            return true;
        }
    }
    PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                        "signAlg Parameter is only support [SHA256withECDSA / SHA384withECDSA]");
    return false;
}

void SignProvider::CheckSignAlignment()
{
    if (signParams.find(ParamConstants::PARAM_BASIC_ALIGNMENT) == signParams.end()) {
        signParams.insert(std::make_pair(ParamConstants::PARAM_BASIC_ALIGNMENT, ParamConstants::ALIGNMENT));
    }
}

bool SignProvider::CheckCompatibleVersion()
{
    auto it = signParams.find(ParamConstants::PARAM_BASIC_COMPATIBLE_VERSION);
    if (it == signParams.end()) {
        signParams[ParamConstants::PARAM_BASIC_COMPATIBLE_VERSION] = "9";
        return true;
    }
    const std::string& compatibleApiVersionVal = it->second;
    int compatibleApiVersion;
    if (!StringUtils::CheckStringToint(compatibleApiVersionVal, compatibleApiVersion)) {
        PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                            "compatibleVersion Parameter is must integer");
        return false;
    }
    return true;
}

bool SignProvider::OutputSignedFile(RandomAccessFile* outputHap,
                                    long centralDirectoryOffset,
                                    ByteBuffer& signingBlock,
                                    ByteBufferDataSource* centralDirectory,
                                    ByteBuffer& eocdBuffer)
{
    std::shared_ptr<RandomAccessFileOutput> outputHapOut =
        std::make_shared<RandomAccessFileOutput>(outputHap, centralDirectoryOffset);
    if (!outputHapOut->Write(signingBlock)) {
        SIGNATURE_TOOLS_LOGE("output hap file write signingBlock failed");
        return false;
    }
    if (!outputHapOut->Write(centralDirectory->GetByteBuffer())) {
        SIGNATURE_TOOLS_LOGE("output hap file write central directory failed");
        return false;
    }
    if (!outputHapOut->Write(eocdBuffer) != 0) {
        SIGNATURE_TOOLS_LOGE("output hap file write eocd failed");
        return false;
    }
    return true;
}

X509* SignProvider::GetCertificate(const std::string& certificate)const
{
    BIO* in = BIO_new_mem_buf(certificate.data(), certificate.size());
    if (!in) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "bio new error ,get ceritificate from base64 certificate failed");
        return NULL;
    }
    X509* cert = NULL;
    cert = PEM_read_bio_X509(in, NULL, NULL, NULL);
    if (!cert) {
        BIO_free(in);
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "read no cert from base64 certificate failed");
        return NULL;
    }
    BIO_free(in);
    return cert;
}

std::string SignProvider::GetCertificateCN(X509* cert)const
{
    X509_NAME* name = NULL;
    int len = 0;
    std::string ret;
    if (cert == NULL) {
        return "";
    }
    name = X509_get_subject_name(cert);
    if (!name) {
        SIGNATURE_TOOLS_LOGE("name is NULL,get X509_NAME from cert failed");
    }
    len = X509_NAME_get_text_by_NID(name, NID_countryName, NULL, 0);
    if (len <= 0) {
        return "";
    }
    ret.resize(len + 1);
    if (X509_NAME_get_text_by_NID(name, NID_countryName, &ret[0], len + 1) != len) {
        return "";
    }
    return ret;
}

std::string SignProvider::FindProfileFromOptionalBlocks()const
{
    std::string profile;
    for (const OptionalBlock& optionalBlock : optionalBlocks) {
        if (optionalBlock.optionalType == HapUtils::HAP_PROFILE_BLOCK_ID) {
            profile = std::string(optionalBlock.optionalBlockValue.GetBufferPtr(),
                                  optionalBlock.optionalBlockValue.GetCapacity());
        }
    }
    return profile;
}

int SignProvider::CheckProfileValid(STACK_OF(X509)* inputCerts)
{
    std::string profile = FindProfileFromOptionalBlocks();
    std::map<std::string, std::string>::const_iterator ite =
        signParams.find(ParamConstants::PARAM_BASIC_PROFILE_SIGNED);
    if (ite == signParams.end()) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "find PARAM_BASIC_PROFILE_SIGNED failed");
        return INVALIDPARAM_ERROR;
    }
    bool isProfileWithoutSign = (ParamConstants::DISABLE_SIGN_CODE == ite->second);
    if (!isProfileWithoutSign) {
        PKCS7Data p7Data;
        if (p7Data.Parse(profile) < 0) {
            SIGNATURE_TOOLS_LOGE("Parse profile error.");
            return PARSE_ERROR;
        }
        if (p7Data.Verify() < 0) {
            SIGNATURE_TOOLS_LOGE("Verify profile pkcs7 failed! Profile is invalid.");
            return VERIFY_ERROR;
        }
        profileContent.clear();
        if (p7Data.GetContent(profileContent) < 0) {
            SIGNATURE_TOOLS_LOGE("get content data failed");
            return  INVALIDPARAM_ERROR;
        }
    } else {
        profileContent = profile;
    }

    ProfileInfo info;
    if (ParseProvision(profileContent, info) != PROVISION_OK) {
        SIGNATURE_TOOLS_LOGE("parse provision error");
        return PARSE_ERROR;
    }
    if (CheckProfileInfo(info, inputCerts) < 0) {
        SIGNATURE_TOOLS_LOGE("Check Profile Info error");
        return RET_FAILED;
    }
    return 0;
}

int SignProvider::CheckProfileInfo(const ProfileInfo& info, STACK_OF(X509)* inputCerts)const
{
    X509* certInProfile = NULL;
    if (info.type == ProvisionType::RELEASE) {
        certInProfile = GetCertificate(info.bundleInfo.distributionCertificate);
    } else if (info.type == ProvisionType::DEBUG) {
        certInProfile = GetCertificate(info.bundleInfo.developmentCertificate);
    } else {
        PrintErrorNumberMsg("NOT_SUPPORT_ERROR", NOT_SUPPORT_ERROR, "Unsupported profile type!");
        return NOT_SUPPORT_ERROR;
    }
    if (sk_X509_num(inputCerts) > 0 && !CheckInputCertMatchWithProfile(sk_X509_value(inputCerts, 0),
                                                                       certInProfile)) {
        X509_free(certInProfile);
        SIGNATURE_TOOLS_LOGE("input certificates do not match with profile!");
        return RET_FAILED;
    }
    std::string cn = GetCertificateCN(certInProfile);
    X509_free(certInProfile);
    SIGNATURE_TOOLS_LOGI("certificate in profile: %s", cn.c_str());
    if (cn.empty()) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR, "Common name of certificate is empty!");
        return CERTIFICATE_ERROR;
    }
    return 0;
}

bool SignProvider::CheckInputCertMatchWithProfile(X509* inputCert, X509* certInProfile)const
{
    return true;
}
} // namespace SignatureTools
} // namespace OHOS