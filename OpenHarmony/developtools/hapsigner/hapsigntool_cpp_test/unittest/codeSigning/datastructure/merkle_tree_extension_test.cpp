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
#include "merkle_tree_extension.h"

namespace OHOS {
namespace SignatureTools {

/*
 * 测试套件,固定写法
 */
class MerkleTreeExtensionTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: fromByteArray001
 * @tc.desc: Go to the third branch, the inMerkleTreeSize is not an integer multiple of 4096.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, fromByteArray001, testing::ext::TestSize.Level1)
{
    // 走进第一个分支:inMerkleTreeSize = 4095
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    ByteBuffer byteBuffer(8);
    byteBuffer.PutInt64(4095);
    byteBuffer.Flip();

    char readComment[8] = { 0 };
    byteBuffer.GetData(readComment, 8);
    std::vector<int8_t> bytes(readComment, readComment + 8);

    Extension* pExtension = api->FromByteArray(bytes);

    EXPECT_EQ(pExtension, nullptr);
}

/**
 * @tc.name: fromByteArray002
 * @tc.desc: Go to the third branch, the inMerkleTreeOffset is not an integer multiple of 4096.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, fromByteArray002, testing::ext::TestSize.Level1)
{
    // 走进第二个分支:inMerkleTreeOffset = 4095
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    ByteBuffer byteBuffer(16);
    byteBuffer.PutInt64(4096);
    byteBuffer.PutInt64(4095); // inMerkleTreeOffset
    byteBuffer.Flip();

    char readComment[16] = { 0 };
    byteBuffer.GetData(readComment, 16);
    std::vector<int8_t> bytes(readComment, readComment + 16);

    Extension* pExtension = api->FromByteArray(bytes);

    EXPECT_EQ(pExtension, nullptr);
}

/**
 * @tc.name: fromByteArray003
 * @tc.desc: Test function of MerkleTreeExtensionTest::fromByteArray003() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, fromByteArray003, testing::ext::TestSize.Level1)
{
    // 走完
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    ByteBuffer byteBuffer(16);
    byteBuffer.PutInt64(4096);
    byteBuffer.PutInt64(4096); // inMerkleTreeOffset
    byteBuffer.Flip();

    char readComment[16] = { 0 };
    byteBuffer.GetData(readComment, 16);
    std::vector<int8_t> bytes(readComment, readComment + 16);

    Extension* pExtension = api->FromByteArray(bytes);

    EXPECT_NE(pExtension, nullptr);
}

/**
 * @tc.name: getMerkleTreeOffset
 * @tc.desc: Test function of MerkleTreeExtensionTest::GetMerkleTreeOffset() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, getMerkleTreeOffset, testing::ext::TestSize.Level1)
{
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    int64_t merkleTreeOffset = api->GetMerkleTreeOffset();

    EXPECT_EQ(merkleTreeOffset, 0);
}

/**
 * @tc.name: getMerkleTreeSize
 * @tc.desc: Test function of MerkleTreeExtensionTest::getMerkleTreeSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, getMerkleTreeSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    int64_t merkleTreeSize = api->GetMerkleTreeSize();

    EXPECT_EQ(merkleTreeSize, 0);
}

/**
 * @tc.name: getSize
 * @tc.desc: Test function of MerkleTreeExtensionTest::getSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, getSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    int32_t merkleTreeExtensionSize = api->GetSize();

    EXPECT_EQ(merkleTreeExtensionSize, 88);
}

/**
 * @tc.name: isType
 * @tc.desc: Test function of MerkleTreeExtensionTest::isType() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, isType, testing::ext::TestSize.Level1)
{
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    int32_t type = 1;
    bool bIsType = api->IsType(type);

    EXPECT_EQ(bIsType, true);
}

/**
 * @tc.name: setMerkleTreeOffset
 * @tc.desc: Test function of MerkleTreeExtensionTest::SetMerkleTreeOffset() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, setMerkleTreeOffset, testing::ext::TestSize.Level1)
{
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    int64_t offset = 927046;
    api->SetMerkleTreeOffset(offset);

    int64_t flag = api->GetMerkleTreeOffset();
    EXPECT_EQ(flag, offset);
}

/**
 * @tc.name: toByteArray
 * @tc.desc: Test function of MerkleTreeExtensionTest::toByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(MerkleTreeExtensionTest, toByteArray, testing::ext::TestSize.Level1)
{
    std::shared_ptr<MerkleTreeExtension> api = std::make_shared<MerkleTreeExtension>();

    std::vector<int8_t> byteArray;
    
    api->ToByteArray(byteArray);

    EXPECT_EQ(byteArray.size(), 88);
}
} // namespace SignatureTools
} // namespace OHOS