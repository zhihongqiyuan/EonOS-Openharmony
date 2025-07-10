/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include "camera_metadata_info.h"
#include <securec.h>
#include "metadata_log.h"
#include <dlfcn.h>
#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include "camera_metadata_item_info.h"
#include "camera_vendor_tag.h"

namespace OHOS::Camera {
static std::mutex g_mtx;
static std::mutex g_vendorTagImplMtx;
static CameraVendorTag* g_vendorTagImpl = nullptr;
const char* g_exampleVendorTagLib = "libcamera_example_vendor_tag_impl.z.so";
const char* g_vendorTagLib = "libcamera_vendor_tag_impl.z.so";
const int METADATA_HEADER_DATA_SIZE = 4;
const uint32_t itemLen = sizeof(camera_metadata_item_entry_t);
const std::vector<uint32_t> g_metadataTags = {
    OHOS_ABILITY_CAMERA_POSITION,
    OHOS_ABILITY_CAMERA_TYPE,
    OHOS_ABILITY_CAMERA_CONNECTION_TYPE,
    OHOS_ABILITY_MEMORY_TYPE,
    OHOS_ABILITY_FPS_RANGES,
    OHOS_ABILITY_FLASH_AVAILABLE,
    OHOS_CAMERA_STREAM_ID,
    OHOS_ABILITY_PRELAUNCH_AVAILABLE,
    OHOS_ABILITY_CUSTOM_VIDEO_FPS,
    OHOS_ABILITY_CAMERA_MODES,
    OHOS_ABILITY_SKETCH_ENABLE_RATIO,
    OHOS_ABILITY_SKETCH_REFERENCE_FOV_RATIO,
    OHOS_ABILITY_SKETCH_INFO_NOTIFICATION,
    OHOS_ABILITY_CAMERA_FOLDSCREEN_TYPE,
    OHOS_ABILITY_AVAILABLE_COLOR_SPACES,
    OHOS_ABILITY_NIGHT_MODE_SUPPORTED_EXPOSURE_TIME,
    OHOS_CAMERA_MESURE_EXPOSURE_TIME,
    OHOS_CAMERA_EXPOSURE_MODE_PREVIEW_STATE,
    OHOS_CAMERA_USER_ID,
    OHOS_ABILITY_MOON_CAPTURE_BOOST,
    OHOS_CONTROL_MOON_CAPTURE_BOOST,
    OHOS_STATUS_MOON_CAPTURE_DETECTION,
    OHOS_ABILITY_CAPTURE_DURATION_SUPPORTED,
    OHOS_CAMERA_CUSTOM_SNAPSHOT_DURATION,
    OHOS_ABILITY_MOVING_PHOTO,
    OHOS_ABILITY_FLASH_SUGGESTION_SUPPORTED,
    OHOS_CONTROL_FLASH_SUGGESTION_SWITCH,
    OHOS_STATUS_FLASH_SUGGESTION,
    OHOS_ABILITY_HIGH_QUALITY_SUPPORT,
    OHOS_ABILITY_CAMERA_FOLD_STATUS,
    OHOS_STATUS_CAMERA_OCCLUSION_DETECTION,
    OHOS_ABILITY_STATISTICS_DETECT_TYPE,
    OHOS_CONTROL_STATISTICS_DETECT_SETTING,
    OHOS_ABILITY_AVAILABLE_EXTENDED_STREAM_INFO_TYPES,
    OHOS_ABILITY_AUTO_DEFERRED_VIDEO_ENHANCE,
    OHOS_CONTROL_AUTO_DEFERRED_VIDEO_ENHANCE,
    OHOS_ABILITY_AUTO_CLOUD_IMAGE_ENHANCE,
    OHOS_CONTROL_AUTO_CLOUD_IMAGE_ENHANCE,
    OHOS_ABILITY_TRIPOD_DETECTION,
    OHOS_CONTROL_TRIPOD_DETECTION,
    OHOS_CONTROL_TRIPOD_STABLITATION,
    OHOS_STATUS_TRIPOD_DETECTION_STATUS,
    OHOS_STATUS_SKETCH_POINT,
    OHOS_STATUS_SKETCH_STREAM_INFO,
    OHOS_ABILITY_LOW_LIGHT_BOOST,
    OHOS_CONTROL_LOW_LIGHT_DETECT,
    OHOS_CONTROL_LOW_LIGHT_BOOST,
    OHOS_STATUS_LOW_LIGHT_DETECTION,
    OHOS_STATUS_CAMERA_LENS_DIRTY_DETECTION,
    OHOS_DEVICE_PROTECTION_STATE,
    OHOS_STATUS_TOF_SENSOR_ABNORMAL,
    OHOS_ABILITY_SUN_BLOCK_PREVIEW_TYPES,
    OHOS_ABILITY_SUN_BLOCK_BLEND_COLORS,
    OHOS_ABILITY_SUN_BLOCK_RGB_CROP_REGION,
    OHOS_ABILITY_SUN_BLOCK_RGB_UV_REGION,
    OHOS_STATUS_SUN_BLOCK_HINT,
    OHOS_ABILITY_AUTO_AIGC_PHOTO,
    OHOS_CONTROL_AUTO_AIGC_PHOTO,
    OHOS_ABILITY_CAMERA_LIMITED_CAPABILITIES,
    OHOS_ABILITY_CAMERA_CONCURRENT_TYPE,
    OHOS_ABILITY_CONCURRENT_SUPPORTED_CAMERAS,

    OHOS_SENSOR_EXPOSURE_TIME,
    OHOS_SENSOR_COLOR_CORRECTION_GAINS,
    OHOS_SENSOR_ORIENTATION,
    OHOS_ABILITY_MUTE_MODES,
    OHOS_CONTROL_MUTE_MODE,

    OHOS_SENSOR_INFO_ACTIVE_ARRAY_SIZE,
    OHOS_SENSOR_INFO_SENSITIVITY_RANGE,
    OHOS_SENSOR_INFO_MAX_FRAME_DURATION,
    OHOS_SENSOR_INFO_PHYSICAL_SIZE,
    OHOS_SENSOR_INFO_PIXEL_ARRAY_SIZE,
    OHOS_SENSOR_INFO_TIMESTAMP,

    OHOS_STATISTICS_FACE_DETECT_MODE,
    OHOS_STATISTICS_FACE_DETECT_SWITCH,
    OHOS_STATISTICS_FACE_DETECT_MAX_NUM,
    OHOS_STATISTICS_HISTOGRAM_MODE,
    OHOS_STATISTICS_FACE_IDS,
    OHOS_STATISTICS_FACE_LANDMARKS,
    OHOS_STATISTICS_FACE_RECTANGLES,
    OHOS_STATISTICS_FACE_SCORES,
    OHOS_STATISTICS_DETECT_HUMAN_FACE_INFOS,
    OHOS_STATISTICS_DETECT_HUMAN_BODY_INFOS,
    OHOS_STATISTICS_DETECT_CAT_FACE_INFOS,
    OHOS_STATISTICS_DETECT_CAT_BODY_INFOS,
    OHOS_STATISTICS_DETECT_DOG_FACE_INFOS,
    OHOS_STATISTICS_DETECT_DOG_BODY_INFOS,
    OHOS_STATISTICS_DETECT_SALIENT_INFOS,
    OHOS_STATISTICS_DETECT_BAR_CODE_INFOS,
    OHOS_STATISTICS_DETECT_BASE_FACE_INFO,

    OHOS_CONTROL_AE_ANTIBANDING_MODE,
    OHOS_CONTROL_AE_EXPOSURE_COMPENSATION,
    OHOS_CONTROL_AE_LOCK,
    OHOS_CONTROL_AE_MODE,
    OHOS_CONTROL_AE_REGIONS,
    OHOS_CONTROL_AE_TARGET_FPS_RANGE,
    OHOS_CONTROL_FPS_RANGES,
    OHOS_CONTROL_AF_MODE,
    OHOS_CONTROL_AF_REGIONS,
    OHOS_CONTROL_AF_TRIGGER,
    OHOS_CONTROL_AF_TRIGGER_ID,
    OHOS_CONTROL_AF_STATE,
    OHOS_CONTROL_AWB_LOCK,
    OHOS_CONTROL_AWB_MODE,
    OHOS_CONTROL_AWB_REGIONS,
    OHOS_CONTROL_AE_AVAILABLE_ANTIBANDING_MODES,
    OHOS_CONTROL_AE_AVAILABLE_MODES,
    OHOS_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES,
    OHOS_CONTROL_AE_COMPENSATION_RANGE,
    OHOS_CONTROL_AE_COMPENSATION_STEP,
    OHOS_ABILITY_AE_COMPENSATION_RANGE,
    OHOS_ABILITY_AE_COMPENSATION_STEP,
    OHOS_CONTROL_AF_AVAILABLE_MODES,
    OHOS_CONTROL_AWB_AVAILABLE_MODES,
    OHOS_CONTROL_CAPTURE_MIRROR_SUPPORTED,
    OHOS_CONTROL_CAPTURE_MIRROR,
    OHOS_CONTROL_FOCUS_STATE,
    OHOS_CONTROL_METER_POINT,
    OHOS_CONTROL_METER_MODE,
    OHOS_CONTROL_EXPOSURE_STATE,
    OHOS_CONTROL_FOCUSED_POINT,
    OHOS_CONTROL_NIGHT_MODE_TRY_AE,
    OHOS_CONTROL_MANUAL_EXPOSURE_TIME,
    OHOS_STATUS_CAMERA_CURRENT_FPS,
    OHOS_ABILITY_ISO_VALUES,
    OHOS_CONTROL_ISO_VALUE,
    OHOS_STATUS_ISO_VALUE,
    OHOS_ABILITY_SENSOR_EXPOSURE_TIME_RANGE,
    OHOS_CONTROL_SENSOR_EXPOSURE_TIME,
    OHOS_STATUS_SENSOR_EXPOSURE_TIME,
    OHOS_CONTROL_MOVING_PHOTO,
    OHOS_ABILITY_SENSOR_WB_VALUES,
    OHOS_CONTROL_SENSOR_WB_VALUE,
    OHOS_CONTROL_HIGH_QUALITY_MODE,
    OHOS_CONTROL_BURST_CAPTURE,
    OHOS_CONTROL_CAMERA_USED_AS_POSITION,
    OHOS_CONTROL_CAMERA_SESSION_USAGE,
    OHOS_CONTROL_EJECT_RETRY,
    OHOS_CONTROL_FALL_PROTECTION,
    OHOS_CONTROL_SUN_BLOCK_PREVIEW_TYPE,
    OHOS_CONTROL_SUN_BLOCK_BLEND_COLOR,
    OHOS_CONTROL_QUALITY_PRIORITIZATION,
    OHOS_CONTROL_AUTO_VIDEO_FRAME_RATE,
    OHOS_ABILITY_AUTO_VIDEO_FRAME_RATE,

    // Camera device image acquisition related
    OHOS_ABILITY_DEVICE_AVAILABLE_EXPOSUREMODES,
    OHOS_CONTROL_EXPOSUREMODE,
    OHOS_ABILITY_EXPOSURE_MODES,
    OHOS_CONTROL_EXPOSURE_MODE,
    OHOS_ABILITY_METER_MODES,
    OHOS_ABILITY_SCENE_EXPOSURE_MODES,
    OHOS_ABILITY_EXPOSURE_TIME,
    OHOS_ABILITY_AE_LOCK,

    OHOS_ABILITY_DEVICE_AVAILABLE_FOCUSMODES,
    OHOS_CONTROL_FOCUSMODE,
    OHOS_ABILITY_FOCUS_MODES,
    OHOS_CONTROL_FOCUS_MODE,
    OHOS_ABILITY_FOCAL_LENGTH,
    OHOS_ABILITY_SCENE_FOCUS_MODES,
    OHOS_ABILITY_FOCUS_ASSIST_FLASH_SUPPORTED_MODES,
    OHOS_CONTROL_FOCUS_ASSIST_FLASH_SUPPORTED_MODE,
    OHOS_ABILITY_LENS_INFO_MINIMUM_FOCUS_DISTANCE,
    OHOS_CONTROL_LENS_FOCUS_DISTANCE,
    OHOS_ABILITY_EQUIVALENT_FOCUS,
    OHOS_CONTROL_FOCUS_DISTANCE,
    OHOS_ABILITY_FOCUS_RANGE_TYPES,
    OHOS_CONTROL_FOCUS_RANGE_TYPE,
    OHOS_CONTROL_FOCUS_TRACKING_MODE,
    OHOS_ABILITY_FOCUS_TRACKING_REGION,
    OHOS_ABILITY_FOCUS_DRIVEN_TYPES,
    OHOS_CONTROL_FOCUS_DRIVEN_TYPE,

    OHOS_ABILITY_DEVICE_AVAILABLE_AWBMODES,
    OHOS_ABILITY_AWB_MODES,
    OHOS_ABILITY_AWB_LOCK,
    OHOS_ABILITY_DEVICE_AVAILABLE_FLASHMODES,
    OHOS_CONTROL_FLASHMODE,
    OHOS_ABILITY_FLASH_MODES,
    OHOS_CONTROL_FLASH_MODE,
    OHOS_CONTROL_FLASH_STATE,
    OHOS_ABILITY_SCENE_FLASH_MODES,

    OHOS_ABILITY_ZOOM_RATIO_RANGE,
    OHOS_CONTROL_ZOOM_RATIO,
    OHOS_CONTROL_ZOOM_CROP_REGION,
    OHOS_ABILITY_ZOOM_CAP,
    OHOS_ABILITY_SCENE_ZOOM_CAP,
    OHOS_STATUS_CAMERA_CURRENT_ZOOM_RATIO,
    OHOS_CONTROL_SMOOTH_ZOOM_RATIOS,
    OHOS_CONTROL_PREPARE_ZOOM,
    OHOS_ABILITY_CAMERA_ZOOM_PERFORMANCE,
    OHOS_STATUS_CAMERA_ZOOM_PERFORMANCE,
    OHOS_ABILITY_CAMERA_ZOOM_BEZIER_CURVC_POINT,

    // Camera flow control related
    OHOS_ABILITY_STREAM_AVAILABLE_BASIC_CONFIGURATIONS,
    OHOS_STREAM_AVAILABLE_FORMATS,
    OHOS_ABILITY_STREAM_AVAILABLE_EXTEND_CONFIGURATIONS,
    OHOS_ABILITY_STREAM_QUICK_THUMBNAIL_AVAILABLE,
    OHOS_ABILITY_PORTRAIT_RESOLUSION,
    OHOS_ABILITY_SCENE_STREAM_QUICK_THUMBNAIL_AVAILABLE,
    OHOS_ABILITY_DEFERRED_IMAGE_DELIVERY,
    OHOS_CONTROL_DEFERRED_IMAGE_DELIVERY,
    OHOS_ABILITY_PRERECORD,
    OHOS_CONTROL_PRERECORD_MODE,
    OHOS_CONTROL_RECORD_STATE,
    OHOS_ABILITY_CHANGETO_OFFLINE_STREAM_OPEATOR,
    OHOS_CONTROL_CHANGETO_OFFLINE_STREAM_OPEATOR,

    OHOS_JPEG_GPS_COORDINATES,
    OHOS_JPEG_GPS_PROCESSING_METHOD,
    OHOS_JPEG_GPS_TIMESTAMP,
    OHOS_JPEG_ORIENTATION,
    OHOS_JPEG_QUALITY,
    OHOS_JPEG_THUMBNAIL_QUALITY,
    OHOS_JPEG_THUMBNAIL_SIZE,
    OHOS_JPEG_AVAILABLE_THUMBNAIL_SIZES,
    OHOS_JPEG_MAX_SIZE,
    OHOS_JPEG_SIZE,

    OHOS_ABILITY_VIDEO_STABILIZATION_MODES,
    OHOS_CONTROL_VIDEO_STABILIZATION_MODE,
    OHOS_CONTROL_VIDEO_DEBUG_SWITCH,

    // camera effects & algorithms
    OHOS_ABILITY_SCENE_FILTER_TYPES,
    OHOS_CONTROL_FILTER_TYPE,
    OHOS_ABILITY_SCENE_PORTRAIT_EFFECT_TYPES,
    OHOS_CONTROL_PORTRAIT_EFFECT_TYPE,
    OHOS_ABILITY_SCENE_BEAUTY_TYPES,
    OHOS_CONTROL_BEAUTY_TYPE,
    OHOS_ABILITY_BEAUTY_AUTO_VALUES,
    OHOS_CONTROL_BEAUTY_AUTO_VALUE,
    OHOS_ABILITY_BEAUTY_FACE_SLENDER_VALUES,
    OHOS_CONTROL_BEAUTY_FACE_SLENDER_VALUE,
    OHOS_ABILITY_BEAUTY_SKIN_TONE_VALUES,
    OHOS_CONTROL_BEAUTY_SKIN_TONE_VALUE,
    OHOS_ABILITY_BEAUTY_SKIN_SMOOTH_VALUES,
    OHOS_CONTROL_BEAUTY_SKIN_SMOOTH_VALUE,
    OHOS_ABILITY_CAMERA_MACRO_SUPPORTED,
    OHOS_ABILITY_SCENE_MACRO_CAP,
    OHOS_CAMERA_MACRO_STATUS,
    OHOS_CONTROL_CAMERA_MACRO,
    OHOS_ABILITY_CAMERA_VIRTUAL_APERTURE_RANGE,
    OHOS_CONTROL_CAMERA_VIRTUAL_APERTURE_VALUE,
    OHOS_ABILITY_CAMERA_PHYSICAL_APERTURE_RANGE,
    OHOS_CONTROL_CAMERA_PHYSICAL_APERTURE_VALUE,
    OHOS_STATUS_CAMERA_APERTURE_VALUE,
    OHOS_DEVICE_EXITCAMERA_EVENT,
    OHOS_CONTROL_MOTION_DETECTION_CHECK_AREA,
    OHOS_STATUS_SLOW_MOTION_DETECTION,
    OHOS_ABILITY_MOTION_DETECTION_SUPPORT,
    OHOS_CONTROL_MOTION_DETECTION,
    OHOS_ABILITY_EXPOSURE_HINT_SUPPORTED,
    OHOS_CONTROL_EXPOSURE_HINT_MODE,
    OHOS_STATUS_ALGO_MEAN_Y,
    OHOS_STATUS_PREVIEW_PHYSICAL_CAMERA_ID,
    OHOS_ABILITY_CAPTURE_EXPECT_TIME,
    OHOS_ABILITY_EFFECT_SUGGESTION_SUPPORTED,
    OHOS_CONTROL_EFFECT_SUGGESTION,
    OHOS_CONTROL_EFFECT_SUGGESTION_DETECTION,
    OHOS_CAMERA_EFFECT_SUGGESTION_TYPE,
    OHOS_CONTROL_EFFECT_SUGGESTION_TYPE,
    OHOS_ABILITY_AVAILABLE_PROFILE_LEVEL,
    OHOS_ABILITY_AVAILABLE_PREVIEW_PROFILE,
    OHOS_ABILITY_AVAILABLE_PHOTO_PROFILE,
    OHOS_ABILITY_AVAILABLE_VIDEO_PROFILE,
    OHOS_ABILITY_AVAILABLE_METADATA_PROFILE,
    OHOS_ABILITY_AVAILABLE_CONFIGURATIONS,
    OHOS_ABILITY_CONFLICT_CONFIGURATIONS,
    OHOS_ABILITY_LIGHT_PAINTING_TYPE,
    OHOS_CONTROL_LIGHT_PAINTING_TYPE,
    OHOS_CONTROL_LIGHT_PAINTING_FLASH,
    OHOS_CONTROL_TIME_LAPSE_INTERVAL,
    OHOS_CONTROL_TIME_LAPSE_TRYAE_STATE,
    OHOS_STATUS_TIME_LAPSE_PREVIEW_TYPE,
    OHOS_STATUS_TIME_LAPSE_TRYAE_HINT,
    OHOS_STATUS_TIME_LAPSE_CAPTURE_INTERVAL,
    OHOS_STATUS_TIME_LAPSE_TRYAE_DONE,
    OHOS_CONTROL_TIME_LAPSE_RECORD_STATE,
    OHOS_CONTROL_TIME_LAPSE_PREVIEW_TYPE,
    OHOS_ABILITY_TIME_LAPSE_INTERVAL_RANGE,
    OHOS_ABILITY_LCD_FLASH,
    OHOS_CONTROL_LCD_FLASH_DETECTION,
    OHOS_CONTROL_LCD_FLASH,
    OHOS_STATUS_LCD_FLASH_STATUS,
    OHOS_ABILITY_DEPTH_DATA_DELIVERY,
    OHOS_CONTROL_DEPTH_DATA_DELIVERY_SWITCH,
    OHOS_ABILITY_DEPTH_DATA_PROFILES,
    OHOS_CONTROL_DEPTH_DATA_ACCURACY,
    OHOS_ABILITY_CAMERA_IS_RETRACTABLE,
    OHOS_CONTROL_CAMERA_CLOSE_AFTER_SECONDS,
    OHOS_ABILITY_CAPTURE_MACRO_DEPTH_FUSION_SUPPORTED,
    OHOS_ABILITY_CAPTURE_MACRO_DEPTH_FUSION_ZOOM_RANGE,
    OHOS_CONTROL_CAPTURE_MACRO_DEPTH_FUSION,
    OHOS_ABILITY_CAMERA_PORTRAIT_THEME_SUPPORTED,
    OHOS_ABILITY_CAMERA_PORTRAIT_THEME_TYPES,
    OHOS_CONTROL_CAMERA_PORTRAIT_THEME_TYPE,
    OHOS_ABILITY_CAMERA_VIDEO_ROTATION_SUPPORTED,
    OHOS_ABILITY_CAMERA_VIDEO_ROTATION,
    OHOS_CONTROL_CAMERA_VIDEO_ROTATION,
    OHOS_MOVING_PHOTO_BUFFER_DURATION,
    OHOS_MOVING_PHOTO_START,
    OHOS_MOVING_PHOTO_END,
    OHOS_ABILITY_COLOR_RESERVATION_TYPES,
    OHOS_CONTROL_COLOR_RESERVATION_TYPE,

    // camera secure related
    OHOS_CONTROL_SECURE_FACE_MODE,
    OHOS_CONTROL_SECURE_FACE_INFO,
    OHOS_CONTROL_SECURE_FACE_AUTH_RESULT,
    OHOS_CONTROL_SECURE_FLASH_SEQ,
    OHOS_CONTROL_SECURE_IR_LED_SWITCH,
    OHOS_CONTROL_SECURE_IR_LOCKAE_SWITCH,

    //XMAGE MODES
    OHOS_ABILITY_SUPPORTED_COLOR_MODES,
    OHOS_CONTROL_SUPPORTED_COLOR_MODES,
};

std::map<uint32_t, uint32_t> g_metadataSectionMap = {
    {OHOS_CAMERA_PROPERTIES, OHOS_SECTION_CAMERA_PROPERTIES},
    {OHOS_CAMERA_SENSOR, OHOS_SECTION_CAMERA_SENSOR},
    {OHOS_CAMERA_SENSOR_INFO, OHOS_SECTION_CAMERA_SENSOR_INFO},
    {OHOS_CAMERA_STATISTICS, OHOS_SECTION_CAMERA_STATISTICS},
    {OHOS_DEVICE_CONTROL, OHOS_SECTION_CAMERA_CONTROL},
    {OHOS_DEVICE_EXPOSURE, OHOS_SECTION_DEVICE_EXPOSURE},
    {OHOS_DEVICE_FOCUS, OHOS_SECTION_DEVICE_FOCUS},
    {OHOS_DEVICE_WHITE_BLANCE, OHOS_SECTION_DEVICE_WHITE},
    {OHOS_DEVICE_FLASH, OHOS_SECTION_DEVICE_FLASH},
    {OHOS_DEVICE_ZOOM, OHOS_SECTION_DEVICE_ZOOM},
    {OHOS_STREAM_ABILITY, OHOS_SECTION_STREAM_ABILITY},
    {OHOS_STREAM_JPEG, OHOS_SECTION_STREAM_JPEG},
    {OHOS_STREAM_VIDEO, OHOS_SECTION_STREAM_VIDEO},
    {OHOS_CAMERA_EFFECT, OHOS_SECTION_CAMERA_EFFECT},
    {OHOS_CAMERA_SECURE, OHOS_SECTION_CAMERA_SECURE},
    {OHOS_XMAGE_COLOR_ABILITY, OHOS_SECTION_CAMERA_XMAGE},
    {OHOS_STREAM_CONTROL, OHOS_SECTION_STREAM_CONTROL},
    {OHOS_LIGHT_STATUS, OHOS_SECTION_LIGHT_STATUS},
};

std::map<uint32_t, uint32_t> g_itemDataTypeMap {
    { OHOS_ABILITY_STREAM_AVAILABLE_BASIC_CONFIGURATIONS, META_TYPE_INT32 },
    { OHOS_ABILITY_STREAM_AVAILABLE_EXTEND_CONFIGURATIONS, META_TYPE_INT32 },
    { OHOS_SENSOR_INFO_MAX_FRAME_DURATION, META_TYPE_INT64 },
    { OHOS_JPEG_MAX_SIZE, META_TYPE_INT32 },
    { OHOS_ABILITY_CAMERA_CONNECTION_TYPE, META_TYPE_BYTE }
};

CameraMetadata::CameraMetadata(size_t itemCapacity, size_t dataCapacity)
{
    metadata_ = AllocateCameraMetadataBuffer(itemCapacity, AlignTo(dataCapacity, DATA_ALIGNMENT));
}

CameraMetadata::~CameraMetadata()
{
    std::lock_guard<std::mutex> lockGuard(g_mtx);
    if (metadata_) {
        FreeCameraMetadataBuffer(metadata_);
        metadata_ = nullptr;
    }
}

bool CameraMetadata::addEntry(uint32_t item, const void *data, size_t data_count)
{
    if (metadata_ == nullptr) {
        METADATA_ERR_LOG("metadata_ is null");
        return false;
    }

    auto result = AddCameraMetadataItem(metadata_, item, data, data_count);
    if (!result) {
        return true;
    }

    if (result != CAM_META_ITEM_CAP_EXCEED && result != CAM_META_DATA_CAP_EXCEED) {
        const char *name = GetCameraMetadataItemName(item);

        if (name) {
            METADATA_ERR_LOG("Failed to add tag. tagname = %{public}s", name);
        } else {
            METADATA_ERR_LOG("Failed to add unknown tag");
        }
        return false;
    }

    return resize_add_metadata(item, data, data_count);
}

bool CameraMetadata::resize_add_metadata(uint32_t item, const void *data, size_t data_count)
{
    uint32_t data_type;

    auto itemCapacity = GetCameraMetadataItemCapacity(metadata_);
    auto data_capacity = GetCameraMetadataDataSize(metadata_);

    int32_t ret = GetCameraMetadataItemType(item, &data_type);
    if (ret != CAM_META_SUCCESS) {
        METADATA_ERR_LOG("GetCameraMetadataItemType invalid item type");
        return false;
    }
    int32_t size = CalculateCameraMetadataItemDataSize(data_type, data_count);
    if (size == CAM_META_FAILURE) {
        METADATA_ERR_LOG("CalculateCameraMetadataItemDataSize invalid datatype:%{public}d", data_type);
        return false;
    }

    common_metadata_header_t *newMetadata = AllocateCameraMetadataBuffer((itemCapacity + 1) * INDEX_COUNTER,
        AlignTo((data_capacity + size) * INDEX_COUNTER, DATA_ALIGNMENT));

    if (newMetadata == nullptr) {
        METADATA_ERR_LOG("Failed to resize the metadata buffer");
        return false;
    }

    auto result = CopyCameraMetadataItems(newMetadata, metadata_);
    if (result != CAM_META_SUCCESS) {
        METADATA_ERR_LOG("Failed to copy the old metadata to new metadata");
        FreeCameraMetadataBuffer(newMetadata);
        newMetadata = nullptr;
        return false;
    }

    result = AddCameraMetadataItem(newMetadata, item, data, data_count);
    if (result != CAM_META_SUCCESS) {
        METADATA_ERR_LOG("Failed to add new entry");
        FreeCameraMetadataBuffer(newMetadata);
        newMetadata = nullptr;
        return false;
    }
    replace_metadata(newMetadata);

    return true;
}

void CameraMetadata::replace_metadata(common_metadata_header_t *newMetadata)
{
    if (metadata_ == newMetadata) {
        return;
    }

    std::lock_guard<std::mutex> lockGuard(g_mtx);
    if (metadata_ != nullptr) {
        FreeCameraMetadataBuffer(metadata_);
    }
    metadata_ = newMetadata;
}

bool CameraMetadata::updateEntry(uint32_t tag, const void *data, size_t dataCount)
{
    if (metadata_ == nullptr) {
        METADATA_ERR_LOG("metadata_ is null");
        return false;
    }

    const char *name = GetCameraMetadataItemName(tag);
    (void)name;
    camera_metadata_item_t item;
    int ret = FindCameraMetadataItem(metadata_, tag, &item);
    if (ret) {
        METADATA_ERR_LOG("Failed to update tag tagname = %{public}s : not present", (name ? name : "<unknown>"));
        return false;
    }
    METADATA_INFO_LOG("updateEntry item id: %{public}d, name: %{public}s, "
                      "dataCount: %{public}zu", tag, name ? name : "<unknown>", dataCount);
    ret = UpdateCameraMetadataItemByIndex(metadata_, item.index, data, dataCount, nullptr);
    if (ret) {
        const char *name_ = GetCameraMetadataItemName(tag);
        (void)name_;
        METADATA_ERR_LOG("Failed to update tag tagname = %{public}s", (name_ ? name_ : "<unknown>"));
        return false;
    }

    return true;
}

common_metadata_header_t *CameraMetadata::get()
{
    return metadata_;
}

const common_metadata_header_t *CameraMetadata::get() const
{
    return metadata_;
}

bool CameraMetadata::isValid() const
{
    return metadata_ != nullptr;
}

uint32_t CameraMetadata::AlignTo(uint32_t val, uint32_t alignment)
{
    return static_cast<uint32_t>((static_cast<uintptr_t>(val) + ((alignment) - 1)) & ~((alignment) - 1));
}

uint32_t CameraMetadata::MaxAlignment(uint32_t dataAlignment, uint32_t metadataAlignment)
{
    return ((dataAlignment > metadataAlignment) ? dataAlignment : metadataAlignment);
}

uint8_t *CameraMetadata::GetMetadataData(const common_metadata_header_t *metadataHeader)
{
    if (!metadataHeader) {
        METADATA_ERR_LOG("GetMetadataData metadataHeader is null");
        return nullptr;
    }
    return (uint8_t *)metadataHeader + metadataHeader->data_start;
}

camera_metadata_item_entry_t *CameraMetadata::GetMetadataItems(const common_metadata_header_t *metadataHeader)
{
    if (metadataHeader == nullptr) {
        METADATA_ERR_LOG("GetMetadataItems metadataHeader is null");
        return nullptr;
    }
    if (metadataHeader->data_start != 0) {
        if (metadataHeader->size != metadataHeader->data_capacity + metadataHeader->data_start) {
            METADATA_ERR_LOG("GetMetadataItems error size:%{public}u, data_capacity:%{public}u, data_start:%{public}u",
                metadataHeader->size, metadataHeader->data_capacity, metadataHeader->data_start);
            return nullptr;
        }
    }
    return reinterpret_cast<camera_metadata_item_entry_t *>(
        (reinterpret_cast<uint8_t *>(const_cast<common_metadata_header_t *>(metadataHeader)) +
        metadataHeader->items_start));
}

common_metadata_header_t *CameraMetadata::FillCameraMetadata(common_metadata_header_t *buffer, size_t memoryRequired,
    uint32_t itemCapacity, uint32_t dataCapacity)
{
    METADATA_DEBUG_LOG("FillCameraMetadata start");
    if (buffer == nullptr) {
        METADATA_ERR_LOG("FillCameraMetadata buffer is null");
        return nullptr;
    }

    common_metadata_header_t *metadataHeader = static_cast<common_metadata_header_t *>(buffer);
    metadataHeader->version = CURRENT_CAMERA_METADATA_VERSION;
    metadataHeader->size = memoryRequired;
    metadataHeader->item_count = 0;
    metadataHeader->item_capacity = itemCapacity;
    metadataHeader->items_start = AlignTo(sizeof(common_metadata_header_t), ITEM_ALIGNMENT);
    metadataHeader->data_count = 0;
    metadataHeader->data_capacity = dataCapacity;
    camera_metadata_item_entry_t *pItem = GetMetadataItems(metadataHeader);
    if (pItem == nullptr) {
        METADATA_ERR_LOG("FillCameraMetadata pItem is null");
        return nullptr;
    }
    size_t dataUnaligned = reinterpret_cast<uint8_t *>(pItem + metadataHeader->item_capacity) -
                           reinterpret_cast<uint8_t *>(metadataHeader);
    metadataHeader->data_start = AlignTo(dataUnaligned, DATA_ALIGNMENT);

    METADATA_DEBUG_LOG("MetadataHeader ItemCapacity Size = %{public}u, DataCapacity Size = %{public}u",
        metadataHeader->item_capacity, metadataHeader->data_capacity);
    METADATA_DEBUG_LOG("FillCameraMetadata end");
    return metadataHeader;
}

size_t CameraMetadata::CalculateCameraMetadataMemoryRequired(uint32_t itemCount, uint32_t dataCount)
{
    METADATA_DEBUG_LOG("CalculateCameraMetadataMemoryRequired start");
    size_t memoryRequired = sizeof(common_metadata_header_t);
    memoryRequired = CameraMetadata::AlignTo(memoryRequired, ITEM_ALIGNMENT);

    memoryRequired += sizeof(camera_metadata_item_entry_t[itemCount]);
    memoryRequired = CameraMetadata::AlignTo(memoryRequired, DATA_ALIGNMENT);

    memoryRequired += sizeof(uint8_t[dataCount]);
    memoryRequired = CameraMetadata::AlignTo(memoryRequired, METADATA_PACKET_ALIGNMENT);

    METADATA_DEBUG_LOG("CalculateCameraMetadataMemoryRequired memoryRequired: %{public}zu", memoryRequired);
    METADATA_DEBUG_LOG("CalculateCameraMetadataMemoryRequired end");
    return memoryRequired;
}

common_metadata_header_t *CameraMetadata::AllocateCameraMetadataBuffer(uint32_t item_capacity, uint32_t data_capacity)
{
    METADATA_DEBUG_LOG("AllocateCameraMetadataBuffer start");
    METADATA_DEBUG_LOG("AllocateCameraMetadataBuffer item_capacity: %{public}u, data_capacity: %{public}u",
        item_capacity, data_capacity);
    if (item_capacity > MAX_ITEM_CAPACITY || data_capacity > MAX_DATA_CAPACITY) {
        METADATA_ERR_LOG("AllocateCameraMetadataBuffer invalid item_capacity or data_capacity");
        return nullptr;
    }
    size_t memoryRequired = CalculateCameraMetadataMemoryRequired(item_capacity, data_capacity);
    void *buffer = calloc(1, memoryRequired);
    if (buffer == nullptr) {
        METADATA_ERR_LOG("AllocateCameraMetadataBuffer memory allocation failed");
        return reinterpret_cast<common_metadata_header_t *>(buffer);
    }

    common_metadata_header_t *metadataHeader = FillCameraMetadata(reinterpret_cast<common_metadata_header_t *>(buffer),
        memoryRequired, item_capacity, data_capacity);
    if (metadataHeader == nullptr) {
        METADATA_ERR_LOG("AllocateCameraMetadataBuffer metadataHeader is null");
        free(buffer);
    }

    METADATA_DEBUG_LOG("AllocateCameraMetadataBuffer end");
    return metadataHeader;
}

// Load vendor tag impl
int32_t LoadVendorTagImpl()
{
    std::lock_guard<std::mutex> lock(g_vendorTagImplMtx);
    if (g_vendorTagImpl == nullptr) {
        void* libHandle = nullptr;
#ifndef CAMERA_VENDOR_TAG
        libHandle = dlopen(g_exampleVendorTagLib, RTLD_LAZY);
#else
        libHandle = dlopen(g_vendorTagLib, RTLD_LAZY);
#endif
        if (libHandle == nullptr) {
            METADATA_ERR_LOG("dlopen failed %{public}s", __func__);
            return CAM_META_FAILURE;
        }

        CreateCameraVendorTag* createVendorTag =
            reinterpret_cast<CreateCameraVendorTag*>(dlsym(libHandle, "CreateVendorTagImpl"));
        if (createVendorTag == nullptr) {
            METADATA_ERR_LOG("CreateCameraVendorTag failed %{public}s", __func__);
            dlclose(libHandle);
            return CAM_META_FAILURE;
        }

        g_vendorTagImpl = createVendorTag();
        if (g_vendorTagImpl == nullptr) {
            METADATA_ERR_LOG("createVendorTag failed %{public}s", __func__);
            dlclose(libHandle);
            return CAM_META_FAILURE;
        }
    }

    return CAM_META_SUCCESS;
}

int32_t CameraMetadata::GetMetadataSection(uint32_t itemSection, uint32_t *section)
{
    METADATA_DEBUG_LOG("GetMetadataSection start");
    if (itemSection < OHOS_CAMERA_PROPERTIES ||
        itemSection >= OHOS_ABILITY_SECTION_END) {
        METADATA_ERR_LOG("GetMetadataSection itemSection is not valid");
        return CAM_META_FAILURE;
    }

    if (section == nullptr) {
        METADATA_ERR_LOG("GetMetadataSection section is nullptr");
        return CAM_META_FAILURE;
    }

    int32_t ret = CAM_META_SUCCESS;
    auto iter = g_metadataSectionMap.find(itemSection);
    if (iter != g_metadataSectionMap.end()) {
        *section = iter->second;
    } else {
        METADATA_ERR_LOG("GetMetadataSection item section is not defined");
        ret = CAM_META_FAILURE;
    }

    METADATA_DEBUG_LOG("GetMetadataSection end");
    return ret;
}

int32_t CameraMetadata::GetCameraMetadataItemType(uint32_t item, uint32_t *dataType)
{
    METADATA_DEBUG_LOG("GetCameraMetadataItemType start");
    if (dataType == nullptr) {
        METADATA_ERR_LOG("GetCameraMetadataItemType dataType is null");
        return CAM_META_INVALID_PARAM;
    }
    uint32_t section;
    uint32_t itemTag = item >> BITWISE_SHIFT_16;
    if (itemTag >= OHOS_VENDOR_SECTION) {
        int32_t ret = LoadVendorTagImpl();
        if (ret != CAM_META_SUCCESS) {
            METADATA_ERR_LOG("LoadVendorTagImpl failed");
            return CAM_META_FAILURE;
        }
        std::lock_guard<std::mutex> lock(g_vendorTagImplMtx);
        *dataType = g_vendorTagImpl->GetVendorTagType(item);
        return CAM_META_SUCCESS;
    }
    int32_t ret = GetMetadataSection(itemTag, &section);
    if (ret != CAM_META_SUCCESS) {
        METADATA_ERR_LOG("GetCameraMetadataItemType section is not valid");
        return ret;
    }

    if (item >= g_ohosCameraSectionBounds[section][1]) {
        METADATA_ERR_LOG("GetCameraMetadataItemType item is not in section bound");
        return CAM_META_FAILURE;
    }

    uint32_t itemIndex = item & 0xFFFF;
    if (g_ohosItemInfo[section][itemIndex].item_type < META_TYPE_BYTE) {
        METADATA_ERR_LOG("GetCameraMetadataItemType item is not initialized");
        return CAM_META_FAILURE;
    }

    *dataType = g_ohosItemInfo[section][itemIndex].item_type;

    METADATA_DEBUG_LOG("GetCameraMetadataItemType end");
    return CAM_META_SUCCESS;
}

const char *CameraMetadata::GetCameraMetadataItemName(uint32_t item)
{
    METADATA_DEBUG_LOG("GetCameraMetadataItemName start");
    METADATA_DEBUG_LOG("GetCameraMetadataItemName item: %{public}u", item);
    uint32_t section;
    uint32_t itemTag = item >> BITWISE_SHIFT_16;
    if (itemTag >= OHOS_VENDOR_SECTION) {
        int32_t ret = LoadVendorTagImpl();
        if (ret != CAM_META_SUCCESS) {
            METADATA_ERR_LOG("LoadVendorTagImpl failed");
            return nullptr;
        }
        std::lock_guard<std::mutex> lock(g_vendorTagImplMtx);
        const char* tagName = g_vendorTagImpl->GetVendorTagName(item);
        return tagName;
    }
    int32_t ret = GetMetadataSection(itemTag, &section);
    if (ret != CAM_META_SUCCESS) {
        METADATA_ERR_LOG("GetCameraMetadataItemName section is not valid");
        return nullptr;
    }

    if (item >= g_ohosCameraSectionBounds[section][1]) {
        METADATA_ERR_LOG("GetCameraMetadataItemName item is not in section bound");
        return nullptr;
    }

    uint32_t itemIndex = item & 0xFFFF;
    METADATA_DEBUG_LOG("GetCameraMetadataItemName end");
    return g_ohosItemInfo[section][itemIndex].item_name;
}

int32_t CameraMetadata::CalculateCameraMetadataItemDataSize(uint32_t type, size_t dataCount)
{
    METADATA_DEBUG_LOG("CalculateCameraMetadataItemDataSize start");
    if (type < META_TYPE_BYTE || type >= META_NUM_TYPES) {
        METADATA_ERR_LOG("CalculateCameraMetadataItemDataSize invalid type");
        return CAM_META_FAILURE;
    }

    size_t dataBytes = dataCount * OHOS_CAMERA_METADATA_TYPE_SIZE[type];

    METADATA_DEBUG_LOG("CalculateCameraMetadataItemDataSize end");
    return (dataBytes <= METADATA_HEADER_DATA_SIZE) ? 0 : AlignTo(dataBytes, DATA_ALIGNMENT);
}

int CameraMetadata::AddCameraMetadataItemVerify(common_metadata_header_t *dst,
    uint32_t item, const void *data, size_t dataCount, uint32_t *dataType)
{
    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        name = "<unknown>";
    }
    METADATA_DEBUG_LOG("AddCameraMetadataItemVerify item id: %{public}u, name: %{public}s, "
        "dataCount: %{public}zu", item, name, dataCount);

    if (dst == nullptr) {
        METADATA_ERR_LOG("AddCameraMetadataItemVerify common_metadata_header_t is null");
        return CAM_META_INVALID_PARAM;
    }

    if (!dataCount || data == nullptr) {
        METADATA_ERR_LOG("AddCameraMetadataItemVerify data is not valid. item: %{public}u, "
            "dataCount: %{public}zu", item, dataCount);
        return CAM_META_INVALID_PARAM;
    }

    if (dst->item_count == dst->item_capacity) {
        METADATA_ERR_LOG("AddCameraMetadataItemVerify item_capacity limit reached. "
            "item_count: %{public}d, item_capacity: %{public}d", dst->item_count, dst->item_capacity);
        return CAM_META_ITEM_CAP_EXCEED;
    }

    int32_t ret = GetCameraMetadataItemType(item, dataType);
    if (ret != CAM_META_SUCCESS) {
        METADATA_ERR_LOG("AddCameraMetadataItemVerify invalid item type");
        return CAM_META_INVALID_PARAM;
    }

    if (*dataType < META_TYPE_BYTE || *dataType >= META_NUM_TYPES) {
        METADATA_ERR_LOG("AddCameraMetadataItemVerify invalid type");
        return CAM_META_INVALID_PARAM;
    }

    return CAM_META_SUCCESS;
}

int CameraMetadata::AddCameraMetadataItem(common_metadata_header_t *dst, uint32_t item,
    const void *data, size_t dataCount)
{
    METADATA_DEBUG_LOG("AddCameraMetadataItem start");
    uint32_t dataType;
    int32_t ret = AddCameraMetadataItemVerify(dst, item, data, dataCount, &dataType);
    if (ret != CAM_META_SUCCESS) return ret;

    if ((dst->size - dst->items_start) < (uint64_t)dst->item_count * itemLen ||
        (dst->size - dst->data_start) < dst->data_count) {
        METADATA_ERR_LOG("AddCameraMetadataItem fail: invalid argument.");
        return CAM_META_INVALID_PARAM;
    }
    
    int32_t dataBytes = CalculateCameraMetadataItemDataSize(dataType, dataCount);
    if (dataBytes + dst->data_count > dst->data_capacity) {
        METADATA_ERR_LOG("AddCameraMetadataItem data_capacity limit reached");
        return CAM_META_DATA_CAP_EXCEED;
    }

    size_t dataPayloadBytes = dataCount * OHOS_CAMERA_METADATA_TYPE_SIZE[dataType];
    camera_metadata_item_entry_t *pItem = GetMetadataItems(dst);
    if (pItem == nullptr) {
        METADATA_ERR_LOG("AddCameraMetadataItem pItem is null");
        return CAM_META_INVALID_PARAM;
    }
    camera_metadata_item_entry_t *metadataItem = pItem + dst->item_count;
    ret = memset_s(metadataItem, sizeof(camera_metadata_item_entry_t), 0, sizeof(camera_metadata_item_entry_t));
    if (ret != EOK) {
        METADATA_ERR_LOG("AddCameraMetadataItem: memset_s failed");
        return CAM_META_FAILURE;
    }
    metadataItem->item = item;
    metadataItem->data_type = dataType;
    metadataItem->count = dataCount;

    if (dataBytes == 0) {
        ret = memcpy_s(metadataItem->data.value, METADATA_HEADER_DATA_SIZE, data, dataPayloadBytes);
        if (ret != EOK) {
            METADATA_ERR_LOG("AddCameraMetadataItem memory copy failed");
            return CAM_META_FAILURE;
        }
    } else {
        metadataItem->data.offset = dst->data_count;
        uint8_t *dstMetadataData = GetMetadataData(dst);
        if (dstMetadataData == nullptr) {
            METADATA_ERR_LOG("AddCameraMetadataItem GetMetadataData failed");
            return CAM_META_FAILURE;
        }
        ret = memcpy_s(dstMetadataData + metadataItem->data.offset, dst->data_capacity - dst->data_count, data,
            dataPayloadBytes);
        if (ret != EOK) {
            METADATA_ERR_LOG("AddCameraMetadataItem memory copy failed");
            return CAM_META_FAILURE;
        }
        dst->data_count += dataBytes;
    }
    dst->item_count++;

    METADATA_DEBUG_LOG("AddCameraMetadataItem end");
    return CAM_META_SUCCESS;
}

int CameraMetadata::GetCameraMetadataItem(const common_metadata_header_t *src, uint32_t index,
    camera_metadata_item_t *item)
{
    METADATA_DEBUG_LOG("GetCameraMetadataItem start");
    if (src == nullptr || item == nullptr) {
        METADATA_ERR_LOG("GetCameraMetadataItem src or item is null");
        return CAM_META_INVALID_PARAM;
    }

    int32_t ret = memset_s(item, sizeof(camera_metadata_item_t), 0, sizeof(camera_metadata_item_t));
    if (ret != EOK) {
        METADATA_ERR_LOG("GetCameraMetadataItem: memset_s failed");
        return CAM_META_FAILURE;
    }
    if (index >= src->item_count || index >= src->item_capacity) {
        METADATA_ERR_LOG("GetCameraMetadataItem index is greater than item count");
        return CAM_META_INVALID_PARAM;
    }
    camera_metadata_item_entry_t *pItem = GetMetadataItems(src);
    if (pItem == nullptr) {
        METADATA_ERR_LOG("GetCameraMetadataItem pItem is null");
        return CAM_META_INVALID_PARAM;
    }
    camera_metadata_item_entry_t *localItem = pItem + index;

    item->index = index;
    item->item = localItem->item;
    item->data_type = localItem->data_type;
    item->count = localItem->count;

    int32_t dataBytes = CalculateCameraMetadataItemDataSize(localItem->data_type, localItem->count);
    if (dataBytes == 0) {
        item->data.u8 = localItem->data.value;
    } else if (dataBytes == CAM_META_FAILURE) {
        METADATA_ERR_LOG("GetCameraMetadataItem invalid datatype:%{public}d", localItem->data_type);
        return CAM_META_FAILURE;
    } else {
        uint8_t *srcMetadataData = GetMetadataData(src);
        if (srcMetadataData == nullptr) {
            METADATA_ERR_LOG("GetCameraMetadataItem GetMetadataData failed");
            return CAM_META_FAILURE;
        }
        if (localItem->data.offset > src->data_count) {
            METADATA_ERR_LOG("GetCameraMetadataItem offset is greater than data count,"
                " offset:%{public}u, data count:%{public}u", localItem->data.offset, src->data_count);
            return CAM_META_FAILURE;
        }
        item->data.u8 = srcMetadataData + localItem->data.offset;
    }

    METADATA_DEBUG_LOG("GetCameraMetadataItem end");
    return CAM_META_SUCCESS;
}

int CameraMetadata::FindCameraMetadataItemIndex(const common_metadata_header_t *src, uint32_t item, uint32_t *idx,
    bool isPrintLog)
{
    METADATA_DEBUG_LOG("FindCameraMetadataItemIndex start");
    METADATA_DEBUG_LOG("FindCameraMetadataItemIndex item: %{public}u", item);
    if (src == nullptr || idx == nullptr) {
        METADATA_ERR_LOG("FindCameraMetadataItemIndex src or index is null");
        return CAM_META_INVALID_PARAM;
    }

    camera_metadata_item_entry_t *searchItem = GetMetadataItems(src);
    if (searchItem == nullptr) {
        METADATA_ERR_LOG("FindCameraMetadataItemIndex item is null");
        return CAM_META_INVALID_PARAM;
    }
    uint32_t index;
    for (index = 0; index < src->item_count; index++, searchItem++) {
        if (searchItem->item == item) {
            break;
        }
    }

    if (index == src->item_count) {
        if (isPrintLog) {
            METADATA_ERR_LOG("FindCameraMetadataItemIndex item: %{public}u not found", item);
        }
        return CAM_META_ITEM_NOT_FOUND;
    }

    if (!CheckItemDataType(*searchItem)) {
        return CAM_META_INVALID_PARAM;
    }

    *idx = index;
    METADATA_DEBUG_LOG("FindCameraMetadataItemIndex index: %{public}u", index);
    METADATA_DEBUG_LOG("FindCameraMetadataItemIndex end");
    return CAM_META_SUCCESS;
}

int CameraMetadata::FindCameraMetadataItem(const common_metadata_header_t *src, uint32_t item,
    camera_metadata_item_t *metadataItem)
{
    uint32_t index = 0;
    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        name = "<unknown>";
    }
    METADATA_DEBUG_LOG("FindCameraMetadataItem item id: %{public}u, name: %{public}s", item, name);
    int ret = FindCameraMetadataItemIndex(src, item, &index);
    if (ret != CAM_META_SUCCESS) {
        return ret;
    }

    return GetCameraMetadataItem(src, index, metadataItem);
}

bool CameraMetadata::IsCameraMetadataItemExist(const common_metadata_header_t *src, uint32_t item)
{
    uint32_t index = 0;
    int ret = FindCameraMetadataItemIndex(src, item, &index, false);
    return ret == CAM_META_SUCCESS;
}

void SetOffset(camera_metadata_item_entry_t *metadataItems, camera_metadata_item_entry_t *item, size_t oldItemSize)
{
    if (metadataItems == nullptr) {
        METADATA_ERR_LOG("SetOffset metadataItems is null");
        return;
    }
    if (CalculateCameraMetadataItemDataSize(metadataItems->data_type, metadataItems->count) > 0 &&
        metadataItems->data.offset > item->data.offset) {
        metadataItems->data.offset -= oldItemSize;
    }
}

int CameraMetadata::MetadataExpandItemMem(common_metadata_header_t *dst, camera_metadata_item_entry_t *item,
    size_t oldItemSize)
{
    if (item == nullptr || dst == nullptr) {
        METADATA_ERR_LOG("MetadataExpandItemMem item is null or dst is null");
        return CAM_META_INVALID_PARAM;
    }
    uint8_t *dstMetadataData = GetMetadataData(dst);
    if (dstMetadataData == nullptr) {
        METADATA_ERR_LOG("MetadataExpandItemMem GetMetadataData failed");
        return CAM_META_FAILURE;
    }
    if (item->data.offset > UINT32_MAX - (uint32_t)oldItemSize ||
        item->data.offset + (uint32_t)oldItemSize > dst->data_count) {
        METADATA_ERR_LOG("MetadataExpandItemMem data.offset:%{public}u, oldItemSize:%{public}zu, "
            "dst->data_count:%{public}u", item->data.offset, oldItemSize, dst->data_count);
        return CAM_META_FAILURE;
    }
    uint8_t *start = dstMetadataData + item->data.offset;
    uint8_t *end = start + oldItemSize;
    size_t length = dst->data_count - item->data.offset - oldItemSize;
    if (length != 0) {
        int32_t ret = memmove_s(start, length, end, length);
        if (ret != EOK) {
            METADATA_ERR_LOG("MetadataExpandItemMem memory move failed");
            return CAM_META_FAILURE;
        }
    }
    dst->data_count -= oldItemSize;

    camera_metadata_item_entry_t *metadataItems = GetMetadataItems(dst);
    for (uint32_t i = 0; i < dst->item_count; i++, ++metadataItems) {
        SetOffset(metadataItems, item, oldItemSize);
    }

    return CAM_META_SUCCESS;
}

int CameraMetadata::copyMetadataMemory(common_metadata_header_t *dst, camera_metadata_item_entry_t *item,
    size_t dataPayloadSize, const void *data)
{
    uint8_t *dstMetadataData = GetMetadataData(dst);
    int32_t ret = CAM_META_SUCCESS;
    if (dstMetadataData == nullptr) {
        METADATA_ERR_LOG("copyMetadataMemory GetMetadataData failed");
        return CAM_META_FAILURE;
    }
    if (dst->size < item->data.offset) {
        METADATA_ERR_LOG("copyMetadataMemory offset is greater than total size,"
            " offset:%{public}u, total size:%{public}u", item->data.offset, dst->size);
        return CAM_META_FAILURE;
    }
    ret = memcpy_s(dstMetadataData + item->data.offset, dataPayloadSize, data, dataPayloadSize);
    if (ret != EOK) {
        METADATA_ERR_LOG("copyMetadataMemory memory copy failed");
        return CAM_META_FAILURE;
    }
    return CAM_META_SUCCESS;
}

int CameraMetadata::UpdateCameraMetadataItemSize(camera_metadata_item_entry_t *item, uint32_t dataCount,
    common_metadata_header_t *dst, const void *data)
{
    if (item == nullptr) {
        METADATA_ERR_LOG("UpdateCameraMetadataItemSize item is null");
        return CAM_META_FAILURE;
    } else if (item->data_type < META_TYPE_BYTE || item->data_type >= META_NUM_TYPES) {
        METADATA_ERR_LOG("UpdateCameraMetadataItemSize invalid datatype:%{public}d", item->data_type);
        return CAM_META_FAILURE;
    }
    int32_t dataSize = CalculateCameraMetadataItemDataSize(item->data_type, dataCount);
    size_t dataPayloadSize = dataCount * OHOS_CAMERA_METADATA_TYPE_SIZE[item->data_type];
    int32_t oldItemSize = CalculateCameraMetadataItemDataSize(item->data_type, item->count);
    int32_t ret = CAM_META_SUCCESS;
    if (item == nullptr || dst == nullptr) {
        METADATA_ERR_LOG("UpdateCameraMetadataItemSize item is null or dst is null");
        return CAM_META_FAILURE;
    }
    if (dataSize != oldItemSize) {
        if (dst->data_capacity < (dst->data_count + dataSize - oldItemSize)) {
            METADATA_ERR_LOG("UpdateCameraMetadataItemByIndex data_capacity limit reached");
            return CAM_META_DATA_CAP_EXCEED;
        }
        if (oldItemSize != 0) {
            ret = MetadataExpandItemMem(dst, item, oldItemSize);
            if (ret != CAM_META_SUCCESS) {
                return ret;
            }
        }
        if (dataSize != 0) {
            item->data.offset = dst->data_count;
            ret = copyMetadataMemory(dst, item, dataPayloadSize, data);
            if (ret != CAM_META_SUCCESS) {
                return ret;
            }
            dst->data_count += dataSize;
        }
    } else if (dataSize != 0) {
        ret = copyMetadataMemory(dst, item, dataPayloadSize, data);
        if (ret != CAM_META_SUCCESS) {
            return ret;
        }
    }
    if (dataSize == 0) {
        ret = memcpy_s(item->data.value, dataPayloadSize, data, dataPayloadSize);
        if (ret != EOK) {
            METADATA_ERR_LOG("UpdateCameraMetadataItemByIndex memory copy failed");
            return CAM_META_FAILURE;
        }
    }
    return ret;
}

int CameraMetadata::UpdateCameraMetadataItemByIndex(common_metadata_header_t *dst, uint32_t index,
    const void *data, uint32_t dataCount, camera_metadata_item_t *updatedItem)
{
    METADATA_DEBUG_LOG("UpdateCameraMetadataItemByIndex start");
    if ((dst == nullptr) || (index >= dst->item_count)) {
        METADATA_ERR_LOG("UpdateCameraMetadataItemByIndex dst is null or invalid index");
        return CAM_META_INVALID_PARAM;
    }

    if (!dataCount || data == nullptr) {
        METADATA_ERR_LOG("UpdateCameraMetadataItemByIndex data is not valid. "
            "dataCount: %{public}u", dataCount);
        return CAM_META_INVALID_PARAM;
    }

    int32_t ret = CAM_META_SUCCESS;
    camera_metadata_item_entry_t *metadataItem = GetMetadataItems(dst);
    if (metadataItem == nullptr) {
        METADATA_ERR_LOG("UpdateCameraMetadataItemByIndex item is null");
        return CAM_META_INVALID_PARAM;
    }
    camera_metadata_item_entry_t *item = metadataItem + index;

    ret = UpdateCameraMetadataItemSize(item, dataCount, dst, data);
    if (ret != CAM_META_SUCCESS) {
        return ret;
    }

    item->count = dataCount;
    if (updatedItem != nullptr) {
        ret = GetCameraMetadataItem(dst, index, updatedItem);
        if (ret != CAM_META_SUCCESS) {
            return ret;
        }
    }

    METADATA_DEBUG_LOG("UpdateCameraMetadataItemByIndex end");
    return ret;
}

int CameraMetadata::UpdateCameraMetadataItem(common_metadata_header_t *dst, uint32_t item, const void *data,
    uint32_t dataCount, camera_metadata_item_t *updatedItem)
{
    METADATA_DEBUG_LOG("UpdateCameraMetadataItem item id: %{public}u, dataCount: %{public}u", item, dataCount);
    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        name = "<unknown>";
    }
    METADATA_DEBUG_LOG("UpdateCameraMetadataItem item id: %{public}u, name: %{public}s, "
        "dataCount: %{public}u", item, name, dataCount);
    if (!dataCount || data == nullptr) {
        METADATA_ERR_LOG("UpdateCameraMetadataItem data is not valid. item: %{public}u, "
            "dataCount: %{public}u", item, dataCount);
        return CAM_META_INVALID_PARAM;
    }

    uint32_t index = 0;
    int32_t ret = FindCameraMetadataItemIndex(dst, item, &index);
    if (ret != CAM_META_SUCCESS) {
        return ret;
    }

    return UpdateCameraMetadataItemByIndex(dst, index, data, dataCount, updatedItem);
}

int CameraMetadata::moveMetadataMemery(common_metadata_header_t *dst, camera_metadata_item_entry_t *itemToDelete,
    size_t dataBytes)
{
    if (itemToDelete == nullptr) {
        METADATA_ERR_LOG("UpdateCameraMetadataItemSize itemToDelete is null");
        return CAM_META_INVALID_PARAM;
    }
    uint8_t *dstMetadataData = GetMetadataData(dst);
    if (dstMetadataData == nullptr) {
        METADATA_ERR_LOG("UpdateCameraMetadataItemSize GetMetadataData failed");
        return CAM_META_FAILURE;
    }
    int32_t ret = CAM_META_SUCCESS;
    uint8_t *start = dstMetadataData + itemToDelete->data.offset;
    uint8_t *end = start + dataBytes;
    size_t length = dst->data_count - itemToDelete->data.offset - dataBytes;
    if (length != 0) {
        ret = memmove_s(start, length, end, length);
        if (ret != EOK) {
            METADATA_ERR_LOG("DeleteCameraMetadataItemByIndex memory move failed");
            return CAM_META_FAILURE;
        }
    }
    return CAM_META_SUCCESS;
}

int CameraMetadata::DeleteCameraMetadataItemByIndex(common_metadata_header_t *dst, uint32_t index)
{
    METADATA_DEBUG_LOG("DeleteCameraMetadataItemByIndex start");
    if (dst == nullptr) {
        METADATA_ERR_LOG("DeleteCameraMetadataItemByIndex dst is null");
        return CAM_META_INVALID_PARAM;
    }

    if (index >= dst->item_count) {
        METADATA_ERR_LOG("DeleteCameraMetadataItemByIndex item not valid");
        return CAM_META_INVALID_PARAM;
    }

    int32_t ret = CAM_META_SUCCESS;
    camera_metadata_item_entry_t *pItem = GetMetadataItems(dst);
    if (pItem == nullptr) {
        METADATA_ERR_LOG("DeleteCameraMetadataItemByIndex pItem is null");
        return CAM_META_INVALID_PARAM;
    }
    camera_metadata_item_entry_t *itemToDelete = pItem + index;
    int32_t dataBytes = CalculateCameraMetadataItemDataSize(itemToDelete->data_type, itemToDelete->count);
    if (dataBytes > 0) {
        ret = moveMetadataMemery(dst, itemToDelete, dataBytes);
        if (ret != CAM_META_SUCCESS) {
            return ret;
        }
        dst->data_count -= dataBytes;

        camera_metadata_item_entry_t *metadataItems = GetMetadataItems(dst);
        for (uint32_t i = 0; i < dst->item_count; i++, ++metadataItems) {
            if (CalculateCameraMetadataItemDataSize(
                metadataItems->data_type, metadataItems->count) > 0 &&
                metadataItems->data.offset > itemToDelete->data.offset) {
                metadataItems->data.offset -= dataBytes;
            }
        }
    } else if (dataBytes == CAM_META_FAILURE) {
        METADATA_ERR_LOG("DeleteCameraMetadataItemByIndex invalid datatype:%{public}d", itemToDelete->data_type);
        return CAM_META_FAILURE;
    }

    uint64_t length = sizeof(camera_metadata_item_entry_t) * (dst->item_count - index - 1);
    if (length != 0) {
        ret = memmove_s(itemToDelete, length, itemToDelete + 1, length);
        if (ret != EOK) {
            METADATA_ERR_LOG("DeleteCameraMetadataItemByIndex memory move failed");
            return CAM_META_FAILURE;
        }
    }
    dst->item_count -= 1;
    METADATA_DEBUG_LOG("DeleteCameraMetadataItemByIndex end");
    return ret;
}

int CameraMetadata::DeleteCameraMetadataItem(common_metadata_header_t *dst, uint32_t item)
{
    METADATA_DEBUG_LOG("DeleteCameraMetadataItem item: %{public}u", item);
    uint32_t index = 0;
    int32_t ret = FindCameraMetadataItemIndex(dst, item, &index);
    if (ret != CAM_META_SUCCESS) {
        return ret;
    }

    return DeleteCameraMetadataItemByIndex(dst, index);
}

void CameraMetadata::FreeCameraMetadataBuffer(common_metadata_header_t *dst)
{
    if (dst != nullptr) {
        free(dst);
    }
}

uint32_t CameraMetadata::GetCameraMetadataItemCount(const common_metadata_header_t *metadataHeader)
{
    if (!metadataHeader) {
        METADATA_ERR_LOG("GetCameraMetadataItemCount::metadataHeader is null");
        return 0;
    }
    return metadataHeader->item_count;
}

uint32_t CameraMetadata::GetCameraMetadataItemCapacity(const common_metadata_header_t *metadataHeader)
{
    if (!metadataHeader) {
        METADATA_ERR_LOG("GetCameraMetadataItemCapacity::metadataHeader is null");
        return 0;
    }
    return metadataHeader->item_capacity;
}

uint32_t CameraMetadata::GetCameraMetadataDataSize(const common_metadata_header_t *metadataHeader)
{
    if (metadataHeader == nullptr) {
        METADATA_ERR_LOG("GetCameraMetadataDataSize::metadataHeader is null");
        return 0;
    }
    return metadataHeader->data_capacity;
}

int32_t CameraMetadata::CopyCameraMetadataItems(common_metadata_header_t *newMetadata,
    const common_metadata_header_t *oldMetadata)
{
    if (newMetadata == nullptr || oldMetadata == nullptr) {
        return CAM_META_INVALID_PARAM;
    }

    int32_t ret;
    if (oldMetadata->item_count != 0) {
        ret = memcpy_s(GetMetadataItems(newMetadata), sizeof(camera_metadata_item_entry_t[newMetadata->item_capacity]),
            GetMetadataItems(oldMetadata), sizeof(camera_metadata_item_entry_t[oldMetadata->item_count]));
        if (ret != EOK) {
            METADATA_ERR_LOG("CopyCameraMetadataItems memory copy failed, ItemCapacity Size = %{public}u,"
                "ItemCount Size = %{public}u", newMetadata->item_capacity, oldMetadata->item_count);
            return CAM_META_FAILURE;
        }
    }

    if (oldMetadata->data_count != 0) {
        uint8_t *newMetadataData = GetMetadataData(newMetadata);
        uint8_t *oldMetadataData = GetMetadataData(oldMetadata);
        if (newMetadataData == nullptr || oldMetadataData == nullptr) {
            METADATA_ERR_LOG("UpdateCameraMetadataItemSize GetMetadataData failed");
            return CAM_META_FAILURE;
        }
        ret = memcpy_s(newMetadataData, sizeof(uint8_t[newMetadata->data_capacity]), oldMetadataData,
            sizeof(uint8_t[oldMetadata->data_count]));
        if (ret != EOK) {
            METADATA_ERR_LOG("CopyCameraMetadataItems memory copy failed, DataCapacity Size = %{public}u,"
                "DataCount Size = %{public}u", newMetadata->data_capacity, oldMetadata->data_count);
            return CAM_META_FAILURE;
        }
    }

    newMetadata->item_count = oldMetadata->item_count;
    newMetadata->data_count = oldMetadata->data_count;

    return CAM_META_SUCCESS;
}

std::string U8ItemToString(int32_t item, const camera_metadata_item_t entry)
{
    std::string st = {};
    uint32_t count = entry.count;
    if (entry.data.u8 == nullptr) {
        METADATA_ERR_LOG("U8ItemToString: entry.data.u8 is null");
        return st;
    }
    std::string dataStr = std::to_string(*(entry.data.u8));
    for (uint32_t i = 1; i < count; i++) {
        if ((i % WRAP_LENGTH) == 0) {
            dataStr += "]\n\t[" + std::to_string(*(entry.data.u8 + i));
        } else {
            dataStr += " " + std::to_string(*(entry.data.u8 + i));
        }
    }

    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        METADATA_ERR_LOG("U8ItemToString: get u8 item name fail");
        return st;
    }
    std::string nameStr(name);

    st = nameStr + " (" + std::to_string(entry.index) + "): " +
        OHOS_CAMERA_METADATA_TYPE[entry.data_type] + "[" + std::to_string(count) + "]" + "\n\t[" + dataStr + "]";

    return st;
}

std::string I32ItemToString(int32_t item, const camera_metadata_item_t entry)
{
    std::string st = {};
    uint32_t count = entry.count;
    if (entry.data.i32 == nullptr) {
        METADATA_ERR_LOG("I32ItemToString: entry.data.i32 is null");
        return st;
    }
    std::string dataStr = std::to_string(*(entry.data.i32));
    for (uint32_t i = 1; i < count; i++) {
        if ((i % WRAP_LENGTH) == 0) {
            dataStr += "]\n\t[" + std::to_string(*(entry.data.i32 + i));
        } else {
            dataStr += " " + std::to_string(*(entry.data.i32 + i));
        }
    }

    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        METADATA_ERR_LOG("I32ItemToString: get i32 item name fail");
        return st;
    }
    std::string nameStr(name);

    st = nameStr + " (" + std::to_string(entry.index) + "): " +
        OHOS_CAMERA_METADATA_TYPE[entry.data_type] + "[" + std::to_string(count) + "]" + "\n\t[" + dataStr + "]";

    return st;
}

std::string U32ItemToString(int32_t item, const camera_metadata_item_t entry)
{
    std::string st = {};
    uint32_t count = entry.count;
    if (entry.data.ui32 == nullptr) {
        METADATA_ERR_LOG("U32ItemToString: entry.data.ui32 is null");
        return st;
    }
    std::string dataStr = std::to_string(*(entry.data.ui32));
    for (uint32_t i = 1; i < count; i++) {
        if ((i % WRAP_LENGTH) == 0) {
            dataStr += "]\n\t[" + std::to_string(*(entry.data.ui32 + i));
        } else {
            dataStr += " " + std::to_string(*(entry.data.ui32 + i));
        }
    }

    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        METADATA_ERR_LOG("U32ItemToString: get u32 item name fail");
        return st;
    }
    std::string nameStr(name);

    st = nameStr + " (" + std::to_string(entry.index) + "): " +
        OHOS_CAMERA_METADATA_TYPE[entry.data_type] + "[" + std::to_string(count) + "]" + "\n\t[" + dataStr + "]";

    return st;
}

std::string I64ItemToString(int32_t item, const camera_metadata_item_t entry)
{
    std::string st = {};
    uint32_t count = entry.count;
    if (entry.data.i64 == nullptr) {
        METADATA_ERR_LOG("I64ItemToString: entry.data.i64 is null");
        return st;
    }
    std::string dataStr = std::to_string(*(entry.data.i64));
    for (uint32_t i = 1; i < count; i++) {
        if ((i % WRAP_LENGTH) == 0) {
            dataStr += "]\n\t[" + std::to_string(*(entry.data.i64 + i));
        } else {
            dataStr += " " + std::to_string(*(entry.data.i64 + i));
        }
    }

    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        METADATA_ERR_LOG("I64ItemToString: get i64 item name fail");
        return st;
    }
    std::string nameStr(name);

    st = nameStr + " (" + std::to_string(entry.index) + "): " +
        OHOS_CAMERA_METADATA_TYPE[entry.data_type] + "[" + std::to_string(count) + "]" + "\n\t[" + dataStr + "]";

    return st;
}

std::string FloatItemToString(int32_t item, const camera_metadata_item_t entry)
{
    std::string st = {};
    uint32_t count = entry.count;
    if (entry.data.f == nullptr) {
        METADATA_ERR_LOG("FloatItemToString: entry.data.f is null");
        return st;
    }
    std::string dataStr = std::to_string(*(entry.data.f));
    for (uint32_t i = 1; i < count; i++) {
        if ((i % WRAP_LENGTH) == 0) {
            dataStr += "]\n\t[" + std::to_string(*(entry.data.f + i));
        } else {
            dataStr += " " + std::to_string(*(entry.data.f + i));
        }
    }

    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        METADATA_ERR_LOG("FloatItemToString: get float item name fail");
        return st;
    }
    std::string nameStr(name);

    st = nameStr + " (" + std::to_string(entry.index) + "): " +
        OHOS_CAMERA_METADATA_TYPE[entry.data_type] + "[" + std::to_string(count) + "]" + "\n\t[" + dataStr + "]";

    return st;
}

std::string DoubleItemToString(int32_t item, const camera_metadata_item_t entry)
{
    std::string st = {};
    uint32_t count = entry.count;
    if (entry.data.d == nullptr) {
        METADATA_ERR_LOG("DoubleItemToString: entry.data.d is null");
        return st;
    }
    std::string dataStr = std::to_string(*(entry.data.d));
    for (uint32_t i = 1; i < count; i++) {
        if ((i % WRAP_LENGTH) == 0) {
            dataStr += "]\n\t[" + std::to_string(*(entry.data.d + i));
        } else {
            dataStr += " " + std::to_string(*(entry.data.d + i));
        }
    }

    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        METADATA_ERR_LOG("DoubleItemToString: get double item name fail");
        return st;
    }
    std::string nameStr(name);

    st = nameStr + " (" + std::to_string(entry.index) + "): " +
        OHOS_CAMERA_METADATA_TYPE[entry.data_type] + "[" + std::to_string(count) + "]" + "\n\t[" + dataStr + "]";

    return st;
}

std::string RationalItemToString(int32_t item, const camera_metadata_item_t entry)
{
    std::string st = {};
    uint32_t count = entry.count;
    if (entry.data.r == nullptr) {
        METADATA_ERR_LOG("RationalItemToString: entry.data.r is null");
        return st;
    }
    std::string dataStr = std::to_string((*(entry.data.r)).numerator) + "/" +
        std::to_string((*(entry.data.r)).denominator);
    for (uint32_t i = 1; i < count; i++) {
        if ((i % WRAP_LENGTH) == 0) {
            dataStr += "]\n\t[" + std::to_string((*(entry.data.r + i)).numerator) + "/" +
                std::to_string((*(entry.data.r + i)).denominator);
        } else {
            dataStr += " " + std::to_string((*(entry.data.r + i)).numerator) + "/" +
                std::to_string((*(entry.data.r + i)).denominator);
        }
    }

    const char *name = GetCameraMetadataItemName(item);
    if (name == nullptr) {
        METADATA_ERR_LOG("RationalItemToString: get rational item name fail");
        return st;
    }
    std::string nameStr(name);

    st = nameStr + " (" + std::to_string(entry.index) + "): " +
        OHOS_CAMERA_METADATA_TYPE[entry.data_type] + "[" + std::to_string(count) + "]" + "\n\t[" + dataStr + "]";

    return st;
}

std::string CameraMetadata::MetadataItemDump(const common_metadata_header_t *metadataHeader, uint32_t item)
{
    camera_metadata_item_t entry;
    std::string st = {};

    int ret = FindCameraMetadataItem(metadataHeader, item, &entry);
    if (ret != 0) {
        METADATA_ERR_LOG("get item error and item = %{public}d", item);
        return st;
    }

    switch (entry.data_type) {
        case META_TYPE_BYTE:
            st = U8ItemToString(item, entry);
            break;
        case META_TYPE_INT32:
            st = I32ItemToString(item, entry);
            break;
        case META_TYPE_UINT32:
            st = U32ItemToString(item, entry);
            break;
        case META_TYPE_FLOAT:
            st = FloatItemToString(item, entry);
            break;
        case META_TYPE_INT64:
            st = I64ItemToString(item, entry);
            break;
        case META_TYPE_DOUBLE:
            st = DoubleItemToString(item, entry);
            break;
        case META_TYPE_RATIONAL:
            st = RationalItemToString(item, entry);
            break;
        default:
            METADATA_ERR_LOG("invalid param and item = %{public}d", item);
            break;
    }

    if (!st.empty()) {
        st += "\n";
    }
    return st;
}

std::string CameraMetadata::FormatCameraMetadataToString(const common_metadata_header_t *metadataHeader)
{
    std::string metaStr;
    if (metadataHeader == nullptr) {
        METADATA_ERR_LOG("metadataHeader is nullptr");
        return metaStr;
    }

    for (auto it = g_metadataTags.begin(); it != g_metadataTags.end(); it++) {
        metaStr += MetadataItemDump(metadataHeader, *it);
    }
    METADATA_DEBUG_LOG("metadataHeader item = %{public}s", metaStr.c_str());
    return metaStr;
}

int32_t CameraMetadata::GetAllVendorTags(std::vector<vendorTag_t>& tagVec)
{
    int32_t ret = LoadVendorTagImpl();
    if (ret != CAM_META_SUCCESS) {
        METADATA_ERR_LOG("LoadVendorTagImpl failed");
        return CAM_META_FAILURE;
    }
    std::lock_guard<std::mutex> lock(g_vendorTagImplMtx);
    g_vendorTagImpl->GetAllVendorTags(tagVec);
    return CAM_META_SUCCESS;
}

bool CameraMetadata::CheckItemDataType(camera_metadata_item_entry_t &item)
{
    auto iter = g_itemDataTypeMap.find(item.item);
    if (iter != g_itemDataTypeMap.end()) {
        if (iter->second != item.data_type) {
            METADATA_ERR_LOG("Invalid data type :%{public}u, item :%{public}u", item.data_type, item.item);
            return false;
        }
    }
    return true;
}
} // Camera
