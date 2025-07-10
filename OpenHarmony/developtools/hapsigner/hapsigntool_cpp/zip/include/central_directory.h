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

#ifndef SIGNATRUETOOLS_CENTRAL_DIRECTORY_H
#define SIGNATRUETOOLS_CENTRAL_DIRECTORY_H

#include <string>

#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {
/**
 * resolve zip CentralDirectory data
 * CentralDirectory format for:
 * central file header signature   4 bytes  (0x02014b50)
 * version made by                 2 bytes
 * version needed to extract       2 bytes
 * general purpose bit flag        2 bytes
 * compression method              2 bytes
 * last mod file time              2 bytes
 * last mod file date              2 bytes
 * crc-32                          4 bytes
 * compressed size                 4 bytes
 * uncompressed size               4 bytes
 * file name length                2 bytes
 * extra field length              2 bytes
 * file comment length             2 bytes
 * disk number start               2 bytes
 * internal file attributes        2 bytes
 * external file attributes        4 bytes
 * relative offset of local header 4 bytes
 * file name (variable size)
 * extra field (variable size)
 * file comment (variable size)
 */
class CentralDirectory {
public:
    // central directory invariable bytes length
    static constexpr int CD_LENGTH = 46;

    // 4 bytes, central directory signature
    static constexpr int SIGNATURE = 0x02014b50;

    static bool GetCentralDirectory(ByteBuffer& bf, CentralDirectory* cd);

    std::string ToBytes();

    static int GetCdLength();

    static int GetSIGNATURE();

    short GetVersion();

    void SetVersion(short version);

    short GetVersionExtra();

    void SetVersionExtra(short versionExtra);

    short GetMethod();

    void SetMethod(short method);

    short GetFlag();

    void SetFlag(short flag);

    int GetCrc32();

    void SetCrc32(int crc32);

    short GetLastTime();

    void SetLastTime(short lastTime);

    short GetLastDate();

    void SetLastDate(short lastDate);

    uint32_t GetCompressedSize();

    void SetCompressedSize(uint32_t compressedSize);

    uint32_t GetUnCompressedSize();

    void SetUnCompressedSize(uint32_t unCompressedSize);

    uint16_t GetExtraLength();

    void SetExtraLength(uint16_t extraLength);

    uint16_t GetFileNameLength();

    void SetFileNameLength(uint16_t fileNameLength);

    uint16_t GetDiskNumStart();

    void SetDiskNumStart(uint16_t diskNumStart);

    uint16_t GetCommentLength();

    void SetCommentLength(uint16_t commentLength);

    short GetInternalFile();

    void SetInternalFile(short internalFile);

    int GetExternalFile();

    void SetExternalFile(int externalFile);

    uint32_t GetOffset();

    void SetOffset(uint32_t offset);

    std::string GetFileName();

    void SetFileName(const std::string& fileName);

    std::string GetExtraData() const;

    void SetExtraData(const std::string& extraData);

    std::string GetComment();

    void SetComment(const std::string& comment);

    uint32_t GetLength();

    void SetLength(uint32_t length);

private:
    static void SetCentralDirectoryValues(ByteBuffer& bf, CentralDirectory* cd);

    /* 2 bytes */
    short m_version = 0;

    /* 2 bytes */
    short m_versionExtra = 0;

    /* 2 bytes */
    short m_flag = 0;

    /* 2 bytes */
    short m_method = 0;

    /* 2 bytes */
    short m_lastTime = 0;

    /* 2 bytes */
    short m_lastDate = 0;

    /* 4 bytes */
    int m_crc32 = 0;

    /* 4 bytes */
    uint32_t m_compressedSize = 0;

    /* 4 bytes */
    uint32_t m_unCompressedSize = 0;

    /* 2 bytes */
    uint16_t m_fileNameLength = 0;

    /* 2 bytes */
    uint16_t m_extraLength = 0;

    /* 2 bytes */
    uint16_t m_commentLength = 0;

    /* 2 bytes */
    uint16_t m_diskNumStart = 0;

    /* 2 bytes */
    short m_internalFile = 0;

    /* 4 bytes */
    int m_externalFile = 0;

    /* 4 bytes */
    uint32_t m_offset = 0;

    /* n bytes */
    std::string m_fileName;

    /* n bytes */
    std::string m_extraData;

    /* n bytes */
    std::string m_comment;

    uint32_t m_length = 0;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_CENTRAL_DIRECTORY_H