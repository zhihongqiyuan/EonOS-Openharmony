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

#include "regScope.h"

#include "varbinder/varbinder.h"
#include "varbinder/scope.h"
#include "varbinder/variable.h"
#include "compiler/base/hoisting.h"
#include "compiler/core/codeGen.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/moduleContext.h"

namespace ark::es2panda::compiler {
// RegScope

RegScope::RegScope(CodeGen *cg) : cg_(cg), regBase_(cg_->usedRegs_) {}

RegScope::~RegScope()
{
    cg_->totalRegs_ = std::min(cg_->totalRegs_, cg_->usedRegs_);
    cg_->usedRegs_ = regBase_;
}

void RegScope::DebuggerCloseScope()
{
    if (!cg_->IsDebug()) {
        return;
    }

    cg_->scope_->SetScopeEnd(cg_->insns_.back());
}

// LocalRegScope

LocalRegScope::LocalRegScope(PandaGen *pg) : RegScope(pg) {}

LocalRegScope::LocalRegScope(CodeGen *cg, varbinder::Scope *scope) : RegScope(cg)
{
    prevScope_ = cg_->scope_;
    cg_->scope_ = scope;

    for (const auto &[_, var] : scope->OrderedBindings(cg_->Allocator())) {
        (void)_;
        if (!var->LexicalBound() && var->IsLocalVariable()) {
            var->AsLocalVariable()->BindVReg(cg_->AllocReg());
        }
    }

    if (cg_->IsDebug() && !cg_->insns_.empty()) {
        cg_->scope_->SetScopeStart(cg_->insns_.back());
        cg_->debugInfo_.VariableDebugInfo().push_back(cg_->scope_);
    }
}

LocalRegScope::LocalRegScope(PandaGen *pg, varbinder::Scope *scope) : LocalRegScope(static_cast<CodeGen *>(pg), scope)
{
    Hoisting::Hoist(pg);
}

LocalRegScope::~LocalRegScope()
{
    if (prevScope_ == nullptr) {
        return;
    }

    DebuggerCloseScope();

    cg_->scope_ = prevScope_;
}

// FunctionRegScope

FunctionRegScope::FunctionRegScope(CodeGen *cg) : RegScope(cg)
{
    InitializeParams([](varbinder::LocalVariable *const param, const VReg paramReg) { param->BindVReg(paramReg); });
}

void FunctionRegScope::InitializeParams(const StoreParamCb &cb)
{
    const auto *funcScope = cg_->Scope()->AsFunctionVariableScope();

    VReg paramReg = VReg(VReg::PARAM_START);
    for (auto *param : funcScope->ParamScope()->Params()) {
        cg_->SetVRegType(paramReg, cg_->TypeForVar(param));
        cb(param, paramReg);
        paramReg++;
    }

    for (const auto it : funcScope->OrderedBindings(cg_->Allocator())) {
        auto *const var = std::get<1>(it);
        if (var->Declaration()->IsParameterDecl() || var->Declaration()->IsTypeAliasDecl()) {
            continue;
        }

        if (!var->LexicalBound() && var->IsLocalVariable()) {
            const auto vreg = cg_->AllocReg();
            var->AsLocalVariable()->BindVReg(vreg);
        }
    }

    if (cg_->IsDebug()) {
        cg_->debugInfo_.VariableDebugInfo().push_back(funcScope);
    }
}

FunctionRegScope::FunctionRegScope(PandaGen *pg) : RegScope(pg), envScope_(pg->Allocator()->New<EnvScope>())
{
    ASSERT(cg_->Scope()->IsFunctionVariableScope());
    ASSERT(cg_->NextReg().GetIndex() == VReg::REG_START);

    VReg lexEnv = pg->AllocReg();
    envScope_->Initialize(pg, lexEnv);

    const auto *funcScope = pg->Scope()->AsFunctionVariableScope();
    const auto *node = pg->RootNode();

    if (funcScope->NeedLexEnv()) {
        pg->NewLexEnv(node, funcScope->LexicalSlots());
    } else {
        pg->LdLexEnv(node);
    }

    pg->StoreAccumulator(node, lexEnv);

    InitializeParams([pg, node](varbinder::LocalVariable *const param, const VReg paramReg) {
        if (param->LexicalBound()) {
            pg->LoadAccumulator(node, paramReg);
            pg->StoreLexicalVar(node, 0, param->LexIdx());
        } else {
            param->BindVReg(paramReg);
        }
    });

    pg->LoadAccFromArgs(pg->rootNode_);

    if (funcScope->IsModuleScope()) {
        ModuleContext::Compile(pg, pg->scope_->AsModuleScope());
    }

    Hoisting::Hoist(pg);
}

FunctionRegScope::~FunctionRegScope()
{
    if (cg_->IsDebug() && !cg_->insns_.empty()) {
        cg_->topScope_->SetScopeStart(cg_->insns_.front());
        DebuggerCloseScope();
    }

    if (envScope_ != nullptr) {
        envScope_->~EnvScope();
    }
}
}  // namespace ark::es2panda::compiler
