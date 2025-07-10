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

import com.ohos.hapsigntool.codesigning.exception.ElfFormatException;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * ELF file
 *
 * @since 2024/07/01
 */
public class ElfFile {
    private ElfHeader elfHeader;

    private final List<ElfProgramHeader> programHeaderList = new ArrayList<>();

    /**
     * Constructor for ElfFile
     *
     * @param is InputStream
     * @throws IOException io error
     * @throws ElfFormatException elf file format error
     */
    public ElfFile(InputStream is) throws IOException, ElfFormatException {
        elfHeader = new ElfHeader(is);
        if (!isElfFile()) {
            return;
        }
        byte eiClass = elfHeader.getEiClass();
        byte eiData = elfHeader.getEiData();
        int ePhnum = elfHeader.getEPhnum();
        long ePhOff = elfHeader.getEPhOff();
        if (eiClass == ElfDefine.ELF_32_CLASS) {
            is.skip(ePhOff - ElfDefine.ELF_HEADER_32_LEN);
        } else if (eiClass == ElfDefine.ELF_64_CLASS) {
            is.skip(ePhOff - ElfDefine.ELF_HEADER_64_LEN);
        }
        for (int i = 0; i < ePhnum; i++) {
            ElfProgramHeader pHeader = new ElfProgramHeader(is, eiClass, eiData);
            programHeaderList.add(pHeader);
        }
    }

    /**
     * filter executable program segment headers
     *
     * @return executable program segment headers
     */
    public List<ElfProgramHeader> filterExecPHeaders() {
        return programHeaderList.stream().filter(phdr -> (phdr.getPFlags() & 1) == 1).collect(Collectors.toList());
    }

    /**
     * return true if magic number is correct
     *
     * @return true if magic number is correct
     */
    public final boolean isElfFile() {
        return elfHeader.isElfFile();
    }
}
