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
#include "file_data_source.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
FileDataSource::FileDataSource(RandomAccessFile& hapFile,
                               int64_t offset, int64_t size, int64_t position)
    : DataSource(), hapFileRandomAccess(hapFile), fileOffset(offset), sourceSize(size), sourcePosition(position)
{
}

FileDataSource::~FileDataSource()
{
}

bool FileDataSource::HasRemaining() const
{
    return sourcePosition < sourceSize;
}

int64_t FileDataSource::Remaining() const
{
    return sourceSize - sourcePosition;
}

void FileDataSource::Reset()
{
    sourcePosition = 0;
}

bool FileDataSource::ReadDataAndDigestUpdate(const DigestParameter& digestParam, int32_t chunkSize)
{
    if (!hapFileRandomAccess.ReadFileFromOffsetAndDigestUpdate(digestParam, chunkSize,
        fileOffset + sourcePosition)) {
        SIGNATURE_TOOLS_LOGE("ReadFileFromOffsetAndDigestUpdate failed");
        return false;
    }
    sourcePosition += chunkSize;
    return true;
}
} // namespace SignatureTools
} // namespace OHOS