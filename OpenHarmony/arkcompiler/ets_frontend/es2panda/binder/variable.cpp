/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "variable.h"

#include <binder/scope.h>

namespace panda::es2panda::binder {

LocalVariable::LocalVariable(Decl *decl, VariableFlags flags) : Variable(decl, flags)
{
    if (decl->IsConstDecl()) {
        flags_ |= VariableFlags::READONLY;
    }
}

const util::StringView &Variable::Name() const
{
    return decl_->Name();
}

LocalVariable *LocalVariable::Copy(ArenaAllocator *allocator, Decl *decl) const
{
    auto *var = allocator->New<LocalVariable>(decl, flags_);
    CHECK_NOT_NULL(var);
    var->vreg_ = vreg_;
    return var;
}

void LocalVariable::SetLexical(Scope *scope, util::PatchFix *patchFixHelper)
{
    if (LexicalBound()) {
        return;
    }

    VariableScope *varScope = scope->IsFunctionParamScope() ?
        scope->AsFunctionParamScope()->GetFunctionScope() : scope->EnclosingVariableScope();
    CHECK_NOT_NULL(varScope);
    uint32_t slot = 0;
    auto name = Declaration()->Name();

    if (patchFixHelper && patchFixHelper->IsScopeValidToPatchLexical(varScope)) {
        // get slot from symbol table for lexical variable, if not found, slot is set to UINT32_MAX
        slot = patchFixHelper->GetSlotIdFromSymbolTable(std::string(name));
        // Store the additional lexical variable into PatchEnv
        if (patchFixHelper->IsAdditionalVarInPatch(slot)) {
            patchFixHelper->AllocSlotfromPatchEnv(std::string(name));
        } else {
            // Just for restore 'newlexenv' instruction for func_main_0 in patch
            varScope->RestoreFuncMain0LexEnv(patchFixHelper->GetEnvSizeOfFuncMain0());
        }
    } else {
        if (decl_ && decl_->NeedSetInSendableEnv(varScope)) {
            AddFlag(VariableFlags::IN_SENDABLE_ENV);
            slot = varScope->NextSendableSlot();
            BindLexEnvSlot(slot);
            return;
        } else {
            slot = varScope->NextSlot();
        }
    }

    BindLexEnvSlot(slot);
    // gather lexical variables for debuginfo
    varScope->AddLexicalVarNameAndType(slot, name,
        static_cast<typename std::underlying_type<binder::DeclType>::type>(Declaration()->Type()));
}

void GlobalVariable::SetLexical([[maybe_unused]] Scope *scope, [[maybe_unused]] util::PatchFix *patchFixHelper) {}
void ModuleVariable::SetLexical([[maybe_unused]] Scope *scope, [[maybe_unused]] util::PatchFix *patchFixHelper) {}
void EnumVariable::SetLexical([[maybe_unused]] Scope *scope, [[maybe_unused]] util::PatchFix *patchFixHelper) {}
void NamespaceVariable::SetLexical([[maybe_unused]] Scope *scope, [[maybe_unused]] util::PatchFix *patchFixHelper) {}
void ImportEqualsVariable::SetLexical([[maybe_unused]] Scope *scope, [[maybe_unused]] util::PatchFix *patchFixHelper) {}
void EnumLiteralVariable::SetLexical([[maybe_unused]] Scope *scope, [[maybe_unused]] util::PatchFix *patchFixHelper) {}

void EnumVariable::ResetDecl(Decl *decl)
{
    decl_ = decl;
}

}  // namespace panda::es2panda::binder
