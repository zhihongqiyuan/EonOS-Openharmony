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

#include <fstream>
#include <iostream>

#include "file_utils.h"
#include "signature_tools_log.h"
#include "zip_entry_data.h"

namespace OHOS {
namespace SignatureTools {
ZipEntryHeader* ZipEntryData::GetZipEntryHeader()
{
    return m_zipEntryHeader;
}

ZipEntryData* ZipEntryData::GetZipEntry(std::ifstream& input, uint32_t entryOffset, uint32_t fileSize)
{
    uint32_t offset = entryOffset;
    /* read entry header by file and offset. */
    std::string headStr;
    if (FileUtils::ReadInputByOffsetAndLength(input, entryOffset, ZipEntryHeader::HEADER_LENGTH, headStr) != 0) {
        SIGNATURE_TOOLS_LOGE("read zip entry head failed in file");
        return nullptr;
    }
    ZipEntryHeader* entryHeader = ZipEntryHeader::GetZipEntryHeader(headStr);
    if (!entryHeader) {
        return nullptr;
    }
    offset += ZipEntryHeader::HEADER_LENGTH;

    /* read entry file name and extra by offset. */
    if (!ReadEntryFileNameAndExtraByOffset(input, entryHeader, offset)) {
        return nullptr;
    }
    /* skip file data , save file offset and size. */
    ZipEntryData* entry = new ZipEntryData();
    entry->SetFileOffset(offset);
    entry->SetFileSize(fileSize);
    input.seekg(fileSize, std::ios::cur);
    int64_t entryLength = entryHeader->GetLength() + fileSize;

    /* set desc null flag */
    if ((entryHeader->GetFlag() & HAS_DATA_DESCRIPTOR_MASK) != NOT_HAS_DATA_DESCRIPTOR_FLAG) {
        /* if entry has data descriptor, read entry data descriptor. */
        std::string retStr;
        if (FileUtils::ReadInputByLength(input, DataDescriptor::DES_LENGTH, retStr) != 0) {
            SIGNATURE_TOOLS_LOGE("read entry data descriptor failed in file");
            return nullptr;
        }
        DataDescriptor* dataDesc = DataDescriptor::GetDataDescriptor(retStr);
        if (!dataDesc) {
            return nullptr;
        }
        entryLength += DataDescriptor::DES_LENGTH;
        entry->SetDataDescriptor(dataDesc);
    }
    entry->SetZipEntryHeader(entryHeader);
    entry->SetLength(entryLength);
    return entry;
}

bool ZipEntryData::ReadEntryFileNameAndExtraByOffset(std::ifstream& input, ZipEntryHeader* entryHeader,
    uint32_t& offset)
{
    if (entryHeader->GetFileNameLength() > 0) {
        std::string fileNameStr;
        if (FileUtils::ReadInputByLength(input, entryHeader->GetFileNameLength(), fileNameStr) != 0) {
            SIGNATURE_TOOLS_LOGE("read entry file name failed in file");
            return false;
        }
        entryHeader->ReadFileName(fileNameStr);
        offset += entryHeader->GetFileNameLength();
    }
    if (entryHeader->GetExtraLength() > 0) {
        std::string extraStr;
        if (FileUtils::ReadInputByLength(input, entryHeader->GetExtraLength(), extraStr) != 0) {
            SIGNATURE_TOOLS_LOGE("read entry file extra failed in file");
            return false;
        }
        entryHeader->ReadExtra(extraStr);
        offset += entryHeader->GetExtraLength();
    }
    return true;
}

void ZipEntryData::SetZipEntryHeader(ZipEntryHeader* zipEntryHeader)
{
    m_zipEntryHeader = zipEntryHeader;
}

DataDescriptor* ZipEntryData::GetDataDescriptor()
{
    return m_dataDescriptor;
}

void ZipEntryData::SetDataDescriptor(DataDescriptor* dataDescriptor)
{
    m_dataDescriptor = dataDescriptor;
}

uint32_t ZipEntryData::GetFileOffset()
{
    return m_fileOffset;
}

void ZipEntryData::SetFileOffset(uint32_t fileOffset)
{
    m_fileOffset = fileOffset;
}

uint32_t ZipEntryData::GetFileSize()
{
    return m_fileSize;
}

void ZipEntryData::SetFileSize(uint32_t fileSize)
{
    m_fileSize = fileSize;
}

uint32_t ZipEntryData::GetLength()
{
    return m_length;
}

void ZipEntryData::SetLength(uint32_t length)
{
    m_length = length;
}
} // namespace SignatureTools
} // namespace OHOS