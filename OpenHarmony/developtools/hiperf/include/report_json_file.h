/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef REPORT_JSON_FILE_H
#define REPORT_JSON_FILE_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <map>

#include "debug_logger.h"
#include "perf_file_reader.h"
#include "utilities.h"
#include "virtual_runtime.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
using jsonStringMap = std::map<std::string, std::string>;
using jsonStringVector = std::vector<std::string>;
using jsonIntVector = std::vector<int>;

template<class T>
void OutputJsonKey(FILE *output, const T &value)
{
    if constexpr (std::is_same<T, std::string>::value) {
        if (value.empty()) {
            // for key vector [] mode, not key is needed
            return;
        }
        fprintf(output, "\"%s\":", value.c_str());
    } else if constexpr (std::is_same<T, std::string_view>::value) {
        if (value.empty()) {
            // for key vector [] mode, not key is needed
            return;
        }
        fprintf(output, "\"%s\":", value.data());
    } else if constexpr (std::is_same<typename std::decay<T>::type, char *>::value) {
        if (value[0] == '\0') {
            // same as value.empty()
            return;
        }
        fprintf(output, "\"%s\":", value);
    } else {
        fprintf(output, "\"%s\":", std::to_string(value).c_str());
    }
}
template<class T>
void OutputJsonValue(FILE *output, const T &value, bool first = true)
{
    if (!first) {
        fprintf(output, ",");
    }
    if constexpr (std::is_same<T, std::string>::value) {
        fprintf(output, "\"%s\"", value.c_str());
    } else if constexpr (std::is_same<T, std::string_view>::value) {
        fprintf(output, "\"%s\"", value.data());
    } else if constexpr (std::is_same<T, int>::value) {
        fprintf(output, "%s", std::to_string(value).c_str());
    } else if constexpr (std::is_same<T, uint64_t>::value) {
        fprintf(output, "%s", std::to_string(value).c_str());
    } else if constexpr (std::is_same<T, bool>::value) {
        fprintf(output, "%s", std::to_string(value).c_str());
    } else if constexpr (std::is_same<T, size_t>::value) {
        fprintf(output, "%s", std::to_string(value).c_str());
    } else if constexpr (std::is_same<typename std::decay<T>::type, char *>::value) {
        fprintf(output, "\"%s\"", value);
    } else {
        value.OutputJson(output);
    }
}

/*
    k:"v"
    k:1
*/
template<class K, class T>
void OutputJsonPair(FILE *output, const K &key, const T &value, bool first = false)
{
    if (!first) {
        if (fprintf(output, ",") < 0) {
            return;
        }
    }
    // for id, symbol
    OutputJsonKey(output, key);
    // ReportFuncMapItem funcName.
    OutputJsonValue(output, value);
}

/*
    k:[v1,v2,v3]
*/
template<class T>
void OutputJsonVectorList(FILE *output, const std::string &key, const std::vector<T> &value,
                          bool first = false)
{
    if (!first) {
        if (fprintf(output, ",") < 0) {
            return;
        }
    }
    if (fprintf(output, "\"%s\":[", key.c_str()) != -1) {
        auto it = value.begin();
        while (it != value.end()) {
            OutputJsonValue(output, *it, it == value.begin());
            it++;
        }
        if (fprintf(output, "]") < 0) {
            return;
        }
    }
}

/*
    k:[v1,v2,v3]
*/
template<class K, class V>
void OutputJsonMapList(FILE *output, const std::string &key, const std::map<K, V> &value,
                       bool first = false)
{
    if (!first) {
        if (fprintf(output, ",") < 0) {
            return;
        }
    }
    if (fprintf(output, "\"%s\":[", key.c_str()) != -1) {
        auto it = value.begin();
        while (it != value.end()) {
            OutputJsonValue(output, it->second, it == value.begin());
            it++;
        }
        if (fprintf(output, "]") < 0) {
            return;
        }
    }
}

/*
    k:{k1:v1,k2:v2,k3:v3}
*/
template<class K, class V>
void OutputJsonMap(FILE *output, const std::string &key, const std::map<K, V> &value,
                   bool first = false)
{
    if (!first) {
        if (fprintf(output, ",") < 0) {
            return;
        }
    }
    if (fprintf(output, "\"%s\":{", key.c_str()) != -1) {
        auto it = value.begin();
        while (it != value.end()) {
            OutputJsonPair(output, it->first, it->second, it == value.begin());
            it++;
        }
        if (fprintf(output, "}") < 0) {
            return;
        }
    }
}

template<class K, class V>
V &GetOrCreateMapItem(std::map<K, V> &map, const K &key)
{
    if (map.count(key) == 0) {
        map.emplace(key, (key));
    }
    return map.at(key);
}

struct ReportFuncMapItem {
    int libId_ = -1;
    std::string funcName_;
    int reportFuncId_ = -1;
    void OutputJson(FILE *output) const
    {
        if (fprintf(output, "{") < 0) {
            return;
        }
        OutputJsonPair(output, "file", libId_, true);
        OutputJsonPair(output, "symbol", funcName_);
        if (fprintf(output, "}") < 0) {
            return;
        }
    }
    ReportFuncMapItem(int libId, std::string &funcName, int reportFuncId)
        : libId_(libId), funcName_(funcName), reportFuncId_(reportFuncId) {}
};

struct ReportFuncItem {
    int functionId_ = -1;
    int functionInLibId_ = -1;
    uint64_t sampleCount_ = 0;
    uint64_t eventCount_ = 0;
    uint64_t subTreeEventCount_ = 0;
    explicit ReportFuncItem(int functionId) : functionId_(functionId) {}
    void OutputJson(FILE *output) const
    {
        if (fprintf(output, "{") < 0) {
            return;
        }
        OutputJsonPair(output, "symbol", functionId_, true);
        OutputJsonVectorList(output, "counts",
                             std::vector<uint64_t> {sampleCount_, eventCount_, subTreeEventCount_});
        if (fprintf(output, "}") < 0) {
            return;
        }
    }
};

struct ReportCallNodeItem {
    uint64_t selfEventCount_ = 0;
    uint64_t subTreeEventCount_ = 0;
    int functionId_ = -1;
    int nodeIndex_ = -1;
    bool reverseCaller_ = false;
    std::string_view funcName_ = "";
    std::string debug_ = "";
    std::map<int, ReportCallNodeItem> childrenMap;

    void OutputJson(FILE *output) const
    {
        if (fprintf(output, "{") < 0) {
            return;
        }
        OutputJsonPair(output, "selfEvents", selfEventCount_, true);
        OutputJsonPair(output, "subEvents", subTreeEventCount_);
        OutputJsonPair(output, "symbol", functionId_);
        if (!funcName_.empty()) { // for debug
            OutputJsonPair(output, "funcName", funcName_);
            OutputJsonPair(output, "nodeIndex", nodeIndex_);
            OutputJsonPair(output, "reversed", reverseCaller_);
        }
        OutputJsonMapList(output, "callStack", childrenMap);
        if (fprintf(output, "}") < 0) {
            return;
        }
    }

    uint64_t UpdateChildrenEventCount()
    {
        subTreeEventCount_ = selfEventCount_;
        for (auto &pair : childrenMap) {
            subTreeEventCount_ += pair.second.UpdateChildrenEventCount();
            if (!funcName_.empty()) {
            }
        }
        return subTreeEventCount_;
    }

    static bool FindByFunctionId(ReportCallNodeItem &a, int functionId)
    {
        return (a.functionId_ == functionId);
    }

    explicit ReportCallNodeItem(int functionId) : functionId_(functionId) {}
};

struct ReportLibItem {
    int libId_ = 0;
    uint64_t eventCount_ = 0;
    std::map<int, ReportFuncItem> funcs_;
    void OutputJson(FILE *output) const
    {
        if (fprintf(output, "{") < 0) {
            return;
        }
        OutputJsonPair(output, "fileId", libId_, true);
        OutputJsonPair(output, "eventCount", eventCount_);
        OutputJsonMapList(output, "functions", funcs_);
        if (fprintf(output, "}") < 0) {
            return;
        }
    }
};

struct ReportThreadItem {
    pid_t tid_ = 0;
    uint64_t eventCount_ = 0;
    uint64_t sampleCount_ = 0;
    std::map<int, ReportLibItem> libs_;
    ReportCallNodeItem callNode;
    ReportCallNodeItem callNodeReverse;
    void OutputJson(FILE *output) const
    {
        if (fprintf(output, "{") < 0) {
            return;
        }
        OutputJsonPair(output, "tid", tid_, true);
        OutputJsonPair(output, "eventCount", eventCount_);
        OutputJsonPair(output, "sampleCount", sampleCount_);
        OutputJsonMapList(output, "libs", libs_);
        OutputJsonPair(output, "CallOrder", callNode);
        OutputJsonPair(output, "CalledOrder", callNodeReverse);
        if (fprintf(output, "}") < 0) {
            return;
        }
    }
    ReportThreadItem(pid_t id) : tid_(id), callNode(-1), callNodeReverse(-1) {}
};

struct ReportProcessItem {
    pid_t pid_ = 0;
    uint64_t eventCount_ = 0;
    std::map<pid_t, ReportThreadItem> threads_;
    void OutputJson(FILE *output) const
    {
        if (fprintf(output, "{") < 0) {
            return;
        }
        OutputJsonPair(output, "pid", pid_, true);
        OutputJsonPair(output, "eventCount", eventCount_);
        OutputJsonMapList(output, "threads", threads_);
        if (fprintf(output, "}") < 0) {
            return;
        }
    }
    explicit ReportProcessItem(pid_t pid) : pid_(pid) {}
};

struct ReportConfigItem {
    int index_;
    std::string eventName_;
    uint64_t eventCount_ = 0;
    std::map<pid_t, ReportProcessItem> processes_;
    void OutputJson(FILE *output) const
    {
        if (fprintf(output, "{") < 0) {
            return;
        }
        OutputJsonPair(output, "eventConfigName", eventName_, true);
        OutputJsonPair(output, "eventCount", eventCount_);
        OutputJsonMapList(output, "processes", processes_);
        if (fprintf(output, "}") < 0) {
            return;
        }
    }
    ReportConfigItem(int index, std::string eventName) : index_(index), eventName_(eventName) {}
};

static constexpr const int keyLibId = 0;
static constexpr const int keyfuncName = 1;

class ReportJsonFile {
public:
    int nodeIndex_ = 0; // debug only
    static bool debug_;
    FILE *output_ = nullptr;
    ReportJsonFile(const std::unique_ptr<PerfFileReader> &recordFileReader,
                   const VirtualRuntime &virtualRuntime)
        : recordFileReader_(recordFileReader), virtualRuntime_(virtualRuntime)
    {
    }

    void UpdateReportSample(uint64_t configid, pid_t pid, pid_t tid, uint64_t eventCount);
    void UpdateReportCallStack(uint64_t id, pid_t pid, pid_t tid, uint64_t eventCount,
                               std::vector<DfxFrame> &frames);
    void UpdateCallNodeEventCount();
    void ProcessSymbolsFiles(const std::vector<std::unique_ptr<SymbolsFile>> &symbolsFiles);

    // json
    bool OutputJson(FILE *output = nullptr);

    std::map<std::vector<uint64_t>, ReportConfigItem> reportConfigItems_;

private:
    const std::unique_ptr<PerfFileReader> &recordFileReader_;
    const VirtualRuntime &virtualRuntime_;
    std::vector<std::string_view> libList_;
    int functionId_ = 0;
    std::map<int, std::map<std::string, ReportFuncMapItem>> functionMap_;
    void AddNewFunction(int libId, std::string name);
    void OutputJsonFunctionMap(FILE *output);

    ReportConfigItem &GetConfig(uint64_t id);
    std::string GetConfigName(uint64_t id);
    uint32_t GetConfigIndex(uint64_t id);

    int GetFunctionID(int libId, const std::string &function);
    int GetLibID(std::string_view filepath);

    void OutputJsonFeatureString();
    void OutputJsonRuntimeInfo();

    void AddReportCallStack(uint64_t eventCount, ReportCallNodeItem &callNode,
                            const std::vector<DfxFrame> &frames);
    void AddReportCallStackReverse(uint64_t eventCount, ReportCallNodeItem &callNode,
                                   const std::vector<DfxFrame> &frames);
    uint64_t sampleCount_ = 0;
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // REPORT_JSON_FILE_H
