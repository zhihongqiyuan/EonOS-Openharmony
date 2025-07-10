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

#include "function.h"

#include "varbinder/varbinder.h"
#include "util/helpers.h"
#include "varbinder/scope.h"
#include "varbinder/variable.h"
#include "compiler/base/lreference.h"
#include "compiler/core/pandagen.h"
#include "ir/base/classDefinition.h"
#include "ir/base/classProperty.h"
#include "ir/base/scriptFunction.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/statements/blockStatement.h"

namespace ark::es2panda::compiler {
static void CompileSourceBlock(PandaGen *pg, const ir::BlockStatement *block)
{
    const auto &statements = block->Statements();
    if (statements.empty()) {
        pg->SetFirstStmt(block);
        pg->ImplicitReturn(block);
        return;
    }

    pg->SetFirstStmt(statements.front());

    for (const auto *stmt : statements) {
        stmt->Compile(pg);
    }

    switch (statements.back()->Type()) {
        case ir::AstNodeType::RETURN_STATEMENT: {
            return;
        }
        case ir::AstNodeType::VARIABLE_DECLARATION:
        case ir::AstNodeType::FUNCTION_DECLARATION:
        case ir::AstNodeType::STRUCT_DECLARATION:
        case ir::AstNodeType::CLASS_DECLARATION: {
            pg->ImplicitReturn(statements.back());
            break;
        }
        default: {
            if (pg->IsEval()) {
                pg->DirectReturn(statements.back());
            } else {
                pg->ImplicitReturn(statements.back());
            }
        }
    }
}

static void CompileFunctionParameterDeclaration(PandaGen *pg, const ir::ScriptFunction *func)
{
    ScopeContext scopeCtx(pg, func->Scope()->ParamScope());

    uint32_t index = 0;

    for (const auto *param : func->Params()) {
        auto ref = JSLReference::Create(pg, param, true);

        [[maybe_unused]] varbinder::Variable *paramVar = ref.Variable();

        if (ref.Kind() == ReferenceKind::DESTRUCTURING) {
            util::StringView name = util::Helpers::ToStringView(pg->Allocator(), index);
            paramVar = pg->Scope()->FindLocal(name, varbinder::ResolveBindingOptions::BINDINGS);
        }

        ASSERT(paramVar && paramVar->IsLocalVariable());

        VReg paramReg = VReg(varbinder::VarBinder::MANDATORY_PARAMS_NUMBER + VReg::PARAM_START + index++);
        ASSERT(paramVar->LexicalBound() || paramVar->AsLocalVariable()->Vreg() == paramReg);

        if (param->IsAssignmentPattern()) {
            RegScope rs(pg);
            pg->LoadAccumulator(func, paramReg);
            auto *nonDefaultLabel = pg->AllocLabel();

            if (ref.Kind() == ReferenceKind::DESTRUCTURING) {
                auto *loadParamLabel = pg->AllocLabel();

                pg->BranchIfNotUndefined(func, loadParamLabel);
                param->AsAssignmentPattern()->Right()->Compile(pg);
                pg->Branch(func, nonDefaultLabel);

                pg->SetLabel(func, loadParamLabel);
                pg->LoadAccumulator(func, paramReg);

                pg->SetLabel(func, nonDefaultLabel);
                ref.SetValue();
            } else {
                pg->BranchIfNotUndefined(func, nonDefaultLabel);

                param->AsAssignmentPattern()->Right()->Compile(pg);
                ref.SetValue();
                pg->SetLabel(func, nonDefaultLabel);
            }

            continue;
        }

        if (param->IsRestElement()) {
            pg->CopyRestArgs(param, func->Params().size() - 1);
        } else if (ref.Kind() == ReferenceKind::DESTRUCTURING) {
            pg->LoadAccumulator(func, paramReg);
        } else {
            continue;
        }
        ref.SetValue();
    }
}

void Function::LoadClassContexts(const ir::AstNode *node, PandaGen *pg, VReg ctor, const util::StringView &name)
{
    auto *classDef = util::Helpers::GetContainingClassDefinition(node);

    do {
        auto res = pg->Scope()->Find(classDef->PrivateId());
        ASSERT(res.variable);

        if (classDef->HasMatchingPrivateKey(name)) {
            pg->LoadLexicalVar(node, res.lexLevel, res.variable->AsLocalVariable()->LexIdx());
            pg->StoreAccumulator(node, ctor);
            break;
        }

        classDef = util::Helpers::GetContainingClassDefinition(classDef->Parent());
    } while (classDef != nullptr);
}

void Function::IterateOverElements(const ArenaVector<ir::AstNode *> &elements, PandaGen *pg, VReg &ctor, VReg &thisReg,
                                   VReg &computedInstanceFieldsArray)
{
    uint32_t computedInstanceFieldsIndex = 0;
    for (auto const &element : elements) {
        if (!element->IsClassProperty()) {
            continue;
        }

        const auto *prop = element->AsClassProperty();

        if ((prop->IsStatic())) {
            continue;
        }

        if (prop->IsPrivateElement()) {
            if (prop->Value() == nullptr) {
                pg->LoadConst(element, Constant::JS_UNDEFINED);
            } else {
                RegScope scopeProp(pg);
                prop->Value()->Compile(pg);
            }

            pg->ClassPrivateFieldAdd(prop, ctor, thisReg, prop->Key()->AsIdentifier()->Name());
            continue;
        }

        RegScope keyScope(pg);

        Operand key;
        if (prop->IsComputed()) {
            VReg keyReg = pg->AllocReg();
            pg->LoadAccumulator(prop, computedInstanceFieldsArray);
            pg->LoadObjByIndex(prop, computedInstanceFieldsIndex++);
            pg->StoreAccumulator(prop, keyReg);
            key = keyReg;
        } else {
            key = pg->ToOwnPropertyKey(prop->Key(), false);
        }

        if (prop->Value() == nullptr) {
            pg->LoadConst(element, Constant::JS_UNDEFINED);
        } else {
            RegScope scopeProp(pg);
            prop->Value()->Compile(pg);
        }

        pg->StoreOwnProperty(prop, thisReg, key);
    }
}

void Function::CompileInstanceFields(PandaGen *pg, const ir::ScriptFunction *decl)
{
    const auto klass = util::Helpers::GetClassDefiniton(decl);
    const auto &elements = klass->Body();

    RegScope rs(pg);
    auto thisReg = pg->AllocReg();
    auto ctor = pg->AllocReg();
    pg->GetThis(decl);
    pg->StoreAccumulator(decl, thisReg);
    pg->GetFunctionObject(decl);
    pg->StoreAccumulator(decl, ctor);

    VReg computedInstanceFieldsArray {};

    if (klass->HasPrivateMethod()) {
        pg->ClassPrivateMethodOrAccessorAdd(decl, ctor, thisReg);
    }

    if (klass->HasComputedInstanceField()) {
        computedInstanceFieldsArray = pg->AllocReg();
        pg->LoadClassComputedInstanceFields(klass, ctor);
        pg->StoreAccumulator(klass, computedInstanceFieldsArray);
    }

    IterateOverElements(elements, pg, ctor, thisReg, computedInstanceFieldsArray);
}

static void CompileFunction(PandaGen *pg)
{
    const auto *decl = pg->RootNode()->AsScriptFunction();

    if (decl->IsConstructor() && (util::Helpers::GetClassDefiniton(decl)->Super() == nullptr)) {
        Function::CompileInstanceFields(pg, decl);
    }

    auto *funcParamScope = pg->TopScope()->ParamScope();
    auto *nameVar = funcParamScope->NameVar();

    if (nameVar != nullptr) {
        RegScope rs(pg);
        pg->GetFunctionObject(pg->RootNode());
        pg->StoreAccToLexEnv(pg->RootNode(), funcParamScope->Find(nameVar->Name()), true);
    }

    CompileFunctionParameterDeclaration(pg, decl);

    pg->FunctionEnter();
    const ir::AstNode *body = decl->Body();

    if (body->IsExpression()) {
        body->Compile(pg);
        pg->DirectReturn(decl);
    } else {
        CompileSourceBlock(pg, body->AsBlockStatement());
    }

    pg->FunctionExit();
}

void Function::Compile(PandaGen *pg)
{
    FunctionRegScope lrs(pg);
    auto *topScope = pg->TopScope();

    if (pg->FunctionHasFinalizer()) {
        ASSERT(topScope->IsFunctionScope());

        TryContext tryCtx(pg);
        pg->FunctionInit(tryCtx.GetCatchTable());

        CompileFunction(pg);
    } else {
        pg->FunctionInit(nullptr);

        if (topScope->IsFunctionScope()) {
            CompileFunction(pg);
        } else {
            ASSERT(topScope->IsGlobalScope() || topScope->IsModuleScope());
            CompileSourceBlock(pg, pg->RootNode()->AsBlockStatement());
        }
    }

    pg->SortCatchTables();
}
}  // namespace ark::es2panda::compiler
