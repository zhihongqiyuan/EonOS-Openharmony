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

#include <algorithm>

#include "elf_sign_block.h"
#include "fs_verity_descriptor.h"
#include "fs_verity_descriptor_with_sign.h"
#include "verify_code_signature.h"
#include "code_signing.h"

namespace OHOS {
namespace SignatureTools {
const int BUFFER_SIZE = 16 * 1024;
const int FILE_NAME_SIZE = 512;
const std::vector<std::string> CodeSigning::SUPPORT_FILE_FORM = { "hap", "hsp", "hqf" };
const std::string CodeSigning::HAP_SIGNATURE_ENTRY_NAME = "Hap";
const std::string CodeSigning::ENABLE_SIGN_CODE_VALUE = "1";
const std::string CodeSigning::LIBS_PATH_PREFIX = "libs/";

const FsVerityHashAlgorithm FS_SHA256(1, "SHA-256", 256 / 8);
const FsVerityHashAlgorithm FS_SHA512(2, "SHA-512", 512 / 8);
const int8_t LOG_2_OF_FSVERITY_HASH_PAGE_SIZE = 12;

CodeSigning::CodeSigning(SignerConfig* signConfig) : mPools(new Uscript::ThreadPool(POOL_SIZE))
{
    m_signConfig = signConfig;
}

CodeSigning::CodeSigning() : mPools(new Uscript::ThreadPool(POOL_SIZE))
{
}

bool CodeSigning::GetCodeSignBlock(const std::string &input, int64_t offset,
                                   const std::string &inForm, const std::string &profileContent,
                                   ZipSigner& zip, std::vector<int8_t>& ret)
{
    SIGNATURE_TOOLS_LOGI("Start to sign code.");
    bool flag = std::find(SUPPORT_FILE_FORM.begin(), SUPPORT_FILE_FORM.end(), inForm) == SUPPORT_FILE_FORM.end();
    if (flag) {
        SIGNATURE_TOOLS_LOGE("only support format is [hap, hqf, hsp, app]");
        return false;
    }
    int64_t dataSize = ComputeDataSize(zip);
    if (dataSize < 0) {
        SIGNATURE_TOOLS_LOGE("SignFile Failed because dataSize is invalid");
        return false;
    }
    m_timestamp = GetTimestamp();
    int64_t fsvTreeOffset = m_codeSignBlock.ComputeMerkleTreeOffset(offset);
    std::unique_ptr<FsVerityInfoSegment> fsVerityInfoSegment =
        std::make_unique<FsVerityInfoSegment>((int8_t)FsVerityDescriptor::VERSION,
                                              (int8_t)FsVerityGenerator::GetFsVerityHashAlgorithm(),
                                              (int8_t)FsVerityGenerator::GetLog2BlockSize());
    m_codeSignBlock.SetFsVerityInfoSegment(*(fsVerityInfoSegment.get()));
    SIGNATURE_TOOLS_LOGI("Sign hap.");
    std::string ownerID = HapUtils::GetAppIdentifier(profileContent);
    std::ifstream inputStream;
    inputStream.open(input, std::ios::binary);
    if (!inputStream.is_open()) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "open file: " + input + "failed");
        inputStream.close();
        return false;
    }
    std::pair<SignInfo, std::vector<int8_t>> hapSignInfoAndMerkleTreeBytesPair;
    bool signFileFlag = SignFile(inputStream, dataSize, true, fsvTreeOffset, ownerID,
                                 hapSignInfoAndMerkleTreeBytesPair);
    if (!signFileFlag) {
        SIGNATURE_TOOLS_LOGE("SignFile Failed");
        inputStream.close();
        return false;
    }
    inputStream.close();
    m_codeSignBlock.GetHapInfoSegment().SetSignInfo(hapSignInfoAndMerkleTreeBytesPair.first);
    m_codeSignBlock.AddOneMerkleTree(HAP_SIGNATURE_ENTRY_NAME,
                                     hapSignInfoAndMerkleTreeBytesPair.second);
    if (!SignNativeLibs(input, ownerID)) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Failed to sign the contents in the compressed file.");
        return false;
    }
    UpdateCodeSignBlock();
    m_codeSignBlock.GenerateCodeSignBlockByte(fsvTreeOffset, ret);
    SIGNATURE_TOOLS_LOGI("Sign successfully.");
    return true;
}

int64_t CodeSigning::ComputeDataSize(ZipSigner& zip)
{
    uint32_t dataSize = 0L;
    for (const auto& entry : zip.GetZipEntries()) {
        ZipEntryHeader* zipEntryHeader = entry->GetZipEntryData()->GetZipEntryHeader();
        bool runnableFileFlag = FileUtils::IsRunnableFile(zipEntryHeader->GetFileName())
            && zipEntryHeader->GetMethod() == ZipSigner::FILE_UNCOMPRESS_METHOD_FLAG;
        if (runnableFileFlag) {
            continue;
        }
        // if the first file is not uncompressed abc or so, set dataSize to zero
        if (entry->GetCentralDirectory()->GetOffset() == 0) {
            break;
        }
        // the first entry which is not abc/so/an is found, return its data offset
        dataSize = entry->GetCentralDirectory()->GetOffset() + ZipEntryHeader::HEADER_LENGTH
            + zipEntryHeader->GetFileNameLength() + zipEntryHeader->GetExtraLength();
        break;
    }
    if ((dataSize % CodeSignBlock::PAGE_SIZE_4K) != 0) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
                            "Invalid dataSize, the dataSize must be an integer multiple of 4096");
        return -1;
    }
    int64_t dataSizeInt64 = static_cast<int64_t>(dataSize);
    return dataSizeInt64;
}

int64_t CodeSigning::GetTimestamp()
{
    auto now = std::chrono::system_clock::now();
    auto nowSeconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
    return nowSeconds.time_since_epoch().count();
}

bool CodeSigning::SignFile(std::istream& inputStream, int64_t fileSize, bool storeTree,
                           int64_t fsvTreeOffset, const std::string &ownerID,
                           std::pair<SignInfo, std::vector<int8_t>>& ret)
{
    std::unique_ptr<FsVerityGenerator> fsVerityGenerator =
        std::make_unique<FsVerityGenerator>();
    if (!fsVerityGenerator->GenerateFsVerityDigest(inputStream, fileSize, fsvTreeOffset)) {
        SIGNATURE_TOOLS_LOGE("SignFile GenerateFsVerityDigest Fail");
        return false;
    }
    std::vector<int8_t> fsVerityDigest = fsVerityGenerator->GetFsVerityDigest();
    std::vector<int8_t> signature;
    bool generateSignatureFlag = GenerateSignature(fsVerityDigest, ownerID, signature);
    if (!generateSignatureFlag) {
        SIGNATURE_TOOLS_LOGE("SignFile GenerateSignature Fail");
        return false;
    }
    int flags = 0;
    if (storeTree) {
        flags = SignInfo::FLAG_MERKLE_TREE_INCLUDED;
    }
    int saltSize = fsVerityGenerator->GetSaltSize();
    std::vector<int8_t> salt = fsVerityGenerator->GetSalt();
    SignInfo signInfo(saltSize, flags, fileSize, salt, signature);
    // if store merkle tree in sign info
    if (storeTree) {
        int merkleTreeSize = fsVerityGenerator->GetTreeBytes().empty() ? 0
            : fsVerityGenerator->GetTreeBytes().size();
        MerkleTreeExtension* merkleTreeExtension = new MerkleTreeExtension(merkleTreeSize,
                                                                           fsvTreeOffset,
                                                                           fsVerityGenerator->GetRootHash());
        if (merkleTreeExtension == nullptr) {
            PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "system failed to allocate memory for MerkleTreeExtension");
            return false;
        }
        signInfo.AddExtension(merkleTreeExtension);
    }
    ret = std::make_pair(signInfo, fsVerityGenerator->GetTreeBytes());
    return true;
}

bool CodeSigning::GetElfCodeSignBlock(const std::string &input, int64_t offset,
                                      const std::string &inForm, const std::string &profileContent,
                                      std::vector<int8_t>& codesignData)
{
    SIGNATURE_TOOLS_LOGI("Start to sign elf code.");
    int paddingSize = ElfSignBlock::ComputeMerkleTreePaddingLength(offset);
    int64_t fsvTreeOffset = offset + FsVerityDescriptorWithSign::INTEGER_BYTES * 2 + paddingSize;
    std::ifstream inputstream(input, std::ios::binary | std::ios::ate);
    if (!inputstream.is_open()) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "open file: " + input + "failed");
        return false;
    }
    std::streamsize fileSize = inputstream.tellg();
    inputstream.seekg(0, std::ios::beg);
    std::unique_ptr<FsVerityGenerator> fsVerityGenerator = std::make_unique<FsVerityGenerator>();
    fsVerityGenerator->GenerateFsVerityDigest(inputstream, fileSize, fsvTreeOffset);
    std::vector<int8_t> fsVerityDigest = fsVerityGenerator->GetFsVerityDigest();
    std::string ownerID = profileContent.empty() ? "DEBUF_LIB_ID" : HapUtils::GetAppIdentifier(profileContent);
    std::vector<int8_t> signature;
    bool generateSignatureFlag = GenerateSignature(fsVerityDigest, ownerID, signature);
    if (!generateSignatureFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf] generate elf signature failed");
        return false;
    }

    FsVerityDescriptor::Builder fsdbuilder;
    fsdbuilder.SetFileSize(fileSize)
              .SetHashAlgorithm(FS_SHA256.GetId())
              .SetLog2BlockSize(LOG_2_OF_FSVERITY_HASH_PAGE_SIZE)
              .SetSaltSize(fsVerityGenerator->GetSaltSize())
              .SetSignSize(signature.size())
              .SetFileSize(fileSize)
              .SetSalt(fsVerityGenerator->Getsalt())
              .SetRawRootHash(fsVerityGenerator->GetRootHash())
              .SetFlags(FsVerityDescriptor::FLAG_STORE_MERKLE_TREE_OFFSET)
              .SetMerkleTreeOffset(fsvTreeOffset)
              .SetCsVersion(FsVerityDescriptor::CODE_SIGN_VERSION);

    FsVerityDescriptorWithSign fsVerityDescriptorWithSign =
        FsVerityDescriptorWithSign(FsVerityDescriptor(fsdbuilder), signature);
    std::vector<int8_t> treeBytes = fsVerityGenerator->GetTreeBytes();
    ElfSignBlock signBlock = ElfSignBlock(paddingSize, treeBytes, fsVerityDescriptorWithSign);
    signBlock.ToByteArray(codesignData);
    return true;
}

bool CodeSigning::SignNativeLibs(const std::string &input, std::string &ownerID)
{
    // sign native files
    std::vector<std::pair<std::string, SignInfo>> ret;
    UnzipHandleParam param(ret, ownerID, true);
    bool nativeLibflag = GetNativeEntriesFromHap(input, param);
    if (!nativeLibflag) {
        SIGNATURE_TOOLS_LOGE("%s native libs handle failed", input.c_str());
        return false;
    }
    std::vector<std::pair<std::string, SignInfo>>& nativeLibInfoList = param.GetRet();
    if (nativeLibInfoList.empty()) {
        SIGNATURE_TOOLS_LOGI("No native libs.");
        return true;
    }
    m_codeSignBlock.GetSoInfoSegment().SetSoInfoList(nativeLibInfoList);
    return true;
}

void CodeSigning::UpdateCodeSignBlock()
{
    // construct segment header list
    m_codeSignBlock.SetSegmentHeaders();
    // Compute and set segment number
    m_codeSignBlock.SetSegmentNum();
    // update code sign block header flag
    m_codeSignBlock.SetCodeSignBlockFlag();
    // compute segment offset
    m_codeSignBlock.ComputeSegmentOffset();
}

bool CodeSigning::GetNativeEntriesFromHap(const std::string& packageName, UnzipHandleParam& param)
{
    unzFile zFile = unzOpen(packageName.c_str());
    if (zFile == NULL) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "zlib open file: " + packageName + " failed.");
        return false;
    }
    // get zipFile all paramets
    unz_global_info zGlobalInfo;
    int getRet = unzGetGlobalInfo(zFile, &zGlobalInfo);
    if (getRet != UNZ_OK) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "zlib get global info failed.");
        unzClose(zFile);
        return false;
    }
    // search each file
    bool handleFlag = HandleZipGlobalInfo(packageName, zFile, zGlobalInfo, param);
    if (!handleFlag) {
        unzClose(zFile);
        return false;
    }

    unzClose(zFile);
    return true;
}

bool CodeSigning::GetSingleFileStreamFromZip(unzFile &zFile, char fileName[],
                                             unz_file_info &zFileInfo,
                                             int &readFileSize, std::stringbuf &sb)
{
    if (UNZ_OK != unzOpenCurrentFile(zFile)) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
                            "unzOpenCurrentFile zipFile error");
        return false;
    }

    char szReadBuffer[BUFFER_SIZE] = { 0 };
    long fileLength = zFileInfo.uncompressed_size;
    int nReadFileSize;
    do {
        nReadFileSize = 0;
        if (memset_s(szReadBuffer, BUFFER_SIZE, 0, BUFFER_SIZE) != EOK) {
            SIGNATURE_TOOLS_LOGE("memset_s failed");
            unzCloseCurrentFile(zFile);
            return false;
        }
        nReadFileSize = unzReadCurrentFile(zFile, szReadBuffer, BUFFER_SIZE);
        if (nReadFileSize > 0) {
            sb.sputn(szReadBuffer, nReadFileSize);
        }
        fileLength -= nReadFileSize;
        readFileSize += nReadFileSize;
    } while (fileLength > 0 && nReadFileSize > 0);
    if (fileLength) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "zlib read stream from "
                            + std::string(fileName) + " failed.");
        unzCloseCurrentFile(zFile);
        return false;
    }

    unzCloseCurrentFile(zFile);
    return true;
}

bool CodeSigning::RunParseZipInfo(const std::string& packageName, UnzipHandleParam& param, uLong index)
{
    unzFile zFile = unzOpen(packageName.c_str());
    if (zFile == NULL) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "zlib open file: " + packageName + " failed.");
        return false;
    }
    for (uLong i = 0; i < index; ++i) {
        int ret = unzGoToNextFile(zFile);
        if (ret != UNZ_OK) {
            PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "zlib go to next file failed.");
            unzClose(zFile);
            return false;
        }
    }
    unz_file_info zFileInfo;
    char fileName[FILE_NAME_SIZE];
    int readFileSize = 0;
    std::stringbuf sb;
    if (memset_s(fileName, FILE_NAME_SIZE, 0, FILE_NAME_SIZE) != 0) {
        SIGNATURE_TOOLS_LOGE("memory of fileName memset_s failed");
        unzClose(zFile);
        return false;
    }
    size_t nameLen = 0;
    if (!CheckUnzParam(zFile, zFileInfo, fileName, &nameLen)) {
        unzClose(zFile);
        return false;
    }
    if (!CheckFileName(fileName, &nameLen)) {
        unzClose(zFile);
        return true;
    }
    bool flag = GetSingleFileStreamFromZip(zFile, fileName, zFileInfo, readFileSize, sb);
    unzClose(zFile);
    if (!flag) {
        return false;
    }
    bool handleFlag = DoNativeLibSignOrVerify(std::string(fileName), sb, param, readFileSize);
    if (!handleFlag) {
        SIGNATURE_TOOLS_LOGE("%s native libs handle failed", fileName);
        return false;
    }
    return true;
}

bool CodeSigning::HandleZipGlobalInfo(const std::string& packageName, unzFile& zFile,
                                      unz_global_info& zGlobalInfo, UnzipHandleParam& param)
{
    std::vector<std::future<bool>> thread_results;
    SIGNATURE_TOOLS_LOGI("zGlobalInfo.number_entry = %lu", zGlobalInfo.number_entry);

    for (uLong i = 0; i < zGlobalInfo.number_entry; ++i) {
        unz_file_info zFileInfo;
        char fileName[FILE_NAME_SIZE];
        size_t nameLen = 0;
        if (memset_s(fileName, FILE_NAME_SIZE, 0, FILE_NAME_SIZE) != 0) {
            SIGNATURE_TOOLS_LOGE("memory of fileName memset_s failed");
            return false;
        }
        if (!CheckUnzParam(zFile, zFileInfo, fileName, &nameLen)) {
            return false;
        }
        if (CheckFileName(fileName, &nameLen)) {
            thread_results.push_back(mPools->Enqueue(&CodeSigning::RunParseZipInfo, this,
                std::ref(packageName), std::ref(param), i));
        }
        if (i != zGlobalInfo.number_entry - 1) {
            int ret = unzGoToNextFile(zFile);
            if (ret != UNZ_OK) {
                PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "zlib go to next file failed.");
                return false;
            }
        }
    }

    bool result = true;
    for (auto& thread_result : thread_results) {
        if (!thread_result.get())
            result = false;
    }
    if (!result) {
        return false;
    }
    return true;
}

bool CodeSigning::DoNativeLibVerify(std::string fileName, std::stringbuf& sb,
                                    UnzipHandleParam& param, int readFileSize)
{
    std::istream input(&sb);
    CodeSignBlock csb = param.GetCodeSignBlock();
    std::vector<std::string>& fileNames = csb.GetSoInfoSegment().GetFileNameList();
    bool isContainFileName = std::find(fileNames.begin(), fileNames.end(), fileName) != fileNames.end();
    if (!isContainFileName) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "verify signed file position failed, file: " + fileName);
        return false;
    }
    for (int j = 0; j < csb.GetSoInfoSegment().GetSectionNum(); j++) {
        SignInfo signInfo = csb.GetSoInfoSegment().GetSignInfoList()[j];
        std::string entryName = csb.GetSoInfoSegment().GetFileNameList()[j];
        std::vector<int8_t> entrySig = signInfo.GetSignature();
        std::string entrySigStr(entrySig.begin(), entrySig.end());
        if (fileName != entryName) {
            continue;
        }
        if (readFileSize != signInfo.GetDataSize()) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "Invalid dataSize of native lib");
            return false;
        }
        bool verifyFlag = VerifyCodeSignature::VerifySingleFile(input, readFileSize, entrySig, 0,
                                                                std::vector<int8_t>());
        if (!verifyFlag) {
            return false;
        }
        std::ifstream* inputFile = (std::ifstream*)(&input);
        inputFile->close();
        std::pair<std::string, std::string> pairResult = param.GetPairResult();
        bool checkOwnerIDFlag = CmsUtils::CheckOwnerID(entrySigStr, pairResult.first, pairResult.second);
        if (!checkOwnerIDFlag) {
            return false;
        }
    }
    return true;
}

bool CodeSigning::DoNativeLibSignOrVerify(std::string fileName, std::stringbuf& sb,
                                          UnzipHandleParam& param, int readFileSize)
{
    bool isSign = param.IsSign();
    if (isSign) {
        std::istream input(&sb);
        std::string ownerID = param.GetOwnerID();
        std::pair<SignInfo, std::vector<int8_t>> pairSignInfoAndMerkleTreeBytes;
        bool signFileFlag = SignFile(input, readFileSize, false, 0, ownerID, pairSignInfoAndMerkleTreeBytes);
        if (!signFileFlag) {
            return false;
        }
        m_mutex.lock();
        std::vector<std::pair<std::string, SignInfo>> &ret = param.GetRet();
        ret.push_back(std::make_pair(fileName, pairSignInfoAndMerkleTreeBytes.first));
        m_mutex.unlock();
    } else {
        bool flag = DoNativeLibVerify(fileName, sb, param, readFileSize);
        if (!flag) {
            return false;
        }
    }
    return true;
}

bool CodeSigning::CheckUnzParam(unzFile& zFile, unz_file_info& zFileInfo,
                                char fileName[], size_t* nameLen)
{
    if (UNZ_OK != unzGetCurrentFileInfo(zFile, &zFileInfo, fileName,
        FILE_NAME_SIZE, NULL, 0, NULL, 0)) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
                            "unzip get file: " + std::string(fileName) + "info failed!");
        return false;
    }
    SIGNATURE_TOOLS_LOGI("Open zipFile filename is : %s", fileName);
    if ((*nameLen = strlen(fileName)) == 0U) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Open zipFile fileName is null");
        return false;
    }
    return true;
}

bool CodeSigning::CheckFileName(char fileName[], size_t* nameLen)
{
    if (fileName[*nameLen - 1] == '/') {
        SIGNATURE_TOOLS_LOGI("It is dictionary.");
        return false;
    }
    std::string str(fileName);
    bool nativeFileFlag = IsNativeFile(str);
    if (!nativeFileFlag) {
        SIGNATURE_TOOLS_LOGI("Suffix mismatched.");
        return false;
    }
    return true;
}

bool CodeSigning::IsNativeFile(const std::string& input)
{
    size_t dotPos = input.rfind('.');
    if (dotPos == std::string::npos) {
        return false;
    }
    std::string suffix = input.substr(dotPos + 1);
    if (suffix == "an") {
        return true;
    }
    std::string libDir = input.substr(0, LIBS_PATH_PREFIX.size());
    int ret = LIBS_PATH_PREFIX.compare(libDir);
    if (ret == 0) {
        return true;
    }
    return false;
}

bool CodeSigning::GenerateSignature(const std::vector<int8_t>& signedData, const std::string& ownerID,
                                    std::vector<int8_t>& ret)
{
    if (m_signConfig->GetSigner() != nullptr) {
        STACK_OF(X509)* certs = NULL;
        certs = m_signConfig->GetSigner()->GetCertificates();
        if (certs == nullptr) {
            PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
                                "No certificates configured for sign.");
            return false;
        }
        sk_X509_pop_free(certs, X509_free);
    } else {
        return false;
    }
    std::unique_ptr<BCSignedDataGenerator> bcSignedDataGenerator =
        std::make_unique<BCSignedDataGenerator>();
    if (!ownerID.empty()) {
        SIGNATURE_TOOLS_LOGW("generate signature get owner id not null.");
        bcSignedDataGenerator->SetOwnerId(ownerID);
    }
    std::string signed_data(signedData.begin(), signedData.end());
    std::string ret_str;
    if (signedData.empty()) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Generate verity digest is null");
        return false;
    }
    bool generateSignedDataFlag = bcSignedDataGenerator->GenerateSignedData(signed_data, m_signConfig, ret_str);
    if (generateSignedDataFlag) {
        SIGNATURE_TOOLS_LOGE("Generate signedData failed");
        return false;
    }
    ret = std::vector<int8_t>(ret_str.begin(), ret_str.end());
    return true;
}
} // namespace SignatureTools
} // namespace OHOS
