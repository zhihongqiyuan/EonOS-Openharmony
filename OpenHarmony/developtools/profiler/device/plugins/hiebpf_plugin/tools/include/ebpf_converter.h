/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EBPF_COVERTER_H
#define EBPF_COVERTER_H

#include <sstream>
#include <map>
#include <vector>

#include "ebpf_std_type.h"

namespace OHOS {
namespace EBPF_TOOLS {
class EbpfConverter {
public:
    EbpfConverter(const std::string& inputPath, const std::string& outPath);
    virtual ~EbpfConverter();
    void StartParsing();

private:
    bool Read(void* buffer, size_t size);
    void EventFsParsing();
    void EventMapsParsing();
    void SymbolInfoParsing();
    void EventMemParsing();
    void EventStrParsing();
    void EventBIOParsing();
    std::pair<std::string, std::vector<std::string>> GetSymbolInfo(uint64_t pid, uint64_t ip);
public:
    static std::pair<bool, std::string> CheckNotExistsFilePath(const std::string& filePath);

private:
    std::stringstream outData_;
    std::string inputPath_;
    std::string outputPath_;
    int32_t fileSize_ { 0 };
    int32_t fd_ { -1 };
    std::vector<EventStr> str_;
    std::map<std::string, Record> record_;
    std::map<std::string, SymbolInfo> symbolInfo_;
    std::map<uint32_t, std::vector<EventMaps>> maps_;
    std::map<uint64_t, uint64_t> vAddr_;
};
} // EBPF_TOOLS
} // OHOS

#endif // EBPF_COVERTER_H