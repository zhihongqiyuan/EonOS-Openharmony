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

#ifndef OHOS_HDI_DISPLAY_V1_0_DISPLAY_BUFFER_HDI_IMPL_H
#define OHOS_HDI_DISPLAY_V1_0_DISPLAY_BUFFER_HDI_IMPL_H

#include <iremote_object.h>
#include <iproxy_broker.h>
#include <unistd.h>
#include "hdf_log.h"
#include "hilog/log.h"
#include "buffer_handle.h"
#include "v1_0/display_buffer_type.h"
#include "v1_0/iallocator.h"
#include "v1_0/imapper.h"
#include "v1_0/include/idisplay_buffer.h"
#include "hdf_trace.h"

#undef LOG_TAG
#define LOG_TAG "DISP_HDI_BUFF"
#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002515
#define DISPLAY_TRACE HdfTrace trace(__func__, "HDI:DISP:IMPL:")

#ifndef BUFFER_HDI_IMPL_LOGE
#define BUFFER_HDI_IMPL_LOGE(format, ...)              \
    do {                                               \
        HDF_LOGE(                                      \
            "\033[0;32;31m"                            \
            "[%{public}s:%{public}d] " format "\033[m" \
            "\n",                                      \
            __FUNCTION__, __LINE__, ##__VA_ARGS__);    \
    } while (0)
#endif

#ifndef CHECK_NULLPOINTER_RETURN_VALUE
#define CHECK_NULLPOINTER_RETURN_VALUE(pointer, ret)                  \
    do {                                                              \
        if ((pointer) == NULL) {                                      \
            BUFFER_HDI_IMPL_LOGE("pointer is null and return ret\n"); \
            return (ret);                                             \
        }                                                             \
    } while (0)
#endif

#ifndef CHECK_NULLPOINTER_RETURN
#define CHECK_NULLPOINTER_RETURN(pointer)                             \
    do {                                                              \
        if ((pointer) == NULL) {                                      \
            BUFFER_HDI_IMPL_LOGE("pointer is null and return\n");     \
            return;                                                   \
        }                                                             \
    } while (0)
#endif

namespace OHOS {
namespace HDI {
namespace Display {
namespace Buffer {
namespace V1_0 {

template<typename Interface>
class DisplayBufferHdiImpl : public Interface {
public:
    explicit DisplayBufferHdiImpl(bool isAllocLocal = false) : allocator_(nullptr),
        mapper_(nullptr), recipient_(nullptr)
    {
        while ((allocator_ = IAllocator::Get(isAllocLocal)) == nullptr) {
            // Waiting for allocator service ready
            usleep(WAIT_TIME_INTERVAL);
        }
        while ((mapper_ = IMapper::Get(true)) == nullptr) {
            // Waiting for mapper IF ready
            usleep(WAIT_TIME_INTERVAL);
        }
    }
    virtual ~DisplayBufferHdiImpl()
    {
        if (recipient_ != nullptr) {
            sptr<IRemoteObject> remoteObj = OHOS::HDI::hdi_objcast<IAllocator>(allocator_);
            remoteObj->RemoveDeathRecipient(recipient_);
            recipient_ = nullptr;
        }
    }

    bool AddDeathRecipient(const sptr<IRemoteObject::DeathRecipient>& recipient) override
    {
        sptr<IRemoteObject> remoteObj = OHOS::HDI::hdi_objcast<IAllocator>(allocator_);
        if (recipient_ != nullptr) {
            HDF_LOGE("%{public}s: the existing recipient is removed, and add the new. %{public}d",
                __func__, __LINE__);
            remoteObj->RemoveDeathRecipient(recipient_);
        }
        bool ret = remoteObj->AddDeathRecipient(recipient);
        if (ret) {
            recipient_ = recipient;
        } else {
            recipient_ = nullptr;
            HDF_LOGE("%{public}s: AddDeathRecipient failed %{public}d", __func__, __LINE__);
        }
        return ret;
    }

    bool RemoveDeathRecipient() override
    {
        if (recipient_ != nullptr) {
            sptr<IRemoteObject> remoteObj = OHOS::HDI::hdi_objcast<IAllocator>(allocator_);
            remoteObj->RemoveDeathRecipient(recipient_);
            recipient_ = nullptr;
        }
        return true;
    }

    int32_t AllocMem(const AllocInfo& info, BufferHandle*& handle) const override
    {
        DISPLAY_TRACE;
        CHECK_NULLPOINTER_RETURN_VALUE(allocator_, HDF_FAILURE);
        sptr<NativeBuffer> hdiBuffer;
        int32_t ret = allocator_->AllocMem(info, hdiBuffer);
        if ((ret == HDF_SUCCESS) && (hdiBuffer != nullptr)) {
            handle = hdiBuffer->Move();
        } else {
            handle = nullptr;
            if (ret == HDF_SUCCESS) {
                ret = HDF_FAILURE;
            }
            HDF_LOGE("%{public}s: AllocMem error", __func__);
        }
        return ret;
    }

    void FreeMem(const BufferHandle& handle) const override
    {
        CHECK_NULLPOINTER_RETURN(mapper_);
        sptr<NativeBuffer> hdiBuffer = new NativeBuffer();
        CHECK_NULLPOINTER_RETURN(hdiBuffer);
        hdiBuffer->SetBufferHandle(const_cast<BufferHandle*>(&handle), true);
        mapper_->FreeMem(hdiBuffer);
    }

    void *Mmap(const BufferHandle& handle) const override
    {
        CHECK_NULLPOINTER_RETURN_VALUE(mapper_, nullptr);
        sptr<NativeBuffer> hdiBuffer = new NativeBuffer();
        CHECK_NULLPOINTER_RETURN_VALUE(hdiBuffer, nullptr);
        hdiBuffer->SetBufferHandle(const_cast<BufferHandle*>(&handle));
        int32_t ret = mapper_->Mmap(hdiBuffer);
        void *virAddr = (ret == HDF_SUCCESS ? handle.virAddr : nullptr);
        return virAddr;
    }

    int32_t Unmap(const BufferHandle& handle) const override
    {
        CHECK_NULLPOINTER_RETURN_VALUE(mapper_, HDF_FAILURE);
        sptr<NativeBuffer> hdiBuffer = new NativeBuffer();
        CHECK_NULLPOINTER_RETURN_VALUE(hdiBuffer, HDF_FAILURE);
        hdiBuffer->SetBufferHandle(const_cast<BufferHandle*>(&handle));
        int32_t ret = mapper_->Unmap(hdiBuffer);
        return ret;
    }

    int32_t FlushCache(const BufferHandle& handle) const override
    {
        CHECK_NULLPOINTER_RETURN_VALUE(mapper_, HDF_FAILURE);
        sptr<NativeBuffer> hdiBuffer = new NativeBuffer();
        CHECK_NULLPOINTER_RETURN_VALUE(hdiBuffer, HDF_FAILURE);
        hdiBuffer->SetBufferHandle(const_cast<BufferHandle*>(&handle));
        int32_t ret = mapper_->FlushCache(hdiBuffer);
        return ret;
    }

    int32_t InvalidateCache(const BufferHandle& handle) const override
    {
        CHECK_NULLPOINTER_RETURN_VALUE(mapper_, HDF_FAILURE);
        sptr<NativeBuffer> hdiBuffer = new NativeBuffer();
        CHECK_NULLPOINTER_RETURN_VALUE(hdiBuffer, HDF_FAILURE);
        hdiBuffer->SetBufferHandle(const_cast<BufferHandle*>(&handle));
        int32_t ret = mapper_->InvalidateCache(hdiBuffer);
        return ret;
    }

    int32_t IsSupportedAlloc(const std::vector<VerifyAllocInfo>& infos,
        std::vector<bool>& supporteds) const override
    {
        (void)infos;
        (void)supporteds;
        return HDF_ERR_NOT_SUPPORT;
    }

protected:
    static constexpr uint32_t WAIT_TIME_INTERVAL = 10000;
    sptr<IAllocator> allocator_;
    sptr<IMapper> mapper_;
    sptr<IRemoteObject::DeathRecipient> recipient_;
};
using HdiDisplayBufferImpl = DisplayBufferHdiImpl<V1_0::IDisplayBuffer>;
} // namespace V1_0
} // namespace Buffer
} // namespace Display
} // namespace HDI
} // namespace OHOS

#endif // OHOS_HDI_DISPLAY_V1_0_DISPLAY_BUFFER_HDI_IMPL_H
