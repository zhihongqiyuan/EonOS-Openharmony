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
#include <algorithm>
#include <mutex>
#include <stdexcept>
#include <string>

#include "random_access_file_input.h"

namespace OHOS {
namespace SignatureTools {
RandomAccessFileInput::RandomAccessFileInput(RandomAccessFile& file) : file(file),
    startIndex(0), size(-1)
{
}

RandomAccessFileInput::RandomAccessFileInput(RandomAccessFile& file, int64_t offset, int64_t size)
    : file(file), startIndex(offset), size(size)
{
    if (offset < 0) {
        SIGNATURE_TOOLS_LOGE("out of range: offset %" PRId64, offset);
        return;
    }
    if (size < 0) {
        SIGNATURE_TOOLS_LOGE("out of range: size %" PRId64, size);
        return;
    }
}

int64_t RandomAccessFileInput::Size()
{
    if (size == -1) {
        return file.GetLength();
    }
    return size;
}

bool RandomAccessFileInput::CopyTo(int64_t offset, int size, ByteBuffer& buffer)
{
    int64_t srcSize = Size();
    if (!CheckBoundValid(offset, size, srcSize)) {
        return false;
    }
    if (size == 0) {
        SIGNATURE_TOOLS_LOGE("Copy size is equal to 0");
        return false;
    }
    if (size > buffer.Remaining()) {
        SIGNATURE_TOOLS_LOGE("The length size passed in is greater than the reserved length of ByteBuffer");
        return false;
    }
    int64_t offsetInFile = startIndex + offset;
    int remaining = size;
    int originalLimit = buffer.GetLimit();

    buffer.SetLimit(buffer.GetPosition() + size);
    while (remaining > 0) {
        int64_t readSize;
        std::mutex tmpMutex;
        {
            std::scoped_lock lock(tmpMutex);
            readSize = file.ReadFileFullyFromOffset(buffer, offsetInFile);
        }
        offsetInFile += readSize;
        remaining -= readSize;
    }
    int cap = buffer.GetCapacity();
    buffer.SetPosition(cap);
    buffer.SetLimit(originalLimit);
    return true;
}

ByteBuffer RandomAccessFileInput::CreateByteBuffer(int64_t offset, int size)
{
    ByteBuffer byteBuffer;
    if (size < 0) {
        SIGNATURE_TOOLS_LOGE("IndexOutOfBounds: The created ByteBuffer size %d is less than 0", size);
        return byteBuffer;
    }

    byteBuffer.SetCapacity(size);
    if (!CopyTo(offset, size, byteBuffer)) {
        byteBuffer.SetCapacity(0);
        return byteBuffer;
    }

    return byteBuffer.Flip();
}

DataSource* RandomAccessFileInput::Slice(int64_t offset, int64_t size)
{
    int64_t srcSize = Size();
    if (!CheckBoundValid(offset, size, srcSize)) {
        return nullptr;
    }
    if (offset == 0 && size == srcSize) {
        SIGNATURE_TOOLS_LOGI("offset: 0, size: %" PRId64 ", it will return itself", size);
        return new FileDataSource(file, startIndex, size, 0);
    }
    return new FileDataSource(file, offset, size, 0);
}

bool RandomAccessFileInput::CheckBoundValid(int64_t offset, int64_t size, int64_t sourceSize)
{
    if (offset < 0) {
        SIGNATURE_TOOLS_LOGE("out of range: offset %" PRId64, offset);
        return false;
    }
    if (size < 0) {
        SIGNATURE_TOOLS_LOGE("out of range: size %" PRId64, size);
        return false;
    }
    if (offset > sourceSize) {
        SIGNATURE_TOOLS_LOGE("out of range: offset %" PRId64 " is greater than sourceSize %" PRId64,
            offset, sourceSize);
        return false;
    }
    int64_t endOffset = offset + size;
    if (endOffset < offset) {
        SIGNATURE_TOOLS_LOGE("out of range: offset %" PRId64 " add size %" PRId64 " is overflow",
            offset, size);
        return false;
    }
    if (endOffset > sourceSize) {
        SIGNATURE_TOOLS_LOGE("out of range: offset %" PRId64 " add size %" PRId64 " is greater than "
            "sourceSize %" PRId64, offset, size, sourceSize);
        return false;
    }
    return true;
}
} // namespace SignatureTools
} // namespace OHOS