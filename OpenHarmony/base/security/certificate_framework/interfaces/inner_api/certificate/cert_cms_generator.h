/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef CERT_CMS_GENERATOR_H
#define CERT_CMS_GENERATOR_H

#include <stddef.h>
#include <stdint.h>
#include "cf_blob.h"
#include "cf_object_base.h"
#include "cf_result.h"
#include "cf_type.h"
#include "x509_certificate.h"

typedef struct HcfCmsSignerOptions HcfCmsSignerOptions;
struct HcfCmsSignerOptions {
    char *mdName;
    bool addCert;
    bool addAttr;
    bool addSmimeCapAttr;
};

typedef struct HcfCmsGeneratorOptions HcfCmsGeneratorOptions;
struct HcfCmsGeneratorOptions {
    HcfCmsContentDataFormat dataFormat;
    HcfCmsFormat outFormat;
    bool isDetachedContent;
};

typedef struct HcfCmsGenerator HcfCmsGenerator;

struct HcfCmsGenerator {
    struct CfObjectBase base;

    /** add signer to cms generator. */
    CfResult (*addSigner)(HcfCmsGenerator *self, const HcfCertificate *x509Cert,
                          const PrivateKeyInfo *privateKey, const HcfCmsSignerOptions *options);
    /** add other certificate to cms generator. */
    CfResult (*addCert)(HcfCmsGenerator *self, const HcfCertificate *x509Cert);
    /** do final to cms generator. */
    CfResult (*doFinal)(HcfCmsGenerator *self, const CfBlob *content, const HcfCmsGeneratorOptions *options,
                        CfBlob *out);
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate Cms generator instance.
 */
CfResult HcfCreateCmsGenerator(HcfCmsContentType type, HcfCmsGenerator **cmsGenerator);

#ifdef __cplusplus
}
#endif

#endif // CERT_CMS_GENERATOR_H
