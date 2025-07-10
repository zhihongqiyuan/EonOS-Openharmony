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

import com.ohos.hapsigntool.codesigning.exception.FsVerityDigestException;
import com.ohos.hapsigntool.codesigning.exception.VerifyCodeSignException;
import com.ohos.hapsigntool.codesigning.fsverity.FsVerityDescriptor;
import com.ohos.hapsigntool.codesigning.fsverity.FsVerityDescriptorWithSign;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * elf sign block is a chunk of bytes attached to elf file.
 * 1) u32 type: 0x2 merkle tree
 * 2) u32 length: merkle tree with padding size
 * 3) u8[] merkle tree data
 * 4) u32 type: 0x1 fsverity descriptor
 * 5) u32 length: fsverity descriptor size
 * 6) u8 version: fs-verity version
 * 7) u8 hashAlgorithm: hash algorithm to use for the Merkle tree
 * 8) u8 log2BlockSize: log2 of size of data and tree blocks
 * 9) u8 saltSize: byte size of salt
 * 10) u32 signSize: byte size of signature
 * 11) u64 dataSize: byte size of data being signed
 * 12) u8[64] rootHash: merkle tree root hash
 * 13) u8[32] salt: salt used in signing
 * 14) u32 flags
 * 15) u32 reserved
 * 16) u64 treeOffset: merkle tree offset
 * 17) u8[127] reserved
 * 18) u8 csVersion: code sign version
 * 19) u8[] signature: signature after signing the data in byte array representation
 *
 * @since 2023/09/08
 */
public class ElfSignBlock {
    /**
     * page size in bytes
     */
    public static final int PAGE_SIZE_4K = 4096;

    /**
     * Type of MerkleTree
     */
    public static final int MERKLE_TREE_INLINED = 0x2;

    private int type = MERKLE_TREE_INLINED;

    private int treeLength;

    private byte[] merkleTreeWithPadding;

    private FsVerityDescriptorWithSign descriptorWithSign;

    /**
     * Constructor of ElfSignBlock
     *
     * @param paddingSize        padding size before merkle tree
     * @param merkleTreeData     merkle tree data
     * @param descriptorWithSign FsVerityDescriptorWithSign object
     */
    public ElfSignBlock(int paddingSize, byte[] merkleTreeData, FsVerityDescriptorWithSign descriptorWithSign) {
        byte[] inMerkleTreeData = new byte[0];
        if (merkleTreeData != null) {
            inMerkleTreeData = merkleTreeData;
        }
        this.treeLength = paddingSize + inMerkleTreeData.length;
        this.merkleTreeWithPadding = new byte[this.treeLength];
        System.arraycopy(inMerkleTreeData, 0, merkleTreeWithPadding, paddingSize, inMerkleTreeData.length);
        this.descriptorWithSign = descriptorWithSign;
    }

    private ElfSignBlock(int type, int treeLength, byte[] merkleTreeWithPadding,
        FsVerityDescriptorWithSign descriptorWithSign) {
        this.type = type;
        this.treeLength = treeLength;
        this.merkleTreeWithPadding = merkleTreeWithPadding;
        this.descriptorWithSign = descriptorWithSign;
    }

    /**
     * Return the byte size of code sign block
     *
     * @return byte size of code sign block
     */
    public int size() {
        return Integer.BYTES * 2 + merkleTreeWithPadding.length + descriptorWithSign.size();
    }

    /**
     * return padding length by the sign block offset
     *
     * @param signBlockOffset sign block offset based on the start of file
     * @return merkle tree raw bytes offset based on the start of file
     */
    public static int computeMerkleTreePaddingLength(long signBlockOffset) {
        return (int) (PAGE_SIZE_4K - (signBlockOffset + Integer.BYTES * 2) % PAGE_SIZE_4K) % PAGE_SIZE_4K;
    }

    public byte[] getMerkleTreeWithPadding() {
        return merkleTreeWithPadding;
    }

    /**
     * get DataSize
     *
     * @return DataSize
     */
    public long getDataSize() {
        return descriptorWithSign.getFsVerityDescriptor().getFileSize();
    }

    /**
     * get TreeOffset
     *
     * @return TreeOffset
     */
    public long getTreeOffset() {
        return descriptorWithSign.getFsVerityDescriptor().getMerkleTreeOffset();
    }

    /**
     * get Signature
     *
     * @return Signature
     */
    public byte[] getSignature() {
        return descriptorWithSign.getSignature();
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     * @throws FsVerityDigestException if error
     */
    public byte[] toByteArray() throws FsVerityDigestException {
        ByteBuffer bf = ByteBuffer.allocate(size()).order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(type);
        bf.putInt(merkleTreeWithPadding.length);
        bf.put(merkleTreeWithPadding);
        bf.put(descriptorWithSign.toByteArray());
        return bf.array();
    }

    /**
     * Init the ElfSignBlock by a byte array
     *
     * @param bytes Byte array representation of a ElfSignBlock object
     * @return a newly created ElfSignBlock object
     * @throws VerifyCodeSignException parse result invalid
     */
    public static ElfSignBlock fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        // after put, rewind is mandatory before get
        bf.rewind();
        int inTreeType = bf.getInt();
        if (MERKLE_TREE_INLINED != inTreeType) {
            throw new VerifyCodeSignException("Invalid merkle tree type of ElfSignBlock");
        }
        int inTreeLength = bf.getInt();
        byte[] treeWithPadding = new byte[inTreeLength];
        bf.get(treeWithPadding);
        int inFsdType = bf.getInt();
        if (FsVerityDescriptor.FS_VERITY_DESCRIPTOR_TYPE != inFsdType) {
            throw new VerifyCodeSignException("Invalid fs-verify descriptor type of ElfSignBlock");
        }
        int inFsdLength = bf.getInt();
        if (bytes.length != Integer.BYTES * 2 + inTreeLength + Integer.BYTES * 2 + inFsdLength) {
            throw new VerifyCodeSignException("Invalid fs-verify descriptor with signature length of ElfSignBlock");
        }
        byte[] fsdArray = new byte[FsVerityDescriptor.DESCRIPTOR_SIZE];
        bf.get(fsdArray);
        FsVerityDescriptor fsd = FsVerityDescriptor.fromByteArray(fsdArray);
        if (inFsdLength != fsd.getSignSize() + FsVerityDescriptor.DESCRIPTOR_SIZE) {
            throw new VerifyCodeSignException("Invalid sign size of ElfSignBlock");
        }
        byte[] inSignature = new byte[inFsdLength - FsVerityDescriptor.DESCRIPTOR_SIZE];
        bf.get(inSignature);
        FsVerityDescriptorWithSign fsVerityDescriptorWithSign = new FsVerityDescriptorWithSign(inFsdType, inFsdLength,
            fsd, inSignature);
        return new ElfSignBlock(inTreeType, inTreeLength, treeWithPadding, fsVerityDescriptorWithSign);
    }
}
