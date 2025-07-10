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
#include <fstream>
#include <gtest/gtest.h>

#include "signing_block.h"

namespace OHOS {
namespace SignatureTools {

class SigningBlockTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: GetLength
 * @tc.desc: Test function of SigningBlockTest::GetLength() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SigningBlockTest, GetLength, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> value;
    SigningBlock signingBlock(0, value, 0);
    int32_t length = signingBlock.GetLength();

    EXPECT_EQ(length, 0);
}

/**
 * @tc.name: GetValue
 * @tc.desc: Test function of SigningBlockTest::GetValue() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SigningBlockTest, GetValue, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> value;
    SigningBlock signingBlock(0, value, 0);
    std::vector<int8_t> val = signingBlock.GetValue();
    int size = val.size();

    EXPECT_EQ(size, 0);
}

/**
 * @tc.name: GetOffset
 * @tc.desc: Test function of SigningBlockTest::GetOffset() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SigningBlockTest, GetOffset, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> value;
    SigningBlock signingBlock(0, value, 0);
    int32_t offset = signingBlock.GetOffset();

    EXPECT_EQ(offset, 0);
}
} // namespace SignatureTools
} // namespace OHOS