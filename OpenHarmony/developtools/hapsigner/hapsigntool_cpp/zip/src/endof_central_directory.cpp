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

#include <cinttypes>

#include "endof_central_directory.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
std::optional<EndOfCentralDirectory*> EndOfCentralDirectory::GetEOCDByBytes(const std::string& bytes)
{
    return GetEOCDByBytes(bytes, 0);
}

std::optional<EndOfCentralDirectory*> EndOfCentralDirectory::GetEOCDByBytes(const std::string& bytes, int offset)
{
    EndOfCentralDirectory* eocd = new EndOfCentralDirectory();
    int remainingDataLen = bytes.size() - offset;
    if (remainingDataLen < EOCD_LENGTH) {
        delete eocd;
        SIGNATURE_TOOLS_LOGE("remainingDataLen is less than EOCD_LENGTH, remainingDataLen: %d, "
                             "EOCD_LENGTH: %d", remainingDataLen, EOCD_LENGTH);
        return std::nullopt;
    }

    ByteBuffer bf(bytes.c_str(), bytes.size());

    int signValue;
    bf.GetInt32(signValue);
    if (signValue != SIGNATURE) {
        delete eocd;
        return std::nullopt;
    }

    SetEndOfCentralDirectoryValues(bf, eocd);

    uint16_t commentLength = eocd->GetCommentLength();
    if (bf.Remaining() != commentLength) {
        delete eocd;
        SIGNATURE_TOOLS_LOGE("bf.Remaining() is not equal to commentLength, bf.Remaining(): %d, "
                             "commentLength: %" PRIu16, bf.Remaining(), commentLength);
        return std::nullopt;
    }
    if (commentLength > 0) {
        std::string readComment(commentLength, 0);
        bf.GetData(&readComment[0], commentLength);
        eocd->SetComment(readComment);
    }
    eocd->SetLength(EOCD_LENGTH + commentLength);
    if (bf.Remaining() != 0) {
        delete eocd;
        SIGNATURE_TOOLS_LOGE("bf.Remaining() is not equal to 0, bf.Remaining(): %d", bf.Remaining());
        return std::nullopt;
    }
    return std::make_optional(eocd);
}

void EndOfCentralDirectory::SetEndOfCentralDirectoryValues(ByteBuffer& bf, EndOfCentralDirectory* eocd)
{
    uint16_t eocdUInt16Value = 0;
    bf.GetUInt16(eocdUInt16Value);
    eocd->SetDiskNum(eocdUInt16Value);

    bf.GetUInt16(eocdUInt16Value);
    eocd->SetcDStartDiskNum(eocdUInt16Value);

    bf.GetUInt16(eocdUInt16Value);
    eocd->SetThisDiskCDNum(eocdUInt16Value);

    bf.GetUInt16(eocdUInt16Value);
    eocd->SetcDTotal(eocdUInt16Value);

    uint32_t eocdUInt32Value;
    bf.GetUInt32(eocdUInt32Value);
    eocd->SetcDSize(eocdUInt32Value);

    bf.GetUInt32(eocdUInt32Value);
    eocd->SetOffset(eocdUInt32Value);

    bf.GetUInt16(eocdUInt16Value);
    eocd->SetCommentLength(eocdUInt16Value);
}

std::string EndOfCentralDirectory::ToBytes()
{
    ByteBuffer bf(m_length);

    bf.PutInt32(SIGNATURE);
    bf.PutUInt16(m_diskNum);
    bf.PutUInt16(m_cDStartDiskNum);
    bf.PutUInt16(m_thisDiskCDNum);
    bf.PutUInt16(m_cDTotal);
    bf.PutUInt32(m_cDSize);
    bf.PutUInt32(m_offset);
    bf.PutUInt16(m_commentLength);

    if (m_commentLength > 0) {
        bf.PutData(m_comment.data(), m_comment.size());
    }

    return bf.ToString();
}

int EndOfCentralDirectory::GetEocdLength()
{
    return EOCD_LENGTH;
}

int EndOfCentralDirectory::GetSIGNATURE()
{
    return SIGNATURE;
}

uint16_t EndOfCentralDirectory::GetDiskNum()
{
    return m_diskNum;
}

void EndOfCentralDirectory::SetDiskNum(uint16_t diskNum)
{
    m_diskNum = diskNum;
}

uint16_t EndOfCentralDirectory::GetcDStartDiskNum()
{
    return m_cDStartDiskNum;
}

void EndOfCentralDirectory::SetcDStartDiskNum(uint16_t cDStartDiskNum)
{
    m_cDStartDiskNum = cDStartDiskNum;
}

uint16_t EndOfCentralDirectory::GetThisDiskCDNum()
{
    return m_thisDiskCDNum;
}

void EndOfCentralDirectory::SetThisDiskCDNum(uint16_t thisDiskCDNum)
{
    m_thisDiskCDNum = thisDiskCDNum;
}

uint16_t EndOfCentralDirectory::GetcDTotal()
{
    return m_cDTotal;
}

void EndOfCentralDirectory::SetcDTotal(uint16_t cDTotal)
{
    m_cDTotal = cDTotal;
}

uint32_t EndOfCentralDirectory::GetcDSize()
{
    return m_cDSize;
}

void EndOfCentralDirectory::SetcDSize(uint32_t cDSize)
{
    m_cDSize = cDSize;
}

uint32_t EndOfCentralDirectory::GetOffset()
{
    return m_offset;
}

void EndOfCentralDirectory::SetOffset(uint32_t offset)
{
    m_offset = offset;
}

uint16_t EndOfCentralDirectory::GetCommentLength()
{
    return m_commentLength;
}

void EndOfCentralDirectory::SetCommentLength(uint16_t commentLength)
{
    m_commentLength = commentLength;
}

std::string EndOfCentralDirectory::GetComment()
{
    return m_comment;
}

void EndOfCentralDirectory::SetComment(const std::string& comment)
{
    m_comment = comment;
}

int EndOfCentralDirectory::GetLength()
{
    return m_length;
}

void EndOfCentralDirectory::SetLength(uint32_t length)
{
    m_length = length;
}
} // namespace SignatureTools
} // namespace OHOS