/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 *
 * HDF is dual licensed: you can use it either under the terms of
 * the GPL, or the BSD license, at your option.
 * See the LICENSE file in the root of this repository for complete details.
 */

#ifndef VIBRATOR_PARSER_H
#define VIBRATOR_PARSER_H

#include "device_resource_if.h"
#include "hdf_base.h"
#include "hdf_device_desc.h"
#include "vibrator_driver_type.h"

int32_t GetVibratorBaseConfigData(const struct DeviceResourceNode *node, struct VibratorCfgData *config);

#endif /* VIBRATOR_PARSER_H */