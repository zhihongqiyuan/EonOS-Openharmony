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
#include "byte_buffer_data_source.h"
#include "digest_common.h"

namespace OHOS {
namespace SignatureTools {

ByteBufferDataSource::ByteBufferDataSource(class ByteBuffer& hapBuffer)
    : DataSource(), bytebuffer(hapBuffer)
{
}

ByteBufferDataSource::~ByteBufferDataSource()
{
}

bool ByteBufferDataSource::HasRemaining() const
{
    return bytebuffer.HasRemaining();
}

int64_t ByteBufferDataSource::Remaining() const
{
    return static_cast<int64_t>(bytebuffer.Remaining());
}

void ByteBufferDataSource::Reset()
{
    bytebuffer.Clear();
}

bool ByteBufferDataSource::ReadDataAndDigestUpdate(const DigestParameter& digestParam, int32_t chunkSize)
{
    const unsigned char* chunk = reinterpret_cast<const unsigned char*>(bytebuffer.GetBufferPtr() +
                                                                        bytebuffer.GetPosition());
    bool res = DigestCommon::DigestUpdate(digestParam, chunk, chunkSize);
    if (res) {
        bytebuffer.SetPosition(bytebuffer.GetPosition() + chunkSize);
    }
    return res;
}

} // namespace SignatureTools
} // namespace OHOS