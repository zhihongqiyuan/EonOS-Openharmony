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
#include <mutex>
#include <stdexcept>
#include <string>

#include "signature_tools_log.h"
#include "random_access_file_output.h"

namespace OHOS {
namespace SignatureTools {
RandomAccessFileOutput::RandomAccessFileOutput(RandomAccessFile* file)
    : RandomAccessFileOutput(file, 0)
{
}

RandomAccessFileOutput::RandomAccessFileOutput(RandomAccessFile* file, int64_t startPosition)
    : file(file)
{
    if (startPosition < 0) {
        SIGNATURE_TOOLS_LOGE("invalide start position: %" PRId64, startPosition);
        return;
    }
    position = startPosition;
}

bool RandomAccessFileOutput::Write(ByteBuffer& buffer)
{
    int length = buffer.GetCapacity();
    if (length == 0) {
        return false;
    }
    std::mutex tmpMutex;
    {
        std::scoped_lock lock(tmpMutex);
        if (file->WriteToFile(buffer, position, length) < 0) {
            PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "write from ByteBuffer to RandomAccessFile failed");
            return false;
        }
        position += length;
    }
    return true;
}
} // namespace SignatureTools
} // namespace OHOS
