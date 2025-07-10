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

#include "hap_signer_block_utils.h"

namespace OHOS {
namespace SignatureTools {
const std::string pathFile = "./zip/tmp.hap";
void HapSignerBlockUtilsFunc(const uint8_t* data, size_t size)
{
    SignatureInfo info;
    HapSignerBlockUtils::CreatTestZipFile(pathFile, info);

    HapSubSignBlockHead signBlock;
    HapSubSignBlockHead profileBlock;
    HapSubSignBlockHead propertyBlock;
    HapSignerBlockUtils::CreateHapSubSignBlockHead(signBlock, profileBlock, propertyBlock);

    std::vector<OptionalBlock> optionBlocks;
    int32_t type = 0;
    int index = 0;
    HapSignerBlockUtils::GetOptionalBlockIndex(optionBlocks, type, index);
}

void DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size)
{
    if (data == nullptr || size == 0) {
        return;
    }

    HapSignerBlockUtilsFunc(data, size);
}
} // namespace SignatureTools
} // namespace OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::SignatureTools::DoSomethingInterestingWithMyAPI(data, size);
    return 0;
}