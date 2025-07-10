/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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
#include "provision_verify_test.h"

#include <gtest/gtest.h>
#include "profile_verify.h"

using namespace testing::ext;
namespace OHOS {
namespace SignatureTools {
const std::string INVALID_PROVISION_1 = R"(
{
    "version-name": "1.0.0",
    "issuer": "App Gallery"
})";
const std::string INVALID_PROVISION_2 = R"(
{
    "version-code": 2,
    "version-name": "1.1.0",
    "uuid": "uuid_1_1",
    "type": "release",
    "app-distribution-type": "os_integration",
    "validity": {
        "not-before": 1262275201,
        "not-after": 2524579301
    },
    "bundle-info": {
        "developer-id": "developer_id_1_1",
        "development-certificate": "ABCD",
        "distribution-certificate": "1234",
        "bundle-name": "com.test1_1",
        "apl": "normal",
        "app-feature": "hos_normal_app"
    }
}})";   // brackets mismatching
const std::string INVALID_PROVISION_3 = R"(
{
    "version-code": 2,
    "version-name": "2.0.0",
    "uuid": "uuid_2",
    "type": "release",
    "app-distribution-type": "os_integration",
    "validity": {
        "not-before": 1262275200,
        "not-after": 2524579300
    },
    "bundle-info": {
        "developer-id": "developer_id_2",
        "development-certificate": "ABCDEFG",
        "distribution-certificate": "123456789",
        "bundle-name": "com.test2",
        "app-feature": "hos_normal_app"
    }
})";  // apl not exist
/* validity: 2010 - 2030 */
const std::string VALID_PROVISION = R"(
{
    "version-code": 2,
    "version-name": "2.0.0",
    "uuid": "uuid_2",
    "type": "release",
    "app-distribution-type": "os_integration",
    "validity": {
        "not-before": 1262275200,
        "not-after": 2524579300
    },
    "bundle-info": {
        "developer-id": "developer_id_2",
        "development-certificate": "ABCDEFG",
        "distribution-certificate": "123456789",
        "bundle-name": "com.test2",
        "apl": "normal",
        "app-feature": "hos_normal_app"
    }
})";
const std::string VALID_PROVISION_PERMISSION = R"(
{
    "version-code": 3,
    "version-name": "3.0.0",
    "uuid": "uuid_3",
    "type": "release",
    "app-distribution-type": "app_gallery",
    "validity": {
        "not-before": 1262275300,
        "not-after": 2524579300
    },
    "bundle-info": {
        "developer-id": "developer_id_3",
        "development-certificate": "ABCDEFGH",
        "distribution-certificate": "1234567890",
        "bundle-name": "com.test3",
        "apl": "normal",
        "app-feature": "hos_normal_app"
    },
    "permissions" : {
        "restricted-permissions" : ["ohos.permission.GET_PHONE_STATE",
            "ohos.permission.GET_APP_ACCOUNTS"],
        "restricted-capabilities" : ["ohos.permission.test"]
    },
    "issuer": "App Gallery"
})";
const std::string UNAUTHORIZED_DEVICE_EMPTY_DEVICE_LIST_PROVISION = R"(
{
    "version-code": 6,
    "version-name": "6.0.0",
    "uuid": "uuid_6",
    "type": "debug",
    "validity": {
        "not-before": 1262275600,
        "not-after": 2524579300
    },
    "bundle-info": {
        "developer-id": "developer_id_6",
        "development-certificate": "ABCDEFGHIJ",
        "distribution-certificate": "1234567890123",
        "bundle-name": "com.test6",
        "apl": "normal",
        "app-feature": "hos_normal_app"
    },
    "debug-info": {
        "device-id-type": "udid"
    },
    "issuer": "App Gallery"
})";
const std::string UNSUPPORTED_DEVICE_TYPE_PROVISION = R"(
{
    "version-code": 7,
    "version-name": "7.0.0",
    "uuid": "uuid_7",
    "type": "debug",
    "validity": {
        "not-before": 1262275600,
        "not-after": 2524579300
    },
    "bundle-info": {
        "developer-id": "developer_id_7",
        "development-certificate": "ABCDEFGHIJM",
        "distribution-certificate": "12345678901231",
        "bundle-name": "com.test7",
        "apl": "normal",
        "app-feature": "hos_normal_app"
    },
    "debug-info": {
        "device-id-type": "unknownType",
        "device-ids" : ["1234ABCD"]
    },
    "issuer": "App Gallery"
})";
const std::string DEVICE_UNAUTH_PROVISION = R"(
{
    "version-code": 8,
    "version-name": "8.0.0",
    "uuid": "uuid_8",
    "type": "debug",
    "validity": {
        "not-before": 1262275700,
        "not-after": 2524579300
    },
    "bundle-info": {
        "developer-id": "developer_id_8",
        "development-certificate": "ABCDEFGHIJK",
        "distribution-certificate": "12345678901234",
        "bundle-name": "com.test8",
        "apl": "normal",
        "app-feature": "hos_normal_app"
    },
    "debug-info": {
        "device-id-type": "udid",
        "device-ids" : ["1234ABCD"]
    },
    "issuer": "App Gallery"
})";

class ProvisionVerifyTest : public testing::Test {
public:
    static void SetUpTestCase(void);

    static void TearDownTestCase(void);

    void SetUp();

    void TearDown();
};

void ProvisionVerifyTest::SetUpTestCase(void)
{
}

void ProvisionVerifyTest::TearDownTestCase(void)
{
}

void ProvisionVerifyTest::SetUp()
{
}

void ProvisionVerifyTest::TearDown()
{
}

/**
 * @tc.name: Test HarmonyAppProvision ProvisionVerify001 function
 * @tc.desc: The static function will return verify result;
 * @tc.type: FUNC
 */
HWTEST_F(ProvisionVerifyTest, ProvisionVerify001, TestSize.Level1)
{
    /*
        * @tc.steps: step1. input a invalid HarmonyAppProvision
        *     and run ParseAndVerify function.
        * @tc.expected: step1. return code will be PROVISION_INVALID.
        */
    ProfileInfo info;
    int32_t ret = ParseAndVerify(INVALID_PROVISION_1, info);
    ASSERT_NE(ret, AppProvisionVerifyResult::PROVISION_OK);
    ret = ParseAndVerify(INVALID_PROVISION_2, info);
    ASSERT_NE(ret, AppProvisionVerifyResult::PROVISION_OK);
    ret = ParseAndVerify(INVALID_PROVISION_3, info);
    ASSERT_EQ(ret, AppProvisionVerifyResult::PROVISION_OK);
}

/**
 * @tc.name: Test HarmonyAppProvision ProvisionVerify002 function
 * @tc.desc: The static function will return verify result;
 * @tc.type: FUNC
 */
HWTEST_F(ProvisionVerifyTest, ProvisionVerify002, TestSize.Level1)
{
    /*
        * @tc.steps: step2. input a valid HarmonyAppProvision (without permission info)
        *     and run ParseAndVerify function.
        * @tc.expected: step2. return code will be PROVISION_OK.
        */
    ProfileInfo info;
    int32_t ret = ParseAndVerify(VALID_PROVISION, info);
    ASSERT_EQ(ret, AppProvisionVerifyResult::PROVISION_OK);
    ASSERT_EQ(info.versionCode, 2); // version code is 2
    ASSERT_EQ(info.versionName, "2.0.0");
    ASSERT_EQ(info.uuid, "uuid_2");
    ASSERT_EQ(info.type, ProvisionType::RELEASE);
    ASSERT_TRUE(info.permissions.restrictedPermissions.empty());
    ASSERT_TRUE(info.permissions.restrictedCapabilities.empty());
}

/**
 * @tc.name: Test HarmonyAppProvision ProvisionVerify003 function
 * @tc.desc: The static function will return verify result;
 * @tc.type: FUNC
 */
HWTEST_F(ProvisionVerifyTest, ProvisionVerify003, TestSize.Level1)
{
    /*
        * @tc.steps: step3. input a valid HarmonyAppProvision (with permission info)
        *     and run ParseAndVerify function.
        * @tc.expected: step3. return code will be PROVISION_OK.
        */
    ProfileInfo info;
    int32_t ret = ParseAndVerify(VALID_PROVISION_PERMISSION, info);
    ASSERT_EQ(ret, AppProvisionVerifyResult::PROVISION_OK);
    ASSERT_EQ(info.versionCode, 3); // version code is 3
    ASSERT_EQ(info.versionName, "3.0.0");
    ASSERT_EQ(info.uuid, "uuid_3");
    ASSERT_EQ(info.type, ProvisionType::RELEASE);
    ASSERT_EQ(info.distributionType, AppDistType::APP_GALLERY);
    ASSERT_TRUE(info.permissions.restrictedPermissions.size() == 2);    // 2 restricted permissions
    ASSERT_EQ(info.permissions.restrictedPermissions[0], "ohos.permission.GET_PHONE_STATE");
    ASSERT_TRUE(info.permissions.restrictedCapabilities.size() == 1);   // 1 restricted capabilities
    ASSERT_EQ(info.permissions.restrictedCapabilities[0], "ohos.permission.test");
}
} // namespace SignatureTools
} // namespace OHOS
