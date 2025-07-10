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

#ifndef CF_TYPE_H
#define CF_TYPE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "cf_blob.h"

#define CF_API_EXPORT __attribute__ ((visibility("default")))

typedef enum {
    CF_OBJ_TYPE_CERT,
    CF_OBJ_TYPE_EXTENSION,
    CF_OBJ_TYPE_CRL,
    CF_OBJ_TYPE_LIST,
} CfObjectType;

typedef struct {
    unsigned long type;
} CfBase;

typedef enum {
    CF_ITEM_TBS = 0, /* TBS Certificate */
    CF_ITEM_PUBLIC_KEY,  /* Subject Public Key */
    CF_ITEM_ISSUER_UNIQUE_ID, /* Issuer Unique Identifier */
    CF_ITEM_SUBJECT_UNIQUE_ID, /* Subject Unique Identifier */
    CF_ITEM_EXTENSIONS, /* Extensions */

    CF_ITEM_ENCODED,
    CF_ITEM_VERSION,
    CF_ITEM_SERIAL_NUMBER,
    CF_ITEM_ISSUE_NAME,
    CF_ITEM_SUBJECT_NAME,
    CF_ITEM_NOT_BEFORE,
    CF_ITEM_NOT_AFTER,
    CF_ITEM_SIGNATURE,
    CF_ITEM_SIGNATURE_ALG_NAME,

    CF_ITEM_INVALID,
} CfItemId;

typedef enum {
    CF_EXT_TYPE_ALL_OIDS,
    CF_EXT_TYPE_CRITICAL_OIDS,
    CF_EXT_TYPE_UNCRITICAL_OIDS,
} CfExtensionOidType;

typedef enum {
    CF_EXT_ENTRY_TYPE_ENTRY,
    CF_EXT_ENTRY_TYPE_ENTRY_CRITICAL,
    CF_EXT_ENTRY_TYPE_ENTRY_VALUE,
} CfExtensionEntryType;

typedef enum {
    CF_GENERAL_NAME_TYPE_OTHER_NAME,
    CF_GENERAL_NAME_TYPE_RFC822_NAME,
    CF_GENERAL_NAME_TYPE_DNS_NAME,
    CF_GENERAL_NAME_TYPE_X400_ADDRESS,
    CF_GENERAL_NAME_TYPE_DIRECTORY_NAME,
    CF_GENERAL_NAME_TYPE_EDI_PARTY_NAME,
    CF_GENERAL_NAME_TYPE_UNIFORM_RESOURCE_ID,
    CF_GENERAL_NAME_TYPE_IP_ADDRESS,
    CF_GENERAL_NAME_TYPE_REGISTERED_ID
} CfGeneralNameType;

typedef enum {
    CF_REVOCATION_CHECK_OPTION_PREFER_OCSP,
    CF_REVOCATION_CHECK_OPTION_ACCESS_NETWORK,
    CF_REVOCATION_CHECK_OPTION_FALLBACK_NO_PREFER,
    CF_REVOCATION_CHECK_OPTION_FALLBACK_LOCAL,
} CfRevocationCheckOptionsType;

typedef enum {
    CF_VALIDATION_POLICY_TYPE_X509,
    CF_VALIDATION_POLICY_TYPE_SSL,
} CfValidationPolicyType;

typedef enum {
    CF_KEYUSAGE_DIGITAL_SIGNATURE,
    CF_KEYUSAGE_NON_REPUDIATION,
    CF_KEYUSAGE_KEY_ENCIPHERMENT,
    CF_KEYUSAGE_DATA_ENCIPHERMENT,
    CF_KEYUSAGE_KEY_AGREEMENT,
    CF_KEYUSAGE_KEY_CERT_SIGN,
    CF_KEYUSAGE_CRL_SIGN,
    CF_KEYUSAGE_ENCIPHER_ONLY,
    CF_KEYUSAGE_DECIPHER_ONLY,
} CfValidationKeyUsageType;

typedef enum {
    CF_GET_TYPE_CERT_ITEM,
    CF_GET_TYPE_EXT_ITEM,
    CF_GET_TYPE_EXT_OIDS,
    CF_GET_TYPE_EXT_ENTRY,
} CfGetType;

typedef enum {
    CF_CHECK_TYPE_EXT_CA,
    CF_CHECK_TYPE_EXT_HAS_UN_SUPPORT,
} CfCheckType;

typedef enum {
    CF_TAG_TYPE_INVALID = 0 << 28,
    CF_TAG_TYPE_INT = 1 << 28,
    CF_TAG_TYPE_UINT = 2 << 28,
    CF_TAG_TYPE_ULONG = 3 << 28,
    CF_TAG_TYPE_BOOL = 4 << 28,
    CF_TAG_TYPE_BYTES = 5 << 28,
} CfTagType;

typedef enum {
    CF_TAG_INVALID = CF_TAG_TYPE_INVALID | 0,

    CF_TAG_RESULT_TYPE = CF_TAG_TYPE_INT | 1, /* choose from CfTagType */
    CF_TAG_RESULT_INT = CF_TAG_TYPE_INT | 2,
    CF_TAG_RESULT_UINT = CF_TAG_TYPE_UINT | 3,
    CF_TAG_RESULT_ULONG = CF_TAG_TYPE_ULONG | 4,
    CF_TAG_RESULT_BOOL = CF_TAG_TYPE_BOOL | 5,
    CF_TAG_RESULT_BYTES = CF_TAG_TYPE_BYTES | 6,

    CF_TAG_GET_TYPE = CF_TAG_TYPE_INT | 1001, /* choose from CfGetType */
    CF_TAG_CHECK_TYPE = CF_TAG_TYPE_INT | 1002, /* choose from CfCheckType */

    CF_TAG_PARAM0_BUFFER = CF_TAG_TYPE_BYTES | 30001,
    CF_TAG_PARAM1_BUFFER = CF_TAG_TYPE_BYTES | 30002,
    CF_TAG_PARAM2_BUFFER = CF_TAG_TYPE_BYTES | 30003,
    CF_TAG_PARAM3_BUFFER = CF_TAG_TYPE_BYTES | 30004,
    CF_TAG_PARAM4_BUFFER = CF_TAG_TYPE_BYTES | 30005,
    CF_TAG_PARAM0_INT32 = CF_TAG_TYPE_INT | 30006,
    CF_TAG_PARAM1_INT32 = CF_TAG_TYPE_INT | 30007,
    CF_TAG_PARAM2_INT32 = CF_TAG_TYPE_INT | 30008,
    CF_TAG_PARAM3_INT32 = CF_TAG_TYPE_INT | 30009,
    CF_TAG_PARAM4_INT32 = CF_TAG_TYPE_INT | 30010,
} CfTag;

typedef enum {
    CF_ENCODING_UTF8 = 0,
} CfEncodinigType;

typedef enum {
    PEM = 0,
    DER = 1,
} CfEncodinigBaseFormat;

typedef struct {
    uint32_t tag;
    union {
        bool boolParam;
        int32_t int32Param;
        uint32_t uint32Param;
        uint64_t uint64Param;
        CfBlob blob;
    };
} CfParam;

typedef struct {
    uint32_t paramSetSize;
    uint32_t paramsCnt;
    CfParam params[];
} CfParamSet;

static inline bool CfIsAdditionOverflow(uint32_t a, uint32_t b)
{
    return (UINT32_MAX - a) < b;
}

typedef enum {
    SIGNED_DATA = 0,
} HcfCmsContentType;

typedef enum {
    BINARY = 0,
    TEXT = 1,
} HcfCmsContentDataFormat;

typedef enum {
    CMS_PEM = 0,
    CMS_DER = 1,
} HcfCmsFormat;

#define MAX_COUNT_OID          100
#define MAX_LEN_OID            128
#define MAX_COUNT_NID          1195

#define MAX_LEN_CERTIFICATE    65536
#define MAX_LEN_EXTENSIONS     65536

#define BASIC_CONSTRAINTS_NO_CA             (-1)
#define BASIC_CONSTRAINTS_PATHLEN_NO_LIMIT  (-2)
#endif /* CF_TYPE_H */
