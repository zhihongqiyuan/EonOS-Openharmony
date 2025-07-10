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
#include "perf_file_reader.h"

#include <bitset>
#include <cinttypes>
#include <cstdlib>
#include <memory>

#include <sys/stat.h>
#include <unistd.h>

#include "hiperf_hilog.h"
#include "utilities.h"

using namespace std::chrono;
namespace OHOS {
namespace Developtools {
namespace HiPerf {
const int FETURE_MAX = 256;
const int SIZE_FETURE_COUNT = 8;
constexpr size_t MAX_VECTOR_RESIZE_COUNT = 100000;
constexpr char UNCOMPRESS_TMP_FILE[] = "/data/local/tmp/.perf.data";

std::unique_ptr<PerfFileReader> PerfFileReader::Instance(const std::string &fileName)
{
    std::string resolvedPath = CanonicalizeSpecPath(fileName.c_str());
    FILE *fp = fopen(resolvedPath.c_str(), "rb");
    if (fp == nullptr) {
        HLOGE("fail to open file %s", fileName.c_str());
        return nullptr;
    }

    std::unique_ptr<PerfFileReader> reader = std::make_unique<PerfFileReader>(fileName, fp);
    if (!reader->ReadFileHeader()) {
        // Fail to read header, maybe its compressed
        if (reader->IsGzipFile()) {
            if (fp != nullptr) {
                fclose(fp);
            }
            reader->fp_ = nullptr;

            CHECK_TRUE(!UncompressFile(fileName, UNCOMPRESS_TMP_FILE), nullptr, 1,
                       "Fail to UncompressFile(%s)", fileName.c_str());

            // open the uncompressed hidden file .perf.data
            FILE *fp2 = fopen(UNCOMPRESS_TMP_FILE, "rb");
            if (fp2 == nullptr) {
                HLOGE("fail to open uncompressed file .perf.data");
                return nullptr;
            }

            reader->fp_ = fp2;
            reader->compressData_ = true;

            if (!reader->ReadFileHeader()) {
                HLOGE("fail to read header of file .perf.data");
                return nullptr;
            }
            goto end;
        }
        return nullptr;
    }
end:
    CHECK_TRUE(!reader->ReadAttrSection(), nullptr, 0, "");
    return reader;
}

PerfFileReader::PerfFileReader(const std::string &fileName, FILE *fp) : fp_(fp), fileName_(fileName)
{
    featureSectionOffset_ = 0;
    struct stat fileStat;
    if (fp != nullptr) {
        if (fstat(fileno(fp), &fileStat) != -1 && fileStat.st_size > 0) {
            fileSize_ = fileStat.st_size;
        }
    }
}

PerfFileReader::~PerfFileReader()
{
    // if file was not closed properly
    if (fp_ != nullptr && fp_ != stdout) {
        fclose(fp_);
    }
    fp_ = nullptr;

    // remove the uncompressed .perf.data
    if (compressData_) {
        if (remove(UNCOMPRESS_TMP_FILE) != 0) {
            HLOGE("Fail to remove uncompressed file .perf.data");
            perror("Fail to remove temp file");
        }
    }
}

bool PerfFileReader::IsValidDataFile()
{
    return (memcmp(header_.magic, PERF_MAGIC, sizeof(header_.magic)) == 0);
}

bool PerfFileReader::IsGzipFile()
{
    return header_.magic[0] == '\x1f' && header_.magic[1] == '\x8b';
}

bool PerfFileReader::ReadFileHeader()
{
    if (Read(&header_, sizeof(header_))) {
        dataSectionSize_ = header_.data.size;
        if (IsValidDataFile()) {
            featureSectionOffset_ = header_.data.offset + header_.data.size;
            for (int i = 0; i < FETURE_MAX / SIZE_FETURE_COUNT; i++) {
                std::bitset<SIZE_FETURE_COUNT> features(header_.features[i]);
                for (int j = 0; j < SIZE_FETURE_COUNT; j++) {
                    if (features.test(j)) {
                        features_.emplace_back((FEATURE)(((uint64_t)i) * SIZE_FETURE_COUNT + j));
                    }
                }
            }
            return true;
        }
    }
    return false;
}

bool PerfFileReader::ReadAttrSection()
{
    if (header_.attrSize != sizeof(perf_file_attr)) {
        // 4.19 and 5.1 use diff size , 128 vs 136
        HLOGW("attr size %" PRId64 " doesn't match expected size %zu", header_.attrSize,
              sizeof(perf_file_attr));
    }
    CHECK_TRUE(header_.attrSize == 0, false, 0, "");
    CHECK_TRUE(header_.attrSize > THOUSANDS, false, 1, "attr size exceeds 1000");
    int attrCount = header_.attrs.size / header_.attrSize;
    CHECK_TRUE(attrCount == 0, false, 1, "no attr in file");
    if (fseek(fp_, header_.attrs.offset, SEEK_SET) != 0) {
        HLOGE("fseek() failed");
        return false;
    }
    for (int i = 0; i < attrCount; ++i) {
        std::vector<char> buf(header_.attrSize);
        CHECK_TRUE(!Read(buf.data(), buf.size()), false, 0, "");
        // size of perf_event_attr change between different linux kernel versions.
        // can not memcpy to perf_file_attr as a whole
        perf_file_attr attr {};
        size_t attrSize = header_.attrSize - sizeof(attr.ids);

        // If the size is smaller, you can use a pointer to point directly.
        // Our UAPI is 4.19. is less than 5.1
        if (header_.attrSize < sizeof(perf_event_attr)) {
            HLOGE("size not match, ptr of perf_event_attr maybe overfollow %zu vs %zu",
                  sizeof(perf_event_attr), attrSize);
        }

        attr.attr = *(reinterpret_cast<perf_event_attr *>(&buf[0]));
        attr.ids = *(reinterpret_cast<perf_file_section *>(&buf[attrSize]));
        vecAttr_.push_back(attr);
    }

    // read ids for attr
    for (size_t i = 0; i < vecAttr_.size(); ++i) {
        std::vector<uint64_t> ids;
        CHECK_TRUE(!ReadIdsForAttr(vecAttr_[i], &ids), false, 0, "");
        vecAttrIds_.push_back(ids);

        // map ids to attr index
        for (auto id : ids) {
            mapId2Attr_[id] = i;
        }
    }

    return true;
}

bool PerfFileReader::ReadIdsForAttr(const perf_file_attr &attr, std::vector<uint64_t> *ids)
{
    if (attr.ids.size > 0) {
        size_t count = attr.ids.size / sizeof(uint64_t) + 1;
        if (count > MAX_VECTOR_RESIZE_COUNT) {
            HLOGE("count(%zu) out of range", count);
            return false;
        }
        if (fseek(fp_, attr.ids.offset, SEEK_SET) != 0) {
            HLOGE("fseek() failed");
            return false;
        }
        CHECK_TRUE(ids == nullptr, false, 0, "");
        ids->resize(count);
        CHECK_TRUE(!Read(ids->data(), attr.ids.size), false, 0, "");
    }
    return true;
}

std::vector<AttrWithId> PerfFileReader::GetAttrSection() const
{
    std::vector<AttrWithId> result(vecAttr_.size());

    for (size_t i = 0; i < vecAttr_.size(); ++i) {
        result[i].attr = vecAttr_[i].attr;
        result[i].ids = vecAttrIds_[i];
    }
    return result;
}

bool PerfFileReader::ReadDataSection(ProcessRecordCB &callback)
{
    if (fseek(fp_, header_.data.offset, SEEK_SET) != 0) {
        HLOGE("fseek() failed");
        return false;
    }

    HLOGD("dataSection_ at offset %" PRId64 " + %" PRId64 "", header_.data.offset,
          header_.data.size);

    CHECK_TRUE(!ReadRecord(callback), false, LOG_TYPE_PRINTF, "some record format is error!\n");

#ifdef HIPERF_DEBUG_TIME
    printf("readRecordTime: %" PRId64 " ms\n",
           duration_cast<milliseconds>(readRecordTime_).count());
    printf("readCallbackTime: %" PRId64 " ms\n",
           duration_cast<milliseconds>(readCallbackTime_).count());
#endif
    return dataSectionSize_ == 0;
}

const perf_event_attr *PerfFileReader::GetDefaultAttr()
{
    CHECK_TRUE(vecAttr_.empty(), nullptr, 0, "");
    return &(vecAttr_[0].attr);
}

bool PerfFileReader::ReadRecord(ProcessRecordCB &callback)
{
#ifdef HIPERF_DEBUG_TIME
    const auto startReadTime = steady_clock::now();
#endif
    // record size can not exceed 64K
    HIPERF_BUF_ALIGN static uint8_t buf[RECORD_SIZE_LIMIT_SPE];
    // diff with reader
    uint64_t remainingSize = header_.data.size;
    size_t recordNumber = 0;
    const perf_event_attr *attr = GetDefaultAttr();
    CHECK_TRUE(attr == nullptr, false, 1, "attr is null");
    while (remainingSize > 0) {
        if (remainingSize < sizeof(perf_event_header)) {
            HLOGW("not enough sizeof perf_event_header");
            return false;
        } else if (!Read(buf, sizeof(perf_event_header))) {
            HLOGW("read perf_event_header failed.");
            return false;
        } else {
            perf_event_header *header = reinterpret_cast<perf_event_header *>(buf);
            if (header->size > RECORD_SIZE_LIMIT || header->size < sizeof(perf_event_header)) {
                HLOGE("read record header size error %hu", header->size);
                return false;
            }
            if (remainingSize >= header->size) {
                size_t headerSize = sizeof(perf_event_header);
                if (Read(buf + headerSize, header->size - headerSize)) {
                    size_t speSize = 0;
                    if (header->type == PERF_RECORD_AUXTRACE) {
                        struct PerfRecordAuxtraceData *auxtrace = reinterpret_cast<struct PerfRecordAuxtraceData *>
                                                                  (header + 1);
                        speSize = auxtrace->size;
                        if (speSize > 0) {
                            Read(buf + header->size, auxtrace->size);
                        }
                    }
                    uint8_t *data = buf;
                    PerfEventRecord& record = PerfEventRecordFactory::GetPerfEventRecord(
                        static_cast<perf_event_type>(header->type), data, *attr);
                    // unknown record , break the process
                    if (record.GetName() == nullptr) {
                        return false;
                    } else {
                        HLOGV("record type %u", record.GetType());
                    }
                    remainingSize = remainingSize - header->size - speSize;
#ifdef HIPERF_DEBUG_TIME
                    const auto startCallbackTime = steady_clock::now();
#endif
                    // call callback to process, then destroy record
                    callback(record);
                    recordNumber++;
#ifdef HIPERF_DEBUG_TIME
                    readCallbackTime_ +=
                        duration_cast<microseconds>(steady_clock::now() - startCallbackTime);
#endif
                } else {
                    HLOGE("read record data size failed %zu", header->size - headerSize);
                    return false;
                }
            } else {
                HLOGE("not enough header->size.");
                return false;
            }
        }
    }
    HLOGD("read back %zu records", recordNumber);
#ifdef HIPERF_DEBUG_TIME
    readRecordTime_ += duration_cast<microseconds>(steady_clock::now() - startReadTime);
#endif
    return true;
}

bool PerfFileReader::Read(void *buf, size_t len)
{
    if (buf == nullptr || len == 0) {
        HLOG_ASSERT(buf != nullptr);
        HLOG_ASSERT(len > 0);
        return false;
    }

    if (fread(buf, len, 1, fp_) != 1) {
        printf("failed to read file: %d", errno);
        return false;
    }
    return true;
}

const perf_file_header &PerfFileReader::GetHeader() const
{
    return header_;
}

bool PerfFileReader::Read(char *buf, uint64_t offset, size_t len)
{
    if (buf == nullptr || len == 0) {
        HLOG_ASSERT(buf != nullptr);
        HLOG_ASSERT(len > 0);
        return false;
    }
    if (fseek(fp_, offset, SEEK_SET) != 0) {
        HLOGE("fseek() failed");
        return false;
    }

    CHECK_TRUE(fread(buf, len, 1, fp_) != 1, false, LOG_TYPE_PRINTF, "failed to read file: %d", errno);
    HLOGM("offset %" PRIx64 " len %zu buf %x %x %x %x", offset, len, buf[0], buf[1], buf[2],
          buf[3]);
    return true;
}
const std::vector<FEATURE> &PerfFileReader::GetFeatures() const
{
    return features_;
}

const std::vector<std::unique_ptr<PerfFileSection>> &PerfFileReader::GetFeatureSections() const
{
    return perfFileSections_;
}

const std::string PerfFileReader::GetFeatureString(const FEATURE feature) const
{
    std::string featureName = PerfFileSection::GetFeatureName(feature);
    HLOGV("GetFeatureSection %s", featureName.c_str());
    if (!IsFeatrureStringSection(feature)) {
        HLOGV("not a string feature: %s", featureName.c_str());
    } else {
        const PerfFileSection *featureSection = GetFeatureSection(feature);
        if (featureSection != nullptr) {
            const PerfFileSectionString *sectionString =
                static_cast<const PerfFileSectionString *>(featureSection);
            return sectionString->ToString();
        } else {
            HLOGV("have not found: %s", featureName.c_str());
        }
    }
    return EMPTY_STRING;
}

const PerfFileSection *PerfFileReader::GetFeatureSection(FEATURE feature) const
{
    for (auto const &it : perfFileSections_) {
        HLOGV("perfFileSections");
        if (it->featureId_ == feature) {
            return it.get();
        }
    }
    return nullptr;
}

bool PerfFileReader::ReadFeatureSection()
{
    uint64_t featureSectionOffsetRead = featureSectionOffset_;
    HLOGV(" ReadDataSection data offset '0x%" PRIx64 " ", featureSectionOffset_);

    for (FEATURE feature : features_) {
        perf_file_section sectionHeader;
        // read failed ??
        CHECK_TRUE(!Read((char *)&sectionHeader, featureSectionOffsetRead, sizeof(sectionHeader)),
                   false, LOG_TYPE_PRINTF,
                   "file format not correct. featureSectionOffsetRead '0x%" PRIx64 "\n", featureSectionOffsetRead);

        HLOGV("process feature %d:%s", feature, PerfFileSection::GetFeatureName(feature).c_str());
        HLOGV(" sectionHeader -> read offset '0x%" PRIx64 " size '0x%" PRIx64 "'",
              sectionHeader.offset, sectionHeader.size);
        CHECK_TRUE(sectionHeader.size == 0 || sectionHeader.size > fileSize_, false, 1,
                   "sectionHeader.size %" PRIu64 " is not correct", sectionHeader.size);

        std::vector<char> buf(sectionHeader.size);
        // read failed ??
        CHECK_TRUE(!Read(&buf[0], sectionHeader.offset, buf.size()), false, LOG_TYPE_PRINTF,
                   "file format not correct. featureSectionDataOffset '0x%" PRIx64 "\n", sectionHeader.offset);
        if (IsFeatrureStringSection(feature)) {
            perfFileSections_.emplace_back(
                std::make_unique<PerfFileSectionString>(feature, (char *)&buf[0], buf.size()));
        } else if (feature == FEATURE::HIPERF_FILES_SYMBOL) {
            perfFileSections_.emplace_back(std::make_unique<PerfFileSectionSymbolsFiles>(
                feature, (char *)&buf[0], buf.size()));
        } else if (feature == FEATURE::EVENT_DESC) {
            perfFileSections_.emplace_back(
                std::make_unique<PerfFileSectionEventDesc>(feature, (char *)&buf[0], buf.size()));
        } else if (feature == FEATURE::HIPERF_FILES_UNISTACK_TABLE) {
            perfFileSections_.emplace_back(
                std::make_unique<PerfFileSectionUniStackTable>(feature, (char *)&buf[0], buf.size()));
            PerfRecordSample::SetDumpRemoveStack(true);
        } else {
            HLOGW("still not imp how to process with feature %d", feature);
        }

        featureSectionOffsetRead += sizeof(sectionHeader); // next feaure
    }
    return true;
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
