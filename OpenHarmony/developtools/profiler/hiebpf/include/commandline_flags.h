/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
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

#ifndef COMMAND_LINE_FLAGS_H
#define COMMAND_LINE_FLAGS_H
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstddef>
#include <inttypes.h>
#include <cstdint>
#include <type_traits>
#include <typeinfo>
#include <cstdlib>
#include <climits>

namespace CommandLineFlags {
enum class FlagTypes {
    FT_void,
    FT_bool,
    FT_int8,
    FT_int16,
    FT_int32,
    FT_int64,
    FT_uint8,
    FT_uint16,
    FT_uint32,
    FT_uint64,
    FT_double,
    FT_string,
    FT_bool_array,
    FT_int8_array,
    FT_int16_array,
    FT_int32_array,
    FT_int64_array,
    FT_uint8_array,
    FT_uint16_array,
    FT_uint32_array,
    FT_uint64_array,
    FT_double_array,
    FT_string_array,
};

struct MetaFlag {
    FlagTypes type_ {FlagTypes::FT_void};
    void* addr_ {nullptr};
    MetaFlag() = default;
    MetaFlag(const FlagTypes& flagType, void* flagPtr): type_ {flagType}, addr_ {flagPtr} {}
};

std::unordered_map<std::string, MetaFlag> flags_ {};

template<typename T>
class FlagRegesterer {
public:
    FlagRegesterer(const std::string& name, const T& flag)
    {
        if (flags_.count(name) != 0) {
            std::cout << "CommandLineFlags WARN: flag " << name << " redefined" << std::endl;
        } else {
            auto& metaFlag = flags_[name];
            metaFlag.addr_ = (void*) &flag;

            if (std::is_same<T, bool>::value) {
                metaFlag.type_ = FlagTypes::FT_bool;
            } else if (std::is_same<T, int8_t>::value) {
                metaFlag.type_ = FlagTypes::FT_int8;
            } else if (std::is_same<T, int16_t>::value) {
                metaFlag.type_ = FlagTypes::FT_int16;
            } else if (std::is_same<T, int32_t>::value) {
                metaFlag.type_ = FlagTypes::FT_int32;
            } else if (std::is_same<T, int64_t>::value) {
                metaFlag.type_ = FlagTypes::FT_int64;
            } else if (std::is_same<T, uint8_t>::value) {
                metaFlag.type_ = FlagTypes::FT_uint8;
            } else if (std::is_same<T, uint16_t>::value) {
                metaFlag.type_ = FlagTypes::FT_uint16;
            } else if (std::is_same<T, uint32_t>::value) {
                metaFlag.type_ = FlagTypes::FT_uint32;
            } else if (std::is_same<T, uint64_t>::value) {
                metaFlag.type_ = FlagTypes::FT_uint64;
            } else if (std::is_same<T, double>::value) {
                metaFlag.type_ = FlagTypes::FT_double;
            } else if (std::is_same<T, std::string>::value) {
                metaFlag.type_ = FlagTypes::FT_string;
            } else if (std::is_same<T, std::vector<bool>>::value) {
                metaFlag.type_ = FlagTypes::FT_bool_array;
            } else if (std::is_same<T, std::vector<int8_t>>::value) {
                metaFlag.type_ = FlagTypes::FT_int8_array;
            } else if (std::is_same<T, std::vector<int16_t>>::value) {
                metaFlag.type_ = FlagTypes::FT_int16_array;
            } else if (std::is_same<T, std::vector<int32_t>>::value) {
                metaFlag.type_ = FlagTypes::FT_int32_array;
            } else if (std::is_same<T, std::vector<int64_t>>::value) {
                metaFlag.type_ = FlagTypes::FT_int64_array;
            } else if (std::is_same<T, std::vector<uint8_t>>::value) {
                metaFlag.type_ = FlagTypes::FT_uint8_array;
            } else if (std::is_same<T, std::vector<uint16_t>>::value) {
                metaFlag.type_ = FlagTypes::FT_uint16_array;
            } else if (std::is_same<T, std::vector<uint32_t>>::value) {
                metaFlag.type_ = FlagTypes::FT_uint32_array;
            } else if (std::is_same<T, std::vector<uint64_t>>::value) {
                metaFlag.type_ = FlagTypes::FT_uint64_array;
            } else if (std::is_same<T, std::vector<double>>::value) {
                metaFlag.type_ = FlagTypes::FT_double_array;
            } else if (std::is_same<T, std::vector<std::string>>::value) {
                metaFlag.type_ = FlagTypes::FT_string_array;
            }
        }
    }
};

static inline void PrintArgFormat()
{
    std::string format {"Arguments Format: <command> -<character> <value> --<word>[-<word>] <value>"};
    std::cout << format << std::endl;
    return;
}

static inline std::string ConvertToFlagName(char* argStr, int argLen)
{
    for (int k = 0; k < argLen; ++k) {
        if (argStr[k] == '-') {
            if (argStr[k-1] == '-') {
                // continuous '-', bad argument
                return "";
            }
            argStr[k] = '_';
        }
    }
    return std::string(argStr, argLen);
}

static int GetFlagName(
    char*** argv,
    int& argIndex,
    std::string& flagName)
{
    if ((*argv)[argIndex] == nullptr) {
        // argument has been removed, skip it
        flagName = "";
        ++argIndex;
        return 0;
    }

    // get argument prefix length and argument length
    char* argStr = (*argv)[argIndex];
    int prefix {0};
    int argLen {0};
    bool keepCounting {true};
    for (; argStr[argLen] != '\0'; ++argLen) {
        if (argStr[argLen] != '-') {
            keepCounting = false;
        }
        if (keepCounting and argStr[argLen] == '-') {
            ++prefix;
        }
    }
    argLen -= prefix;

    // short argument
    constexpr int shortFlagLen {1};
    if (prefix == shortFlagLen and argLen == shortFlagLen) {
        flagName = std::string(argStr + prefix, argLen);
        ++argIndex;
        return 0;
    }

    // long argument
    constexpr int longPrefix {2};
    if (prefix == longPrefix and argLen > shortFlagLen) {
        flagName = ConvertToFlagName(argStr + longPrefix, argLen);
        if (flagName.empty()) {
            // bad argument
            std::cout << "CommandLineFlags ERROR: unrecognized argument format: "
                    << argStr << std::endl;
            PrintArgFormat();
            return -1;
        }
        ++argIndex;
        return 0;
    }
    // bad argument
    std::cout << "CommandLineFlags ERROR: unrecognized argument format: "
                << argStr << std::endl;
    PrintArgFormat();
    return -1;
}

static int GetFlagValue(
    char*** argv,
    int& argIndex,
    std::string& flagValue)
{
    if ((*argv)[argIndex] == nullptr) {
        // arg removed, skip it
        flagValue = "";
        ++argIndex;
        return 0;
    }
    char* argStr = (*argv)[argIndex];
    int argLen {0};
    for (; argStr[argLen] != '\0'; ++argLen);
    flagValue = std::string(argStr, argLen);
    ++argIndex;
    return 0;
}

static void ClearCString(char* str)
{
    if (str == nullptr) {
        return;
    }
    for (int k = 0; ; ++k) {
        if (str[k] == '\0') {
            break;
        }
        str[k] = '\0';
    }
    return;
}

static int GetNextFlag(
    char*** argv,
    int& argIndex,
    bool rm,
    std::string& flagName,
    std::string& flagValue)
{
    if (GetFlagName(argv, argIndex, flagName) != 0) {
        return -1;
    }
    GetFlagValue(argv, argIndex, flagValue);
    // flag parsing done
    if (rm and flags_.count(flagName) != 0) {
        // remove parsed argument and its value
        ClearCString((*argv)[argIndex - 1]);
        ClearCString((*argv)[argIndex - 2]); // 2: point of args
    }
    return 0;
}

static int ParseBoolFlag(const std::string& flag, const std::string& valStr)
{
    bool *val = (bool*) flags_[flag].addr_;
    if (valStr.compare("false") == 0) {
        *val = false;
    } else if (valStr.compare("true") == 0) {
        *val = true;
    } else {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of bool type should only be assigned with 'true' or 'false' but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    return 0;
}

// we will not handle any exceptions thrown by std::stoi() series functions, we should
// just let exceptions abort the process.
static int ParseInt8Flag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    long int tmpVal = strtol(valStr.c_str(), &pos, 10);
    if (tmpVal == 0L || tmpVal == LONG_MAX || tmpVal == LONG_MIN) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    int8_t* val = (int8_t*) flags_[flag].addr_;
    *val = static_cast<int8_t>(tmpVal);
    return 0;
}

static int ParseInt16Flag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    long int tmpVal = strtol(valStr.c_str(), &pos, 10);
    if (tmpVal == 0L || tmpVal == LONG_MAX || tmpVal == LONG_MIN) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    int16_t* val = (int16_t*) flags_[flag].addr_;
    *val = static_cast<int16_t>(tmpVal);
    return 0;
}

static int ParseInt32Flag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    long int tmpVal = strtol(valStr.c_str(), &pos, 10);
    if (tmpVal == 0L || tmpVal == LONG_MAX || tmpVal == LONG_MIN) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    int32_t* val = (int32_t*) flags_[flag].addr_;
    *val = static_cast<int32_t>(tmpVal);
    return 0;
}

static int ParseInt64Flag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    long int tmpVal = strtol(valStr.c_str(), &pos, 10);
    if (tmpVal == 0L || tmpVal == LONG_MAX || tmpVal == LONG_MIN) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    int64_t* val = (int64_t*) flags_[flag].addr_;
    *val = static_cast<int64_t>(tmpVal);
    return 0;
}

static int ParseUint8Flag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    unsigned long long int tmpVal = strtoull(valStr.c_str(), &pos, 10);
    if (tmpVal == 0ULL || tmpVal == ULONG_MAX) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    uint8_t* val = (uint8_t*) flags_[flag].addr_;
    *val = static_cast<uint8_t>(tmpVal);
    return 0;
}

static int ParseUint16Flag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    unsigned long long int tmpVal = strtoull(valStr.c_str(), &pos, 10);
    if (tmpVal == 0ULL || tmpVal == ULONG_MAX) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    uint16_t* val = (uint16_t*) flags_[flag].addr_;
    *val = static_cast<uint16_t>(tmpVal);
    return 0;
}

static int ParseUint32Flag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    unsigned long long int tmpVal = strtoull(valStr.c_str(), &pos, 10);
    if (tmpVal == 0ULL || tmpVal == ULONG_MAX) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    uint32_t* val = (uint32_t*) flags_[flag].addr_;
    *val = static_cast<uint32_t>(tmpVal);
    return 0;
}

static int ParseUint64Flag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    unsigned long long int tmpVal = strtoull(valStr.c_str(), &pos, 10);
    if (tmpVal == 0ULL || tmpVal == ULONG_MAX) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    uint64_t* val = (uint64_t*) flags_[flag].addr_;
    *val = static_cast<uint64_t>(tmpVal);
    return 0;
}

static int ParseDoubleFlag(const std::string& flag, const std::string& valStr)
{
    char* pos = nullptr;
    double tmpVal = strtod(valStr.c_str(), &pos);
    if (tmpVal == 0.0 || tmpVal == HUGE_VAL) {
        std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
        return -1;
    }
    double* val = (double*) flags_[flag].addr_;
    *val = tmpVal;
    return 0;
}

static int ParseStringFlag(const std::string& flag, const std::string& valStr)
{
    std::string* val = (std::string*) flags_[flag].addr_;
    *val = valStr;
    return 0;
}

static int ParseBoolArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<bool>* val = (std::vector<bool>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            if (str.compare("false") == 0) {
                val->push_back(false);
            } else if (str.compare("true") == 0) {
                val->push_back(true);
            } else {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                          << " of bool type should only be assigned with 'true' or 'false' but "
                          << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseInt8ArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<int8_t>* val = (std::vector<int8_t>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            long int tmpVal = strtol(valStr.c_str(), &pos, 10);
            if (tmpVal == 0L || tmpVal == LONG_MAX || tmpVal == LONG_MIN) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(static_cast<int8_t>(tmpVal));
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseInt16ArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<int16_t>* val = (std::vector<int16_t>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            long int tmpVal = strtol(valStr.c_str(), &pos, 10);
            if (tmpVal == 0L || tmpVal == LONG_MAX || tmpVal == LONG_MIN) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(static_cast<int16_t>(tmpVal));
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseInt32ArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<int32_t>* val = (std::vector<int32_t>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            long int tmpVal = strtol(valStr.c_str(), &pos, 10);
            if (tmpVal == 0L || tmpVal == LONG_MAX || tmpVal == LONG_MIN) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(static_cast<int32_t>(tmpVal));
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseInt64ArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<int64_t>* val = (std::vector<int64_t>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            long int tmpVal = strtol(valStr.c_str(), &pos, 10);
            if (tmpVal == 0L || tmpVal == LONG_MAX || tmpVal == LONG_MIN) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                  << " of numeric type should only be assigned with number but "
                  << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(static_cast<int64_t>(tmpVal));
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseUint8ArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<uint8_t>* val = (std::vector<uint8_t>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            unsigned long long int tmpVal = strtoull(valStr.c_str(), &pos, 10);
            if (tmpVal == 0ULL || tmpVal == ULONG_MAX) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                          << " of numeric type should only be assigned with number but "
                          << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(static_cast<uint8_t>(tmpVal));
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseUint16ArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<uint16_t>* val = (std::vector<uint16_t>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            unsigned long long int tmpVal = strtoull(valStr.c_str(), &pos, 10);
            if (tmpVal == 0ULL || tmpVal == ULONG_MAX) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                          << " of numeric type should only be assigned with number but "
                          << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(static_cast<uint16_t>(tmpVal));
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseUint32ArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<uint32_t>* val = (std::vector<uint32_t>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            unsigned long long int tmpVal = strtoull(valStr.c_str(), &pos, 10);
            if (tmpVal == 0ULL || tmpVal == ULONG_MAX) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                          << " of numeric type should only be assigned with number but "
                          << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(static_cast<uint32_t>(tmpVal));
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseUint64ArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<uint64_t>* val = (std::vector<uint64_t>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            unsigned long long int tmpVal = strtoull(valStr.c_str(), &pos, 10);
            if (tmpVal == 0ULL || tmpVal == ULONG_MAX) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                          << " of numeric type should only be assigned with number but "
                          << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(static_cast<uint64_t>(tmpVal));
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseDoubleArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<double>* val = (std::vector<double>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            auto str = valStr.substr(start, end - start);
            char* pos = nullptr;
            double tmpVal = strtod(valStr.c_str(), &pos);
            if (tmpVal == 0.0 || tmpVal == HUGE_VAL) {
                std::cout << "CommandLineFlags ERROR: argument " << flag
                          << " of numeric type should only be assigned with number but "
                          << valStr << " is given" << std::endl;
                val->clear();
                return -1;
            }
            val->push_back(tmpVal);
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseStringArrayFlag(const std::string& flag, const std::string& valStr)
{
    std::vector<std::string>* val = (std::vector<std::string>*) flags_[flag].addr_;
    for (std::size_t start = 0, end = 0; end <= valStr.length(); ++end) {
        if (valStr[end] == ',' or end == valStr.length()) {
            if (start == end) {
                ++start;
                continue;
            }
            val->emplace_back(valStr.c_str() + start, end - start);
            start =  end + 1;
        }
    }
    return 0;
}

static int ParseFlag(const std::string& flagName, const std::string flagValue)
{
    int ret {0};
    switch (flags_[flagName].type_) {
        case FlagTypes::FT_bool: ret = ParseBoolFlag(flagName, flagValue); break;
        case FlagTypes::FT_int8: ret =  ParseInt8Flag(flagName, flagValue); break;
        case FlagTypes::FT_int16: ret = ParseInt16Flag(flagName, flagValue); break;
        case FlagTypes::FT_int32: ret = ParseInt32Flag(flagName, flagValue); break;
        case FlagTypes::FT_int64: ret = ParseInt64Flag(flagName, flagValue); break;
        case FlagTypes::FT_uint8: ret = ParseUint8Flag(flagName, flagValue); break;
        case FlagTypes::FT_uint16: ret = ParseUint16Flag(flagName, flagValue); break;
        case FlagTypes::FT_uint32: ret = ParseUint32Flag(flagName, flagValue); break;
        case FlagTypes::FT_uint64: ret = ParseUint64Flag(flagName, flagValue); break;
        case FlagTypes::FT_double: ret = ParseDoubleFlag(flagName, flagValue); break;
        case FlagTypes::FT_string: ret = ParseStringFlag(flagName, flagValue); break;
        case FlagTypes::FT_bool_array: ret = ParseInt8ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_int8_array: ret = ParseInt8ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_int16_array: ret = ParseInt16ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_int32_array: ret = ParseInt32ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_int64_array: ret = ParseInt64ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_uint8_array: ret = ParseUint8ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_uint16_array: ret = ParseUint16ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_uint32_array: ret = ParseUint32ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_uint64_array: ret = ParseUint64ArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_double_array: ret = ParseDoubleArrayFlag(flagName, flagValue); break;
        case FlagTypes::FT_string_array: ret = ParseStringArrayFlag(flagName, flagValue); break;
        default: ret = -1;
    }
    return ret;
}

int ParseFlags(int *argc, char ***argv, bool rm = true)
{
    std::string flagName {};
    std::string flagValue {};
    int argIndex {1};
    if (((*argc) % 2) == 0) { // 2: double
        std::cout << "CommandLineFlags ERROE: incorrect number of arguments" << std::endl;
        PrintArgFormat();
        return -1;
    }
    while (argIndex < (*argc) - 1) {
        if (GetNextFlag(argv, argIndex, rm, flagName, flagValue) != 0) {
            // argument format error
            return -1;
        }
        if (flagName.empty() or flags_.count(flagName) == 0) {
            // argument removed or undefined, skip it and continue;
            continue;
        }
        if (ParseFlag(flagName, flagValue) != 0) {
            // argument value error
            return -1;
        }
    }

    return 0;
}
} // end of namespace CommandLineFlags

#define HIEBPF_DEFINE_bool(name, default_val, help)                                        \
namespace CommandLineFlags {                                                        \
namespace BoolFlags {                                                               \
    static bool FLAGS_##name {default_val};                                         \
    static std::string FLAGS_HELP_##name {help};                                    \
    static FlagRegesterer<bool> name##_obj {#name, FLAGS_##name};                   \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define HIEBPF_DEFINE_int8(name, default_val, help)                                        \
namespace CommandLineFlags {                                                        \
namespace Int8Flags {                                                               \
    static int8_t FLAGS_##name {default_val};                                       \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::Int8Flags::FLAGS_##name;

#define HIEBPF_DEFINE_int16(name, default_val, help)                                       \
namespace CommandLineFlags {                                                        \
namespace Int16Flags {                                                              \
    static int16_t FLAGS_##name {default_val};                                      \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::Int16Flags::FLAGS_##name;

#define HIEBPF_DEFINE_int32(name, default_val, help)                                       \
namespace CommandLineFlags {                                                        \
namespace Int32Flags {                                                              \
    static int32_t FLAGS_##name {default_val};                                      \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::Int32Flags::FLAGS_##name;

#define HIEBPF_DEFINE_int64(name, default_val, help)                                       \
namespace CommandLineFlags {                                                        \
namespace Int64Flags {                                                              \
    static int64_t FLAGS_##name {default_val};                                      \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::Int64Flags::FLAGS_##name;

#define HIEBPF_DEFINE_uint8(name, default_val, help)                                       \
namespace CommandLineFlags {                                                        \
namespace Uint8Flags {                                                              \
    static uint8_t FLAGS_##name {default_val};                                      \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::Uint8Flags::FLAGS_##name;

#define HIEBPF_DEFINE_uint16(name, default_val, help)                                      \
namespace CommandLineFlags {                                                        \
namespace Uint16Flags {                                                             \
    static uint16_t FLAGS_##name {default_val};                                     \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::Uint16Flags::FLAGS_##name;

#define HIEBPF_DEFINE_uint32(name, default_val, help)                                      \
namespace CommandLineFlags {                                                        \
namespace Uint32Flags {                                                             \
    static uint32_t FLAGS_##name {default_val};                                     \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::Uint32Flags::FLAGS_##name;

#define HIEBPF_DEFINE_uint64(name, default_val, help)                                      \
namespace CommandLineFlags {                                                        \
namespace Uint64Flags {                                                             \
    static uint64_t FLAGS_##name {default_val};                                     \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::Uint64Flags::FLAGS_##name;

#define HIEBPF_DEFINE_double(name, default_val, help)                                      \
namespace CommandLineFlags {                                                        \
namespace DoubleFlags {                                                             \
    static double FLAGS_##name {default_val};                                       \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::DoubleFlags::FLAGS_##name;

#define HIEBPF_DEFINE_string(name, default_val, help)                                      \
namespace CommandLineFlags {                                                        \
namespace StringFlags {                                                             \
    static std::string FLAGS_##name {default_val};                                  \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::StringFlags::FLAGS_##name;

#define HIEBPF_DEFINE_array(name, type, help)                                              \
namespace CommandLineFlags {                                                        \
namespace ArrayFlags {                                                              \
    using std::string;                                                              \
    static std::vector<type> FLAGS_##name {};                                       \
    static std::string FLAGS_##name##_help {help};                                  \
    static FlagRegesterer name##_obj {#name, FLAGS_##name};                         \
}                                                                                   \
}                                                                                   \
using CommandLineFlags::ArrayFlags::FLAGS_##name;

#define DECLARE_bool(name)                                                          \
extern bool CommandLineFlags::BoolFlags::FLAGS_##name;                              \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_int8(name)                                                          \
extern int8_t CommandLineFlags::BoolFlags::FLAGS_##name;                            \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_int16(name)                                                         \
extern int16_t CommandLineFlags::BoolFlags::FLAGS_##name;                           \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_int32(name)                                                         \
extern int32_t CommandLineFlags::BoolFlags::FLAGS_##name;                           \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_int64(name)                                                         \
extern int64_t CommandLineFlags::BoolFlags::FLAGS_##name;                           \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_uint8(name)                                                         \
extern uint8_t CommandLineFlags::BoolFlags::FLAGS_##name;                           \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_uint16(name)                                                        \
extern uint16_t CommandLineFlags::BoolFlags::FLAGS_##name;                          \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_uint32(name)                                                        \
extern uint32_t CommandLineFlags::BoolFlags::FLAGS_##name;                          \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_uint64(name)                                                        \
extern uint64_t CommandLineFlags::BoolFlags::FLAGS_##name;                          \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_double(name)                                                        \
extern double CommandLineFlags::BoolFlags::FLAGS_##name;                            \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_string(name)                                                        \
extern std::string CommandLineFlags::BoolFlags::FLAGS_##name;                       \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#define DECLARE_array(name, type)                                                   \
using std::string;                                                                  \
extern std::vector<type> CommandLineFlags::BoolFlags::FLAGS_##name;                 \
using CommandLineFlags::BoolFlags::FLAGS_##name;

#endif