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

#include "cfcreate_fuzzer.h"

#include <securec.h>

#include "cf_api.h"
#include "cf_memory.h"
#include "cf_result.h"

namespace OHOS {
    bool CfCreateFuzzTest(const uint8_t* data, size_t size)
    {
        if (size < sizeof(CfObjectType) + sizeof(CfEncodingBlob)) {
            return false;
        }
        uint8_t *tmpData = const_cast<uint8_t *>(data);
        size_t usedSize = 0;
        CfObjectType objType = *(reinterpret_cast<CfObjectType *>(tmpData));
        usedSize += sizeof(CfObjectType);

        CfEncodingBlob inStream = { 0 };
        inStream.encodingFormat = *(reinterpret_cast<enum CfEncodingFormat *>(tmpData + usedSize));
        usedSize += sizeof(enum CfEncodingFormat);
        inStream.len = *(reinterpret_cast<size_t *>(tmpData + usedSize));
        usedSize += sizeof(size_t);
        if (inStream.len > size - usedSize) {
            return false;
        }
        inStream.data = static_cast<uint8_t *>(CfMalloc(inStream.len, 0));
        if (inStream.data == nullptr) {
            return false;
        }
        (void)memcpy_s(inStream.data, inStream.len, tmpData + usedSize, inStream.len);

        CfObject *object = nullptr;
        (void)CfCreate(objType, &inStream, &object);
        CfFree(inStream.data);
        if (object != nullptr) {
            object->destroy(&object);
        }

        return true;
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::CfCreateFuzzTest(data, size);
    return 0;
}
