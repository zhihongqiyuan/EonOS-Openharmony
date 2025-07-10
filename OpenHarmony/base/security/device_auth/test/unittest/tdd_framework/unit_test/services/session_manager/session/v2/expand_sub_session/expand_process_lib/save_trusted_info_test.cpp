/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "common_defs.h"
#include "group_data_manager.h"
#include "device_auth.h"
#include "device_auth_defines.h"
#include "dev_info_mock.h"
#include "exception_controller.h"
#include "hc_types.h"
#include "json_utils.h"
#include "memory_mock.h"
#include "memory_monitor.h"
#include "save_trusted_info.h"
#include "uint8buff_utils.h"

using namespace std;
using namespace testing::ext;

namespace {
static const char *AUTH_ID_C = "5420459D93FE773F9945FD64277FBA2CAB8FB996DDC1D0B97676FBB1242B3930";
static const char *AUTH_ID_S = "52E2706717D5C39D736E134CC1E3BE1BAA2AA52DB7C76A37C749558BD2E6492C";
static const char *GROUP_ID = "E2EE6F830B176B2C96A9F99BFAE2A61F5D1490B9F4A090E9D8C2874C230C7C21";
static const char *USER_ID_C = "4269DC28B639681698809A67EDAD08E39F207900038F91FEF95DD042FE2874E4";
static const char *USER_ID_S = "9F207900038F91FEF95DD042FE2874E44269DC28B639681698809A67EDAD08E3";
static const char *GROUP_NAME = "testGroup";
static const char *GROUP_OWNER = "testApp";

#define TEST_OS_ACCOUNT_ID1 0
#define TEST_OS_ACCOUNT_ID2 100

static SaveTrustedInfoParams g_paramsC = { TEST_OS_ACCOUNT_ID1, ASYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY,
    GROUP_VISIBILITY_PUBLIC, GROUP_OWNER, GROUP_ID, AUTH_ID_C, false };
static SaveTrustedInfoParams g_paramsS = { TEST_OS_ACCOUNT_ID2, ASYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY,
    GROUP_VISIBILITY_PUBLIC, GROUP_OWNER, GROUP_ID, AUTH_ID_S, false };

static const char *g_invalidMsg1 = "{\"errCode\": 1}";
static const char *g_invalidMsg2 = "{\"event\": -1, \"errCode\": 1}";
static const char *g_invalidMsg3 = "{\"event\": 10, \"errCode\": 1}";
static const char *g_errorMsg1 = "{\"event\": 3, \"errCode\": 1}";
static const char *g_errorMsg2 = "{\"event\": 3}";

static const int TEST_DEV_AUTH_BUFFER_SIZE = 128;

typedef struct {
    int32_t osAccountId;
    int32_t credType;
    int32_t userTypeSelf;
    int32_t visibility;
    const char *groupId;
    const char *groupName;
    const char *appId;
    const char *authIdSelf;
    const char *udidSelf;
    const char *userIdSelf;
} CreateParams;

static void RemoveDir(const char *path)
{
    char strBuf[TEST_DEV_AUTH_BUFFER_SIZE] = {0};
    if (path == nullptr) {
        return;
    }
    if (sprintf_s(strBuf, sizeof(strBuf) - 1, "rm -rf %s", path) < 0) {
        return;
    }
    system(strBuf);
    return;
}

static void DeleteDatabase()
{
    const char *groupPath = GetStorageDirPath();
    RemoveDir(groupPath);
    return;
}

static int32_t GenerateGroupParams(const CreateParams *params, TrustedGroupEntry *groupParams)
{
    if (!StringSetPointer(&groupParams->name, params->groupName)) {
        return HC_ERR_MEMORY_COPY;
    }
    if (!StringSetPointer(&groupParams->id, params->groupId)) {
        return HC_ERR_MEMORY_COPY;
    }
    HcString ownerName = CreateString();
    if (!StringSetPointer(&ownerName, params->appId)) {
        DeleteString(&ownerName);
        return HC_ERR_MEMORY_COPY;
    }
    if (groupParams->managers.pushBackT(&groupParams->managers, ownerName) == NULL) {
        DeleteString(&ownerName);
        return HC_ERR_MEMORY_COPY;
    }
    groupParams->visibility = params->visibility;
    groupParams->type = PEER_TO_PEER_GROUP;
    groupParams->expireTime = DEFAULT_EXPIRE_TIME;
    return HC_SUCCESS;
}

static int32_t GenerateSelfDevParams(const CreateParams *params, TrustedDeviceEntry *devParams)
{
    if (!StringSetPointer(&devParams->udid, params->udidSelf)) {
        return HC_ERR_MEMORY_COPY;
    }
    if (!StringSetPointer(&devParams->authId, params->authIdSelf)) {
        return HC_ERR_MEMORY_COPY;
    }
    if (!StringSetPointer(&devParams->groupId, params->groupId)) {
        return HC_ERR_MEMORY_COPY;
    }
    if (!StringSetPointer(&devParams->serviceType, params->groupId)) {
        return HC_ERR_MEMORY_COPY;
    }
    devParams->devType = params->userTypeSelf;
    devParams->source = SELF_CREATED;
    devParams->credential = params->credType;
    return HC_SUCCESS;
}

static int32_t AddTrustedGroup(const CreateParams *params)
{
    TrustedGroupEntry *groupParams = CreateGroupEntry();
    if (groupParams == NULL) {
        return HC_ERR_ALLOC_MEMORY;
    }
    int32_t res = GenerateGroupParams(params, groupParams);
    if (res != HC_SUCCESS) {
        DestroyGroupEntry(groupParams);
        return res;
    }
    res = AddGroup(params->osAccountId, groupParams);
    DestroyGroupEntry(groupParams);
    return res;
}

static int32_t AddSelfTrustedDevice(const CreateParams *params)
{
    TrustedDeviceEntry *devParams = CreateDeviceEntry();
    if (devParams == NULL) {
        return HC_ERR_ALLOC_MEMORY;
    }
    int32_t res = GenerateSelfDevParams(params, devParams);
    if (res != HC_SUCCESS) {
        DestroyDeviceEntry(devParams);
        return res;
    }
    res = AddTrustedDevice(params->osAccountId, devParams);
    DestroyDeviceEntry(devParams);
    return res;
}

static int32_t CreatePeerToPeerGroup(const CreateParams *params)
{
    int32_t res = AddTrustedGroup(params);
    if (res != HC_SUCCESS) {
        return res;
    }
    return AddSelfTrustedDevice(params);
}

class SaveTrustedInfoTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void SaveTrustedInfoTest::SetUpTestCase() {}

void SaveTrustedInfoTest::TearDownTestCase() {}

void SaveTrustedInfoTest::SetUp()
{
    InitExceptionController();
    HcInitMallocMonitor();
    cJSON_Hooks hooks = {
        .malloc_fn = MockMallocForJson,
        .free_fn = MockFree
    };
    cJSON_InitHooks(&hooks);
}

void SaveTrustedInfoTest::TearDown()
{
    bool isMemoryLeak = IsMemoryLeak();
    EXPECT_FALSE(isMemoryLeak);
    if (isMemoryLeak) {
        ReportMonitor();
    }
    cJSON_Hooks hooks = {
        .malloc_fn = malloc,
        .free_fn = free
    };
    cJSON_InitHooks(&hooks);
    HcDestroyMallocMonitor();
    DestroyExceptionController();
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest001, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);
    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest002, TestSize.Level0)
{
    DeleteDatabase();
    int32_t res = InitDatabase();
    ASSERT_EQ(res, HC_SUCCESS);
    CreateParams params = { TEST_OS_ACCOUNT_ID1, SYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY, GROUP_VISIBILITY_PUBLIC,
        GROUP_ID, GROUP_NAME, GROUP_OWNER, AUTH_ID_C, AUTH_ID_C, USER_ID_C };
    res = CreatePeerToPeerGroup(&params);
    ASSERT_EQ(res, HC_SUCCESS);

    BaseCmd *client = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(client, nullptr);

    BaseCmd *server = CreateSaveTrustedInfoCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(server, nullptr);

    CJson *clientOut = nullptr;
    CJson *serverOut = nullptr;

    SetDeviceStatus(true);
    res = client->start(client, &clientOut);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_NE(clientOut, nullptr);

    CmdState clientState;
    CmdState serverState;
    SetDeviceStatus(false);
    res = server->process(server, clientOut, &serverOut, &serverState);
    FreeJson(clientOut);
    clientOut = nullptr;
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_NE(serverOut, nullptr);
    ASSERT_EQ(serverState, CMD_STATE_FINISH);

    SetDeviceStatus(true);
    res = client->process(client, serverOut, &clientOut, &clientState);
    FreeJson(serverOut);
    serverOut = nullptr;
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_EQ(clientOut, nullptr);
    ASSERT_EQ(clientState, CMD_STATE_FINISH);

    client->destroy(client);
    server->destroy(server);

    DestroyDatabase();
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest003, TestSize.Level0)
{
    DeleteDatabase();
    int32_t res = InitDatabase();
    ASSERT_EQ(res, HC_SUCCESS);
    CreateParams params = { TEST_OS_ACCOUNT_ID2, SYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY, GROUP_VISIBILITY_PUBLIC,
        GROUP_ID, GROUP_NAME, GROUP_OWNER, AUTH_ID_S, AUTH_ID_S, USER_ID_S };
    res = CreatePeerToPeerGroup(&params);
    ASSERT_EQ(res, HC_SUCCESS);

    BaseCmd *client = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(client, nullptr);

    BaseCmd *server = CreateSaveTrustedInfoCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(server, nullptr);

    CJson *clientOut = nullptr;
    CJson *serverOut = nullptr;

    SetDeviceStatus(true);
    res = client->start(client, &clientOut);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_NE(clientOut, nullptr);

    CmdState clientState;
    CmdState serverState;
    SetDeviceStatus(false);
    res = server->process(server, clientOut, &serverOut, &serverState);
    FreeJson(clientOut);
    clientOut = nullptr;
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_NE(serverOut, nullptr);
    ASSERT_EQ(serverState, CMD_STATE_FINISH);

    SetDeviceStatus(true);
    res = client->process(client, serverOut, &clientOut, &clientState);
    FreeJson(serverOut);
    serverOut = nullptr;
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_EQ(clientOut, nullptr);
    ASSERT_EQ(clientState, CMD_STATE_FINISH);

    client->destroy(client);
    server->destroy(server);

    DestroyDatabase();
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest101, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd(nullptr, true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest102, TestSize.Level0)
{
    SaveTrustedInfoParams errorParams = { 0, ASYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY, GROUP_VISIBILITY_PUBLIC,
        nullptr, GROUP_ID, AUTH_ID_C, false };
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)(&errorParams), true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest103, TestSize.Level0)
{
    SaveTrustedInfoParams errorParams = { 0, ASYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY, GROUP_VISIBILITY_PUBLIC,
        GROUP_OWNER, nullptr, AUTH_ID_C, false };
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)(&errorParams), true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest104, TestSize.Level0)
{
    SaveTrustedInfoParams errorParams = { 0, ASYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY, GROUP_VISIBILITY_PUBLIC,
        GROUP_OWNER, GROUP_ID, nullptr, false };
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)(&errorParams), true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest106, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);
    self->destroy(nullptr);
    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest107, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *out = nullptr;
    int32_t res = self->start(nullptr, &out);
    ASSERT_NE(res, HC_SUCCESS);
    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest108, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    int32_t res = self->start(self, nullptr);
    ASSERT_NE(res, HC_SUCCESS);
    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest109, TestSize.Level0)
{
    DeleteDatabase();
    int32_t res = InitDatabase();
    ASSERT_EQ(res, HC_SUCCESS);
    CreateParams params = { TEST_OS_ACCOUNT_ID2, SYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY, GROUP_VISIBILITY_PUBLIC,
        GROUP_ID, GROUP_NAME, GROUP_OWNER, AUTH_ID_S, AUTH_ID_S, USER_ID_S };
    res = CreatePeerToPeerGroup(&params);
    ASSERT_EQ(res, HC_SUCCESS);

    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *out = nullptr;
    res = self->start(self, &out);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_NE(out, nullptr);
    FreeJson(out);
    out = nullptr;

    res = self->start(self, &out);
    ASSERT_NE(res, HC_SUCCESS);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);

    DestroyDatabase();
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest110, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson in;
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(nullptr, &in, &out, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest111, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, nullptr, &out, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest112, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson in;
    CmdState state;
    int32_t res = self->process(self, &in, nullptr, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest113, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson in;
    CJson *out = nullptr;
    int32_t res = self->process(self, &in, &out, nullptr);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest114, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    self->curState = self->finishState;

    CJson in;
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, &in, &out, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest115, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    self->curState = self->failState;

    CJson in;
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, &in, &out, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest116, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *failEvent = CreateJsonFromString(g_errorMsg1);
    ASSERT_NE(failEvent, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, failEvent, &out, &state);
    FreeJson(failEvent);
    ASSERT_EQ(res, HC_ERROR);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest117, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *failEvent = CreateJsonFromString(g_errorMsg2);
    ASSERT_NE(failEvent, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, failEvent, &out, &state);
    FreeJson(failEvent);
    ASSERT_EQ(res, HC_ERR_PEER_ERROR);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest118, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *invalidMsg = CreateJsonFromString(g_invalidMsg1);
    ASSERT_NE(invalidMsg, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, invalidMsg, &out, &state);
    FreeJson(invalidMsg);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest119, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *invalidMsg = CreateJsonFromString(g_invalidMsg2);
    ASSERT_NE(invalidMsg, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, invalidMsg, &out, &state);
    FreeJson(invalidMsg);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest120, TestSize.Level0)
{
    BaseCmd *self = CreateSaveTrustedInfoCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *invalidMsg = CreateJsonFromString(g_invalidMsg3);
    ASSERT_NE(invalidMsg, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, invalidMsg, &out, &state);
    FreeJson(invalidMsg);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

static int32_t TestMemoryInner(BaseCmd **clientPtr, BaseCmd **serverPtr)
{
    BaseCmd *client = CreateSaveTrustedInfoCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    if (client == nullptr) {
        return HC_ERR_NULL_PTR;
    }
    *clientPtr = client;
    BaseCmd *server = CreateSaveTrustedInfoCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    if (server == nullptr) {
        return HC_ERR_NULL_PTR;
    }
    *serverPtr = server;
    CmdState clientState;
    CmdState serverState;
    CJson *clientOut = nullptr;
    CJson *serverOut = nullptr;
    SetDeviceStatus(true);
    int32_t res = client->start(client, &clientOut);
    if (res != HC_SUCCESS) {
        FreeJson(clientOut);
        clientOut = nullptr;
        return res;
    }
    SetDeviceStatus(false);
    res = server->process(server, clientOut, &serverOut, &serverState);
    FreeJson(clientOut);
    clientOut = nullptr;
    if (res != HC_SUCCESS) {
        FreeJson(serverOut);
        serverOut = nullptr;
        return res;
    }
    SetDeviceStatus(true);
    res = client->process(client, serverOut, &clientOut, &clientState);
    FreeJson(serverOut);
    serverOut = nullptr;
    if (clientOut != nullptr) {
        FreeJson(clientOut);
        clientOut = nullptr;
    }
    return res;
}

static int32_t TestMemoryException(void)
{
    SetControllerMode(false);
    DeleteDatabase();
    int32_t res = InitDatabase();
    if (res != HC_SUCCESS) {
        return res;
    }
    CreateParams params = { TEST_OS_ACCOUNT_ID1, SYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY, GROUP_VISIBILITY_PUBLIC,
        GROUP_ID, GROUP_NAME, GROUP_OWNER, AUTH_ID_C, AUTH_ID_C, USER_ID_C };
    res = CreatePeerToPeerGroup(&params);
    if (res != HC_SUCCESS) {
        DestroyDatabase();
        return res;
    }
    SetControllerMode(true);
    BaseCmd *client = nullptr;
    BaseCmd *server = nullptr;
    res = TestMemoryInner(&client, &server);
    if (client != nullptr) {
        client->destroy(client);
    }
    if (server != nullptr) {
        server->destroy(server);
    }
    DestroyDatabase();
    return res;
}

static int32_t TestMemoryException2(void)
{
    SetControllerMode(false);
    DeleteDatabase();
    int32_t res = InitDatabase();
    if (res != HC_SUCCESS) {
        return res;
    }
    CreateParams params = { TEST_OS_ACCOUNT_ID2, SYMMETRIC_CRED, DEVICE_TYPE_ACCESSORY, GROUP_VISIBILITY_PUBLIC,
        GROUP_ID, GROUP_NAME, GROUP_OWNER, AUTH_ID_S, AUTH_ID_S, USER_ID_S };
    res = CreatePeerToPeerGroup(&params);
    if (res != HC_SUCCESS) {
        DestroyDatabase();
        return res;
    }
    SetControllerMode(true);
    BaseCmd *client = nullptr;
    BaseCmd *server = nullptr;
    res = TestMemoryInner(&client, &server);
    if (client != nullptr) {
        client->destroy(client);
    }
    if (server != nullptr) {
        server->destroy(server);
    }
    DestroyDatabase();
    return res;
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest126, TestSize.Level0)
{
    int32_t res = TestMemoryException();
    ASSERT_EQ(res, HC_SUCCESS);
    uint32_t callNum = GetCallNum();
    for (uint32_t i = 0; i < callNum; i++) {
        SetThrowExceptionIndex(i);
        (void)TestMemoryException();
        bool isMemoryLeak = IsMemoryLeak();
        EXPECT_FALSE(isMemoryLeak);
        if (isMemoryLeak) {
            ReportMonitor();
            break;
        }
    }
}

HWTEST_F(SaveTrustedInfoTest, SaveTrustedInfoTest127, TestSize.Level0)
{
    int32_t res = TestMemoryException2();
    ASSERT_EQ(res, HC_SUCCESS);
    uint32_t callNum = GetCallNum();
    for (uint32_t i = 0; i < callNum; i++) {
        SetThrowExceptionIndex(i);
        (void)TestMemoryException2();
        bool isMemoryLeak = IsMemoryLeak();
        EXPECT_FALSE(isMemoryLeak);
        if (isMemoryLeak) {
            ReportMonitor();
            break;
        }
    }
}
}
