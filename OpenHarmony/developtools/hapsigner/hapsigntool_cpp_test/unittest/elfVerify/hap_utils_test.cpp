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

#include "hap_utils.h"
#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {

class HapUtilsTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: GetAppIdentifier
 * @tc.desc: Test function of HapUtilsTest::GetAppIdentifier() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapUtilsTest, GetAppIdentifier, testing::ext::TestSize.Level1)
{
    std::string content = "{\"type\":\"xxx\",\"bundle-info\":{\"app-identifier\":\"123\"}}";
    std::string result = HapUtils::GetAppIdentifier(content);
    int length = result.length();

    EXPECT_NE(length, 0);
}

/**
 * @tc.name: GetHapSigningBlockMagic
 * @tc.desc: Test function of HapUtilsTest::GetHapSigningBlockMagic() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapUtilsTest, GetHapSigningBlockMagic, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> result = HapUtils::GetHapSigningBlockMagic(4);
    int size = result.size();

    EXPECT_NE(size, 0);
}

/**
 * @tc.name: GetHapSigningBlockVersion
 * @tc.desc: Test function of HapUtilsTest::GetHapSigningBlockVersion() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapUtilsTest, GetHapSigningBlockVersion, testing::ext::TestSize.Level1)
{
    int result = HapUtils::GetHapSigningBlockVersion(4);

    EXPECT_NE(result, 0);
}

/**
 * @tc.name: ReadFileToByteBuffer
 * @tc.desc: Test function of HapUtilsTest::ReadFileToByteBuffer() interface for FAILED.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapUtilsTest, ReadFileToByteBuffer, testing::ext::TestSize.Level1)
{
    std::string file;
    ByteBuffer buffer;
    bool result = HapUtils::ReadFileToByteBuffer(file, buffer);

    EXPECT_EQ(result, false);
}
} // namespace SignatureTools
} // namespace OHOS