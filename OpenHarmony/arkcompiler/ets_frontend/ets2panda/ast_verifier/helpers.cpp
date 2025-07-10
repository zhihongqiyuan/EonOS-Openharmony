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

#include "helpers.h"

#include "checker/types/typeFlag.h"
#include "checker/types/type.h"
#include "checker/types/ets/etsObjectType.h"
#include "ir/statements/blockStatement.h"
#include "ir/ets/etsScript.h"
#include "parser/program/program.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/callExpression.h"

namespace ark::es2panda::compiler::ast_verifier {

bool IsImportLike(const ir::AstNode *ast)
{
    return (ast->IsETSImportDeclaration() || ast->IsETSReExportDeclaration() || ast->IsImportExpression() ||
            ast->IsImportSpecifier() || ast->IsImportDefaultSpecifier() || ast->IsImportNamespaceSpecifier());
}

bool IsExportLike(const ir::AstNode *ast)
{
    return (ast->IsExportDefaultDeclaration() || ast->IsExportSpecifier() || ast->IsExportAllDeclaration() ||
            ast->IsExportNamedDeclaration() || ast->IsETSReExportDeclaration());
}

bool IsBooleanType(const ir::AstNode *ast)
{
    if (ast == nullptr) {
        return false;
    }

    if (!ast->IsTyped()) {
        return false;
    }

    auto typedAst = static_cast<const ir::TypedAstNode *>(ast);

    if (typedAst->TsType() == nullptr) {
        return false;
    }

    if (typedAst->TsType()->HasTypeFlag(checker::TypeFlag::ETS_OBJECT) &&
        ast->HasBoxingUnboxingFlags(ir::BoxingUnboxingFlags::UNBOXING_FLAG)) {
        return typedAst->TsType()->AsETSObjectType()->HasObjectFlag(checker::ETSObjectFlags::BUILTIN_BOOLEAN);
    }

    return typedAst->TsType()->HasTypeFlag(checker::TypeFlag::ETS_BOOLEAN) ||
           typedAst->TsType()->HasTypeFlag(checker::TypeFlag::BOOLEAN_LIKE);
}

bool IsValidTypeForBinaryOp(const ir::AstNode *ast, bool isBitwise)
{
    if (ast == nullptr) {
        std::cout << __LINE__ << std::endl;
        return false;
    }

    if (!ast->IsTyped()) {
        std::cout << __LINE__ << std::endl;
        return false;
    }

    auto typedAst = static_cast<const ir::TypedAstNode *>(ast);

    if (typedAst->TsType() == nullptr) {
        // std::cout << typedAst
        std::cout << __LINE__ << std::endl;
        return false;
    }

    if (IsBooleanType(ast)) {
        return isBitwise;
    }

    if (typedAst->TsType()->HasTypeFlag(checker::TypeFlag::ETS_OBJECT) &&
        typedAst->TsType()->AsETSObjectType()->HasObjectFlag(checker::ETSObjectFlags::BUILTIN_BIGINT)) {
        return true;
    }

    if (typedAst->TsType()->HasTypeFlag(checker::TypeFlag::ETS_OBJECT) &&
        ast->HasBoxingUnboxingFlags(ir::BoxingUnboxingFlags::UNBOXING_FLAG)) {
        return typedAst->TsType()->AsETSObjectType()->HasObjectFlag(checker::ETSObjectFlags::BUILTIN_TYPE) &&
               !typedAst->TsType()->AsETSObjectType()->HasObjectFlag(checker::ETSObjectFlags::BUILTIN_BOOLEAN);
    }

    return typedAst->TsType()->HasTypeFlag(checker::TypeFlag::ETS_CONVERTIBLE_TO_NUMERIC) ||
           typedAst->TsType()->HasTypeFlag(checker::TypeFlag::NUMBER_LITERAL) ||
           typedAst->TsType()->HasTypeFlag(checker::TypeFlag::BIGINT) ||
           typedAst->TsType()->HasTypeFlag(checker::TypeFlag::BIGINT_LITERAL);
}

bool IsStringType(const ir::AstNode *ast)
{
    if (ast == nullptr) {
        return false;
    }

    if (!ast->IsTyped()) {
        return false;
    }

    auto typedAst = static_cast<const ir::TypedAstNode *>(ast);

    if (typedAst->TsType() == nullptr) {
        return false;
    }

    if (typedAst->TsType()->HasTypeFlag(checker::TypeFlag::ETS_OBJECT)) {
        return typedAst->TsType()->AsETSObjectType()->HasObjectFlag(checker::ETSObjectFlags::STRING) ||
               typedAst->TsType()->AsETSObjectType()->HasObjectFlag(checker::ETSObjectFlags::BUILTIN_STRING);
    }

    return typedAst->TsType()->HasTypeFlag(checker::TypeFlag::STRING_LIKE);
}

bool IsVisibleInternalNode(const ir::AstNode *ast, const ir::AstNode *objTypeDeclNode)
{
    // NOTE(orlovskymaxim) This relies on the fact, that GetTopStatement has no bugs, that is not the case for now
    if (!ast->GetTopStatement()->IsETSScript()) {
        return false;
    }
    auto *currentTopStatement = (static_cast<const ir::ETSScript *>(ast->GetTopStatement()));
    auto *currentProgram = currentTopStatement->Program();
    if (currentProgram == nullptr) {
        return false;
    }
    util::StringView moduleNameCurrent = currentProgram->ModuleName();
    // NOTE(orlovskymaxim) This relies on the fact, that GetTopStatement has no bugs, that is not the case for now
    if (!objTypeDeclNode->GetTopStatement()->IsETSScript()) {
        return false;
    }
    auto *objectTopStatement = (static_cast<const ir::ETSScript *>(objTypeDeclNode->GetTopStatement()));
    auto *objectProgram = objectTopStatement->Program();
    if (objectProgram == nullptr) {
        return false;
    }
    util::StringView moduleNameObject = objectProgram->ModuleName();
    return currentTopStatement == objectTopStatement || moduleNameCurrent == moduleNameObject;
}

const checker::Type *GetClassDefinitionType(const ir::AstNode *ast)
{
    const ir::AstNode *tmpNode = ast;
    while (tmpNode->Parent() != nullptr && !tmpNode->IsClassDefinition()) {
        tmpNode = tmpNode->Parent();
    }
    if (!tmpNode->IsClassDefinition()) {
        return nullptr;
    }
    auto *classDefinition = tmpNode->AsClassDefinition();
    return classDefinition->TsType();
}

const checker::Type *GetTSInterfaceDeclarationType(const ir::AstNode *ast)
{
    const ir::AstNode *tmpNode = ast;
    while (tmpNode->Parent() != nullptr && !tmpNode->IsTSInterfaceDeclaration()) {
        tmpNode = tmpNode->Parent();
    }
    if (!tmpNode->IsTSInterfaceDeclaration()) {
        return nullptr;
    }
    auto *tsInterfaceDeclaration = tmpNode->AsTSInterfaceDeclaration();
    return tsInterfaceDeclaration->TsType();
}

bool ValidateMethodAccessForClass(const ir::AstNode *ast, const ir::AstNode *ownerSignDeclNode,
                                  checker::Signature *signature, const ir::AstNode *memberObjTypeDeclNode)
{
    // Check if the method is used where it is declared
    if (IsContainedIn<const ir::AstNode>(ast, ownerSignDeclNode)) {
        return true;
    }
    if (signature->HasSignatureFlag(checker::SignatureFlags::PRIVATE)) {
        return false;
    }
    if (signature->HasSignatureFlag(checker::SignatureFlags::PROTECTED)) {
        // Check if the method is inherited and is used in class in which it is inherited
        auto *classDefinitionType = GetClassDefinitionType(ast);
        if (classDefinitionType == nullptr || !classDefinitionType->IsETSObjectType()) {
            return false;
        }
        auto *classObjectType = classDefinitionType->AsETSObjectType();
        return classObjectType->IsDescendantOf(signature->Owner());
    }
    if (signature->HasSignatureFlag(checker::SignatureFlags::INTERNAL)) {
        return IsVisibleInternalNode(ast, memberObjTypeDeclNode);
    }
    return true;
}

bool ValidateMethodAccessForTSInterface(const ir::AstNode *ast, const ir::AstNode *ownerSignDeclNode,
                                        checker::Signature *signature, const ir::AstNode *memberObjTypeDeclNode)
{
    // Check if the method is used where it is declared
    if (IsContainedIn<const ir::AstNode>(ast, ownerSignDeclNode)) {
        return true;
    }
    if (signature->HasSignatureFlag(checker::SignatureFlags::PRIVATE)) {
        return false;
    }
    if (signature->HasSignatureFlag(checker::SignatureFlags::PROTECTED)) {
        // Check if the method is inherited and is used in class in which it is inherited
        auto *tsInterfaceDeclarationType = GetTSInterfaceDeclarationType(ast);
        if (tsInterfaceDeclarationType == nullptr || !tsInterfaceDeclarationType->IsETSObjectType()) {
            return false;
        }
        auto *tsInterfaceObjectType = tsInterfaceDeclarationType->AsETSObjectType();
        return tsInterfaceObjectType->IsDescendantOf(signature->Owner());
    }
    if (signature->HasSignatureFlag(checker::SignatureFlags::INTERNAL)) {
        return IsVisibleInternalNode(ast, memberObjTypeDeclNode);
    }
    return true;
}

bool ValidatePropertyAccessForClass(const ir::AstNode *ast, const ir::AstNode *propVarDeclNode,
                                    const ir::AstNode *propVarDeclNodeParent, const varbinder::LocalVariable *propVar,
                                    const ir::AstNode *objTypeDeclNode)
{
    // Check if the variable is used where it is declared
    if (IsContainedIn<const ir::AstNode>(ast, propVarDeclNodeParent)) {
        return true;
    }
    if (propVarDeclNode->IsPrivate()) {
        return false;
    }
    if (propVarDeclNode->IsProtected()) {
        auto *classDefinitionType = GetClassDefinitionType(ast);
        if (classDefinitionType != nullptr && classDefinitionType->IsETSObjectType()) {
            auto *classObjectType = classDefinitionType->AsETSObjectType();
            return classObjectType->IsPropertyOfAscendant(propVar);
        }
        auto *interfaceDefType = GetTSInterfaceDeclarationType(ast);
        if (interfaceDefType != nullptr && interfaceDefType->IsETSObjectType()) {
            auto *interfaceObjectType = interfaceDefType->AsETSObjectType();
            return interfaceObjectType->IsPropertyOfAscendant(propVar);
        }
        return false;
    }
    if (propVarDeclNode->IsInternal()) {
        return IsVisibleInternalNode(ast, objTypeDeclNode);
    }
    return true;
}

bool ValidateVariableAccess(const varbinder::LocalVariable *propVar, const ir::MemberExpression *ast)
{
    const auto *propVarDecl = propVar->Declaration();
    if (propVarDecl == nullptr) {
        return false;
    }
    const auto *propVarDeclNode = propVarDecl->Node();
    if (propVarDeclNode == nullptr) {
        return false;
    }
    auto *objType = ast->ObjType();
    if (objType == nullptr) {
        return false;
    }
    const auto *objTypeDeclNode = objType->GetDeclNode();
    if (objTypeDeclNode == nullptr) {
        return false;
    }
    if (objTypeDeclNode->Parent() != nullptr && objTypeDeclNode->Parent()->IsImportNamespaceSpecifier()) {
        return true;
    }
    const auto *propVarDeclNodeParent = propVarDeclNode->Parent();
    if (propVarDeclNodeParent == nullptr) {
        return false;
    }
    if ((propVarDeclNodeParent->IsClassDefinition() && objTypeDeclNode->IsClassDefinition()) ||
        (propVarDeclNodeParent->IsTSInterfaceDeclaration() && objTypeDeclNode->IsTSInterfaceDeclaration())) {
        return ValidatePropertyAccessForClass(ast, propVarDeclNode, propVarDeclNodeParent, propVar, objTypeDeclNode);
    }
    return false;
}

bool ValidateMethodAccess(const ir::MemberExpression *memberExpression, const ir::CallExpression *ast)
{
    if (memberExpression->Object()->TsType() != nullptr) {
        // When calling enum methods member expression
        // object has ETSEnumType instead of ETSObjectType.
        const auto *const type = memberExpression->Object()->TsType();
        if (type->IsETSEnumType()) {
            return true;
        }
    }

    auto *memberObjType = memberExpression->ObjType();
    if (memberObjType == nullptr) {
        return false;
    }
    if (memberObjType->HasObjectFlag(checker::ETSObjectFlags::RESOLVED_SUPER) &&
        memberObjType->SuperType() != nullptr &&
        memberObjType->SuperType()->HasObjectFlag(checker::ETSObjectFlags::BUILTIN_TYPE |
                                                  checker::ETSObjectFlags::GLOBAL)) {
        return true;
    }
    const auto *memberObjTypeDeclNode = memberObjType->GetDeclNode();
    if (memberObjTypeDeclNode == nullptr) {
        return false;
    }
    if (memberObjTypeDeclNode->Parent() != nullptr && memberObjTypeDeclNode->Parent()->IsImportNamespaceSpecifier()) {
        return true;
    }
    auto *signature = ast->Signature();
    if (signature == nullptr) {
        return false;
    }
    auto *ownerSign = signature->Owner();
    if (ownerSign == nullptr) {
        return false;
    }
    auto *ownerSignDeclNode = ownerSign->GetDeclNode();
    if (ownerSignDeclNode == nullptr) {
        return false;
    }
    if (!ownerSignDeclNode->IsClassDefinition() && !ownerSignDeclNode->IsTSInterfaceDeclaration()) {
        return false;
    }
    bool ret = false;
    if (memberObjTypeDeclNode->IsClassDefinition()) {
        ret = ValidateMethodAccessForClass(ast, ownerSignDeclNode, signature, memberObjTypeDeclNode);
    } else if (memberObjTypeDeclNode->IsTSInterfaceDeclaration()) {
        ret = ValidateMethodAccessForTSInterface(ast, ownerSignDeclNode, signature, memberObjTypeDeclNode);
    }
    return ret;
}

}  // namespace ark::es2panda::compiler::ast_verifier
