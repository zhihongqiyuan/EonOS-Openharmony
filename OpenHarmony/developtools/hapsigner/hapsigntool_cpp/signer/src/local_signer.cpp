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

#include "signature_tools_log.h"
#include "pkcs7_data.h"
#include "constant.h"
#include "local_signer.h"

namespace OHOS {
namespace SignatureTools {
/**
* Create local signer.
*
* @param keyPair   Private key to sign
* @param certificates Cert chain to sign
*/
LocalSigner::LocalSigner(EVP_PKEY* keyPair, STACK_OF(X509)* certificates) :m_keyPair(keyPair),
m_certificates(certificates)
{
    bool check = m_keyPair == NULL ||
                 PKCS7Data::SortX509Stack(m_certificates) < 0 ||
                 X509_check_private_key(sk_X509_value(m_certificates, 0), m_keyPair) != 1;
    if (check) {
        SIGNATURE_TOOLS_LOGW("Warning: invalid local signer!\n");
        sk_X509_pop_free(m_certificates, X509_free);
        m_certificates = NULL;
    }
}
LocalSigner::~LocalSigner()
{
    if (m_keyPair) {
        EVP_PKEY_free(m_keyPair);
        m_keyPair = NULL;
    }
    if (m_certificates) {
        sk_X509_pop_free(m_certificates, X509_free);
        m_certificates = NULL;
    }
}
STACK_OF(X509_CRL)* LocalSigner::GetCrls() const
{
    return NULL;
}

static X509* X509Dup(const X509* x509)
{
    return X509_dup(const_cast<X509*>(x509));
}

STACK_OF(X509)* LocalSigner::GetCertificates() const
{
    if (m_certificates == NULL) {
        return m_certificates;
    }
    return sk_X509_deep_copy(m_certificates, X509Dup, X509_free);
}
std::string LocalSigner::GetSignature(const std::string& data, const std::string& signAlg) const
{
    EVP_MD_CTX* hashCtx = NULL;
    EVP_PKEY_CTX* privateKeyCtx = NULL;
    unsigned char* sigResult = NULL;
    const EVP_MD* hash = NULL;
    size_t sigLen;
    std::string ret;

    if (signAlg == SIGN_ALG_SHA256) {
        hash = EVP_sha256();
    } else if (signAlg == SIGN_ALG_SHA384) {
        hash = EVP_sha384();
    } else {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            signAlg + "is invalid sigAlg, please use SHA256withECDSA/SHA384withECDSA, sign failed");
        return ret;
    }

    /* calculate signature value */
    bool result = !(hashCtx = EVP_MD_CTX_new())                                          ||
                  (EVP_DigestSignInit(hashCtx, &privateKeyCtx, hash, NULL, m_keyPair) <= 0) ||
                  (EVP_DigestSignUpdate(hashCtx, data.data(), data.size()) <= 0)         ||
                  (EVP_DigestSignFinal(hashCtx, NULL, &sigLen) <= 0)                     ||
                  !(sigResult = reinterpret_cast<unsigned char*>(OPENSSL_malloc(sigLen)))  ||
                  (EVP_DigestSignFinal(hashCtx, sigResult, &sigLen) <= 0);
    if (result) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "compute signature value failed");
        goto err;
    }
    ret.assign(reinterpret_cast<const char*>(sigResult), sigLen);
err:
    OPENSSL_free(sigResult);
    EVP_MD_CTX_free(hashCtx);
    return ret;
}
} // namespace SignatureTools
} // namespace OHOS