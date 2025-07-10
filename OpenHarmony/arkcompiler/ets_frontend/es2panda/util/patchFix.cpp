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

#include "patchFix.h"
#include <compiler/core/pandagen.h>
#include <ir/expressions/literal.h>

namespace panda::es2panda::util {

const std::string EXTERNAL_ATTRIBUTE = "external";

void PatchFix::ProcessFunction(const compiler::PandaGen *pg, panda::pandasm::Function *func,
    LiteralBuffers &literalBuffers)
{
    if (generateSymbolFile_) {
        DumpFunctionInfo(pg, func, literalBuffers);
        return;
    }

    if (generatePatch_ || IsHotReload()) {
        HandleFunction(pg, func, literalBuffers);
        return;
    }
}

void PatchFix::ProcessModule(const std::string &recordName,
    std::vector<panda::pandasm::LiteralArray::Literal> &moduleBuffer)
{
    if (generateSymbolFile_) {
        DumpModuleInfo(recordName, moduleBuffer);
        return;
    }

    if (generatePatch_ || IsHotReload()) {
        ValidateModuleInfo(recordName, moduleBuffer);
        return;
    }
}

void PatchFix::ProcessJsonContentRecord(const std::string &recordName, const std::string &jsonFileContent)
{
    if (generateSymbolFile_) {
        DumpJsonContentRecInfo(recordName, jsonFileContent);
        return;
    }

    if (generatePatch_ || IsHotReload()) {
        ValidateJsonContentRecInfo(recordName, jsonFileContent);
        return;
    }
}

void PatchFix::DumpModuleInfo(const std::string &recordName,
    std::vector<panda::pandasm::LiteralArray::Literal> &moduleBuffer)
{
    std::stringstream ss;
    ss << recordName << SymbolTable::SECOND_LEVEL_SEPERATOR;
    ss << Helpers::GetHashString(ConvertLiteralToString(moduleBuffer)) << std::endl;
    symbolTable_->FillSymbolTable(ss);
}

void PatchFix::ValidateModuleInfo(const std::string &recordName,
    std::vector<panda::pandasm::LiteralArray::Literal> &moduleBuffer)
{
    auto it = originModuleInfo_->find(recordName);
    if (!IsHotReload() && it == originModuleInfo_->end()) {
        errMsg_ << "[Patch] Found new import/export expression in " + recordName + ", not supported!" << std::endl;
        patchError_ = true;
        return;
    }

    if (!IsHotReload() && Helpers::GetHashString(ConvertLiteralToString(moduleBuffer)) != it->second) {
        errMsg_ << "[Patch] Found import/export expression changed in " + recordName + ", not supported!" <<
            std::endl;
        patchError_ = true;
        return;
    }
}

void PatchFix::DumpJsonContentRecInfo(const std::string &recordName, const std::string &jsonFileContent)
{
    std::stringstream ss;
    ss << recordName << SymbolTable::SECOND_LEVEL_SEPERATOR;
    ss << Helpers::GetHashString(jsonFileContent) << std::endl;
    symbolTable_->FillSymbolTable(ss);
}

void PatchFix::ValidateJsonContentRecInfo(const std::string &recordName, const std::string &jsonFileContent)
{
    auto it = originModuleInfo_->find(recordName);
    if (!IsHotReload() && it == originModuleInfo_->end()) {
        errMsg_ << "[Patch] Found new import/require json file expression in " + recordName +
            ", not supported!" << std::endl;
        patchError_ = true;
        return;
    }

    if (!IsHotReload() && Helpers::GetHashString(jsonFileContent) != it->second) {
        errMsg_ << "[Patch] Found imported/required json file content changed in " + recordName +
            ", not supported!" << std::endl;
        patchError_ = true;
        return;
    }
}

bool PatchFix::IsAnonymousOrSpecialOrDuplicateFunction(const std::string &funcName)
{
    if (util::Helpers::IsDefaultApiVersion(targetApiVersion_, targetApiSubVersion_)) {
        return funcName.find(binder::Binder::ANONYMOUS_SPECIAL_DUPLICATE_FUNCTION_SPECIFIER) != std::string::npos;
    }
    // Function name is like: #scopes^1#functionname^1
    // Special function name is which includes "\\" or ".", the name should be transformed into "".
    // Anonymous function name is "", it's the same with special function name here.
    // Duplicate function name includes "^" after the last "#".
    auto pos = funcName.find_last_of(Helpers::FUNC_NAME_SEPARATOR);
    if (pos == std::string::npos) {
        return false;
    }

    if (pos == funcName.size() - 1) {
        return true;
    }

    auto posOfDuplicateSep = funcName.find_last_of(Helpers::DUPLICATED_SEPERATOR);
    if (posOfDuplicateSep != std::string::npos && posOfDuplicateSep > pos) {
        return true;
    }

    return false;
}

int64_t PatchFix::GetLiteralIdxFromStringId(const std::string &stringId)
{
    auto recordPrefix = recordName_ + "_";
    auto idxStr = stringId.substr(recordPrefix.size());
    return std::atoi(idxStr.c_str());
}

void PatchFix::CollectFunctionsWithDefinedClasses(std::string funcName, std::string className)
{
    auto funcInfo = funcDefinedClasses_.find(funcName);
    if (funcInfo != funcDefinedClasses_.end()) {
        funcInfo->second.push_back(className);
        return;
    }
    std::vector<std::string> funcDefinedClasses = {className};
    funcDefinedClasses_.insert({funcName, funcDefinedClasses});
}

std::vector<std::pair<std::string, std::string>> PatchFix::GenerateFunctionAndClassHash(panda::pandasm::Function *func,
    LiteralBuffers &literalBuffers)
{
    std::stringstream ss;
    std::vector<std::pair<std::string, std::string>> hashList;

    ss << ".function any " << func->name << '(';

    for (uint32_t i = 0; i < func->GetParamsNum(); i++) {
        ss << "any a" << std::to_string(i);
        if (i != func->GetParamsNum() - 1) {
            ss << ", ";
        }
    }
    ss << ") {" << std::endl;

    for (const auto &ins : func->ins) {
        ss << (ins->IsLabel() ? "" : "\t") << ins->ToString("", true, func->GetTotalRegs()) << " ";
        if (ins->opcode == panda::pandasm::Opcode::CREATEARRAYWITHBUFFER ||
            ins->opcode == panda::pandasm::Opcode::CREATEOBJECTWITHBUFFER) {
            int64_t bufferIdx = GetLiteralIdxFromStringId(ins->GetId(0));
            ss << ExpandLiteral(bufferIdx, literalBuffers) << " ";
        } else if (ins->opcode == panda::pandasm::Opcode::DEFINECLASSWITHBUFFER) {
            CollectFunctionsWithDefinedClasses(func->name, ins->GetId(0));
            int64_t bufferIdx = GetLiteralIdxFromStringId(ins->GetId(1));
            std::string literalStr = ExpandLiteral(bufferIdx, literalBuffers);
            auto classHash = Helpers::GetHashString(literalStr);
            hashList.push_back(std::pair<std::string, std::string>(ins->GetId(0), classHash));
            CollectClassMemberFunctions(ins->GetId(0), bufferIdx, literalBuffers);
        }
        ss << " ";
    }

    ss << "}" << std::endl;

    for (const auto &ct : func->catch_blocks) {
        ss << ".catchall " << ct.try_begin_label << ", " << ct.try_end_label << ", " << ct.catch_begin_label
            << std::endl;
    }

    auto funcHash = Helpers::GetHashString(ss.str());
    hashList.push_back(std::pair<std::string, std::string>(func->name, funcHash));
    return hashList;
}

std::string PatchFix::ConvertLiteralToString(std::vector<panda::pandasm::LiteralArray::Literal> &literalBuffer)
{
    std::stringstream ss;
    int count = 0;
    for (auto &literal : literalBuffer) {
        ss << "{" << "index: " << count++ << " ";
        ss << "tag: " << static_cast<std::underlying_type<panda::es2panda::ir::LiteralTag>::type>(literal.tag_);
        ss << " ";
        std::string val;
        std::visit([&val](auto&& element) {
            val += "val: ";
            val += element;
            val += " ";
        }, literal.value_);
        ss << val;
        ss << "},";
    }

    return ss.str();
}

std::string PatchFix::ExpandLiteral(int64_t bufferIdx, PatchFix::LiteralBuffers &literalBuffers)
{
    for (auto &litPair : literalBuffers) {
        if (litPair.first == bufferIdx) {
            return ConvertLiteralToString(litPair.second);
        }
    }

    return "";
}

std::vector<std::string> PatchFix::GetLiteralMethods(int64_t bufferIdx, PatchFix::LiteralBuffers &literalBuffers)
{
    std::vector<std::string> methods;
    for (auto &litPair : literalBuffers) {
        if (litPair.first != bufferIdx) {
            continue;
        }
        for (auto &literal : litPair.second) {
            switch (literal.tag_) {
                case panda::panda_file::LiteralTag::METHOD:
                case panda::panda_file::LiteralTag::GENERATORMETHOD:
                case panda::panda_file::LiteralTag::ASYNCGENERATORMETHOD: {
                    methods.push_back(std::get<std::string>(literal.value_));
                    break;
                }
                default:
                    break;
            }
        }
    }

    return methods;
}

void PatchFix::CollectClassMemberFunctions(const std::string &className, int64_t bufferIdx,
    PatchFix::LiteralBuffers &literalBuffers)
{
    std::vector<std::string> classMemberFunctions = GetLiteralMethods(bufferIdx, literalBuffers);
    classMemberFunctions.push_back(className);
    classMemberFunctions_.insert({className, classMemberFunctions});
}

bool PatchFix::IsScopeValidToPatchLexical(binder::VariableScope *scope) const
{
    if (IsDumpSymbolTable()) {
        return false;
    }

    CHECK_NOT_NULL(scope);
    if (!scope->IsFunctionVariableScope()) {
        return false;
    }

    auto funcName = scope->AsFunctionVariableScope()->InternalName();
    if (std::string(funcName) != funcMain0_) {
        return false;
    }
    return true;
}

void PatchFix::AllocSlotfromPatchEnv(const std::string &variableName)
{
    if (!topScopeLexEnvs_.count(variableName)) {
        topScopeLexEnvs_[variableName] = topScopeIdx_++;
    }
}

uint32_t PatchFix::GetSlotIdFromSymbolTable(const std::string &variableName)
{
    auto functionIter = originFunctionInfo_->find(funcMain0_);
    if (functionIter != originFunctionInfo_->end()) {
        for (const auto &lexenv : functionIter->second.lexenv) {
            if (lexenv.second.first == variableName) {
                return lexenv.first;
            }
        }
    }
    return UINT32_MAX;
}

uint32_t PatchFix::GetEnvSizeOfFuncMain0()
{
    auto functionIter = originFunctionInfo_->find(funcMain0_);
    ASSERT(functionIter != originFunctionInfo_->end());
    return functionIter->second.lexenv.size();
}

uint32_t PatchFix::GetPatchLexicalIdx(const std::string &variableName)
{
    ASSERT(topScopeLexEnvs_.count(variableName));
    return topScopeLexEnvs_[variableName];
}

bool IsFunctionOrClassDefineIns(panda::pandasm::Ins *ins)
{
    if (ins->opcode == panda::pandasm::Opcode::DEFINEMETHOD ||
        ins->opcode == panda::pandasm::Opcode::DEFINEFUNC ||
        ins->opcode == panda::pandasm::Opcode::DEFINECLASSWITHBUFFER) {
        return true;
    }
    return false;
}

bool IsStPatchVarIns(panda::pandasm::Ins *ins)
{
    return ins->opcode == panda::pandasm::Opcode::WIDE_STPATCHVAR;
}

void PatchFix::CollectFuncDefineIns(panda::pandasm::Function *func)
{
    for (size_t i = 0; i < func->ins.size(); ++i) {
        if (IsFunctionOrClassDefineIns(func->ins[i].get())) {
            funcDefineIns_.push_back(func->ins[i].get());  // push define ins
            funcDefineIns_.push_back(func->ins[i + 1].get());  // push store ins
        }
    }
}

void PatchFix::HandleModifiedClasses(panda::pandasm::Program *prog)
{
    for (auto &cls: classMemberFunctions_) {
        for (auto &func: cls.second) {
            if (!prog->function_table.at(func).metadata->IsForeign()) {
                modifiedClassNames_.insert(cls.first);
                break;
            }
        }
    }

    for (auto &cls: modifiedClassNames_) {
        auto &memberFunctions = classMemberFunctions_[cls];
        for (auto &func: memberFunctions) {
            if (prog->function_table.at(func).metadata->IsForeign()) {
                prog->function_table.at(func).metadata->RemoveAttribute(EXTERNAL_ATTRIBUTE);
            }
        }
    }
}

void PatchFix::HandleModifiedDefinedClassFunc(panda::pandasm::Program *prog)
{
    for (auto &funcInfo: funcDefinedClasses_) {
        for (auto &definedClass: funcInfo.second) {
            if (modifiedClassNames_.count(definedClass) &&
                prog->function_table.at(funcInfo.first).metadata->IsForeign()) {
                prog->function_table.at(funcInfo.first).metadata->RemoveAttribute(EXTERNAL_ATTRIBUTE);
            }
        }
    }
}

void PatchFix::AddHeadAndTailInsForPatchFuncMain0(std::vector<panda::pandasm::InsPtr> &ins)
{
    auto returnUndefined = new pandasm::Returnundefined();
    if (ins.size() == 0) {
        ins.emplace_back(returnUndefined);
        return;
    }

    auto newLexenv = new pandasm::Newlexenv(long(ins.size() / 2));  // each new function has 2 ins: define and stor
    ins.emplace(ins.begin(), newLexenv);
    ins.emplace_back(returnUndefined);
}

void PatchFix::AddTailInsForPatchFuncMain1(std::vector<panda::pandasm::InsPtr> &ins)
{
    auto returnUndefined = new pandasm::Returnundefined();
    ins.emplace_back(returnUndefined);
}

void PatchFix::CreateFunctionPatchMain0AndMain1(panda::pandasm::Function &patchFuncMain0,
    panda::pandasm::Function &patchFuncMain1)
{
    const size_t defaultParamCount = 3;
    patchFuncMain0.params.reserve(defaultParamCount);
    patchFuncMain1.params.reserve(defaultParamCount);
    for (uint32_t i = 0; i < defaultParamCount; ++i) {
        patchFuncMain0.params.emplace_back(panda::pandasm::Type("any", 0), patchFuncMain0.language);
        patchFuncMain1.params.emplace_back(panda::pandasm::Type("any", 0), patchFuncMain1.language);
    }

    std::vector<panda::pandasm::InsPtr> patchMain0DefineIns;
    std::vector<panda::pandasm::InsPtr> patchMain1DefineIns;

    for (size_t i = 0; i < funcDefineIns_.size(); ++i) {
        if (IsFunctionOrClassDefineIns(funcDefineIns_[i])) {
            auto name = funcDefineIns_[i]->GetId(0);
            if (newFuncNames_.count(name) && IsStPatchVarIns(funcDefineIns_[i + 1])) {
                patchMain0DefineIns.emplace_back(funcDefineIns_[i]->DeepCopy());
                patchMain0DefineIns.emplace_back(funcDefineIns_[i + 1]->DeepCopy());
                continue;
            }
            if (patchFuncNames_.count(name) || modifiedClassNames_.count(name)) {
                patchMain1DefineIns.emplace_back(funcDefineIns_[i]->DeepCopy());
                continue;
            }
        }
    }

    AddHeadAndTailInsForPatchFuncMain0(patchMain0DefineIns);
    AddTailInsForPatchFuncMain1(patchMain1DefineIns);

    patchFuncMain0.ins = std::move(patchMain0DefineIns);
    patchFuncMain1.ins = std::move(patchMain1DefineIns);

    patchFuncMain0.return_type = panda::pandasm::Type("any", 0);
    patchFuncMain1.return_type = panda::pandasm::Type("any", 0);
}

void PatchFix::Finalize(panda::pandasm::Program **prog)
{
    if (IsDumpSymbolTable() || IsColdReload()) {
        return;
    }

    HandleModifiedClasses(*prog);

    HandleModifiedDefinedClassFunc(*prog);

    if (patchError_) {
        *prog = nullptr;
        errMsg_ << "[Patch] Found unsupported change in file, will not generate patch!" << std::endl;
        std::cerr << errMsg_.str();
        return;
    }

    if (IsHotReload() || IsColdFix()) {
        return;
    }

    panda::pandasm::Function patchFuncMain0(patchMain0_, (*prog)->lang);
    panda::pandasm::Function patchFuncMain1(patchMain1_, (*prog)->lang);
    CreateFunctionPatchMain0AndMain1(patchFuncMain0, patchFuncMain1);

    (*prog)->function_table.emplace(patchFuncMain0.name, std::move(patchFuncMain0));
    (*prog)->function_table.emplace(patchFuncMain1.name, std::move(patchFuncMain1));
}

bool PatchFix::CompareLexenv(const std::string &funcName, const compiler::PandaGen *pg,
    SymbolTable::OriginFunctionInfo &bytecodeInfo)
{
    auto &lexicalVarNameAndTypes = pg->TopScope()->GetLexicalVarNameAndTypes();
    auto &lexenv = bytecodeInfo.lexenv;
    if (funcName != funcMain0_) {
        if (lexenv.size() != lexicalVarNameAndTypes.size()) {
            errMsg_ << "[Patch] Found lexical variable added or removed in " + funcName + ", not supported!"
                << std::endl;
            patchError_ = true;
            return false;
        }
        for (auto &variable: lexicalVarNameAndTypes) {
            auto varSlot = variable.first;
            auto lexenvIter = lexenv.find(varSlot);
            if (lexenvIter == lexenv.end()) {
                errMsg_ << "[Patch] Found new lexical variable added in function " + funcName + ", not supported!"
                    << std::endl;
                patchError_ = true;
                return false;
            }

            auto &lexInfo = lexenvIter->second;
            if (!IsColdFix() && (std::string(variable.second.first) != lexInfo.first ||
                                 variable.second.second != lexInfo.second)) {
                errMsg_ << "[Patch] Found lexical variable changed in function " + funcName + ", not supported!"
                    << std::endl;
                patchError_ = true;
                return false;
            }
        }
    }
    return true;
}

bool PatchFix::CompareClassHash(std::vector<std::pair<std::string, std::string>> &hashList,
    SymbolTable::OriginFunctionInfo &bytecodeInfo)
{
    auto &classInfo = bytecodeInfo.classHash;
    for (size_t i = 0; i < hashList.size() - 1; ++i) {
        auto &className = hashList[i].first;
        auto classIter = classInfo.find(className);
        if (!IsHotReload() && classIter != classInfo.end() && classIter->second != hashList[i].second) {
            if (IsColdFix()) {
                modifiedClassNames_.insert(className);
                continue;
            } else {
                ASSERT(IsHotFix());
                errMsg_ << "[Patch] Found class " + hashList[i].first + " changed, not supported!" << std::endl;
            }
            patchError_ = true;
            return false;
        }
    }
    return true;
}

void PatchFix::CheckAndRestoreSpecialFunctionName(uint32_t globalIndexForSpecialFunc, std::string &funcInternalName,
    std::string recordName)
{
    auto it = originRecordHashFunctionNames_->find(recordName);
    if (it != originRecordHashFunctionNames_->end()) {
        if (it->second.size() == 0 || globalIndexForSpecialFunc > it->second.size()) {
            // anonymous, special or duplicate function added
            errMsg_ << "[Patch] Found new anonymous, special(containing '.' or '\\') or duplicate name function "
                    + funcInternalName + " not supported!" << std::endl;
            patchError_ = true;
            return;
        }
        std::string originalName = it->second.at(std::to_string(globalIndexForSpecialFunc));
        // special name function in the same position must have the same real function name as original
        if (originalName.substr(originalName.find_last_of("#")) !=
            funcInternalName.substr(funcInternalName.find_last_of("#"))) {
            errMsg_ << "[Patch] Found new anonymous, special(containing '.' or '\\') or duplicate name function "
                    + funcInternalName + " not supported!" << std::endl;
            patchError_ = true;
            return;
        }
        funcInternalName = originalName;
    }
}

void PatchFix::HandleFunction(const compiler::PandaGen *pg, panda::pandasm::Function *func,
    LiteralBuffers &literalBuffers)
{
    std::string funcName = func->name;
    auto originFunction = originFunctionInfo_->find(funcName);
    if (originFunction == originFunctionInfo_->end()) {
        if ((!util::Helpers::IsDefaultApiVersion(targetApiVersion_, targetApiSubVersion_)) &&
            IsHotFix() &&
            IsAnonymousOrSpecialOrDuplicateFunction(funcName)) {
            errMsg_ << "[Patch] Found new anonymous, special(containing '.' or '\\') or duplicate name function "
                      + funcName + " not supported!" << std::endl;
            patchError_ = true;
            return;
        }
        newFuncNames_.insert(funcName);
        CollectFuncDefineIns(func);
        return;
    }

    auto &bytecodeInfo = originFunction->second;
    if (!CompareLexenv(funcName, pg, bytecodeInfo)) {
        return;
    }

    auto hashList = GenerateFunctionAndClassHash(func, literalBuffers);
    if (!CompareClassHash(hashList, bytecodeInfo)) {
        return;
    }

    if (IsHotReload()) {
        return;
    }

    auto funcHash = hashList.back().second;

    if (funcName == funcMain0_) {
        if (IsHotFix()) {
            func->metadata->SetAttribute(EXTERNAL_ATTRIBUTE);
        } else {
            patchFuncNames_.insert(funcName);
        }
    } else {
        if (funcHash == bytecodeInfo.funcHash) {
            func->metadata->SetAttribute(EXTERNAL_ATTRIBUTE);
        } else {
            patchFuncNames_.insert(funcName);
        }
    }

    CollectFuncDefineIns(func);
}

void PatchFix::DumpFunctionInfo(const compiler::PandaGen *pg, panda::pandasm::Function *func,
    PatchFix::LiteralBuffers &literalBuffers)
{
    std::stringstream ss;

    ss << pg->InternalName();
    ss << SymbolTable::SECOND_LEVEL_SEPERATOR << pg->InternalName() << SymbolTable::SECOND_LEVEL_SEPERATOR;

    std::vector<std::pair<std::string, std::string>> hashList = GenerateFunctionAndClassHash(func, literalBuffers);
    ss << hashList.back().second << SymbolTable::SECOND_LEVEL_SEPERATOR;

    if (util::Helpers::IsDefaultApiVersion(targetApiVersion_, targetApiSubVersion_)) {
        auto internalNameStr = pg->InternalName().Mutf8();
        if (internalNameStr.find("#") != std::string::npos) {
            ss << (pg->Binder()->SpecialFuncNameIndexMap()).at(internalNameStr) << SymbolTable::SECOND_LEVEL_SEPERATOR;
        } else {
            // index 0 for all the normal name functions
            ss << "0" << SymbolTable::SECOND_LEVEL_SEPERATOR;
        }
    }

    ss << SymbolTable::FIRST_LEVEL_SEPERATOR;
    for (size_t i = 0; i < hashList.size() - 1; ++i) {
        ss << hashList[i].first << SymbolTable::SECOND_LEVEL_SEPERATOR << hashList[i].second <<
            SymbolTable::SECOND_LEVEL_SEPERATOR;
    }
    ss << SymbolTable::SECOND_LEVEL_SEPERATOR << SymbolTable::FIRST_LEVEL_SEPERATOR;

    for (auto &variable: pg->TopScope()->GetLexicalVarNameAndTypes()) {
        ss << variable.second.first << SymbolTable::SECOND_LEVEL_SEPERATOR
           << variable.first << SymbolTable::SECOND_LEVEL_SEPERATOR
           << variable.second.second << SymbolTable::SECOND_LEVEL_SEPERATOR;
    }
    ss << SymbolTable::SECOND_LEVEL_SEPERATOR << std::endl;

    symbolTable_->FillSymbolTable(ss);
}

bool PatchFix::IsAdditionalVarInPatch(uint32_t slot)
{
    return slot == UINT32_MAX;
}

bool PatchFix::IsDumpSymbolTable() const
{
    return patchFixKind_ == PatchFixKind::DUMPSYMBOLTABLE;
}

bool PatchFix::IsHotFix() const
{
    return patchFixKind_ == PatchFixKind::HOTFIX;
}

bool PatchFix::IsColdFix() const
{
    return patchFixKind_ == PatchFixKind::COLDFIX;
}

bool PatchFix::IsHotReload() const
{
    return patchFixKind_ == PatchFixKind::HOTRELOAD;
}

bool PatchFix::IsColdReload() const
{
    return patchFixKind_ == PatchFixKind::COLDRELOAD;
}

} // namespace panda::es2panda::util
