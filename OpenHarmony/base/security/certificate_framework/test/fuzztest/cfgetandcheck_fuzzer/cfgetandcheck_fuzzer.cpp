/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cfgetandcheck_fuzzer.h"

#include <securec.h>

#include "cf_api.h"
#include "cf_memory.h"
#include "cf_param.h"
#include "cf_result.h"
#include "cf_test_sdk_common.h"
#include "cf_test_data.h"

using namespace CertframeworkSdkTest;
using namespace CertframeworkTestData;

namespace OHOS {
    constexpr size_t PARAMS_SIZE_TWO = 2;
    constexpr size_t PARAMS_SIZE_THREE = 3;

    const static CfEncodingBlob g_cert[] = {
        { const_cast<uint8_t *>(g_certData01), sizeof(g_certData01), CF_FORMAT_DER },
    };

    const static CfEncodingBlob g_extensionBlob[] = {
        { const_cast<uint8_t *>(g_extensionData03), sizeof(g_extensionData03), CF_FORMAT_DER },
    };

    void TestCommonfunc(const CfObject *object, uint32_t cnt, const CfParam *params, uint32_t functype)
    {
        CfParamSet *inParamSet = nullptr;
        CfParamSet *outParamSet = nullptr;
        int32_t ret = TestConstructParamSetIn(params, cnt, &inParamSet);
        if (ret != CF_SUCCESS) {
            return;
        }

        if (functype == CF_TAG_GET_TYPE) {
            (void)object->get(object, inParamSet, &outParamSet);
        } else {
            (void)object->check(object, inParamSet, &outParamSet);
        }

        CfFreeParamSet(&outParamSet);
        CfFreeParamSet(&inParamSet);
    }

    void TestObjectTypeFunc1(const CfObject *object, uint8_t* data, size_t size)
    {
        size_t offset = 0;

        if (size < (sizeof(CfParam) * PARAMS_SIZE_THREE)) {
            return;
        }

        CfParam params[PARAMS_SIZE_THREE];
        for (int i = 0; i < PARAMS_SIZE_THREE; i++) {
            params[i].tag = *reinterpret_cast<uint32_t *>(data + offset);
            offset += sizeof(uint32_t);
            params[i].int32Param = *reinterpret_cast<int32_t *>(data + offset);
            offset += sizeof(int32_t);
        }

        TestCommonfunc(object, PARAMS_SIZE_THREE, params, CF_TAG_GET_TYPE);
        TestCommonfunc(object, PARAMS_SIZE_THREE, params, CF_TAG_CHECK_TYPE);
    }

    void TestObjectTypeFunc2(const CfObject *object, uint8_t* data, size_t size)
    {
        if (size < (sizeof(CfParam) * PARAMS_SIZE_TWO)) {
            return;
        }

        CfParam params[] = {
            { .tag = CF_TAG_GET_TYPE, .int32Param = CF_GET_TYPE_EXT_ITEM },
            { .tag = CF_TAG_PARAM0_INT32, .int32Param = *reinterpret_cast<int32_t *>(data) },
        };

        TestCommonfunc(object, sizeof(params) / sizeof(CfParam), params, CF_TAG_GET_TYPE);
    }

    void TestObjectTypeFunc3(const CfObject *object, uint8_t* data, size_t size)
    {
        if (size < (sizeof(CfParam) * PARAMS_SIZE_TWO)) {
            return;
        }

        CfParam params[] = {
            { .tag = CF_TAG_GET_TYPE, .int32Param = CF_GET_TYPE_EXT_OIDS },
            { .tag = CF_TAG_PARAM0_INT32, .int32Param = *reinterpret_cast<int32_t *>(data) },
        };

        TestCommonfunc(object, sizeof(params) / sizeof(CfParam), params, CF_TAG_GET_TYPE);
    }

    void TestObjectTypeFunc4(const CfObject *object, uint8_t* data, size_t size)
    {
        if (size < (sizeof(CfParam) * PARAMS_SIZE_THREE)) {
            return;
        }

        CfBlob oid = { size - sizeof(uint32_t), reinterpret_cast<uint8_t *>(data + sizeof(uint32_t)) };

        CfParam params[] = {
            { .tag = CF_TAG_GET_TYPE, .int32Param = CF_GET_TYPE_EXT_ENTRY },
            { .tag = CF_TAG_PARAM0_INT32, .int32Param = *reinterpret_cast<int32_t *>(data) },
            { .tag = CF_TAG_PARAM1_BUFFER, .blob = oid },
        };

        TestCommonfunc(object, sizeof(params) / sizeof(CfParam), params, CF_TAG_GET_TYPE);
    }

    void TestObjectTypeFunc5(const CfObject *object, uint8_t* data, size_t size)
    {
        if (size < (sizeof(CfParam) * PARAMS_SIZE_TWO)) {
            return;
        }

        CfParam params[] = {
            { .tag = CF_TAG_GET_TYPE, .int32Param = CF_GET_TYPE_CERT_ITEM },
            { .tag = CF_TAG_PARAM0_INT32, .int32Param = *reinterpret_cast<int32_t *>(data) },
        };

        TestCommonfunc(object, sizeof(params) / sizeof(CfParam), params, CF_TAG_GET_TYPE);
    }

    void TestObjectTypeFunc6(const CfObject *object, uint8_t* data, size_t size)
    {
        if (size < (sizeof(CfParam))) {
            return;
        }

        CfParam params[] = {
            { .tag = CF_TAG_CHECK_TYPE, .int32Param = *reinterpret_cast<int32_t *>(data) },
        };

        TestCommonfunc(object, sizeof(params) / sizeof(CfParam), params, CF_TAG_CHECK_TYPE);
    }

    bool CfObjectFuzzTest(const uint8_t* data, size_t size, CfObjectType objType)
    {
        uint8_t *tmpData = static_cast<uint8_t *>(CfMalloc(size, 0));
        if (tmpData == nullptr) {
            return false;
        }
        (void)memcpy_s(tmpData, size, data, size);

        CfObject *object = nullptr;
        if (objType == CF_OBJ_TYPE_EXTENSION) {
            int32_t ret = CfCreate(CF_OBJ_TYPE_EXTENSION, &g_extensionBlob[0], &object);
            if (ret != CF_SUCCESS) {
                CfFree(tmpData);
                if (object != nullptr) {
                    object->destroy(&object);
                }
                return false;
            }
        } else {
            int32_t ret = CfCreate(CF_OBJ_TYPE_CERT, &g_cert[0], &object);
            if (ret != CF_SUCCESS) {
                CfFree(tmpData);
                if (object != nullptr) {
                    object->destroy(&object);
                }
                return false;
            }
        }
        TestObjectTypeFunc1(object, tmpData, size);
        TestObjectTypeFunc2(object, tmpData, size);
        TestObjectTypeFunc3(object, tmpData, size);
        TestObjectTypeFunc4(object, tmpData, size);
        TestObjectTypeFunc5(object, tmpData, size);
        TestObjectTypeFunc6(object, tmpData, size);

        if (object != nullptr) {
            object->destroy(&object);
        }
        CfFree(tmpData);
        return true;
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::CfObjectFuzzTest(data, size, CF_OBJ_TYPE_EXTENSION);
    OHOS::CfObjectFuzzTest(data, size, CF_OBJ_TYPE_CERT);
    return 0;
}
