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

#include "evaluate/debugInfoStorage.h"
#include "evaluate/helpers.h"
#include "evaluate/entityDeclarator.h"
#include "evaluate/scopedDebugInfoPlugin-inl.h"
#include "evaluate/debugInfoDeserialization/debugInfoDeserializer.h"
#include "evaluate/debugInfoDeserialization/classBuilder.h"
#include "evaluate/debugInfoDeserialization/methodBuilder.h"

#include "public/public.h"
#include "varbinder/variable.h"
#include "parser/ETSparser.h"
#include "parser/program/program.h"
#include "parser/context/parserContext.h"
#include "compiler/lowering/scopesInit/scopesInitPhase.h"
#include "compiler/lowering/util.h"

#include "libpandafile/class_data_accessor-inl.h"
#include "libpandafile/file-inl.h"

namespace ark::es2panda::evaluate {

namespace {

std::string GetVarDeclSourceCode(std::string_view varName, DebugInfoDeserializer::RegisterNumber regNumber,
                                 const std::string &typeSignature, panda_file::Type::TypeId typeId,
                                 checker::GlobalTypesHolder *globalTypes)
{
    auto returnType = helpers::ToTypeName(typeSignature, globalTypes);
    ASSERT(returnType.has_value());

    std::stringstream sstream;
    sstream << "let " << varName << ':' << *returnType << '=' << helpers::DEBUGGER_API_CLASS_NAME << '.'
            << helpers::CreateGetterName(typeId) << '(' << regNumber << ')';
    // Must add cast from Object.
    if (typeId == panda_file::Type::TypeId::REFERENCE) {
        sstream << " as " << *returnType;
    }
    return sstream.str();
}

std::string GetVarUpdateSourceCode(std::string_view varName, DebugInfoDeserializer::RegisterNumber regNumber,
                                   panda_file::Type::TypeId typeId)
{
    std::stringstream sstream;
    sstream << helpers::DEBUGGER_API_CLASS_NAME << '.' << helpers::CreateSetterName(typeId) << '(' << regNumber << ','
            << varName << ')';
    return sstream.str();
}

}  // namespace

DebugInfoDeserializer::DebugInfoDeserializer(ScopedDebugInfoPlugin &debugInfoPlugin) : debugInfoPlugin_(debugInfoPlugin)
{
}

varbinder::Variable *DebugInfoDeserializer::CreateIrClass(panda_file::File::EntityId classId, parser::Program *program,
                                                          util::StringView pathToSource, util::StringView classDeclName)
{
    LOG(DEBUG, ES2PANDA) << "DebugInfoDeserializer::CreateIrClass " << classDeclName << " in " << pathToSource;
    ASSERT(program);

    const auto *pf = debugInfoPlugin_.GetDebugInfoStorage()->GetPandaFile(pathToSource.Utf8());
    ASSERT(pf);

    // NOTE: may cache the created `ClassDataAccessor`.
    auto cda = panda_file::ClassDataAccessor(*pf, classId);

    auto *classDecl = CreateClassDeclaration(classDeclName, cda, GetSuperClass(cda), program);
    return classDecl->Definition()->Ident()->Variable();
}

ir::ClassDeclaration *DebugInfoDeserializer::CreateClassDeclaration(util::StringView identName,
                                                                    panda_file::ClassDataAccessor &cda,
                                                                    ir::ETSTypeReference *superClass,
                                                                    parser::Program *program)
{
    LOG(DEBUG, ES2PANDA) << "Create class declaration from debug info: " << identName;

    auto *checkHelper = debugInfoPlugin_.GetIrCheckHelper();
    auto *checker = checkHelper->GetChecker();
    auto *varBinder = debugInfoPlugin_.GetETSBinder();

    auto *classDecl = ClassBuilder(checker, identName, cda, superClass).Build(program);

    helpers::DoScopedAction(checker, varBinder, program, nullptr, nullptr, [varBinder, classDecl]() {
        compiler::InitScopesPhaseETS::RunExternalNode(classDecl, varBinder);
        varBinder->ResolveReferencesForScope(classDecl, compiler::NearestScope(classDecl));
    });
    // Run checker to assign types to all entities.
    checkHelper->CheckNewNode(classDecl, program->GlobalScope(), nullptr, program);

    return classDecl;
}

varbinder::Variable *DebugInfoDeserializer::CreateIrLocalVariable(
    ir::Identifier *ident, const panda_file::LocalVariableTable &localVariableTable, size_t bytecodeOffset)
{
    ASSERT(ident);

    auto typedVarIter = localVariableTable.end();
    uint32_t startOffset = 0;

    const auto &identName = ident->Name();
    for (auto iter = localVariableTable.begin(); iter != localVariableTable.end(); ++iter) {
        const auto &var = *iter;
        if (identName.Is(var.name) && var.IsAccessibleAt(bytecodeOffset) && startOffset <= var.startOffset) {
            typedVarIter = iter;
            startOffset = var.startOffset;
        }
    }
    if (typedVarIter != localVariableTable.end()) {
        return CreateLocalVarDecl(ident, typedVarIter->regNumber, typedVarIter->typeSignature);
    }

    return nullptr;
}

varbinder::Variable *DebugInfoDeserializer::CreateIrGlobalVariable(parser::Program *program,
                                                                   util::StringView pathToSource,
                                                                   util::StringView varDeclName)
{
    ASSERT(program);

    auto *debugInfoStorage = debugInfoPlugin_.GetDebugInfoStorage();
    auto *checkHelper = debugInfoPlugin_.GetIrCheckHelper();

    const auto *pf = debugInfoStorage->GetPandaFile(pathToSource.Utf8());
    ASSERT(pf);

    varbinder::Variable *var = nullptr;

    auto *cda = debugInfoStorage->GetGlobalClassAccessor(pathToSource.Utf8());
    cda->EnumerateFields([program, varDeclName, pf, &var, checkHelper](panda_file::FieldDataAccessor &fda) {
        // All ETSGLOBAL fields must be static.
        ASSERT(fda.IsStatic());

        const char *name = utf::Mutf8AsCString(pf->GetStringData(fda.GetNameId()).data);
        if (!varDeclName.Is(name)) {
            return;
        }
        // Must be unique within global variables.
        ASSERT(var == nullptr);

        auto *typeNode = helpers::PandaTypeToTypeNode(*pf, fda, checkHelper->GetChecker());
        ASSERT(typeNode);

        // Global variable is found - add it into source module's global class properties.
        auto modFlags = helpers::GetModifierFlags(fda, true) | ir::ModifierFlags::EXPORT;
        auto *field = helpers::CreateClassProperty(checkHelper->GetChecker(), name, typeNode, modFlags);
        // Fields parent will be set in `AddProperties`.
        program->GlobalClass()->AddProperties(
            ArenaVector<ir::AstNode *>(1, field, checkHelper->GetChecker()->Allocator()->Adapter()));

        checkHelper->CheckGlobalEntity(program, field);
        var = field->Key()->AsIdentifier()->Variable();
    });

    return var;
}

varbinder::Variable *DebugInfoDeserializer::CreateIrGlobalMethods(ArenaVector<ir::AstNode *> &createdMethods,
                                                                  parser::Program *program,
                                                                  util::StringView pathToSource,
                                                                  util::StringView methodDeclName)
{
    ASSERT(program);

    varbinder::Variable *var = nullptr;

    auto *cda = debugInfoPlugin_.GetDebugInfoStorage()->GetGlobalClassAccessor(pathToSource.Utf8());
    cda->EnumerateMethods([this, &var, &createdMethods, program, methodDeclName,
                           &cda](panda_file::MethodDataAccessor &mda) {
        if (!methodDeclName.Is(mda.GetFullName())) {
            return;
        }

        MethodBuilder builder(debugInfoPlugin_.GetIrCheckHelper()->GetChecker(), mda, helpers::GetModifierFlags(*cda));
        auto *method = std::move(builder).Build();

        method->SetParent(program->GlobalClass());
        createdMethods.emplace_back(method);

        // Postpone Checker until the whole overload set is created.
        debugInfoPlugin_.GetIrCheckHelper()->CheckGlobalEntity(program, method, false);

        // Sanity checks.
        auto *methodVar = method->AsClassElement()->Value()->AsFunctionExpression()->Function()->Id()->Variable();
        ASSERT(methodVar != nullptr);
        ASSERT(var == nullptr || var == methodVar);
        var = methodVar;
    });

    return var;
}

varbinder::Variable *DebugInfoDeserializer::CreateLocalVarDecl(ir::Identifier *ident, RegisterNumber regNumber,
                                                               const std::string &typeSignature)
{
    ASSERT(ident);

    auto *checkHelper = debugInfoPlugin_.GetIrCheckHelper();
    auto *varBinder = debugInfoPlugin_.GetETSBinder();
    auto identName = ident->Name().Utf8();

    auto typeId = helpers::GetTypeId(typeSignature);
    auto *checker = checkHelper->GetChecker();
    auto varDeclSource =
        GetVarDeclSourceCode(identName, regNumber, typeSignature, typeId, checker->GetGlobalTypesHolder());

    // Set up correct scope before parsing statements.
    auto *topStatement = helpers::GetEnclosingBlock(ident);
    checker::ScopeContext ctx(checker, topStatement->Scope());
    auto statementScope = varbinder::LexicalScope<varbinder::Scope>::Enter(varBinder, topStatement->Scope());

    parser::Program p(checker->Allocator(), varBinder);
    auto parser = parser::ETSParser(&p, varBinder->GetContext()->config->options->CompilerOptions(),
                                    parser::ParserStatus::NO_OPTS);

    auto *varDecl = parser.CreateFormattedStatement(varDeclSource, parser::ParserContext::DEFAULT_SOURCE_FILE);
    ASSERT(varDecl != nullptr);
    varDecl->SetParent(topStatement);
    // Declaration will be placed at start of current scope.
    // Can't insert right away until block's statements iteration ends.
    debugInfoPlugin_.RegisterPrologueEpilogue<true>(topStatement, varDecl);
    checkHelper->CheckLocalEntity(varDecl);

    // Yet don't track whether the value was modified, so store result unconditionally in the end of the scope.
    auto varUpdateSource = GetVarUpdateSourceCode(identName, regNumber, typeId);

    auto *varUpdate = parser.CreateFormattedStatement(varUpdateSource, parser::ParserContext::DEFAULT_SOURCE_FILE);
    ASSERT(varUpdate != nullptr);
    varUpdate->SetParent(topStatement);
    // Can't insert right away until block's statements iteration ends.
    debugInfoPlugin_.RegisterPrologueEpilogue<false>(topStatement, varUpdate);
    checkHelper->CheckLocalEntity(varUpdate);

    // Local variables are not registered, as they can be found in local scope.
    ASSERT(varDecl->AsVariableDeclaration()->Declarators().size() == 1);
    return varDecl->AsVariableDeclaration()->Declarators()[0]->Id()->Variable();
}

}  // namespace ark::es2panda::evaluate
