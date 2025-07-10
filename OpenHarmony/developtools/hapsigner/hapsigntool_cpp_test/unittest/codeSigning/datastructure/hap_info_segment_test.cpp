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
#include "hap_info_segment.h"

namespace OHOS {
namespace SignatureTools {

/*
 * 测试套件,固定写法
 */
class HapInfoSegmentTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: fromByteArray001
 * @tc.desc: Go to the first branch, fromByteArray001 inequality.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapInfoSegmentTest, fromByteArray001, testing::ext::TestSize.Level1)
{
    // 走第一个分支:inMagic 不一样
    std::shared_ptr<HapInfoSegment> api = std::make_shared<HapInfoSegment>();

    ByteBuffer byteBuffer(32);
    byteBuffer.PutInt32(0); // inMagic
    byteBuffer.Flip();

    char readComment[32] = { 0 };
    byteBuffer.GetData(readComment, 32);
    std::vector<int8_t> bytes(readComment, readComment + 32);

    api->FromByteArray(bytes);

    EXPECT_NE(api->GetSize(), 0);
}

/**
 * @tc.name: fromByteArray002
 * @tc.desc: Go to the third branch, the dataSize is not an integer multiple of 4096.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapInfoSegmentTest, fromByteArray002, testing::ext::TestSize.Level1)
{
    // 走第三个分支:inHapSignInfo.getDataSize()
    std::shared_ptr<HapInfoSegment> api = std::make_shared<HapInfoSegment>();

    ByteBuffer byteBuffer(64);
    byteBuffer.PutInt32(-1045050266); // inMagic
    byteBuffer.PutInt32(0); // inSaltSize
    byteBuffer.PutInt32(0); // inSigSize
    byteBuffer.PutInt32(1); // inFlags
    byteBuffer.PutInt64(5); // inDataSize

    std::vector<int8_t> inSalt(32, 0);
    byteBuffer.PutData((const char*)inSalt.data(), 32);

    byteBuffer.PutInt32(1); // inExtensionNum
    byteBuffer.PutInt32(4); // inExtensionOffset

    byteBuffer.Flip();

    char readComment[64] = { 0 };
    byteBuffer.GetData(readComment, 64);
    std::vector<int8_t> bytes(readComment, readComment + 64);

    api->FromByteArray(bytes);

    EXPECT_NE(api->GetSize(), 0);
}

/**
 * @tc.name: fromByteArray003
 * @tc.desc: Go to the forth branch, the data has wrong extensionNum in the HapInfoSegment.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapInfoSegmentTest, fromByteArray003, testing::ext::TestSize.Level1)
{
    // 走第四个分支:inHapSignInfo.getExtensionNum()
    std::shared_ptr<HapInfoSegment> api = std::make_shared<HapInfoSegment>();

    ByteBuffer byteBuffer(64);
    byteBuffer.PutInt32(-1045050266); // inMagic
    byteBuffer.PutInt32(0); // inSaltSize
    byteBuffer.PutInt32(0); // inSigSize
    byteBuffer.PutInt32(1); // inFlags
    byteBuffer.PutInt64(4096); // inDataSize

    std::vector<int8_t> inSalt(32, 0);
    byteBuffer.PutData((const char*)inSalt.data(), 32);

    byteBuffer.PutInt32(2); // inExtensionNum
    byteBuffer.PutInt32(4); // inExtensionOffset

    byteBuffer.Flip();

    char readComment[64] = { 0 };
    byteBuffer.GetData(readComment, 64);
    std::vector<int8_t> bytes(readComment, readComment + 64);

    api->FromByteArray(bytes);

    EXPECT_NE(api->GetSize(), 0);
}

/**
 * @tc.name: fromByteArray004
 * @tc.desc: Test function of SignToolServiceImpl::fromByteArray004() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapInfoSegmentTest, fromByteArray004, testing::ext::TestSize.Level1)
{
    // 走完
    std::shared_ptr<HapInfoSegment> api = std::make_shared<HapInfoSegment>();

    ByteBuffer byteBuffer(64);
    byteBuffer.PutInt32(-1045050266); // inMagic
    byteBuffer.PutInt32(0); // inSaltSize
    byteBuffer.PutInt32(0); // inSigSize
    byteBuffer.PutInt32(1); // inFlags
    byteBuffer.PutInt64(4096); // inDataSize

    std::vector<int8_t> inSalt(32, 0);
    byteBuffer.PutData((const char*)inSalt.data(), 32);

    byteBuffer.PutInt32(1); // inExtensionNum
    byteBuffer.PutInt32(4); // inExtensionOffset

    byteBuffer.Flip();

    char readComment[64] = { 0 };
    byteBuffer.GetData(readComment, 64);
    std::vector<int8_t> bytes(readComment, readComment + 64);

    api->FromByteArray(bytes);

    EXPECT_NE(api->GetSize(), 0);
}


/**
 * @tc.name: getSignInfo
 * @tc.desc: Test function of SignToolServiceImpl::getSignInfo() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapInfoSegmentTest, getSignInfo, testing::ext::TestSize.Level1)
{
    std::shared_ptr<HapInfoSegment> api = std::make_shared<HapInfoSegment>();

    api->GetSignInfo();

    EXPECT_NE(api->GetSize(), 0);
}

/**
 * @tc.name: getSize
 * @tc.desc: Test function of SignToolServiceImpl::getSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapInfoSegmentTest, getSize, testing::ext::TestSize.Level1)
{
    std::shared_ptr<HapInfoSegment> api = std::make_shared<HapInfoSegment>();

    int32_t hapInfoSegmentSize = api->GetSize();

    EXPECT_EQ(hapInfoSegmentSize, 64);
}

/**
 * @tc.name: setSignInfo
 * @tc.desc: Test function of SignToolServiceImpl::setSignInfo() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapInfoSegmentTest, setSignInfo, testing::ext::TestSize.Level1)
{
    std::shared_ptr<HapInfoSegment> api = std::make_shared<HapInfoSegment>();

    int32_t saltSize = 0;
    int32_t flags = 1;
    int64_t dataSize = 5390336;
    std::vector<int8_t> salt;
    std::vector<int8_t> sig{ 48, -126, 7, -46, 6, 9, 42, -122, 72, -122, -9, 13,
        1, 7, 2, -96, -126, 7, -61, 48, -126, 7, -65, 2, 1, 1, 49, 13, 48, 11, 6,
        9, 96, -122, 72, 1, 101, 3, 4, 2, 1, 48, 11, 6, 9, 42, -122, 72, -122, -9,
        13, 1, 7, 1, -96, -126, 6, 43, 48, -126, 1, -32, 48, -126, 1, -121, -96, 3,
        2, 1, 2, 2, 4, 85, -67, -54, 116, 48, 10, 6, 8, 42, -122, 72, -50, 61, 4, 3,
        3, 48, 85, 49, 11, 48, 9, 6, 3, 85, 4, 6, 1 };
    SignInfo signInfo(saltSize, flags, dataSize, salt, sig);
    api->SetSignInfo(signInfo);

    EXPECT_NE(api->GetSize(), 0);
}

/**
 * @tc.name: toByteArray
 * @tc.desc: Test function of SignToolServiceImpl::toByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(HapInfoSegmentTest, toByteArray, testing::ext::TestSize.Level1)
{
    std::shared_ptr<HapInfoSegment> api = std::make_shared<HapInfoSegment>();
    
    std::vector<int8_t> byteArray;
    api->ToByteArray(byteArray);

    EXPECT_EQ(byteArray.size(), 64);
}
} // namespace SignatureTools
} // namespace OHOS