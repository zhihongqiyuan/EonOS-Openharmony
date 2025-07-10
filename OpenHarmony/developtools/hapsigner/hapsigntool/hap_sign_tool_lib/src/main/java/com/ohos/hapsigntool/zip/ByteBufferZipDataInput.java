/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.zip;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * ByteBuffer implementation of ZipDataInput
 *
 * @since 2021/12/20
 */
public class ByteBufferZipDataInput implements ZipDataInput {
    private final ByteBuffer buffer;
    private final int sourceSize;

    public ByteBufferZipDataInput(ByteBuffer buffer) {
        this(buffer, true);
    }

    private ByteBufferZipDataInput(ByteBuffer buffer, boolean needSlice) {
        this.buffer = needSlice ? buffer.slice() : buffer;
        this.sourceSize = this.buffer.remaining();
    }

    @Override
    public long size() {
        return sourceSize;
    }

    @Override
    public void copyTo(long offset, long size, ZipDataOutput output) throws IOException {
        if (size < 0 || size > sourceSize) {
            throw new IndexOutOfBoundsException("size: " + size + ", source size: " + sourceSize);
        }
        output.write(createByteBuffer(offset, (int) size));
    }

    @Override
    public void copyTo(long offset, int size, ByteBuffer buffer) throws IOException {
        buffer.put(createByteBuffer(offset, size));
    }

    @Override
    public ByteBuffer createByteBuffer(long offset, int size) {
        checkChunkValid(offset, size);
        int position = (int) offset;
        int limit = position + size;
        synchronized (buffer) {
            buffer.position(0);
            buffer.limit(limit);
            buffer.position(position);
            return buffer.slice();
        }
    }

    @Override
    public ZipDataInput slice(long offset, long size) {
        if (offset == 0 && size == sourceSize) {
            return this;
        }
        if (size < 0 || size > sourceSize) {
            throw new IndexOutOfBoundsException("size: " + size + ", source size: " + sourceSize);
        }
        ByteBuffer byteBuffer = createByteBuffer(offset, (int) size);
        return new ByteBufferZipDataInput(byteBuffer, false);
    }

    private void checkChunkValid(long offset, long size) {
        if (offset < 0) {
            throw new IndexOutOfBoundsException("offset: " + offset);
        }
        if (size < 0) {
            throw new IndexOutOfBoundsException("sourceSize: " + size);
        }
        if (offset > sourceSize) {
            throw new IndexOutOfBoundsException(
                    "offset (" + offset + ") > source sourceSize (" + sourceSize + ")");
        }
        long endOffset = offset + size;
        if (endOffset < offset) {
            throw new IndexOutOfBoundsException(
                    "offset (" + offset + ") + sourceSize (" + size + ") overflow");
        }
        if (endOffset > sourceSize) {
            throw new IndexOutOfBoundsException(
                    "offset (" + offset + ") + sourceSize (" + size + ") > source sourceSize (" + sourceSize + ")");
        }
    }
}