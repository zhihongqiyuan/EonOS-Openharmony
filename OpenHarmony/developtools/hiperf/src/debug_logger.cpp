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

#include "debug_logger.h"

#include "option.h"
#if defined(is_ohos) && is_ohos
#include "hiperf_hilog.h"
#endif

namespace OHOS {
namespace Developtools {
namespace HiPerf {
DebugLogger::DebugLogger() : timeStamp_(std::chrono::steady_clock::now()), logPath_(DEFAULT_LOG_PATH)
{
    OpenLog();
}

ScopeDebugLevel::ScopeDebugLevel(DebugLevel level, bool mix)
{
    savedDebugLevel_ = DebugLogger::GetInstance()->SetLogLevel(level);
    savedMixOutput_ = DebugLogger::GetInstance()->SetMixLogOutput(mix);
}

ScopeDebugLevel::~ScopeDebugLevel()
{
    DebugLogger::GetInstance()->SetLogLevel(savedDebugLevel_);
    DebugLogger::GetInstance()->SetMixLogOutput(savedMixOutput_);
}

DebugLogger::~DebugLogger()
{
    Disable();
    if (file_ != nullptr) {
        fclose(file_);
        file_ = nullptr;
    }
}

void DebugLogger::Disable(bool disable)
{
    if (logDisabled_ != disable) {
        logDisabled_ = disable;
        if (!disable) {
            // reopen the log file
            OpenLog();
        }
    }
}

#if is_ohos
#ifndef CONFIG_NO_HILOG
int DebugLogger::HiLog(std::string &buffer) const
{
    size_t lastLF = buffer.find_last_of('\n');
    if (lastLF != std::string::npos) {
        buffer.erase(lastLF, 1);
    }
    return OHOS::HiviewDFX::HiLog::Info(HIPERF_HILOG_LABLE[MODULE_DEFAULT], "%{public}s",
                                        buffer.c_str());
}
#endif
#endif

int DebugLogger::Log(DebugLevel level, const std::string &logTag, const char *fmt, ...) const
{
    constexpr const int DEFAULT_STRING_BUF_SIZE = 4096;
#ifdef HIPERF_DEBUG_TIME
    const auto startSprintf = std::chrono::steady_clock::now();
#endif
    const auto startTime = std::chrono::steady_clock::now();
    if (!ShouldLog(level, logTag) || logDisabled_ || fmt == nullptr) {
#ifdef HIPERF_DEBUG_TIME
        logTimes_ += duration_cast<microseconds>(std::chrono::steady_clock::now() - startSprintf);
#endif
        return 0;
    }
    va_list va;
    int ret = 0;

    std::string buffer(DEFAULT_STRING_BUF_SIZE, '\0');
    va_start(va, fmt);
    ret = vsnprintf_s(buffer.data(), buffer.size(), buffer.size() >= 1 ? buffer.size() - 1 : 0, fmt, va);
    va_end(va);
#ifdef HIPERF_DEBUG_TIME
    logSprintfTimes_ += duration_cast<microseconds>(std::chrono::steady_clock::now() - startSprintf);
#endif
    if ((mixLogOutput_ && level < LEVEL_FATAL) || level == LEVEL_FATAL) {
        ret = fprintf(stdout, "%s", buffer.data()); // to the stdout
    }

    if (enableHilog_) {
#if is_ohos && !defined(CONFIG_NO_HILOG)
        std::lock_guard<std::recursive_mutex> lock(logMutex_);
        ret = HiLog(buffer); // to the hilog
#endif
    } else if (file_ != nullptr) {
        std::lock_guard<std::recursive_mutex> lock(logMutex_);
#ifdef HIPERF_DEBUG_TIME
        const auto startWriteTime = std::chrono::steady_clock::now();
#endif
        auto timeStamp = startTime - timeStamp_;
        fprintf(file_, "%05" PRId64 "ms %s", (int64_t)timeStamp.count(), buffer.data()); // to the file
#ifdef HIPERF_DEBUG_TIME
        logWriteTimes_ += duration_cast<microseconds>(std::chrono::steady_clock::now() - startWriteTime);
#endif
    }

#ifdef HIPERF_DEBUG_TIME
    logTimes_ += duration_cast<microseconds>(std::chrono::steady_clock::now() - startTime);
    logCount_++;
#endif
    if (level == LEVEL_FATAL && exitOnFatal_) {
        fflush(file_);
        logDisabled_ = true;
        exit(-1);
    }
    return ret;
}

bool DebugLogger::EnableHiLog(bool enable)
{
    enableHilog_ = enable;
    if (enable) {
        if (fprintf(stdout, "change to use hilog\n") < 0) {
            printf("fprintf failed.\n");
        }
    }
    return enableHilog_;
}

bool DebugLogger::ShouldLog(DebugLevel level, const std::string &logtag) const
{
    return GetLogLevelByTag(logtag) <= level;
}

DebugLevel DebugLogger::SetLogLevel(DebugLevel debugLevel)
{
    DebugLevel lastLevel = DebugLogger::GetInstance()->debugLevel_;
    debugLevel_ = debugLevel;
    // force print
    printf("setLogLevel %d\n", debugLevel);
    return lastLevel;
}

bool DebugLogger::SetMixLogOutput(bool enable)
{
    bool lastMixLogOutput = mixLogOutput_;
    mixLogOutput_ = enable;
    return lastMixLogOutput;
}

bool DebugLogger::SetLogPath(const std::string &newLogPath)
{
    // make sure not write happend when rename
    std::lock_guard<std::recursive_mutex> lock(logMutex_);
    if (newLogPath.empty() && newLogPath != logPath_) {
        return false;
    }
    if (file_ != nullptr) {
        fclose(file_);
        file_ = nullptr;
        if (rename(logPath_.c_str(), newLogPath.c_str()) != 0) {
            // reopen the old log file path
            OpenLog();
            return false;
        }
    }
    logPath_ = newLogPath;
    return OpenLog();
}

void DebugLogger::SetLogTags(const std::string &tags)
{
    HLOGI(" tags is '%s'", tags.c_str());
    auto tagLevels = StringSplit(tags, ",");
    logTagLevelmap_.clear();
    for (auto tagLevel : tagLevels) {
        auto tagLevelPair = StringSplit(tagLevel, ":");
        if (tagLevelPair.size() == 1) { // only tag
            logTagLevelmap_[tagLevelPair[0]] = LEVEL_MUCH;
        } else { // tag:level
            logTagLevelmap_[tagLevelPair[0]] = GetLogLevelByName(tagLevelPair[1].c_str());
        }
    }
    for (auto it = logTagLevelmap_.begin(); it != logTagLevelmap_.end(); it++) {
        HLOGD(" '%s'='%s'", it->first.c_str(), GetLogLevelName(it->second).c_str());
    }
}

DebugLevel DebugLogger::GetLogLevelByTag(const std::string &tag) const
{
    if (logTagLevelmap_.count(tag) > 0) {
        return logTagLevelmap_.at(tag);
    } else {
        return GetLogLevel();
    }
}

const std::string DebugLogger::GetLogLevelName(DebugLevel level) const
{
    return DebugLevelMap.at(level);
}

DebugLevel DebugLogger::GetLogLevelByName(const std::string &name) const
{
    for (auto it = DebugLevelMap.begin(); it != DebugLevelMap.end(); it++) {
        if (it->second == name) {
            return it->first;
        }
    }
    // not found ?
    return LEVEL_MUCH;
}

// only use for UT
void DebugLogger::Reset()
{
    EnableHiLog(false);
    SetLogLevel(LEVEL_VERBOSE);
    Disable(false);
    SetLogPath(DEFAULT_LOG_PATH);
    SetLogTags("");
}

bool DebugLogger::RestoreLog()
{
    // use append not write for continually write
    return OpenLog(logPath_, "a");
}

bool DebugLogger::OpenLog(const std::string &tempLogPath, const std::string &flags)
{
    std::lock_guard<std::recursive_mutex> lock(logMutex_);

    if (logDisabled_) {
        // don't reopen it when we crash or something else.
        return false;
    }
    if (!tempLogPath.empty()) {
        if (file_ != nullptr) {
            fclose(file_);
        }
        std::string resolvedPath = CanonicalizeSpecPath(tempLogPath.c_str());
        file_ = fopen(resolvedPath.c_str(), flags.c_str());
    }
    if (file_ != nullptr) {
        // already open
        return true;
    } else {
        std::string resolvedPath = CanonicalizeSpecPath(logPath_.c_str());
        file_ = fopen(resolvedPath.c_str(), "w");
    }
    if (file_ == nullptr) {
        fprintf(stdout, "unable save log file to '%s' because '%d'\n", logPath_.c_str(), errno);
        return false;
    } else {
        fseek(file_, 0, SEEK_SET);
        // ecach log can save 6ms (29ms -> 23ms)
        fprintf(stdout, "log will save at '%s'\n", logPath_.c_str());
        return true;
    }
}

__attribute__((weak)) DebugLevel DebugLogger::debugLevel_ = LEVEL_DEBUG;
__attribute__((weak)) bool DebugLogger::logDisabled_ = true;
std::unique_ptr<DebugLogger> DebugLogger::logInstance_;

DebugLogger *DebugLogger::GetInstance()
{
    if (logInstance_ == nullptr) {
        logInstance_ = std::make_unique<DebugLogger>();
    }
    return logInstance_.get();
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
