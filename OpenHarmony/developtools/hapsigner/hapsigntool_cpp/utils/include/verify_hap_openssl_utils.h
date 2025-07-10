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
#ifndef SIGNATRUETOOLS_VERIFY_OPENSSL_UTILS_H
#define SIGNATRUETOOLS_VERIFY_OPENSSL_UTILS_H
#include <string>
#include <vector>

#include "pkcs7_context.h"
#include "signature_info.h"
#include "export_define.h"
#include "byte_buffer.h"
#include "openssl/evp.h"
#include "openssl/ossl_typ.h"
#include "openssl/pkcs7.h"
#include "openssl/safestack.h"
#include "digest_parameter.h"
#include "verify_cert_openssl_utils.h"

namespace OHOS {
namespace SignatureTools {

class VerifyHapOpensslUtils {
public:
    VerifyHapOpensslUtils() = delete;
    DLL_EXPORT static bool ParsePkcs7Package(const unsigned char packageData[],
                                             uint32_t packageLen, Pkcs7Context& pkcs7Context);
    DLL_EXPORT static bool GetCertChains(PKCS7* p7, Pkcs7Context& pkcs7Context);

    DLL_EXPORT static bool GetCrlStack(PKCS7* p7, STACK_OF(X509_CRL)* x509Crl);
    DLL_EXPORT static bool VerifyPkcs7(Pkcs7Context& pkcs7Context);

    static void GetOpensslErrorMessage();

private:
    DLL_EXPORT static bool VerifyPkcs7SignedData(Pkcs7Context& pkcs7Context);
    DLL_EXPORT static bool VerifySignInfo(STACK_OF(PKCS7_SIGNER_INFO)* signerInfoStack,
                                          BIO* p7Bio, int32_t signInfoNum, Pkcs7Context& pkcs7Context);
    static bool VerifyCertChain(CertChain& certsChain, PKCS7* p7, PKCS7_SIGNER_INFO* signInfo,
                                Pkcs7Context& pkcs7Context, CertSign& certVisitSign);
    static bool GetContentInfo(const PKCS7* p7ContentInfo, ByteBuffer& content);
    static bool CheckPkcs7SignedDataIsValid(const PKCS7* p7);

    static const int32_t OPENSSL_PKCS7_VERIFY_SUCCESS;
    static const int32_t OPENSSL_ERR_MESSAGE_MAX_LEN;
    static const int32_t OPENSSL_READ_DATA_MAX_TIME;
    static const int32_t OPENSSL_READ_DATA_LEN_EACH_TIME;
    static const int32_t MAX_OID_LENGTH;
    static const std::string PKCS7_EXT_SHAWITHRSA_PSS;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_VERIFY_OPENSSL_UTILS_H
