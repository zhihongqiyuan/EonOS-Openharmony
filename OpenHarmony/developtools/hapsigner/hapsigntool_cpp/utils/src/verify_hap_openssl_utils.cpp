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
#include "verify_hap_openssl_utils.h"
#include "signature_tools_log.h"
#include "openssl/asn1.h"
#include "openssl/bio.h"
#include "openssl/crypto.h"
#include "openssl/err.h"
#include "openssl/obj_mac.h"
#include "openssl/objects.h"
#include "openssl/rsa.h"
#include "openssl/x509.h"
#include "openssl/pem.h"
#include "constant.h"
#include "signature_info.h"

namespace OHOS {
namespace SignatureTools {
using Pkcs7SignerInfoStack = STACK_OF(PKCS7_SIGNER_INFO);
using X509AttributeStack = STACK_OF(X509_ATTRIBUTE);
const int32_t VerifyHapOpensslUtils::OPENSSL_PKCS7_VERIFY_SUCCESS = 1;
const int32_t VerifyHapOpensslUtils::OPENSSL_ERR_MESSAGE_MAX_LEN = 1024;
/*
* OPENSSL_READ_DATA_MAX_TIME * OPENSSL_READ_DATA_LEN_EACH_TIME < 2GBytes.
* make the maximum size of data that can be read each time be 1 KBytes,
* so the maximum times of read data is 1024 * 1024 * 2 = 2097152;
*/
const int32_t VerifyHapOpensslUtils::OPENSSL_READ_DATA_MAX_TIME = 2097152;
const int32_t VerifyHapOpensslUtils::OPENSSL_READ_DATA_LEN_EACH_TIME = 1024;
/* Signature algorithm OID for extended PKCS7 */
const std::string VerifyHapOpensslUtils::PKCS7_EXT_SHAWITHRSA_PSS = PKCS7_EXT_SIGNATURE_OID;
const int32_t VerifyHapOpensslUtils::MAX_OID_LENGTH = 128;

bool VerifyHapOpensslUtils::ParsePkcs7Package(const unsigned char packageData[],
                                              uint32_t packageLen, Pkcs7Context& pkcs7Context)
{
    if (packageData == nullptr || packageLen == 0) {
        SIGNATURE_TOOLS_LOGE("invalid input");
        return false;
    }
    pkcs7Context.p7 = d2i_PKCS7(nullptr, &packageData, packageLen);
    if (!CheckPkcs7SignedDataIsValid(pkcs7Context.p7)) {
        GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("p7 is invalid");
        return false;
    }
    if (!GetContentInfo(pkcs7Context.p7->d.sign->contents, pkcs7Context.content)) {
        SIGNATURE_TOOLS_LOGE("Get content from pkcs7 failed");
        return false;
    }
    return true;
}

bool VerifyHapOpensslUtils::GetCertChains(PKCS7* p7, Pkcs7Context& pkcs7Context)
{
    if (!CheckPkcs7SignedDataIsValid(p7)) {
        SIGNATURE_TOOLS_LOGE("p7 is invalid");
        return false;
    }
    CertSign certVisitSign;
    VerifyCertOpensslUtils::GenerateCertSignFromCertStack(p7->d.sign->cert, certVisitSign);
    Pkcs7SignerInfoStack* signerInfoStack = PKCS7_get_signer_info(p7);
    if (signerInfoStack == nullptr) {
        SIGNATURE_TOOLS_LOGE("get signerInfoStack error");
        GetOpensslErrorMessage();
        return false;
    }
    int32_t signCount = sk_PKCS7_SIGNER_INFO_num(signerInfoStack);
    if (signCount <= 0) {
        SIGNATURE_TOOLS_LOGE("can not find signinfo");
        return false;
    }
    for (int32_t i = 0; i < signCount; i++) {
        /* get ith signInfo */
        PKCS7_SIGNER_INFO* signInfo = sk_PKCS7_SIGNER_INFO_value(signerInfoStack, i);
        if (signInfo == nullptr) {
            SIGNATURE_TOOLS_LOGE("signInfo %dst is nullptr", i);
            return false;
        }
        /* GET X509 certificate */
        X509* cert = PKCS7_cert_from_signer_info(p7, signInfo);
        if (cert == nullptr) {
            SIGNATURE_TOOLS_LOGE("get cert for %dst signInfo failed", i);
            return false;
        }
        CertChain certChain;
        pkcs7Context.certChain.push_back(certChain);
        pkcs7Context.certChain[i].push_back(X509_dup(cert));
        VerifyCertOpensslUtils::ClearCertVisitSign(certVisitSign);
        certVisitSign[cert] = true;
        if (!VerifyCertChain(pkcs7Context.certChain[i], p7, signInfo, pkcs7Context, certVisitSign)) {
            SIGNATURE_TOOLS_LOGE("verify %dst certchain failed", i);
            return false;
        }
    }
    return true;
}

bool VerifyHapOpensslUtils::VerifyCertChain(CertChain& certsChain, PKCS7* p7,
                                            PKCS7_SIGNER_INFO* signInfo,
                                            Pkcs7Context& pkcs7Context,
                                            CertSign& certVisitSign)
{
    if (!VerifyCertOpensslUtils::GetCertsChain(certsChain, certVisitSign)) {
        SIGNATURE_TOOLS_LOGE("get cert chain for signInfo failed");
        return false;
    }
    ASN1_TYPE* signTime = PKCS7_get_signed_attribute(signInfo, NID_pkcs9_signingTime);
    if (!VerifyCertOpensslUtils::VerifyCertChainPeriodOfValidity(certsChain, signTime)) {
        SIGNATURE_TOOLS_LOGE("VerifyCertChainPeriodOfValidity for signInfo failed");
        return false;
    }
    return true;
}

bool VerifyHapOpensslUtils::CheckPkcs7SignedDataIsValid(const PKCS7* p7)
{
    if (p7 == nullptr || !PKCS7_type_is_signed(p7) || p7->d.sign == nullptr) {
        return false;
    }
    return true;
}

bool VerifyHapOpensslUtils::GetCrlStack(PKCS7* p7, STACK_OF(X509_CRL)* x509Crl)
{
    if (!CheckPkcs7SignedDataIsValid(p7)) {
        return false;
    }
    x509Crl = p7->d.sign->crl;
    return true;
}

bool VerifyHapOpensslUtils::VerifyPkcs7(Pkcs7Context& pkcs7Context)
{
    if (!CheckPkcs7SignedDataIsValid(pkcs7Context.p7)) {
        SIGNATURE_TOOLS_LOGE("p7 type is invalid signed_data_pkcs7");
        return false;
    }
    if (!VerifyPkcs7SignedData(pkcs7Context)) {
        SIGNATURE_TOOLS_LOGE("verify p7 error");
        return false;
    }
    return true;
}

bool VerifyHapOpensslUtils::VerifyPkcs7SignedData(Pkcs7Context& pkcs7Context)
{
    /* get signed data which was used to be signed */
    BIO* p7Bio = PKCS7_dataDecode(pkcs7Context.p7, nullptr, nullptr, nullptr);
    if (p7Bio == nullptr) {
        SIGNATURE_TOOLS_LOGE("get p7bio error");
        GetOpensslErrorMessage();
        return false;
    }
    char buf[OPENSSL_READ_DATA_LEN_EACH_TIME] = { 0 };
    int32_t readLen = BIO_read(p7Bio, buf, sizeof(buf));
    int32_t readTime = 0;
    while ((readLen > 0) && (++readTime < OPENSSL_READ_DATA_MAX_TIME)) {
        readLen = BIO_read(p7Bio, buf, sizeof(buf));
    }
    Pkcs7SignerInfoStack* signerInfoStack = PKCS7_get_signer_info(pkcs7Context.p7);
    if (signerInfoStack == nullptr) {
        SIGNATURE_TOOLS_LOGE("get signerInfoStack error");
        BIO_free_all(p7Bio);
        GetOpensslErrorMessage();
        return false;
    }
    /* get the num of signInfo */
    int32_t signCount = sk_PKCS7_SIGNER_INFO_num(signerInfoStack);
    if (signCount <= 0) {
        SIGNATURE_TOOLS_LOGE("can not find signinfo");
        BIO_free_all(p7Bio);
        return false;
    }
    for (int32_t i = 0; i < signCount; i++) {
        if (!VerifySignInfo(signerInfoStack, p7Bio, i, pkcs7Context)) {
            SIGNATURE_TOOLS_LOGE("Verify %dst signInfo failed", i);
            BIO_free_all(p7Bio);
            return false;
        }
    }
    BIO_free_all(p7Bio);
    return true;
}

bool VerifyHapOpensslUtils::VerifySignInfo(STACK_OF(PKCS7_SIGNER_INFO)* signerInfoStack,
                                           BIO* p7Bio, int32_t signInfoNum, Pkcs7Context& pkcs7Context)
{
    if (signerInfoStack == nullptr || p7Bio == nullptr) {
        SIGNATURE_TOOLS_LOGE("invalid input");
        return false;
    }
    /* get signInfo */
    PKCS7_SIGNER_INFO* signInfo = sk_PKCS7_SIGNER_INFO_value(signerInfoStack, signInfoNum);
    if (signInfo == nullptr) {
        SIGNATURE_TOOLS_LOGE("signInfo %dst is nullptr", signInfoNum);
        return false;
    }
    /* GET X509 certificate */
    X509* cert = pkcs7Context.certChain[signInfoNum][0];

    if (PKCS7_signatureVerify(p7Bio, pkcs7Context.p7, signInfo, cert) <= 0) {
        SIGNATURE_TOOLS_LOGE("PKCS7_signatureVerify %dst signInfo failed", signInfoNum);
        GetOpensslErrorMessage();
        return false;
    }

    return true;
}

bool VerifyHapOpensslUtils::GetContentInfo(const PKCS7* p7ContentInfo, ByteBuffer& content)
{
    if ((p7ContentInfo == nullptr) || !PKCS7_type_is_data(p7ContentInfo)) {
        SIGNATURE_TOOLS_LOGE("p7ContentInfo is invalid");
        return false;
    }
    ASN1_OCTET_STRING* strContentInfo = p7ContentInfo->d.data;
    if (strContentInfo == nullptr) {
        SIGNATURE_TOOLS_LOGE("strContentInfo is invalid");
        return false;
    }
    int32_t strContentInfoLen = strContentInfo->length;
    unsigned char* strContentInfoData = strContentInfo->data;
    if (strContentInfoData == nullptr || strContentInfoLen <= 0) {
        SIGNATURE_TOOLS_LOGE("ASN1_OCTET_STRING is invalid");
        return false;
    }
    content.SetCapacity(strContentInfoLen);
    content.PutData(0, reinterpret_cast<char*>(strContentInfoData), strContentInfoLen);
    SIGNATURE_TOOLS_LOGD("strContentInfoLen: %d", strContentInfoLen);
    return true;
}

void VerifyHapOpensslUtils::GetOpensslErrorMessage()
{
    unsigned long retOpenssl;
    char errOpenssl[OPENSSL_ERR_MESSAGE_MAX_LEN];
    while ((retOpenssl = ERR_get_error()) != 0) {
        ERR_error_string(retOpenssl, errOpenssl);
        SIGNATURE_TOOLS_LOGE("openssl err: %lu, message: %s", retOpenssl, errOpenssl);
    }
}
} // namespace SignatureTools
} // namespace OHOS