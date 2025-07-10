/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * define class of hap signature sub-block head
 *
 * @since 2023/11/07
 */
public class BlockHead {
    /**
     * bin file sign block length is 8 byte
     */
    public static final int BLOCK_LEN = 8;

    /**
     * elf file sign block length is 12 byte
     */
    public static final int ELF_BLOCK_LEN = 12;

    private static final int BIT_SIZE = 8;

    private static final int DOUBLE_BIT_SIZE = 16;

    private static final int TRIPLE_BIT_SIZE = 24;

    /**
     * get bin block length
     *
     * @return return bin block length
     */
    public static int getBlockLen() {
        return BLOCK_LEN;
    }

    /**
     * get elf block length
     *
     * @return return elf block length
     */
    public static int getElfBlockLen() {
        return ELF_BLOCK_LEN;
    }

    /**
     * get serialization of file type bin BlockHead
     *
     * @param type type of signature block
     * @param tag tags of signature block
     * @param length the length of block data
     * @param offset Byte offset of the data block relative to the start position of the signature block
     * @return Byte array after serialization of HwBlockHead
     */
    public static byte[] getBlockHead(char type, char tag, short length, int offset) {
        return new byte[] {
            (byte) (type),
            (byte) (tag),
            (byte) ((length >> BIT_SIZE) & 0xff),
            (byte) (length & 0xff),
            (byte) ((offset >> TRIPLE_BIT_SIZE) & 0xff),
            (byte) ((offset >> DOUBLE_BIT_SIZE) & 0xff),
            (byte) ((offset >> BIT_SIZE) & 0xff),
            (byte) (offset & 0xff)
        };
    }

    /**
     * get serialization of file type elf BlockHead
     *
     * @param type type of signature block
     * @param tag tags of signature block
     * @param length the length of block data
     * @param offset Byte offset of the data block relative to the start position of the signature block
     * @return Byte array after serialization of HwBlockHead
     */
    public static byte[] getBlockHeadLittleEndian(char type, char tag, int length, int offset) {
        ByteBuffer bf = ByteBuffer.allocate(BlockHead.ELF_BLOCK_LEN).order(ByteOrder.LITTLE_ENDIAN);
        bf.putChar(type);
        bf.putChar(tag);
        bf.putInt(length);
        bf.putInt(offset);
        return bf.array();
    }
}