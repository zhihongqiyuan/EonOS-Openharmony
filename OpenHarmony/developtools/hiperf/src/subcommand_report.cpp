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

#include "subcommand_report.h"

#include <memory>
#include <set>
#include <sstream>

#if defined(is_mingw) && is_mingw
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#include "hiperf_hilog.h"
#include "perf_events.h"
#include "register.h"
#include "utilities.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
bool SubCommandReport::ParseOption(std::vector<std::string> &args)
{
    if (!Option::GetOptionValue(args, "-i", recordFile_[FIRST])) {
        return false;
    }
    if (!Option::GetOptionValue(args, "-o", reportFile_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--diff", recordFile_[SECOND])) {
        return false;
    }
    if (!recordFile_[SECOND].empty()) {
        // remove tid pid
        reportOption_.sortKeys_ = {"comm", "dso", "func"};
    }
    if (!Option::GetOptionValue(args, "--sort", reportOption_.sortKeys_)) {
        return false;
    }

    if (!Option::GetOptionValue(args, "--symbol-dir", symbolsPaths_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--limit-percent", reportOption_.heatLimit_)) {
        return false;
    }

    if (!Option::GetOptionValue(args, "-s", showCallStack_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--call-stack", showCallStack_)) {
        return false;
    }

    if (!Option::GetOptionValue(args, "--call-stack-limit-percent",
                                reportOption_.callStackHeatLimit_)) {
        return false;
    }

    if (!Option::GetOptionValue(args, "--comms", reportOption_.displayComms_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--pids", reportOption_.displayPids_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--tids", reportOption_.displayTids_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--dsos", reportOption_.displayDsos_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--funcs", reportOption_.displayFuncs_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--from_dsos", reportOption_.displayFuncs_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--from_funcs", reportOption_.displayFuncs_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--proto", protobufFormat_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--json", jsonFormat_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--debug", debug_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--branch", branch_)) {
        return false;
    }
    // this is a hidden option for compare result
    if (!Option::GetOptionValue(args, "--hide_count", reportOption_.hideCount_)) {
        return false;
    }
    return VerifyOption();
}

void SubCommandReport::DumpOptions() const
{
    printf("DumpOptions:\n");
    printf(" recordFile_:\t%s\n", recordFile_[FIRST].c_str());
    printf(" recordFile_:\t%s\n", recordFile_[SECOND].c_str());
    printf(" reportFile_:\t%s\n", reportFile_.c_str());
    printf(" sortKeys:\t%s\n", VectorToString(reportOption_.sortKeys_).c_str());
}
bool SubCommandReport::VerifyDisplayOption()
{
    for (std::string &number : reportOption_.displayPids_) {
        if (!IsDigits(number) || number.front() == '-') {
            printf("error number for pid '%s'\n", number.c_str());
            return false;
        }
    }

    for (std::string &number : reportOption_.displayTids_) {
        if (!IsDigits(number) || number.front() == '-') {
            printf("error number for tid '%s'\n", number.c_str());
            return false;
        }
    }
    return true;
}

bool SubCommandReport::VerifyOption()
{
    for (auto key : reportOption_.sortKeys_) {
        if (key == "count") {
            printf("unknown sort key name '%s'\n", key.c_str());
            return false;
        } else if (GetReport().reportKeyMap_.count(key) == 0) {
            printf("unknown sort key name '%s'\n", key.c_str());
            return false;
        }
    }
    const float min = 0.0;
    const float max = 100.0;
    if (reportOption_.heatLimit_ < min || reportOption_.heatLimit_ > max) {
        printf("head limit error. must in (0 <= limit < 100).\n");
        return false;
    }
    if (reportOption_.callStackHeatLimit_ < min || reportOption_.callStackHeatLimit_ > max) {
        printf("head limit error. must in (0 <= limit < 100).\n");
        return false;
    }
    if (recordFile_[FIRST].empty()) {
        printf("input file name can't be empty\n");
        return false;
    }
    if (!recordFile_[SECOND].empty()) {
        if (protobufFormat_ || jsonFormat_ || showCallStack_) {
            printf("diff don't support any export mode(like json , flame or proto)\n");
        } else {
            diffMode_ = true;
        }
    }
    if (!IsValidOutPath(reportFile_)) {
        printf("Invalid output file path, permission denied\n");
        return false;
    }
    // default report file name
    if (reportFile_.empty()) {
        if (protobufFormat_) {
            reportFile_ = "perf.proto";
        } else if (jsonFormat_) {
            reportFile_ = "perf.json";
        }
    }

    // misc config
    reportOption_.debug_ = debug_;
    ReportJsonFile::debug_ = debug_;

    return VerifyDisplayOption();
}

void SubCommandReport::BroadcastSample(std::unique_ptr<PerfRecordSample> &sample)
{
    // this func use for cpuoff mode , it will Broadcast the sampe to every event config
    for (auto &config : GetReport().configs_) {
        HLOGM("resend as id %" PRIu64 "", config.ids_[0]);
        sample->data_.id = config.ids_[0];
        ProcessSample(sample);
    }
}

void SubCommandReport::ProcessSample(std::unique_ptr<PerfRecordSample> &sample)
{
    sample->DumpLog(__FUNCTION__);
    if (jsonFormat_) {
        reportJsonFile_->UpdateReportSample(sample->data_.id, sample->data_.pid, sample->data_.tid,
                                            sample->data_.period);
        reportJsonFile_->UpdateReportCallStack(sample->data_.id, sample->data_.pid,
                                               sample->data_.tid, sample->data_.period,
                                               sample->callFrames_);
    } else if (protobufFormat_) {
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
        // make some cook
        // redesgin here
        protobufOutputFileWriter_->ProcessSampleRecord(
            *sample, static_cast<uint32_t>(GetReport().GetConfigIndex(sample->data_.id)),
            GetReport().virtualRuntime_.GetSymbolsFiles());
#endif
    } else {
        if (branch_) {
            GetReport().AddReportItemBranch(*sample);
        } else {
            GetReport().AddReportItem(*sample, showCallStack_);
        }
    }
}

bool SubCommandReport::RecordCallBack(PerfEventRecord& record)
{
    // tell process tree what happend for rebuild symbols
    GetReport().virtualRuntime_.UpdateFromRecord(record);

    if (record.GetType() == PERF_RECORD_SAMPLE) {
        std::unique_ptr<PerfRecordSample> sample
            = std::make_unique<PerfRecordSample>(static_cast<PerfRecordSample&>(record));
        std::unique_ptr<PerfRecordSample> prevSample = nullptr;
        if (cpuOffMode_) {
            auto prevIt = prevSampleCache_.find(sample->data_.tid);
            if (prevIt == prevSampleCache_.end()) {
                // this thread first sample
                prevSampleCache_[sample->data_.tid] = std::move(sample);
                // do nothing because we unable to calc the period
                return true;
            } else {
                // we have prev sample
                prevSample = std::move(prevIt->second);
                HLOGV("calc time %llu - %llu", sample->data_.time, prevSample->data_.time);
                if (sample->data_.time > prevSample->data_.time) {
                    prevSample->data_.period = sample->data_.time - prevSample->data_.time;
                } else {
                    prevSample->data_.period = 1u;
                }

                // current move the prev
                prevIt->second = std::move(sample);
                // go on with prevSample
                sample = std::move(prevSample);

                HLOGV("current sample period %llu ", sample->data_.period);
            }
        }
        if (cpuOffMode_ && cpuOffids_.size() > 0 && cpuOffids_.count(sample->data_.id) > 0) {
            BroadcastSample(sample);
        } else {
            ProcessSample(sample);
        }
    } else {
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
        if (protobufFormat_) {
            protobufOutputFileWriter_->ProcessRecord(record);
        }
#endif
    }
    return true;
}

void SubCommandReport::LoadPerfDataCompleted()
{
    if (jsonFormat_) {
        reportJsonFile_->UpdateCallNodeEventCount();
    }
    HLOGV("load perf data done");
}

void SubCommandReport::ProcessSymbolsData()
{
    GetReport().virtualRuntime_.SetSymbolsPaths(symbolsPaths_);
    // we need unwind it (for function name match) even not give us path
    GetReport().virtualRuntime_.SetDisableUnwind(false);

    // found symbols in file
    const auto featureSection = recordFileReader_->GetFeatureSection(FEATURE::HIPERF_FILES_SYMBOL);
    if (featureSection != nullptr) {
        const PerfFileSectionSymbolsFiles *sectionSymbolsFiles =
            static_cast<const PerfFileSectionSymbolsFiles *>(featureSection);
        GetReport().virtualRuntime_.UpdateFromPerfData(sectionSymbolsFiles->symbolFileStructs_);
    }
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
    // we have load the elf
    // write it to proto first
    if (protobufFormat_) {
        protobufOutputFileWriter_->ProcessSymbolsFiles(
            GetReport().virtualRuntime_.GetSymbolsFiles());
    }
#endif
    if (jsonFormat_) {
        reportJsonFile_->ProcessSymbolsFiles(GetReport().virtualRuntime_.GetSymbolsFiles());
    }
}

void SubCommandReport::ProcessUniStackTableData()
{
    auto featureSection = recordFileReader_->GetFeatureSection(FEATURE::HIPERF_FILES_UNISTACK_TABLE);
    if (featureSection != nullptr) {
        PerfFileSectionUniStackTable *sectioniStackTable =
            static_cast<PerfFileSectionUniStackTable *>(const_cast<PerfFileSection *>(featureSection));
        GetReport().virtualRuntime_.ImportUniqueStackNodes(sectioniStackTable->uniStackTableInfos_);
        GetReport().virtualRuntime_.SetDedupStack();
    }
}

void SubCommandReport::UpdateReportInfo()
{
    // get some meta info for protobuf
    if (protobufFormat_) {
        // workload
        const PerfFileSection *featureSection =
            recordFileReader_->GetFeatureSection(FEATURE::HIPERF_WORKLOAD_CMD);
        std::string workloader = "";
        if (featureSection != nullptr) {
            HLOGV("found HIPERF_META_WORKLOAD_CMD");
            const PerfFileSectionString *sectionString =
                static_cast<const PerfFileSectionString *>(featureSection);
            workloader = sectionString->ToString();
        } else {
            HLOGW("NOT found HIPERF_META_WORKLOAD_CMD");
        }
        protobufOutputFileWriter_->ProcessReportInfo(configNames_, workloader);
    }
}

void SubCommandReport::LoadEventConfigData()
{
    auto features = recordFileReader_->GetFeatures();
    cpuOffMode_ = find(features.begin(), features.end(), FEATURE::HIPERF_CPU_OFF) != features.end();
    if (cpuOffMode_) {
        HLOGD("this is cpuOffMode ");
    }
    const PerfFileSection *featureSection =
        recordFileReader_->GetFeatureSection(FEATURE::EVENT_DESC);
    if (featureSection != nullptr) {
        HLOGV("have EVENT_DESC");
        LoadEventDesc();
    } else {
        HLOGV("have Attr Section");
        LoadAttrSection();
    }
    HLOG_ASSERT(GetReport().configs_.size() > 0);
    HLOGV("record %d have %zu configs", index_, GetReport().configs_.size());
}

void SubCommandReport::LoadEventDesc()
{
    const PerfFileSection *featureSection =
        recordFileReader_->GetFeatureSection(FEATURE::EVENT_DESC);
    CHECK_TRUE(featureSection == nullptr, NO_RETVAL, 1, "featureSection invalid");
    const PerfFileSectionEventDesc &sectionEventdesc =
        *static_cast<const PerfFileSectionEventDesc *>(featureSection);
    HLOGV("Event descriptions: %zu", sectionEventdesc.eventDesces_.size());
    for (size_t i = 0; i < sectionEventdesc.eventDesces_.size(); i++) {
        const AttrWithId &fileAttr = sectionEventdesc.eventDesces_[i];

        HLOGV("event name[%zu]: %s ids: %s", i, fileAttr.name.c_str(),
              VectorToString(fileAttr.ids).c_str());
        if (cpuOffMode_ && fileAttr.name == cpuOffEventName) {
            // found cpuoff event id
            std::set<uint64_t> cpuOffids(fileAttr.ids.begin(), fileAttr.ids.end());
            cpuOffids_ = cpuOffids;
            HLOGV("this is cpu off event");
        } else {
            // don't add cpuoff event
            if (protobufFormat_) {
                configNames_.emplace_back(fileAttr.name);
            }
            for (uint64_t id : fileAttr.ids) {
                GetReport().configIdIndexMaps_[id] = GetReport().configs_.size(); // setup index
                HLOGV("add config id map %" PRIu64 " to %zu", id, GetReport().configs_.size());
            }
            // when cpuOffMode_ , don't use count mode , use time mode.
            auto &config =
                GetReport().configs_.emplace_back(fileAttr.name, fileAttr.attr.type,
                                                  fileAttr.attr.config, cpuOffMode_ ? false : true);
            config.ids_ = fileAttr.ids;
            HLOG_ASSERT(config.ids_.size() > 0);
            if (jsonFormat_) {
                reportJsonFile_->reportConfigItems_.emplace(
                    fileAttr.ids,
                    ReportConfigItem(reportJsonFile_->reportConfigItems_.size(), fileAttr.name));
            }
        }
    }
}

void SubCommandReport::LoadAttrSection()
{
    std::vector<AttrWithId> attrIds = recordFileReader_->GetAttrSection();
    for (size_t i = 0; i < attrIds.size(); ++i) {
        const AttrWithId &fileAttr = attrIds[i];
        std::string name = PerfEvents::GetStaticConfigName(
            static_cast<perf_type_id>(fileAttr.attr.type), fileAttr.attr.config);
        configNames_.emplace_back(name);
        for (uint64_t id : fileAttr.ids) {
            GetReport().configIdIndexMaps_[id] = GetReport().configs_.size(); // setup index
            HLOGV("add config id map %" PRIu64 " to %zu", id, GetReport().configs_.size());
        }
        auto &config = GetReport().configs_.emplace_back(fileAttr.name, fileAttr.attr.type,
                                                         fileAttr.attr.config);
        config.ids_ = fileAttr.ids;
        HLOG_ASSERT(config.ids_.size() > 0);
        if (jsonFormat_) {
            reportJsonFile_->reportConfigItems_.emplace(
                fileAttr.ids, ReportConfigItem(reportJsonFile_->reportConfigItems_.size(), name));
        }
        HLOGV("event name[%zu]: %s ids: %s", i, name_.c_str(),
              VectorToString(fileAttr.ids).c_str());
    }
}

void SubCommandReport::ProcessFeaturesData()
{
    LoadEventConfigData();

    // update device arch from feature
    SetDeviceArch(GetArchTypeFromUname(recordFileReader_->GetFeatureString(FEATURE::ARCH)));

#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
    UpdateReportInfo();
#endif
}

void SubCommandReport::FlushCacheRecord()
{
    for (auto &pair : prevSampleCache_) {
        std::unique_ptr<PerfRecordSample> sample = std::move(pair.second);
        sample->data_.period = 1u;
        if (cpuOffMode_ && cpuOffids_.size() > 0 && cpuOffids_.count(sample->data_.id) > 0) {
            BroadcastSample(sample);
        } else {
            ProcessSample(sample);
        }
    }
    prevSampleCache_.clear();
}

bool SubCommandReport::LoadPerfData()
{
    // check if file exist
    if (access(recordFile_[index_].c_str(), F_OK) != 0) {
        // file not exists
        printf("Can not access data file %s\n", recordFile_[index_].c_str());
        return false;
    }

    // try load the file
    recordFileReader_ = PerfFileReader::Instance(recordFile_[index_]);
    if (recordFileReader_ == nullptr) {
        HLOGE("FileReader::Instance(%s) return null", recordFile_[index_].c_str());
        return false;
    }

    CHECK_TRUE(!recordFileReader_->ReadFeatureSection(), false, LOG_TYPE_PRINTF, "record format error.\n");
    if (jsonFormat_) {
        reportJsonFile_ =
            std::make_unique<ReportJsonFile>(recordFileReader_, GetReport().virtualRuntime_);
    }

    ProcessFeaturesData();
    ProcessSymbolsData();
    ProcessUniStackTableData();
    HLOGD("process record");
    // before load data section
    SetHM();
    recordFileReader_->ReadDataSection(
        [this] (PerfEventRecord& record) -> bool {
            return this->RecordCallBack(record);
        });
    if (cpuOffMode_) {
        FlushCacheRecord();
    }
    HLOGD("process record completed");

    LoadPerfDataCompleted();
    return true;
}

bool SubCommandReport::OutputStd()
{
    if (fprintf(output_, "<<Hiperf Report%s>>\n", diffMode_ ? " Diff" : "") < 0) {
        return false;
    }

    // feature string:
    const auto &featureSections = recordFileReader_->GetFeatureSections();
    HLOGV("featureSections: %zu ", featureSections.size());

    for (auto &featureSection : featureSections) {
        if (recordFileReader_->IsFeatrureStringSection(featureSection->featureId_)) {
            const PerfFileSectionString *sectionString =
                static_cast<const PerfFileSectionString *>(featureSection.get());

            fprintf(output_, "%s: %s\n",
                    PerfFileSection::GetFeatureName(featureSection.get()->featureId_).c_str(),
                    sectionString->ToString().c_str());
        }
    }

    if (cpuOffMode_) {
        fprintf(output_, "cpu off mode: enabled\n");
    }

    if (!diffMode_) {
        GetReport(FIRST).AdjustReportItems();
        GetReport(FIRST).OutputStd(output_);
    } else {
        GetReport(FIRST).AdjustReportItems();
        GetReport(SECOND).AdjustReportItems();
        GetReport(FIRST).OutputStdDiff(output_, GetReport(SECOND));
    }

    return true;
}

bool SubCommandReport::OutputReport()
{
    if (output_ == nullptr) {
        HLOGD("nothing need output");
        return true; //
    } else if (jsonFormat_) {
        HLOGD("report as json");
        return reportJsonFile_->OutputJson(output_);
    } else {
        return OutputStd();
    }
}

bool SubCommandReport::PrepareOutput()
{
    if (protobufFormat_) {
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
        // check if file exist
        if (access(recordFile_[index_].c_str(), F_OK) != 0) {
            printf("Can not access data file %s\n", recordFile_[index_].c_str());
            return false;
        }
        printf("save to protobuf file: '%s'\n", reportFile_.c_str());
        protobufOutputFileWriter_ = std::make_unique<ReportProtobufFileWriter>();
        protobufOutputFileWriter_->Create(reportFile_);
#endif
        return true;
    }

    if (!reportFile_.empty()) {
        std::string resolvedPath = CanonicalizeSpecPath(reportFile_.c_str());
        output_ = fopen(resolvedPath.c_str(), "w");
        if (output_ == nullptr) {
            printf("unable open file to '%s' because '%d'\n", reportFile_.c_str(), errno);
            return false;
        } else {
            printf("report will save at '%s'\n", reportFile_.c_str());
        }
    } else {
        output_ = stdout;
    }

    return true;
}

SubCommandReport::~SubCommandReport()
{
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
    if (protobufOutputFileWriter_ != nullptr) {
        protobufOutputFileWriter_->Close();
    }
#endif
    if (output_ != nullptr && output_ != stdout) {
        fclose(output_);
    }

    SymbolsFile::onRecording_ = true; // back to default for UT
}

HiperfError SubCommandReport::OnSubCommand(std::vector<std::string>& args)
{
    RETURN_IF(!PrepareOutput(), HiperfError::PREPARE_OUTPUT_FAIL);

    // any way tell symbols this is not on recording
    SymbolsFile::onRecording_ = false;

    printf("loading data\n");
    if (!LoadPerfData()) {
        return HiperfError::LOAD_PERF_DATA_FAIL;
    }

    if (diffMode_) {
        // we are in diff mode
        index_ = SECOND;
        // load again with second file
        CHECK_TRUE(!LoadPerfData(), HiperfError::LOAD_SECOND_PERF_DATA_FAIL, 0, "");
        // back to first
        index_ = FIRST;
    }
    printf("prepare report\n");
    CHECK_TRUE(!OutputReport(), HiperfError::OUTPUT_REPORT_FAIL, 1, "OutputReport failed");
#ifdef HIPERF_DEBUG_TIME
    printf("SymbolicRecordTimes: %0.3f ms\n",
           GetReport(FIRST).virtualRuntime_.symbolicRecordTimes_.count() / MS_DURATION);
#endif

    printf("report done\n");
    AgeHiperflogFiles();
    return HiperfError::NO_ERR;
}

bool SubCommandReport::RegisterSubCommandReport()
{
    return SubCommand::RegisterSubCommand("report", SubCommandReport::GetInstance);
}

void SubCommandReport::SetHM()
{
    std::string os = recordFileReader_->GetFeatureString(FEATURE::OSRELEASE);
    isHM_ = os.find(HMKERNEL) != std::string::npos;
    GetReport().virtualRuntime_.SetHM(isHM_);
    HLOGD("Set isHM_: %d", isHM_);
    if (isHM_) {
        pid_t devhost = -1;
        std::string str = recordFileReader_->GetFeatureString(FEATURE::HIPERF_HM_DEVHOST);
        if (str != EMPTY_STRING && IsNumeric(str)) {
            devhost = std::stoll(str);
        }
        GetReport().virtualRuntime_.SetDevhostPid(devhost);
    }
}

SubCommand& SubCommandReport::GetInstance()
{
    static SubCommandReport subCommand;
    return subCommand;
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
