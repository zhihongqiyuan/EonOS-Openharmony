/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#ifndef SYSCAP_TOOL_H
#define SYSCAP_TOOL_H

#include <stdint.h>

#define TYPE_FILE (0U)
#define TYPE_STRING (1U)

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/* in: inputFile, out: outputPath/<product>.sc */
int32_t PCIDEncode(char *inputFile, char *outputPath);
/* in: inputFile, file name format: <product>.sc, out: outputPath/<product>.json */
int32_t PCIDDecode(char *inputFile, char *outputPath);
/* in: inputFile, out: outputPath/rpcid.sc */
int32_t RPCIDEncode(char *inputFile, char *outputPath);
/* in: inputFile, out: outputPath/rpcid.json */
int32_t RPCIDDecode(char *inputFile, char *outputPath);
/* in: inputFile, out: outputPath/rpcid.json */
int32_t EncodeRpcidscToString(char *inputFile, char *outDirPath);
/* in: pcidFile, rpcidFile */
int32_t ComparePcidWithRpcidString(char *pcidFile, char *rpcidFile, uint32_t type);
int32_t SeparateSyscapFromString(const char *inputString, uint32_t *osArray, uint32_t osArraySize,
                                 char **priSyscap, uint32_t *priSyscapLen);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SYSCAP_TOOL_H */