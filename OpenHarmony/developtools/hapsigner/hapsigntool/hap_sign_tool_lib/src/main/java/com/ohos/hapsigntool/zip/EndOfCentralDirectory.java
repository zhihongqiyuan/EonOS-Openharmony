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

package com.ohos.hapsigntool.zip;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Optional;

/**
 * resolve zip EndOfCentralDirectory data
 * EndOfCentralDirectory format for:
 * end of central dir signature    4 bytes  (0x06054b50)
 * number of this disk             2 bytes
 * number of the disk with the
 * start of the central directory  2 bytes
 * total number of entries in the
 * central directory on this disk  2 bytes
 * total number of entries in
 * the central directory           2 bytes
 * size of the central directory   4 bytes
 * offset of start of central
 * directory with respect to
 * the starting disk number        4 bytes
 * .ZIP file comment length        2 bytes
 * .ZIP file comment       (variable size)
 *
 * @since 2023/12/04
 */
public class EndOfCentralDirectory {
    /**
     * EndOfCentralDirectory invariable bytes length
     */
    public static final int EOCD_LENGTH = 22;

    /**
     * 4 bytes , central directory signature
     */
    public static final int SIGNATURE = 0x06054b50;

    /**
     * 2 bytes
     */
    private int diskNum;

    /**
     * 2 bytes
     */
    private int cDStartDiskNum;

    /**
     * 2 bytes
     */
    private int thisDiskCDNum;

    /**
     * 2 bytes
     */
    private int cDTotal;

    /**
     * 4 bytes
     */
    private long cDSize;

    /**
     * 4 bytes
     */
    private long offset;

    /**
     * 2 bytes
     */
    private int commentLength;

    /**
     * n bytes
     */
    private byte[] comment;

    private int length;

    /**
     * init End Of Central Directory, default offset is 0
     *
     * @param bytes End Of Central Directory bytes
     * @return End Of Central Directory
     */
    public static Optional<EndOfCentralDirectory> getEOCDByBytes(byte[] bytes) {
        return getEOCDByBytes(bytes, 0);
    }

    /**
     * init End Of Central Directory
     *
     * @param bytes End Of Central Directory bytes
     * @param offset offset
     * @return End Of Central Directory
     */
    public static Optional<EndOfCentralDirectory> getEOCDByBytes(byte[] bytes, int offset) {
        EndOfCentralDirectory eocd = new EndOfCentralDirectory();
        int remainingDataLen = bytes.length - offset;
        if (remainingDataLen < EOCD_LENGTH) {
            return Optional.empty();
        }
        ByteBuffer bf = ByteBuffer.wrap(bytes, offset, remainingDataLen);
        bf.order(ByteOrder.LITTLE_ENDIAN);
        if (bf.getInt() != SIGNATURE) {
            return Optional.empty();
        }
        eocd.setDiskNum(UnsignedDecimalUtil.getUnsignedShort(bf));
        eocd.setcDStartDiskNum(UnsignedDecimalUtil.getUnsignedShort(bf));
        eocd.setThisDiskCDNum(UnsignedDecimalUtil.getUnsignedShort(bf));
        eocd.setCDTotal(UnsignedDecimalUtil.getUnsignedShort(bf));
        eocd.setCDSize(UnsignedDecimalUtil.getUnsignedInt(bf));
        eocd.setOffset(UnsignedDecimalUtil.getUnsignedInt(bf));
        eocd.setCommentLength(UnsignedDecimalUtil.getUnsignedShort(bf));
        if (bf.remaining() != eocd.getCommentLength()) {
            return Optional.empty();
        }
        if (eocd.getCommentLength() > 0) {
            byte[] readComment = new byte[eocd.getCommentLength()];
            bf.get(readComment);
            eocd.setComment(readComment);
        }
        eocd.setLength(EOCD_LENGTH + eocd.getCommentLength());
        if (bf.remaining() != 0) {
            return Optional.empty();
        }
        return Optional.of(eocd);
    }

    /**
     * change End Of Central Directory to bytes
     *
     * @return bytes
     */
    public byte[] toBytes() {
        ByteBuffer bf = ByteBuffer.allocate(length).order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(SIGNATURE);
        UnsignedDecimalUtil.setUnsignedShort(bf, diskNum);
        UnsignedDecimalUtil.setUnsignedShort(bf, cDStartDiskNum);
        UnsignedDecimalUtil.setUnsignedShort(bf, thisDiskCDNum);
        UnsignedDecimalUtil.setUnsignedShort(bf, cDTotal);
        UnsignedDecimalUtil.setUnsignedInt(bf, cDSize);
        UnsignedDecimalUtil.setUnsignedInt(bf, offset);
        UnsignedDecimalUtil.setUnsignedShort(bf, commentLength);
        if (commentLength > 0) {
            bf.put(comment);
        }
        return bf.array();
    }

    public static int getEocdLength() {
        return EOCD_LENGTH;
    }

    public static int getSIGNATURE() {
        return SIGNATURE;
    }

    public int getDiskNum() {
        return diskNum;
    }

    public void setDiskNum(int diskNum) {
        this.diskNum = diskNum;
    }

    public int getcDStartDiskNum() {
        return cDStartDiskNum;
    }

    public void setcDStartDiskNum(int cDStartDiskNum) {
        this.cDStartDiskNum = cDStartDiskNum;
    }

    public int getThisDiskCDNum() {
        return thisDiskCDNum;
    }

    public void setThisDiskCDNum(int thisDiskCDNum) {
        this.thisDiskCDNum = thisDiskCDNum;
    }

    public int getCDTotal() {
        return cDTotal;
    }

    public void setCDTotal(int cDTotal) {
        this.cDTotal = cDTotal;
    }

    public long getCDSize() {
        return cDSize;
    }

    public void setCDSize(long cDSize) {
        this.cDSize = cDSize;
    }

    public long getOffset() {
        return offset;
    }

    public void setOffset(long offset) {
        this.offset = offset;
    }

    public int getCommentLength() {
        return commentLength;
    }

    public void setCommentLength(int commentLength) {
        this.commentLength = commentLength;
    }

    public byte[] getComment() {
        return comment;
    }

    public void setComment(byte[] comment) {
        this.comment = comment;
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
    }
}