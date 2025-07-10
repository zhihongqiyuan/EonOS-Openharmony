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

#ifndef OHOS_HDI_DISPLAY_V1_1_IDISPLAY_COMPOSER_INTERFACE_H
#define OHOS_HDI_DISPLAY_V1_1_IDISPLAY_COMPOSER_INTERFACE_H

#include "v1_0/include/idisplay_composer_interface.h"
#include "v1_1/display_composer_type.h"

namespace OHOS {
namespace HDI {
namespace Display {
namespace Composer {
namespace V1_1 {

class IDisplayComposerInterface : public V1_0::IDisplayComposerInterface {
public:
    /**
     * @brief Obtains all interfaces of IDisplayComposerInterface.
     *
     * @return Returns <b>IDisplayComposerInterface*</b> if the operation is successful;
     * returns an null point otherwise.
     * @since 4.0
     * @version 1.1
     */
    static IDisplayComposerInterface* Get(bool needSMQ = true);

    /**
     * @brief Registers the callback to be invoked when it's ready to change framerate.
     *
     * @param cb Indicates the callback
     * @param data Data used by cb
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t RegSeamlessChangeCallback(SeamlessChangeCallback cb, void* data) = 0;

    /**
     * @brief Obtains the display modes supported by a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param modes Indicates the vector of the information about all modes supported by the display device,
     * including all supported resolutions, refresh rates and groupId. Each mode has an ID, which will be used when
     * the mode is set or obtained. For details, see {@link DisplayModeInfoExt}.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t GetDisplaySupportedModesExt(unsigned int devId, std::vector<DisplayModeInfoExt>& modes) = 0;

    /**
     * @brief Sets the display mode of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param modeId Indicates the ID of the display mode. The device is switched to the display mode specified by
     * this parameter in this interface.
     * @param cb Indicates the callback to be invoked when mode is change.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t SetDisplayModeAsync(uint32_t devId, uint32_t modeId, ModeCallback cb) = 0;

    /**
     * @brief Get the current vblank period.
     * @param devId Indicates the ID of the display device.
     * @param period Indicates the vblank period(ns).
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t GetDisplayVBlankPeriod(uint32_t devId, uint64_t &period) = 0;

    /* *
     * @brief Set the layer per frame keys supported by a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param layerId Indicates the layer ID, which uniquely identifies a layer. You can perform operations on the layer
     * with the specified layer ID.
     * @param key Indicates the metadata key.
     * @param value Indicates the property to get.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t SetLayerPerFrameParameter(uint32_t devId, uint32_t layerId, const std::string& key,
        const std::vector<int8_t>& value) = 0;

    /* *
     * @brief Obtains the layer per frame keys supported by a display device.
     *
     * @param keys Indicates the vector of the information about all HDR metadata keys supported by the display device.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t GetSupportedLayerPerFrameParameterKey(std::vector<std::string>& keys) = 0;

    /* *
     * @brief Set display width and height of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param width Indicates the pixel width of the display device.
     * @param height Indicates the pixel height of the display device.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t SetDisplayOverlayResolution(uint32_t devId, uint32_t width, uint32_t height) = 0;

    /**
     * @brief Registers the callback to be invoked when a refresh event occurs.
     *
     * @param cb Indicates the instance used to notify the graphics service of a refresh event occurred.
     * @param data Indicates the pointer to the private data returned to the graphics service in the
     * <b>RefreshCallback</b> callback.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t RegRefreshCallback(RefreshCallback cb, void *data) = 0;

    /* *
     * @brief Obtains the capabilities of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param gamuts Indicates the vector of the information about all color gamuts supported by the display device.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t GetDisplaySupportedColorGamuts(uint32_t devId, std::vector<ColorGamut>& gamuts) = 0;

    /* *
     * @brief Obtains the capabilities of a display device.
     *
     * @param devId Indicates the ID of the display device.
     * @param info Indicates the pointer to the capabilities supported by the hdr device.
     *
     * @return Returns <b>0</b> if the operation is successful; returns an error code defined
     * in {@link DispErrCode} otherwise.
     * @since 4.1
     * @version 1.1
     */
    virtual int32_t GetHDRCapabilityInfos(uint32_t devId, HDRCapability& info) = 0;
};
} // V1_1
} // Composer
} // Display
} // HDI
} // OHOS
#endif // OHOS_HDI_DISPLAY_V1_1_IDISPLAY_COMPOSER_INTERFACE_H
