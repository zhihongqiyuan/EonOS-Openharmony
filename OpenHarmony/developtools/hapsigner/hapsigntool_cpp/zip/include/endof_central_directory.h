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

#ifndef SIGNATRUETOOLS_ENDOF_CENTRAL_DIRECTORY_H
#define SIGNATRUETOOLS_ENDOF_CENTRAL_DIRECTORY_H

#include <optional>
#include <string>

#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {
/**
 * resolve zip EndOfCentralDirectory data
 * EndOfCentralDirectory format for:
 * end of central dir signature    4 bytes  (0x06054b50)
 * number of this disk             2 bytes
 * number of the disk with the
 * start of the central directory  2 bytes
 * total number of entries in the
 * central directory on this disk  2 bytes
 * total number of entries in
 * the central directory           2 bytes
 * size of the central directory   4 bytes
 * offset of start of central
 * directory with respect to
 * the starting disk number        4 bytes
 * .ZIP file comment length        2 bytes
 * .ZIP file comment       (variable size)
 */
class EndOfCentralDirectory {
public:
    /* EndOfCentralDirectory invariable bytes length */
    static constexpr int EOCD_LENGTH = 22;

    /* 4 bytes , central directory signature */
    static constexpr int SIGNATURE = 0x06054b50;

    /**
     * init End Of Central Directory, default offset is 0
     *
     * @param bytes End Of Central Directory bytes
     * @return End Of Central Directory
     */
    static std::optional<EndOfCentralDirectory*> GetEOCDByBytes(const std::string& bytes);

    /**
     * init End Of Central Directory
     *
     * @param bytes End Of Central Directory bytes
     * @param offset offset
     * @return End Of Central Directory
     */
    static std::optional<EndOfCentralDirectory*> GetEOCDByBytes(const std::string& bytes, int offset);

    /**
     * change End Of Central Directory to bytes
     *
     * @return bytes
     */
    std::string ToBytes();

    static int GetEocdLength();

    static int GetSIGNATURE();

    uint16_t GetDiskNum();

    void SetDiskNum(uint16_t diskNum);

    uint16_t GetcDStartDiskNum();

    void SetcDStartDiskNum(uint16_t cDStartDiskNum);

    uint16_t GetThisDiskCDNum();

    void SetThisDiskCDNum(uint16_t thisDiskCDNum);

    uint16_t GetcDTotal();

    void SetcDTotal(uint16_t cDTotal);

    uint32_t GetcDSize();

    void SetcDSize(uint32_t cDSize);

    uint32_t GetOffset();

    void SetOffset(uint32_t offset);

    uint16_t GetCommentLength();

    void SetCommentLength(uint16_t commentLength);

    std::string GetComment();

    void SetComment(const std::string& comment);

    int GetLength();

    void SetLength(uint32_t length);

private:
    static void SetEndOfCentralDirectoryValues(ByteBuffer& bf, EndOfCentralDirectory* eocd);

    /* 2 bytes */
    uint16_t m_diskNum = 0;

    /* 2 bytes */
    uint16_t m_cDStartDiskNum = 0;

    /* 2 bytes */
    uint16_t m_thisDiskCDNum = 0;

    /* 2 bytes */
    uint16_t m_cDTotal = 0;

    /* 4 bytes */
    uint32_t m_cDSize = 0;

    /* 4bytes */
    uint32_t m_offset = 0;

    /* 2 bytes */
    uint16_t m_commentLength = 0;

    /* n bytes */
    std::string m_comment;

    uint32_t m_length = 0;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_ENDOF_CENTRAL_DIRECTORY_H