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

#include "cf_api.h"

#include "securec.h"

#include "cf_ability.h"
#include "cf_log.h"
#include "cf_magic.h"
#include "cf_memory.h"
#include "cf_object_ability_define.h"
#include "cf_result.h"
#include "cf_type.h"

typedef struct {
    CfObject object;
    CfObjectAbilityFunc func;
    CfBase *base;
} CfLifeCtx;

static int32_t CfLifeGet(const CfObject *object, const CfParamSet *in, CfParamSet **out)
{
    if ((object == NULL) || (in == NULL) || (out == NULL)) {
        CF_LOG_E("input params invalid");
        return CF_NULL_POINTER;
    }

    CfLifeCtx *tmp = (CfLifeCtx *)object;
    int32_t ret = tmp->func.get(tmp->base, in, out);
    return ret;
}

static int32_t CfLifeCheck(const CfObject *object, const CfParamSet *in, CfParamSet **out)
{
    if ((object == NULL) || (in == NULL) || (out == NULL)) {
        CF_LOG_E("input params invalid");
        return CF_NULL_POINTER;
    }

    CfLifeCtx *tmp = (CfLifeCtx *)object;
    int32_t ret = tmp->func.check(tmp->base, in, out);
    return ret;
}

static void CfLifeDestroy(CfObject **object)
{
    if ((object == NULL) || (*object == NULL)) {
        CF_LOG_I("param is null");
        return;
    }

    CfLifeCtx *tmp = (CfLifeCtx *)*object;
    tmp->func.destroy(&tmp->base);
    CfFree(tmp);
    *object = NULL;
}

CF_API_EXPORT int32_t CfCreate(CfObjectType objType, const CfEncodingBlob *in, CfObject **object)
{
    CF_LOG_I("enter: create object [%{public}d]", objType);
    if ((in == NULL) || (object == NULL)) {
        CF_LOG_E("input params invalid");
        return CF_NULL_POINTER;
    }

    CfObjectAbilityFunc *func = (CfObjectAbilityFunc *)GetAbility(CF_ABILITY(CF_ABILITY_TYPE_OBJECT, objType));
    if ((func == NULL) || (func->base.type != CF_MAGIC(CF_MAGIC_TYPE_OBJ_FUNC, objType))) {
        CF_LOG_E("invalid func type");
        return CF_INVALID_PARAMS;
    }

    CfLifeCtx *tmp = CfMalloc(sizeof(CfLifeCtx), 0);
    if (tmp == NULL) {
        CF_LOG_E("malloc ctx failed");
        return CF_ERR_MALLOC;
    }

    int32_t ret = func->create(in, &tmp->base);
    if (ret != CF_SUCCESS) {
        CF_LOG_E("create object resource failed, ret = %{public}d", ret);
        CfFree(tmp);
        return ret;
    }
    (void)memcpy_s(&tmp->func, sizeof(CfObjectAbilityFunc), func, sizeof(CfObjectAbilityFunc));

    tmp->object.get = CfLifeGet;
    tmp->object.check = CfLifeCheck;
    tmp->object.destroy = CfLifeDestroy;
    *object = &tmp->object;

    return CF_SUCCESS;
}

