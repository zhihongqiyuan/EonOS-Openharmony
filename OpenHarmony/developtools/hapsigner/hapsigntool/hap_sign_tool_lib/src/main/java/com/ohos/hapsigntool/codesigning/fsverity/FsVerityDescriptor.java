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

package com.ohos.hapsigntool.codesigning.fsverity;

import com.ohos.hapsigntool.codesigning.exception.CodeSignErrMsg;
import com.ohos.hapsigntool.codesigning.exception.FsVerityDigestException;
import com.ohos.hapsigntool.codesigning.exception.VerifyCodeSignException;
import com.ohos.hapsigntool.codesigning.utils.NumberUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Format of FsVerity descriptor
 * uint8 version
 * uint8 hashAlgorithm
 * uint8 log2BlockSize
 * uint8 saltSize
 * uint32 signSize
 * le64 dataSize
 * uint8[64] rootHash
 * uint8[32] salt
 * uint32 flags
 * uint8[4] 0
 * uint64 treeOffset
 * uint8[127] 0
 * uint8 csVersion
 *
 * @since 2023/06/05
 */
public class FsVerityDescriptor {
    /**
     * fs-verity version, must be 1
     */
    public static final byte VERSION = 1;

    /**
     * page size in bytes
     */
    public static final int PAGE_SIZE_4K = 4096;

    /**
     * Indicating merkle tree offset is set in fs-verity descriptor
     */
    public static final int FLAG_STORE_MERKLE_TREE_OFFSET = 0x1;

    /**
     * Indicating fs-verity descriptor type
     */
    public static final int FS_VERITY_DESCRIPTOR_TYPE = 0x1;

    /**
     * code sign version
     */
    public static final byte CODE_SIGN_VERSION = 0x1;

    /**
     * code sign version
     */
    public static final byte CODE_SIGN_VERSION_V2 = 0x2;

    /**
     * FsVerity descriptor size
     */
    public static final int DESCRIPTOR_SIZE = 256;

    /**
     * root hash size
     */
    public static final int ROOT_HASH_FILED_SIZE = 64;

    /**
     * salt size
     */
    public static final int SALT_SIZE = 32;

    /**
     * reserved size
     */
    public static final int RESERVED_SIZE_AFTER_TREE_OFFSET = 119;

    private byte version;

    private long fileSize;

    private byte hashAlgorithm;

    private byte log2BlockSize;

    private byte saltSize;

    private int signSize;

    private byte[] salt;

    private byte[] rawRootHash;

    private int flags;

    private int bitMapSize;

    private long merkleTreeOffset;

    private long bitMapOffset;

    private byte csVersion;

    private FsVerityDescriptor(Builder builder) {
        this.version = builder.version;
        this.fileSize = builder.fileSize;
        this.hashAlgorithm = builder.hashAlgorithm;
        this.log2BlockSize = builder.log2BlockSize;
        this.saltSize = builder.saltSize;
        this.signSize = builder.signSize;
        this.salt = builder.salt;
        this.rawRootHash = builder.rawRootHash;
        this.flags = builder.flags;
        this.merkleTreeOffset = builder.merkleTreeOffset;
        this.csVersion = builder.csVersion;
    }

    public long getFileSize() {
        return fileSize;
    }

    public long getMerkleTreeOffset() {
        return merkleTreeOffset;
    }

    public int getSignSize() {
        return signSize;
    }

    /**
     * Init the FsVerityDescriptor by a byte array
     *
     * @param bytes Byte array representation of a FsVerityDescriptor object
     * @return a newly created FsVerityDescriptor object
     * @throws VerifyCodeSignException parse result invalid
     */
    public static FsVerityDescriptor fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        // after put, rewind is mandatory before get
        bf.rewind();
        FsVerityDescriptor.Builder builder = new FsVerityDescriptor.Builder();
        byte inFsVersion = bf.get();
        if (inFsVersion != FsVerityDescriptor.VERSION) {
            throw new VerifyCodeSignException("Invalid fs-verify descriptor version of ElfSignBlock");
        }
        byte inFsHashAlgorithm = bf.get();
        byte inLog2BlockSize = bf.get();
        builder.setVersion(inFsVersion).setHashAlgorithm(inFsHashAlgorithm).setLog2BlockSize(inLog2BlockSize);
        byte inSaltSize = bf.get();
        int inSignSize = bf.getInt();
        long inDataSize = bf.getLong();
        byte[] inRootHash = new byte[FsVerityDescriptor.ROOT_HASH_FILED_SIZE];
        bf.get(inRootHash);
        builder.setSaltSize(inSaltSize).setSignSize(inSignSize).setFileSize(inDataSize).setRawRootHash(inRootHash);
        byte[] inSalt = new byte[FsVerityDescriptor.SALT_SIZE];
        bf.get(inSalt);
        int inFlags = bf.getInt();
        bf.getInt();
        long inTreeOffset = bf.getLong();
        if (!NumberUtils.isMultiple4K(inTreeOffset)) {
            throw new VerifyCodeSignException("Invalid merkle tree offset of ElfSignBlock");
        }
        bf.get(new byte[FsVerityDescriptor.RESERVED_SIZE_AFTER_TREE_OFFSET]);
        byte inCsVersion = bf.get();
        builder.setSalt(inSalt).setFlags(inFlags).setMerkleTreeOffset(inTreeOffset).setCsVersion(inCsVersion);
        return builder.build();
    }

    /**
     * Get FsVerity descriptor bytes
     *
     * @return bytes of descriptor
     * @throws FsVerityDigestException if error
     */
    public byte[] toByteArray() throws FsVerityDigestException {
        ByteBuffer buffer = ByteBuffer.allocate(DESCRIPTOR_SIZE).order(ByteOrder.LITTLE_ENDIAN);
        buffer.put(VERSION);
        buffer.put(hashAlgorithm);
        buffer.put(log2BlockSize);
        if (this.saltSize > SALT_SIZE) {
            throw new FsVerityDigestException(CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Salt is too long"));
        }
        buffer.put(this.saltSize);
        buffer.putInt(signSize);
        buffer.putLong(fileSize);
        writeBytesWithSize(buffer, rawRootHash, ROOT_HASH_FILED_SIZE);
        writeBytesWithSize(buffer, salt, SALT_SIZE);
        buffer.putInt(flags);
        buffer.putInt(0);
        buffer.putLong(merkleTreeOffset);
        buffer.putLong(0);
        writeBytesWithSize(buffer, null, RESERVED_SIZE_AFTER_TREE_OFFSET);
        buffer.put(csVersion);
        return buffer.array();
    }

    /**
     * Get bytes for generate digest, first byte is CODE_SIGN_VERSION, sign size is 0, last 128 bytes is 0
     *
     * @return bytes of descriptor
     * @throws FsVerityDigestException if error
     */
    public byte[] getDiscByte() throws FsVerityDigestException {
        ByteBuffer buffer = ByteBuffer.allocate(DESCRIPTOR_SIZE).order(ByteOrder.LITTLE_ENDIAN);
        buffer.put(CODE_SIGN_VERSION);
        buffer.put(hashAlgorithm);
        buffer.put(log2BlockSize);
        if (this.saltSize > SALT_SIZE) {
            throw new FsVerityDigestException(CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Salt is too long"));
        }
        buffer.put(this.saltSize);
        buffer.putInt(0);
        buffer.putLong(fileSize);
        writeBytesWithSize(buffer, rawRootHash, ROOT_HASH_FILED_SIZE);
        writeBytesWithSize(buffer, salt, SALT_SIZE);
        buffer.putInt(flags);
        buffer.putInt(0);
        buffer.putLong(merkleTreeOffset);
        return buffer.array();
    }

    /**
     * Get bytes for generate digest, cs_version 2
     *
     * @param mapOffset bit map data offset at file
     * @param mapSize bit map size
     * @param unitSize bit map unit size corresponding to each page
     * @return bytes of descriptor
     * @throws FsVerityDigestException if error
     */
    public byte[] getDiscByteCsv2(long mapOffset, long mapSize, byte unitSize) throws FsVerityDigestException {
        ByteBuffer buffer = ByteBuffer.allocate(DESCRIPTOR_SIZE).order(ByteOrder.LITTLE_ENDIAN);
        buffer.put(VERSION);
        buffer.put(hashAlgorithm);
        buffer.put(log2BlockSize);
        if (this.saltSize > SALT_SIZE) {
            throw new FsVerityDigestException(CodeSignErrMsg.CODE_SIGN_INTERNAL_ERROR.toString("Salt is too long"));
        }
        buffer.put(this.saltSize);
        buffer.putInt(0);
        buffer.putLong(fileSize);
        writeBytesWithSize(buffer, rawRootHash, ROOT_HASH_FILED_SIZE);
        writeBytesWithSize(buffer, salt, SALT_SIZE);
        buffer.putInt((unitSize << 1 | flags));
        buffer.putInt((int) mapSize);
        buffer.putLong(merkleTreeOffset);
        buffer.putLong(mapOffset);
        writeBytesWithSize(buffer, null, RESERVED_SIZE_AFTER_TREE_OFFSET);
        buffer.put(CODE_SIGN_VERSION_V2);
        return buffer.array();
    }

    /**
     * Write bytes to ByteBuffer with specific size
     *
     * @param buffer target buffer
     * @param src    bytes to write
     * @param size   size of written bytes, fill 0 if src bytes is long enough
     */
    private void writeBytesWithSize(ByteBuffer buffer, byte[] src, int size) {
        int pos = buffer.position();
        if (src != null) {
            if (src.length > size) {
                buffer.put(src, 0, size);
            } else {
                buffer.put(src);
            }
        }
        buffer.position(pos + size);
    }

    /**
     * Builder of FsVerityDescriptor class
     */
    public static class Builder {
        private byte version = VERSION;

        private long fileSize;

        private byte hashAlgorithm;

        private byte log2BlockSize;

        private byte saltSize;

        private int signSize;

        private byte[] salt;

        private byte[] rawRootHash;

        private int flags;

        private int bitMapSize;

        private long merkleTreeOffset;

        private long bitMapOffset;

        private byte csVersion;

        public Builder setVersion(byte version) {
            this.version = version;
            return this;
        }

        public Builder setFileSize(long fileSize) {
            this.fileSize = fileSize;
            return this;
        }

        public Builder setHashAlgorithm(byte hashAlgorithm) {
            this.hashAlgorithm = hashAlgorithm;
            return this;
        }

        public Builder setLog2BlockSize(byte log2BlockSize) {
            this.log2BlockSize = log2BlockSize;
            return this;
        }

        public Builder setSignSize(int signSize) {
            this.signSize = signSize;
            return this;
        }

        public Builder setSaltSize(byte saltSize) {
            this.saltSize = saltSize;
            return this;
        }

        public Builder setSalt(byte[] salt) {
            this.salt = salt;
            return this;
        }

        public Builder setRawRootHash(byte[] rawRootHash) {
            this.rawRootHash = rawRootHash;
            return this;
        }

        public Builder setFlags(int flags) {
            this.flags = flags;
            return this;
        }

        public Builder setMerkleTreeOffset(long merkleTreeOffset) {
            this.merkleTreeOffset = merkleTreeOffset;
            return this;
        }

        public Builder setCsVersion(byte csVersion) {
            this.csVersion = csVersion;
            return this;
        }

        /**
         * Create a FsVerityDescriptor object
         *
         * @return a FsVerityDescriptor object
         */
        public FsVerityDescriptor build() {
            return new FsVerityDescriptor(this);
        }
    }
}
