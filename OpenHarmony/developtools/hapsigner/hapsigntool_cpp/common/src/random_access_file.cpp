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

#include "securec.h"
#include "signature_info.h"
#include "signature_tools_log.h"
#include "digest_common.h"
#include "random_access_file.h"

namespace OHOS {
namespace SignatureTools {
int32_t RandomAccessFile::memoryPageSize = sysconf(_SC_PAGESIZE);

RandomAccessFile::RandomAccessFile()
    : fd(-1), fileLength(0)
{
}

RandomAccessFile::~RandomAccessFile()
{
    if (fd != -1) {
        close(fd);
    }
}

bool RandomAccessFile::Init(const std::string& filePath)
{
    fd = open(filePath.c_str(), O_RDWR);
    if (fd == -1) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "open file: " + filePath + " failed");
        return false;
    }

    if (memoryPageSize <= 0) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                            "getting pagesize failed. memoryPageSize: " + std::to_string(memoryPageSize));
        return false;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                            filePath + "get status failed");
        return false;
    }
    fileLength = file_stat.st_size;

    return true;
}

int64_t RandomAccessFile::GetLength() const
{
    return fileLength;
}

bool RandomAccessFile::CheckLittleEndian()
{
    union LittleEndian {
        int32_t number;
        char ch;
    } t;
    t.number = 1;
    return (t.ch == 1);
}

int32_t RandomAccessFile::DoMMap(int32_t bufCapacity, int64_t offset, MmapInfo& mmapInfo)
{
    if (!CheckLittleEndian()) {
        SIGNATURE_TOOLS_LOGE("CheckLittleEndian: failed");
        return MMAP_FAILED;
    }

    // Starting address for memory mapping
    mmapInfo.mapAddr = reinterpret_cast<char*>(MAP_FAILED);
    if (fd == -1) {
        SIGNATURE_TOOLS_LOGE("random access file's fd is -1, the file is closed");
        return FILE_IS_CLOSE;
    }
    if (offset < 0 || offset > fileLength - bufCapacity) {
        SIGNATURE_TOOLS_LOGE("offset is less than 0 OR read offset is out of range. offset: %" PRId64
                             ", range: %" PRId64, offset, fileLength - bufCapacity);
        return READ_OFFSET_OUT_OF_RANGE;
    }
    // Memory mapped file offset, 0 OR an integer multiple of 4K
    mmapInfo.mmapPosition = (offset / memoryPageSize) * memoryPageSize;
    // How many more bytes can be read from the current mapped memory page to find
    mmapInfo.readMoreLen = static_cast<int>(offset - mmapInfo.mmapPosition);
    mmapInfo.mmapSize = bufCapacity + mmapInfo.readMoreLen;
    mmapInfo.mapAddr = reinterpret_cast<char*>(mmap(nullptr, mmapInfo.mmapSize, PROT_READ | PROT_WRITE,
                                                    MAP_SHARED | MAP_POPULATE, fd, mmapInfo.mmapPosition));
    if (mmapInfo.mapAddr == MAP_FAILED) {
        SIGNATURE_TOOLS_LOGE("mmap failed");
        return MMAP_FAILED;
    }
    return 0;
}

int32_t RandomAccessFile::ReadFileFullyFromOffset(char buf[], int64_t offset, int64_t bufCapacity)
{
    if (buf == nullptr) {
        SIGNATURE_TOOLS_LOGE("The dest buffer is null");
        return DEST_BUFFER_IS_NULL;
    }

    MmapInfo mmapInfo;
    int32_t ret = DoMMap(bufCapacity, offset, mmapInfo);
    if (ret < 0) {
        return ret;
    }

    if (memcpy_s(buf, bufCapacity, mmapInfo.mapAddr + mmapInfo.readMoreLen,
                 mmapInfo.mmapSize - mmapInfo.readMoreLen) != EOK) {
        munmap(mmapInfo.mapAddr, mmapInfo.mmapSize);
        SIGNATURE_TOOLS_LOGE("memcpy_s error, errno: %d", IO_ERROR);
        return MMAP_COPY_FAILED;
    }
    munmap(mmapInfo.mapAddr, mmapInfo.mmapSize);
    return bufCapacity;
}

int32_t RandomAccessFile::ReadFileFullyFromOffset(ByteBuffer& buffer, int64_t offset)
{
    if (!buffer.HasRemaining()) {
        SIGNATURE_TOOLS_LOGE("The dest buffer has not remaining");
        return DEST_BUFFER_IS_NULL;
    }

    MmapInfo mmapInfo;
    int32_t bufCapacity = buffer.GetCapacity();
    int64_t ret = DoMMap(bufCapacity, offset, mmapInfo);
    if (ret < 0) {
        return ret;
    }

    buffer.PutData(0, mmapInfo.mapAddr + mmapInfo.readMoreLen, bufCapacity);
    munmap(mmapInfo.mapAddr, mmapInfo.mmapSize);
    return bufCapacity;
}

int32_t RandomAccessFile::WriteToFile(ByteBuffer& buffer, int64_t position, int64_t length)
{
    // write file, file length may change
    int64_t remainLength = fileLength - position;
    fileLength = (length <= remainLength) ? fileLength : (fileLength + (length - remainLength));
    // update file length
    if (ftruncate(fd, fileLength) == -1) {
        SIGNATURE_TOOLS_LOGE("RandomAccessFile ftruncate error: %s", strerror(errno));
        return IO_ERROR;
    }

    int32_t bufCapacity = buffer.GetCapacity();
    if (bufCapacity == 0) {
        SIGNATURE_TOOLS_LOGE("The dest buffer capacity is 0");
        return DEST_BUFFER_IS_NULL;
    }

    MmapInfo mmapInfo;

    int32_t ret = DoMMap(bufCapacity, position, mmapInfo);
    if (ret < 0) {
        SIGNATURE_TOOLS_LOGE("DoMMap failed: %d", ret);
        return ret;
    }

    if (memcpy_s(mmapInfo.mapAddr + mmapInfo.readMoreLen,
                 mmapInfo.mmapSize - mmapInfo.readMoreLen,
        buffer.GetBufferPtr(), bufCapacity) != RET_OK) {
        SIGNATURE_TOOLS_LOGE("memcpy_s error, errno: %d", IO_ERROR);
        return MMAP_COPY_FAILED;
    }
    munmap(mmapInfo.mapAddr, mmapInfo.mmapSize);
    return bufCapacity;
}

bool RandomAccessFile::ReadFileFromOffsetAndDigestUpdate(const DigestParameter& digestParam,
                                                         int32_t chunkSize, int64_t offset)
{
    MmapInfo mmapInfo;
    int32_t ret = DoMMap(chunkSize, offset, mmapInfo);
    if (ret < 0) {
        SIGNATURE_TOOLS_LOGE("DoMMap failed: %d", ret);
        return false;
    }
    unsigned char* content = reinterpret_cast<unsigned char*>(mmapInfo.mapAddr + mmapInfo.readMoreLen);
    bool res = DigestCommon::DigestUpdate(digestParam, content, chunkSize);
    munmap(mmapInfo.mapAddr, mmapInfo.mmapSize);
    return res;
}
}
}