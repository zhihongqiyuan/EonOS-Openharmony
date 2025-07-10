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

#include "checker/ETSchecker.h"

#include "ir/ets/etsNullishTypes.h"
#include "compiler/lowering/scopesInit/scopesInitPhase.h"
#include "ir/ets/etsUnionType.h"
#include "ir/expressions/literals/undefinedLiteral.h"
#include "varbinder/ETSBinder.h"
#include "checker/types/ets/etsPartialTypeParameter.h"

namespace ark::es2panda::checker {

ir::TypeNode *ETSChecker::GetUtilityTypeTypeParamNode(const ir::TSTypeParameterInstantiation *const typeParams,
                                                      const std::string_view &utilityTypeName)
{
    if (typeParams->Params().size() != 1) {
        LogTypeError({"Invalid number of type parameters for ", utilityTypeName, " type"}, typeParams->Start());
    }

    return typeParams->Params().front();
}

Type *ETSChecker::HandleUtilityTypeParameterNode(const ir::TSTypeParameterInstantiation *const typeParams,
                                                 const std::string_view &utilityType)
{
    if (typeParams == nullptr) {
        return GlobalTypeError();
    }

    ir::TypeNode *typeParam = GetUtilityTypeTypeParamNode(typeParams, utilityType);

    Type *bareType = typeParam->Check(this);

    if (bareType == nullptr) {
        bareType = typeParam->GetType(this);
    }

    if (!bareType->IsETSReferenceType()) {
        LogTypeError("Only reference types can be converted to utility types.", typeParams->Start());
        return GlobalTypeError();
    }

    if (utilityType == compiler::Signatures::PARTIAL_TYPE_NAME) {
        return CreatePartialType(bareType);
    }

    if (utilityType == compiler::Signatures::READONLY_TYPE_NAME) {
        return GetReadonlyType(bareType);
    }

    if (utilityType == compiler::Signatures::REQUIRED_TYPE_NAME) {
        return HandleRequiredType(bareType);
    }

    LogTypeError("This utility type is not yet implemented.", typeParams->Start());
    return bareType;
}
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Partial utility type
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
template <typename T>
static T *CloneNodeIfNotNullptr(T *node, ArenaAllocator *allocator)
{
    return node != nullptr ? node->Clone(allocator, nullptr) : nullptr;
}

Type *ETSChecker::CreatePartialType(Type *const typeToBePartial)
{
    ASSERT(typeToBePartial->IsETSReferenceType());

    if (typeToBePartial->IsETSTypeParameter()) {
        return CreatePartialTypeParameter(typeToBePartial->AsETSTypeParameter());
    }

    if (typeToBePartial->IsETSUnionType()) {
        return HandleUnionForPartialType(typeToBePartial->AsETSUnionType());
    }

    auto typeDeclNode = typeToBePartial->Variable()->Declaration()->Node();
    if (typeDeclNode->IsClassDefinition() || typeDeclNode->IsTSInterfaceDeclaration()) {
        // NOTE (mmartin): there is a bug, that modifies the declaration of the variable of a type. That could make the
        // declaration node of an ETSObjectType eg. a ClassProperty, instead of the actual class declaration. When it's
        // fixed, remove this.
        return CreatePartialTypeClass(typeToBePartial->AsETSObjectType(), typeDeclNode);
    }

    return typeToBePartial;
}

Type *ETSChecker::CreatePartialTypeParameter(ETSTypeParameter *typeToBePartial)
{
    return Allocator()->New<ETSPartialTypeParameter>(typeToBePartial, this);
}

Type *ETSChecker::CreatePartialTypeClass(ETSObjectType *typeToBePartial, ir::AstNode *typeDeclNode)
{
    auto *declIdent = typeDeclNode->IsClassDefinition() ? typeDeclNode->AsClassDefinition()->Ident()
                                                        : typeDeclNode->AsTSInterfaceDeclaration()->Id();
    const auto partialClassName = util::UString(declIdent->Name(), Allocator()).Append(PARTIAL_CLASS_SUFFIX).View();
    auto *const classDefProgram = typeDeclNode->GetTopStatement()->AsETSScript()->Program();
    const bool isClassDeclaredInCurrentFile = classDefProgram == VarBinder()->Program();
    auto *const programToUse = isClassDeclaredInCurrentFile ? VarBinder()->Program() : classDefProgram;
    const auto qualifiedClassName = GetQualifiedClassName(programToUse, partialClassName);

    // Check if we've already generated the partial class, then don't do it again
    const auto classNameToFind =
        isClassDeclaredInCurrentFile || VarBinder()->IsGenStdLib() ? partialClassName : qualifiedClassName;
    if (auto *var =
            SearchNamesInMultiplePrograms({programToUse, VarBinder()->Program()}, {classNameToFind, partialClassName});
        var != nullptr) {
        return var->TsType();
    }

    if (typeDeclNode->IsTSInterfaceDeclaration()) {
        return HandlePartialInterface(typeDeclNode->AsTSInterfaceDeclaration(), isClassDeclaredInCurrentFile,
                                      partialClassName, programToUse, typeToBePartial);
    }

    ir::ClassDefinition *partialClassDef = CreateClassPrototype(partialClassName, programToUse);

    partialClassDef->SetInternalName(
        util::UString(typeDeclNode->AsClassDefinition()->InternalName().Mutf8() + PARTIAL_CLASS_SUFFIX, Allocator())
            .View());

    // If class prototype was created before, then we cached it's type. In that case return it.
    // This handles cases where a Partial<T> presents in class T, because during generating T$partial we'd need the
    // complete class T$partial which is not present at the time. Binding it's own type for it however will make it
    // possible to resolve member references later, when the full T$partial class was created.
    if (const auto found = NamedTypeStack().find(partialClassDef->TsType()); found != NamedTypeStack().end()) {
        return *found;
    }

    auto *const recordTableToUse = isClassDeclaredInCurrentFile
                                       ? VarBinder()->AsETSBinder()->GetGlobalRecordTable()
                                       : VarBinder()->AsETSBinder()->GetExternalRecordTable().at(programToUse);
    const varbinder::BoundContext boundCtx(recordTableToUse, partialClassDef);

    NamedTypeStackElement ntse(this, partialClassDef->TsType());

    // If class is external, put partial of it in global scope for the varbinder
    if (!isClassDeclaredInCurrentFile) {
        VarBinder()->Program()->GlobalScope()->InsertBinding(partialClassDef->Ident()->Name(),
                                                             partialClassDef->Variable());
    }

    return CreatePartialTypeClassDef(partialClassDef, typeDeclNode->AsClassDefinition(), typeToBePartial,
                                     recordTableToUse);
}

Type *ETSChecker::HandlePartialInterface(ir::TSInterfaceDeclaration *interfaceDecl, bool isClassDeclaredInCurrentFile,
                                         util::StringView const &partialClassName, parser::Program *const programToUse,
                                         ETSObjectType *const typeToBePartial)
{
    auto *const partialInterDecl = CreateInterfaceProto(partialClassName, interfaceDecl->IsStatic(),
                                                        isClassDeclaredInCurrentFile, interfaceDecl->Modifiers());

    const auto qualifiedName = GetQualifiedClassName(programToUse, partialClassName);
    partialInterDecl->SetInternalName(qualifiedName);

    if (const auto found = NamedTypeStack().find(partialInterDecl->TsType()); found != NamedTypeStack().end()) {
        return *found;
    }

    auto *const recordTable = isClassDeclaredInCurrentFile
                                  ? VarBinder()->AsETSBinder()->GetGlobalRecordTable()
                                  : VarBinder()->AsETSBinder()->GetExternalRecordTable().at(programToUse);
    const varbinder::BoundContext boundCtx(recordTable, partialInterDecl);

    NamedTypeStackElement ntse(this, partialInterDecl->TsType());

    // If class is external, put partial of it in global scope for the varbinder
    if (!isClassDeclaredInCurrentFile) {
        VarBinder()->Program()->GlobalScope()->InsertBinding(partialInterDecl->Id()->Name(),
                                                             partialInterDecl->Variable());
    }

    return CreatePartialTypeInterfaceDecl(interfaceDecl, typeToBePartial, partialInterDecl);
}

ir::ClassProperty *ETSChecker::CreateNullishPropertyFromAccessorInInterface(
    ir::MethodDefinition *const accessor, ir::TSInterfaceDeclaration *const newTSInterfaceDefinition)
{
    auto *ident = accessor->Id()->Clone(Allocator(), nullptr);
    auto modifierFlag = ir::ModifierFlags::NONE;
    if (accessor->Function()->IsGetter() && accessor->Overloads().empty()) {
        modifierFlag |= ir::ModifierFlags::READONLY;
    }

    auto *prop = Allocator()->New<ir::ClassProperty>(ident, nullptr, nullptr, modifierFlag, Allocator(), false);

    prop->SetParent(newTSInterfaceDefinition);
    ident->SetParent(prop);

    prop->SetTypeAnnotation(accessor->Function()->IsGetter()
                                ? accessor->Function()->ReturnTypeAnnotation()
                                : accessor->Function()->Params()[0]->AsETSParameterExpression()->TypeAnnotation());

    if (prop->TypeAnnotation() != nullptr) {
        return CreateNullishProperty(prop, newTSInterfaceDefinition);
    }

    if (accessor->TsType() == nullptr) {
        accessor->Parent()->Check(this);
    }

    ASSERT(accessor->TsType() != nullptr);
    auto callSign = accessor->TsType()->AsETSFunctionType()->CallSignatures()[0];

    auto tsType = accessor->Function()->IsGetter() ? callSign->ReturnType() : callSign->Params()[0]->TsType();

    prop->SetTypeAnnotation(Allocator()->New<ir::OpaqueTypeNode>(tsType));

    return CreateNullishProperty(prop, newTSInterfaceDefinition);
}

ir::ClassProperty *ETSChecker::CreateNullishPropertyFromAccessor(ir::MethodDefinition *const accessor,
                                                                 ir::ClassDefinition *const newClassDefinition)
{
    auto *ident = accessor->Id()->Clone(Allocator(), nullptr);
    auto modifierFlag = accessor->Function()->IsGetter() && accessor->Overloads().empty() ? ir::ModifierFlags::READONLY
                                                                                          : ir::ModifierFlags::NONE;

    auto *prop = Allocator()->New<ir::ClassProperty>(ident, nullptr, nullptr, modifierFlag, Allocator(), false);

    prop->SetParent(newClassDefinition);
    ident->SetParent(prop);

    prop->SetTypeAnnotation(accessor->Function()->IsGetter()
                                ? accessor->Function()->ReturnTypeAnnotation()
                                : accessor->Function()->Params()[0]->AsETSParameterExpression()->TypeAnnotation());

    if (prop->TypeAnnotation() != nullptr) {
        return CreateNullishProperty(prop, newClassDefinition);
    }

    if (accessor->TsType() == nullptr) {
        accessor->Parent()->Check(this);
    }

    auto callSign = accessor->TsType()->AsETSFunctionType()->CallSignatures()[0];

    auto tsType = accessor->Function()->IsGetter() ? callSign->ReturnType() : callSign->Params()[0]->TsType();

    prop->SetTypeAnnotation(Allocator()->New<ir::OpaqueTypeNode>(tsType));

    return CreateNullishProperty(prop, newClassDefinition);
}

ir::ClassProperty *ETSChecker::CreateNullishProperty(ir::ClassProperty *const prop,
                                                     ir::TSInterfaceDeclaration *const newTSInterfaceDefinition)
{
    auto *const propSavedValue = prop->Value();

    // Set value to nullptr to prevent cloning it (as for arrow functions that is not possible yet), we set it
    // to 'undefined' anyway
    prop->SetValue(nullptr);
    auto *const propClone = prop->Clone(Allocator(), newTSInterfaceDefinition)->AsClassProperty();

    // Revert original property value
    prop->SetValue(propSavedValue);
    propClone->SetValue(Allocator()->New<ir::UndefinedLiteral>());

    auto *propTypeAnn = propClone->TypeAnnotation();
    ArenaVector<ir::TypeNode *> types(Allocator()->Adapter());

    // Handle implicit type annotation
    if (propTypeAnn == nullptr) {
        propTypeAnn = Allocator()->New<ir::OpaqueTypeNode>(prop->TsType());
    }

    // Create new nullish type
    types.push_back(propTypeAnn);
    types.push_back(AllocNode<ir::ETSUndefinedType>());
    auto *const unionType = AllocNode<ir::ETSUnionType>(std::move(types));
    propClone->SetTypeAnnotation(unionType);

    // Set new parents
    unionType->SetParent(propClone);
    propClone->SetParent(newTSInterfaceDefinition);

    return propClone;
}

ir::ClassProperty *ETSChecker::CreateNullishProperty(ir::ClassProperty *const prop,
                                                     ir::ClassDefinition *const newClassDefinition)
{
    auto *const propSavedValue = prop->Value();

    // Set value to nullptr to prevent cloning it (as for arrow functions that is not possible yet), we set it
    // to 'undefined' anyway
    prop->SetValue(nullptr);
    auto *const propClone = prop->Clone(Allocator(), newClassDefinition)->AsClassProperty();

    // Revert original property value
    prop->SetValue(propSavedValue);
    propClone->SetValue(Allocator()->New<ir::UndefinedLiteral>());
    propClone->AsClassProperty()->Value()->Check(this);

    ir::TypeNode *propertyTypeAnnotation = propClone->TypeAnnotation();
    if (propertyTypeAnnotation == nullptr) {
        propertyTypeAnnotation = Allocator()->New<ir::OpaqueTypeNode>(prop->Check(this));
    }

    // Create new nullish type annotation
    ArenaVector<ir::TypeNode *> types(Allocator()->Adapter());
    types.push_back(propertyTypeAnnotation);
    types.push_back(AllocNode<ir::ETSUndefinedType>());
    propertyTypeAnnotation = AllocNode<ir::ETSUnionType>(std::move(types));
    propClone->SetTypeAnnotation(propertyTypeAnnotation);
    propClone->SetTsType(nullptr);

    // Set new parents
    propertyTypeAnnotation->SetParent(propClone);
    propClone->SetParent(newClassDefinition);

    return propClone;
}

ir::MethodDefinition *ETSChecker::CreateNullishAccessor(ir::MethodDefinition *const accessor,
                                                        ir::ClassDefinition *classDefinition)
{
    const auto interfaceCtx = varbinder::LexicalScope<varbinder::Scope>::Enter(VarBinder(), classDefinition->Scope());
    auto *paramScope = Allocator()->New<varbinder::FunctionParamScope>(Allocator(), classDefinition->Scope());
    auto *functionScope = Allocator()->New<varbinder::FunctionScope>(Allocator(), paramScope);
    functionScope->BindParamScope(paramScope);
    paramScope->BindFunctionScope(functionScope);

    {
        auto paramScopeCtx = varbinder::LexicalScope<varbinder::FunctionParamScope>::Enter(VarBinder(), paramScope);
        VarBinder()->AddMandatoryParam(varbinder::TypedBinder::MANDATORY_PARAM_THIS);
    }

    ir::MethodDefinition *nullishAccessor = accessor->Clone(Allocator(), classDefinition);

    auto *decl = Allocator()->New<varbinder::FunctionDecl>(Allocator(), nullishAccessor->Key()->AsIdentifier()->Name(),
                                                           nullishAccessor);
    auto *var = Allocator()->New<varbinder::LocalVariable>(decl, varbinder::VariableFlags::VAR);
    var->AddFlag(varbinder::VariableFlags::METHOD);
    nullishAccessor->Id()->SetVariable(var);
    nullishAccessor->SetVariable(var);

    functionScope->BindName(classDefinition->InternalName());

    auto *function = nullishAccessor->Function();

    function->SetVariable(var);
    function->SetIdent(nullishAccessor->Id());
    function->SetScope(functionScope);
    paramScope->BindNode(function);
    functionScope->BindNode(function);

    if (function->IsGetter()) {
        auto *propTypeAnn = function->ReturnTypeAnnotation();

        auto *unionType = AllocNode<ir::ETSUnionType>(
            ArenaVector<ir::TypeNode *>({propTypeAnn, AllocNode<ir::ETSUndefinedType>()}, Allocator()->Adapter()));
        function->SetReturnTypeAnnotation(unionType);
    } else {
        for (auto *params : function->Params()) {
            auto *paramExpr = params->AsETSParameterExpression();

            auto *unionType = AllocNode<ir::ETSUnionType>(ArenaVector<ir::TypeNode *>(
                {paramExpr->Ident()->TypeAnnotation(), AllocNode<ir::ETSUndefinedType>()}, Allocator()->Adapter()));
            paramExpr->Ident()->SetTsTypeAnnotation(unionType);

            auto *const paramVar = std::get<2>(paramScope->AddParamDecl(Allocator(), paramExpr));
            paramExpr->SetVariable(paramVar);
        }
    }
    ArenaVector<ir::MethodDefinition *> overloads(Allocator()->Adapter());
    nullishAccessor->SetOverloads(std::move(overloads));

    return nullishAccessor;
}

ir::TSTypeParameterDeclaration *ETSChecker::ProcessTypeParamAndGenSubstitution(
    ir::TSTypeParameterDeclaration const *const thisTypeParams,
    ArenaMap<ir::TSTypeParameter *, ir::TSTypeParameter *> *likeSubstitution,
    ir::TSTypeParameterDeclaration *newTypeParams = nullptr)
{
    ArenaVector<ir::TSTypeParameter *> typeParams(Allocator()->Adapter());
    if (newTypeParams == nullptr) {
        newTypeParams = AllocNode<ir::TSTypeParameterDeclaration>(std::move(typeParams), typeParams.size());
    }
    for (auto *const classOrInterfaceDefTypeParam : thisTypeParams->Params()) {
        auto *newTypeParam = AllocNode<ir::TSTypeParameter>(
            CloneNodeIfNotNullptr(classOrInterfaceDefTypeParam->Name(), Allocator()),
            CloneNodeIfNotNullptr(classOrInterfaceDefTypeParam->Constraint(), Allocator()),
            CloneNodeIfNotNullptr(classOrInterfaceDefTypeParam->DefaultType(), Allocator()));
        newTypeParams->AddParam(newTypeParam);
        newTypeParam->SetParent(newTypeParams);
        (*likeSubstitution)[classOrInterfaceDefTypeParam] = newTypeParam;
    }
    return newTypeParams;
}

ir::TSTypeParameterInstantiation *ETSChecker::CreateNewSuperPartialRefTypeParamsDecl(
    ArenaMap<ir::TSTypeParameter *, ir::TSTypeParameter *> *likeSubstitution, const Type *const superPartialType,
    ir::Expression *superRef)
{
    ir::TSTypeParameterInstantiation *superPartialRefTypeParams = nullptr;
    if (superPartialType == nullptr || superRef == nullptr ||
        superRef->AsETSTypeReference()->Part()->TypeParams() == nullptr) {
        return superPartialRefTypeParams;
    }
    superPartialRefTypeParams = superRef->AsETSTypeReference()->Part()->TypeParams()->Clone(Allocator(), nullptr);
    superPartialRefTypeParams->SetTsType(nullptr);
    auto superRefParams = superPartialRefTypeParams->Params();
    auto originRefParams = superRef->AsETSTypeReference()->Part()->TypeParams()->Params();
    for (size_t ix = 0; ix < superRefParams.size(); ++ix) {
        if (!originRefParams[ix]->IsETSTypeReference() ||
            !originRefParams[ix]->AsETSTypeReference()->Part()->TsType()->IsETSTypeParameter()) {
            continue;
        }
        auto it = likeSubstitution->find(
            originRefParams[ix]->AsETSTypeReference()->Part()->TsType()->AsETSTypeParameter()->GetDeclNode());
        if (it != likeSubstitution->end()) {
            auto *typeParamRefPart =
                AllocNode<ir::ETSTypeReferencePart>(it->second->Name()->Clone(Allocator(), nullptr));
            typeParamRefPart->Name()->SetParent(typeParamRefPart);
            auto *typeParamRef = AllocNode<ir::ETSTypeReference>(typeParamRefPart);
            typeParamRefPart->SetParent(typeParamRef);

            typeParamRef->SetParent(superPartialRefTypeParams);
            superRefParams[ix] = typeParamRef;
        }
    }
    return superPartialRefTypeParams;
}

ir::ETSTypeReference *ETSChecker::BuildSuperPartialTypeReference(
    Type *superPartialType, ir::TSTypeParameterInstantiation *superPartialRefTypeParams)
{
    ir::ETSTypeReference *superPartialRef = nullptr;
    if (superPartialType != nullptr) {
        auto *superPartialDeclNode = superPartialType->AsETSObjectType()->GetDeclNode();
        auto *clonedId = superPartialDeclNode->IsClassDefinition()
                             ? superPartialDeclNode->AsClassDefinition()->Ident()->Clone(Allocator(), nullptr)
                             : superPartialDeclNode->AsTSInterfaceDeclaration()->Id()->Clone(Allocator(), nullptr);
        auto *superPartialRefPart = AllocNode<ir::ETSTypeReferencePart>(clonedId, superPartialRefTypeParams, nullptr);
        superPartialRefPart->Name()->SetParent(superPartialRefPart);
        if (superPartialRefTypeParams != nullptr) {
            superPartialRefTypeParams->SetParent(superPartialRefPart);
        }

        superPartialRef = AllocNode<ir::ETSTypeReference>(superPartialRefPart);
        superPartialRefPart->SetParent(superPartialRef);
    }
    return superPartialRef;
}

void ETSChecker::CreatePartialClassDeclaration(ir::ClassDefinition *const newClassDefinition,
                                               ir::ClassDefinition *classDef)
{
    if (classDef->TypeParams() != nullptr) {
        ArenaVector<ir::TSTypeParameter *> typeParams(Allocator()->Adapter());
        for (auto *const classDefTypeParam : classDef->TypeParams()->Params()) {
            auto *const newTypeParam =
                AllocNode<ir::TSTypeParameter>(CloneNodeIfNotNullptr(classDefTypeParam->Name(), Allocator()),
                                               CloneNodeIfNotNullptr(classDefTypeParam->Constraint(), Allocator()),
                                               CloneNodeIfNotNullptr(classDefTypeParam->DefaultType(), Allocator()));
            typeParams.emplace_back(newTypeParam);
        }

        auto *const newTypeParams =
            AllocNode<ir::TSTypeParameterDeclaration>(std::move(typeParams), classDef->TypeParams()->RequiredParams());

        newClassDefinition->SetTypeParams(newTypeParams);
        newTypeParams->SetParent(newClassDefinition);
    }

    newClassDefinition->SetVariable(newClassDefinition->Ident()->Variable());
    newClassDefinition->AddModifier(static_cast<const ir::AstNode *>(classDef)->Modifiers());

    for (auto *const prop : classDef->Body()) {
        // Only handle class properties (members)
        // Method calls on partial classes will make the class not type safe, so we don't copy any methods
        if (prop->IsClassProperty()) {
            if (prop->AsClassProperty()->Id()->Name().Mutf8().find(compiler::Signatures::PROPERTY, 0) == 0) {
                continue;
            }

            auto *const newProp = CreateNullishProperty(prop->AsClassProperty(), newClassDefinition);

            // Put the new property into the class declaration
            newClassDefinition->Body().emplace_back(newProp);
        }

        if (prop->IsMethodDefinition() && (prop->AsMethodDefinition()->Function()->IsGetter() ||
                                           prop->AsMethodDefinition()->Function()->IsSetter())) {
            auto *propMethod = prop->AsMethodDefinition();
            if (newClassDefinition->Scope()->FindLocal(propMethod->Id()->Name(),
                                                       varbinder::ResolveBindingOptions::VARIABLES) != nullptr) {
                continue;
            }
            auto *newProp = CreateNullishPropertyFromAccessor(propMethod, newClassDefinition);
            newClassDefinition->Body().emplace_back(newProp);
        }
    }
    if (classDef->IsDeclare()) {
        newClassDefinition->AddModifier(ir::ModifierFlags::DECLARE);
    }

    // Run varbinder for new partial class to set scopes
    compiler::InitScopesPhaseETS::RunExternalNode(newClassDefinition, VarBinder());

    newClassDefinition->SetTsType(nullptr);
    newClassDefinition->Variable()->SetTsType(nullptr);
}

void ETSChecker::ConvertGetterAndSetterToProperty(ir::TSInterfaceDeclaration *interfaceDecl,
                                                  ir::TSInterfaceDeclaration *partialInterface)
{
    auto *propertyAdded = Allocator()->New<ArenaSet<util::StringView>>(Allocator()->Adapter());
    for (auto *const prop : interfaceDecl->Body()->Body()) {
        if (prop->IsMethodDefinition() && (prop->AsMethodDefinition()->Function()->IsGetter() ||
                                           prop->AsMethodDefinition()->Function()->IsSetter())) {
            auto *propMethod = prop->AsMethodDefinition();
            if (propertyAdded->find(propMethod->Id()->Name()) != propertyAdded->end()) {
                continue;
            }
            auto *newProp = CreateNullishPropertyFromAccessorInInterface(propMethod, partialInterface);
            propertyAdded->insert(propMethod->Id()->Name());

            partialInterface->Body()->Body().emplace_back(newProp);
        }
    }
}
ir::MethodDefinition *ETSChecker::CreateNullishAccessor(ir::MethodDefinition *const accessor,
                                                        ir::TSInterfaceDeclaration *interface)
{
    const auto interfaceCtx = varbinder::LexicalScope<varbinder::Scope>::Enter(VarBinder(), interface->Scope());
    auto *paramScope = Allocator()->New<varbinder::FunctionParamScope>(Allocator(), interface->Scope());
    auto *functionScope = Allocator()->New<varbinder::FunctionScope>(Allocator(), paramScope);
    functionScope->BindParamScope(paramScope);
    paramScope->BindFunctionScope(functionScope);

    {
        auto paramScopeCtx = varbinder::LexicalScope<varbinder::FunctionParamScope>::Enter(VarBinder(), paramScope);
        VarBinder()->AddMandatoryParam(varbinder::TypedBinder::MANDATORY_PARAM_THIS);
    }

    ir::MethodDefinition *nullishAccessor = accessor->Clone(Allocator(), interface->Body());

    auto *decl = Allocator()->New<varbinder::FunctionDecl>(Allocator(), nullishAccessor->Key()->AsIdentifier()->Name(),
                                                           nullishAccessor);
    auto *var = Allocator()->New<varbinder::LocalVariable>(decl, varbinder::VariableFlags::VAR);
    var->AddFlag(varbinder::VariableFlags::METHOD);
    nullishAccessor->Id()->SetVariable(var);
    nullishAccessor->SetVariable(var);

    functionScope->BindName(interface->InternalName());

    auto *function = nullishAccessor->Function();

    function->SetVariable(var);
    function->SetIdent(nullishAccessor->Id());
    function->SetScope(functionScope);
    paramScope->BindNode(function);
    functionScope->BindNode(function);

    if (function->IsGetter()) {
        auto *propTypeAnn = function->ReturnTypeAnnotation();

        auto *unionType = AllocNode<ir::ETSUnionType>(
            ArenaVector<ir::TypeNode *>({propTypeAnn, AllocNode<ir::ETSUndefinedType>()}, Allocator()->Adapter()));
        function->SetReturnTypeAnnotation(unionType);
    } else {
        for (auto *params : function->Params()) {
            auto *paramExpr = params->AsETSParameterExpression();

            auto *unionType = AllocNode<ir::ETSUnionType>(ArenaVector<ir::TypeNode *>(
                {paramExpr->Ident()->TypeAnnotation(), AllocNode<ir::ETSUndefinedType>()}, Allocator()->Adapter()));
            paramExpr->Ident()->SetTsTypeAnnotation(unionType);

            auto *const paramVar = std::get<2>(paramScope->AddParamDecl(Allocator(), paramExpr));
            paramExpr->SetVariable(paramVar);
        }
    }
    ArenaVector<ir::MethodDefinition *> overloads(Allocator()->Adapter());
    nullishAccessor->SetOverloads(std::move(overloads));

    return nullishAccessor;
}

ir::TSInterfaceDeclaration *ETSChecker::CreateInterfaceProto(util::StringView name, const bool isStatic,
                                                             const bool isClassDeclaredInCurrentFile,
                                                             const ir::ModifierFlags flags)
{
    const auto globalCtx =
        varbinder::LexicalScope<varbinder::GlobalScope>::Enter(VarBinder(), VarBinder()->Program()->GlobalScope());

    auto *const interfaceId = AllocNode<ir::Identifier>(name, Allocator());
    const auto [decl, var] =
        VarBinder()->NewVarDecl<varbinder::InterfaceDecl>(interfaceId->Start(), Allocator(), interfaceId->Name());
    interfaceId->SetVariable(var);

    auto *body = AllocNode<ir::TSInterfaceBody>(ArenaVector<ir::AstNode *>(Allocator()->Adapter()));
    ArenaVector<ir::TSInterfaceHeritage *> extends(Allocator()->Adapter());

    ArenaVector<ir::TSTypeParameter *> typeParams(Allocator()->Adapter());
    auto *newTypeParams = AllocNode<ir::TSTypeParameterDeclaration>(std::move(typeParams), typeParams.size());
    auto partialInterface = AllocNode<ir::TSInterfaceDeclaration>(
        Allocator(), std::move(extends),
        ir::TSInterfaceDeclaration::ConstructorData {interfaceId, newTypeParams, body, isStatic,
                                                     !isClassDeclaredInCurrentFile, Language(Language::Id::ETS)});

    const auto classCtx = varbinder::LexicalScope<varbinder::ClassScope>(VarBinder());
    partialInterface->TypeParams()->SetParent(partialInterface);
    partialInterface->SetScope(classCtx.GetScope());
    partialInterface->SetVariable(var);
    decl->BindNode(partialInterface);

    partialInterface->AddModifier(flags);

    return partialInterface;
}

Type *ETSChecker::CreatePartialTypeInterfaceDecl(ir::TSInterfaceDeclaration *const interfaceDecl,
                                                 ETSObjectType *const typeToBePartial,
                                                 ir::TSInterfaceDeclaration *partialInterface)
{
    ConvertGetterAndSetterToProperty(interfaceDecl, partialInterface);
    // Create nullish properties of the partial class
    // Build the new Partial class based on the 'T' type parameter of 'Partial<T>'
    auto *likeSubstitution =
        Allocator()->New<ArenaMap<ir::TSTypeParameter *, ir::TSTypeParameter *>>(Allocator()->Adapter());

    if (interfaceDecl->TypeParams() != nullptr) {
        ProcessTypeParamAndGenSubstitution(interfaceDecl->TypeParams(), likeSubstitution,
                                           partialInterface->TypeParams());
    }

    compiler::InitScopesPhaseETS::RunExternalNode(partialInterface, VarBinder());

    // Create partial type for super type
    for (auto *extend : interfaceDecl->Extends()) {
        auto *t = extend->Expr()->AsETSTypeReference()->Part()->GetType(this);
        if (auto *superPartialType = CreatePartialType(t); superPartialType != nullptr) {
            ir::TSTypeParameterInstantiation *superPartialRefTypeParams =
                CreateNewSuperPartialRefTypeParamsDecl(likeSubstitution, superPartialType, extend->Expr());

            ir::ETSTypeReference *superPartialRef =
                BuildSuperPartialTypeReference(superPartialType, superPartialRefTypeParams);
            partialInterface->Extends().push_back(AllocNode<ir::TSInterfaceHeritage>(superPartialRef));
        }
    }

    auto *const partialType = partialInterface->Check(this)->AsETSObjectType();
    partialType->SetAssemblerName(partialInterface->InternalName());
    partialType->SetBaseType(typeToBePartial);

    return partialType;
}

void ETSChecker::CreateConstructorForPartialType(ir::ClassDefinition *const partialClassDef,
                                                 checker::ETSObjectType *const partialType,
                                                 varbinder::RecordTable *const recordTable)
{
    // Create scopes
    auto *const scope = partialClassDef->Scope()->AsClassScope();
    const auto classCtx = varbinder::LexicalScope<varbinder::ClassScope>::Enter(VarBinder(), scope);

    // Create ctor
    auto *const ctor = CreateNonStaticClassInitializer(classCtx.GetScope(), recordTable);
    auto *const ctorFunc = ctor->Function();
    if (partialClassDef->IsDeclare()) {
        ctorFunc->AddFlag(ir::ScriptFunctionFlags::EXTERNAL);
    }
    auto *const ctorId = ctor->Function()->Id();

    // Handle bindings, create method decl for ctor
    ctorFunc->Scope()->Find(varbinder::VarBinder::MANDATORY_PARAM_THIS).variable->SetTsType(partialType);
    partialType->AddConstructSignature(ctorFunc->Signature());
    ctorFunc->Signature()->SetOwner(partialType);
    ctor->SetParent(partialClassDef);
    ctorId->SetVariable(Allocator()->New<varbinder::LocalVariable>(
        Allocator()->New<varbinder::MethodDecl>(ctorId->Name()), varbinder::VariableFlags::METHOD));
    ctor->Id()->SetVariable(ctorId->Variable());

    // Put ctor in partial class body
    partialClassDef->Body().emplace_back(ctor);
}

ir::ClassDefinition *ETSChecker::CreateClassPrototype(util::StringView name, parser::Program *const classDeclProgram)
{
    const auto globalCtx =
        varbinder::LexicalScope<varbinder::GlobalScope>::Enter(VarBinder(), classDeclProgram->GlobalScope());

    // Create class name, and declaration variable
    auto *const classId = AllocNode<ir::Identifier>(name, Allocator());
    const auto [decl, var] = VarBinder()->NewVarDecl<varbinder::ClassDecl>(classId->Start(), classId->Name());
    classId->SetVariable(var);

    // Create class definition node
    const auto classCtx = varbinder::LexicalScope<varbinder::ClassScope>(VarBinder());
    auto *const classDef =
        AllocNode<ir::ClassDefinition>(Allocator(), classId, ir::ClassDefinitionModifiers::DECLARATION,
                                       ir::ModifierFlags::NONE, Language(Language::Id::ETS));
    classDef->SetScope(classCtx.GetScope());
    classDef->SetVariable(var);

    // Create class declaration node
    auto *const classDecl = AllocNode<ir::ClassDeclaration>(classDef, Allocator());
    classDecl->SetParent(classDeclProgram->Ast());

    // Class definition is scope bearer, not class declaration
    classDef->Scope()->BindNode(classDecl->Definition());
    decl->BindNode(classDef);

    // Put class declaration in global scope, and in program AST
    classDeclProgram->Ast()->Statements().push_back(classDecl);
    classDeclProgram->GlobalScope()->InsertBinding(name, var);

    // Create only class 'header' (no properties and methods, but base type created)
    BuildBasicClassProperties(classDef);

    return classDef;
}

varbinder::Variable *ETSChecker::SearchNamesInMultiplePrograms(const std::set<const parser::Program *> &programs,
                                                               const std::set<util::StringView> &classNamesToFind)
{
    for (const auto *const program : programs) {
        for (const auto &className : classNamesToFind) {
            auto *const var = program->GlobalScope()->Find(className, varbinder::ResolveBindingOptions::ALL).variable;
            if (var == nullptr) {
                continue;
            }

            if (var->TsType() == nullptr) {
                var->Declaration()->Node()->Check(this);
            }

            return var;
        }
    }

    return nullptr;
}

util::StringView ETSChecker::GetQualifiedClassName(const parser::Program *const classDefProgram,
                                                   const util::StringView className)
{
    auto packageName = classDefProgram->ModuleName().Mutf8();
    if (!packageName.empty()) {
        packageName.append(".");
    }

    return util::UString(packageName + className.Mutf8(), Allocator()).View();
}

Type *ETSChecker::HandleUnionForPartialType(ETSUnionType *const typeToBePartial)
{
    // Convert a union type to partial, by converting all types in it to partial, and making a new union
    // type out of them
    const auto *const unionTypeNode = typeToBePartial->AsETSUnionType();
    ArenaVector<checker::Type *> newTypesForUnion(Allocator()->Adapter());

    for (auto *const typeFromUnion : unionTypeNode->ConstituentTypes()) {
        if ((typeFromUnion->Variable() != nullptr) && (typeFromUnion->Variable()->Declaration() != nullptr)) {
            newTypesForUnion.emplace_back(CreatePartialType(typeFromUnion));
        } else {
            newTypesForUnion.emplace_back(typeFromUnion);
        }
    }

    return CreateETSUnionType(std::move(newTypesForUnion));
}

Type *ETSChecker::CreatePartialTypeClassDef(ir::ClassDefinition *const partialClassDef,
                                            ir::ClassDefinition *const classDef, ETSObjectType *const typeToBePartial,
                                            varbinder::RecordTable *const recordTableToUse)
{
    CreatePartialClassDeclaration(partialClassDef, classDef);

    // Run checker
    auto *const partialType = partialClassDef->Check(this)->AsETSObjectType();

    for (auto *interface : typeToBePartial->Interfaces()) {
        partialType->AddInterface(CreatePartialType(interface)->AsETSObjectType());
    }

    partialType->SetAssemblerName(partialClassDef->InternalName());
    partialType->SetBaseType(typeToBePartial);

    CreateConstructorForPartialType(partialClassDef, partialType, recordTableToUse);

    // Create partial type for super type
    if (typeToBePartial != GlobalETSObjectType()) {
        auto *const partialSuper =
            CreatePartialType(classDef->Super() == nullptr ? GlobalETSObjectType() : classDef->Super()->TsType());

        partialType->SetSuperType(partialSuper->AsETSObjectType());
    }

    return partialType;
}

std::pair<ir::ScriptFunction *, ir::Identifier *> ETSChecker::CreateScriptFunctionForConstructor(
    varbinder::FunctionScope *const scope)
{
    ArenaVector<ir::Statement *> statements(Allocator()->Adapter());
    ArenaVector<ir::Expression *> params(Allocator()->Adapter());

    ir::ScriptFunction *func {};
    ir::Identifier *id {};

    auto *const body = AllocNode<ir::BlockStatement>(Allocator(), std::move(statements));
    body->SetScope(scope);
    id = AllocNode<ir::Identifier>(util::UString(std::string("constructor"), Allocator()).View(), Allocator());
    auto funcSignature = ir::FunctionSignature(nullptr, std::move(params), nullptr);
    func = AllocNode<ir::ScriptFunction>(Allocator(),
                                         ir::ScriptFunction::ScriptFunctionData {
                                             body, std::move(funcSignature),
                                             ir::ScriptFunctionFlags::CONSTRUCTOR | ir::ScriptFunctionFlags::EXPRESSION,
                                             ir::ModifierFlags::PUBLIC});

    func->SetScope(scope);
    scope->BindNode(func);
    func->SetIdent(id);
    VarBinder()->AsETSBinder()->AddFunctionThisParam(func);

    return std::make_pair(func, id);
}

ir::MethodDefinition *ETSChecker::CreateNonStaticClassInitializer(varbinder::ClassScope *classScope,
                                                                  varbinder::RecordTable *const recordTable)
{
    const auto classCtx = varbinder::LexicalScope<varbinder::ClassScope>::Enter(VarBinder(), classScope);

    auto *paramScope = Allocator()->New<varbinder::FunctionParamScope>(Allocator(), classScope);
    auto *const functionScope = Allocator()->New<varbinder::FunctionScope>(Allocator(), paramScope);
    functionScope->BindParamScope(paramScope);
    paramScope->BindFunctionScope(functionScope);

    const auto funcParamCtx = varbinder::LexicalScope<varbinder::FunctionParamScope>::Enter(VarBinder(), paramScope);

    auto [func, id] = CreateScriptFunctionForConstructor(functionScope);

    paramScope->BindNode(func);
    functionScope->BindNode(func);

    auto *const signatureInfo = CreateSignatureInfo();
    auto *const signature = CreateSignature(signatureInfo, GlobalVoidType(), func);
    func->SetSignature(signature);

    VarBinder()->AsETSBinder()->BuildInternalNameWithCustomRecordTable(func, recordTable);
    VarBinder()->AsETSBinder()->BuildFunctionName(func);
    VarBinder()->Functions().push_back(functionScope);

    auto *funcExpr = AllocNode<ir::FunctionExpression>(func);
    auto *const ctor = AllocNode<ir::MethodDefinition>(ir::MethodDefinitionKind::CONSTRUCTOR,
                                                       id->Clone(Allocator(), classScope->Node()), funcExpr,
                                                       ir::ModifierFlags::NONE, Allocator(), false);

    auto *const funcType = CreateETSFunctionType(signature, id->Name());
    ctor->SetTsType(funcType);

    return ctor;
}

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Readonly utility type
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Type *ETSChecker::HandleReadonlyType(const ir::TSTypeParameterInstantiation *const typeParams)
{
    if (typeParams == nullptr) {
        return GlobalTypeError();
    }

    auto *const typeParamNode = GetUtilityTypeTypeParamNode(typeParams, compiler::Signatures::READONLY_TYPE_NAME);
    auto *typeToBeReadonly = typeParamNode->Check(this);

    if (auto found = NamedTypeStack().find(typeToBeReadonly); found != NamedTypeStack().end()) {
        return *found;
    }

    NamedTypeStackElement ntse(this, typeToBeReadonly);
    return GetReadonlyType(typeToBeReadonly);
}

Type *ETSChecker::GetReadonlyType(Type *type)
{
    if (const auto found = NamedTypeStack().find(type); found != NamedTypeStack().end()) {
        return *found;
    }

    NamedTypeStackElement ntse(this, type);

    if (type->IsETSArrayType() && !type->IsETSTupleType()) {
        ETSArrayType *clonedArrayType = Allocator()->New<ETSArrayType>(type->AsETSArrayType()->ElementType());
        clonedArrayType->AddTypeFlag(TypeFlag::READONLY);
        return clonedArrayType;
    }
    if (type->IsETSTupleType()) {
        Type *clonedType = type->Clone(this);
        clonedType->AddTypeFlag(TypeFlag::READONLY);
        return clonedType;
    }

    if (type->IsETSObjectType()) {
        type->AsETSObjectType()->InstanceFields();
        auto *clonedType = type->Clone(this)->AsETSObjectType();
        MakePropertiesReadonly(clonedType);
        return clonedType;
    }
    if (type->IsETSTypeParameter()) {
        return Allocator()->New<ETSReadonlyType>(type->AsETSTypeParameter());
    }
    if (type->IsETSUnionType()) {
        ArenaVector<Type *> unionTypes(Allocator()->Adapter());
        for (auto *t : type->AsETSUnionType()->ConstituentTypes()) {
            unionTypes.emplace_back(t->IsETSObjectType() ? GetReadonlyType(t) : t->Clone(this));
        }
        return CreateETSUnionType(std::move(unionTypes));
    }
    return type;
}

void ETSChecker::MakePropertiesReadonly(ETSObjectType *const classType)
{
    classType->UpdateTypeProperties(this, [this](auto *property, auto *propType) {
        auto *newDecl = Allocator()->New<varbinder::ReadonlyDecl>(property->Name(), property->Declaration()->Node());
        auto *const propCopy = property->Copy(Allocator(), newDecl);
        propCopy->AddFlag(varbinder::VariableFlags::READONLY);
        propCopy->SetTsType(propType);
        return propCopy;
    });
}

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// Required utility type
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Type *ETSChecker::HandleRequiredType(Type *typeToBeRequired)
{
    if (const auto found = NamedTypeStack().find(typeToBeRequired); found != NamedTypeStack().end()) {
        return *found;
    }

    NamedTypeStackElement ntse(this, typeToBeRequired);

    if (typeToBeRequired->IsETSTypeParameter()) {
        auto *const requiredClone = typeToBeRequired->Clone(this);
        requiredClone->AddTypeFlag(TypeFlag::ETS_REQUIRED_TYPE_PARAMETER);
        return requiredClone;
    }

    if (typeToBeRequired->IsETSUnionType()) {
        ArenaVector<Type *> unionTypes(Allocator()->Adapter());
        for (auto *type : typeToBeRequired->AsETSUnionType()->ConstituentTypes()) {
            if (type->IsETSObjectType()) {
                type = type->Clone(this);
                MakePropertiesNonNullish(type->AsETSObjectType());
            }

            if (type->IsETSNullType() || type->IsETSUndefinedType()) {
                continue;
            }

            unionTypes.emplace_back(type);
        }

        return CreateETSUnionType(std::move(unionTypes));
    }

    if (typeToBeRequired->IsETSObjectType()) {
        typeToBeRequired->AsETSObjectType()->InstanceFields();  // call to instantiate properties
    }

    typeToBeRequired = typeToBeRequired->Clone(this);

    MakePropertiesNonNullish(typeToBeRequired->AsETSObjectType());

    return typeToBeRequired;
}

void ETSChecker::MakePropertiesNonNullish(ETSObjectType *const classType)
{
    classType->AddObjectFlag(ETSObjectFlags::REQUIRED);
    classType->InstanceFields();

    for (const auto &[_, propVar] : classType->InstanceFields()) {
        MakePropertyNonNullish<PropertyType::INSTANCE_FIELD>(classType, propVar);
    }

    for (const auto &[_, propVar] : classType->StaticFields()) {
        MakePropertyNonNullish<PropertyType::STATIC_FIELD>(classType, propVar);
    }

    if (classType->SuperType() != nullptr) {
        auto *const superRequired = classType->SuperType()->Clone(this)->AsETSObjectType();
        MakePropertiesNonNullish(superRequired);
        classType->SetSuperType(superRequired);
    }
}

template <PropertyType PROP_TYPE>
void ETSChecker::MakePropertyNonNullish(ETSObjectType *const classType, varbinder::LocalVariable *const prop)
{
    auto *const propType = prop->TsType();
    auto *const nonNullishPropType = GetNonNullishType(propType);

    auto *const propCopy = prop->Copy(Allocator(), prop->Declaration());

    propCopy->SetTsType(nonNullishPropType);
    classType->RemoveProperty<PROP_TYPE>(prop);
    classType->AddProperty<PROP_TYPE>(propCopy);
}

static bool StringEqualsPropertyName(const util::StringView pname1, const ir::Expression *const prop2Key)
{
    util::StringView pname2;
    if (prop2Key->IsStringLiteral()) {
        pname2 = prop2Key->AsStringLiteral()->Str();
    } else if (prop2Key->IsIdentifier()) {
        pname2 = prop2Key->AsIdentifier()->Name();
    }

    return pname1 == pname2;
}

void ETSChecker::ValidateObjectLiteralForRequiredType(const ETSObjectType *const requiredType,
                                                      const ir::ObjectExpression *const initObjExpr)
{
    const std::size_t classPropertyCount = requiredType->InstanceFields().size() + requiredType->StaticFields().size();

    auto initObjExprContainsField = [&initObjExpr](const util::StringView pname1) {
        return std::find_if(initObjExpr->Properties().begin(), initObjExpr->Properties().end(),
                            [&pname1](const ir::Expression *const initProp) {
                                return StringEqualsPropertyName(pname1, initProp->AsProperty()->Key());
                            }) != initObjExpr->Properties().end();
    };

    if (classPropertyCount > initObjExpr->Properties().size()) {
        std::string_view missingProp;

        for (const auto &[propName, _] : requiredType->InstanceFields()) {
            if (!initObjExprContainsField(propName)) {
                missingProp = propName.Utf8();
            }
        }

        for (const auto &[propName, _] : requiredType->StaticFields()) {
            if (!initObjExprContainsField(propName)) {
                missingProp = propName.Utf8();
            }
        }

        if (!missingProp.empty()) {
            LogTypeError({"Class property '", missingProp, "' needs to be initialized for required type."},
                         initObjExpr->Start());
        }
    }
}
}  // namespace ark::es2panda::checker
