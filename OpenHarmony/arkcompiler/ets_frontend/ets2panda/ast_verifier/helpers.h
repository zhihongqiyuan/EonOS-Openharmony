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

#ifndef ES2PANDA_COMPILER_CORE_AST_VERIFIER_HELPERS_H
#define ES2PANDA_COMPILER_CORE_AST_VERIFIER_HELPERS_H

#include "ir/astNode.h"
#include "checker/types/signature.h"

namespace ark::es2panda::compiler::ast_verifier {

bool IsImportLike(const ir::AstNode *ast);
bool IsExportLike(const ir::AstNode *ast);
bool IsBooleanType(const ir::AstNode *ast);
bool IsValidTypeForBinaryOp(const ir::AstNode *ast, bool isBitwise);
bool IsStringType(const ir::AstNode *ast);
bool IsVisibleInternalNode(const ir::AstNode *ast, const ir::AstNode *objTypeDeclNode);
const checker::Type *GetClassDefinitionType(const ir::AstNode *ast);
const checker::Type *GetTSInterfaceDeclarationType(const ir::AstNode *ast);
bool ValidateMethodAccessForClass(const ir::AstNode *ast, const ir::AstNode *ownerSignDeclNode,
                                  checker::Signature *signature, const ir::AstNode *memberObjTypeDeclNode);
bool ValidateMethodAccessForTSInterface(const ir::AstNode *ast, const ir::AstNode *ownerSignDeclNode,
                                        checker::Signature *signature, const ir::AstNode *memberObjTypeDeclNode);
bool ValidatePropertyAccessForClass(const ir::AstNode *ast, const ir::AstNode *propVarDeclNode,
                                    const ir::AstNode *propVarDeclNodeParent, const varbinder::LocalVariable *propVar,
                                    const ir::AstNode *objTypeDeclNode);
bool ValidateVariableAccess(const varbinder::LocalVariable *propVar, const ir::MemberExpression *ast);
bool ValidateMethodAccess(const ir::MemberExpression *memberExpression, const ir::CallExpression *ast);

template <typename T>
bool IsContainedIn(const T *child, const T *parent)
{
    if (child == nullptr || parent == nullptr) {
        return false;
    }

    std::unordered_set<const T *> savedNodes;
    while (child != nullptr && child != parent) {
        savedNodes.emplace(child);
        child = child->Parent();
        if (savedNodes.find(child) != savedNodes.end()) {
            return false;
        }
    }
    return child == parent;
}

}  // namespace ark::es2panda::compiler::ast_verifier

#endif  // ES2PANDA_COMPILER_CORE_AST_VERIFIER_HELPERS_H
