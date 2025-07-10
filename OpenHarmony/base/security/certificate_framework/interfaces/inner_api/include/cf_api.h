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

#ifndef CF_API_H
#define CF_API_H

#include "cf_type.h"

typedef struct CfObjectInner CfObject;
struct CfObjectInner {
    int32_t (*get)(const CfObject *object, const CfParamSet *paramSetIn, CfParamSet **paramSetOut);
    int32_t (*check)(const CfObject *object, const CfParamSet *paramSetIn, CfParamSet **paramSetOut);
    void (*destroy)(CfObject **object);
};

#ifdef __cplusplus
extern "C" {
#endif

CF_API_EXPORT int32_t CfCreate(CfObjectType objType, const CfEncodingBlob *in, CfObject **object);

#ifdef __cplusplus
}
#endif

#endif /* CF_API_H */
