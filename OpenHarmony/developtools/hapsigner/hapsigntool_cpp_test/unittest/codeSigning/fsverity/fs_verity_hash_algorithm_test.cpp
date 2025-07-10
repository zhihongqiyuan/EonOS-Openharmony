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
#include "fs_verity_hash_algorithm.h"

namespace OHOS {
namespace SignatureTools {

class FsVerityHashAlgorithmTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: GetId
 * @tc.desc: Test function of FsVerityHashAlgorithm::GetId() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityHashAlgorithmTest, GetId, testing::ext::TestSize.Level1)
{
    const FsVerityHashAlgorithm SHA256((char)1, "SHA-256", 256 / 8);

    EXPECT_EQ(SHA256.GetId(), 1);
}

/**
 * @tc.name: GetHashAlgorithm
 * @tc.desc: Test function of FsVerityHashAlgorithm::GetHashAlgorithm() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityHashAlgorithmTest, GetHashAlgorithm, testing::ext::TestSize.Level1)
{
    const FsVerityHashAlgorithm SHA256((char)1, "SHA-256", 256 / 8);

    EXPECT_EQ(SHA256.GetHashAlgorithm(), "SHA-256");
}

/**
 * @tc.name: GetOutputByteSize
 * @tc.desc: Test function of FsVerityHashAlgorithm::GetOutputByteSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityHashAlgorithmTest, GetOutputByteSize, testing::ext::TestSize.Level1)
{
    const FsVerityHashAlgorithm SHA256((char)1, "SHA-256", 256 / 8);

    EXPECT_EQ(SHA256.GetOutputByteSize(), 256 / 8);
}
} // namespace SignatureTools
} // namespace OHOS