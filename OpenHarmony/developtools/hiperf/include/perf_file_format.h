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
#ifndef HIPERF_PERF_FILE_FORMAT_H
#define HIPERF_PERF_FILE_FORMAT_H

#include <string>

#include "perf_event_record.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
enum class FEATURE {
    RESERVED = 0, /* always cleared */
    FIRST_FEATURE = 1,
    TRACING_DATA = 1,
    BUILD_ID, // build_id_event

    HOSTNAME,  // A perf_header_string with the hostname where the data was collected (uname -n)
    OSRELEASE, // A perf_header_string with the os release where the data was collected (uname -r)
    VERSION,   // A perf_header_string with the perf user tool version where the data was collected.
               // This is the same as the version of the source tree the perf tool was built from.
    ARCH,      // A perf_header_string with the CPU architecture (uname -m)
    NRCPUS,    // A structure defining the number of CPUs.
    CPUDESC,   // A perf_header_string with description of the CPU. On x86 this is the model name
               // in /proc/cpuinfo
    CPUID,     // A perf_header_string with the exact CPU type. On x86 this is
               // vendor,family,model,stepping. For example: GenuineIntel,6,69,1
    TOTAL_MEM, // An uint64_t with the total memory in kilobytes.
    CMDLINE,   // A perf_header_string_list with the perf arg-vector used to collect the data.
    EVENT_DESC,    // Another description of the perf_event_attrs
    CPU_TOPOLOGY,  //
    NUMA_TOPOLOGY, // A list of NUMA node descriptions
    BRANCH_STACK,  // Not implemented in perf.
    PMU_MAPPINGS,  // A list of PMU structures, defining the different PMUs supported by perf.
    GROUP_DESC,    // Description of counter groups ({...} in perf syntax)
    AUXTRACE,      // Define additional auxtrace areas in the perf.data. auxtrace is used to store
                   // undecoded hardware tracing information, such as Intel Processor Trace data.
    STAT,
    CACHE,
    SAMPLE_TIME,
    MEM_TOPOLOGY,
    LAST_FEATURE,

    HIPERF_FIRST_FEATURE = 192,
    HIPERF_FILES_SYMBOL = HIPERF_FIRST_FEATURE,
    HIPERF_WORKLOAD_CMD,
    HIPERF_RECORD_TIME,
    HIPERF_CPU_OFF,
    HIPERF_HM_DEVHOST,
    // HIPERF_LAST_FEATURE = HIPERF_HM_DEVHOST,
    HIPERF_FILES_UNISTACK_TABLE,
    HIPERF_LAST_FEATURE = HIPERF_FILES_UNISTACK_TABLE,
    FEATURE_MAX_BITS = 256,
};

const static std::vector<FEATURE> FeatureStrings = {
    FEATURE::HOSTNAME,
    FEATURE::OSRELEASE,
    FEATURE::VERSION,
    FEATURE::ARCH,
    FEATURE::CPUDESC,
    FEATURE::CPUID,
    FEATURE::CMDLINE,

    FEATURE::HIPERF_WORKLOAD_CMD,
    FEATURE::HIPERF_RECORD_TIME,
    FEATURE::HIPERF_HM_DEVHOST,
};

struct perf_file_section {
    uint64_t offset;
    uint64_t size;
};

struct perf_file_attr {
    perf_event_attr attr;
    perf_file_section ids;
};

struct perf_header_string {
    uint32_t len;
    char string[0]; /* zero terminated */
};

constexpr char PERF_MAGIC[] = "PERFILE2";
constexpr int BITS_IN_BYTE = 8;
constexpr int NUM_FEATURES_FILE_HEADER = 256;

struct perf_file_header {
    char magic[8] = {'P', 'E', 'R', 'F', 'I', 'L', 'E', '2'};
    uint64_t size = sizeof(perf_file_header);
    uint64_t attrSize = sizeof(perf_file_attr);
    perf_file_section attrs;
    perf_file_section data;
    perf_file_section eventTypes;
    uint8_t features[NUM_FEATURES_FILE_HEADER / BITS_IN_BYTE] = {0};
};

class PerfFileSection {
public:
    struct perf_file_section header;
    const FEATURE featureId_;

    virtual bool GetBinary(char *buf, size_t size) = 0;
    virtual size_t GetSize() = 0;
    virtual ~PerfFileSection() {}
    explicit PerfFileSection(const FEATURE featureId) : featureId_(featureId)
    {
        header.size = 0;
        header.offset = 0;
    }
    static std::string GetFeatureName(FEATURE featureId);

protected:
    const char *rBuffer_ = nullptr;
    char *wBuffer_ = nullptr;
    size_t maxSize_ = 0;
    size_t offset_ = 0;

    // for read
    void Init(const char *buffer, size_t maxSize);
    // for write
    void Init(char *buffer, size_t maxSize);
    bool Write(uint32_t u32);
    bool Write(uint64_t u64);
    bool Write(const std::string &str);

    bool Write(const char *buf, size_t size);
    bool Write(const char *buf, size_t size, size_t max);

    bool Read(uint32_t &value);
    bool Read(uint64_t &value);
    bool Read(std::string &value);
    bool Read(char *buf, size_t size);
    void Skip(size_t size);

    uint32_t SizeOf(std::string &string);
};

class PerfFileSectionString : public PerfFileSection {
    std::string stdString_;

public:
    // convert buff to PerfFileSectionString, used to read file
    // if the data in buf is incorrect, ......
    PerfFileSectionString(FEATURE id, const char *buf, size_t size);
    PerfFileSectionString(FEATURE id, const std::string &charString);

    bool GetBinary(char *buf, size_t size);
    size_t GetSize();
    const std::string ToString() const;
};

// ref struct
struct SymbolStruct {
    uint64_t vaddr_ = 0;
    uint32_t len_ = 0;
    std::string symbolName_ = EMPTY_STRING;
    SymbolStruct() {}
    SymbolStruct(uint64_t vaddr, uint32_t len, const std::string &symbolName)
        : vaddr_(vaddr), len_(len), symbolName_(symbolName)
    {
    }
};

struct SymbolFileStruct {
    std::string filePath_ = EMPTY_STRING;
    uint32_t symbolType_;
    uint64_t textExecVaddr_;
    uint64_t textExecVaddrFileOffset_;
    std::string buildId_;
    std::vector<SymbolStruct> symbolStructs_;
};

class PerfFileSectionSymbolsFiles : public PerfFileSection {
public:
    std::vector<SymbolFileStruct> symbolFileStructs_;

    size_t GetSize();
    PerfFileSectionSymbolsFiles(FEATURE id, const std::vector<SymbolFileStruct> &symbolFileStructs)
        : PerfFileSection(id), symbolFileStructs_(symbolFileStructs)
    {
    }
    // if the data in buf is incorrect, ......
    PerfFileSectionSymbolsFiles(FEATURE id, const char *buf, size_t size);

    bool GetBinary(char *buf, size_t size);
    void ReadSymbolFileStructs();
private:
};

// NRCPUS: A structure defining the number of CPUs.
class PerfFileSectionNrCpus : public PerfFileSection {
    uint32_t nrCpusAvailable_ = 0; /* CPUs not yet onlined */
    uint32_t nrCpusOnline_ = 0;

public:
    PerfFileSectionNrCpus(FEATURE id, const char *buf, size_t size);
    PerfFileSectionNrCpus(FEATURE id, uint32_t nrCpusAvailable, uint32_t nrCpusOnline);

    bool GetBinary(char *buf, size_t size);
    size_t GetSize();
    void GetValue(uint32_t &nrCpusAvailable, uint32_t &nrCpusOnline) const;
};

class PerfFileSectionU64 : public PerfFileSection {
    uint64_t value_;

public:
    PerfFileSectionU64(FEATURE id, const char *buf, size_t size);
    PerfFileSectionU64(FEATURE id, uint64_t v);

    bool GetBinary(char *buf, size_t size);
    size_t GetSize();
    void GetValue(uint64_t &v) const;
};

class PerfFileSectionUniStackTable : public PerfFileSection {
public:
    std::vector<UniStackTableInfo> uniStackTableInfos_;
    PerfFileSectionUniStackTable(FEATURE id,
        const ProcessStackMap *table)
        : PerfFileSection(id), processStackTable_(table) {}
    PerfFileSectionUniStackTable(FEATURE id, const char *buf, size_t size);
private:
    const ProcessStackMap *processStackTable_;
    size_t GetSize();
    bool GetBinary(char *buf, size_t size);
};

struct AttrWithId;
class PerfFileSectionEventDesc : public PerfFileSection {
public:
    std::vector<AttrWithId> eventDesces_;

    PerfFileSectionEventDesc(FEATURE id, const char *buf, size_t size);
    PerfFileSectionEventDesc(FEATURE id, const std::vector<AttrWithId> &eventDesces);

    bool GetBinary(char *buf, size_t size);
    size_t GetSize();
    void GetValue(std::vector<AttrWithId> &eventDesces) const;
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_PERF_FILE_FORMAT_H
