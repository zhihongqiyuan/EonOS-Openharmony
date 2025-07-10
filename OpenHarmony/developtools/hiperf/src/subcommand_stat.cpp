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

#define HILOG_TAG "Stat"

#include "subcommand_stat.h"

#include <csignal>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <sys/wait.h>
#include <unistd.h>

#include "debug_logger.h"
#include "hiperf_client.h"
#include "hiperf_hilog.h"
#include "ipc_utilities.h"
#include "utilities.h"

using namespace std::chrono;
const uint16_t ONE_HUNDRED = 100;
const uint16_t THOUSANDS_SEPARATOR = 3;
namespace OHOS {
namespace Developtools {
namespace HiPerf {
const std::string DEFAULT_STAT_FILE = "/data/local/tmp/perf_stat.txt";
// when there are many events, start record will take more time.
const std::chrono::milliseconds CONTROL_WAITREPY_TIMEOUT = 2000ms;
static std::map<pid_t, ThreadInfos> thread_map_;
static bool g_reportCpuFlag = false;
static bool g_reportThreadFlag = false;
static VirtualRuntime g_runtimeInstance;

SubCommandStat::~SubCommandStat()
{
    if (filePtr_ != nullptr) {
        fclose(filePtr_);
        filePtr_ = nullptr;
    }
    CloseClientThread();
}

void SubCommandStat::DumpOptions() const
{
    printf("DumpOptions:\n");
    printf(" targetSystemWide:\t%s\n", targetSystemWide_ ? "true" : "false");
    printf(" selectCpus:\t%s\n", VectorToString(selectCpus_).c_str());
    printf(" timeStopSec:\t%f sec\n", timeStopSec_);
    printf(" timeReportMs:\t%d ms\n", timeReportMs_);
    printf(" selectEvents:\t%s\n", VectorToString(selectEvents_).c_str());
    printf(" selectGroups:\t%s\n", VectorToString(selectGroups_).c_str());
    printf(" noCreateNew:\t%s\n", noCreateNew_ ? "true" : "false");
    printf(" appPackage:\t%s\n", appPackage_.c_str());
    printf(" checkAppMs_:\t%d\n", checkAppMs_);
    printf(" selectPids:\t%s\n", VectorToString(selectPids_).c_str());
    printf(" selectTids:\t%s\n", VectorToString(selectTids_).c_str());
    printf(" restart:\t%s\n", restart_ ? "true" : "false");
    printf(" perCore:\t%s\n", perCpus_ ? "true" : "false");
    printf(" perTread:\t%s\n", perThreads_ ? "true" : "false");
    printf(" verbose:\t%s\n", verboseReport_ ? "true" : "false");
}

bool SubCommandStat::ParseOption(std::vector<std::string> &args)
{
    if (args.size() == 1 && args[0] == "-h") {
        args.clear();
        helpOption_ = true;
        PrintUsage();
        return true;
    }
    if (!Option::GetOptionValue(args, "-a", targetSystemWide_)) {
        HLOGD("get option -a failed");
        return false;
    }
    if (targetSystemWide_ && !IsSupportNonDebuggableApp()) {
        HLOGD("-a option needs root privilege for system wide profiling.");
        printf("-a option needs root privilege for system wide profiling.\n");
        return false;
    }
    if (!Option::GetOptionValue(args, "-c", selectCpus_)) {
        HLOGD("get option -c failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "-d", timeStopSec_)) {
        HLOGD("get option -d failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "-i", timeReportMs_)) {
        HLOGD("get option -i failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "-e", selectEvents_)) {
        HLOGD("get option -e failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "-g", selectGroups_)) {
        HLOGD("get option -g failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "--no-inherit", noCreateNew_)) {
        HLOGD("get option --no-inherit failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "-o", outputFilename_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--app", appPackage_)) {
        HLOGD("get option --app failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "--control", controlCmd_)) {
        return false;
    }
    allowIpc_ = controlCmd_ != CONTROL_CMD_PREPARE;
    std::string err = "";
    if (allowIpc_ && !IsExistDebugByApp(appPackage_, err)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--chkms", checkAppMs_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "-p", selectPids_)) {
        HLOGD("get option -p failed");
        return false;
    }
    inputPidTidArgs_ = selectPids_;
    if (!Option::GetOptionValue(args, "-t", selectTids_)) {
        HLOGD("get option -t failed");
        return false;
    }
    inputPidTidArgs_.insert(inputPidTidArgs_.end(), selectTids_.begin(), selectTids_.end());
    if (!Option::GetOptionValue(args, "--restart", restart_)) {
        HLOGD("get option --restart failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "--per-core", perCpus_)) {
        HLOGD("get option --per-core failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "--per-thread", perThreads_)) {
        HLOGD("get option --per-thread failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "--verbose", verboseReport_)) {
        HLOGD("get option --verbose failed");
        return false;
    }
    return ParseSpecialOption(args);
}

bool SubCommandStat::ParseSpecialOption(std::vector<std::string> &args)
{
    if (!Option::GetOptionTrackedCommand(args, trackedCommand_)) {
        HLOGD("get cmd failed");
        return false;
    }
    if (!args.empty()) {
        HLOGD("redundant option(s)");
        return false;
    }
    return true;
}

void SubCommandStat::PrintUsage()
{
    printf("%s\n", Help().c_str());
}

void SubCommandStat::SetReportFlags(bool cpuFlag, bool threadFlag)
{
    g_reportCpuFlag = cpuFlag;
    g_reportThreadFlag = threadFlag;
}

void SubCommandStat::Report(const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents,
                            FILE* filePtr)
{
    bool isNeedPerCpuTid = false;
    for (const auto &it : countEvents) {
        if (!(it.second->summaries.empty())) {
            isNeedPerCpuTid = true;
            break;
        }
    }
    if (isNeedPerCpuTid) {
        PrintPerHead(filePtr);
        ReportDetailInfos(countEvents, filePtr);
    } else {
        ReportNormal(countEvents, filePtr);
    }
}

void SubCommandStat::PrintPerHead(FILE* filePtr)
{
    // print head
    if (g_reportCpuFlag && g_reportThreadFlag) {
        if (filePtr == nullptr) {
            printf(" %24s  %-30s | %-30s %10s %10s %10s | %-32s | %s\n", "count", "event_name", "thread_name",
                   "pid", "tid", "coreid", "comment", "coverage");
        } else {
            fprintf(filePtr, " %24s  %-30s | %-30s %10s %10s %10s | %-32s | %s\n",
                "count", "event_name", "thread_name", "pid", "tid", "coreid", "comment", "coverage");
        }
        return;
    }
    if (g_reportCpuFlag) {
        if (filePtr == nullptr) {
            printf(" %24s  %-30s | %10s | %-32s | %s\n", "count", "event_name", "coreid", "comment", "coverage");
        } else {
            fprintf(filePtr, " %24s  %-30s | %10s | %-32s | %s\n",
                "count", "event_name", "coreid", "comment", "coverage");
        }
        return;
    }
    if (filePtr == nullptr) {
        printf(" %24s  %-30s | %-30s %10s %10s | %-32s | %s\n", "count", "event_name", "thread_name", "pid", "tid",
               "comment", "coverage");
    } else {
        fprintf(filePtr, " %24s  %-30s | %-30s %10s %10s | %-32s | %s\n", "count", "event_name", "thread_name",
            "pid", "tid", "comment", "coverage");
    }
    return;
}

void SubCommandStat::PrintPerValue(const std::unique_ptr<PerfEvents::ReportSum> &reportSum, const float &ratio,
                                   std::string &configName, FILE* filePtr)
{
    if (reportSum == nullptr) {
        return;
    }
    // print value
    std::string strEventCount = std::to_string(reportSum->eventCountSum);
    for (size_t i = strEventCount.size() >= 1 ? strEventCount.size() - 1 : 0, j = 1; i > 0; --i, ++j) {
        if (j == THOUSANDS_SEPARATOR) {
            j = 0;
            strEventCount.insert(strEventCount.begin() + i, ',');
        }
    }

    std::string commentStr;
    MakeComments(reportSum, commentStr);

    if (g_reportCpuFlag && g_reportThreadFlag) {
        if (filePtr == nullptr) {
            printf(" %24s  %-30s | %-30s %10d %10d %10d | %-32s | (%.0lf%%)\n", strEventCount.c_str(),
                   configName.c_str(), reportSum->threadName.c_str(), reportSum->pid, reportSum->tid, reportSum->cpu,
                   commentStr.c_str(), reportSum->scaleSum * ratio);
        } else {
            fprintf(filePtr, " %24s  %-30s | %-30s %10d %10d %10d | %-32s | (%.0lf%%)\n", strEventCount.c_str(),
                configName.c_str(), reportSum->threadName.c_str(), reportSum->pid, reportSum->tid,
                reportSum->cpu, commentStr.c_str(), reportSum->scaleSum * ratio);
        }
    } else if (g_reportCpuFlag) {
        if (filePtr == nullptr) {
            printf(" %24s  %-30s | %10d | %-32s | (%.0lf%%)\n", strEventCount.c_str(), configName.c_str(),
                   reportSum->cpu, commentStr.c_str(), reportSum->scaleSum * ratio);
        } else {
            fprintf(filePtr, " %24s  %-30s | %10d | %-32s | (%.0lf%%)\n", strEventCount.c_str(), configName.c_str(),
                reportSum->cpu, commentStr.c_str(), reportSum->scaleSum * ratio);
        }
    } else {
        if (filePtr == nullptr) {
            printf(" %24s  %-30s | %-30s %10d %10d | %-32s | (%.0lf%%)\n", strEventCount.c_str(), configName.c_str(),
                   reportSum->threadName.c_str(), reportSum->pid, reportSum->tid, commentStr.c_str(),
                   reportSum->scaleSum * ratio);
        } else {
            fprintf(filePtr, " %24s  %-30s | %-30s %10d %10d | %-32s | (%.0lf%%)\n", strEventCount.c_str(),
                configName.c_str(), reportSum->threadName.c_str(), reportSum->pid, reportSum->tid,
                commentStr.c_str(), reportSum->scaleSum * ratio);
        }
    }
    fflush(stdout);
}

void SubCommandStat::InitPerMap(const std::unique_ptr<PerfEvents::ReportSum> &newPerMap,
                                const PerfEvents::Summary &summary, VirtualRuntime& virtualInstance)
{
    CHECK_TRUE(newPerMap == nullptr, NO_RETVAL, 0, "");
    newPerMap->cpu = summary.cpu;
    if (g_reportCpuFlag && !g_reportThreadFlag) {
        return;
    }
    newPerMap->tid = summary.tid;
    newPerMap->pid = thread_map_.find(summary.tid)->second.pid;
    bool isTid = true;
    if (newPerMap->pid == newPerMap->tid) {
        isTid = false;
    }
    newPerMap->threadName = virtualInstance.ReadThreadName(summary.tid, isTid);
}

void SubCommandStat::GetPerKey(std::string &perKey, const PerfEvents::Summary &summary)
{
    perKey = "";
    if (g_reportCpuFlag) {
        perKey += std::to_string(summary.cpu);
        perKey += "|";
    }
    if (g_reportThreadFlag) {
        perKey += std::to_string(summary.tid);
    }
    return;
}

void SubCommandStat::ReportDetailInfos(
    const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents, FILE* filePtr)
{
    std::string perKey = "";
    std::map<std::string, std::unique_ptr<PerfEvents::ReportSum>> perMaps;
    for (auto event = countEvents.begin(); event != countEvents.end(); ++event) {
        if (event->second == nullptr || event->second->eventCount == 0) {
            continue;
        }
        constexpr float ratio {100.0};
        std::string configName = event->first;
        perMaps.clear();
        for (auto &it : event->second->summaries) {
            GetPerKey(perKey, it);
            if (perMaps.count(perKey) == 0) {
                auto perMap = std::make_unique<PerfEvents::ReportSum>(PerfEvents::ReportSum {});
                InitPerMap(perMap, it, g_runtimeInstance);
                perMaps[perKey] = std::move(perMap);
            }
            if (perMaps[perKey] == nullptr) {
                continue;
            }
            perMaps[perKey]->configName = GetDetailComments(event->second, perMaps[perKey]->commentSum,
                                                            it, configName);
            perMaps[perKey]->eventCountSum += it.eventCount;
            if (it.timeRunning < it.timeEnabled && it.timeRunning != 0) {
                perMaps[perKey]->scaleSum = 1 / (static_cast<double>(it.timeEnabled) / it.timeRunning);
            }
        }
        for (auto iper = perMaps.begin(); iper != perMaps.end(); iper++) {
            PrintPerValue(iper->second, ratio, configName, filePtr);
        }
    }
}

void SubCommandStat::ReportNormal(
    const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents, FILE* filePtr)
{
    // print head
    if (filePtr == nullptr) {
        printf(" %24s  %-30s | %-32s | %s\n", "count", "name", "comment", "coverage");
    } else {
        fprintf(filePtr, " %24s  %-30s | %-32s | %s\n", "count", "name", "comment", "coverage");
    }
    std::map<std::string, std::string> comments;
    GetComments(countEvents, comments);
    for (auto it = countEvents.begin(); it != countEvents.end(); ++it) {
        double scale = 1.0;
        constexpr float ratio {100.0};
        std::string configName = it->first;
        std::string comment = comments[configName];
        std::string strEventCount = std::to_string(it->second->eventCount);
        for (size_t i = strEventCount.size() >= 1 ? strEventCount.size() - 1 : 0, j = 1; i > 0; --i, ++j) {
            if (j == THOUSANDS_SEPARATOR) {
                strEventCount.insert(strEventCount.begin() + i, ',');
                j = 0;
            }
        }
        if (it->second->timeRunning < it->second->timeEnabled && it->second->timeRunning != 0) {
            scale = 1 / (static_cast<double>(it->second->timeEnabled) / it->second->timeRunning);
        }
        if (filePtr == nullptr) {
            printf(" %24s  %-30s | %-32s | (%.0lf%%)\n", strEventCount.c_str(), configName.c_str(),
                   comment.c_str(), scale * ratio);
        } else {
            fprintf(filePtr, " %24s  %-30s | %-32s | (%.0lf%%)\n", strEventCount.c_str(), configName.c_str(),
                comment.c_str(), scale * ratio);
        }
        fflush(stdout);
    }
}

bool SubCommandStat::FindEventCount(const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents,
    const std::string &configName, const __u64 group_id, __u64 &eventCount, double &scale)
{
    auto itr = countEvents.find(configName);
    if (itr != countEvents.end()) {
        eventCount = itr->second->eventCount;
        if (itr->second->id == group_id
            && itr->second->timeRunning < itr->second->timeEnabled
            && itr->second->timeRunning != 0) {
            scale = static_cast<double>(itr->second->timeEnabled) / itr->second->timeRunning;
            return true;
        }
    }
    return false;
}

bool SubCommandStat::FindPerCoreEventCount(PerfEvents::Summary &summary, __u64 &eventCount, double &scale)
{
    eventCount = summary.eventCount;
    if (summary.timeRunning < summary.timeEnabled && summary.timeRunning != 0) {
        scale = static_cast<double>(summary.timeEnabled) / summary.timeRunning;
        return true;
    }
    return false;
}

std::string SubCommandStat::GetCommentConfigName(
    const std::unique_ptr<PerfEvents::CountEvent> &countEvent, std::string eventName)
{
    std::string commentConfigName = "";
    CHECK_TRUE(countEvent == nullptr || eventName.length() == 0, commentConfigName, 1, "countEvent is nullptr");
    if (countEvent->userOnly) {
        commentConfigName = eventName + ":u";
    } else if (countEvent->kernelOnly) {
        commentConfigName = eventName + ":k";
    } else {
        commentConfigName = eventName;
    }
    return commentConfigName;
}

void SubCommandStat::MakeComments(const std::unique_ptr<PerfEvents::ReportSum> &reportSum, std::string &commentStr)
{
    CHECK_TRUE(reportSum == nullptr || reportSum->commentSum == 0, NO_RETVAL, 0, "");
    if (reportSum->configName == "sw-task-clock") {
        commentStr = StringPrintf("%lf cpus used", reportSum->commentSum);
        return;
    }
    if (reportSum->configName == "hw-cpu-cycles") {
        commentStr = StringPrintf("%lf GHz", reportSum->commentSum);
        return;
    }
    if (reportSum->configName == "hw-instructions") {
        commentStr = StringPrintf("%lf cycles per instruction", reportSum->commentSum);
        return;
    }
    if (reportSum->configName == "hw-branch-misses") {
        commentStr = StringPrintf("%lf miss rate", reportSum->commentSum);
        return;
    }

    if (reportSum->commentSum > 1e9) {
        commentStr = StringPrintf("%.3lf G/sec", reportSum->commentSum / 1e9);
        return;
    }
    if (reportSum->commentSum > 1e6) {
        commentStr = StringPrintf("%.3lf M/sec", reportSum->commentSum / 1e6);
        return;
    }
    if (reportSum->commentSum > 1e3) {
        commentStr = StringPrintf("%.3lf K/sec", reportSum->commentSum / 1e3);
        return;
    }
    commentStr = StringPrintf("%.3lf /sec", reportSum->commentSum);
}

std::string SubCommandStat::GetDetailComments(const std::unique_ptr<PerfEvents::CountEvent> &countEvent,
    double &comment, PerfEvents::Summary &summary, std::string &configName)
{
    double running_time_in_sec = 0;
    double main_scale = 1.0;
    bool findRunningTime = FindPercoreRunningTime(summary, running_time_in_sec, main_scale);
    if (configName == GetCommentConfigName(countEvent, "sw-cpu-clock")) {
        comment = 0;
        return "sw-cpu-clock";
    }
    double scale = 1.0;
    if (summary.timeRunning < summary.timeEnabled && summary.timeRunning != 0) {
        scale = static_cast<double>(summary.timeEnabled) / summary.timeRunning;
    }
    if (configName == GetCommentConfigName(countEvent, "sw-task-clock")) {
        comment += countEvent->usedCpus * scale;
        return "sw-task-clock";
    }
    if (configName == GetCommentConfigName(countEvent, "hw-cpu-cycles")) {
        if (findRunningTime) {
            double hz = 0;
            if (abs(running_time_in_sec) > ALMOST_ZERO) {
                hz = summary.eventCount / (running_time_in_sec / scale);
            }
            comment += hz / 1e9;
        }
        return "hw-cpu-cycles";
    }
    if (configName == GetCommentConfigName(countEvent, "hw-instructions") && summary.eventCount != 0) {
        double otherScale = 1.0;
        __u64 cpuCyclesCount = 0;
        bool other = FindPerCoreEventCount(summary, cpuCyclesCount, otherScale);
        if (other || (IsMonitoredAtAllTime(otherScale) && IsMonitoredAtAllTime(scale))) {
            comment += static_cast<double>(cpuCyclesCount) / summary.eventCount;
            return "hw-instructions";
        }
    }
    if (configName == GetCommentConfigName(countEvent, "hw-branch-misses")) {
        double otherScale = 1.0;
        __u64 branchInstructionsCount = 0;
        bool other = FindPerCoreEventCount(summary, branchInstructionsCount, otherScale);
        if ((other || (IsMonitoredAtAllTime(otherScale) && IsMonitoredAtAllTime(scale))) &&
            branchInstructionsCount != 0) {
            comment += (static_cast<double>(summary.eventCount) / branchInstructionsCount) * ONE_HUNDRED;
            return "hw-branch-misses";
        }
    }
    return HandleOtherConfig(comment, summary, running_time_in_sec, scale, findRunningTime);
}

std::string SubCommandStat::HandleOtherConfig(double &comment, PerfEvents::Summary &summary, double running_time_in_sec,
                                              double scale, bool findRunningTime)
{
    comment = 0;
    if (findRunningTime) {
        double rate = 0;
        if (scale != 0) {
            rate = summary.eventCount / (running_time_in_sec / scale);
        }
        comment += rate;
    }
    return "";
}

bool SubCommandStat::IsMonitoredAtAllTime(const double &scale)
{
    constexpr double SCALE_ERROR_LIMIT = 1e-5;
    return (fabs(scale - 1.0) < SCALE_ERROR_LIMIT);
}

void SubCommandStat::GetComments(const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents,
    std::map<std::string, std::string> &comments)
{
    double running_time_in_sec = 0;
    __u64 group_id = 0;
    double main_scale = 1.0;
    bool findRunningTime = FindRunningTime(countEvents, running_time_in_sec, group_id, main_scale);
    for (auto it = countEvents.begin(); it != countEvents.end(); it++) {
        std::string configName = it->first;
        std::string commentConfigName = GetCommentConfigName(it->second, "sw-cpu-clock");
        if (configName == commentConfigName) {
            comments[configName] = "";
            continue;
        }
        double scale = 1.0;
        if (it->second->timeRunning < it->second->timeEnabled && it->second->timeRunning != 0) {
            scale = static_cast<double>(it->second->timeEnabled) / it->second->timeRunning;
        }
        commentConfigName = GetCommentConfigName(it->second, "sw-task-clock");
        if (configName == commentConfigName) {
            double usedCpus = it->second->usedCpus * scale;
            comments[configName] = StringPrintf("%lf cpus used", usedCpus);
            continue;
        }
        commentConfigName = GetCommentConfigName(it->second, "hw-cpu-cycles");
        if (configName == commentConfigName) {
            if (findRunningTime &&
                ((group_id == it->second->id) ||
                 (IsMonitoredAtAllTime(main_scale) && IsMonitoredAtAllTime(scale)))) {
                double hz = 0;
                if (abs(running_time_in_sec) > ALMOST_ZERO) {
                    hz = it->second->eventCount / (running_time_in_sec / scale);
                }
                comments[configName] = StringPrintf("%lf GHz", hz / 1e9);
            } else {
                comments[configName] = "";
            }
            continue;
        }
        commentConfigName = GetCommentConfigName(it->second, "hw-instructions");
        if (configName == commentConfigName && it->second->eventCount != 0) {
            std::string cpuSyclesName = GetCommentConfigName(it->second, "hw-cpu-cycles");
            double otherScale = 1.0;
            __u64 cpuCyclesCount = 0;
            bool other = FindEventCount(countEvents, cpuSyclesName, it->second->id, cpuCyclesCount,
                                        otherScale);
            if (other || (IsMonitoredAtAllTime(otherScale) && IsMonitoredAtAllTime(scale))) {
                double cpi = static_cast<double>(cpuCyclesCount) / it->second->eventCount;
                comments[configName] = StringPrintf("%lf cycles per instruction", cpi);
                continue;
            }
        }
        commentConfigName = GetCommentConfigName(it->second, "hw-branch-misses");
        if (configName == commentConfigName) {
            std::string branchInsName = GetCommentConfigName(it->second, "hw-branch-instructions");
            double otherScale = 1.0;
            __u64 branchInstructionsCount = 0;
            bool other = FindEventCount(countEvents, branchInsName, it->second->id,
                                        branchInstructionsCount, otherScale);
            if ((other || (IsMonitoredAtAllTime(otherScale) && IsMonitoredAtAllTime(scale))) &&
                branchInstructionsCount != 0) {
                double miss_rate =
                    static_cast<double>(it->second->eventCount) / branchInstructionsCount;
                comments[configName] = StringPrintf("%lf miss rate", miss_rate * ONE_HUNDRED);
                continue;
            }
        }
        if (findRunningTime && ((group_id == it->second->id) || (IsMonitoredAtAllTime(main_scale) &&
                                                                 IsMonitoredAtAllTime(scale)))) {
            double rate = it->second->eventCount / (running_time_in_sec / scale);
            if (rate > 1e9) {
                comments[configName] = StringPrintf("%.3lf G/sec", rate / 1e9);
                continue;
            }
            if (rate > 1e6) {
                comments[configName] = StringPrintf("%.3lf M/sec", rate / 1e6);
                continue;
            }
            if (rate > 1e3) {
                comments[configName] = StringPrintf("%.3lf K/sec", rate / 1e3);
                continue;
            }
            comments[configName] = StringPrintf("%.3lf /sec", rate);
        } else {
            comments[configName] = "";
        }
    }
}

bool SubCommandStat::FindRunningTime(
    const std::map<std::string, std::unique_ptr<PerfEvents::CountEvent>> &countEvents,
    double &running_time_in_sec, __u64 &group_id, double &main_scale)
{
    for (auto it = countEvents.begin(); it != countEvents.end(); it++) {
        if ((it->first == "sw-task-clock" || it->first == "sw-task-clock:u" ||
             it->first == "sw-task-clock:k" || it->first == "sw-cpu-clock" ||
             it->first == "sw-cpu-clock:u" || it->first == "sw-cpu-clock:k") &&
            it->second->eventCount != 0u) {
            group_id = it->second->id;
            running_time_in_sec = it->second->eventCount / 1e9;
            if (it->second->timeRunning < it->second->timeEnabled &&
                it->second->timeRunning != 0) {
                main_scale =
                    static_cast<double>(it->second->timeEnabled) / it->second->timeRunning;
            }
            return true;
        }
    }
    return false;
}

bool SubCommandStat::FindPercoreRunningTime(PerfEvents::Summary &summary, double &running_time_int_sec,
                                            double &main_scale)
{
    CHECK_TRUE(summary.eventCount == 0, false, 0, "");
    running_time_int_sec = summary.eventCount / 1e9;
    if (summary.timeRunning < summary.timeEnabled && summary.timeRunning != 0) {
        main_scale = static_cast<double>(summary.timeEnabled) / summary.timeRunning;
    }
    return true;
}

bool SubCommandStat::CheckOptionPidAndApp(std::vector<pid_t> pids)
{
    if (!CheckOptionPid(pids)) {
        printf("Problems finding threads of monitor\n\n");
        printf("Usage: perf stat [<options>] [<command>]\n\n");
        printf("-p <pid>        stat events on existing process id\n");
        printf("-t <tid>        stat events on existing thread id\n");
        return false;
    }
    return true;
}

bool SubCommandStat::CheckOptionPid(std::vector<pid_t> pids)
{
    if (pids.empty()) {
        return true;
    }

    for (auto pid : pids) {
        std::ostringstream oss;
        oss << "/proc/" << pid;
        if (!IsDir(oss.str())) {
            printf("not exit pid %d\n", pid);
            return false;
        }
    }
    return true;
}

void SubCommandStat::SetPerfEvent()
{
    SetReportFlags(perCpus_, perThreads_);
    perfEvents_.SetSystemTarget(targetSystemWide_);
    perfEvents_.SetTimeOut(timeStopSec_);
    perfEvents_.SetTimeReport(timeReportMs_);
    perfEvents_.SetPerCpu(perCpus_);
    perfEvents_.SetPerThread(perThreads_);
    perfEvents_.SetVerboseReport(verboseReport_);
    perfEvents_.SetInherit(!noCreateNew_);
    perfEvents_.SetTrackedCommand(trackedCommand_);
    // set report handle
    perfEvents_.SetStatCallBack(Report);
    perfEvents_.SetStatReportFd(filePtr_);
}

bool SubCommandStat::CreateFifoServer()
{
    char errInfo[ERRINFOLEN] = { 0 };
    if (!perfPipe_.CreateFifoFile()) {
        return false;
    }
    CheckIpcBeforeFork();
    pid_t pid = fork();
    allowIpc_ = true;

    if (pid == -1) {
        strerror_r(errno, errInfo, ERRINFOLEN);
        HLOGE("fork failed. %d:%s", errno, errInfo);
        return false;
    } else if (pid == 0) { // child process
        close(STDIN_FILENO);
        close(STDERR_FILENO);
        isFifoServer_ = true;
        clientPipeOutput_ = open(fifoFileS2C_.c_str(), O_WRONLY);
        if (clientPipeOutput_ == -1) {
            strerror_r(errno, errInfo, ERRINFOLEN);
            HLOGE("open fifo file(%s) failed. %d:%s", fifoFileS2C_.c_str(), errno, errInfo);
            HIPERF_HILOGE(MODULE_DEFAULT, "open fifo file(%{public}s) failed. %d:%s",
                fifoFileS2C_.c_str(), errno, errInfo);
            return false;
        }
        nullFd_ = open("/dev/null", O_WRONLY);
        (void)dup2(nullFd_, STDOUT_FILENO); // redirect stdout to /dev/null
         std::string err = OHOS::Developtools::HiPerf::HandleAppInfo(appPackage_, inputPidTidArgs_);
        if (!err.empty()) {
            ClientCommandResponse(err);
            return false;
        }
    } else {            // parent process
        isFifoClient_ = true;
        int fd = open(fifoFileS2C_.c_str(), O_RDONLY | O_NONBLOCK);
        std::string reply = "";
        if (fd != -1) {
            perfPipe_.WaitFifoReply(fd, CONTROL_WAITREPY_TIMEOUT, reply);
        }
        if (fd == -1 || reply != HiperfClient::ReplyOK) {
            if (reply != HiperfClient::ReplyOK) {
                printf("%s", reply.c_str());
                HLOGE("reply is %s", reply.c_str());
                HIPERF_HILOGE(MODULE_DEFAULT, "reply is %{public}s", reply.c_str());
            }
            HLOGI("fd is %d", fd);
            HIPERF_HILOGI(MODULE_DEFAULT, "fd is %{public}d", fd);
            close(fd);
            if (kill(pid, SIGTERM) != 0) {
                HLOGE("Failed to send SIGTERM: %d", pid);
                HIPERF_HILOGE(MODULE_DEFAULT, "Failed to send SIGTERM to pid: %{public}d", pid);
            }
            // wait for process exit
            if (waitpid(pid, nullptr, 0) == -1) {
                HLOGE("Failed to wait for pid: %d", pid);
                HIPERF_HILOGE(MODULE_DEFAULT, "Failed to wait for pid: %{public}d", pid);
            }
            remove(fifoFileC2S_.c_str());
            remove(fifoFileS2C_.c_str());
            strerror_r(errno, errInfo, ERRINFOLEN);
            printf("create control hiperf counting failed.\n");
            HLOGI("errno is %d:%s", errno, errInfo);
            HIPERF_HILOGI(MODULE_DEFAULT, "errno is %{public}d:%{public}s", errno, errInfo);
            return false;
        }
        close(fd);
        printf("%s control hiperf counting success.\n", restart_ ? "start" : "create");
        printf("stat result will saved in %s.\n", outputFilename_ .c_str());
    }
    return true;
}

bool SubCommandStat::ClientCommandResponse(bool response)
{
    return ClientCommandResponse(response ? HiperfClient::ReplyOK : HiperfClient::ReplyFAIL);
}

bool SubCommandStat::ClientCommandResponse(const std::string& str)
{
    size_t size = write(clientPipeOutput_, str.c_str(), str.size());
    if (size != str.size()) {
        char errInfo[ERRINFOLEN] = { 0 };
        strerror_r(errno, errInfo, ERRINFOLEN);
        HLOGD("Server:%s -> %d : %zd %d:%s", str.c_str(), clientPipeOutput_, size, errno, errInfo);
        return false;
    }
    return true;
}

bool SubCommandStat::IsSamplingRunning()
{
    constexpr int maxWaitTrackingCount = 3000 / 100; // wait 3 second
    int waitTrackingCount = maxWaitTrackingCount;
    while (!perfEvents_.IsTrackRunning()) {
        waitTrackingCount--;
        if (waitTrackingCount <= 0) {
            return false;
        }
        constexpr uint64_t waitTrackingSleepMs = 100;
        std::this_thread::sleep_for(milliseconds(waitTrackingSleepMs));
    }
    return true;
}

void SubCommandStat::InitControlCommandHandlerMap()
{
    controlCommandHandlerMap_.clear();
    controlCommandHandlerMap_.emplace(HiperfClient::ReplyStart, ControlCommandHandler{
        std::bind(&PerfEvents::EnableTracking, &perfEvents_)
    });

    controlCommandHandlerMap_.emplace(HiperfClient::ReplyCheck, ControlCommandHandler{
        std::bind(&SubCommandStat::clientRunning_, this)
    });

    controlCommandHandlerMap_.emplace(HiperfClient::ReplyStop, ControlCommandHandler{
        std::bind(&PerfEvents::StopTracking, &perfEvents_)
    });
}

inline void SubCommandStat::CreateClientThread()
{
    // make a thread wait the other command
    if (clientPipeOutput_ != -1) {
        clientCommandHandle_ = std::thread(&SubCommandStat::ClientCommandHandle, this);
    }
}

void SubCommandStat::ClientCommandHandle()
{
    using namespace HiperfClient;
    CHECK_TRUE(!IsSamplingRunning(), NO_RETVAL, 0, "");
    // tell the caller if Exist
    ClientCommandResponse(true);
    InitControlCommandHandlerMap();

    bool hasRead = true;
    while (clientRunning_) {
        if (isFifoServer_ && hasRead) {
            if (clientPipeInput_ != -1) {
                // after read(), block is disabled, the poll will be waked neven if no data
                close(clientPipeInput_);
            }
            clientPipeInput_ = open(fifoFileC2S_.c_str(), O_RDONLY | O_NONBLOCK);
        }
        struct pollfd pollFd {
            clientPipeInput_, POLLIN, 0
        };
        int polled = poll(&pollFd, 1, CONTROL_WAITREPY_TIMEOUT.count());
        if (polled <= 0) {
            hasRead = false;
            continue;
        }
        hasRead = true;
        std::string command;
        bool exitLoop = false;
        while (!exitLoop) {
            char c;
            ssize_t result = TEMP_FAILURE_RETRY(read(clientPipeInput_, &c, 1));
            if (result <= 0) {
                HLOGD("server :read from pipe file failed");
                HIPERF_HILOGI(MODULE_DEFAULT, "server :read from pipe file failed");
                exitLoop = true;
            }
            command.push_back(c);
            if (c == '\n') {
                exitLoop = true;
            }
        }
        HLOGD("server:new command %s", command.c_str());
        HIPERF_HILOGI(MODULE_DEFAULT, "server:new command : %{public}s", command.c_str());
        DispatchControlCommand(command);
    }
}

void SubCommandStat::DispatchControlCommand(const std::string& command)
{
    auto it = controlCommandHandlerMap_.find(command);
    if (it == controlCommandHandlerMap_.end()) {
        return;
    }

    ControlCommandHandler& handler = it->second;
    bool ret = handler.preProcess();
    ClientCommandResponse(ret);
    handler.postProcess(ret);
}

bool SubCommandStat::ProcessControl()
{
    if (controlCmd_.empty()) {
        return true;
    }
    HIPERF_HILOGI(MODULE_DEFAULT, "control cmd : %{public}s", controlCmd_.c_str());
    perfPipe_.SetFifoFileName(CommandType::STAT, controlCmd_, fifoFileC2S_, fifoFileS2C_);
    if (controlCmd_ == CONTROL_CMD_PREPARE) {
        CHECK_TRUE(!CreateFifoServer(), false, 0, "");
        return true;
    }

    isFifoClient_ = true;
    return perfPipe_.ProcessControlCmd();
}

HiperfError SubCommandStat::CheckStatOption()
{
    if (!CheckRestartOption(appPackage_, targetSystemWide_, restart_, selectPids_)) {
        return HiperfError::CHECK_RESTART_OPTION_FAIL;
    }
    
    // check option
    if (!CheckSelectCpuPidOption()) {
        return HiperfError::CHECK_SELECT_CPU_PID_FAIL;
    }
    if (!CheckOptions(selectPids_)) {
        HLOGV("CheckOptions() failed");
        return HiperfError::CHECK_STAT_OPTION_FAIL;
    }
    if (!CheckAppIsRunning(selectPids_, appPackage_, checkAppMs_)) {
        HLOGV("CheckAppIsRunning() failed");
        return HiperfError::CHECK_APP_RUNNING_FAIL;
    }
    if (!CheckOptionPid(selectPids_)) {
        HLOGV("CheckOptionPid() failed");
        return HiperfError::CHECK_OPTION_PID_FAIL;
    }

    perfEvents_.SetCpu(selectCpus_);
    std::vector<pid_t> pids;
    for (auto selectPid : selectPids_) {
        HLOGD("[OnSubCommand] selectPid %d\n", selectPid);
        std::vector<pid_t> subTids = GetSubthreadIDs(selectPid, thread_map_);
        if (!subTids.empty()) {
            pids.insert(pids.end(), subTids.begin(), subTids.end());
        } else {
            HLOGD("[OnSubCommand] subTids empty for %d\n", selectPid);
        }
    }
    pids.insert(pids.end(), selectTids_.begin(), selectTids_.end());
    perfEvents_.SetPid(pids);
    if (!CheckOptionPidAndApp(pids)) {
        HLOGV("CheckOptionPidAndApp() failed");
        return HiperfError::CHECK_OPTION_PID_APP_FAIL;
    }
    std::string err = "";
    if (allowIpc_ && !IsExistDebugByPid(inputPidTidArgs_, err)) {
        return HiperfError::CHECK_OPTION_PID_APP_FAIL;
    }
    return HiperfError::NO_ERR;
}

HiperfError SubCommandStat::OnSubCommand(std::vector<std::string>& args)
{
    CHECK_TRUE(HelpOption(), HiperfError::NO_ERR, 0, "");
    if (!CheckOutPutFile()) {
        return HiperfError::CHECK_OUT_PUT_ERROR;
    }
    if (!ParseControlCmd(controlCmd_)) {
        return HiperfError::WRONG_CONTROL_CMD;
    }
    if (controlCmd_.empty() || controlCmd_ == CONTROL_CMD_PREPARE) {
        HiperfError errorCode = CheckStatOption();
        if (errorCode != HiperfError::NO_ERR) {
            return errorCode;
        }
    }
    if (!ProcessControl()) {
        return HiperfError::PROCESS_CONTROL_FAIL;
    } else if (isFifoClient_) {
        return HiperfError::NO_ERR;
    }
    SetPerfEvent();
    if (!PrepairEvents()) {
        HLOGV("PrepairEvents() failed");
        return HiperfError::PREPAIR_EVENTS_FAIL;
    }

    // preapare fd
    perfEvents_.PrepareTracking();
    CreateClientThread();
    // start tracking
    if (restart_ && controlCmd_ == CONTROL_CMD_PREPARE) {
        RETURN_IF(!perfEvents_.StartTracking(isFifoServer_), HiperfError::PREPARE_START_TRACKING_FAIL);
    } else {
        RETURN_IF(!perfEvents_.StartTracking((!isFifoServer_) && (clientPipeInput_ == -1)),
                  HiperfError::START_TRACKING_FAIL);
    }
    CloseClientThread();
    return HiperfError::NO_ERR;
}

void SubCommandStat::CloseClientThread()
{
    if (clientCommandHandle_.joinable()) {
        clientRunning_ = false;
        HLOGI("CloseClientThread");
        if (nullFd_ != -1) {
            close(nullFd_);
        }
        clientCommandHandle_.join();
        close(clientPipeInput_);
        close(clientPipeOutput_);
        if (isFifoServer_) {
            remove(fifoFileC2S_.c_str());
            remove(fifoFileS2C_.c_str());
        }
    }
}

bool SubCommandStat::ParseControlCmd(const std::string& cmd)
{
    if (cmd.empty() || cmd == CONTROL_CMD_PREPARE || cmd == CONTROL_CMD_START || cmd == CONTROL_CMD_STOP) {
        return true;
    }

    printf("Invalid --control %s option, command should be: prepare, start, stop.\n", cmd.c_str());
    return false;
}

bool RegisterSubCommandStat()
{
    return SubCommand::RegisterSubCommand("stat", SubCommandStat::GetInstance);
}

bool SubCommandStat::PrepairEvents()
{
    if (selectEvents_.empty() && selectGroups_.empty()) {
        perfEvents_.AddDefaultEvent(PERF_TYPE_HARDWARE);
        perfEvents_.AddDefaultEvent(PERF_TYPE_SOFTWARE);
    } else {
        for (auto events : selectEvents_) {
            if (!perfEvents_.AddEvents(events)) {
                HLOGV("add events failed");
                return false;
            }
        }
        for (auto events : selectGroups_) {
            if (!perfEvents_.AddEvents(events, true)) {
                HLOGV("add groups failed");
                return false;
            }
        }
    }
    return true;
}

bool SubCommandStat::CheckSelectCpuPidOption()
{
    if (!selectCpus_.empty()) {
        // the only value is not -1
        if (!(selectCpus_.size() == 1 && selectCpus_.front() == -1)) {
            int maxCpuid = sysconf(_SC_NPROCESSORS_CONF) - 1;
            for (auto cpu : selectCpus_) {
                if (cpu < 0 || cpu > maxCpuid) {
                    printf("Invalid -c value '%d', the CPU ID should be in 0~%d \n", cpu, maxCpuid);
                    return false;
                }
            }
        }
    } else {
        // the cpu default -1
        if (!targetSystemWide_) {
            selectCpus_.push_back(-1);
        }
    }

    if (!selectPids_.empty()) {
        for (auto pid : selectPids_) {
            if (pid <= 0) {
                printf("Invalid -p value '%d', the pid should be larger than 0\n", pid);
                return false;
            }
        }
    }
    if (!selectTids_.empty()) {
        for (auto tid : selectTids_) {
            if (tid <= 0) {
                printf("Invalid -t value '%d', the tid should be larger than 0\n", tid);
                return false;
            }
        }
    }
    return true;
}

bool SubCommandStat::CheckOptions(const std::vector<pid_t> &pids)
{
    if (targetSystemWide_) {
        if (!pids.empty() || !selectTids_.empty()) {
            printf("You cannot specify -a and -t/-p at the same time\n");
            return false;
        }
        if (!appPackage_.empty()) {
            printf("You cannot specify -a and --app at the same time\n");
            return false;
        }
    }
    if (!appPackage_.empty() && (!pids.empty() || !selectTids_.empty())) {
        printf("You cannot specify --app and -t/-p at the same time\n");
        return false;
    }
    if (!targetSystemWide_ && trackedCommand_.empty() && pids.empty() && appPackage_.empty()
        && selectTids_.empty()) {
        printf("You need to set the -p option or --app option.\n");
        return false;
    }
    if (targetSystemWide_ && !trackedCommand_.empty()) {
        printf("You cannot specify -a and a cmd at the same time\n");
        return false;
    }
    if (!trackedCommand_.empty()) {
        if (!pids.empty() || !selectTids_.empty()) {
            printf("You cannot specify a cmd and -t/-p at the same time\n");
            return false;
        }
        if (!appPackage_.empty()) {
            printf("You cannot specify a cmd and --app at the same time\n");
            return false;
        }
        if (!IsRoot()) {
            printf("%s options needs root privilege, please check usage\n",
                   VectorToString(trackedCommand_).c_str());
            return false;
        }
    }
    if (checkAppMs_ < MIN_CHECK_APP_MS || checkAppMs_ > MAX_CHECK_APP_MS) {
        printf("Invalid --chkms value '%d', the milliseconds should be in %d~%d \n", checkAppMs_,
               MIN_CHECK_APP_MS, MAX_CHECK_APP_MS);
        return false;
    }
    if (timeStopSec_ < 0) {
        printf("monitoring duration should be positive but %f is given\n", timeStopSec_);
        return false;
    }
    if (timeReportMs_ < 0) {
        printf("print interval should be non-negative but %d is given\n", timeReportMs_);
        return false;
    }
    return true;
}

bool SubCommandStat::CheckOutPutFile()
{
    if (controlCmd_ != CONTROL_CMD_PREPARE) {
        if (!outputFilename_.empty()) {
            printf("-o option must use with --control prepare option\n");
            return false;
        } else {
            return true;
        }
    }
    if (outputFilename_.empty()) {
        outputFilename_ = DEFAULT_STAT_FILE;
    }
    if (!IsValidOutPath(outputFilename_)) {
        printf("Invalid output file path, permission denied\n");
        return false;
    }
    std::string resolvedPath = CanonicalizeSpecPath(outputFilename_.c_str());
    filePtr_ = fopen(resolvedPath.c_str(), "w");
    if (filePtr_ == nullptr) {
        printf("unable open file to '%s' because '%d'\n", outputFilename_.c_str(), errno);
        return false;
    }
    return true;
}

void SubCommandStat::AddReportArgs(CommandReporter& reporter)
{
    if (targetSystemWide_) {
        reporter.targetProcess_ = "ALL";
    } else if (!appPackage_.empty()) {
        reporter.targetProcess_ = appPackage_;
    } else {
        std::unordered_set<std::string> processNames = {};
        for_each(selectPids_.begin(), selectPids_.end(), [&processNames] (const pid_t& pid) {
            processNames.insert(GetProcessName(pid));
        });
        reporter.targetProcess_ = SetToString<std::string>(processNames);
    }
}

SubCommand& SubCommandStat::GetInstance()
{
    static SubCommandStat subCommand;
    return subCommand;
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
