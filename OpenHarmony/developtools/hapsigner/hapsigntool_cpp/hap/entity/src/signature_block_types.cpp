/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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
#include "signature_block_types.h"

namespace OHOS {
namespace SignatureTools {
char SignatureBlockTypes::GetProfileBlockTypes(const std::string& isSigned)
{
    if (!isSigned.empty() && 0 == isSigned.compare("0")) {
        return PROFILE_NOSIGNED_BLOCK;
    }
    if (!isSigned.empty() && 0 == isSigned.compare("1")) {
        return PROFILE_SIGNED_BLOCK;
    }
    return PROFILE_NOSIGNED_BLOCK;
}
} // namespace SignatureTools
} // namespace OHOS