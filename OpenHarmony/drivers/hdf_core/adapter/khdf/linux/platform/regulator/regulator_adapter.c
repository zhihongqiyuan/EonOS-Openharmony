/*
 * regulator driver adapter of linux
 *
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
#include "regulator_adapter.h"
#include "regulator_adapter_consumer.h"
#include "regulator_core.h"
#include <linux/regulator/consumer.h>
#include "device_resource_if.h"
#include "hdf_device_desc.h"
#include "hdf_core_log.h"
#include "osal_mem.h"

#define HDF_LOG_TAG regulator_linux_adapter
static int32_t LinuxRegulatorOpen(struct RegulatorNode *node)
{
    if (node == NULL || node->priv == NULL) {
        HDF_LOGE("LinuxRegulatorOpen: node or priv is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        const char *devname = dev_name(info->dev);
        if ((devname == NULL) || (strcmp(devname, info->devName) != 0)) {
            HDF_LOGE("LinuxRegulatorOpen: dev info error [%s][%s]!", devname, info->devName);
            return HDF_FAILURE;
        }

        info->adapterReg = regulator_get(info->dev, info->supplyName);
        if (IS_ERR(info->adapterReg)) {
            HDF_LOGE("LinuxRegulatorOpen: regulator_get [%s][%s] ERROR!", devname, info->supplyName);
            info->adapterReg = NULL;
            return HDF_FAILURE;
        }
        if (info->adapterReg == NULL) {
            HDF_LOGE("LinuxRegulatorOpen: regulator_get [%s][%s]!", devname, info->supplyName);
            return HDF_FAILURE;
        }
    }

    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorClose(struct RegulatorNode *node)
{
    if (node == NULL || node->priv == NULL) {
        HDF_LOGE("LinuxRegulatorClose: node or priv is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg != NULL) {
        if (regulator_disable(info->adapterReg) != HDF_SUCCESS) {
            HDF_LOGE("LinuxRegulatorClose: regulator_disable[%s][%s] FAIL", node->regulatorInfo.name,
                info->supplyName);
        }
        regulator_put(info->adapterReg);
        info->adapterReg = NULL;
    }
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorRemove(struct RegulatorNode *node)
{
    if (node == NULL || node->priv == NULL) {
        HDF_LOGE("LinuxRegulatorRemove: node or priv is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    HDF_LOGI("LinuxRegulatorRemove: regulator [%s][%s] release!", info->devName, info->supplyName);
    if (LinuxRegulatorClose(node) != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorRemove: LinuxRegulatorClose fail[%s][%s]!", info->devName, info->supplyName);
    }

    OsalMemFree(info);
    node->priv = NULL;
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorEnable(struct RegulatorNode *node)
{
    if (node == NULL || node->priv == NULL) {
        HDF_LOGE("LinuxRegulatorEnable: node or priv is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        HDF_LOGE("LinuxRegulatorEnable: adapterReg null, please open dev!");
        return HDF_ERR_INVALID_OBJECT;
    }
    int ret = regulator_enable(info->adapterReg);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorEnable: [%s][%s][%d] fail!", node->regulatorInfo.name, info->supplyName, ret);
        return HDF_FAILURE;
    }
    if (regulator_is_enabled(info->adapterReg) > 0) {
        node->regulatorInfo.status = REGULATOR_STATUS_ON;
    } else {
        node->regulatorInfo.status = REGULATOR_STATUS_OFF;
    }
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorDisable(struct RegulatorNode *node)
{
    if (node == NULL || node->priv == NULL) {
        HDF_LOGE("LinuxRegulatorDisable: node or priv is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        HDF_LOGE("LinuxRegulatorDisable: adapterReg null, please open dev!");
        return HDF_ERR_INVALID_OBJECT;
    }
    if (regulator_disable(info->adapterReg) != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorDisable: [%s][%s] fail!", node->regulatorInfo.name, info->supplyName);
        return HDF_FAILURE;
    }
    // node maybe alwayson
    if (regulator_is_enabled(info->adapterReg) > 0) {
        node->regulatorInfo.status = REGULATOR_STATUS_ON;
    } else {
        node->regulatorInfo.status = REGULATOR_STATUS_OFF;
    }
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorForceDisable(struct RegulatorNode *node)
{
    if (node == NULL || node->priv == NULL) {
        HDF_LOGE("LinuxRegulatorForceDisable: node or priv is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        HDF_LOGE("LinuxRegulatorForceDisable: adapterReg is null, please open dev!");
        return HDF_ERR_INVALID_OBJECT;
    }
    if (regulator_force_disable(info->adapterReg) != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorForceDisable: regulator_force_disable[%s][%s] fail!", node->regulatorInfo.name,
            info->supplyName);
        return HDF_FAILURE;
    }
    node->regulatorInfo.status = REGULATOR_STATUS_OFF;
    HDF_LOGI("LinuxRegulatorForceDisable: [%s][%s] success!", info->devName, info->supplyName);
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorSetVoltage(struct RegulatorNode *node, uint32_t minUv, uint32_t maxUv)
{
    if (node == NULL || node->priv == NULL) {
        HDF_LOGE("LinuxRegulatorSetVoltage: node or priv is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        HDF_LOGE("LinuxRegulatorSetVoltage: adapterReg null, please open dev!");
        return HDF_ERR_INVALID_OBJECT;
    }
    if (regulator_set_voltage(info->adapterReg, minUv, maxUv) != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorSetVoltage: [%s][%s] fail!", node->regulatorInfo.name, info->supplyName);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorGetVoltage(struct RegulatorNode *node, uint32_t *voltage)
{
    if (node == NULL || node->priv == NULL || voltage == NULL) {
        HDF_LOGE("LinuxRegulatorGetVoltage: node or priv or voltage is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        HDF_LOGE("LinuxRegulatorGetVoltage: adapterReg null, please open dev!");
        return HDF_ERR_INVALID_OBJECT;
    }
    int ret = regulator_get_voltage(info->adapterReg);
    if (ret < 0) {
        HDF_LOGE("LinuxRegulatorGetVoltage: [%s] fail!", node->regulatorInfo.name);
        return HDF_FAILURE;
    }

    *voltage = ret;
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorSetCurrent(struct RegulatorNode *node, uint32_t minUa, uint32_t maxUa)
{
    if (node == NULL) {
        HDF_LOGE("LinuxRegulatorSetCurrent: node is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        HDF_LOGE("LinuxRegulatorSetCurrent: adapterReg null, please open dev!");
        return HDF_ERR_INVALID_OBJECT;
    }
    if (regulator_set_current_limit(info->adapterReg, minUa, maxUa) != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorSetCurrent: [%s][%s] fail!", node->regulatorInfo.name, info->supplyName);
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorGetCurrent(struct RegulatorNode *node, uint32_t *regCurrent)
{
    if (node == NULL || regCurrent == NULL) {
        HDF_LOGE("LinuxRegulatorGetCurrent: node or regCurrent is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        HDF_LOGE("LinuxRegulatorGetCurrent: adapterReg null, please open dev!");
        return HDF_ERR_INVALID_OBJECT;
    }
    int ret = regulator_get_current_limit(info->adapterReg);
    if (ret < 0) {
        HDF_LOGE("LinuxRegulatorGetCurrent: [%s] fail!", node->regulatorInfo.name);
        return HDF_FAILURE;
    }

    *regCurrent = ret;
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorGetStatus(struct RegulatorNode *node, uint32_t *status)
{
    if (node == NULL || status == NULL) {
        HDF_LOGE("LinuxRegulatorGetStatus: node or status is null!");
        return HDF_ERR_INVALID_OBJECT;
    }
    struct LinuxRegulatorInfo *info = (struct LinuxRegulatorInfo *)node->priv;
    if (info->adapterReg == NULL) {
        HDF_LOGE("LinuxRegulatorGetStatus: adapterReg null, please open dev!");
        return HDF_ERR_INVALID_OBJECT;
    }

    if (regulator_is_enabled(info->adapterReg) > 0) {
        *status = REGULATOR_STATUS_ON;
    } else {
        *status = REGULATOR_STATUS_OFF;
    }
    return HDF_SUCCESS;
}

static struct RegulatorMethod g_method = {
    .open = LinuxRegulatorOpen,
    .close = LinuxRegulatorClose,
    .release = LinuxRegulatorRemove,
    .enable = LinuxRegulatorEnable,
    .disable = LinuxRegulatorDisable,
    .forceDisable = LinuxRegulatorForceDisable,
    .setVoltage = LinuxRegulatorSetVoltage,
    .getVoltage = LinuxRegulatorGetVoltage,
    .setCurrent = LinuxRegulatorSetCurrent,
    .getCurrent = LinuxRegulatorGetCurrent,
    .getStatus = LinuxRegulatorGetStatus,
};

static struct device *g_consumer_dev;
int32_t LinuxRegulatorSetConsumerDev(struct device *dev)
{
    if (dev == NULL) {
        HDF_LOGE("LinuxRegulatorSetConsumerDev: node is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    g_consumer_dev = dev;
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorBind(struct HdfDeviceObject *device)
{
    (void)device;
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorReadHcs(struct RegulatorNode *regNode, const struct DeviceResourceNode *node)
{
    int32_t ret;
    struct DeviceResourceIface *drsOps = NULL;

    drsOps = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (drsOps == NULL || drsOps->GetString == NULL) {
        HDF_LOGE("LinuxRegulatorReadHcs: invalid drs ops fail!");
        return HDF_FAILURE;
    }

    ret = drsOps->GetString(node, "name", &(regNode->regulatorInfo.name), "ERROR");
    if ((ret != HDF_SUCCESS) || (regNode->regulatorInfo.name == NULL)) {
        HDF_LOGE("LinuxRegulatorReadHcs: read name fail!");
        return HDF_FAILURE;
    }
    HDF_LOGD("LinuxRegulatorReadHcs: name[%s]", regNode->regulatorInfo.name);

    ret = drsOps->GetUint8(node, "mode", &regNode->regulatorInfo.constraints.mode, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorReadHcs: read mode fail!");
        return HDF_FAILURE;
    }

    ret = drsOps->GetUint32(node, "minUv", &regNode->regulatorInfo.constraints.minUv, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorReadHcs: read minUv fail!");
        return HDF_FAILURE;
    }

    ret = drsOps->GetUint32(node, "maxUv", &regNode->regulatorInfo.constraints.maxUv, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorReadHcs: read maxUv fail!");
        return HDF_FAILURE;
    }

    ret = drsOps->GetUint32(node, "minUa", &regNode->regulatorInfo.constraints.minUa, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorReadHcs: read minUa fail!");
        return HDF_FAILURE;
    }

    ret = drsOps->GetUint32(node, "maxUa", &regNode->regulatorInfo.constraints.maxUa, 0);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorReadHcs: read maxUa fail!");
        return HDF_FAILURE;
    }

    regNode->regulatorInfo.parentName = NULL;
    regNode->regulatorInfo.status = REGULATOR_STATUS_OFF;

    HDF_LOGI("LinuxRegulatorReadHcs: name[%s][%d]--[%d][%d]--[%d][%d]",
        regNode->regulatorInfo.name, regNode->regulatorInfo.constraints.mode,
        regNode->regulatorInfo.constraints.minUv, regNode->regulatorInfo.constraints.maxUv,
        regNode->regulatorInfo.constraints.minUa, regNode->regulatorInfo.constraints.maxUa);

    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorReadAdapterHcs(struct LinuxRegulatorInfo *info, const struct DeviceResourceNode *node)
{
    int32_t ret;
    struct DeviceResourceIface *drsOps = NULL;

    drsOps = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (info == NULL || drsOps == NULL || drsOps->GetString == NULL) {
        HDF_LOGE("LinuxRegulatorReadAdapterHcs: invalid drs ops fail!");
        return HDF_FAILURE;
    }

    ret = drsOps->GetString(node, "devName", &(info->devName), "ERROR");
    if ((ret != HDF_SUCCESS) || (info->devName == NULL)) {
        HDF_LOGE("LinuxRegulatorReadAdapterHcs: read devName fail!");
        return HDF_FAILURE;
    }
    HDF_LOGI("LinuxRegulatorReadAdapterHcs:devName[%s]", info->devName);

    ret = drsOps->GetString(node, "supplyName", &(info->supplyName), "ERROR");
    if ((ret != HDF_SUCCESS) || (info->supplyName == NULL)) {
        HDF_LOGE("LinuxRegulatorReadAdapterHcs: read supplyName fail!");
        return HDF_FAILURE;
    }
    HDF_LOGI("LinuxRegulatorReadAdapterHcs: supplyName[%s]", info->supplyName);

    info->dev = g_consumer_dev;
    return HDF_SUCCESS;
}

static int32_t LinuxRegulatorParseAndInit(struct HdfDeviceObject *device, const struct DeviceResourceNode *node)
{
    int32_t ret;
    struct RegulatorNode *regNode = NULL;
    struct LinuxRegulatorInfo *info = NULL;
    (void)device;

    regNode = (struct RegulatorNode *)OsalMemCalloc(sizeof(*regNode));
    if (regNode == NULL) {
        HDF_LOGE("LinuxRegulatorParseAndInit: malloc regNode fail!");
        return HDF_ERR_MALLOC_FAIL;
    }

    info = (struct LinuxRegulatorInfo *)OsalMemCalloc(sizeof(*info));
    if (info == NULL) {
        HDF_LOGE("LinuxRegulatorParseAndInit: malloc info fail!");
        OsalMemFree(regNode);
        regNode = NULL;
        return HDF_ERR_MALLOC_FAIL;
    }

    do {
        ret = LinuxRegulatorReadHcs(regNode, node);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("LinuxRegulatorParseAndInit: read drs fail! ret:%d", ret);
            break;
        }

        ret = LinuxRegulatorReadAdapterHcs(info, node);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("LinuxRegulatorParseAndInit: LinuxRegulatorReadAdapterHcs fail! ret:%d", ret);
            break;
        }

        HDF_LOGI("LinuxRegulatorParseAndInit: name[%s][%d]--[%d][%d]--[%d][%d]--[%s][%s]",
            regNode->regulatorInfo.name, regNode->regulatorInfo.constraints.mode,
            regNode->regulatorInfo.constraints.minUv, regNode->regulatorInfo.constraints.maxUv,
            regNode->regulatorInfo.constraints.minUa, regNode->regulatorInfo.constraints.maxUa,
            info->devName, info->supplyName);

        regNode->priv = (void *)info;
        regNode->ops = &g_method;

        ret = RegulatorNodeAdd(regNode);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("LinuxRegulatorParseAndInit: add regulator controller fail, ret: %d!", ret);
            break;
        }
    } while (0);

    if (ret != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorParseAndInit: fail [%d]!", ret);
        OsalMemFree(regNode);
        regNode = NULL;
        OsalMemFree(info);
        info = NULL;
        return HDF_FAILURE;
    }
    return HDF_SUCCESS;
}

/* get all linux regulator, then add to hdf */
static int32_t LinuxRegulatorInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    const struct DeviceResourceNode *childNode = NULL;
    RegulatorAdapterConsumerInit();

    if (device == NULL || device->property == NULL) {
        HDF_LOGE("LinuxRegulatorInit: device is null!");
        return HDF_ERR_INVALID_OBJECT;
    }

    DEV_RES_NODE_FOR_EACH_CHILD_NODE(device->property, childNode) {
        ret = LinuxRegulatorParseAndInit(device, childNode);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("LinuxRegulatorInit: LinuxRegulatorParseAndInit fail!");
            return HDF_FAILURE;
        }
    }
    return ret;
}
static int32_t LinuxRegulatorParseAndRelease(struct HdfDeviceObject *device, const struct DeviceResourceNode *node)
{
    int32_t ret;
    struct DeviceResourceIface *drsOps = NULL;
    (void)device;

    HDF_LOGI("LinuxRegulatorParseAndRelease: enter!");

    drsOps = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    if (drsOps == NULL || drsOps->GetString == NULL) {
        HDF_LOGE("LinuxRegulatorParseAndRelease: invalid drs ops fail!");
        return HDF_FAILURE;
    }

    const char *name = NULL;
    ret = drsOps->GetString(node, "name", &(name), "ERROR");
    if ((ret != HDF_SUCCESS) || (name == NULL)) {
        HDF_LOGE("LinuxRegulatorParseAndRelease: read name fail!");
        return HDF_FAILURE;
    }

    HDF_LOGI("LinuxRegulatorParseAndRelease: name[%s]", name);

    ret = RegulatorNodeRemove(name);
    if (ret != HDF_SUCCESS) {
        HDF_LOGE("LinuxRegulatorParseAndRelease: LinuxRegulatorRelease fail, ret:%d!", ret);
        return HDF_FAILURE;
    }
    HDF_LOGI("LinuxRegulatorParseAndRelease: name[%s] success!", name);
    return HDF_SUCCESS;
}

static void LinuxRegulatorRelease(struct HdfDeviceObject *device)
{
    HDF_LOGI("LinuxRegulatorRelease: enter!");
    if (device == NULL || device->property == NULL) {
        HDF_LOGE("LinuxRegulatorRelease: device is null!");
        return;
    }
    const struct DeviceResourceNode *childNode = NULL;

    DEV_RES_NODE_FOR_EACH_CHILD_NODE(device->property, childNode) {
        int ret = LinuxRegulatorParseAndRelease(device, childNode);
        if (ret != HDF_SUCCESS) {
            HDF_LOGE("LinuxRegulatorRelease: LinuxRegulatorParseAndInit fail!");
        }
    }
}

struct HdfDriverEntry g_regulatorLinuxDriverEntry = {
    .moduleVersion = 1,
    .Bind = LinuxRegulatorBind,
    .Init = LinuxRegulatorInit,
    .Release = LinuxRegulatorRelease,
    .moduleName = "linux_regulator_adapter",
};
HDF_INIT(g_regulatorLinuxDriverEntry);
