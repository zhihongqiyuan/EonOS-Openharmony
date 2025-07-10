/*
 * Copyright (c) 2021 - 2023 Huawei Device Co., Ltd.
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

#include "envScope.h"

#include "compiler/core/codeGen.h"
#include "compiler/core/pandagen.h"
#include "ir/statement.h"

namespace ark::es2panda::compiler {
ScopeContext::ScopeContext(CodeGen *cg, varbinder::Scope *newScope) : cg_(cg), prevScope_(cg_->scope_)
{
    cg->scope_ = newScope;
}

ScopeContext::~ScopeContext()
{
    cg_->scope_ = prevScope_;
}

void EnvScope::Initialize(PandaGen *pg, VReg lexEnv)
{
    pg_ = pg;
    prev_ = pg_->envScope_;
    lexEnv_ = lexEnv;
    pg_->envScope_ = this;
}

EnvScope::~EnvScope()
{
    if (pg_ == nullptr) {
        return;
    }

    pg_->envScope_ = prev_;
}

void LoopEnvScope::CopyBindings(PandaGen *pg, varbinder::VariableScope *scope, varbinder::VariableFlags flag)
{
    if (!HasEnv()) {
        return;
    }

    Initialize(pg, pg->AllocReg());

    pg_->NewLexEnv(scope_->Node(), scope->LexicalSlots());
    pg_->StoreAccumulator(scope_->Node(), lexEnv_);

    ASSERT(scope->NeedLexEnv());

    for (const auto &[_, variable] : scope_->OrderedBindings(pg_->Allocator())) {
        (void)_;
        if (!variable->HasFlag(flag)) {
            continue;
        }

        pg->LoadLexicalVar(scope_->Node(), 1, variable->AsLocalVariable()->LexIdx());
        pg->StoreLexicalVar(scope_->Parent()->Node(), 0, variable->AsLocalVariable()->LexIdx());
    }
}

void LoopEnvScope::CopyPetIterationCtx()
{
    if (!HasEnv()) {
        return;
    }

    pg_->CopyLexEnv(scope_->Node());
    pg_->StoreAccumulator(scope_->Node(), lexEnv_);
}
}  // namespace ark::es2panda::compiler
