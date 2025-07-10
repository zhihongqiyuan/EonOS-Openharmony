/**
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

#include "entityNameVisitor.h"

namespace ark::es2panda::parser {

void EntityNameVisitor::VisitClassDeclaration(ir::ClassDeclaration *classDecl)
{
    name_ = classDecl->AsClassDeclaration()->Definition()->Ident()->Name();
}

void EntityNameVisitor::VisitFunctionDeclaration(ir::FunctionDeclaration *funcDecl)
{
    name_ = funcDecl->AsFunctionDeclaration()->Function()->Id()->Name();
}

void EntityNameVisitor::VisitVariableDeclaration(ir::VariableDeclaration *varDecl)
{
    const auto &decls = varDecl->AsVariableDeclaration()->Declarators();
    ASSERT(decls.size() == 1);
    auto *id = decls[0]->Id();
    ASSERT(id->IsIdentifier());
    name_ = id->AsIdentifier()->Name();
}

void EntityNameVisitor::VisitTSEnumDeclaration(ir::TSEnumDeclaration *enumDecl)
{
    name_ = enumDecl->AsTSEnumDeclaration()->Key()->Name();
}

void EntityNameVisitor::VisitTSInterfaceDeclaration(ir::TSInterfaceDeclaration *interfaceDecl)
{
    name_ = interfaceDecl->AsTSInterfaceDeclaration()->Id()->Name();
}

void EntityNameVisitor::VisitETSStructDeclaration(ir::ETSStructDeclaration *structDecl)
{
    name_ = structDecl->AsETSStructDeclaration()->Definition()->Ident()->Name();
}

void EntityNameVisitor::VisitTSTypeAliasDeclaration(ir::TSTypeAliasDeclaration *typeAliasDecl)
{
    name_ = typeAliasDecl->AsTSTypeAliasDeclaration()->Id()->Name();
}

}  // namespace ark::es2panda::parser
