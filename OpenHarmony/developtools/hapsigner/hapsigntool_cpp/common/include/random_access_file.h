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

#ifndef SIGNATRUETOOLS_RANDOM_ACCESS_FILE_H
#define SIGNATRUETOOLS_RANDOM_ACCESS_FILE_H

#include <cerrno>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>

#include "export_define.h"
#include "byte_buffer.h"
#include "digest_parameter.h"

namespace OHOS {
namespace SignatureTools {

struct MmapInfo {
    int64_t mmapPosition;
    int32_t readMoreLen = 0;
    int32_t mmapSize = 0;
    char* mapAddr;
};

class RandomAccessFile {
public:
    DLL_EXPORT RandomAccessFile();
    DLL_EXPORT ~RandomAccessFile();
    DLL_EXPORT bool Init(const std::string& filePath);
    DLL_EXPORT int64_t GetLength() const;
    DLL_EXPORT int32_t ReadFileFullyFromOffset(ByteBuffer& buffer, int64_t offset);
    DLL_EXPORT int32_t ReadFileFullyFromOffset(char buf[], int64_t offset, int64_t bufCapacity);
    DLL_EXPORT int32_t WriteToFile(ByteBuffer& buffer, int64_t position, int64_t length);
    DLL_EXPORT bool ReadFileFromOffsetAndDigestUpdate(const DigestParameter& digestParam, int32_t chunkSize,
                                                      int64_t offset);

private:
    int32_t DoMMap(int32_t bufCapacity, int64_t offset, MmapInfo& mmapInfo);
    bool CheckLittleEndian();
    static int32_t memoryPageSize;
    int32_t fd = 0;
    int64_t fileLength;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_RANDOM_ACCESS_FILE_H
