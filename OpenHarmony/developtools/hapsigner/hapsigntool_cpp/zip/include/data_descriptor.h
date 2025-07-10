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

#ifndef SIGNATRUETOOLS_DATA_DESCRIPTOR_H
#define SIGNATRUETOOLS_DATA_DESCRIPTOR_H

#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {
/**
 * resolve zip DataDescriptor data
 * DataDescriptor format:
 * crc-32                          4 bytes
 * compressed size                 4 bytes
 * uncompressed size               4 bytes
 */
class DataDescriptor {
public:
    /* DataDescriptor invariable bytes length */
    static constexpr int DES_LENGTH = 16;

    /* 4 bytes , DataDescriptor signature */
    static constexpr int SIGNATURE = 0x08074b50;

    /**
     * get Data Descriptor
     *
     * @param bytes DataDescriptor bytes
     * @return DataDescriptor
     * @throws ZipException read data descriptor exception
     */
    static DataDescriptor* GetDataDescriptor(const std::string& bytes);

    std::string ToBytes();

    static int GetDesLength();

    static int GetSIGNATURE();

    int GetCrc32();

    void SetCrc32(int crc32);

    uint32_t GetCompressedSize();

    void SetCompressedSize(uint32_t compressedSize);

    uint32_t GetUnCompressedSize();

    void SetUnCompressedSize(uint32_t unCompressedSize);

private:
    /* 4 bytes */
    int m_crc32 = 0;

    /* 4 bytes */
    uint32_t m_compressedSize = 0;

    /* 4 bytes */
    uint32_t m_unCompressedSize = 0;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_DATA_DESCRIPTOR_H