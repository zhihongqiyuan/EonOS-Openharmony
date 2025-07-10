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
import com.ohos.hapsigntool.codesigning.fsverity.FsVerityDescriptor;
import com.ohos.hapsigntool.codesigning.fsverity.FsVerityGenerator;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Locale;

/**
 * Fs-verity info segment contains information of fs-verity protection
 * More information of fs-verity can be found <a herf="https://www.kernel.org/doc/html/next/filesystems/fsverity.html">here</a>
 * <p>
 * Structure
 * <p>
 * 1) u32 magic: magic number
 * <p>
 * 2) u8 version: fs-verity version
 * <p>
 * 3) u8 hashAlgorithm: hash algorithm to use for the Merkle tree
 * <p>
 * 4) u8 log2BlockSize: log2 of size of data and tree blocks
 * <p>
 * 5) u8[] reserved: for reservation
 *
 * @since 2023/09/08
 */
public class FsVerityInfoSegment {
    /**
     * fs-verity info segment size in bytes
     */
    public static final int FS_VERITY_INFO_SEGMENT_SIZE = 64;

    // lower 4 bytes of the MD5 result of string "fs-verity info segment" (1E38 31AB)
    private static final int MAGIC = (0x1E38 << 16) + (0x31AB);

    private static final int RESERVED_BYTE_ARRAY_LENGTH = 57;

    private int magic = MAGIC;

    private byte hashAlgorithm;

    private byte version;

    private byte log2BlockSize;

    private byte[] reserved = new byte[RESERVED_BYTE_ARRAY_LENGTH];

    /**
     * Default constructor
     */
    public FsVerityInfoSegment() {
    }

    public FsVerityInfoSegment(byte version, byte hashAlgorithm, byte log2BlockSize) {
        this(MAGIC, version, hashAlgorithm, log2BlockSize, new byte[RESERVED_BYTE_ARRAY_LENGTH]);
    }

    /**
     * Constructor of FsVerityInfoSegment
     *
     * @param magic         magic num
     * @param version       version of fs-verity
     * @param hashAlgorithm hash algorithm to use for the Merkle tree
     * @param log2BlockSize log2 of size of data and tree blocks
     * @param reserved      for reservation
     */
    public FsVerityInfoSegment(int magic, byte version, byte hashAlgorithm, byte log2BlockSize, byte[] reserved) {
        this.magic = magic;
        this.version = version;
        this.hashAlgorithm = hashAlgorithm;
        this.log2BlockSize = log2BlockSize;
        this.reserved = reserved;
    }

    public int size() {
        return FS_VERITY_INFO_SEGMENT_SIZE;
    }

    /**
     * Converts FsVerityInfoSegment to a newly created byte array
     *
     * @return Byte array representation of FsVerityInfoSegment
     */
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(FS_VERITY_INFO_SEGMENT_SIZE).order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(this.magic);
        bf.put(version);
        bf.put(hashAlgorithm);
        bf.put(log2BlockSize);
        bf.put(reserved);
        return bf.array();
    }

    /**
     * Init the FsVerityInfoSegment by a byte array
     *
     * @param bytes Byte array representation of a FsVerityInfoSegment object
     * @return a newly created FsVerityInfoSegment object
     * @throws VerifyCodeSignException parsing result invalid
     */
    public static FsVerityInfoSegment fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        if (bytes.length != FS_VERITY_INFO_SEGMENT_SIZE) {
            throw new VerifyCodeSignException("Invalid size of FsVerityInfoSegment");
        }
        ByteBuffer bf = ByteBuffer.allocate(bytes.length).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        bf.rewind();
        int inMagic = bf.getInt();
        if (inMagic != MAGIC) {
            throw new VerifyCodeSignException("Invalid magic number of FsVerityInfoSegment");
        }
        byte inVersion = bf.get();
        if (inVersion != FsVerityDescriptor.VERSION) {
            throw new VerifyCodeSignException("Invalid version of FsVerityInfoSegment");
        }
        byte inHashAlgorithm = bf.get();
        if (inHashAlgorithm != FsVerityGenerator.getFsVerityHashAlgorithm()) {
            throw new VerifyCodeSignException("Invalid hashAlgorithm of FsVerityInfoSegment");
        }
        byte inLog2BlockSize = bf.get();
        if (inLog2BlockSize != FsVerityGenerator.getLog2BlockSize()) {
            throw new VerifyCodeSignException("Invalid log2BlockSize of FsVerityInfoSegment");
        }
        byte[] inReservedBytes = new byte[RESERVED_BYTE_ARRAY_LENGTH];
        bf.get(inReservedBytes);
        return new FsVerityInfoSegment(inMagic, inVersion, inHashAlgorithm, inLog2BlockSize, inReservedBytes);
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        return String.format(Locale.ROOT, "FsVerityInfoSeg: magic[%d], version[%d], hashAlg[%d], log2BlockSize[%d]",
            this.magic, this.version, this.hashAlgorithm, this.log2BlockSize);
    }
}
