/*
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

#include "codeGen.h"

#include "compiler/core/emitter.h"
#include "compiler/core/regAllocator.h"
#include "compiler/core/regScope.h"
#include "public/public.h"
#include "compiler/core/dynamicContext.h"
#include "compiler/base/catchTable.h"
#include "ir/base/scriptFunction.h"
#include "ir/expressions/identifier.h"

namespace ark::es2panda::compiler {

ArenaAllocator *CodeGen::Allocator() const noexcept
{
    return allocator_;
}

const ArenaVector<CatchTable *> &CodeGen::CatchList() const noexcept
{
    return catchList_;
}

const varbinder::FunctionScope *CodeGen::TopScope() const noexcept
{
    return topScope_;
}

const varbinder::Scope *CodeGen::Scope() const noexcept
{
    return scope_;
}

const ir::AstNode *CodeGen::RootNode() const noexcept
{
    return rootNode_;
}

ArenaVector<IRNode *> &CodeGen::Insns() noexcept
{
    return insns_;
}

const ArenaVector<IRNode *> &CodeGen::Insns() const noexcept
{
    return insns_;
}

VReg CodeGen::NextReg() const noexcept
{
    return VReg {usedRegs_};
}

std::uint32_t CodeGen::TotalRegsNum() const noexcept
{
    return totalRegs_;
}

std::size_t CodeGen::LabelCount() const noexcept
{
    return labelId_;
}

const DebugInfo &CodeGen::Debuginfo() const noexcept
{
    return debugInfo_;
}

VReg CodeGen::AllocReg()
{
    const VReg vreg(usedRegs_--);
    SetVRegType(vreg, nullptr);
    return vreg;
}

VReg CodeGen::AllocRegWithType(const checker::Type *const type)
{
    const VReg vreg(usedRegs_--);
    SetVRegType(vreg, type);
    return vreg;
}

void CodeGen::SetVRegType(const VReg vreg, const checker::Type *const type)
{
    typeMap_.insert_or_assign(vreg, type);
}

const checker::Type *CodeGen::GetVRegType(const VReg vreg) const
{
    const auto it = typeMap_.find(vreg);
    return it != typeMap_.end() ? it->second : nullptr;
}

checker::Type const *CodeGen::TypeForVar(varbinder::Variable const *var) const noexcept
{
    return var->TsType();
}

Label *CodeGen::AllocLabel()
{
    std::string id = std::string {Label::PREFIX} + std::to_string(labelId_++);
    return sa_.AllocLabel(std::move(id));
}

bool CodeGen::IsDebug() const noexcept
{
    return context_->config->options->CompilerOptions().isDebug;
}

std::uint32_t CodeGen::ParamCount() const noexcept
{
    if (rootNode_->IsProgram()) {
        return 0U;
    }

    return rootNode_->AsScriptFunction()->Params().size();
}

std::uint32_t CodeGen::FormalParametersCount() const noexcept
{
    if (rootNode_->IsProgram()) {
        return 0U;
    }

    ASSERT(rootNode_->IsScriptFunction());

    return rootNode_->AsScriptFunction()->FormalParamsLength();
}

std::uint32_t CodeGen::InternalParamCount() const noexcept
{
    static constexpr std::uint32_t HIDDEN_PARAMS = 3U;
    return ParamCount() + HIDDEN_PARAMS;
}

const util::StringView &CodeGen::InternalName() const noexcept
{
    return topScope_->InternalName();
}

const util::StringView &CodeGen::FunctionName() const noexcept
{
    return topScope_->Name();
}

varbinder::VarBinder *CodeGen::VarBinder() const noexcept
{
    return context_->parserProgram->VarBinder();
}

std::uint32_t CodeGen::AddLiteralBuffer(LiteralBuffer &&buf)
{
    programElement_->BuffStorage().emplace_back(std::move(buf));
    return literalBufferIdx_++;
}

void CodeGen::LoadAccumulatorString(const ir::AstNode *node, const util::StringView &str)
{
    sa_.Emit<LdaStr>(node, str);
}

void CodeGen::SetLabel([[maybe_unused]] const ir::AstNode *node, Label *label)
{
    sa_.AddLabel(label);
}

void CodeGen::Branch(const ir::AstNode *node, Label *label)
{
    sa_.Emit<Jmp>(node, label);
}

bool CodeGen::CheckControlFlowChange() const
{
    const auto *iter = dynamicContext_;

    while (iter != nullptr) {
        if (iter->HasFinalizer()) {
            return true;
        }

        iter = iter->Prev();
    }

    return false;
}

Label *CodeGen::ControlFlowChangeBreak(const ir::Identifier *label)
{
    auto *iter = dynamicContext_;

    util::StringView labelName = label != nullptr ? label->Name() : LabelTarget::BREAK_LABEL;
    Label *breakTarget = nullptr;

    while (iter != nullptr) {
        iter->AbortContext(ControlFlowChange::BREAK, labelName);
        const auto *constIter = iter;

        const auto &labelTargetName = constIter->Target().BreakLabel();

        if (constIter->Target().BreakTarget() != nullptr) {
            breakTarget = constIter->Target().BreakTarget();
        }

        if (labelTargetName == labelName) {
            break;
        }

        iter = iter->Prev();
    }

    return breakTarget;
}

Label *CodeGen::ControlFlowChangeContinue(const ir::Identifier *label)
{
    auto *iter = dynamicContext_;
    util::StringView labelName = label != nullptr ? label->Name() : LabelTarget::CONTINUE_LABEL;
    Label *continueTarget = nullptr;

    while (iter != nullptr) {
        iter->AbortContext(ControlFlowChange::CONTINUE, labelName);
        const auto *constIter = iter;

        const auto &labelTargetName = constIter->Target().ContinueLabel();

        if (constIter->Target().ContinueTarget() != nullptr) {
            continueTarget = constIter->Target().ContinueTarget();
        }

        if (labelTargetName == labelName) {
            break;
        }

        iter = iter->Prev();
    }

    return continueTarget;
}

std::uint32_t CodeGen::TryDepth() const
{
    const auto *iter = dynamicContext_;
    std::uint32_t depth = 0;

    while (iter != nullptr) {
        if (iter->HasTryCatch()) {
            depth++;
        }

        iter = iter->Prev();
    }

    return depth;
}

CatchTable *CodeGen::CreateCatchTable(const util::StringView exceptionType)
{
    auto *catchTable = allocator_->New<CatchTable>(this, TryDepth(), exceptionType);
    catchList_.push_back(catchTable);
    return catchTable;
}

CatchTable *CodeGen::CreateCatchTable(const LabelPair tryLabelPair, const util::StringView exceptionType)
{
    auto *catchTable = allocator_->New<CatchTable>(this, TryDepth(), tryLabelPair, exceptionType);
    catchList_.push_back(catchTable);
    return catchTable;
}

void CodeGen::SortCatchTables()
{
    std::stable_sort(catchList_.begin(), catchList_.end(),
                     [](const CatchTable *a, const CatchTable *b) { return b->Depth() < a->Depth(); });
}

void CodeGen::SetFirstStmt(const ir::Statement *stmt) noexcept
{
    debugInfo_.firstStmt_ = stmt;
}

void CodeGen::Unimplemented()
{
    throw Error(ErrorType::GENERIC, "", "Unimplemented code path");
}

SimpleAllocator &CodeGen::Sa() noexcept
{
    return sa_;
}

const SimpleAllocator &CodeGen::Sa() const noexcept
{
    return sa_;
}

RegAllocator &CodeGen::Ra() noexcept
{
    return ra_;
}

const RegAllocator &CodeGen::Ra() const noexcept
{
    return ra_;
}

RangeRegAllocator &CodeGen::Rra() noexcept
{
    return rra_;
}

const RangeRegAllocator &CodeGen::Rra() const noexcept
{
    return rra_;
}

public_lib::Context *CodeGen::Context() const noexcept
{
    return context_;
}

ProgramElement *CodeGen::ProgElement() const noexcept
{
    return programElement_;
}

CodeGen::TypeMap &CodeGen::GetTypeMap() noexcept
{
    return typeMap_;
}

const CodeGen::TypeMap &CodeGen::GetTypeMap() const noexcept
{
    return typeMap_;
}

compiler::AstCompiler *CodeGen::GetAstCompiler() const
{
    return astCompiler_;
}
}  // namespace ark::es2panda::compiler
