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
#ifndef SINATURETOOLS_LOCAL_SIGNER_H
#define SINATURETOOLS_LOCAL_SIGNER_H

#include "openssl/x509.h"
#include "signer.h"

namespace OHOS {
namespace SignatureTools {

class LocalSigner : public Signer {
public:
    /**
    * Create local signer.
    *
    * @param keyPair   Private key to sign
    * @param certificates Cert chain to sign
    */
    LocalSigner(EVP_PKEY* keyPair, STACK_OF(X509)* certificates);
    ~LocalSigner()override;
    /**
    * GetCrls.
    *
    * @return crls
    */
    STACK_OF(X509_CRL)* GetCrls()const override;
    /**
    * GetCertificates.
    *
    * @return Certificates
    */
    STACK_OF(X509)* GetCertificates()const  override;
    std::string GetSignature(const std::string& data, const std::string& signAlg)const override;
    
private:
    /**
    * key pair.
    */
    EVP_PKEY* m_keyPair;
    /**
    * certchain.
    */
    STACK_OF(X509)* m_certificates;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SINATURETOOLS_LOCAL_SIGNER_H