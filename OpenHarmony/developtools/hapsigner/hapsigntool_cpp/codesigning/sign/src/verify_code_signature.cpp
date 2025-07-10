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
#include "verify_code_signature.h"
#include "elf_sign_block.h"
#include "constant.h"

namespace OHOS {
namespace SignatureTools {
bool VerifyCodeSignature::VerifyHap(std::string file, int64_t offset, int64_t length,
                                    std::string fileFormat, std::string profileContent)
{
    std::string fileFromatTmp = fileFormat;
    std::transform(fileFromatTmp.begin(), fileFromatTmp.end(), fileFromatTmp.begin(), ::tolower);
    if (std::find(CodeSigning::SUPPORT_FILE_FORM.begin(), CodeSigning::SUPPORT_FILE_FORM.end(),
        fileFromatTmp) == CodeSigning::SUPPORT_FILE_FORM.end()) {
        SIGNATURE_TOOLS_LOGI("Not hap, hsp or hqf file, skip code signing verify, file type: %s",
            fileFromatTmp.c_str());
        return true;
    }
    // 1) generate CodeSignBlock
    std::pair<std::string, std::string> pairResult = HapUtils::ParseAppIdentifier(profileContent);
    CodeSignBlock csb;
    bool generateFlag = GenerateCodeSignBlock(file, offset, length, csb);
    if (!generateFlag) {
        SIGNATURE_TOOLS_LOGE("VerifyHap GenerateCodeSignBlock failed, file: %s", file.c_str());
        return false;
    }
    // 2) verify hap
    bool verifyFlag = VerifyCodeSign(file, pairResult, csb);
    if (!verifyFlag) {
        SIGNATURE_TOOLS_LOGE("VerifyHap VerifyCodeSign failed");
        return false;
    }
    return true;
}

bool VerifyCodeSignature::VerifyElf(std::string file, int64_t offset, int64_t length,
                                    std::string fileFormat, std::string profileContent)
{
    std::transform(fileFormat.begin(), fileFormat.end(), fileFormat.begin(), ::tolower);
    if (ELF != fileFormat) {
        SIGNATURE_TOOLS_LOGI("Not elf file, skip code signing verify, file type: %s", fileFormat.c_str());
        return true;
    }
    // 1) parse sign block to ElfSignBlock object
    std::ifstream signedElf(file, std::ios::binary);
    if (!signedElf.is_open()) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "open file: " + file + "failed");
        return false;
    }
    signedElf.seekg(offset, std::ios::beg);
    std::vector<char> codeSignBlockBytes(length);
    signedElf.read(codeSignBlockBytes.data(), codeSignBlockBytes.size());
    signedElf.close();
    ElfSignBlock elfSignBlock;
    bool arrFlag = ElfSignBlock::FromByteArray(*(std::vector<int8_t> *) & codeSignBlockBytes, elfSignBlock);
    if (!arrFlag) {
        SIGNATURE_TOOLS_LOGE("parse sign block to ElfCodeSignBlock object failed");
        return false;
    }
    // 2) verify file data
    int32_t paddingSize = ElfSignBlock::ComputeMerkleTreePaddingLength(offset);
    std::vector<int8_t> &merkleTreeWithPadding = elfSignBlock.GetMerkleTreeWithPadding();
    std::vector<int8_t> merkleTree;
    merkleTree.insert(merkleTree.end(), merkleTreeWithPadding.begin() + paddingSize, merkleTreeWithPadding.end());
    std::ifstream elf(file, std::ios::binary);
    bool verifySingleFileFlag = VerifySingleFile(elf, elfSignBlock.GetDataSize(), elfSignBlock.GetSignature(),
        elfSignBlock.GetTreeOffset(), merkleTree);
    if (!verifySingleFileFlag) {
        SIGNATURE_TOOLS_LOGE("verify elf file data failed");
        elf.close();
        return false;
    }
    elf.close();
    // 3) check ownerID
    if (!profileContent.empty()) {
        std::pair<std::string, std::string> pairResult = HapUtils::ParseAppIdentifier(profileContent);
        std::vector<int8_t>& signature = elfSignBlock.GetSignature();
        std::string signatureStr(signature.begin(), signature.end());
        bool checkOwnerIDFlag = CmsUtils::CheckOwnerID(signatureStr, pairResult.first, pairResult.second);
        if (!checkOwnerIDFlag) {
            SIGNATURE_TOOLS_LOGE("elf check owner id failed");
            return false;
        }
    }
    return true;
}

bool VerifyCodeSignature::VerifyNativeLib(CodeSignBlock& csb, std::string& file, unzFile& zFile,
                                          std::pair<std::string, std::string>& pairResult)
{
    UnzipHandleParam param(csb, pairResult, false);
    CodeSigning codeSigning;
    bool nativeFlag = codeSigning.GetNativeEntriesFromHap(file, param);
    if (!nativeFlag) {
        SIGNATURE_TOOLS_LOGE("verify native libs failed, file: %s", file.c_str());
        return false;
    }
    return true;
}

bool VerifyCodeSignature::VerifyCodeSign(std::string file, std::pair<std::string, std::string>& pairResult,
                                         CodeSignBlock& csb)
{
    // 1) verify hap
    std::ifstream hap(file, std::ios::binary);
    if (!hap.is_open()) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "open file: " + file + "failed");
        return false;
    }
    int64_t dataSize = csb.GetHapInfoSegment().GetSignInfo().GetDataSize();
    std::vector<int8_t> signature = csb.GetHapInfoSegment().GetSignInfo().GetSignature();
    Extension* ext = csb.GetHapInfoSegment().GetSignInfo()
        .GetExtensionByType(MerkleTreeExtension::MERKLE_TREE_INLINED);
    MerkleTreeExtension* mte = new MerkleTreeExtension(0, 0, std::vector<int8_t>());
    if (mte == nullptr) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "system failed to allocate memory for MerkleTreeExtension");
        return false;
    }
    if (ext != nullptr) {
        delete mte;
        mte = static_cast<MerkleTreeExtension*>(ext);
    } else {
        std::shared_ptr<MerkleTreeExtension> merkleTreeExt(mte);
    }
    // temporary: merkle tree offset set to zero, change to merkleTreeOffset
    bool verifyFlag = VerifySingleFile(hap, dataSize, signature, mte->GetMerkleTreeOffset(),
        csb.GetOneMerkleTreeByFileName(CodeSigning::HAP_SIGNATURE_ENTRY_NAME));
    if (!verifyFlag) {
        return false;
    }
    hap.close();
    std::string signature_(signature.begin(), signature.end());
    bool checkOwnerIDFlag = CmsUtils::CheckOwnerID(signature_, pairResult.first, pairResult.second);
    if (!checkOwnerIDFlag) {
        return false;
    }
    // 2) verify native libs
    unzFile zFile = unzOpen(file.c_str());
    if (zFile == NULL) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "unzOpen file: " + file + "failed");
        return false;
    }
    bool verifyNativeLibFlag = VerifyNativeLib(csb, file, zFile, pairResult);
    if (!verifyNativeLibFlag) {
        return false;
    }
    unzClose(zFile);
    return true;
}

bool VerifyCodeSignature::VerifySingleFile(std::istream& input, int64_t length,
                                           std::vector<int8_t> signature,
                                           int64_t merkleTreeOffset,
                                           std::vector<int8_t> inMerkleTreeBytes)
{
    std::pair<std::vector<int8_t>, std::vector<int8_t>> pairResult
        = GenerateFsVerityDigest(input, length, merkleTreeOffset);
    std::vector<int8_t> generatedMerkleTreeBytes = pairResult.second;
    // For native libs, inMerkleTreeBytes is null, skip check here
    bool verifyFlag = (!inMerkleTreeBytes.empty()) && !AreVectorsEqual(inMerkleTreeBytes, generatedMerkleTreeBytes);
    if (verifyFlag) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "verify merkle tree bytes failed");
        return false;
    }
    return CmsUtils::VerifySignDataWithUnsignedDataDigest(pairResult.first, signature);
}

bool VerifyCodeSignature::GenerateCodeSignBlock(const std::string& file, int64_t offset,
                                                int64_t length, CodeSignBlock& csb)
{
    // 1) parse sign block to CodeSignBlock object
    std::ifstream signedHap(file, std::ios::binary);
    if (!signedHap.is_open()) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "open file: " + file + "failed");
        return false;
    }
    int32_t fileReadOffset = 0;
    // 1.1) skip data part, but fileReadOffset remains at start(0)
    signedHap.seekg(offset, std::ios::beg);
    // 1.2) parse codeSignBlockHeader
    std::vector<char> codeSignBlockHeaderByteArray(CodeSignBlockHeader::Size());
    signedHap.read(codeSignBlockHeaderByteArray.data(), codeSignBlockHeaderByteArray.size());
    fileReadOffset += signedHap.gcount();
    CodeSignBlockHeader* pCodeSignBlockHeader =
        CodeSignBlockHeader::FromByteArray(*(std::vector<int8_t>*) &codeSignBlockHeaderByteArray);
    if (pCodeSignBlockHeader == nullptr) {
        SIGNATURE_TOOLS_LOGE("Invalid code Sign block header");
        signedHap.close();
        return false;
    }
    std::shared_ptr<CodeSignBlockHeader> codeSignBlockHeader(pCodeSignBlockHeader);
    csb.SetCodeSignBlockHeader(*codeSignBlockHeader);
    int64_t headerBlockSize = csb.GetCodeSignBlockHeader().GetBlockSize();
    if (headerBlockSize != length) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
            "Invalid code Sign block size of setCodeSignBlockHeader");
        signedHap.close();
        return false;
    }
    // 2) parse segment headers
    for (int i = 0; i < csb.GetCodeSignBlockHeader().GetSegmentNum(); i++) {
        std::vector<char> segmentHeaderByteArray(SegmentHeader::SEGMENT_HEADER_LENGTH);
        signedHap.read(segmentHeaderByteArray.data(), segmentHeaderByteArray.size());
        fileReadOffset += signedHap.gcount();
        csb.AddToSegmentList(*SegmentHeader::FromByteArray(*(std::vector<int8_t> *) &segmentHeaderByteArray));
    }
    // compute merkle tree offset by alignment, based on file start
    int64_t computedTreeOffset = GetAlignmentAddr(CodeSignBlock::PAGE_SIZE_4K, fileReadOffset + offset);
    // skip zero padding before merkle tree, adds zero padding length to fileReadOffset
    signedHap.seekg(computedTreeOffset - offset - fileReadOffset, std::ios::cur);
    fileReadOffset += computedTreeOffset - offset - fileReadOffset;
    bool parseMerkleTreeFlag = ParseMerkleTree(csb, fileReadOffset, signedHap, computedTreeOffset);
    if (!parseMerkleTreeFlag) {
        SIGNATURE_TOOLS_LOGE("GenerateCodeSignBlock parse merkle tree failed.");
        return false;
    }
    signedHap.close();
    return true;
}

bool VerifyCodeSignature::ParseSegmentHead(CodeSignBlock& csb, std::ifstream& signedHap,
    std::vector<char> &merkleTreeBytes, int32_t &fileReadOffset)
{
    for (auto& segmentHeader : csb.GetSegmentHeaderList()) {
        if (fileReadOffset > segmentHeader.GetSegmentOffset()) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "Invaild offset of merkle tree and segment header");
            return false;
        }
        // get merkle tree bytes
        if (fileReadOffset < segmentHeader.GetSegmentOffset()) {
            merkleTreeBytes.resize(segmentHeader.GetSegmentOffset() - fileReadOffset);
            signedHap.read(merkleTreeBytes.data(), merkleTreeBytes.size());
            fileReadOffset += signedHap.gcount();
        }
        std::vector<char> sh(segmentHeader.GetSegmentSize(), 0);
        signedHap.read(sh.data(), sh.size());
        fileReadOffset += signedHap.gcount();
        int32_t type = segmentHeader.GetType();
        if (type == SegmentHeader::CSB_FSVERITY_INFO_SEG) {
            // 3) parse fs-verity info segment
            FsVerityInfoSegment fsVerityInfoSegment =
                FsVerityInfoSegment::FromByteArray(*(std::vector<int8_t> *) & sh);
            csb.SetFsVerityInfoSegment(fsVerityInfoSegment);
        } else if (type == SegmentHeader::CSB_HAP_META_SEG) {
            // 4) parse hap info segment
            HapInfoSegment hapInfoSegment = HapInfoSegment::FromByteArray(*(std::vector<int8_t> *) & sh);
            csb.SetHapInfoSegment(hapInfoSegment);
        } else if (type == SegmentHeader::CSB_NATIVE_LIB_INFO_SEG) {
            // 5) parse so info segment
            NativeLibInfoSegment nativeLibInfoSegment =
                NativeLibInfoSegment::FromByteArray(*(std::vector<int8_t> *) & sh);
            csb.SetSoInfoSegment(nativeLibInfoSegment);
        }
    }

    return true;
}

bool VerifyCodeSignature::ParseMerkleTree(CodeSignBlock& csb, int32_t readOffset, std::ifstream& signedHap,
                                          int64_t computedTreeOffset)
{
    std::vector<char> merkleTreeBytes;
    int32_t fileReadOffset = readOffset;
    bool parseFlag = ParseSegmentHead(csb, signedHap, merkleTreeBytes, fileReadOffset);
    if (!parseFlag) {
        SIGNATURE_TOOLS_LOGE("ParsesegmentHead failed");
        return false;
    }
    int32_t blockSize = csb.GetCodeSignBlockHeader().GetBlockSize();
    if (fileReadOffset != blockSize) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
            "Invalid blockSize of getCodeSignBlockHeader");
        return false;
    }
    // parse merkle tree
    Extension* extension = csb.GetHapInfoSegment().GetSignInfo().
        GetExtensionByType(MerkleTreeExtension::MERKLE_TREE_INLINED);
    if (!extension) {
        SIGNATURE_TOOLS_LOGE("Missing merkleTreeExtension in verifycation");
        return false;
    }
    MerkleTreeExtension* mte = static_cast<MerkleTreeExtension*>(extension);
    if (mte != nullptr) {
        bool merkleTreeFlag = computedTreeOffset != mte->GetMerkleTreeOffset() ||
            merkleTreeBytes.size() != mte->GetMerkleTreeSize();
        if (merkleTreeFlag) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "Invalid merkle tree offset or tree size");
            return false;
        }
        csb.AddOneMerkleTree(CodeSigning::HAP_SIGNATURE_ENTRY_NAME, *(std::vector<int8_t> *) &merkleTreeBytes);
    }
    return true;
}

int64_t VerifyCodeSignature::GetAlignmentAddr(int64_t alignment, int64_t input)
{
    if (alignment == 0) {
        return input;
    }
    int64_t residual = input % alignment;
    if (residual == 0) {
        return input;
    } else {
        return input + (alignment - residual);
    }
}

std::pair<std::vector<int8_t>, std::vector<int8_t>> VerifyCodeSignature::GenerateFsVerityDigest(
    std::istream& inputStream, int64_t size, int64_t merkleTreeOffset)
{
    std::unique_ptr<FsVerityGenerator> fsVerityGenerator = std::make_unique<FsVerityGenerator>();
    fsVerityGenerator->GenerateFsVerityDigest(inputStream, size, merkleTreeOffset);
    return std::make_pair(fsVerityGenerator->GetFsVerityDigest(), fsVerityGenerator->GetTreeBytes());
}

bool VerifyCodeSignature::AreVectorsEqual(const std::vector<int8_t>& vec1, const std::vector<int8_t>& vec2)
{
    if (vec1.size() != vec2.size()) {
        return false;
    }
    for (int64_t i = 0; i < vec1.size(); i++) {
        if (vec1[i] != vec2[i]) {
            return false;
        }
    }
    return true;
}
} // namespace SignatureTools
} // namespace OHOS