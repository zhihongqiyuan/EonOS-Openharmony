/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */

#include "vibrator_parser.h"
#include <securec.h>
#include "device_resource_if.h"
#include "hdf_base.h"
#include "hdf_device_desc.h"

static int32_t ParseVibratorAttr(struct DeviceResourceIface *parser, const struct DeviceResourceNode *attrNode,
    struct VibratorCfgData *config)
{
    int32_t ret;
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(parser, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(attrNode, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(config, HDF_ERR_INVALID_PARAM);

    ret = parser->GetUint16(attrNode, "chipIdRegister", &config->vibratorAttr.chipIdReg, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "chipIdRegister");
    ret = parser->GetUint16(attrNode, "chipIdValue", &config->vibratorAttr.chipIdValue, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "chipIdValue");
    ret = parser->GetUint16(attrNode, "defaultIntensity", &config->vibratorAttr.defaultIntensity, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "defaultIntensity");
    ret = parser->GetUint16(attrNode, "defaultFrequency", &config->vibratorAttr.defaultFrequency, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "defaultFrequency");

    return ret;
}

static int32_t ParseVibratorInfo(struct DeviceResourceIface *parser, const struct DeviceResourceNode *infoNode,
    struct VibratorCfgData *config)
{
    int32_t ret;
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(parser, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(infoNode, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(config, HDF_ERR_INVALID_PARAM);

    ret = parser->GetBool(infoNode, "isSupportIntensity");
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "isSupportIntensity");
    ret = parser->GetBool(infoNode, "isSupportFrequency");
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "isSupportFrequency");
    ret = parser->GetUint16(infoNode, "intensityMaxValue", (uint16_t *)&config->vibratorInfo.intensityMaxValue, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "intensityMaxValue");
    ret = parser->GetUint16(infoNode, "intensityMinValue", (uint16_t *)&config->vibratorInfo.intensityMinValue, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "intensityMinValue");
    ret = parser->GetUint16(infoNode, "frequencyMaxValue", (uint16_t *)&config->vibratorInfo.frequencyMaxValue, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "frequencyMaxValue");
    ret = parser->GetUint16(infoNode, "frequencyMinValue", (uint16_t *)&config->vibratorInfo.frequencyMinValue, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "frequencyMinValue");

    return ret;
}


static int32_t ParseVibratorBus(struct DeviceResourceIface *parser, const struct DeviceResourceNode *busNode,
    struct VibratorCfgData *config)
{
    int32_t ret;
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(parser, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(busNode, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(config, HDF_ERR_INVALID_PARAM);

    ret = parser->GetUint8(busNode, "busType", &config->vibratorBus.busType, 0);
    CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "busType");

    if (config->vibratorBus.busType == VIBRATOR_BUS_I2C) {
        ret = parser->GetUint16(busNode, "busNum", &config->vibratorBus.i2cCfg.busNum, 0);
        CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "busNum");
        ret = parser->GetUint16(busNode, "busAddr", &config->vibratorBus.i2cCfg.devAddr, 0);
        CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "busAddr");
        ret = parser->GetUint16(busNode, "regWidth", &config->vibratorBus.i2cCfg.regWidth, 0);
        CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "regWidth");
    } else if (config->vibratorBus.busType == VIBRATOR_BUS_GPIO) {
        ret = parser->GetUint32(busNode, "busNum", &config->vibratorBus.GpioNum, 0);
        CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "busNum");
    }

    return ret;
}

int32_t GetVibratorBaseConfigData(const struct DeviceResourceNode *node, struct VibratorCfgData *config)
{
    int32_t ret;
    struct DeviceResourceIface *parser = NULL;
    const struct DeviceResourceNode *infoNode = NULL;
    const struct DeviceResourceNode *busNode = NULL;
    const struct DeviceResourceNode *attrNode = NULL;

    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(node, HDF_ERR_INVALID_PARAM);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(config, HDF_ERR_INVALID_PARAM);

    parser = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);
    CHECK_VIBRATOR_NULL_PTR_RETURN_VALUE(parser, HDF_ERR_INVALID_PARAM);

    busNode = parser->GetChildNode(node, "VibratorBusConfig");
    if (busNode != NULL) {
        ret = ParseVibratorBus(parser, busNode, config);
        CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "VibratorBusConfig");
    }

    if (config->vibratorBus.busType == VIBRATOR_BUS_I2C) {
        attrNode = parser->GetChildNode(node, "VibratorAttr");
        if (attrNode != NULL) {
            ret = ParseVibratorAttr(parser, attrNode, config);
            CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "VibratorAttr");
        }
    }

    infoNode = parser->GetChildNode(node, "VibratorChipConfig");
    if (infoNode != NULL) {
        ret = ParseVibratorInfo(parser, infoNode, config);
        CHECK_VIBRATOR_PARSER_RESULT_RETURN_VALUE(ret, "VibratorChipConfig");
    }

    return ret;
}