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

#ifndef CF_CERT_CHAIN_VALIDATOR_H
#define CF_CERT_CHAIN_VALIDATOR_H

#include <stddef.h>
#include <stdint.h>
#include "cf_blob.h"
#include "cf_object_base.h"
#include "cf_result.h"

typedef struct HcfCertChainValidator HcfCertChainValidator;

typedef struct {
    /* data format: len-value-len-value..., size of len is 2 bytes. */
    uint8_t *data;
    uint32_t dataLen;
    uint8_t count;
    enum CfEncodingFormat format;
} HcfCertChainData;

struct HcfCertChainValidator {
    struct CfObjectBase base;

    /** verify the cert chain. */
    CfResult (*validate)(HcfCertChainValidator *self, const HcfCertChainData *certChainData);

    /** Get algorithm name. */
    const char *(*getAlgorithm)(HcfCertChainValidator *self);
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate cert chain validator instance.
 */
CfResult HcfCertChainValidatorCreate(const char *algorithm, HcfCertChainValidator **pathValidator);

#ifdef __cplusplus
}
#endif

#endif // CF_CERT_CHAIN_VALIDATOR_H
