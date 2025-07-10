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
#include "native_lib_info_segment.h"

namespace OHOS {
namespace SignatureTools {

/*
 * 测试套件,固定写法
 */
class NativeLibInfoSegmentTest : public testing::Test {
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
 * @tc.desc: inMagic values are not standard.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, fromByteArray001, testing::ext::TestSize.Level1)
{
    // 走进 inMagic = 4096这个分支
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    ByteBuffer byteBuffer(4);
    byteBuffer.PutInt32(4096); // inMagic
    byteBuffer.Flip();

    char readComment[4] = { 0 };
    byteBuffer.GetData(readComment, 4);
    std::vector<int8_t> bytes(readComment, readComment + 4);

    NativeLibInfoSegment segment = api->FromByteArray(bytes);

    EXPECT_NE(segment.Size(), 0);
}

/**
 * @tc.name: fromByteArray002
 * @tc.desc: The value of inSegmentSize is less than 0
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, fromByteArray002, testing::ext::TestSize.Level1)
{
    // 走进 if (inSegmentSize < 0) 这个分支
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    ByteBuffer byteBuffer(8);
    byteBuffer.PutInt32(248702752); // inMagic
    byteBuffer.PutInt32(-1); // inSegmentSize
    byteBuffer.Flip();

    char readComment[8] = { 0 };
    byteBuffer.GetData(readComment, 8);
    std::vector<int8_t> bytes(readComment, readComment + 8);

    NativeLibInfoSegment segment = api->FromByteArray(bytes);

    EXPECT_NE(segment.Size(), 0);
}

/**
 * @tc.name: fromByteArray003
 * @tc.desc: The value of inSectionNum is less than 0
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, fromByteArray003, testing::ext::TestSize.Level1)
{
    // 走进 if (inSectionNum < 0) 这个分支
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    ByteBuffer byteBuffer(12);
    byteBuffer.PutInt32(248702752); // inMagic
    byteBuffer.PutInt32(0); // inSegmentSize
    byteBuffer.PutInt32(-1); // inSectionNum
    byteBuffer.Flip();

    char readComment[12] = { 0 };
    byteBuffer.GetData(readComment, 12);
    std::vector<int8_t> bytes(readComment, readComment + 12);

    NativeLibInfoSegment segment = api->FromByteArray(bytes);

    EXPECT_NE(segment.GetSectionNum(), -1);
}

/**
 * @tc.name: fromByteArray004
 * @tc.desc: The value of inSignInfoOffset is not a multiple of 4.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, fromByteArray004, testing::ext::TestSize.Level1)
{
    // 走进 if (pos.getSignInfoOffset() % ALIGNMENT_FOR_SIGNINFO != 0) 这个分支
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    ByteBuffer byteBuffer(36);
    byteBuffer.PutInt32(248702752); // inMagic
    byteBuffer.PutInt32(0); // inSegmentSize
    byteBuffer.PutInt32(1); // inSectionNum

    byteBuffer.PutInt32(1); // inFileNameOffset
    byteBuffer.PutInt32(7); // inFileNameSize
    byteBuffer.PutInt32(3); // inSignInfoOffset
    byteBuffer.PutInt32(4); // inSignInfoSize

    std::string fileName = { "test.so" };
    byteBuffer.PutData(fileName.c_str(), 7);

    std::vector<int8_t> inZeroPadding(1, 33);
    byteBuffer.PutData((char*)inZeroPadding.data(), 1);

    byteBuffer.Flip();

    char readComment[36] = { 0 };
    byteBuffer.GetData(readComment, 36);
    std::vector<int8_t> bytes(readComment, readComment + 36);

    NativeLibInfoSegment segment = api->FromByteArray(bytes);

    EXPECT_NE(segment.GetSectionNum(), 1);
}

/**
 * @tc.name: fromByteArray005
 * @tc.desc: Test function of NativeLibInfoSegmentTest::FromByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, fromByteArray005, testing::ext::TestSize.Level1)
{
    // 走完
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    ByteBuffer byteBuffer(36);
    byteBuffer.PutInt32(248702752); // inMagic
    byteBuffer.PutInt32(0); // inSegmentSize
    byteBuffer.PutInt32(1); // inSectionNum

    byteBuffer.PutInt32(1); // inFileNameOffset
    byteBuffer.PutInt32(7); // inFileNameSize
    byteBuffer.PutInt32(4); // inSignInfoOffset
    byteBuffer.PutInt32(4); // inSignInfoSize

    std::string fileName = { "test.so" };
    byteBuffer.PutData(fileName.c_str(), 7);

    std::vector<int8_t> inZeroPadding(1, 33);
    byteBuffer.PutData((char*)inZeroPadding.data(), 1);

    byteBuffer.Flip();

    char readComment[36] = { 0 };
    byteBuffer.GetData(readComment, 36);
    std::vector<int8_t> bytes(readComment, readComment + 36);

    NativeLibInfoSegment segment = api->FromByteArray(bytes);

    EXPECT_NE(segment.GetFileNameList()[0], "");
}

/**
 * @tc.name: getFileNameList
 * @tc.desc: Test function of NativeLibInfoSegmentTest::GetFileNameList() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, getFileNameList, testing::ext::TestSize.Level1)
{
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    std::vector<std::string> fileNameVec = api->GetFileNameList();

    EXPECT_EQ(fileNameVec.size(), 0);
}

/**
 * @tc.name: getSectionNum
 * @tc.desc: Test function of NativeLibInfoSegmentTest::GetSectionNum() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, getSectionNum, testing::ext::TestSize.Level1)
{
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();
    int32_t num = api->GetSectionNum();

    EXPECT_EQ(num, 0);
}

/**
 * @tc.name: getSignInfoList
 * @tc.desc: Test function of NativeLibInfoSegmentTest::GetSignInfoList() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, getSignInfoList, testing::ext::TestSize.Level1)
{
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    std::vector<SignInfo> signInfoVec = api->GetSignInfoList();

    EXPECT_EQ(signInfoVec.size(), 0);
}

/**
 * @tc.name: setSoInfoList
 * @tc.desc: Test function of NativeLibInfoSegmentTest::SetSoInfoList() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, setSoInfoList, testing::ext::TestSize.Level1)
{
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    int32_t saltSize = 0;
    int32_t flags = 1;
    int64_t dataSize = 5390336;
    std::vector<int8_t> salt;
    std::vector<int8_t> sig1{ 48, -126, 7, -46, 6, 9, 42, -122, 72, -122, -9, 13, 1, 7, 2,
        -96, -126, 7, -61, 48, -126, 7, -65, 2, 1, 1, 49, 13, 48, 11, 6, 9, 96, -122, 72,
        1, 101, 3, 4, 2, 1, 48, 11, 6, 9, 42, -122, 72, -122, -9, 13, 1, 7, 1, -96, -126,
        6, 43, 48, -126, 1, -32, 48, -126, 1, -121, -96, 3, 2, 1, 2, 2, 4, 85, -67, -54,
        116, 48, 10, 6, 8, 42, -122, 72, -50, 61, 4, 3, 3, 48, 85, 49, 11, 48, 9, 6, 3,
        85, 4, 6, 1 };
    std::vector<int8_t> sig2{ 49, -127, 8, -47, 7, 10, 43, -123, 73, -123, -10, 14, 2, 8,
        3, -97, -127, 8, -62, 49, -127, 8, -66, 3, 2, 2, 50, 14, 49, 12, 7, 10, 97, -123,
        73, 2, 102, 4, 5, 3, 2, 49, 12, 7, 10, 43, -123, 73, -123, -10, 14, 2, 8, 2, -97,
        -127, 7, 44, 49, -127, 2, -33, 49, -127, 2, -122, -97, 4, 3, 2, 3, 3, 5, 86, -68,
        -55, 117, 49, 11, 7, 9, 43, -123, 73, -51, 62, 5, 4, 4, 49, 86, 50, 12, 49, 10, 7,
        4, 86, 5, 7, 2 };
    SignInfo signInfo1(saltSize, flags, dataSize, salt, sig1);
    SignInfo signInfo2(saltSize, flags, dataSize, salt, sig2);

    std::vector<std::pair<std::string, SignInfo>> soInfoList;
    soInfoList.push_back(std::make_pair("info.so", signInfo1));
    soInfoList.push_back(std::make_pair("info.so", signInfo2));

    api->SetSoInfoList(soInfoList);

    EXPECT_EQ(api->GetSectionNum(), 2);
}

/**
 * @tc.name: size
 * @tc.desc: Test function of NativeLibInfoSegmentTest::Size() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(NativeLibInfoSegmentTest, size, testing::ext::TestSize.Level1)
{
    std::shared_ptr<NativeLibInfoSegment> api = std::make_shared<NativeLibInfoSegment>();

    int32_t nativeLibInfoSegmentSize = api->Size();

    EXPECT_NE(nativeLibInfoSegmentSize, 0);
}
} // namespace SignatureTools
} // namespace OHOS
