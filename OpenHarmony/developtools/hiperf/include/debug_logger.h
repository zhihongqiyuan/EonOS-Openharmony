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
 *
 */
#ifndef HIPERF_DEBUG_H
#define HIPERF_DEBUG_H

#include <chrono>
#include <cstdio>
#include <cstring>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <unistd.h>
#if !is_mingw
#include <sys/syscall.h>
#undef gettid
#define gettid() syscall(SYS_gettid)
#else
#include "windows.h"
inline long gettid()
{
    return GetCurrentThreadId();
}
#endif // !is_mingw

namespace OHOS {
namespace Developtools {
namespace HiPerf {
enum DebugLevel {
    LEVEL_MUCH = 1,
    LEVEL_VERBOSE,
    LEVEL_DEBUG,
    LEVEL_INFO,
    LEVEL_WARNING,
    LEVEL_ERROR,
    LEVEL_FATAL,
    LEVEL_STDOUT, // printf
    LEVEL_MAX,    // max
};

#ifdef HIPERF_DEBUG
#if is_ohos || is_double_framework
const std::string DEFAULT_UT_LOG_DIR = "/data/local/tmp/";
const std::string DEFAULT_LOG_PATH = "/data/local/tmp/hiperf_log.txt";
#elif is_mingw
const std::string DEFAULT_LOG_PATH = ".\\hiperf_log.txt";
#elif is_linux
const std::string DEFAULT_UT_LOG_DIR = "./";
const std::string DEFAULT_LOG_PATH = "hiperf_log.txt";
#else
#error unkow os
#endif

#define HILOG_BASE_TAG "HILOG"
#ifndef HILOG_TAG
#define HILOG_TAG      ""
#define HILOG_TAG_NAME HILOG_BASE_TAG
#else
#define HILOG_TAG_NAME HILOG_BASE_TAG "_" HILOG_TAG
#endif

const std::map<DebugLevel, const std::string> DebugLevelMap = {
    {LEVEL_MUCH, "M"},    {LEVEL_VERBOSE, "V"}, {LEVEL_DEBUG, "D"}, {LEVEL_INFO, "I"},
    {LEVEL_WARNING, "W"}, {LEVEL_ERROR, "E"},   {LEVEL_FATAL, "F"},
};
constexpr const int LOG_BUFFER_SIZE = 4 * 1024 * 1024;

class DebugLogger {
public:
    DebugLogger();
    ~DebugLogger();

    static DebugLogger *GetInstance();
    DebugLevel SetLogLevel(DebugLevel debugLevel);
    bool SetMixLogOutput(bool enable);
    bool SetLogPath(const std::string &logPath);
    void SetLogTags(const std::string &tags);

    int Log(DebugLevel level, const std::string &logTag, const char *fmt, ...) const
        __attribute__((format(printf, 4, 5)));
    // for class, pointer need add 1 offset (first one is *this)

    bool EnableHiLog(bool enable = true);
    DebugLevel GetLogLevel() const
    {
        return debugLevel_;
    };

    void Disable(bool disable = true);
    static bool logDisabled_;

#ifdef HIPERF_DEBUG_TIME
    mutable size_t logCount_ = 0;
    mutable std::chrono::microseconds logTimes_ = std::chrono::microseconds::zero();
    mutable std::chrono::microseconds logWriteTimes_ = std::chrono::microseconds::zero();
    mutable std::chrono::microseconds logSprintfTimes_ = std::chrono::microseconds::zero();
#endif

    // used in UT
    bool OpenLog(const std::string &tempLogPath = "", const std::string &flags = "w");
    bool RestoreLog();
    void Reset();

private:
    bool ShouldLog(DebugLevel level, const std::string &logtag) const;
    DebugLevel GetLogLevelByName(const std::string &) const;
    DebugLevel GetLogLevelByTag(const std::string &) const;
    const std::string GetLogLevelName(DebugLevel) const;

    int HiLog(std::string &buffer) const;

    static std::unique_ptr<DebugLogger> logInstance_;

    mutable std::recursive_mutex logMutex_;
    static DebugLevel debugLevel_;
    const std::chrono::steady_clock::time_point timeStamp_;
    FILE *file_ = nullptr;
    bool mixLogOutput_ = false; // log mix to std
    bool enableHilog_ = false;
    bool exitOnFatal_ = true;
    std::string logPath_;
    std::map<std::string, DebugLevel> logTagLevelmap_;
};

#ifdef HIPERF_DEBUG_PRINTF
#ifndef printf
#define printf(format, ...)                                                                        \
    do {                                                                                           \
        std::printf(format, ##__VA_ARGS__);                                                        \
        DebugLogger::GetInstance()->Log(LEVEL_STDOUT, HILOG_TAG, format, ##__VA_ARGS__);           \
    } while (0)
#endif

#ifndef perror
#define perror(format, ...)                                                                        \
    do {                                                                                           \
        std::perror(format);                                                                       \
        DebugLogger::GetInstance()->Log(LEVEL_STDOUT, HILOG_TAG, format "<%d>\n",                  \
                                        ##__VA_ARGS__, errno);                                     \
    } while (0)
#endif
#endif

class ScopeDebugLevel {
public:
    explicit ScopeDebugLevel(DebugLevel level, bool mix = false);
    ~ScopeDebugLevel();

private:
    DebugLevel savedDebugLevel_;
    bool savedMixOutput_ = false; // log mix to std
};

#define LOG_LEVEL(LEVEL)  LOG_##LEVEL
#define LOG_LEVEL_MUCH    "M:"
#define LOG_LEVEL_VERBOSE "V:"
#define LOG_LEVEL_DEBUG   "D:"
#define LOG_LEVEL_INFO    "I:"
#define LOG_LEVEL_WARNING "W:"
#define LOG_LEVEL_ERROR   "E:"
#define LOG_LEVEL_FATAL   "F:"

#ifndef HLOG
#ifdef IS_RELEASE_VERSION
#define HLOG(level, format, ...)                                                                   \
    do {                                                                                           \
        if (__builtin_expect(!DebugLogger::logDisabled_, false)) {                                 \
            DebugLogger::GetInstance()->Log(                                                       \
                level, HILOG_TAG,                                                                  \
                HILOG_TAG_NAME "/" LOG_LEVEL(level) "<%ld>%s:" format "\n", gettid(),              \
                __FUNCTION__, ##__VA_ARGS__);                                                      \
        }                                                                                          \
    } while (0)
#else
#define HLOG(level, format, ...)                                                                   \
    do {                                                                                           \
        if (__builtin_expect(!DebugLogger::logDisabled_, false)) {                                 \
            DebugLogger::GetInstance()->Log(                                                       \
                level, HILOG_TAG,                                                                  \
                HILOG_TAG_NAME "/" LOG_LEVEL(level) "<%ld>[%s:%d]%s:" format "\n", gettid(),       \
                __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);                            \
        }                                                                                          \
    } while (0)
#endif
#endif

// only log first n times
#ifndef HLOGV_FIRST
#define HLOGV_FIRST(first, format, ...)                                                            \
    do {                                                                                           \
        static int limit = first;                                                                  \
        if (limit > 0) {                                                                           \
            HLOG(LEVEL_VERBOSE, format, ##__VA_ARGS__);                                            \
            if (--limit == 0) {                                                                    \
                HLOG(LEVEL_VERBOSE, " nexttime log will be suppressed...");                        \
            }                                                                                      \
        }                                                                                          \
    } while (0)
#endif

#ifndef HLOGV_FIRST_LOCAL
#define HLOGV_FIRST_LOCAL(local_limit, format, ...)                                                \
    {                                                                                              \
        if (local_limit != 0) {                                                                    \
            HLOG(LEVEL_VERBOSE, format, ##__VA_ARGS__);                                            \
            if (local_limit > 0 && --local_limit == 0) {                                           \
                HLOG(LEVEL_VERBOSE, " nexttime log will be suppressed...");                        \
            }                                                                                      \
        }                                                                                          \
    }
#endif

#ifndef HLOGV
#define HLOGV_IF(condition, format, ...)                                                           \
    if (condition) {                                                                               \
        HLOG(LEVEL_VERBOSE, format, ##__VA_ARGS__)                                                 \
    }
#define HLOGVVV HLOGV
#endif

#ifndef HLOGDUMMY
#define HLOGDUMMY(format, ...) while (0)
#endif

#ifndef HLOGM
#define HLOGM(format, ...) HLOG(LEVEL_MUCH, format, ##__VA_ARGS__)
#define HLOGMMM            HLOGM
#endif

#ifndef HLOGV
#define HLOGV(format, ...) HLOG(LEVEL_VERBOSE, format, ##__VA_ARGS__)
#endif

#ifndef HLOGD
#define HLOGD(format, ...) HLOG(LEVEL_DEBUG, format, ##__VA_ARGS__)
#define HLOGDDD            HLOGM
#endif

#ifndef HLOGI
#define HLOGI(format, ...) HLOG(LEVEL_INFO, format, ##__VA_ARGS__)
#endif

#ifndef HLOGW
#define HLOGW(format, ...) HLOG(LEVEL_WARNING, format, ##__VA_ARGS__)
#endif

#ifndef HLOGE
#define HLOGE(format, ...) HLOG(LEVEL_ERROR, format, ##__VA_ARGS__)
#endif

#ifndef HLOGEP
#define HLOGEP(format, ...)                                                                        \
    HLOG(LEVEL_ERROR, format "(errno %d)", ##__VA_ARGS__, errno)
#endif

#ifndef HLOGF
#ifdef IS_RELEASE_VERSION
#define HLOGF(format, ...) HLOG(LEVEL_FATAL, "FATAL error occured, " format, ##__VA_ARGS__)
#else
#define HLOGF(format, ...)                                                                         \
    HLOG(LEVEL_FATAL, "FATAL error at %s:%d " format, __FILE_NAME__, __LINE__, ##__VA_ARGS__)
#endif
#endif

#ifndef HLOG_ASSERT_MESSAGE
#define HLOG_ASSERT_MESSAGE(condition, format, ...)                                                \
    if (!(condition)) {                                                                            \
        HLOG(LEVEL_FATAL, " assert failed: '%s' " format, #condition, ##__VA_ARGS__);              \
    }
#endif

#ifndef HLOG_ASSERT
#define HLOG_ASSERT(condition) HLOG_ASSERT_MESSAGE(condition, "")
#endif

#ifndef HIPERF_ASSERT
#define HIPERF_ASSERT(condition, format, ...)                                                      \
    if (!(condition)) [[unlikely]] {                                                               \
        printf(format, ##__VA_ARGS__);                                                             \
        exit(-1);                                                                                  \
    }
#endif

#undef assert
#else
#define HLOGDUMMY(...)                                                                             \
    do {                                                                                           \
    } while (0)
#define HLOGEP(...)                                                                                \
    do {                                                                                           \
    } while (0)
#define HLOGM(...)                                                                                 \
    do {                                                                                           \
    } while (0)
#define HLOGMMM(...)                                                                               \
    do {                                                                                           \
    } while (0)
#define HLOGV(...)                                                                                 \
    do {                                                                                           \
    } while (0)
#define HLOGVVV(...)                                                                               \
    do {                                                                                           \
    } while (0)
#define HLOGD(...)                                                                                 \
    do {                                                                                           \
    } while (0)
#define HLOGDDD(...)                                                                               \
    do {                                                                                           \
    } while (0)
#define HLOGI(...)                                                                                 \
    do {                                                                                           \
    } while (0)
#define HLOGW(...)                                                                                 \
    do {                                                                                           \
    } while (0)
#define HLOGE(...)                                                                                 \
    do {                                                                                           \
    } while (0)
#define HLOGF(...)                                                                                 \
    do {                                                                                           \
    } while (0)
#define HLOG_ASSERT_MESSAGE(...)                                                                   \
    do {                                                                                           \
    } while (0)
#define HLOG_ASSERT(...)                                                                           \
    do {                                                                                           \
    } while (0)

class ScopeDebugLevel {
public:
    ScopeDebugLevel(DebugLevel level, bool mix = false) {}
};
#endif
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // _HIPERF_DEBUG_H_
