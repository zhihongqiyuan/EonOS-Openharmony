/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef X509_CRL_MATCH_PARAMETERS_H
#define X509_CRL_MATCH_PARAMETERS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cf_blob.h"
#include "cf_object_base.h"
#include "cf_result.h"
#include "certificate.h"

typedef struct HcfX509CrlMatchParams HcfX509CrlMatchParams;
struct HcfX509CrlMatchParams {
    CfBlobArray *issuer;
    HcfCertificate *x509Cert;
    CfBlob *updateDateTime;
    CfBlob *maxCRL;
    CfBlob *minCRL;
};

#endif // X509_CRL_MATCH_PARAMETERS_H
