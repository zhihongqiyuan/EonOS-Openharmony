/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#ifndef HIPERF_UTILITIES_H_
#define HIPERF_UTILITIES_H_

// for security function
#include <securec.h>
#include <algorithm>
#include <cctype>
#include <cinttypes>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <set>

#include <dirent.h>
#include <fcntl.h>
#include <file_ex.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unique_fd.h>
#include <unistd.h>
#if !is_mingw
#include <sys/syscall.h>
#endif
#include <linux/types.h>

#ifdef CONFIG_HAS_CCM
#include "cJSON.h"
#endif

#include "debug_logger.h"
#include "noncopyable.h"

// data and value
/*
long long always 64 only in ILP64, int is 64 otherwise int is always 32
*/
using s8 = __s8;
using u8 = __u8;
using s16 = __s16;
using u16 = __u16;
using s32 = __s32;
using u32 = __u32;
using s64 = __s64;
using u64 = __u64;

constexpr const int NUMBER_FORMAT_HEX_BASE = 16;
constexpr const int BYTE_PRINT_WIDTH = 2;
constexpr const int UINT64_PRINT_WIDTH = BYTE_PRINT_WIDTH * 8;
constexpr const int BITS_OF_BYTE = 8;
constexpr const int BITS_OF_TWO_BYTE = 2 * BITS_OF_BYTE;
constexpr const int BITS_OF_FOUR_BYTE = 4 * BITS_OF_BYTE;
constexpr const int FULL_PERCENTAGE = 100;
constexpr const int FULL_PERCENTAGE_NUM_LEN = 5;      // 100.00
constexpr const int FULL_PERCENTAGE_DIFF_NUM_LEN = 6; // +100.00
constexpr const int FULL_PERCENTAGE_LEN = 6;          // 100.00%
constexpr const int FULL_PERCENTAGE_DIFF_LEN = 7;     // +100.00%
constexpr const int THOUSANDS = 1000;
constexpr const int HUNDREDS = 100;
constexpr const int DEFAULT_STRING_BUF_SIZE = 4096;
constexpr const int FIVE_THOUSANDS = 5000;
constexpr const int DATA_MAX_SIZE = 1001;
constexpr const int LITTLE_MEMORY_SIZE = 1;
constexpr const int MULTIPLE_SIZE = 1024;
constexpr const uint16_t CHECK_FREQUENCY = 100; //
constexpr const uint8_t CHECK_TIMEOUT = 30;
constexpr const int INDENT_TWO = 2;
constexpr const float ALMOST_ZERO = 0.001;
#if !is_mingw
#ifndef O_BINARY
#define O_BINARY 0
#endif
#endif

constexpr const double MS_DURATION =
    static_cast<double>(std::chrono::milliseconds::duration::period::den);

constexpr uint64_t KILO = 1024;

namespace OHOS {
namespace Developtools {
namespace HiPerf {
std::string CanonicalizeSpecPath(const char* src);
const std::string EMPTY_STRING = "";
const ssize_t ERRINFOLEN = 512;
const std::set<int> ALLOW_UIDS = {1201};

const std::string SAVED_CMDLINES = "/sys/kernel/tracing/saved_cmdlines";
static FILE *g_outputDump = nullptr;
const uint64_t waitAppRunCheckTimeOut = 20;
constexpr mode_t HIPERF_FILE_PERM_770 = S_IRWXU | S_IRWXG;
struct ThreadInfos {
    pid_t tid;
    pid_t pid;
};
// string function
class MemoryHold {
public:
    ~MemoryHold()
    {
        Clean();
    }
    // only use in UT
    void Clean()
    {
        for (auto &p : holder_) {
            delete[] p;
        }
        holder_.clear();
    }
    static MemoryHold &Get()
    {
        static MemoryHold instance;
        return instance;
    }

private:
    std::vector<char *> holder_;
};

std::string StringReplace(std::string source, const std::string &from, const std::string &to);

template<class T>
std::string VectorToString(const std::vector<T> &items)
{
    if constexpr (std::is_same<T, std::vector<std::string>>::value) {
        std::vector<std::string> stringItems;
        for (auto item : items) {
            stringItems.push_back("[" + VectorToString(item) + "]");
        }
        return VectorToString(stringItems);
    } else {
        std::string itemsString;
        const std::string split = ",";
        for (auto item : items) {
            if (!itemsString.empty())
                itemsString.append(split);
            if constexpr (std::is_same<T, std::string>::value) {
                itemsString.append(item);
            } else {
                itemsString.append(std::to_string(item));
            }
        }
        if (itemsString.empty())
            itemsString.append("<empty>");
        return itemsString;
    }
}

template<class T>
std::string SetToString(const std::unordered_set<T> &items)
{
    std::string result = "";
    const std::string split = ",";
    for (auto item : items) {
        if (!result.empty()) {
            result.append(split);
        }
        if constexpr (std::is_same<T, std::string>::value) {
            result.append(item);
        } else {
            result.append(std::to_string(item));
        }
    }
    if (result.empty()) {
        result.append("<empty>");
    }
    return result;
}

std::string BufferToHexString(const std::vector<unsigned char> &vec);
std::string BufferToHexString(const unsigned char buf[], size_t size);
bool HexDump(const void *buf, size_t size, size_t max_size = 0);

std::string &StringTrim(std::string &s);

std::vector<std::string> StringSplit(std::string source, const std::string &split = ",");

size_t SubStringCount(const std::string &source, const std::string &sub);

bool StringStartsWith(const std::string &string, const std::string &with);

bool StringEndsWith(const std::string &string, const std::string &with);

bool IsSameCommand(const std::string &cmdLine, const std::string &cmdName);
bool IsSameCommand(const std::string &cmdLine, const std::vector<std::string>& cmdNames);

std::vector<pid_t> GetSubthreadIDs(const pid_t pid);

bool IsDigits(const std::string &str);

bool IsHexDigits(const std::string &str);

constexpr const int COMPRESS_READ_BUF_SIZE = 4096;
// compress specified dataFile into gzip file
bool CompressFile(const std::string &dataFile, const std::string &destFile);
// uncompress specified gzip file into dataFile
bool UncompressFile(const std::string &gzipFile, const std::string &dataFile);

template<typename... VA>
std::string StringPrintf(const char *stringFormat, VA... args)
{
    // check howmany bytes we need
    char bytes[DEFAULT_STRING_BUF_SIZE];
    bytes[DEFAULT_STRING_BUF_SIZE - 1] = '\0';

    if (stringFormat == nullptr) {
        return EMPTY_STRING;
    }

    // print it to bytes
    if (snprintf_s(bytes, sizeof(bytes), sizeof(bytes) - 1, stringFormat,
                   args...) < 0) {
        return EMPTY_STRING;
    }

    // make a string return
    return std::string(bytes);
}

// path check
std::vector<std::string> GetEntriesInDir(const std::string &basePath);

std::vector<std::string> GetSubDirs(const std::string &basePath);
std::vector<pid_t> GetSubthreadIDs(const pid_t pid, std::map<pid_t, ThreadInfos> &thread_map);

bool IsDir(const std::string &path);

bool IsPath(const std::string &fileName);

bool LittleMemory();

#if defined(is_mingw) && is_mingw
const char PATH_SEPARATOR = '\\';
#else
const char PATH_SEPARATOR = '/';
#endif
const std::string PATH_SEPARATOR_STR = std::string(1, PATH_SEPARATOR);

std::string PlatformPathConvert(const std::string &path);

// attribute
#define PACKED __attribute__((packed))

// data align

// some time u will meet signal 7 (SIGBUS), code 1 (BUS_ADRALN) in 32 or 64 arch cpu
#define HIPERF_BUF_ALIGN alignas(64)

#define ALIGN(size, align) (((size) + (align) - 1) & (~((align) - 1)))

uint32_t RoundUp(uint32_t x, const int align);

// data convert function
template<class T>
std::string ToHex(const T &source, int size = sizeof(T), bool prefix = false)
{
    std::stringstream ss;
    if (prefix) {
        ss << "0x";
    }
    ss << std::hex << std::setw(BYTE_PRINT_WIDTH * size) << std::setfill('0') << (uint64_t)source;
    return ss.str();
}

// data move and copy
template<class S, class T>
size_t inline CopyFromBufferAndMove(S *&buffer, T *dest, size_t size = 0)
{
    if (size == 0) {
        size = sizeof(T);
    }
    if (memcpy_s(dest, size, buffer, size) != EOK) {
        return size;
    }
    buffer = buffer + size;
    return size;
}

// file read write
bool ReadIntFromProcFile(const std::string &path, int &value);
bool WriteIntToProcFile(const std::string &path, int value);
std::string ReadFileToString(const std::string &fileName);
bool ReadFileToString(const std::string &fileName, std::string &content, size_t fileSize = 0);
bool WriteStringToFile(const std::string &fileName, const std::string &value);

// stdout
class StdoutRecord {
public:
    ~StdoutRecord()
    {
        Stop(); // stdout need restore
    }
    StdoutRecord(const std::string &tempFile = EMPTY_STRING,
                 const std::string &mode = EMPTY_STRING);

    bool Start();
    std::string Stop();

private:
    OHOS::UniqueFd stdoutFile_;       // back and restore stdout
    std::FILE *recordFile_ = nullptr; // save the output
    bool stop_ = true;
    std::string content_ = EMPTY_STRING;
};

// misc
template<class T>
float Percentage(const T &a, const T &b)
{
    if (b == 0) {
        return 0;
    }
    return static_cast<float>(a) / static_cast<float>(b) * FULL_PERCENTAGE;
}

bool IsRoot();
bool IsBeta();
bool IsAllowProfilingUid();
bool IsHiviewCall();
bool PowerOfTwo(uint64_t n);
bool IsNumeric(const std::string& str);
bool IscontainDigits(const std::string& str);
bool IsStringToIntSuccess(const std::string &str, int &num);
bool StringToUint64(const std::string &str, uint64_t &val);
bool IsDirectoryExists(const std::string& fileName);
bool CreateDirectory(const std::string& path, mode_t mode);
bool IsValidOutPath(const std::string& path);
void AgeHiperflogFiles();

const std::string HMKERNEL = "HongMeng";

#define PRINT_INDENT(indent, format, ...)                                                          \
    if (indent >= 0) {                                                                             \
        if (g_outputDump == nullptr) {                                                             \
            printf("%*s" format, (indent)*2, "", ##__VA_ARGS__);                                   \
        } else {                                                                                   \
            fprintf(g_outputDump, "%*s" format, (indent)*2, "", ##__VA_ARGS__);                    \
        }                                                                                          \
    } else {                                                                                       \
        HLOGV("%s" format, "", ##__VA_ARGS__);                                                     \
    }

#ifndef MMAP_FAILED
#define MMAP_FAILED reinterpret_cast<void *>(-1)
#endif
#ifndef MAP_FAILED
#define MAP_FAILED MMAP_FAILED
#endif
pid_t GetAppPackagePid(const std::string &appPackage, const pid_t oldPid, const int checkAppMs,
                       const uint64_t waitAppTimeOut);
bool IsRestarted(const std::string &appPackage);

template<typename Container>
void CollectPidsByAppname(std::set<pid_t> &pids, const Container& appPackage)
{
    const std::string basePath {"/proc/"};
    const std::string cmdline {"/cmdline"};
    std::vector<std::string> subDirs = GetSubDirs(basePath);
    for (const auto &subDir : subDirs) {
        if (!IsDigits(subDir)) {
            continue;
        }
        std::string fileName {basePath + subDir + cmdline};
        if (IsSameCommand(ReadFileToString(fileName), appPackage)) {
            pids.emplace(std::stoul(subDir, nullptr));
        }
    }
}

bool CheckAppIsRunning(std::vector<pid_t> &selectPids, const std::string &appPackage, int checkAppMs);
bool IsExistDebugByApp(const std::string& bundleName, std::string& err);
bool IsExistDebugByPid(const std::vector<pid_t> &pids, std::string& err);
bool IsSupportNonDebuggableApp();
const std::string GetUserType();
bool GetDeveloperMode();
bool IsArkJsFile(const std::string& filepath);
std::string GetProcessName(int pid);
std::string HandleAppInfo(const std::string& appPackage, const std::vector<pid_t> &inputPidTidArgs);
bool NeedAdaptSandboxPath(char *filename, int pid, u16 &headerSize);
bool NeedAdaptHMBundlePath(std::string& filename, const std::string& threadname);

template<typename T>
inline bool CheckOutOfRange(const T& value, const T& min, const T& max)
{
    return value < min || value > max;
}

#define RETURN_IF(condition, retValue) do {         \
    if (condition) {                                \
        return retValue;                            \
    }                                               \
} while (0)

#ifdef CONFIG_HAS_CCM
cJSON* GetProductCfgRoot(const char* cfgPath);
cJSON* ParseJson(const std::string &filePath);
bool GetJsonNum(cJSON* tag, const char* key, size_t &value);
bool GetCfgValue(const char* cfgPath, const char* cfgKey, size_t &value);
#endif
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_UTILITIES_H_
