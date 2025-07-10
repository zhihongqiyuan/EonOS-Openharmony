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

#ifndef ES2PANDA_PARSER_PROGRAM_ENTITY_NAME_VISITOR_H
#define ES2PANDA_PARSER_PROGRAM_ENTITY_NAME_VISITOR_H

#include "libpandabase/mem/arena_allocator.h"
#include "macros.h"
#include "ir/visitor/IterateAstVisitor.h"
#include "util/ustring.h"

namespace ark::es2panda::parser {
class EntityNameVisitor final : public ir::visitor::AbortAstVisitor {
public:
    void VisitClassDeclaration(ir::ClassDeclaration *classDecl) override;
    void VisitFunctionDeclaration(ir::FunctionDeclaration *funcDecl) override;
    void VisitVariableDeclaration(ir::VariableDeclaration *varDecl) override;
    void VisitTSEnumDeclaration(ir::TSEnumDeclaration *enumDecl) override;
    void VisitTSInterfaceDeclaration(ir::TSInterfaceDeclaration *interfaceDecl) override;
    void VisitETSStructDeclaration(ir::ETSStructDeclaration *structDecl) override;
    void VisitTSTypeAliasDeclaration(ir::TSTypeAliasDeclaration *typeAliasDecl) override;

    util::StringView GetName() const
    {
        return name_;
    }

private:
    util::StringView name_;
};
}  // namespace ark::es2panda::parser

#endif /* ES2PANDA_PARSER_PROGRAM_ENTITY_NAME_VISITOR_H */
