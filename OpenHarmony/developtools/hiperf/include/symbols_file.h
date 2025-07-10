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

#ifndef HIPERF_SYMBOLS_H
#define HIPERF_SYMBOLS_H

#include <cinttypes>
#include <iomanip>
#include <sstream>
#include <string>

#include "dfx_elf.h"
#include "dfx_symbol.h"
#include "perf_file_format.h"
#include "utilities.h"

#define HIPERF_ELF_READ_USE_MMAP

namespace OHOS {
namespace Developtools {
namespace HiPerf {
using namespace OHOS::HiviewDFX;

constexpr const char KERNEL_MMAP_NAME[] = "[kernel.kallsyms]";
constexpr const char KERNEL_MODULES_EXT_NAME[] = ".ko";
constexpr const char KERNEL_ELF_NAME[] = "vmlinux";
constexpr const char MMAP_VDSO_NAME[] = "[vdso]";
constexpr const char MMAP_ANONYMOUS_NAME[] = "[anon]";
constexpr const char MMAP_ANONYMOUS_OHOS_NAME[] = "//anon";

const std::string NOTE_GNU_BUILD_ID = ".note.gnu.build-id";
const std::string EH_FRAME_HR = ".eh_frame_hdr";
const std::string EH_FRAME = ".eh_frame";
const std::string ARM_EXIDX = ".ARM.exidx";
const std::string SYMTAB = ".symtab";
const std::string DYNSYM = ".dynsym";
const std::string GNU_DEBUGDATA = ".gnu_debugdata";
const std::string PLT = ".plt";
const std::string LINKER_PREFIX = "__dl_";
const std::string LINKER_PREFIX_NAME = "[linker]";

const int MAX_SYMBOLS_TYPE_NAME_LEN = 10;

const pid_t SYSMGR_PID = 2;
const std::string SYSMGR_NAME = "sysmgr";
const std::string SYSMGR_FILE_NAME = "sysmgr.elf";
const std::string DEVHOST_FILE_NAME = "devhost.elf";
const std::string DEVHOST_LINUX_FILE_NAME = "/lib/libdh-linux.so";
const std::string DEVHOST_LINUX_PREFIX = "/liblinux/";

class FileSymbol {
    [[maybe_unused]] uint64_t vaddr_ = 0;
    [[maybe_unused]] uint64_t len_ = 0;
    std::string name_ = "";
    std::string demangle_ = ""; // demangle string
    FileSymbol(uint64_t vaddr, uint64_t len, const char *name, const char *demangle)
        : vaddr_(vaddr), len_(len), name_(name), demangle_(demangle)
    {
    }
};

enum SymbolsFileType {
    SYMBOL_KERNEL_FILE,
    SYMBOL_KERNEL_MODULE_FILE,
    SYMBOL_KERNEL_THREAD_FILE,
    // .hap!elf, normal elf.
    SYMBOL_ELF_FILE,
    SYMBOL_JAVA_FILE,
    SYMBOL_JS_FILE,
    SYMBOL_HAP_FILE,
    SYMBOL_CJ_FILE,
    SYMBOL_UNKNOW_FILE,
};

class SymbolsFile {
public:
    SymbolsFileType symbolFileType_;
    std::string filePath_ = "";
    int32_t id_ = -1; // used to report protobuf file

    // [14] .text             PROGBITS         00000000002c5000  000c5000
    // min exec addr , general it point to .text
    // we make a default value for min compare
    static const uint64_t maxVaddr = std::numeric_limits<uint64_t>::max();

    uint64_t textExecVaddr_ = maxVaddr;
    uint64_t textExecVaddrFileOffset_ = 0;
    uint64_t textExecVaddrRange_ = maxVaddr;
    std::shared_ptr<DfxMap> map_ = nullptr;

    SymbolsFile(SymbolsFileType symbolType, const std::string path)
        : symbolFileType_(symbolType), filePath_(path) {};
    virtual ~SymbolsFile();

    virtual std::shared_ptr<DfxElf> GetElfFile()
    {
        return nullptr;
    }

    // create the symbols file object
    static std::unique_ptr<SymbolsFile> CreateSymbolsFile(
        SymbolsFileType = SYMBOL_UNKNOW_FILE, const std::string symbolFilePath = EMPTY_STRING, pid_t pid = 0);
    static std::unique_ptr<SymbolsFile> CreateSymbolsFile(const std::string &symbolFilePath, pid_t pid = 0);

    // set symbols path
    bool setSymbolsFilePath(const std::string &symbolsSearchPath)
    {
        std::vector<std::string> symbolsSearchPaths = {symbolsSearchPath};
        return setSymbolsFilePath(symbolsSearchPaths);
    };
    bool setSymbolsFilePath(const std::vector<std::string> &);
    virtual bool IsAbc()
    {
        return false;
    }

    virtual void SetBoolValue(bool value);
    // load symbol from file
    virtual bool LoadSymbols([[maybe_unused]] std::shared_ptr<DfxMap> map = nullptr,
                             [[maybe_unused]] const std::string &symbolFilePath = EMPTY_STRING)
    {
        HLOGV("virtual dummy function called");
        symbolsLoaded_ = true;
        return false;
    };
    // load debug info for unwind
    virtual bool LoadDebugInfo(std::shared_ptr<DfxMap> map = nullptr,
                               [[maybe_unused]] const std::string &symbolFilePath = EMPTY_STRING)
    {
        HLOGV("virtual dummy function called");
        debugInfoLoaded_ = true;
        return false;
    };

    virtual const std::unordered_map<uint64_t, ElfLoadInfo> GetPtLoads()
    {
        std::unordered_map<uint64_t, ElfLoadInfo> loadInfoMap;
        return loadInfoMap;
    }

    // get the build if from symbols
    const std::string GetBuildId() const;

    virtual void EnableMiniDebugInfo() {}
    // get the symbols vector
    const std::vector<DfxSymbol> &GetSymbols();
    const std::vector<DfxSymbol *> &GetMatchedSymbols();

    // get vaddr(in symbol) from ip(real addr , after mmap reloc)
    virtual uint64_t GetVaddrInSymbols(uint64_t ip, uint64_t mapStart, uint64_t mapOffset) const;

    // get symbols from vaddr
    const DfxSymbol GetSymbolWithVaddr(uint64_t vaddrInFile);

    // get the section info , like .ARM.exidx
    virtual bool GetSectionInfo([[maybe_unused]] const std::string &name,
                                [[maybe_unused]] uint64_t &sectionVaddr,
                                [[maybe_unused]] uint64_t &sectionSize,
                                [[maybe_unused]] uint64_t &sectionFileOffset) const
    {
        HLOGV("virtual dummy function called");
        return false;
    }
#ifndef __arm__
    // get hdr info for unwind , need provide the fde table location and entry count
    virtual bool GetHDRSectionInfo([[maybe_unused]] uint64_t &ehFrameHdrElfOffset,
                                   [[maybe_unused]] uint64_t &fdeTableElfOffset,
                                   [[maybe_unused]] uint64_t &fdeTableSize)
    {
        HLOGV("virtual dummy function called");
        return false;
    }
#endif
    // load from symbols from the perf.data format
    static std::unique_ptr<SymbolsFile> LoadSymbolsFromSaved(const SymbolFileStruct &);
    // save the symbols to perf.data format
    void ExportSymbolToFileFormat(SymbolFileStruct &symbolFileStruct);

    bool SymbolsLoaded()
    {
        return symbolsLoaded_;
    }

    void AddSymbol(DfxSymbol symbol);

    // this means we are in recording
    // will try read some elf in runtime path
    static bool onRecording_;
    static bool needParseJsFunc_;
    std::vector<DfxSymbol> symbols_ {};
    std::vector<DfxSymbol *> matchedSymbols_ {};
    std::map<uint64_t, DfxSymbol> symbolsMap_;
    virtual DfxSymbol GetSymbolWithPcAndMap(uint64_t pc, std::shared_ptr<DfxMap> map)
    {
        return DfxSymbol();
    }

    // set map info
    void SetMapsInfo(std::shared_ptr<DfxMap> map)
    {
        map_ = map;
    }

protected:
    bool symbolsLoaded_ = false;
    bool symbolsLoadResult_ = false;
    bool debugInfoLoaded_ = false;
    bool debugInfoLoadResult_ = false;
    const std::string FindSymbolFile(const std::vector<std::string> &,
                                     std::string symboleFilePath = EMPTY_STRING) const;

    std::string SearchReadableFile(const std::vector<std::string> &searchPaths,
                                   const std::string &filePath) const;
    bool UpdateBuildIdIfMatch(std::string buildId);
    std::string buildId_ = "";
    std::vector<std::string> symbolsFileSearchPaths_;

    std::vector<FileSymbol> fileSymbols_ {};
    std::mutex mutex_;

    void AdjustSymbols();
    void SortMatchedSymbols();
    bool CheckPathReadable(const std::string &path) const;
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_SYMBOLS_H
