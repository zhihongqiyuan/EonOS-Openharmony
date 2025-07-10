/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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

package com.ohos.hapsigntool.codesigning.sign;

import com.ohos.hapsigntool.codesigning.datastructure.CodeSignBlock;
import com.ohos.hapsigntool.codesigning.datastructure.PageInfoExtension;
import com.ohos.hapsigntool.codesigning.elf.ElfFile;
import com.ohos.hapsigntool.codesigning.elf.ElfProgramHeader;
import com.ohos.hapsigntool.codesigning.exception.CodeSignErrMsg;
import com.ohos.hapsigntool.codesigning.exception.ElfFormatException;
import com.ohos.hapsigntool.codesigning.utils.NumberUtils;
import com.ohos.hapsigntool.error.HapFormatException;
import com.ohos.hapsigntool.utils.FileUtils;
import com.ohos.hapsigntool.utils.LogUtils;
import com.ohos.hapsigntool.zip.EntryType;
import com.ohos.hapsigntool.zip.Zip;
import com.ohos.hapsigntool.zip.ZipEntry;
import com.ohos.hapsigntool.zip.ZipEntryHeader;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

/**
 * pages info bitmap generator
 *
 * @since 2024/07/01
 */
public class PageInfoGenerator {
    private static final byte ABC_M_CODE = 2;

    private static final byte ELF_M_CODE = 1;

    private static final LogUtils LOGGER = new LogUtils(PageInfoGenerator.class);

    private long maxEntryDataOffset = 0L;

    private final List<ExcSegment> excSegmentList = new ArrayList<>();

    /**
     * Constructor for PageInfoGenerator
     *
     * @param zip zip
     * @throws IOException io error
     * @throws HapFormatException hap file format error
     * @throws ElfFormatException ElfFormatException
     */
    public PageInfoGenerator(Zip zip) throws IOException, HapFormatException, ElfFormatException {
        Map<String, Long> runnableFileNames = new LinkedHashMap<>();
        List<ZipEntry> zipEntries = zip.getZipEntries();
        for (ZipEntry entry : zipEntries) {
            ZipEntryHeader zipEntryHeader = entry.getZipEntryData().getZipEntryHeader();
            long entryDataOffset = entry.getCentralDirectory().getOffset() + ZipEntryHeader.HEADER_LENGTH
                + zipEntryHeader.getFileNameLength() + zipEntryHeader.getExtraLength();
            if (!NumberUtils.isMultiple4K(entryDataOffset)) {
                throw new HapFormatException(CodeSignErrMsg.FILE_4K_ALIGNMENT_ERROR.toString(entryDataOffset));
            }
            if (EntryType.RUNNABLE_FILE.equals(entry.getZipEntryData().getType())
                    && Zip.FILE_UNCOMPRESS_METHOD_FLAG == entry.getZipEntryData().getZipEntryHeader().getMethod()) {
                runnableFileNames.put(zipEntryHeader.getFileName(), entryDataOffset);
                continue;
            }
            maxEntryDataOffset = entryDataOffset;
            break;
        }
        File input = new File(zip.getFile());
        try (JarFile hap = new JarFile(input, false)) {
            for (Map.Entry<String, Long> en : runnableFileNames.entrySet()) {
                this.libExecSegment(hap, en.getKey(), en.getValue());
            }
        }
    }

    private void libExecSegment(JarFile hap, String libFileName, long entryDataOffset)
        throws IOException, ElfFormatException {
        JarEntry libEntry = hap.getJarEntry(libFileName);
        if (libFileName.endsWith(FileUtils.ABC_FILE_SUFFIX)) {
            long size = libEntry.getSize();
            excSegmentList.add(new ExcSegment(ABC_M_CODE, libFileName, entryDataOffset, entryDataOffset + size));
        } else {
            try (InputStream stream = hap.getInputStream(libEntry)) {
                ElfFile elfFile = new ElfFile(stream);
                if (!elfFile.isElfFile()) {
                    LOGGER.info("{} not ELF file", libFileName);
                    return;
                }
                List<ElfProgramHeader> elfPHeaders = elfFile.filterExecPHeaders();
                for (ElfProgramHeader programHeader : elfPHeaders) {
                    long pOffset = programHeader.getPOffset();
                    long pFilesz = programHeader.getPFilesz();
                    long off = entryDataOffset + pOffset;
                    long endoff = off + pFilesz;
                    excSegmentList.add(new ExcSegment(ELF_M_CODE, libFileName, off, endoff));
                }
            }
        }
    }

    /**
     * generate bitMap
     *
     * @return byte array of bitmap
     * @throws HapFormatException hap format error
     */
    public byte[] generateBitMap() throws HapFormatException {
        if (excSegmentList.isEmpty()) {
            return new byte[0];
        }
        if (!NumberUtils.isMultiple4K(maxEntryDataOffset)) {
            throw new HapFormatException(CodeSignErrMsg.FILE_4K_ALIGNMENT_ERROR.toString(maxEntryDataOffset));
        }
        int len = (int) (maxEntryDataOffset / CodeSignBlock.PAGE_SIZE_4K * PageInfoExtension.DEFAULT_UNIT_SIZE);
        BitSet bitmap = new BitSet(len);
        for (ExcSegment es : excSegmentList) {
            int begin = (int) (es.getStartOffset() >> 12) * PageInfoExtension.DEFAULT_UNIT_SIZE;
            int end = (NumberUtils.isMultiple4K(es.getEndOffset()))
                ? (int) ((es.getEndOffset() >> 12)) * PageInfoExtension.DEFAULT_UNIT_SIZE
                : (int) ((es.getEndOffset() >> 12) + 1) * PageInfoExtension.DEFAULT_UNIT_SIZE;
            for (int i = begin; i < end; i = i + PageInfoExtension.DEFAULT_UNIT_SIZE) {
                if ((ELF_M_CODE == es.getType())) {
                    bitmap.set(i);
                } else {
                    bitmap.set(i + 1);
                }
            }
        }
        long[] longArray = bitmap.toLongArray();
        int byteLen = bitmap.size() / Byte.SIZE;
        ByteBuffer buffer = ByteBuffer.allocate(byteLen).order(ByteOrder.LITTLE_ENDIAN);
        for (long l : longArray) {
            buffer.putLong(l);
        }
        return buffer.array();
    }

    static class ExcSegment {
        /**
         * abc or elf
         */
        private byte type;

        private String fileName;

        private long startOffset;

        private long endOffset;

        ExcSegment(byte type, String fileName, long startOffset, long endOffset) {
            this.type = type;
            this.fileName = fileName;
            this.startOffset = startOffset;
            this.endOffset = endOffset;
        }

        public byte getType() {
            return type;
        }

        public String getFileName() {
            return fileName;
        }

        public long getStartOffset() {
            return startOffset;
        }

        public long getEndOffset() {
            return endOffset;
        }
    }
}
