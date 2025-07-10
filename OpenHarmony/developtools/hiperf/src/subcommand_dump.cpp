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
#define HILOG_TAG "Dump"

#include "subcommand_dump.h"

#include <cerrno>
#include <cinttypes>
#include <cstring>
#include <iostream>
#include <memory>

#include "debug_logger.h"
#include "hiperf_hilog.h"
#include "option.h"
#include "perf_event_record.h"
#include "perf_events.h"
#include "register.h"
#include "spe_decoder.h"
#include "symbols_file.h"
#include "utilities.h"
#include "virtual_runtime.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
using namespace OHOS::HiviewDFX;

static const std::string DEFAULT_DUMP_FILENAME = "perf.data";

bool SubCommandDump::CheckInputFile()
{
    if (!dumpFileName_.empty()) {
        if (elfFileName_.empty() && protobufDumpFileName_.empty()) {
            return true;
        }
    } else if (!elfFileName_.empty()) {
        if (protobufDumpFileName_.empty()) {
            return true;
        }
    } else if (!protobufDumpFileName_.empty()) {
        return true;
    } else { // all is empty
        dumpFileName_ = DEFAULT_DUMP_FILENAME;
        return true;
    }

    printf("options conflict, please check usage\n");
    return false;
}

bool SubCommandDump::CheckOutputFile()
{
    if (!IsValidOutPath(outputFilename_)) {
        printf("Invalid output file path, permission denied\n");
        return false;
    }
    return true;
}

bool SubCommandDump::CheckOptions()
{
    if (!CheckInputFile()) {
        return false;
    }
    if (!CheckOutputFile()) {
        return false;
    }
    return true;
}
bool SubCommandDump::ParseOption(std::vector<std::string> &args)
{
    if (!Option::GetOptionValue(args, "--head", dumpHeader_)) {
        HLOGD("get option --head failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "-f", dumpFeatures_)) {
        HLOGD("get option -f failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "-d", dumpData_)) {
        HLOGD("get option -d failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "--sympath", dumpSymbolsPaths_)) {
        HLOGD("get option --sympath failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "--elf", elfFileName_)) {
        HLOGD("get option --elf failed");
        return false;
    }
    if (!Option::GetOptionValue(args, "-i", dumpFileName_)) {
        return false;
    }
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
    if (!Option::GetOptionValue(args, "--proto", protobufDumpFileName_)) {
        HLOGD("get option --proto failed");
        return false;
    }
#endif
    if (!Option::GetOptionValue(args, "-o", outputFilename_)) {
        return false;
    }
    if (!Option::GetOptionValue(args, "--export", exportSampleIndex_)) {
        HLOGD("get option --export failed");
        return false;
    }

    if (dumpHeader_ || dumpFeatures_ || dumpData_) {
        dumpAll_ = false;
    }
    if (!args.empty()) {
        printf("'%s' option usage error, please check usage.\n", VectorToString(args).c_str());
        return false;
    }

    return CheckOptions();
}

bool SubCommandDump::PrepareDumpOutput()
{
    if (outputFilename_.empty()) {
        return true;
    }
    std::string resolvedPath = CanonicalizeSpecPath(outputFilename_.c_str());
    g_outputDump = fopen(resolvedPath.c_str(), "w");
    if (g_outputDump == nullptr) {
        printf("unable open file to '%s' because '%d'\n", outputFilename_.c_str(), errno);
        return false;
    }
    printf("dump result will save at '%s'\n", outputFilename_.c_str());
    return true;
}

SubCommandDump::~SubCommandDump()
{
    if (g_outputDump != nullptr && g_outputDump != stdout) {
        fclose(g_outputDump);
    }
    SymbolsFile::onRecording_ = true; // back to default for UT
}

HiperfError SubCommandDump::OnSubCommand(std::vector<std::string>& args)
{
    RETURN_IF(!PrepareDumpOutput(), HiperfError::PREPARE_DUMP_OUTPUT_FAIL);

    if (!elfFileName_.empty()) {
        return DumpElfFile() ? HiperfError::NO_ERR : HiperfError::DUMP_ELF_FILE_ERROR;
    }

#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
    if (!protobufDumpFileName_.empty()) {
        return DumpProtoFile() ? HiperfError::NO_ERR : HiperfError::DUMP_PROTO_FILE_ERROR;
    }
#endif

    if (access(dumpFileName_.c_str(), F_OK) != 0) {
        printf("Can not access data file %s\n", dumpFileName_.c_str());
        return HiperfError::ACCESS_DATA_FILE_FAIL;
    }
    // only one file should created
    HLOG_ASSERT_MESSAGE(reader_ == nullptr, " perf file reader for %s\n", dumpFileName_.c_str());
    reader_ = PerfFileReader::Instance(dumpFileName_);
    if (reader_ == nullptr) {
        HLOGE("HiperfFileReader::Instance(%s) return null", dumpFileName_.c_str());
        return HiperfError::OPEN_DATA_FILE_FAIL;
    }

    // any way tell symbols this is not on device
    SymbolsFile::onRecording_ = false;
    // we need unwind it (for function name match) even not give us path
    vr_.SetDisableUnwind(false);

    if (!dumpSymbolsPaths_.empty()) {
        // user give us path , we enable unwind
        if (!vr_.SetSymbolsPaths(dumpSymbolsPaths_)) {
            printf("Failed to set symbol path(%s)\n", VectorToString(dumpSymbolsPaths_).c_str());
            return HiperfError::SET_SYMBOLS_PATH_FAIL;
        }
    }

    if (dumpHeader_ || dumpAll_) {
        DumpPrintFileHeader(indent_);
        DumpAttrPortion(indent_);
    }

    if (dumpAll_ || dumpData_) {
        // before load data section
        SetHM();
        DumpDataPortion(indent_);
        DumpSpeReport();
    }

    if (dumpFeatures_ || dumpAll_) {
        DumpFeaturePortion(indent_);
    }
    AgeHiperflogFiles();
    return HiperfError::NO_ERR;
}

bool SubCommandDump::DumpElfFile()
{
    printf("dump elf: '%s'\n", elfFileName_.c_str());
    auto elf = SymbolsFile::CreateSymbolsFile(elfFileName_);
    if (!elf->LoadSymbols(nullptr, "")) {
        printf("load elf failed.\n");
        return false;
    } else {
        printf("load elf succeed.\n");
    }
    return true;
}
#if defined(HAVE_PROTOBUF) && HAVE_PROTOBUF
bool SubCommandDump::DumpProtoFile()
{
    printf("dump protobuf file: '%s'\n", protobufDumpFileName_.c_str());
    protobufInputFileReader_ = std::make_unique<ReportProtobufFileReader>();
    if (!protobufInputFileReader_->Dump(protobufDumpFileName_)) {
        printf("load proto failed.\n");
        return false;
    }
    return true;
}
#endif

void SubCommandDump::PrintHeaderInfo(const int &indent)
{
    const perf_file_header &header = reader_->GetHeader();
    // magic
    PRINT_INDENT(indent, "magic: ");
    for (size_t i = 0; i < sizeof(header.magic); ++i) {
        PRINT_INDENT(indent, "%c", header.magic[i]);
    }
    PRINT_INDENT(indent, "\n");
    PRINT_INDENT(indent, "header_size: %" PRId64 "\n", header.size);
    if (header.size != sizeof(header)) {
        HLOGW("record file header size doesn't match");
    }
    PRINT_INDENT(indent, "attr_size: %" PRId64 "\n", header.attrSize);
    if (header.attrSize != sizeof(perf_file_attr)) {
        HLOGW("attr size doesn't match");
    }
    // attr
    PRINT_INDENT(indent, "attrs[file section]: offset %" PRId64 ", size %" PRId64 "\n",
                 header.attrs.offset, header.attrs.size);
    // data
    PRINT_INDENT(indent, "data[file section]: offset %" PRId64 ", size %" PRId64 "\n",
                 header.data.offset, header.data.size);
    PRINT_INDENT(indent, "event_types[file section]: offset %" PRId64 ", size %" PRId64 "\n",
                 header.eventTypes.offset, header.eventTypes.size);
    // feature
    PRINT_INDENT(indent,
                 "adds_features[]: 0x%" PRIX64 " 0x%" PRIX64 " 0x%" PRIX64 " 0x%" PRIX64 "\n",
                 *(reinterpret_cast<const uint64_t *>(&header.features[0])),
                 *(reinterpret_cast<const uint64_t *>(&header.features[8])),
                 *(reinterpret_cast<const uint64_t *>(&header.features[16])),
                 *(reinterpret_cast<const uint64_t *>(&header.features[24])));
}

void SubCommandDump::DumpPrintFileHeader(int indent)
{
    // print header
    PrintHeaderInfo(indent);

    // print feature
    auto features = reader_->GetFeatures();
    for (auto feature : features) {
        PRINT_INDENT(indent, "feature: %s\n", PerfFileSection::GetFeatureName(feature).c_str());
    }

    // read here , because we need found symbols
    reader_->ReadFeatureSection();

    SetDeviceArch(GetArchTypeFromUname(reader_->GetFeatureString(FEATURE::ARCH)));

    // found symbols in file
    for (auto &featureSection : reader_->GetFeatureSections()) {
        if (featureSection.get()->featureId_ == FEATURE::HIPERF_FILES_SYMBOL) {
            const PerfFileSectionSymbolsFiles *sectionSymbolsFiles =
                static_cast<const PerfFileSectionSymbolsFiles *>(featureSection.get());
            vr_.UpdateFromPerfData(sectionSymbolsFiles->symbolFileStructs_);
        } else if (featureSection.get()->featureId_ == FEATURE::HIPERF_FILES_UNISTACK_TABLE) {
            const PerfFileSectionUniStackTable *sectionUniStackTable  =
                static_cast<const PerfFileSectionUniStackTable *>(featureSection.get());
            vr_.ImportUniqueStackNodes(sectionUniStackTable->uniStackTableInfos_);
            vr_.SetDedupStack();
        }
    }
}

static std::map<int, std::string> g_sampleTypeNames = {
    {PERF_SAMPLE_IP, "ip"},
    {PERF_SAMPLE_TID, "tid"},
    {PERF_SAMPLE_TIME, "time"},
    {PERF_SAMPLE_ADDR, "addr"},
    {PERF_SAMPLE_READ, "read"},
    {PERF_SAMPLE_CALLCHAIN, "callchain"},
    {PERF_SAMPLE_ID, "id"},
    {PERF_SAMPLE_CPU, "cpu"},
    {PERF_SAMPLE_PERIOD, "period"},
    {PERF_SAMPLE_STREAM_ID, "stream_id"},
    {PERF_SAMPLE_RAW, "raw"},
    {PERF_SAMPLE_BRANCH_STACK, "stack"},
    {PERF_SAMPLE_REGS_USER, "regs_user"},
    {PERF_SAMPLE_STACK_USER, "stack_user"},
    {PERF_SAMPLE_WEIGHT, "weight"},
    {PERF_SAMPLE_DATA_SRC, "data_src"},
    {PERF_SAMPLE_IDENTIFIER, "identifier"},
    {PERF_SAMPLE_TRANSACTION, "transaction"},
    {PERF_SAMPLE_REGS_INTR, "reg_intr"},
    {PERF_SAMPLE_SERVER_PID, "server_pid"},
};

void SubCommandDump::DumpSampleType(uint64_t sampleType, int indent)
{
    std::string names;
    for (auto &pair : g_sampleTypeNames) {
        if (sampleType & pair.first) {
            if (!names.empty()) {
                names.append(",");
            }
            names.append(pair.second);
        }
    }
    PRINT_INDENT(indent + 1, "sample_type names: %s\n", names.c_str());
}

void SubCommandDump::DumpPrintEventAttr(const perf_event_attr &attr, int indent)
{
    PRINT_INDENT(indent, "event_attr: \n");

    PRINT_INDENT(indent + 1, "type %u, size %u, config %llu\n", attr.type, attr.size, attr.config);

    if (attr.freq != 0) {
        PRINT_INDENT(indent + 1, "sample_freq %llu\n", attr.sample_freq);
    } else {
        PRINT_INDENT(indent + 1, "sample_period %llu\n", attr.sample_period);
    }

    PRINT_INDENT(indent + 1, "sample_type (0x%llx) \n", attr.sample_type);
    DumpSampleType(attr.sample_type, indent);

    PRINT_INDENT(indent + 1, "read_format (0x%llx) \n", attr.read_format);

    PRINT_INDENT(indent + 1, "disabled %u, inherit %u, pinned %u, exclusive %u\n", attr.disabled,
                 attr.inherit, attr.pinned, attr.exclusive);

    PRINT_INDENT(indent + 1, "exclude_user %u, exclude_kernel %u, exclude_hv %u, exclude_idle %u\n",
                 attr.exclude_user, attr.exclude_kernel, attr.exclude_hv, attr.exclude_idle);

    PRINT_INDENT(indent + 1, "mmap %u, mmap2 %u, comm %u, comm_exec %u, freq %u\n", attr.mmap,
                 attr.mmap2, attr.comm, attr.comm_exec, attr.freq);

    PRINT_INDENT(indent + 1, "inherit_stat %u, enable_on_exec %u, task %u, use_clockid %u\n",
                 attr.inherit_stat, attr.enable_on_exec, attr.task, attr.use_clockid);

    PRINT_INDENT(indent + 1, "watermark %u, precise_ip %u, mmap_data %u, clockid %d\n", attr.watermark,
                 attr.precise_ip, attr.mmap_data, attr.clockid);

    PRINT_INDENT(indent + 1, "sample_id_all %u, exclude_host %u, exclude_guest %u\n", attr.sample_id_all,
                 attr.exclude_host, attr.exclude_guest);
    PRINT_INDENT(indent + 1, "branch_sample_type 0x%llx\n", attr.branch_sample_type);
    PRINT_INDENT(indent + 1, "exclude_callchain_kernel %u, exclude_callchain_user %u\n",
                 attr.exclude_callchain_kernel, attr.exclude_callchain_user);
    PRINT_INDENT(indent + 1, "sample_regs_user 0x%llx\n", attr.sample_regs_user);
    PRINT_INDENT(indent + 1, "sample_stack_user 0x%x\n", attr.sample_stack_user);
}

void SubCommandDump::DumpAttrPortion(int indent)
{
    attrIds_ = reader_->GetAttrSection();
    for (size_t i = 0; i < attrIds_.size(); ++i) {
        const AttrWithId &attr = attrIds_[i];
        PRINT_INDENT(indent, "attr %zu:\n", i + 1);
        DumpPrintEventAttr(attr.attr, indent_ + 1);
        if (!attr.ids.empty()) {
            PRINT_INDENT(indent, "  ids:");
            for (const auto &id : attr.ids) {
                PRINT_INDENT(indent, " %" PRId64, id);
            }
            PRINT_INDENT(indent, "\n");
        }
    }
}

void SubCommandDump::ExportUserStack(const PerfRecordSample &recordSample)
{
    if (recordSample.data_.reg_nr > 0 && recordSample.data_.dyn_size > 0) {
        // <pid>_<tid>_user_regs_<time>
        std::string userRegs =
            StringPrintf("hiperf_%d_%d_user_regs_%zu.dump", recordSample.data_.pid,
                         recordSample.data_.tid, exportSampleIndex_);
        std::string resolvedPath = CanonicalizeSpecPath(userRegs.c_str());
        FILE *userRegsFp = fopen(resolvedPath.c_str(), "wb");
        CHECK_TRUE(userRegsFp == nullptr, NO_RETVAL, 1, "open userRegs failed");
        std::unique_ptr<FILE, decltype(&fclose)> fpUserRegs(userRegsFp, fclose);
        fwrite(recordSample.data_.user_regs, sizeof(u64), recordSample.data_.reg_nr,
               fpUserRegs.get());

        std::string userData =
            StringPrintf("hiperf_%d_%d_user_data_%zu.dump", recordSample.data_.pid,
                         recordSample.data_.tid, exportSampleIndex_);
        std::string resolvePath = CanonicalizeSpecPath(userData.c_str());
        FILE *UserDataFp = fopen(resolvePath.c_str(), "wb");
        CHECK_TRUE(UserDataFp == nullptr, NO_RETVAL, 1, "open UserData failed");
        std::unique_ptr<FILE, decltype(&fclose)> fpUserData(UserDataFp, fclose);
        fwrite(recordSample.data_.stack_data, sizeof(u8), recordSample.data_.dyn_size,
               fpUserData.get());
    }
}

void SubCommandDump::ExportUserData(PerfEventRecord& record)
{
    if (record.GetType() == PERF_RECORD_SAMPLE) {
        if (currectSampleIndex_++ != exportSampleIndex_) {
            return;
        }
        PerfRecordSample* recordSample = static_cast<PerfRecordSample*>(&record);
        ExportUserStack(*recordSample);

        std::string userData =
            StringPrintf("hiperf_%d_%d_sample_record_%zu_%" PRIu64 ".dump", recordSample->data_.pid,
                         recordSample->data_.tid, exportSampleIndex_, recordSample->data_.time);
        std::string resolvedPath = CanonicalizeSpecPath(userData.c_str());
        std::unique_ptr<FILE, decltype(&fclose)> fpUserData(fopen(resolvedPath.c_str(), "wb"), fclose);
        static std::vector<u8> buf(RECORD_SIZE_LIMIT);
        CHECK_TRUE(!recordSample->GetBinary(buf), NO_RETVAL, 1, "export user sample data failed");
        fwrite(buf.data(), sizeof(u8), recordSample->GetSize(), fpUserData.get());

        HLOGD("export user data index %d time %llu", exportSampleIndex_, recordSample->data_.time);
    }
}

void SubCommandDump::DumpCallChain(int indent, const PerfRecordSample& sample)
{
    PRINT_INDENT(indent, "\n callchain: %zu\n", sample.callFrames_.size());
    if (sample.callFrames_.size() > 0) {
        indent += indent + 1;
        for (auto frameIt = sample.callFrames_.begin(); frameIt != sample.callFrames_.end();
             frameIt++) {
            PRINT_INDENT(indent, "%02zd:%s\n", std::distance(frameIt, sample.callFrames_.end()),
                         frameIt->ToSymbolString().c_str());
        }
    }
}

void SubCommandDump::DumpDataPortion(int indent)
{
    int recordCount = 0;
    auto recordcCallback = [&](PerfEventRecord& record) {
        CHECK_TRUE(record.GetName() == nullptr, false, 0, ""); // return false in callback can stop the read process

        // for UT
        if (exportSampleIndex_ > 0) {
            ExportUserData(record);
        }

        // tell process tree what happend for rebuild symbols
        vr_.UpdateFromRecord(record);

        recordCount++;
        record.Dump(indent, outputFilename_, g_outputDump);

        if (record.GetType() == PERF_RECORD_SAMPLE) {
            DumpCallChain(indent, static_cast<PerfRecordSample&>(record));
        }

        return true;
    };

    reader_->ReadDataSection(recordcCallback);

    PRINT_INDENT(indent, "\n ======= there are %d records ======== \n", recordCount);
}

void SubCommandDump::PrintSymbolFile(const int &indent, const SymbolFileStruct &symbolFileStruct)
{
    PRINT_INDENT(indent + INDENT_TWO, "filePath:%s\n", symbolFileStruct.filePath_.c_str());
    PRINT_INDENT(indent + INDENT_TWO, "symbolType:%u\n", symbolFileStruct.symbolType_);
    PRINT_INDENT(indent + INDENT_TWO, "minExecAddr:0x%" PRIx64 "\n", symbolFileStruct.textExecVaddr_);
    PRINT_INDENT(indent + INDENT_TWO, "minExecAddrFileOffset:0x%08" PRIx64 "\n",
                symbolFileStruct.textExecVaddrFileOffset_);
    if (!symbolFileStruct.buildId_.empty()) {
        PRINT_INDENT(indent + INDENT_TWO, "buildId:'%s'\n", symbolFileStruct.buildId_.c_str());
    }
    PRINT_INDENT(indent + INDENT_TWO, "symbol number: %zu\n", symbolFileStruct.symbolStructs_.size());
    int symbolid = 0;
    for (auto &symbolStruct : symbolFileStruct.symbolStructs_) {
        PRINT_INDENT(indent + 3, "%05d [0x%016" PRIx64 "@0x%08x]  %s\n", symbolid, symbolStruct.vaddr_,
                     symbolStruct.len_, symbolStruct.symbolName_.c_str());
        symbolid++;
    }
}

void SubCommandDump::PrintFeatureEventdesc(int indent,
                                           const PerfFileSectionEventDesc &sectionEventdesc)
{
    PRINT_INDENT(indent + INDENT_TWO, "Event descriptions: %zu\n", sectionEventdesc.eventDesces_.size());
    for (size_t i = 0; i < sectionEventdesc.eventDesces_.size(); i++) {
        const AttrWithId &desc = sectionEventdesc.eventDesces_[i];
        PRINT_INDENT(indent + INDENT_TWO, "event name[%zu]: %s ids: %s\n", i, desc.name.c_str(),
                     VectorToString(desc.ids).c_str());

        // attr is duplicated the attrs section
    }
    PRINT_INDENT(indent + INDENT_TWO, "\n");
}

void SubCommandDump::DumpFeaturePortion(int indent)
{
    PRINT_INDENT(indent, "\n ==== features ====\n");
    auto features = reader_->GetFeatures();
    for (auto feature : features) {
        PRINT_INDENT(indent + 1, "feature %d:%s\n", feature,
                     PerfFileSection::GetFeatureName(feature).c_str());
    }

    const auto &featureSections = reader_->GetFeatureSections();
    HLOGV("featureSections: %zu ", featureSections.size());

    PRINT_INDENT(indent, "\n ==== feature sections ====\n");

    for (auto &featureSection : featureSections) {
        PRINT_INDENT(indent + 1, "feature %d:%s content: \n", featureSection.get()->featureId_,
                     PerfFileSection::GetFeatureName(featureSection.get()->featureId_).c_str());
        if (reader_->IsFeatrureStringSection(featureSection.get()->featureId_)) {
            const PerfFileSectionString *sectionString =
                static_cast<const PerfFileSectionString *>(featureSection.get());
            PRINT_INDENT(indent + INDENT_TWO, "%s\n", sectionString->ToString().c_str());
            continue;
        } else if (featureSection.get()->featureId_ == FEATURE::EVENT_DESC) {
            PrintFeatureEventdesc(
                indent, *static_cast<const PerfFileSectionEventDesc *>(featureSection.get()));
            continue;
        } else if (featureSection.get()->featureId_ == FEATURE::HIPERF_FILES_SYMBOL) {
            const PerfFileSectionSymbolsFiles *sectionSymbolsFiles =
                static_cast<const PerfFileSectionSymbolsFiles *>(featureSection.get());
            if (sectionSymbolsFiles != nullptr) {
                PRINT_INDENT(indent + INDENT_TWO, "SymbolFiles:%zu\n",
                             sectionSymbolsFiles->symbolFileStructs_.size());

                int fileid = 0;
                for (auto &symbolFileStruct : sectionSymbolsFiles->symbolFileStructs_) {
                    PRINT_INDENT(indent + INDENT_TWO, "\n");
                    PRINT_INDENT(indent + INDENT_TWO, "fileid:%d\n", fileid);
                    fileid++;
                    // symbol file info
                    PrintSymbolFile(indent, symbolFileStruct);
                }
            } else {
                PRINT_INDENT(indent + INDENT_TWO, "get SymbolFiles failed\n");
            }
            continue;
        } else if (featureSection.get()->featureId_ == FEATURE::HIPERF_FILES_UNISTACK_TABLE) {
            const PerfFileSectionUniStackTable *sectioniStackTable =
                static_cast<PerfFileSectionUniStackTable *>(const_cast<PerfFileSection *>(featureSection.get()));
            if (sectioniStackTable != nullptr) {
                DumpUniqueStackTableNode(indent + 1, *sectioniStackTable);
            } else {
                PRINT_INDENT(indent + INDENT_TWO, "get StackTable failed\n");
            }
            continue;
        } else {
            PRINT_INDENT(indent + INDENT_TWO, "not support dump this feature(%d).\n", featureSection.get()->featureId_);
        }
    }
}

void SubCommandDump::DumpUniqueStackTableNode(int indent, const PerfFileSectionUniStackTable &uniStackTable)
{
    int tableid = 0;
    PRINT_INDENT(indent + 1, "TableNums: %zu\n\n", uniStackTable.uniStackTableInfos_.size());
    for (const auto& uniStackTableInfo : uniStackTable.uniStackTableInfos_) {
        PRINT_INDENT(indent + INDENT_TWO, "tableid: %d\n", tableid);
        PRINT_INDENT(indent + INDENT_TWO, "pid: %" PRIu32 "\n", uniStackTableInfo.pid);
        PRINT_INDENT(indent + INDENT_TWO, "tableSize: %" PRIu32 "\n", uniStackTableInfo.tableSize);
        PRINT_INDENT(indent + INDENT_TWO, "numNodes: %" PRIu32 "\n", uniStackTableInfo.numNodes);
        PRINT_INDENT(indent + INDENT_TWO, "%-7s %-7s %-8s\n", "no", "index", "node");
        for (size_t i = 0; i < uniStackTableInfo.nodes.size(); i++) {
            UniStackNode node = uniStackTableInfo.nodes[i];
            PRINT_INDENT(indent + INDENT_TWO, "%-7zu %-7" PRIu32 " 0x%-8" PRIx64 "\n", i, node.index, node.node.value);
        }
        tableid++;
    }
}

bool SubCommandDump::RegisterSubCommandDump()
{
    return SubCommand::RegisterSubCommand("dump", SubCommandDump::GetInstance);
}

void SubCommandDump::SetHM()
{
    std::string os = reader_->GetFeatureString(FEATURE::OSRELEASE);
    isHM_ = os.find(HMKERNEL) != std::string::npos;
    vr_.SetHM(isHM_);
    HLOGD("Set isHM_: %d", isHM_);
    if (isHM_) {
        pid_t devhost = -1;
        std::string str = reader_->GetFeatureString(FEATURE::HIPERF_HM_DEVHOST);
        if (IsNumeric(str)) {
            devhost = std::stoll(str);
        }
        vr_.SetDevhostPid(devhost);
    }
}

void SubCommandDump::DumpSpeReport()
{
#if defined(is_ohos) && is_ohos
    std::string cmdline = reader_->GetFeatureString(FEATURE::CMDLINE);
    if (cmdline.find("-e arm_spe_0") != std::string::npos) {
        HLOGD("dump spe report data");
        UpdateHeating();
        DumpSpeReportData(indent_, g_outputDump);
    }
#endif
}

SubCommand& SubCommandDump::GetInstance()
{
    static SubCommandDump subCommand;
    return subCommand;
}

} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
