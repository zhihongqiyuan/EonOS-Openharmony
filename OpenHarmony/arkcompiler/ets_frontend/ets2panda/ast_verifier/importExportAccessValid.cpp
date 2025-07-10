/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "importExportAccessValid.h"
#include "helpers.h"
#include "ir/expressions/callExpression.h"
#include "checker/types/ets/etsObjectType.h"
#include "ir/module/importSpecifier.h"
#include "ir/module/importNamespaceSpecifier.h"
#include "ir/module/importDefaultSpecifier.h"
#include "ir/expressions/identifier.h"
#include "ir/ets/etsImportDeclaration.h"
#include "checker/types/signature.h"

namespace ark::es2panda::compiler::ast_verifier {

[[nodiscard]] CheckResult ImportExportAccessValid::operator()(CheckContext &ctx, const ir::AstNode *ast)
{
    std::unordered_set<std::string> importedVariables {};
    if (ast->IsETSImportDeclaration()) {
        const auto importDecl = ast->AsETSImportDeclaration()->Specifiers();
        const auto name = [](ir::AstNode *const specifier) {
            if (specifier->IsImportNamespaceSpecifier()) {
                return specifier->AsImportNamespaceSpecifier()->Local()->Name();
            }
            if (specifier->IsImportSpecifier()) {
                return specifier->AsImportSpecifier()->Local()->Name();
            }
            return specifier->AsImportDefaultSpecifier()->Local()->Name();
        };
        for (const auto import : importDecl) {
            importedVariables.emplace(name(import));
        }
    }
    if (ast->IsCallExpression()) {
        const auto *callExpr = ast->AsCallExpression();
        const auto *callee = callExpr->Callee();
        if (callee != nullptr && callee->IsIdentifier() &&
            !HandleImportExportIdentifier(importedVariables, callee->AsIdentifier(), callExpr)) {
            ctx.AddCheckMessage("PROPERTY_NOT_VISIBLE_HERE(NOT_EXPORTED)", *callee, callee->Start());
            return {CheckDecision::INCORRECT, CheckAction::CONTINUE};
        }
    }
    if (ast->IsIdentifier() && !HandleImportExportIdentifier(importedVariables, ast->AsIdentifier(), nullptr)) {
        ctx.AddCheckMessage("PROPERTY_NOT_VISIBLE_HERE(NOT_EXPORTED)", *ast, ast->Start());
        return {CheckDecision::INCORRECT, CheckAction::CONTINUE};
    }
    return {CheckDecision::CORRECT, CheckAction::CONTINUE};
}

bool ImportExportAccessValid::ValidateExport(const varbinder::Variable *var)
{
    const auto *decl = var->Declaration();
    if (decl == nullptr) {
        return false;
    }
    const auto *node = decl->Node();
    if (node == nullptr) {
        return false;
    }
    return node->IsExported() || node->IsExportedType();
}

bool ImportExportAccessValid::InvariantImportExportMethod(const std::unordered_set<std::string> &importedVariables,
                                                          const varbinder::Variable *varCallee,
                                                          const ir::AstNode *callExpr, util::StringView name)
{
    auto *signature = callExpr->AsCallExpression()->Signature();
    if (signature == nullptr || signature->Owner() == nullptr) {
        // NOTE(vpukhov): Add a synthetic owner for dynamic signatures
        ASSERT(callExpr->AsCallExpression()->Callee()->TsType()->HasTypeFlag(checker::TypeFlag::ETS_DYNAMIC_FLAG));
        return true;
    }

    if (signature != nullptr && varCallee->Declaration() != nullptr && varCallee->Declaration()->Node() != nullptr &&
        !IsContainedIn(varCallee->Declaration()->Node(), signature->Owner()->GetDeclNode()) &&
        varCallee->Declaration()->Node() != signature->Owner()->GetDeclNode()) {
        if (importedVariables.find(name.Mutf8()) != importedVariables.end() ||
            importedVariables.find("") != importedVariables.end()) {
            return ValidateExport(varCallee);
        }
        return false;
    }
    return true;
}

bool ImportExportAccessValid::InvariantImportExportVariable(const std::unordered_set<std::string> &importedVariables,
                                                            const varbinder::Variable *var, const ir::Identifier *ident,
                                                            util::StringView name)
{
    if (!var->HasFlag(varbinder::VariableFlags::LOCAL) && !var->HasFlag(varbinder::VariableFlags::VAR) &&
        var->HasFlag(varbinder::VariableFlags::INITIALIZED) && var->Declaration() != nullptr &&
        var->Declaration()->Node() != nullptr && !var->Declaration()->Node()->IsMethodDefinition() &&
        !var->Declaration()->Node()->IsClassProperty()) {
        auto varParent = var->Declaration()->Node()->Parent();
        if (varParent != nullptr && !IsContainedIn(ident->Parent(), varParent) && ident->Parent() != varParent) {
            if (var->GetScope() != nullptr && var->GetScope()->Parent() != nullptr &&
                var->GetScope()->Parent()->IsGlobalScope() &&
                ident->GetTopStatement() == varParent->GetTopStatement()) {
                return true;
            }
            if (importedVariables.find(name.Mutf8()) != importedVariables.end() ||
                importedVariables.find("") != importedVariables.end()) {
                return ValidateExport(var);
            }
            return false;
        }
    }
    return true;
}

bool ImportExportAccessValid::HandleImportExportIdentifier(std::unordered_set<std::string> &importedVariables,
                                                           const ir::Identifier *ident, const ir::AstNode *callExpr)
{
    // We are running AST verifier only for ETS files so it is correct to pass ETS extension here
    if (ident->IsReference(ScriptExtension::ETS)) {
        const auto *var = ident->Variable();
        if (var != nullptr) {
            if (var->HasFlag(varbinder::VariableFlags::METHOD) && callExpr != nullptr) {
                return InvariantImportExportMethod(importedVariables, var, callExpr, ident->Name());
            }
            return InvariantImportExportVariable(importedVariables, var, ident, ident->Name());
        }
    }
    return true;
}

}  // namespace ark::es2panda::compiler::ast_verifier
