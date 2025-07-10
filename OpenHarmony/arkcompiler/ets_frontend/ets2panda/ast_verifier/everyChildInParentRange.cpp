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

#include "everyChildInParentRange.h"

namespace ark::es2panda::compiler::ast_verifier {

static bool IsInParentsRange(const ir::AstNode *ast, ir::AstNode const *node)
{
    if (ast != node->Parent()) {
        return false;
    }
    if (ast->Start().line > node->Start().line || ast->End().line < node->End().line) {
        return false;
    }
    if (ast->Start().line == node->Start().line && ast->Start().index > node->Start().index) {
        return false;
    }
    if (ast->End().line == node->End().line && ast->End().index < node->End().index) {
        return false;
    }
    return true;
}

CheckResult EveryChildInParentRange::operator()(CheckContext &ctx, const ir::AstNode *ast)
{
    auto result = std::make_tuple(CheckDecision::CORRECT, CheckAction::CONTINUE);
    if (ast->Parent() == nullptr) {
        return result;
    }
    ast->Iterate([&](const ir::AstNode *node) {
        if (!IsInParentsRange(ast, node)) {
            ctx.AddCheckMessage("INCORRECT_CHILD_RANGE", *node, node->Start());
            result = {CheckDecision::INCORRECT, CheckAction::CONTINUE};
        }
    });
    return result;
}

}  // namespace ark::es2panda::compiler::ast_verifier
