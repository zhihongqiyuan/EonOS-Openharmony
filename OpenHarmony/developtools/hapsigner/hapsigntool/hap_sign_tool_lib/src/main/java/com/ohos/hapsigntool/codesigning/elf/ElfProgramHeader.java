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

package com.ohos.hapsigntool.codesigning.elf;

import com.ohos.hapsigntool.codesigning.exception.CodeSignErrMsg;
import com.ohos.hapsigntool.codesigning.exception.ElfFormatException;
import com.ohos.hapsigntool.zip.UnsignedDecimalUtil;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * ELF program header info
 *
 * @since 2024/07/01
 */
public class ElfProgramHeader {
    /**
     * Segment type
     */
    private int pType;

    /**
     * Segment flags
     */
    private int pFlags;

    /**
     * Segment file offset
     */
    private long pOffset;

    /**
     * Segment virtual address
     */
    private long pVaddr;

    /**
     * Segment physical address
     */
    private long pPaddr;

    /**
     * Segment size in file
     */
    private long pFilesz;

    /**
     * Segment size in memory
     */
    private long pMemsz;

    /**
     * Segment alignment
     */
    private long pAlign;

    /**
     * Constructor for ElfPHeader
     *
     * @param is InputStream
     * @param eiClass eiClass
     * @param eiData eiData
     * @throws IOException io error
     * @throws ElfFormatException elf file format error
     */
    public ElfProgramHeader(InputStream is, byte eiClass, byte eiData) throws IOException, ElfFormatException {
        ByteOrder bo = ByteOrder.LITTLE_ENDIAN;
        if (eiData == ElfDefine.ELF_DATA_2_LSB) {
            bo = ByteOrder.LITTLE_ENDIAN;
        } else if (eiData == ElfDefine.ELF_DATA_2_MSB) {
            bo = ByteOrder.BIG_ENDIAN;
        } else {
            throw new ElfFormatException(CodeSignErrMsg.ELF_FILE_HEADER_ERROR.toString("ei_data"));
        }
        if (eiClass == ElfDefine.ELF_32_CLASS) {
            byte[] bytes = new byte[ElfDefine.ELF_PHEADER_32_LEN];
            int read = is.read(bytes);
            if (read != ElfDefine.ELF_PHEADER_32_LEN) {
                throw new ElfFormatException(CodeSignErrMsg.ELF_FILE_HEADER_ERROR.toString("program header"));
            }
            ByteBuffer byteBuffer = ByteBuffer.wrap(bytes);
            byteBuffer.order(bo);
            pType = byteBuffer.getInt();
            pOffset = UnsignedDecimalUtil.getUnsignedInt(byteBuffer);
            pVaddr = UnsignedDecimalUtil.getUnsignedInt(byteBuffer);
            pPaddr = UnsignedDecimalUtil.getUnsignedInt(byteBuffer);
            pFilesz = byteBuffer.getInt();
            pMemsz = byteBuffer.getInt();
            pFlags = byteBuffer.getInt();
            pAlign = byteBuffer.getInt();
        } else {
            byte[] bytes = new byte[ElfDefine.ELF_PHEADER_64_LEN];
            int read = is.read(bytes);
            if (read != ElfDefine.ELF_PHEADER_64_LEN) {
                throw new ElfFormatException(CodeSignErrMsg.ELF_FILE_HEADER_ERROR.toString("program header"));
            }
            ByteBuffer byteBuffer = ByteBuffer.wrap(bytes);
            byteBuffer.order(bo);
            pType = byteBuffer.getInt();
            pFlags = byteBuffer.getInt();
            pOffset = byteBuffer.getLong();
            pVaddr = byteBuffer.getLong();
            pPaddr = byteBuffer.getLong();
            pFilesz = byteBuffer.getLong();
            pMemsz = byteBuffer.getLong();
            pAlign = byteBuffer.getLong();
        }
    }

    public int getPFlags() {
        return pFlags;
    }

    public long getPOffset() {
        return pOffset;
    }

    public long getPFilesz() {
        return pFilesz;
    }
}
