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
#include "block_head.h"

namespace OHOS {
namespace SignatureTools {

/*
* 测试套件,固定写法
*/
class BlockHeadTest : public testing::Test {
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
 * @tc.name: GetBlockHead
 * @tc.desc: Test get block head for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BlockHeadTest, GetBlockHead, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BlockHead> api = std::make_shared<BlockHead>();
    std::string blockHead = api->GetBlockHead(0, 0, 0, 29148868);
    EXPECT_NE(blockHead.size(), 0);
}

/**
 * @tc.name: GetBlockHeadLittleEndian
 * @tc.desc: Test get block head by little endian for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BlockHeadTest, GetBlockHeadLittleEndian, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BlockHead> api = std::make_shared<BlockHead>();
    std::vector<int8_t> head = api->GetBlockHeadLittleEndian(0, 0, 0, 29148868);
    EXPECT_NE(head.size(), 0);
}

/**
 * @tc.name: GetBlockLen
 * @tc.desc: Test get block length for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BlockHeadTest, GetBlockLen, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BlockHead> api = std::make_shared<BlockHead>();
    int blockLen = api->GetBlockLen();
    EXPECT_EQ(blockLen, 8);
}

/**
 * @tc.name: GetElfBlockLen
 * @tc.desc: Test get elf block length for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(BlockHeadTest, GetElfBlockLen, testing::ext::TestSize.Level1)
{
    std::shared_ptr<BlockHead> api = std::make_shared<BlockHead>();
    int elfBlockLen = api->GetElfBlockLen();
    EXPECT_EQ(elfBlockLen, 12);
}
} // namespace SignatureTools
} // namespace OHOS