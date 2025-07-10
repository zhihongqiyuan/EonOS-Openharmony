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
 * segment header has three field:
 * <p>
 * u32 type: indicates the type of segment: fs-verity/so/hap info segment
 * <p>
 * u32 segment offset: the segment position based on the start of code sign block
 * <p>
 * u32 segment size: byte size of the segment
 *
 * @since 2023/09/08
 */
public class SegmentHeader {
    /**
     * Byte size of SegmentHeader
     */
    public static final int SEGMENT_HEADER_LENGTH = 12;

    /**
     * Fs-verity segment type
     */
    public static final int CSB_FSVERITY_INFO_SEG = 0x1;

    /**
     * Hap info segment type
     */
    public static final int CSB_HAP_META_SEG = 0x2;

    /**
     * So info segment type
     */
    public static final int CSB_NATIVE_LIB_INFO_SEG = 0x3;

    private final int type;

    private int segmentOffset;

    private final int segmentSize;

    /**
     * Constructor for SegmentHeader
     *
     * @param type        segment type
     * @param segmentSize byte size of the segment
     */
    public SegmentHeader(int type, int segmentSize) {
        this(type, 0, segmentSize);
    }

    /**
     * Constructor for SegmentHeader
     *
     * @param type          segment type
     * @param segmentOffset segment offset based on the start of code sign block
     * @param segmentSize   byte size of segment
     */
    public SegmentHeader(int type, int segmentOffset, int segmentSize) {
        this.type = type;
        this.segmentOffset = segmentOffset;
        this.segmentSize = segmentSize;
    }

    public int getType() {
        return type;
    }

    public void setSegmentOffset(int offset) {
        this.segmentOffset = offset;
    }

    public int getSegmentOffset() {
        return segmentOffset;
    }

    public int getSegmentSize() {
        return segmentSize;
    }

    /**
     * Converts SegmentHeader to a newly created byte array
     *
     * @return Byte array representation of SegmentHeader
     */
    public byte[] toByteArray() {
        ByteBuffer bf = ByteBuffer.allocate(SEGMENT_HEADER_LENGTH).order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(type);
        bf.putInt(segmentOffset);
        bf.putInt(segmentSize);
        return bf.array();
    }

    /**
     * Init the SegmentHeader by a byte array
     *
     * @param bytes Byte array representation of a SegmentHeader object
     * @return a newly created SegmentHeader object
     * @throws VerifyCodeSignException parsing result invalid
     */
    public static SegmentHeader fromByteArray(byte[] bytes) throws VerifyCodeSignException {
        if (bytes.length != SEGMENT_HEADER_LENGTH) {
            throw new VerifyCodeSignException("Invalid size of SegmentHeader");
        }
        ByteBuffer bf = ByteBuffer.allocate(SEGMENT_HEADER_LENGTH).order(ByteOrder.LITTLE_ENDIAN);
        bf.put(bytes);
        bf.rewind();
        int inType = bf.getInt();
        if ((inType != CSB_FSVERITY_INFO_SEG) && (inType != CSB_HAP_META_SEG) && (inType != CSB_NATIVE_LIB_INFO_SEG)) {
            throw new VerifyCodeSignException("Invalid type of SegmentHeader");
        }
        int inSegmentOffset = bf.getInt();
        // segment offset is always larger than the size of CodeSignBlockHeader
        if (inSegmentOffset < CodeSignBlockHeader.size()) {
            throw new VerifyCodeSignException("Invalid segmentOffset of SegmentHeader");
        }
        int inSegmentSize = bf.getInt();
        if (inSegmentSize < 0) {
            throw new VerifyCodeSignException("Invalid segmentSize of SegmentHeader");
        }
        if ((inType == CSB_FSVERITY_INFO_SEG) && (inSegmentSize != FsVerityInfoSegment.FS_VERITY_INFO_SEGMENT_SIZE)) {
            throw new VerifyCodeSignException("Invalid segmentSize of fs-verity SegmentHeader");
        }
        return new SegmentHeader(inType, inSegmentOffset, inSegmentSize);
    }

    /**
     * Return a string representation of the object
     *
     * @return string representation of the object
     */
    public String toString() {
        return String.format(Locale.ROOT, "Segment Header: type=%d, seg_offset = %d, seg_size = %d", this.type,
            this.segmentOffset, this.segmentSize);
    }
}
