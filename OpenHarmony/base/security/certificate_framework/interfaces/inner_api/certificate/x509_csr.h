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

#ifndef CF_X509CSR_H
#define CF_X509CSR_H

#include "cf_blob.h"
#include "x509_distinguished_name.h"

typedef struct HcfAttributes HcfAttributes;
struct HcfAttributes {
    /** Name of the attribute (e.g., "challengePassword") */
    char *attributeName;
    /** Value of the attribute */
    char *attributeValue;
};

typedef struct {
    HcfAttributes *array;
    uint32_t attributeSize;
} HcfAttributesArray;

typedef struct HcfGenCsrConf HcfGenCsrConf;
struct HcfGenCsrConf {
    /** Subject information for the CSR */
    HcfX509DistinguishedName *subject;
    /** Additional attributes to be included in the CSR */
    HcfAttributesArray attribute;
    /** Message digest algorithm name (e.g., "SHA256", "SHA512") */
    char *mdName;
    /** Flag to indicate if the output format should be PEM (true) or DER (false) */
    bool isPem;
};


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif // CF_X509CRL_H