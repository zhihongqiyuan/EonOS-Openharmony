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
#ifndef SIGNATRUETOOLS_DATASOURCE_H
#define SIGNATRUETOOLS_DATASOURCE_H

#include "byte_buffer.h"
#include "verify_hap_openssl_utils.h"

namespace OHOS {
namespace SignatureTools {

class DataSource {
public:
    virtual bool HasRemaining() const = 0;
    virtual int64_t Remaining() const = 0;
    virtual void Reset() = 0;
    virtual bool ReadDataAndDigestUpdate(const DigestParameter& digestParam, int32_t chunkSize) = 0;

    virtual ~DataSource()
    {
    }
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_DATASOURCE_H
