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
#define HILOG_TAG "Report"

#include "report.h"

#include <memory>
#include <set>
#include <sstream>

#if defined(is_mingw) && is_mingw
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#include "hiperf_hilog.h"

using namespace std::placeholders;
namespace OHOS {
namespace Developtools {
namespace HiPerf {
unsigned long long ReportItem::allIndex_ = 0;
void Report::AddReportItem(const PerfRecordSample &sample, bool includeCallStack)
{
    size_t configIndex = GetConfigIndex(sample.data_.id);
    HLOG_ASSERT_MESSAGE(configs_.size() > configIndex,
                        "in %zu configs found index %zu, from ids %llu", configs_.size(),
                        configIndex, sample.data_.id);
    VirtualThread &thread = virtualRuntime_.GetThread(sample.data_.pid, sample.data_.tid);
    HLOG_ASSERT(sample.callFrames_.size() > 0);
    // if we need callstack ?
    if (includeCallStack) {
        // we will use caller mode , from last to first
        auto frameIt = sample.callFrames_.rbegin();
        ReportItem &item = configs_[configIndex].reportItems_.emplace_back(
            sample.data_.pid, sample.data_.tid, thread.name_, frameIt->mapName,
            frameIt->funcName, frameIt->funcOffset, sample.data_.period);
        HLOGD("ReportItem: %s", item.ToDebugString().c_str());
        HLOG_ASSERT(!item.func_.empty());

        std::vector<ReportItemCallFrame> *currentCallFrames = &item.callStacks_;
        for (frameIt = sample.callFrames_.rbegin(); frameIt != sample.callFrames_.rend();
                frameIt++) {
            HLOG_ASSERT(frameIt->pc < PERF_CONTEXT_MAX);
            // in add items case , right one should only have 1 callstack
            // so just new callfames and move to next level
            ReportItemCallFrame &nextCallFrame = currentCallFrames->emplace_back(
                frameIt->funcName, frameIt->funcOffset, frameIt->mapName,
                sample.data_.period,
                (std::next(frameIt) == sample.callFrames_.rend()) ? sample.data_.period : 0);
            HLOGV("add callframe %s", nextCallFrame.ToDebugString().c_str());
            currentCallFrames = &nextCallFrame.childs;
        }
        HLOGV("callstack %zu", item.callStacks_.size());
        if (item.callStacks_.size() > 0) {
            HLOGV("callstack 2nd level %zu", item.callStacks_[0].childs.size());
        }
    } else {
        auto frameIt = sample.callFrames_.begin();
        if (frameIt != sample.callFrames_.end()) {
            HLOG_ASSERT(frameIt->pc < PERF_CONTEXT_MAX);
            // for arkjs frame, skip the stub.an frame
            if (StringEndsWith(frameIt->mapName, "stub.an") && sample.callFrames_.size() > 1) {
                HLOGV("stub.an frame, go to next, mapname %s", frameIt->mapName.c_str());
                frameIt++;
            }
            ReportItem &item = configs_[configIndex].reportItems_.emplace_back(
                sample.data_.pid, sample.data_.tid, thread.name_, frameIt->mapName,
                frameIt->funcName, frameIt->funcOffset, sample.data_.period);
            HLOGV("%s", item.ToDebugString().c_str());
            HLOG_ASSERT(!item.func_.empty());
        }
    }
    configs_[configIndex].sampleCount_++;
    configs_[configIndex].eventCount_ += sample.data_.period;
}

void Report::AddReportItemBranch(const PerfRecordSample &sample)
{
    size_t configIndex = GetConfigIndex(sample.data_.id);
    HLOG_ASSERT(configs_.size() > configIndex);
    VirtualThread &thread = virtualRuntime_.GetThread(sample.data_.pid, sample.data_.tid);
    for (u64 i = 0; i < sample.data_.bnr; i++) {
        DfxSymbol symbolTo =
            virtualRuntime_.GetSymbol(sample.data_.lbr[i].to, sample.data_.pid, sample.data_.tid);
        DfxSymbol symbolFrom =
            virtualRuntime_.GetSymbol(sample.data_.lbr[i].from, sample.data_.pid, sample.data_.tid);

        // branch only have 1 time only for period
        ReportItem &item = configs_[configIndex].reportItems_.emplace_back(
            sample.data_.pid, sample.data_.tid, thread.name_, symbolTo.module_, symbolTo.GetName(),
            symbolTo.funcVaddr_, 1u);

        item.fromDso_ = symbolFrom.module_;
        item.fromFunc_ = symbolFrom.GetName();

        HLOGV("%s 0x%" PRIx64 "", item.ToDebugString().c_str(), symbolTo.taskVaddr_);
    }
    configs_[configIndex].sampleCount_++;
    configs_[configIndex].eventCount_ += sample.data_.bnr;
}

void Report::StatisticsRecords()
{
    for (auto &config : configs_) {
        size_t duplicates = 0;
        size_t totalReportCount = config.reportItems_.size();
        // merge duplicate
        HLOGD("uniquing %zu", totalReportCount);
        auto last = std::unique(config.reportItems_.begin(), config.reportItems_.end(),
            [this] (ReportItem &l, ReportItem &r) -> bool {
                return this->MultiLevelSameAndUpdateCount(l, r);
            });

        config.reportItems_.erase(last, config.reportItems_.end());

        duplicates = totalReportCount - config.reportItems_.size();
        HLOGD("duplicates %zu, %zu -> %zu", duplicates, totalReportCount,
              config.reportItems_.size());
    }
}

void Report::FilterDisplayRecords()
{
    // remove the item with not in fliter
    for (auto &config : configs_) {
        size_t filterOuts = 0;
        size_t totalReportCount = config.reportItems_.size();
        for (const auto &reportKeyPair : reportKeyMap_) {
            auto reportKey = reportKeyPair.second;
            if (reportKey.displayFilter_.size() != 0) {
                auto itemIt = config.reportItems_.begin();
                while (itemIt != config.reportItems_.end()) {
                    if (!reportKey.ShouldDisplay(*itemIt)) {
                        HLOGM("filter out %s", itemIt->ToDebugString().c_str());

                        // we need recalc the heating ,so also remove in total count
                        config.eventCount_ -= itemIt->eventCount_;

                        // after update total eventCount remove this
                        itemIt = config.reportItems_.erase(itemIt);
                        filterOuts++;
                    } else {
                        itemIt++;
                    }
                }
            }
        }
        HLOGD("filter out %zu, %zu -> %zu", filterOuts, totalReportCount,
              config.reportItems_.size());
    }
}

void Report::UpdateReportItemsAfterAdjust()
{
    for (auto &config : configs_) {
        HLOGV("percentage %zu items", config.reportItems_.size());
        uint64_t totalEventCount = 0; // just for debug check
        for (auto &item : config.reportItems_) {
            item.heat = Percentage(item.eventCount_, config.eventCount_);
            totalEventCount += item.eventCount_;
            HLOGM("%s percentage from %5.2f%% %" PRIu64 "/ %" PRIu64 "",
                  item.ToDebugString().c_str(), item.heat, item.eventCount_, config.eventCount_);
            for (auto keyPair : reportKeyMap_) {
                reportKeyMap_.at(keyPair.first).UpdateValueMaxLen(keyPair.second.GetValue(item));
            }
        }
        // check again
        HLOGV("recalc totalEventCount is %" PRIu64 " old totalEventCount is %" PRIu64 "",
              totalEventCount, config.eventCount_);
        HLOG_ASSERT(totalEventCount == config.eventCount_);
    }
}

void Report::AdjustReportItems()
{
    HLOGD("Adjust Record Order ....");
    for (auto &config : configs_) {
        uint64_t totalReportCount = config.reportItems_.size();
        if (option_.debug_) {
            for (auto &reportItem : config.reportItems_) {
                HLOGV("reportItem %s", reportItem.ToDebugString().c_str());
            }
        }
        // sort first.
        HLOGD("MultiLevelSorting %" PRIu64 "", totalReportCount);
        std::sort(config.reportItems_.begin(), config.reportItems_.end(),
            [this] (const ReportItem &a, const ReportItem &b) -> bool {
                return this->MultiLevelSorting(a, b);
            });
        HLOGD("MultiLevelSorting %" PRIu64 " done", totalReportCount);
        // reorder the callstack
        if (option_.debug_) {
            for (auto &reportItem : config.reportItems_) {
                HLOGV("reportItem %s", reportItem.ToDebugString().c_str());
            }
        }
        StatisticsRecords();
        FilterDisplayRecords();

        // reorder by count
        std::sort(config.reportItems_.begin(), config.reportItems_.end(),
                  &ReportItem::CompareSortingEventCount);

        // reorder the callstack
        for (auto &reportItem : config.reportItems_) {
            ReportItemCallFrame::OrderCallFrames(reportItem.callStacks_);
        }
        HLOGD("afater sorting and unique, we have %zu report items,", config.reportItems_.size());
    }
    // udpate percentage
    UpdateReportItemsAfterAdjust();
}

int Report::MultiLevelCompare(const ReportItem &a, const ReportItem &b)
{
    HLOGM("MultiLevelCompare %s vs %s sort order %s", a.ToDebugString().c_str(),
          b.ToDebugString().c_str(), VectorToString(option_.sortKeys_).c_str());

    // check each key user care
    for (auto it = option_.sortKeys_.begin(); it != option_.sortKeys_.end(); ++it) {
        int result = reportKeyMap_.at(*it).compareFunction_(a, b);
        if (result == 0) {
            // this key is same , check  the next one
            continue;
        } else {
            // if onekey is not same ,  returl as not same
            HLOGM("not same because %s %d : %s vs %s", it->c_str(), result,
                  reportKeyMap_.at(*it).GetValue(a).c_str(),
                  reportKeyMap_.at(*it).GetValue(b).c_str());
            return result;
        }
    }
    // all the key is same
    return 0;
}

bool Report::MultiLevelSame(const ReportItem &a, const ReportItem &b)
{
    return MultiLevelCompare(a, b) == 0;
}

void Report::MergeCallFrameCount(ReportItem &leftItem, ReportItem &rightItem)
{
    // add to left (right to left)
    std::vector<ReportItemCallFrame> *leftCallFrames = &leftItem.callStacks_;
    const std::vector<ReportItemCallFrame> *rightCallFrames = &rightItem.callStacks_;
    uint64_t maxEventCount = leftItem.eventCount_;
    // right should only have one call stack
    int level = 0;
    while (rightCallFrames->size() != 0) {
        HLOG_ASSERT(rightCallFrames->size() == 1u);
        const ReportItemCallFrame &rightFrame = rightCallFrames->at(0);
        auto leftFrameIt = std::find(leftCallFrames->begin(), leftCallFrames->end(), rightFrame);
        if (leftFrameIt == leftCallFrames->end()) {
            // new callfames
            auto &leftCallFrame = leftCallFrames->emplace_back(rightFrame);
            HLOGV("%*s create frame %s in %s", level, "", leftCallFrame.ToDebugString().c_str(),
                  leftItem.ToDebugString().c_str());
            HLOG_ASSERT(leftCallFrame.eventCount_ <= maxEventCount);
            // this is a new call stack ,
            // all the child in rightFrame has been copy to left.
            break;
        } else {
            // already have , add count
            leftFrameIt->eventCount_ += rightFrame.eventCount_;
            leftFrameIt->selfEventCount_ += rightFrame.selfEventCount_;
            // left move to next
            leftCallFrames = &(leftFrameIt->childs);
            HLOGM("%*s udpate frame +%" PRIu64 " %s in %s", level, "", rightFrame.eventCount_,
                  leftFrameIt->ToDebugString().c_str(), leftItem.ToDebugString().c_str());
            HLOG_ASSERT_MESSAGE(leftFrameIt->eventCount_ <= maxEventCount,
                                " maxEventCount is %" PRIu64 "", maxEventCount);
            maxEventCount = leftFrameIt->eventCount_;
        }
        // move to next level
        rightCallFrames = &(rightFrame.childs);
        level++;
    }
}

bool Report::MultiLevelSameAndUpdateCount(ReportItem &l, ReportItem &r)
{
    if (MultiLevelCompare(l, r) == 0) {
        l.eventCount_ += r.eventCount_;
        HLOGM("l %" PRIu64 " %s c:%zu vs r %" PRIu64 " %s c:%zu", l.eventCount_, l.func_.data(),
              l.callStacks_.size(), r.eventCount_, r.func_.data(), r.callStacks_.size());
        // if it have call stack?
        if (r.callStacks_.size() != 0) {
            // add to left (right to left)
            MergeCallFrameCount(l, r);
        }
        return true;
    } else {
        return false;
    }
}

bool Report::MultiLevelSorting(const ReportItem &a, const ReportItem &b)
{
    /*
    The value returned indicates whether the element passed as first argument is
    considered to go before the second in the specific strict weak ordering it defines.
    */
    bool result = MultiLevelCompare(a, b) > 0;
#ifdef HIPERF_DEBUG
    if (DebugLogger::GetInstance()->GetLogLevel() <= LEVEL_VERBOSE) {
        bool result2 = MultiLevelCompare(b, a) > 0;
        if (result && result == result2) {
            HLOGE("MultiLevelSorting a->b %d vs b->a %d", result, result2);
            HLOGE("left %s", a.ToDebugString().c_str());
            HLOGE("right %s", b.ToDebugString().c_str());
            HLOG_ASSERT(false);
        }
    }
#endif
    return result;
}

void Report::OutputStdStatistics(ReportEventConfigItem &config)
{
    if (fprintf(output_, "\n") < 0) {
        return;
    } // make a blank line for new event
    if (fprintf(output_, "Event: %s (type %" PRIu32 " id %" PRIu64 ")\n", config.eventName_.c_str(),
            config.type_, config.config_) < 0) {
        return;
    }
    if (fprintf(output_, "Samples Count: %" PRIu64 "\n", config.sampleCount_) < 0) {
        return;
    }
    if (!config.coutMode_) {
        fprintf(output_, "Time in ns: ");
    } else {
        fprintf(output_, "Event Count: ");
    }
    fprintf(output_, "%" PRIu64 "\n", config.eventCount_);
}

void Report::OutputStdHead(ReportEventConfigItem &config, bool diffMode)
{
    // head print
    const std::string head = "Heating";
    if (fprintf(output_, "%-*s ", FULL_PERCENTAGE_LEN, head.c_str()) < 0) {
        return;
    }

    if (diffMode) {
        const std::string diff = "Diff";
        fprintf(output_, "%-*s ", FULL_PERCENTAGE_DIFF_LEN, diff.c_str());
    }

    // merge sort key and no-sort key (like count)

    displayKeyNames_ = option_.sortKeys_;
    if (!option_.hideCount_) {
        displayKeyNames_.insert(displayKeyNames_.begin(), "count");
    }

    unsigned int remainingWidth = consoleWidth_;
    // sort key head
    for (auto &keyName : displayKeyNames_) {
        auto &key = reportKeyMap_.at(keyName);
        remainingWidth -= key.maxLen_;
        if (remainingWidth == 0) {
            key.maxLen_ = 0;
        }
        if (fprintf(output_, "%-*s ", (remainingWidth > 0) ? static_cast<unsigned int>(key.maxLen_) : 0,
            key.keyName_.c_str()) < 0) {
            return;
        }
        HLOGD("'%s' max len %zu(from '%s') console width %d", key.keyName_.c_str(), key.maxLen_,
              key.maxValue_.c_str(), remainingWidth);
    }
    if (fprintf(output_, "\n") < 0) {
        return;
    }
}

bool Report::OutputStdCallFrame(int indent, const std::string_view &funcName, uint64_t eventCount,
                                uint64_t totalEventCount)
{
    float heat = Percentage(eventCount, totalEventCount);
    float num = 100.0;
    HLOGV("frame %f indent %d at %s", heat, indent, funcName.data());

    CHECK_TRUE(heat < option_.callStackHeatLimit_, false, 0, ""); // don't print this three anymore

    if (abs(heat - num) < ALMOST_ZERO) {
        fprintf(output_, "%*s", indent, "   ");
        fprintf(output_, "%*s  ", FULL_PERCENTAGE_NUM_LEN, " ");
    } else {
        fprintf(output_, "%*s", indent, "|- ");
        fprintf(output_, "%*.2f%% ", FULL_PERCENTAGE_NUM_LEN, heat);
    }
    if (option_.debug_) {
        fprintf(output_, "%" PRIu64 "/%" PRIu64 " %s\n", eventCount, totalEventCount,
                funcName.data());
    } else {
        fprintf(output_, "%s\n", funcName.data());
    }
    return true;
}

void Report::PrepareConsole()
{
#if defined(is_mingw) && is_mingw
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    consoleWidth_ = static_cast<unsigned int>(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(handle, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(handle, mode);
#else
    struct winsize w = {0, 0, 0, 0};
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    consoleWidth_ = static_cast<unsigned int>(w.ws_col);
#endif
    if (consoleWidth_ == 0) {
        consoleWidth_ = ConsoleDefaultWidth;
    }
    HLOGD("consoleWidth_:%d", consoleWidth_);
}

void Report::OutputStdCallFrames(int indent, const ReportItemCallFrame &callFrame, uint64_t totalEventCount)
{
    /*
    90% a
        |- 80% b
            c
            d
            |- 50% e
            |- 50% f
                   g
    */
    // this is the first call frame
    // this tree will skipped.
    CHECK_TRUE(!OutputStdCallFrame(indent, callFrame.func_, callFrame.eventCount_, totalEventCount),
               NO_RETVAL, 0, "");

    // print it self
    if (callFrame.selfEventCount_ != 0 && callFrame.selfEventCount_ != callFrame.eventCount_) {
        OutputStdCallFrame(indent + CALLSTACK_INDENT, "[run in self function]",
                           callFrame.selfEventCount_, callFrame.eventCount_);
    }

    // printf children
    // if only one children
    if (callFrame.childs.size() == 1u &&
        callFrame.childs[0].eventCount_ == callFrame.eventCount_) {
        HLOGV("childCallFream %*c %s", indent, ' ', callFrame.childs[0].func_.data());
        // don't indent if same count (only one 100% children)
        OutputStdCallFrames(indent, callFrame.childs[0], callFrame.eventCount_);
    } else {
        // else a lot children
        for (const ReportItemCallFrame &childCallFrame : callFrame.childs) {
            HLOGV("childCallFream %*c %s", indent, ' ', childCallFrame.func_.data());
            OutputStdCallFrames(indent + CALLSTACK_INDENT, childCallFrame, callFrame.eventCount_);
        }
    }
}

void Report::OutputStdContent(ReportEventConfigItem &config)
{
    // content print
    auto it = config.reportItems_.begin();
    while (it != config.reportItems_.end()) {
        const ReportItem &reportItem = it.operator*();
        // if we need skip it ?
        if (reportItem.heat < option_.heatLimit_) {
            it++;
            continue; // below limit
        } else {
            fprintf(output_, "%*.2f%%  ", FULL_PERCENTAGE_NUM_LEN, reportItem.heat);
        }
        OutputStdContentItem(reportItem);
        if (reportItem.callStacks_.size() != 0) {
            HLOGV("reportItem.callStacks_ %zu %s", reportItem.callStacks_.size(),
                  reportItem.ToDebugString().c_str());
            HLOG_ASSERT(reportItem.callStacks_.size() == 1u);
            for (auto &callFrame : reportItem.callStacks_) {
                OutputStdCallFrames(CALLSTACK_INDENT, callFrame, reportItem.eventCount_);
            }
        }
        it++;
    }
}

void Report::OutputStdContentItem(const ReportItem &reportItem)
{
    // output by sort keys
    for (auto sortKey : displayKeyNames_) {
        ReportKey &reportKey = Report::reportKeyMap_.at(sortKey);
        if (fprintf(output_, "%s ", reportKey.GetValue(reportItem).c_str()) < 0) {
            return;
        }
    }
    if (fprintf(output_, "\n") < 0) {
        return;
    }
}

void Report::OutputStdItemHeating(float heat, float heat2)
{
    if (heat == heat2 && heat == 0.0f) {
        fprintf(output_, "something error , all it is end.\n");
    } else if (heat2 == 0) {
        // only have first
        fprintf(output_, "%*.2f%%  ", FULL_PERCENTAGE_NUM_LEN, heat);
        fprintf(output_, "%*s ", FULL_PERCENTAGE_DIFF_LEN, "");
    } else if (heat == 0) {
        // only have second
        fprintf(output_, "%*s  ", FULL_PERCENTAGE_LEN, "");
        fprintf(output_, "%+*.2f%% ", FULL_PERCENTAGE_DIFF_NUM_LEN, heat2);
    } else if (heat2 > heat) {
        fprintf(output_, "%s%*.2f%%%s  ", TEXT_RED.c_str(), FULL_PERCENTAGE_NUM_LEN, heat,
                TEXT_RESET.c_str());
        fprintf(output_, "%s%+*.2f%%%s ", TEXT_GREEN.c_str(), FULL_PERCENTAGE_DIFF_NUM_LEN,
                heat2 - heat, TEXT_RESET.c_str());
    } else if (heat2 < heat) {
        fprintf(output_, "%s%*.2f%%%s  ", TEXT_GREEN.c_str(), FULL_PERCENTAGE_NUM_LEN, heat,
                TEXT_RESET.c_str());
        fprintf(output_, "%s%+*.2f%%%s ", TEXT_RED.c_str(), FULL_PERCENTAGE_DIFF_NUM_LEN,
                heat2 - heat, TEXT_RESET.c_str());
    } else {
        // same heating
        fprintf(output_, "%*.2f%% ", FULL_PERCENTAGE_NUM_LEN, heat);
        fprintf(output_, "%+*.2f%% ", FULL_PERCENTAGE_DIFF_NUM_LEN, heat2 - heat);
    }
}

void Report::OutputStdContentDiff(ReportEventConfigItem &left, ReportEventConfigItem &right)
{
    // first we need found the match config
    HLOGD("first count %zu second count %zu", left.reportItems_.size(), right.reportItems_.size());
    ReportItemsConstIt it = left.reportItems_.begin();
    ReportItemsConstIt it2 = right.reportItems_.begin();
    while (it != left.reportItems_.end()) {
        // still have it2 ?
        if (it2 != right.reportItems_.end()) {
            // find the same item in it2 by same sort key
            while (it2 != right.reportItems_.end()) {
                if (MultiLevelSame(*it, *it2)) {
                    // we found the same item
                    // output the diff heating
                    if (it->heat > option_.heatLimit_ && it2->heat > option_.heatLimit_) {
                        OutputStdItemHeating(it->heat, it2->heat);
                        OutputStdContentItem(*it);
                    }
                    it++;
                    it2++;
                    break; // next it
                } else {
                    // only print it2 item
                    if (it2->heat > option_.heatLimit_) {
                        OutputStdItemHeating(0.0f, it2->heat);
                        OutputStdContentItem(*it2);
                    }
                    it2++;
                    continue; // next it2
                }
            }
        } else {
            // no more it2, go on print all the it
            if (it->heat > option_.heatLimit_) {
                OutputStdItemHeating(it->heat, 0.0f);
                OutputStdContentItem(*it);
            }
            it++;
            continue; // next it
        }
    }
    while (it2 != right.reportItems_.end()) {
        // if diff still have some item in it2 ,print it
        OutputStdItemHeating(0, it2->heat);
        OutputStdContentItem(*it2);
        it2++;
    }
}

void Report::OutputStd(FILE *output)
{
    output_ = output;
    PrepareConsole();

    for (auto &config : configs_) {
        OutputStdStatistics(config);
        OutputStdHead(config);
        OutputStdContent(config);
    }
}

void Report::OutputStdDiff(FILE *output, Report &other)
{
    output_ = output;
    PrepareConsole();

    auto left = configs_.begin();
    while (left != configs_.end()) {
        auto right = other.configs_.begin();
        while (right != other.configs_.end()) {
            if (*left == *right) {
                OutputStdStatistics(*left);
                OutputStdHead(*left, true);
                OutputStdContentDiff(*left, *right);
                break; // check next left
            }
            right++;
        }
        left++; // go on left
    }
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
