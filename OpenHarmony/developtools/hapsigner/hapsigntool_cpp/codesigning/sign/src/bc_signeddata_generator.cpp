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
#include <vector>

#include "constant.h"
#include "pkcs7_data.h"
#include "local_signer.h"
#include "signer_config.h"
#include "signature_tools_log.h"
#include "signature_algorithm_helper.h"
#include "signature_tools_errno.h"
#include "bc_signeddata_generator.h"

namespace OHOS {
namespace SignatureTools {

int BCSignedDataGenerator::GenerateSignedData(const std::string& content,
                                              SignerConfig* signerConfig, std::string& ret)
{
    std::string sigAlg;
    int result = RET_OK;
    if (content.empty()) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "digest content is empty, generate signed data failed");
        return INVALIDPARAM_ERROR;
    }
    if (signerConfig == NULL) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "signerConfig is NULL");
        return INVALIDPARAM_ERROR;
    }
    std::shared_ptr<Signer> signer = signerConfig->GetSigner();
    if (signer == NULL) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "signer is NULL");
        return INVALIDPARAM_ERROR;
    }
    result = GetSigAlg(signerConfig, sigAlg);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("get sign algorithm from signerConfig failed");
        return INVALIDPARAM_ERROR;
    }
    result = PackageSignedData(content, signer, sigAlg, ret);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("package signed data failed");
        return result;
    }
    return result;
}

void BCSignedDataGenerator::SetOwnerId(const std::string& ownerID)
{
    m_ownerID = ownerID;
}

int BCSignedDataGenerator::PackageSignedData(const std::string& content, const std::shared_ptr<Signer>& signer,
                                             const std::string& sigAlg, std::string& ret)
{
    int result = RET_OK;
    PKCS7Data p7Data(PKCS7_DETACHED_FLAGS);
    std::vector<PKCS7Attr> attrs;
    if (m_ownerID.empty() == false) {
        if (AddOwnerID(attrs, m_ownerID) < 0) {
            PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                                "create ownerIDOid failed");
            return INVALIDPARAM_ERROR;
        }
    }
    // Generate PKCS#7
    result = p7Data.Sign(content, signer, sigAlg, ret, attrs);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("generate pkcs7 signed data block failed");
        return SIGN_ERROR;
    }
    // Verify after parsing
    result = p7Data.Parse(ret);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("verify pkcs7 signed data block bytes failed");
        return PARSE_ERROR;
    }
    result = p7Data.Verify(content);
    if (result < 0) {
        SIGNATURE_TOOLS_LOGE("verify pkcs7 signed data block failed");
        return VERIFY_ERROR;
    }
    return result;
}

int BCSignedDataGenerator::GetSigAlg(SignerConfig* signerConfig, std::string& sigAlg)
{
    std::vector<SignatureAlgorithmHelper> sigs = signerConfig->GetSignatureAlgorithms();
    if (sigs.size() != 1) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR, "sigAlg count not equal to 1");
        return INVALIDPARAM_ERROR;
    }
    SignatureAlgorithmHelper signatureAlg = sigs[0];
    if (signatureAlg.m_id == SignatureAlgorithmId::ECDSA_WITH_SHA256) {
        sigAlg = SIGN_ALG_SHA256;
    } else if (signatureAlg.m_id == SignatureAlgorithmId::ECDSA_WITH_SHA384) {
        sigAlg = SIGN_ALG_SHA384;
    } else {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            sigAlg + "is invalid sigAlg, please use SHA256withECDSA/SHA384withECDSA, sign failed");
        return NOT_SUPPORT_ERROR;
    }
    return RET_OK;
}

int BCSignedDataGenerator::CreateNIDFromOID(const std::string& oid, const std::string& shortName,
                                            const std::string& longName)
{
    int nid = OBJ_txt2nid(oid.c_str());
    if (nid == NID_undef) {
        nid = OBJ_create(oid.c_str(), shortName.c_str(), longName.c_str());
    }
    return nid;
}

int BCSignedDataGenerator::AddOwnerID(std::vector<PKCS7Attr>& attrs, const std::string& ownerID)
{
    PKCS7Attr attr;
    int nid = CreateNIDFromOID(OWNERID_OID, OWNERID_OID_SHORT_NAME, OWNERID_OID_LONG_NAME);
    if (nid == NID_undef) {
        return RET_FAILED;
    }
    ASN1_STRING* ownerIDAsn1 = ASN1_STRING_new();
    if (ownerIDAsn1 == NULL) {
        SIGNATURE_TOOLS_LOGE("asn1 string create error!\n");
        return RET_FAILED;
    }
    ASN1_STRING_set(ownerIDAsn1, ownerID.c_str(), ownerID.length());
    attr.nid = nid;
    attr.atrtype = V_ASN1_UTF8STRING;
    attr.value = ownerIDAsn1;
    attrs.push_back(attr);
    return RET_OK;
}
} // namespace SignatureTools
} // namespace OHOS