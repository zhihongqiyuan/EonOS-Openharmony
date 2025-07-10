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
#include "auth_code_import.h"
#include "device_auth_defines.h"
#include "exception_controller.h"
#include "hc_types.h"
#include "hks_api.h"
#include "hks_param.h"
#include "hks_type.h"
#include "json_utils.h"
#include "memory_mock.h"
#include "memory_monitor.h"
#include "uint8buff_utils.h"
#include "device_auth.h"

using namespace std;
using namespace testing::ext;

namespace {
static const char *AUTH_ID_C_VAL = "5420459D93FE773F9945FD64277FBA2CAB8FB996DDC1D0B97676FBB1242B3930";
static const char *AUTH_ID_S_VAL = "52E2706717D5C39D736E134CC1E3BE1BAA2AA52DB7C76A37C749558BD2E6492C";
static const char *GROUP_ID = "E2EE6F830B176B2C96A9F99BFAE2A61F5D1490B9F4A090E9D8C2874C230C7C21";
static const char *GROUP_OWNER = "testApp";

static Uint8Buff g_authIdC = { (uint8_t *)AUTH_ID_C_VAL, 64 };
static Uint8Buff g_authIdS = { (uint8_t *)AUTH_ID_S_VAL, 64 };
static AuthCodeImportParams g_paramsC = { 0, GROUP_OWNER, GROUP_ID, g_authIdC, DEFAULT_OS_ACCOUNT };
static AuthCodeImportParams g_paramsS = { 0, GROUP_OWNER, GROUP_ID, g_authIdS, DEFAULT_OS_ACCOUNT };

static const char *INVALID_MSG1 = "{\"errCode\": 1}";
static const char *INVALID_MSG2 = "{\"event\": -1, \"errCode\": 1}";
static const char *INVALID_MSG3 = "{\"event\": 10, \"errCode\": 1}";
static const char *ERROR_MSG1 = "{\"event\": 3, \"errCode\": 1}";
static const char *ERROR_MSG2 = "{\"event\": 3}";

class AuthCodeImportTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void AuthCodeImportTest::SetUpTestCase()
{
    HksInitialize();
}

void AuthCodeImportTest::TearDownTestCase() {}

void AuthCodeImportTest::SetUp()
{
    InitExceptionController();
    HcInitMallocMonitor();
    cJSON_Hooks hooks = {
        .malloc_fn = MockMallocForJson,
        .free_fn = MockFree
    };
    cJSON_InitHooks(&hooks);
}

void AuthCodeImportTest::TearDown()
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

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest001, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);
    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest002, TestSize.Level0)
{
    BaseCmd *client = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(client, nullptr);

    BaseCmd *server = CreateAuthCodeImportCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(server, nullptr);

    CJson *clientOut = nullptr;
    CJson *serverOut = nullptr;

    int32_t res = client->start(client, &clientOut);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_NE(clientOut, nullptr);

    CmdState clientState;
    CmdState serverState;
    res = server->process(server, clientOut, &serverOut, &serverState);
    FreeJson(clientOut);
    clientOut = nullptr;
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_NE(serverOut, nullptr);
    ASSERT_EQ(serverState, CMD_STATE_FINISH);

    res = client->process(client, serverOut, &clientOut, &clientState);
    FreeJson(serverOut);
    serverOut = nullptr;
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_EQ(clientOut, nullptr);
    ASSERT_EQ(clientState, CMD_STATE_FINISH);

    client->destroy(client);
    server->destroy(server);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest101, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd(nullptr, true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest102, TestSize.Level0)
{
    AuthCodeImportParams errorParams = { 0, nullptr, GROUP_ID, g_authIdC, DEFAULT_OS_ACCOUNT };
    BaseCmd *self = CreateAuthCodeImportCmd((void *)(&errorParams), true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest103, TestSize.Level0)
{
    AuthCodeImportParams errorParams = { 0, GROUP_OWNER, nullptr, g_authIdC, DEFAULT_OS_ACCOUNT };
    BaseCmd *self = CreateAuthCodeImportCmd((void *)(&errorParams), true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest104, TestSize.Level0)
{
    AuthCodeImportParams errorParams = { 0, GROUP_OWNER, GROUP_ID, { nullptr, 0 }, DEFAULT_OS_ACCOUNT };
    BaseCmd *self = CreateAuthCodeImportCmd((void *)(&errorParams), true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest105, TestSize.Level0)
{
    AuthCodeImportParams errorParams =
        { 0, GROUP_OWNER, GROUP_ID, { (uint8_t *)AUTH_ID_C_VAL, 0 }, DEFAULT_OS_ACCOUNT };
    BaseCmd *self = CreateAuthCodeImportCmd((void *)(&errorParams), true, ABORT_IF_ERROR);
    ASSERT_EQ(self, nullptr);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest106, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);
    self->destroy(nullptr);
    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest107, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *out = nullptr;
    int32_t res = self->start(nullptr, &out);
    ASSERT_NE(res, HC_SUCCESS);
    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest108, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    int32_t res = self->start(self, nullptr);
    ASSERT_NE(res, HC_SUCCESS);
    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest109, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *out = nullptr;
    int32_t res = self->start(self, &out);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_NE(out, nullptr);
    FreeJson(out);
    out = nullptr;

    res = self->start(self, &out);
    ASSERT_NE(res, HC_SUCCESS);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest110, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson in;
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(nullptr, &in, &out, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest111, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, nullptr, &out, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest112, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson in;
    CmdState state;
    int32_t res = self->process(self, &in, nullptr, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest113, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson in;
    CJson *out = nullptr;
    int32_t res = self->process(self, &in, &out, nullptr);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest114, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    self->curState = self->finishState;

    CJson in;
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, &in, &out, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest115, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    self->curState = self->failState;

    CJson in;
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, &in, &out, &state);
    ASSERT_NE(res, HC_SUCCESS);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest116, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *failEvent = CreateJsonFromString(ERROR_MSG1);
    ASSERT_NE(failEvent, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, failEvent, &out, &state);
    FreeJson(failEvent);
    ASSERT_EQ(res, HC_ERROR);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest117, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *failEvent = CreateJsonFromString(ERROR_MSG2);
    ASSERT_NE(failEvent, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, failEvent, &out, &state);
    FreeJson(failEvent);
    ASSERT_EQ(res, HC_ERR_PEER_ERROR);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest118, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *invalidMsg = CreateJsonFromString(INVALID_MSG1);
    ASSERT_NE(invalidMsg, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, invalidMsg, &out, &state);
    FreeJson(invalidMsg);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest119, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *invalidMsg = CreateJsonFromString(INVALID_MSG2);
    ASSERT_NE(invalidMsg, nullptr);
    CJson *out = nullptr;
    CmdState state;
    int32_t res = self->process(self, invalidMsg, &out, &state);
    FreeJson(invalidMsg);
    ASSERT_EQ(res, HC_SUCCESS);
    ASSERT_EQ(out, nullptr);

    self->destroy(self);
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest120, TestSize.Level0)
{
    BaseCmd *self = CreateAuthCodeImportCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    ASSERT_NE(self, nullptr);

    CJson *invalidMsg = CreateJsonFromString(INVALID_MSG3);
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
    BaseCmd *client = CreateAuthCodeImportCmd((void *)&g_paramsC, true, ABORT_IF_ERROR);
    if (client == nullptr) {
        return HC_ERR_NULL_PTR;
    }
    *clientPtr = client;
    BaseCmd *server = CreateAuthCodeImportCmd((void *)&g_paramsS, false, ABORT_IF_ERROR);
    if (server == nullptr) {
        return HC_ERR_NULL_PTR;
    }
    *serverPtr = server;
    CmdState clientState;
    CmdState serverState;
    CJson *clientOut = nullptr;
    CJson *serverOut = nullptr;
    int32_t res = client->start(client, &clientOut);
    if (res != HC_SUCCESS) {
        FreeJson(clientOut);
        clientOut = nullptr;
        return res;
    }
    res = server->process(server, clientOut, &serverOut, &serverState);
    FreeJson(clientOut);
    clientOut = nullptr;
    if (res != HC_SUCCESS) {
        FreeJson(serverOut);
        serverOut = nullptr;
        return res;
    }
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
    BaseCmd *client = nullptr;
    BaseCmd *server = nullptr;
    int32_t res = TestMemoryInner(&client, &server);
    if (client != nullptr) {
        client->destroy(client);
    }
    if (server != nullptr) {
        server->destroy(server);
    }
    return res;
}

HWTEST_F(AuthCodeImportTest, AuthCodeImportTest126, TestSize.Level0)
{
    SetControllerMode(true);
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
}
