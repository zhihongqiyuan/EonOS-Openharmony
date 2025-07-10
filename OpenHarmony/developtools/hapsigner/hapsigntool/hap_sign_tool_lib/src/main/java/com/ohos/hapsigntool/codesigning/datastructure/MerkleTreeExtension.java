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
import com.ohos.hapsigntool.codesigning.utils.NumberUtils;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.Locale;

/**
 * Merkle tree extension is a type of Extension to store a merkle tree's information, i.e. size and root hash, ect.
 * <p>
 * structure
 * <p>
 * 1) u32 type
 * <p>
 * 2) u64 merkleTreeSize: the size of merkle tree
 * <p>
 * 3) u64 merkleTreeOffset: offset of the merkle tree by the start of the file.
 * <p>
 * 4) u8[64] rootHash: merkle tree root hash
 *
 * @since 2023/09/08
 */
public class MerkleTreeExtension extends Extension {
    /**
     * Type of MerkleTreeExtension
     */
    public static final int MERKLE_TREE_INLINED = 0x1;

    /**
     * Byte size of MerkleTreeExtension including merkleTreeSize, offset and root hash.
     */
    public static final int MERKLE_TREE_EXTENSION_DATA_SIZE = 80;

    private static final int ROOT_HASH_SIZE = 64;

    private final long merkleTreeSize;

    private long merkleTreeOffset;

    private byte[] rootHash;

    /**
     * Constructor for MerkleTreeExtension
     *
     * @param merkleTreeSize   Byte array representation of merkle tree
     * @param merkleTreeOffset merkle tree offset based on file start
     * @param rootHash         Root hash of the merkle tree
     */
    public MerkleTreeExtension(long merkleTreeSize, long merkleTreeOffset, byte[] rootHash) {
        super(MERKLE_TREE_INLINED, MERKLE_TREE_EXTENSION_DATA_SIZE);
        this.merkleTreeSize = merkleTreeSize;
        this.merkleTreeOffset = merkleTreeOffset;
        if (rootHash == null) {
            this.rootHash = new byte[ROOT_HASH_SIZE];
        } else {
            this.rootHash = Arrays.copyOf(rootHash, ROOT_HASH_SIZE);
        }
    }

    @Override
    public int size() {
        return Extension.EXTENSION_HEADER_SIZE + MERKLE_TREE_EXTENSION_DATA_SIZE;
    }

    public long getMerkleTreeSize() {
        return merkleTreeSize;
    }

    public long getMerkleTreeOffset() {
        return merkleTreeOffset;
    }

    public void setMerkleTreeOffset(long offset) {
        this.merkleTreeOffset = offset;
    }

    /**
     * Converts MerkleTreeExtension to a newly created byte array
     *
     * @return Byte array representation of MerkleTreeExtension
     */
    @Override
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(size()).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(super.toByteArray());
        bf.putLong(this.merkleTreeSize);
        bf.putLong(this.merkleTreeOffset);
        bf.put(this.rootHash);
        return bf.array();
    }

    /**
     * Init the MerkleTreeExtension by a byte array
     *
     * @param bytes Byte array representation of a MerkleTreeExtension object
     * @return a newly created MerkleTreeExtension object
     * @throws VerifyCodeSignException parsing result invalid
     */
    public static MerkleTreeExtension fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        bf.rewind();
        long inMerkleTreeSize = bf.getLong();
        if (!NumberUtils.isMultiple4K(inMerkleTreeSize)) {
            throw new VerifyCodeSignException("merkleTreeSize is not a multiple of 4096");
        }
        long inMerkleTreeOffset = bf.getLong();
        if (!NumberUtils.isMultiple4K(inMerkleTreeOffset)) {
            throw new VerifyCodeSignException("merkleTreeOffset is not a aligned to 4096");
        }
        byte[] inRootHash = new byte[ROOT_HASH_SIZE];
        bf.get(inRootHash);
        return new MerkleTreeExtension(inMerkleTreeSize, inMerkleTreeOffset, inRootHash);
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    @Override
    public String toString() {
        return String.format(Locale.ROOT, "MerkleTreeExtension: merkleTreeSize[%d], merkleTreeOffset[%d],"
                + " rootHash[%s]", this.merkleTreeSize, this.merkleTreeOffset, Arrays.toString(this.rootHash));
    }
}
