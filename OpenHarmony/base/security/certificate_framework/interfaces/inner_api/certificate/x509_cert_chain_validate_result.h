/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef X509_CERT_CHAIN_VALIDATE_RESULT_H
#define X509_CERT_CHAIN_VALIDATE_RESULT_H

#include "x509_trust_anchor.h"

typedef struct HcfX509CertChainValidateResult HcfX509CertChainValidateResult;
struct HcfX509CertChainValidateResult {
    HcfX509TrustAnchor *trustAnchor; // X509TrustAnchor
    HcfX509Certificate *entityCert;  // entityCert: X509Cert
};

#endif // X509_CERT_CHAIN_VALIDATE_RESULT_H
