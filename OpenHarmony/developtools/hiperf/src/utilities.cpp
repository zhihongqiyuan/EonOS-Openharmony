/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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

#include "utilities.h"

#include <zlib.h>
#include <thread>
#if defined(CONFIG_HAS_SYSPARA) && defined(is_ohos) && is_ohos
#include <parameters.h>
#endif
#if defined(is_mingw) && is_mingw
#include <io.h>
#endif

#ifdef CONFIG_HAS_CCM
#include "config_policy_utils.h"
#endif
#include "directory_ex.h"
#include "hiperf_hilog.h"
#include "ipc_utilities.h"

using namespace std::chrono;
namespace OHOS {
namespace Developtools {
namespace HiPerf {

static const std::string USER_DOMESTIC_BETA = "beta";
static const std::string USER_TYPE_PARAM = "const.logsystem.versiontype";
static const std::string USER_TYPE_PARAM_GET = "";
static const std::string HIVIEW_CMDLINE = "/system/bin/hiview";

std::string CanonicalizeSpecPath(const char* src)
{
    if (src == nullptr) {
        HLOGE("Error: CanonicalizeSpecPath failed");
        return "";
    } else if (strlen(src) + 1 >= PATH_MAX) {
        HLOGE("Error: CanonicalizeSpecPath %s failed", src);
        return "";
    }
    char resolvedPath[PATH_MAX] = { 0 };
#if defined(_WIN32)
    if (!_fullpath(resolvedPath, src, PATH_MAX)) {
        HLOGE("Error: _fullpath %s failed", src);
        return "";
    }
#else
    if (access(src, F_OK) == 0) {
        if (strstr(src, "/proc/") == src && strstr(src, "/data/storage") != nullptr) { // for sandbox
            if (strncpy_s(resolvedPath, sizeof(resolvedPath), src, strlen(src)) == -1) {
                HLOGE("Error: strncpy_s %s failed", src);
                return "";
            }
        } else if (realpath(src, resolvedPath) == nullptr) {
            HLOGE("Error: realpath %s failed", src);
            return "";
        }
    } else {
        std::string fileName(src);
        if (fileName.find("..") == std::string::npos) {
            if (sprintf_s(resolvedPath, PATH_MAX, "%s", src) == -1) {
                HLOGE("Error: sprintf_s %s failed", src);
                return "";
            }
        } else {
            HLOGE("Error: find .. %s failed", src);
            return "";
        }
    }
#endif
    std::string res(resolvedPath);
    return res;
}

uint32_t RoundUp(uint32_t x, const int align)
{
    return (((x) + (align) >= 1 ? (x) + (align) - 1 : 0) / (align)) * (align);
}

std::string StringReplace(std::string source, const std::string &from, const std::string &to)
{
    size_t pos = 0;
    std::string result;
    // find
    while ((pos = source.find(from)) != std::string::npos) {
        // replace
        result.append(source.substr(0, pos) + to);
        source.erase(0, pos + from.length());
    }
    // add last token
    result.append(source);
    return result;
}

size_t SubStringCount(const std::string &source, const std::string &sub)
{
    size_t count(0);
    size_t pos(0);
    if (sub.empty()) {
        return source.size();
    }
    while ((pos = source.find(sub, pos)) != std::string::npos) {
        pos += sub.size();
        count++;
    }
    return count;
}

std::vector<std::string> StringSplit(std::string source, const std::string &split)
{
    std::vector<std::string> result;

    // find
    if (!split.empty()) {
        size_t pos = 0;
        while ((pos = source.find(split)) != std::string::npos) {
            // split
            std::string token = source.substr(0, pos);
            if (!token.empty()) {
                result.push_back(token);
            }
            source.erase(0, pos + split.length());
        }
    }
    // add last token
    if (!source.empty()) {
        result.push_back(source);
    }
    return result;
}
StdoutRecord::StdoutRecord(const std::string &tempFile, const std::string &mode)
{
    if (!tempFile.empty()) {
        std::string resolvedPath = CanonicalizeSpecPath(tempFile.c_str());
        recordFile_ = fopen(resolvedPath.c_str(), mode.c_str());
        if (recordFile_ == nullptr) {
            HLOGE("tmpfile create failed '%s' with mode '%s'", tempFile.c_str(), mode.c_str());
        } else {
            // auto start it
            Start();
        }
    }
}
bool StdoutRecord::Start()
{
    content_ = EMPTY_STRING;
    fflush(stdout);

    // we will save output here
    if (recordFile_ == nullptr) {
        recordFile_ = std::tmpfile();
    }
    if (recordFile_ == nullptr) {
        // try second way
        std::string fileName = "/data/local/tmp/temp.stdout";
        std::string resolvedPath = CanonicalizeSpecPath(fileName.c_str());
        recordFile_ = fopen(resolvedPath.c_str(), "w+");
        if (recordFile_ == nullptr) {
            HLOGF("tmpfile create failed '%s'", fileName.c_str());
            return false;
        }
    }

    // we save the stdout
    stdoutFile_ = OHOS::UniqueFd(dup(STDOUT_FILENO));
    CHECK_TRUE(stdoutFile_ == -1, false, 1, "std dup failed");

    // setup temp file as stdout
    if (dup2(fileno(recordFile_), STDOUT_FILENO) != -1) {
        stop_ = false;
        return true;
    } else {
        HLOGF("std dup2 failed");
        return false;
    }
}

std::string StdoutRecord::Stop()
{
    if (stop_) {
        return content_;
    }
    fflush(stdout);
    // restore fd
    dup2(stdoutFile_, STDOUT_FILENO);

    // return file content
    if (recordFile_ != nullptr) {
        const long fileLength = lseek(fileno(recordFile_), 0, SEEK_END);
        content_.resize(fileLength);
        lseek(fileno(recordFile_), 0, SEEK_SET);
        const long len = read(fileno(recordFile_), content_.data(), fileLength);
        std::fclose(recordFile_);
        recordFile_ = nullptr;
        if (len < 0) {
            HLOGE("tmp file read failed (try read %ld)", fileLength);
        } else if (len < fileLength) {
            HLOGE("not all the data is read, lost %ld/%ld bytes", fileLength - len, fileLength);
        }
    } else {
        HLOGE("recordFile_ is nullptr");
    }
    stop_ = true;
    return content_;
}

bool IsDigits(const std::string &str)
{
    if (str.empty()) {
        return false;
    } else {
        return std::all_of(str.begin(), str.end(), ::isdigit);
    }
}

bool IsHexDigits(const std::string &str)
{
    if (str.empty()) {
        return false;
    }
    const std::string prefix {"0x"};
    std::string effectStr {str};
    if (prefix.compare(0, prefix.size(), effectStr.substr(0, prefix.size())) == 0) {
        effectStr = effectStr.substr(prefix.size(), effectStr.size() - prefix.size());
    }
    CHECK_TRUE(effectStr.empty(), false, 0, "");
    std::size_t start {0};
    for (; start < effectStr.size(); ++start) {
        if (effectStr[start] == '0') {
            continue;
        }
        break;
    }
    if (start == effectStr.size()) {
        effectStr = "0";
    }
    return std::all_of(effectStr.begin(), effectStr.end(), ::isxdigit);
}

bool IsDir(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    return false;
}

bool IsPath(const std::string &fileName)
{
    HLOG_ASSERT(!fileName.empty());
    if (fileName[0] == PATH_SEPARATOR) {
        return true;
    }
    const int prefixPathLen = 2;
    if (fileName.substr(0, prefixPathLen) == "./") {
        return true;
    }
    return false;
}

std::string PlatformPathConvert(const std::string &path)
{
#if defined(is_mingw) && is_mingw
    return StringReplace(path, "/", "\\");
#else
    return path;
#endif
}

std::string ReadFileToString(const std::string &fileName)
{
    std::string resolvedPath = CanonicalizeSpecPath(fileName.c_str());
    std::ifstream inputString(resolvedPath, std::ios::in);
    if (!inputString || !inputString.is_open()) {
        return EMPTY_STRING;
    }
    std::istreambuf_iterator<char> firstIt = {inputString};
    std::istreambuf_iterator<char> lastIt = {};

    std::string content(firstIt, lastIt);
    return content;
}

bool ReadFileToString(const std::string &fileName, std::string &fileData, size_t fileSize)
{
    fileData.clear();
    std::string resolvedPath = CanonicalizeSpecPath(fileName.c_str());
    OHOS::UniqueFd fd(open(resolvedPath.c_str(), O_RDONLY | O_BINARY));
    if (fileSize == 0) {
        struct stat fileStat;
        if (fstat(fd.Get(), &fileStat) != -1 && fileStat.st_size > 0) {
            fileData.reserve(fileStat.st_size);
        }
    } else {
        fileData.reserve(fileSize);
    }

    char buf[BUFSIZ] __attribute__((__uninitialized__));
    ssize_t readSize;
    while ((readSize = read(fd.Get(), &buf[0], sizeof(buf))) > 0) {
        fileData.append(buf, readSize);
    }
    return (readSize == 0) ? true : false;
}

bool WriteStringToFile(const std::string &fileName, const std::string &value)
{
    std::string resolvedPath = CanonicalizeSpecPath(fileName.c_str());
    std::ofstream output(resolvedPath, std::ios::out);
    if (!output) {
        return false;
    }
    output << value;

    return output.good();
}

bool IsRoot()
{
#if defined(is_ohos) && is_ohos
    std::string debugMode = "0";
    debugMode = OHOS::system::GetParameter("const.debuggable", debugMode);
    return debugMode == "1";
#else
    return true;
#endif
}

bool PowerOfTwo(uint64_t n)
{
    return n && (!(n & (n - 1)));
}

bool IscontainDigits(const std::string& str)
{
    for (char c : str) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            return true;
        }
    }
    HLOGE("not contain [0-9], str: %s", str.c_str());
    return false;
}

bool IsStringToIntSuccess(const std::string &str, int &val)
{
    char *endPtr = nullptr;
    errno = 0;
    long num = 0;
    num = std::strtol(str.c_str(), &endPtr, 10); // 10 : decimal scale
    if (endPtr == str.c_str() || *endPtr != '\0' || errno != 0 || num > INT_MAX || num < INT_MIN) {
        HLOGE("get int failed, str: %s", str.c_str());
        return false;
    }
    val = static_cast<int>(num);
    return true;
}

bool StringToUint64(const std::string &str, uint64_t &val)
{
    char *endPtr = nullptr;
    errno = 0;
    uint64_t num = std::strtoull(str.c_str(), &endPtr, 10); // 10 : decimal scale
    if (endPtr == str.c_str() || *endPtr != '\0' || errno != 0 || num > ULLONG_MAX || str.c_str()[0] == '-') {
        HIPERF_HILOGE(MODULE_DEFAULT, "get uint64 failed, str: %{public}s", str.c_str());
        return false;
    }
    val = num;
    return true;
}

bool ReadIntFromProcFile(const std::string &path, int &value)
{
    std::string s = ReadFileToString(path);
    CHECK_TRUE(s.empty(), false, 0, "");
    if (!IscontainDigits(s)) {
        return false;
    }
    if (s.size() >= 2 && StringEndsWith(s, "\n")) { // 2: string size
        s.resize(s.size() - 1);
    }
    if (!IsStringToIntSuccess(s, value)) {
        return false;
    }
    return true;
}

bool WriteIntToProcFile(const std::string &path, int value)
{
    std::string s = std::to_string(value);

    return WriteStringToFile(path, s);
}

// compress specified dataFile into gzip file
bool CompressFile(const std::string &dataFile, const std::string &destFile)
{
    std::string resolvedPath = CanonicalizeSpecPath(dataFile.c_str());
    FILE *fp = fopen(resolvedPath.c_str(), "rb");
    if (fp == nullptr) {
        HLOGE("Fail to open data file %s", dataFile.c_str());
        perror("Fail to fopen(rb)");
        return false;
    }

    std::unique_ptr<gzFile_s, decltype(&gzclose)> fgz(gzopen(destFile.c_str(), "wb"), gzclose);
    if (fgz == nullptr) {
        HLOGE("Fail to call gzopen(%s)", destFile.c_str());
        fclose(fp);
        return false;
    }

    std::vector<char> buf(COMPRESS_READ_BUF_SIZE);
    size_t len = 0;
    while ((len = fread(buf.data(), sizeof(uint8_t), buf.size(), fp))) {
        if (gzwrite(fgz.get(), buf.data(), len) == 0) {
            HLOGE("Fail to call gzwrite for %zu bytes", len);
            fclose(fp);
            return false;
        }
    }
    if (!feof(fp)) {
        if (ferror(fp) != 0) {
            HLOGE("ferror return err");
            fclose(fp);
            return false;
        }
    }
    if (fclose(fp) < 0) {
        return false;
    }
    return true;
}

// uncompress specified gzip file into dataFile
bool UncompressFile(const std::string &gzipFile, const std::string &dataFile)
{
    std::string resolvedPath = CanonicalizeSpecPath(dataFile.c_str());
    FILE *fp = fopen(resolvedPath.c_str(), "wb");
    if (fp == nullptr) {
        HLOGE("Fail to open data file %s", dataFile.c_str());
        perror("Fail to fopen(rb)");
        return false;
    }
    std::unique_ptr<gzFile_s, decltype(&gzclose)> fgz(gzopen(gzipFile.c_str(), "rb"), gzclose);
    if (fgz == nullptr) {
        HLOGE("Fail to call gzopen(%s)", gzipFile.c_str());
        fclose(fp);
        return false;
    }

    std::vector<char> buf(COMPRESS_READ_BUF_SIZE);
    z_size_t len = 0;
    while ((len = gzfread(buf.data(), sizeof(uint8_t), buf.size(), fgz.get()))) {
        if (len != fwrite(buf.data(), sizeof(uint8_t), len, fp)) {
            HLOGE("Fail to call fwrite for %zu bytes", len);
            fclose(fp);
            return false;
        }
    }
    if (!gzeof(fgz.get())) {
        int rc = 0;
        const char *err = gzerror(fgz.get(), &rc);
        if (rc != Z_OK) {
            HLOGE("gzfread return %d:%s", rc, err);
            fclose(fp);
            return false;
        }
    }
    if (fclose(fp) < 0) {
        return false;
    }
    return true;
}

std::string &StringTrim(std::string &string)
{
    if (!string.empty()) {
        string.erase(0, string.find_first_not_of(" "));
        string.erase(string.find_last_not_of(" ") + 1);
    }
    return string;
}

std::vector<std::string> GetEntriesInDir(const std::string &basePath)
{
    std::vector<std::string> result;
    std::string resolvedPath = CanonicalizeSpecPath(basePath.c_str());
    CHECK_TRUE(resolvedPath.empty(), result, 0, "");
    DIR *dir = opendir(resolvedPath.c_str());
    CHECK_TRUE(dir == nullptr, result, 0, "");
    dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        result.push_back(entry->d_name);
    }
    closedir(dir);
    return result;
}

std::vector<std::string> GetSubDirs(const std::string &basePath)
{
    std::vector<std::string> entries = GetEntriesInDir(basePath);
    std::vector<std::string> result = {};
    for (std::size_t index = 0; index < entries.size(); ++index) {
        if (IsDir(basePath + "/" + entries[index])) {
            result.push_back(std::move(entries[index]));
        }
    }
    return result;
}

bool IsSameCommand(const std::string &cmdLine, const std::string &cmdName)
{
    std::vector<std::string> cmdpaths = StringSplit(cmdLine, "/");
    if (!cmdpaths.empty()) {
        if (strcmp(cmdpaths.back().c_str(), cmdName.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

bool IsSameCommand(const std::string &cmdLine, const std::vector<std::string>& cmdNames)
{
    std::vector<std::string> cmdpaths = StringSplit(cmdLine, "/");
    if (cmdpaths.empty()) {
        return false;
    }

    for (const auto& cmdName : cmdNames) {
        if (strcmp(cmdpaths.back().c_str(), cmdName.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

std::vector<pid_t> GetSubthreadIDs(const pid_t pid)
{
    std::string path {"/proc/"};
    path += std::to_string(pid);
    path += "/task/";
    auto tids = GetSubDirs(path);
    std::vector<pid_t> res {};
    for (auto tidStr : tids) {
        if (!tidStr.empty()) {
            uint64_t dest = 0;
            if (!StringToUint64(tidStr, dest)) {
                continue;
            }
            pid_t tid = static_cast<pid_t>(dest);
            if (tid == pid) {
                continue;
            }
            res.push_back(tid);
        }
    }
    return res;
}

std::vector<pid_t> GetSubthreadIDs(const pid_t pid, std::map<pid_t, ThreadInfos> &thread_map)
{
    std::string path {"/proc/"};
    path += std::to_string(pid);
    path += "/task/";
    auto tids = GetSubDirs(path);
    std::vector<pid_t> res{};
    for (auto tidStr : tids) {
        ThreadInfos info;
        uint64_t dest = 0;
        if (!StringToUint64(tidStr, dest)) {
            continue;
        }
        pid_t tid = static_cast<pid_t>(dest);
        info.tid = tid;
        info.pid = pid;
        thread_map[tid] = info;
        res.push_back(tid);
    }
    return res;
}

bool StringStartsWith(const std::string &string, const std::string &with)
{
    return string.find(with) == 0;
}

bool StringEndsWith(const std::string &string, const std::string &with)
{
    if (string.empty()) {
        // empty string only end with empty string
        if (with.empty()) {
            return true;
        } else {
            return false;
        }
    }
    return string.rfind(with) == (string.length() - with.length());
}

bool HexDump(const void *buf, size_t size, size_t maxSize)
{
    CHECK_TRUE(buf == nullptr, false, 0, "");
    const unsigned char *byteBuf = static_cast<const unsigned char *>(buf);
    const size_t dumpByteEachLine = 8;
    size_t outputBytes = 0;
    if (!maxSize) {
        outputBytes = size;
    } else {
        outputBytes = std::min(size, maxSize);
    }

    for (size_t i = 0; i <= outputBytes; i += dumpByteEachLine) {
        HLOGM(" %02zu: %s ", i, BufferToHexString(byteBuf, dumpByteEachLine).c_str());
        byteBuf += dumpByteEachLine;
    }
    return true;
}

std::string BufferToHexString(const std::vector<unsigned char> &vec)
{
    return BufferToHexString(vec.data(), vec.size());
}

std::string BufferToHexString(const unsigned char buf[], size_t size)
{
    std::stringstream ss;
    ss << size << ":";
    for (size_t i = 0; i < size; i++) {
        ss << " 0x" << std::setfill('0') << std::setw(BYTE_PRINT_WIDTH) << std::hex
           << (unsigned short)buf[i];
    }
    return ss.str();
}

bool IsRestarted(const std::string &appPackage)
{
    printf("please restart %s for profiling within 30 seconds\n", appPackage.c_str());
    std::set<pid_t> oldPids {};
    std::set<pid_t> newPids {};
    std::vector<pid_t> intersection;
    const auto startTime = steady_clock::now();
    const auto endTime = startTime + std::chrono::seconds(CHECK_TIMEOUT);
    CollectPidsByAppname(oldPids, appPackage);
    do {
        CollectPidsByAppname(newPids, appPackage);
        std::set_intersection(oldPids.begin(), oldPids.end(),
            newPids.begin(), newPids.end(), std::back_insert_iterator(intersection));
        // app names are same, no intersection, means app restarted
        CHECK_TRUE(intersection.empty(), true, 0, "");
        intersection.clear();
        newPids.clear();
        std::this_thread::sleep_for(milliseconds(CHECK_FREQUENCY));
    } while (steady_clock::now() < endTime);
    printf("app %s was not stopped within 30 seconds\n", appPackage.c_str());
    return false;
}

pid_t GetAppPackagePid(const std::string &appPackage, const pid_t oldPid, const int checkAppMs,
                       const uint64_t waitAppTimeOut)
{
    pid_t res {-1};
    const std::string basePath {"/proc/"};
    const std::string cmdline {"/cmdline"};
    const auto startTime = steady_clock::now();
    const auto endTime = startTime + std::chrono::seconds(waitAppTimeOut);
    do {
        std::vector<std::string> subDirs = GetSubDirs(basePath);
        for (const auto &subDir : subDirs) {
            if (!IsDigits(subDir)) {
                continue;
            }
            std::string fileName {basePath + subDir + cmdline};
            if (IsSameCommand(ReadFileToString(fileName), appPackage)) {
                res = std::stoul(subDir, nullptr);
                HLOGD("[GetAppPackagePid]: get appid for %s is %d", appPackage.c_str(), res);
                return res;
            }
        }
        std::this_thread::sleep_for(milliseconds(checkAppMs));
    } while (steady_clock::now() < endTime);

    return res;
}

bool CheckAppIsRunning (std::vector<pid_t> &selectPids, const std::string &appPackage, int checkAppMs)
{
    if (!appPackage.empty()) {
        pid_t appPid = GetAppPackagePid(appPackage, -1, checkAppMs, waitAppRunCheckTimeOut);
        if (appPid <= 0) {
            printf("app %s not running\n", appPackage.c_str());
            return false;
        }
        HLOGD("[CheckAppIsRunning] get appPid %d for app %s\n", appPid, appPackage.c_str());
        selectPids.push_back(appPid);
    }
    return true;
}

bool IsExistDebugByApp(const std::string& bundleName, std::string& err)
{
    std::string bundleNameTmp = bundleName;
    auto pos = bundleNameTmp.find(":");
    if (pos != std::string::npos) {
        bundleNameTmp = bundleNameTmp.substr(0, pos);
    }
    if (!IsSupportNonDebuggableApp() && !bundleNameTmp.empty() && !IsDebugableApp(bundleNameTmp)) {
        HLOGE("--app option only support debug application.");
        err = "--app option only support debug application\n";
        printf("%s", err.c_str());
        return false;
    }
    return true;
}

bool IsExistDebugByPid(const std::vector<pid_t> &pids, std::string& err)
{
    CHECK_TRUE(pids.empty(), true, 1, "IsExistDebugByPid: pids is empty.");
    for (auto pid : pids) {
        if (pid <= 0) {
            err = "Invalid -p value '" + std::to_string(pid) + "', the pid should be larger than 0\n";
            printf("%s", err.c_str());
            return false;
        }
        std::string bundleName = GetProcessName(pid);
        auto pos = bundleName.find(":");
        if (pos != std::string::npos) {
            bundleName = bundleName.substr(0, pos);
        }
        if (!IsSupportNonDebuggableApp() && !IsDebugableApp(bundleName)) {
            HLOGE("-p option only support debug application for %s", bundleName.c_str());
            err = "-p option only support debug application\n";
            printf("%s", err.c_str());
            return false;
        }
    }
    return true;
}

std::string HandleAppInfo(const std::string& appPackage, const std::vector<pid_t> &inputPidTidArgs)
{
    std::string err = "";
    if (!appPackage.empty()) {
        if (!IsExistDebugByApp(appPackage, err)) {
            return err;
        }
    } else {
        if (!IsExistDebugByPid(inputPidTidArgs, err)) {
            return err;
        }
    }
    return err;
}

bool IsSupportNonDebuggableApp()
{
    // root first
    if (IsRoot()) {
        return true;
    }
    // user mode
    if (!IsBeta()) {
        HIPERF_HILOGE(MODULE_DEFAULT, "IsSupportNonDebuggableApp error, not beta");
        return false;
    }
    if (!IsAllowProfilingUid()) {
        HIPERF_HILOGE(MODULE_DEFAULT, "IsSupportNonDebuggableApp error, not allow profiling uid");
        return false;
    }
    return true;
}

const std::string GetUserType()
{
#if defined(is_ohos) && is_ohos
    std::string userType = OHOS::system::GetParameter(USER_TYPE_PARAM, USER_TYPE_PARAM_GET);
    HLOGD("GetUserType: userType is %s", userType.c_str());
    return userType;
#else
    return "";
#endif
}

bool GetDeveloperMode()
{
#if defined(is_ohos) && is_ohos
    bool developerMode = OHOS::system::GetBoolParameter("const.security.developermode.state", false);
    HLOGD("GetDeveloperMode: developerMode is %d", developerMode);
    return developerMode;
#else
    return true;
#endif
}

bool LittleMemory()
{
    std::ifstream file("/proc/meminfo");
    std::string line;
    while (getline(file, line)) {
        if (line.find("MemTotal:") != std::string::npos) {
            int memSize = stoi(line.substr(line.find(":") + 1));
            CHECK_TRUE(memSize < (LITTLE_MEMORY_SIZE * MULTIPLE_SIZE * MULTIPLE_SIZE), true, 0, "");
        }
    }
    return false;
}

// only for domestic beta
bool IsBeta()
{
    std::string userTypeRsp = GetUserType();
    if (userTypeRsp == USER_DOMESTIC_BETA) {
        return true;
    }
    // default release when usertype param is invalid
    CHECK_TRUE(userTypeRsp.empty(), true, 1, "GetUserType is empty [%s]", userTypeRsp.c_str());
    return false;
}

bool IsAllowProfilingUid()
{
#if (defined(is_linux) && is_linux) || (defined(is_ohos) && is_ohos)
    static unsigned int curUid = getuid();
    HLOGD("curUid is %d\n", curUid);
    CHECK_TRUE(ALLOW_UIDS.find(curUid) != ALLOW_UIDS.end(), true, 0, "");
    return false;
#else
    return false;
#endif
}

std::string GetProcessName(int pid)
{
#if defined(is_ohos) && is_ohos
    std::string filePath = "/proc/" + std::to_string(pid) + "/cmdline";
    std::string bundleName = ReadFileToString(filePath);
    return bundleName.substr(0, strlen(bundleName.c_str()));
#else
    return "";
#endif
}

bool NeedAdaptSandboxPath(char *filename, int pid, u16 &headerSize)
{
    std::string oldFilename = filename;
    if (oldFilename.find("/data/storage") == 0 && access(oldFilename.c_str(), F_OK) != 0) {
        std::string newFilename = "/proc/" + std::to_string(pid) + "/root" + oldFilename;
        (void)memset_s(filename, KILO, '\0', KILO);
        if (strncpy_s(filename, KILO, newFilename.c_str(), newFilename.size()) != 0) {
            HLOGD("strncpy_s recordMmap2 failed!");
        }
        headerSize += newFilename.size() - oldFilename.size();
        return true;
    }
    return false;
}

bool NeedAdaptHMBundlePath(std::string& filename, const std::string& threadname)
{
    static bool needCheck = true;
    if (!needCheck) {
        return false;
    }
    const std::string path = "/data/storage/el1/bundle";
    std::string newFileName = filename;
    size_t pos = newFileName.find(path);
    if (pos != std::string::npos) {
        if (access(filename.c_str(), F_OK) != 0) {
            const std::string newpath = "/data/app/el1/bundle/public/";
            // /data/storage/el1/bundle/libs/arm64/libentry.so
            newFileName.replace(pos, path.length(), newpath + threadname);
            if (access(newFileName.c_str(), F_OK) != 0) {
                return false;
            }
            // /data/app/el1/bundle/public/<procname>/libs/arm64/libentry.so
            filename = newFileName;
            HLOGD("Fix hm bundle path to %s", filename.c_str());
            return true;
        } else {
            needCheck = false;
        }
    }
    return false;
}

bool IsArkJsFile(const std::string& filepath)
{
    return (StringEndsWith(filepath, ".hap") ||
            StringStartsWith(filepath, "[anon:ArkTS Code") ||
            StringEndsWith(filepath, ".hsp") ||
            StringEndsWith(filepath, ".abc") ||
            StringEndsWith(filepath, ".hqf"));
}

bool IsHiviewCall()
{
#if defined(is_ohos) && is_ohos
    pid_t ppid = syscall(SYS_getppid);
    std::string cmdline = GetProcessName(ppid);
    HLOGD("getppid is %d, cmdline is %s", ppid, cmdline.c_str());
    if (cmdline == HIVIEW_CMDLINE) {
        HLOGD("hiview call");
        return true;
    }
    return false;
#else
    return false;
#endif
}

bool IsNumeric(const std::string& str)
{
    std::istringstream iss(str);
    int number;
    char trailingCharacter;
    if (!(iss >> number)) {
        return false;
    }
    if (iss >> trailingCharacter) {
        return false;
    }
    return true;
}

#ifdef CONFIG_HAS_CCM
cJSON* GetProductCfgRoot(const char* cfgPath)
{
    char buf[PATH_MAX] = {0};
    char* configFilePath = GetOneCfgFile(cfgPath, buf, PATH_MAX);
    if (configFilePath == nullptr || (configFilePath[0] == '\0') || (strlen(configFilePath) > PATH_MAX)) {
        HLOGD("get configFilePath from cfgPath %s failed", cfgPath);
        return nullptr;
    }
    HLOGD("get configFilePath %s from cfgPath %s", configFilePath, cfgPath);
    return ParseJson(configFilePath);
}

cJSON* ParseJson(const std::string &filePath)
{
    std::string resolvedPath = CanonicalizeSpecPath(filePath.c_str());
    std::ifstream inFile(resolvedPath, std::ios::in);
    if (!inFile.is_open()) {
        HLOGE("open file %s failed", filePath.c_str());
        return nullptr;
    }
    std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    cJSON* root = cJSON_Parse(fileContent.c_str());
    inFile.close();
    return root;
}

bool GetJsonNum(cJSON* tag, const char* key, size_t &value)
{
    cJSON* subNode = cJSON_GetObjectItem(tag, key);
    if (subNode != nullptr && cJSON_IsNumber(subNode)) {
        value = static_cast<size_t>(subNode->valuedouble);
        return true;
    }
    HLOGE("GetJsonNum key %s failed", key);
    return false;
}

bool GetCfgValue(const char* cfgPath, const char* cfgKey, size_t &value)
{
    bool ret = false;
    cJSON* root = GetProductCfgRoot(cfgPath);
    if (root == nullptr) {
        HLOGD("GetProductCfgRoot file %s failed", cfgPath);
        return ret;
    }
    if (GetJsonNum(root, cfgKey, value)) {
        ret = true;
        HLOGD("GetCfgValue %s: %zu", cfgKey, value);
    }
    cJSON_Delete(root);
    return ret;
}
#endif

bool IsDirectoryExists(const std::string& fileName)
{
    struct stat fileInfo;
    if (stat(fileName.c_str(), &fileInfo) == 0) {
        return S_ISDIR(fileInfo.st_mode);
    }
    return false;
}

bool CreateDirectory(const std::string& path, mode_t mode)
{
#if defined(is_ohos) && is_ohos
    std::string::size_type pos = 0;
    do {
        pos = path.find('/', pos + 1);
        std::string subPath = (pos == std::string::npos) ? path : path.substr(0, pos);
        if (access(subPath.c_str(), F_OK) != 0) {
            if (mkdir(subPath.c_str(), mode) != 0) {
                return false;
            }
        }
    } while (pos != std::string::npos);
    return access(path.c_str(), F_OK) == 0;
#else
    return false;
#endif
}

bool IsValidOutPath(const std::string& path)
{
    std::vector<std::string> fileName = {"/data/log/hiperflog/", "data/log/hiperflog/"};
    for (auto name : fileName) {
        if (StringStartsWith(path, name)) {
            return false;
        }
    }
    return true;
}

void AgeHiperflogFiles()
{
#if defined(is_ohos) && is_ohos
    std::vector<std::string> allFiles;
    OHOS::GetDirFiles("/data/log/hiperflog/", allFiles);
    std::set<std::string> fileNames = {"/data/log/hiperflog/[shmm]", "/data/log/hiperflog/[vdso]",
                                       "/data/log/hiperflog/.hiperf_record_control_c2s",
                                       "/data/log/hiperflog/.hiperf_record_control_s2c",
                                       "/data/log/hiperflog/.hiperf_stat_control_c2s",
                                       "/data/log/hiperflog/.hiperf_stat_control_s2c"};
    for (std::string file : allFiles) {
        if (fileNames.count(file)) {
            HLOGD("the file is %s,not need to delete", file.c_str());
            continue;
        }
        if (!OHOS::RemoveFile(file)) {
            HIPERF_HILOGI(MODULE_DEFAULT, "remove hiperflog file : %{public}s failed", file.c_str());
            continue;
        }
        HIPERF_HILOGI(MODULE_DEFAULT, "remove hiperflog file : %{public}s", file.c_str());
    }
#endif
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
