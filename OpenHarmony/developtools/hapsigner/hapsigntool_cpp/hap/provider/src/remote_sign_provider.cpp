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
#include "remote_sign_provider.h"

namespace OHOS {
namespace SignatureTools {
bool RemoteSignProvider::CheckParams(Options* options)
{
    if (!SignProvider::CheckParams(options)) {
        SIGNATURE_TOOLS_LOGE("SignProvider::Parameter check failed !");
        return false;
    }
    // The following code is for reference only.
    std::vector<std::string> paramFileds;
    paramFileds.emplace_back(ParamConstants::PARAM_REMOTE_SERVER);
    paramFileds.emplace_back(ParamConstants::PARAM_REMOTE_USERNAME);
    paramFileds.emplace_back(ParamConstants::PARAM_REMOTE_USERPWD);
    paramFileds.emplace_back(ParamConstants::PARAM_REMOTE_ONLINEAUTHMODE);
    paramFileds.emplace_back(ParamConstants::PARAM_REMOTE_SIGNERPLUGIN);
    std::unordered_set<std::string> paramSet = Params::InitParamField(paramFileds);
    for (auto it = options->begin(); it != options->end(); it++) {
        if (paramSet.find(it->first) != paramSet.end()) {
            size_t size = it->first.size();
            std::string str = it->first.substr(size - 3);
            if (str == "Pwd") {
                signParams.insert(std::make_pair(it->first, ""));
            } else {
                signParams.insert(std::make_pair(it->first, options->GetString(it->first)));
            }
        }
    }
    for (const auto& param : paramFileds) {
        if (signParams.find(param) == signParams.end()) {
            PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                                "Missing parameter:" + param);
            return false;
        }
    }
    return true;
}

bool RemoteSignProvider::CheckInputCertMatchWithProfile(X509* inputCert, X509* certInProfile) const
{
    bool ret = true;
    if (inputCert == nullptr || certInProfile == nullptr) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "The certificate is empty");
        return false;
    }
    X509_NAME* subject1 = X509_get_subject_name(inputCert);
    X509_NAME* subject2 = X509_get_subject_name(certInProfile);
    if (X509_NAME_cmp(subject1, subject2) != 0) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "The subject does not match!");
        return false;
    }
    X509_NAME* issuer1 = X509_get_issuer_name(inputCert);
    X509_NAME* issuer2 = X509_get_issuer_name(certInProfile);
    if (X509_NAME_cmp(issuer1, issuer2) != 0) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "The issuer name does not match!");
        return false;
    }
    ASN1_INTEGER* serial1 = X509_get_serialNumber(inputCert);
    ASN1_INTEGER* serial2 = X509_get_serialNumber(certInProfile);
    if (ASN1_INTEGER_cmp(serial1, serial2) != 0) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "serial number does not match!");
        return false;
    }
    EVP_PKEY* pkey1 = X509_get_pubkey(inputCert);
    EVP_PKEY* pkey2 = X509_get_pubkey(certInProfile);
    if (pkey1 && pkey2 && EVP_PKEY_cmp(pkey1, pkey2) != 1) {
        EVP_PKEY_free(pkey1);
        EVP_PKEY_free(pkey2);
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "The public key does not match!");
        return false;
    }
    if (!pkey1 || !pkey2) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "The public key is null!");
        ret = false;
    }
    if (pkey1) EVP_PKEY_free(pkey1);
    if (pkey2) EVP_PKEY_free(pkey2);
    return ret;
}
} // namespace SignatureTools
} // namespace OHOS