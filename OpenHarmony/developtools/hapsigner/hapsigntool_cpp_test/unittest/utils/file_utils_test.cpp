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
#include <fstream>

#include "cert_dn_utils.h"
#include "securec.h"
#include "file_utils.h"

namespace OHOS {
namespace SignatureTools {

class FileUtilsTest : public testing::Test {
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

const int THREAD_NUMS = 8;
const size_t MAX_FILE_SIZE = 1024 * 1024 * 10;
const size_t BUFFER_SIZE = 1024 * 128;
const int CHMOD = 0777;
const size_t MAX_VALUE = 0x80000000LL;

/**
 * Generate files for thread pool testing.
 */
void CreateTestFile()
{
    (void)mkdir("tmp", CHMOD);

    for (int i = 1; i <= THREAD_NUMS; i++) {
        std::string fileName = "tmp/tmp-" + std::to_string(i) + ".txt";
        std::ofstream file(fileName, std::ios::binary);
        if (!file.is_open()) {
            printf("open file: %s failed.\n", fileName.c_str());
            continue;
        }

        char buffer[BUFFER_SIZE];
        // The contents of the generated file are all characters '0'
        (void)memset_s(buffer, sizeof(buffer), '0', sizeof(buffer));

        size_t remaining = MAX_FILE_SIZE;
        while (remaining > 0) {
            size_t min = std::min(BUFFER_SIZE, remaining);
            file.write(buffer, min);
            if (!file.good()) {
                printf("write file: %s failed.\n", fileName.c_str());
                break;
            }

            remaining -= min;
        }
        printf("File %s has been created with %zu bytes.\n", fileName.c_str(), MAX_FILE_SIZE);
    }
}

/**
 * Thread pool execution function
 */
int Worker(const std::string& inputFile, const std::string& outputFile, int length)
{
    std::ifstream input(inputFile, std::ios::binary);
    std::ofstream output(outputFile, std::ios::binary);
    if (!input) {
        printf("open file: %s failed.\n", inputFile.c_str());
        return -1;
    }
    if (!output) {
        printf("open file: %s failed.\n", outputFile.c_str());
        return -1;
    }

    int res = FileUtils::WriteInputToOutPut(input, output, length);

    std::thread::id id = std::this_thread::get_id();
    printf("thread: %u completed: %s -> %s res: %d\n", *(uint32_t*)&id, inputFile.c_str(),
           outputFile.c_str(), res);

    output.close();
    input.close();
    return res;
}

/**
 * @tc.name: WriteByteToOutFile001
 * @tc.desc: Test WriteByteToOutFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, WriteByteToOutFile001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the output file is not exist
     * @tc.expected: step1. failed to get output stream object, the return will be true.
     */
    std::vector<int8_t> bytes;
    std::ofstream output("./utilstmp/signed-file.out", std::ios::binary);
    bool flag = FileUtils::WriteByteToOutFile(bytes, output);
    EXPECT_EQ(flag, false);
}

/**
 * @tc.name: WriteByteToOutFile002
 * @tc.desc: Test WriteByteToOutFile function for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, WriteByteToOutFile002, testing::ext::TestSize.Level1)
{
    std::filesystem::path dir_path("./utils");
    ASSERT_TRUE(std::filesystem::create_directory(dir_path));

    std::vector<int8_t> bytes;
    std::ofstream output("./utils/unsigned-file.out", std::ios::binary);
    bool flag = FileUtils::WriteByteToOutFile(bytes, output);
    EXPECT_EQ(flag, true);
}

/**
 * @tc.name: WriteByteToOutFile003
 * @tc.desc: Test function interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, WriteByteToOutFile003, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the output file is not exist
     * @tc.expected: step1. failed to get output stream object, the return will be true.
     */
    std::string bytes;
    std::ofstream output("./utilsxxx/signed-file.out", std::ios::binary);
    bool result = FileUtils::WriteByteToOutFile(bytes, output);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: Write
 * @tc.desc: Test Write function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, Write, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the output file is not exist
     * @tc.expected: step1. open output file failed, the return will be IO_ERROR.
     */
    std::string str;
    std::string fileName = "./utilsxxx/signed-file.out";
    int result = FileUtils::Write(str, fileName);
    EXPECT_EQ(result, IO_ERROR);
}

/**
 * @tc.name: Read
 * @tc.desc: Test Read function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, Read, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the input file is not exist
     * @tc.expected: step1. open input file failed, the return will be IO_ERROR.
     */
    std::string outstr;
    std::ifstream input("./utilsxxx/unsigned-file.out", std::ios::binary);
    int result = FileUtils::Read(input, outstr);
    EXPECT_EQ(result, IO_ERROR);
}

/**
 * @tc.name: ReadFile
 * @tc.desc: Test ReadFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, ReadFile, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the input file is not exist
     * @tc.expected: step1. open input file failed, the return will be IO_ERROR.
     */
    std::string outstr;
    std::string fileName = "./utilsxxx/unsigned-file.out";
    int result = FileUtils::ReadFile(fileName, outstr);
    EXPECT_EQ(result, IO_ERROR);
}

/**
 * @tc.name: ReadFileByOffsetAndLength001
 * @tc.desc: Test ReadFileByOffsetAndLength function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, ReadFileByOffsetAndLength001, testing::ext::TestSize.Level1)
{
    std::string outstr;
    std::ifstream input("./utils/unsigned-file.out", std::ios::binary);
    /*
     * @tc.steps: step1. make the write size is greater than INT_MAX
     * @tc.expected: step1. the return will be RET_FAILED.
     */
    int result = FileUtils::ReadFileByOffsetAndLength(input, 0, MAX_VALUE, outstr);
    EXPECT_EQ(result, RET_FAILED);
}

/**
 * @tc.name: ReadFileByOffsetAndLength002
 * @tc.desc: Test ReadFileByOffsetAndLength function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, ReadFileByOffsetAndLength002, testing::ext::TestSize.Level1)
{
    std::string outstr;
    std::ifstream input("./utils/unsigned-file.out", std::ios::binary);
    /*
     * @tc.steps: step1. make the input file is not exist, and the offset is -1
     * @tc.expected: step1. the input stream is bad, and the offset is error, the return will be IO_ERROR.
     */
    int result = FileUtils::ReadFileByOffsetAndLength(input, -1, 32, outstr);
    EXPECT_EQ(result, IO_ERROR);
}

/**
 * @tc.name: ReadInputByOffsetAndLength001
 * @tc.desc: Test ReadInputByOffsetAndLength function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, ReadInputByOffsetAndLength001, testing::ext::TestSize.Level1)
{
    std::string outstr;
    std::ifstream input("./utils/unsigned-file.out", std::ios::binary);
    /*
     * @tc.steps: step1. make the length is greater than INT_MAX
     * @tc.expected: step1. Size cannot be greater than Integer max value, the return will be -1.
     */
    int result = FileUtils::ReadInputByOffsetAndLength(input, 0, MAX_VALUE, outstr);
    EXPECT_EQ(result, -1);
}

/**
 * @tc.name: ReadInputByOffsetAndLength002
 * @tc.desc: Test ReadInputByOffsetAndLength function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, ReadInputByOffsetAndLength002, testing::ext::TestSize.Level1)
{
    std::string outstr;
    /*
     * @tc.steps: step1. make the input file is not exist
     * @tc.expected: step1. input error occurred, the return will be -1.
     */
    std::ifstream input("./utils/unsigned-filexx.out", std::ios::binary);
    int result = FileUtils::ReadInputByOffsetAndLength(input, 0, 32, outstr);
    EXPECT_EQ(result, -1);
}

/**
 * @tc.name: ReadInputByLength001
 * @tc.desc: Test ReadInputByLength function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, ReadInputByLength001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the length is greater than INT_MAX
     * @tc.expected: step1. Size cannot be greater than Integer max value, the return will be -1.
     */
    std::string outstr;
    std::ifstream input("./utils/unsigned-file.out", std::ios::binary);
    int result = FileUtils::ReadInputByLength(input, MAX_VALUE, outstr);
    EXPECT_EQ(result, -1);
}

/**
 * @tc.name: ReadInputByLength002
 * @tc.desc: Test ReadInputByLength function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, ReadInputByLength002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the input file is not exist
     * @tc.expected: step1. input error occurred, the return will be -1.
     */
    std::string outstr;
    std::ifstream input("./utils/unsigned-filexx.out", std::ios::binary);
    int result = FileUtils::ReadInputByLength(input, 32, outstr);
    EXPECT_EQ(result, -1);
}

/**
 * @tc.name: AppendWriteFileByOffsetToFile001
 * @tc.desc: Test AppendWriteFileByOffsetToFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, AppendWriteFileByOffsetToFile001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the input file is not exist
     * @tc.expected: step1. input error occurred, the return will be false.
     */
    std::ifstream input("./utils/unsigned-filexx.out", std::ios::binary);
    std::ofstream output("./utils/signed-file.out", std::ios::binary | std::ios::out);
    bool result = FileUtils::AppendWriteFileByOffsetToFile(input, output, 0, 32);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: AppendWriteFileByOffsetToFile002
 * @tc.desc: Test AppendWriteFileByOffsetToFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, AppendWriteFileByOffsetToFile002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the output file is not exist
     * @tc.expected: step1. output error occurred, the return will be false.
     */
    std::ifstream input("./utils/unsigned-file.out", std::ios::binary);
    std::ofstream output("./utilsxxx/signed-file.out", std::ios::binary);
    bool result = FileUtils::AppendWriteFileByOffsetToFile(input, output, 0, 32);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: AppendWriteFileByOffsetToFile003
 * @tc.desc: Test AppendWriteFileByOffsetToFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, AppendWriteFileByOffsetToFile003, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the offset is -1
     * @tc.expected: step1. write error occurred, the return will be false.
     */
    std::ifstream input("./utils/unsigned-file.out", std::ios::binary);
    std::ofstream output("./utils/signed-file.out", std::ios::binary);
    bool result = FileUtils::AppendWriteFileByOffsetToFile(input, output, -1, 32);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: AppendWriteFileToFile001
 * @tc.desc: Test AppendWriteFileToFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, AppendWriteFileToFile001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the input file is not exist.
     * @tc.expected: step1. failed to get input stream object, the return will be false.
     */
    std::string inputFile = "./utils/unsigned-filexx.out";
    std::string outputFile = "./utils/signed-file.out";
    bool result = FileUtils::AppendWriteFileToFile(inputFile, outputFile);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: AppendWriteFileToFile002
 * @tc.desc: Test AppendWriteFileToFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, AppendWriteFileToFile002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the output file is not exist.
     * @tc.expected: step1. failed to get output stream object, the return will be false.
     */
    std::string inputFile = "./utils/unsigned-file.out";
    std::string outputFile = "./utilsxxx/signed-file.out";
    bool result = FileUtils::AppendWriteFileToFile(inputFile, outputFile);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: AppendWriteByteToFile
 * @tc.desc: Test AppendWriteByteToFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, AppendWriteByteToFile, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the output file is not exist.
     * @tc.expected: step1. Failed to write data to output stream, the return will be false.
     */
    std::string bytes;
    std::string outputFile = "./utilsxxx/signed-file.out";
    bool result = FileUtils::AppendWriteByteToFile(bytes, outputFile);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: WriteInputToOutPut001
 * @tc.desc: Test WriteInputToOutPut function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, WriteInputToOutPut001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the input file is not exist.
     * @tc.expected: step1. failed to get input stream object, the return will be false.
     */
    std::string inputFile = "./utils/unsigned-filexx.out";
    std::string outputFile = "./utils/signed-file.out";
    bool result = FileUtils::WriteInputToOutPut(inputFile, outputFile);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: WriteInputToOutPut002
 * @tc.desc: Test function interface for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, WriteInputToOutPut002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the output file is not exist.
     * @tc.expected: step1. failed to get output stream object, the return will be false.
     */
    std::string inputFile = "./utils/unsigned-file.out";
    std::string outputFile = "./utilsxxx/signed-file.out";
    bool result = FileUtils::WriteInputToOutPut(inputFile, outputFile);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: WriteInputToOutPut003
 * @tc.desc: Test WriteInputToOutPut function for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, WriteInputToOutPut003, testing::ext::TestSize.Level1)
{
    std::string inputFile = "./utils/unsigned-file.out";
    std::string outputFile = "./utils/signed-file.out";
    bool result = FileUtils::WriteInputToOutPut(inputFile, outputFile);
    EXPECT_EQ(result, true);
}

/**
 * @tc.name: IsRunnableFile001
 * @tc.desc: Test IsRunnableFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, IsRunnableFile001, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the file name is empty.
     * @tc.expected: step1. the return will be false.
     */
    std::string fileName;
    bool result = FileUtils::IsRunnableFile(fileName);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: IsRunnableFile002
 * @tc.desc: Test IsRunnableFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, IsRunnableFile002, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the file name has not dot symbol.
     * @tc.expected: step1. the return will be false.
     */
    std::string fileName = "xxx";
    bool result = FileUtils::IsRunnableFile(fileName);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: IsRunnableFile003
 * @tc.desc: Test IsRunnableFile function for FAIL.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, IsRunnableFile003, testing::ext::TestSize.Level1)
{
    /*
     * @tc.steps: step1. make the file name is end of xxx
     * @tc.expected: step1. the return will be false.
     */
    std::string fileName = "test.xxx";
    bool result = FileUtils::IsRunnableFile(fileName);
    EXPECT_EQ(result, false);
}

/**
 * @tc.name: DelDir
 * @tc.desc: Test DelDir function for SUCCESS.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, DelDir, testing::ext::TestSize.Level1)
{
    std::string fileName = "./utils/testdeldir";
    FileUtils::DelDir(fileName);

    // create dir and file again
    std::filesystem::path dir_path(fileName);
    bool ret = std::filesystem::create_directories(dir_path);

    std::filesystem::path file_path = dir_path / "example.txt";
    std::ofstream file(file_path, std::ios::binary | std::ios::out);
    file.close();
    FileUtils::DelDir(fileName);
    EXPECT_EQ(ret, true);
}

/**
 * @tc.name: WriteInputToOutPut
 * @tc.desc: Test WriteInputToOutPut function for SUCCESS through multithreading.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, WriteInputToOutPutMultithreadingTest, testing::ext::TestSize.Level1)
{
    CreateTestFile();

    std::vector<std::thread> threads;

    for (int i = 1; i <= THREAD_NUMS; ++i) {
        std::string inputFile = "tmp/tmp-" + std::to_string(i) + ".txt";
        std::string outputFile = "tmp/tmp-" + std::to_string(i) + "-copy.txt";
        auto length = std::filesystem::file_size(inputFile);

        threads.emplace_back(Worker, inputFile, outputFile, length);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    printf("All threads completed.\n");
}

/**
 * @tc.name: GCheckDn001
 * @tc.desc: Test GCheckDn001 function for SUCCESS through multithreading.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, GCheckDn001, testing::ext::TestSize.Level1)
{
    std::vector<std::pair<std::string, std::string>> pairs;
    pairs.push_back({ "1111", "2222" });
    int ret = g_checkDn(std::string(","), pairs);
    EXPECT_NE(ret, 0);
}

/**
 * @tc.name: GCheckDn002
 * @tc.desc: Test GCheckDn002 function for SUCCESS through multithreading.
 * @tc.size: MEDIUM
 * @tc.type: FUNC
 * @tc.level Level 1
 * @tc.require: SR000H63TL
 */
HWTEST_F(FileUtilsTest, GCheckDn002, testing::ext::TestSize.Level1)
{
    std::vector<std::pair<std::string, std::string>> pairs;
    pairs.push_back({ "1111", "2222" });
    int ret = g_checkDn(std::string("hello=="), pairs);
    EXPECT_NE(ret, 0);
}
} // namespace SignatureTools
} // namespace OHOS