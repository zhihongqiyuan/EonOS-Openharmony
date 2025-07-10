/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CF_PARAM_H
#define CF_PARAM_H

#include "cf_type.h"

#define CF_PARAM_SET_MAX_SIZE (4 * 1024 * 1024)
#define CF_DEFAULT_PARAM_SET_SIZE 512
#define CF_DEFAULT_PARAM_CNT ((uint32_t)((CF_DEFAULT_PARAM_SET_SIZE - sizeof(CfParamSet)) / sizeof(CfParam)))
#define CF_TAG_TYPE_MASK (0xF << 28)

#ifdef __cplusplus
extern "C" {
#endif

CfTagType CfGetTagType(CfTag tag);

int32_t CfInitParamSet(CfParamSet **paramSet);

int32_t CfAddParams(CfParamSet *paramSet, const CfParam *params, uint32_t paramCnt);

int32_t CfBuildParamSet(CfParamSet **paramSet);

void CfFreeParamSet(CfParamSet **paramSet);

int32_t CfGetParam(const CfParamSet *paramSet, uint32_t tag, CfParam **param);

#ifdef __cplusplus
}
#endif

#endif /* CF_PARAM_H */
