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

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include "signing_block.h"
#include "sign_block_info.h"
namespace OHOS {
namespace SignatureTools {
bool SignBlockInfoObjectBuild(const uint8_t* data, size_t size)
{
    SignBlockInfo object;
    std::vector<int8_t> sig{ 48, -126, 7, -46, 6, 9, 42, -122, 72, -122, -9, 13, 1 };
    object.SetFileDigest(sig);
    return true;
}

bool SigningBlockObjectBuild(const uint8_t* data, size_t size)
{
    int32_t type = 1;
    std::vector<int8_t> sig{ 48, -126, 7, -46, 6, 9, 42, -122, 72, -122, -9, 13, 1 };
    SigningBlock object(type, sig);
    return true;
}
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::SignatureTools::SignBlockInfoObjectBuild(data, size);
    OHOS::SignatureTools::SigningBlockObjectBuild(data, size);
    return 0;
}