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

#ifndef CF_CERTIFICATE_H
#define CF_CERTIFICATE_H

#include "cf_blob.h"
#include "cf_object_base.h"
#include "cf_result.h"

typedef struct HcfCertificate HcfCertificate;

struct HcfCertificate {
    struct CfObjectBase base;

    /** Verify that this certificate corresponding to the specified public key. */
    CfResult (*verify)(HcfCertificate *self, void *key);

    /** Get the serialized cert data.*/
    CfResult (*getEncoded)(HcfCertificate *self, CfEncodingBlob *encodedByte);

    /** Get the public key from this certificate. */
    CfResult (*getPublicKey)(HcfCertificate *self, void **keyOut);
};

#endif // CF_CERTIFICATE_H
