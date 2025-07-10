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

#define HILOG_TAG "Symbols"

#include "symbols_file.h"

#include <algorithm>
#include <chrono>
#include <cxxabi.h>
#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <string_view>
#include <type_traits>

#if defined(is_mingw) && is_mingw
#include <memoryapi.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#endif
#include <unistd.h>

#include "dfx_ark.h"
#include "dfx_extractor_utils.h"
#include "dfx_symbols.h"
#include "dwarf_encoding.h"
#include "elf_factory.h"
#include "hiperf_hilog.h"
#include "unwinder_config.h"
#include "utilities.h"
#include "ipc_utilities.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
bool SymbolsFile::onRecording_ = true;
bool SymbolsFile::needParseJsFunc_ = false;

const std::string SymbolsFile::GetBuildId() const
{
    return buildId_;
}

bool SymbolsFile::UpdateBuildIdIfMatch(std::string buildId)
{
    /*
        here we have two case
        1 buildId_ is empty
            a) always return match
        2 buildId_ is not empty
            a) really check if the same one
    */

    if (buildId_.empty()) {
        // we have new empty build
        if (buildId.empty()) {
            // both empty , no build id provided
            HLOGD("build id is empty.");
            return true;
        } else {
            buildId_ = buildId;
            HLOGD("new buildId %s", buildId_.c_str());
            return true;
        }
    } else {
        // we already have a build id
        // so this is not the first time load symbol
        // we need check if it match
        HLOGV("expected buildid: %s vs %s", buildId_.c_str(), buildId.c_str());

        if (buildId_ != buildId) {
            HLOGW("id not match");
            return false;
        } else {
            HLOGD("id match");
            return true;
        }
    }
}

std::string SymbolsFile::SearchReadableFile(const std::vector<std::string> &searchPaths,
                                            const std::string &filePath) const
{
    if (filePath.empty()) {
        HLOGW("filePath is empty, nothing to found");
        return filePath;
    }
    for (auto searchPath : searchPaths) {
        if (searchPath.empty()) {
            HLOGW("searchPath is empty.");
            continue;
        }
        if (searchPath.back() != PATH_SEPARATOR) {
            searchPath += PATH_SEPARATOR;
        }
        std::string PossibleFilePath = searchPath + filePath;
        if (CheckPathReadable(PossibleFilePath)) {
            return PossibleFilePath;
        }
        HLOGW("have not found '%s' in search paths %s", filePath.c_str(), searchPath.c_str());
    }
    return EMPTY_STRING;
}

const std::string SymbolsFile::FindSymbolFile(
    const std::vector<std::string> &symbolsFileSearchPaths, std::string symboleFilePath) const
{
    /*
        this function do 2 things:
        find by name:
            1 find dso path
            2 find search path
                a) search path + dso path
                b) search path + dso name

        show we should return filePath_ as default ?
    */
    if (symboleFilePath.empty()) {
        symboleFilePath = filePath_;
        HLOGD("use default filename: %s ", symboleFilePath.c_str());
    }
    symboleFilePath = PlatformPathConvert(symboleFilePath);
    std::string foundPath;
    // search first if we have path
    if (symbolsFileSearchPaths.size() != 0) {
        foundPath = SearchReadableFile(symbolsFileSearchPaths, symboleFilePath);
        if (foundPath.empty()) {
            HLOGV("try base name for: %s split with %s", symboleFilePath.c_str(),
                  PATH_SEPARATOR_STR.c_str());
            auto pathSplit = StringSplit(symboleFilePath, PATH_SEPARATOR_STR);
            if (pathSplit.size() > 1) {
                HLOGV("base name is: %s ", pathSplit.back().c_str());
                // found it again with base name , split it and get last name
                foundPath = SearchReadableFile(symbolsFileSearchPaths, pathSplit.back());
            }
        }
    }

    // only access the patch in onRecording_
    // in report mode we don't load any thing in runtime path
    if (foundPath.empty() && onRecording_) {
        // try access direct at last
        if (CheckPathReadable(symboleFilePath)) {
            // found direct folder
            HLOGD("find %s in current work dir", symboleFilePath.c_str());
            return symboleFilePath;
        }
    }
    return foundPath;
}

class ElfFileSymbols : public SymbolsFile {
public:
    explicit ElfFileSymbols(const std::string &symbolFilePath,
                            const SymbolsFileType symbolsFileType = SYMBOL_ELF_FILE)
        : SymbolsFile(symbolsFileType, symbolFilePath)
    {
    }

    virtual ~ElfFileSymbols()
    {
    }

    DfxSymbol GetSymbolWithPcAndMap(uint64_t pc, std::shared_ptr<DfxMap> map) override
    {
        const DfxSymbol symbol;
        return symbol;
    }

    bool LoadSymbols(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        symbolsLoaded_ = true;
        std::string findPath = FindSymbolFile(symbolsFileSearchPaths_, symbolFilePath);
        if (findPath.empty() && elfFile_ == nullptr) { // elf not compressed in hap has been initialized before
            HLOGW("elf found failed (belong to %s)", filePath_.c_str());
            return false;
        }
        if (LoadElfSymbols(map, findPath)) {
            return true;
        } else {
            HLOGW("elf open failed with '%s'", findPath.c_str());
            return false;
        }
        return false;
    }

    void EnableMiniDebugInfo() override
    {
        UnwinderConfig::SetEnableMiniDebugInfo(true);
    }

    std::shared_ptr<DfxElf> GetElfFile() override
    {
        return elfFile_;
    }

    const std::unordered_map<uint64_t, ElfLoadInfo> GetPtLoads() override
    {
        CHECK_TRUE(elfFile_ == nullptr, info_, 0, "");
        return elfFile_->GetPtLoads();
    }

protected:
    bool LoadDebugInfo(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (debugInfoLoadResult_) {
            return true; // it must have been loaded
        } else if (debugInfoLoaded_) {
            return debugInfoLoadResult_; // return the result of loaded
        } else {
            debugInfoLoaded_ = true;
        }
        std::string elfPath = FindSymbolFile(symbolsFileSearchPaths_, symbolFilePath);
        if (elfPath.empty()) {
            HLOGW("elf found failed (belong to %s)", filePath_.c_str());
            return false;
        }
        if (elfFile_ == nullptr) {
            if (StringEndsWith(elfPath, ".hap") && StringStartsWith(elfPath, "/proc")) {
                if (map == nullptr) {
                    HLOGW("map should not be nullptr.");
                    return false;
                }
                if (!map->IsMapExec()) {
                    HLOGW("map is not exec, no need parse elf.");
                    return false;
                }
                CompressHapElfFactory elfFactory(elfPath, map->prevMap);
                elfFile_ = elfFactory.Create();
                map->offset -= map->prevMap->offset;
                HLOGD("try create elf from hap");
            } else {
                RegularElfFactory elfFactory(elfPath);
                elfFile_ = elfFactory.Create();
            }
        }

        CHECK_TRUE(elfFile_ == nullptr, false, 1, "Failed to create elf file for %s.", elfPath.c_str());

        CHECK_TRUE(!elfFile_->IsValid(), false, 1, "parser elf file failed.");

        HLOGD("loaded elf %s", elfPath.c_str());
        // update path for so in hap
        if (StringEndsWith(elfPath, ".hap")) {
            filePath_ = elfPath + "!" + elfFile_->GetElfName();
            HLOGD("update path for so in hap %s.", filePath_.c_str());
            if (map == nullptr) {
                HLOGW("map should not be nullptr.");
                return false;
            }
            map->name = filePath_;
            map->elf = elfFile_;
            map->prevMap->name = filePath_;
            map->prevMap->elf = elfFile_;
        }

        textExecVaddr_ = elfFile_->GetStartVaddr();
        textExecVaddrFileOffset_ = elfFile_->GetStartOffset();

        HLOGD("textExecVaddr_ 0x%016" PRIx64 " file offset 0x%016" PRIx64 "", textExecVaddr_,
              textExecVaddrFileOffset_);

#ifndef __arm__
        ShdrInfo shinfo;
        if (elfFile_->GetSectionInfo(shinfo, ".eh_frame_hdr")) {
            auto mmapPtr = elfFile_->GetMmapPtr();
            CHECK_TRUE(mmapPtr == nullptr, false, 1, "mmapPtr should not be nullptr.");
            LoadEhFrameHDR(mmapPtr + shinfo.offset, shinfo.size, shinfo.offset);
        }
#endif

        HLOGD("LoadDebugInfo success!");
        debugInfoLoadResult_ = true;
        return true;
    }

private:
    bool EhFrameHDRValid_ {false};
    uint64_t ehFrameHDRElfOffset_ {0};
    uint64_t ehFrameHDRFdeCount_ {0};
    uint64_t ehFrameHDRFdeTableItemSize_ {0};
    uint64_t ehFrameHDRFdeTableElfOffset_ {0};
    std::shared_ptr<DfxElf> elfFile_ = nullptr;
    std::unordered_map<uint64_t, ElfLoadInfo> info_;

    bool GetSectionInfo(const std::string &name, uint64_t &sectionVaddr, uint64_t &sectionSize,
                        uint64_t &sectionFileOffset) const override
    {
        struct ShdrInfo shdrInfo;
        if (elfFile_ != nullptr && elfFile_->GetSectionInfo(shdrInfo, name)) {
            sectionVaddr = shdrInfo.addr;
            sectionSize = shdrInfo.size;
            sectionFileOffset = shdrInfo.offset;
            HLOGM("Get Section '%s' %" PRIx64 " - %" PRIx64 "", name.c_str(), sectionVaddr, sectionSize);
            return true;
        } else {
            HLOGW("Section '%s' not found", name.c_str());
            return false;
        }
    }

#ifndef __arm__
    bool GetHDRSectionInfo(uint64_t &ehFrameHdrElfOffset, uint64_t &fdeTableElfOffset,
                           uint64_t &fdeTableSize) override
    {
        CHECK_TRUE(elfFile_ == nullptr, false, 1, "elfFile_ is nullptr");
        ShdrInfo shinfo;
        if (!elfFile_->GetSectionInfo(shinfo, ".eh_frame_hdr")) {
            return false;
        }

        ehFrameHDRElfOffset_ = shinfo.offset;
        if (EhFrameHDRValid_) {
            ehFrameHdrElfOffset = ehFrameHDRElfOffset_;
            fdeTableElfOffset = ehFrameHDRFdeTableElfOffset_;
            fdeTableSize = ehFrameHDRFdeCount_;
            return true;
        }
        auto mmapPtr = elfFile_->GetMmapPtr();
        if (mmapPtr == nullptr) {
            HLOGE("mmapPtr should not be nullptr.");
            return false;
        }
        if (!LoadEhFrameHDR(mmapPtr + shinfo.offset, elfFile_->GetMmapSize(), shinfo.offset)) {
            HLOGW("Failed to load eh_frame_hdr");
            return false;
        }

        ehFrameHdrElfOffset = ehFrameHDRElfOffset_;
        fdeTableElfOffset = ehFrameHDRFdeTableElfOffset_;
        fdeTableSize = ehFrameHDRFdeCount_;
        return true;
    }
#endif

    void DumpEhFrameHDR()
    {
        HLOGD("  ehFrameHDRElfOffset_:          0x%" PRIx64 "", ehFrameHDRElfOffset_);
        HLOGD("  ehFrameHDRFdeCount_:           0x%" PRIx64 "", ehFrameHDRFdeCount_);
        HLOGD("  ehFrameHDRFdeTableElfOffset_:  0x%" PRIx64 "", ehFrameHDRFdeTableElfOffset_);
        HLOGD("  ehFrameHDRFdeTableItemSize_:   0x%" PRIx64 "", ehFrameHDRFdeTableItemSize_);
    }

    bool LoadEhFrameHDR(const unsigned char *buffer, size_t bufferSize, uint64_t shdrOffset)
    {
        const eh_frame_hdr *ehFrameHdr = reinterpret_cast<const eh_frame_hdr *>(buffer);
        CHECK_TRUE(ehFrameHdr == nullptr, false, 0, "");
        const uint8_t *dataPtr = ehFrameHdr->encode_data;
        DwarfEncoding dwEhFramePtr(ehFrameHdr->eh_frame_ptr_enc, dataPtr);
        DwarfEncoding dwFdeCount(ehFrameHdr->fde_count_enc, dataPtr);
        DwarfEncoding dwTable(ehFrameHdr->table_enc, dataPtr);
        DwarfEncoding dwTableValue(ehFrameHdr->table_enc, dataPtr);

        HLOGD("eh_frame_hdr:");
        if (HexDump(ehFrameHdr, BITS_OF_FOUR_BYTE, bufferSize) == false) {
            HLOGW("HexDump failed.");
        }
        unsigned char version = ehFrameHdr->version;
        HLOGD("  version:             %02x:%s", version, (version == 1) ? "valid" : "invalid");
        HLOGD("  eh_frame_ptr_enc:    %s", dwEhFramePtr.ToString().c_str());
        HLOGD("  fde_count_enc:       %s", dwFdeCount.ToString().c_str());
        HLOGD("  table_enc:           %s", dwTable.ToString().c_str());
        HLOGD("  table_value_enc:     %s", dwTableValue.ToString().c_str());
        HLOGD("  table_item_size:     %zd", dwTable.GetSize() + dwTableValue.GetSize());
        HLOGD("  table_offset_in_hdr: %zu", dwTable.GetData() - buffer);

        CHECK_TRUE(version != 1, false, 1, "eh_frame_hdr version is invalid");
        EhFrameHDRValid_ = true;
        ehFrameHDRElfOffset_ = shdrOffset;
        ehFrameHDRFdeCount_ = dwFdeCount.GetAppliedValue();
        ehFrameHDRFdeTableElfOffset_ = dwTable.GetData() - buffer + shdrOffset;
        ehFrameHDRFdeTableItemSize_ = dwTable.GetSize() + dwTableValue.GetSize();
        DumpEhFrameHDR();

        if (!dwFdeCount.IsOmit() && dwFdeCount.GetValue() > 0) {
            return true;
        } else {
            HLOGW("fde table not found.\n");
        }
        return false;
    }

    void UpdateSymbols(std::vector<DfxSymbol> &symbolsTable, const std::string &elfPath)
    {
        symbols_.clear();
        HLOGD("%zu symbols loadded from symbolsTable.", symbolsTable.size());

        symbols_.swap(symbolsTable);

        AdjustSymbols();
        HLOGD("%zu symbols loadded from elf '%s'.", symbols_.size(), elfPath.c_str());
        for (auto& symbol: symbols_) {
            HLOGD("symbol %s", symbol.ToDebugString().c_str());
        }
        if (buildId_.empty()) {
            HLOGD("buildId not found from elf '%s'.", elfPath.c_str());
            // don't failed. some time the lib have not got the build id
            // buildId not found from elf '/system/bin/ld-musl-arm.so.1'.
        }
    }

    void AddSymbols(std::vector<DfxSymbol>& symbolsTable, std::shared_ptr<DfxElf> elf, const std::string& filePath)
    {
        // use elfFile_ to get symbolsTable
        DfxSymbols::ParseSymbols(symbolsTable, elf, filePath);
        DfxSymbols::AddSymbolsByPlt(symbolsTable, elf, filePath);
    }

    void GetTextExecVaddr()
    {
#if defined(is_ohos) && is_ohos
        textExecVaddr_ = elfFile_->GetStartVaddr();
        textExecVaddrFileOffset_ = elfFile_->GetStartOffset();
#else
        if (textExecVaddr_ == maxVaddr) {
            textExecVaddr_ = elfFile_->GetStartVaddr();
            textExecVaddrFileOffset_ = elfFile_->GetStartOffset();
        }
#endif
        HLOGD("textExecVaddr_ 0x%016" PRIx64 " file offset 0x%016" PRIx64 "", textExecVaddr_,
              textExecVaddrFileOffset_);
    }

    bool LoadElfSymbols(std::shared_ptr<DfxMap> map, std::string elfPath)
    {
#ifdef HIPERF_DEBUG_TIME
        const auto startTime = steady_clock::now();
#endif
        if (elfFile_ == nullptr) {
            if (StringEndsWith(elfPath, ".hap") && StringStartsWith(elfPath, "/proc") && map != nullptr) {
                if (!map->IsMapExec()) {
                    HLOGW("map is not exec, no need parse elf.");
                    return false;
                }
                CompressHapElfFactory elfFactory(elfPath, map->prevMap);
                elfFile_ = elfFactory.Create();
                map->offset -= map->prevMap->offset;
                map->elf = elfFile_;
            } else {
                RegularElfFactory elfFactory(elfPath);
                elfFile_ = elfFactory.Create();
            }
        }
        CHECK_TRUE(elfFile_ == nullptr, false, 1, "Failed to create elf file for %s.", elfPath.c_str());
        HLOGD("loaded elf %s", elfPath.c_str());
        if (!elfFile_->IsValid()) {
            HLOGD("parser elf file failed.");
            return false;
        }
        GetTextExecVaddr();

        // we prepare two table here
        // only one we will push in to symbols_
        // or both drop if build id is not same
        std::string buildIdFound = elfFile_->GetBuildId();
        std::vector<DfxSymbol> symbolsTable;
        AddSymbols(symbolsTable, elfFile_, filePath_);
        if (UpdateBuildIdIfMatch(buildIdFound)) {
            UpdateSymbols(symbolsTable, elfPath);
        } else {
            HLOGW("symbols will not update for '%s' because buildId is not match.",
                  elfPath.c_str());
            // this mean failed . we don't goon for this.
            return false;
        }

#ifdef HIPERF_DEBUG_TIME
        auto usedTime = duration_cast<microseconds>(steady_clock::now() - startTime);
        if (usedTime.count() != 0) {
            HLOGV("cost %0.3f ms to load symbols '%s'",
                  usedTime.count() / static_cast<double>(milliseconds::duration::period::den),
                  elfPath.c_str());
        }
#endif
        return true;
    }

    uint64_t GetVaddrInSymbols(uint64_t ip, uint64_t mapStart,
                               uint64_t mapPageOffset) const override
    {
        /*
            00200000-002c5000 r--p 00000000 08:02 46400311
            002c5000-00490000 r-xp 000c5000 08:02 4640031

            [14] .text             PROGBITS         00000000002c5000  000c5000

            if ip is 0x46e6ab
            1. find the map range is 002c5000-00490000
            2. ip - map start(002c5000) = map section offset
            3. map section offset + map page offset(000c5000) = elf file offset
            4. elf file offset - exec file offset(000c5000)
                = ip offset (ip always in exec file offset)
            5. ip offset + exec begin vaddr(2c5000) = virtual ip in elf
        */
        uint64_t vaddr = ip - mapStart + mapPageOffset - textExecVaddrFileOffset_ + textExecVaddr_;
        HLOGM(" ip :0x%016" PRIx64 " -> elf offset :0x%016" PRIx64 " -> vaddr :0x%016" PRIx64 " ",
              ip, ip - mapStart + mapPageOffset, vaddr);
        HLOGM("(minExecAddrFileOffset_ is 0x%" PRIx64 " textExecVaddr_ is 0x%" PRIx64 ")",
              textExecVaddrFileOffset_, textExecVaddr_);
        return vaddr;
    }
};

class KernelSymbols : public ElfFileSymbols {
public:
    explicit KernelSymbols(const std::string &symbolFilePath)
        : ElfFileSymbols(symbolFilePath, SYMBOL_KERNEL_FILE)
    {
    }

    KernelSymbols(const std::string &symbolFilePath,
                  const SymbolsFileType symbolsFileType)
        : ElfFileSymbols(symbolFilePath, symbolsFileType)
    {
    }

    static constexpr const int KSYM_MIN_TOKENS = 3;
    static constexpr const int KSYM_DEFAULT_LINE = 35000;
    static constexpr const int KSYM_DEFAULT_SIZE = 1024 * 1024 * 1; // 1MB

    bool ParseKallsymsLine(const std::string &kallsymsPath)
    {
#ifdef HIPERF_DEBUG_SYMBOLS_TIME
        const auto startTime = steady_clock::now();
        std::chrono::microseconds parseLineTime = std::chrono::microseconds::zero();
        std::chrono::microseconds sscanfTime = std::chrono::microseconds::zero();
        std::chrono::microseconds newTime = std::chrono::microseconds::zero();
        std::chrono::microseconds readFileTime = std::chrono::microseconds::zero();
#endif
        size_t lines = 0;
#ifdef HIPERF_DEBUG_SYMBOLS_TIME
        const auto eachFileStartTime = steady_clock::now();
#endif
        std::string kallsym;
        CHECK_TRUE(!ReadFileToString(kallsymsPath, kallsym, KSYM_DEFAULT_SIZE) || kallsym.empty(), false, 1,
                   "%s load failed.", kallsymsPath.c_str());
#ifdef HIPERF_DEBUG_SYMBOLS_TIME
        // any way we finish the line scan
        readFileTime += duration_cast<milliseconds>(steady_clock::now() - eachFileStartTime);
#endif
        // reduce the mem alloc
        symbols_.reserve(KSYM_DEFAULT_LINE);

        char *lineBegin = kallsym.data();
        char *dataEnd = lineBegin + kallsym.size();
        while (lineBegin < dataEnd) {
            char *lineEnd = strchr(lineBegin, '\n');
            if (lineEnd != nullptr) {
                *lineEnd = '\0';
            } else {
                lineEnd = dataEnd;
            }
            size_t lineSize = (lineEnd != nullptr) ? (lineEnd - lineBegin) : (dataEnd - lineBegin);

#ifdef HIPERF_DEBUG_SYMBOLS_TIME
            const auto eachLineStartTime = steady_clock::now();
#endif
            lines++;
            uint64_t addr = 0;
            char type = '\0';

            char nameRaw[lineSize];
            char moduleRaw[lineSize];
            int ret = sscanf_s(lineBegin, "%" PRIx64 " %c %s%s", &addr, &type, sizeof(type),
                               nameRaw, sizeof(nameRaw), moduleRaw, sizeof(moduleRaw));

#ifdef HIPERF_DEBUG_SYMBOLS_TIME
            // any way we finish the line scan
            sscanfTime += duration_cast<milliseconds>(steady_clock::now() - eachLineStartTime);
#endif
            if (ret >= KSYM_MIN_TOKENS) {
                if (ret == KSYM_MIN_TOKENS) {
                    moduleRaw[0] = '\0';
                }
                HLOGM(" 0x%016" PRIx64 " %c '%s' '%s'", addr, type, nameRaw, moduleRaw);
            } else {
                HLOGW("unknown line %d: '%s'", ret, lineBegin);
                lineBegin = lineEnd + 1;
                continue;
            }
            lineBegin = lineEnd + 1;
            std::string name = nameRaw;
            std::string module = moduleRaw;

            /*
            T
            The symbol is in the text (code) section.

            W
            The symbol is a weak symbol that has not been specifically
            tagged as a weak object symbol. When a weak defined symbol is
            linked with a normal defined symbol, the normal defined symbol
            is used with no error. When a weak undefined symbol is linked
            and the symbol is not defined, the value of the weak symbol
            becomes zero with no error.
            */
            if (addr != 0 && strchr("TtWw", type)) {
#ifdef HIPERF_DEBUG_SYMBOLS_TIME
                const auto eachNewSymbolTime = steady_clock::now();
#endif
                // we only need text symbols
                symbols_.emplace_back(addr, name, module.empty() ? filePath_ : module);
#ifdef HIPERF_DEBUG_SYMBOLS_TIME
                newTime += duration_cast<milliseconds>(steady_clock::now() - eachNewSymbolTime);
#endif
            }
#ifdef HIPERF_DEBUG_SYMBOLS_TIME
            parseLineTime += duration_cast<milliseconds>(steady_clock::now() - eachLineStartTime);
#endif
        }
#ifdef HIPERF_DEBUG_SYMBOLS_TIME
        std::chrono::microseconds usedTime =
            duration_cast<milliseconds>(steady_clock::now() - startTime);
        printf("parse kernel symbols use : %0.3f ms\n", usedTime.count() / MS_DURATION);
        printf("parse line use : %0.3f ms\n", parseLineTime.count() / MS_DURATION);
        printf("sscanf line use : %0.3f ms\n", sscanfTime.count() / MS_DURATION);
        printf("new symbols use : %0.3f ms\n", newTime.count() / MS_DURATION);
        printf("read file use : %0.3f ms\n", readFileTime.count() / MS_DURATION);
#endif
        HLOGD("load %s: %zu line processed(%zu symbols)", kallsymsPath.c_str(), lines, symbols_.size());
        return true;
    }

    const std::string KPTR_RESTRICT = "/proc/sys/kernel/kptr_restrict";

    bool LoadKernelSyms()
    {
        if (!IsRoot()) {
            return false;
        }
        HLOGD("try read /proc/kallsyms");
        if (access("/proc/kallsyms", R_OK) != 0) {
            printf("No vmlinux path is given, and kallsyms cannot be opened\n");
            return false;
        }
        bool hasChangeKptr = false;
        std::string oldKptrRestrict = ReadFileToString(KPTR_RESTRICT);
        if (oldKptrRestrict.front() != '0') {
            printf("/proc/sys/kernel/kptr_restrict is NOT 0, will try set it to 0.\n");
            hasChangeKptr = WriteStringToFile(KPTR_RESTRICT, "0");
            if (!hasChangeKptr) {
                printf("/proc/sys/kernel/kptr_restrict write failed and we can't not change it.\n");
            }
        }

        // getline end
        CHECK_TRUE(!ParseKallsymsLine("/proc/kallsyms"), false, 0, "");

        if (hasChangeKptr) {
            if (!WriteStringToFile(KPTR_RESTRICT, oldKptrRestrict)) {
                printf("recover /proc/sys/kernel/kptr_restrict fail.\n");
            }
        }

        if (symbols_.empty()) {
            printf("The symbol table addresses in /proc/kallsyms are all 0.\n"
                   "Please check the value of /proc/sys/kernel/kptr_restrict, it "
                   "should be 0.\n"
                   "Or provide a separate vmlinux path.\n");

            if (buildId_.size() != 0) {
                // but we got the buildid , so we make a dummpy symbols
                HLOGD("kallsyms not found. but we have the buildid");
                return true;
            } else {
                // we got nothing
                return false;
            }
        } else {
            AdjustSymbols();
            HLOGV("%zu symbols_ loadded from kallsyms.\n", symbols_.size());
            return true;
        }
    }
    bool LoadSymbols(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        symbolsLoaded_ = true;
        HLOGV("KernelSymbols try read '%s' search paths size %zu, inDeviceRecord %d",
              symbolFilePath.c_str(), symbolsFileSearchPaths_.size(), onRecording_);

        if (onRecording_) {
            const auto startTime = std::chrono::steady_clock::now();
            if (!LoadKernelSyms()) {
                if (IsRoot()) {
                    printf("parse kalsyms failed.\n");
                }
                return false;
            } else {
                const auto thisTime = std::chrono::steady_clock::now();
                const auto usedTimeMsTick =
                    std::chrono::duration_cast<std::chrono::milliseconds>(thisTime - startTime);
                HLOGV("Load kernel symbols (total %" PRId64 " ms)\n", (int64_t)usedTimeMsTick.count());
                // load complete
                return true;
            }

            // try read
            HLOGD("try read /sys/kernel/notes");
            std::string notes = ReadFileToString("/sys/kernel/notes");
            if (notes.empty()) {
                printf("notes cannot be opened, unable get buildid\n");
                return false;
            } else {
                HLOGD("kernel notes size: %zu", notes.size());
                buildId_ = DfxElf::GetBuildId((uint64_t)notes.data(), (uint64_t)notes.size());
            }
        } // no search path

        // try vmlinux
        return ElfFileSymbols::LoadSymbols(nullptr, KERNEL_ELF_NAME);
    }
    uint64_t GetVaddrInSymbols(uint64_t ip, uint64_t mapStart, uint64_t) const override
    {
        // ip is vaddr in /proc/kallsyms
        return ip;
    }
    ~KernelSymbols() override {}
};

class KernelThreadSymbols : public KernelSymbols {
public:
    explicit KernelThreadSymbols(const std::string &symbolFilePath)
        : KernelSymbols(symbolFilePath, SYMBOL_KERNEL_THREAD_FILE)
    {
    }

    bool LoadKernelSyms()
    {
        if (!IsRoot()) {
            return false;
        }
        // find real proc path by filePath_
        std::string procPath;
        if (filePath_ == SYSMGR_FILE_NAME) {
            procPath = StringPrintf("/proc/%u/uallsyms", SYSMGR_PID);
        } else if (filePath_ == DEVHOST_FILE_NAME) {
            procPath = "/proc/devhost/root/kallsyms";
        }
        HLOGD("try read kernel thread symbol file %s in %s", filePath_.c_str(), procPath.c_str());
        CHECK_TRUE(access(procPath.c_str(), R_OK) != 0, false, LOG_TYPE_PRINTF,
                   "kernel thread symbol file %s cannot be opened\n", filePath_.c_str());
        bool hasChangeKptr = false;
        std::string oldKptrRestrict = ReadFileToString(KPTR_RESTRICT);
        if (oldKptrRestrict.front() != '0') {
            printf("/proc/sys/kernel/kptr_restrict is NOT 0, will try set it to 0.\n");
            hasChangeKptr = WriteStringToFile(KPTR_RESTRICT, "0");
            if (!hasChangeKptr) {
                printf("/proc/sys/kernel/kptr_restrict write failed and we can't not change it.\n");
            }
        }

        // getline end
        CHECK_TRUE(!ParseKallsymsLine(procPath), false, 0, "");

        if (hasChangeKptr) {
            if (!WriteStringToFile(KPTR_RESTRICT, oldKptrRestrict)) {
                printf("recover /proc/sys/kernel/kptr_restrict fail.\n");
            }
        }

        if (symbols_.empty()) {
            printf("The symbol table addresses in %s are all 0.\n"
                   "Please check the value of /proc/sys/kernel/kptr_restrict, it "
                   "should be 0.\n", filePath_.c_str());
            return false;
        } else {
            AdjustSymbols();
            HLOGV("%zu symbols_ loadded from %s.\n", symbols_.size(), procPath.c_str());
            return true;
        }
    }

    bool LoadSymbols(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        symbolsLoaded_ = true;
        HLOGV("KernelThreadSymbols try read '%s', inDeviceRecord %d",
              filePath_.c_str(), onRecording_);

        if (onRecording_) {
            const auto startTime = std::chrono::steady_clock::now();
            if (!LoadKernelSyms()) {
                if (IsRoot()) {
                    printf("parse %s failed.\n", filePath_.c_str());
                }
            } else {
                const auto thisTime = std::chrono::steady_clock::now();
                const auto usedTimeMsTick =
                    std::chrono::duration_cast<std::chrono::milliseconds>(thisTime - startTime);
                HLOGV("Load kernel thread symbols (total %" PRId64 " ms)\n", (int64_t)usedTimeMsTick.count());
                // load complete
                return true;
            }
        } // no search path

        // try elf
        return ElfFileSymbols::LoadSymbols(nullptr, filePath_);
    }
    ~KernelThreadSymbols() override {}
};

class KernelModuleSymbols : public ElfFileSymbols {
public:
    explicit KernelModuleSymbols(const std::string &symbolFilePath) : ElfFileSymbols(symbolFilePath)
    {
        HLOGV("create %s", symbolFilePath.c_str());
        symbolFileType_ = SYMBOL_KERNEL_MODULE_FILE;
        module_ = symbolFilePath;
    }
    ~KernelModuleSymbols() override {};

    bool LoadSymbols(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        symbolsLoaded_ = true;
        if (module_ == filePath_ && onRecording_) {
            // file name still not convert to ko file path
            // this is in record mode
            HLOGV("find ko name %s", module_.c_str());
            for (const std::string &path : kernelModulePaths) {
                if (access(path.c_str(), R_OK) == 0) {
                    std::string koPath = path + module_ + KERNEL_MODULES_EXT_NAME;
                    HLOGV("found ko in %s", koPath.c_str());
                    if (access(koPath.c_str(), R_OK) == 0) {
                        // create symbol
                        filePath_ = koPath;
                        break; // find next ko
                    }
                }
            }
            LoadBuildId();
        } else {
            HLOGV("we have file path, load with %s", filePath_.c_str());
            return ElfFileSymbols::LoadSymbols(nullptr, filePath_);
        }
        return false;
    }
    uint64_t GetVaddrInSymbols(uint64_t ip, uint64_t mapStart, uint64_t) const override
    {
        return ip - mapStart;
    }

private:
    bool LoadBuildId()
    {
        std::string sysFile = "/sys/module/" + module_ + "/notes/.note.gnu.build-id";
        std::string buildIdRaw = ReadFileToString(sysFile);
        if (!buildIdRaw.empty()) {
            buildId_ = DfxElf::GetBuildId((uint64_t)buildIdRaw.data(), (uint64_t)buildIdRaw.size());
            HLOGD("kerne module %s(%s) build id %s", module_.c_str(), filePath_.c_str(),
                  buildId_.c_str());
            return buildId_.empty() ? false : true;
        }
        return false;
    }

    const std::vector<std::string> kernelModulePaths = {"/vendor/modules/"};
    std::string module_ = "";
};

class JavaFileSymbols : public ElfFileSymbols {
public:
    explicit JavaFileSymbols(const std::string &symbolFilePath) : ElfFileSymbols(symbolFilePath)
    {
        symbolFileType_ = SYMBOL_KERNEL_FILE;
    }
    bool LoadSymbols(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        symbolsLoaded_ = true;
        return false;
    }
    ~JavaFileSymbols() override {}

    uint64_t GetVaddrInSymbols(uint64_t ip, uint64_t mapStart,
                                       uint64_t mapPageOffset) const override
    {
        // this is different with elf
        // elf use  ip - mapStart + mapPageOffset - minExecAddrFileOffset_ + textExecVaddr_
        return ip - mapStart + mapPageOffset;
    }
};

class JSFileSymbols : public ElfFileSymbols {
public:
    explicit JSFileSymbols(const std::string &symbolFilePath) : ElfFileSymbols(symbolFilePath)
    {
        symbolFileType_ = SYMBOL_KERNEL_FILE;
    }
    bool LoadSymbols(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        symbolsLoaded_ = true;
        return false;
    }
    ~JSFileSymbols() override {}
};

class HapFileSymbols : public ElfFileSymbols {
private:
#if defined(is_ohos) && is_ohos
    std::unique_ptr<DfxExtractor> dfxExtractor_;
    bool hapExtracted_ = false;
#endif
    std::unique_ptr<uint8_t[]> abcDataPtr_ = nullptr;
    [[maybe_unused]] uintptr_t loadOffSet_ = 0;
    [[maybe_unused]] size_t abcDataSize_ = 0;
    [[maybe_unused]] uintptr_t arkExtractorptr_ = 0;
    bool isHapAbc_ = false;
    pid_t pid_ = 0;
public:
    explicit HapFileSymbols(const std::string &symbolFilePath, pid_t pid)
        : ElfFileSymbols(symbolFilePath, SYMBOL_HAP_FILE)
    {
        pid_ = pid;
    }

    ~HapFileSymbols() override
    {
#if defined(is_ohos) && is_ohos
        abcDataPtr_ = nullptr;
        if (arkExtractorptr_ != 0) {
            DfxArk::Instance().ArkDestoryJsSymbolExtractor(arkExtractorptr_);
            arkExtractorptr_ = 0;
        }
#endif
    }

    bool IsHapAbc()
    {
#if defined(is_ohos) && is_ohos
        if (hapExtracted_) {
            return isHapAbc_;
        }
        hapExtracted_ = true;
        HLOGD("the symbol file is %s, pid is %d.", filePath_.c_str(), pid_);
        if (IsRoot()) {
            if (IsApplicationEncryped(pid_)) {
                HLOGD("no need to parse js symbols");
                return false;
            }
        }

        CHECK_TRUE(StringEndsWith(filePath_, ".hap") && map_->IsMapExec(), false, 1,
                   "map is exec not abc file , the symbol file is:%s", map_->name.c_str());

        if (StringEndsWith(filePath_, ".hap") || StringEndsWith(filePath_, ".hsp") ||
            StringEndsWith(filePath_, ".hqf")) {
            dfxExtractor_ = std::make_unique<DfxExtractor>(filePath_);
            CHECK_TRUE(!dfxExtractor_->GetHapAbcInfo(loadOffSet_, abcDataPtr_, abcDataSize_), false, 1,
                       "failed to call GetHapAbcInfo, the symbol file is:%s", filePath_.c_str());
            HLOGD("loadOffSet %u", (uint32_t)loadOffSet_);
            if (abcDataPtr_ != nullptr) {
                isHapAbc_ = true;
                HLOGD("symbol file : %s, isAbc: %d", filePath_.c_str(), isHapAbc_);
            }
        } else {
            loadOffSet_ = map_->offset;
            abcDataSize_ = map_->end - map_->begin;
            abcDataPtr_ = std::make_unique<uint8_t[]>(abcDataSize_);
            auto size = DfxMemory::ReadProcMemByPid(pid_, map_->begin, abcDataPtr_.get(), map_->end - map_->begin);
            if (size != abcDataSize_) {
                HLOGD("return size is small abcDataPtr : %s, isAbc: %d", abcDataPtr_.get(), isHapAbc_);
                return false;
            }
            isHapAbc_ = true;
            HLOGD("symbol file name %s loadOffSet %u abcDataSize_ %u",
                  filePath_.c_str(), (uint32_t)loadOffSet_, (uint32_t)abcDataSize_);
        }
        auto ret = DfxArk::Instance().ArkCreateJsSymbolExtractor(&arkExtractorptr_);
        if (ret < 0) {
            arkExtractorptr_ = 0;
            HLOGE("failed to call ArkCreateJsSymbolExtractor, the symbol file is:%s", filePath_.c_str());
        }
#endif
        return isHapAbc_;
    }

    bool IsAbc() override
    {
        return isHapAbc_ == true;
    }

    void SetBoolValue(bool value) override
    {
        isHapAbc_ = value;
    }

    bool LoadDebugInfo(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        if (map == nullptr) {
            HLOGD("map is null, symbolFilePath: %s", symbolFilePath.c_str());
            return false;
        }
        HLOGD("map ptr:%p, map name:%s", map.get(), map->name.c_str());
        if (debugInfoLoaded_) {
            return true;
        }
        CHECK_TRUE(!onRecording_, true, 0, "");

        if (!IsHapAbc() && map_->IsMapExec()) {
            ElfFileSymbols::LoadDebugInfo(map, "");
        }
        debugInfoLoaded_ = true;
        debugInfoLoadResult_ = true;
        return true;
    }

    bool LoadSymbols(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        if (map == nullptr) {
            HLOGD("map is null, symbolFilePath: %s", symbolFilePath.c_str());
            return false;
        }
        HLOGD("map ptr:%p, map name:%s", map.get(), map->name.c_str());
        CHECK_TRUE(symbolsLoaded_ || !onRecording_, true, 0, "");
        symbolsLoaded_ = true;
        if (!IsHapAbc() && map_->IsMapExec()) {
            ElfFileSymbols::LoadSymbols(map, "");
        }
        return true;
    }

    DfxSymbol GetSymbolWithPcAndMap(uint64_t ip, std::shared_ptr<DfxMap> map) override
    {
        // get cache
        auto iter = symbolsMap_.find(ip);
        if (iter != symbolsMap_.end()) {
            return iter->second;
        }
        if (map == nullptr) {
            return DfxSymbol(ip, "");
        }
        HLOGD("map ptr:%p, map name:%s", map.get(), map->name.c_str());

#if defined(is_ohos) && is_ohos
        if (IsAbc() && needParseJsFunc_) {
            JsFunction jsFunc;
            std::string module = map->name;
            HLOGD("map->name module:%s", module.c_str());
            auto ret = DfxArk::Instance().ParseArkFrameInfo(static_cast<uintptr_t>(ip),
                                                            static_cast<uintptr_t>(map->begin),
                                                            loadOffSet_, abcDataPtr_.get(), abcDataSize_,
                                                            arkExtractorptr_, &jsFunc);
            if (ret == -1) {
                HLOGD("failed to call ParseArkFrameInfo, the symbol file is : %s", map->name.c_str());
                return DfxSymbol(ip, "");
            }
            this->symbolsMap_.insert(std::make_pair(ip,
                                                    DfxSymbol(ip,
                                                    jsFunc.codeBegin,
                                                    jsFunc.functionName,
                                                    jsFunc.ToString(),
                                                    map->name)));

            DfxSymbol &foundSymbol = symbolsMap_[ip];
            if (!foundSymbol.matched_) {
                foundSymbol.matched_ = true;
                foundSymbol.symbolFileIndex_ = id_;
                matchedSymbols_.push_back(&(symbolsMap_[ip]));
            }

            HLOGD("ip : 0x%" PRIx64 " the symbol file is : %s, function is %s demangle_ : %s", ip,
                  symbolsMap_[ip].module_.data(), jsFunc.functionName, matchedSymbols_.back()->demangle_.data());
            return symbolsMap_[ip];
        }
#endif
        DfxSymbol symbol(ip, "");
        return symbol;
    }
};

class UnknowFileSymbols : public SymbolsFile {
public:
    explicit UnknowFileSymbols(const std::string &symbolFilePath)
        : SymbolsFile(SYMBOL_UNKNOW_FILE, symbolFilePath)
    {
    }
    bool LoadSymbols(std::shared_ptr<DfxMap> map, const std::string &symbolFilePath) override
    {
        symbolsLoaded_ = true;
        return false;
    }
    ~UnknowFileSymbols() override {}
};

class CJFileSymbols : public ElfFileSymbols {
public:
    explicit CJFileSymbols(const std::string &symbolFilePath) : ElfFileSymbols(symbolFilePath)
    {
        symbolFileType_ = SYMBOL_CJ_FILE;
    }
    ~CJFileSymbols() override {}
};

SymbolsFile::~SymbolsFile() {}

std::unique_ptr<SymbolsFile> SymbolsFile::CreateSymbolsFile(SymbolsFileType symbolType,
                                                            const std::string symbolFilePath, pid_t pid)
{
    switch (symbolType) {
        case SYMBOL_KERNEL_FILE:
            return std::make_unique<KernelSymbols>(symbolFilePath.empty() ? KERNEL_MMAP_NAME
                                                                          : symbolFilePath);
        case SYMBOL_KERNEL_MODULE_FILE:
            return std::make_unique<KernelModuleSymbols>(symbolFilePath);
        case SYMBOL_KERNEL_THREAD_FILE:
            return std::make_unique<KernelThreadSymbols>(symbolFilePath);
        case SYMBOL_ELF_FILE:
            return std::make_unique<ElfFileSymbols>(symbolFilePath);
        case SYMBOL_JAVA_FILE:
            return std::make_unique<JavaFileSymbols>(symbolFilePath);
        case SYMBOL_JS_FILE:
            return std::make_unique<JSFileSymbols>(symbolFilePath);
        case SYMBOL_HAP_FILE:
            return std::make_unique<HapFileSymbols>(symbolFilePath, pid);
        case SYMBOL_CJ_FILE:
            return std::make_unique<CJFileSymbols>(symbolFilePath);
        default:
            return std::make_unique<SymbolsFile>(SYMBOL_UNKNOW_FILE, symbolFilePath);
    }
}

static bool IsCJFile(const std::string& filepath)
{
    RegularElfFactory elfFactory(filepath);
    std::shared_ptr<DfxElf> elfFile_ = elfFactory.Create();
    ShdrInfo shinfo;
    if (elfFile_->GetSectionInfo(shinfo, ".cjmetadata")) {
        return true;
    }
    return false;
}

std::unique_ptr<SymbolsFile> SymbolsFile::CreateSymbolsFile(const std::string &symbolFilePath, pid_t pid)
{
    // we need check file name here
    if (symbolFilePath == KERNEL_MMAP_NAME) {
        return SymbolsFile::CreateSymbolsFile(SYMBOL_KERNEL_FILE, symbolFilePath);
    } else if (symbolFilePath == SYSMGR_FILE_NAME ||
               symbolFilePath == DEVHOST_LINUX_FILE_NAME ||
               StringStartsWith(symbolFilePath, DEVHOST_LINUX_PREFIX)) {
        return SymbolsFile::CreateSymbolsFile(SYMBOL_KERNEL_THREAD_FILE, symbolFilePath);
    } else if (StringEndsWith(symbolFilePath, KERNEL_MODULES_EXT_NAME)) {
        return SymbolsFile::CreateSymbolsFile(SYMBOL_KERNEL_MODULE_FILE, symbolFilePath);
    } else if (IsArkJsFile(symbolFilePath)) {
        return SymbolsFile::CreateSymbolsFile(SYMBOL_HAP_FILE, symbolFilePath, pid);
    } else if (IsCJFile(symbolFilePath)) {
        return SymbolsFile::CreateSymbolsFile(SYMBOL_CJ_FILE, symbolFilePath, pid);
    } else {
        // default is elf, this may be problematic in the future.
        return SymbolsFile::CreateSymbolsFile(SYMBOL_ELF_FILE, symbolFilePath);
    }
}

void SymbolsFile::AdjustSymbols()
{
    if (symbols_.size() <= 0) {
        return;
    }

    // order
    sort(symbols_.begin(), symbols_.end(), [](const DfxSymbol& a, const DfxSymbol& b) {
        return a.funcVaddr_ < b.funcVaddr_;
    });
    HLOGV("sort completed");

    size_t fullSize = symbols_.size();
    size_t erased = 0;

    // Check for duplicate vaddr
    auto last = std::unique(symbols_.begin(), symbols_.end(), [](const DfxSymbol &a, const DfxSymbol &b) {
        return (a.funcVaddr_ == b.funcVaddr_);
    });
    symbols_.erase(last, symbols_.end());
    erased = fullSize - symbols_.size();
    HLOGV("uniqued completed");
    auto it = symbols_.begin();
    while (it != symbols_.end()) {
        it->index_ = it - symbols_.begin();
        it++;
    }
    HLOGV("indexed completed");

    HLOG_ASSERT(symbols_.size() != 0);

    if (textExecVaddrRange_ == maxVaddr) {
        textExecVaddrRange_ = symbols_.back().funcVaddr_ - symbols_.front().funcVaddr_;
    }

    HLOGDDD("%zu symbols after adjust (%zu erased) 0x%016" PRIx64 " - 0x%016" PRIx64
            " @0x%016" PRIx64 " ",
            symbols_.size(), erased, symbols_.front().funcVaddr_, symbols_.back().funcVaddr_,
            textExecVaddrFileOffset_);
}

void SymbolsFile::SortMatchedSymbols()
{
    if (matchedSymbols_.size() <= 1u) {
        return;
    }
    sort(matchedSymbols_.begin(), matchedSymbols_.end(), [](const DfxSymbol* a, const DfxSymbol* b) {
        if (a == nullptr || b == nullptr) {
            return true;
        }
        return a->funcVaddr_ < b->funcVaddr_;
    });
}

const std::vector<DfxSymbol> &SymbolsFile::GetSymbols()
{
    return symbols_;
}

const std::vector<DfxSymbol *> &SymbolsFile::GetMatchedSymbols()
{
    return matchedSymbols_;
}

const DfxSymbol SymbolsFile::GetSymbolWithVaddr(uint64_t vaddrInFile)
{
#ifdef HIPERF_DEBUG_TIME
    const auto startTime = steady_clock::now();
#endif
    DfxSymbol symbol;
    // it should be already order from small to large
    auto found =
        std::upper_bound(symbols_.begin(), symbols_.end(), vaddrInFile, DfxSymbol::ValueLessThen);
    /*
    if data is { 1, 2, 4, 5, 5, 6 };
    upper_bound for each val :
        0 < 1 at index 0
        1 < 2 at index 1
        2 < 4 at index 2
        3 < 4 at index 2
        4 < 5 at index 3
        5 < 6 at index 5
        6 < not found
    if key symbol vaddr is { 1, 2, 4, 5, 5, 6 };
     check ip vaddr for each val :
       ip   sym
        0   not found
        1   1
        1   1
        2   2
        3   3
        4   4
        5   5
        6   6
        7   7
    */
    if (found != symbols_.begin()) {
        found = std::prev(found);
        if (found != symbols_.end()) {
            if (found->Contain(vaddrInFile)) {
                found->offsetToVaddr_ = vaddrInFile - found->funcVaddr_;
                if (!found->matched_) {
                    found->matched_ = true;
                    matchedSymbols_.push_back(&(*found));
                }
                symbol = *found; // copy
                HLOGV("found '%s' for vaddr 0x%016" PRIx64 "", symbol.ToString().c_str(), vaddrInFile);
            }
        }
    }

    if (!symbol.IsValid()) {
        HLOGV("NOT found vaddr 0x%" PRIx64 " in symbole file %s(%zu)", vaddrInFile,
              filePath_.c_str(), symbols_.size());
    }
    symbol.fileVaddr_ = vaddrInFile;
    symbol.symbolFileIndex_ = id_;

#ifdef HIPERF_DEBUG_TIME
    auto usedTime = duration_cast<milliseconds>(steady_clock::now() - startTime);
    if (usedTime > 1ms) {
        HLOGW("cost %" PRId64 "ms to search ", usedTime.count());
    }
#endif
    return symbol;
}

bool SymbolsFile::CheckPathReadable(const std::string &path) const
{
    if (access(path.c_str(), R_OK) == 0) {
        return true;
    }
    // access does not have the access permission in some scenarios
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return true;
    } else {
#if defined(is_ohos) && is_ohos
        char errInfo[ERRINFOLEN] = { 0 };
        strerror_r(errno, errInfo, ERRINFOLEN);
        HLOGM("'%s' is unable read,errno: %d, errmsg: %s", path.c_str(), errno, errInfo);
#endif
        return false;
    }
}

bool SymbolsFile::setSymbolsFilePath(const std::vector<std::string> &symbolsSearchPaths)
{
    symbolsFileSearchPaths_.clear();
    for (auto &symbolsSearchPath : symbolsSearchPaths) {
        if (CheckPathReadable(symbolsSearchPath)) {
            symbolsFileSearchPaths_.emplace_back(symbolsSearchPath);
            HLOGV("'%s' is add to symbolsSearchPath", symbolsSearchPath.c_str());
        }
    }
    return (symbolsFileSearchPaths_.size() > 0);
}

std::unique_ptr<SymbolsFile> SymbolsFile::LoadSymbolsFromSaved(
    const SymbolFileStruct &symbolFileStruct)
{
    bool isHapSymbolFile = static_cast<SymbolsFileType>(symbolFileStruct.symbolType_) == SYMBOL_HAP_FILE;
    HLOGD("isHapSymbolFile : %d", isHapSymbolFile);
    auto symbolsFile = CreateSymbolsFile(symbolFileStruct.filePath_);

    // default create elf file. but hap file need special operation.
    symbolsFile->filePath_ = symbolFileStruct.filePath_;
    symbolsFile->symbolFileType_ = static_cast<SymbolsFileType>(symbolFileStruct.symbolType_);
    symbolsFile->textExecVaddr_ = symbolFileStruct.textExecVaddr_;
    symbolsFile->textExecVaddrFileOffset_ = symbolFileStruct.textExecVaddrFileOffset_;
    symbolsFile->buildId_ = symbolFileStruct.buildId_;
    for (auto &symbolStruct : symbolFileStruct.symbolStructs_) {
        symbolsFile->symbols_.emplace_back(symbolStruct.vaddr_, symbolStruct.len_,
                                           symbolStruct.symbolName_, symbolFileStruct.filePath_);
    }
    symbolsFile->AdjustSymbols(); // reorder
    if (isHapSymbolFile) {
        for (const auto& symbol : symbolsFile->symbols_) {
            symbolsFile->symbolsMap_.emplace(symbol.funcVaddr_, symbol);
        }
        symbolsFile->SetBoolValue(true);
    }
    symbolsFile->debugInfoLoadResult_ = true;
    symbolsFile->symbolsLoaded_ = true; // all ready LoadFrom perf.data
    HLOGV("load %zu symbol from SymbolFileStruct for file '%s'", symbolsFile->symbols_.size(),
          symbolsFile->filePath_.c_str());
    return symbolsFile;
}

void SymbolsFile::SetBoolValue(bool value)
{
}

void SymbolsFile::ExportSymbolToFileFormat(SymbolFileStruct &symbolFileStruct)
{
    symbolFileStruct.filePath_ = filePath_;
    symbolFileStruct.symbolType_ = symbolFileType_;
    symbolFileStruct.textExecVaddr_ = textExecVaddr_;
    symbolFileStruct.textExecVaddrFileOffset_ = textExecVaddrFileOffset_;
    symbolFileStruct.buildId_ = buildId_;

    SortMatchedSymbols();
    auto symbols = GetMatchedSymbols();
    symbolFileStruct.symbolStructs_.reserve(symbols.size());
    for (const auto symbol : symbols) {
        auto &symbolStruct = symbolFileStruct.symbolStructs_.emplace_back();
        symbolStruct.vaddr_ = symbol->funcVaddr_;
        symbolStruct.len_ = symbol->size_;
        symbolStruct.symbolName_ = symbol->GetName();
    }

    HLOGV("export %zu symbol to SymbolFileStruct from %s", symbolFileStruct.symbolStructs_.size(),
          filePath_.c_str());
}

uint64_t SymbolsFile::GetVaddrInSymbols(uint64_t ip, uint64_t mapStart, uint64_t mapOffset) const
{
    // no convert
    return ip;
}

void SymbolsFile::AddSymbol(DfxSymbol symbol)
{
    symbolsLoaded_ = true;
    symbols_.emplace_back(symbol);
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
