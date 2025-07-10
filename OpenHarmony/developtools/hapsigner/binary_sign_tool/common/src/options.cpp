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

#include "options.h"

namespace OHOS {
namespace SignatureTools {

/* Initializes the static member constant. */
const std::string Options::KEY_ALIAS = "keyAlias";
const std::string Options::KEY_RIGHTS = "keyPwd";
const std::string Options::KEY_ALG = "keyAlg";
const std::string Options::KEY_SIZE = "keySize";
const std::string Options::KEY_STORE_FILE = "keystoreFile";
const std::string Options::KEY_STORE_RIGHTS = "keystorePwd";
const std::string Options::ISSUER_KEY_ALIAS = "issuerKeyAlias";
const std::string Options::ISSUER_KEY_RIGHTS = "issuerKeyPwd";
const std::string Options::ISSUER_KEY_STORE_FILE = "issuerKeystoreFile";
const std::string Options::ISSUER_KEY_STORE_RIGHTS = "issuerKeystorePwd";
const std::string Options::ISSUER = "issuer";
const std::string Options::SUBJECT = "subject";
const std::string Options::VALIDITY = "validity";
const std::string Options::SIGN_ALG = "signAlg";
const std::string Options::BASIC_CONSTRAINTS_PATH_LEN = "basicConstraintsPathLen";
const std::string Options::OUT_FILE = "outFile";
const std::string Options::OUT_FORM = "outForm";
const std::string Options::SUB_CA_CERT_FILE = "subCaCertFile";
const std::string Options::CA_CERT_FILE = "rootCaCertFile";
const std::string Options::IN_FILE = "inFile";
const std::string Options::PROFILE_CERT_FILE = "profileCertFile";
const std::string Options::APP_CERT_FILE = "appCertFile";
const std::string Options::KEY_USAGE = "keyUsage";
const std::string Options::EXT_KEY_USAGE = "extKeyUsage";
const std::string Options::KEY_USAGE_CRITICAL = "keyUsageCritical";
const std::string Options::EXT_KEY_USAGE_CRITICAL = "extKeyUsageCritical";
const std::string Options::BASIC_CONSTRAINTS_CA = "basicConstraintsCa";
const std::string Options::BASIC_CONSTRAINTS_CRITICAL = "basicConstraintsCritical";
const std::string Options::BASIC_CONSTRAINTS = "basicConstraints";
const std::string Options::OUT_FORM_SCOPE = "cert,certChain";
const std::string Options::MODE = "mode";
const std::string Options::INFORM = "inForm";
const std::string Options::OUT_CERT_CHAIN = "outCertChain";
const std::string Options::OUT_PROFILE = "outProfile";
const std::string Options::PROOF_FILE = "outproof";
const std::string Options::PROFILE_FILE = "profileFile";
const std::string Options::PROFILE_SIGNED = "profileSigned";
const std::string Options::MODULE_FILE = "moduleFile";
const std::string Options::AD_HOC = "adHoc";

char* Options::GetChars(const std::string& key)
{
    if (this->count(key) == 0) {
        return nullptr;
    }

    auto value = (*this)[key];
    char** charsPtr = std::get_if<char*>(&value);
    if (charsPtr == nullptr) {
        SIGNATURE_TOOLS_LOGI("GetChars key = %s  value = nullptr !", key.c_str());
        return nullptr;
    }
    return *charsPtr;
}

std::string Options::GetString(const std::string& key)
{
    if (this->count(key) == 0) {
        return "";
    }

    auto value = (*this)[key];
    std::string* stringPtr = std::get_if<std::string>(&value);
    if (stringPtr == nullptr) {
        SIGNATURE_TOOLS_LOGI("GetString  key = %s value = """, key.c_str());
        return "";
    }
    return *stringPtr;
}

std::string Options::GetString(const std::string& key, const std::string& checkStr)
{
    if (this->count(key) == 0) {
        return "";
    }

    auto value = (*this)[key];
    std::string* stringPtr = std::get_if<std::string>(&value);
    if (stringPtr == nullptr || *stringPtr == "") {
        SIGNATURE_TOOLS_LOGI("GetString  key = %s value = %s ", key.c_str(), checkStr.c_str());
        return checkStr;
    }
    return *stringPtr;
}

int Options::GetInt(const std::string& key)
{
    if (this->count(key) == 0) {
        return 0;
    }

    auto value = (*this)[key];
    int* stringPtr = std::get_if<int>(&value);
    if (stringPtr == nullptr) {
        SIGNATURE_TOOLS_LOGI("GetInt  key = %s value = 0 ", key.c_str());
        return 0;
    }
    return *stringPtr;
}

bool Options::Equals(const std::string& argf, const std::string& args)
{
    std::string ksFile = GetString(argf);
    std::string iksFile = GetString(args);
    if (ksFile == iksFile) {
        return true;
    }
    return  false;
}

bool Options::Required(const std::initializer_list<std::string>& keys)
{
    for (auto& key : keys) {
        if (!this->IsEmpty(key) && !(this->find(key) != this->end())) {
            PrintErrorNumberMsg("COMMAND_ERROR", COMMAND_ERROR, "Params '-" + key + "' is required");
            return false;
        }
    }
    return true;
}

bool Options::IsEmpty(const std::string& cs)
{
    if (cs.empty()) {
        return true;
    }
    return false;
}

bool Options::GetBool(const std::string& key)
{
    auto value = (*this)[key];
    bool* stringPtr = std::get_if<bool>(&value);
    return *stringPtr;
}
} // namespace SignatureTools
} // namespace OHOS