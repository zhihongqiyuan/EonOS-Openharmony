/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.hap.entity;

import com.ohos.hapsigntool.utils.ByteArrayUtils;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * define class of hap signature block head
 *
 * @since 2021-12-13
 */
public class SignHead {
    /**
     * length of sign head
     */
    public static final int SIGN_HEAD_LEN = 32;

    /**
     * sign hap magic string 16Bytes-Magic
     */
    public static final char[] MAGIC = "hw signed app   ".toCharArray();

    /**
     * sign elf magic string 16Bytes-Magic
     */
    public static final char[] ELF_MAGIC = "elf sign block  ".toCharArray();

    /**
     * sign block version 4-Bytes, version is 1.0.0.0
     */
    public static final char[] VERSION = "1000".toCharArray();

    private static final int NUM_OF_BLOCK = 2; // number of sub-block

    private static final int RESERVE_LENGTH = 4;

    private char[] reserve = new char[RESERVE_LENGTH];

    /**
     * get serialization of HwSignHead file type of bin
     *
     * @param subBlockSize the total size of all sub-blocks
     * @return Byte array after serialization of HwSignHead
     */
    public byte[] getSignHead(int subBlockSize) {
        int size = subBlockSize; // total size of sub-block
        byte[] signHead = new byte[SIGN_HEAD_LEN];
        int start = 0;
        try {
            start = ByteArrayUtils.insertCharToByteArray(signHead, start, MAGIC);
            if (start < 0) {
                throw new IOException();
            }
            start = ByteArrayUtils.insertCharToByteArray(signHead, start, VERSION);
            if (start < 0) {
                throw new IOException();
            }
            start = ByteArrayUtils.insertIntToByteArray(signHead, start, size);
            if (start < 0) {
                throw new IOException();
            }
            start = ByteArrayUtils.insertIntToByteArray(signHead, start, NUM_OF_BLOCK);
            if (start < 0) {
                throw new IOException();
            }
            start = ByteArrayUtils.insertCharToByteArray(signHead, start, reserve);
            if (start < 0) {
                throw new IOException();
            }
        } catch (IOException e) {
            return new byte[0];
        }
        return signHead;
    }

    /**
     * get serialization of HwSignHead file type of elf
     *
     * @param subBlockSize the total size of all sub-blocks
     * @param subBlockNum the sign block num
     * @return Byte array after serialization of HwSignHead
     */
    public byte[] getSignHeadLittleEndian(int subBlockSize, int subBlockNum) {
        ByteBuffer bf = ByteBuffer.allocate(SIGN_HEAD_LEN).order(ByteOrder.LITTLE_ENDIAN);
        for (char c : ELF_MAGIC) {
            bf.put((byte) c);
        }
        for (char c : VERSION) {
            bf.put((byte) c);
        }
        bf.putInt(subBlockSize);
        bf.putInt(subBlockNum);
        for (char c : reserve) {
            bf.put((byte) c);
        }
        return bf.array();
    }
}
