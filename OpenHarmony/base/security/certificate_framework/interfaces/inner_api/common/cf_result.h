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

#ifndef CF_RESULT_H
#define CF_RESULT_H

typedef enum CfResult {
    /* Indicates success. */
    CF_SUCCESS = 0,

    /* Indicates that input params is invalid . */
    CF_INVALID_PARAMS = -10001,
    /* Indicates that function or algorithm is not supported. */
    CF_NOT_SUPPORT = -10002,
    /* Indicates that input pointer is not null. */
    CF_NULL_POINTER = -10003,
    /* Indicates that something expected is not exist. */
    CF_NOT_EXIST = -10004,

    /* Indicates that memory malloc fails. */
    CF_ERR_MALLOC = -20001,
    /* Indicates that memory copy fails. */
    CF_ERR_COPY = -20002,
    /* Indicates that call napi api fails. */
    CF_ERR_NAPI = -20003,
    /* Indicates internal fails. */
    CF_ERR_INTERNAL = -20004,
    /* Indicates parameter check fails. */
    CF_ERR_PARAMETER_CHECK = -20005,

    /* Indicates that third part has something wrong. */
    CF_ERR_CRYPTO_OPERATION = -30001,
    /* Indicates that cert signature check fails. */
    CF_ERR_CERT_SIGNATURE_FAILURE = -30002,
    /* Indicates that cert is not yet valid. */
    CF_ERR_CERT_NOT_YET_VALID = -30003,
    /* Indicates that cert has expired. */
    CF_ERR_CERT_HAS_EXPIRED = -30004,
    /* Indicates that we can not get the untrusted cert's issuer. */
    CF_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY = -30005,
    /* Indicates that key usage does not include certificate sign. */
    CF_ERR_KEYUSAGE_NO_CERTSIGN = -30006,
    /* Indicates that key usage does not include digital sign. */
    CF_ERR_KEYUSAGE_NO_DIGITAL_SIGNATURE = -30007,
    /* Indicates that cert data format is invalid. */
    CF_ERR_INVALID_CODE_FORMAT = -30008,
    /* Indicates that private key is invalid. */
    CF_ERR_CERT_INVALID_PRIVATE_KEY = -30009,
} CfResult;
#endif
