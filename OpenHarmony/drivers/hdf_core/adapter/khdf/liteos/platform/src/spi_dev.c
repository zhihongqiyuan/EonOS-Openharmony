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

#include "fs/driver.h"
#include "securec.h"
#include "user_copy.h"
#include "hdf_core_log.h"
#include "osal_mem.h"
#include "spi_dev.h"

#define HDF_LOG_TAG spi_dev
#define HDF_SPI_FS_MODE 0660

#ifdef LOSCFG_FS_VFS
static struct SpiDev *SpiDevGetDevFromFilep(struct file *filep)
{
    if (filep == NULL) {
        HDF_LOGE("SpiDevGetDevFromFilep: filep is invalid!");
        return NULL;
    }

    struct Vnode *vnode = filep->f_vnode;
    struct SpiDev *dev = (struct SpiDev *)(((struct drv_data *)vnode->data)->priv);

    if (dev == NULL || dev->cntlr == NULL) {
        HDF_LOGE("SpiDevGetDevFromFilep: dev is invalid!");
        return NULL;
    }
    return dev;
}

static int32_t SpiDevOpen(struct file *filep)
{
    struct SpiDev *dev = NULL;

    dev = SpiDevGetDevFromFilep(filep);
    if (dev == NULL) {
        HDF_LOGE("SpiDevOpen: dev is null!");
        return HDF_ERR_INVALID_PARAM;
    }
    HDF_LOGE("SpiDevOpen: spi bus is %d, cs is %d!", dev->cntlr->busNum, dev->csNum);
    return HDF_SUCCESS;
}

static ssize_t SpiDevRead(struct file *filep, char *buf, size_t size)
{
    int32_t ret;
    struct SpiMsg msg = {0};
    uint8_t *tmpReadBuf = NULL;
    struct SpiDev *dev = NULL;

    if (buf == NULL || size == 0) {
        HDF_LOGE("SpiDevRead: buf or size %d is invalid!", size);
        return HDF_ERR_INVALID_PARAM;
    }
    dev = SpiDevGetDevFromFilep(filep);
    if (dev == NULL) {
        HDF_LOGE("SpiDevRead: spi get dev fail!");
        return HDF_ERR_INVALID_PARAM;
    }
    msg.len = size;
    msg.wbuf = NULL;
    if (LOS_IsUserAddressRange((vaddr_t)(uintptr_t)buf, size)) {
        tmpReadBuf = (uint8_t *)OsalMemCalloc(size);
        if (tmpReadBuf == NULL) {
            HDF_LOGE("SpiDevRead: OsalMemCalloc error!");
            return HDF_ERR_MALLOC_FAIL;
        }
        msg.rbuf = tmpReadBuf;
        ret = SpiCntlrTransfer(dev->cntlr, dev->csNum, &msg, 1);
        if (ret == HDF_SUCCESS) {
            ret = LOS_CopyFromKernel(buf, size, tmpReadBuf, size);
        }
        OsalMemFree(tmpReadBuf);
        return ret;
    } else {
        msg.rbuf = (uint8_t *)buf;
        return SpiCntlrTransfer(dev->cntlr, dev->csNum, &msg, 1);
    }
}

static ssize_t SpiDevWrite(struct file *filep, const char *buf, size_t size)
{
    int32_t ret;
    struct SpiMsg msg = {0};
    uint8_t *tmpWriteBuf = NULL;
    struct SpiDev *dev = NULL;

    if (buf == NULL || size == 0) {
        HDF_LOGE("SpiDevWrite: buf or size %d is invalid!", size);
        return HDF_ERR_INVALID_PARAM;
    }
    dev = SpiDevGetDevFromFilep(filep);
    if (dev == NULL) {
        HDF_LOGE("SpiDevWrite: spi get dev fail!");
        return HDF_ERR_INVALID_PARAM;
    }
    msg.len = size;
    msg.rbuf = NULL;
    if (LOS_IsUserAddressRange((vaddr_t)(uintptr_t)buf, size)) {
        tmpWriteBuf = (uint8_t *)OsalMemCalloc(size);
        if (tmpWriteBuf == NULL) {
            HDF_LOGE("SpiDevWrite: OsalMemCalloc error!");
            return HDF_ERR_MALLOC_FAIL;
        }
        ret = LOS_CopyToKernel(tmpWriteBuf, size, buf, size);
        if (ret != LOS_OK) {
            OsalMemFree(tmpWriteBuf);
            return ret;
        }
        msg.wbuf = tmpWriteBuf;
        ret = SpiCntlrTransfer(dev->cntlr, dev->csNum, &msg, 1);
        OsalMemFree(tmpWriteBuf);
        return ret;
    } else {
        msg.wbuf = (uint8_t *)buf;
        return SpiCntlrTransfer(dev->cntlr, dev->csNum, &msg, 1);
    }
}

static int32_t SpiDevGetCfg(struct SpiDev *dev, struct SpiCfg *mask, unsigned long arg)
{
    int32_t ret;
    uint32_t tmp = 0;
    struct SpiCfg cfg = {0};
    struct SpiCntlr *cntlr = dev->cntlr;

    if (arg == 0) {
        HDF_LOGE("SpiDevGetCfg: arg is 0!");
        return HDF_ERR_INVALID_PARAM;
    }
    ret = SpiCntlrGetCfg(cntlr, dev->csNum, &cfg);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("SpiDevGetCfg: GetCfg error!");
        return HDF_FAILURE;
    }
    if (mask->mode == 1) {
        tmp = cfg.mode;
        HDF_LOGI("SpiDevGetCfg: get mode 0x%x!", tmp);
    } else if (mask->bitsPerWord == 1) {
        tmp = cfg.bitsPerWord;
        HDF_LOGI("SpiDevGetCfg: get word %u!", tmp);
    } else if (mask->maxSpeedHz == 1) {
        tmp = cfg.maxSpeedHz;
        HDF_LOGI("SpiDevGetCfg: get maxspeed %d!", tmp);
    }
    ret = LOS_CopyFromKernel((void *)(uintptr_t)arg, sizeof(uint32_t), (void *)&tmp, sizeof(uint32_t));
    if (ret != 0) {
        HDF_LOGE("SpiDevGetCfg: memery copy error!");
    }
    return ret;
}

static int32_t SpiDevSetCfg(struct SpiDev *dev, struct SpiCfg *mask, unsigned long arg)
{
    int32_t ret;
    uint32_t tmp;
    struct SpiCfg cfg = {0};
    struct SpiCntlr *cntlr = dev->cntlr;

    if (arg == 0) {
        HDF_LOGE("SpiDevSetCfg: arg is 0!");
        return HDF_ERR_INVALID_PARAM;
    }
    ret = LOS_CopyToKernel((void *)&tmp, sizeof(uint32_t), (void *)(uintptr_t)arg, sizeof(uint32_t));
    if (ret != 0) {
        HDF_LOGE("SpiDevSetCfg: memery copy error!");
        return ret;
    }

    ret = SpiCntlrGetCfg(cntlr, dev->csNum, &cfg);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("SpiDevSetCfg: GetCfg error!");
        return HDF_FAILURE;
    }
    if (mask->mode == 1) {
        HDF_LOGI("SpiDevSetCfg: set mode 0x%x!", tmp);
        cfg.mode = tmp;
    } else if (mask->bitsPerWord == 1) {
        HDF_LOGI("SpiDevSetCfg: set word %u!", tmp);
        cfg.bitsPerWord = tmp;
    } else if (mask->maxSpeedHz == 1) {
        HDF_LOGI("SpiDevSetCfg: set maxspeed %d!", tmp);
        cfg.maxSpeedHz = tmp;
    }
    return SpiCntlrSetCfg(cntlr, dev->csNum, &cfg);
}

static struct SpiIocMsg *SpiDevGetIocMsgFromUser(unsigned long arg)
{
    int32_t ret;
    struct SpiIocMsg *umsg = NULL;

    if (arg == 0) {
        HDF_LOGE("SpiDevGetIocMsgFromUser: arg is 0!");
        return NULL;
    }
    umsg = (struct SpiIocMsg *)OsalMemCalloc(sizeof(struct SpiIocMsg));
    if (umsg == NULL) {
        HDF_LOGE("SpiDevGetIocMsgFromUser: melloc umsg error!");
        return NULL;
    }
    ret = LOS_CopyToKernel((void *)umsg, sizeof(struct SpiIocMsg), (void *)(uintptr_t)arg, sizeof(struct SpiIocMsg));
    if (ret != 0) {
        HDF_LOGE("SpiDevGetIocMsgFromUser: copy to kernel error!");
        OsalMemFree(umsg);
        return NULL;
    }
    return umsg;
}

static struct SpiMsg *SpiDevGetSpiMsgFromUser(struct SpiIocMsg *umsg)
{
    int32_t ret;
    int32_t count;
    struct SpiMsg *msg = NULL;

    count = umsg->count;
    msg = (struct SpiMsg *)OsalMemCalloc(sizeof(struct SpiMsg) * count + sizeof(struct SpiMsg) * count);
    if (msg == NULL) {
        HDF_LOGE("SpiDevGetSpiMsgFromUser: melloc msg error!");
        return NULL;
    }
    ret = LOS_CopyToKernel((void *)msg, sizeof(struct SpiMsg) * count,
        (void *)(umsg->msg), sizeof(struct SpiMsg) * count);
    if (ret != 0) {
        HDF_LOGE("SpiDevGetSpiMsgFromUser: copy to kernel error!");
        OsalMemFree(msg);
        return NULL;
    }
    return msg;
}

static int32_t SpiDevRealTransfer(struct SpiDev *dev, struct SpiMsg *msg, struct SpiMsg *kmsg, int32_t count)
{
    int32_t i;
    int32_t len = 0;
    uint32_t pos = 0;
    uint8_t *wbuf = NULL;
    uint8_t *rbuf = NULL;

    for (i = 0; i < count; i++) {
        len += msg[i].len;
    }
    if (len <= 0) {
        HDF_LOGE("SpiDevRealTransfer: err, msg total len is %d!", len);
        return HDF_ERR_INVALID_PARAM;
    }
    wbuf = (uint8_t *)OsalMemCalloc(sizeof(uint8_t) * (len + len));
    if (wbuf == NULL) {
        HDF_LOGE("SpiDevRealTransfer: melloc wbuf error!");
        return HDF_ERR_MALLOC_FAIL;
    }
    rbuf = wbuf + sizeof(uint8_t) * len;
    for (i = 0; i < count; i++) {
        if (LOS_CopyToKernel(wbuf + pos, msg[i].len, (void *)msg[i].wbuf, msg[i].len) != 0) {
            HDF_LOGE("SpiDevRealTransfer: copy to kernel error!");
            OsalMemFree(wbuf);
            return HDF_ERR_IO;
        }
        kmsg[i].wbuf = wbuf + pos;
        kmsg[i].rbuf = rbuf + pos;
        kmsg[i].len = msg[i].len;
        kmsg[i].keepCs = msg[i].keepCs;
        kmsg[i].delayUs = msg[i].delayUs;
        kmsg[i].speed = msg[i].speed;
        pos += msg[i].len;
    }
    if (SpiCntlrTransfer(dev->cntlr, dev->csNum, kmsg, count) != HDF_SUCCESS) {
        HDF_LOGE("SpiDevRealTransfer: transfer error!");
        OsalMemFree(wbuf);
        return HDF_FAILURE;
    }
    for (i = 0; i < count; i++) {
        if (LOS_CopyFromKernel((void *)msg[i].rbuf, msg[i].len, (void *)kmsg[i].rbuf, msg[i].len) != 0) {
            HDF_LOGE("SpiDevRealTransfer: copy from kernel error!");
            OsalMemFree(wbuf);
            return HDF_ERR_IO;
        }
    }
    OsalMemFree(wbuf);
    return HDF_SUCCESS;
}

static int32_t SpiDevTransfer(struct SpiDev *dev, unsigned long arg)
{
    int32_t ret;
    int32_t count;
    struct SpiMsg *msg = NULL;
    struct SpiMsg *kmsg = NULL;
    struct SpiIocMsg *umsg = NULL;

    if (!LOS_IsUserAddressRange((vaddr_t)(uintptr_t)arg, sizeof(struct SpiIocMsg))) {
        umsg = (struct SpiIocMsg *)(uintptr_t)arg;
        return SpiCntlrTransfer(dev->cntlr, dev->csNum, umsg->msg, umsg->count);
    }
    umsg = SpiDevGetIocMsgFromUser(arg);
    if (umsg == NULL) {
        HDF_LOGE("SpiDevTransfer: melloc umsg error!");
        return HDF_ERR_MALLOC_FAIL;
    }
    count = umsg->count;
    if (count <= 0) {
        HDF_LOGE("SpiDevTransfer: umsg error!");
        OsalMemFree(umsg);
        return HDF_ERR_INVALID_OBJECT;
    }
    msg = SpiDevGetSpiMsgFromUser(umsg);
    if (msg == NULL) {
        OsalMemFree(umsg);
        return HDF_ERR_IO;
    }
    kmsg = msg + count;
    ret = SpiDevRealTransfer(dev, msg, kmsg, count);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("SpiDevTransfer: spi dev transfer error: %d!", ret);
    }
    OsalMemFree(msg);
    OsalMemFree(umsg);
    return ret;
}

static int32_t SpiDevIoctl(struct file *filep, int32_t cmd, unsigned long arg)
{
    int ret;
    struct SpiCfg mask = {0};
    struct SpiDev *dev = NULL;

    dev = SpiDevGetDevFromFilep(filep);
    if (dev == NULL) {
        HDF_LOGE("SpiDevIoctl: dev is null!");
        return HDF_ERR_INVALID_PARAM;
    }
    switch (cmd) {
        case SPI_IOC_MESSAGE:
            ret = SpiDevTransfer(dev, arg);
            break;
        case SPI_IOC_RD_MODE:
            mask.mode = 1;
            ret = SpiDevGetCfg(dev, &mask, arg);
            break;
        case SPI_IOC_RD_BITS_PER_WORD:
            mask.bitsPerWord = 1;
            ret = SpiDevGetCfg(dev, &mask, arg);
            break;
        case SPI_IOC_RD_MAX_SPEED_HZ:
            mask.maxSpeedHz = 1;
            ret = SpiDevGetCfg(dev, &mask, arg);
            break;
        case SPI_IOC_WR_MODE:
            mask.mode = 1;
            ret = SpiDevSetCfg(dev, &mask, arg);
            break;
        case SPI_IOC_WR_BITS_PER_WORD:
            mask.bitsPerWord = 1;
            ret = SpiDevSetCfg(dev, &mask, arg);
            break;
        case SPI_IOC_WR_MAX_SPEED_HZ:
            mask.maxSpeedHz = 1;
            ret = SpiDevSetCfg(dev, &mask, arg);
            break;
        default:
            HDF_LOGE("SpiDevIoctl: cmd %d is not support!", cmd);
            ret = HDF_ERR_NOT_SUPPORT;
            break;
    }
    return ret;
}

const struct file_operations_vfs g_spiDevFops = {
    .open   = SpiDevOpen,
    .read   = SpiDevRead,
    .write  = SpiDevWrite,
    .ioctl  = SpiDevIoctl,
};

#define MAX_DEV_NAME_SIZE 32
static void SpiAddRemoveDev(struct SpiDev *dev, bool add)
{
    int32_t ret;
    char *devName = NULL;

    if (dev == NULL || dev->cntlr == NULL) {
        HDF_LOGE("SpiAddRemoveDev invalid parameter!");
        return;
    }
    devName = (char *)OsalMemCalloc(sizeof(char) * (MAX_DEV_NAME_SIZE + 1));
    if (devName == NULL) {
        HDF_LOGE("SpiAddRemoveDev: OsalMemCalloc error!");
        return;
    }
    ret = snprintf_s(devName, MAX_DEV_NAME_SIZE + 1, MAX_DEV_NAME_SIZE, "/dev/spidev%u.%u",
        dev->cntlr->busNum, dev->csNum);
    if (ret < 0) {
        HDF_LOGE("SpiAddRemoveDev snprintf_s fail!");
        OsalMemFree(devName);
        return;
    }
    if (add) {
        HDF_LOGI("create /dev/spidev%u.%u", dev->cntlr->busNum, dev->csNum);
        if (register_driver(devName, &g_spiDevFops, HDF_SPI_FS_MODE, dev) != HDF_SUCCESS) {
            HDF_LOGE("SpiAddRemoveDev: gen /dev/spidev%u.%u", dev->cntlr->busNum, dev->csNum);
        }
    } else {
        if (unregister_driver(devName) != HDF_SUCCESS) {
            HDF_LOGE("SpiAddRemoveDev: remove /dev/spidev%u.%u", dev->cntlr->busNum, dev->csNum);
        }
    }
    OsalMemFree(devName);
}

void SpiAddDev(struct SpiDev *device)
{
    SpiAddRemoveDev(device, true);
}

void SpiRemoveDev(struct SpiDev *device)
{
    SpiAddRemoveDev(device, false);
}
#else
void SpiAddDev(struct SpiDev *device)
{
    if (device != NULL && device->cntlr != NULL) {
        HDF_LOGE("SpiAddDev: add /dev/spidev%d.%d error", device->csNum, device->cntlr->busNum);
    }
    HDF_LOGE("SpiAddDev: LOSCFG_FS_VFS not define");
}

void SpiRemoveDev(struct SpiDev *device)
{
    if (device != NULL && device->cntlr != NULL) {
        HDF_LOGE("SpiRemoveDev: remove /dev/spidev%d.%d error", device->csNum, device->cntlr->busNum);
    }
    HDF_LOGE("SpiRemoveDev: LOSCFG_FS_VFS not define");
}
#endif /* end of LOSCFG_FS_VFS */
