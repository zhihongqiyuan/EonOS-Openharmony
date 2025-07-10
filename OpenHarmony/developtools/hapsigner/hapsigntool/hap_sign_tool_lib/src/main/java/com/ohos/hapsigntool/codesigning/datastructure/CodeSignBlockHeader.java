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

import com.ohos.hapsigntool.codesigning.exception.VerifyCodeSignException;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Locale;

/**
 * Code sign block header
 * <p>
 * Structure:
 * 1) u64 magic: magic number
 * 2) u32 version: sign tool version
 * 3) u32 blockSize: size of code sign block
 * 4) u32 segmentNum: number of segments, i.e. FsVerityInfoSegment, HapInfoSegment, SoInfoSegment
 * 5) u32 flags
 * 6) u8[8] reserved: for reservation
 * <p>
 * The Size of Code sign Block header if fixed, getBlockLength() method returns the size.
 *
 * @since 2023/09/08
 */
public class CodeSignBlockHeader {
    /**
     * Flag indicating that merkle tree is included in code sign block
     */
    public static final int FLAG_MERKLE_TREE_INLINED = 0x1;

    /**
     * Flag indicating that native lib is included in code sign block
     */
    public static final int FLAG_NATIVE_LIB_INCLUDED = 0x2;

    // code signing version
    private static final int CODE_SIGNING_VERSION = 1;

    // byte size of magic number
    private static final byte MAGIC_BYTE_ARRAY_LENGTH = Long.BYTES;

    // lower 8 bytes of MD5 result of string "hap code sign block" (E046 C8C6 5389 FCCD)
    private static final long MAGIC_NUM = ((0xE046C8C6L << 32) + 0x5389FCCDL);

    // size of byte[8] reserved
    private static final byte RESERVED_BYTE_ARRAY_LENGTH = 8;

    // At all times three segment are always included in code sign block, update this if new segments are created.
    private static final int SEGMENT_NUM = 3;

    private long magic;

    private int version;

    private int blockSize;

    private int segmentNum;

    private int flags;

    private byte[] reserved;

    /**
     * Construct of CodeSignBlockHeader
     *
     * @param builder builder
     */
    private CodeSignBlockHeader(Builder builder) {
        this.magic = builder.magic;
        this.version = builder.version;
        this.blockSize = builder.blockSize;
        this.segmentNum = builder.segmentNum;
        this.flags = builder.flags;
        this.reserved = builder.reserved;
    }

    public void setSegmentNum(int num) {
        this.segmentNum = num;
    }

    public int getSegmentNum() {
        return segmentNum;
    }

    public void setBlockSize(long size) {
        this.blockSize = (int) size;
    }

    public int getBlockSize() {
        return blockSize;
    }

    public void setFlags(int flags) {
        this.flags = flags;
    }

    /**
     * Converts code sign block headers to a newly created byte array
     *
     * @return Byte array representation of a code sign block header
     */
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(size()).order(ByteOrder.LITTLE_ENDIAN);
        bf.putLong(magic);
        bf.putInt(version);
        bf.putInt(blockSize);
        bf.putInt(segmentNum);
        bf.putInt(flags);
        bf.put(reserved);
        return bf.array();
    }

    /**
     * Init the CodeSignBlockHeader by a byte array
     *
     * @param bytes Byte array representation of a CodeSignBlockHeader object
     * @return a newly created CodeSignBlockHeader object
     * @throws VerifyCodeSignException parse result invalid
     */
    public static CodeSignBlockHeader fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        if (bytes.length != size()) {
            throw new VerifyCodeSignException("Invalid size of CodeSignBlockHeader");
        }
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        // after put, rewind is mandatory before get
        bf.rewind();
        long inMagic = bf.getLong();
        if (inMagic != MAGIC_NUM) {
            throw new VerifyCodeSignException("Invalid magic num of CodeSignBlockHeader");
        }
        int inVersion = bf.getInt();
        if (inVersion != CODE_SIGNING_VERSION) {
            throw new VerifyCodeSignException("Invalid version of CodeSignBlockHeader");
        }
        int inBlockSize = bf.getInt();
        int inSegmentNum = bf.getInt();
        if (inSegmentNum != SEGMENT_NUM) {
            throw new VerifyCodeSignException("Invalid segmentNum of CodeSignBlockHeader");
        }
        int inFlags = bf.getInt();
        if (inFlags < 0 || inFlags > (FLAG_MERKLE_TREE_INLINED + FLAG_NATIVE_LIB_INCLUDED)) {
            throw new VerifyCodeSignException("Invalid flags of CodeSignBlockHeader");
        }
        byte[] inReserved = new byte[RESERVED_BYTE_ARRAY_LENGTH];
        bf.get(inReserved);
        return new Builder().setMagic(inMagic).setVersion(inVersion).setBlockSize(inBlockSize)
            .setSegmentNum(inSegmentNum).setFlags(inFlags).setReserved(inReserved).build();
    }

    /**
     * Return the byte size of code sign block header
     *
     * @return byte size of code sign block header
     */
    public static int size() {
        return MAGIC_BYTE_ARRAY_LENGTH + Integer.BYTES * 4 + RESERVED_BYTE_ARRAY_LENGTH;
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        return String.format(Locale.ROOT,
            "CodeSignBlockHeader{magic: %d, version: %d, blockSize: %d, segmentNum: %d, flags: %d}", this.magic,
            this.version, this.blockSize, this.segmentNum, this.flags);
    }

    /**
     * Builder of CodeSignBlockHeader class
     */
    public static class Builder {
        private long magic = MAGIC_NUM;

        private int version = CODE_SIGNING_VERSION;

        private int blockSize;

        private int segmentNum;

        private int flags;

        private byte[] reserved = new byte[RESERVED_BYTE_ARRAY_LENGTH];

        public Builder setMagic(long magic) {
            this.magic = magic;
            return this;
        }

        public Builder setVersion(int version) {
            this.version = version;
            return this;
        }

        public Builder setBlockSize(int blockSize) {
            this.blockSize = blockSize;
            return this;
        }

        public Builder setSegmentNum(int segmentNum) {
            this.segmentNum = segmentNum;
            return this;
        }

        public Builder setFlags(int flags) {
            this.flags = flags;
            return this;
        }

        public Builder setReserved(byte[] reserved) {
            this.reserved = reserved;
            return this;
        }

        /**
         * Create a CodeSignBlockHeader object
         *
         * @return a CodeSignBlockHeader object
         */
        public CodeSignBlockHeader build() {
            return new CodeSignBlockHeader(this);
        }
    }
}
