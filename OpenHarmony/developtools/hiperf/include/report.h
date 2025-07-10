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

#ifndef REPORT_H
#define REPORT_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <map>

#include "debug_logger.h"
// remove me latter
#include "report_json_file.h"
#include "utilities.h"
#include "virtual_runtime.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
class ReportItemCallFrame {
public:
    std::string func_;
    uint64_t vaddr_;
    std::string dso_;
    uint64_t eventCount_ = 0;     // call chain event
    uint64_t selfEventCount_ = 0; // call chain event end in this function
    std::vector<ReportItemCallFrame> childs;
    ReportItemCallFrame(std::string func, uint64_t vaddr, std::string dso,
                        uint64_t eventCount, uint64_t selfEventCount)
        : func_(func),
          vaddr_(vaddr),
          dso_(dso),
          eventCount_(eventCount),
          selfEventCount_(selfEventCount)
    {
    }

    bool operator==(const ReportItemCallFrame &b) const
    {
        return Same(b);
    }

    bool operator!=(const ReportItemCallFrame &b) const
    {
        return !Same(b);
    }

    static int CompareSortingEventCount(const ReportItemCallFrame &a, const ReportItemCallFrame &b)
    {
        return a.eventCount_ > b.eventCount_;
    }

    static void OrderCallFrames(std::vector<ReportItemCallFrame> &callframes, int indent = 2)
    {
        int i = 2;
        if (callframes.size() > 0) {
            std::sort(callframes.begin(), callframes.end(),
                      &ReportItemCallFrame::CompareSortingEventCount);

            for (auto &callframe : callframes) {
                HLOGDUMMY("%*s%s", indent, "", callframe.ToDebugString().c_str());
                if (callframe.childs.size() > 0) {
                    OrderCallFrames(callframe.childs, indent + i);
                }
            }
        }
    }

    // just a log
    static void DumpCallFrames(std::vector<ReportItemCallFrame> &callframes, int indent = 2)
    {
        int y = 2;
        if (callframes.size() > 0) {
            for (auto &callframe : callframes) {
                HLOGV("%*s%s", indent, "", callframe.ToDebugString().c_str());
                if (callframe.childs.size() > 0) {
                    DumpCallFrames(callframe.childs, indent + y);
                }
            }
        }
    }

    const std::string ToDebugString() const
    {
        return StringPrintf("%" PRIu64 "(%" PRIu64 ")%s(%s+0x%" PRIx64 ") child %zu", eventCount_,
                            selfEventCount_, func_.data(), dso_.data(), vaddr_, childs.size());
    }

private:
    bool Same(const ReportItemCallFrame &b) const
    {
        return (func_ == b.func_) and (vaddr_ == b.vaddr_) and (dso_ == b.dso_);
    }
};

// one item or one line in report
class ReportItem {
public:
    pid_t pid_ = 0;
    pid_t tid_ = 0;
    std::string_view comm_ = "";
    std::string_view dso_ = "";
    std::string_view fromDso_ = "";
    std::string_view fromFunc_ = "";
    std::string_view func_ = "";
    uint64_t vaddr_ = 0;
    uint64_t eventCount_ = 0; // event count
    std::vector<ReportItemCallFrame> callStacks_;
    float heat = 0.0f;
    static unsigned long long allIndex_; // debug only
    unsigned long long index_;

    // only for ut test
    ReportItem(pid_t pid, pid_t tid, const char *comm, const char *dso, const char *func,
               uint64_t vaddr, uint64_t eventCount)
        : pid_(pid),
          tid_(tid),
          comm_(comm),
          dso_(dso),
          func_(func),
          vaddr_(vaddr),
          eventCount_(eventCount)
    {
        HLOG_ASSERT(comm != nullptr);
        index_ = allIndex_++;
    }

    ReportItem(pid_t pid, pid_t tid, std::string &comm, const std::string_view &dso,
               const std::string_view &func, uint64_t vaddr, uint64_t eventCount)
        : pid_(pid),
          tid_(tid),
          comm_(comm),
          dso_(StringViewHold::Get().Hold(dso)),
          func_(StringViewHold::Get().Hold(func)),
          vaddr_(vaddr),
          eventCount_(eventCount)
    {
        index_ = allIndex_++;
    }

    bool operator==(const ReportItem &b) const
    {
        return Same(b);
    }

    bool operator!=(const ReportItem &b) const
    {
        return !Same(b);
    }

    // debug only
    const std::string ToDebugString() const
    {
        return StringPrintf("%d:%d:%s-%s(%s):%zu i:%llu", pid_, tid_, comm_.data(), func_.data(),
                            dso_.data(), eventCount_, index_);
    }

    // Count
    static int CompareEventCount(const ReportItem &a, const ReportItem &b)
    {
        if (a.eventCount_ != b.eventCount_) {
            return (a.eventCount_ > b.eventCount_) ? 1 : -1;
        } else {
            return 0;
        }
    }

    static int CompareSortingEventCount(const ReportItem &a, const ReportItem &b)
    {
        return a.eventCount_ > b.eventCount_;
    }

    static const std::string GetEventCount(const ReportItem &a, size_t len,
                                           const std::string &format)
    {
        return StringPrintf(format.c_str(), len, a.eventCount_);
    }

    // Pid
    static int ComparePid(const ReportItem &a, const ReportItem &b)
    {
        if (a.pid_ != b.pid_) {
            return (a.pid_ > b.pid_) ? 1 : -1;
        } else {
            return 0;
        }
    }
    static const std::string GetPid(const ReportItem &a, size_t len, const std::string &format)
    {
        return StringPrintf(format.c_str(), len, a.pid_);
    }

    // Tid
    static int CompareTid(const ReportItem &a, const ReportItem &b)
    {
        if (a.tid_ != b.tid_) {
            return (a.tid_ > b.tid_) ? 1 : -1;
        } else {
            return 0;
        }
    }
    static const std::string GetTid(const ReportItem &a, size_t len, const std::string &format)
    {
        return StringPrintf(format.c_str(), len, a.tid_);
    }

    // Comm
    static int CompareComm(const ReportItem &a, const ReportItem &b)
    {
        int result = a.comm_.compare(b.comm_);
        return result;
    }
    static const std::string GetComm(const ReportItem &a, size_t len, const std::string &format)
    {
        return StringPrintf(format.c_str(), len, a.comm_.data());
    }

    // Func
    static int CompareFunc(const ReportItem &a, const ReportItem &b)
    {
        return a.func_.compare(b.func_);
    }
    static const std::string GetFunc(const ReportItem &a, size_t len, const std::string &format)
    {
        return StringPrintf(format.c_str(), len, a.func_.data());
    }

    // Dso
    static int CompareDso(const ReportItem &a, const ReportItem &b)
    {
        return a.dso_.compare(b.dso_);
    }
    static const std::string GetDso(const ReportItem &a, size_t len, const std::string &format)
    {
        return StringPrintf(format.c_str(), len, a.dso_.data());
    }

    // fromDso
    static int CompareFromDso(const ReportItem &a, const ReportItem &b)
    {
        return a.fromDso_.compare(b.fromDso_);
    }
    static const std::string GetFromDso(const ReportItem &a, size_t len, const std::string &format)
    {
        return StringPrintf(format.c_str(), len, a.fromDso_.data());
    }

    // fromFunc
    static int CompareFromFunc(const ReportItem &a, const ReportItem &b)
    {
        return a.fromFunc_.compare(b.fromFunc_);
    }
    static const std::string GetFromFunc(const ReportItem &a, size_t len, const std::string &format)
    {
        return StringPrintf(format.c_str(), len, a.fromFunc_.data());
    }

private:
    bool Same(const ReportItem &b) const
    {
        return (comm_ == b.comm_) && (pid_ == b.pid_) && (tid_ == b.tid_) && (func_ == b.func_) &&
               (dso_ == b.dso_) && (vaddr_ == b.vaddr_);
    }
};

using ReportKeyCompareFunction = int(const ReportItem &, const ReportItem &);
using ReportKeyGetFunction = const std::string(const ReportItem &, size_t, const std::string &);

constexpr const int MAX_FILED_LEN = 20;
constexpr const int CALLSTACK_INDENT = 4;
struct ReportKey {
    const std::string keyName_;
    const std::string valueFormat_;
    size_t maxLen_ = 0u;
    std::string maxValue_;
    ReportKeyCompareFunction &compareFunction_;
    ReportKeyGetFunction &GetFunction_;
    const std::vector<std::string> &displayFilter_;

    ReportKey(const std::string keyName, ReportKeyCompareFunction &compareFunction,
              ReportKeyGetFunction &GetFunction, const std::string valueFormat,
              const std::vector<std::string> &displayFilter)
        : keyName_(keyName),
          valueFormat_(valueFormat),
          compareFunction_(compareFunction),
          GetFunction_(GetFunction),
          displayFilter_(displayFilter)
    {
        maxLen_ = keyName.size();
    }

    void UpdateValueMaxLen(const std::string &value)
    {
        size_t newMaxLen = std::max(maxLen_, value.size());
        if (maxLen_ < newMaxLen) {
            maxValue_ = value;
            maxLen_ = newMaxLen;
        }
    }

    void UpdateValueMaxLen(size_t value)
    {
        size_t newMaxLen = std::max(maxLen_, std::to_string(value).size());
        if (maxLen_ < newMaxLen) {
            maxValue_ = std::to_string(value);
            maxLen_ = newMaxLen;
        }
    }

    std::string GetValue(const ReportItem &i)
    {
        return GetFunction_(i, maxLen_, valueFormat_);
    }

    bool ShouldDisplay(const ReportItem &i)
    {
        if (displayFilter_.size() == 0) {
            return true;
        } else {
            std::string value = GetFunction_(i, 0, valueFormat_);
            auto it = find(displayFilter_.begin(), displayFilter_.end(), value);
            if (it == displayFilter_.end()) {
                HLOGV("  not found '%s' in %s", value.c_str(),
                      VectorToString(displayFilter_).c_str());
            }
            return (it != displayFilter_.end());
        }
    }
};

using ReportItems = std::vector<ReportItem>;
using ReportItemsIt = ReportItems::iterator;
using ReportItemsConstIt = ReportItems::const_iterator;

struct ReportOption {
    float heatLimit_ = 0.0f;
    float callStackHeatLimit_ = 0.0f;

    // display filter
    std::vector<std::string> displayComms_ {};
    std::vector<std::string> displayPids_ {};
    std::vector<std::string> displayTids_ {};
    std::vector<std::string> displayDsos_ {};
    std::vector<std::string> displayFromDsos_ {};
    std::vector<std::string> displayFuncs_ {};
    std::vector<std::string> displayFromFuncs_ {};
    std::vector<std::string> displayDummy_ {};

    std::vector<std::string> sortKeys_ = {"comm", "pid", "tid", "dso", "func"};

    bool debug_ = false;
    bool hideCount_ = false;
};

class Report {
public:
    Report() : option_(defaultOption_), virtualRuntime_(false)
    {
        // works for ut test
    }
    Report(ReportOption &option) : option_(option), virtualRuntime_(false) {}
    bool MultiLevelSame(const ReportItem &a, const ReportItem &b);
    void AdjustReportItems();
    void AddReportItem(const PerfRecordSample &sample, bool includeCallStack);
    void AddReportItemBranch(const PerfRecordSample &sample);
    void OutputStd(FILE *output);
    void OutputStdDiff(FILE *output, Report &other);

    ReportOption &option_;

    VirtualRuntime virtualRuntime_;

    std::map<std::string, ReportKey> reportKeyMap_ = {
        {
            "count",
            {
                "count",
                ReportItem::CompareEventCount,
                ReportItem::GetEventCount,
                "%*" PRIu64 "",
                option_.displayDummy_,
            },
        },
        {
            "comm",
            {
                "comm",
                ReportItem::CompareComm,
                ReportItem::GetComm,
                "%-*s",
                option_.displayComms_,
            },
        },
        {
            "pid",
            {
                "pid",
                ReportItem::ComparePid,
                ReportItem::GetPid,
                "%*d",
                option_.displayPids_,
            },
        },
        {
            "tid",
            {
                "tid",
                ReportItem::CompareTid,
                ReportItem::GetTid,
                "%*d",
                option_.displayTids_,
            },
        },
        {
            "dso",
            {
                "dso",
                ReportItem::CompareDso,
                ReportItem::GetDso,
                "%-*s",
                option_.displayDsos_,
            },
        },
        {
            "from_dso",
            {
                "from_dso",
                ReportItem::CompareFromDso,
                ReportItem::GetFromDso,
                "%-*s",
                option_.displayFromDsos_,
            },
        },
        {
            "func",
            {
                "func",
                ReportItem::CompareFunc,
                ReportItem::GetFunc,
                "%-*s",
                option_.displayFuncs_,
            },
        },
        {
            "from_func",
            {
                "from_func",
                ReportItem::CompareFromFunc,
                ReportItem::GetFromFunc,
                "%-*s",
                option_.displayFromFuncs_,
            },
        },
    };
    struct ReportEventConfigItem {
        ReportEventConfigItem(const ReportEventConfigItem &) = delete;
        ReportEventConfigItem &operator=(const ReportEventConfigItem &) = delete;
        ReportEventConfigItem(ReportEventConfigItem &&) = default;
        ReportEventConfigItem &operator=(ReportEventConfigItem &&) = default;
        std::string eventName_;
        uint64_t sampleCount_ = 0;
        uint64_t eventCount_ = 0;
        std::vector<ReportItem> reportItems_;
        uint32_t type_;
        uint64_t config_;
        std::vector<uint64_t> ids_;

        bool coutMode_ = true; // use cout or time ?
        bool operator==(const ReportEventConfigItem &o) const
        {
            return (type_ == o.type_) && (config_ == o.config_);
        }
        bool operator!=(const ReportEventConfigItem &o) const
        {
            return !(operator==(o));
        }
        std::string toDebugString()
        {
            return StringPrintf("%s(%" PRIu32 "-%" PRIu64 "):PRIu64", eventName_.c_str(), type_,
                                config_, sampleCount_);
        }
        ReportEventConfigItem(std::string eventName, uint32_t type, uint64_t config,
                              bool coutMode = true)
            : eventName_(eventName), type_(type), config_(config), coutMode_(coutMode)
        {
        }
    };
    std::vector<ReportEventConfigItem> configs_;
    virtual ~Report() {}

    std::map<uint64_t, size_t> configIdIndexMaps_; // index of configNames_
    std::string GetConfigName(uint64_t id)
    {
        size_t index = GetConfigIndex(id);
        HIPERF_ASSERT(index < configs_.size(), "unable found config index %zu\n", index);
        return configs_[index].eventName_;
    }
    size_t GetConfigIndex(uint64_t id)
    {
        HIPERF_ASSERT(configIdIndexMaps_.find(id) != configIdIndexMaps_.end(), "unable found id %" PRIx64 "\n", id);
        return configIdIndexMaps_.at(id);
    }

private:
    FILE *output_ = nullptr;
    const std::string TEXT_RED = "\x1b[31m";
    const std::string TEXT_GREEN = "\x1b[32m";
    const std::string TEXT_RESET = "\033[0m";
    const unsigned int ConsoleDefaultWidth = 80;

    // sometime caller don't give the option
    ReportOption defaultOption_;

    std::vector<std::string> displayKeyNames_;

    // use virtual only for gmock test
    bool MultiLevelSorting(const ReportItem &a, const ReportItem &b);
    bool MultiLevelSameAndUpdateCount(ReportItem &l, ReportItem &r);
    void MergeCallFrameCount(ReportItem &leftItem, ReportItem &rightItem);
    virtual int MultiLevelCompare(const ReportItem &a, const ReportItem &b);

    void StatisticsRecords();
    void FilterDisplayRecords();
    void UpdateReportItemsAfterAdjust();

    // std out
    unsigned int consoleWidth_ = 0;
    void PrepareConsole();

    void OutputStdStatistics(ReportEventConfigItem &config);

    void OutputStdHead(ReportEventConfigItem &config, bool diffMode = false);

    void OutputStdContent(ReportEventConfigItem &config);
    void OutputStdContentDiff(ReportEventConfigItem &left, ReportEventConfigItem &right);

    void OutputStdContentItem(const ReportItem &reportItem);

    void OutputStdCallFrames(int indent, const ReportItemCallFrame &callFrame, uint64_t totalEventCount);
    bool OutputStdCallFrame(int indent, const std::string_view &funcName, uint64_t eventCount,
                            uint64_t totalEventCount);
    void OutputStdItemHeating(float heat, float heat2);
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // REPORT_H
