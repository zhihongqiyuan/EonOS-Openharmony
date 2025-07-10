/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_HDI_DISPLAY_V1_1_IDISPLAY_BUFFER_H
#define OHOS_HDI_DISPLAY_V1_1_IDISPLAY_BUFFER_H

#include "v1_0/include/idisplay_buffer.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace Buffer {
namespace V1_1 {
class IDisplayBuffer : public V1_0::IDisplayBuffer {
public:
    virtual ~IDisplayBuffer() = default;

    /**
     * @brief Obtains all interfaces of IDisplayBuffer.
     *
     * @return Returns <b>IDisplayBuffer*</b> if the operation is successful; returns an null point otherwise.
     * @since 4.0
     * @version 1.0
     */
    static IDisplayBuffer *Get();
    virtual int32_t RegisterBuffer(const BufferHandle& handle) = 0;
    virtual int32_t SetMetadata(const BufferHandle& handle, uint32_t key, const std::vector<uint8_t>& value) = 0;
    virtual int32_t GetMetadata(const BufferHandle& handle, uint32_t key, std::vector<uint8_t>& value) = 0;
    virtual int32_t ListMetadataKeys(const BufferHandle& handle, std::vector<uint32_t>& keys) = 0;
    virtual int32_t EraseMetadataKey(const BufferHandle& handle, uint32_t key) = 0;
};
} // namespace V1_1
} // namespace Buffer
} // namespace Display
} // namespace HDI
} // namespace OHOS

#endif // OHOS_HDI_DISPLAY_V1_1_IDISPLAY_BUFFER_H
