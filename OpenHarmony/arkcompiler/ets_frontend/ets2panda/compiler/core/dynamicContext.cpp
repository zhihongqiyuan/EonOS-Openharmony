/**
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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

#include "dynamicContext.h"

#include "checker/types/ets/etsObjectType.h"
#include "checker/types/type.h"
#include "compiler/core/envScope.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "compiler/base/catchTable.h"
#include "ir/expressions/identifier.h"
#include "ir/base/catchClause.h"
#include "ir/statements/blockStatement.h"
#include "ir/statements/breakStatement.h"
#include "ir/statements/continueStatement.h"
#include "ir/statements/returnStatement.h"
#include "ir/statements/tryStatement.h"
#include "ir/statements/labelledStatement.h"

namespace ark::es2panda::compiler {
DynamicContext::DynamicContext(CodeGen *cg, LabelTarget target) : cg_(cg), target_(target), prev_(Cg()->dynamicContext_)
{
    Cg()->dynamicContext_ = this;
}

DynamicContext::~DynamicContext()
{
    Cg()->dynamicContext_ = prev_;
}

LabelContext::LabelContext(CodeGen *cg, const ir::LabelledStatement *labelledStmt)
    : DynamicContext(cg, LabelTarget(labelledStmt->Ident()->Name())), labelledStmt_(labelledStmt)
{
    if (!labelledStmt->Body()->IsBlockStatement()) {
        return;
    }

    label_ = cg->AllocLabel();
    Target().SetBreakTarget(label_);
}

LabelContext::~LabelContext()
{
    if (label_ == nullptr) {
        return;
    }

    Cg()->SetLabel(labelledStmt_, label_);
}

LexEnvContext::LexEnvContext(LoopEnvScope *envScope, PandaGen *pg, LabelTarget target)
    : DynamicContext(pg, target), envScope_(envScope)
{
    if (!envScope_->HasEnv()) {
        return;
    }

    catchTable_ = Cg()->CreateCatchTable();
    const auto &labelSet = catchTable_->LabelSet();
    const auto *node = envScope_->Scope()->Node();

    Cg()->SetLabel(node, labelSet.TryBegin());
}

LexEnvContext::~LexEnvContext()
{
    if (!envScope_->HasEnv()) {
        return;
    }

    const auto &labelSet = catchTable_->LabelSet();
    const auto *node = envScope_->Scope()->Node();

    Cg()->SetLabel(node, labelSet.TryEnd());
    Cg()->Branch(node, labelSet.CatchEnd());

    Cg()->SetLabel(node, labelSet.CatchBegin());
    AsPandaGen()->PopLexEnv(node);
    AsPandaGen()->EmitThrow(node);
    Cg()->SetLabel(node, labelSet.CatchEnd());
    AsPandaGen()->PopLexEnv(node);
}

PandaGen *LexEnvContext::AsPandaGen() const
{
    return static_cast<PandaGen *>(Cg());
}

bool LexEnvContext::HasTryCatch() const
{
    return envScope_->HasEnv();
}

void LexEnvContext::AbortContext([[maybe_unused]] ControlFlowChange cfc,
                                 [[maybe_unused]] const util::StringView &targetLabel)
{
    if (cfc == ControlFlowChange::CONTINUE || !envScope_->HasEnv()) {
        return;
    }

    const auto *node = envScope_->Scope()->Node();
    AsPandaGen()->PopLexEnv(node);
}

IteratorContext::IteratorContext(PandaGen *pg, const Iterator &iterator, LabelTarget target)
    : DynamicContext(pg, target), iterator_(iterator), catchTable_(pg->CreateCatchTable())
{
    const auto &labelSet = catchTable_->LabelSet();
    pg->SetLabel(iterator_.Node(), labelSet.TryBegin());
}

IteratorContext::~IteratorContext()
{
    const auto &labelSet = catchTable_->LabelSet();
    const auto *node = iterator_.Node();

    Cg()->SetLabel(node, labelSet.TryEnd());
    Cg()->Branch(node, labelSet.CatchEnd());

    Cg()->SetLabel(node, labelSet.CatchBegin());
    iterator_.Close(true);
    Cg()->SetLabel(node, labelSet.CatchEnd());
}

void IteratorContext::AbortContext([[maybe_unused]] ControlFlowChange cfc,
                                   [[maybe_unused]] const util::StringView &targetLabel)
{
    if (cfc == ControlFlowChange::CONTINUE && Target().ContinueLabel() == targetLabel) {
        return;
    }

    iterator_.Close(false);
}

void TryContext::InitFinalizer()
{
    ASSERT(tryStmt_);

    if (!hasFinalizer_ || (tryStmt_->FinallyBlock() == nullptr)) {
        return;
    }

    auto *pg = static_cast<PandaGen *>(Cg());

    finalizerRun_ = pg->AllocReg();
    pg->StoreConst(tryStmt_, finalizerRun_, Constant::JS_UNDEFINED);
}

void CatchContext::InitCatchTable()
{
    auto *pg = static_cast<PandaGen *>(Cg());
    catchTable_ = pg->CreateCatchTable();
}

const TryLabelSet &CatchContext::LabelSet() const
{
    return catchTable_->LabelSet();
}

bool TryContext::HasFinalizer() const
{
    return hasFinalizer_;
}

void TryContext::EmitFinalizer()
{
    if (!hasFinalizer_ || inFinalizer_ || (tryStmt_->FinallyBlock() == nullptr)) {
        return;
    }

    auto *pg = static_cast<PandaGen *>(Cg());
    inFinalizer_ = true;
    tryStmt_->FinallyBlock()->Compile(pg);
    inFinalizer_ = false;
}

CatchTable *ETSCatchContext::AddNewCathTable(const util::StringView assemblerType)
{
    auto *cg = Cg();

    CatchTable *catchTable = cg->CreateCatchTable(assemblerType);
    catchTables_.push_back(catchTable);

    return catchTable;
}

CatchTable *ETSCatchContext::AddNewCathTable(const util::StringView assemblerType, const LabelPair tryLabelPair)
{
    auto *cg = Cg();

    CatchTable *catchTable = cg->CreateCatchTable(tryLabelPair, assemblerType);
    catchTables_.push_back(catchTable);

    return catchTable;
}

void ETSTryContext::EmitFinalizer(
    LabelPair trycatchLabelPair,
    const ArenaVector<std::pair<compiler::LabelPair, const ir::Statement *>> &finalizerInsertions)
{
    ASSERT(tryStmt_);

    if (!hasFinalizer_ || (tryStmt_->FinallyBlock() == nullptr)) {
        return;
    }
    auto *etsg = static_cast<ETSGen *>(Cg());

    CatchTable *finalizerTable = AddNewCathTable("", trycatchLabelPair);
    // First compile of the finaly clause, executed if the statement executed normally
    tryStmt_->FinallyBlock()->Compile(etsg);

    etsg->Branch(tryStmt_, finalizerTable->LabelSet().CatchEnd());

    for (std::pair<compiler::LabelPair, const ir::Statement *> insertion : finalizerInsertions) {
        EmitFinalizerInsertion(etsg, insertion.first, insertion.second);
    }

    etsg->SetLabel(tryStmt_, finalizerTable->LabelSet().CatchBegin());

    compiler::VReg exception = etsg->StoreException(tryStmt_);
    // Third compile of the finaly clause, executed if the statement executed abruptly
    tryStmt_->FinallyBlock()->Compile(etsg);

    etsg->LoadAccumulator(tryStmt_, exception);
    etsg->EmitThrow(tryStmt_, exception);

    etsg->SetLabel(tryStmt_, finalizerTable->LabelSet().CatchEnd());
}

void ETSTryContext::EmitFinalizerInsertion(ETSGen *etsg, compiler::LabelPair labelPair, const ir::Statement *statement)
{
    etsg->SetLabel(tryStmt_, labelPair.Begin());

    ASSERT(statement != nullptr);
    bool isReturn = statement->IsReturnStatement();

    compiler::RegScope rs(etsg);
    compiler::VReg res = etsg->AllocReg();

    if (isReturn) {
        etsg->SetAccumulatorType(statement->AsReturnStatement()->ReturnType());
        etsg->StoreAccumulator(tryStmt_, res);
        etsg->SetVRegType(res, statement->AsReturnStatement()->ReturnType());
    }

    // Second compile of the finaly clause, executed if the statement executed normally, but abrupted by
    // return, break, or continue statements.
    tryStmt_->FinallyBlock()->Compile(etsg);

    if (isReturn) {
        etsg->SetAccumulatorType(statement->AsReturnStatement()->ReturnType());
        etsg->LoadAccumulator(tryStmt_, res);
    }

    if (labelPair.End() != nullptr) {
        etsg->Branch(tryStmt_, labelPair.End());
    } else if (isReturn) {
        if (etsg->CheckControlFlowChange()) {
            etsg->StoreAccumulator(tryStmt_, res);
            etsg->ControlFlowChangeBreak();
            etsg->LoadAccumulator(tryStmt_, res);
        }

        etsg->ReturnAcc(tryStmt_);
    } else if (statement->IsBreakStatement()) {
        compiler::Label *target = etsg->ControlFlowChangeBreak(statement->AsBreakStatement()->Ident());
        etsg->Branch(tryStmt_, target);
    } else if (statement->IsContinueStatement()) {
        compiler::Label *target = etsg->ControlFlowChangeContinue(statement->AsContinueStatement()->Ident());
        etsg->Branch(tryStmt_, target);
    } else {
        UNREACHABLE();
    }
}

}  // namespace ark::es2panda::compiler
