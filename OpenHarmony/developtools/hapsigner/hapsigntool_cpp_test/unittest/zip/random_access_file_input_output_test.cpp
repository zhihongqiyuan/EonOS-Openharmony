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

#include <unistd.h>
#include <memory>
#include <string>
#include <utility>
#include <gtest/gtest.h>

#include "data_source.h"
#include "hap_signer_block_utils.h"
#include "random_access_file.h"
#include "random_access_file_input.h"
#include "random_access_file_output.h"

namespace OHOS {
namespace SignatureTools {
const std::string UNSIGNED_HAP_PATH = "./zip/unsigned_random_access_file.hap";
const std::string SIGNED_HAP_PATH = "./zip/signed_random_access_file.hap";
class RandomAccessFileInputOutputTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void RandomAccessFileInputOutputTest::SetUpTestCase(void)
{
    (void)rename("./zip/unsigned_random_access_file.txt", UNSIGNED_HAP_PATH.c_str());
    (void)rename("./zip/signed_random_access_file.txt", SIGNED_HAP_PATH.c_str());
    sync();
}

void RandomAccessFileInputOutputTest::TearDownTestCase(void)
{
}

void RandomAccessFileInputOutputTest::SetUp()
{
}

void RandomAccessFileInputOutputTest::TearDown()
{
}

/**
 * @tc.name: Test Init Function
 * @tc.desc: Test function of RandomAccessFile::Init() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, InitTest001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    bool res = outputHap->Init(UNSIGNED_HAP_PATH);
    EXPECT_EQ(res, true);
}

/**
 * @tc.name: Test Init Function
 * @tc.desc: Test function of RandomAccessFile::Init() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, InitTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    /*
     * @tc.steps: step1. test Init function
     * @tc.expected: step1. make the random access file is not exist, the return will be false.
     */
    bool res = outputHap->Init("");
    EXPECT_EQ(res, false);
}

/**
 * @tc.name: Test WriteToFile Function
 * @tc.desc: Test function of RandomAccessFile::WriteToFile() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, WriteToFileTest001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    bool res = outputHap->Init(UNSIGNED_HAP_PATH);
    EXPECT_EQ(res, true);
    ByteBuffer buffer(1);
    buffer.PutByte(1);
    EXPECT_EQ(outputHap->WriteToFile(buffer, 0, 1) > 0, true);
}

/**
 * @tc.name: Test WriteToFile Function
 * @tc.desc: Test function of RandomAccessFile::WriteToFile() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, WriteToFileTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    bool res = outputHap->Init(UNSIGNED_HAP_PATH);
    EXPECT_EQ(res, true);
    /*
     * @tc.steps: step1. test WriteToFile function
     * @tc.expected: step1. make the ByteBuffer's capacity is 0, the return will be -1.
     */
    ByteBuffer buffer;
    EXPECT_EQ(outputHap->WriteToFile(buffer, 0, 0), -1);
}

/**
 * @tc.name: Test WriteToFile Function
 * @tc.desc: Test function of RandomAccessFile::WriteToFile() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, WriteToFileTest003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    bool res = outputHap->Init(UNSIGNED_HAP_PATH);
    EXPECT_EQ(res, true);
    ByteBuffer buffer(1);
    buffer.PutByte(1);
    /*
     * @tc.steps: step1. test WriteToFile function
     * @tc.expected: step1. make the position is -1, the return will be READ_OFFSET_OUT_OF_RANGE.
     */
    EXPECT_EQ(outputHap->WriteToFile(buffer, -1, 0), READ_OFFSET_OUT_OF_RANGE);
}

/**
 * @tc.name: Test ReadFileFullyFromOffset Function
 * @tc.desc: Test function of RandomAccessFile::ReadFileFullyFromOffset() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, ReadFileFullyFromOffsetTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    ASSERT_TRUE(outputHap->Init(UNSIGNED_HAP_PATH));
    std::string buf(1, 0);
    EXPECT_GT(outputHap->ReadFileFullyFromOffset(buf.data(), 0, 1), 0);
}

/**
 * @tc.name: Test RandomAccessFileInput Function
 * @tc.desc: Test function of RandomAccessFileInput::RandomAccessFileInput() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileInputTest001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(SIGNED_HAP_PATH), true);
    std::shared_ptr<ZipDataInput> outputHapIn = std::make_shared<RandomAccessFileInput>(*outputHap, 1, 1);
    EXPECT_EQ(outputHapIn != nullptr, true);
}

/**
 * @tc.name: Test RandomAccessFileInput Function
 * @tc.desc: Test function of RandomAccessFileInput::RandomAccessFileInput() interface.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileInputTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(SIGNED_HAP_PATH), true);
    /*
     * @tc.steps: step1. test RandomAccessFileInput Constructor.
     * @tc.expected: step1. make the size is -1, the operation will be return.
     */
    std::shared_ptr<ZipDataInput> outputHapIn = std::make_shared<RandomAccessFileInput>(*outputHap, 1, -1);
    EXPECT_EQ(outputHapIn != nullptr, true);
}

/**
 * @tc.name: Test RandomAccessFileInput Function
 * @tc.desc: Test function of RandomAccessFileInput::RandomAccessFileInput() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileInputTest003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(SIGNED_HAP_PATH), true);
    /*
     * @tc.steps: step1. test RandomAccessFileInput Constructor.
     * @tc.expected: step1. make the offset is -1, the operation will be return.
     */
    std::shared_ptr<ZipDataInput> outputHapIn = std::make_shared<RandomAccessFileInput>(*outputHap, -1, 1);
    EXPECT_EQ(outputHapIn != nullptr, true);
}

/**
 * @tc.name: Test Slice Function
 * @tc.desc: Test function of RandomAccessFileInput::Slice() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileInputSliceTest001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(SIGNED_HAP_PATH), true);
    std::shared_ptr<ZipDataInput> outputHapIn = std::make_shared<RandomAccessFileInput>(*outputHap);
    std::pair<ByteBuffer, int64_t> eocdPair;
    EXPECT_EQ(HapSignerBlockUtils::FindEocdInHap(*outputHap, eocdPair), true);
    int64_t centralDirectoryOffset;
    EXPECT_EQ(HapSignerBlockUtils::GetCentralDirectoryOffset(eocdPair.first, eocdPair.second, centralDirectoryOffset),
              true);
    DataSource* beforeCentralDir = outputHapIn->Slice(0, centralDirectoryOffset);
    EXPECT_EQ(beforeCentralDir != nullptr, true);
}

/**
 * @tc.name: Test Slice Function
 * @tc.desc: Test function of RandomAccessFileInput::Slice() interface
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileInputSliceTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    ASSERT_TRUE(outputHap->Init(SIGNED_HAP_PATH));
    auto outputHapIn1 = std::make_shared<RandomAccessFileInput>(*outputHap, 0, 10);
    // step1: make the offset is less than 0
    ASSERT_EQ(outputHapIn1->Slice(-1, 10), nullptr);

    auto outputHapIn2 = std::make_shared<RandomAccessFileInput>(*outputHap, 0, 10);
    // step2: make the size is less than 0
    ASSERT_EQ(outputHapIn2->Slice(0, -1), nullptr);

    auto outputHapIn3 = std::make_shared<RandomAccessFileInput>(*outputHap, 0, 10);
    // step3: make the offset is greater than RandomAccessFile's length
    ASSERT_EQ(outputHapIn3->Slice(20, 1), nullptr);

    auto outputHapIn4 = std::make_shared<RandomAccessFileInput>(*outputHap, 0, 10);
    // step4: make the offset plus the size is greater than RandomAccessFile's length
    ASSERT_EQ(outputHapIn4->Slice(0, 20), nullptr);

    auto outputHapIn5 = std::make_shared<RandomAccessFileInput>(*outputHap, 0, 10);
    // step5: make the offset is zero and the offset is RandomAccessFile's length
    ASSERT_NE(outputHapIn5->Slice(0, 10), nullptr);

    auto outputHapIn6 = std::make_shared<RandomAccessFileInput>(*outputHap, 0, 10);
    // step6: make the offset plus the offset is overflow
    ASSERT_NE(outputHapIn6->Slice(1, LLONG_MAX), nullptr);

    auto outputHapIn7 = std::make_shared<RandomAccessFileInput>(*outputHap, 0, 10);
    // step7: make the offset is not equal to zero and the offset is not equal to RandomAccessFile's length
    ASSERT_NE(outputHapIn7->Slice(1, 5), nullptr);
}

/**
 * @tc.name: Test CreateByteBuffer Function
 * @tc.desc: Test function of RandomAccessFileInput::CreateByteBuffer() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileInputCreateByteBufferTest001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(SIGNED_HAP_PATH), true);
    std::shared_ptr<ZipDataInput> outputHapIn = std::make_shared<RandomAccessFileInput>(*outputHap);
    std::pair<ByteBuffer, int64_t> eocdPair;
    EXPECT_EQ(HapSignerBlockUtils::FindEocdInHap(*outputHap, eocdPair), true);
    int64_t centralDirectoryOffset;
    EXPECT_EQ(HapSignerBlockUtils::GetCentralDirectoryOffset(eocdPair.first, eocdPair.second, centralDirectoryOffset),
              true);
    DataSource* beforeCentralDir = outputHapIn->Slice(0, centralDirectoryOffset);
    EXPECT_EQ(beforeCentralDir != nullptr, true);
    long centralDirectorySize;
    EXPECT_EQ(HapSignerBlockUtils::GetCentralDirectorySize(eocdPair.first, centralDirectorySize), true);
    ByteBuffer centralDirBuffer = outputHapIn->CreateByteBuffer(centralDirectoryOffset, centralDirectorySize);
    EXPECT_EQ(centralDirBuffer.GetCapacity() > 0, true);
}

/**
 * @tc.name: Test CreateByteBuffer Function
 * @tc.desc: Test function of RandomAccessFileInput::CreateByteBuffer() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileInputCreateByteBufferTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    ASSERT_TRUE(outputHap->Init(SIGNED_HAP_PATH));
    std::shared_ptr<ZipDataInput> outputHapIn = std::make_shared<RandomAccessFileInput>(*outputHap);
    std::pair<ByteBuffer, int64_t> eocdPair;
    ASSERT_TRUE(HapSignerBlockUtils::FindEocdInHap(*outputHap, eocdPair));
    /*
     * @tc.steps: step1. test CreateByteBuffer function.
     * @tc.expected: step1. make the offset is -1, the CheckBoundValid function return will be false,
     * and ByteBuffer's capacity is 0
     */
    ByteBuffer centralDirBuffer1 = outputHapIn->CreateByteBuffer(-1, 1);
    ASSERT_EQ(centralDirBuffer1.GetCapacity(), 0);
    /*
     * @tc.steps: step2. test CreateByteBuffer function.
     * @tc.expected: step2. make the size is -1, the CopyTo function return will be false, and ByteBuffer's
     * capacity is 0
     */
    ByteBuffer centralDirBuffer2 = outputHapIn->CreateByteBuffer(1, 0);
    ASSERT_EQ(centralDirBuffer2.GetCapacity(), 0);
}

/**
 * @tc.name: Test Write ByteBuffer Function
 * @tc.desc: Test function of RandomAccessFileOutput::Write() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileOutputWriteTest001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(UNSIGNED_HAP_PATH), true);
    std::pair<ByteBuffer, int64_t> eocdPair;
    EXPECT_EQ(HapSignerBlockUtils::FindEocdInHap(*outputHap, eocdPair), true);
    int64_t centralDirectoryOffset;
    EXPECT_EQ(HapSignerBlockUtils::GetCentralDirectoryOffset(eocdPair.first, eocdPair.second, centralDirectoryOffset),
              true);
    std::shared_ptr<RandomAccessFileOutput> outputHapOut =
        std::make_shared<RandomAccessFileOutput>(outputHap.get(), centralDirectoryOffset);
    ByteBuffer signingBlock(1);
    signingBlock.PutByte(1);
    EXPECT_EQ(outputHapOut->Write(signingBlock), true);
}

/**
 * @tc.name: Test Write ByteBuffer Function
 * @tc.desc: Test function of RandomAccessFileOutput::Write() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileOutputWriteTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(UNSIGNED_HAP_PATH), true);
    std::pair<ByteBuffer, int64_t> eocdPair;
    EXPECT_EQ(HapSignerBlockUtils::FindEocdInHap(*outputHap, eocdPair), true);
    int64_t centralDirectoryOffset;
    EXPECT_EQ(HapSignerBlockUtils::GetCentralDirectoryOffset(eocdPair.first, eocdPair.second, centralDirectoryOffset),
              true);
    std::shared_ptr<RandomAccessFileOutput> outputHapOut =
        std::make_shared<RandomAccessFileOutput>(outputHap.get(), centralDirectoryOffset);
    /*
     * @tc.steps: step1. test Write function.
     * @tc.expected: step1. make the ByteBuffer is empty, the return will be false.
     */
    ByteBuffer signingBlock;
    EXPECT_EQ(outputHapOut->Write(signingBlock), false);
}

/**
 * @tc.name: Test Write ByteBuffer Function
 * @tc.desc: Test function of RandomAccessFileOutput() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileOutputTest001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(UNSIGNED_HAP_PATH), true);
    std::shared_ptr<RandomAccessFileOutput> outputHapOut =
        std::make_shared<RandomAccessFileOutput>(outputHap.get());
    EXPECT_EQ(outputHapOut != nullptr, true);
}

/**
 * @tc.name: Test Write ByteBuffer Function
 * @tc.desc: Test function of RandomAccessFileOutput() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(RandomAccessFileInputOutputTest, RandomAccessFileOutputTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(UNSIGNED_HAP_PATH), true);
    /*
     * @tc.steps: step1. test RandomAccessFileOutput Constructor.
     * @tc.expected: step1. make the startPosition is -1, the operation will be return.
     */
    std::shared_ptr<RandomAccessFileOutput> outputHapOut =
        std::make_shared<RandomAccessFileOutput>(outputHap.get(), -1);
    EXPECT_EQ(outputHapOut != nullptr, true);
}
} // namespace SignatureTools
} // namespace OHOS