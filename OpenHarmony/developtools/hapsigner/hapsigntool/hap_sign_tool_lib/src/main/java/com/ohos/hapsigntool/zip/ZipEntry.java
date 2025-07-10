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

import java.util.Arrays;
import java.util.Calendar;
import java.util.zip.CRC32;

/**
 * ZipEntry and CentralDirectory data
 *
 * @since 2023/12/02
 */
public class ZipEntry {
    private ZipEntryData zipEntryData;

    private CentralDirectory fileEntryInCentralDirectory;

    /**
     * updateLength
     */
    public void updateLength() {
        zipEntryData.updateLength();
        fileEntryInCentralDirectory.updateLength();
    }

    /**
     * alignment one entry
     *
     * @param alignNum  need align bytes length
     * @return add bytes length
     * @throws ZipException alignment exception
     */
    public int alignment(int alignNum) throws ZipException {
        // if cd extra len bigger than entry extra len, make cd and entry extra length equals
        int padding = calZeroPaddingLengthForEntryExtra();
        int remainder = (int) ((zipEntryData.getZipEntryHeader().getLength()
                + fileEntryInCentralDirectory.getOffset()) % alignNum);

        if (remainder == 0) {
            return padding;
        }
        int add = alignNum - remainder;
        int newExtraLength = zipEntryData.getZipEntryHeader().getExtraLength() + add;
        if (newExtraLength > UnsignedDecimalUtil.MAX_UNSIGNED_SHORT_VALUE) {
            throw new ZipException("can not align " + zipEntryData.getZipEntryHeader().getFileName());
        }
        setEntryHeaderNewExtraLength(newExtraLength);
        setCenterDirectoryNewExtraLength(newExtraLength);

        return add;
    }

    private int calZeroPaddingLengthForEntryExtra() throws ZipException {
        int entryExtraLen = zipEntryData.getZipEntryHeader().getExtraLength();
        int cdExtraLen = fileEntryInCentralDirectory.getExtraLength();
        if (cdExtraLen > entryExtraLen) {
            setEntryHeaderNewExtraLength(cdExtraLen);
            return cdExtraLen - entryExtraLen;
        }
        if (cdExtraLen < entryExtraLen) {
            setCenterDirectoryNewExtraLength(entryExtraLen);
            return entryExtraLen - cdExtraLen;
        }
        return 0;
    }

    private void setCenterDirectoryNewExtraLength(int newLength) throws ZipException {
        byte[] newCDExtra = getAlignmentNewExtra(newLength, fileEntryInCentralDirectory.getExtraData());
        fileEntryInCentralDirectory.setExtraData(newCDExtra);
        fileEntryInCentralDirectory.setExtraLength(newLength);
        fileEntryInCentralDirectory.setLength(CentralDirectory.CD_LENGTH
                + fileEntryInCentralDirectory.getFileNameLength()
                + fileEntryInCentralDirectory.getExtraLength() + fileEntryInCentralDirectory.getCommentLength());
    }

    private void setEntryHeaderNewExtraLength(int newLength) throws ZipException {
        ZipEntryHeader zipEntryHeader = zipEntryData.getZipEntryHeader();
        byte[] newExtra = getAlignmentNewExtra(newLength, zipEntryHeader.getExtraData());
        zipEntryHeader.setExtraData(newExtra);
        zipEntryHeader.setExtraLength(newLength);
        zipEntryHeader.setLength(ZipEntryHeader.HEADER_LENGTH + zipEntryHeader.getExtraLength()
                + zipEntryHeader.getFileNameLength());
        zipEntryData.setLength(zipEntryHeader.getLength() + zipEntryData.getFileSize()
                + (zipEntryData.getDataDescriptor() == null ? 0 : DataDescriptor.DES_LENGTH));
    }

    private byte[] getAlignmentNewExtra(int newLength, byte[] old) throws ZipException {
        if (old == null) {
            return new byte[newLength];
        }
        if (newLength < old.length) {
            throw new ZipException("can not align " + zipEntryData.getZipEntryHeader().getFileName());
        }
        return Arrays.copyOf(old, newLength);
    }

    public ZipEntryData getZipEntryData() {
        return zipEntryData;
    }

    public void setZipEntryData(ZipEntryData zipEntryData) {
        this.zipEntryData = zipEntryData;
    }

    public CentralDirectory getCentralDirectory() {
        return fileEntryInCentralDirectory;
    }

    public void setCentralDirectory(CentralDirectory centralDirectory) {
        this.fileEntryInCentralDirectory = centralDirectory;
    }

    /**
     * zip entry builder
     */
    public static class Builder {
        private short version = 10;

        private short flag = 2048;

        private short method = 0;

        private long compressedSize;

        private long unCompressedSize;

        private String fileName;

        private byte[] extraData;

        private byte[] comment;

        private byte[] data;

        public Builder setVersion(short version) {
            this.version = version;
            return this;
        }

        public Builder setFlag(short flag) {
            this.flag = flag;
            return this;
        }

        public Builder setMethod(short method) {
            this.method = method;
            return this;
        }

        public Builder setCompressedSize(long compressedSize) {
            this.compressedSize = compressedSize;
            return this;
        }

        public Builder setUncompressedSize(long unCompressedSize) {
            this.unCompressedSize = unCompressedSize;
            return this;
        }

        public Builder setFileName(String fileName) {
            this.fileName = fileName;
            return this;
        }

        public Builder setExtraData(byte[] extraData) {
            this.extraData = extraData;
            return this;
        }

        public Builder setComment(byte[] comment) {
            this.comment = comment;
            return this;
        }

        public Builder setData(byte[] data) {
            this.data = data;
            return this;
        }

        /**
         * build zip entry
         *
         * @return Zip Entry
         * @throws ZipException ZipException
         */
        public ZipEntry build() throws ZipException {
            Calendar calendar = Calendar.getInstance();
            // java time stamp to dos timestamp, dos time start 1980
            int time = (calendar.get(Calendar.YEAR) - 1980) << 25 | (calendar.get(Calendar.MONTH) + 1) << 21
                    | calendar.get(Calendar.DAY_OF_MONTH) << 16 | calendar.get(Calendar.HOUR_OF_DAY) << 11
                    | calendar.get(Calendar.MINUTE) << 5 | calendar.get(Calendar.SECOND) >> 1;
            CentralDirectory cd = addCenterDirectory(time);
            ZipEntryHeader zipEntryHeader = addZipEntryHeader(time);
            if (data == null) {
                throw new ZipException("can not find entry data");
            }
            final CRC32 c = new CRC32();
            c.update(data);
            final int crc32 = new Long(c.getValue()).intValue();
            cd.setCrc32(crc32);
            zipEntryHeader.setCrc32(crc32);

            ZipEntryData entryData = new ZipEntryData();
            entryData.setData(data);
            entryData.setZipEntryHeader(zipEntryHeader);
            ZipEntry entry = new ZipEntry();
            entry.setZipEntryData(entryData);
            entryData.setType(EntryType.BIT_MAP);
            entry.setCentralDirectory(cd);
            return entry;
        }

        private CentralDirectory addCenterDirectory(int time) {
            CentralDirectory cd = new CentralDirectory();
            cd.setVersion(version);
            cd.setVersionExtra(version);
            cd.setFlag(flag);
            cd.setMethod(method);
            cd.setLastTime((short) time);
            cd.setLastDate((short) (time >> 16));
            cd.setCompressedSize(compressedSize);
            cd.setUnCompressedSize(unCompressedSize);
            cd.setFileName(fileName);
            cd.setFileNameLength(fileName.length());
            if (extraData != null) {
                cd.setExtraData(extraData);
                cd.setExtraLength(extraData.length);
            } else {
                cd.setExtraLength(0);
            }
            if (comment != null) {
                cd.setComment(comment);
                cd.setCommentLength(comment.length);
            } else {
                cd.setCommentLength(0);
            }
            cd.setDiskNumStart(0);
            cd.setExternalFile(0);

            cd.updateLength();
            return cd;
        }

        private ZipEntryHeader addZipEntryHeader(int time) {
            ZipEntryHeader zipEntryHeader = new ZipEntryHeader();
            zipEntryHeader.setVersion(version);
            zipEntryHeader.setFlag(flag);
            zipEntryHeader.setMethod(method);
            zipEntryHeader.setLastTime((short) time);
            zipEntryHeader.setLastDate((short) (time >> 16));
            zipEntryHeader.setCompressedSize(compressedSize);
            zipEntryHeader.setUnCompressedSize(unCompressedSize);
            zipEntryHeader.setFileName(fileName);
            zipEntryHeader.setFileNameLength(fileName.length());
            if (extraData != null) {
                zipEntryHeader.setExtraData(extraData);
                zipEntryHeader.setExtraLength(extraData.length);
            } else {
                zipEntryHeader.setExtraLength(0);
            }
            zipEntryHeader.updateLength();
            return zipEntryHeader;
        }
    }
}