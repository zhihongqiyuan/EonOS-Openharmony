/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */

#ifndef LIGHT_DRIVER_H
#define LIGHT_DRIVER_H

#include "device_resource_if.h"
#include "gpio_if.h"
#include "hdf_base.h"
#include "hdf_device_desc.h"
#include "hdf_core_log.h"
#include "hdf_workqueue.h"
#include "osal_mutex.h"
#include "osal_timer.h"

#define LIGHT_MAKE_R_BIT    0X00FF0000
#define LIGHT_MAKE_G_BIT    0X0000FF00
#define LIGHT_MAKE_B_BIT    0X000000FF
#define LIGHT_WAIT_TIME     50

#define LIGHT_INVALID_GPIO    (-1)

#define LIGHT_ID_NUM    4

#define NAME_MAX_LEN    16

#define CHECK_LIGHT_NULL_PTR_RETURN_VALUE(ptr, ret) do { \
    if ((ptr) == NULL) { \
        HDF_LOGE("%s:line %d pointer is null and return ret", __func__, __LINE__); \
        return (ret); \
    } \
} while (0)

#define CHECK_LIGHT_NULL_PTR_RETURN(ptr) do { \
    if ((ptr) == NULL) { \
        HDF_LOGE("%s:line %d pointer is null and return", __func__, __LINE__); \
        return; \
    } \
} while (0)

#define CHECK_LIGHT_PARSER_RESULT_RETURN_VALUE(ret, str) do { \
    if ((ret) != HDF_SUCCESS) { \
        HDF_LOGE("%s:line %d %s fail, ret = %d!", __func__, __LINE__, str, ret); \
        return HDF_FAILURE; \
    } \
} while (0)

#define CHECK_LIGHT_PARSER_NUM_RETURN_VALUE(ret, num) do { \
    if ((ret) != HDF_SUCCESS) { \
        (num) = LIGHT_INVALID_GPIO; \
    } \
} while (0)

enum LightIoCmd {
    LIGHT_IO_CMD_GET_INFO_LIST     = 0,
    LIGHT_IO_CMD_OPS               = 1,
    LIGHT_IO_CMD_END,
};

enum LightOpsCmd {
    LIGHT_OPS_IO_CMD_ENABLE              = 0,
    LIGHT_OPS_IO_CMD_DISABLE             = 1,
    LIGHT_OPS_IO_CMD_ENABLE_MULTI_LIGHTS = 2,
    LIGHT_OPS_IO_CMD_END,
};

enum LightState {
    LIGHT_STATE_START   = 0,
    LIGHT_STATE_STOP    = 1,
    LIGHT_STATE_BUTT,
};

enum LightId {
    LIGHT_ID_NONE                = 0,
    LIGHT_ID_BATTERY             = 1,
    LIGHT_ID_NOTIFICATIONS       = 2,
    LIGHT_ID_ATTENTION           = 3,
    LIGHT_ID_BUTT,
};

enum HdfLightType {
    HDF_LIGHT_TYPE_SINGLE_COLOR = 1,
    HDF_LIGHT_TYPE_RGB_COLOR = 2,
    HDF_LIGHT_TYPE_WRGB_COLOR = 3,
};

enum LightFlashMode {
    LIGHT_FLASH_NONE = 0,
    LIGHT_FLASH_BLINK = 1,
    LIGHT_FLASH_GRADIENT = 2,
    LIGHT_FLASH_BUTT = 3,
};

struct LightFlashEffect {
    int32_t flashMode;
    uint32_t onTime;
    uint32_t offTime;
};

struct WRGBColor {
    uint8_t w;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct RGBColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t reserved;
};

union ColorValue {
    int32_t singleColor;
    struct RGBColor rgbColor;
    struct WRGBColor wrgbColor;
};

struct LightColor {
    union ColorValue colorValue;
};

struct LightEffect {
    struct LightColor lightColor;
    struct LightFlashEffect flashEffect;
};

struct LightInfo {
    char lightName[NAME_MAX_LEN];
    uint32_t lightId;
    uint32_t lightNumber;
    int32_t lightType;
};

struct LightDeviceInfo {
    enum LightState lightState;
    int32_t busRNum;
    int32_t busGNum;
    int32_t busBNum;
    uint32_t lightBrightness;
    uint32_t defaultBrightness;
    uint32_t onTime;
    uint32_t offTime;
    struct LightInfo lightInfo;
};

struct LightDriverData {
    struct IDeviceIoService ioService;
    struct HdfDeviceObject *device;
    HdfWorkQueue workQueue;
    HdfWork work;
    OsalTimer timer;
    struct OsalMutex mutex;
    uint32_t lightId;
    uint32_t lightNum;
    struct LightDeviceInfo *info[LIGHT_ID_BUTT];
};

typedef int32_t (*LightCmdHandle)(uint32_t lightId, struct HdfSBuf *data, struct HdfSBuf *reply);

struct LightCmdHandleList {
    enum LightOpsCmd cmd;
    LightCmdHandle func;
};

#endif /* LIGHT_DRIVER_H */
