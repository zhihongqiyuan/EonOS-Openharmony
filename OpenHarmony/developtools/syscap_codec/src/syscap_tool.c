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

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include "securec.h"
#include "endian_internal.h"
#include "cJSON.h"
#include "create_pcid.h"
#include "syscap_tool.h"
#include "context_tool.h"
#include "common_method.h"

#ifdef SYSCAP_DEFINE_EXTERN_ENABLE
#include "syscap_define_custom.h"
#else
#include "syscap_define.h"
#endif

#define SYSCAP_PREFIX_LEN 17
#define SINGLE_FEAT_LEN (SINGLE_SYSCAP_LEN - SYSCAP_PREFIX_LEN)
#define DOUBLE_SPACE 2
#define UINT8_BIT 8
#define INT_BIT 32
#define RPCID_OUT_BUFFER 32
#define PCID_OUT_BUFFER RPCID_OUT_BUFFER
#define BYTES_OF_OS_SYSCAP 120
#define U32_TO_STR_MAX_LEN 11
#define STRING_FORMAT_LEN_MAX 1024

#define FREE_CONVERT_OUT_RPCID_ENCODE 1
#define FREE_CONTEXT_OUT_RPCID_ENCODE 2

#define FREE_OUTBUFFER_AFTER_RPCIDSC 1
#define FREE_MALLOC_RPISYSCAP_AFTER_RPCIDSC 2
#define FREE_MALLOC_OSSYSCAP_AFTER_RPCIDSC 3
#define FREE_WHOLE_SYSCAP_AFTER_RPCIDSC 4
#define FREE_RPCID_ROOT_AFTER_RPCIDSC 5
#define FREE_CONTEXT_OUT_AFTER_RPCIDSC 6

struct JsonObjectSysCap {
    cJSON *cjsonObjectRoot;
    cJSON *sysCapPtr;
};

struct FreeAfterEncodeRpcidscInfo {
    char *outBuffer;
    char *priSyscapArray;
    uint16_t *osSysCapIndex;
    cJSON *sysCapDefine;
    cJSON *rpcidRoot;
    char *contextBuffer;
    int32_t type;
    int16_t flag;
};

static void FreePointerMemory(char *pcidContent, char *rpcidContent, char *pcidPriSyscap, char *rpcidPriSyscap)
{
    SafeFree(pcidContent);
    SafeFree(rpcidPriSyscap);
    SafeFree(rpcidContent);
    SafeFree(pcidPriSyscap);
}

static int32_t FillOsCapLength(char *convertedBuffer, char *contextBuffer, struct JsonObjectSysCap gJsonObjectSysCap,
        uint32_t sysCapSize, int32_t ret)
{
    RPCIDHead *headPtr = NULL;
    char *fillTmpPtr = NULL;
    cJSON *apiVerItem = NULL;
    cJSON *arrayItemPtr = NULL;

    headPtr = (RPCIDHead *)convertedBuffer;
    apiVerItem = cJSON_GetObjectItem(gJsonObjectSysCap.cjsonObjectRoot, "api_version");
    if (apiVerItem == NULL || !cJSON_IsNumber(apiVerItem)) {
        PRINT_ERR("get \"api_version\" failed\n");
        return -1;
    }
    headPtr->apiVersion = HtonsInter((uint16_t)apiVerItem->valueint);
    headPtr->apiVersionType = 1;

    fillTmpPtr = convertedBuffer + sizeof(RPCIDHead);
    *(uint16_t *)fillTmpPtr = HtonsInter(2); // 2, SysCap Type, 2: request Cap
    fillTmpPtr += sizeof(uint16_t);
    // fill osCap Length
    *(uint16_t *)fillTmpPtr = HtonsInter((uint16_t)(sysCapSize * SINGLE_FEAT_LEN));
    fillTmpPtr += sizeof(uint16_t);
    for (uint32_t i = 0; i < sysCapSize; i++) {
        arrayItemPtr = cJSON_GetArrayItem(gJsonObjectSysCap.sysCapPtr, (int)i);
        if (arrayItemPtr->valuestring == NULL) {
            PRINT_ERR("arrayItemPtr->valuestring is NULL\n");
            return -1;
        }
        
        char *pointPos = strchr(arrayItemPtr->valuestring, '.');
        if (pointPos == NULL) {
            PRINT_ERR("context of \"syscap\" array is invalid\n");
            return -1;
        }
        ret = strncmp(arrayItemPtr->valuestring, "SystemCapability.", pointPos - arrayItemPtr->valuestring + 1);
        if (ret != 0) {
            PRINT_ERR("context of \"syscap\" array is invalid\n");
            return -1;
        }

        ret = memcpy_s(fillTmpPtr, SINGLE_FEAT_LEN, pointPos + 1, strlen(pointPos + 1));
        if (ret != 0) {
            PRINT_ERR("context of \"syscap\" array is invalid\n");
            return -1;
        }
        fillTmpPtr += SINGLE_FEAT_LEN;
    }
    return ret;
}

static int32_t FreeAfterRPCIDEncode(
    cJSON *cjsonObjectRoot, char *convertedBuffer, char *contextBuffer, int32_t type, int32_t ret)
{
    if (type == FREE_CONVERT_OUT_RPCID_ENCODE) {
        free(convertedBuffer);
    }
    cJSON_Delete(cjsonObjectRoot);
    FreeContextBuffer(contextBuffer);
    return ret;
}

int32_t RPCIDEncode(char *inputFile, char *outputPath)
{
    char *contextBuffer = NULL;
    uint32_t bufferLen, sysCapSize;
    struct JsonObjectSysCap gJsonObjectSysCap;
    gJsonObjectSysCap.cjsonObjectRoot = NULL;
    gJsonObjectSysCap.sysCapPtr = NULL;

    if (CheckFileAndGetFileContext(inputFile, &contextBuffer, &bufferLen) != 0) {
        return -1;
    }

    gJsonObjectSysCap.cjsonObjectRoot = cJSON_ParseWithLength(contextBuffer, bufferLen);
    if (gJsonObjectSysCap.cjsonObjectRoot == NULL) {
        PRINT_ERR("cJSON_Parse failed, context buffer is:\n%s\n", contextBuffer);
        return FreeAfterRPCIDEncode(gJsonObjectSysCap.cjsonObjectRoot, NULL, contextBuffer,
                                    FREE_CONTEXT_OUT_RPCID_ENCODE, -1);
    }

    gJsonObjectSysCap.sysCapPtr = cJSON_GetObjectItem(gJsonObjectSysCap.cjsonObjectRoot, "syscap");
    if (gJsonObjectSysCap.sysCapPtr == NULL || !cJSON_IsArray(gJsonObjectSysCap.sysCapPtr)) {
        PRINT_ERR("get \"syscap\" object failed.\n");
        return FreeAfterRPCIDEncode(gJsonObjectSysCap.cjsonObjectRoot, NULL, contextBuffer,
                                    FREE_CONTEXT_OUT_RPCID_ENCODE, -1);
    }

    int32_t ret = cJSON_GetArraySize(gJsonObjectSysCap.sysCapPtr);
    if (ret < 0) {
        PRINT_ERR("get \"syscap\" array size failed\n");
        return FreeAfterRPCIDEncode(gJsonObjectSysCap.cjsonObjectRoot, NULL, contextBuffer,
                                    FREE_CONTEXT_OUT_RPCID_ENCODE, -1);
    }

    // 2, to save SysCaptype & SysCapLength
    sysCapSize = (uint32_t) ret;
    uint32_t convertedBufLen = sizeof(RPCIDHead);
    convertedBufLen += (DOUBLE_SPACE * sizeof(uint16_t) + sysCapSize * SINGLE_FEAT_LEN);
    char *convertedBuffer = (char *) malloc(convertedBufLen);
    if (convertedBuffer == NULL) {
        PRINT_ERR("malloc failed\n");
        return FreeAfterRPCIDEncode(gJsonObjectSysCap.cjsonObjectRoot, convertedBuffer, contextBuffer,
                                    FREE_CONTEXT_OUT_RPCID_ENCODE, -1);
    }
    (void) memset_s(convertedBuffer, convertedBufLen, 0, convertedBufLen);

    if (FillOsCapLength(convertedBuffer, contextBuffer, gJsonObjectSysCap, sysCapSize, ret) == -1) {
        return FreeAfterRPCIDEncode(gJsonObjectSysCap.cjsonObjectRoot, convertedBuffer, contextBuffer,
                                    FREE_CONVERT_OUT_RPCID_ENCODE, -1);
    }
    ret = ConvertedContextSaveAsFile(outputPath, "rpcid.sc", convertedBuffer, convertedBufLen);
    if (ret != 0) {
        PRINT_ERR("ConvertedContextSaveAsFile failed, outputPath:%s, filename:rpcid.sc\n", outputPath);
    }
    return FreeAfterRPCIDEncode(gJsonObjectSysCap.cjsonObjectRoot, convertedBuffer, contextBuffer,
                                FREE_CONVERT_OUT_RPCID_ENCODE, ret);
}

static int32_t ParseRpcidToJson(char *input, uint32_t inputLen, cJSON *rpcidJson)
{
    uint32_t i;
    int32_t ret = 0;
    uint16_t sysCapLength = NtohsInter(*(uint16_t *)(input + sizeof(uint32_t)));
    if (sysCapLength > inputLen - sizeof(uint32_t)) {
        PRINT_ERR("Get sysCapLength(%u) error, inputLen = %u\n", sysCapLength, inputLen);
        return -1;
    }
    uint16_t sysCapCount = sysCapLength / SINGLE_FEAT_LEN;
    char *sysCapBegin = input + sizeof(RPCIDHead) + sizeof(uint32_t);
    RPCIDHead *rpcidHeader = (RPCIDHead *)input;
    cJSON *sysCapJson = cJSON_CreateArray();
    if (sysCapJson == NULL) {
        PRINT_ERR("Failed to create sysCapJson array\n");
        return -1;
    }

    for (i = 0; i < sysCapCount; i++) {
        char *temp = sysCapBegin + i * SINGLE_FEAT_LEN;
        if (strlen(temp) >= SINGLE_FEAT_LEN || strlen(temp) == 0) {
            PRINT_ERR("Get SysCap failed, string length(%u) error.\n", (uint32_t)strlen(temp));
            cJSON_Delete(sysCapJson);
            return -1;
        }
        char buffer[SINGLE_SYSCAP_LEN] = "SystemCapability.";

        ret = strncat_s(buffer, sizeof(buffer), temp, SINGLE_FEAT_LEN);
        if (ret != EOK) {
            PRINT_ERR("strncat_s failed.\n");
            cJSON_Delete(sysCapJson);
            return ret;
        }

        if (!cJSON_AddItemToArray(sysCapJson, cJSON_CreateString(buffer))) {
            PRINT_ERR("Add syscap string to json failed.\n");
            cJSON_Delete(sysCapJson);
            return -1;
        }
    }

    if (!cJSON_AddNumberToObject(rpcidJson, "api_version", NtohsInter(rpcidHeader->apiVersion))) {
        PRINT_ERR("Add api_version to json failed.\n");
        cJSON_Delete(sysCapJson);
        return -1;
    }
    if (!cJSON_AddItemToObject(rpcidJson, "syscap", sysCapJson)) {
        PRINT_ERR("Add syscap to json failed.\n");
        cJSON_Delete(sysCapJson);
        return -1;
    }

    return ret;
}

int32_t RPCIDDecode(char *inputFile, char *outputPath)
{
    int32_t ret = 0;
    char *contextBuffer = NULL;
    char *convertedBuffer = NULL;
    uint32_t bufferLen;

    if (inputFile == NULL) {
        PRINT_ERR("inputFile is null.\n");
        return -1;
    }

    // check rpcid.sc
    if (CheckRpcidFormat(inputFile, &contextBuffer, &bufferLen)) {
        PRINT_ERR("Check rpcid.sc format failed. Input failed: %s\n", inputFile);
        goto FREE_CONTEXT_OUT;
    }

    // parse rpcid to json
    cJSON *rpcidRoot = cJSON_CreateObject();
    if (ParseRpcidToJson(contextBuffer, bufferLen, rpcidRoot) != 0) {
        PRINT_ERR("Prase rpcid to json failed. Input failed: %s\n", inputFile);
        goto FREE_RPCID_ROOT;
    }

    // save to json file
    convertedBuffer = cJSON_Print(rpcidRoot);
    if (convertedBuffer == NULL) {
        PRINT_ERR("cJSON_Print failed to create JSON string\n");
        goto FREE_RPCID_ROOT;
    }

    ret = ConvertedContextSaveAsFile(outputPath, "rpcid.json", convertedBuffer, strlen(convertedBuffer));
    if (ret != 0) {
        PRINT_ERR("ConvertedContextSaveAsFile failed, outputPath:%s, filename:rpcid.json\n", outputPath);
        cJSON_free(convertedBuffer);
        goto FREE_RPCID_ROOT;
    }
    cJSON_free(convertedBuffer);
FREE_RPCID_ROOT:
    cJSON_Delete(rpcidRoot);
FREE_CONTEXT_OUT:
    FreeContextBuffer(contextBuffer);
    return ret;
}

static int SetOsSysCapBitMap(uint8_t *out, uint16_t outLen, const uint16_t *index, uint16_t indexLen)
{
    uint16_t sector, pos;

    if (outLen != BYTES_OF_OS_SYSCAP) {
        PRINT_ERR("Input array error.\n");
        return -1;
    }

    for (uint16_t i = 0; i < indexLen; i++) {
        sector = index[i] / UINT8_BIT;
        pos = index[i] % UINT8_BIT;
        if (sector >= BYTES_OF_OS_SYSCAP) {
            PRINT_ERR("Syscap num(%u) out of range(120).\n", sector);
            return -1;
        }
        out[sector] |=  (1 << pos);
    }
    return 0;
}

static int32_t PrintOutputToFile(struct FreeAfterEncodeRpcidscInfo freeAfterEncodeRpcidscInfo,
        uint32_t outUint[RPCID_OUT_BUFFER], uint16_t indexPri, char *outDirPath)
{
    int32_t ret = 0;

    uint16_t outBufferLen = U32_TO_STR_MAX_LEN * RPCID_OUT_BUFFER + SINGLE_SYSCAP_LEN * indexPri;
    freeAfterEncodeRpcidscInfo.outBuffer = (char *)malloc(outBufferLen);
    if (freeAfterEncodeRpcidscInfo.outBuffer == NULL) {
        PRINT_ERR("malloc(%u) failed.\n", outBufferLen);
        freeAfterEncodeRpcidscInfo.flag = 1;
        return ret;
    }

    freeAfterEncodeRpcidscInfo.type = FREE_OUTBUFFER_AFTER_RPCIDSC;
    (void)memset_s(freeAfterEncodeRpcidscInfo.outBuffer, outBufferLen, 0, outBufferLen);
    ret = sprintf_s(freeAfterEncodeRpcidscInfo.outBuffer, outBufferLen, "%u", outUint[0]);
    if (ret == -1) {
        PRINT_ERR("sprintf_s failed.\n");
        freeAfterEncodeRpcidscInfo.flag = 1;
        return ret;
    }
    for (int i = 1; i < RPCID_OUT_BUFFER; i++) {
        ret = sprintf_s(freeAfterEncodeRpcidscInfo.outBuffer, outBufferLen, "%s,%u",
                freeAfterEncodeRpcidscInfo.outBuffer, outUint[i]);
        if (ret == -1) {
            PRINT_ERR("sprintf_s failed.\n");
            freeAfterEncodeRpcidscInfo.flag = 1;
            return ret;
        }
    }

    for (uint16_t i = 0; i < indexPri; i++) {
        ret = sprintf_s(freeAfterEncodeRpcidscInfo.outBuffer, outBufferLen, "%s,%s",
            freeAfterEncodeRpcidscInfo.outBuffer, freeAfterEncodeRpcidscInfo.priSyscapArray + i * SINGLE_SYSCAP_LEN);
        if (ret == -1) {
            PRINT_ERR("sprintf_s failed.\n");
            freeAfterEncodeRpcidscInfo.flag = 1;
            return ret;
        }
    }

    const char outputFilename[] = "rpcid.txt";
    ret = ConvertedContextSaveAsFile(outDirPath, outputFilename, freeAfterEncodeRpcidscInfo.outBuffer,
            strlen(freeAfterEncodeRpcidscInfo.outBuffer));
    if (ret != 0) {
        PRINT_ERR("Save to txt file failed. Output path:%s/%s\n", outDirPath, outputFilename);
        freeAfterEncodeRpcidscInfo.flag = 1;
    }
    return ret;
}

static int32_t OutputSetMemAndPrintToFile(struct FreeAfterEncodeRpcidscInfo freeAfterEncodeRpcidscInfo,
    int32_t sysCapArraySize, cJSON *sysCapArray, char *outDirPath)
{
    char *priSyscap = NULL;
    cJSON *cJsonTemp = NULL;
    uint16_t indexPri = 0;
    int32_t ret = 0;
    freeAfterEncodeRpcidscInfo.type = FREE_MALLOC_OSSYSCAP_AFTER_RPCIDSC;

    (void)memset_s(freeAfterEncodeRpcidscInfo.osSysCapIndex, sizeof(uint16_t) * sysCapArraySize,
                   0, sizeof(uint16_t) * sysCapArraySize);
    // malloc for save private syscap string
    freeAfterEncodeRpcidscInfo.priSyscapArray = (char *)malloc((uint32_t)sysCapArraySize * SINGLE_SYSCAP_LEN);
    if (freeAfterEncodeRpcidscInfo.priSyscapArray == NULL) {
        PRINT_ERR("malloc(%d) failed.\n", sysCapArraySize * SINGLE_SYSCAP_LEN);
        freeAfterEncodeRpcidscInfo.flag = 1;
        return ret;
    }
    (void)memset_s(freeAfterEncodeRpcidscInfo.priSyscapArray, (size_t)(sysCapArraySize * SINGLE_SYSCAP_LEN),
                   0, (size_t)(sysCapArraySize * SINGLE_SYSCAP_LEN));
    priSyscap = freeAfterEncodeRpcidscInfo.priSyscapArray;
    // part os syscap and ptivate syscap
    uint16_t indexOs = 0;
    for (int i = 0; i < sysCapArraySize; i++) {
        cJSON *cJsonItem = cJSON_GetArrayItem(sysCapArray, i);
        if (cJsonItem->valuestring == NULL) {
            continue;
        }
        cJsonTemp = cJSON_GetObjectItem(freeAfterEncodeRpcidscInfo.sysCapDefine, cJsonItem->valuestring);
        if (cJsonTemp != NULL && cJSON_IsNumber(cJsonTemp)) {
            freeAfterEncodeRpcidscInfo.osSysCapIndex[indexOs++] = (uint16_t)(cJsonTemp->valueint);
        } else {
            ret = strcpy_s(priSyscap, SINGLE_SYSCAP_LEN, cJsonItem->valuestring);
            if (ret != EOK) {
                PRINT_ERR("strcpy_s failed.\n");
                freeAfterEncodeRpcidscInfo.flag = 1;
                return ret;
            }
            priSyscap += SINGLE_SYSCAP_LEN;
            indexPri++;
        }
    }
    uint32_t outUint[RPCID_OUT_BUFFER] = {0};
    outUint[0] = *(uint32_t *)freeAfterEncodeRpcidscInfo.contextBuffer;
    outUint[1] = *(uint32_t *)(freeAfterEncodeRpcidscInfo.contextBuffer + sizeof(uint32_t));
    uint8_t *osOutUint = (uint8_t *)(outUint + 2);
    // 120, len of osOutUint
    if (SetOsSysCapBitMap(osOutUint, 120, freeAfterEncodeRpcidscInfo.osSysCapIndex, indexOs) != 0) {
        PRINT_ERR("Set os syscap bit map failed.\n");
        freeAfterEncodeRpcidscInfo.flag = 1;
        return ret;
    }
    return PrintOutputToFile(freeAfterEncodeRpcidscInfo, outUint, indexPri, outDirPath);
}

static int32_t FreeAfterEncodeRpcidsc(struct FreeAfterEncodeRpcidscInfo freeAfterEncodeRpcidscInfo, int32_t type,
        int32_t ret)
{
    switch (type) {
        case FREE_OUTBUFFER_AFTER_RPCIDSC:
            free(freeAfterEncodeRpcidscInfo.outBuffer);
            free(freeAfterEncodeRpcidscInfo.priSyscapArray);
            free(freeAfterEncodeRpcidscInfo.osSysCapIndex);
            cJSON_Delete(freeAfterEncodeRpcidscInfo.sysCapDefine);
            cJSON_Delete(freeAfterEncodeRpcidscInfo.rpcidRoot);
            FreeContextBuffer(freeAfterEncodeRpcidscInfo.contextBuffer);
            break;
        case FREE_MALLOC_RPISYSCAP_AFTER_RPCIDSC:
            free(freeAfterEncodeRpcidscInfo.priSyscapArray);
            free(freeAfterEncodeRpcidscInfo.osSysCapIndex);
            cJSON_Delete(freeAfterEncodeRpcidscInfo.sysCapDefine);
            cJSON_Delete(freeAfterEncodeRpcidscInfo.rpcidRoot);
            FreeContextBuffer(freeAfterEncodeRpcidscInfo.contextBuffer);
            break;
        case FREE_MALLOC_OSSYSCAP_AFTER_RPCIDSC:
            free(freeAfterEncodeRpcidscInfo.osSysCapIndex);
            cJSON_Delete(freeAfterEncodeRpcidscInfo.sysCapDefine);
            cJSON_Delete(freeAfterEncodeRpcidscInfo.rpcidRoot);
            FreeContextBuffer(freeAfterEncodeRpcidscInfo.contextBuffer);
            break;
        case FREE_WHOLE_SYSCAP_AFTER_RPCIDSC:
            cJSON_Delete(freeAfterEncodeRpcidscInfo.sysCapDefine);
            cJSON_Delete(freeAfterEncodeRpcidscInfo.rpcidRoot);
            FreeContextBuffer(freeAfterEncodeRpcidscInfo.contextBuffer);
            break;
        case FREE_RPCID_ROOT_AFTER_RPCIDSC:
            cJSON_Delete(freeAfterEncodeRpcidscInfo.rpcidRoot);
            FreeContextBuffer(freeAfterEncodeRpcidscInfo.contextBuffer);
            break;
        case FREE_CONTEXT_OUT_AFTER_RPCIDSC:
        default:
            FreeContextBuffer(freeAfterEncodeRpcidscInfo.contextBuffer);
    }
    return ret;
}

int32_t EncodeRpcidscToString(char *inputFile, char *outDirPath)
{
    int32_t ret = 0;
    int32_t sysCapArraySize;
    uint32_t bufferLen;
    cJSON *sysCapArray = NULL;
    struct FreeAfterEncodeRpcidscInfo freeAfterEncodeRpcidscInfo;
    freeAfterEncodeRpcidscInfo.priSyscapArray = NULL;
    freeAfterEncodeRpcidscInfo.osSysCapIndex = NULL;
    freeAfterEncodeRpcidscInfo.sysCapDefine = NULL;
    freeAfterEncodeRpcidscInfo.rpcidRoot = NULL;
    freeAfterEncodeRpcidscInfo.outBuffer = NULL;
    freeAfterEncodeRpcidscInfo.contextBuffer = NULL;
    freeAfterEncodeRpcidscInfo.type = 0;
    freeAfterEncodeRpcidscInfo.flag = 0;

    if (inputFile == NULL) {
        PRINT_ERR("inputFile is null.\n");
        return -1;
    }

    // check rpcid.sc
    if (CheckRpcidFormat(inputFile, &freeAfterEncodeRpcidscInfo.contextBuffer, &bufferLen) != 0) {
        PRINT_ERR("Check rpcid.sc format failed. Input file: %s\n", inputFile);
        return FreeAfterEncodeRpcidsc(freeAfterEncodeRpcidscInfo, FREE_CONTEXT_OUT_AFTER_RPCIDSC, ret);
    }

    // parse rpcid to json
    freeAfterEncodeRpcidscInfo.rpcidRoot = cJSON_CreateObject();
    if (ParseRpcidToJson(freeAfterEncodeRpcidscInfo.contextBuffer, bufferLen,
            freeAfterEncodeRpcidscInfo.rpcidRoot) != 0) {
        PRINT_ERR("Prase rpcid to json failed. Input file: %s\n", inputFile);
        return FreeAfterEncodeRpcidsc(freeAfterEncodeRpcidscInfo, FREE_RPCID_ROOT_AFTER_RPCIDSC, ret);
    }

    // trans to string format
    freeAfterEncodeRpcidscInfo.sysCapDefine =  CreateWholeSyscapJsonObj();
    sysCapArray = cJSON_GetObjectItem(freeAfterEncodeRpcidscInfo.rpcidRoot, "syscap");
    if (sysCapArray == NULL || !cJSON_IsArray(sysCapArray)) {
        PRINT_ERR("Get syscap failed. Input file: %s\n", inputFile);
        return FreeAfterEncodeRpcidsc(freeAfterEncodeRpcidscInfo, FREE_WHOLE_SYSCAP_AFTER_RPCIDSC, ret);
    }
    sysCapArraySize = cJSON_GetArraySize(sysCapArray);
    if (sysCapArraySize < 0) {
        PRINT_ERR("Get syscap size failed. Input file: %s\n", inputFile);
        return FreeAfterEncodeRpcidsc(freeAfterEncodeRpcidscInfo, FREE_WHOLE_SYSCAP_AFTER_RPCIDSC, ret);
    }
    // malloc for save os syscap index
    freeAfterEncodeRpcidscInfo.osSysCapIndex = (uint16_t *)malloc(sizeof(uint16_t) * sysCapArraySize);
    if (freeAfterEncodeRpcidscInfo.osSysCapIndex == NULL) {
        PRINT_ERR("malloc failed.\n");
        return FreeAfterEncodeRpcidsc(freeAfterEncodeRpcidscInfo, FREE_WHOLE_SYSCAP_AFTER_RPCIDSC, ret);
    }

    ret = OutputSetMemAndPrintToFile(freeAfterEncodeRpcidscInfo, sysCapArraySize, sysCapArray, outDirPath);
    if (freeAfterEncodeRpcidscInfo.flag == 1) {
        return FreeAfterEncodeRpcidsc(freeAfterEncodeRpcidscInfo, freeAfterEncodeRpcidscInfo.type, ret);
    }
    return FreeAfterEncodeRpcidsc(freeAfterEncodeRpcidscInfo, FREE_OUTBUFFER_AFTER_RPCIDSC, ret);
}

char *CopyInputString(const char *inputString)
{
    if (inputString == NULL || *inputString == '\0') {
        PRINT_ERR("inputString is null or empty.\n");
        return NULL;
    }
    size_t inputLen = strlen(inputString);
    if (inputLen > STRING_FORMAT_LEN_MAX) {
        PRINT_ERR("input string too long(%zu).\n", inputLen);
        return NULL;
    }
    char *input = (char *)malloc(inputLen + 1);
    if (input == NULL) {
        PRINT_ERR("malloc failed.\n");
        return NULL;
    }
    int32_t ret = strcpy_s(input, inputLen + 1, inputString);
    if (ret != EOK) {
        PRINT_ERR("strcpy_s failed.\n");
        free(input);
        return NULL;
    }
    input[inputLen] = '\0';
    return input;
}

int32_t GetPriSyscapData(char *input, char **priSyscap, uint32_t *priSyscapLen)
{
    // count private syscaps
    uint32_t count = 0;
    for (uint32_t i = 0; *(input + i) != '\0'; i++) {
        if (*(input + i) == ',') {
            count++;
        }
    }
    count++;
    // get private syscap string
    char *priSysCapOut = (char *)malloc(SINGLE_SYSCAP_LEN * count);
    if (priSysCapOut == NULL) {
        PRINT_ERR("sscanf_s failed.\n");
        return -1;
    }
    (void)memset_s(priSysCapOut, SINGLE_SYSCAP_LEN * count, 0, SINGLE_SYSCAP_LEN * count);
    char *private = priSysCapOut;

    char *tok = NULL;
    char *temp = strtok_r(input, ",", &tok);
    while (temp) {
        int ret = strncpy_s(private, SINGLE_SYSCAP_LEN, temp, SINGLE_SYSCAP_LEN - 1);
        if (ret != EOK) {
            PRINT_ERR("strncpy_s failed.\n");
            free(priSysCapOut);
            return -1;
        }
        temp = strtok_r(NULL, ",", &tok);
        private += SINGLE_SYSCAP_LEN;
    }

    *priSyscap = priSysCapOut;
    *priSyscapLen = count;
    return 0;
}

int32_t SeparateSyscapFromString(const char *inputString, uint32_t *osArray, uint32_t osArraySize,
                                 char **priSyscap, uint32_t *priSyscapLen)
{
    if (osArraySize != PCID_OUT_BUFFER) {
        return -1;
    }

    // copy origin string
    char *input = CopyInputString(inputString);
    if (input == NULL) {
        return -1;
    }

    // get os syscap data
    for (uint32_t i = 0; i < PCID_OUT_BUFFER; i++) {
        if (sscanf_s(input, "%u,%s", &osArray[i], input, strlen(input)) == -1) {
            PRINT_ERR("sscanf_s failed.\n");
            free(input);
            return -1;
        }
    }

    // get private syscap data
    if (GetPriSyscapData(input, priSyscap, priSyscapLen) != 0) {
        free(input);
        return -1;
    }

    free(input);
    return 0;
}

int32_t GetSyscapByIndex(uint32_t index)
{
    const size_t allSyscapNum = sizeof(g_arraySyscap) / sizeof(SyscapWithNum);
    for (uint32_t i = 0; i < allSyscapNum; i++) {
        if (g_arraySyscap[i].num == index) {
            return i;
        }
    }
    return -1;
}

int32_t CompareOsSyscap(const uint32_t pcidOsArray[], const uint32_t rpcidOsAarry[])
{
    int32_t ossyscapFlag = 0;
    const size_t allSyscapNum = sizeof(g_arraySyscap) / sizeof(SyscapWithNum);

    for (uint32_t i = 2; i < PCID_OUT_BUFFER; i++) { // 2, header of pcid & rpcid
        uint32_t blockBits = (pcidOsArray[i] ^ rpcidOsAarry[i]) & rpcidOsAarry[i];
        if (!blockBits) {
            continue;
        }
        for (uint8_t k = 0; k < INT_BIT; k++) {
            if (!(blockBits & (1U << k))) {
                continue;
            }
            // 2, header of pcid & rpcid
            size_t pos = (size_t)((i - 2) * INT_BIT + k);
            if (pos >= allSyscapNum) {
                break;
            }
            printf("Missing: %s\n", g_arraySyscap[GetSyscapByIndex(pos)].str);
            ossyscapFlag += 1;
        }
    }
    return ossyscapFlag;
}

int32_t ComparePriSyscap(char *pcid, char *rpcid, uint32_t pcidLen, uint32_t rpcidLen)
{
    uint32_t i, j;
    bool priSysFound = false;
    int32_t prisyscapFlag = 0;

    for (i = 0; i < rpcidLen; i++) {
        for (j = 0; j < pcidLen; j++) {
            if (strcmp(rpcid + SINGLE_SYSCAP_LEN * i,
                       pcid + SINGLE_SYSCAP_LEN * j) == 0) {
                priSysFound = true;
                break;
            }
        }
        if (priSysFound != true) {
            printf("Missing: %s\n", rpcid + SINGLE_SYSCAP_LEN * i);
            prisyscapFlag += 1;
        }
        priSysFound = false;
    }

    return prisyscapFlag;
}

static int32_t CompareVersion(uint32_t *pcidOsArray, uint32_t *rpcidOsAarry)
{
    int32_t versionFlag = 0;
    uint16_t pcidVersion = NtohsInter(((PCIDMain *)pcidOsArray)->apiVersion);
    uint16_t rpcidVersion = NtohsInter(((RPCIDHead *)rpcidOsAarry)->apiVersion);
    if (pcidVersion < rpcidVersion) {
        printf("ERROR: Pcid version(%u) less than rpcid version(%u).\n", pcidVersion, rpcidVersion);
        versionFlag = 1;
    }
    return versionFlag;
}

int32_t ComparePcidWithRpcidString(char *pcidFile, char *rpcidFile, uint32_t type)
{
    int32_t ret;
    char *pcidContent = NULL;
    char *rpcidContent = NULL;
    char *pcidPriSyscap = NULL;
    char *rpcidPriSyscap = NULL;
    uint32_t pcidContentLen, rpcidContentLen, pcidPriSyscapLen, rpcidPriSyscapLen;
    uint32_t pcidOsArray[PCID_OUT_BUFFER] = {0};
    uint32_t rpcidOsAarry[PCID_OUT_BUFFER] = {0};

    if (pcidFile == NULL || rpcidFile == NULL) {
        PRINT_ERR("pcidFile or rpcidFile is null.\n");
        return -1;
    }

    if (type == TYPE_FILE) {
        if (GetFileContext(pcidFile, &pcidContent, &pcidContentLen)) {
            PRINT_ERR("Get pcid file context failed, input file : %s\n", pcidFile);
            return -1;
        }
        if (GetFileContext(rpcidFile, &rpcidContent, &rpcidContentLen)) {
            PRINT_ERR("Get rpcid file context failed, input file : %s\n", rpcidFile);
            free(pcidContent);
            return -1;
        }
    } else if (type == TYPE_STRING) {
        pcidContent = pcidFile;
        rpcidContent = rpcidFile;
    } else {
        PRINT_ERR("Input file type error, type=%u\n", type);
        return -1;
    }

    ret =  SeparateSyscapFromString(pcidContent, pcidOsArray, PCID_OUT_BUFFER,
                                    &pcidPriSyscap, &pcidPriSyscapLen);
    ret += SeparateSyscapFromString(rpcidContent, rpcidOsAarry, RPCID_OUT_BUFFER,
                                    &rpcidPriSyscap, &rpcidPriSyscapLen);
    if (ret != 0) {
        PRINT_ERR("Separate syscap from string failed. ret = %d\n", ret);
        FreePointerMemory(pcidContent, rpcidContent, pcidPriSyscap, rpcidPriSyscap);
        return -1;
    }

    int32_t versionFlag = CompareVersion(pcidOsArray, rpcidOsAarry);
    int32_t ossyscapFlag = CompareOsSyscap(pcidOsArray, rpcidOsAarry);
    int32_t prisyscapFlag = ComparePriSyscap(pcidPriSyscap, rpcidPriSyscap, pcidPriSyscapLen, rpcidPriSyscapLen);
    if (!versionFlag && !ossyscapFlag && !prisyscapFlag) {
        printf("Succeed! The pcid meets the rpcid.\n");
    } else {
        printf("Fail! The pcid does not meet the rpcid\n");
    }

    return 0;
}
