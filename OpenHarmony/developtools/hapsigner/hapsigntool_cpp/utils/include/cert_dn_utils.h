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
#ifndef SIGNATRUETOOLS_CERTUTILS_H
#define SIGNATRUETOOLS_CERTUTILS_H

#include <string>
#include <sstream>
#include <vector>
#include <utility>

#include "openssl/ossl_typ.h"
#include "openssl/x509.h"
#include "openssl/x509v3.h"
#include "string_utils.h"
#include "signature_tools_errno.h"
#include "signature_tools_log.h"

using std::pair;
using std::istringstream;
using std::getline;

namespace OHOS {
namespace SignatureTools {
/**
* buildDN
*
* @param nameString nameString
* @return X500Name
*/
X509_NAME* BuildDN(const std::string &nameString, X509_REQ* req);
/**
* To verify the format of subject or issuer.
* Refer to X509_NAMEStyle.fromstring().
*
* @param nameString subject or issuer
*/
int g_checkDn(const std::string &nameString, std::vector<pair<std::string, std::string>>& pairs);
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_CERTUTILS_H