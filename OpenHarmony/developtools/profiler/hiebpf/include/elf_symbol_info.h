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

#ifndef HIEBPF_ELF_SYMBOL_INFO_H_
#define HIEBPF_ELF_SYMBOL_INFO_H_

#include <set>
#include <string>
#include <vector>

namespace OHOS {
namespace Developtools {
namespace Hiebpf {
class ElfSymbolInfo {
public:
    struct ElfSymbolTable {
        uint64_t textVaddr_;
        uint32_t textOffset_;
        uint32_t symEntSize_;
        std::vector<uint8_t> strTable_;
        std::vector<uint8_t> symTable_;
        std::string fileName_;
    };

    ElfSymbolInfo() {};
    ~ElfSymbolInfo() {};

    bool IsCached(const std::string &fileName)
    {
        return (fileNames_.find(fileName) != fileNames_.end());
    }

    void CacheFileName(const std::string &fileName)
    {
        fileNames_.insert(fileName);
    }

    bool GetSymbolTable(const std::string &fileName, ElfSymbolTable &symbolTable);
    uint32_t GetBinary(const ElfSymbolTable &symbolTable, std::vector<uint8_t> &buf);

private:
    std::set<std::string> fileNames_;
};
} // namespace Hiebpf
} // namespace Developtools
} // namespace OHOS
#endif // HIEBPF_ELF_SYMBOL_INFO_H_