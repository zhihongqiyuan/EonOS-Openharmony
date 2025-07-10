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

/**
 * ELF struct define
 *
 * @since 2024/07/01
 */
public interface ElfDefine {
    /**
     * 32-bit elf file
     */
    byte ELF_32_CLASS = 1;

    /**
     * 64-bit elf file
     */
    byte ELF_64_CLASS = 2;

    /**
     * little endian
     */
    byte ELF_DATA_2_LSB = 1;

    /**
     * big endian
     */
    byte ELF_DATA_2_MSB = 2;

    /**
     * 32-bit elf file's program header length
     */
    int ELF_PHEADER_32_LEN = 32;

    /**
     * 64-bit elf file's program header length
     */
    int ELF_PHEADER_64_LEN = 56;

    /**
     * elf header e_ident length
     */
    int EI_NIDENT_LEN = 16;

    /**
     * 32-bit elf header length
     */
    int ELF_HEADER_32_LEN = EI_NIDENT_LEN + 36;

    /**
     * 64-bit elf header length
     */
    int ELF_HEADER_64_LEN = EI_NIDENT_LEN + 48;
}
