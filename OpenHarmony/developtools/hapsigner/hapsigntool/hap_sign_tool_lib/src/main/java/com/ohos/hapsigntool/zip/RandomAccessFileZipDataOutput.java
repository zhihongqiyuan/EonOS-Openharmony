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
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

/**
 * RandomAccessFile implementation of ZipDataOutput
 *
 * @since 2021/12/20
 */
public class RandomAccessFileZipDataOutput implements ZipDataOutput {
    private final RandomAccessFile file;
    private final FileChannel fileChannel;
    private long position;

    /**
     * RandomAccessFileZipDataOutput
     *
     * @param file zip file
     */
    public RandomAccessFileZipDataOutput(RandomAccessFile file) {
        this(file, 0);
    }

    /**
     * RandomAccessFileZipDataOutput
     *
     * @param file zip file
     * @param startPosition start position offset
     */
    public RandomAccessFileZipDataOutput(RandomAccessFile file, long startPosition) {
        if (file == null) {
            throw new NullPointerException("file is null");
        }
        if (startPosition < 0) {
            throw new IllegalArgumentException("Invalid start position: " + startPosition);
        }
        this.file = file;
        this.fileChannel = file.getChannel();
        this.position = startPosition;
    }

    @Override
    public void write(byte[] buffer, int offset, int length) throws IOException {
        if (offset < 0) {
            throw new IndexOutOfBoundsException("offset: " + offset);
        }
        if (offset > buffer.length) {
            throw new IndexOutOfBoundsException("offset (" + offset + ") > buffer length(" + buffer.length + ")");
        }
        if (length == 0) {
            return;
        }
        synchronized (file) {
            file.seek(position);
            file.write(buffer, offset, length);
            position += length;
        }
    }

    @Override
    public void write(ByteBuffer buffer) throws IOException {
        int length = buffer.remaining();
        if (length == 0) {
            return;
        }
        synchronized (file) {
            file.seek(position);
            while (buffer.hasRemaining()) {
                fileChannel.write(buffer);
            }
            position += length;
        }
    }
}
