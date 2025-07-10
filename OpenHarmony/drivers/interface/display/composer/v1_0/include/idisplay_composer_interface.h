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

#ifndef OHOS_HDI_DISPLAY_V1_0_IDISPLAY_COMPOSER_INTERFACE_H
#define OHOS_HDI_DISPLAY_V1_0_IDISPLAY_COMPOSER_INTERFACE_H

#include <vector>
#include "buffer_handle.h"
#include "common/include/display_common.h"
#include "iproxy_broker.h"
#include "v1_0/display_composer_type.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
namespace V1_0 {
using namespace OHOS::HDI::Display::Composer::V1_0;

class IDisplayComposerInterface : public virtual RefBase {
public:
    virtual ~IDisplayComposerInterface() = default;

    /**
     * @brief Obtains all interfaces of IDisplayComposerInterface.
     *
     * @return Returns <b>IDisplayComposerInterface*</b> if the operation is successful;
     * returns an null point otherwise.
     * @since 4.0
     * @version 1.0
     */
    static IDisplayComposerInterface* Get(bool needSMQ = true);

    /**
     * @brief Adds a recipient for death notifications of a remote stub object.
     *
     * @param recipient Indicates the recipient of the DeathRecipient pointer.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual bool AddDeathRecipient(const sptr<IRemoteObject::DeathRecipient>& recipient) = 0;

    /**
     * @brief Removes the recipient for death notifications of a remote stub object.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual bool RemoveDeathRecipient() = 0;

    /* device func */
    /**
     * @brief Registers the callback to be invoked when a hot plug event occurs.
     *
     * @param cb Indicates the instance used to notify the graphics service of a hot plug event occurred.
     * @param data Indicates the pointer to the private data returned to the graphics service in the
     * <b>HotPlugCallback</b> callback.

     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t RegHotPlugCallback(HotPlugCallback cb, void* data) = 0;

    /**
     * @brief Sets the client buffer cache count of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param count client buffer cache count.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetClientBufferCacheCount(uint32_t devId, uint32_t count) = 0;

    /**
     * @brief Obtains the capabilities of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param info Indicates the pointer to the capabilities supported by the display device. For details,
     * see {@link DisplayCapability}.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayCapability(uint32_t devId, DisplayCapability& info) = 0;

    /**
     * @brief Obtains the display modes supported by a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param modes Indicates the vector of the information about all modes supported by the display device,
     * including all supported resolutions and refresh rates. Each mode has an ID, which will be used when
     * the mode is set or obtained. For details, see {@link DisplayModeInfo}.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplaySupportedModes(uint32_t devId, std::vector<DisplayModeInfo>& modes) = 0;

    /**
     * @brief Obtains the current display mode of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param modeId indicates the pointer to the ID of the current display mode of the device. The display mode ID
     * is written by this API.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayMode(uint32_t devId, uint32_t& modeId) = 0;

    /**
     * @brief Sets the display mode of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param modeId Indicates the ID of the display mode. The device is switched to the display mode specified by
     * this parameter in this interface.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayMode(uint32_t devId, uint32_t modeId) = 0;

    /**
     * @brief Obtains the power status of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param status Indicates the pointer to the power status of the device. The status is written by this interface.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayPowerStatus(uint32_t devId, DispPowerStatus& status) = 0;

    /**
     * @brief Sets the power status of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param status Indicates the power status to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayPowerStatus(uint32_t devId, DispPowerStatus status) = 0;

    /**
     * @brief Obtains the backlight value of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param level Indicates the pointer to the backlight value of the device. The backlight value is written
     * by this interface.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayBacklight(uint32_t devId, uint32_t& level) = 0;

    /**
     * @brief Sets the backlight value for a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param level Indicates the backlight value to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayBacklight(uint32_t devId, uint32_t level) = 0;

    /**
     * @brief Obtains the property for a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param id Indicates the property ID returned by <b>GetDisplayCapability</b>.
     * @param value Indicates the property to get.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayProperty(uint32_t devId, uint32_t id, uint64_t& value) = 0;

    /**
     * @brief Obtains the layers whose composition types have changed.
     *
     * In the preparation for composition, the display device changes the composition type for each layer based on
     * the composition capability of the device. This function returns the layers whose composition types have changed.
     *
     * @param devId Indicates the ID of the display device.
     * @param layers Indicates the vector of the start address of the layer array.
     * @param types Indicates the vector of the start address of the composition type array.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayCompChange(uint32_t devId, std::vector<uint32_t>& layers,
        std::vector<int32_t>& types) = 0;

    /**
     * @brief Sets the cropped region for a display device.
     *
     * You can use this interface to set the cropped region of the client buffer of the display device.
     * The cropped region cannot exceed the size of the client buffer.
     *
     * @param devId Indicates the ID of the display device.
     * @param rect Indicates the pointer to the cropped region of the client buffer.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayClientCrop(uint32_t devId, const IRect& rect) = 0;

    /**
     * @brief Sets the display buffer for a display device.
     *
     * The display buffer stores the hardware composition result of the display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param buffer Indicates the pointer to the display buffer.
     * @param seqNo Indicates the sequence number of buffer cache.
     * @param fence Indicates the sync fence that specifies whether the display buffer can be accessed. The display
     * buffer is created and released by the graphics service. It can be accessed only when the sync fence is in the
     * signaled state.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayClientBuffer(uint32_t devId, const BufferHandle* buffer, uint32_t seqNo,
        int32_t fence) = 0;

    /**
     * @brief Sets the dirty region for a display device.
     *
     * The dirty region consists of multiple rectangular regions. The rectangular regions can be refreshed based on
     * the settings.
     *
     * @param devId Indicates the ID of the display device.
     * @param rects Indicates the vector of the start address of the rectangle array.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayClientDamage(uint32_t devId, std::vector<IRect>& rects) = 0;
    /**
     * @brief Enables or disables the vertical sync signal.
     *
     * When the vertical sync signal is generated, the <b>VBlankCallback</b> callback registered
     * by <b>RegDisplayVBlankCallback</b> will be invoked. The vertical sync signal must be enabled when the graphics
     * service needs to refresh the display, and disabled when display refresh is not required. The display does not
     * need to refresh when <b>VBlankCallback</b> is invoked and the graphics service composes layers and sends the
     * composition result to the device for display.
     *
     * @param devId Indicates the ID of the display device.
     * @param enabled Specifies whether to enable the vertical sync signal. The value <b>true</b> means to enable the
     * vertical sync signal, and <b>false</b> means to disable it.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode}otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayVsyncEnabled(uint32_t devId, bool enabled) = 0;

    /**
     * @brief Registers the callback to be invoked when a VBLANK event occurs.
     *
     * @param devId Indicates the ID of the display device.
     * @param cb Indicates the instance used to notify the graphics service of the VBLANK event occurred when
     * <b>DisplayVsync</b> is enabled.
     * @param data Indicates the pointer to the private data returned to the graphics service in the
     * <b>VBlankCallback</b> callback.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t RegDisplayVBlankCallback(uint32_t devId, VBlankCallback cb, void* data) = 0;

    /**
     * @brief Obtains the fences of the display layers after the commit operation.
     *
     * @param devId Indicates the ID of the display device.
     * @param layers Indicates the vector of the start address of the layer array.
     * @param fences Indicates the vector of the start address of the fence array.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayReleaseFence(uint32_t devId, std::vector<uint32_t>& layers,
        std::vector<int32_t>& fences) = 0;

    /**
     * @brief Creates a virtual display device.
     *
     * @param width Indicates the pixel width of the display device.
     * @param height Indicates the pixel height of the display device.
     * @param format Indicates the pointer to the pixel format of the display device.
     * For details, see {@link PixelFormat}. The <b>format</b> can be modified based on hardware requirements and
     * returned to the graphics service.
     * @param devId Indicates the pointer to the ID of the virtual display device created.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t CreateVirtualDisplay(uint32_t width, uint32_t height, int32_t& format, uint32_t& devId) = 0;

    /**
     * @brief Destroys a virtual display device.
     *
     * @param devId Indicates the ID of the display device.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t DestroyVirtualDisplay(uint32_t devId) = 0;

    /**
     * @brief Sets the output buffer for a virtual display device.
     *
     * This buffer stores the output of the virtual display device. The buffer can be used only after the sync fence
     * is in the signaled state.
     *
     * @param devId Indicates the ID of the display device.
     * @param buffer Indicates the pointer to the output buffer.
     * @param fence Indicates the sync fence.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetVirtualDisplayBuffer(uint32_t devId, const BufferHandle& buffer, const int32_t fence) = 0;

    /**
     * @brief Sets the property for a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param id Indicates the property ID returned by <b>GetDisplayCapability</b>.
     * @param value Indicates the property to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayProperty(uint32_t devId, uint32_t id, uint64_t value) = 0;

    /**
     * @brief Commits the request for composition and display.
     *
     * If there is a hardware composition layer, the composition is performed and the composition result is sent to
     * the hardware for display.
     *
     * @param devId Indicates the ID of the display device.
     * @param fence Indicates the pointer to the start address of the fence array.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t Commit(uint32_t devId, int32_t& fence) = 0;

    /**
     * @brief Obtains the color gamuts supported by a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param gamuts Indicates the vector of the information about all color gamuts supported by the display device.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplaySupportedColorGamuts(uint32_t devId, std::vector<ColorGamut>& gamuts) = 0;

    /**
     * @brief Obtains the color gamut of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param gamut Indicates the pointer to the color gamut of the device. The color gamut is written
     * by this interface.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayColorGamut(uint32_t devId, ColorGamut& gamut) = 0;

    /**
     * @brief Sets the color gamut for a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param gamut Indicates the color gamut to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayColorGamut(uint32_t devId, const ColorGamut& gamut) = 0;

    /**
     * @brief Obtains the gamut map of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param gamutMap Indicates the pointer to the gamut map of the device. The gamut map is written
     * by this interface.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetDisplayGamutMap(uint32_t devId, GamutMap& gamutMap) = 0;

    /**
     * @brief Sets the gamut map for a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param gamutMap Indicates the gamut map to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayGamutMap(uint32_t devId, const GamutMap& gamutMap) = 0;

    /**
     * @brief Sets a 4x4 color transformation matrix for a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param matrix Indicates the vector of the 4x4 color transformation matrix to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetDisplayColorTransform(uint32_t devId, const std::vector<float>& matrix) = 0;

    /**
     * @brief Obtains the HDR capability of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param info Indicates the pointer to the HDR capability of the device. The <b>info</b> is written
     * by this interface.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetHDRCapabilityInfos(uint32_t devId, HDRCapability& info) = 0;

    /**
     * @brief Obtains the HDR metadata keys supported by a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param keys Indicates the vector of the information about all HDR metadata keys supported by the display device.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetSupportedMetadataKey(uint32_t devId, std::vector<HDRMetadataKey>& keys) = 0;

    /* layer func */
    /**
     * @brief Opens a layer on a specified display device.
     *
     * Before using a layer on the GUI, you must open the layer based on the layer information. After the layer is
     * opened, you can obtain the layer ID and then use other functions based on the layer ID.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerInfo Indicates the pointer to the layer information passed to open a layer, including the layer
     * type, layer size, and pixel format.
     * @param cacheCount Indicates the count of buffer cache.
     * @param layerId Indicates the pointer to the layer ID, which uniquely identifies a layer. The layer ID is returned
     * to the GUI after the layer is successfully opened.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @see DestroyLayer
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t CreateLayer(uint32_t devId, const LayerInfo& layerInfo, uint32_t cacheCount, uint32_t& layerId) = 0;

    /**
     * @brief Closes a layer that is no longer required on a specified display device.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @see OpenLayer
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t DestroyLayer(uint32_t devId, uint32_t layerId) = 0;

    /**
    * @brief Prepares for the composition to be performed by a display device.
    *
    * Before the composition, the graphics service needs to notify the display device of the preparation to be made
    * through this interface.
    *
    * @param devId Indicates the ID of the display device.
    * @param needFlushFb Indicates the pointer that specifies whether the graphics service needs to reset the display
    * framebuffer by using <b>SetDisplayClientBuffer</b> before the commit operation.
    * The value <b>true</b> means that the framebuffer needs to be reset, and <b>false</b> means the opposite.
    *
    * @return Returns <b>0</b> if the operation is successful; returns an error code defined
    * in {@link DispErrCode} otherwise.
    * @since 4.0
    * @version 1.0
    */
    virtual int32_t PrepareDisplayLayers(uint32_t devId, bool& needFlushFb) = 0;

    /**
     * @brief Sets the alpha value for a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param alpha Indicates the pointer to the alpha value to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @see GetLayerAlpha
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerAlpha(uint32_t devId, uint32_t layerId, const LayerAlpha& alpha) = 0;

    /**
     * @brief Sets the region for a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param rect Indicates the pointer to the region where set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @see GetLayerAlpha
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerRegion(uint32_t devId, uint32_t layerId, const IRect& rect) = 0;

    /**
     * @brief Sets the rectangular area to crop for a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param rect Indicates the pointer to the rectangular area to crop.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerCrop(uint32_t devId, uint32_t layerId, const IRect& rect) = 0;

    /**
     * @brief Sets the z-order for a layer.
     *
     * A larger z-order value indicates a higher layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param zorder Indicates the z-order to set. The value is an integer ranging from 0 to 255.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @see GetLayerZorder
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerZorder(uint32_t devId, uint32_t layerId, uint32_t zorder) = 0;

    /**
     * @brief Sets layer premultiplication.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param preMul Specifies whether to enable layer premultiplication. The value <b>1</b> means to enable layer
     * premultiplication, and <b>0</b> means the opposite.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @see GetLayerPreMulti
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerPreMulti(uint32_t devId, uint32_t layerId, bool preMul) = 0;

    /**
     * @brief Sets the type of graphic rotation.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param type Indicates the transform types of images.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerTransformMode(uint32_t devId, uint32_t layerId, TransformType type) = 0;

    /**
     * @brief Sets the flushing area for a layer.
     *
     * After the GUI draws an image, you must set the layer flushing area before calling the {@link Flush} function to
     * flush the screen.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param regions Indicates the vector of the flushing area to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerDirtyRegion(uint32_t devId, uint32_t layerId, std::vector<IRect>& regions) = 0;

    /**
     * @brief Set the visible region for a layer
     *
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param rects Indicates the vector of the rectes.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 2.0
     * @version 2.0
     */
    virtual int32_t SetLayerVisibleRegion(uint32_t devId, uint32_t layerId, std::vector<IRect>& rects) = 0;

    /**
     * @brief Set the buffer for a layer.
     *
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param buffer Indicates the pointer of the buffer handle. The buffer handle should contain all information of the
     * buffer which will be used for composition.
     * @param seqNo Indicates the sequence number of buffer cache.
     * @param fence Indicates the fd of a sync file.
     * @param deletingList Indicates the list of buffer cache to delete.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 2.0
     * @version 2.0
     */
    virtual int32_t SetLayerBuffer(uint32_t devId, uint32_t layerId, const BufferHandle* buffer, uint32_t seqNo,
        int32_t fence, const std::vector<uint32_t>& deletingList) = 0;

    /**
     * @brief set the composition type which the client expect
     *
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param type Indicates the composition type which the client expect. It may vary with the implementation.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 2.0
     * @version 2.0
     */
    virtual int32_t SetLayerCompositionType(uint32_t devId, uint32_t layerId, CompositionType type) = 0;

    /**
     * @brief set the blend type
     *
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param type Indicates blend type
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 2.0
     * @version 2.0
     */
    virtual int32_t SetLayerBlendType(uint32_t devId, uint32_t layerId, BlendType type) = 0;

    /**
     * @brief Sets a 4x4 color transformation matrix.
     *
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param matrix Indicates the 4x4 color transformation matrix.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerColorTransform(uint32_t devId, uint32_t layerId, const std::vector<float>& matrix) = 0;

    /**
     * @brief Sets a color data space for a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param colorSpace Indicates the color data space to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerColorDataSpace(uint32_t devId, uint32_t layerId, const ColorDataSpace colorSpace) = 0;

    /**
     * @brief Obtains the color data space of a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param colorSpace Indicates the pointer to the color data space obtained.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetLayerColorDataSpace(uint32_t devId, uint32_t layerId, ColorDataSpace& colorSpace) = 0;

    /**
     * @brief Sets metadata for a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param num Indicates the number of metadata records.
     * @param metaData Indicates the vecter of the metadata to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerMetaData(uint32_t devId, uint32_t layerId, const std::vector<HDRMetaData>& metaData) = 0;

    /**
     * @brief Sets a metadata set for a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param key Indicates the metadata key.
     * @param metaData Indicates the vector of the metadata set of the uint8_t type to set.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t SetLayerMetaDataSet(uint32_t devId, uint32_t layerId, HDRMetadataKey key,
         const std::vector<uint8_t>& metaData) = 0;

    /**
     * @brief Obtains the hardware display present timestamp type supported by a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param type Indicates the pointer to the present timestamp type obtained.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetSupportedPresentTimestamp(uint32_t devId, uint32_t layerId, PresentTimestampType& type) = 0;

    /**
     * @brief Obtains the hardware display present timestamp of a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param pts Indicates the pointer to the present timestamp obtained.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 4.0
     * @version 1.0
     */
    virtual int32_t GetHwPresentTimestamp(uint32_t devId, uint32_t layerId, PresentTimestamp& pts) = 0;

    /**
     * @brief Sets a tunnel handle for a layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param handle Indicates the handle of ExtDataHandle.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 3.2
     * @version 1.0
     */
    virtual int32_t SetLayerTunnelHandle(uint32_t devId, uint32_t layerId, const ExtDataHandle& handle) = 0;

    /**
     * @brief Sets the current mask frame information to the vendor driver.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param maskInfo Indicates the contains all of the information for a single mask.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 3.2
     * @version 1.0
     */
    virtual int32_t SetLayerMaskInfo(uint32_t devId, uint32_t layerId, const MaskInfo maskInfo) = 0;

    /**
     * @brief Sets the solid color layer.
     *
     * @param devId Indicates the ID of the display device. The value ranges from 0 to 4, where 0 indicates the first
     * display device, and 4 indicates the last display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param layerColor Indicates the current color.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined in {@link DispErrCode}
     * otherwise.
     * @since 3.2
     * @version 1.0
     */
    virtual int32_t SetLayerColor(uint32_t devId, uint32_t layerId, const LayerColor& layerColor) = 0;
};
} // V1_0
} // Composer
} // Display
} // HDI
} // OHOS
#endif // OHOS_HDI_DISPLAY_V1_0_IDISPLAY_COMPOSER_INTERFACE_H
