/**
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "function.h"

#include <compiler/base/lreference.h>
#include <compiler/core/pandagen.h>
#include <ir/base/classDefinition.h>
#include <ir/base/classStaticBlock.h>
#include <ir/base/scriptFunction.h>
#include <ir/expressions/assignmentExpression.h>
#include <ir/statements/blockStatement.h>
#include <ir/ts/tsParameterProperty.h>

namespace panda::es2panda::compiler {

static void FindLastStatement(const ir::AstNode *&lastNode, const ir::AstNode *currentNode)
{
    if (currentNode->IsStatement()) {
        if (currentNode->Range().start.index > lastNode->Range().start.index) {
            lastNode = currentNode;
        }
        currentNode->Iterate([&lastNode](auto *childNode) {
            FindLastStatement(lastNode, childNode);
        });
    }
}

static void CompileSourceBlock(PandaGen *pg, const ir::BlockStatement *block)
{
    bool endReturn = false;

    const auto &statements = block->Statements();
    pg->SetFirstStmt(statements.empty() ? block : statements.front());

    for (const auto *stmt : statements) {
        stmt->Compile(pg);

        if (stmt->IsReturnStatement() && (stmt == statements[statements.size() - 1])) {
            endReturn = true;
        }
    }

    if (endReturn) {
        return;
    }

    const ir::AstNode *associatedNode = block;
    if (!statements.empty()) {
        FindLastStatement(associatedNode, statements.back());
    }
    pg->ImplicitReturn(associatedNode);
}

static void CompileFunctionParameterDeclaration(PandaGen *pg, const ir::ScriptFunction *func)
{
    ScopeContext scopeCtx(pg, func->Scope()->ParamScope());

    uint32_t index = 0;

    for (const auto *it : func->Params()) {
        auto *param = it;
        if (param->IsTSParameterProperty()) {
            param = param->AsTSParameterProperty()->Parameter();
        }
        LReference ref = LReference::CreateLRef(pg, param, true);

        [[maybe_unused]] binder::Variable *paramVar = ref.Variable();

        if (ref.Kind() == ReferenceKind::DESTRUCTURING) {
            util::StringView name = util::Helpers::ToStringView(pg->Allocator(), index);
            paramVar = pg->Scope()->FindLocal(name, binder::ResolveBindingOptions::BINDINGS);
        }

        ASSERT(paramVar && paramVar->IsLocalVariable());

        VReg paramReg = binder::Binder::MANDATORY_PARAMS_NUMBER + index++;
        ASSERT(paramVar->LexicalBound() || paramVar->AsLocalVariable()->Vreg() == paramReg);

        // parameter has default value
        if (param->IsAssignmentPattern()) {
            RegScope rs(pg);

            ref.Kind() == ReferenceKind::DESTRUCTURING ?
                pg->LoadAccumulator(func, paramReg) : ref.GetValue();

            auto *nonDefaultLabel = pg->AllocLabel();

            if (ref.Kind() == ReferenceKind::DESTRUCTURING) {
                auto *loadParamLabel = pg->AllocLabel();

                pg->BranchIfStrictNotUndefined(func, loadParamLabel);
                param->AsAssignmentPattern()->Right()->Compile(pg);
                pg->Branch(func, nonDefaultLabel);

                pg->SetLabel(func, loadParamLabel);
                pg->LoadAccumulator(func, paramReg);

                pg->SetLabel(func, nonDefaultLabel);
                ref.SetValue();
            } else {
                pg->BranchIfStrictNotUndefined(func, nonDefaultLabel);

                param->AsAssignmentPattern()->Right()->Compile(pg);
                ref.SetValue();
                pg->SetLabel(func, nonDefaultLabel);
            }

            continue;
        }
        // There's extra optimization for super call in default constuctor since api13,
        // no need to generate copyrestargs in this scene.
        if (param->IsRestElement() && (!func->HasFlag(ir::ScriptFunctionFlags::GENERATED_CONSTRUCTOR) ||
            pg->Binder()->Program()->TargetApiVersion() < util::Helpers::SUPER_CALL_OPT_MIN_SUPPORTED_API_VERSION)) {
            pg->CopyRestArgs(param, func->Params().size() - 1);
        } else if (ref.Kind() == ReferenceKind::DESTRUCTURING) {
            pg->LoadAccumulator(func, paramReg);
        } else {
            continue;
        }
        ref.SetValue();
    }
}

static void CompileField(PandaGen *pg, const ir::ClassProperty *prop, VReg thisReg, int32_t level)
{
    RegScope rs(pg);
    Operand op;
    binder::PrivateNameFindResult result;
    if (prop->IsPrivate()) {
        result = pg->Scope()->FindPrivateName(prop->Key()->AsPrivateIdentifier()->Name());
    } else if (prop->IsComputed() && prop->NeedCompileKey()) {
        auto slot = prop->Parent()->AsClassDefinition()->GetSlot(prop->Key());
        pg->LoadLexicalVar(prop->Key(), level, slot);
        op = pg->AllocReg();
        pg->StoreAccumulator(prop->Key(), std::get<VReg>(op));
    } else {
        op = pg->ToPropertyKey(prop->Key(), prop->IsComputed());
    }

    if (!prop->Value()) {
        pg->LoadConst(prop, Constant::JS_UNDEFINED);
    } else {
        RegScope rsProp(pg);
        prop->Value()->Compile(pg);
    }

    if (prop->IsPrivate()) {
        pg->DefineClassPrivateField(prop, result.lexLevel, result.result.slot, thisReg);
    } else {
        pg->DefineOwnProperty(prop, thisReg, op);
    }
}

static void CompileClassInitializer(PandaGen *pg, const ir::ScriptFunction *decl, bool isStatic)
{
    const auto *classDef = decl->Parent()->Parent()->Parent()->AsClassDefinition();
    const auto &statements = classDef->Body();

    RegScope rs(pg);
    auto thisReg = pg->AllocReg();
    pg->GetThis(decl);
    pg->StoreAccumulator(decl, thisReg);
    auto [level, slot] = pg->Scope()->Find(nullptr, true);

    if (!isStatic && classDef->HasInstancePrivateMethod()) {
        binder::PrivateNameFindResult result = pg->Scope()->FindPrivateName("#method");
        pg->LoadConst(classDef, Constant::JS_UNDEFINED);
        pg->DefineClassPrivateField(classDef, result.lexLevel, result.result.slot, thisReg);
    }
    for (auto const &stmt : statements) {
        if (stmt->IsMethodDefinition()) {
            continue;
        }

        if (stmt->IsClassProperty()) {
            const auto *prop = stmt->AsClassProperty();

            // Do not process non-static public fields when not using define semantic.
            if (!prop->IsPrivate() && !prop->IsStatic() && !pg->Binder()->Program()->UseDefineSemantic()) {
                continue;
            }

            if (prop->IsStatic() == isStatic) {
                CompileField(pg, prop, thisReg, level);
            }
            continue;
        }

        if (!isStatic) {
            continue;
        }

        ASSERT(stmt->IsClassStaticBlock());
        const auto *staticBlock = stmt->AsClassStaticBlock();
        staticBlock->Compile(pg);
    }
}

static void CompileFunction(PandaGen *pg)
{
    const auto *decl = pg->RootNode()->AsScriptFunction();

    if (decl->IsConstructor()) {
        const auto *classDef = util::Helpers::GetClassDefiniton(decl);
        if (classDef->Super() == nullptr && classDef->NeedInstanceInitializer()) {
            RegScope rs(pg);
            auto thisReg = pg->AllocReg();

            pg->GetThis(decl);
            pg->StoreAccumulator(decl, thisReg);

            auto [level, slot] = pg->Scope()->Find(classDef->InstanceInitializer()->Key());
            pg->LoadLexicalVar(decl, level, slot);
            pg->CallInit(decl, thisReg);
        }
    }

    if (decl->IsStaticInitializer() || decl->IsInstanceInitializer()) {
        CompileClassInitializer(pg, decl, decl->IsStaticInitializer());
        pg->ImplicitReturn(decl);
        return;
    }

    auto *funcParamScope = pg->TopScope()->ParamScope();
    if (funcParamScope->NameVar()) {
        RegScope rs(pg);
        pg->GetFunctionObject(pg->RootNode());
        pg->StoreAccToLexEnv(pg->RootNode(), funcParamScope->Find(funcParamScope->NameVar()->Name()), true);
    }

    pg->SetSourceLocationFlag(lexer::SourceLocationFlag::INVALID_SOURCE_LOCATION);
    pg->FunctionEnter();
    pg->SetSourceLocationFlag(lexer::SourceLocationFlag::VALID_SOURCE_LOCATION);

    if (pg->IsAsyncFunction()) {
        CompileFunctionParameterDeclaration(pg, decl);
    }

    const ir::AstNode *body = decl->Body();

    if (body->IsExpression()) {
        body->Compile(pg);
        pg->ExplicitReturn(decl);
    } else {
        CompileSourceBlock(pg, body->AsBlockStatement());
    }

    pg->FunctionExit();
}

static void CompileFunctionOrProgram(PandaGen *pg)
{
    FunctionRegScope lrs(pg);
    const auto *topScope = pg->TopScope();

    if (pg->FunctionHasFinalizer()) {
        ASSERT(topScope->IsFunctionScope() || topScope->IsModuleScope());

        if (!pg->IsAsyncFunction()) {
            CompileFunctionParameterDeclaration(pg, pg->RootNode()->AsScriptFunction());
        }

        TryContext tryCtx(pg);
        pg->FunctionInit(tryCtx.GetCatchTable());
        if (topScope->IsModuleScope()) {
            pg->FunctionEnter();
            CompileSourceBlock(pg, pg->RootNode()->AsBlockStatement());
            pg->FunctionExit();
            return;
        }
        CompileFunction(pg);
    } else {
        pg->FunctionInit(nullptr);

        if (topScope->IsFunctionScope() || topScope->IsTSModuleScope() || topScope->IsTSEnumScope()) {
            CompileFunctionParameterDeclaration(pg, pg->RootNode()->AsScriptFunction());
            CompileFunction(pg);
        } else {
            ASSERT(topScope->IsGlobalScope() || topScope->IsModuleScope());
            CompileSourceBlock(pg, pg->RootNode()->AsBlockStatement());
        }
    }
}

void Function::Compile(PandaGen *pg)
{
    pg->SetFunctionKind();
    pg->SetInSendable();
    CompileFunctionOrProgram(pg);
    pg->SetSourceLocationFlag(lexer::SourceLocationFlag::INVALID_SOURCE_LOCATION);
    pg->CopyFunctionArguments(pg->RootNode());
    pg->InitializeLexEnv(pg->RootNode());
    pg->SetSourceLocationFlag(lexer::SourceLocationFlag::VALID_SOURCE_LOCATION);
    pg->AdjustSpillInsns();
    pg->SortCatchTables();
    pg->ReArrangeIc();
}

}  // namespace panda::es2panda::compiler
