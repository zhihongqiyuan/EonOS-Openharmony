/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "resolveDepsRelation.h"

#include <util/commonUtil.h>

namespace panda::es2panda::aot {

bool DepsRelationResolver::CollectCommonjsAndJsonRecords(const std::vector<panda::pandasm::Field> &fieldList,
                                                         const std::string &progKey, const std::string &recordName)
{
    for (const auto &field: fieldList) {
        if (field.name == util::JSON_FilE_CONTENT) {
            resolvedDepsRelation_[progKey].insert(recordName);
            return true;
        }
        if (field.name.find(util::IS_COMMONJS) == std::string::npos) {
            continue;
        }
        ASSERT(field.metadata->GetValue().has_value());
        ASSERT(field.type.GetId() == panda_file::Type::TypeId::U8);
        if (field.metadata->GetValue().value().GetValue<uint8_t>() > 0) {
            resolvedDepsRelation_[progKey].insert(recordName);
            return true;
        }
    }
    return false;
}

void DepsRelationResolver::FillRecord2ProgramMap(std::unordered_map<std::string, std::string> &record2ProgramMap)
{
    for (const auto &progInfo : progsInfo_) {
        for (const auto &record : progInfo.second->program.record_table) {
            if (record.second.field_list.empty()) {
                continue;
            }
            if (progInfo.first.find(util::NPM_ENTRIES) != std::string::npos) {
                resolvedDepsRelation_[progInfo.first].insert(record.second.name);
                continue;
            }

            // All commonjs files will be include as dependencies for compilation without resolve. Since commonjs files
            // will only have commonjs as dependencies, there'll be no dependencies missing.
            if (CollectCommonjsAndJsonRecords(record.second.field_list, progInfo.first, record.second.name)) {
                break;
            }
            record2ProgramMap[record.second.name] = progInfo.first;
        }
    }
}

void DepsRelationResolver::CollectDepsIfNeeded(const std::string &ohmurl)
{
    if (ohmurl.find(util::NORMALIZED_OHMURL_NOT_SO) != std::string::npos &&
        !util::IsExternalPkgNames(ohmurl, compileContextInfo_.externalPkgNames)) {
        std::string collectRecord = util::GetRecordNameFromNormalizedOhmurl(ohmurl);
        if (!collectRecord.empty() && this->resolvedRecords_.count(collectRecord) == 0) {
            this->depsToBeResolved_.push(collectRecord);
            this->resolvedRecords_.insert(collectRecord);
        }
    }
}

void DepsRelationResolver::DumpDepsRelations()
{
    auto &ss = std::cout;
    ss << "All Dependency Files:" << std::endl;
    for (auto dep : resolvedDepsRelation_) {
        auto fileName = dep.first;
        size_t abcFileNameSeparatorPos = dep.first.rfind(util::CHAR_VERTICAL_LINE);
        size_t pos = dep.first.rfind(util::SLASH_TAG, abcFileNameSeparatorPos);
        if (pos != std::string::npos) {
            fileName = dep.first.substr(pos + 1, abcFileNameSeparatorPos - pos - 1);
        }
        ss << "program_file: " << fileName << std::endl;
        for (auto r : dep.second) {
            ss << "record_name: " << r << std::endl;
        }
    }
    ss << std::endl;
}

bool DepsRelationResolver::Resolve()
{
    std::unordered_map<std::string, std::string> record2ProgramMap {};
    FillRecord2ProgramMap(record2ProgramMap);

    for (auto &entryRecord : compileContextInfo_.compileEntries) {
        depsToBeResolved_.push(entryRecord);
        resolvedRecords_.insert(entryRecord);

        while (!depsToBeResolved_.empty()) {
            auto record = depsToBeResolved_.front();
            depsToBeResolved_.pop();
            const auto progkeyItr = record2ProgramMap.find(record);
            if (progkeyItr == record2ProgramMap.end()) {
                // Skip external record, may happen at PatchFix or HSP scenario
                continue;
            }
            const auto progItr = progsInfo_.find(progkeyItr->second);
            if (progItr == progsInfo_.end()) {
                std::cerr << "Failed to find program for file: " << progkeyItr->second << std::endl;
                return false;
            }
            resolvedDepsRelation_[progkeyItr->second].insert(record);

            CollectStaticImportDepsRelation(progItr->second->program, record);
            CollectDynamicImportDepsRelation(progItr->second->program, record);
        }
    }
    if (dumpDepsInfo_) {
        DumpDepsRelations();
    }
    return true;
}

void DepsRelationResolver::CollectStaticImportDepsRelation(const panda::pandasm::Program &program,
                                                           const std::string &recordName)
{
    auto &recordTable = program.record_table;
    std::string literalArrayKey;
    for (auto &pair : recordTable) {
        if (pair.first.find(recordName) == std::string::npos) {
            continue;
        }
        util::VisitStaticImports<true>(program, pair.second, [this](const std::string &ohmurl) {
            this->CollectDepsIfNeeded(ohmurl);
        });
    }
}

void DepsRelationResolver::CollectDynamicImportDepsRelation(const panda::pandasm::Program &program,
                                                            const std::string &recordName)
{
    for (const auto &func: program.function_table) {
        if (func.second.name.find(recordName) == std::string::npos) {
            continue;
        }
        util::VisitDyanmicImports<true>(func.second, [this](const std::string &ohmurl) {
            this->CollectDepsIfNeeded(ohmurl);
        });
    }
}

} // namespace panda::es2panda::aot
