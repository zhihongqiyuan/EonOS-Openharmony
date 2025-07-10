/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#ifndef HIPERF_REPORT_PROTOBUF_FILE
#define HIPERF_REPORT_PROTOBUF_FILE

#include <fstream>
#include <stdint.h>
#include <linux/perf_event.h>

#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"

#include "debug_logger.h"
#include "perf_event_record.h"
#include "report_sample.pb.h"
#include "symbols_file.h"
#include "utilities.h"

namespace Proto = OHOS::Developtools::Hiperf::Proto;
namespace OHOS {
namespace Developtools {
namespace HiPerf {
static const char FILE_MAGIC[] = "HIPERF_PB_";
static const uint16_t FILE_VERSION = 1u;
using ProtobufReadBack = std::function<void(Proto::HiperfRecord &record)>;

class ReportProtobufFileWriter : public google::protobuf::io::CopyingOutputStream {
public:
    bool Create(std::string fileName);

    bool ProcessRecord(const PerfEventRecord &record);
    bool ProcessSampleRecord(const PerfRecordSample &recordSample, uint32_t configIndex,
                             const std::vector<std::unique_ptr<SymbolsFile>> &symbolsFiles);
    bool ProcessSymbolsFiles(const std::vector<std::unique_ptr<SymbolsFile>> &symbolsFiles);
    bool ProcessReportInfo(const std::vector<std::string> &configNames,
                           const std::string &workloadCmd);

    ~ReportProtobufFileWriter();
    void Close();

private:
    std::unique_ptr<google::protobuf::io::CopyingOutputStreamAdaptor> protpbufOutputStream_;
    std::unique_ptr<google::protobuf::io::CodedOutputStream> protpbufCodedOutputStream_;
    std::string fileName_;
    std::unique_ptr<std::ofstream> protobufFileStream_ = std::make_unique<std::ofstream>();

    uint64_t recordCount_ = 0;
    uint64_t recordLost_ = 0;

    bool IsOpen();
    bool Write(const void *buffer, int size) override;
    virtual bool ProcessRecord(const PerfRecordComm &recordComm);
    virtual bool ProcessRecord(const PerfRecordLost &recordLost);
    void BeforeClose();
};

class ReportProtobufFileReader : public google::protobuf::io::CopyingInputStream {
public:
    bool Dump(std::string fileName, ProtobufReadBack readBack = nullptr);

private:
    std::unique_ptr<google::protobuf::io::CopyingInputStreamAdaptor> protpbufInputStream_;
    std::unique_ptr<google::protobuf::io::CodedInputStream> protpbufCodedInputStream_;
    std::string fileName_;
    std::unique_ptr<std::ifstream> protobufFileStream_ = std::make_unique<std::ifstream>();

    bool IsOpen();
    bool CheckFileMagic();
    int Read(void *buffer, int size) override;
    bool Dump(const Proto::HiperfRecord &record, int indent = 0);
    bool Dump(const Proto::CallStackSample &message, int indent = 0);
    bool Dump(const Proto::SampleStatistic &message, int indent = 0);
    bool Dump(const Proto::SymbolTableFile &message, int indent = 0);
    bool Dump(const Proto::VirtualThreadInfo &message, int indent = 0);
    bool Dump(const Proto::ReportInfo &message, int indent = 0);
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_REPORT_PROTOBUF_FILE
