/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include "securec.h"
#include "cJSON.h"
#include "endian_internal.h"
#include "create_pcid.h"
#include "context_tool.h"
#include "common_method.h"

#ifdef SYSCAP_DEFINE_EXTERN_ENABLE
#include "syscap_define_custom.h"
#else
#include "syscap_define.h"
#endif

#define SYSCAP_PREFIX_LEN 17
#define SINGLE_FEAT_LEN (SINGLE_SYSCAP_LEN - SYSCAP_PREFIX_LEN)
#define PCID_OUT_BUFFER 32
#define PRIVATE_SYSCAP_SIZE 1000
#define UINT8_BIT 8

#define U32_TO_STR_MAX_LEN 11
#define FREE_CREATE_PCID_BUFFER_OUT 1

#define FREE_DECODE_PCID_CONVERT_OUT 1
#define FREE_DECODE_PCID_ROOT_OUT 2
#define FREE_DECODE_PCID_SYSCAP_OUT 3
#define FREE_DECODE_PCID_CONTEXT_OUT 4

#define ENCODE_PCID_OUTPUT 1
#define FREE_ENCODE_PCID_OUT 2
#define FREE_ENCODE_PCID_PRISYSCAP_FULL_OUT 3
#define FREE_ENCODE_PCID_CONTEXT_OUT 4

struct FreeEncodePcidInfo {
    char *output;
    char *priSyscapFull;
    char *contextBuffer;
    char *outDirPathFinal;
};

struct FreeDecodePcidJsonInfo {
    char *strJson;
    char *contextBuffer;
    cJSON *jsonRootObj;
    cJSON *sysCapObj;
    int32_t flag;
};

int32_t SetOsSyscap(PCIDMain *pcidBuffer, uint32_t osCapSize,
                    const cJSON *jsonOsSyscapObj, const cJSON *allOsSyscapObj)
{
    int32_t sectorOfBits, posOfBits;
    cJSON *jsonArrayItem = NULL;
    cJSON *osCapIndex = NULL;

    for (uint32_t i = 0; i < osCapSize; i++) {
        jsonArrayItem = cJSON_GetArrayItem(jsonOsSyscapObj, (int)i);
        if (jsonArrayItem == NULL || !cJSON_IsString(jsonArrayItem)) {
            PRINT_ERR("Get jsonArrayItem failed.");
            return -1;
        }
        osCapIndex = cJSON_GetObjectItem(allOsSyscapObj, jsonArrayItem->valuestring);
        if (osCapIndex == NULL) {
            PRINT_ERR("can't find the syscap: %s, please add it in syscap_define.h.\n", jsonArrayItem->valuestring);
            return -1;
        }
        if (!cJSON_IsNumber(osCapIndex)) {
            PRINT_ERR("Get osCapIndex failed.");
            return -1;
        }
        sectorOfBits = (osCapIndex->valueint) / UINT8_BIT;
        posOfBits = (osCapIndex->valueint) % UINT8_BIT;
        if (sectorOfBits >= OS_SYSCAP_BYTES) {
            PRINT_ERR("num of \"os syscap\" is out of 960\n");
            return -1;
        }
        pcidBuffer->osSyscap[sectorOfBits] |= 1 << (posOfBits);
    }

    return 0;
}

int32_t SetPriSyscap(PCIDMain *pcidBuffer, cJSON *jsonPriSyscapObj,
                     uint32_t privateCapSize, uint16_t allPriSyscapStrLen)
{
    char *priSyscapHead = (char *)(pcidBuffer + 1);
    char *priSyscapStr = NULL;
    for (uint32_t i = 0; i < privateCapSize; i++) {
        cJSON *jsonArrayItem = cJSON_GetArrayItem(jsonPriSyscapObj, (int)i);
        if (jsonArrayItem == NULL || !cJSON_IsString(jsonArrayItem)) {
            PRINT_ERR("get jsonArrayItem failed!");
            return -1;
        }
        priSyscapStr = strchr(jsonArrayItem->valuestring, '.');
        if (priSyscapStr == NULL) {
            PRINT_ERR("get priSyscapStr failed!");
            return -1;
        }
        priSyscapStr += 1;
        errno_t nRet = strcat_s(priSyscapHead, allPriSyscapStrLen + 1, priSyscapStr);
        nRet += strcat_s(priSyscapHead, allPriSyscapStrLen + 1, ",");
        if (nRet != EOK) {
            PRINT_ERR("strcat_s \"pri\" string is failed\n");
            return -1;
        }
    }
    return 0;
}

int32_t SetPCIDHeader(PCIDMain *pcidBuffer, const cJSON *jsonRootObj)
{
    cJSON *jsonSyscapObj = cJSON_GetObjectItem(jsonRootObj, "api_version");
    if (jsonSyscapObj == NULL || !cJSON_IsNumber(jsonSyscapObj)) {
        PRINT_ERR("get \"api_version\" failed\n");
        return -1;
    }
    pcidBuffer->apiVersion = HtonsInter((uint16_t)jsonSyscapObj->valueint);
    pcidBuffer->apiVersionType = 0;

    jsonSyscapObj = cJSON_GetObjectItem(jsonRootObj, "system_type");
    if (jsonSyscapObj == NULL || !cJSON_IsString(jsonSyscapObj)) {
        PRINT_ERR("get \"system_type\" failed\n");
        return -1;
    }
    char *systemType = jsonSyscapObj->valuestring;
    pcidBuffer->systemType = !strcmp(systemType, "mini") ? 0b001 :
                             (!strcmp(systemType, "small") ? 0b010 :
                              (!strcmp(systemType, "standard") ? 0b100 : 0));
    if (pcidBuffer->systemType == 0) {
        PRINT_ERR("\"system_type\" is invaild, systemType = \"%s\"\n", systemType);
        return -1;
    }

    jsonSyscapObj = cJSON_GetObjectItem(jsonRootObj, "manufacturer_id");
    if (jsonSyscapObj == NULL || !cJSON_IsNumber(jsonSyscapObj)) {
        PRINT_ERR("get \"manufacturer_id\" failed\n");
        return -1;
    }
    pcidBuffer->manufacturerID = HtonlInter((uint32_t)jsonSyscapObj->valueint);

    return 0;
}

int32_t GetOsAndPriSyscapSize(const cJSON *osSyscap, const cJSON *priSyscap,
                              uint32_t *osCapSize, uint32_t *privateCapSize)
{
    *osCapSize = 0;
    *privateCapSize = 0;

    // get os syscap size
    if (osSyscap == NULL || !cJSON_IsArray(osSyscap)) {
        PRINT_ERR("get \"os\" array failed\n");
        return -1;
    }
    int32_t ret = cJSON_GetArraySize(osSyscap);
    if (ret < 0) {
        PRINT_ERR("get \"os\" array size failed\n");
        return -1;
    }
    *osCapSize = (uint32_t)ret;

    // get private syscap size
    if (priSyscap != NULL && cJSON_IsArray(priSyscap)) {
        ret = cJSON_GetArraySize(priSyscap);
        if (ret < 0) {
            PRINT_ERR("get \"private syscap\" array size failed\n");
            return -1;
        }
        *privateCapSize = (uint32_t)ret;
    } else if (priSyscap == NULL) {
        *privateCapSize = 0;
    } else {
        PRINT_ERR("get \"private\" array failed\n");
        return -1;
    }

    return 0;
}

int32_t GetPriSyscapLen(uint32_t privateCapSize, cJSON *jsonPriSyscapObj, uint16_t *len)
{
    for (uint32_t i = 0; i < privateCapSize; i++) {
        cJSON *jsonArrayItem = cJSON_GetArrayItem(jsonPriSyscapObj, (int)i);
        if (jsonArrayItem == NULL || !cJSON_IsString(jsonArrayItem)) {
            PRINT_ERR("Get jsonArrayItem failed.");
            return -1;
        }
        *len += (uint16_t)strlen(strchr(jsonArrayItem->valuestring, '.') + 1);
        (*len)++; // for separator ','
    }
    if ((*len + 1) > PRIVATE_SYSCAP_SIZE) {
        PRINT_ERR("context of \"pri\" array is too many.\n");
        return -1;
    }
    return 0;
}

static int32_t CheckConvertedContextSaveAsFile(char *outDirPath, PCIDMain *pcidBuffer, uint16_t pcidLength, int32_t ret)
{
    const char pcidFileName[] = "pcid.sc";
    ret = ConvertedContextSaveAsFile(outDirPath, pcidFileName, (char *)pcidBuffer, pcidLength);
    if (ret != 0) {
        PRINT_ERR("Save as file failed, outDirPath:%s, filename:%s\n", outDirPath, pcidFileName);
    }
    return ret;
}

static int32_t FreeAfterCreatePCID(PCIDMain *pcidBuffer, cJSON *allOsSyscapObj, char *contextBuffer,
    int32_t type, int32_t ret)
{
    if (type == FREE_CREATE_PCID_BUFFER_OUT) {
        free(pcidBuffer);
    }
    cJSON_Delete(allOsSyscapObj);
    FreeContextBuffer(contextBuffer);
    return ret;
}

static int32_t PreFreeAfterCreatePCID(PCIDMain *pcidBuffer, cJSON *allOsSyscapObj, cJSON *jsonRootObj,
    char *contextBuffer, int32_t type)
{
    cJSON_Delete(jsonRootObj);
    return FreeAfterCreatePCID(pcidBuffer, allOsSyscapObj, contextBuffer, type, -1);
}

int32_t CreatePCID(char *inputFile, char *outDirPath)
{
    uint32_t privateCapSize, osCapSize;
    uint32_t contextBufLen;
    char *contextBuffer = NULL;

    cJSON *allOsSyscapObj = CreateWholeSyscapJsonObj();
    int32_t ret = CheckFileAndGetFileContext(inputFile, &contextBuffer, (uint32_t *)&contextBufLen);
    if (ret != 0) {
        return FreeAfterCreatePCID(NULL, allOsSyscapObj, contextBuffer, 0, -1);
    }

    cJSON *jsonRootObj = cJSON_ParseWithLength(contextBuffer, contextBufLen);
    if (jsonRootObj == NULL) {
        PRINT_ERR("cJSON_Parse failed, context buffer is:\n%s\n", contextBuffer);
        return PreFreeAfterCreatePCID(NULL, allOsSyscapObj, jsonRootObj, contextBuffer, 0);
    }

    cJSON *jsonSyscapObj = cJSON_GetObjectItem(jsonRootObj, "syscap");
    if (jsonSyscapObj == NULL || !cJSON_IsObject(jsonSyscapObj)) {
        PRINT_ERR("get \"syscap\" object failed\n");
        return PreFreeAfterCreatePCID(NULL, allOsSyscapObj, jsonRootObj, contextBuffer, 0);
    }

    cJSON *jsonOsSyscapObj = cJSON_GetObjectItem(jsonSyscapObj, "os");
    cJSON *jsonPriSyscapObj = cJSON_GetObjectItem(jsonSyscapObj, "private");

    ret = GetOsAndPriSyscapSize(jsonOsSyscapObj, jsonPriSyscapObj, &osCapSize, &privateCapSize);
    if (ret != 0) {
        return PreFreeAfterCreatePCID(NULL, allOsSyscapObj, jsonRootObj, contextBuffer, 0);
    }

    uint16_t allPriSyscapStrLen = 0;
    ret = GetPriSyscapLen(privateCapSize, jsonPriSyscapObj, &allPriSyscapStrLen);
    if (ret != 0) {
        return PreFreeAfterCreatePCID(NULL, allOsSyscapObj, jsonRootObj, contextBuffer, 0);
    }

    uint16_t pcidLength = sizeof(PCIDMain) + allPriSyscapStrLen + 1;
    PCIDMain *pcidBuffer = (PCIDMain *)malloc(pcidLength);
    if (pcidBuffer == NULL) {
        PRINT_ERR("malloc for pcid buffer failed\n");
        return PreFreeAfterCreatePCID(NULL, allOsSyscapObj, jsonRootObj, contextBuffer, 0);
    }
    (void)memset_s(pcidBuffer, pcidLength, 0, pcidLength);

    ret = SetOsSyscap(pcidBuffer, osCapSize, jsonOsSyscapObj, allOsSyscapObj);
    ret += SetPriSyscap(pcidBuffer, jsonPriSyscapObj, privateCapSize, allPriSyscapStrLen);
    ret += SetPCIDHeader(pcidBuffer, jsonRootObj);
    if (ret != 0) {
        return PreFreeAfterCreatePCID(pcidBuffer, allOsSyscapObj, jsonRootObj, contextBuffer,
            FREE_CREATE_PCID_BUFFER_OUT);
    }

    ret = CheckConvertedContextSaveAsFile(outDirPath, pcidBuffer, pcidLength, ret);
    cJSON_Delete(jsonRootObj);
    return FreeAfterCreatePCID(pcidBuffer, allOsSyscapObj, contextBuffer, FREE_CREATE_PCID_BUFFER_OUT, ret);
}

int32_t GetOsSyscap(PCIDMain *pcidMain, cJSON *sysCapObject)
{
    uint32_t i, j, countOfSyscap = 0;
    uint8_t osSyscap[OS_SYSCAP_BYTES] = {0};
    uint16_t indexOfSyscap[OS_SYSCAP_BYTES * UINT8_BIT] = {0};

    cJSON *capVectorPtr = cJSON_CreateArray();
    if (capVectorPtr == NULL) {
        PRINT_ERR("cJSON_CreateArray failed\n");
        return -1;
    }

    // 8, bytes of pcid header
    errno_t nRet = memcpy_s(osSyscap, OS_SYSCAP_BYTES, (uint8_t *)pcidMain + 8, OS_SYSCAP_BYTES);
    if (nRet != EOK) {
        PRINT_ERR("memcpy_s failed.");
        cJSON_Delete(capVectorPtr);
        return -1;
    }

    for (i = 0; i < OS_SYSCAP_BYTES; i++) {
        for (j = 0; j < UINT8_BIT; j++) {
            if (osSyscap[i] & (0x01 << j)) {
                indexOfSyscap[countOfSyscap++] = i * UINT8_BIT + j;
            }
        }
    }
    for (i = 0; i < countOfSyscap; i++) {
        for (j = 0; j < sizeof(g_arraySyscap) / sizeof(SyscapWithNum); j++) {
            if (g_arraySyscap[j].num != indexOfSyscap[i]) {
                continue;
            }
            if (!cJSON_AddItemToArray(capVectorPtr, cJSON_CreateString(g_arraySyscap[j].str))) {
                printf("cJSON_AddItemToArray or cJSON_CreateString failed\n");
                cJSON_Delete(capVectorPtr);
                return -1;
            }
        }
    }

    if (!cJSON_AddItemToObject(sysCapObject, "os", capVectorPtr)) {
        PRINT_ERR("cJSON_AddItemToObject failed\n");
        cJSON_Delete(capVectorPtr);
        return -1;
    }

    return 0;
}

static int32_t GetPriSyscapResult(cJSON *capVectorPtr, int32_t ret)
{
    cJSON_Delete(capVectorPtr);
    return ret;
}

static int32_t GetPriSyscap(PCIDMain *pcidMain, cJSON *sysCapObject, int32_t contextBufLen)
{
    cJSON *capVectorPtr = cJSON_CreateArray();
    if (capVectorPtr == NULL) {
        PRINT_ERR("cJSON_CreateArray failed\n");
        return -1;
    }

    int32_t privateSyscapLen = contextBufLen - sizeof(PCIDMain) - 1;
    if (privateSyscapLen < 0 || privateSyscapLen > INT32_MAX) {
        PRINT_ERR("parse private syscap failed.");
        return GetPriSyscapResult(capVectorPtr, -1);
    } else if (privateSyscapLen == 0) {
        return GetPriSyscapResult(capVectorPtr, 0);
    }

    char fullCapStr[SINGLE_SYSCAP_LEN] = {0};
    char priSyscapStr[SINGLE_SYSCAP_LEN] = {0};
    char *tempPriSyscapStr = priSyscapStr;
    char *ptrPrivateSyscap = (char *)(pcidMain + 1);
    while (*ptrPrivateSyscap != '\0') {
        size_t currentLen = tempPriSyscapStr - priSyscapStr;
        if (currentLen >= SINGLE_SYSCAP_LEN - 1) {
            printf("Syscap name exceeds maximum length\n");
            return GetPriSyscapResult(capVectorPtr, -1);
        }
        if (*ptrPrivateSyscap == ',') {
            *tempPriSyscapStr = '\0';
            int32_t ret = sprintf_s(fullCapStr, SINGLE_SYSCAP_LEN, "SystemCapability.%s", priSyscapStr);
            if (ret == -1) {
                printf("sprintf_s failed\n");
                return GetPriSyscapResult(capVectorPtr, -1);
            }
            if (!cJSON_AddItemToArray(capVectorPtr, cJSON_CreateString(fullCapStr))) {
                printf("cJSON_AddItemToArray or cJSON_CreateString failed\n");
                return GetPriSyscapResult(capVectorPtr, -1);
            }
            tempPriSyscapStr = priSyscapStr;
            ptrPrivateSyscap++;
            continue;
        }
        *tempPriSyscapStr++ = *ptrPrivateSyscap++;
    }
    if (!cJSON_AddItemToObject(sysCapObject, "private", capVectorPtr)) {
        PRINT_ERR("cJSON_AddItemToObject failed\n");
        return GetPriSyscapResult(capVectorPtr, -1);
    }
    return 0;
}

static int32_t CheckSysCapObj(struct FreeDecodePcidJsonInfo freePcidJsonInfo, PCIDMain *pcidMain,
        uint32_t contextBufLen, int32_t ret)
{
    if (freePcidJsonInfo.sysCapObj == NULL) {
        PRINT_ERR("cJSON_CreateObject failed\n");
        freePcidJsonInfo.flag = -1;
        return -1;
    }
    if (GetOsSyscap(pcidMain, freePcidJsonInfo.sysCapObj) != 0) {
        freePcidJsonInfo.flag = -1;
        return ret;
    }
    if (GetPriSyscap(pcidMain, freePcidJsonInfo.sysCapObj, contextBufLen) != 0) {
        freePcidJsonInfo.flag = -1;
    }
    return ret;
}

static int32_t CheckJsonRootObj(struct FreeDecodePcidJsonInfo freePcidJsonInfo, PCIDMain *pcidMain, char *systemType)
{
    if (!cJSON_AddNumberToObject(freePcidJsonInfo.jsonRootObj, "api_version", NtohsInter(pcidMain->apiVersion))) {
        PRINT_ERR("cJSON_AddNumberToObject failed\n");
        return -1;
    }
    if (!cJSON_AddNumberToObject(freePcidJsonInfo.jsonRootObj, "manufacturer_id",
                                 NtohlInter(pcidMain->manufacturerID))) {
        PRINT_ERR("cJSON_AddNumberToObject failed\n");
        return -1;
    }
    if (!cJSON_AddStringToObject(freePcidJsonInfo.jsonRootObj, "system_type", systemType)) {
        PRINT_ERR("cJSON_AddStringToObject failed\n");
        return -1;
    }
    if (!cJSON_AddItemToObject(freePcidJsonInfo.jsonRootObj, "syscap", freePcidJsonInfo.sysCapObj)) {
        PRINT_ERR("cJSON_AddItemToObject failed\n");
        return -1;
    }
    return 0;
}

static int32_t FreeAfterDecodePCID(struct FreeDecodePcidJsonInfo freePcidJsonInfo, int32_t type, int32_t ret)
{
    switch (type) {
        case FREE_DECODE_PCID_CONVERT_OUT:
            cJSON_free(freePcidJsonInfo.strJson);
            cJSON_Delete(freePcidJsonInfo.jsonRootObj);
            FreeContextBuffer(freePcidJsonInfo.contextBuffer);
            break;
        case FREE_DECODE_PCID_ROOT_OUT:
            cJSON_Delete(freePcidJsonInfo.jsonRootObj);
            cJSON_Delete(freePcidJsonInfo.sysCapObj);
            FreeContextBuffer(freePcidJsonInfo.contextBuffer);
            break;
        case FREE_DECODE_PCID_SYSCAP_OUT:
            cJSON_Delete(freePcidJsonInfo.sysCapObj);
            FreeContextBuffer(freePcidJsonInfo.contextBuffer);
            break;
        case FREE_DECODE_PCID_CONTEXT_OUT:
            cJSON_Delete(freePcidJsonInfo.sysCapObj);
            FreeContextBuffer(freePcidJsonInfo.contextBuffer);
            break;
        default:
            FreeContextBuffer(freePcidJsonInfo.contextBuffer);
    }
    return ret;
}

int32_t DecodePCID(char *inputFile, char *outDirPath)
{
    int32_t ret = 0;
    uint32_t contextBufLen;
    struct FreeDecodePcidJsonInfo freePcidJsonInfo;
    freePcidJsonInfo.strJson = NULL;
    freePcidJsonInfo.contextBuffer = NULL;
    freePcidJsonInfo.jsonRootObj = NULL;
    freePcidJsonInfo.sysCapObj = NULL;
    freePcidJsonInfo.flag = 0;

    ret = CheckFileAndGetFileContext(inputFile, &freePcidJsonInfo.contextBuffer, (uint32_t *)&contextBufLen);
    if (ret != 0) {
        return -1;
    }

    PCIDMain *pcidMain = (PCIDMain *)freePcidJsonInfo.contextBuffer;

    /* api version */
    if (pcidMain->apiVersionType != 0) {
        PRINT_ERR("Prase file failed, apiVersionType is invaild, input file : %s\n", inputFile);
        return FreeAfterDecodePCID(freePcidJsonInfo, FREE_DECODE_PCID_CONTEXT_OUT, -1);
    }

    /* system type */
    char *systemType = pcidMain->systemType == 0b001 ? "mini" :(pcidMain->systemType == 0b010 ? "small" :
                        (pcidMain->systemType == 0b100 ? "standard" : NULL));
    if (systemType == NULL) {
        PRINT_ERR("prase file failed, systemType is invaild, %u\n", pcidMain->systemType);
        return FreeAfterDecodePCID(freePcidJsonInfo, FREE_DECODE_PCID_CONTEXT_OUT, -1);
    }

    /* syscap */
    freePcidJsonInfo.sysCapObj = cJSON_CreateObject();
    ret = CheckSysCapObj(freePcidJsonInfo, pcidMain, contextBufLen, ret);
    if (freePcidJsonInfo.flag == -1) {
        return FreeAfterDecodePCID(freePcidJsonInfo, FREE_DECODE_PCID_CONTEXT_OUT, ret);
    }

    // create json root
    freePcidJsonInfo.jsonRootObj = cJSON_CreateObject();
    if (freePcidJsonInfo.jsonRootObj == NULL) {
        PRINT_ERR("cJSON_CreateObject failed\n");
        return FreeAfterDecodePCID(freePcidJsonInfo, FREE_DECODE_PCID_SYSCAP_OUT, -1);
    }

    ret = CheckJsonRootObj(freePcidJsonInfo, pcidMain, systemType);
    if (ret == -1) {
        return FreeAfterDecodePCID(freePcidJsonInfo, FREE_DECODE_PCID_ROOT_OUT, ret);
    }

    freePcidJsonInfo.strJson = cJSON_Print(freePcidJsonInfo.jsonRootObj);
    if (freePcidJsonInfo.strJson  == NULL) {
        return FreeAfterDecodePCID(freePcidJsonInfo, FREE_DECODE_PCID_ROOT_OUT, -1);
    }

    const char outputFileName[] = "pcid.json";
    ret = ConvertedContextSaveAsFile(outDirPath, outputFileName, freePcidJsonInfo.strJson,
                                     strlen(freePcidJsonInfo.strJson));
    if (ret != 0) {
        PRINT_ERR("ConvertedContextSaveAsFile failed, outDirPath:%s, filename:%s\n", outDirPath, outputFileName);
    }
    return FreeAfterDecodePCID(freePcidJsonInfo, FREE_DECODE_PCID_CONVERT_OUT, ret);
}

#define U32_TO_STR_MAX_LEN 11
#define OS_SYSCAP_NUM 30
#define PCID_HEADER 2
static int32_t ParseStringSyscap(char *input, uint32_t *osSyscap, uint32_t osSyscapNum,
                                 uint32_t *header, uint32_t headerLen)
{
    int32_t ret;
    uint32_t tempNum;
    uint32_t i = 0;
    size_t inputLen = strlen(input);

    if (osSyscapNum != OS_SYSCAP_NUM || headerLen != PCID_HEADER) {
        PRINT_ERR("Input osSyscapNum(%u) or headerLen(%u) error.\n", osSyscapNum, headerLen);
        return -1;
    }

    if (sscanf_s(input, "%u,%u,%s", &header[0], &header[1], input, inputLen) != 3) { // 3, return val of "%u,%u,%s"
        PRINT_ERR("Get pcid header failed.\n");
        return -1;
    }

    while ((ret = sscanf_s(input, "%u,%s", &tempNum, input, inputLen)) > 0) {
        osSyscap[i++] = tempNum;
        if (i >= OS_SYSCAP_NUM) {
            break;
        }
    }
    if (ret == -1) {
        PRINT_ERR("sscanf_s failed, i = %u.\n", i);
        return -1;
    }

    if (strlen(input) <= 1) {
        *input = '\0';
    }

    return 0;
}

static int32_t AddHeaderToJsonObj(uint32_t *pcidHeader, uint32_t pcidHeaderLen, cJSON *rootObj)
{
    if (pcidHeaderLen != PCID_HEADER) {
        PRINT_ERR("input pcidHeader(%u) error.\n", pcidHeaderLen);
        return -1;
    }

    PCIDHeader *header = (PCIDHeader *)pcidHeader;
    // trans system type to string
    char *systemType = header->systemType  == 0b001 ? "mini" :
                       (header->systemType == 0b010 ? "small" :
                       (header->systemType == 0b100 ? "standard" : NULL));
    if (systemType == NULL) {
        PRINT_ERR("prase system type failed.\n");
        return -1;
    }

    // add to json
    if (!cJSON_AddNumberToObject(rootObj, "api_version", NtohsInter(header->apiVersion))) {
        PRINT_ERR("add api_version(%u) to json object failed.\n", NtohsInter(header->apiVersion));
        return -1;
    }
    if (!cJSON_AddNumberToObject(rootObj, "manufacturer_id", NtohlInter(header->manufacturerID))) {
        PRINT_ERR("add manufacturer_id(%u) to json object failed\n", NtohlInter(header->manufacturerID));
        return -1;
    }
    if (!cJSON_AddStringToObject(rootObj, "system_type", systemType)) {
        PRINT_ERR("add system_type(%s) to json object failed\n", systemType);
        return -1;
    }
    return 0;
}

static int32_t AddOsSyscapToJsonObj(uint32_t *osSyscapArray, uint32_t osSyscapArrayLen, cJSON *sysCapObj)
{
    cJSON *sysCapArray = cJSON_CreateArray();
    if (sysCapArray == NULL) {
        PRINT_ERR("Create cJSON array failed.\n");
        return -1;
    }

    if (osSyscapArrayLen != OS_SYSCAP_NUM) {
        PRINT_ERR("Input os syscap array len error.\n");
        cJSON_Delete(sysCapArray);
        return -1;
    }
    uint8_t *osSysCapArrayUint8 = (uint8_t *)osSyscapArray;

    uint32_t i, j;
    uint32_t osSyscapCount = 0;
    uint16_t index[OS_SYSCAP_BYTES * UINT8_BIT] = {0};
    for (i = 0; i < OS_SYSCAP_BYTES; i++) {
        for (j = 0; j < UINT8_BIT; j++) {
            if (osSysCapArrayUint8[i] & (0x01 << j)) {
                index[osSyscapCount++] = i * UINT8_BIT + j;
            }
        }
    }

    for (i = 0; i < osSyscapCount; i++) {
        for (j = 0; j < sizeof(g_arraySyscap) / sizeof(SyscapWithNum); j++) {
            if (index[i] != g_arraySyscap[j].num) {
                continue;
            }
            if (!cJSON_AddItemToArray(sysCapArray, cJSON_CreateString(g_arraySyscap[j].str))) {
                PRINT_ERR("Add os syscap string to json failed.\n");
                cJSON_Delete(sysCapArray);
                return -1;
            }
            break;
        }
    }

    if (!cJSON_AddItemToObject(sysCapObj, "os", sysCapArray)) {
        PRINT_ERR("Add os syscap item to json object failed.\n");
        cJSON_Delete(sysCapArray);
        return -1;
    }
    return 0;
}

static int32_t AddPriSyscapToJsonObj(char *priSyscapString, uint32_t priSyscapStringLen, cJSON *sysCapObj)
{
    char *token = NULL;

    cJSON *sysCapArray = cJSON_CreateArray();
    if (sysCapArray == NULL) {
        PRINT_ERR("Create cJSON array failed.\n");
        return -1;
    }
    if (priSyscapStringLen == 0) {
        if (!cJSON_AddItemToObject(sysCapObj, "private", sysCapArray)) {
            PRINT_ERR("Add private syscap array to json failed.\n");
            cJSON_Delete(sysCapArray);
            return -1;
        }
        cJSON_Delete(sysCapArray);
        return 0;
    }

    token = strtok(priSyscapString, ",");
    while (token != NULL) {
        if (!cJSON_AddItemToArray(sysCapArray, cJSON_CreateString(token))) {
            PRINT_ERR("Add private syscap string to json failed.\n");
            cJSON_Delete(sysCapArray);
            return -1;
        }
        token = strtok(NULL, ",");
    }
    if (!cJSON_AddItemToObject(sysCapObj, "private", sysCapArray)) {
        PRINT_ERR("Add private syscap array to json failed.\n");
        cJSON_Delete(sysCapArray);
        return -1;
    }
    return 0;
}

static int32_t GetSyscapStr(char *input, char const *priSyscapStr, uint32_t* osSyscap, uint32_t *pcidHeader)
{
    if (input == NULL) {
        PRINT_ERR("inputFile is null.\n");
        return -1;
    }
    char *ctx = NULL;
    uint32_t fileContextLen;
    if (GetFileContext(input, &ctx, (uint32_t *)&fileContextLen) != 0) {
        PRINT_ERR("GetFileContext failed, input file : %s\n", input);
        return -1;
    }
    if (ParseStringSyscap(ctx, osSyscap, OS_SYSCAP_NUM, pcidHeader, PCID_HEADER) != 0) {
        PRINT_ERR("Parse string syscap failed.\n");
        free(ctx);
        return -1;
    }
    priSyscapStr = ctx;
    return 0;
}

int32_t DecodeStringPCIDToJson(char *input, char *outDirPath)
{
    int32_t ret = -1;
    uint32_t osSyscap[OS_SYSCAP_NUM] = {0};
    uint32_t pcidHeader[PCID_HEADER];
    char *priSyscapStr = NULL;
    char *jsonBuffer = NULL;

    ret = GetSyscapStr(input, priSyscapStr, osSyscap, pcidHeader);
    if (ret == -1) {
        return ret;
    }

    // add to json object
    cJSON *sysCapObj = cJSON_CreateObject();
    cJSON *rootObj = cJSON_CreateObject();
    if (sysCapObj == NULL || rootObj == NULL) {
        PRINT_ERR("Failed to create cJSON objects.\n");
        goto FAILED;
    }

    if (!cJSON_AddItemToObject(rootObj, "syscap", sysCapObj)) {
        PRINT_ERR("Add syscap to json failed.\n");
        goto FAILED;
    }
    if (AddHeaderToJsonObj(pcidHeader, PCID_HEADER, rootObj) != 0) {
        PRINT_ERR("Add header to json object failed.\n");
        goto FAILED;
    }
    if (AddOsSyscapToJsonObj(osSyscap, OS_SYSCAP_NUM, sysCapObj) != 0) {
        PRINT_ERR("Add os syscap json object failed.\n");
        goto FAILED;
    }

    if (AddPriSyscapToJsonObj(priSyscapStr, (uint32_t) strlen(priSyscapStr), sysCapObj) != 0) {
        PRINT_ERR("Add private syscap json object failed.\n");
        goto FAILED;
    }
    // save as json file
    jsonBuffer = cJSON_Print(rootObj);
    if (jsonBuffer == NULL) {
        PRINT_ERR("json buffer is null.\n");
        goto FAILED;
    }
    const char outputFileName[] = "pcid.json";
    if (ConvertedContextSaveAsFile(outDirPath, outputFileName, jsonBuffer, strlen(jsonBuffer)) != 0) {
        PRINT_ERR("Save as json file failed.\n");
        goto FAILED;
    }
    ret = 0;

FAILED:
    cJSON_free(jsonBuffer);
    SafeFree(priSyscapStr);
    cJSON_Delete(sysCapObj);
    cJSON_Delete(rootObj);
    return ret;
}

static int32_t FreeAfterEncodePCID(struct FreeEncodePcidInfo freePcidInfo, int32_t type, int32_t ret)
{
    switch (type) {
        case FREE_ENCODE_PCID_OUT:
            free(freePcidInfo.output);
            free(freePcidInfo.priSyscapFull);
            free(freePcidInfo.contextBuffer);
            break;
        case FREE_ENCODE_PCID_PRISYSCAP_FULL_OUT:
            free(freePcidInfo.priSyscapFull);
            free(freePcidInfo.contextBuffer);
            break;
        case FREE_ENCODE_PCID_CONTEXT_OUT:
        default:
            free(freePcidInfo.contextBuffer);
    }
    return ret;
}

static int32_t GetEncodePCIDOut(uint16_t priSyscapCount, uint32_t privateSyscapLen, uint32_t *mainSyscap,
        struct FreeEncodePcidInfo freePcidInfo, int32_t ret)
{
    // 17, size of "SystemCapability."
    uint32_t outputLen = U32_TO_STR_MAX_LEN * PCID_OUT_BUFFER + 17 * priSyscapCount + privateSyscapLen + 1;
    char *output = NULL;
    uint32_t i;
    output = (char *)malloc(outputLen);
    if (output == NULL) {
        PRINT_ERR("malloc failed\n");
        return FreeAfterEncodePCID(freePcidInfo, FREE_ENCODE_PCID_PRISYSCAP_FULL_OUT, ret);
    }
    (void)memset_s(output, outputLen, 0, outputLen);
    ret = sprintf_s(output, outputLen, "%u", mainSyscap[0]);
    if (ret == -1) {
        PRINT_ERR("sprintf_s failed\n");
        free(output);
        return FreeAfterEncodePCID(freePcidInfo, FREE_ENCODE_PCID_OUT, ret);
    }
    for (i = 1; i < PCID_OUT_BUFFER; i++) {
        ret = sprintf_s(output, outputLen, "%s,%u", output, mainSyscap[i]);
        if (ret == -1) {
            PRINT_ERR("sprintf_s failed\n");
            free(output);
            return FreeAfterEncodePCID(freePcidInfo, FREE_ENCODE_PCID_OUT, ret);
        }
    }
    for (i = 0; i < priSyscapCount; i++) {
        ret = sprintf_s(output, outputLen, "%s,%s", output, freePcidInfo.priSyscapFull + i * SINGLE_SYSCAP_LEN);
        if (ret == -1) {
            PRINT_ERR("sprintf_s failed\n");
            free(output);
            return FreeAfterEncodePCID(freePcidInfo, FREE_ENCODE_PCID_OUT, ret);
        }
    }
    // save as file
    const char outputFileName[] = "pcid.txt";
    ret = ConvertedContextSaveAsFile(freePcidInfo.outDirPathFinal, outputFileName, output, strlen(output));
    if (ret != 0) {
        PRINT_ERR("ConvertedContextSaveAsFile failed, outDirPath:%s, filename:%s\n",
                  freePcidInfo.outDirPathFinal, outputFileName);
    }
    free(output);
    return FreeAfterEncodePCID(freePcidInfo, FREE_ENCODE_PCID_OUT, ret);
}

static int32_t CheckPrivateSyCap(struct FreeEncodePcidInfo freePcidInfo, uint32_t privateSyscapLen,
    char *privateSyscap, int32_t ret)
{
    uint32_t i, j;
    char tempSyscap[SINGLE_SYSCAP_LEN] = {0};
    char *temp = tempSyscap;
    for (i = 0, j = 0; i < privateSyscapLen; i++) {
        if (*privateSyscap == ',') {
            *temp = '\0';
            ret = sprintf_s(freePcidInfo.priSyscapFull + j * SINGLE_SYSCAP_LEN, SINGLE_SYSCAP_LEN,
                            "SystemCapability.%s", tempSyscap);
            if (ret == -1) {
                PRINT_ERR("sprintf_s failed\n");
                return ret;
            }
            temp = tempSyscap;
            privateSyscap++;
            j++;
            continue;
        }
        *temp++ = *privateSyscap++;
    }
    return ret;
}

int32_t EncodePcidscToString(char *inputFile, char *outDirPath)
{
    int32_t ret = 0;
    uint32_t bufferLen, privateSyscapLen;
    uint32_t i;
    uint32_t *mainSyscap = NULL;
    uint16_t priSyscapCount = 0;

    char *privateSyscap = NULL;
    struct FreeEncodePcidInfo freePcidInfo;
    freePcidInfo.contextBuffer = NULL;
    freePcidInfo.priSyscapFull = NULL;
    freePcidInfo.output = NULL;
    freePcidInfo.outDirPathFinal = outDirPath;
    PCIDMain *pcidMain = NULL;

    ret = CheckFileAndGetFileContext(inputFile, &freePcidInfo.contextBuffer, (uint32_t *)&bufferLen);
    if (ret != 0) {
        return -1;
    }

    if (bufferLen > 1128) { // 1128, max size of pcid.sc
        PRINT_ERR("Input pcid file too large, pcid file size: %u\n", bufferLen);
        return FreeAfterEncodePCID(freePcidInfo, FREE_ENCODE_PCID_CONTEXT_OUT, ret);
    }

    pcidMain = (PCIDMain *)freePcidInfo.contextBuffer;
    privateSyscap = (char *)(pcidMain + 1);
    privateSyscapLen = strlen(privateSyscap);

    // process os syscap
    mainSyscap = (uint32_t *)pcidMain;

    // process private syscap
    for (i = 0; i < privateSyscapLen; i++) {
        if (privateSyscap[i] == ',') {
            priSyscapCount++;
        }
    }
    if (priSyscapCount == 0) {
        return GetEncodePCIDOut(priSyscapCount, privateSyscapLen, mainSyscap,  freePcidInfo, ret);
    }
    freePcidInfo.priSyscapFull = (char *)malloc(priSyscapCount * SINGLE_SYSCAP_LEN);
    if (freePcidInfo.priSyscapFull == NULL) {
        PRINT_ERR("malloc failed\n");
        return FreeAfterEncodePCID(freePcidInfo, FREE_ENCODE_PCID_CONTEXT_OUT, ret);
    }
    (void)memset_s(freePcidInfo.priSyscapFull, priSyscapCount * SINGLE_SYSCAP_LEN,
                   0, priSyscapCount * SINGLE_SYSCAP_LEN);

    ret = CheckPrivateSyCap(freePcidInfo, privateSyscapLen, privateSyscap, ret);
    if (ret == -1) {
        return FreeAfterEncodePCID(freePcidInfo, FREE_ENCODE_PCID_PRISYSCAP_FULL_OUT, ret);
    }

    // output
    return GetEncodePCIDOut(priSyscapCount, privateSyscapLen, mainSyscap,  freePcidInfo, ret);
}