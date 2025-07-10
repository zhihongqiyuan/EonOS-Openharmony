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

#ifndef SIGNATRUETOOLS_OPTIONS_H
#define SIGNATRUETOOLS_OPTIONS_H

#include <initializer_list>
#include <unordered_map>
#include <variant>
#include <string>

#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
class Options : public std::unordered_map<std::string, std::variant<std::string, int, bool, char*>> {
public:
    Options() = default;
    ~Options() = default;
    char* GetChars(const std::string& key);
    std::string GetString(const std::string& key);
    std::string GetString(const std::string& key, const std::string& checkStr);
    int GetInt(const std::string& key);
    bool Equals(const std::string& argf, const std::string& args);
    bool Required(const std::initializer_list<std::string>& keys);
    bool IsEmpty(const std::string& cs);
    bool GetBool(const std::string& key);

public:
    /* Key alias parameter name. */
    static const std::string KEY_ALIAS;

    /* Key right parameter name. */
    static const std::string KEY_RIGHTS;

    /* Key size parameter name(NIST-P-256, NIST-P-384). */
    static const std::string KEY_SIZE;

    /* Key alg parameter name(ECC). */
    static const std::string KEY_ALG;

    /* Keystore file parameter name. */
    static const std::string KEY_STORE_FILE;

    /* Keystore right parameter name. */
    static const std::string KEY_STORE_RIGHTS;

    /* Issuer key alias parameter name. */
    static const std::string ISSUER_KEY_ALIAS;

    /* Issuer key right parameter name. */
    static const std::string ISSUER_KEY_RIGHTS;

    /* Issuer Key Store File parameter name. */
    static const std::string ISSUER_KEY_STORE_FILE;

    /* Issuer Key Store Pwd parameter name. */
    static const std::string ISSUER_KEY_STORE_RIGHTS;

    /* issuer subject. */
    static const  std::string ISSUER;

    /* issuer subject. */
    static const  std::string INFORM;

    /* certificate subject. */
    static const  std::string SUBJECT;

    /* the validity period of the certificate. */
    static const  std::string VALIDITY;

    /* signature algorithm. */
    static const  std::string  SIGN_ALG;

    /* length of path. */
    static const  std::string BASIC_CONSTRAINTS_PATH_LEN;

    /* outfile. */
    static const std::string OUT_FILE;

    /* End file type of cert. values in: cert / certChain */
    static const std::string OUT_FORM;

    /* Sub cert for sig. */
    static const std::string SUB_CA_CERT_FILE;

    /* Ca cert file parameter name. */
    static const std::string CA_CERT_FILE;

    /* In file parameter name. */
    static const std::string IN_FILE;
    static const std::string PROFILE_CERT_FILE;
    static const std::string APP_CERT_FILE;
    static const std::string KEY_USAGE;
    static const std::string EXT_KEY_USAGE;
    static const std::string KEY_USAGE_CRITICAL;
    static const std::string  EXT_KEY_USAGE_CRITICAL;
    static const std::string  BASIC_CONSTRAINTS;
    static const std::string  BASIC_CONSTRAINTS_CRITICAL;
    static const std::string BASIC_CONSTRAINTS_CA;

    /* Out form includes all forms. */
    static const std::string OUT_FORM_SCOPE;
    static const std::string MODE;
    static const std::string OUT_CERT_CHAIN;
    static const std::string OUT_PROFILE;
    static const std::string PROOF_FILE;
    static const std::string PROFILE_FILE;
    static const std::string PROFILE_SIGNED;
    static const std::string MODULE_FILE;
    static const std::string AD_HOC;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_OPTIONS_H
