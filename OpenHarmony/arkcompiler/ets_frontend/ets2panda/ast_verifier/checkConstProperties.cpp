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

#include "checkConstProperties.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/memberExpression.h"
#include "checker/types/signature.h"
#include "ir/base/classProperty.h"

namespace ark::es2panda::compiler::ast_verifier {

[[nodiscard]] CheckResult CheckConstProperties::operator()(CheckContext &ctx, const ir::AstNode *ast) const
{
    if (ast->IsClassProperty()) {
        auto parent = ast->Parent();
        if (parent != nullptr && parent->IsClassDefinition() &&
            parent->AsClassDefinition()->Ident()->Name() == "ETSGLOBAL") {
            return {CheckDecision::CORRECT, CheckAction::CONTINUE};
        }
        auto property = ast->AsClassProperty();
        if (property->IsConst()) {
            ctx.AddCheckMessage("Class property cannot be const", *ast, ast->Start());
            return {CheckDecision::INCORRECT, CheckAction::CONTINUE};
        }
    }
    return {CheckDecision::CORRECT, CheckAction::CONTINUE};
}

}  // namespace ark::es2panda::compiler::ast_verifier
