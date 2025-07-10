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

#include <memory>
#include <gtest/gtest.h>
#include "fs_digest_utils.h"

namespace OHOS {
namespace SignatureTools {

class DigestUtilsTest : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
    };
    static void TearDownTestCase()
    {
    };
    void SetUp()
    {
    };
    void TearDown()
    {
    };
};

/**
 * @tc.name: addData001
 * @tc.desc: add digest data without length
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(DigestUtilsTest, addData001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<DigestUtils> api = std::make_shared<DigestUtils>(HASH_SHA256);

    char charData[32] = { -66, -72, 8, -21, 1, 28, 23, 2, -1, -1, -1, -1, 15, 16, 40, -57, -34,
          -119, 1, 6, -76, -72, 8, -66, -72, 8, -66, -72, 8, -86, -50, 8 };
    std::string data(charData);
    api->AddData(data);

    DigestUtils::Type type = DigestUtils::Type::HEX;
    std::string str = api->Result(type);

    EXPECT_NE(str.size(), 0);
}

/**
 * @tc.name: addData002
 * @tc.desc: add digest data with length
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(DigestUtilsTest, addData002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<DigestUtils> api = std::make_shared<DigestUtils>(HASH_SHA256);

    char pData[32] = { -66, -72, 8, -21, 1, 28, 23, 2, -1, -1, -1, -1, 15, 16, 40, -57, -34,
                       -119, 1, 6, -76, -72, 8, -66, -72, 8, -66, -72, 8, -86, -50, 8 };
    int length = 32;
    api->AddData(pData, length);

    DigestUtils::Type type = DigestUtils::Type::HEX;
    std::string str = api->Result(type);

    EXPECT_NE(str.size(), 0);
}

/**
 * @tc.name: result
 * @tc.desc: get digest result and size
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(DigestUtilsTest, result, testing::ext::TestSize.Level1)
{
    std::shared_ptr<DigestUtils> api = std::make_shared<DigestUtils>(HASH_SHA256);

    DigestUtils::Type type = DigestUtils::Type::HEX;
    std::string str = api->Result(type);

    EXPECT_EQ(str.size(), 64);
}
} // namespace SignatureTools
} // namespace OHOS