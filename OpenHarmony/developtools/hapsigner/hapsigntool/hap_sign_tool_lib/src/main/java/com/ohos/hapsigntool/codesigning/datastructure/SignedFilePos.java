/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.codesigning.datastructure;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Locale;

/**
 * Sign info of file
 *
 * @since 2023/09/08
 */
public class SignedFilePos {
    /**
     * file name offset based on start of so info segment
     */
    private int fileNameOffset;

    /**
     * byte size of file
     */
    private final int fileNameSize;

    /**
     * sign info offset based on start of so info segment
     */
    private int signInfoOffset;

    /**
     * byte size of sign info
     */
    private final int signInfoSize;

    /**
     * Constructor for SignedFilePos
     *
     * @param fileNameOffset file name offset based on segment start
     * @param fileNameSize   byte size of file name string
     * @param signInfoOffset sign info offset based on segment start
     * @param signInfoSize   byte size of sign info
     */
    public SignedFilePos(int fileNameOffset, int fileNameSize, int signInfoOffset, int signInfoSize) {
        this.fileNameOffset = fileNameOffset;
        this.fileNameSize = fileNameSize;
        this.signInfoOffset = signInfoOffset;
        this.signInfoSize = signInfoSize;
    }

    public int getFileNameOffset() {
        return fileNameOffset;
    }

    public int getFileNameSize() {
        return fileNameSize;
    }

    public int getSignInfoOffset() {
        return signInfoOffset;
    }

    public int getSignInfoSize() {
        return signInfoSize;
    }

    /**
     * increase file name offset
     *
     * @param incOffset increase value
     */
    public void increaseFileNameOffset(int incOffset) {
        this.fileNameOffset += incOffset;
    }

    /**
     * increase sign info offset
     *
     * @param incOffset increase value
     */
    public void increaseSignInfoOffset(int incOffset) {
        this.signInfoOffset += incOffset;
    }

    /**
     * Constructor for SignedFilePos by byte array
     *
     * @param bytes Byte array representation of SignedFilePos
     * @return a newly created SignedFilePos object
     */
    public static SignedFilePos fromByteArray(byte[] bytes) {
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        bf.rewind();
        int inFileNameOffset = bf.getInt();
        int inFileNameSize = bf.getInt();
        int inSignInfoOffset = bf.getInt();
        int inSignInfoSize = bf.getInt();
        return new SignedFilePos(inFileNameOffset, inFileNameSize, inSignInfoOffset, inSignInfoSize);
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        return String.format(Locale.ROOT, "SignedFilePos: fileNameOffset, Size[%d, %d], signInfoOffset, Size[%d, %d]",
            this.fileNameOffset, this.fileNameSize, this.signInfoOffset, this.signInfoSize);
    }
}
