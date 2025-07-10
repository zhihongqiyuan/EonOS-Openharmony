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

#ifndef SIGNATRUETOOLS_CONSTANT_H
#define SIGNATRUETOOLS_CONSTANT_H

#include <stdint.h>
#include <string>

namespace OHOS {
namespace SignatureTools {
const char APP_SIGNING_CAPABILITY[] = {0x30, 0x06, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x00};
const char PROFILE_SIGNING_CAPABILITY[] = {0x30, 0x06, 0x02, 0x01, 0x01, 0x0A, 0x01, 0x01};

constexpr long DEFAULT_START_VALIDITY = 0;
constexpr long ONE_DAY_TIME = 86400;
constexpr long DEFAULT_CERT_VERSION = 2;

constexpr int NIST_P_256 = 256;
constexpr int NIST_P_384 = 384;
constexpr int DEFAULT_VALIDITY_DAYS = 3650;
constexpr int INVALIDCHAR = 3;
constexpr int DEFAULT_BASIC_CONSTRAINTS_PATH_LEN = 0;
constexpr int RANDOM_SERIAL_NUMBER_LENGTH = 32;
constexpr int DEFAULT_CUSTOM_VALIDITY_DAYS = 1095;

const bool DEFAULT_KEY_USAGE_CRITICAL = true;
const bool DEFAULT_EXT_KEY_USAGE_CRITICAL = false;
const bool DEFAULT_BASIC_CONSTRAINTS = false;
const bool DEFAULT_BASIC_CONSTRAINTS_CRITICAL = false;
const bool DEFAULT_BASIC_CONSTRAINTS_CA = false;

const std::string SIGN_ALG_SHA256 = "SHA256withECDSA";
const std::string SIGN_ALG_SHA384 = "SHA384withECDSA";
const std::string DEFAULT_BASIC_EXTENSION = "critical,CA:FALSE";
const std::string DEFAULT_KEYUSAGE_EXTENSION = "digitalSignature";
const std::string DEFAULT_EXTEND_KEYUSAGE = "codeSigning";
const std::string NID_BASIC_CONST = "basicConstraints";
const std::string NID_KEYUSAGE_CONST = "keyUsage";
const std::string NID_EXT_KEYUSAGE_CONST = "extendedKeyUsage";
const std::string PKCS7_EXT_SIGNATURE_OID = "1.2.840.113549.1.1.10";
const std::string X509_EXT_OID = "1.3.6.1.4.1.2011.2.376.1.3";
const std::string OWNERID_OID = "1.3.6.1.4.1.2011.2.376.1.4.1"; // SIGNED_ID
const std::string OWNERID_OID_SHORT_NAME = "ownerID";
const std::string OWNERID_OID_LONG_NAME = "Code Signature Owner ID";
const std::string DEFAULT_PROFILE_SIGNED_1 = "1";
const std::string DEFAULT_PROFILE_SIGNED_0 = "0";
const std::string ZIP = "zip";
const std::string ELF = "elf";
const std::string BIN = "bin";
const std::string OUT_FORM_CERT = "cert";
const std::string OUT_FORM_CERT_CHAIN = "certChain";
const std::string DEBUG_STR = "debug";
const std::string DEBUG_LIB_ID = "DEBUG_LIB_ID";
const std::string REMOTE_SIGN = "remoteSign";
const std::string LOCAL_SIGN = "localSign";

// Corresponding to each functional module
const std::string GENERATE_KEYPAIR = "generate-keypair";
const std::string GENERATE_CSR = "generate-csr";
const std::string GENERATE_CA = "generate-ca";
const std::string GENERATE_CERT = "generate-cert";
const std::string GENERATE_APP_CERT = "generate-app-cert";
const std::string GENERATE_PROFILE_CERT = "generate-profile-cert";
const std::string SIGN_ELF = "sign";
const std::string SIGN_PROFILE = "sign-profile";
const std::string VERIFY_APP = "verify-app";
const std::string VERIFY_PROFILE = "verify-profile";

constexpr int32_t ONE_DAY_HOUR = 24;
constexpr int32_t ONE_DAY_MINUTE = 60;
constexpr int32_t ONE_DAY_SECOND = 60;

static constexpr int YEAR1900 = 1900;
static constexpr int MIN_CERTS_NUM = 2;
static constexpr int MAX_CERTS_NUM = 3;
} // namespace UpdateEngine
} // namespace OHOS
#endif // SIGNATRUETOOLS_CONSTANT_H