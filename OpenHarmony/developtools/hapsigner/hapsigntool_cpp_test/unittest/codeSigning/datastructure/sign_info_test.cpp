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
#include "sign_info.h"
#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {

/*
 * 测试套件,固定写法
 */
class SignInfoTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: fromByteArray
 * @tc.desc: Test function of SignInfoTest::FromByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignInfoTest, fromByteArray, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignInfo> api = std::make_shared<SignInfo>();

    std::vector<int8_t> bytes = { 11, -93, 88, 107, -121, 96, 121, 23, -64, -58, -95,
        -71, -126, 60, 116, 60, 10, 15, -125, 107, 127, -123, 81, 68, 28, -121, -20,
        -42, -116, -81, -6, 118, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SignInfo signInfo = api->FromByteArray(bytes);

    EXPECT_NE(signInfo.GetSize(), 0);
}

/**
 * @tc.name: getDataSize
 * @tc.desc: Test function of SignInfoTest::GetDataSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignInfoTest, getDataSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignInfo> api = std::make_shared<SignInfo>();

    int64_t dataSize = api->GetDataSize();

    EXPECT_EQ(dataSize, 0);
}

/**
 * @tc.name: getExtensionByType
 * @tc.desc: Test function of SignInfoTest::getExtensionByType() interface for nullptr.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignInfoTest, getExtensionByType, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignInfo> api = std::make_shared<SignInfo>();

    int32_t type = 1;
    Extension* pExtension = api->GetExtensionByType(type);

    EXPECT_EQ(pExtension, nullptr);
}

/**
 * @tc.name: getExtensionNum
 * @tc.desc: Test function of SignInfoTest::GetExtensionNum() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignInfoTest, getExtensionNum, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignInfo> api = std::make_shared<SignInfo>();
    
    int32_t extensionNum = api->GetExtensionNum();

    EXPECT_EQ(extensionNum, 0);
}

/**
 * @tc.name: getSignature
 * @tc.desc: Test function of SignInfoTest::getSignature() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignInfoTest, getSignature, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignInfo> api = std::make_shared<SignInfo>();

    std::vector<int8_t> signatureVec = api->GetSignature();

    EXPECT_EQ(signatureVec.size(), 0);
}

/**
 * @tc.name: getSize
 * @tc.desc: Test function of SignInfoTest::GetSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignInfoTest, getSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignInfo> api = std::make_shared<SignInfo>();

    int32_t sizeInt = api->GetSize();

    EXPECT_EQ(sizeInt, 60);
}

/**
 * @tc.name: parseMerkleTreeExtension
 * @tc.desc: The data has the wrong extensionType in the SignInfo
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignInfoTest, parseMerkleTreeExtension, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignInfo> api = std::make_shared<SignInfo>();

    std::vector<int8_t> bytes = std::vector<int8_t>{ 1, 0, 0, 0, 80, 0, 0, 0, 0, 16, 0, 0,
        0, 0, 0, 0, 0, 96, 21, 0, 0, 0, 0, 0, 75, 43, 18, -27, 86, 118, 101, 64, -128, -112,
        84, 68, 4, -107, 110, 92, 33, -118, 113, -65, -79, -103, 40, 59, 82, -90, -87, -115,
        27, 77, 3, 95, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0 };
    ByteBuffer bf((int32_t)bytes.size());
    bf.PutData((char*)bytes.data(), bytes.size());

    std::vector<MerkleTreeExtension*> extensionVec = api->ParseMerkleTreeExtension(&bf, 1);

    EXPECT_EQ(extensionVec.size(), 0);
}

/**
 * @tc.name: toByteArray
 * @tc.desc: Test function of SignInfoTest::ToByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SignInfoTest, toByteArray, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SignInfo> api = std::make_shared<SignInfo>();

    std::vector<int8_t> bytes;
    api->ToByteArray(bytes);

    EXPECT_EQ(bytes.size(), 60);
}
} // namespace SignatureTools
} // namespace OHOS