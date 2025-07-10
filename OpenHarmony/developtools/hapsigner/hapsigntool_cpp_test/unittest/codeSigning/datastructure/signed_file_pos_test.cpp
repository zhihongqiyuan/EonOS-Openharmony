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
#include "signed_file_pos.h"

namespace OHOS {
namespace SignatureTools {

/*
 * 测试套件,固定写法
 */
class SignedFilePosTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: fromByteArray
 * @tc.desc: Test function of SignedFilePosTest::FromByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignedFilePosTest, fromByteArray, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignedFilePos> api = std::make_shared<SignedFilePos>(108, 31, 280, 2068);

    std::vector<int8_t> bytes = { 11, -93, 88, 107, -121, 96, 121, 23, -64, -58, -95, -71,
        -126, 60, 116, 60, 10, 15, -125, 107, 127, -123, 81, 68, 28, -121, -20, -42, -116,
        -81, -6, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SignedFilePos pos = api->FromByteArray(bytes);

    EXPECT_NE(pos.GetFileNameOffset(), 0);
}

/**
 * @tc.name: getFileNameOffset
 * @tc.desc: Test function of SignedFilePosTest::GetFileNameOffset() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignedFilePosTest, getFileNameOffset, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignedFilePos> api = std::make_shared<SignedFilePos>(108, 31, 280, 2068);

    int32_t offset = api->GetFileNameOffset();

    EXPECT_EQ(offset, 108);
}

/**
 * @tc.name: getFileNameSize
 * @tc.desc: Test function of SignedFilePosTest::GetFileNameSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignedFilePosTest, getFileNameSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignedFilePos> api = std::make_shared<SignedFilePos>(108, 31, 280, 2068);

    int32_t fileNameSize = api->GetFileNameSize();

    EXPECT_EQ(fileNameSize, 31);
}

/**
 * @tc.name: getSignInfoOffset
 * @tc.desc: Test function of SignedFilePosTest::GetSignInfoOffset() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignedFilePosTest, getSignInfoOffset, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignedFilePos> api = std::make_shared<SignedFilePos>(108, 31, 280, 2068);

    int32_t signInfoOffset = api->GetSignInfoOffset();

    EXPECT_EQ(signInfoOffset, 280);
}

/**
 * @tc.name: getSignInfoSize
 * @tc.desc: Test function of SignedFilePosTest::GetSignInfoSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignedFilePosTest, getSignInfoSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignedFilePos> api = std::make_shared<SignedFilePos>(108, 31, 280, 2068);

    int32_t signInfoSize = api->GetSignInfoSize();

    EXPECT_EQ(signInfoSize, 2068);
}

/**
 * @tc.name: increaseFileNameOffset
 * @tc.desc: Test function of SignedFilePosTest::IncreaseFileNameOffset() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignedFilePosTest, increaseFileNameOffset, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignedFilePos> api = std::make_shared<SignedFilePos>(108, 31, 280, 2068);

    api->IncreaseFileNameOffset(1);
    int32_t offset = api->GetFileNameOffset();

    EXPECT_EQ(offset, 109);
}

/**
 * @tc.name: increaseSignInfoOffset
 * @tc.desc: Test function of SignedFilePosTest::IncreaseSignInfoOffset() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignedFilePosTest, increaseSignInfoOffset, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignedFilePos> api = std::make_shared<SignedFilePos>(108, 31, 280, 2068);

    api->IncreaseSignInfoOffset(2);
    int32_t offset = api->GetSignInfoOffset();

    EXPECT_EQ(offset, 282);
}
} // namespace SignatureTools
} // namespace OHOS