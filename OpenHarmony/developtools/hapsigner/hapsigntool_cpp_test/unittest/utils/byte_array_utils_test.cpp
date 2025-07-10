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
#include "byte_array_utils.h"

namespace OHOS {
namespace SignatureTools {
/*
* 测试套件,固定写法
*/
class ByteArrayUtilsTest : public testing::Test {
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
 * @tc.name: InsertIntToByteArray
 * @tc.desc: insert int number to byte array
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ByteArrayUtilsTest, InsertIntToByteArray, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> ret = { 49, 48, 48, 48, 0, 48, 0, 1, 0, -120, 0, 6, 0, 0, 0, 0 };
    int result = ByteArrayUtils::InsertIntToByteArray(ret, 16, 32);
    EXPECT_EQ(result, -1);
}

/**
 * @tc.name: InsertShortToByteArray
 * @tc.desc: insert short number to byte array
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ByteArrayUtilsTest, InsertShortToByteArray, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> ret = { 49, 48, 48, 48, 0, 48, 0, 1, 0, -120, 0, 0, 0, 0, 0, 0 };
    int result = ByteArrayUtils::InsertShortToByteArray(ret, 16, 16, 6);
    EXPECT_EQ(result, -1);
}

/**
 * @tc.name: InsertByteToByteArray
 * @tc.desc: insert byte data to byte array
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ByteArrayUtilsTest, InsertByteToByteArray, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> ret = { 49, 48, 48, 48, 0, 48, 0, 1, 0, -120, 0, 6, 0, 0, 0, 32 };
    std::vector<int8_t> hashValue = { -114, -19, -78, 49, 26, 23, 116, -70, 72, 35,
-41, -43, 43, -115, -2, -93, -91, -67, -76, 77,
100, -83, -69, -36, -18, 59, -12, -64, -118, 123, 48, -99 };
    int result = ByteArrayUtils::InsertByteToByteArray(ret, 16, hashValue, 33);
    EXPECT_EQ(result, -1);
}

/**
 * @tc.name: InsertCharToByteArray
 * @tc.desc: insert charactor to byte array
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ByteArrayUtilsTest, InsertCharToByteArray, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> ret(48, 0);
    int result = ByteArrayUtils::InsertCharToByteArray(ret, 45, "1000");
    EXPECT_EQ(result, -1);
}

} // namespace SignatureTools
} // namespace OHOS