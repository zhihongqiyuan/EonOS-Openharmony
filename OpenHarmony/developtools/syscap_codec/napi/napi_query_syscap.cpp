/*
 * Copyright (C) 2022-2022 Huawei Device Co., Ltd.
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

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <securec.h>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "syscap_interface.h"
#include "context_tool.h"

namespace OHOS {
EXTERN_C_START
constexpr size_t OS_SYSCAP_U32_NUM = 30;
constexpr size_t PCID_MAIN_U32 = OS_SYSCAP_U32_NUM + 2;
constexpr size_t U32_TO_STR_MAX_LEN = 11;
constexpr size_t KEY_BUFFER_SIZE = 32;

#define GET_PARAMS(env, info, num) \
    size_t argc = num;             \
    napi_value argv[num] = {0};    \
    napi_value thisVar = nullptr;  \
    void *data;                    \
    napi_get_cb_info(env, info, &argc, argv, &thisVar, &data)

// Async Function Set
struct SystemCapabilityAsyncContext {
    napi_env env = nullptr;
    napi_async_work work = nullptr;
    char key[KEY_BUFFER_SIZE] = { 0 };
    size_t keyLen = 0;
    char *value = nullptr;
    size_t valueLen = 0;
    napi_deferred deferred = nullptr;
    napi_ref callbackRef = nullptr;

    int status = 0;
};

static char* CalculateAllStringLength(char osCapArray[PCID_MAIN_U32][U32_TO_STR_MAX_LEN],
    char (*priCapArray)[SINGLE_SYSCAP_LEN], bool retBool, int priCapArrayCnt)
{
    errno_t err = EOK;
    char *temp = nullptr;
    int retError;
    int sumLen = 0;
    char *allSyscapBuffer = nullptr;

    if (!retBool) {
        PRINT_ERR("get encoded private syscap failed.");
        return allSyscapBuffer;
    }

    for (size_t i = 0; i < PCID_MAIN_U32; i++) {
        sumLen += strlen(osCapArray[i]);
    }
    for (int i = 0; i < priCapArrayCnt; i++) {
        sumLen += strlen(*(priCapArray + i));
    }
    sumLen += (PCID_MAIN_U32 + priCapArrayCnt + 1);  // split with ','

    // splicing string
    allSyscapBuffer = (char *)malloc(sumLen);
    if (allSyscapBuffer == nullptr) {
        PRINT_ERR("malloc failed!");
        return allSyscapBuffer;
    }
    err = memset_s(allSyscapBuffer, sumLen, 0, sumLen);
    if (err != EOK) {
        PRINT_ERR("memset failed!");
        free(allSyscapBuffer);
        return nullptr;
    }
    temp = *osCapArray;

    for (size_t i = 1; i < PCID_MAIN_U32; i++) {
        retError = sprintf_s(allSyscapBuffer, sumLen, "%s,%s", temp, osCapArray[i]);
        if (retError == -1) {
            PRINT_ERR("splicing os syscap string failed.");
            free(allSyscapBuffer);
            return nullptr;
        }
        temp = allSyscapBuffer;
    }
    for (int i = 0; i < priCapArrayCnt; i++) {
        retError = sprintf_s(allSyscapBuffer, sumLen, "%s,%s", temp, *(priCapArray + i));
        if (retError == -1) {
            PRINT_ERR("splicing pri syscap string failed.");
            free(allSyscapBuffer);
            return nullptr;
        }
        temp = allSyscapBuffer;
    }
    return allSyscapBuffer;
}

static char* GetSystemCapability()
{
    bool retBool;
    int retError, priOutputLen, priCapArrayCnt;
    char osOutput[SINGLE_SYSCAP_LEN] = {};

    uint32_t *osCapU32 = nullptr;
    char *priOutput = nullptr;

    char *allSyscapBuffer = nullptr;
    char osCapArray[PCID_MAIN_U32][U32_TO_STR_MAX_LEN] = {};
    char (*priCapArray)[SINGLE_SYSCAP_LEN] = nullptr;

    retBool = EncodeOsSyscap(osOutput, PCID_MAIN_BYTES);
    if (!retBool) {
        PRINT_ERR("get encoded os syscap failed.");
        return nullptr;
    }
    retBool = EncodePrivateSyscap(&priOutput, &priOutputLen);
    if (!retBool) {
        PRINT_ERR("get encoded private syscap failed.");
        goto FREE_PRIOUTPUT;
    }

    osCapU32 = reinterpret_cast<uint32_t *>(osOutput);
    for (size_t i = 0; i < PCID_MAIN_U32; i++) { // 2, header of pcid.sc
        retError = sprintf_s(osCapArray[i], U32_TO_STR_MAX_LEN, "%u", osCapU32[i]);
        if (retError == -1) {
            PRINT_ERR("get uint32_t syscap string failed.");
            goto FREE_PRIOUTPUT;
        }
    }
    retBool = DecodePrivateSyscap(priOutput, &priCapArray, &priCapArrayCnt);
    allSyscapBuffer = CalculateAllStringLength(osCapArray, priCapArray, retBool, priCapArrayCnt);
    free(priCapArray);

FREE_PRIOUTPUT:
    free(priOutput);

    return allSyscapBuffer;
}

napi_value PreHandleSystemCapability(
    napi_env env, napi_callback_info info, SystemCapabilityAsyncContext *asyncContext)
{
    GET_PARAMS(env, info, 1);
    NAPI_ASSERT(env, argc <= 1, "too many parameters");
    napi_value result = nullptr;

    asyncContext->env = env;

    napi_valuetype valueType = napi_undefined;
    if (argc == 1) {
        napi_typeof(env, argv[0], &valueType);
    }
    if (valueType == napi_function) {
        napi_create_reference(env, argv[0], 1, &asyncContext->callbackRef);
    }

    if (asyncContext->callbackRef == nullptr) {
        napi_create_promise(env, &asyncContext->deferred, &result);
    } else {
        napi_get_undefined(env, &result);
    }
    return result;
}

napi_value QuerySystemCapability(napi_env env, napi_callback_info info)
{
    SystemCapabilityAsyncContext *asyncContext = new SystemCapabilityAsyncContext();
    napi_value result = PreHandleSystemCapability(env, info, asyncContext);
    napi_value resource = nullptr;
    napi_create_string_utf8(env, "napi_value QuerySystemCapability", NAPI_AUTO_LENGTH, &resource);

    napi_create_async_work(
        env, nullptr, resource,
        [](napi_env env, void* data) {
            SystemCapabilityAsyncContext *asyncContext = (SystemCapabilityAsyncContext *)data;
            char *syscapStr = GetSystemCapability();
            if (syscapStr != nullptr) {
                asyncContext->value = syscapStr;
                asyncContext->status = 0;
            } else {
                asyncContext->status = 1;
            }
        },
        [](napi_env env, napi_status status, void* data) {
            SystemCapabilityAsyncContext *asyncContext = (SystemCapabilityAsyncContext *)data;
            napi_value result[2] = {nullptr, nullptr};
            if (!asyncContext->status) {
                napi_get_undefined(env, &result[0]);
                napi_create_string_utf8(env, asyncContext->value, strlen(asyncContext->value), &result[1]); // ?
            } else {
                napi_value message = nullptr;
                napi_create_string_utf8(env, "key does not exist", NAPI_AUTO_LENGTH, &message);
                napi_create_error(env, nullptr, message, &result[0]);
                napi_get_undefined(env, &result[1]);
            }
            if (asyncContext->deferred) {
                if (!asyncContext->status) {
                    napi_resolve_deferred(env, asyncContext->deferred, result[1]);
                } else {
                    napi_reject_deferred(env, asyncContext->deferred, result[0]);
                }
            } else {
                napi_value callback = nullptr;
                napi_value returnVal;
                napi_get_reference_value(env, asyncContext->callbackRef, &callback);
                napi_call_function(env, nullptr, callback, 2, result, &returnVal); // 2, count of result
                napi_delete_reference(env, asyncContext->callbackRef);
            }
            napi_delete_async_work(env, asyncContext->work);
            delete asyncContext;
        },
        (void*)asyncContext, &asyncContext->work);
    napi_queue_async_work(env, asyncContext->work);

    return result;
}

napi_value QuerryExport(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("querySystemCapabilities", QuerySystemCapability),
    };

    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc));
    return exports;
}
EXTERN_C_END

/*
 * Module define
 */
static napi_module g_systemCapabilityModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = QuerryExport,
    .nm_modname = "systemCapability",
    .nm_priv = nullptr,
    .reserved = {nullptr},
};

/*
 * Module register function
 */
extern "C" __attribute__((constructor)) void SystemCapabilityRegisterModule(void)
{
    napi_module_register(&g_systemCapabilityModule);
}
}