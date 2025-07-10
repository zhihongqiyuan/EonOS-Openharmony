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
#include "sign_head.h"

namespace OHOS {
namespace SignatureTools {

/*
* 测试套件,固定写法
*/
class SignHeadTest : public testing::Test {
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
 * @tc.name: GetSignHead
 * @tc.desc: Test get sign head for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignHeadTest, GetSignHead, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignHead> api = std::make_shared<SignHead>();
    std::vector<int8_t> signHead = api->GetSignHead(5254);
    EXPECT_EQ(signHead.size(), 32);
}

/**
 * @tc.name: getSignHeadLittleEndian
 * @tc.desc: Test get sign head by little endian for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignHeadTest, getSignHeadLittleEndian, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignHead> api = std::make_shared<SignHead>();
    std::vector<int8_t> signHeadLittleEndian = api->GetSignHeadLittleEndian(1024, 1);
    EXPECT_EQ(signHeadLittleEndian.size(), 32);
}
} // namespace SignatureTools
} // namespace OHOS