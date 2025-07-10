/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include "ASTVerifier.h"
#include "ast_verifier/sequenceExpressionHasLastType.h"
#include "ast_verifier/checkAbstractMethod.h"
#include "ast_verifier/checkInfiniteLoop.h"
#include "ast_verifier/checkContext.h"
#include "ast_verifier/everyChildHasValidParent.h"
#include "ast_verifier/everyChildInParentRange.h"
#include "ast_verifier/getterSetterValidation.h"
#include "ast_verifier/identifierHasVariable.h"
#include "ast_verifier/nodeHasParent.h"
#include "ast_verifier/nodeHasSourceRange.h"
#include "ast_verifier/nodeHasType.h"
#include "ast_verifier/referenceTypeAnnotationIsNull.h"
#include "ast_verifier/variableHasScope.h"
#include "ast_verifier/variableHasEnclosingScope.h"
#include "ast_verifier/forLoopCorrectlyInitialized.h"
#include "ast_verifier/modifierAccessValid.h"
#include "ast_verifier/importExportAccessValid.h"
#include "ast_verifier/arithmeticOperationValid.h"
#include "ast_verifier/variableNameIdentifierNameSame.h"
#include "ast_verifier/checkScopeDeclaration.h"
#include "ast_verifier/checkConstProperties.h"

namespace ark::es2panda::compiler::ast_verifier {

ASTVerifier::ASTVerifier(ArenaAllocator *allocator)
{
    AddInvariant<NodeHasParent>(allocator, "NodeHasParent");
    AddInvariant<NodeHasSourceRange>(allocator, "NodeHasSourceRange");
    AddInvariant<NodeHasType>(allocator, "NodeHasType");
    AddInvariant<IdentifierHasVariable>(allocator, "IdentifierHasVariable");
    AddInvariant<VariableHasScope>(allocator, "VariableHasScope");
    AddInvariant<EveryChildHasValidParent>(allocator, "EveryChildHasValidParent");
    AddInvariant<EveryChildInParentRange>(allocator, "EveryChildInParentRange");
    AddInvariant<VariableHasEnclosingScope>(allocator, "VariableHasEnclosingScope");
    AddInvariant<CheckInfiniteLoop>(allocator, "CheckInfiniteLoop");
    AddInvariant<ForLoopCorrectlyInitialized>(allocator, "ForLoopCorrectlyInitialized");
    AddInvariant<ModifierAccessValid>(allocator, "ModifierAccessValid");
    AddInvariant<ImportExportAccessValid>(allocator, "ImportExportAccessValid");
    AddInvariant<ArithmeticOperationValid>(allocator, "ArithmeticOperationValid");
    AddInvariant<SequenceExpressionHasLastType>(allocator, "SequenceExpressionHasLastType");
    AddInvariant<ReferenceTypeAnnotationIsNull>(allocator, "ReferenceTypeAnnotationIsNull");
    AddInvariant<VariableNameIdentifierNameSame>(allocator, "VariableNameIdentifierNameSame");
    AddInvariant<CheckAbstractMethod>(allocator, "CheckAbstractMethod");
    AddInvariant<GetterSetterValidation>(allocator, "GetterSetterValidation");
    AddInvariant<CheckScopeDeclaration>(allocator, "CheckScopeDeclaration");
    AddInvariant<CheckConstProperties>(allocator, "CheckConstProperties");
}

Messages ASTVerifier::VerifyFull(const ir::AstNode *ast)
{
    auto recursiveChecks = InvariantNameSet {};
    std::copy_if(invariantsNames_.begin(), invariantsNames_.end(),
                 std::inserter(recursiveChecks, recursiveChecks.end()),
                 [](const std::string &s) { return s.find(RECURSIVE_SUFFIX) != s.npos; });
    return Verify(ast, recursiveChecks);
}

Messages ASTVerifier::Verify(const ir::AstNode *ast, const InvariantNameSet &invariantSet)
{
    CheckContext ctx {};
    const auto containsInvariants =
        std::includes(invariantsNames_.begin(), invariantsNames_.end(), invariantSet.begin(), invariantSet.end());
    if (!containsInvariants) {
        auto invalidInvariants = InvariantNameSet {};
        for (const auto &invariant : invariantSet) {
            if (invariantsNames_.find(invariant) == invariantsNames_.end()) {
                invalidInvariants.insert(invariant);
            }
        }
        for (const auto &invariant : invalidInvariants) {
            ctx.AddCheckMessage(std::string {"Invariant was not found: "} + invariant, *ast, lexer::SourcePosition {});
        }
    }

    for (const auto &name : invariantSet) {
        if (const auto &found = invariantsChecks_.find(name); found != invariantsChecks_.end()) {
            if (ast == nullptr) {
                continue;
            }

            auto invariant = found->second;
            ctx.SetCheckName(name.data());
            invariant(ctx, ast);
        }
    }

    return ctx.GetMessages();
}

}  // namespace ark::es2panda::compiler::ast_verifier
