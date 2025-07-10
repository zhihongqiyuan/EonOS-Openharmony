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

#ifndef CJCERTCHAINVALIDATOR_H
#define CJCERTCHAINVALIDATOR_H

#include "cert_chain_validator.h"

#include "cj_declare_macro.h"

#define FFI_DECLARE_HcfCertChainValidator(FUNC, RETURN_TYPE, ...) \
    FFI_DECLARE_API(CjCertChainValidator, FUNC, RETURN_TYPE, ##__VA_ARGS__)

extern "C" {
typedef struct CjCertChainValidator CjCertChainValidator;

struct CjCertChainValidator {
    HcfCertChainValidator* validator;
};

FFI_DECLARE_INSTANCE(CjCertChainValidator, const char *algorithm);
FFI_DECLARE_HcfCertChainValidator(Validate, CfResult, const HcfCertChainData *certChainData);
}


#endif //CJCERTCHAINVALIDATOR_H
