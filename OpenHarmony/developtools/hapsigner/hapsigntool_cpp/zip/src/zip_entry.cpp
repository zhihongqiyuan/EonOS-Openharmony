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

#include <stdexcept>

#include "signature_tools_log.h"
#include "zip_entry.h"

namespace OHOS {
namespace SignatureTools {
int ZipEntry::Alignment(int alignNum)
{
    /* if cd extra len bigger than entry extra len, make cd and entry extra length equals */
    if (alignNum == 0) {
        return -1;
    }
    uint16_t padding = 0;
    if (!CalZeroPaddingLengthForEntryExtra(padding)) {
        return -1;
    }
    uint32_t remainder = (m_zipEntryData->GetZipEntryHeader()->GetLength() +
        m_fileEntryInCentralDirectory->GetOffset()) % alignNum;
    if (remainder == 0) {
        return padding;
    }
    int add = alignNum - remainder;
    uint16_t newExtraLength = m_zipEntryData->GetZipEntryHeader()->GetExtraLength() + add;
    static constexpr int MAX_UNSIGNED_SHORT_VALUE = 0xFFFF;
    if (newExtraLength > MAX_UNSIGNED_SHORT_VALUE) {
        SIGNATURE_TOOLS_LOGE("can not align %s", m_zipEntryData->GetZipEntryHeader()->GetFileName().c_str());
        return -1;
    }
    SetEntryHeaderNewExtraLength(newExtraLength);
    SetCenterDirectoryNewExtraLength(newExtraLength);

    return add;
}

bool ZipEntry::CalZeroPaddingLengthForEntryExtra(uint16_t& padding)
{
    uint16_t entryExtraLen = m_zipEntryData->GetZipEntryHeader()->GetExtraLength();
    uint16_t cdExtraLen = m_fileEntryInCentralDirectory->GetExtraLength();
    if (cdExtraLen > entryExtraLen) {
        if (!SetEntryHeaderNewExtraLength(cdExtraLen)) {
            return false;
        }
        padding =  cdExtraLen - entryExtraLen;
    }
    if (cdExtraLen < entryExtraLen) {
        if (!SetCenterDirectoryNewExtraLength(entryExtraLen)) {
            return false;
        }
        padding =  entryExtraLen - cdExtraLen;
    }
    return true;
}

bool ZipEntry::SetCenterDirectoryNewExtraLength(uint16_t newLength)
{
    const std::string oldExtraData = m_fileEntryInCentralDirectory->GetExtraData();
    std::string newCDExtra;
    if (!GetAlignmentNewExtra(newLength, oldExtraData, newCDExtra)) {
        return false;
    }
    m_fileEntryInCentralDirectory->SetExtraData(newCDExtra);
    m_fileEntryInCentralDirectory->SetExtraLength(newLength);
    m_fileEntryInCentralDirectory->SetLength(CentralDirectory::CD_LENGTH +
        m_fileEntryInCentralDirectory->GetFileNameLength() +
        m_fileEntryInCentralDirectory->GetExtraLength() +
        m_fileEntryInCentralDirectory->GetCommentLength());
    return true;
}

bool ZipEntry::SetEntryHeaderNewExtraLength(uint16_t newLength)
{
    ZipEntryHeader* zipEntryHeader = m_zipEntryData->GetZipEntryHeader();
    const std::string oldExtraData = zipEntryHeader->GetExtraData();
    std::string alignmentNewExtra;
    if (!GetAlignmentNewExtra(newLength, oldExtraData, alignmentNewExtra)) {
        return false;
    }
    zipEntryHeader->SetExtraData(alignmentNewExtra);
    zipEntryHeader->SetExtraLength(newLength);
    zipEntryHeader->SetLength(ZipEntryHeader::HEADER_LENGTH +
        zipEntryHeader->GetExtraLength() + zipEntryHeader->GetFileNameLength());
    m_zipEntryData->SetLength(zipEntryHeader->GetLength() +
        m_zipEntryData->GetFileSize() +
        (m_zipEntryData->GetDataDescriptor() == nullptr ? 0 : DataDescriptor::DES_LENGTH));
    return true;
}

bool ZipEntry::GetAlignmentNewExtra(uint16_t newLength, const std::string& old, std::string& res)
{
    if (old.empty()) {
        res = std::string(newLength, 0);
        return true;
    }
    if (newLength < old.size()) {
        SIGNATURE_TOOLS_LOGE("can not align %s", m_zipEntryData->GetZipEntryHeader()->GetFileName().c_str());
        return false;
    }

    res = old;
    res.resize(newLength);
    return true;
}

ZipEntryData* ZipEntry::GetZipEntryData()
{
    return m_zipEntryData;
}

void ZipEntry::SetZipEntryData(ZipEntryData* zipEntryData)
{
    m_zipEntryData = zipEntryData;
}

CentralDirectory* ZipEntry::GetCentralDirectory()
{
    return m_fileEntryInCentralDirectory;
}

void ZipEntry::SetCentralDirectory(CentralDirectory* centralDirectory)
{
    m_fileEntryInCentralDirectory = centralDirectory;
}
} // namespace SignatureTools
} // namespace OHOS
