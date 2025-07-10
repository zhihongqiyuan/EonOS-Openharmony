/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 )(the "License");
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

#ifndef OHOS_HDI_VDI_ADAPTER_INTERFACE_H
#define OHOS_HDI_VDI_ADAPTER_INTERFACE_H

#include <vector>
#include <string>
#include "base/buffer_handle.h"
#include "v1_0/include/idisplay_composer_interface.h"
#include "v1_0/display_composer_type.h"
#include "v1_2/display_composer_type.h"
#include "v1_1/imode_callback.h"
#include "v1_1/iseamless_change_callback.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
using namespace OHOS::HDI::Display::Composer::V1_0;
using namespace OHOS::HDI::Display::Composer::V1_1;
using namespace OHOS::HDI::Display::Composer::V1_2;
struct DisplayComposerVdiAdapter {
    int32_t (*LoadVdiImpl)();
    int32_t (*DestroyVdiImpl)();
    int32_t (*RegHotPlugCallback)(HotPlugCallback cb, void* data);
    int32_t (*GetDisplayCapability)(uint32_t devId, DisplayCapability& info);
    int32_t (*GetDisplaySupportedModes)(uint32_t devId, std::vector<DisplayModeInfo>& modes);
    int32_t (*GetDisplayMode)(uint32_t devId, uint32_t& modeId);
    int32_t (*SetDisplayMode)(uint32_t devId, uint32_t modeId);
    int32_t (*GetDisplayPowerStatus)(uint32_t devId, V1_0::DispPowerStatus& status);
    int32_t (*SetDisplayPowerStatus)(uint32_t devId, V1_0::DispPowerStatus status);
    int32_t (*GetDisplayBacklight)(uint32_t devId, uint32_t& level);
    int32_t (*SetDisplayBacklight)(uint32_t devId, uint32_t level);
    int32_t (*GetDisplayProperty)(uint32_t devId, uint32_t id, uint64_t& value);
    int32_t (*GetDisplayCompChange)(uint32_t devId, std::vector<uint32_t>& layers, std::vector<int32_t>& types);
    int32_t (*SetDisplayClientCrop)(uint32_t devId, const IRect& rect);
    int32_t (*SetDisplayClientBuffer)(uint32_t devId, const BufferHandle& buffer, int32_t fence);
    int32_t (*SetDisplayClientDamage)(uint32_t devId, std::vector<IRect>& rects);
    int32_t (*SetDisplayVsyncEnabled)(uint32_t devId, bool enabled);
    int32_t (*RegDisplayVBlankCallback)(uint32_t devId, VBlankCallback cb, void* data);
    int32_t (*GetDisplayReleaseFence)(uint32_t devId, std::vector<uint32_t>& layers, std::vector<int32_t>& fences);
    int32_t (*CreateVirtualDisplay)(uint32_t width, uint32_t height, int32_t& format, uint32_t& devId);
    int32_t (*DestroyVirtualDisplay)(uint32_t devId);
    int32_t (*SetVirtualDisplayBuffer)(uint32_t devId, const BufferHandle& buffer, const int32_t fence);
    int32_t (*SetDisplayProperty)(uint32_t devId, uint32_t id, uint64_t value);
    int32_t (*Commit)(uint32_t devId, int32_t& fence);
    int32_t (*CreateLayer)(uint32_t devId, const LayerInfo& layerInfo, uint32_t& layerId);
    int32_t (*DestroyLayer)(uint32_t devId, uint32_t layerId);
    int32_t (*PrepareDisplayLayers)(uint32_t devId, bool& needFlushFb);
    int32_t (*SetLayerAlpha)(uint32_t devId, uint32_t layerId, const LayerAlpha& alpha);
    int32_t (*SetLayerRegion)(uint32_t devId, uint32_t layerId, const IRect& rect);
    int32_t (*SetLayerCrop)(uint32_t devId, uint32_t layerId, const IRect& rect);
    int32_t (*SetLayerZorder)(uint32_t devId, uint32_t layerId, uint32_t zorder);
    int32_t (*SetLayerPreMulti)(uint32_t devId, uint32_t layerId, bool preMul);
    int32_t (*SetLayerTransformMode)(uint32_t devId, uint32_t layerId, TransformType type);
    int32_t (*SetLayerDirtyRegion)(uint32_t devId, uint32_t layerId, const std::vector<IRect>& rects);
    int32_t (*SetLayerVisibleRegion)(uint32_t devId, uint32_t layerId, std::vector<IRect>& rects);
    int32_t (*SetLayerBuffer)(uint32_t devId, uint32_t layerId, const BufferHandle& buffer, int32_t fence);
    int32_t (*SetLayerCompositionType)(uint32_t devId, uint32_t layerId, V1_0::CompositionType type);
    int32_t (*SetLayerBlendType)(uint32_t devId, uint32_t layerId, BlendType type);
    int32_t (*SetLayerMaskInfo)(uint32_t devId, uint32_t layerId, const MaskInfo maskInfo);
    int32_t (*SetLayerColor)(uint32_t devId, uint32_t layerId, const LayerColor& layerColor);
    int32_t (*RegSeamlessChangeCallback)(SeamlessChangeCallback cb, void* data);
    int32_t (*GetDisplaySupportedModesExt)(uint32_t devId, std::vector<DisplayModeInfoExt>& modes);
    int32_t (*SetDisplayModeAsync)(uint32_t devId, uint32_t modeId, ModeCallback cb, void *data);
    int32_t (*GetDisplayVBlankPeriod)(uint32_t devId, uint64_t& period);
    int32_t (*SetLayerPerFrameParameter)(
        uint32_t devId, uint32_t layerId, const std::string& key, const std::vector<int8_t>& value);
    int32_t (*SetDisplayPerFrameParameter)(
        uint32_t devId, const std::string& key, const std::vector<int8_t>& value);
    int32_t (*GetSupportedLayerPerFrameParameterKey)(std::vector<std::string>& keys);
    int32_t (*SetDisplayOverlayResolution)(uint32_t devId, uint32_t width, uint32_t height);
    int32_t (*RegRefreshCallback)(RefreshCallback cb, void* data);
    int32_t (*GetDisplaySupportedColorGamuts)(uint32_t devId, std::vector<ColorGamut>& gamuts);
    int32_t (*GetHDRCapabilityInfos)(uint32_t devId, HDRCapability& info);
    int32_t (*RegDisplayVBlankIdleCallback)(VBlankIdleCallback cb, void* data);
    int32_t (*SetDisplayConstraint)(uint32_t devId, uint64_t frameID, uint64_t ns, uint32_t type);
    int32_t (*UpdateHardwareCursor)(uint32_t devId, int32_t x, int32_t y, BufferHandle* buffer);
    int32_t (*EnableHardwareCursorStats)(uint32_t devId, bool enable);
    int32_t (*GetHardwareCursorStats)(uint32_t devId, uint32_t& frameCount, uint32_t& vsyncCount);
    int32_t (*SetDisplayActiveRegion)(uint32_t devId, const IRect& rect);
    int32_t (*FastPresent)(uint32_t devId, const PresentParam& param,
        const std::vector<BufferHandle*>& inHandles);
    int32_t (*ClearDisplayClientBuffer)(uint32_t devId);
    int32_t (*ClearLayerBuffer)(uint32_t devId, uint32_t layerId);
    int32_t (*GetDisplayIdentificationData)(uint32_t devId, uint8_t& portId, std::vector<uint8_t>& edidData);
};

using LoadVdiImplFunc = int32_t (*)();
using DestroyVdiImplFunc = int32_t (*)();
using RegHotPlugCallbackFunc = int32_t (*)(HotPlugCallback cb, void* data);
using GetDisplayCapabilityFunc = int32_t (*)(uint32_t devId, DisplayCapability& info);
using GetDisplaySupportedModesFunc = int32_t (*)(uint32_t devId, std::vector<DisplayModeInfo>& modes);
using GetDisplayModeFunc = int32_t (*)(uint32_t devId, uint32_t& modeId);
using SetDisplayModeFunc = int32_t (*)(uint32_t devId, uint32_t modeId);
using GetDisplayPowerStatusFunc = int32_t (*)(uint32_t devId, V1_0::DispPowerStatus& status);
using SetDisplayPowerStatusFunc = int32_t (*)(uint32_t devId, V1_0::DispPowerStatus status);
using GetDisplayBacklightFunc = int32_t (*)(uint32_t devId, uint32_t& level);
using SetDisplayBacklightFunc = int32_t (*)(uint32_t devId, uint32_t level);
using GetDisplayPropertyFunc = int32_t (*)(uint32_t devId, uint32_t id, uint64_t& value);
using GetDisplayCompChangeFunc = int32_t (*)(
    uint32_t devId, std::vector<uint32_t>& layers, std::vector<int32_t>& types);
using SetDisplayClientCropFunc = int32_t (*)(uint32_t devId, const IRect& rect);
using SetDisplayClientBufferFunc = int32_t (*)(uint32_t devId, const BufferHandle& buffer, int32_t fence);
using SetDisplayClientDamageFunc = int32_t (*)(uint32_t devId, std::vector<IRect>& rects);
using SetDisplayVsyncEnabledFunc = int32_t (*)(uint32_t devId, bool enabled);
using RegDisplayVBlankCallbackFunc = int32_t (*)(uint32_t devId, VBlankCallback cb, void* data);
using GetDisplayReleaseFenceFunc = int32_t (*)(
    uint32_t devId, std::vector<uint32_t>& layers, std::vector<int32_t>& fences);
using CreateVirtualDisplayFunc = int32_t (*)(uint32_t width, uint32_t height, int32_t& format, uint32_t& devId);
using DestroyVirtualDisplayFunc = int32_t (*)(uint32_t devId);
using SetVirtualDisplayBufferFunc = int32_t (*)(uint32_t devId, const BufferHandle& buffer, const int32_t fence);
using SetDisplayPropertyFunc = int32_t (*)(uint32_t devId, uint32_t id, uint64_t value);
using CommitFunc = int32_t (*)(uint32_t devId, int32_t& fence);
using CreateLayerFunc = int32_t (*)(uint32_t devId, const LayerInfo& layerInfo, uint32_t& layerId);
using DestroyLayerFunc = int32_t (*)(uint32_t devId, uint32_t layerId);
using PrepareDisplayLayersFunc = int32_t (*)(uint32_t devId, bool& needFlushFb);
using SetLayerAlphaFunc = int32_t (*)(uint32_t devId, uint32_t layerId, const LayerAlpha& alpha);
using SetLayerRegionFunc = int32_t (*)(uint32_t devId, uint32_t layerId, const IRect& rect);
using SetLayerCropFunc = int32_t (*)(uint32_t devId, uint32_t layerId, const IRect& rect);
using SetLayerZorderFunc = int32_t (*)(uint32_t devId, uint32_t layerId, uint32_t zorder);
using SetLayerPreMultiFunc = int32_t (*)(uint32_t devId, uint32_t layerId, bool preMul);
using SetLayerTransformModeFunc = int32_t (*)(uint32_t devId, uint32_t layerId, TransformType type);
using SetLayerDirtyRegionFunc = int32_t (*)(uint32_t devId, uint32_t layerId, const std::vector<IRect>& rects);
using SetLayerVisibleRegionFunc = int32_t (*)(uint32_t devId, uint32_t layerId, std::vector<IRect>& rects);
using SetLayerBufferFunc = int32_t (*)(uint32_t devId, uint32_t layerId, const BufferHandle& buffer, int32_t fence);
using SetLayerCompositionTypeFunc = int32_t (*)(uint32_t devId, uint32_t layerId, V1_0::CompositionType type);
using SetLayerBlendTypeFunc = int32_t (*)(uint32_t devId, uint32_t layerId, BlendType type);
using SetLayerMaskInfoFunc = int32_t (*)(uint32_t devId, uint32_t layerId, const MaskInfo maskInfo);
using SetLayerColorFunc = int32_t (*)(uint32_t devId, uint32_t layerId, const LayerColor& layerColor);
using RegSeamlessChangeCallbackFunc = int32_t (*)(SeamlessChangeCallback cb, void* data);
using GetDisplaySupportedModesExtFunc = int32_t (*)(uint32_t devId, std::vector<DisplayModeInfoExt>& modes);
using SetDisplayModeAsyncFunc = int32_t (*)(uint32_t devId, uint32_t modeId, ModeCallback cb, void *data);
using GetDisplayVBlankPeriodFunc = int32_t (*)(uint32_t devId, uint64_t& period);
using SetLayerPerFrameParameterFunc = int32_t (*)(
    uint32_t devId, uint32_t layerId, const std::string& key, const std::vector<int8_t>& value);
using SetDisplayPerFrameParameterFunc = int32_t (*)(
    uint32_t devId, const std::string& key, const std::vector<int8_t>& value);
using GetSupportedLayerPerFrameParameterKeyFunc = int32_t (*)(std::vector<std::string>& keys);
using SetDisplayOverlayResolutionFunc = int32_t (*)(uint32_t devId, uint32_t width, uint32_t height);
using RegRefreshCallbackFunc = int32_t (*)(RefreshCallback cb, void* data);
using GetDisplaySupportedColorGamutsFunc = int32_t (*)(uint32_t devId, std::vector<ColorGamut>& gamuts);
using GetHDRCapabilityInfosFunc = int32_t (*)(uint32_t devId, HDRCapability& info);
using RegDisplayVBlankIdleCallbackFunc = int32_t (*)(VBlankIdleCallback cb, void* data);
using SetDisplayConstraintFunc = int32_t (*)(uint32_t devId, uint64_t frameID, uint64_t ns, uint32_t type);
using UpdateHardwareCursorFunc = int32_t (*)(uint32_t devId, int32_t x, int32_t y, BufferHandle* buffer);
using EnableHardwareCursorStatsFunc = int32_t (*)(uint32_t devId, bool enable);
using GetHardwareCursorStatsFunc = int32_t (*)(uint32_t devId, uint32_t& frameCount, uint32_t& vsyncCount);
using SetDisplayActiveRegionFunc = int32_t (*)(uint32_t devId, const IRect& rect);
using FastPresentFunc = int32_t (*)(uint32_t devId, const PresentParam& param,
    const std::vector<BufferHandle*>& inHandles);
using ClearDisplayClientBufferFunc = int32_t (*)(uint32_t devId);
using ClearLayerBufferFunc = int32_t (*)(uint32_t devId, uint32_t layerId);
using GetDisplayIdentificationDataFunc = int32_t (*)(uint32_t devId, uint8_t& portId, std::vector<uint8_t>& edidData);


} // namespace Composer
} // namespace Display
} // namespace HDI
} // namespace OHOS
#endif // OHOS_HDI_VDI_ADAPTER_INTERFACE_H
