/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
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

#ifndef ELF_FILE_H_
#define ELF_FILE_H_
#include <cassert>
#include <cstring>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <elf.h>

#include <fcntl.h>
#include <stdint.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "hhlog.h"
#include "utilities.h"

namespace OHOS {
namespace Developtools {
namespace Hiebpf {
using namespace std::string_literals;

constexpr std::size_t ehdr32Size {52};
constexpr std::size_t ehdr64Size {64};
constexpr std::size_t shdr32Size {40};
constexpr std::size_t shdr64Size {64};
constexpr std::size_t phdr32Size {32};
constexpr std::size_t phdr64Size {56};
constexpr std::size_t symEnt32Size {16};
constexpr std::size_t symEnt64Size {24};

class ElfHeader {
public:
    static std::unique_ptr<ElfHeader> MakeUnique(unsigned char * const ehdrBuf,
                                                 const std::size_t bufSize);
    bool Init(unsigned char * const ehdrBuf, const std::size_t bufSize);

    unsigned char ehdrIdent_[EI_NIDENT];
    uint16_t type_;
    uint16_t machine_;
    uint16_t ehdrSize_;
    uint16_t phdrEntSize_;
    uint16_t phdrNumEnts_;
    uint16_t shdrEntSize_;
    uint16_t shdrNumEnts_;
    uint16_t shdrStrTabIdx_;
    uint32_t elfVersion_;
    uint32_t ehdrFlags_;
    uint64_t prgEntryVaddr_;
    uint64_t phdrOffset_;
    uint64_t shdrOffset_;

private:
    explicit ElfHeader() = default;
    bool ParseElf32Header(unsigned char * const ehdrBuf, const std::size_t bufSize);
    bool ParseElf64Header(unsigned char * const ehdrBuf, const std::size_t bufSize);
};

class ProgramHeader {
public:
    static std::unique_ptr<ProgramHeader> MakeUnique(char * const phdrBuf, const size_t bufSize);
    inline bool Init(char * const phdrBuf, const size_t bufSize)
    {
        if (bufSize == phdr32Size and ParsePrgHeader32(phdrBuf)) {
            return true;
        }
        if (bufSize == phdr64Size and ParsePrgHeader64(phdrBuf)) {
            return true;
        }
        HHLOGE(true, "parse program header failed, program header buffer dumped");
        return false;
    }

    uint32_t type_;
    uint32_t flags_;
    uint64_t offset_;
    uint64_t vaddr_;
    uint64_t paddr_;
    uint64_t fileSize_;
    uint64_t memSize_;
    uint64_t secAlign_;

private:
    explicit ProgramHeader() = default;
    bool ParsePrgHeader32(char * const phdrBuf);
    bool ParsePrgHeader64(char * const phdrBuf);
};

class SectionHeader {
public:
    static std::unique_ptr<SectionHeader> MakeUnique(char * const shdrBuf, const size_t bufSize,
                                                     const size_t index);

    inline bool Init(char * const shdrBuf, const size_t bufSize, const size_t index)
    {
        secIndex_ = index;
        if (bufSize == shdr32Size and ParseSecHeader32(shdrBuf)) {
            return true;
        }
        if (bufSize == shdr64Size and ParseSecHeader64(shdrBuf)) {
            return true;
        }
        HHLOGE(true, "parse section header failed, section header buffer dumped");
        return false;
    }

    uint32_t nameIndex_;
    uint32_t link_;
    uint32_t info_;
    uint64_t secFlags_;
    uint64_t secVaddr_;
    uint64_t fileOffset_;
    uint64_t secSize_;
    uint64_t secAddrAlign_;
    uint64_t secEntrySize_;
    uint64_t secType_;
    uint32_t secIndex_;
    std::string secTypeName_;

private:
    explicit SectionHeader() = default;
    bool ParseSecHeader32(char * const shdrBuf);
    bool ParseSecHeader64(char * const shdrBuf);
};

class ElfFile {
public:
    ~ElfFile();
    static std::unique_ptr<ElfFile> MakeUnique(const std::string &filename);
    bool ParseFile();
    std::string GetSectionName(const uint32_t startIndex);

    inline bool IsOpened() const
    {
        return fd_ != -1;
    }

    inline const char *GetStrPtr(uint32_t sh_link, uint32_t st_name)
    {
        for (const auto &shdrsItem : shdrs_) {
            if (shdrsItem.second->secIndex_ == sh_link) {
                if (mmap_ != MMAP_FAILED) {
                    char *elfFileBegin = (char *)mmap_;
                    return elfFileBegin + shdrsItem.second->fileOffset_ + st_name;
                }
            }
        }
        HHLOGE(true, "string not found sh_link %u st_name %d", sh_link, st_name);
        return nullptr;
    }

    inline const unsigned char *GetSectionData(uint32_t shIndex)
    {
        for (const auto &shdrsItem : shdrs_) {
            if (shdrsItem.second->secIndex_ == shIndex) {
                if (mmap_ != MMAP_FAILED) {
                    const unsigned char *elfFileBegin = (const unsigned char *)mmap_;
                    return elfFileBegin + shdrsItem.second->fileOffset_;
                }
            }
        }
        HHLOGE(true, "string not found shIndex %u ", shIndex);
        return nullptr;
    }

    using SecHeaderTableType = std::unordered_map<std::string, std::unique_ptr<SectionHeader>>;
    using PrgHeaderTableType = std::vector<std::unique_ptr<ProgramHeader>>;
    int fd_ {-1};
    std::unique_ptr<ElfHeader> ehdr_ {nullptr};
    SecHeaderTableType shdrs_ {};
    PrgHeaderTableType phdrs_ {};
    std::string secNamesStr_ {};

private:
    ssize_t ReadFile(void *buf, size_t count)
    {
        return read(fd_, buf, count);
    };
    explicit ElfFile(const std::string &filename);

    bool ParseElfHeader();
    bool ParsePrgHeaders();
    bool ParseSecNamesStr();
    bool ParseSecHeaders();

    void *mmap_ = MMAP_FAILED;
    uint64_t mmapSize_ = 0;
};
} // namespace Hiebpf
} // namespace Developtools
} // namespace OHOS
#endif // ELF_FILE_H_
