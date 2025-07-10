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
#include <filesystem>

#include "file_utils.h"
#include "zip_entry.h"
#include "zip_signer.h"

namespace OHOS {
namespace SignatureTools {
bool ZipSigner::Init(std::ifstream& inputFile)
{
    if (!inputFile.good()) {
        return false;
    }

    /* 1. get eocd data */
    m_endOfCentralDirectory = GetZipEndOfCentralDirectory(inputFile);
    if (!m_endOfCentralDirectory) {
        SIGNATURE_TOOLS_LOGE("get eocd data failed.");
        return false;
    }

    m_cDOffset = m_endOfCentralDirectory->GetOffset();

    /* 2. use eocd's cd offset, get cd data */
    if (!GetZipCentralDirectory(inputFile)) {
        SIGNATURE_TOOLS_LOGE("get zip central directory failed.");
        return false;
    }

    /* 3. use cd's entry offset and file size, get entry data */
    if (!GetZipEntries(inputFile)) {
        SIGNATURE_TOOLS_LOGE("get zip entries failed.");
        return false;
    }

    ZipEntry* endEntry = m_zipEntries[m_zipEntries.size() - 1];
    CentralDirectory* endCD = endEntry->GetCentralDirectory();
    ZipEntryData* endEntryData = endEntry->GetZipEntryData();
    m_signingOffset = endCD->GetOffset() + endEntryData->GetLength();

    /* 4. file all data - eocd - cd - entry = sign block */
    m_signingBlock = GetSigningBlock(inputFile);

    return true;
}

EndOfCentralDirectory* ZipSigner::GetZipEndOfCentralDirectory(std::ifstream& input)
{
    /* move file pointer to the end */
    input.seekg(0, std::ios::end);
    uint64_t fileSize = static_cast<uint64_t>(input.tellg());
    /* move file pointer to the begin */
    input.seekg(0, std::ios::beg);

    if (fileSize < EndOfCentralDirectory::EOCD_LENGTH) {
        SIGNATURE_TOOLS_LOGE("find zip eocd failed");
        return nullptr;
    }

    /* try to read EOCD without comment */
    int eocdLength = EndOfCentralDirectory::EOCD_LENGTH;
    m_eOCDOffset = fileSize - eocdLength;

    std::string retStr;
    int ret = FileUtils::ReadFileByOffsetAndLength(input, m_eOCDOffset, eocdLength, retStr);
    if (0 != ret) {
        SIGNATURE_TOOLS_LOGE("read eocd without comment failed in file");
        return nullptr;
    }

    std::optional<EndOfCentralDirectory*> eocdByBytes = EndOfCentralDirectory::GetEOCDByBytes(retStr);
    if (eocdByBytes) {
        return eocdByBytes.value();
    }

    /* try to search EOCD with comment */
    uint64_t eocdMaxLength = std::min(static_cast<uint64_t>(EndOfCentralDirectory::EOCD_LENGTH + MAX_COMMENT_LENGTH),
        fileSize);
    m_eOCDOffset = static_cast<uint64_t>(input.tellg()) - eocdMaxLength;

    retStr.clear();
    ret = FileUtils::ReadFileByOffsetAndLength(input, m_eOCDOffset, eocdMaxLength, retStr);
    if (0 != ret) {
        SIGNATURE_TOOLS_LOGE("read eocd with comment failed in file");
        return nullptr;
    }

    for (uint64_t start = 0; start < eocdMaxLength; start++) {
        eocdByBytes = EndOfCentralDirectory::GetEOCDByBytes(retStr, start);
        if (eocdByBytes) {
            m_eOCDOffset += start;
            return eocdByBytes.value();
        }
    }
    SIGNATURE_TOOLS_LOGE("read zip failed: can not find eocd in file");
    PrintErrorNumberMsg("ZIP_ERROR", ZIP_ERROR, "can not find eocd in file");
    return nullptr;
}

bool ZipSigner::GetZipCentralDirectory(std::ifstream& input)
{
    input.seekg(0, std::ios::beg);

    uint16_t cDtotal = m_endOfCentralDirectory->GetcDTotal();
    m_zipEntries.reserve(cDtotal);
    /* read full central directory bytes */
    std::string retStr;

    int ret = FileUtils::ReadFileByOffsetAndLength(input, m_cDOffset, m_endOfCentralDirectory->GetcDSize(), retStr);
    if (0 != ret) {
        SIGNATURE_TOOLS_LOGE("read full central directory failed in file");
        return false;
    }

    if (retStr.size() < CentralDirectory::CD_LENGTH) {
        SIGNATURE_TOOLS_LOGE("find zip cd failed");
        return false;
    }

    ByteBuffer bf(retStr.c_str(), retStr.size());

    std::string::size_type offset = 0;
    /* one by one format central directory */
    while (offset < retStr.size()) {
        CentralDirectory* cd = new CentralDirectory();
        if (!CentralDirectory::GetCentralDirectory(bf, cd)) {
            return false;
        }
        ZipEntry* entry = new ZipEntry();
        entry->SetCentralDirectory(cd);
        m_zipEntries.emplace_back(entry);
        offset += cd->GetLength();
    }

    if (offset + m_cDOffset != m_eOCDOffset) {
        SIGNATURE_TOOLS_LOGE("cd end offset not equals to eocd offset, maybe this is a zip64 file");
        return false;
    }
    return true;
}

std::string ZipSigner::GetSigningBlock(std::ifstream& file)
{
    int64_t size = static_cast<int64_t>(m_cDOffset) - static_cast<int64_t>(m_signingOffset);
    if (size < 0) {
        SIGNATURE_TOOLS_LOGE("signing offset in front of entry end");
        return "";
    }
    if (size == 0) {
        return "";
    }

    std::string retStr;
    int ret = FileUtils::ReadFileByOffsetAndLength(file, m_signingOffset, size, retStr);
    if (0 != ret) {
        SIGNATURE_TOOLS_LOGE("read signing block failed in file");
        return "";
    }
    return retStr;
}

bool ZipSigner::GetZipEntries(std::ifstream& input)
{
    /* use central directory data, find entry data */
    for (auto& entry : m_zipEntries) {
        CentralDirectory* cd = entry->GetCentralDirectory();
        uint32_t offset = cd->GetOffset();
        uint32_t unCompressedSize = cd->GetUnCompressedSize();
        uint32_t compressedSize = cd->GetCompressedSize();
        uint32_t fileSize = cd->GetMethod() == FILE_UNCOMPRESS_METHOD_FLAG ? unCompressedSize : compressedSize;

        ZipEntryData* zipEntryData = ZipEntryData::GetZipEntry(input, offset, fileSize);
        if (!zipEntryData) {
            return false;
        }
        if (m_cDOffset - offset < zipEntryData->GetLength()) {
            SIGNATURE_TOOLS_LOGE("cd offset in front of entry end");
            return false;
        }
        entry->SetZipEntryData(zipEntryData);
    }
    return true;
}

bool ZipSigner::ToFile(std::ifstream& input, std::ofstream& output)
{
    SIGNATURE_TOOLS_LOGI("Zip To File begin");
    if (!input.good()) {
        SIGNATURE_TOOLS_LOGE("read zip input file failed");
        return false;
    }
    if (!output.good()) {
        SIGNATURE_TOOLS_LOGE("read zip output file failed");
        return false;
    }

    for (const auto& entry : m_zipEntries) {
        ZipEntryData* zipEntryData = entry->GetZipEntryData();
        std::string zipEntryHeaderStr = zipEntryData->GetZipEntryHeader()->ToBytes();
        if (!FileUtils::WriteByteToOutFile(zipEntryHeaderStr, output)) {
            return false;
        }

        uint32_t fileOffset = zipEntryData->GetFileOffset();
        uint32_t fileSize = zipEntryData->GetFileSize();
        bool isSuccess = FileUtils::AppendWriteFileByOffsetToFile(input, output, fileOffset, fileSize);
        if (!isSuccess) {
            SIGNATURE_TOOLS_LOGE("write zip data failed");
            return false;
        }
        DataDescriptor* dataDescriptor = zipEntryData->GetDataDescriptor();
        if (dataDescriptor) {
            std::string dataDescriptorStr = dataDescriptor->ToBytes();
            if (!FileUtils::WriteByteToOutFile(dataDescriptorStr, output)) {
                return false;
            }
        }
    }

    if (!m_signingBlock.empty()) {
        if (!FileUtils::WriteByteToOutFile(m_signingBlock, output)) {
            return false;
        }
    }

    for (const auto& entry : m_zipEntries) {
        CentralDirectory* cd = entry->GetCentralDirectory();
        if (!FileUtils::WriteByteToOutFile(cd->ToBytes(), output)) {
            return false;
        }
    }

    if (!FileUtils::WriteByteToOutFile(m_endOfCentralDirectory->ToBytes(), output)) {
        return false;
    }

    SIGNATURE_TOOLS_LOGI("Zip To File end");
    return true;
}

void ZipSigner::Alignment(int alignment)
{
    Sort();
    bool isFirstUnRunnableFile = true;
    for (const auto& entry : m_zipEntries) {
        ZipEntryData* zipEntryData = entry->GetZipEntryData();
        short method = zipEntryData->GetZipEntryHeader()->GetMethod();
        if (method != FILE_UNCOMPRESS_METHOD_FLAG && !isFirstUnRunnableFile) {
            /* only align uncompressed entry and the first compress entry. */
            break;
        }
        int alignBytes;
        if (method == FILE_UNCOMPRESS_METHOD_FLAG &&
            FileUtils::IsRunnableFile(zipEntryData->GetZipEntryHeader()->GetFileName())) {
            /* .abc and .so file align 4096 byte. */
            alignBytes = 4096;
        } else if (isFirstUnRunnableFile) {
            /* the first file after runnable file, align 4096 byte. */
            alignBytes = 4096;
            isFirstUnRunnableFile = false;
        } else {
            /* normal file align 4 byte. */
            alignBytes = alignment;
        }
        int add = entry->Alignment(alignBytes);
        if (add > 0) {
            ResetOffset();
        }
    }
}

void ZipSigner::RemoveSignBlock()
{
    m_signingBlock = std::string();
    ResetOffset();
}

void ZipSigner::Sort()
{
    /* sort uncompress file (so, abc, an) - other uncompress file - compress file */
    std::sort(m_zipEntries.begin(), m_zipEntries.end(), [&](ZipEntry* entry1, ZipEntry* entry2) {
        short entry1Method = entry1->GetZipEntryData()->GetZipEntryHeader()->GetMethod();
        short entry2Method = entry2->GetZipEntryData()->GetZipEntryHeader()->GetMethod();
        std::string entry1FileName = entry1->GetZipEntryData()->GetZipEntryHeader()->GetFileName();
        std::string entry2FileName = entry2->GetZipEntryData()->GetZipEntryHeader()->GetFileName();
        if (entry1Method == FILE_UNCOMPRESS_METHOD_FLAG && entry2Method == FILE_UNCOMPRESS_METHOD_FLAG) {
            bool isRunnableFile1 = FileUtils::IsRunnableFile(entry1FileName);
            bool isRunnableFile2 = FileUtils::IsRunnableFile(entry2FileName);
            if (isRunnableFile1 && isRunnableFile2) {
                return entry1FileName < entry2FileName;
            } else if (isRunnableFile1) {
                return true;
            } else if (isRunnableFile2) {
                return false;
            }
        } else if (entry1Method == FILE_UNCOMPRESS_METHOD_FLAG) {
            return true;
        } else if (entry2Method == FILE_UNCOMPRESS_METHOD_FLAG) {
            return false;
        }
        return entry1FileName < entry2FileName;
    });
    ResetOffset();
}

void ZipSigner::ResetOffset()
{
    uint32_t offset = 0U;
    uint32_t cdLength = 0U;
    for (const auto& entry : m_zipEntries) {
        entry->GetCentralDirectory()->SetOffset(offset);
        offset += entry->GetZipEntryData()->GetLength();
        cdLength += entry->GetCentralDirectory()->GetLength();
    }
    if (!m_signingBlock.empty()) {
        offset += m_signingBlock.size();
    }
    m_cDOffset = offset;
    m_endOfCentralDirectory->SetOffset(offset);
    m_endOfCentralDirectory->SetcDSize(cdLength);
    offset += cdLength;
    m_eOCDOffset = offset;
}

std::vector<ZipEntry*>& ZipSigner::GetZipEntries()
{
    return m_zipEntries;
}

void ZipSigner::SetZipEntries(const std::vector<ZipEntry*>& zipEntries)
{
    m_zipEntries = zipEntries;
}

uint32_t ZipSigner::GetSigningOffset()
{
    return m_signingOffset;
}

void ZipSigner::SetSigningOffset(uint32_t signingOffset)
{
    m_signingOffset = signingOffset;
}

std::string ZipSigner::GetSigningBlock()
{
    return m_signingBlock;
}

void ZipSigner::SetSigningBlock(const std::string& signingBlock)
{
    m_signingBlock = signingBlock;
}

uint32_t ZipSigner::GetCDOffset()
{
    return m_cDOffset;
}

void ZipSigner::SetCDOffset(uint32_t cDOffset)
{
    m_cDOffset = cDOffset;
}

uint32_t ZipSigner::GetEOCDOffset()
{
    return m_eOCDOffset;
}

void ZipSigner::SetEOCDOffset(uint32_t eOCDOffset)
{
    m_eOCDOffset = eOCDOffset;
}

EndOfCentralDirectory* ZipSigner::GetEndOfCentralDirectory()
{
    return m_endOfCentralDirectory;
}

void ZipSigner::SetEndOfCentralDirectory(EndOfCentralDirectory* endOfCentralDirectory)
{
    m_endOfCentralDirectory = endOfCentralDirectory;
}
} // namespace SignatureTools
} // namespace OHOS