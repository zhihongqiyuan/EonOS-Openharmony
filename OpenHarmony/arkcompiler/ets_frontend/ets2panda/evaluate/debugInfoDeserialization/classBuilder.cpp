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

#include "evaluate/debugInfoDeserialization/classBuilder.h"
#include "parser/program/program.h"
#include "checker/ETSchecker.h"
#include "ir/statements/classDeclaration.h"
#include "ir/expressions/identifier.h"
#include "libpandafile/class_data_accessor-inl.h"
#include "libpandafile/file-inl.h"
#include "libpandafile/method_data_accessor.h"
#include "evaluate/helpers.h"
#include "evaluate/debugInfoDeserialization/methodBuilder.h"

namespace ark::es2panda::evaluate {

ClassBuilder::ClassBuilder(checker::ETSChecker *checker, util::StringView name, panda_file::ClassDataAccessor &cda,
                           ir::Expression *superClass)
    : checker_(checker), className_(name), cda_(cda), superClass_(superClass)
{
    classModifiers_ = ir::ClassDefinitionModifiers::ID_REQUIRED | ir::ClassDefinitionModifiers::CLASS_DECL |
                      ir::ClassDefinitionModifiers::DECLARATION;
    if (superClass_ != nullptr) {
        classModifiers_ |= ir::ClassDefinitionModifiers::HAS_SUPER;
    }

    modifierFlags_ = helpers::GetModifierFlags(cda);
}

ir::ClassDeclaration *ClassBuilder::Build(parser::Program *program) &&
{
    auto *allocator = checker_->Allocator();
    ArenaVector<ir::AstNode *> classBody(allocator->Adapter());

    BuildFields(classBody);
    BuildMethods(classBody);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classId = checker_->AllocNode<ir::Identifier>(className_, allocator);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classDef = checker_->AllocNode<ir::ClassDefinition>(
        util::StringView(), classId, nullptr, nullptr, ArenaVector<ir::TSClassImplements *>(allocator->Adapter()),
        nullptr, superClass_, ArenaVector<ir::AstNode *>(allocator->Adapter()), classModifiers_, modifierFlags_,
        ToLanguage(program->Extension()));

    // Set parents for all class'es fields and methods.
    classDef->AddProperties(std::move(classBody));

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classDecl = checker_->AllocNode<ir::ClassDeclaration>(classDef, allocator);
    // Declare everything as exported for correct import resolution.
    classDecl->AddModifier(ir::ModifierFlags::EXPORT);

    auto *programAst = program->Ast();
    classDecl->SetParent(programAst);
    // Here we assume that global statements of the passed `program` are not currently checked, so that
    // insertion is safe.
    programAst->Statements().push_back(classDecl);

    return classDecl;
}

void ClassBuilder::BuildFields(ArenaVector<ir::AstNode *> &classBody)
{
    const auto &pf = cda_.GetPandaFile();

    cda_.EnumerateFields([&](panda_file::FieldDataAccessor &fda) -> void {
        const char *name = utf::Mutf8AsCString(pf.GetStringData(fda.GetNameId()).data);

        auto *typeNode = helpers::PandaTypeToTypeNode(pf, fda, checker_);
        ASSERT(typeNode);

        auto *field = helpers::CreateClassProperty(checker_, name, typeNode, helpers::GetModifierFlags(fda, true));
        classBody.push_back(field);
    });
}

void ClassBuilder::BuildMethods(ArenaVector<ir::AstNode *> &classBody)
{
    cda_.EnumerateMethods([&](panda_file::MethodDataAccessor &mda) -> void {
        auto *method = MethodBuilder(checker_, mda, modifierFlags_).Build();
        classBody.push_back(method);
    });
}

}  // namespace ark::es2panda::evaluate
