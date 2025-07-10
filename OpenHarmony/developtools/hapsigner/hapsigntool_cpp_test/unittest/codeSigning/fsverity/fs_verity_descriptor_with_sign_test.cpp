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

#include "fs_verity_descriptor_with_sign.h"

namespace OHOS {
namespace SignatureTools {

class FsVerityDescriptorWithSignTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: Size001
 * @tc.desc: Test function of FsVerityDescriptorWithSign::Size() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityDescriptorWithSignTest, Size001, testing::ext::TestSize.Level1)
{
    FsVerityDescriptorWithSign descriptor;
    int32_t size = descriptor.Size();

    EXPECT_NE(size, 0);
}

/**
 * @tc.name: Size002
 * @tc.desc: Test function of FsVerityDescriptorWithSign::Size() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityDescriptorWithSignTest, Size002, testing::ext::TestSize.Level1)
{
    FsVerityDescriptor fsVerityDescriptor;
    std::vector<int8_t> signature = { 1, 1, 1, 1 };
    FsVerityDescriptorWithSign descriptor(fsVerityDescriptor, signature);
    int32_t size = descriptor.Size();

    EXPECT_NE(size, 0);
}

/**
 * @tc.name: Size003
 * @tc.desc: Test function of FsVerityDescriptorWithSign::Size() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityDescriptorWithSignTest, Size003, testing::ext::TestSize.Level1)
{
    FsVerityDescriptor fsVerityDescriptor;
    std::vector<int8_t> signature;
    FsVerityDescriptorWithSign descriptor(fsVerityDescriptor, signature);
    int32_t size = descriptor.Size();

    EXPECT_NE(size, 0);
}

/**
 * @tc.name: ToByteArray
 * @tc.desc: Test function of FsVerityDescriptorWithSign::ToByteArray() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityDescriptorWithSignTest, ToByteArray, testing::ext::TestSize.Level1)
{
    FsVerityDescriptor fsVerityDescriptor;
    std::vector<int8_t> signature = { 1, 1, 1, 1 };
    int32_t type = 1;
    int32_t length = 0;
    FsVerityDescriptorWithSign descriptor(type, length, fsVerityDescriptor, signature);
    std::vector<int8_t> bytes;
    descriptor.ToByteArray(bytes);
    int32_t size = bytes.size();

    EXPECT_NE(size, 0);
}

/**
 * @tc.name: GetFsVerityDescriptor
 * @tc.desc: Test function of FsVerityDescriptorWithSign::GetFsVerityDescriptor() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityDescriptorWithSignTest, GetFsVerityDescriptor, testing::ext::TestSize.Level1)
{
    FsVerityDescriptor fsVerityDescriptor;
    std::vector<int8_t> signature = { 1, 1, 1, 1 };
    int32_t type = 1;
    int32_t length = 0;
    FsVerityDescriptorWithSign descriptor(type, length, fsVerityDescriptor, signature);
    FsVerityDescriptor getObj = descriptor.GetFsVerityDescriptor();
    int32_t size = getObj.GetSignSize();

    EXPECT_EQ(size, 0);
}

/**
 * @tc.name: GetSignature
 * @tc.desc: Test function of FsVerityDescriptorWithSign::GetSignature() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityDescriptorWithSignTest, GetSignature, testing::ext::TestSize.Level1)
{
    FsVerityDescriptor fsVerityDescriptor;
    std::vector<int8_t> signature = { 1, 1, 1, 1 };
    int32_t type = 1;
    int32_t length = 0;
    FsVerityDescriptorWithSign descriptor(type, length, fsVerityDescriptor, signature);
    std::vector<int8_t> sig = descriptor.GetSignature();
    int32_t size = sig.size();

    EXPECT_EQ(size, 4);
}
} // namespace SignatureTools
} // namespace OHOS