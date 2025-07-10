/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef JSVM_LOG_H
#define JSVM_LOG_H
#include <fstream>
#include <iostream>
#include <sstream>

#include "platform/platform.h"

namespace jsvm {
class LogStream : public std::stringstream {
public:
    LogStream() : std::stringstream() {}

    virtual ~LogStream() = default;
};

class LogConsole : public LogStream {
public:
    explicit LogConsole(platform::OS::LogLevel level) : LogStream(), level(level) {}

    ~LogConsole() override
    {
        platform::OS::PrintString(level, str().c_str());
    }

private:
    platform::OS::LogLevel level;
};

class LogFile : public LogStream {
public:
    explicit LogFile(const char* filename) : file(filename, std::ios::app) {};

    ~LogFile() override
    {
        file << str() << std::endl;
    }

private:
    std::ofstream file;
};

class LogInfo : public LogConsole {
public:
    LogInfo() : LogConsole(platform::OS::LogLevel::LOG_INFO) {}
};

class LogError : public LogConsole {
public:
    LogError() : LogConsole(platform::OS::LogLevel::LOG_ERROR) {}
};

class LogFatal : public LogConsole {
public:
    LogFatal() : LogConsole(platform::OS::LogLevel::LOG_FATAL) {}
};
} // namespace jsvm

// Support LOG(Info), LOG(Error), LOG(Fatal)
#define LOG(level) jsvm::Log##level()

// Print Log to file
#define LOG_FILE(filename) jsvm::LogFile(filename)
#endif