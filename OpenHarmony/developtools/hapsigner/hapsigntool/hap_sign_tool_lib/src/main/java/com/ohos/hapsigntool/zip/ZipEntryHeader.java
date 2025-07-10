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

import com.ohos.hapsigntool.error.ZipException;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;

/**
 * resolve zip ZipEntryHeader data
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
 * @since 2023/12/02
 */
public class ZipEntryHeader {
    /**
     * ZipEntryHeader invariable bytes length
     */
    public static final int HEADER_LENGTH = 30;

    /**
     * 4 bytes , entry header signature
     */
    public static final int SIGNATURE = 0x04034b50;

    /**
     * 2 bytes
     */
    private short version;

    /**
     * 2 bytes
     */
    private short flag;

    /**
     * 2 bytes
     */
    private short method;

    /**
     * 2 bytes
     */
    private short lastTime;

    /**
     * 2 bytes
     */
    private short lastDate;

    /**
     * 4 bytes
     */
    private int crc32;

    /**
     * 4 bytes
     */
    private long compressedSize;

    /**
     * 4 bytes
     */
    private long unCompressedSize;

    /**
     * 2 bytes
     */
    private int fileNameLength;

    /**
     * 2 bytes
     */
    private int extraLength;

    /**
     * n bytes
     */
    private String fileName;

    /**
     * n bytes
     */
    private byte[] extraData;

    private int length;

    /**
     * updateLength
     */
    public void updateLength() {
        length = HEADER_LENGTH + fileNameLength + extraLength;
    }

    /**
     * get Zip Entry Header
     *
     * @param bytes ZipEntryHeader bytes
     * @return ZipEntryHeader
     * @throws ZipException read entry header exception
     */
    public static ZipEntryHeader getZipEntryHeader(byte[] bytes) throws ZipException {
        ZipEntryHeader entryHeader = new ZipEntryHeader();
        ByteBuffer bf = ByteBuffer.wrap(bytes);
        bf.order(ByteOrder.LITTLE_ENDIAN);
        if (bf.getInt() != ZipEntryHeader.SIGNATURE) {
            throw new ZipException("find zip entry head failed");
        }
        entryHeader.setVersion(bf.getShort());
        entryHeader.setFlag(bf.getShort());
        entryHeader.setMethod(bf.getShort());
        entryHeader.setLastTime(bf.getShort());
        entryHeader.setLastDate(bf.getShort());
        entryHeader.setCrc32(bf.getInt());
        entryHeader.setCompressedSize(UnsignedDecimalUtil.getUnsignedInt(bf));
        entryHeader.setUnCompressedSize(UnsignedDecimalUtil.getUnsignedInt(bf));
        entryHeader.setFileNameLength(UnsignedDecimalUtil.getUnsignedShort(bf));
        entryHeader.setExtraLength(UnsignedDecimalUtil.getUnsignedShort(bf));
        entryHeader.updateLength();
        return entryHeader;
    }

    /**
     * set entry header name
     *
     * @param bytes name bytes
     */
    public void readFileName(byte[] bytes) {
        ByteBuffer bf = ByteBuffer.wrap(bytes);
        bf.order(ByteOrder.LITTLE_ENDIAN);
        if (fileNameLength > 0) {
            byte[] nameBytes = new byte[fileNameLength];
            bf.get(nameBytes);
            this.fileName = new String(nameBytes, StandardCharsets.UTF_8);
        }
    }

    /**
     * set entry header  extra
     *
     * @param bytes extra bytes
     */
    public void readExtra(byte[] bytes) {
        ByteBuffer bf = ByteBuffer.wrap(bytes);
        bf.order(ByteOrder.LITTLE_ENDIAN);
        if (extraLength > 0) {
            byte[] extra = new byte[extraLength];
            bf.get(extra);
            this.extraData = extra;
        }
    }

    /**
     * change Zip Entry Header to bytes
     *
     * @return bytes
     */
    public byte[] toBytes() {
        ByteBuffer bf = ByteBuffer.allocate(length).order(ByteOrder.LITTLE_ENDIAN);
        bf.putInt(SIGNATURE);
        bf.putShort(version);
        bf.putShort(flag);
        bf.putShort(method);
        bf.putShort(lastTime);
        bf.putShort(lastDate);
        bf.putInt(crc32);
        UnsignedDecimalUtil.setUnsignedInt(bf, compressedSize);
        UnsignedDecimalUtil.setUnsignedInt(bf, unCompressedSize);
        UnsignedDecimalUtil.setUnsignedShort(bf, fileNameLength);
        UnsignedDecimalUtil.setUnsignedShort(bf, extraLength);
        if (fileNameLength > 0) {
            bf.put(fileName.getBytes(StandardCharsets.UTF_8));
        }
        if (extraLength > 0) {
            bf.put(extraData);
        }
        return bf.array();
    }

    public static int getHeaderLength() {
        return HEADER_LENGTH;
    }

    public static int getSIGNATURE() {
        return SIGNATURE;
    }

    public short getVersion() {
        return version;
    }

    public void setVersion(short version) {
        this.version = version;
    }

    public short getFlag() {
        return flag;
    }

    public void setFlag(short flag) {
        this.flag = flag;
    }

    public short getMethod() {
        return method;
    }

    public void setMethod(short method) {
        this.method = method;
    }

    public short getLastTime() {
        return lastTime;
    }

    public void setLastTime(short lastTime) {
        this.lastTime = lastTime;
    }

    public short getLastDate() {
        return lastDate;
    }

    public void setLastDate(short lastDate) {
        this.lastDate = lastDate;
    }

    public int getCrc32() {
        return crc32;
    }

    public void setCrc32(int crc32) {
        this.crc32 = crc32;
    }

    public long getCompressedSize() {
        return compressedSize;
    }

    public void setCompressedSize(long compressedSize) {
        this.compressedSize = compressedSize;
    }

    public long getUnCompressedSize() {
        return unCompressedSize;
    }

    public void setUnCompressedSize(long unCompressedSize) {
        this.unCompressedSize = unCompressedSize;
    }

    public int getFileNameLength() {
        return fileNameLength;
    }

    public void setFileNameLength(int fileNameLength) {
        this.fileNameLength = fileNameLength;
    }

    public int getExtraLength() {
        return extraLength;
    }

    public void setExtraLength(int extraLength) {
        this.extraLength = extraLength;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public byte[] getExtraData() {
        return extraData;
    }

    public void setExtraData(byte[] extraData) {
        this.extraData = extraData;
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
    }
}