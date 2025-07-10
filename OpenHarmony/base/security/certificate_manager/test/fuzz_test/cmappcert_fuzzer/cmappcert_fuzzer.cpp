/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "cmappcert_fuzzer.h"

#include "cm_fuzz_test_common.h"
#include "cm_cert_data_rsa.h"
#include "cm_ipc_client_serialization.h"
#include "cm_ipc_service.h"
#include "cm_param.h"
#include "cert_manager_api.h"
#include "cm_type.h"

using namespace CmFuzzTest;
namespace OHOS {
    constexpr uint32_t TEST_USERID = 100;
    constexpr uint32_t MIN_DATA_USE_TIME = 4;
    static bool InstallAppCert(uint8_t *tmpData, uint32_t *remainSize, uint32_t *offset, uint32_t store,
        struct CmBlob *keyUri)
    {
        uint8_t certAliasBuf[] = "keyA";
        struct CmBlob certAlias = { sizeof(certAliasBuf), certAliasBuf };
        if (TenPercentChanceOfBeingTrue(tmpData, remainSize, offset)) {
            if (!GetCmBlobFromBuffer(tmpData, remainSize, offset, keyUri)) {
                return false;
            }
        }

        const struct CmBlob appCert = { sizeof(g_rsa2048P12CertInfo), const_cast<uint8_t *>(g_rsa2048P12CertInfo) };
        const struct CmBlob appCertPwd = { sizeof(g_certPwd), const_cast<uint8_t *>(g_certPwd) };
        struct CmParam params01[] = {
            { .tag = CM_TAG_PARAM0_BUFFER, .blob = appCert },
            { .tag = CM_TAG_PARAM1_BUFFER, .blob = appCertPwd },
            { .tag = CM_TAG_PARAM2_BUFFER, .blob = certAlias },
            { .tag = CM_TAG_PARAM0_UINT32, .uint32Param = store },
            { .tag = CM_TAG_PARAM1_UINT32, .uint32Param = TEST_USERID },
        };
        struct CmParamSet *paramSet01 = NULL;
        int32_t ret = CmParamsToParamSet(params01, CM_ARRAY_SIZE(params01), &paramSet01);
        if (ret != CM_SUCCESS) {
            return false;
        }
        struct CmBlob paramSetBlob = { paramSet01->paramSetSize, reinterpret_cast<uint8_t *>(paramSet01) };

        (void)CmIpcServiceInstallAppCert(&paramSetBlob, keyUri, nullptr);
        CmFreeParamSet(&paramSet01);
        return true;
    }

    static bool GetAllAppCert(uint8_t *tmpData, uint32_t *remainSize, uint32_t *offset, uint32_t store)
    {
        struct CmParam params02[] = { { .tag = CM_TAG_PARAM0_UINT32, .uint32Param = store } };
        struct CmParamSet *paramSet02 = NULL;
        int32_t ret = CmParamsToParamSet(params02, CM_ARRAY_SIZE(params02), &paramSet02);
        if (ret != CM_SUCCESS) {
            return false;
        }
        struct CmBlob paramSetBlob = { paramSet02->paramSetSize, reinterpret_cast<uint8_t *>(paramSet02) };

        (void)CmIpcServiceGetAppCertList(&paramSetBlob, nullptr, nullptr);
        CmFreeParamSet(&paramSet02);
        return true;
    }

    static bool GetAppCert(uint8_t *tmpData, uint32_t *remainSize, uint32_t *offset, uint32_t store,
        struct CmBlob *keyUri)
    {
        if (TenPercentChanceOfBeingTrue(tmpData, remainSize, offset)) {
            if (!GetCmBlobFromBuffer(tmpData, remainSize, offset, keyUri)) {
                return false;
            }
        }

        struct CmParam params03[] = {
            { .tag = CM_TAG_PARAM0_BUFFER, .blob = *keyUri },
            { .tag = CM_TAG_PARAM0_UINT32, .uint32Param = store },
        };
        struct CmParamSet *paramSet03 = NULL;
        int32_t ret = CmParamsToParamSet(params03, CM_ARRAY_SIZE(params03), &paramSet03);
        if (ret != CM_SUCCESS) {
            return false;
        }
        struct CmBlob paramSetBlob = { paramSet03->paramSetSize, reinterpret_cast<uint8_t *>(paramSet03) };

        (void)CmIpcServiceGetAppCert(&paramSetBlob, nullptr, nullptr);
        CmFreeParamSet(&paramSet03);
        return true;
    }

    static bool UnInstallAppCert(uint8_t *tmpData, uint32_t *remainSize, uint32_t *offset, uint32_t store,
        struct CmBlob *keyUri)
    {
        if (TenPercentChanceOfBeingTrue(tmpData, remainSize, offset)) {
            if (!GetCmBlobFromBuffer(tmpData, remainSize, offset, keyUri)) {
                return false;
            }
        }

        struct CmParam params04[] = {
            { .tag = CM_TAG_PARAM0_BUFFER, .blob = *keyUri },
            { .tag = CM_TAG_PARAM0_UINT32, .uint32Param = store },
        };
        struct CmParamSet *paramSet04 = NULL;
        int32_t ret = CmParamsToParamSet(params04, CM_ARRAY_SIZE(params04), &paramSet04);
        if (ret != CM_SUCCESS) {
            return false;
        }
        struct CmBlob paramSetBlob = { paramSet04->paramSetSize, reinterpret_cast<uint8_t *>(paramSet04) };

        (void)CmIpcServiceUninstallAppCert(&paramSetBlob, nullptr, nullptr);
        CmFreeParamSet(&paramSet04);
        return true;
    }

    bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
    {
        uint8_t *tmpData = nullptr;
        if (!CopyMyData(data, size, sizeof(uint32_t) * MIN_DATA_USE_TIME, &tmpData)) {
            return false;
        }

        uint32_t remainSize = static_cast<uint32_t>(size);
        uint32_t offset = 0;
        SetATPermission();

        uint32_t store;
        if (!GetUintFromBuffer(tmpData, &remainSize, &offset, &store)) {
            CmFree(tmpData);
            return false;
        }
        store %= CM_SYS_CREDENTIAL_STORE + 1;
        bool ret = false;
        uint8_t uriBuf[MAX_LEN_URI] = {0};
        struct CmBlob keyUri = { sizeof(uriBuf), uriBuf };

        do {
            if (!InstallAppCert(tmpData, &remainSize, &offset, store, &keyUri)) {
                break;
            }

            if (!GetAllAppCert(tmpData, &remainSize, &offset, store)) {
                break;
            }

            if (!GetAppCert(tmpData, &remainSize, &offset, store, &keyUri)) {
                break;
            }

            if (!UnInstallAppCert(tmpData, &remainSize, &offset, store, &keyUri)) {
                break;
            }
            ret = true;
        } while (0);
        (void)CmIpcServiceUninstallAllAppCert(nullptr, nullptr, nullptr);
        CmFree(tmpData);
        return ret;
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}

