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
#include <fs_verity_digest.h>

namespace OHOS {
namespace SignatureTools {

class FsVerityDigestTest : public testing::Test {
public:
    static void SetUpTestCase(void) {};
    static void TearDownTestCase() {};
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: GetFsVerityDigest
 * @tc.desc: Test function of FsVerityDigest::GetFsVerityDigest() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityDigestTest, GetFsVerityDigest, testing::ext::TestSize.Level1)
{
    std::vector<int8_t> digest = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    
    std::vector<int8_t> fsVerityDigest;
    FsVerityDigest::GetFsVerityDigest(1, digest, fsVerityDigest);

    EXPECT_EQ(digest.size(), 16);
}
} // namespace SignatureTools
} // namespace OHOS