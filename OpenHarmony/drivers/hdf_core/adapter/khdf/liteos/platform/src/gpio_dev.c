/*
 * Copyright (c) 2020-2023 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 *    of conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gpio_dev.h"
#include "fcntl.h"
#include "fs/driver.h"
#include "gpio_core.h"
#include "gpio_if.h"
#include "hdf_base.h"
#include "hdf_core_log.h"
#include "osal_mem.h"
#include "user_copy.h"

static int GpioOpen(struct file *filep)
{
    (void)filep;
    return 0;
}

static int GpioClose(struct file *filep)
{
    (void)filep;
    return 0;
}

static int GpioExecCmd(uint16_t gpio, struct GpioBitInfo *info, int cmd)
{
    int32_t ret;
    uint16_t tmp;

    switch (cmd) {
        case GPIO_SET_DIR:
            ret = GpioSetDir(gpio, info->direction);
            if (ret != HDF_SUCCESS) {
                HDF_LOGE("GpioExecCmd: set dir fail, ret: %d!", ret);
                return -1;
            }
            PLAT_LOGV("GpioExecCmd: gpio:%hu set dir:%u done!", gpio, info->direction);
            break;
        case GPIO_GET_DIR:
            ret = GpioGetDir(gpio, &tmp);
            if (ret != HDF_SUCCESS) {
                HDF_LOGE("GpioExecCmd: get dir fail, ret: %d!", ret);
                return -1;
            }
            info->direction = (unsigned char)tmp;
            PLAT_LOGV("GpioExecCmd: gpio:%hu get dir:%u done!", gpio, info->direction);
            break;
        case GPIO_READ_BIT:
            ret = GpioRead(gpio, &tmp);
            if (ret != HDF_SUCCESS) {
                HDF_LOGE("GpioExecCmd: read gpio fail, ret: %d!", ret);
                return -1;
            }
            info->value = (unsigned char)tmp;
            PLAT_LOGV("GpioExecCmd: gpio:%hu read:%u done!", gpio, info->value);
            break;
        case GPIO_WRITE_BIT:
            ret = GpioWrite(gpio, info->value);
            if (ret != HDF_SUCCESS) {
                HDF_LOGE("GpioExecCmd: write gpio fail, ret: %d!", ret);
                return -1;
            }
            PLAT_LOGV("GpioExecCmd: gpio:%hu write:%u done!", gpio, info->value);
            break;
        default:
            return -1;
    }
    return 0;
}

static int GpioIoctl(struct file *filep, int cmd, unsigned long arg)
{
    int ret;
    uint16_t bitNum;
    uint16_t gpio;
    struct GpioBitInfo info = {0};
    struct drv_data *drvData = NULL;

    if (arg == 0) {
        HDF_LOGE("GpioIoctl: arg is 0!");
        return HDF_ERR_INVALID_PARAM;
    }

    if (filep == NULL || filep->f_vnode == NULL || filep->f_vnode->data == NULL) {
        HDF_LOGE("GpioIoctl: function parameter is null!");
        return HDF_ERR_INVALID_PARAM;
    }

    drvData = (struct drv_data *)filep->f_vnode->data;
    bitNum = (uint16_t)(uintptr_t)drvData->priv;

    ret = LOS_CopyToKernel(&info, sizeof(struct GpioBitInfo),
        (const VOID *)(uintptr_t)arg, sizeof(struct GpioBitInfo));
    if (ret != 0) {
        return -1;
    }
    gpio = info.groupnumber * bitNum + info.bitnumber;
    PLAT_LOGV("GpioIoctl: gn:%u, bn:%u, gpio:%hu!", info.groupnumber, info.bitnumber, gpio);

    ret = GpioExecCmd(gpio, &info, cmd);
    if (ret != 0) {
        return -1;
    }

    if ((unsigned int)cmd == GPIO_GET_DIR || (unsigned int)cmd == GPIO_READ_BIT) {
        ret = LOS_CopyFromKernel((VOID *)(uintptr_t)arg, sizeof(struct GpioBitInfo),
            &info, sizeof(struct GpioBitInfo));
        if (ret != 0) {
            HDF_LOGE("GpioIoctl: copy back fail, ret: %d!", ret);
            return -1;
        }
    }
    return 0;
}

static const struct file_operations_vfs g_gpioDevOps = {
    .open = GpioOpen,
    .close = GpioClose,
    .ioctl = GpioIoctl,
};

#define GPIO_VFS_MODE 0660
#define GPIO_VFS_NAME "/dev/gpio"
int32_t GpioAddVfs(uint16_t bitNum)
{
    int ret;

    ret = register_driver(GPIO_VFS_NAME, &g_gpioDevOps, GPIO_VFS_MODE, (void *)(uintptr_t)bitNum);
    if (ret != 0) {
        HDF_LOGE("GpioAddVfs: register vfs fail, ret: %d!", ret);
        return HDF_FAILURE;
    }
    HDF_LOGI("GpioAddVfs: register vfs success!");
    return HDF_SUCCESS;
}

void GpioRemoveVfs(void)
{
    int ret;

    ret = unregister_driver(GPIO_VFS_NAME);
    if (ret != 0) {
        HDF_LOGE("GpioRemoveVfs: unregister vfs fail, ret: %d!", ret);
    }
}
