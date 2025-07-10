/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef CERT_CRL_COMMON_H
#define CERT_CRL_COMMON_H

#include "x509_certificate.h"
#include "x509_crl.h"
#include "cf_result.h"

#ifdef __cplusplus
extern "C" {
#endif

CfResult CloneCertificateObj(HcfX509Certificate *in, HcfX509Certificate **out);

CfResult CloneCrlObj(HcfX509Crl *in, HcfX509Crl **out);

void FreeCertArrayData(HcfX509CertificateArray *certs);

void FreeCrlArrayData(HcfX509CrlArray *crls);

#ifdef __cplusplus
}
#endif

#endif /* CERT_CRL_COMMON_H */