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
#include "code_sign_block_header.h"

namespace OHOS {
namespace SignatureTools {
class CodeSignBlockHeaderTest : public testing::Test {
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
 * @tc.name: fromByteArray001
 * @tc.desc: The return will be nullptr, because the header size is different with standard.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, fromByteArray001, testing::ext::TestSize.Level1)
{
    // 走第一个分支:长度不相等，返回
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    std::vector<int8_t> bytes;
    CodeSignBlockHeader* codeSignBlockHeader = api->FromByteArray(bytes);

    EXPECT_EQ(codeSignBlockHeader, nullptr);
}

/**
 * @tc.name: fromByteArray002
 * @tc.desc: The return will be nullptr, because the inMagic is different with standard.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, fromByteArray002, testing::ext::TestSize.Level1)
{
    // 走第二个分支:inMagic不相等
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    std::vector<int8_t> bytes{ -1, -91, 34, -16, 97, -32, -121, 1, 5, 3, 8, 8, 15,
        12, 12, 13, 58, 19, 50, 10, 54, 29, 59, 17, 102, 105, 15, 19, 29, 30, 32, 59 };
    CodeSignBlockHeader* codeSignBlockHeader = api->FromByteArray(bytes);

    EXPECT_EQ(codeSignBlockHeader, nullptr);
}

/**
 * @tc.name: fromByteArray003
 * @tc.desc: The return will be nullptr, because the inVersion is different with standard.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, fromByteArray003, testing::ext::TestSize.Level1)
{
    // 走第三个分支:inVersion不相等
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    ByteBuffer byteBuffer(33);
    byteBuffer.PutInt64(-2285919006713316147);
    byteBuffer.PutInt32(2);
    byteBuffer.Flip();

    char readComment[32] = { 0 };
    byteBuffer.GetData(readComment, 32);
    std::vector<int8_t> bytes(readComment, readComment + 32);

    CodeSignBlockHeader* codeSignBlockHeader = api->FromByteArray(bytes);
    EXPECT_EQ(codeSignBlockHeader, nullptr);
}

/**
 * @tc.name: fromByteArray004
 * @tc.desc: The return will be nullptr, because the inSegmentNum is different with standard.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, fromByteArray004, testing::ext::TestSize.Level1)
{
    // 走第四个分支:inSegmentNum 不相等
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    ByteBuffer byteBuffer(33);
    byteBuffer.PutInt64(-2285919006713316147);
    byteBuffer.PutInt32(1);
    byteBuffer.PutInt32(4);  // inBlockSize
    byteBuffer.PutInt32(4);  // inSegmentNum
    byteBuffer.Flip();

    char readComment[32] = { 0 };
    byteBuffer.GetData(readComment, 32);
    std::vector<int8_t> bytes(readComment, readComment + 32);

    CodeSignBlockHeader* codeSignBlockHeader = api->FromByteArray(bytes);
    EXPECT_EQ(codeSignBlockHeader, nullptr);
}

/**
 * @tc.name: fromByteArray005
 * @tc.desc: The return will be nullptr, because the inFlags is different with standard.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, fromByteArray005, testing::ext::TestSize.Level1)
{
    // 走第五个分支:inFlags 不相等
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    ByteBuffer byteBuffer(33);
    byteBuffer.PutInt64(-2285919006713316147);
    byteBuffer.PutInt32(1);
    byteBuffer.PutInt32(4);  // inBlockSize
    byteBuffer.PutInt32(3);  // inSegmentNum
    byteBuffer.PutInt32(-1); // inFlags
    byteBuffer.Flip();

    char readComment[32] = { 0 };
    byteBuffer.GetData(readComment, 32);
    std::vector<int8_t> bytes(readComment, readComment + 32);

    CodeSignBlockHeader* codeSignBlockHeader = api->FromByteArray(bytes);
    EXPECT_EQ(codeSignBlockHeader, nullptr);
}

/**
 * @tc.name: fromByteArray006
 * @tc.desc: Test function of CodeSignBlockHeaderTest::fromByteArray006() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, fromByteArray006, testing::ext::TestSize.Level1)
{
    // 走完
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    ByteBuffer byteBuffer(33);
    byteBuffer.PutInt64(-2285919006713316147);
    byteBuffer.PutInt32(1);
    byteBuffer.PutInt32(4);  // inBlockSize
    byteBuffer.PutInt32(3);  // inSegmentNum
    byteBuffer.PutInt32(0);  // inFlags
    byteBuffer.Flip();

    char readComment[32] = { 0 };
    byteBuffer.GetData(readComment, 32);
    std::vector<int8_t> bytes(readComment, readComment + 32);

    CodeSignBlockHeader* codeSignBlockHeader = api->FromByteArray(bytes);
    EXPECT_NE(codeSignBlockHeader, nullptr);
}

/**
 * @tc.name: getBlockSize
 * @tc.desc: Test function of CodeSignBlockHeaderTest::getBlockSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, getBlockSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    int blockSize = api->GetBlockSize();

    EXPECT_EQ(blockSize, 0);
}

/**
 * @tc.name: getSegmentNum
 * @tc.desc: Test function of CodeSignBlockHeaderTest::getSegmentNum() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, getSegmentNum, testing::ext::TestSize.Level1)
{
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    int segmentNum = api->GetSegmentNum();

    EXPECT_EQ(segmentNum, 0);
}

/**
 * @tc.name: setBlockSize
 * @tc.desc: Test function of CodeSignBlockHeaderTest::setBlockSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, setBlockSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    api->SetBlockSize(1024);

    EXPECT_EQ(api->GetBlockSize(), 1024);
}

/**
 * @tc.name: setSegmentNum
 * @tc.desc: Test function of CodeSignBlockHeader::setSegmentNum() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, setSegmentNum, testing::ext::TestSize.Level1)
{
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    api->SetFlags(1);
    api->SetSegmentNum(4);

    EXPECT_EQ(api->GetSegmentNum(), 4);
}

/**
 * @tc.name: size
 * @tc.desc: Test function of CodeSignBlockHeaderTest::size() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, size, testing::ext::TestSize.Level1)
{
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    int headerSize = api->Size();

    EXPECT_EQ(headerSize, 32);
}

/**
 * @tc.name: toByteArray
 * @tc.desc: Test function of CodeSignBlockHeaderTest::toByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(CodeSignBlockHeaderTest, toByteArray, testing::ext::TestSize.Level1)
{
    std::shared_ptr<CodeSignBlockHeader> api = std::make_shared<CodeSignBlockHeader>();

    std::vector<int8_t> byteArray;
    api->ToByteArray(byteArray);

    EXPECT_EQ(byteArray.size(), 32);
}
} // namespace SignatureTools
} // namespace OHOS