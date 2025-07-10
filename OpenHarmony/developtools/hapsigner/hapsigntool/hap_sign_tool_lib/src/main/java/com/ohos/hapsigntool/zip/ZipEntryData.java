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

import com.ohos.hapsigntool.utils.FileUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

/**
 * resolve zip ZipEntry data
 *
 * @since 2023/12/04
 */
public class ZipEntryData {
    /**
     * data descriptor has or not mask
     */
    public static final short HAS_DATA_DESCRIPTOR_MASK = 0x08;

    /**
     * data descriptor has or not flag mask
     */
    public static final short NOT_HAS_DATA_DESCRIPTOR_FLAG = 0;

    private ZipEntryHeader zipEntryHeader;

    private long fileOffset;

    private long fileSize;

    private DataDescriptor dataDescriptor;

    private long length;

    private EntryType type;

    private byte[] data;

    /**
     * updateLength
     */
    public void updateLength() {
        zipEntryHeader.updateLength();
        if (data != null) {
            length = zipEntryHeader.getLength() + data.length + (dataDescriptor == null ? 0 : 16);
        } else {
            length = zipEntryHeader.getLength() + fileSize + (dataDescriptor == null ? 0 : 16);
        }
    }

    /**
     * init zip entry by file
     *
     * @param file zip file
     * @param entryOffset entry start offset
     * @param fileSize compress file size
     * @return zip entry
     * @throws IOException read zip exception
     */
    public static ZipEntryData getZipEntry(File file, long entryOffset, long fileSize)
        throws IOException {
        try (FileInputStream input = new FileInputStream(file)) {
            long offset = entryOffset;
            // read entry header by file and offset.
            byte[] headBytes = FileUtils.readInputByOffsetAndLength(input, entryOffset, ZipEntryHeader.HEADER_LENGTH);
            ZipEntryHeader entryHeader = ZipEntryHeader.getZipEntryHeader(headBytes);
            offset += ZipEntryHeader.HEADER_LENGTH;

            // read entry file name and extra by offset.
            if (entryHeader.getFileNameLength() > 0) {
                byte[] fileNameBytes = FileUtils.readInputByLength(input, entryHeader.getFileNameLength());
                entryHeader.readFileName(fileNameBytes);
                offset += entryHeader.getFileNameLength();
            }

            if (entryHeader.getExtraLength() > 0) {
                byte[] extraBytes = FileUtils.readInputByLength(input, entryHeader.getExtraLength());
                entryHeader.readExtra(extraBytes);
                offset += entryHeader.getExtraLength();
            }

            // skip file data , save file offset and size.
            ZipEntryData entry = new ZipEntryData();
            entry.setFileOffset(offset);
            entry.setFileSize(fileSize);
            input.skip(fileSize);

            if (FileUtils.isRunnableFile(entryHeader.getFileName())) {
                entry.setType(EntryType.RUNNABLE_FILE);
            } else if (FileUtils.BIT_MAP_FILENAME.equals(entryHeader.getFileName())) {
                entry.setType(EntryType.BIT_MAP);
            } else {
                entry.setType(EntryType.RESOURCE_FILE);
            }

            long entryLength = entryHeader.getLength() + fileSize;
            short flag = entryHeader.getFlag();
            // set desc null flag
            boolean hasDesc = (flag & HAS_DATA_DESCRIPTOR_MASK) != NOT_HAS_DATA_DESCRIPTOR_FLAG;
            if (hasDesc) {
                // if entry has data descriptor, read entry data descriptor.
                byte[] desBytes = FileUtils.readInputByLength(input, DataDescriptor.DES_LENGTH);
                DataDescriptor dataDesc = DataDescriptor.getDataDescriptor(desBytes);
                entryLength += DataDescriptor.DES_LENGTH;
                entry.setDataDescriptor(dataDesc);
            }
            entry.setZipEntryHeader(entryHeader);
            entry.setLength(entryLength);
            return entry;
        }
    }

    public void setZipEntryHeader(ZipEntryHeader zipEntryHeader) {
        this.zipEntryHeader = zipEntryHeader;
    }

    public ZipEntryHeader getZipEntryHeader() {
        return zipEntryHeader;
    }

    public DataDescriptor getDataDescriptor() {
        return dataDescriptor;
    }

    public void setDataDescriptor(DataDescriptor dataDescriptor) {
        this.dataDescriptor = dataDescriptor;
    }

    public long getFileOffset() {
        return fileOffset;
    }

    public void setFileOffset(long fileOffset) {
        this.fileOffset = fileOffset;
    }

    public long getFileSize() {
        return fileSize;
    }

    public void setFileSize(long fileSize) {
        this.fileSize = fileSize;
    }

    public long getLength() {
        return length;
    }

    public void setLength(long length) {
        this.length = length;
    }

    public byte[] getData() {
        return data;
    }

    public void setData(byte[] data) {
        this.data = data;
    }

    public EntryType getType() {
        return type;
    }

    public void setType(EntryType type) {
        this.type = type;
    }
}