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
import java.io.RandomAccessFile;
import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

/**
 * RandomAccessFile implementation of ZipDataInput
 *
 * @since 2021/12/20
 */
public class RandomAccessFileZipDataInput implements ZipDataInput {
    private static final int MAX_READ_BLOCK_SIZE = 1024 * 1024;

    private final RandomAccessFile file;

    private final FileChannel fileChannel;

    private final long startIndex;

    private final long size;

    /**
     * Random Access File Zip Data Input
     *
     * @param file zip file
     */
    public RandomAccessFileZipDataInput(RandomAccessFile file) {
        this.file = file;
        this.fileChannel = file.getChannel();
        this.startIndex = 0;
        this.size = -1;
    }

    /**
     * Random Access File Zip Data Input
     *
     * @param file zip file
     * @param offset offset
     * @param size size
     */
    public RandomAccessFileZipDataInput(RandomAccessFile file, long offset, long size) {
        if (offset < 0) {
            throw new IndexOutOfBoundsException("offset: " + offset);
        }
        if (size < 0) {
            throw new IndexOutOfBoundsException("size: " + size);
        }
        this.file = file;
        this.fileChannel = file.getChannel();
        this.startIndex = offset;
        this.size = size;
    }

    @Override
    public long size() {
        if (size == -1) {
            try {
                return file.length();
            } catch (IOException e) {
                return 0;
            }
        }
        return size;
    }

    @Override
    public void copyTo(long offset, long size, ZipDataOutput output) throws IOException {
        long srcSize = size();
        checkBoundValid(offset, size, srcSize);
        if (size == 0) {
            return;
        }
        long offsetInFile = startIndex + offset;
        long remaining = size;
        int blockSize;
        byte[] buf = new byte[MAX_READ_BLOCK_SIZE];
        while (remaining > 0) {
            blockSize = (int) Math.min(remaining, buf.length);
            synchronized (file) {
                file.seek(offsetInFile);
                file.readFully(buf, 0, blockSize);
                output.write(buf, 0, blockSize);
            }
            offsetInFile += blockSize;
            remaining -= blockSize;
        }
    }

    @Override
    public void copyTo(long offset, int size, ByteBuffer buffer) throws IOException {
        long srcSize = size();
        checkBoundValid(offset, size, srcSize);
        if (size == 0) {
            return;
        }
        if (size > buffer.remaining()) {
            throw new BufferOverflowException();
        }
        long offsetInFile = startIndex + offset;
        int remaining = size;
        int originalLimit = buffer.limit();
        try {
            buffer.limit(buffer.position() + size);
            int readSize;
            while (remaining > 0) {
                synchronized (file) {
                    fileChannel.position(offsetInFile);
                    readSize = fileChannel.read(buffer);
                }
                offsetInFile += readSize;
                remaining -= readSize;
            }
        } finally {
            buffer.limit(originalLimit);
        }
    }

    @Override
    public ByteBuffer createByteBuffer(long offset, int size) throws IOException {
        if (size < 0) {
            throw new IndexOutOfBoundsException("size: " + size);
        }
        ByteBuffer buffer = ByteBuffer.allocate(size);
        copyTo(offset, size, buffer);
        buffer.flip();
        return buffer;
    }

    @Override
    public ZipDataInput slice(long offset, long size) {
        long srcSize = size();
        checkBoundValid(offset, size, srcSize);
        if (offset == 0 && size == srcSize) {
            return this;
        }
        return new RandomAccessFileZipDataInput(file, offset + startIndex, size);
    }

    private void checkBoundValid(long offset, long size, long sourceSize) {
        if (offset < 0) {
            throw new IndexOutOfBoundsException("offset: " + offset);
        }
        if (size < 0) {
            throw new IndexOutOfBoundsException("size: " + size);
        }
        if (offset > sourceSize) {
            throw new IndexOutOfBoundsException("offset (" + offset + ") > sourceSize (" + sourceSize + ")");
        }
        long endOffset = offset + size;
        if (endOffset < offset) {
            throw new IndexOutOfBoundsException("offset (" + offset + ") + size (" + size + ") overflow");
        }
        if (endOffset > sourceSize) {
            throw new IndexOutOfBoundsException("offset (" + offset + ") + size (" + size
                + ") > sourceSize (" + sourceSize + ")");
        }
    }
}
