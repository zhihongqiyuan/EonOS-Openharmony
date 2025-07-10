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

#include "getterSetterValidation.h"
#include "ir/expression.h"
#include "ir/expressions/functionExpression.h"
#include "ir/typeNode.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/scriptFunction.h"

namespace ark::es2panda::compiler::ast_verifier {

[[nodiscard]] CheckResult GetterSetterValidation::operator()(CheckContext &ctx, const ir::AstNode *ast)
{
    if (!ast->IsMethodDefinition()) {
        return {CheckDecision::CORRECT, CheckAction::CONTINUE};
    }

    bool errorFound = false;
    auto const validateMethod = [&ctx, &errorFound, this](ir::MethodDefinition const *const method) {
        auto const kind = method->Kind();
        if (kind == ir::MethodDefinitionKind::GET) {
            errorFound |= !ValidateGetter(ctx, method);
        } else if (kind == ir::MethodDefinitionKind::SET) {
            errorFound |= !ValidateSetter(ctx, method);
        };
    };

    auto const *const method = ast->AsMethodDefinition();
    validateMethod(method);
    for (auto const *const overload : method->Overloads()) {
        validateMethod(overload);
    }

    if (errorFound) {
        return {CheckDecision::INCORRECT, CheckAction::CONTINUE};
    }

    return {CheckDecision::CORRECT, CheckAction::CONTINUE};
}

bool GetterSetterValidation::ValidateGetter(CheckContext &ctx, ir::MethodDefinition const *const method) const
{
    if (!method->Value()->IsFunctionExpression()) {
        return true;
    }

    bool result = true;
    auto const report = [&ctx, &result, method](const std::string &msg) {
        ctx.AddCheckMessage(msg, *method, method->Start());
        result = false;
    };

    auto const *const function = method->Value()->AsFunctionExpression()->Function();

    // Check getter flag
    if (!function->IsGetter()) {
        report("GETTER METHOD DOES NOT HAVE GETTER FLAG");
    }

    // Check return type annotation if it exists
    if (function->ReturnTypeAnnotation() != nullptr) {
        auto const *const type = function->ReturnTypeAnnotation()->TsType();
        if (type != nullptr && type->IsETSVoidType()) {
            report("GETTER METHOD HAS VOID RETURN TYPE IN RETURN TYPE ANNOTATION");
        }
    }

    // For non-abstract and non-ambient getters return statement should always exist
    if (!function->HasReturnStatement() && !function->HasThrowStatement() && !function->IsAbstract() &&
        !function->IsDeclare()) {
        report("MISSING RETURN STATEMENT IN GETTER METHOD");
    }

    // Check return statements
    auto const &returns = function->ReturnStatements();
    if (function->ReturnTypeAnnotation() == nullptr) {
        if (returns.empty()) {
            report("MISSING RETURN TYPE ANNOTATION AND RETURN STATEMENT IN GETTER METHOD");
        }
    }

    // Check that all return statements are not void
    for (auto const *const stmt : returns) {
        if (stmt->ReturnType()->IsETSVoidType()) {
            // All getters should have non-void return type
            report("GETTER METHOD HAS VOID RETURN TYPE");
        }
    }

    // Check number of arguments
    auto const &params = function->Params();
    if (!params.empty()) {
        report("GETTER METHOD HAS INCORRECT NUMBER OF ARGUMENTS");
    }

    return result;
}

bool GetterSetterValidation::ValidateSetter(CheckContext &ctx, ir::MethodDefinition const *const method) const
{
    if (!method->Value()->IsFunctionExpression()) {
        return true;
    }

    bool result = true;
    auto const report = [&ctx, &result, method](const std::string &msg) {
        ctx.AddCheckMessage(msg, *method, method->Start());
        result = false;
    };

    auto const *const function = method->Value()->AsFunctionExpression()->Function();

    // Check setter flag
    if (!function->IsSetter()) {
        report("SETTER METHOD DOES NOT HAVE SETTER FLAG");
    }

    // Check return type annotation
    if (function->ReturnTypeAnnotation() != nullptr) {
        auto const *const type = function->ReturnTypeAnnotation()->TsType();
        if (type != nullptr && !type->IsETSVoidType()) {
            report("SETTER METHOD HAS NON-VOID RETURN TYPE");
        }
    }

    // Check number of arguments
    auto const &params = function->Params();
    if (params.size() != 1) {
        report("SETTER METHOD HAS INCORRECT NUMBER OF ARGUMENTS");
    }

    return result;
}

}  // namespace ark::es2panda::compiler::ast_verifier
