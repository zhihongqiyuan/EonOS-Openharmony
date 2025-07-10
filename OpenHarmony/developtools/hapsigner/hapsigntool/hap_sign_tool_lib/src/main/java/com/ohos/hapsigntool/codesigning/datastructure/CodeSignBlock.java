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

import com.ohos.hapsigntool.codesigning.sign.CodeSigning;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;

/**
 * code sign block is a chunk of bytes attached to hap package or file.
 * It consists of two headers:
 * 1) code sign block header
 * 2) segment header
 * three segments:
 * 1) fs-verity info segment
 * 2) hap info segment
 * 3) so info segment
 * one zero padding area in order to align merkle tree raw bytes
 * 1) zero padding
 * and one area storing merkle tree bytes:
 * 1) merkle tree raw bytes
 * <p>
 * After signing a hap, call toByteArray() method to generate a block of bytes.
 *
 * @since 2023/09/08
 */
public class CodeSignBlock {
    /**
     * page size in bytes
     */
    public static final long PAGE_SIZE_4K = 4096L;

    /**
     * Segment header count, including fs-verity info, hap info, so info segment
     */
    public static final int SEGMENT_HEADER_COUNT = 3;

    private CodeSignBlockHeader codeSignBlockHeader;

    private final List<SegmentHeader> segmentHeaderList;

    private FsVerityInfoSegment fsVerityInfoSegment;

    private HapInfoSegment hapInfoSegment;

    private NativeLibInfoSegment nativeLibInfoSegment;

    private byte[] zeroPadding;

    private final Map<String, byte[]> merkleTreeMap;

    public CodeSignBlock() {
        this.codeSignBlockHeader = new CodeSignBlockHeader.Builder().build();
        this.segmentHeaderList = new ArrayList<>();
        this.fsVerityInfoSegment = new FsVerityInfoSegment();
        this.hapInfoSegment = new HapInfoSegment();
        this.nativeLibInfoSegment = new NativeLibInfoSegment.Builder().build();
        this.merkleTreeMap = new HashMap<>();
    }

    /**
     * Add one merkle tree into merkleTreeMap
     *
     * @param key        file name
     * @param merkleTree merkle tree raw bytes
     */
    public void addOneMerkleTree(String key, byte[] merkleTree) {
        if (merkleTree == null) {
            this.merkleTreeMap.put(key, new byte[0]);
        } else {
            this.merkleTreeMap.put(key, merkleTree);
        }
    }

    /**
     * Get one merkle tree from merkleTreeMap by file name
     *
     * @param key file name
     * @return merkle tree bytes
     */
    public byte[] getOneMerkleTreeByFileName(String key) {
        return this.merkleTreeMap.get(key);
    }

    /**
     * set code sign block flag
     */
    public void setCodeSignBlockFlag() {
        int flags = CodeSignBlockHeader.FLAG_MERKLE_TREE_INLINED;
        if (this.nativeLibInfoSegment.getSectionNum() != 0) {
            flags += CodeSignBlockHeader.FLAG_NATIVE_LIB_INCLUDED;
        }
        this.codeSignBlockHeader.setFlags(flags);
    }

    /**
     * set segmentNum defined in code sign block header, equals length of segmentHeaderList
     */
    public void setSegmentNum() {
        this.codeSignBlockHeader.setSegmentNum(segmentHeaderList.size());
    }

    /**
     * add one segment to segmentHeaderList
     *
     * @param sh segment header
     */
    public void addToSegmentList(SegmentHeader sh) {
        this.segmentHeaderList.add(sh);
    }

    public List<SegmentHeader> getSegmentHeaderList() {
        return segmentHeaderList;
    }

    /**
     * set segment header list
     */
    public void setSegmentHeaders() {
        // fs-verity info segment
        segmentHeaderList.add(new SegmentHeader(SegmentHeader.CSB_FSVERITY_INFO_SEG, this.fsVerityInfoSegment.size()));
        // hap info segment
        segmentHeaderList.add(new SegmentHeader(SegmentHeader.CSB_HAP_META_SEG, this.hapInfoSegment.size()));
        // native lib info segment
        segmentHeaderList.add(
            new SegmentHeader(SegmentHeader.CSB_NATIVE_LIB_INFO_SEG, this.nativeLibInfoSegment.size()));
    }

    public CodeSignBlockHeader getCodeSignBlockHeader() {
        return codeSignBlockHeader;
    }

    public void setCodeSignBlockHeader(CodeSignBlockHeader csbHeader) {
        this.codeSignBlockHeader = csbHeader;
    }

    public void setFsVerityInfoSegment(FsVerityInfoSegment fsVeritySeg) {
        this.fsVerityInfoSegment = fsVeritySeg;
    }

    public FsVerityInfoSegment getFsVerityInfoSegment() {
        return fsVerityInfoSegment;
    }

    public HapInfoSegment getHapInfoSegment() {
        return hapInfoSegment;
    }

    public void setHapInfoSegment(HapInfoSegment hapSeg) {
        this.hapInfoSegment = hapSeg;
    }

    public NativeLibInfoSegment getSoInfoSegment() {
        return nativeLibInfoSegment;
    }

    public void setSoInfoSegment(NativeLibInfoSegment soSeg) {
        this.nativeLibInfoSegment = soSeg;
    }

    /**
     * Convert code sign block object to a newly created byte array
     *
     * @return Byte array representation of a CodeSignBlock object
     */
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(this.codeSignBlockHeader.getBlockSize()).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(this.codeSignBlockHeader.toByteArray());
        for (SegmentHeader sh : this.segmentHeaderList) {
            bf.put(sh.toByteArray());
        }
        bf.put(this.zeroPadding);
        // Hap merkle tree
        if (this.hapInfoSegment.getSignInfo().getExtensionByType(MerkleTreeExtension.MERKLE_TREE_INLINED) != null) {
            bf.put(merkleTreeMap.get("Hap"));
        }
        bf.put(this.fsVerityInfoSegment.toByteArray());
        bf.put(this.hapInfoSegment.toByteArray());
        bf.put(this.nativeLibInfoSegment.toByteArray());
        return bf.array();
    }

    /**
     * SegmentOffset is the position of each segment defined in segmentHeaderList,
     * based on the start position of code sign block
     */
    public void computeSegmentOffset() {
        // 1) the first segment is placed after merkle tree
        int segmentOffset = CodeSignBlockHeader.size()
            + this.segmentHeaderList.size() * SegmentHeader.SEGMENT_HEADER_LENGTH + this.zeroPadding.length
            + this.getOneMerkleTreeByFileName(CodeSigning.HAP_SIGNATURE_ENTRY_NAME).length;
        for (SegmentHeader sh : segmentHeaderList) {
            sh.setSegmentOffset(segmentOffset);
            segmentOffset += sh.getSegmentSize();
        }
    }

    /**
     * Compute the offset to store merkle tree raw bytes based on file start
     *
     * @param codeSignBlockOffset offset to store code sign block based on file start
     * @return offset to store merkle tree based on the file start, it includes the codeSignBlockOffset
     */
    public long computeMerkleTreeOffset(long codeSignBlockOffset) {
        long sizeWithoutMerkleTree = CodeSignBlockHeader.size()
            + SEGMENT_HEADER_COUNT * SegmentHeader.SEGMENT_HEADER_LENGTH;
        // add code sign block offset while computing align position for merkle tree
        long residual = (codeSignBlockOffset + sizeWithoutMerkleTree) % PAGE_SIZE_4K;
        if (residual == 0) {
            this.zeroPadding = new byte[0];
        } else {
            this.zeroPadding = new byte[(int) (PAGE_SIZE_4K - residual)];
        }
        return codeSignBlockOffset + sizeWithoutMerkleTree + zeroPadding.length;
    }

    /**
     * Convert CodeSignBlock to bytes
     *
     * @param fsvTreeOffset merkle tree offset
     * @return byte array representing the code sign block
     */
    public byte[] generateCodeSignBlockByte(long fsvTreeOffset) {
        // 1) compute overall block size without merkle tree
        long csbSize = CodeSignBlockHeader.size()
            + (long) this.segmentHeaderList.size() * SegmentHeader.SEGMENT_HEADER_LENGTH + this.zeroPadding.length
            + this.getOneMerkleTreeByFileName(CodeSigning.HAP_SIGNATURE_ENTRY_NAME).length
            + this.fsVerityInfoSegment.size() + this.hapInfoSegment.size() + this.nativeLibInfoSegment.size();
        Extension ext = this.hapInfoSegment.getSignInfo().getExtensionByType(MerkleTreeExtension.MERKLE_TREE_INLINED);
        if (ext instanceof MerkleTreeExtension) {
            MerkleTreeExtension merkleTreeExtension = (MerkleTreeExtension) ext;
            merkleTreeExtension.setMerkleTreeOffset(fsvTreeOffset);
        }
        this.codeSignBlockHeader.setBlockSize(csbSize);
        // 2) generate byte array of complete code sign block
        return toByteArray();
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        return String.format(Locale.ROOT,
            "CodeSignBlockHeader[%s], SegmentHeaders[%s], FsVeritySeg[%s], HapInfoSeg[%s], SoInfoSeg[%s]",
            this.codeSignBlockHeader, Arrays.toString(this.segmentHeaderList.toArray()), this.fsVerityInfoSegment,
            this.hapInfoSegment, this.nativeLibInfoSegment);
    }
}
