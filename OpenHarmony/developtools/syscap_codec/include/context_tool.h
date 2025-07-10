/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CONTEXT_TOOL_H
#define CONTEXT_TOOL_H

#include <stdint.h>
#include <stdio.h>

#include "cJSON.h"

#define PRINT_ERR(...) \
    do { \
        printf("ERROR: [%s: %d] -> ", __FILE__, __LINE__); \
        printf(__VA_ARGS__); \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct RequiredProductCompatibilityIDHead {
    uint16_t apiVersion : 15;
    uint16_t apiVersionType : 1;
} RPCIDHead;

void FreeContextBuffer(char *contextBuffer);
int32_t GetFileContext(const char *inputFile, char **contextBufPtr, uint32_t *bufferLen);
int32_t CheckFileAndGetFileContext(const char *inputFile, char **contextBufPtr, uint32_t *bufferLen);
int32_t ConvertedContextSaveAsFile(char *outDirPath, const char *filename, char *convertedBuffer, size_t contextBufLen);
int32_t CheckRpcidFormat(const char *inputFile, char **buffer, uint32_t *len);
cJSON *CreateWholeSyscapJsonObj(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SYSCAP_TOOL_H */