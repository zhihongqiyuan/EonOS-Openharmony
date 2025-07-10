/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef OHOS_HDI_DISPLAY_V1_2_DISPLAY_BUFFER_HDI_IMPL_H
#define OHOS_HDI_DISPLAY_V1_2_DISPLAY_BUFFER_HDI_IMPL_H

#include <iproxy_broker.h>
#include <unistd.h>
#include "hdf_log.h"
#include "hilog/log.h"
#include "v1_0/hdi_impl/display_buffer_hdi_impl.h"
#include "v1_0/imapper.h"
#include "v1_2/imapper.h"
#include "v1_1/include/idisplay_buffer.h"
#include "v1_2/include/idisplay_buffer.h"
#include "v1_1/hdi_impl/display_buffer_hdi_impl.h"

#undef LOG_TAG
#define LOG_TAG "DISP_HDI_BUFF"
#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002515

namespace OHOS {
namespace HDI {
namespace Display {
namespace Buffer {
namespace V1_2 {
template<typename Interface>
class DisplayBufferHdiImpl : public V1_1::DisplayBufferHdiImpl<Interface> {
public:
    explicit DisplayBufferHdiImpl(bool isAllocLocal = false) : BaseType2_0(isAllocLocal), mapper_v1_2_(nullptr)
    {
        while ((mapper_v1_2_ = IMapper::Get(true)) == nullptr) {
            // Waiting for metadata service ready
            usleep(WAIT_TIME_INTERVAL);
        }
    }
    virtual ~DisplayBufferHdiImpl() {};

    int32_t GetImageLayout(const BufferHandle& handle, ImageLayout& layout) const override
    {
        CHECK_NULLPOINTER_RETURN_VALUE(mapper_v1_2_, HDF_FAILURE);
        sptr<NativeBuffer> hdiBuffer = new NativeBuffer();
        CHECK_NULLPOINTER_RETURN_VALUE(hdiBuffer, HDF_FAILURE);
        hdiBuffer->SetBufferHandle(const_cast<BufferHandle*>(&handle));
        int32_t ret = mapper_v1_2_->GetImageLayout(hdiBuffer, layout);
        return ret;
    }

private:
    using BaseType2_0 = V1_1::DisplayBufferHdiImpl<Interface>;
protected:
    using BaseType2_0::WAIT_TIME_INTERVAL;
    sptr<IMapper> mapper_v1_2_;
};
using HdiDisplayBufferImpl = DisplayBufferHdiImpl<V1_2::IDisplayBuffer>;
} // namespace V1_2
} // namespace Buffer
} // namespace Display
} // namespace HDI
} // namespace OHOS

#endif // OHOS_HDI_DISPLAY_V1_1_DISPLAY_BUFFER_HDI_IMPL_H
