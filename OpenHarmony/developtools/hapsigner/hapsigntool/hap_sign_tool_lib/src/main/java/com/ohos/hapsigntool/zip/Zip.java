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

import com.ohos.hapsigntool.error.CustomException;
import com.ohos.hapsigntool.error.ERROR;
import com.ohos.hapsigntool.error.SignToolErrMsg;
import com.ohos.hapsigntool.error.ZipException;
import com.ohos.hapsigntool.utils.FileUtils;

import com.ohos.hapsigntool.utils.LogUtils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

/**
 * resolve zip data
 *
 * @since 2023/12/02
 */
public class Zip {
    private static final LogUtils LOGGER = new LogUtils(Zip.class);

    /**
     * file is uncompress file flag
     */
    public static final short FILE_UNCOMPRESS_METHOD_FLAG = 0;

    /**
     * max comment length
     */
    public static final int MAX_COMMENT_LENGTH = 65535;

    private List<ZipEntry> zipEntries;

    private long signingOffset;

    private byte[] signingBlock;

    private long cDOffset;

    private long eOCDOffset;

    private EndOfCentralDirectory endOfCentralDirectory;

    private String file;

    /**
     * create Zip by file
     *
     * @param inputFile file
     */
    public Zip(File inputFile) {
        try {
            this.file = inputFile.getCanonicalPath();
            if (!inputFile.exists()) {
                throw new ZipException("read zip file failed");
            }
            long start = System.currentTimeMillis();
            // 1. get eocd data
            endOfCentralDirectory = getZipEndOfCentralDirectory(inputFile);
            cDOffset = endOfCentralDirectory.getOffset();
            long eocdEnd = System.currentTimeMillis();
            LOGGER.debug("getZipEndOfCentralDirectory use {} ms", eocdEnd - start);
            // 2. use eocd's cd offset, get cd data
            getZipCentralDirectory(inputFile);
            long cdEnd = System.currentTimeMillis();
            LOGGER.debug("getZipCentralDirectory use {} ms", cdEnd - start);
            // 3. use cd's entry offset and file size, get entry data
            getZipEntries(inputFile);
            ZipEntry endEntry = zipEntries.get(zipEntries.size() - 1);
            CentralDirectory endCD = endEntry.getCentralDirectory();
            ZipEntryData endEntryData = endEntry.getZipEntryData();
            signingOffset = endCD.getOffset() + endEntryData.getLength();
            long entryEnd = System.currentTimeMillis();
            LOGGER.debug("getZipEntries use {} ms", entryEnd - start);
            // 4. file all data - eocd - cd - entry = sign block
            signingBlock = getSigningBlock(inputFile);
        } catch (IOException e) {
            CustomException.throwException(ERROR.ZIP_ERROR, SignToolErrMsg.READ_ZIP_FAILED.toString(e.getMessage()));
        }
    }

    private EndOfCentralDirectory getZipEndOfCentralDirectory(File file) throws IOException {
        if (file.length() < EndOfCentralDirectory.EOCD_LENGTH) {
            throw new ZipException("find zip eocd failed");
        }

        // try to read EOCD without comment
        int eocdLength = EndOfCentralDirectory.EOCD_LENGTH;
        eOCDOffset = file.length() - eocdLength;
        byte[] bytes = FileUtils.readFileByOffsetAndLength(file, eOCDOffset, eocdLength);
        Optional<EndOfCentralDirectory> eocdByBytes = EndOfCentralDirectory.getEOCDByBytes(bytes);
        if (eocdByBytes.isPresent()) {
            return eocdByBytes.get();
        }

        // try to search EOCD with comment
        long eocdMaxLength = Math.min(EndOfCentralDirectory.EOCD_LENGTH + MAX_COMMENT_LENGTH, file.length());
        eOCDOffset = file.length() - eocdMaxLength;
        bytes = FileUtils.readFileByOffsetAndLength(file, eOCDOffset, eocdMaxLength);
        for (int start = 0; start < eocdMaxLength; start++) {
            eocdByBytes = EndOfCentralDirectory.getEOCDByBytes(bytes, start);
            if (eocdByBytes.isPresent()) {
                eOCDOffset += start;
                return eocdByBytes.get();
            }
        }
        throw new ZipException("read zip failed: can not find eocd in file");
    }

    private void getZipCentralDirectory(File file) throws IOException {
        zipEntries = new ArrayList<>(endOfCentralDirectory.getCDTotal());
        // read full central directory bytes
        byte[] cdBytes = FileUtils.readFileByOffsetAndLength(file, cDOffset, endOfCentralDirectory.getCDSize());
        if (cdBytes.length < CentralDirectory.CD_LENGTH) {
            throw new ZipException("find zip cd failed");
        }
        ByteBuffer bf = ByteBuffer.wrap(cdBytes);
        bf.order(ByteOrder.LITTLE_ENDIAN);
        int offset = 0;
        // one by one format central directory
        while (offset < cdBytes.length) {
            CentralDirectory cd = CentralDirectory.getCentralDirectory(bf);
            ZipEntry entry = new ZipEntry();
            entry.setCentralDirectory(cd);
            zipEntries.add(entry);
            offset += cd.getLength();
        }
        if (offset + cDOffset != eOCDOffset) {
            throw new ZipException("cd end offset not equals to eocd offset, maybe this is a zip64 file");
        }
    }

    private byte[] getSigningBlock(File file) throws IOException {
        long size = cDOffset - signingOffset;
        if (size < 0) {
            throw new ZipException("signing offset in front of entry end");
        }
        if (size == 0) {
            return new byte[0];
        }
        return FileUtils.readFileByOffsetAndLength(file, signingOffset, size);
    }

    private void getZipEntries(File file) throws IOException {
        // use central directory data, find entry data
        for (ZipEntry entry : zipEntries) {
            CentralDirectory cd = entry.getCentralDirectory();
            long offset = cd.getOffset();
            long unCompressedSize = cd.getUnCompressedSize();
            long compressedSize = cd.getCompressedSize();
            long fileSize = cd.getMethod() == FILE_UNCOMPRESS_METHOD_FLAG ? unCompressedSize : compressedSize;

            ZipEntryData zipEntryData = ZipEntryData.getZipEntry(file, offset, fileSize);
            if (cDOffset - offset < zipEntryData.getLength()) {
                throw new ZipException("cd offset in front of entry end");
            }
            entry.setZipEntryData(zipEntryData);
        }
    }

    /**
     * output zip to zip file
     *
     * @param outFile file path
     */
    public void toFile(String outFile) {
        try (FileOutputStream fos = new FileOutputStream(outFile)) {
            for (ZipEntry entry : zipEntries) {
                ZipEntryData zipEntryData = entry.getZipEntryData();
                FileUtils.writeByteToOutFile(zipEntryData.getZipEntryHeader().toBytes(), fos);
                boolean isSuccess;
                if (entry.getZipEntryData().getData() != null) {
                    ByteBuffer bf = ByteBuffer.wrap(entry.getZipEntryData().getData());
                    bf.order(ByteOrder.LITTLE_ENDIAN);
                    isSuccess = FileUtils.writeByteToOutFile(bf.array(), fos);
                } else {
                    isSuccess = FileUtils.appendWriteFileByOffsetToFile(file, fos,
                            zipEntryData.getFileOffset(), zipEntryData.getFileSize());
                }
                if (!isSuccess) {
                    throw new ZipException("write zip data failed");
                }
                if (zipEntryData.getDataDescriptor() != null) {
                    FileUtils.writeByteToOutFile(zipEntryData.getDataDescriptor().toBytes(), fos);
                }
            }
            if (signingBlock != null) {
                FileUtils.writeByteToOutFile(signingBlock, fos);
            }
            for (ZipEntry entry : zipEntries) {
                CentralDirectory cd = entry.getCentralDirectory();
                FileUtils.writeByteToOutFile(cd.toBytes(), fos);
            }
            FileUtils.writeByteToOutFile(endOfCentralDirectory.toBytes(), fos);
        } catch (IOException e) {
            CustomException.throwException(ERROR.ZIP_ERROR, SignToolErrMsg.WRITE_ZIP_FAILED.toString(e.getMessage()));
        }
    }

    /**
     * alignment uncompress entry
     *
     * @param alignment int alignment
     */
    public void alignment(int alignment) {
        try {
            sort();
            boolean isFirstUnRunnableFile = true;
            for (ZipEntry entry : zipEntries) {
                ZipEntryData zipEntryData = entry.getZipEntryData();
                short method = zipEntryData.getZipEntryHeader().getMethod();
                if (method != FILE_UNCOMPRESS_METHOD_FLAG && !isFirstUnRunnableFile) {
                    // only align uncompressed entry and the first compress entry.
                    break;
                }
                int alignBytes;
                EntryType type = entry.getZipEntryData().getType();
                if ((type == EntryType.RUNNABLE_FILE && method == FILE_UNCOMPRESS_METHOD_FLAG) ||
                    type == EntryType.BIT_MAP) {
                    // .abc and .so file align 4096 byte.
                    alignBytes = 4096;
                } else if (isFirstUnRunnableFile) {
                    // the first file after runnable file, align 4096 byte.
                    alignBytes = 4096;
                    isFirstUnRunnableFile = false;
                } else {
                    // normal file align 4 byte.
                    alignBytes = alignment;
                }
                int add = entry.alignment(alignBytes);
                if (add > 0) {
                    resetOffset();
                }
            }
        } catch (ZipException e) {
            CustomException.throwException(ERROR.ZIP_ERROR, SignToolErrMsg.ALIGNMENT_ZIP_FAILED
                    .toString(e.getMessage()));
        }
    }

    /**
     * add bit map entry
     *
     * @param data bitmap data
     * @throws ZipException ZipException
     */
    public void addBitMap(byte[] data) throws ZipException {
        zipEntries.removeIf(e -> e.getZipEntryData().getType() == EntryType.BIT_MAP);
        ZipEntry entry = new ZipEntry.Builder().setMethod(FILE_UNCOMPRESS_METHOD_FLAG)
                .setUncompressedSize(data.length)
                .setCompressedSize(data.length)
                .setFileName(FileUtils.BIT_MAP_FILENAME)
                .setData(data)
                .build();
        zipEntries.add(entry);
    }

    /**
     * remove sign block
     */
    public void removeSignBlock() {
        signingBlock = null;
        resetOffset();
    }

    /**
     * sort uncompress entry in the front.
     */
    private void sort() {
        // sort uncompress file (so, abc, an) - bitmap - other uncompress file - compress file
        zipEntries.sort((entry1, entry2) -> {
            short entry1Method = entry1.getZipEntryData().getZipEntryHeader().getMethod();
            short entry2Method = entry2.getZipEntryData().getZipEntryHeader().getMethod();
            String entry1FileName = entry1.getZipEntryData().getZipEntryHeader().getFileName();
            String entry2FileName = entry2.getZipEntryData().getZipEntryHeader().getFileName();
            if (entry1Method == FILE_UNCOMPRESS_METHOD_FLAG && entry2Method == FILE_UNCOMPRESS_METHOD_FLAG) {
                EntryType entry1Type = entry1.getZipEntryData().getType();
                EntryType entry2Type = entry2.getZipEntryData().getType();
                if (entry1Type != entry2Type) {
                    return entry1Type.compareTo(entry2Type);
                }
                return entry1FileName.compareTo(entry2FileName);
            } else if (entry1Method == FILE_UNCOMPRESS_METHOD_FLAG) {
                return -1;
            } else if (entry2Method == FILE_UNCOMPRESS_METHOD_FLAG) {
                return 1;
            }
            return entry1FileName.compareTo(entry2FileName);
        });
        resetOffset();
    }

    private void resetOffset() {
        long offset = 0L;
        long cdLength = 0L;
        for (ZipEntry entry : zipEntries) {
            entry.updateLength();
            entry.getCentralDirectory().setOffset(offset);
            offset += entry.getZipEntryData().getLength();
            cdLength += entry.getCentralDirectory().getLength();
        }
        if (signingBlock != null) {
            offset += signingBlock.length;
        }
        cDOffset = offset;
        endOfCentralDirectory.setOffset(offset);
        endOfCentralDirectory.setCDSize(cdLength);
        offset += cdLength;
        eOCDOffset = offset;
        endOfCentralDirectory.setCDTotal(zipEntries.size());
        endOfCentralDirectory.setThisDiskCDNum(zipEntries.size());
    }

    public List<ZipEntry> getZipEntries() {
        return zipEntries;
    }

    public void setZipEntries(List<ZipEntry> zipEntries) {
        this.zipEntries = zipEntries;
    }

    public long getSigningOffset() {
        return signingOffset;
    }

    public void setSigningOffset(long signingOffset) {
        this.signingOffset = signingOffset;
    }

    public byte[] getSigningBlock() {
        return signingBlock;
    }

    public void setSigningBlock(byte[] signingBlock) {
        this.signingBlock = signingBlock;
    }

    public long getCDOffset() {
        return cDOffset;
    }

    public void setCDOffset(long cDOffset) {
        this.cDOffset = cDOffset;
    }

    public long getEOCDOffset() {
        return eOCDOffset;
    }

    public void setEOCDOffset(long eOCDOffset) {
        this.eOCDOffset = eOCDOffset;
    }

    public EndOfCentralDirectory getEndOfCentralDirectory() {
        return endOfCentralDirectory;
    }

    public void setEndOfCentralDirectory(EndOfCentralDirectory endOfCentralDirectory) {
        this.endOfCentralDirectory = endOfCentralDirectory;
    }

    public String getFile() {
        return file;
    }

    public void setFile(String file) {
        this.file = file;
    }
}