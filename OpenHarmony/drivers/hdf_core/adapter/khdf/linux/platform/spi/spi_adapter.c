/*
 * spi_adapter.c
 *
 * spi driver adapter of linux
 *
 * Copyright (c) 2020-2023 Huawei Device Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/version.h>
#include "device_resource_if.h"
#include "hdf_base.h"
#include "hdf_dlist.h"
#include "hdf_core_log.h"
#include "osal_io.h"
#include "osal_mem.h"
#include "osal_time.h"
#include "platform_errno.h"
#include "spi_core.h"
#include "spi_if.h"

#define HDF_LOG_TAG HDF_SPI_LINUX_ADAPTER
#define SPI_DEV_NEED_FIND_NEXT 0
#define SPI_DEV_CREAT_FAILURE  1
#define SPI_DEV_FIND_SUCCESS   2

static void *g_linuxDefaultTransferOneMessage = NULL;

static uint16_t HdfSpiModeToLinuxMode(uint16_t mode)
{
    return ((!!(mode & SPI_CLK_PHASE) ? SPI_CPHA : 0) |
            (!!(mode & SPI_CLK_POLARITY) ? SPI_CPOL : 0) |
            (!!(mode & SPI_MODE_CS_HIGH) ? SPI_CS_HIGH : 0) |
            (!!(mode & SPI_MODE_LSBFE) ? SPI_LSB_FIRST : 0) |
            (!!(mode & SPI_MODE_3WIRE) ? SPI_3WIRE : 0) |
            (!!(mode & SPI_MODE_LOOP) ? SPI_LOOP : 0) |
            (!!(mode & SPI_MODE_NOCS) ? SPI_NO_CS : 0) |
            (!!(mode & SPI_MODE_READY) ? SPI_READY : 0));
}

static uint16_t LinuxSpiModeToHdfMode(uint16_t mode)
{
    return ((!!(mode & SPI_CPHA) ? SPI_CLK_PHASE : 0) |
            (!!(mode & SPI_CPOL) ? SPI_CLK_POLARITY : 0) |
            (!!(mode & SPI_CS_HIGH) ? SPI_MODE_CS_HIGH : 0) |
            (!!(mode & SPI_LSB_FIRST) ? SPI_MODE_LSBFE : 0) |
            (!!(mode & SPI_3WIRE) ? SPI_MODE_3WIRE : 0) |
            (!!(mode & SPI_LOOP) ? SPI_MODE_LOOP : 0) |
            (!!(mode & SPI_NO_CS) ? SPI_MODE_NOCS : 0) |
            (!!(mode & SPI_READY) ? SPI_MODE_READY : 0));
}

static struct SpiDev *SpiFindDeviceByCsNum(const struct SpiCntlr *cntlr, uint32_t cs)
{
    struct SpiDev *dev = NULL;
    struct SpiDev *tmpDev = NULL;

    if (cntlr->numCs <= cs) {
        HDF_LOGE("SpiFindDeviceByCsNum: invalid cs %u!", cs);
        return NULL;
    }
    DLIST_FOR_EACH_ENTRY_SAFE(dev, tmpDev, &(cntlr->list), struct SpiDev, list) {
        if (dev->csNum == cs) {
            return dev;
        }
    }
    return NULL;
}

static int32_t SpiAdapterSetCfg(struct SpiCntlr *cntlr, struct SpiCfg *cfg)
{
    int32_t ret;
    struct SpiDev *dev = NULL;
    struct spi_device *spidev = NULL;

    if (cntlr == NULL || cfg == NULL) {
        HDF_LOGE("SpiAdapterSetCfg: invalid parameter!");
        return HDF_ERR_INVALID_PARAM;
    }
    dev = SpiFindDeviceByCsNum(cntlr, cntlr->curCs);
    if (dev == NULL || dev->priv == NULL) {
        HDF_LOGE("SpiAdapterSetCfg: dev is invalid!");
        return HDF_FAILURE;
    }

    spidev = (struct spi_device *)dev->priv;
    spidev->bits_per_word = cfg->bitsPerWord;
    spidev->max_speed_hz = cfg->maxSpeedHz;
    spidev->mode = HdfSpiModeToLinuxMode(cfg->mode);
    ret = spi_setup(spidev);
    if (ret != 0) {
        HDF_LOGE("SpiAdapterSetCfg: spi_setup fail, ret is %d!", ret);
        return HDF_FAILURE;
    }

    dev->cfg = *cfg;
    return HDF_SUCCESS;
}

static int32_t SpiAdapterGetCfg(struct SpiCntlr *cntlr, struct SpiCfg *cfg)
{
    struct SpiDev *dev = NULL;

    if (cntlr == NULL || cfg == NULL) {
        HDF_LOGE("SpiAdapterGetCfg: invalid parameter!");
        return HDF_ERR_INVALID_PARAM;
    }
    dev = SpiFindDeviceByCsNum(cntlr, cntlr->curCs);
    if (dev == NULL || dev->priv == NULL) {
        HDF_LOGE("SpiAdapterGetCfg: fail, dev is invalid!");
        return HDF_FAILURE;
    }

    *cfg = dev->cfg;
    return HDF_SUCCESS;
}

static bool SpiAdapterIsDefaultTransferOneMessage(const struct SpiCntlr *cntlr)
{
    struct spi_device *linuxSpiDevice = NULL;
    struct SpiDev *spiDev = NULL;

    spiDev = SpiFindDeviceByCsNum(cntlr, cntlr->curCs);
    if (spiDev == NULL || spiDev ->priv == NULL) {
        HDF_LOGE("SpiAdapterIsDefaultTransferOneMessage: fail, spidev is null!\n");
        return false;
    }

    linuxSpiDevice = (struct spi_device *)spiDev->priv;
    if (linuxSpiDevice->controller != NULL) {
        return linuxSpiDevice->controller->transfer_one_message == g_linuxDefaultTransferOneMessage;
    }

    if (linuxSpiDevice->master != NULL) {
        return linuxSpiDevice->master->transfer_one_message == g_linuxDefaultTransferOneMessage;
    }
    return false;
}

static int32_t SpiAdapterTransferOneMsg(struct SpiCntlr *cntlr, struct SpiMsg *msg)
{
    int32_t ret;
    struct spi_message xfer;
    struct SpiDev *dev = NULL;
    struct spi_transfer *transfer = NULL;

    dev = SpiFindDeviceByCsNum(cntlr, cntlr->curCs);
    if (dev == NULL || dev->priv == NULL) {
        HDF_LOGE("SpiAdapterTransferOneMsg: fail, spidev is null!\n");
        return HDF_FAILURE;
    }

    transfer = kcalloc(1, sizeof(*transfer), GFP_KERNEL);
    if (transfer == NULL) {
        HDF_LOGE("SpiAdapterTransferOneMsg: transfer alloc memory fail!");
        return HDF_ERR_MALLOC_FAIL;
    }

    spi_message_init(&xfer);
    transfer->tx_buf = msg->wbuf;
    transfer->rx_buf = msg->rbuf;
    transfer->len = msg->len;
    transfer->speed_hz = msg->speed;
    transfer->cs_change = msg->keepCs; // yes! cs_change will keep the last cs active ...
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
    transfer->delay_usecs = msg->delayUs;
#else
    transfer->delay.value = msg->delayUs;
    transfer->delay.unit = SPI_DELAY_UNIT_USECS;
#endif
    spi_message_add_tail(transfer, &xfer);

    ret = spi_sync(dev->priv, &xfer);
    kfree(transfer);
    return ret;
}

static int32_t SpiAdapterTransferDefault(struct SpiCntlr *cntlr, struct SpiMsg *msgs, uint32_t count)
{
    int32_t ret;
    uint32_t i;

    if (cntlr == NULL || msgs == NULL || count == 0) {
        HDF_LOGE("SpiAdapterTransferDefault: invalid parameter!");
        return HDF_ERR_INVALID_PARAM;
    }

    for (i = 0; i < count; i++) {
        ret = SpiAdapterTransferOneMsg(cntlr, &msgs[i]);
        if (ret != 0) {
            HDF_LOGE("SpiAdapterTransferDefault: transfer one fail, ret: %d!", ret);
            return HDF_PLT_ERR_OS_API;
        }
    }
    return HDF_SUCCESS;
}

static int32_t SpiAdapterTransferDifferent(struct SpiCntlr *cntlr, struct SpiMsg *msg, uint32_t count)
{
    int32_t ret;
    uint32_t i;
    struct spi_message xfer;
    struct SpiDev *dev = NULL;
    struct spi_transfer *transfer = NULL;

    if (cntlr == NULL || msg == NULL || count == 0) {
        HDF_LOGE("SpiAdapterTransferDifferent: invalid parameter!");
        return HDF_ERR_INVALID_PARAM;
    }

    dev = SpiFindDeviceByCsNum(cntlr, cntlr->curCs);
    if (dev == NULL || dev->priv == NULL) {
        HDF_LOGE("SpiAdapterTransferDifferent: fail, spidev is null!\n");
        return HDF_FAILURE;
    }

    transfer = kcalloc(count, sizeof(*transfer), GFP_KERNEL);
    if (transfer == NULL) {
        HDF_LOGE("SpiAdapterTransferDifferent: transfer alloc memory fail!");
        return HDF_ERR_MALLOC_FAIL;
    }

    spi_message_init(&xfer);
    for (i = 0; i < count; i++) {
        transfer[i].tx_buf = msg[i].wbuf;
        transfer[i].rx_buf = msg[i].rbuf;
        transfer[i].len = msg[i].len;
        if (msg[i].speed != 0)
            transfer[i].speed_hz = msg[i].speed;
        if (msg[i].keepCs != 0)
            transfer[i].cs_change = msg[i].keepCs;
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
        if (msg[i].delayUs != 0)
            transfer[i].delay_usecs = msg[i].delayUs;
#else
        if (msg[i].delayUs != 0) {
            transfer[i].delay.value = msg[i].delayUs;
            transfer[i].delay.unit = SPI_DELAY_UNIT_USECS;
        }
#endif
        spi_message_add_tail(&transfer[i], &xfer);
    }

    ret = spi_sync(dev->priv, &xfer);
    kfree(transfer);
    return ret;
}

static const char *GetSpiDevName(const struct device *dev)
{
    if (dev->init_name) {
        return dev->init_name;
    }
    return kobject_name(&dev->kobj);
}

static void SpiDevInit(struct SpiDev *dev, const struct spi_device *spidev)
{
    dev->cfg.bitsPerWord = spidev->bits_per_word;
    dev->cfg.maxSpeedHz = spidev->max_speed_hz;
    dev->cfg.mode = LinuxSpiModeToHdfMode(spidev->mode);
    dev->cfg.transferMode = SPI_INTERRUPT_TRANSFER;
    dev->priv = (struct spi_device *)spidev;
}

static struct SpiDev *SpiDevCreat(struct SpiCntlr *cntlr)
{
    struct SpiDev *dev = NULL;

    dev = (struct SpiDev *)OsalMemCalloc(sizeof(*dev));
    if (dev == NULL) {
        HDF_LOGE("SpiDevCreat: OsalMemCalloc dev error!");
        return NULL;
    }

    DListHeadInit(&dev->list);
    DListInsertTail(&dev->list, &cntlr->list);
    return dev;
}

static int32_t SpiFindDeviceFromBus(struct device *dev, void *para)
{
    struct spi_device *spidev = NULL;
    struct SpiDev *spi = NULL;
    struct SpiCntlr *cntlr = (struct SpiCntlr *)para;

    if (dev == NULL || cntlr == NULL) {
        HDF_LOGE("SpiFindDeviceFromBus: invalid parameter!");
        return HDF_ERR_INVALID_PARAM;
    }

    spidev = CONTAINER_OF(dev, struct spi_device, dev);
    get_device(&spidev->dev);

    if (spidev->master == NULL) {
        put_device(&spidev->dev);
        HDF_LOGE("SpiFindDeviceFromBus: spi_device %s is invalid!", GetSpiDevName(&spidev->dev));
        return HDF_ERR_INVALID_PARAM;
    }
    HDF_LOGI("SpiFindDeviceFromBus: spi_device %s, find success!", GetSpiDevName(&spidev->dev));
    HDF_LOGI("SpiFindDeviceFromBus: spi_device bus_num %d, chip_select %d!",
        spidev->master->bus_num, spidev->chip_select);

    if (spidev->master->bus_num == cntlr->busNum && spidev->chip_select == cntlr->curCs) {
        spi = SpiFindDeviceByCsNum(cntlr, cntlr->curCs);
        if (spi == NULL) {
            spi = SpiDevCreat(cntlr);
        }
        if (spi == NULL) {
            put_device(&spidev->dev);
            return SPI_DEV_CREAT_FAILURE;
        }
        SpiDevInit(spi, spidev);
        return SPI_DEV_FIND_SUCCESS;
    } else {
        put_device(&spidev->dev);
        return SPI_DEV_NEED_FIND_NEXT;
    }
}

static int32_t SpiAdapterOpen(struct SpiCntlr *cntlr)
{
    int32_t ret;

    if (cntlr == NULL) {
        HDF_LOGE("SpiAdapterOpen: fail, cntlr is null!");
        return HDF_ERR_INVALID_PARAM;
    }
    ret = bus_for_each_dev(&spi_bus_type, NULL, (void *)cntlr, SpiFindDeviceFromBus);
    if (ret != SPI_DEV_FIND_SUCCESS) {
        HDF_LOGE("SpiAdapterOpen: spidev find fail, ret is %d!", ret);
        return HDF_FAILURE;
    }
    if (SpiAdapterIsDefaultTransferOneMessage(cntlr)) {
        HDF_LOGD("SpiAdapterOpen: use default method!");
        cntlr->method->Transfer = SpiAdapterTransferDefault;
    } else {
        HDF_LOGD("SpiAdapterOpen: use different method!");
        cntlr->method->Transfer = SpiAdapterTransferDifferent;
    }
    return HDF_SUCCESS;
}

static int32_t SpiAdapterClose(struct SpiCntlr *cntlr)
{
    struct SpiDev *dev = NULL;
    struct spi_device *spidev = NULL;

    if (cntlr == NULL) {
        HDF_LOGE("SpiAdapterClose: fail, cntlr is null!");
        return HDF_FAILURE;
    }

    dev = SpiFindDeviceByCsNum(cntlr, cntlr->curCs);
    if (dev == NULL) {
        HDF_LOGE("SpiAdapterClose: fail, dev is null!");
        return HDF_FAILURE;
    }

    spidev = (struct spi_device *)dev->priv;
    if (spidev == NULL) {
        HDF_LOGE("SpiAdapterClose: fail, spidev is null!");
        return HDF_FAILURE;
    }
    put_device(&spidev->dev);
    return HDF_SUCCESS;
}

struct SpiCntlrMethod g_method = {
    .SetCfg = SpiAdapterSetCfg,
    .GetCfg = SpiAdapterGetCfg,
    .Open = SpiAdapterOpen,
    .Close = SpiAdapterClose,
};

static int SpiAdapterTransferOneDummy(struct spi_controller *cntlr,
    struct spi_device *spi, struct spi_transfer *xfer)
{
    (void)cntlr;
    (void)spi;
    (void)xfer;
    return 0;
}

static void SpiPlatformDeviceRelease(struct device *dev)
{
    put_device(dev);
}

static int SpiAdapterRegisterDummyController(void)
{
    static struct platform_device pdev = {0};
    static struct spi_controller *dummyCntlr = NULL;

    if (g_linuxDefaultTransferOneMessage != NULL) {
        return HDF_SUCCESS;
    }
    pdev.name = "spi_adapter_dummy";
    pdev.id = -1;
    pdev.dev.release = SpiPlatformDeviceRelease;
    if (platform_device_register(&pdev) != 0) {
        HDF_LOGE("SpiAdapterRegisterDummyController: register platform device fail!");
        return HDF_PLT_ERR_OS_API;
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 9, 11)
    dummyCntlr = spi_alloc_master(&pdev.dev, 0);
#else
    dummyCntlr = devm_spi_alloc_master(&pdev.dev, 0);
#endif
    if (dummyCntlr == NULL) {
        HDF_LOGE("SpiAdapterRegisterDummyController: alloc dummyCntlr fail!");
        return HDF_ERR_MALLOC_FAIL;
    }
    dummyCntlr->slave = false;
    dummyCntlr->bus_num = -1;
    dummyCntlr->num_chipselect = 1;
    dummyCntlr->transfer_one = SpiAdapterTransferOneDummy;
    if (devm_spi_register_controller(&pdev.dev, dummyCntlr) != 0) {
        HDF_LOGE("SpiAdapterRegisterDummyController: register dummy controller fail!");
        return HDF_PLT_ERR_OS_API;
    }
    g_linuxDefaultTransferOneMessage = dummyCntlr->transfer_one_message;
    spi_controller_put(dummyCntlr);

    return HDF_SUCCESS;
}

static int32_t SpiGetBaseCfgFromHcs(struct SpiCntlr *cntlr, const struct DeviceResourceNode *node)
{
    struct DeviceResourceIface *iface = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);

    if (iface == NULL || iface->GetUint32 == NULL) {
        HDF_LOGE("SpiGetBaseCfgFromHcs: face is invalid!");
        return HDF_FAILURE;
    }
    if (iface->GetUint32(node, "busNum", &cntlr->busNum, 0) != HDF_SUCCESS) {
        HDF_LOGE("SpiGetBaseCfgFromHcs: read busNum fail!");
        return HDF_FAILURE;
    }
    if (iface->GetUint32(node, "numCs", &cntlr->numCs, 0) != HDF_SUCCESS) {
        HDF_LOGE("SpiGetBaseCfgFromHcs: read numCs fail!");
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int SpiCntlrInit(struct SpiCntlr *cntlr, const struct HdfDeviceObject *device)
{
    int ret;

    if (device->property == NULL) {
        HDF_LOGE("SpiCntlrInit: property is null!");
        return HDF_ERR_INVALID_PARAM;
    }

    ret = SpiGetBaseCfgFromHcs(cntlr, device->property);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("SpiCntlrInit: SpiGetBaseCfgFromHcs error!");
        return HDF_FAILURE;
    }

    cntlr->method = &g_method;
    return 0;
}

static int32_t HdfSpiDeviceBind(struct HdfDeviceObject *device)
{
    int ret;

    HDF_LOGI("HdfSpiDeviceBind: entry!");
    if (device == NULL) {
        return HDF_ERR_INVALID_OBJECT;
    }
    ret = SpiAdapterRegisterDummyController();
    if (ret != 0) {
        HDF_LOGE("HdfSpiDeviceBind: Register Dummy fail, ret is %d!", ret);
        return HDF_FAILURE;
    }

    return (SpiCntlrCreate(device) == NULL) ? HDF_FAILURE : HDF_SUCCESS;
}

static int32_t HdfSpiDeviceInit(struct HdfDeviceObject *device)
{
    int ret;
    struct SpiCntlr *cntlr = NULL;

    HDF_LOGI("HdfSpiDeviceInit: entry!");
    if (device == NULL) {
        HDF_LOGE("HdfSpiDeviceInit: device is null!");
        return HDF_ERR_INVALID_OBJECT;
    }
    cntlr = SpiCntlrFromDevice(device);
    if (cntlr == NULL) {
        HDF_LOGE("HdfSpiDeviceInit: cntlr is null!");
        return HDF_FAILURE;
    }

    ret = SpiCntlrInit(cntlr, device);
    if (ret != 0) {
        HDF_LOGE("HdfSpiDeviceInit: SpiCntlrInit error!");
        return HDF_FAILURE;
    }
    return ret;
}

static void HdfSpiDeviceRelease(struct HdfDeviceObject *device)
{
    struct SpiCntlr *cntlr = NULL;
    struct SpiDev *dev = NULL;
    struct SpiDev *tmpDev = NULL;

    HDF_LOGI("HdfSpiDeviceRelease: entry!");
    if (device == NULL) {
        HDF_LOGE("HdfSpiDeviceRelease: device is null!");
        return;
    }
    cntlr = SpiCntlrFromDevice(device);
    if (cntlr == NULL) {
        HDF_LOGE("HdfSpiDeviceRelease: cntlr is null!");
        return;
    }

    DLIST_FOR_EACH_ENTRY_SAFE(dev, tmpDev, &(cntlr->list), struct SpiDev, list) {
        DListRemove(&(dev->list));
        OsalMemFree(dev);
    }
    SpiCntlrDestroy(cntlr);
}

struct HdfDriverEntry g_hdfSpiDevice = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_SPI",
    .Bind = HdfSpiDeviceBind,
    .Init = HdfSpiDeviceInit,
    .Release = HdfSpiDeviceRelease,
};

HDF_INIT(g_hdfSpiDevice);
