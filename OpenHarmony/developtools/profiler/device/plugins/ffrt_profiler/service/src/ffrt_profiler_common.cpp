/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
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

#include "ffrt_profiler_common.h"

#include "logging.h"

namespace OHOS::Developtools::Profiler {
void SplitString(const std::string& str, const std::string &sep, std::vector<std::string>& ret)
{
    if (str.empty()) {
        PROFILER_LOG_ERROR(LOG_CORE, "The string splited is empty!");
        return;
    }
    std::string::size_type beginPos = str.find_first_not_of(sep);
    std::string::size_type findPos = 0;
    while (beginPos != std::string::npos) {
        findPos = str.find(sep, beginPos);
        std::string tmp;
        if (findPos != std::string::npos) {
            tmp = str.substr(beginPos, findPos - beginPos);
            beginPos = findPos + sep.length();
        } else {
            tmp = str.substr(beginPos);
            beginPos = findPos;
        }
        if (!tmp.empty()) {
            ret.push_back(tmp);
            tmp.clear();
        }
    }
}

std::string GetProcessName(int32_t pid)
{
    std::string path = "/proc/" + std::to_string(pid) + "/cmdline";
    std::ifstream cmdlineFile(path);
    if (!cmdlineFile) {
        return "";
    }

    std::string processName;
    std::getline(cmdlineFile, processName, '\0');

    static constexpr size_t headSize = 2;
    if (processName.substr(0, headSize) == "./") {
        processName = processName.substr(headSize);
    }
    size_t found = processName.rfind("/");
    std::string procName;
    if (found != std::string::npos) {
        procName = processName.substr(found + 1);
    } else {
        procName = processName;
    }
    return procName;
}
}