/**
 * Copyright (c) 2021 - 2024 Huawei Device Co., Ltd.
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

#include "emitter.h"

#include "ir/irnode.h"
#include "util/helpers.h"
#include "varbinder/scope.h"
#include "varbinder/variable.h"
#include "compiler/base/literals.h"
#include "compiler/core/codeGen.h"
#include "compiler/core/regSpiller.h"
#include "compiler/debugger/debuginfoDumper.h"
#include "compiler/base/catchTable.h"
#include "es2panda.h"
#include "parser/program/program.h"
#include "checker/types/type.h"
#include "generated/isa.h"
#include "macros.h"
#include "public/public.h"

#include <string>
#include <string_view>
#include <tuple>
#include <utility>

namespace ark::es2panda::compiler {
using LiteralPair = std::pair<pandasm::LiteralArray::Literal, pandasm::LiteralArray::Literal>;

static LiteralPair TransformMethodLiterals(const compiler::Literal *literal)
{
    pandasm::LiteralArray::Literal valueLit;
    pandasm::LiteralArray::Literal tagLit;

    compiler::LiteralTag tag = literal->Tag();

    switch (tag) {
        case compiler::LiteralTag::METHOD: {
            valueLit.tag = panda_file::LiteralTag::METHOD;
            valueLit.value = literal->GetMethod();
            break;
        }
        case compiler::LiteralTag::ASYNC_METHOD: {
            valueLit.tag = panda_file::LiteralTag::ASYNCMETHOD;
            valueLit.value = literal->GetMethod();
            break;
        }
        case compiler::LiteralTag::GENERATOR_METHOD: {
            valueLit.tag = panda_file::LiteralTag::GENERATORMETHOD;
            valueLit.value = literal->GetMethod();
            break;
        }
        case compiler::LiteralTag::ASYNC_GENERATOR_METHOD: {
            valueLit.tag = panda_file::LiteralTag::ASYNCGENERATORMETHOD;
            valueLit.value = literal->GetMethod();
            break;
        }
        default: {
            UNREACHABLE();
            break;
        }
    }

    tagLit.tag = panda_file::LiteralTag::TAGVALUE;
    tagLit.value = static_cast<uint8_t>(valueLit.tag);

    return {tagLit, valueLit};
}

static LiteralPair TransformLiteral(const compiler::Literal *literal)
{
    pandasm::LiteralArray::Literal valueLit;
    pandasm::LiteralArray::Literal tagLit;

    compiler::LiteralTag tag = literal->Tag();

    switch (tag) {
        case compiler::LiteralTag::BOOLEAN: {
            valueLit.tag = panda_file::LiteralTag::BOOL;
            valueLit.value = literal->GetBoolean();
            break;
        }
        case compiler::LiteralTag::INTEGER: {
            valueLit.tag = panda_file::LiteralTag::INTEGER;
            valueLit.value = literal->GetInteger();
            break;
        }
        case compiler::LiteralTag::DOUBLE: {
            valueLit.tag = panda_file::LiteralTag::DOUBLE;
            valueLit.value = literal->GetDouble();
            break;
        }
        case compiler::LiteralTag::STRING: {
            valueLit.tag = panda_file::LiteralTag::STRING;
            valueLit.value = literal->GetString();
            break;
        }
        case compiler::LiteralTag::ACCESSOR: {
            valueLit.tag = panda_file::LiteralTag::ACCESSOR;
            valueLit.value = static_cast<uint8_t>(0);
            break;
        }
        case compiler::LiteralTag::NULL_VALUE: {
            valueLit.tag = panda_file::LiteralTag::NULLVALUE;
            valueLit.value = static_cast<uint8_t>(0);
            break;
        }
        default:
            return TransformMethodLiterals(literal);
    }

    tagLit.tag = panda_file::LiteralTag::TAGVALUE;
    tagLit.value = static_cast<uint8_t>(valueLit.tag);

    return {tagLit, valueLit};
}

void FunctionEmitter::Generate()
{
    auto *func = GenFunctionSignature();
    GenFunctionInstructions(func);
    GenVariablesDebugInfo(func);
    GenSourceFileDebugInfo(func);
    GenFunctionCatchTables(func);
    GenFunctionAnnotations(func);
}

util::StringView FunctionEmitter::SourceCode() const
{
    return cg_->VarBinder()->Program()->SourceCode();
}

static Format MatchFormat(const IRNode *node, const Formats &formats)
{
    std::array<const VReg *, IRNode::MAX_REG_OPERAND> regs {};
    auto regCnt = node->Registers(&regs);
    auto registers = Span<const VReg *>(regs.data(), regs.data() + regCnt);

    const auto *iter = formats.begin();

    for (; iter != formats.end(); iter++) {  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto format = *iter;
        size_t limit = 0;
        for (const auto &formatItem : format.GetFormatItem()) {
            if (formatItem.IsVReg()) {
                limit = 1U << formatItem.BitWidth();
                break;
            }
        }

        if (std::all_of(registers.begin(), registers.end(), [limit](const VReg *reg) { return reg->IsValid(limit); })) {
            return format;
        }
    }

    UNREACHABLE();
    return *iter;
}

static size_t GetIRNodeWholeLength(const IRNode *node)
{
    Formats formats = node->GetFormats();
    if (formats.empty()) {
        return 0;
    }

    size_t len = 1;
    const auto format = MatchFormat(node, formats);

    for (auto fi : format.GetFormatItem()) {
        len += fi.BitWidth() / 8U;
    }

    return len;
}

static std::string WholeLine(const util::StringView &source, lexer::SourceRange range)
{
    if (source.Empty()) {
        return {};
    }
    ASSERT(range.end.index <= source.Length());
    ASSERT(range.end.index >= range.start.index);
    return source.Substr(range.start.index, range.end.index).EscapeSymbol<util::StringView::Mutf8Encode>();
}

void FunctionEmitter::GenInstructionDebugInfo(const IRNode *ins, pandasm::Ins *pandaIns)
{
    const ir::AstNode *astNode = ins->Node();

    ASSERT(astNode != nullptr);

    if (astNode == FIRST_NODE_OF_FUNCTION) {
        astNode = cg_->Debuginfo().FirstStatement();
        if (astNode == nullptr) {
            return;
        }
    }

    auto nodeRange = astNode->Range();
    pandaIns->insDebug.lineNumber = nodeRange.start.line + 1;

    if (cg_->IsDebug()) {
        size_t insLen = GetIRNodeWholeLength(ins);
        if (insLen != 0) {
            pandaIns->insDebug.boundLeft = offset_;
            pandaIns->insDebug.boundRight = offset_ + insLen;
        }

        offset_ += insLen;
        pandaIns->insDebug.wholeLine = WholeLine(SourceCode(), nodeRange);
    }
}

void FunctionEmitter::GenFunctionInstructions(pandasm::Function *func)
{
    func->ins.reserve(cg_->Insns().size());

    uint32_t totalRegs = cg_->TotalRegsNum();

    for (const auto *ins : cg_->Insns()) {
        auto &pandaIns = func->ins.emplace_back();

        ins->Transform(&pandaIns, programElement_, totalRegs);
        GenInstructionDebugInfo(ins, &pandaIns);
    }
}

void FunctionEmitter::GenFunctionAnnotations(pandasm::Function *func)
{
    pandasm::AnnotationData funcAnnotationData("_ESAnnotation");
    pandasm::AnnotationElement icSizeAnnotationElement(
        "icSize",
        std::make_unique<pandasm::ScalarValue>(pandasm::ScalarValue::Create<pandasm::Value::Type::U32>(cg_->IcSize())));
    funcAnnotationData.AddElement(std::move(icSizeAnnotationElement));

    pandasm::AnnotationElement parameterLengthAnnotationElement(
        "parameterLength", std::make_unique<pandasm::ScalarValue>(
                               pandasm::ScalarValue::Create<pandasm::Value::Type::U32>(cg_->FormalParametersCount())));
    funcAnnotationData.AddElement(std::move(parameterLengthAnnotationElement));

    pandasm::AnnotationElement funcNameAnnotationElement(
        "funcName", std::make_unique<pandasm::ScalarValue>(
                        pandasm::ScalarValue::Create<pandasm::Value::Type::STRING>(cg_->FunctionName().Mutf8())));
    funcAnnotationData.AddElement(std::move(funcNameAnnotationElement));

    func->metadata->AddAnnotations({funcAnnotationData});
}

void FunctionEmitter::GenFunctionCatchTables(pandasm::Function *func)
{
    func->catchBlocks.reserve(cg_->CatchList().size());

    for (const auto *catchBlock : cg_->CatchList()) {
        const auto &labelSet = catchBlock->LabelSet();

        auto &pandaCatchBlock = func->catchBlocks.emplace_back();
        pandaCatchBlock.exceptionRecord = catchBlock->Exception();
        pandaCatchBlock.tryBeginLabel = labelSet.TryBegin()->Id();
        pandaCatchBlock.tryEndLabel = labelSet.TryEnd()->Id();
        pandaCatchBlock.catchBeginLabel = labelSet.CatchBegin()->Id();
        pandaCatchBlock.catchEndLabel = labelSet.CatchBegin()->Id();
    }
}

void FunctionEmitter::GenSourceFileDebugInfo(pandasm::Function *func)
{
    func->sourceFile = std::string {cg_->VarBinder()->Program()->SourceFile().GetAbsolutePath()};

    if (!cg_->IsDebug()) {
        return;
    }

    if (cg_->RootNode()->IsProgram()) {
        func->sourceCode = SourceCode().EscapeSymbol<util::StringView::Mutf8Encode>();
    }
}

static void GenLocalVariableInfo(pandasm::debuginfo::LocalVariable &variableDebug, varbinder::Variable *var,
                                 std::tuple<uint32_t, uint32_t, uint32_t> info, const ScriptExtension extension)
{
    const auto [start, varsLength, totalRegsNum] = info;

    variableDebug.name = var->Name().Mutf8();

    if (extension == ScriptExtension::JS) {
        variableDebug.signature = "any";
        variableDebug.signatureType = "any";
    } else {
        ASSERT(var->AsLocalVariable()->TsType() != nullptr);
        std::stringstream ss;
        var->AsLocalVariable()->TsType()->ToDebugInfoType(ss);
        variableDebug.signature = ss.str();
        variableDebug.signatureType = ss.str();  // NOTE: Handle typeParams, either class or interface
    }

    variableDebug.reg =
        static_cast<int32_t>(IRNode::MapRegister(var->AsLocalVariable()->Vreg().GetIndex(), totalRegsNum));
    variableDebug.start = start;
    variableDebug.length = static_cast<uint32_t>(varsLength);
}

void FunctionEmitter::GenScopeVariableInfoEnd(pandasm::Function *func, const varbinder::Scope *scope, uint32_t count,
                                              uint32_t scopeStart, const VariablesStartsMap &starts) const
{
    const auto extension = cg_->VarBinder()->Program()->Extension();
    auto varsLength = static_cast<uint32_t>(count - scopeStart + 1);

    if (scope->IsFunctionScope()) {
        for (auto *param : scope->AsFunctionScope()->ParamScope()->Params()) {
            auto &variableDebug = func->localVariableDebug.emplace_back();
            GenLocalVariableInfo(variableDebug, param, std::make_tuple(scopeStart, varsLength, cg_->TotalRegsNum()),
                                 extension);
        }
    }
    const auto &unsortedBindings = scope->Bindings();
    std::map<util::StringView, es2panda::varbinder::Variable *> bindings(unsortedBindings.begin(),
                                                                         unsortedBindings.end());
    for (const auto &[_, variable] : bindings) {
        (void)_;
        const auto *decl = variable->Declaration();
        // NOTE(dslynko, #19203): do not generate debug-info for labels and local classes and interfaces
        if (!variable->IsLocalVariable() || variable->LexicalBound() || decl->IsTypeAliasDecl() ||
            decl->IsLabelDecl() || decl->IsClassDecl() || decl->IsInterfaceDecl()) {
            continue;
        }
        if (decl->IsParameterDecl() && !scope->IsCatchParamScope()) {
            continue;
        }

        uint32_t localStart = scopeStart;
        uint32_t localLength = varsLength;
        auto iter = starts.find(variable);
        if (iter != starts.end()) {
            localStart = iter->second;
            localLength = static_cast<uint32_t>(count - localStart + 1);
        }

        auto &variableDebug = func->localVariableDebug.emplace_back();
        GenLocalVariableInfo(variableDebug, variable, std::make_tuple(localStart, localLength, cg_->TotalRegsNum()),
                             extension);
    }
}

void FunctionEmitter::GenScopeVariableInfo(pandasm::Function *func, const varbinder::Scope *scope) const
{
    const auto &instructions = cg_->Insns();
    auto lastIter = instructions.end();
    auto iter = std::find(instructions.begin(), lastIter, scope->ScopeStart());
    if (iter == lastIter || *iter == scope->ScopeEnd()) {
        return;
    }
    uint32_t count = iter - instructions.begin();
    uint32_t start = count;

    auto checkNodeIsValid = [](const ir::AstNode *node) { return node != nullptr && node != FIRST_NODE_OF_FUNCTION; };
    // NOTE(dslynko, #19090): need to track start location for each local variable
    std::unordered_map<const varbinder::Variable *, uint32_t> varsStarts;
    for (const auto *scopeEnd = scope->ScopeEnd(); iter != lastIter && *iter != scopeEnd; ++iter, ++count) {
        const auto *node = (*iter)->Node();
        if (!checkNodeIsValid(node)) {
            continue;
        }
        auto *parent = node->Parent();
        if (!checkNodeIsValid(parent)) {
            continue;
        }

        const varbinder::Variable *var = nullptr;
        if (parent->IsVariableDeclarator()) {
            var = parent->AsVariableDeclarator()->Id()->Variable();
        } else if (parent->IsCatchClause()) {
            var = node->Variable();
        }
        if (var != nullptr && varsStarts.count(var) == 0) {
            varsStarts.emplace(var, count);
        }
    }
    ASSERT(iter != lastIter);

    GenScopeVariableInfoEnd(func, scope, count, start, varsStarts);
}

void FunctionEmitter::GenVariablesDebugInfo(pandasm::Function *func)
{
    if (!cg_->IsDebug()) {
        return;
    }

    for (const auto *scope : cg_->Debuginfo().VariableDebugInfo()) {
        GenScopeVariableInfo(func, scope);
    }
}

// Emitter

Emitter::Emitter(const public_lib::Context *context) : context_(context)
{
    prog_ = new pandasm::Program();
}

Emitter::~Emitter()
{
    delete prog_;
}

static void UpdateLiteralBufferId([[maybe_unused]] ark::pandasm::Ins *ins, [[maybe_unused]] uint32_t offset)
{
#ifdef PANDA_WITH_ECMASCRIPT
    switch (ins->opcode) {
        case pandasm::Opcode::ECMA_DEFINECLASSWITHBUFFER: {
            ins->imms.back() = std::get<int64_t>(ins->imms.back()) + offset;
            break;
        }
        case pandasm::Opcode::ECMA_CREATEARRAYWITHBUFFER:
        case pandasm::Opcode::ECMA_CREATEOBJECTWITHBUFFER:
        case pandasm::Opcode::ECMA_CREATEOBJECTHAVINGMETHOD:
        case pandasm::Opcode::ECMA_DEFINECLASSPRIVATEFIELDS: {
            uint32_t storedOffset = std::stoi(ins->ids.back());
            storedOffset += offset;
            ins->ids.back() = std::to_string(storedOffset);
            break;
        }
        default: {
            UNREACHABLE();
            break;
        }
    }
#else
    UNREACHABLE();
#endif
}

void Emitter::AddProgramElement(ProgramElement *programElement)
{
    prog_->strings.insert(programElement->Strings().begin(), programElement->Strings().end());

    uint32_t newLiteralBufferIndex = literalBufferIndex_;
    for (const auto &buff : programElement->BuffStorage()) {
        AddLiteralBuffer(buff, newLiteralBufferIndex++);
    }

    for (auto *ins : programElement->LiteralBufferIns()) {
        UpdateLiteralBufferId(ins, literalBufferIndex_);
    }

    literalBufferIndex_ = newLiteralBufferIndex;

    auto *function = programElement->Function();
    prog_->functionTable.emplace(function->name, std::move(*function));
}

static std::string CanonicalizeName(std::string name)
{
    std::replace_if(
        name.begin(), name.end(), [](char c) { return (c == '<' || c == '>' || c == '.' || c == ':' || c == ';'); },
        '-');
    name.append(std::to_string(0));
    return name;
}

void Emitter::DumpAsm(const pandasm::Program *prog)
{
    auto &ss = std::cout;

    ss << ".language ECMAScript" << std::endl << std::endl;

    for (auto &[name, func] : prog->functionTable) {
        ss << ".function any " << CanonicalizeName(name) << '(';

        for (uint32_t i = 0; i < func.GetParamsNum(); i++) {
            ss << "any a" << std::to_string(i);

            if (i != func.GetParamsNum() - 1) {
                ss << ", ";
            }
        }

        ss << ") {" << std::endl;

        for (const auto &ins : func.ins) {
            ss << (ins.setLabel ? "" : "\t") << ins.ToString("", true, func.GetTotalRegs()) << std::endl;
        }

        ss << "}" << std::endl << std::endl;

        for (const auto &ct : func.catchBlocks) {
            if (ct.exceptionRecord.empty()) {
                ss << ".catchall ";
            } else {
                ss << ".catch " << ct.exceptionRecord << ", ";
            }
            ss << ct.tryBeginLabel << ", " << ct.tryEndLabel << ", " << ct.catchBeginLabel << std::endl << std::endl;
        }
    }

    ss << std::endl;
}

void Emitter::AddLiteralBuffer(const LiteralBuffer &literals, uint32_t index)
{
    std::vector<pandasm::LiteralArray::Literal> literalArray;

    for (const auto &literal : literals) {
        auto [tagLit, valueLit] = TransformLiteral(&literal);
        literalArray.emplace_back(tagLit);
        literalArray.emplace_back(valueLit);
    }

    auto literalArrayInstance = pandasm::LiteralArray(std::move(literalArray));
    prog_->literalarrayTable.emplace(std::to_string(index), std::move(literalArrayInstance));
}

pandasm::Program *Emitter::Finalize(bool dumpDebugInfo, std::string_view globalClass)
{
    if (dumpDebugInfo) {
        debuginfo::DebugInfoDumper dumper(prog_);
        dumper.Dump();
    }

    if (context_->parserProgram->VarBinder()->IsGenStdLib()) {
        auto it = prog_->recordTable.find(std::string(globalClass));
        if (it != prog_->recordTable.end()) {
            prog_->recordTable.erase(it);
        }
    }
    auto *prog = prog_;
    prog_ = nullptr;
    return prog;
}
}  // namespace ark::es2panda::compiler
