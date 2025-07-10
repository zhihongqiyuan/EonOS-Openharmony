/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */

#include "vibrator_driver.h"
#include <securec.h>
#include "hdf_base.h"
#include "hdf_device_desc.h"
#include "osal_mem.h"
#include "vibrator_haptic.h"

#define HDF_LOG_TAG    khdf_vibrator_driver

#define VIBRATOR_WORK_QUEUE_NAME    "vibrator_queue"
#define VIBRATOR_START_TIME    10

struct VibratorDriverData *g_vibratorDrvData = NULL;

static struct VibratorDriverData *GetVibratorDrvData(void)
{
    return g_vibratorDrvData;
}

int32_t RegisterVibratorOps(struct VibratorOps *ops)
{
    struct VibratorDriverData *drvData = GetVibratorDrvData();

    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(ops, HDF_FAILURE);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_FAILURE);

    (void)OsalMutexLock(&drvData->mutex);
    drvData->ops.Start = ops->Start;
    drvData->ops.StartEffect = ops->StartEffect;
    drvData->ops.Stop = ops->Stop;
    drvData->ops.SetParameter = ops->SetParameter;
    (void)OsalMutexUnlock(&drvData->mutex);

    return HDF_SUCCESS;
}

int32_t RegisterVibratorInfo(struct VibratorInfo *vibratorInfo)
{
    struct VibratorDriverData *drvData = GetVibratorDrvData();

    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(vibratorInfo, HDF_FAILURE);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_FAILURE);

    (void)OsalMutexLock(&drvData->mutex);
    if (memcpy_s(&drvData->vibratorInfo, sizeof(drvData->vibratorInfo), vibratorInfo, sizeof(*vibratorInfo)) != EOK) {
        (void)OsalMutexUnlock(&drvData->mutex);
        HDF_LOGE("%s: Memcpy vibrator config failed!", __func__);
        return HDF_FAILURE;
    }
    (void)OsalMutexUnlock(&drvData->mutex);

    return HDF_SUCCESS;
}

void StartVibrator(void)
{
    struct VibratorDriverData *drvData = GetVibratorDrvData();

    CHECK_VIBRATOR_NULL_PTR_RETURN(drvData);

    drvData->state = VIBRATOR_STATE_START_TIMER;
    HdfAddWork(&drvData->workQueue, &drvData->work);
}

void StopVibrator(void)
{
    struct VibratorDriverData *drvData = GetVibratorDrvData();
    CHECK_VIBRATOR_NULL_PTR_RETURN(drvData);

    drvData->state = VIBRATOR_STATE_STOP;
    HdfAddWork(&drvData->workQueue, &drvData->work);
}

void SetEffectVibrator(uint32_t type)
{
    int32_t ret;
    struct VibratorDriverData *drvData = GetVibratorDrvData();

    CHECK_VIBRATOR_NULL_PTR_RETURN(drvData);
    CHECK_VIBRATOR_NULL_PTR_RETURN(drvData->ops.StartEffect);

    ret = drvData->ops.StartEffect(type);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: start effect failed!", __func__);
        return;
    }

    drvData->state = VIBRATOR_STATE_SET_EFFECT;
}

static void VibratorWorkEntry(void *para)
{
    int32_t ret = HDF_FAILURE;
    struct VibratorDriverData *drvData = (struct VibratorDriverData *)para;

    CHECK_VIBRATOR_NULL_PTR_RETURN(drvData);
    CHECK_VIBRATOR_NULL_PTR_RETURN(drvData->ops.Start);
    CHECK_VIBRATOR_NULL_PTR_RETURN(drvData->ops.Stop);

    if (drvData->state == VIBRATOR_STATE_START_TIMER) {
        ret = drvData->ops.Start();
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%s: add vibrator work fail! device state[%d]!", __func__, drvData->state);
        }
    }

    if (drvData->state == VIBRATOR_STATE_STOP) {
        ret = drvData->ops.Stop();
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%s: add vibrator work fail! device state[%d]!", __func__, drvData->state);
        }
    }
}

static int32_t StartOnce(struct HdfSBuf *data, struct HdfSBuf *reply)
{
    uint32_t duration;
    int32_t ret;
    struct VibratorEffectCfg config;
    struct VibratorDriverData *drvData = GetVibratorDrvData();
    (void)reply;

    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(data, HDF_FAILURE);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_FAILURE);

    if (!HdfSbufReadUint32(data, &duration)) {
        HDF_LOGE("%s: sbuf read duration failed!", __func__);
        return HDF_FAILURE;
    }

    if (drvData->mode != VIBRATOR_MODE_BUTT) {
        HDF_LOGI("%s: vibrater haptic is busy now, stop first!", __func__);
        ret = StopHaptic();
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%s: stop haptic failed!", __func__);
            return ret;
        }
    }

    (void)OsalMutexLock(&drvData->mutex);
    drvData->mode = VIBRATOR_MODE_ONCE;
    (void)OsalMutexUnlock(&drvData->mutex);

    // start once time vibrate
    config.cfgMode = VIBRATOR_MODE_ONCE;
    config.duration = duration;
    config.effect = NULL;

    ret = StartHaptic(&config);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: start haptic failed!", __func__);
        return ret;
    }

    return HDF_SUCCESS;
}

static int32_t IsVibratorRunning(struct HdfSBuf *data, struct HdfSBuf *reply)
{
    (void)data;
    struct VibratorDriverData *drvData = GetVibratorDrvData();
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(reply, HDF_ERR_INVALID_PARAM);

    int32_t state = drvData->mode == VIBRATOR_MODE_BUTT ? 0 : 1;
    HDF_LOGI("%s: state %d", __func__, state);

    if (!HdfSbufWriteInt32(reply, state)) {
        HDF_LOGE("%s: write state failed!", __func__);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t StartEffect(struct HdfSBuf *data, struct HdfSBuf *reply)
{
    int32_t ret;
    const char *effect = NULL;
    struct VibratorEffectCfg config;
    struct VibratorDriverData *drvData = GetVibratorDrvData();
    (void)reply;

    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(data, HDF_FAILURE);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_FAILURE);

    effect = HdfSbufReadString(data);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(effect, HDF_FAILURE);

    if (drvData->mode != VIBRATOR_MODE_BUTT) {
        HDF_LOGI("%s: vibrater haptic is busy now, stop first!", __func__);
        ret = StopHaptic();
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("%s: stop haptic failed!", __func__);
            return ret;
        }
    }

    (void)OsalMutexLock(&drvData->mutex);
    drvData->mode = VIBRATOR_MODE_PRESET;
    (void)OsalMutexUnlock(&drvData->mutex);

    // start once time vibrate
    config.cfgMode = VIBRATOR_MODE_PRESET;
    config.duration = 0;
    config.effect = effect;

    ret = StartHaptic(&config);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: start haptic failed!", __func__);
        return ret;
    }

    return HDF_SUCCESS;
}

static int32_t Stop(struct HdfSBuf *data, struct HdfSBuf *reply)
{
    int32_t ret;
    int32_t mode;
    struct VibratorDriverData *drvData = GetVibratorDrvData();
    (void)reply;

    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(data, HDF_FAILURE);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_FAILURE);

    if (!HdfSbufReadInt32(data, &mode)) {
        HDF_LOGE("%s: sbuf read mode failed!", __func__);
        return HDF_FAILURE;
    }

    if ((mode != VIBRATOR_MODE_ONCE) && (mode != VIBRATOR_MODE_PRESET)) {
        HDF_LOGE("%s: vibrator stop mode failed!", __func__);
        return HDF_FAILURE;
    }

    if (drvData->mode == VIBRATOR_MODE_BUTT) {
        HDF_LOGD("%s: vibrater haptic had stopped!", __func__);
        return HDF_SUCCESS;
    }

    ret = StopHaptic();
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: stop haptic failed!", __func__);
        return ret;
    }

    (void)OsalMutexLock(&drvData->mutex);
    drvData->mode = VIBRATOR_MODE_BUTT;
    (void)OsalMutexUnlock(&drvData->mutex);

    return HDF_SUCCESS;
}

static int32_t GetVibratorInfo(struct HdfSBuf *data, struct HdfSBuf *reply)
{
    (void)data;
    struct VibratorDriverData *drvData;

    drvData = GetVibratorDrvData();
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(reply, HDF_ERR_INVALID_PARAM);

    if (!HdfSbufWriteBuffer(reply, &drvData->vibratorInfo, sizeof(drvData->vibratorInfo))) {
        HDF_LOGE("%s: write sbuf failed!", __func__);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t EnableModulationParameter(struct HdfSBuf *data, struct HdfSBuf *reply)
{
    (void)reply;
    struct VibratorEffectCfg config;
    struct VibratorDriverData *drvData;
    uint32_t duration;
    uint16_t intensity;
    int16_t frequency;
    int32_t ret;

    drvData = GetVibratorDrvData();
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData->ops.SetParameter, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(data, HDF_ERR_INVALID_PARAM);

    if (drvData->mode != VIBRATOR_MODE_BUTT) {
        HDF_LOGE("%s: vibrater is busy now, please stop first!", __func__);
        return HDF_ERR_DEVICE_BUSY;
    }

    if (!HdfSbufReadUint32(data, &duration)) {
        HDF_LOGE("%s: sbuf read vibration period failed!", __func__);
        return HDF_FAILURE;
    }

    if (!HdfSbufReadUint16(data, &intensity)) {
        HDF_LOGE("%s: sbuf read intensity failed!", __func__);
        return HDF_FAILURE;
    }

    if (!HdfSbufReadInt16(data, &frequency)) {
        HDF_LOGE("%s: sbuf read frequency failed!", __func__);
        return HDF_FAILURE;
    }

    (void)OsalMutexLock(&drvData->mutex);
    drvData->mode = VIBRATOR_MODE_ONCE;
    (void)OsalMutexUnlock(&drvData->mutex);

    ret = drvData->ops.SetParameter(intensity, frequency);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: set parameter failed!", __func__);
        return HDF_FAILURE;
    }

    config.cfgMode = VIBRATOR_MODE_ONCE;
    config.duration = duration;
    config.effect = NULL;

    ret = StartHaptic(&config);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("%s: start haptic failed!", __func__);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}
static struct VibratorCmdHandleList g_vibratorCmdHandle[] = {
    {VIBRATOR_DRV_IO_START_ONCE, StartOnce},
    {VIBRATOR_DRV_IO_START_PRESET, StartEffect},
    {VIBRATOR_DRV_IO_STOP, Stop},
    {VIBRATOR_DRV_IO_GET_INFO, GetVibratorInfo},
    {VIBRATOR_DRV_IO_ENABLE_MODULATION_PARAMETER, EnableModulationParameter},
    {VIBRATOR_DRV_IO_IS_VIBRATOR_RUNNING, IsVibratorRunning},
};

static int32_t DispatchVibrator(struct HdfDeviceIoClient *client,
    int32_t cmd, struct HdfSBuf *data, struct HdfSBuf *reply)
{
    (void)client;
    int32_t loop;

    for (loop = 0; loop < sizeof(g_vibratorCmdHandle) / sizeof(g_vibratorCmdHandle[0]); ++loop) {
        if ((cmd == g_vibratorCmdHandle[loop].cmd) && (g_vibratorCmdHandle[loop].func != NULL)) {
            return g_vibratorCmdHandle[loop].func(data, reply);
        }
    }

    return HDF_SUCCESS;
}

int32_t BindVibratorDriver(struct HdfDeviceObject *device)
{
    struct VibratorDriverData *drvData = NULL;
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(device, HDF_FAILURE);

    drvData = (struct VibratorDriverData *)OsalMemCalloc(sizeof(*drvData));
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_ERR_MALLOC_FAIL);

    drvData->ioService.Dispatch = DispatchVibrator;
    drvData->device = device;
    device->service = &drvData->ioService;
    g_vibratorDrvData = drvData;

    return HDF_SUCCESS;
}

int32_t InitVibratorDriver(struct HdfDeviceObject *device)
{
    struct VibratorDriverData *drvData = NULL;

    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(device, HDF_FAILURE);
    drvData = (struct VibratorDriverData *)device->service;
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(drvData, HDF_FAILURE);

    drvData->mode = VIBRATOR_MODE_BUTT;
    drvData->state = VIBRATOR_STATE_IDLE;

    if (OsalMutexInit(&drvData->mutex) != HDF_SUCCESS) {
        HDF_LOGE("%s: init mutex failed!", __func__);
        return HDF_FAILURE;
    }

    if (HdfWorkQueueInit(&drvData->workQueue, VIBRATOR_WORK_QUEUE_NAME) != HDF_SUCCESS) {
        HDF_LOGE("%s: init workQueue failed!", __func__);
        return HDF_FAILURE;
    }

    if (HdfWorkInit(&drvData->work, VibratorWorkEntry, (void*)drvData) != HDF_SUCCESS) {
        HDF_LOGE("%s: init workQueue failed!", __func__);
        return HDF_FAILURE;
    }

    if (CreateVibratorHaptic(device) != HDF_SUCCESS) {
        HDF_LOGE("%s: init workQueue failed!", __func__);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

void ReleaseVibratorDriver(struct HdfDeviceObject *device)
{
    struct VibratorDriverData *drvData = NULL;

    if (device == NULL) {
        HDF_LOGE("%s: device is null!", __func__);
        return;
    }

    drvData = (struct VibratorDriverData *)device->service;
    if (drvData == NULL) {
        HDF_LOGE("%s: drvData is null!", __func__);
        return;
    }

    (void)DestroyVibratorHaptic();
    (void)OsalMutexDestroy(&drvData->mutex);
    OsalMemFree(drvData);
    g_vibratorDrvData = NULL;
}

struct HdfDriverEntry g_vibratorDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_VIBRATOR",
    .Bind = BindVibratorDriver,
    .Init = InitVibratorDriver,
    .Release = ReleaseVibratorDriver,
};

HDF_INIT(g_vibratorDriverEntry);
