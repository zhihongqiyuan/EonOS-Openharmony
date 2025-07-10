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
#include "segment_header.h"

namespace OHOS {
namespace SignatureTools {

/*
 * 测试套件,固定写法
 */
class SegmentHeaderTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: getSegmentOffset
 * @tc.desc: Test function of SegmentHeaderTest::getSegmentOffset() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SegmentHeaderTest, getSegmentOffset, testing::ext::TestSize.Level1)
{
    std::shared_ptr<SegmentHeader> api = std::make_shared<SegmentHeader>();

    int32_t segmentOffset = api->GetSegmentOffset();

    EXPECT_NE(segmentOffset, -1);
}

/**
 * @tc.name: FromByteArray001
 * @tc.desc: Test function of SegmentHeaderTest::FromByteArray001() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SegmentHeaderTest, FromByteArray001, testing::ext::TestSize.Level1)
{
    SegmentHeader segmentHeader(1, 1, 8);
    std::vector<int8_t> arr = { 1, 1, 1, 1 };
    std::unique_ptr<SegmentHeader> ptr = segmentHeader.FromByteArray(arr);

    EXPECT_EQ(arr.size(), 4);
}

/**
 * @tc.name: FromByteArray002
 * @tc.desc: Test function of SegmentHeaderTest::FromByteArray002() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SegmentHeaderTest, FromByteArray002, testing::ext::TestSize.Level1)
{
    SegmentHeader segmentHeader(0, 1, 8);
    std::vector<int8_t> arr;
    segmentHeader.ToByteArray(arr);
    std::unique_ptr<SegmentHeader> ptr = segmentHeader.FromByteArray(arr);

    EXPECT_NE(arr.size(), 0);
}

/**
 * @tc.name: FromByteArray003
 * @tc.desc: Test function of SegmentHeaderTest::FromByteArray003() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(SegmentHeaderTest, FromByteArray003, testing::ext::TestSize.Level1)
{
    SegmentHeader segmentHeader(1, 1, -1);
    std::vector<int8_t> arr;
    segmentHeader.ToByteArray(arr);
    std::unique_ptr<SegmentHeader> ptr = segmentHeader.FromByteArray(arr);

    EXPECT_NE(arr.size(), 0);
}
} // namespace SignatureTools
} // namespace OHOS