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

#ifndef OHOS_HDI_DISPLAY_V1_0_DISPLAY_COMPOSER_HDI_IMPL_H
#define OHOS_HDI_DISPLAY_V1_0_DISPLAY_COMPOSER_HDI_IMPL_H

#include <unistd.h>
#include "hdf_trace.h"
#include "hilog/log.h"
#include "iproxy_broker.h"
#include "iremote_object.h"
#include "v1_0/display_command/display_cmd_requester.h"
#include "v1_0/display_composer_type.h"
#include "v1_0/idisplay_composer.h"
#include "v1_0/include/idisplay_composer_interface.h"

#undef LOG_TAG
#define LOG_TAG "DISP_HDI_COMP"
#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002515
#define MAX_COUNT 10

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
namespace V1_0 {
using namespace OHOS::HDI::Display::Composer::V1_0;

#define DISPLAY_TRACE HdfTrace trace(__func__, "HDI:DISP:")

#define COMPOSER_CHECK_NULLPTR_RETURN(ptr)                                                  \
    if ((ptr) == nullptr) {                                                                 \
        HDF_LOGE("%{public}d@%{public}s nullptr error", __LINE__, __func__);                \
        return DISPLAY_NULL_PTR;                                                            \
    }

template <typename Interface, typename CompHdi, typename CmdReq>
class DisplayComposerHdiImpl : public Interface, public IHotPlugCallback, public IVBlankCallback {
public:
    static IDisplayComposerInterface* Create(bool needSMQ)
    {
        sptr<CompHdi> hdi;
        std::shared_ptr<CmdReq> req = nullptr;
        HDF_LOGI("%{public}s: hdi V1_0 start", __func__);
        while ((hdi = CompHdi::Get()) == nullptr) {
            // Waiting for display composer service ready
            usleep(WAIT_TIME_INTERVAL);
        }
        HDF_LOGI("%{public}s: hdi V1_0 end", __func__);
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
        : hdi_(hdi),
        req_(req),
        hotPlugCb_(nullptr),
        vBlankCb_(nullptr),
        hotPlugCbData_(nullptr),
        vBlankCbData_(nullptr),
        recipient_(nullptr) {}

    virtual ~DisplayComposerHdiImpl()
    {
        if (recipient_ != nullptr) {
            sptr<IRemoteObject> remoteObj = OHOS::HDI::hdi_objcast<CompHdi>(hdi_);
            remoteObj->RemoveDeathRecipient(recipient_);
            recipient_ = nullptr;
        }
    }

    virtual bool AddDeathRecipient(const sptr<IRemoteObject::DeathRecipient>& recipient) override
    {
        sptr<IRemoteObject> remoteObj = OHOS::HDI::hdi_objcast<CompHdi>(hdi_);
        if (recipient_ != nullptr) {
            HDF_LOGI("%{public}s: the existing recipient is removed, and add the new. %{public}d",
                __func__, __LINE__);
            remoteObj->RemoveDeathRecipient(recipient_);
        }
        bool ret = remoteObj->AddDeathRecipient(recipient);
        if (ret) {
            recipient_ = recipient;
        } else {
            recipient_ = nullptr;
            HDF_LOGE("%{public}s: AddDeathRecipient failed %{public}d",  __func__, __LINE__);
        }
        return ret;
    }

    virtual bool RemoveDeathRecipient() override
    {
        if (recipient_ != nullptr) {
            sptr<IRemoteObject> remoteObj = OHOS::HDI::hdi_objcast<CompHdi>(hdi_);
            remoteObj->RemoveDeathRecipient(recipient_);
            recipient_ = nullptr;
        }
        return true;
    }

    // device func
    virtual int32_t RegHotPlugCallback(HotPlugCallback cb, void *data) override
    {
        hotPlugCb_ = cb;
        hotPlugCbData_ = data;
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->RegHotPlugCallback(this));
    }

    virtual int32_t SetClientBufferCacheCount(uint32_t devId, uint32_t cacheCount) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->SetClientBufferCacheCount(devId, cacheCount));
    }

    virtual int32_t GetDisplayCapability(uint32_t devId, DisplayCapability& info) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->GetDisplayCapability(devId, info));
    }

    virtual int32_t GetDisplaySupportedModes(uint32_t devId, std::vector<DisplayModeInfo>& modes) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->GetDisplaySupportedModes(devId, modes));
    }

    virtual int32_t GetDisplayMode(uint32_t devId, uint32_t& modeId) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->GetDisplayMode(devId, modeId));
    }

    virtual int32_t SetDisplayMode(uint32_t devId, uint32_t modeId) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->SetDisplayMode(devId, modeId));
    }

    virtual int32_t GetDisplayPowerStatus(uint32_t devId, DispPowerStatus& status) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->GetDisplayPowerStatus(devId, status));
    }

    virtual int32_t SetDisplayPowerStatus(uint32_t devId, DispPowerStatus status) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        int32_t ret = hdi_->SetDisplayPowerStatus(devId, status);
        HDF_LOGI("devId: %{public}u, status: %{public}u", devId, status);
        return ToDispErrCode(ret);
    }

    virtual int32_t GetDisplayBacklight(uint32_t devId, uint32_t& level) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->GetDisplayBacklight(devId, level));
    }

    virtual int32_t SetDisplayBacklight(uint32_t devId, uint32_t level) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->SetDisplayBacklight(devId, level));
    }

    virtual int32_t GetDisplayProperty(uint32_t devId, uint32_t id, uint64_t& value) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->GetDisplayProperty(devId, id, value));
    }

    virtual int32_t GetDisplayCompChange(
        uint32_t devId, std::vector<uint32_t>& layers, std::vector<int32_t>& types) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->GetDisplayCompChange(devId, layers, types));
    }

    virtual int32_t SetDisplayClientCrop(uint32_t devId, const IRect& rect) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->SetDisplayClientCrop(devId, rect));
    }

    virtual int32_t SetDisplayClientBuffer(uint32_t devId, const BufferHandle* buffer, uint32_t seqNo,
        int32_t fence) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetDisplayClientBuffer(devId, buffer, seqNo, fence));
    }

    virtual int32_t SetDisplayClientDamage(uint32_t devId, std::vector<IRect>& rects) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetDisplayClientDamage(devId, rects));
    }

    virtual int32_t SetDisplayVsyncEnabled(uint32_t devId, bool enabled) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->SetDisplayVsyncEnabled(devId, enabled));
    }

    virtual int32_t RegDisplayVBlankCallback(uint32_t devId, VBlankCallback cb, void *data) override
    {
        vBlankCb_ = cb;
        vBlankCbData_ = data;
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->RegDisplayVBlankCallback(devId, this));
    }

    virtual int32_t GetDisplayReleaseFence(
        uint32_t devId, std::vector<uint32_t>& layers, std::vector<int32_t>& fences) override
    {
        std::vector<sptr<HdifdParcelable>> hdiFences;
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        int32_t ret = ToDispErrCode(hdi_->GetDisplayReleaseFence(devId, layers, hdiFences));
        if (ret == DISPLAY_SUCCESS) {
            for (uint32_t i = 0; i < hdiFences.size(); i++) {
                if (hdiFences[i] == nullptr) {
                    HDF_LOGE("%{public}s: GetReleaseFence contains nullptr", __func__);
                    continue;
                }
                fences.push_back(hdiFences[i]->Move());
            }
        }
        return ret;
    }

    virtual int32_t CreateVirtualDisplay(uint32_t width, uint32_t height, int32_t& format, uint32_t& devId) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->CreateVirtualDisplay(width, height, format, devId));
    }

    virtual int32_t DestroyVirtualDisplay(uint32_t devId) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->DestroyVirtualDisplay(devId));
    }

    virtual int32_t SetVirtualDisplayBuffer(uint32_t devId, const BufferHandle& buffer, const int32_t fence) override
    {
        int32_t ret = DISPLAY_SUCCESS;

        sptr<NativeBuffer> hdiBuffer = new NativeBuffer();
        if (hdiBuffer == nullptr) {
            ret = DISPLAY_FAILURE;
        } else {
            hdiBuffer->SetBufferHandle(const_cast<BufferHandle*>(&buffer));
            sptr<HdifdParcelable> hdiFence(new HdifdParcelable);
            hdiFence->Init(fence);
            ret = ToDispErrCode(hdi_->SetVirtualDisplayBuffer(devId, hdiBuffer, hdiFence));
        }

        return ret;
    }

    virtual int32_t SetDisplayProperty(uint32_t devId, uint32_t id, uint64_t value) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->SetDisplayProperty(devId, id, value));
    }

    virtual int32_t Commit(uint32_t devId, int32_t& fence) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->Commit(devId, fence));
    }

    virtual int32_t GetDisplaySupportedColorGamuts(uint32_t devId, std::vector<ColorGamut>& gamuts) override
    {
        return DISPLAY_NOT_SUPPORT;
    }
    virtual int32_t GetDisplayColorGamut(uint32_t devId, ColorGamut& gamut) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t SetDisplayColorGamut(uint32_t devId, const ColorGamut& gamut) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t GetDisplayGamutMap(uint32_t devId, GamutMap& gamutMap) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t SetDisplayGamutMap(uint32_t devId, const GamutMap& gamutMap) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t SetDisplayColorTransform(uint32_t devId, const std::vector<float>& matrix) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t GetHDRCapabilityInfos(uint32_t devId, HDRCapability& info) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t GetSupportedMetadataKey(uint32_t devId, std::vector<HDRMetadataKey>& keys) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    // layer func
    virtual int32_t CreateLayer(uint32_t devId, const LayerInfo& layerInfo, uint32_t cacheCount,
        uint32_t& layerId) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->CreateLayer(devId, layerInfo, cacheCount, layerId));
    }

    virtual int32_t DestroyLayer(uint32_t devId, uint32_t layerId) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(hdi_->DestroyLayer(devId, layerId));
    }

    virtual int32_t PrepareDisplayLayers(uint32_t devId, bool& needFlushFb) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->PrepareDisplayLayers(devId, needFlushFb));
    }

    virtual int32_t SetLayerAlpha(uint32_t devId, uint32_t layerId, const LayerAlpha& alpha) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerAlpha(devId, layerId, alpha));
    }

    virtual int32_t SetLayerRegion(uint32_t devId, uint32_t layerId, const IRect& rect) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerRegion(devId, layerId, rect));
    }

    virtual int32_t SetLayerCrop(uint32_t devId, uint32_t layerId, const IRect& rect) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerCrop(devId, layerId, rect));
    }

    virtual int32_t SetLayerZorder(uint32_t devId, uint32_t layerId, uint32_t zorder) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerZorder(devId, layerId, zorder));
    }

    virtual int32_t SetLayerPreMulti(uint32_t devId, uint32_t layerId, bool preMul) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerPreMulti(devId, layerId, preMul));
    }

    virtual int32_t SetLayerTransformMode(uint32_t devId, uint32_t layerId, TransformType type) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerTransformMode(devId, layerId, type));
    }

    virtual int32_t SetLayerDirtyRegion(uint32_t devId, uint32_t layerId, std::vector<IRect>& rects) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerDirtyRegion(devId, layerId, rects));
    }

    virtual int32_t SetLayerVisibleRegion(uint32_t devId, uint32_t layerId, std::vector<IRect>& rects) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerVisibleRegion(devId, layerId, rects));
    }

    virtual int32_t SetLayerBuffer(uint32_t devId, uint32_t layerId, const BufferHandle* buffer, uint32_t seqNo,
        int32_t fence, const std::vector<uint32_t>& deletingList) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerBuffer(devId, layerId, buffer, seqNo, fence, deletingList));
    }

    virtual int32_t SetLayerCompositionType(uint32_t devId, uint32_t layerId, CompositionType type) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerCompositionType(devId, layerId, type));
    }

    virtual int32_t SetLayerBlendType(uint32_t devId, uint32_t layerId, BlendType type) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerBlendType(devId, layerId, type));
    }

    virtual int32_t SetLayerMaskInfo(uint32_t devId, uint32_t layerId, const MaskInfo maskInfo) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(hdi_);
        return ToDispErrCode(req_->SetLayerMaskInfo(devId, layerId, maskInfo));
    }

    virtual int32_t SetLayerColorTransform(uint32_t devId, uint32_t layerId, const std::vector<float>& matrix) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t SetLayerColorDataSpace(uint32_t devId, uint32_t layerId, const ColorDataSpace colorSpace) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t GetLayerColorDataSpace(uint32_t devId, uint32_t layerId, ColorDataSpace& colorSpace) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t SetLayerMetaData(uint32_t devId, uint32_t layerId,
        const std::vector<HDRMetaData>& metaData) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t SetLayerMetaDataSet(uint32_t devId, uint32_t layerId, HDRMetadataKey key,
        const std::vector<uint8_t>& metaData) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t GetSupportedPresentTimestamp(uint32_t devId, uint32_t layerId, PresentTimestampType& type) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t GetHwPresentTimestamp(uint32_t devId, uint32_t layerId, PresentTimestamp& pts) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t SetLayerTunnelHandle(uint32_t devId, uint32_t layerId, const ExtDataHandle& handle) override
    {
        return DISPLAY_NOT_SUPPORT;
    }

    virtual int32_t SetLayerColor(uint32_t devId, uint32_t layerId, const LayerColor& layerColor) override
    {
        COMPOSER_CHECK_NULLPTR_RETURN(req_);
        return ToDispErrCode(req_->SetLayerColor(devId, layerId, layerColor));
    }

    // Callback implement
    virtual int32_t OnHotPlug(uint32_t outputId, bool connected) override
    {
        DISPLAY_TRACE;
        
        HDF_LOGI("OnHotPlug(%{public}u, %{public}u)", outputId, connected);
        int32_t ret = HDF_SUCCESS;
        if (hotPlugCb_ != nullptr) {
            hotPlugCb_(outputId, connected, hotPlugCbData_);
        } else {
            HDF_LOGE("error: hot plug callback fn is nullptr");
            ret = HDF_FAILURE;
        }

        return ret;
    }

    virtual int32_t OnVBlank(uint32_t sequence, uint64_t ns) override
    {
        DISPLAY_TRACE;

        int32_t ret = HDF_SUCCESS;
        if (vBlankCb_ != nullptr) {
            vBlankCb_(sequence, ns, vBlankCbData_);
        } else {
            HDF_LOGE("error: vblank callback fn is nullptr");
            ret = HDF_FAILURE;
        }

        return ret;
    }

protected:
    int32_t ToDispErrCode(int32_t hdfStatus)
    {
        int32_t ec = DISPLAY_FAILURE;
        switch (hdfStatus) {
            case HDF_SUCCESS:
                ec = DISPLAY_SUCCESS;
                break;
            case HDF_FAILURE:
                ec = DISPLAY_FAILURE;
                break;
            case HDF_ERR_NOT_SUPPORT:
                ec = DISPLAY_NOT_SUPPORT;
                break;
            case HDF_ERR_INVALID_PARAM:
                ec = DISPLAY_PARAM_ERR;
                break;
            case HDF_ERR_DEVICE_BUSY:
                ec = DISPLAY_SYS_BUSY;
                break;
            case HDF_ERR_BAD_FD:
                ec = DISPLAY_FD_ERR;
                break;
            case HDF_ERR_NOPERM:
                ec = DISPLAY_NOT_PERM;
                break;
            case HDF_DEV_ERR_NO_MEMORY:
                ec = DISPLAY_NOMEM;
                break;
            case HDF_ERR_IO:
            case HDF_ERR_INVALID_OBJECT:
            case HDF_ERR_MALLOC_FAIL:
            case HDF_ERR_TIMEOUT:
            case HDF_ERR_THREAD_CREATE_FAIL:
            case HDF_ERR_QUEUE_FULL:
            case HDF_BSP_ERR_OP:
            case HDF_ERR_BSP_PLT_API_ERR:
            case HDF_PAL_ERR_DEV_CREATE:
            case HDF_PAL_ERR_INNER:
            case HDF_DEV_ERR_NO_DEVICE:
            case HDF_DEV_ERR_NO_DEVICE_SERVICE:
            case HDF_DEV_ERR_DEV_INIT_FAIL:
            case HDF_DEV_ERR_PUBLISH_FAIL:
            case HDF_DEV_ERR_ATTACHDEV_FAIL:
            case HDF_DEV_ERR_NODATA:
            case HDF_DEV_ERR_NORANGE:
            case HDF_DEV_ERR_OP:
            default:
                break;
        }
        return ec;
    }

protected:
    static constexpr uint32_t WAIT_TIME_INTERVAL = 10000;
    sptr<CompHdi> hdi_;
    std::shared_ptr<CmdReq> req_;
    HotPlugCallback hotPlugCb_;
    VBlankCallback vBlankCb_;
    void *hotPlugCbData_;
    void *vBlankCbData_;
    sptr<IRemoteObject::DeathRecipient> recipient_;
};
using HdiDisplayComposer = DisplayComposerHdiImpl<IDisplayComposerInterface, IDisplayComposer, HdiDisplayCmdRequester>;
} // namespace V1_0
} // namespace Composer
} // namespace Display
} // namespace HDI
} // namespace OHOS
#endif // OHOS_HDI_DISPLAY_V1_0_DISPLAY_COMPOSER_HDI_IMPL_H
