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
#ifndef SIGNATURETOOLS_PARAM_CONSTANTS_H
#define SIGNATURETOOLS_PARAM_CONSTANTS_H
#include <string>

namespace OHOS {
namespace SignatureTools {
class ParamConstants {
public:
    const static int HAP_FORMAT_ERROR = 20001;
    const static int HAP_PARSE_ERROR = 20002;
    const static int HAP_SIGNATURE_ERROR = 20003;
    const static int HAP_SIGNATURE_NOT_FOUND_ERROR = 20004;
    static const std::string HAP_SIG_SCHEME_V256_DIGEST_ALGORITHM;
    static const std::string HAP_SIG_SCHEME_V384_DIGEST_ALGORITHM;
    static const std::string HAP_SIG_SCHEME_V512_DIGEST_ALGORITHM;
    static const std::string HAP_SIG_ALGORITHM_SHA256_ECDSA;
    static const std::string HAP_SIG_ALGORITHM_SHA384_ECDSA;
    static const std::string HAP_SIG_ALGORITHM_SHA512_ECDSA;
    static const std::string ALIGNMENT;
    static const std::string PARAM_SIGN_MODE;
    static const std::string PARAM_BASIC_CRL;
    static const std::string PARAM_BASIC_PROPERTY;
    static const std::string PARAM_BASIC_PROFILE;
    static const std::string PARAM_PROFILE_JSON_CONTENT;
    static const std::string PARAM_BASIC_PROOF;
    static const std::string PARAM_BASIC_ALIGNMENT;
    static const std::string PARAM_BASIC_PRIVATE_KEY;
    static const std::string PARAM_BASIC_INPUT_FILE;
    static const std::string PARAM_BASIC_OUTPUT_FILE;
    static const std::string PARAM_BASIC_SIGANTURE_ALG;
    static const std::string PARAM_BASIC_PROFILE_SIGNED;
    static const std::string PARAM_BASIC_COMPATIBLE_VERSION;
    static const std::string PARAM_REMOTE_SERVER;
    static const std::string PARAM_REMOTE_USERNAME;
    static const std::string PARAM_REMOTE_USERPWD;
    static const std::string PARAM_REMOTE_CODE;
    static const std::string PARAM_REMOTE_ONLINEAUTHMODE;
    static const std::string PARAM_REMOTE_SIGNERPLUGIN;
    static const std::string PARAM_LOCAL_JKS_KEYSTORE;
    static const std::string PARAM_LOCAL_JKS_KEYSTORE_CODE;
    static const std::string PARAM_LOCAL_JKS_KEYALIAS_CODE;
    static const std::string PARAM_LOCAL_PUBLIC_CERT;
    static const std::string PARAM_VERIFY_CERTCHAIN_FILE;
    static const std::string PARAM_VERIFY_PROFILE_FILE;
    static const std::string PARAM_VERIFY_PROOF_FILE;
    static const std::string PARAM_VERIFY_PROPERTY_FILE;
    static const std::string PARAM_RESIGN_CONFIG_FILE;
    static const std::string PARAM_IN_FORM;
    static const std::string PARAM_SIGN_CODE;
    static const std::string PARAM_AD_HOC;
    static const std::string PARAM_MODULE_FILE;
    static constexpr int FILE_NAME_MIN_LENGTH = 2;
    static const std::string DISABLE_SIGN_CODE;
    static const std::string ENABLE_SIGN_CODE;
    static const std::string AD_HOC_TYPE_0;
    static const std::string AD_HOC_TYPE_1;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_PARAM_CONSTANTS_H
