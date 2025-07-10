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

#ifndef X509_DISTINGUISHED_NAME_H
#define X509_DISTINGUISHED_NAME_H

#include <stddef.h>
#include <stdint.h>

#include "cf_blob.h"
#include "cf_object_base.h"
#include "cf_result.h"
#include "cf_type.h"

typedef struct HcfX509DistinguishedName HcfX509DistinguishedName;
struct HcfX509DistinguishedName {
    CfObjectBase base;

    /** Get encode from Distinguished Name. */
    CfResult (*getEncode)(HcfX509DistinguishedName *self, CfEncodingBlob *out);

    /** Get name from Distinguished Name. */
    CfResult (*getName)(HcfX509DistinguishedName *self, CfBlob *type, CfBlob *out, CfArray *outArr);

    /** Get utf8 name from Distinguished Name. */
    CfResult (*getNameEx)(HcfX509DistinguishedName *self, CfEncodinigType encodingType, CfBlob *out);
};

typedef struct HcfX509Attribute HcfX509Attribute;
struct HcfX509Attribute {
    CfBlob *AttributeType;           // AttributeType : Uint8Array DER format
    CfBlob *AttributeValue;          // AttributeValue : Uint8Array DER format
};

typedef struct {
    HcfX509Attribute **data;
    uint32_t count;
} HcfX509DistinguishedNameArray;

#ifdef __cplusplus
extern "C" {
#endif

CfResult HcfX509DistinguishedNameCreate(const CfBlob *inStream, bool bString, HcfX509DistinguishedName **returnObj);

#ifdef __cplusplus
}
#endif

#endif // X509_DISTINGUISHED_NAME_H
