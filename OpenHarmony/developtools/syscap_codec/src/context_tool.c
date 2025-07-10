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

#include "context_tool.h"

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>
#include "securec.h"

#include "endian_internal.h"

#ifdef SYSCAP_DEFINE_EXTERN_ENABLE
#include "syscap_define_custom.h"
#else
#include "syscap_define.h"
#endif

void FreeContextBuffer(char *contextBuffer)
{
    if (contextBuffer != NULL) {
        (void)free(contextBuffer);
    }
}

int32_t GetFileContext(const char *inputFile, char **contextBufPtr, uint32_t *bufferLen)
{
    int32_t ret;
    FILE *fp = NULL;
    struct stat statBuf;
    char *contextBuffer = NULL;
    char path[PATH_MAX + 1] = {0x00};

#ifdef _POSIX_
    if (strlen(inputFile) > PATH_MAX || strncpy_s(path, PATH_MAX, inputFile, strlen(inputFile)) != EOK) {
        PRINT_ERR("get path(%s) failed\n", inputFile);
        return -1;
    }
#else
    if (strlen(inputFile) > PATH_MAX || realpath(inputFile, path) == NULL) {
        PRINT_ERR("get file(%s) real path failed\n", inputFile);
        return -1;
    }
#endif

    ret = stat(path, &statBuf);
    if (ret != 0) {
        PRINT_ERR("get file(%s) st_mode failed, errno = %d\n", path, errno);
        return -1;
    }
    if (!(statBuf.st_mode & S_IRUSR)) {
        PRINT_ERR("don't have permission to read the file(%s)\n", path);
        return -1;
    }
    contextBuffer = (char *)malloc(statBuf.st_size + 1);
    if (contextBuffer == NULL) {
        PRINT_ERR("malloc buffer failed, size = %d, errno = %d\n", (int32_t)statBuf.st_size + 1, errno);
        return -1;
    }
    fp = fopen(path, "rb");
    if (fp == NULL) {
        PRINT_ERR("open file(%s) failed, errno = %d\n", path, errno);
        FreeContextBuffer(contextBuffer);
        return -1;
    }
    size_t retFread = fread(contextBuffer, statBuf.st_size, 1, fp);
    if (retFread != 1) {
        PRINT_ERR("read file(%s) failed, errno = %d\n", path, errno);
        FreeContextBuffer(contextBuffer);
        (void)fclose(fp);
        return -1;
    }
    contextBuffer[statBuf.st_size] = '\0';
    (void)fclose(fp);

    *contextBufPtr = contextBuffer;
    *bufferLen = statBuf.st_size + 1;
    return 0;
}

int32_t CheckFileAndGetFileContext(const char *inputFile, char **contextBufPtr, uint32_t *bufferLen)
{
    if (inputFile == NULL) {
        PRINT_ERR("input file is NULL.\n");
        return -1;
    }
    int32_t ret = GetFileContext(inputFile, contextBufPtr, bufferLen);
    if (ret != 0) {
        PRINT_ERR("GetFileContext failed, input file : %s\n", inputFile);
    }
    return ret;
}

int32_t ConvertedContextSaveAsFile(char *outDirPath, const char *filename, char *convertedBuffer, size_t contextBufLen)
{
    int32_t ret;
    FILE *fp = NULL;
    char path[PATH_MAX + 1] = {0x00};

#ifdef _POSIX_
    if (strlen(outDirPath) >= PATH_MAX || strncpy_s(path, PATH_MAX, outDirPath, strlen(outDirPath)) != EOK) {
        PRINT_ERR("get path(%s) failed\n", outDirPath);
        return -1;
    }
#else
    if (strlen(outDirPath) >= PATH_MAX || realpath(outDirPath, path) == NULL) {
        PRINT_ERR("get file(%s) real path failed\n", outDirPath);
        return -1;
    }
#endif
    int32_t pathLen = strlen(path);
    if (path[pathLen - 1] != '/' && path[pathLen - 1] != '\\') {
        path[pathLen] = '/';
    }

    if (strlen(path) + strlen(filename) + 1 > PATH_MAX) {
        PRINT_ERR("length of path too long.\n");
        return -1;
    }
    ret = strncat_s(path, PATH_MAX, filename, strlen(filename) + 1);
    if (ret != 0) {
        PRINT_ERR("strncat_s failed, (%s, %d, %s, %d), errno = %d\n",
                  path, PATH_MAX, filename, (int32_t)strlen(filename) + 1, errno);
        return -1;
    }

    fp = fopen(path, "wb");
    if (fp == NULL) {
        PRINT_ERR("can't create file(%s), errno = %d\n", path, errno);
        return -1;
    }

    if (fwrite(convertedBuffer, contextBufLen, 1, fp) != 1) {
        PRINT_ERR("can't write file(%s),errno = %d\n", path, errno);
        (void)fclose(fp);
        return -1;
    }

    (void)fclose(fp);

    return 0;
}

int32_t CheckRpcidFormat(const char *inputFile, char **buffer, uint32_t *len)
{
    uint32_t bufferLen;
    uint16_t sysCaptype, sysCapLength;
    char *contextBuffer = NULL;
    RPCIDHead *rpcidHeader = NULL;

    if (GetFileContext(inputFile, &contextBuffer, &bufferLen)) {
        PRINT_ERR("GetFileContext failed, input file : %s\n", inputFile);
        return -1;
    }
    if (bufferLen < (2 * sizeof(uint32_t))) { // 2, header of rpcid.sc
        PRINT_ERR("Parse file failed(format is invalid), input file : %s\n", inputFile);
        FreeContextBuffer(contextBuffer);
        return -1;
    }
    rpcidHeader = (RPCIDHead *)contextBuffer;
    if (rpcidHeader->apiVersionType != 1) {
        PRINT_ERR("Parse file failed(apiVersionType != 1), input file : %s\n", inputFile);
        FreeContextBuffer(contextBuffer);
        return -1;
    }
    sysCaptype = NtohsInter(*(uint16_t *)(rpcidHeader + 1));
    if (sysCaptype != 2) { // 2, app syscap type
        PRINT_ERR("Parse file failed(sysCaptype != 2), input file : %s\n", inputFile);
        FreeContextBuffer(contextBuffer);
        return -1;
    }
    sysCapLength = NtohsInter(*(uint16_t *)((char *)(rpcidHeader + 1) + sizeof(uint16_t)));
    if (bufferLen < sizeof(RPCIDHead) + sizeof(uint32_t) + sysCapLength) {
        PRINT_ERR("Parse file failed(SysCap length exceeded), input file : %s\n", inputFile);
        FreeContextBuffer(contextBuffer);
        return -1;
    }

    *buffer = contextBuffer;
    *len = bufferLen;
    return 0;
}

cJSON *CreateWholeSyscapJsonObj(void)
{
    cJSON *syscapJsonObj =  cJSON_CreateObject();
    if (syscapJsonObj == NULL) {
        PRINT_ERR("interface-create jsonObj failed.");
        return NULL;
    }

    size_t syscapNums = sizeof(g_arraySyscap) / sizeof(SyscapWithNum);
    for (size_t i = 0; i < syscapNums; i++) {
        cJSON_AddItemToObject(syscapJsonObj, g_arraySyscap[i].str, cJSON_CreateNumber(g_arraySyscap[i].num));
    }
    return syscapJsonObj;
}
