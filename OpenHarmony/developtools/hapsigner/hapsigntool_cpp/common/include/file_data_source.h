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
#ifndef SIGNATRUETOOLS_HAPFILEDATASOURCE_H
#define SIGNATRUETOOLS_HAPFILEDATASOURCE_H

#include "data_source.h"
#include "random_access_file.h"
#include "digest_parameter.h"

namespace OHOS {
namespace SignatureTools {

class FileDataSource : public DataSource {
public:
    FileDataSource(RandomAccessFile& hapFile, int64_t offset, int64_t size, int64_t position);
    ~FileDataSource();
    bool HasRemaining() const override;
    int64_t Remaining() const override;
    void Reset() override;
    bool ReadDataAndDigestUpdate(const DigestParameter& digestParam, int32_t chunkSize) override;

private:
    RandomAccessFile& hapFileRandomAccess;
    int64_t fileOffset;
    int64_t sourceSize;
    int64_t sourcePosition;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_HAPFILEDATASOURCE_H
