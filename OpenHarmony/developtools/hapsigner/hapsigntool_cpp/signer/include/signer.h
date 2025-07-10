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
#ifndef SINATURETOOLS_ISINGER_H
#define SINATURETOOLS_ISINGER_H

#include <string>
#include "openssl/x509.h"

namespace OHOS {
namespace SignatureTools {

/**
 * Signer.
 *
 */
class Signer {
public:
    virtual STACK_OF(X509_CRL)* GetCrls()const = 0;
    virtual STACK_OF(X509)* GetCertificates()const = 0;
    virtual ~Signer() = default;
    virtual std::string GetSignature(const std::string& data, const std::string& signAlg)const = 0;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SINATURETOOLS_ISINGER_H