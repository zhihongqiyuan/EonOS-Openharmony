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
#ifndef SIGNATRUETOOLS_PKCS7DATA_H
#define SIGNATRUETOOLS_PKCS7DATA_H

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <algorithm>
#include <list>

#include "openssl/pem.h"
#include "openssl/ecdsa.h"
#include "openssl/evp.h"
#include "openssl/ec.h"
#include "openssl/asn1t.h"
#include "openssl/pkcs7.h"
#include "signer.h"
#include "verify_cert_openssl_utils.h"

 /* compare cert is equal in std::unordered_set<X509*> for SortX509Stack */
template<>
struct std::equal_to<X509*> {
    bool operator() (X509* cert1, X509* cert2) const
    {
        ASN1_INTEGER* serial1 = X509_get_serialNumber(cert1);
        ASN1_INTEGER* serial2 = X509_get_serialNumber(cert2);
        bool serialEqual = ASN1_INTEGER_cmp(serial1, serial2) == 0;
        bool nameEqual = X509_NAME_cmp(X509_get_subject_name(cert1), X509_get_subject_name(cert2)) == 0;
        return serialEqual && nameEqual;
    }
};

/* all cert is put into one bottle */
template<>
struct std::hash<X509*> {
    size_t operator()(const X509* cert) const
    {
        return 0;
    }
};

namespace OHOS {
namespace SignatureTools {
#define  PKCS7_NODETACHED_FLAGS  (PKCS7_BINARY | PKCS7_NOVERIFY)
#define  PKCS7_DETACHED_FLAGS    (PKCS7_BINARY | PKCS7_NOVERIFY | PKCS7_DETACHED)
struct PKCS7Attr {
    int nid;
    int atrtype;
    void* value;
};

class PKCS7Data {
public:
    PKCS7Data(int flags = PKCS7_NODETACHED_FLAGS);
    PKCS7Data(const PKCS7Data& pkcs7) = delete;
    const PKCS7Data& operator=(const PKCS7Data& pkcs7) = delete;
    ~PKCS7Data();
    /*
    * @param content The data to be signed
    * @param signer  signer
    * @param sigAlg  Signature algorithm SHA256withECDSA/SHA384withECDSA
    * @param ret     The returned signature result is pkcs7
    * @param attrs   It is only used when you need to add an ownerID, and you don't need to process it by default
    * @return        0 :success <0 :error
    */
    int Sign(const std::string& content, const std::shared_ptr<Signer>& signer, const std::string& sigAlg,
             std::string& ret, std::vector<PKCS7Attr> attrs = std::vector<PKCS7Attr>());
    /* d2i deserialize */
    int Parse(const std::string& p7bBytes);
    int Parse(const std::vector<int8_t>& p7bBytes);
    /* When verifying the signature, you don't need to enter content by default, if the data is separated
     * (content is not in pkcs7, you need to pass in the original data for verification)
     */
    int Verify(const std::string& content = "")const;
    /* get original raw content */
    int GetContent(std::string& content) const;

    /* In C++, the certificate chain order is forward, and Java is reversed,
    which is historically the result of correcting the certificate chain order */
    static int SortX509Stack(STACK_OF(X509)* certs);
    /* Subject information for printing the certificate chain */
    static void PrintCertChainSub(const STACK_OF(X509)* certs);
    static std::string GetASN1Time(const ASN1_TIME* tm);
    /* Compare the two, first certificate issuers with the second certificate subject info */
    static bool X509NameCompare(const X509* cert, const X509* issuerCert);
    /* check pkcs7 sign time in certchain valid period */
    static int CheckSignTimeInValidPeriod(const ASN1_TYPE* signTime,
                                          const ASN1_TIME* notBefore, const ASN1_TIME* notAfter);

private:
    int Parse(const unsigned char** in, long len);
    int InitPkcs7(const std::string& content, const std::shared_ptr<Signer>& signer,
                  const std::string& sigAlg, std::vector<PKCS7Attr> attrs);
    /* Verify Signature Value The certificate chain is not verified here */
    int VerifySign(const std::string& content)const;
    int VerifyCertChain()const;
    /* Verify the validity of the time */
    int CheckSginerInfoSignTimeInCertChainValidPeriod(PKCS7_SIGNER_INFO* signerInfo, STACK_OF(X509)* certs)const;
    /* @param cert Entity Certificate
     * @param certs Certificate chain (without Entity certificates)
     * @param certChain Certificate chain (with Entity certificates)
     * @retrun 0 success <0 error
     */
    int VerifySignerInfoCertchain(PKCS7* p7,
                                  PKCS7_SIGNER_INFO* signerInfo,
                                  STACK_OF(X509)* certs,
                                  STACK_OF(X509)* certChain)const;

private:
    /* For ease of reading, the following interface will be as consistent as possible with the OpenSSL
    library interface style, and the return value is 1 success and 0 failure */
    int Pkcs7SignAttr(PKCS7_SIGNER_INFO* si);

    int Pkcs7AddTimeDigestAndSignAttr(PKCS7_SIGNER_INFO* si, EVP_MD_CTX* mctx);

    int Pkcs7DataFinalSignAttr(STACK_OF(PKCS7_SIGNER_INFO)* si_sk, BIO* bio);

    int Pkcs7DataFinal(PKCS7* p7, BIO* bio);

    int Pkcs7Final(PKCS7* p7, const std::string& content, int flags);

    PKCS7* Pkcs7Sign(X509* signcert, STACK_OF(X509)* certs, const EVP_MD* md,
                     const std::string& content, int flags, const std::vector<PKCS7Attr>& attrs);

private:
    PKCS7* m_p7 = NULL;
    int m_flags;
    std::shared_ptr<Signer> m_signer; // tmp
    std::string m_sigAlg; // tmp
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_PKCS7DATA_H