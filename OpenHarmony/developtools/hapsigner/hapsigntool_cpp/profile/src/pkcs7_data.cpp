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
#include "signature_tools_errno.h"
#include "verify_hap_openssl_utils.h"
#include "signer.h"
#include "securec.h"
#include "constant.h"
#include "pkcs7_data.h"

namespace OHOS {
namespace SignatureTools {

static constexpr int BUFFER_SIZE = 4096;

static int PKCS7AddAttribute(PKCS7* p7, const std::vector<PKCS7Attr>& attrs)
{
    STACK_OF(PKCS7_SIGNER_INFO)* signerInfos = PKCS7_get_signer_info(p7);
    if (signerInfos == NULL || sk_PKCS7_SIGNER_INFO_num(signerInfos) != 1) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "signer info count not equal 1,pkcs7 add customize attribute failed");
        return INVALIDPARAM_ERROR;
    }
    PKCS7_SIGNER_INFO* signerInfo = sk_PKCS7_SIGNER_INFO_value(signerInfos, 0);
    for (PKCS7Attr attr : attrs) {
        if (PKCS7_add_signed_attribute(signerInfo, attr.nid, attr.atrtype, attr.value) != 1) {
            if (attr.atrtype == V_ASN1_UTF8STRING)
                ASN1_STRING_free(reinterpret_cast<ASN1_STRING*>(attr.value));
            PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                                "pkcs7 add customize attribute failed");
            return RET_FAILED;
        }
    }
    return RET_OK;
}

static int I2dPkcs7Str(PKCS7* p7, std::string& ret)
{
    /* raw data exported in pkcs7 */
    unsigned char* out = NULL;
    int outSize = 0;
    /* Deserialize to obtain the p7b byte stream */
    outSize = i2d_PKCS7(p7, &out);
    if (out == NULL || outSize <= 0) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "pkcs7 is invalid");
        return INVALIDPARAM_ERROR;
    }
    ret.clear();
    ret.resize(outSize);
    std::copy(out, out + outSize, &ret[0]);
    OPENSSL_free(out);
    return RET_OK;
}

static int SetSignerInfoSignAlgor(PKCS7_SIGNER_INFO* info)
{
    int signNid = 0;
    int hashNid = 0;
    X509_ALGOR* dig;
    X509_ALGOR* sig;
    PKCS7_SIGNER_INFO_get0_algs(info, NULL, &dig, &sig);
    if (dig == NULL || dig->algorithm == NULL ||
        (hashNid = OBJ_obj2nid(dig->algorithm)) == NID_undef ||
        !OBJ_find_sigid_by_algs(&signNid, hashNid, NID_X9_62_id_ecPublicKey) ||
        X509_ALGOR_set0(sig, OBJ_nid2obj(signNid), V_ASN1_UNDEF, 0) != 1) {
        return 0;
    }
    return 1;
}

static int VerifySignature(PKCS7* pkcs7, BIO* p7bio)
{
    /* signature information */
    STACK_OF(PKCS7_SIGNER_INFO)* skSignerInfo = NULL;
    /* signature count */
    int signerCount = 0;
    /* verify signature value */
    skSignerInfo = PKCS7_get_signer_info(pkcs7);
    signerCount = sk_PKCS7_SIGNER_INFO_num(skSignerInfo);
    for (int i = 0; i < signerCount; i++) {
        PKCS7_SIGNER_INFO* signerInfo = sk_PKCS7_SIGNER_INFO_value(skSignerInfo, i);
        X509* sigCert = PKCS7_cert_from_signer_info(pkcs7, signerInfo);
        if (PKCS7_signatureVerify(p7bio, pkcs7, signerInfo, sigCert) != 1) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "signature value verify failed");
            return VERIFY_ERROR;
        }
    }
    return RET_OK;
}

PKCS7Data::PKCS7Data(int flags) : m_p7(nullptr), m_flags(flags)
{
}

PKCS7Data::~PKCS7Data()
{
    PKCS7_free(m_p7);
    m_p7 = NULL;
}

int PKCS7Data::Sign(const std::string& content, const std::shared_ptr<Signer>& signer,
                    const std::string& sigAlg, std::string& ret, std::vector<PKCS7Attr> attrs)
{
    int result = RET_OK;
    if ((result = InitPkcs7(content, signer, sigAlg, attrs)) < 0) {
        goto err;
    }

    /* serialization */
    if ((result = I2dPkcs7Str(m_p7, ret)) < 0) {
        goto err;
    }
    /* release resources */
err:
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("sign failed");
    }
    return result;
}

int PKCS7Data::Parse(const std::string& p7bBytes)
{
    const unsigned char* data = reinterpret_cast<const unsigned char*>(&p7bBytes[0]);
    return Parse(&data, static_cast<long>(p7bBytes.size()));
}
int PKCS7Data::Parse(const std::vector<int8_t>& p7bBytes)
{
    const unsigned char* data = reinterpret_cast<const unsigned char*>(&p7bBytes[0]);
    return Parse(&data, static_cast<long>(p7bBytes.size()));
}
int PKCS7Data::Parse(const unsigned char** in, long len)
{
    /* If p7 has been initialized, it will be released */
    if (m_p7) {
        PKCS7_free(m_p7);
        m_p7 = NULL;
    }
    /* Deserialize */
    m_p7 = d2i_PKCS7(NULL, in, len);
    if (m_p7 == NULL) {
        PrintErrorNumberMsg("PARSE_ERROR", PARSE_ERROR, "invalid p7b data, parse failed");
        return PARSE_ERROR;
    }
    return RET_OK;
}

int PKCS7Data::Verify(const std::string& content) const
{
    if (VerifySign(content) < 0) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "signature verify failed");
        return VERIFY_ERROR;
    }
    if (VerifyCertChain() < 0) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "cert Chain verify failed");
        PrintCertChainSub(m_p7->d.sign->cert);
        return VERIFY_ERROR;
    }
    return RET_OK;
}

int PKCS7Data::GetContent(std::string& originalRawData) const
{
    BIO* oriBio = PKCS7_dataDecode(m_p7, NULL, NULL, NULL);
    if (oriBio == NULL) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR, "pkcs7 get content data failed!");
        return INVALIDPARAM_ERROR;
    }
    char buf[BUFFER_SIZE]{0};
    size_t readBytes = 0;
    while (BIO_read_ex(oriBio, buf, sizeof(buf), &readBytes) == 1) {
        originalRawData.append(buf, readBytes);
    }
    BIO_free_all(oriBio);
    return RET_OK;
}

static void PKCS7AddCrls(PKCS7* p7, STACK_OF(X509_CRL)* crls)
{
    for (int i = 0; i < sk_X509_CRL_num(crls); i++) {
        PKCS7_add_crl(p7, sk_X509_CRL_value(crls, i));
    }
}

int PKCS7Data::InitPkcs7(const std::string& content, const std::shared_ptr<Signer>& signer,
                         const std::string& sigAlg, std::vector<PKCS7Attr> attrs)
{
    STACK_OF(X509)* certs = NULL;
    /* hash algorithm */
    const EVP_MD* md = NULL;
    /* entity certificate */
    X509* cert = NULL;
    int result = RET_OK;
    if (signer == NULL) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR, "signer is NULL , sign failed");
        result = INVALIDPARAM_ERROR;
        goto err;
    }
    m_signer = signer;
    m_sigAlg = sigAlg;
    certs = signer->GetCertificates();
    if (SortX509Stack(certs) < 0) {
        result = RET_FAILED;
        goto err;
    }
    if (sigAlg == SIGN_ALG_SHA384) {
        md = EVP_sha384();
    } else if (sigAlg == SIGN_ALG_SHA256) {
        md = EVP_sha256();
    } else {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            sigAlg + "is invalid sigAlg, please use SHA256withECDSA/SHA384withECDSA, sign failed");
        result = INVALIDPARAM_ERROR;
        goto err;
    }
    /* Extract the entity certificate from the certificate chain */
    cert = sk_X509_delete(certs, 0);
    m_p7 = Pkcs7Sign(cert, certs, md, content, m_flags, attrs);
    if (m_p7 == NULL) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", SIGN_ERROR, "p7 is NULL, pkcs7 sign failed");
        result = SIGN_ERROR;
        goto err;
    }
    PKCS7AddCrls(m_p7, signer->GetCrls());

err:
    sk_X509_pop_free(certs, X509_free);
    X509_free(cert);
    return result;
}

void PKCS7Data::PrintCertChainSub(const STACK_OF(X509)* certs)
{
    if (certs == NULL)
        return;
    SIGNATURE_TOOLS_LOGI("certChainSubject:");
    int certNum = sk_X509_num(certs);
    SIGNATURE_TOOLS_LOGI("certNum%s", std::to_string(certNum).c_str());
    for (int i = 0; i < certNum; i++) {
        SIGNATURE_TOOLS_LOGI("certificate %s", std::to_string(i).c_str());
        std::string sub;
        VerifyCertOpensslUtils::GetSubjectFromX509(sk_X509_value(certs, i), sub);
        SIGNATURE_TOOLS_LOGI("%s", sub.c_str());
    }
}

std::string PKCS7Data::GetASN1Time(const ASN1_TIME* tm)
{
    if (tm == NULL) {
        return "";
    }
    /* Convert the ASN1_TIME structure to a standard tm structure. */
    struct tm time;
    ASN1_TIME_to_tm(tm, &time);
    /* Convert to local time(considering the time zone) */
    time_t t = mktime(&time);
    if (t < 0) {
        return "";
    }
    struct tm* localTime = localtime(&t);
    if (localTime == nullptr) {
        return "";
    }
    /* Print local time */
    char buf[128] = {0};
    if (sprintf_s(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
                  localTime->tm_year + YEAR1900, localTime->tm_mon + 1, localTime->tm_mday,
                  localTime->tm_hour, localTime->tm_min, localTime->tm_sec) == -1) {
        return "";
    }
    return std::string(buf, strlen(buf));
}

bool PKCS7Data::X509NameCompare(const X509* cert, const X509* issuerCert)
{
    if (cert == nullptr || issuerCert == nullptr) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "input cert is NULL");
        return false;
    }
    X509_NAME* aName = X509_get_issuer_name(cert);
    X509_NAME* bName = X509_get_subject_name(issuerCert);
    if (X509_NAME_cmp(aName, bName) != 0) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "cert issuer name is not equal to its issuer\'s  name");
        return false;
    }
    return true;
}

int PKCS7Data::CheckSignTimeInValidPeriod(const ASN1_TYPE* signTime,
                                          const ASN1_TIME* notBefore, const ASN1_TIME* notAfter)
{
    if (notBefore == nullptr || notBefore->data == nullptr || notAfter == nullptr || notAfter->data == nullptr) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "invalid period, check signtime failed please use valid period to to check signtime");
        return INVALIDPARAM_ERROR;
    }
    if (signTime == nullptr || signTime->value.asn1_string == nullptr ||
        signTime->value.asn1_string->data == nullptr) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR, "signtime is NULL");
        return INVALIDPARAM_ERROR;
    }
    ASN1_TIME* tm = ASN1_TIME_new();
    ASN1_TIME_set_string(tm, (reinterpret_cast<const char*>(signTime->value.asn1_string->data)));
    if (ASN1_TIME_compare(notBefore, signTime->value.asn1_string) > 0 ||
        ASN1_TIME_compare(notAfter, signTime->value.asn1_string) < 0) {
        SIGNATURE_TOOLS_LOGE("sign time invalid, signTime: %s, notBefore: %s, "
                             "notAfter: %s", GetASN1Time(tm).c_str(),
                             GetASN1Time(notBefore).c_str(), GetASN1Time(notAfter).c_str());
        ASN1_TIME_free(tm);
        return RET_FAILED;
    }
    ASN1_TIME_free(tm);
    return RET_OK;
}

static X509* FindSubCertThenEraseItFromSets(X509* cert, std::unordered_set<X509*>& x509Sets)
{
    X509* ret = NULL;
    for (X509* c : x509Sets) {
        X509_NAME* name1 = X509_get_subject_name(cert);
        X509_NAME* name2 = X509_get_issuer_name(c);
        if (X509_NAME_cmp(name1, name2) == 0) {
            x509Sets.erase(c);
            ret = c;
            break;
        }
    }
    return ret;
}

int PKCS7Data::SortX509Stack(STACK_OF(X509)* certs)
{
    std::unordered_set<X509*> x509Sets;
    std::list<X509*>certChain;
    X509* tmp = NULL;
    int result = RET_FAILED;

    if (sk_X509_num(certs) < MIN_CERTS_NUM) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR, "cert of certchain count less than two!");
        goto err;
    }
    for (int i = 0; i < sk_X509_num(certs); i++) {
        x509Sets.insert(sk_X509_value(certs, i));
    }
    if (sk_X509_num(certs) != static_cast<int>(x509Sets.size())) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR, "sort x509 certchain failed!");
        goto err;
    }
    for (X509* cert : x509Sets) {
        if (X509_name_cmp(X509_get_subject_name(cert), X509_get_issuer_name(cert)) == 0) {
            tmp = cert;
            x509Sets.erase(cert);
            break;
        }
    }
    if (tmp == NULL) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "can't find root cert from certchain ,sort x509 certchain failed!");
        goto err;
    }
    certChain.push_front(tmp);
    while ((tmp = FindSubCertThenEraseItFromSets(tmp, x509Sets))) {
        certChain.push_front(tmp);
    }
    if (x509Sets.size() != 0) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "certchain contain invalid cert, sort x509 certchain failed!");
        goto err;
    }
    while (sk_X509_num(certs)) {
        sk_X509_pop(certs);
    }
    for (X509* cert : certChain) {
        sk_X509_push(certs, cert);
    }
    result = RET_OK;
err:
    return result;
}

int PKCS7Data::VerifySign(const std::string& content)const
{
    BIO* inBio = NULL;
    if ((m_flags & PKCS7_DETACHED)) {
        inBio = BIO_new_mem_buf(reinterpret_cast<const void*>(content.c_str()),
                                static_cast<int>(content.size()));
        if (inBio == NULL) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                                "new mem buf error!,pkcs7 verify signature failed");
            return VERIFY_ERROR;
        }
    }
    if (PKCS7_verify(m_p7, NULL, NULL, inBio, NULL, m_flags) != 1) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "pkcs7 verify signature failed");
        BIO_free(inBio);
        return VERIFY_ERROR;
    }
    BIO_free(inBio);
    return RET_OK;
}

int PKCS7Data::VerifyCertChain()const
{
    /* Validate the certificate chain */
    STACK_OF(PKCS7_SIGNER_INFO)* skSignerInfo = PKCS7_get_signer_info(m_p7);
    int signerCount = sk_PKCS7_SIGNER_INFO_num(skSignerInfo);
    int c = signerCount;
    STACK_OF(X509)* certs = NULL;
    int result = RET_FAILED;
    /* Original certificate chain */
    STACK_OF(X509)* certChain = m_p7->d.sign->cert;
    /* Copy of the certificate chain, with the entity certificate removed later */
    certs = sk_X509_dup(certChain);
    if (SortX509Stack(certs) < 0) {
        SIGNATURE_TOOLS_LOGE("sort x509 stack failed, verify certchain failed");
        goto err;
    }
    /* Retrieve the certificate chain without the entity certificate */
    while (c--) {
        sk_X509_delete(certs, 0);
    }
    for (int i = 0; i < signerCount; i++) {
        PKCS7_SIGNER_INFO* signerInfo = sk_PKCS7_SIGNER_INFO_value(skSignerInfo, i);
        if ((result = VerifySignerInfoCertchain(m_p7, signerInfo, certs, certChain)) < 0) {
            SIGNATURE_TOOLS_LOGE("verify certchain failed");
            goto err;
        }
    }
    result = RET_OK;
err:
    sk_X509_free(certs);
    return result;
}

int PKCS7Data::CheckSginerInfoSignTimeInCertChainValidPeriod(PKCS7_SIGNER_INFO* signerInfo,
                                                             STACK_OF(X509)* certs) const
{
    if (signerInfo == NULL || certs == NULL) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR, "input is NULL, check signtime invalid");
        return INVALIDPARAM_ERROR;
    }
    ASN1_TYPE* signTime = PKCS7_get_signed_attribute(signerInfo, NID_pkcs9_signingTime);
    for (int i = 0; i < sk_X509_num(certs); i++) {
        X509* cert = sk_X509_value(certs, i);
        const ASN1_TIME* notBefore = X509_get0_notBefore(cert);
        const ASN1_TIME* notAfter = X509_get0_notAfter(cert);
        if (CheckSignTimeInValidPeriod(signTime, notBefore, notAfter) < 0) {
            SIGNATURE_TOOLS_LOGE("pkcs7 sign time check failed");
            return INVALIDPARAM_ERROR;
        }
    }
    return RET_OK;
}

int PKCS7Data::VerifySignerInfoCertchain(PKCS7* pkcs7, PKCS7_SIGNER_INFO* signerInfo,
                                         STACK_OF(X509)* certs, STACK_OF(X509)* certChain)const
{
    X509* sigCert = PKCS7_cert_from_signer_info(pkcs7, signerInfo);
    int j = 0;
    /* Trace back through the subject information and validate the signature value of each certificate */
    if (!X509NameCompare(sigCert, sk_X509_value(certs, 0))) {
        SIGNATURE_TOOLS_LOGE("entity name compare not equal, verify failed");
        return VERIFY_ERROR;
    }
    /* verify entity certificate signature value */
    if (!VerifyCertOpensslUtils::CertVerify(sigCert, sk_X509_value(certs, 0))) {
        SIGNATURE_TOOLS_LOGE("entity cert signature verify failed");
        return VERIFY_ERROR;
    }
    for (; j + 1 < sk_X509_num(certs); j++) {
        if (!X509NameCompare(sk_X509_value(certs, j), sk_X509_value(certs, j + 1))) {
            SIGNATURE_TOOLS_LOGE("sub cert name compare not equal, verify failed");
            return VERIFY_ERROR;
        }
        /* Verify the signature value of the intermediate certificate */
        if (!VerifyCertOpensslUtils::CertVerify(sk_X509_value(certs, j), sk_X509_value(certs, j + 1))) {
            SIGNATURE_TOOLS_LOGE("sub cert signature verify failed");
            return VERIFY_ERROR;
        }
    }
    if (!X509NameCompare(sk_X509_value(certs, j), sk_X509_value(certs, j))) {
        SIGNATURE_TOOLS_LOGE("root cert name compare not equal, verify failed");
        return VERIFY_ERROR;
    }
    /* Verify the signature value of the root certificate */
    if (!VerifyCertOpensslUtils::CertVerify(sk_X509_value(certs, j), sk_X509_value(certs, j))) {
        SIGNATURE_TOOLS_LOGE("root cert signature verify failed");
        return VERIFY_ERROR;
    }
    /* Verify that the signature time in the signature information is within the validity period of
    the certificate chain (entity certificate will be verified in PKCS7_verify) */
    if (CheckSginerInfoSignTimeInCertChainValidPeriod(signerInfo, certChain) < 0) {
        SIGNATURE_TOOLS_LOGE("sign time is invalid,verify failed");
        return VERIFY_ERROR;
    }
    return RET_OK;
}

int PKCS7Data::Pkcs7SignAttr(PKCS7_SIGNER_INFO* info)
{
    unsigned char* attrBuf = NULL;
    int attrLen;

    std::string data;
    std::string signature;
    unsigned char* sigRet = NULL;
    int sigLen = 0;

    attrLen = ASN1_item_i2d(reinterpret_cast<ASN1_VALUE*>(info->auth_attr), &attrBuf,
                            ASN1_ITEM_rptr(PKCS7_ATTR_SIGN));
    if (!attrBuf) {
        OPENSSL_free(attrBuf);
        return 0;
    }

    data.assign(reinterpret_cast<const char*>(attrBuf), attrLen);
    signature = m_signer->GetSignature(data, m_sigAlg);
    if (signature.empty()) {
        OPENSSL_free(attrBuf);
        return 0;
    }
    sigLen = signature.size();
    sigRet = reinterpret_cast<unsigned char*>(OPENSSL_malloc(sigLen));
    if (sigRet == NULL) {
        OPENSSL_free(attrBuf);
        return 0;
    }
    std::copy(&signature[0], &signature[0] + signature.size(), sigRet);
    ASN1_STRING_set0(info->enc_digest, sigRet, sigLen);
    OPENSSL_free(attrBuf);
    return 1;
}

static ASN1_OCTET_STRING* PKCS7GetASN1Content(PKCS7* pkcs7)
{
    if (PKCS7_type_is_data(pkcs7)) {
        return pkcs7->d.data;
    }
    return NULL;
}

int PKCS7Data::Pkcs7AddTimeDigestAndSignAttr(PKCS7_SIGNER_INFO* info, EVP_MD_CTX* hashCtx)
{
    unsigned char hashData[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    /* add signing time */
    if (!PKCS7_get_signed_attribute(info, NID_pkcs9_signingTime)) {
        if (!PKCS7_add0_attrib_signing_time(info, NULL)) {
            return 0;
        }
    }

    /* add digest */
    if (!EVP_DigestFinal_ex(hashCtx, hashData, &hashLen)) {
        return 0;
    }
    if (!PKCS7_add1_attrib_digest(info, hashData, hashLen)) {
        return 0;
    }

    /* sign the attributes */
    if (!Pkcs7SignAttr(info)) {
        return 0;
    }

    return 1;
}

static BIO* PKCS7SearchDigest(EVP_MD_CTX** pHash, BIO* io, int numberID)
{
    while ((io = BIO_find_type(io, BIO_TYPE_MD))) {
        BIO_get_md_ctx(io, pHash);
        if (*pHash == NULL) {
            return NULL;
        }
        if (EVP_MD_CTX_type(*pHash) == numberID) {
            return io;
        }
        io = BIO_next(io);
    }
    return NULL;
}

static int PKCS7DataFinalCheck(PKCS7* pkcs7, BIO* bio,
                               STACK_OF(PKCS7_SIGNER_INFO)** psk, ASN1_OCTET_STRING** pos)
{
    int id = 0;

    if (pkcs7 == NULL || pkcs7->d.ptr == NULL) {
        return 0;
    }

    id = OBJ_obj2nid(pkcs7->type);
    pkcs7->state = PKCS7_S_HEADER;

    if (id == NID_pkcs7_signed) {
        *psk = pkcs7->d.sign->signer_info;
        *pos = PKCS7GetASN1Content(pkcs7->d.sign->contents);
        if (PKCS7_type_is_data(pkcs7->d.sign->contents) && pkcs7->detached) {
            ASN1_OCTET_STRING_free(*pos);
            *pos = NULL;
            pkcs7->d.sign->contents->d.data = NULL;
        }
        return 1;
    }
    return 0;
}

int PKCS7Data::Pkcs7DataFinalSignAttr(STACK_OF(PKCS7_SIGNER_INFO)* infoStack, BIO* bio)
{
    EVP_MD_CTX* hashCtx = NULL;
    STACK_OF(X509_ATTRIBUTE)* attrStack = NULL;
    BIO* ioTmp = NULL;
    int result = 0;
    EVP_MD_CTX* ctxTmp = EVP_MD_CTX_new();
    if (ctxTmp == NULL) {
        return 0;
    }

    if (infoStack == NULL) {
        goto err;
    }
    for (int i = 0; i < sk_PKCS7_SIGNER_INFO_num(infoStack); i++) {
        PKCS7_SIGNER_INFO* info = sk_PKCS7_SIGNER_INFO_value(infoStack, i);

        int numberID = OBJ_obj2nid(info->digest_alg->algorithm);

        ioTmp = bio;

        ioTmp = PKCS7SearchDigest(&hashCtx, ioTmp, numberID);

        if (ioTmp == NULL || !EVP_MD_CTX_copy_ex(ctxTmp, hashCtx)) {
            goto err;
        }

        attrStack = info->auth_attr;

        if (sk_X509_ATTRIBUTE_num(attrStack) > 0) {
            if (!Pkcs7AddTimeDigestAndSignAttr(info, ctxTmp)) {
                goto err;
            }
        } else {
            goto err;
        }
    }
    result = 1;
err:
    EVP_MD_CTX_free(ctxTmp);
    return result;
}

static int PKCS7DataFinalSetContent(PKCS7* pkcs7, ASN1_OCTET_STRING* asn1Str, BIO* io)
{
    BIO* ioTmp = NULL;
    if (!PKCS7_is_detached(pkcs7)) {
        if (asn1Str == NULL) {
            return 0;
        }
        if (!(asn1Str->flags & ASN1_STRING_FLAG_NDEF)) {
            char* contentData;
            long contentLen;
            ioTmp = BIO_find_type(io, BIO_TYPE_MEM);
            if (ioTmp == NULL) {
                return 0;
            }
            contentLen = BIO_get_mem_data(ioTmp, &contentData);

            BIO_set_flags(ioTmp, BIO_FLAGS_MEM_RDONLY);
            BIO_set_mem_eof_return(ioTmp, 0);
            ASN1_STRING_set0(asn1Str, reinterpret_cast<unsigned char*>(contentData), contentLen);
        }
    }
    return 1;
}
int PKCS7Data::Pkcs7DataFinal(PKCS7* pkcs7, BIO* io)
{
    STACK_OF(PKCS7_SIGNER_INFO)* infoStack = NULL;
    ASN1_OCTET_STRING* os = NULL;

    if (!PKCS7DataFinalCheck(pkcs7, io, &infoStack, &os) ||
        !Pkcs7DataFinalSignAttr(infoStack, io) ||
        !PKCS7DataFinalSetContent(pkcs7, os, io)) {
        return 0;
    }
    return 1;
}

int PKCS7Data::Pkcs7Final(PKCS7* pkcs7, const std::string& content, int flags)
{
    BIO* p7bio;
    int result = 0;

    if ((p7bio = PKCS7_dataInit(pkcs7, NULL)) == NULL) {
        return 0;
    }

    if (BIO_write(p7bio, content.c_str(), static_cast<int>(content.size())) <= 0) {
        SIGNATURE_TOOLS_LOGE("add json data to pkcs7 failed");
        goto err;
    }

    (void)BIO_flush(p7bio);

    if (!Pkcs7DataFinal(pkcs7, p7bio)) {
        goto err;
    }
    /* Verify the signature value */
    if (VerifySignature(pkcs7, p7bio) < 0) {
        goto err;
    }
    result = 1;

err:
    BIO_free_all(p7bio);
    return result;
}

static int Pkcs7SetSignerInfo(PKCS7_SIGNER_INFO* info, X509* cert, const EVP_MD* hash)
{
    if (!ASN1_INTEGER_set(info->version, 1) ||
        !X509_NAME_set(&info->issuer_and_serial->issuer, X509_get_issuer_name(cert))) {
        return 0;
    }

    ASN1_INTEGER_free(info->issuer_and_serial->serial);
    if (!(info->issuer_and_serial->serial =
          ASN1_INTEGER_dup(X509_get_serialNumber(cert)))) {
        return 0;
    }

    X509_ALGOR_set0(info->digest_alg, OBJ_nid2obj(EVP_MD_type(hash)),
                    V_ASN1_NULL, NULL);

    if (!SetSignerInfoSignAlgor(info)) {
        return 0;
    }
    return 1;
}

static PKCS7_SIGNER_INFO* Pkcs7AddSignature(PKCS7* pkcs7, X509* cert, const EVP_MD* hash)
{
    PKCS7_SIGNER_INFO* info = NULL;

    if (!(info = PKCS7_SIGNER_INFO_new()) ||
        !Pkcs7SetSignerInfo(info, cert, hash) ||
        !PKCS7_add_signer(pkcs7, info)) {
        goto err;
    }
    return info;
err:
    PKCS7_SIGNER_INFO_free(info);
    return NULL;
}


static PKCS7_SIGNER_INFO* Pkcs7AddSignerInfo(PKCS7* pkcs7, X509* entityCert, const EVP_MD* hash, int flags)
{
    PKCS7_SIGNER_INFO* info = NULL;
    if ((info = Pkcs7AddSignature(pkcs7, entityCert, hash)) == NULL) {
        return NULL;
    }
    if (!PKCS7_add_certificate(pkcs7, entityCert)) {
        return NULL;
    }
    if (!PKCS7_add_attrib_content_type(info, NULL)) {
        return NULL;
    }
    return info;
}

PKCS7* PKCS7Data::Pkcs7Sign(X509* entityCert, STACK_OF(X509)* certs, const EVP_MD* hash,
                            const std::string& content, int flags, const std::vector<PKCS7Attr>& attrs)
{
    PKCS7* pkcs7;

    if (!(pkcs7 = PKCS7_new()) ||
        !PKCS7_set_type(pkcs7, NID_pkcs7_signed) ||
        !PKCS7_content_new(pkcs7, NID_pkcs7_data) ||
        !Pkcs7AddSignerInfo(pkcs7, entityCert, hash, flags) ||
        (PKCS7AddAttribute(pkcs7, attrs) < 0)) {
        PKCS7_free(pkcs7);
        return NULL;
    }

    if (!(flags & PKCS7_NOCERTS)) {
        for (int i = 0; i < sk_X509_num(certs); i++) {
            if (!PKCS7_add_certificate(pkcs7, sk_X509_value(certs, i))) {
                PKCS7_free(pkcs7);
                return NULL;
            }
        }
    }

    if (flags & PKCS7_DETACHED) {
        PKCS7_set_detached(pkcs7, 1);
    }

    if (Pkcs7Final(pkcs7, content, flags)) {
        return pkcs7;
    }
    PKCS7_free(pkcs7);
    return NULL;
}
} // namespace SignatureTools
} // namespace OHOS