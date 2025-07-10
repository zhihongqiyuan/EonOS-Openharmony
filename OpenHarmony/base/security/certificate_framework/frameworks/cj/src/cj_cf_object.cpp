/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "cf_param.h"

#include "cj_cf_object.h"

int32_t FfiCertCjCfObjectNewInstance(const CfEncodingBlob *blob, CjCfObject *returnObj)
{
    auto cfObj = static_cast<CfObject *>(malloc(sizeof(CfObject)));
    if (cfObj == nullptr) {
        return CF_ERR_MALLOC;
    }
    const auto errCode = CfResult(CfCreate(CF_OBJ_TYPE_EXTENSION, blob, &cfObj));
    if (errCode != CF_SUCCESS) {
        free(cfObj);
        return errCode;
    }
    returnObj->cfObj = cfObj;
    return CF_SUCCESS;
}

void FfiCertCjCfObjectDeleteInstance(CjCfObject self)
{
    self.cfObj->destroy(&self.cfObj);
}

CfResult FfiCertCjCfObjectGetEncoded(const CjCfObject self, CfBlob *out)
{
    CfParamSet *inParamSet = nullptr;
    int32_t ret;
    if ((ret = CfInitParamSet(&inParamSet)) != CF_SUCCESS) {
        return CfResult(ret);
    }

    const CfParam param[] = {
        CfParam{.tag = CF_TAG_GET_TYPE, .int32Param = CF_GET_TYPE_EXT_ITEM},
        CfParam{.tag = CF_TAG_PARAM0_INT32, .int32Param = CF_ITEM_ENCODED},
    };
    if ((ret = CfAddParams(inParamSet, param, sizeof(param) / sizeof(CfParam))) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    CfParamSet *outParamSet = nullptr;
    if ((ret = self.cfObj->get(self.cfObj, inParamSet, &outParamSet)) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    CfParam *resultParam = nullptr;
    ret = CfGetParam(outParamSet, CF_TAG_RESULT_BYTES, &resultParam);
    CfFreeParamSet(&inParamSet);
    CfFreeParamSet(&outParamSet);

    if (ret == CF_SUCCESS) {
        *out = resultParam->blob;
    }
    return CfResult(ret);
}

CfResult FfiCertCjCfObjectGetOidList(const CjCfObject self, int32_t valueType, CfArray *out)
{
    CfParamSet *inParamSet = nullptr;
    int32_t ret;
    if ((ret = CfInitParamSet(&inParamSet)) != CF_SUCCESS) {
        return CfResult(ret);
    }

    const CfParam param[] = {
        CfParam{.tag = CF_TAG_GET_TYPE, .int32Param = CF_GET_TYPE_EXT_OIDS},
        CfParam{.tag = CF_TAG_PARAM0_INT32, .int32Param = valueType},
    };
    if ((ret = CfAddParams(inParamSet, param, sizeof(param) / sizeof(CfParam))) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    CfParamSet *outParamSet = nullptr;
    if ((ret = self.cfObj->get(self.cfObj, inParamSet, &outParamSet)) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    if (outParamSet->paramSetSize <= 1) {
        ret = CF_INVALID_PARAMS;
    } else {
        out->count = outParamSet->paramsCnt - 1;
        out->format = CF_FORMAT_DER;
        out->data = static_cast<CfBlob *>(malloc(sizeof(CfBlob) * out->count));
        if (out->data == nullptr) {
            CfFreeParamSet(&inParamSet);
            CfFreeParamSet(&outParamSet);
            return CF_ERR_MALLOC;
        }
        for (uint32_t i = 0; i < out->count; ++i) {
            out->data[i] = outParamSet->params[i + 1].blob;
        }
        ret = CF_SUCCESS;
    }

    CfFreeParamSet(&inParamSet);
    CfFreeParamSet(&outParamSet);
    return CfResult(ret);
}

CfResult FfiCertCjCfObjectGetEntry(const CjCfObject self, int32_t valueType, CfBlob *oid, CfBlob *out)
{
    CfParamSet *inParamSet = nullptr;
    int32_t ret;
    if ((ret = CfInitParamSet(&inParamSet)) != CF_SUCCESS) {
        return CfResult(ret);
    }

    const CfParam param[] = {
        CfParam{.tag = CF_TAG_GET_TYPE, .int32Param = CF_GET_TYPE_EXT_ENTRY},
        CfParam{.tag = CF_TAG_PARAM0_INT32, .int32Param = valueType},
        CfParam{.tag = CF_TAG_PARAM1_BUFFER, .blob = *oid},
    };
    if ((ret = CfAddParams(inParamSet, param, sizeof(param) / sizeof(CfParam))) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    CfParamSet *outParamSet = nullptr;
    if ((ret = self.cfObj->get(self.cfObj, inParamSet, &outParamSet)) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }
    CfParam *resultParam = nullptr;
    ret = CfGetParam(outParamSet, CF_TAG_RESULT_BYTES, &resultParam);
    CfFreeParamSet(&inParamSet);
    CfFreeParamSet(&outParamSet);

    if (ret == CF_SUCCESS) {
        *out = resultParam->blob;
    }
    return CfResult(ret);
}

CfResult FfiCertCjCfObjectCheckCA(const CjCfObject self, int32_t *out)
{
    CfParamSet *inParamSet = nullptr;
    int32_t ret;
    if ((ret = CfInitParamSet(&inParamSet)) != CF_SUCCESS) {
        return CfResult(ret);
    }

    const CfParam param[] = {
        CfParam{.tag = CF_TAG_CHECK_TYPE, .int32Param = CF_CHECK_TYPE_EXT_CA},
    };
    if ((ret = CfAddParams(inParamSet, param, sizeof(param) / sizeof(CfParam))) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    CfParamSet *outParamSet = nullptr;
    if ((ret = self.cfObj->check(self.cfObj, inParamSet, &outParamSet)) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }
    CfParam *resultParam = nullptr;
    ret = CfGetParam(outParamSet, CF_TAG_RESULT_INT, &resultParam);
    CfFreeParamSet(&inParamSet);
    CfFreeParamSet(&outParamSet);

    if (ret == CF_SUCCESS) {
        *out = resultParam->int32Param;
    }
    return CfResult(ret);
}

CfResult FfiCertCjCfObjectHasUnsupportedCriticalExtension(const CjCfObject self, bool *out)
{
    CfParamSet *inParamSet = nullptr;
    int32_t ret;
    if ((ret = CfInitParamSet(&inParamSet)) != CF_SUCCESS) {
        return CfResult(ret);
    }

    const CfParam param[] = {
        CfParam{.tag = CF_TAG_CHECK_TYPE, .int32Param = CF_CHECK_TYPE_EXT_HAS_UN_SUPPORT},
    };
    if ((ret = CfAddParams(inParamSet, param, sizeof(param) / sizeof(CfParam))) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    CfParamSet *outParamSet = nullptr;
    if ((ret = self.cfObj->check(self.cfObj, inParamSet, &outParamSet)) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }
    CfParam *resultParam = nullptr;
    ret = CfGetParam(outParamSet, CF_TAG_RESULT_BOOL, &resultParam);
    CfFreeParamSet(&inParamSet);
    CfFreeParamSet(&outParamSet);

    if (ret == CF_SUCCESS) {
        *out = resultParam->boolParam;
    }
    return CfResult(ret);
}
