/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import com.ohos.hapsigntool.entity.Pair;
import com.ohos.hapsigntool.error.HapFormatException;
import com.ohos.hapsigntool.error.SignToolErrMsg;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Utils functions of zip-files.
 *
 * @since 2021/12/22
 */
public class ZipUtils {
    private static final int ZIP_EOCD_SEGMENT_MIN_SIZE = 22;

    private static final int ZIP_EOCD_SEGMENT_FLAG = 0x06054b50;

    private static final int ZIP_CENTRAL_DIR_COUNT_OFFSET_IN_EOCD = 10;

    private static final int ZIP_CENTRAL_DIR_SIZE_OFFSET_IN_EOCD = 12;

    private static final int ZIP_CENTRAL_DIR_OFFSET_IN_EOCD = 16;

    private static final int ZIP_EOCD_COMMENT_LENGTH_OFFSET = 20;

    private static final int ZIP64_EOCD_LOCATOR_SIZE = 20;

    private static final int ZIP64_EOCD_LOCATOR_SIG = 0x07064b50;

    private static final int UINT16_MAX_VALUE = 0xffff;

    private static final long UINT32_MAX_VALUE = 0xffffffffL;

    private static final int ZIP_DATA_SIZE = 4;

    /**
     * Constructor of Method
     */
    private ZipUtils() {
    }

    /**
     * This function find Eocd by searching Eocd flag from input buffer(searchBuffer) and
     * making sure the comment length is equal to the expected value
     *
     * @param searchBuffer data buffer used to search EOCD.
     * @return offset from buffer start point.
     */
    public static int findEocdInSearchBuffer(ByteBuffer searchBuffer) {
        checkBufferIsLittleEndian(searchBuffer);
        /*
         * Eocd format:
         * 4-bytes: End of central directory flag
         * 2-bytes: Number of this disk
         * 2-bytes: Number of the disk with the start of central directory
         * 2-bytes: Total number of entries in the central directory on this disk
         * 2-bytes: Total number of entries in the central directory
         * 4-bytes: Size of central directory
         * 4-bytes: offset of central directory in zip file
         * 2-bytes: ZIP file comment length, the value n is in the range of [0, 65535]
         * n-bytes: ZIP Comment block data
         */
        int searchBufferSize = searchBuffer.capacity();
        if (searchBufferSize < ZIP_EOCD_SEGMENT_MIN_SIZE) {
            return -1;
        }

        int currentOffset = searchBufferSize - ZIP_EOCD_SEGMENT_MIN_SIZE;
        while (currentOffset >= 0) {
            if (searchBuffer.getInt(currentOffset) == ZIP_EOCD_SEGMENT_FLAG) {
                int commentLength = getUInt16FromBuffer(searchBuffer, currentOffset + ZIP_EOCD_COMMENT_LENGTH_OFFSET);
                int expectedCommentLength = searchBufferSize - ZIP_EOCD_SEGMENT_MIN_SIZE - currentOffset;
                if (commentLength == expectedCommentLength) {
                    return currentOffset;
                }
            }
            currentOffset--;
        }
        return -1;
    }

    /**
     * Check whether the zip is zip64 by finding ZIP64 End of Central Directory Locator.
     * ZIP64 End of Central Directory Locator immediately precedes the ZIP End of Central Directory.
     *
     * @param zip object of RandomAccessFile for zip-file.
     * @param zipEocdOffset offset of the ZIP EOCD in the file.
     * @return true, if ZIP64 End of Central Directory Locator is present.
     * @throws IOException read file error.
     */
    public static boolean checkZip64EoCDLocatorIsPresent(ZipDataInput zip, long zipEocdOffset) throws IOException {
        long locatorPos = zipEocdOffset - ZIP64_EOCD_LOCATOR_SIZE;
        if (locatorPos < 0) {
            return false;
        }
        ByteBuffer byteBuffer = zip.createByteBuffer(locatorPos, ZIP_DATA_SIZE);
        byteBuffer.order(ByteOrder.LITTLE_ENDIAN);
        return byteBuffer.getInt() == ZIP64_EOCD_LOCATOR_SIG;
    }

    /**
     * Get offset value of Central Directory from End of Central Directory Record.
     *
     * @param eocd buffer of End of Central Directory Record
     * @return offset value of Central Directory.
     */
    public static long getCentralDirectoryOffset(ByteBuffer eocd) {
        checkBufferIsLittleEndian(eocd);
        return getUInt32FromBuffer(eocd, eocd.position() + ZIP_CENTRAL_DIR_OFFSET_IN_EOCD);
    }

    /**
     * set offset value of Central Directory to End of Central Directory Record.
     *
     * @param eocd buffer of End of Central Directory Record.
     * @param offset offset value of Central Directory.
     */
    public static void setCentralDirectoryOffset(ByteBuffer eocd, long offset) {
        checkBufferIsLittleEndian(eocd);
        setUInt32ToBuffer(eocd, eocd.position() + ZIP_CENTRAL_DIR_OFFSET_IN_EOCD, offset);
    }

    /**
     * Get size of Central Directory from End of Central Directory Record.
     *
     * @param eocd buffer of End of Central Directory Record.
     * @return size of Central Directory.
     */
    public static long getCentralDirectorySize(ByteBuffer eocd) {
        checkBufferIsLittleEndian(eocd);
        return getUInt32FromBuffer(eocd, eocd.position() + ZIP_CENTRAL_DIR_SIZE_OFFSET_IN_EOCD);
    }

    /**
     * Get total count of Central Directory from End of Central Directory Record.
     *
     * @param eocd buffer of End of Central Directory Record.
     * @return size of Central Directory.
     */
    public static int getCentralDirectoryCount(ByteBuffer eocd) {
        checkBufferIsLittleEndian(eocd);
        return getUInt16FromBuffer(eocd, eocd.position() + ZIP_CENTRAL_DIR_COUNT_OFFSET_IN_EOCD);
    }

    private static void checkBufferIsLittleEndian(ByteBuffer buffer) {
        if (buffer.order() == ByteOrder.LITTLE_ENDIAN) {
            return;
        }
        throw new IllegalArgumentException("ByteBuffer is not little endian");
    }

    static int getUInt16FromBuffer(ByteBuffer buffer, int offset) {
        return buffer.getShort(offset) & 0xffff;
    }

    static long getUInt32FromBuffer(ByteBuffer buffer, int offset) {
        return buffer.getInt(offset) & UINT32_MAX_VALUE;
    }

    private static void setUInt32ToBuffer(ByteBuffer buffer, int offset, long value) {
        if ((value < 0) || (value > UINT32_MAX_VALUE)) {
            throw new IllegalArgumentException("uint32 value of out range: " + value);
        }
        buffer.putInt(buffer.position() + offset, (int) value);
    }

    /**
     * Find the key information for parsing the zip file.
     *
     * @param in zip file
     * @return the key information for parsing the zip file.
     * @throws IOException file operation error
     * @throws HapFormatException hap file format error
     */
    public static ZipFileInfo findZipInfo(ZipDataInput in) throws IOException, HapFormatException {
        Pair<Long, ByteBuffer> eocdOffsetAndBuffer = findEocdInHap(in);
        if (eocdOffsetAndBuffer == null) {
            throw new HapFormatException(SignToolErrMsg.ZIP_FORMAT_FAILED
                    .toString("ZIP End of Central Directory not found"));
        }
        long eocdOffset = eocdOffsetAndBuffer.getFirst();
        ByteBuffer eocdBuffer = eocdOffsetAndBuffer.getSecond().order(ByteOrder.LITTLE_ENDIAN);
        long centralDirectoryStartOffset = ZipUtils.getCentralDirectoryOffset(eocdBuffer);
        if (centralDirectoryStartOffset > eocdOffset) {
            throw new HapFormatException(SignToolErrMsg.ZIP_FORMAT_FAILED
                    .toString("ZIP Central Directory start offset(" + centralDirectoryStartOffset
                + ") larger than ZIP End of Central Directory offset(" + eocdOffset + ")"));
        }
        long centralDirectorySizeLong = ZipUtils.getCentralDirectorySize(eocdBuffer);
        if (centralDirectorySizeLong > Integer.MAX_VALUE) {
            throw new HapFormatException(SignToolErrMsg.ZIP_FORMAT_FAILED
                    .toString("ZIP Central Directory out of range: " + centralDirectorySizeLong));
        }
        int centralDirectorySize = (int) centralDirectorySizeLong;
        long centralDirectoryEndOffset = centralDirectoryStartOffset + centralDirectorySizeLong;
        if (centralDirectoryEndOffset != eocdOffset) {
            throw new HapFormatException(SignToolErrMsg.ZIP_FORMAT_FAILED
                    .toString("ZIP Central Directory end offset(" + centralDirectoryEndOffset + ") "
                + " different from ZIP End of Central Directory offset(" + eocdOffset + ")"));
        }
        int centralDirectoryCount = ZipUtils.getCentralDirectoryCount(eocdBuffer);
        return new ZipFileInfo(centralDirectoryStartOffset, centralDirectorySize, centralDirectoryCount, eocdOffset,
                eocdBuffer);
    }

    private static Pair<Long, ByteBuffer> findEocdInHap(ZipDataInput in) throws IOException {
        Pair<Long, ByteBuffer> eocdInHap = findEocdInHap(in, 0);
        if (eocdInHap != null) {
            return eocdInHap;
        }
        return findEocdInHap(in, UINT16_MAX_VALUE);
    }

    private static Pair<Long, ByteBuffer> findEocdInHap(ZipDataInput zip, int maxCommentSize) throws IOException {
        if ((maxCommentSize < 0) || (maxCommentSize > UINT16_MAX_VALUE)) {
            throw new IllegalArgumentException("maxCommentSize: " + maxCommentSize);
        }
        long fileSize = zip.size();
        if (fileSize < ZIP_EOCD_SEGMENT_MIN_SIZE) {
            throw new IllegalArgumentException("file length " + fileSize + " is too smaller");
        }
        int finalMaxCommentSize = (int) Math.min(maxCommentSize, fileSize - ZIP_EOCD_SEGMENT_MIN_SIZE);
        int searchBufferSize = finalMaxCommentSize + ZIP_EOCD_SEGMENT_MIN_SIZE;
        long bufferOffsetInFile = fileSize - searchBufferSize;
        ByteBuffer searchEocdBuffer = zip.createByteBuffer(bufferOffsetInFile, searchBufferSize);
        searchEocdBuffer.order(ByteOrder.LITTLE_ENDIAN);
        int eocdOffsetInSearchBuffer = findEocdInSearchBuffer(searchEocdBuffer);
        if (eocdOffsetInSearchBuffer == -1) {
            return null;
        }
        searchEocdBuffer.position(eocdOffsetInSearchBuffer);
        ByteBuffer eocdBuffer = searchEocdBuffer.slice().order(ByteOrder.LITTLE_ENDIAN);
        return Pair.create(bufferOffsetInFile + eocdOffsetInSearchBuffer, eocdBuffer);
    }
}