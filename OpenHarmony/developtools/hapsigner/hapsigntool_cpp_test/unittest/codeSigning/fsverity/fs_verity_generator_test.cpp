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
#include "fs_verity_generator.h"

namespace OHOS {
namespace SignatureTools {

class FsVerityGeneratorTest : public testing::Test {
public:
    static void SetUpTestCase(void)
    {
        (void)rename("./codeSigning/entry-default-signed-so.txt", "./codeSigning/entry-default-signed-so.hap");
    }
    static void TearDownTestCase()
    {
    }
    void SetUp() {};
    void TearDown() {};
};

/**
 * @tc.name: GenerateMerkleTree
 * @tc.desc: Test function of FsVerityGenerator::GenerateMerkleTree() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityGeneratorTest, GenerateMerkleTree, testing::ext::TestSize.Level1)
{
    const FsVerityHashAlgorithm FS_SHA256(1, "SHA-256", 256 / 8);
    std::ifstream inputStream("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    FsVerityGenerator fsVerityGenerator;

    MerkleTree* merkleTree = fsVerityGenerator.GenerateMerkleTree(inputStream, 69632, FS_SHA256);

    EXPECT_NE(merkleTree, nullptr);
}

/**
 * @tc.name: GenerateFsVerityDigest
 * @tc.desc: Test function of FsVerityGenerator::GenerateFsVerityDigest() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityGeneratorTest, GenerateFsVerityDigest, testing::ext::TestSize.Level1)
{
    std::ifstream inputStream("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    FsVerityGenerator fsVerityGenerator;

    bool flag = fsVerityGenerator.GenerateFsVerityDigest(inputStream, 69632, 1400832);

    EXPECT_NE(flag, -1);
}

/**
 * @tc.name: GetFsVerityDigest
 * @tc.desc: Test function of FsVerityGenerator::GetFsVerityDigest() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityGeneratorTest, GetFsVerityDigest, testing::ext::TestSize.Level1)
{
    std::ifstream inputStream("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    FsVerityGenerator fsVerityGenerator;

    fsVerityGenerator.GenerateFsVerityDigest(inputStream, 69632, 1400832);
    std::vector<int8_t> digest = fsVerityGenerator.GetFsVerityDigest();

    EXPECT_NE(digest.size(), -1);
}

/**
 * @tc.name: GetTreeBytes
 * @tc.desc: Test function of FsVerityGenerator::GetTreeBytes() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityGeneratorTest, GetTreeBytes, testing::ext::TestSize.Level1)
{
    std::ifstream inputStream("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    FsVerityGenerator fsVerityGenerator;

    fsVerityGenerator.GenerateFsVerityDigest(inputStream, 69632, 1400832);
    std::vector<int8_t> treeBytes = fsVerityGenerator.GetTreeBytes();

    EXPECT_NE(treeBytes.size(), -1);
}

/**
 * @tc.name: GetRootHash
 * @tc.desc: Test function of FsVerityGenerator::GetRootHash() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityGeneratorTest, GetRootHash, testing::ext::TestSize.Level1)
{
    std::ifstream inputStream("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    FsVerityGenerator fsVerityGenerator;

    fsVerityGenerator.GenerateFsVerityDigest(inputStream, 69632, 1400832);
    std::vector<int8_t> rootHash = fsVerityGenerator.GetRootHash();

    EXPECT_NE(rootHash.size(), -1);
}

/**
 * @tc.name: GetSaltSize
 * @tc.desc: Test function of FsVerityGenerator::GetSaltSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityGeneratorTest, GetSaltSize, testing::ext::TestSize.Level1)
{
    std::ifstream inputStream("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    FsVerityGenerator fsVerityGenerator;

    fsVerityGenerator.GenerateFsVerityDigest(inputStream, 69632, 1400832);
    int saltSize = fsVerityGenerator.GetSaltSize();

    EXPECT_EQ(saltSize, 0);
}

/**
 * @tc.name: GetFsVerityHashAlgorithm
 * @tc.desc: Test function of FsVerityGenerator::GetFsVerityHashAlgorithm() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityGeneratorTest, GetFsVerityHashAlgorithm, testing::ext::TestSize.Level1)
{
    std::ifstream inputStream("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    FsVerityGenerator fsVerityGenerator;

    fsVerityGenerator.GenerateFsVerityDigest(inputStream, 69632, 1400832);
    uint8_t algorithm = fsVerityGenerator.GetFsVerityHashAlgorithm();

    EXPECT_NE(algorithm, 0);
}

/**
 * @tc.name: GetLog2BlockSize
 * @tc.desc: Test function of FsVerityGenerator::GetLog2BlockSize() interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FsVerityGeneratorTest, GetLog2BlockSize, testing::ext::TestSize.Level1)
{
    std::ifstream inputStream("./codeSigning/entry-default-signed-so.hap", std::ios::binary);
    FsVerityGenerator fsVerityGenerator;

    fsVerityGenerator.GenerateFsVerityDigest(inputStream, 69632, 1400832);
    uint8_t blockSize = fsVerityGenerator.GetLog2BlockSize();

    EXPECT_NE(blockSize, 0);
}
} // namespace SignatureTools
} // namespace OHOS