/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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
#ifndef SIGNATRUETOOLS_DIGESTPARAMETER_H
#define SIGNATRUETOOLS_DIGESTPARAMETER_H

#include "export_define.h"
#include "openssl/ossl_typ.h"

namespace OHOS {
namespace SignatureTools {

class DigestParameter {
public:
    DLL_EXPORT DigestParameter();
    DLL_EXPORT DigestParameter(const DigestParameter& other);
    DLL_EXPORT DigestParameter& operator = (const DigestParameter& other);
    DLL_EXPORT ~DigestParameter();

public:
    int32_t digestOutputSizeBytes;
    const EVP_MD* md;
    EVP_MD_CTX* ctxPtr;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_DIGESTPARAMETER_H
