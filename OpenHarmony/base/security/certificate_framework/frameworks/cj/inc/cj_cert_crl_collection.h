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

#ifndef CJCERTCRLCOLLECTION_H
#define CJCERTCRLCOLLECTION_H

#include "cert_crl_collection.h"

#include "cj_cert_common.h"
#include "cj_declare_macro.h"

#define FFI_DECLARE_HcfCertCrlCollection(FUNC, RETURN_TYPE, ...) \
FFI_DECLARE_API(CjCertCrlCollection, FUNC, RETURN_TYPE, ##__VA_ARGS__)

extern "C" {
typedef struct CjCertCrlCollection CjCertCrlCollection;

struct CjCertCrlCollection {
    HcfCertCrlCollection *collection;
};

FFI_DECLARE_INSTANCE(CjCertCrlCollection, const HcfX509CertificateArray *inCerts, const HcfX509CrlArray *inCrls);
FFI_DECLARE_HcfCertCrlCollection(SelectCerts, CfResult, const CjX509CertMatchParams *matchParams,
                                 HcfX509CertificateArray *retCerts);
FFI_DECLARE_HcfCertCrlCollection(SelectCRLs, CfResult, const CjX509CrlMatchParams *matchParams,
                                 HcfX509CrlArray *retCrls);
}

#endif //CJCERTCRLCOLLECTION_H
