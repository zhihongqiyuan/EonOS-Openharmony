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

#ifndef ES2PANDA_UTIL_COMMON_H
#define ES2PANDA_UTIL_COMMON_H

#include <functional>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <string_view>

#include "assembler/assembly-function.h"
#include "assembler/assembly-literals.h"
#include "assembler/assembly-program.h"
#include "assembler/assembly-record.h"

namespace panda::es2panda {
struct CompileContextInfo;
struct PkgInfo;
};

namespace panda::es2panda::util {
const std::string NPM_ENTRIES = "npmEntries.txt";
const std::string IS_COMMONJS = "isCommonjs";
const std::string JSON_FilE_CONTENT = "jsonFileContent";
// The format of ohmurl for non-SO files are start with '@normalized:N'.
const std::string NORMALIZED_OHMURL_NOT_SO = "@normalized:N";
const std::string NORMALIZED = "@normalized:";
const std::string MODULE_RECORD_IDX = "moduleRecordIdx";
const std::string GLOBAL_TYPE_NAME = "_GLOBAL";

constexpr char NORMALIZED_OHMURL_SEPARATOR = '&';
constexpr char NORMALIZED_OHMURL_PREFIX = '@';
constexpr char SLASH_TAG = '/';
constexpr char CHAR_VERTICAL_LINE = '|';
constexpr char COLON_SEPARATOR = ':';
constexpr char DOT_SEPARATOR = '.';
constexpr size_t ORIGINAL_PKG_NAME_POS = 0U;
constexpr size_t TARGET_PKG_NAME_POS = 1U;

constexpr size_t MODULE_NAME_POS = 1U;
constexpr size_t BUNDLE_NAME_POS = 2U;
constexpr size_t NORMALIZED_IMPORT_POS = 3U;
constexpr size_t VERSION_POS = 4U;

std::vector<std::string> Split(const std::string &str, const char delimiter);
std::string GetStringByVectorElementsWithDelimiter(const std::vector<std::string> &vec, const char delimiter);
bool IsExternalPkgNames(const std::string &ohmurl, const std::set<std::string> &externalPkgNames);
std::string GetRecordNameFromNormalizedOhmurl(const std::string &ohmurl);
std::string GetPkgNameFromNormalizedOhmurl(const std::string &ohmurl);
std::string GetPkgNameFromNormalizedImport(const std::string &normalizedImport);
std::string UpdatePackageVersionIfNeeded(const std::string &ohmurl,
                                         const std::map<std::string, PkgInfo> &pkgContextInfo);
std::string UpdatePackageNameIfNeeded(const std::string &ohmurl, const std::string &modifiedPkgName);
std::string UpdateBundleNameIfNeeded(std::string &ohmurl, const std::string &bundleName,
                                     const std::set<std::string> &externalPkgNames);
bool RecordNotGeneratedFromBytecode(std::string recordName);

template<bool isConst, typename T>
using ConstReferenceIf = typename std::conditional<isConst, const T &, T &>::type;

template<bool isConst>
using ImportTraverser = std::function<void(ConstReferenceIf<isConst, std::string>)>;

template <bool isConst>
void VisitStaticImports(ConstReferenceIf<isConst, pandasm::Program> program,
                        ConstReferenceIf<isConst, pandasm::Record> record,
                        const ImportTraverser<isConst> &cb)
{
    for (const pandasm::Field &field : record.field_list) {
        if (field.name == util::MODULE_RECORD_IDX) {
            auto moduleLiteralKey = field.metadata->GetValue().value().GetValue<std::string>();
            auto iter = program.literalarray_table.find(moduleLiteralKey);
            ASSERT(iter != program.literalarray_table.end());
            auto &array = iter->second;
            uint32_t importSize = std::get<uint32_t>(iter->second.literals_[0].value_);
            for (size_t idx = 1; idx < importSize + 1; ++idx) {
                cb(std::get<std::string>(array.literals_[idx].value_));
            }
        }
    }
}

// Only visit dynamic imports for import("xxxx") expression
template <bool isConst>
void VisitDyanmicImports(ConstReferenceIf<isConst, pandasm::Function> function, const ImportTraverser<isConst> &cb)
{
    for (auto iter = function.ins.begin(); iter != function.ins.end(); iter++) {
        // Only visit dynamic imports for import("xxxx") expression, whose bytecode always is:
        // lda.str -> dynamicimport
        // The dynamicimport bytecode should not have label, otherwise the dyanmicimport might be a jump
        // target and its parameter is a variable instead of a constant string expression (Check
        // AbcCodeProcessor::AddJumpLabels for more details).
        if ((*iter)->opcode != pandasm::Opcode::DYNAMICIMPORT || (*iter)->IsLabel()) {
            continue;
        }
        auto prevIns = iter - 1;
        if ((*prevIns)->opcode != pandasm::Opcode::LDA_STR) {
            continue;
        }
        ASSERT((*prevIns)->Ids().size() == 1);
        cb((*prevIns)->GetId(0));  // 0: index of the string in lda.str bytecode
    }
}
}  // namespace panda::es2panda::util

#endif