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

#include "sign_block_info.h"

namespace OHOS {
namespace SignatureTools {

class SignBlockInfoTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: GetSignBlockMap
 * @tc.desc: Test function of SignBlockInfoTest::GetSignBlockMap() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignBlockInfoTest, GetSignBlockMap, testing::ext::TestSize.Level1)
{
    SignBlockInfo signBlockInfo(false);
    std::unordered_map<int8_t, SigningBlock> signBlockMap = signBlockInfo.GetSignBlockMap();
    int size = signBlockMap.size();

    EXPECT_EQ(size, 0);
}

/**
 * @tc.name: GetFileDigest
 * @tc.desc: Test function of SignBlockInfoTest::GetFileDigest() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignBlockInfoTest, GetFileDigest, testing::ext::TestSize.Level1)
{
    SignBlockInfo signBlockInfo(true);
    std::vector<int8_t> fileDigest = signBlockInfo.GetFileDigest();
    int size = fileDigest.size();

    EXPECT_EQ(size, 0);
}

/**
 * @tc.name: SetFileDigest
 * @tc.desc: Test function of SignBlockInfoTest::SetFileDigest() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignBlockInfoTest, SetFileDigest, testing::ext::TestSize.Level1)
{
    SignBlockInfo signBlockInfo(true);
    std::vector<int8_t> fileDigest;
    signBlockInfo.SetFileDigest(fileDigest);
    int size = fileDigest.size();

    EXPECT_EQ(size, 0);
}

/**
 * @tc.name: GetRawDigest
 * @tc.desc: Test function of SignBlockInfoTest::GetRawDigest() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignBlockInfoTest, GetRawDigest, testing::ext::TestSize.Level1)
{
    SignBlockInfo signBlockInfo(true);
    std::vector<int8_t> digest = signBlockInfo.GetRawDigest();
    int size = digest.size();

    EXPECT_EQ(size, 0);
}

/**
 * @tc.name: SetRawDigest
 * @tc.desc: Test function of SignBlockInfoTest::SetRawDigest() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignBlockInfoTest, SetRawDigest, testing::ext::TestSize.Level1)
{
    SignBlockInfo signBlockInfo(true);
    std::vector<int8_t> digest;
    signBlockInfo.SetRawDigest(digest);
    int size = digest.size();

    EXPECT_EQ(size, 0);
}

/**
 * @tc.name: GetNeedGenerateDigest
 * @tc.desc: Test function of SignBlockInfoTest::GetNeedGenerateDigest() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignBlockInfoTest, GetNeedGenerateDigest, testing::ext::TestSize.Level1)
{
    SignBlockInfo signBlockInfo(true);
    bool needGenerateDigest = signBlockInfo.GetNeedGenerateDigest();

    EXPECT_EQ(needGenerateDigest, true);
}
} // namespace SignatureTools
} // namespace OHOS