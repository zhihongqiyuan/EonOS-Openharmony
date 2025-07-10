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

#ifndef SIGNATRUETOOLS_RANDOM_ACCESS_FILE_INPUT_H
#define SIGNATRUETOOLS_RANDOM_ACCESS_FILE_INPUT_H

#include "file_data_source.h"
#include "random_access_file.h"
#include "signature_tools_log.h"
#include "zip_data_input.h"

namespace OHOS {
namespace SignatureTools {

class RandomAccessFileInput : public ZipDataInput {
public:
    RandomAccessFileInput(RandomAccessFile& file);

    /**
     * Random Access File Zip Data Input
     *
     * @param file zip file
     * @param offset offset
     * @param size size
     */
    RandomAccessFileInput(RandomAccessFile& file, int64_t offset, int64_t size);

    ~RandomAccessFileInput()
    {
    }

    int64_t Size() override;

    bool CopyTo(int64_t offset, int size, ByteBuffer& buffer) override;

    ByteBuffer CreateByteBuffer(int64_t offset, int size) override;

    DataSource* Slice(int64_t offset, int64_t size) override;

private:
    static constexpr int MAX_READ_BLOCK_SIZE = 1024 * 1024;

    bool CheckBoundValid(int64_t offset, int64_t size, int64_t sourceSize);

    RandomAccessFile& file;

    const int64_t startIndex;

    const int64_t size;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_RANDOM_ACCESS_FILE_INPUT_H