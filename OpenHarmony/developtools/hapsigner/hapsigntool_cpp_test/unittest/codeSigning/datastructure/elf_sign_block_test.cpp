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

#include "elf_sign_block.h"
#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {

class ElfSignBlockTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: FromByteArray001
 * @tc.desc: The return will be false，because the data has wrong signature length in the ElfSignBlock.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ElfSignBlockTest, FromByteArray001, testing::ext::TestSize.Level1)
{
    int32_t treeLength = 8;
    std::vector<int8_t> merkleTreeWithPadding = { 1, 1, 1, 1, 1, 1, 1, 1 };
    FsVerityDescriptorWithSign descriptorWithSign;
    ElfSignBlock elfSignBlock(treeLength, merkleTreeWithPadding, descriptorWithSign);
    std::vector<int8_t> arr;
    elfSignBlock.ToByteArray(arr);
    ElfSignBlock block;
    bool flag = elfSignBlock.FromByteArray(arr, block);

    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: FromByteArray002
 * @tc.desc: The return will be false，because the data has wrong fs-verify descriptor type in the ElfSignBlock
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(ElfSignBlockTest, FromByteArray002, testing::ext::TestSize.Level1)
{
    int32_t treeLength = 8;
    std::vector<int8_t> merkleTreeWithPadding = { 1, 1, 1, 1, 1, 1, 1, 1 };
    FsVerityDescriptorWithSign descriptorWithSign;
    ElfSignBlock elfSignBlock(treeLength, merkleTreeWithPadding, descriptorWithSign);
    std::vector<int8_t> arr;
    elfSignBlock.ToByteArray(arr);
    arr[24] = 0;
    ElfSignBlock block;
    bool flag = elfSignBlock.FromByteArray(arr, block);

    EXPECT_EQ(flag, false);
}
} // namespace SignatureTools
} // namespace OHOS