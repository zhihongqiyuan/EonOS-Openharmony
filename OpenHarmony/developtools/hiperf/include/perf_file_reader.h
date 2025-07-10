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

#ifndef HIPERF_FILE_READER
#define HIPERF_FILE_READER

#include <functional>
#include <string>
#include <unordered_map>

#include "perf_event_record.h"
#include "perf_file_format.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
using ProcessRecordCB = const std::function<bool(PerfEventRecord& record)>;
// read record from data file, like perf.data.
// format of file follow
// tools/perf/Documentation/perf.data-file-format.txt
class PerfFileReader {
public:
    virtual ~PerfFileReader();

    static std::unique_ptr<PerfFileReader> Instance(const std::string &fileName);

    const perf_file_header &GetHeader() const;

    std::vector<AttrWithId> GetAttrSection() const;

    // read data section, construct record, call callback for each record
    bool ReadDataSection(ProcessRecordCB &callback);

    bool ReadFeatureSection();
    const std::vector<FEATURE> &GetFeatures() const;
    const std::vector<std::unique_ptr<PerfFileSection>> &GetFeatureSections() const;
    const PerfFileSection *GetFeatureSection(FEATURE feature) const;
    explicit PerfFileReader(const std::string &fileName, FILE *fp);

    const std::string GetFeatureString(const FEATURE feature) const;

    bool IsFeatrureStringSection(const FEATURE featureId) const
    {
        return find(FeatureStrings.begin(), FeatureStrings.end(), featureId) !=
               FeatureStrings.end();
    }

    // fuzz user this
protected:
    virtual bool Read(void *buf, size_t len);
    virtual bool Read(char *buf, uint64_t offset, size_t len);
    FILE *fp_ = nullptr;
    bool ReadFileHeader();
    bool ReadAttrSection();

private:
    bool ReadRecord(ProcessRecordCB &callback);
    bool IsValidDataFile();
    bool IsGzipFile();

    // file header must be read first

    bool ReadIdsForAttr(const perf_file_attr &attr, std::vector<uint64_t> *ids);

    const perf_event_attr *GetDefaultAttr();

    const std::string fileName_;
    uint64_t dataSectionSize_;
    bool compressData_ = false;

    perf_file_header header_;
    std::vector<perf_file_attr> vecAttr_;
    std::vector<std::vector<uint64_t>> vecAttrIds_;

    std::unordered_map<uint64_t, size_t> mapId2Attr_;
    uint64_t featureSectionOffset_;
    std::vector<FEATURE> features_;
    std::vector<std::unique_ptr<PerfFileSection>> perfFileSections_;

    size_t fileSize_ = 0;
#ifdef HIPERF_DEBUG_TIME
    std::chrono::microseconds readRecordTime_ = std::chrono::microseconds::zero();
    std::chrono::microseconds readCallbackTime_ = std::chrono::microseconds::zero();
#endif
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_FILE_READER
