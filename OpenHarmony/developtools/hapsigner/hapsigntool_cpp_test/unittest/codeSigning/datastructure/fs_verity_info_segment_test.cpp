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
#include "fs_verity_info_segment.h"

namespace OHOS {
namespace SignatureTools {

/*
 * 测试套件,固定写法
 */
class FsVerityInfoSegmentTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: fromByteArray001
 * @tc.desc: Go to the first branch, size inequality.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, fromByteArray001, testing::ext::TestSize.Level1)
{
    // 走第一个分支:size不相等
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();

    std::vector<int8_t> bytes;
    api->FromByteArray(bytes);

    EXPECT_EQ(api->Size(), 64);
}

/**
 * @tc.name: fromByteArray002
 * @tc.desc: Go to the second branch, inMagic inequality.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, fromByteArray002, testing::ext::TestSize.Level1)
{
    // 走第二个分支:inMagic 不相等
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();

    ByteBuffer byteBuffer(64);
    byteBuffer.PutInt32(0);  // inMagic
    byteBuffer.Flip();

    char readComment[64] = { 0 };
    byteBuffer.GetData(readComment, 64);
    std::vector<int8_t> bytes(readComment, readComment + 64);
    
    api->FromByteArray(bytes);

    EXPECT_EQ(api->Size(), 64);
}

/**
 * @tc.name: fromByteArray003
 * @tc.desc: Go to the third branch, inVersion inequality.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, fromByteArray003, testing::ext::TestSize.Level1)
{
    // 走第三个分支:inVersion 不相等
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();

    ByteBuffer byteBuffer(64);
    byteBuffer.PutInt32(506999211);  // inMagic
    byteBuffer.PutUInt8(0);  // inVersion
    byteBuffer.Flip();

    char readComment[64] = { 0 };
    byteBuffer.GetData(readComment, 64);
    std::vector<int8_t> bytes(readComment, readComment + 64);
    
    api->FromByteArray(bytes);

    EXPECT_EQ(api->Size(), 64);
}

/**
 * @tc.name: fromByteArray004
 * @tc.desc: Go to the forth branch, inHashAlgorithm inequality.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, fromByteArray004, testing::ext::TestSize.Level1)
{
    // 走第四个分支:inHashAlgorithm 不相等
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();

    ByteBuffer byteBuffer(64);
    byteBuffer.PutInt32(506999211);  // inMagic
    byteBuffer.PutUInt8(1);  // inVersion
    byteBuffer.PutUInt8(0);  // inHashAlgorithm
    byteBuffer.Flip();

    char readComment[64] = { 0 };
    byteBuffer.GetData(readComment, 64);
    std::vector<int8_t> bytes(readComment, readComment + 64);
    
    api->FromByteArray(bytes);

    EXPECT_EQ(api->Size(), 64);
}

/**
 * @tc.name: fromByteArray005
 * @tc.desc: Go to the fifth branch, inLog2BlockSize inequality.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, fromByteArray005, testing::ext::TestSize.Level1)
{
    // 走第五个分支:inLog2BlockSize 不相等
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();

    ByteBuffer byteBuffer(64);
    byteBuffer.PutInt32(506999211);  // inMagic
    byteBuffer.PutUInt8(1);  // inVersion
    byteBuffer.PutUInt8(1);  // inHashAlgorithm
    byteBuffer.PutUInt8(0);  // inLog2BlockSize
    byteBuffer.Flip();

    char readComment[64] = { 0 };
    byteBuffer.GetData(readComment, 64);
    std::vector<int8_t> bytes(readComment, readComment + 64);
    
    api->FromByteArray(bytes);

    EXPECT_EQ(api->Size(), 64);
}

/**
 * @tc.name: fromByteArray006
 * @tc.desc: Test function of FsVerityInfoSegmentTest::fromByteArray006() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, fromByteArray006, testing::ext::TestSize.Level1)
{
    // 走完
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();

    ByteBuffer byteBuffer(64);
    byteBuffer.PutInt32(506999211);  // inMagic
    byteBuffer.PutUInt8(1);  // inVersion
    byteBuffer.PutUInt8(1);  // inHashAlgorithm
    byteBuffer.PutUInt8(12);  // inLog2BlockSize
    byteBuffer.Flip();

    char readComment[64] = { 0 };
    byteBuffer.GetData(readComment, 64);
    std::vector<int8_t> bytes(readComment, readComment + 64);
    
    api->FromByteArray(bytes);

    EXPECT_EQ(api->Size(), 64);
}

/**
 * @tc.name: fromByteArray
 * @tc.desc: Test function of FsVerityInfoSegmentTest::fromByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, fromByteArray, testing::ext::TestSize.Level1)
{
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();

    std::vector<int8_t> bytes{ 12, 45, 58, -12, 38, 29, 12, 45, 58, -12, 38, 29, 12, 45,
        58, -12, 38, 29, 12, 45, 58, -12, 38, 29, 12, 45, 58, -12, 38, 29, 13, 26, 12, 45,
        58, -12, 38, 29, 12, 45, 58, -12, 38, 29, 12, 45, 58, -12, 38, 29, 12, 45, 58, -12,
        38, 29, 12, 45, 58, -12, 38, 29, 13, 26 };
    api->FromByteArray(bytes);

    EXPECT_EQ(api->Size(), 64);
}

/**
 * @tc.name: size
 * @tc.desc: Test function of FsVerityInfoSegmentTest::size() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, size, testing::ext::TestSize.Level1)
{
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();
    
    int fsVerityInfoSegmentSize = api->Size();

    EXPECT_EQ(fsVerityInfoSegmentSize, 64);
}

/**
 * @tc.name: toByteArray
 * @tc.desc: Test function of FsVerityInfoSegmentTest::toByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityInfoSegmentTest, toByteArray, testing::ext::TestSize.Level1)
{
    std::shared_ptr<FsVerityInfoSegment> api = std::make_shared<FsVerityInfoSegment>();

    std::vector<int8_t> byteArray;
    api->ToByteArray(byteArray);

    EXPECT_EQ(byteArray.size(), 64);
}
} // namespace SignatureTools
} // namespace OHOS