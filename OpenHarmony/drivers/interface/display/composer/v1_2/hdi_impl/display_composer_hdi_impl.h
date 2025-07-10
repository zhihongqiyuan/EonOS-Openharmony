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

#ifndef OHOS_HDI_DISPLAY_V1_2_DISPLAY_COMPOSER_HDI_IMPL_H
#define OHOS_HDI_DISPLAY_V1_2_DISPLAY_COMPOSER_HDI_IMPL_H

#include "v1_1/hdi_impl/display_composer_hdi_impl.h"
#include "v1_2/display_command/display_cmd_requester.h"
#include "v1_2/display_composer_type.h"
#include "v1_2/idisplay_composer.h"
#include "v1_2/include/idisplay_composer_interface.h"
#include <cinttypes>

#undef LOG_TAG
#define LOG_TAG "DISP_HDI_COMP"
#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002515

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
namespace V1_2 {

template <typename Interface, typename CompHdi, typename CmdReq>
class DisplayComposerHdiImpl : public V1_1::DisplayComposerHdiImpl<Interface, CompHdi, CmdReq>,
    public IVBlankIdleCallback {
public:
    static IDisplayComposerInterface* Create(bool needSMQ)
    {
        sptr<CompHdi> hdi;
        std::shared_ptr<CmdReq> req = nullptr;
        HDF_LOGI("%{public}s: hdi v1_2 start", __func__);
        while ((hdi = CompHdi::Get()) == nullptr) {
            // Waiting for display composer service ready
            usleep(WAIT_TIME_INTERVAL);
        }
        HDF_LOGI("%{public}s: hdi v1_2 end", __func__);
        if (needSMQ) {
            req = CmdReq::Create(hdi);
            if (req == nullptr) {
                HDF_LOGE("%{public}s: Create DisplayCmdRequester failed %{public}d", __func__, __LINE__);
                return nullptr;
            }
        }
        return new DisplayComposerHdiImpl(hdi, req);
    }

    DisplayComposerHdiImpl(sptr<CompHdi> hdi, std::shared_ptr<CmdReq> req)
        : BaseType1_1(hdi, req),
        req_v1_2_(req),
        hdi_v1_2_(hdi),
        isSupportSkipValidate_(0),
        VBlankIdleCb_(nullptr),
        VBlankIdleCbData_(nullptr) {}

    virtual ~DisplayComposerHdiImpl() {}

    virtual int32_t CommitAndGetReleaseFence(uint32_t devId, int32_t& fence, int32_t& skipState, bool& needFlush,
        std::vector<uint32_t>& layers, std::vector<int32_t>& fences, bool isValidated) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_v1_2_);
        bool isSupportSkipValidate = (isSupportSkipValidate_ == 1) ? 1 : 0;
        return ToDispErrCode(req_v1_2_->CommitAndGetReleaseFence(devId, fence,
            isSupportSkipValidate, skipState, needFlush, layers, fences, isValidated));
    }

    virtual int32_t GetDisplayProperty(uint32_t devId, uint32_t id, uint64_t& value) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        value = 0;
        int32_t ret = ToDispErrCode(hdi_v1_2_->GetDisplayProperty(devId, id, value));
        if (ret == DISPLAY_SUCCESS && id == V1_2::DisplayPropertyID::DISPLAY_PROPERTY_ID_SKIP_VALIDATE) {
            isSupportSkipValidate_ = value;
        }

        return ret;
    }

    virtual int32_t RegDisplayVBlankIdleCallback(VBlankIdleCallback cb, void *data) override
    {
        VBlankIdleCb_ = cb;
        VBlankIdleCbData_ = data;
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        return ToDispErrCode(hdi_v1_2_->RegDisplayVBlankIdleCallback(this));
    }

    virtual int32_t SetDisplayConstraint(uint32_t devId, uint64_t frameID, uint64_t ns, uint32_t type) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_v1_2_);
        return ToDispErrCode(req_v1_2_->SetDisplayConstraint(devId, frameID, ns, type));
    }

    virtual int32_t OnVBlankIdleCallback(uint32_t devId, uint64_t ns) override
    {
        int32_t ret = HDF_SUCCESS;
        if (VBlankIdleCb_ != nullptr) {
            VBlankIdleCb_(devId, ns, VBlankIdleCbData_);
        } else {
            HDF_LOGE("error: VBlankIdle is nullptr");
            ret = HDF_FAILURE;
        }
        return ret;
    }
    virtual int32_t ClearClientBuffer(uint32_t devId) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        return ToDispErrCode(hdi_v1_2_->ClearClientBuffer(devId));
    }

    virtual int32_t ClearLayerBuffer(uint32_t devId, uint32_t layerId) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        return ToDispErrCode(hdi_v1_2_->ClearLayerBuffer(devId, layerId));
    }

    virtual int32_t UpdateHardwareCursor(uint32_t devId, int32_t x, int32_t y, BufferHandle* buffer) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        COMPOSER_CHECK_NULLPTR_RETURN(buffer);
        sptr<NativeBuffer> hdiBuffer = new NativeBuffer(buffer);
        COMPOSER_CHECK_NULLPTR_RETURN(hdiBuffer);

        int32_t ret = ToDispErrCode(hdi_v1_2_->UpdateHardwareCursor(devId, x, y, hdiBuffer));
        return ret;
    }

    virtual int32_t EnableHardwareCursorStats(uint32_t devId, bool enable) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        int32_t ret = ToDispErrCode(hdi_v1_2_->EnableHardwareCursorStats(devId, enable));
        return ret;
    }

    virtual int32_t GetHardwareCursorStats(uint32_t devId, uint32_t& frameCount, uint32_t& vsyncCount) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        int32_t ret = ToDispErrCode(hdi_v1_2_->GetHardwareCursorStats(devId, frameCount, vsyncCount));
        return ret;
    }

    virtual int32_t SetDisplayActiveRegion(uint32_t devId, const IRect& rect) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        int32_t ret = ToDispErrCode(hdi_v1_2_->SetDisplayActiveRegion(devId, rect));
        return ret;
    }

    virtual int32_t FastPresent(uint32_t devId, const PresentParam& param,
        const std::vector<BufferHandle*> inHandles) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        std::vector<sptr<NativeBuffer>> outHandles;
        for (auto inHandle : inHandles) {
            sptr<NativeBuffer> nativeBuffer = new NativeBuffer(inHandle);
            if (nativeBuffer == nullptr) {
                HDF_LOGE("failed to alloc mem for NativeBuffer");
                return DISPLAY_FAILURE;
            }
            outHandles.emplace_back(nativeBuffer);
        }
        return ToDispErrCode(hdi_v1_2_->FastPresent(devId, param, outHandles));
    }

    virtual int32_t SetLayerPerFrameParameterSmq(uint32_t devId, uint32_t layerId, const std::string& key,
        const std::vector<int8_t>& value) override
    {
        return ToDispErrCode(req_v1_2_->SetLayerPerFrameParameterSmq(devId, layerId, key, value));
    }

    virtual int32_t SetDisplayPerFrameParameterSmq(uint32_t devId, const std::string& key,
        const std::vector<int8_t>& value) override
    {
        return ToDispErrCode(req_v1_2_->SetDisplayPerFrameParameterSmq(devId, key, value));
    }
	
    virtual int32_t GetDisplayIdentificationData(uint32_t devId, uint8_t& portId,
        std::vector<uint8_t>& edidData) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_v1_2_);
        return ToDispErrCode(hdi_v1_2_->GetDisplayIdentificationData(devId, portId, edidData));
    }
	
protected:
    using BaseType1_1 = V1_1::DisplayComposerHdiImpl<Interface, CompHdi, CmdReq>;
    using BaseType1_1::WAIT_TIME_INTERVAL;
    using BaseType1_1::ToDispErrCode;
    std::shared_ptr<CmdReq> req_v1_2_;
    sptr<CompHdi> hdi_v1_2_;
    uint64_t isSupportSkipValidate_;
private:
    VBlankIdleCallback VBlankIdleCb_;
    void *VBlankIdleCbData_;
};
using HdiDisplayComposer = DisplayComposerHdiImpl<IDisplayComposerInterface, IDisplayComposer, HdiDisplayCmdRequester>;
} // namespace V1_2
} // namespace Composer
} // namespace Display
} // namespace HDI
} // namespace OHOS
#endif // OHOS_HDI_DISPLAY_V1_2_DISPLAY_COMPOSER_HDI_IMPL_H
