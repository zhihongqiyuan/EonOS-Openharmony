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
#ifndef HIPERF_PERF_FILE_WRITER_H
#define HIPERF_PERF_FILE_WRITER_H

#include <functional>
#include <unordered_map>
#include <vector>

#include "perf_event_record.h"
#include "perf_file_format.h"
#include "symbols_file.h"
#include "virtual_runtime.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
constexpr const int WRITER_BUFFER_SIZE = 4 * 1024 * 1024;

// write record to data file, like perf.data.
// format of file follow
// tools/perf/Documentation/perf.data-file-format.txt
class PerfFileWriter {
public:
    PerfFileWriter()
    {
        attrSection_.offset = 0;
        attrSection_.size = 0;
        defaultEventAttr_.branch_sample_type = 0;
        defaultEventAttr_.namespaces = 0;
    }
    ~PerfFileWriter();

    bool Open(const std::string &fileName, bool compressData = false);
    // WriteAttrAndId() must be called before WriteRecord()
    bool WriteAttrAndId(const std::vector<AttrWithId> &attrIds, bool isSpe = false);
    bool WriteRecord(const PerfEventRecord &record);
    bool AddNrCpusFeature(FEATURE feature, uint32_t nrCpusAvailable, uint32_t nrCpusOnline);
    bool AddEventDescFeature(FEATURE feature, const std::vector<AttrWithId> &eventDesces);
    bool AddStringFeature(FEATURE feature, std::string string);
    bool AddU64Feature(FEATURE feature, uint64_t v);
    bool AddBoolFeature(FEATURE feature);
    bool AddSymbolsFeature(const std::vector<std::unique_ptr<SymbolsFile>> &);
    bool AddUniStackTableFeature(const ProcessStackMap *table);
    // close file
    bool Close();

    uint64_t GetDataSize() const;
    uint GetRecordCount() const;
    std::chrono::microseconds writeTimes_ = std::chrono::microseconds::zero();

    using ProcessRecordCB = const std::function<bool(PerfEventRecord& record)>;
    bool ReadDataSection(ProcessRecordCB &callback);
    bool ReadRecords(ProcessRecordCB &callback);
    bool Read(void *buf, size_t len);
    void SetWriteRecordStat(bool isWrite);

private:
    std::string fileBuffer_;

    bool GetFilePos(uint64_t &pos) const;
    bool Write(const void *buf, size_t len);
    bool WriteHeader();
    bool WriteFeatureData();
    bool WriteTimeConvEvent();
    bool WriteAuxTraceInfoEvent();
    bool WriteCpuMapEvent();
    bool WriteAuxTraceEvent(bool isSpe);

    std::string fileName_;
    FILE *fp_ = nullptr;

    perf_file_header header_;
    perf_file_section attrSection_;
    perf_file_section dataSection_;
    std::vector<std::unique_ptr<PerfFileSection>> featureSections_;

    perf_event_attr defaultEventAttr_;

    uint recordCount_ = 0;
    bool compressData_ = false;
    bool isWritingRecord = false;
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_PERF_FILE_WRITER_H
