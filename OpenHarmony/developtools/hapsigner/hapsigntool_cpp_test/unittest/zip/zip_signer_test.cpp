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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <filesystem>
#include <gtest/gtest.h>

#include "file_utils.h"
#include "hap_signer_block_utils.h"
#include "zip_entry.h"
#include "zip_signer.h"
#include "zip_utils.h"

namespace OHOS {
namespace SignatureTools {
const std::string UNSIGNED_HAP_PATH = "./zip/unsigned.hap";
const std::string SIGNED_HAP_PATH = "./zip/signed.hap";
const std::string EMPTY_HAP_PATH = "./zip/empty.hap";
const std::string DATA_DESCRIPTOR_HAP_PATH = "./zip/data_descriptor_hap.hap";
const std::string EOCD_ONLY_HAP_PATH = "./zip/eocd_only.hap";
const std::string DUMMY_HAP_PATH = "./zip/dummy.hap";
const std::string ZIP_ENTRIES_WRONG_HAP_V1_PATH = "./zip/zip_entries_wrong_v1.hap";
const std::string ZIP_ENTRIES_WRONG_HAP_V2_PATH = "./zip/zip_entries_wrong_v2.hap";
const std::string ZIP_ENTRIES_WRONG_HAP_V3_PATH = "./zip/zip_entries_wrong_v3.hap";
const std::string ZIP_ENTRIES_WRONG_HAP_V4_PATH = "./zip/zip_entries_wrong_v4.hap";
const std::string ZIP_ENTRY_DATA_WRONG_HAP_PATH = "./zip/zip_entry_data_wrong.hap";
const std::string CD_ONLY_HAP_V1_PATH = "./zip/cd_only_v1.hap";
const std::string CD_ONLY_HAP_V2_PATH = "./zip/cd_only_v2.hap";
const std::string OUTPUT_HAP_PATH = "./zip/output.hap";
const std::string ZERO_HAP_PATH = "./zip/zero.hap";
const static int ALIGNMENT = 4;
class ZipSignerTest : public testing::Test {
public:
    static void SetUpTestCase(void);
    static void TearDownTestCase(void);
    void SetUp();
    void TearDown();
};

void ZipSignerTest::SetUpTestCase(void)
{
    (void)rename("./zip/cd_only_v1.txt", CD_ONLY_HAP_V1_PATH.c_str());
    (void)rename("./zip/cd_only_v2.txt", CD_ONLY_HAP_V2_PATH.c_str());
    (void)rename("./zip/data_descriptor_hap.txt", DATA_DESCRIPTOR_HAP_PATH.c_str());
    (void)rename("./zip/unsigned.txt", UNSIGNED_HAP_PATH.c_str());
    (void)rename("./zip/signed.txt", SIGNED_HAP_PATH.c_str());
    (void)rename("./zip/zip_entries_wrong_v1.txt", ZIP_ENTRIES_WRONG_HAP_V1_PATH.c_str());
    (void)rename("./zip/zip_entries_wrong_v2.txt", ZIP_ENTRIES_WRONG_HAP_V2_PATH.c_str());
    (void)rename("./zip/zip_entries_wrong_v3.txt", ZIP_ENTRIES_WRONG_HAP_V3_PATH.c_str());
    (void)rename("./zip/zip_entries_wrong_v4.txt", ZIP_ENTRIES_WRONG_HAP_V4_PATH.c_str());
    (void)rename("./zip/zip_entry_data_wrong.txt", ZIP_ENTRY_DATA_WRONG_HAP_PATH.c_str());
    sync();
}

void ZipSignerTest::TearDownTestCase(void)
{
}

void ZipSignerTest::SetUp()
{
}

void ZipSignerTest::TearDown()
{
}

/**
 * @tc.name: Test ZipSigner Full process
 * @tc.desc: Test function of ZipSigner interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, FullProcessTest001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test ZipSigner full process function
     * @tc.expected: step1. use the unsigned hap file, the return will be true.
     */
    std::ifstream rawInput(UNSIGNED_HAP_PATH, std::ios::binary);
    std::ofstream rawOutput(OUTPUT_HAP_PATH, std::ios::binary | std::ios::trunc | std::ios::out);
    auto zip1 = std::make_shared<ZipSigner>();
    ASSERT_TRUE(zip1->Init(rawInput));
    zip1->Alignment(ALIGNMENT);
    zip1->RemoveSignBlock();
    ASSERT_TRUE(zip1->ToFile(rawInput, rawOutput));
    rawOutput.close();
    rawInput.close();

    /*
     * @tc.steps: step2. test ZipSigner full process function
     * @tc.expected: step2. use the signed hap file, the return will be true.
     */
    std::ifstream wholeInput(SIGNED_HAP_PATH, std::ios::binary);
    std::ofstream wholeOutput(OUTPUT_HAP_PATH, std::ios::binary | std::ios::trunc | std::ios::out);
    auto zip2 = std::make_shared<ZipSigner>();
    ASSERT_TRUE(zip2->Init(wholeInput));
    zip2->Alignment(ALIGNMENT);
    zip2->RemoveSignBlock();
    ASSERT_TRUE(zip2->ToFile(wholeInput, wholeOutput));
    wholeOutput.close();
    wholeInput.close();

    /*
     * @tc.steps: step3. test ZipSigner full process function
     * @tc.expected: step3. use the hap file with data descriptor, the return will be true.
     */
    std::ifstream dataDescInput(DATA_DESCRIPTOR_HAP_PATH, std::ios::binary);
    std::ofstream dataDescOutput(OUTPUT_HAP_PATH, std::ios::binary | std::ios::trunc | std::ios::out);
    auto zip3 = std::make_shared<ZipSigner>();
    ASSERT_TRUE(zip3->Init(dataDescInput));
    zip3->Alignment(ALIGNMENT);
    zip3->RemoveSignBlock();
    ASSERT_TRUE(zip3->ToFile(dataDescInput, dataDescOutput));
    dataDescOutput.close();
    dataDescInput.close();
}

/**
 * @tc.name: Test ZipSigner Init Function
 * @tc.desc: Test function of ZipSigner interface for SUCCESS and FAIL
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ZipSignerInitTest001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test Init function
     * @tc.expected: step1. make the hap file is not exist, the return will be false.
     */
    auto zip1 = std::make_shared<ZipSigner>();
    std::ifstream dummyInput(DUMMY_HAP_PATH, std::ios::binary);
    ASSERT_FALSE(zip1->Init(dummyInput));
    /*
     * @tc.steps: step2. test Init function
     * @tc.expected: step2. make the hap file is empty, the return will be false.
     */
    std::ofstream emptyOuptut(EMPTY_HAP_PATH, std::ios::binary | std::ios::trunc | std::ios::out);
    emptyOuptut << "";
    emptyOuptut.close();
    auto zip2 = std::make_shared<ZipSigner>();
    std::ifstream emptyInput(EMPTY_HAP_PATH, std::ios::binary);
    ASSERT_FALSE(zip2->Init(emptyInput));
    emptyInput.close();

    /*
     * @tc.steps: step3. test Init function
     * @tc.expected: step3. make the hap file is ONLY have the eocd block, the return will be false.
     */
    std::ofstream eocdOutput(EOCD_ONLY_HAP_PATH, std::ios::binary | std::ios::trunc | std::ios::out);
    char eocd[] = {0x50, 0x4b, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00,
                    0x46, 0x02, 0x00, 0x00, 0x00, 0x00};
    eocdOutput.write(eocd, sizeof(eocd) / sizeof(eocd[0]));
    eocdOutput.close();
    auto zip3 = std::make_shared<ZipSigner>();
    std::ifstream eocdInput(EOCD_ONLY_HAP_PATH, std::ios::binary);
    ASSERT_FALSE(zip3->Init(eocdInput));
    eocdInput.close();
}

/**
 * @tc.name: Test GetZipEntries Function
 * @tc.desc: Test function of ZipSigner::GetZipEntries() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, GetZipEntriesTest001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test GetZipEntries function
     * @tc.expected: step1. make the cd offset int front of entry end, return will be false.
     */
    std::ifstream inputFile(ZIP_ENTRIES_WRONG_HAP_V3_PATH, std::ios::binary);
    auto zip = std::make_shared<ZipSigner>();
    ASSERT_FALSE(zip->Init(inputFile));
}

/**
 * @tc.name: Test Alignment Function
 * @tc.desc: Test function of ZipEntry::Alignment() interface.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, AlignmentTest001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test Alignment function
     * @tc.expected: step1. make the alignment is equal to 0, return will be equal to -1.
     */
    std::ifstream inputFile(SIGNED_HAP_PATH, std::ios::binary);
    auto zip1 = std::make_shared<ZipSigner>();
    ASSERT_TRUE(zip1->Init(inputFile));
    auto zipEntries1 = zip1->GetZipEntries();
    int zeroAlignment = 0;
    for (const auto& zipEntry : zipEntries1) {
        ASSERT_EQ(zipEntry->Alignment(zeroAlignment), -1);
    }

    /*
     * @tc.steps: step2. test Alignment function
     * @tc.expected: step2. make the alignment is equal to 4, return will be equal to 0.
     */
    auto zip2 = std::make_shared<ZipSigner>();
    ASSERT_TRUE(zip2->Init(inputFile));
    auto zipEntries2 = zip2->GetZipEntries();
    for (const auto& zipEntry : zipEntries2) {
        ASSERT_EQ(zipEntry->Alignment(ALIGNMENT), 0);
    }

    /*
     * @tc.steps: step3. test Alignment function
     * @tc.expected: step3. make the alignment is equal to 4, return will be equal to 1.
     */
    std::ifstream zipEntriesInput(ZIP_ENTRIES_WRONG_HAP_V4_PATH, std::ios::binary);
    auto zip3 = std::make_shared<ZipSigner>();
    ASSERT_TRUE(zip3->Init(zipEntriesInput));
    auto zipEntries3 = zip3->GetZipEntries();
    for (const auto& zipEntry : zipEntries3) {
        ASSERT_EQ(zipEntry->Alignment(ALIGNMENT), 1);
    }

    zipEntriesInput.close();
    inputFile.close();
}

/**
 * @tc.name: Test GetCDOffset Function
 * @tc.desc: Test function of ZipSigner::GetCDOffset() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, GetCDOffsetTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool res = zip->Init(inputFile);
    uint64_t cdOffset = zip->GetCDOffset();
    EXPECT_EQ(res && cdOffset != 0, true);
}

/**
 * @tc.name: Test GetEOCDOffset Function
 * @tc.desc: Test function of ZipSigner::GetEOCDOffset() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, GetEOCDOffsetTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool res = zip->Init(inputFile);
    uint64_t eocdOffset = zip->GetEOCDOffset();
    EXPECT_EQ(res && eocdOffset != 0, true);
}

/**
 * @tc.name: Test ToFile Function
 * @tc.desc: Test function of ZipSigner::ToFile() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ToFileTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::ofstream outputFile(OUTPUT_HAP_PATH, std::ios::binary | std::ios::trunc);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool initRes = zip->Init(inputFile);
    bool toFileRes = zip->ToFile(inputFile, outputFile);
    EXPECT_EQ(initRes && toFileRes, true);
}

/**
 * @tc.name: Test ToFile Function
 * @tc.desc: Test function of ZipSigner::ToFile() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ToFileTest002, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    /*
     * @tc.steps: step1. test ToFile function
     * @tc.expected: step1. make the output file stream is bad, return will be false.
     */
    std::ofstream outputFile("", std::ios::binary | std::ios::trunc);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool initRes = zip->Init(inputFile);
    bool toFileRes = zip->ToFile(inputFile, outputFile);
    EXPECT_EQ(initRes && !toFileRes, true);
}

/**
 * @tc.name: Test ToFile Function
 * @tc.desc: Test function of ZipSigner::ToFile() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ToFileTest003, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::ofstream outputFile("", std::ios::binary | std::ios::trunc);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool initRes = zip->Init(inputFile);
    /*
     * @tc.steps: step1. test ToFile function
     * @tc.expected: step1. make the input file stream is bad, return will be false.
     */
    std::ifstream bad("", std::ios::binary);

    bool toFileRes = zip->ToFile(bad, outputFile);
    EXPECT_EQ(initRes && !toFileRes, true);
}

/**
 * @tc.name: Test ToFile Function
 * @tc.desc: Test function of ZipSigner::ToFile() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ToFileTest004, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::ofstream outputFile(OUTPUT_HAP_PATH, std::ios::binary | std::ios::trunc);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    ASSERT_TRUE(zip->Init(inputFile));
    ASSERT_TRUE(zip->ToFile(inputFile, outputFile));
}

/*
 * @tc.name: Alignment_Test_001
 * @tc.desc: Test function of ZipEntry::Alignment() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ZipEntryAlignmentTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool res = zip->Init(inputFile);
    std::vector<ZipEntry*> zipEntries = zip->GetZipEntries();
    int alignment = 4;
    for (auto& zipEntry : zipEntries) {
        int add = zipEntry->Alignment(alignment);
        EXPECT_EQ(res && add > 0, true);
    }
}

/**
 * @tc.name: Test GetEOCDByBytes Function
 * @tc.desc: Test function of EndOfCentralDirectory::GetEOCDByBytes() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, EndOfCentralDirectoryGetEOCDByBytesTest001, testing::ext::TestSize.Level1)
{
    std::ifstream input(UNSIGNED_HAP_PATH, std::ios::binary);
    input.seekg(0, std::ios::end);
    uint64_t fileSize = input.tellg();
    input.seekg(0, std::ios::beg);

    int eocdLength = EndOfCentralDirectory::EOCD_LENGTH;
    uint64_t eocdOffset = reinterpret_cast<uint64_t>(fileSize - eocdLength);

    std::string retStr;
    int res = FileUtils::ReadFileByOffsetAndLength(input, eocdOffset, eocdLength, retStr);
    std::optional<EndOfCentralDirectory*> eocdByBytes = EndOfCentralDirectory::GetEOCDByBytes(retStr);
    EXPECT_EQ(res == 0 && eocdByBytes != std::nullopt, true);
}

/**
 * @tc.name: Test GetEOCDByBytes Function
 * @tc.desc: Test function of EndOfCentralDirectory::GetEOCDByBytes() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, EndOfCentralDirectoryGetEOCDByBytesTest002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test GetEOCDByBytes function
     * @tc.expected: step1. make the eocd bytes is empty, the return will be nullopt.
     */
    std::string str;
    std::optional<EndOfCentralDirectory*> eocdByBytes = EndOfCentralDirectory::GetEOCDByBytes(str);
    EXPECT_EQ(eocdByBytes == std::nullopt, true);
}

/**
 * @tc.name: Test GetEOCDByBytes Function
 * @tc.desc: Test function of EndOfCentralDirectory::GetEOCDByBytes() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, EndOfCentralDirectoryGetEOCDByBytesTest003, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test GetEOCDByBytes function
     * @tc.expected: step1. make the eocd bytes is all of zero, the return will be nullopt.
     */
    std::string bytes(22, 0);
    std::optional<EndOfCentralDirectory*> eocdByBytes = EndOfCentralDirectory::GetEOCDByBytes(bytes);
    EXPECT_EQ(eocdByBytes == std::nullopt, true);
}

/**
 * @tc.name: Test GetEOCDByBytes Function
 * @tc.desc: Test function of EndOfCentralDirectory::GetEOCDByBytes() interface for FAIL with eocd length is wrong
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, EndOfCentralDirectoryGetEOCDByBytesTest004, testing::ext::TestSize.Level1)
{
    std::string bytes{
        80, 75, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    std::optional<EndOfCentralDirectory*> eocdByBytes = EndOfCentralDirectory::GetEOCDByBytes(bytes);
    EXPECT_EQ(eocdByBytes == std::nullopt, true);
}

/**
 * @tc.name: Test GetEOCDByBytes Function
 * @tc.desc: Test function of EndOfCentralDirectory::GetEOCDByBytes() interface for SUCCESS with comment.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, EndOfCentralDirectoryGetEOCDByBytesTest005, testing::ext::TestSize.Level1)
{
    std::string bytes{
        80, 75, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1
    };
    std::optional<EndOfCentralDirectory*> eocdByBytes = EndOfCentralDirectory::GetEOCDByBytes(bytes);
    EXPECT_EQ(eocdByBytes != std::nullopt, true);
    std::string eocdBytes = eocdByBytes.value()->ToBytes();
    EXPECT_EQ(eocdBytes.size() > 0, true);
}

/**
 * @tc.name: Test GetEOCDByBytes Function
 * @tc.desc: Test function of EndOfCentralDirectory::SetComment() interface.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, EndOfCentralDirectorySetCommentTest001, testing::ext::TestSize.Level1)
{
    std::string bytes{
        80, 75, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    std::optional<EndOfCentralDirectory*> eocdByBytes = EndOfCentralDirectory::GetEOCDByBytes(bytes);
    std::string comment{1};
    eocdByBytes.value()->SetComment(comment);
    EXPECT_EQ(eocdByBytes != std::nullopt, true);
}

/**
 * @tc.name: Test ToBytes Function
 * @tc.desc: Test function of EndOfCentralDirectory::ToBytes() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, EndOfCentralDirectoryToBytesTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool initRes = zip->Init(inputFile);
    EndOfCentralDirectory* eocd = zip->GetEndOfCentralDirectory();
    std::string eocdBytes = eocd->ToBytes();
    int signature = eocd->GetSIGNATURE();
    int diskNum = eocd->GetDiskNum();
    std::string comment = eocd->GetComment();
    EXPECT_EQ(initRes && eocd && eocdBytes.size() > 0 && signature != -1 && comment.size() == 0 && diskNum != -1,
              true);
}

/**
 * @tc.name: Test EndOfCentralDirectory Class
 * @tc.desc: Test function of EndOfCentralDirectory interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, EndOfCentralDirectoryTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool initRes = zip->Init(inputFile);
    EndOfCentralDirectory* eocd = zip->GetEndOfCentralDirectory();
    int eocdLength = eocd->GetEocdLength();
    int cdStartDiskNum = eocd->GetcDStartDiskNum();
    int diskCDNum = eocd->GetThisDiskCDNum();
    int eocdLen = eocd->GetLength();
    EXPECT_EQ(initRes && eocd && eocdLength > 0 && cdStartDiskNum != -1 && diskCDNum != -1 && eocdLen != -1,
              true);
}

/**
 * @tc.name: Test SetCentralDirectoryOffset Function
 * @tc.desc: Test function of ZipUtils::SetCentralDirectoryOffset() interface for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ZipUtilsSetCentralDirectoryOffsetTest001, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(UNSIGNED_HAP_PATH), true);
    std::pair<ByteBuffer, int64_t> eocdPair;
    EXPECT_EQ(HapSignerBlockUtils::FindEocdInHap(*outputHap, eocdPair), true);
    int64_t centralDirectoryOffset;
    EXPECT_EQ(HapSignerBlockUtils::GetCentralDirectoryOffset(eocdPair.first, eocdPair.second,
                                                             centralDirectoryOffset),
              true);
    int64_t newCentralDirOffset = centralDirectoryOffset + 10;
    eocdPair.first.SetPosition(0);
    EXPECT_EQ(ZipUtils::SetCentralDirectoryOffset(eocdPair.first, newCentralDirOffset), true);
}

/**
 * @tc.name: Test SetCentralDirectoryOffset Function
 * @tc.desc: Test function of ZipUtils::SetCentralDirectoryOffset() interface.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ZipUtilsSetCentralDirectoryOffsetTest002, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    EXPECT_EQ(outputHap->Init(UNSIGNED_HAP_PATH), true);
    std::pair<ByteBuffer, int64_t> eocdPair;
    EXPECT_EQ(HapSignerBlockUtils::FindEocdInHap(*outputHap, eocdPair), true);
    eocdPair.first.SetPosition(0);
    /*
     * @tc.steps: step1. test SetCentralDirectoryOffset function
     * @tc.expected: step1. make the central directory offset is -1, the return will be false.
     */
    EXPECT_EQ(ZipUtils::SetCentralDirectoryOffset(eocdPair.first, -1), false);
}

/**
 * @tc.name: Test SetCentralDirectoryOffset Function
 * @tc.desc: Test function of ZipUtils::SetCentralDirectoryOffset() interface for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ZipUtilsSetCentralDirectoryOffsetTest003, testing::ext::TestSize.Level1)
{
    std::shared_ptr<RandomAccessFile> outputHap = std::make_shared<RandomAccessFile>();
    ASSERT_TRUE(outputHap->Init(UNSIGNED_HAP_PATH));
    std::pair<ByteBuffer, int64_t> eocdPair;
    ASSERT_TRUE(HapSignerBlockUtils::FindEocdInHap(*outputHap, eocdPair));
    eocdPair.first.SetPosition(0);
    /*
     * @tc.steps: step1. test SetCentralDirectoryOffset function
     * @tc.expected: step1. make the central directory offset is 0x100000000LL, it's greater than 0xffffffffLL,
     * the return will be false.
     */
    ASSERT_FALSE(ZipUtils::SetCentralDirectoryOffset(eocdPair.first, 0x100000000LL));
}

/**
 * @tc.name: Test ZipEntryHeader Class
 * @tc.desc: Test function of ZipEntryHeader for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ZipEntryHeaderTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    auto zip = std::make_shared<ZipSigner>();
    ASSERT_TRUE(zip->Init(inputFile));

    auto zipEntries = zip->GetZipEntries();
    ASSERT_NE(zipEntries.size(), 0);

    for (const auto& zipEntry : zipEntries) {
        auto zipEntryData = zipEntry->GetZipEntryData();
        ASSERT_NE(zipEntryData, nullptr);

        int crc32 = zipEntryData->GetZipEntryHeader()->GetCrc32();
        ASSERT_NE(crc32, -1);
        short lastTime = zipEntryData->GetZipEntryHeader()->GetLastTime();
        ASSERT_NE(lastTime, -1);
        short lastData = zipEntryData->GetZipEntryHeader()->GetLastDate();
        ASSERT_NE(lastData, -1);
        int64_t compressedSize = zipEntryData->GetZipEntryHeader()->GetCompressedSize();
        ASSERT_NE(compressedSize, -1);
        int64_t unCompressedSize = zipEntryData->GetZipEntryHeader()->GetUnCompressedSize();
        ASSERT_NE(unCompressedSize, -1);
        int headerLength = zipEntryData->GetZipEntryHeader()->GetHeaderLength();
        ASSERT_NE(headerLength, -1);
        int signature = zipEntryData->GetZipEntryHeader()->GetSIGNATURE();
        ASSERT_NE(signature, -1);
        short version = zipEntryData->GetZipEntryHeader()->GetVersion();
        ASSERT_NE(version, -1);
        zipEntryData->GetZipEntryHeader()->SetFileName("");
    }
}

/**
 * @tc.name: Test ZipEntryHeader Class
 * @tc.desc: Test function ToBytes of ZipEntryHeader for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ZipEntryHeaderTest002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test ReadFileName and ReadExtra function
     * @tc.expected: step1. make the file name and extra is empty, the return will be 0.
     */
    ZipEntryHeader zipEntryHeader;
    zipEntryHeader.ReadFileName("");
    zipEntryHeader.ReadExtra("");
    std::string bytes = zipEntryHeader.ToBytes();
    ASSERT_EQ(bytes.size(), 0);
}

/**
 * @tc.name: Test DataDescriptor Class
 * @tc.desc: Test function of DataDescriptor for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, DataDescriptorTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(DATA_DESCRIPTOR_HAP_PATH, std::ios::binary);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool zipRes = zip->Init(inputFile);
    std::vector<ZipEntry*> zipEntries = zip->GetZipEntries();
    EXPECT_EQ(zipRes && zipEntries.size() > 0, true);
    for (const auto& zipEntry : zipEntries) {
        ZipEntryData* zipEntryData = zipEntry->GetZipEntryData();
        DataDescriptor* dataDescriptor = zipEntryData->GetDataDescriptor();
        if (dataDescriptor) {
            uint64_t compressedSize = dataDescriptor->GetCompressedSize();
            uint64_t unCompressedSize = dataDescriptor->GetUnCompressedSize();
            int crc32 = dataDescriptor->GetCrc32();
            int signature = dataDescriptor->GetSIGNATURE();
            int desLength = dataDescriptor->GetDesLength();
            EXPECT_EQ(zipEntryData != nullptr && dataDescriptor != nullptr && compressedSize != -1 &&
                      unCompressedSize != -1 && crc32 != -1 && signature != -1 && desLength != -1, true);
        } else {
            EXPECT_EQ(dataDescriptor == nullptr, true);
        }
    }
}

/**
 * @tc.name: Test DataDescriptor Class
 * @tc.desc: Test function of GetDataDescriptor for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, GetDataDescriptorTest001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test GetDataDescriptor and GetDataDescriptor function
     * @tc.expected: step1. make the data descriptor is error, the return will be nullptr.
     */
    std::string bytes1{1};
    ASSERT_EQ(DataDescriptor::GetDataDescriptor(bytes1), nullptr);

    std::string bytes2(16, 0);
    ASSERT_EQ(DataDescriptor::GetDataDescriptor(bytes2), nullptr);
}

/**
 * @tc.name: Test CentralDirectory Class
 * @tc.desc: Test function of CentralDirectory for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, CentralDirectoryTest001, testing::ext::TestSize.Level1)
{
    std::ifstream inputFile(UNSIGNED_HAP_PATH, std::ios::binary);
    std::shared_ptr<ZipSigner> zip = std::make_shared<ZipSigner>();
    bool zipRes = zip->Init(inputFile);
    std::vector<ZipEntry*> zipEntries = zip->GetZipEntries();
    EXPECT_EQ(zipRes && zipEntries.size() > 0, true);
    CentralDirectory* cd = zipEntries[0]->GetCentralDirectory();
    int cdLength = cd->GetCdLength();
    int signature = cd->GetSIGNATURE();
    short flag = cd->GetFlag();
    short lastTime = cd->GetLastTime();
    short lastDate = cd->GetLastDate();
    int crc32 = cd->GetCrc32();
    std::string fileName = cd->GetFileName();
    short version = cd->GetVersion();
    short versionExtra = cd->GetVersionExtra();
    int diskNumStart = cd->GetDiskNumStart();
    short internalFile = cd->GetInternalFile();
    int externalFile = cd->GetExternalFile();
    std::string comment = cd->GetComment();
    EXPECT_EQ(cd != nullptr && cdLength != -1 && signature != -1 && flag != -1 && lastTime != -1 && lastDate != -1 &&
              crc32 != -1 && fileName.size() > 0 && version != -1 && versionExtra != -1 && diskNumStart != -1 &&
              internalFile != -1 && externalFile != -1 && comment.size() == 0, true);
}

/**
 * @tc.name: Test CentralDirectory Class
 * @tc.desc: Test function of CentralDirectory for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, CentralDirectoryTest002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test GetCentralDirectory function
     * @tc.expected: step1. make the central directory is error, the return will be nullptr.
     */
    ByteBuffer bf(1);
    CentralDirectory* cd = new CentralDirectory();
    EXPECT_EQ(CentralDirectory::GetCentralDirectory(bf, cd), false);
    delete cd;
}

/**
 * @tc.name: Test CentralDirectory Class
 * @tc.desc: Test function of CentralDirectory for SUCCESS with comment.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, CentralDirectoryTest003, testing::ext::TestSize.Level1)
{
    std::string str;
    EXPECT_EQ(FileUtils::ReadFile(CD_ONLY_HAP_V1_PATH, str) == 0, true);
    ByteBuffer bf(str.c_str(), str.size());
    CentralDirectory* cd = new CentralDirectory();
    EXPECT_EQ(CentralDirectory::GetCentralDirectory(bf, cd), true);
    std::string cdBytes = cd->ToBytes();
    EXPECT_EQ(cdBytes.size() > 0, true);
}

/**
 * @tc.name: Test CentralDirectory Class
 * @tc.desc: Test function of CentralDirectory for SUCCESS without fileNameLength.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, CentralDirectoryTest004, testing::ext::TestSize.Level1)
{
    std::string str;
    EXPECT_EQ(FileUtils::ReadFile(CD_ONLY_HAP_V2_PATH, str) == 0, true);
    ByteBuffer bf(str.c_str(), str.size());
    CentralDirectory* cd = new CentralDirectory();
    EXPECT_EQ(CentralDirectory::GetCentralDirectory(bf, cd), true);
    std::string cdBytes = cd->ToBytes();
    EXPECT_EQ(cdBytes.size() > 0, true);
}

/**
 * @tc.name: Test ZipEntryHeader Class
 * @tc.desc: Test function of GetZipEntryHeader for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, GetZipEntryHeaderTest001, testing::ext::TestSize.Level1)
{
    std::string headBytes{1};
    ZipEntryHeader* entryHeader = ZipEntryHeader::GetZipEntryHeader(headBytes);
    EXPECT_EQ(entryHeader == nullptr, true);
    delete entryHeader;
}

/**
 * @tc.name: Test ZipEntryData Class
 * @tc.desc: Test function of GetZipEntry for FAIL.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, GetZipEntryTest001, testing::ext::TestSize.Level1)
{
    std::ofstream zeroOutput(ZERO_HAP_PATH, std::ios::binary | std::ios::trunc | std::ios::out);
    std::string zeros(1024, 0x00);
    zeroOutput.write(zeros.c_str(), zeros.size());
    zeroOutput.close();

    std::ifstream inputFile(ZERO_HAP_PATH, std::ios::binary);
    ZipEntryData* zipEntryData = ZipEntryData::GetZipEntry(inputFile, 0, 1024);
    EXPECT_EQ(zipEntryData == nullptr, true);
}

/**
 * @tc.name: Test ZipEntryData Class
 * @tc.desc: Test function of GetZipEntry for success.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, GetZipEntryTest002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. test ZipEntryData::ReadEntryFileNameAndExtraByOffset function
     * @tc.expected: step1. FileNameLength and ExtraLength is zero
     */
    std::ifstream inputFile(ZIP_ENTRY_DATA_WRONG_HAP_PATH, std::ios::binary);
    ZipEntryData* zipEntryData = ZipEntryData::GetZipEntry(inputFile, 0, 1024);
    ASSERT_EQ(zipEntryData != nullptr, true);
}

/**
 * @tc.name: Test ZipSigner Class
 * @tc.desc: Test ZipSigner interfaces for SUCCESS.
 * @tc.type: FUNC
 * @tc.require: SR000H63TL
 */
HWTEST_F(ZipSignerTest, ZipSignerTest001, testing::ext::TestSize.Level1)
{
    ZipSigner zip;
    std::ifstream ifs(ZIP_ENTRIES_WRONG_HAP_V2_PATH, std::ios::binary);
    ASSERT_TRUE(zip.Init(ifs));
    std::vector<ZipEntry*> zipEntries{nullptr};
    zip.SetZipEntries(zipEntries);
    zip.SetSigningOffset(1);
    int64_t offset = zip.GetSigningOffset();
    EXPECT_EQ(offset, 1);
    std::string signingBlock{0x1, 0x1, 0x1, 0x1, 0x1};
    zip.SetSigningBlock(signingBlock);
    signingBlock = zip.GetSigningBlock();
    EXPECT_NE(signingBlock.size(), 0);
    zip.SetCDOffset(1);
    zip.SetEOCDOffset(1);
    zip.SetEndOfCentralDirectory(nullptr);
}
} // namespace SignatureTools
} // namespace OHOS