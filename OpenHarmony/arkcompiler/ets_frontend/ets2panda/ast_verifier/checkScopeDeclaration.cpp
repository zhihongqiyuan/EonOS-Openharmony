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

#include "checkScopeDeclaration.h"
#include "ir/expressions/identifier.h"
#include "ir/base/classDefinition.h"
#include "varbinder/scope.h"

namespace ark::es2panda::compiler::ast_verifier {

CheckResult CheckScopeDeclaration::operator()(CheckContext &ctx, const ir::AstNode *ast) const
{
    if (!ast->IsIdentifier()) {
        return {CheckDecision::CORRECT, CheckAction::CONTINUE};
    }

    auto const *const id = ast->AsIdentifier();
    auto const *const var = id->Variable();
    if (var == nullptr) {
        // This is checked in IdentifierHasVariable
        return {CheckDecision::CORRECT, CheckAction::CONTINUE};
    }

    auto const *const scope = var->GetScope();
    if (scope == nullptr) {
        // This is checked in VariableHasScope
        return {CheckDecision::CORRECT, CheckAction::CONTINUE};
    }

    return CheckScope(ctx, scope);
}

CheckResult CheckScopeDeclaration::CheckScope(CheckContext &ctx, varbinder::Scope const *const scope) const
{
    auto const *const node = scope->Node();
    if (node == nullptr) {
        // This is checked in VariableHasEnclosingScope
        return {CheckDecision::CORRECT, CheckAction::CONTINUE};
    }

    if (!node->IsScopeBearer()) {
        ctx.AddCheckMessage("NODE IS NOT SCOPE BEARER", *node, node->Start());
        return {CheckDecision::INCORRECT, CheckAction::CONTINUE};
    }

    if (node->Scope() == scope) {
        return {CheckDecision::CORRECT, CheckAction::CONTINUE};
    }

    if (node->Scope()->IsFunctionScope()) {
        auto const *const functionScope = node->Scope()->AsFunctionScope();
        if (functionScope->ParamScope() == scope) {
            return {CheckDecision::CORRECT, CheckAction::CONTINUE};
        }
    }

    // NOTE(kkonkuznetsov): cannot check with IsLoopScope
    // Loop Scope has type Local, using flag instead
    if (node->Scope()->HasFlag(varbinder::ScopeFlags::LOOP_SCOPE)) {
        // NOTE(kkonkuznetsov): cannot use AsLoopScope here due to assert failure with IsLoopScope
        auto *const loopScope = reinterpret_cast<varbinder::LoopScope *>(node->Scope());
        if (loopScope->DeclScope() == scope) {
            return {CheckDecision::CORRECT, CheckAction::CONTINUE};
        }
    }

    if (node->Scope()->IsCatchScope()) {
        auto *const catchScope = node->Scope()->AsCatchScope();
        if (catchScope->ParamScope() == scope) {
            return {CheckDecision::CORRECT, CheckAction::CONTINUE};
        }
    }

    ctx.AddCheckMessage("SCOPE IS NOT CONSISTENT WITH ITS DECLARING NODE", *node, node->Start());
    return {CheckDecision::INCORRECT, CheckAction::CONTINUE};
}

}  // namespace ark::es2panda::compiler::ast_verifier
