/*
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

#include "ETSfunction.h"

#include "varbinder/varbinder.h"
#include "varbinder/ETSBinder.h"
#include "util/helpers.h"
#include "varbinder/scope.h"
#include "varbinder/variable.h"
#include "compiler/base/lreference.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/envScope.h"
#include "ir/base/spreadElement.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/classDefinition.h"
#include "ir/base/classProperty.h"
#include "ir/ets/etsParameterExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/statements/blockStatement.h"
#include "ir/ts/tsEnumDeclaration.h"
#include "ir/ts/tsEnumMember.h"
#include "checker/types/ets/types.h"

namespace ark::es2panda::compiler {
void ETSFunction::CallImplicitCtor(ETSGen *etsg)
{
    RegScope rs(etsg);
    auto *superType = etsg->ContainingObjectType()->SuperType();

    if (superType == nullptr) {
        etsg->CallExact(etsg->RootNode(), Signatures::BUILTIN_OBJECT_CTOR, etsg->GetThisReg());

        return;
    }

    auto res = std::find_if(superType->ConstructSignatures().cbegin(), superType->ConstructSignatures().cend(),
                            [](const checker::Signature *sig) { return sig->Params().empty(); });
    if (res == superType->ConstructSignatures().cend()) {
        return;
    }

    etsg->CallExact(etsg->RootNode(), (*res)->InternalName(), etsg->GetThisReg());
}

void ETSFunction::CompileSourceBlock(ETSGen *etsg, const ir::BlockStatement *block)
{
    auto *scriptFunc = etsg->RootNode()->AsScriptFunction();

    if (scriptFunc->IsEnum()) {
        // NOTE: add enum methods
    } else if (scriptFunc->IsStaticBlock()) {
        CompileAsStaticBlock(etsg);
    } else if (scriptFunc->IsConstructor()) {
        CompileAsConstructor(etsg, scriptFunc);
    }

    const auto &statements = block->Statements();

    if (statements.empty()) {
        etsg->SetFirstStmt(block);
        ExtendWithDefaultReturn(etsg, block, scriptFunc);
        return;
    }

    etsg->SetFirstStmt(statements.front());

    etsg->CompileStatements(statements);

    if (!statements.back()->IsReturnStatement()) {
        ExtendWithDefaultReturn(etsg, statements.back(), scriptFunc);
    }
}

void ETSFunction::ExtendWithDefaultReturn(ETSGen *etsg, const ir::AstNode *node, const ir::ScriptFunction *scriptFunc)
{
    if (etsg->ReturnType()->IsETSVoidType()) {
        etsg->EmitReturnVoid(node);
        return;
    }

    if (scriptFunc->ReturnTypeAnnotation() != nullptr && scriptFunc->ReturnTypeAnnotation()->TsType() != nullptr &&
        scriptFunc->ReturnTypeAnnotation()->TsType()->IsETSAsyncFuncReturnType()) {
        etsg->LoadDefaultValue(node, scriptFunc->ReturnTypeAnnotation()->TsType());
    } else {
        etsg->LoadDefaultValue(node, scriptFunc->Signature()->ReturnType());
    }
    etsg->ReturnAcc(node);
}

void ETSFunction::CompileAsStaticBlock(ETSGen *etsg)
{
    const auto *classDef = etsg->ContainingObjectType()->GetDeclNode()->AsClassDefinition();

    auto const checkInitializer = [](ArenaVector<ir::AstNode *> const &nodes) -> bool {
        for (auto const *const node : nodes) {
            if (node->IsMethodDefinition() && node->AsClassElement()->Key()->IsIdentifier() &&
                node->AsClassElement()->Id()->Name() == compiler::Signatures::INIT_METHOD) {
                return false;
            }
        }
        return true;
    };

    // Check if it is the Global class static constructor and the special '_$init$_" method exists
    bool const compileInitializer = classDef->IsGlobal() ? checkInitializer(classDef->Body()) : true;

    for (const auto *prop : classDef->Body()) {
        if (!prop->IsClassProperty() || !prop->IsStatic()) {
            continue;
        }

        // Don't compile variable initializers if they present in '_$init$_" method
        auto *const item = prop->AsClassProperty();
        if (item->Value() != nullptr &&
            (compileInitializer || item->IsConst() || item->Value()->IsArrowFunctionExpression())) {
            item->Compile(etsg);
        }
    }
}

void ETSFunction::CompileAsConstructor(ETSGen *etsg, const ir::ScriptFunction *scriptFunc)
{
    if (scriptFunc->IsImplicitSuperCallNeeded()) {
        CallImplicitCtor(etsg);
    }

    const auto *classDef = etsg->ContainingObjectType()->GetDeclNode()->AsClassDefinition();

    for (const auto *prop : classDef->Body()) {
        if (prop->IsClassProperty() && !prop->IsStatic()) {
            prop->AsClassProperty()->Compile(etsg);
        }
    }
}

void ETSFunction::CompileFunction(ETSGen *etsg)
{
    if (const auto *decl = etsg->RootNode()->AsScriptFunction(); !decl->IsDeclare() && !decl->IsExternal()) {
        if (auto *const body = decl->Body(); body != nullptr && body->IsBlockStatement()) {
            CompileSourceBlock(etsg, body->AsBlockStatement());
        }
    }
}

void ETSFunction::Compile(ETSGen *etsg)
{
    FunctionRegScope lrs(etsg);
    auto *topScope = etsg->TopScope();

    if (topScope->IsFunctionScope()) {
        CompileFunction(etsg);
    } else {
        ASSERT(topScope->IsGlobalScope());
        CompileSourceBlock(etsg, etsg->RootNode()->AsBlockStatement());
    }

    etsg->SortCatchTables();
}

}  // namespace ark::es2panda::compiler
