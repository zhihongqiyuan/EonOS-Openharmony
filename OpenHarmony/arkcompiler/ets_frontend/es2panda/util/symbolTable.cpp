/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "symbolTable.h"

#include <util/helpers.h>

namespace panda::es2panda::util {
const std::string SymbolTable::FIRST_LEVEL_SEPERATOR = "|";
const std::string SymbolTable::SECOND_LEVEL_SEPERATOR = ";";
const size_t FUNCTION_ITEM_NUMBER = 3;
const size_t MODULE_ITEM_NUMBER = 1;

bool SymbolTable::Initialize(int targetApiVersion, std::string targetApiSubVersion)
{
    targetApiVersion_ = targetApiVersion;
    targetApiSubVersion_ = targetApiSubVersion;
    if (!symbolTable_.empty() && !ReadSymbolTable(symbolTable_)) {
        std::cerr << "Failed to open the symbol table file'" << std::endl;
        return false;
    }

    if (!dumpSymbolTable_.empty()) {
        std::fstream fs;
        fs.open(panda::os::file::File::GetExtendedFilePath(dumpSymbolTable_),
            std::ios_base::out | std::ios_base::trunc);
        if (!fs.is_open()) {
            std::cerr << "Failed to create or open the output symbol table file '"
                      << dumpSymbolTable_ << "' during symbol table initialization." << std::endl
                      << "This error could be due to invalid file path, lack of write permissions, "
                      << "or the file being in use by another process." << std::endl;
            return false;
        }
        fs.close();
    }

    return true;
}

void SymbolTable::ReadRecordHashFunctionNames(const std::string &recordName, const std::string &funcInternalName,
                                              const std::string &specialFuncIndex)
{
    auto recordHashFunctionNames = originRecordHashFunctionNames_.find(recordName);
    if (specialFuncIndex == "0") {
        // 0: not anonymous, special or duplicate function
        if (recordHashFunctionNames == originRecordHashFunctionNames_.end())  {
            std::unordered_map<std::string, std::string> functionIndexNameMap {};
            originRecordHashFunctionNames_.insert({recordName, functionIndexNameMap});
        }
    } else {
        if (recordHashFunctionNames != originRecordHashFunctionNames_.end()) {
            recordHashFunctionNames->second.insert({specialFuncIndex, funcInternalName});
        } else {
            std::unordered_map<std::string, std::string> functionIndexNameMap {{specialFuncIndex, funcInternalName}};
            originRecordHashFunctionNames_.insert({recordName, functionIndexNameMap});
        }
    }
}

bool SymbolTable::ReadSymbolTable(const std::string &symbolTable)
{
    std::ifstream ifs;
    std::string line;
    ifs.open(panda::os::file::File::GetExtendedFilePath(symbolTable));
    if (!ifs.is_open()) {
        std::cerr << "Failed to open the symbol table file '"
                  << symbolTable << "' during symbol table reading." << std::endl
                  << "Please check if the file exists, the path is correct, "
                  << "and your program has the necessary permissions to access the file." << std::endl;
        return false;
    }

    while (std::getline(ifs, line)) {
        auto itemList = GetStringItems(line, FIRST_LEVEL_SEPERATOR);

        if (itemList.size() == FUNCTION_ITEM_NUMBER) {
            // read function info
            struct OriginFunctionInfo info(&allocator_);
            auto funcItems = GetStringItems(itemList[0], SECOND_LEVEL_SEPERATOR);
            auto classItems = GetStringItems(itemList[1], SECOND_LEVEL_SEPERATOR);
            auto lexItems = GetStringItems(itemList[2], SECOND_LEVEL_SEPERATOR);

            info.recordName = funcItems[0].substr(0, funcItems[0].find_last_of("."));
            info.funcInternalName = funcItems[1];
            // 2: use the third element of the function as the hash of the function
            info.funcHash = funcItems[2];

            // 2 is to process each class name and its corresponding hash value
            for (size_t i = 0; i < classItems.size(); i = i + 2) {
                info.classHash.insert(std::pair<std::string, std::string>(classItems[i], classItems[i + 1]));
            }
            // 3 is to process a complete lexical environment entry
            for (size_t i = 0; i < lexItems.size(); i = i + 3) {
                auto name = std::string(lexItems[i]);
                auto slot = std::atoi(std::string(lexItems[i + 1]).c_str());
                auto type = std::atoi(std::string(lexItems[i + 2]).c_str());
                info.lexenv.insert({slot, std::pair<std::string, int>(name, type)});
            }

            originFunctionInfo_.insert(std::pair<std::string, OriginFunctionInfo>(info.funcInternalName, info));
            if (util::Helpers::IsDefaultApiVersion(targetApiVersion_, targetApiSubVersion_)) {
                // index of function in its record's special function array
                std::string specialFuncIndex{funcItems[3]};
                ReadRecordHashFunctionNames(info.recordName, info.funcInternalName, specialFuncIndex);
            }
        } else if (itemList.size() == MODULE_ITEM_NUMBER) {
            // read module info
            auto moduleItems = GetStringItems(itemList[0], SECOND_LEVEL_SEPERATOR);
            originModuleInfo_.insert(std::pair<std::string, std::string>(moduleItems[0], moduleItems[1]));
        } else {
            std::cerr << "Failed to read the symbol table line: '" << line
                      << "' from the symbol table file '" << symbolTable << "' due to unrecognized format." << std::endl
                      << "Please verify the format of the symbol table." << std::endl;
        }
    }
    return true;
}

void SymbolTable::FillSymbolTable(const std::stringstream &content)
{
    std::lock_guard<std::mutex> lock(m_);
    symbolTableContent_ << content.rdbuf();
}

void SymbolTable::WriteSymbolTable()
{
    std::fstream fs;
    fs.open(panda::os::file::File::GetExtendedFilePath(dumpSymbolTable_),
        std::ios_base::app | std::ios_base::in);
    if (fs.is_open()) {
        fs << symbolTableContent_.str();
        fs.close();
    }
}

std::vector<std::string_view> SymbolTable::GetStringItems(std::string_view input, const std::string &separator)
{
    std::vector<std::string_view> items;
    size_t curPos = 0;
    size_t lastPos = 0;

    while ((curPos = input.find(separator, lastPos)) != std::string_view::npos) {
        auto token = input.substr(lastPos, curPos - lastPos);
        if (!token.empty()) {
            items.push_back(token);
        }
        lastPos = curPos + separator.size();
    }

    auto tail = input.substr(lastPos);
    if (!tail.empty()) {
        items.push_back(tail);
    }

    return items;
}
}  // namespace panda::es2panda::util