/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include "cminstallusertrustedcertp7b_fuzzer.h"

#include "cert_manager_api.h"
#include "cm_fuzz_test_common.h"

using namespace CmFuzzTest;
namespace OHOS {
    static const uint32_t CM_BLOB_NUM = 3;

    bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
    {
        uint32_t minSize = CM_BLOB_NUM * sizeof(struct CmBlob);
        uint8_t *myData = nullptr;
        if (!CopyMyData(data, size, minSize, &myData)) {
            return false;
        }

        uint32_t remainSize = static_cast<uint32_t>(size);
        uint32_t offset = 0;
        struct CmBlob userCert = { 0, nullptr };
        if (!GetCmBlobFromBuffer(myData, &remainSize, &offset, &userCert)) {
            CmFree(myData);
            return false;
        }

        struct CmBlob certAlias = { 0, nullptr };
        if (!GetCmBlobFromBuffer(myData, &remainSize, &offset, &certAlias)) {
            CmFree(myData);
            return false;
        }

        struct CertUriList certUriList = { 0, nullptr };

        uint32_t randomUserId = 0;
        if (!GetUintFromBuffer(myData, &remainSize, &offset, &randomUserId)) {
            return false;
        }
        struct CmInstallCertInfo installCertInfo = {
            .userCert = &userCert,
            .certAlias = &certAlias,
            .userId = randomUserId
        };

        SetATPermission();
        (void)CmInstallUserTrustedP7BCert(&installCertInfo, true, &certUriList);

        CM_FREE_PTR(certUriList.uriList);
        CmFree(myData);
        return true;
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}
