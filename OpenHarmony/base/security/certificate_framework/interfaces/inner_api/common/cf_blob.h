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

#ifndef CF_BLOB_H
#define CF_BLOB_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct CfBlob CfBlob;
struct CfBlob {
    uint32_t size;
    uint8_t *data;
};

enum CfEncodingFormat {
    CF_FORMAT_DER = 0,
    CF_FORMAT_PEM = 1,
    CF_FORMAT_PKCS7 = 2,
};

typedef struct {
    uint8_t *data;
    size_t len;
    enum CfEncodingFormat encodingFormat;
} CfEncodingBlob;

typedef struct {
    CfBlob *data;
    enum CfEncodingFormat format;
    uint32_t count;
} CfArray;

typedef struct {
    CfBlob *data;
    uint32_t count;
} CfBlobArray;

#ifdef __cplusplus
extern "C" {
#endif

void CfBlobFree(CfBlob **blob);
void CfBlobDataFree(CfBlob *blob);
void CfBlobDataClearAndFree(CfBlob *blob);
void CfEncodingBlobDataFree(CfEncodingBlob *encodingBlob);
void CfArrayDataClearAndFree(CfArray *array);
void FreeCfBlobArray(CfBlob *array, uint32_t arrayLen);
bool CfBlobIsStr(const CfBlob *blob);

#ifdef __cplusplus
}
#endif

#endif
