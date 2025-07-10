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

#include "signature_block_types.h"

namespace OHOS {
namespace SignatureTools {

class SignatureBlockTypesTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: GetProfileBlockTypes001
 * @tc.desc: get profile block types of 0
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignatureBlockTypesTest, GetProfileBlockTypes001, testing::ext::TestSize.Level1)
{
    char type = SignatureBlockTypes::GetProfileBlockTypes("0");
    bool flag = type == SignatureBlockTypes::PROFILE_NOSIGNED_BLOCK;

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: GetProfileBlockTypes002
 * @tc.desc: get profile block types of 1
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignatureBlockTypesTest, GetProfileBlockTypes002, testing::ext::TestSize.Level1)
{
    char type = SignatureBlockTypes::GetProfileBlockTypes("1");
    bool flag = type == SignatureBlockTypes::PROFILE_SIGNED_BLOCK;

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: GetProfileBlockTypes003
 * @tc.desc: get profile block types of 2
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignatureBlockTypesTest, GetProfileBlockTypes003, testing::ext::TestSize.Level1)
{
    char type = SignatureBlockTypes::GetProfileBlockTypes("2");
    bool flag = type == SignatureBlockTypes::PROFILE_NOSIGNED_BLOCK;

    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: GetProfileBlockTypes004
 * @tc.desc: get profile block types of null
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignatureBlockTypesTest, GetProfileBlockTypes004, testing::ext::TestSize.Level1)
{
    char type = SignatureBlockTypes::GetProfileBlockTypes("");
    bool flag = type == SignatureBlockTypes::PROFILE_NOSIGNED_BLOCK;

    EXPECT_EQ(flag, true);
}
} // namespace SignatureTools
} // namespace OHOS