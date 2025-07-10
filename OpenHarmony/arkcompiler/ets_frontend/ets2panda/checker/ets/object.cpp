/**
 * Copyright (c) 2021-2024 - Huawei Device Co., Ltd.
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

#include "boxingConverter.h"
#include "checker/ETSchecker.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/types/ets/etsDynamicType.h"
#include "checker/types/ets/etsObjectType.h"
#include "checker/types/ets/etsTupleType.h"
#include "checker/types/ets/etsPartialTypeParameter.h"
#include "ir/astNode.h"
#include "ir/typeNode.h"
#include "ir/base/classDefinition.h"
#include "ir/base/classElement.h"
#include "ir/base/classProperty.h"
#include "ir/base/classStaticBlock.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/scriptFunction.h"
#include "ir/ets/etsNewClassInstanceExpression.h"
#include "ir/ets/etsTypeReference.h"
#include "ir/ets/etsTypeReferencePart.h"
#include "ir/ets/etsUnionType.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/functionExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/memberExpression.h"
#include "ir/statements/blockStatement.h"
#include "ir/statements/expressionStatement.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/ts/tsClassImplements.h"
#include "ir/ts/tsInterfaceDeclaration.h"
#include "ir/ts/tsInterfaceHeritage.h"
#include "ir/ts/tsTypeParameter.h"
#include "ir/ts/tsTypeParameterDeclaration.h"
#include "varbinder/declaration.h"
#include "varbinder/variableFlags.h"
#include "generated/signatures.h"

namespace ark::es2panda::checker {
ETSObjectType *ETSChecker::GetSuperType(ETSObjectType *type)
{
    ComputeSuperType(type);
    if (type == GlobalETSObjectType()) {
        return GlobalETSObjectType();
    }
    if (type->SuperType() == nullptr) {
        return nullptr;
    }
    return type->SuperType();
}

bool ETSChecker::ComputeSuperType(ETSObjectType *type)
{
    if (type->HasObjectFlag(ETSObjectFlags::RESOLVED_SUPER)) {
        return true;
    }

    ASSERT(type->Variable() && type->GetDeclNode()->IsClassDefinition());
    auto *classDef = type->GetDeclNode()->AsClassDefinition();

    if (classDef->Super() == nullptr) {
        type->AddObjectFlag(ETSObjectFlags::RESOLVED_SUPER);
        if (type != GlobalETSObjectType()) {
            type->SetSuperType(GlobalETSObjectType());
        }
        return true;
    }

    TypeStackElement tse(this, type, {"Cyclic inheritance involving ", type->Name(), "."}, classDef->Ident()->Start());
    if (tse.HasTypeError()) {
        type->AddObjectFlag(ETSObjectFlags::RESOLVED_SUPER);
        return false;
    }

    Type *superType = classDef->Super()->AsTypeNode()->GetType(this);
    if (superType == nullptr) {
        return true;
    }
    if (!superType->IsETSObjectType() || !superType->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::CLASS)) {
        LogTypeError({"The super type of '", classDef->Ident()->Name(), "' class is not extensible."},
                     classDef->Super()->Start());
        type->SetSuperType(GlobalETSObjectType());
        return true;
    }

    ETSObjectType *superObj = superType->AsETSObjectType();

    // struct node has class definition, too
    if (superObj->GetDeclNode()->Parent()->IsETSStructDeclaration()) {
        LogTypeError({"struct ", classDef->Ident()->Name(), " is not extensible."}, classDef->Super()->Start());
    }

    if (superObj->GetDeclNode()->IsFinal()) {
        LogTypeError("Cannot inherit with 'final' modifier.", classDef->Super()->Start());
        /* It still makes sense to treat superObj as the supertype in future checking */
    }
    if (GetSuperType(superObj) == nullptr) {
        superObj = GlobalETSObjectType();
    }
    type->SetSuperType(superObj);
    type->AddObjectFlag(ETSObjectFlags::RESOLVED_SUPER);
    return true;
}

void ETSChecker::ValidateImplementedInterface(ETSObjectType *type, Type *interface,
                                              std::unordered_set<Type *> *extendsSet, const lexer::SourcePosition &pos)
{
    if (!interface->IsETSObjectType() || !interface->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::INTERFACE)) {
        LogTypeError("Interface expected here.", pos);
        return;
    }

    if (!extendsSet->insert(interface).second) {
        LogTypeError("Repeated interface.", pos);
    }

    type->AddInterface(interface->AsETSObjectType());
    GetInterfaces(interface->AsETSObjectType());
}

void ETSChecker::GetInterfacesOfClass(ETSObjectType *type)
{
    if (type->HasObjectFlag(ETSObjectFlags::RESOLVED_INTERFACES)) {
        return;
    }

    const auto *declNode = type->GetDeclNode()->AsClassDefinition();

    std::unordered_set<Type *> extendsSet;
    for (auto *it : declNode->Implements()) {
        ValidateImplementedInterface(type, it->Expr()->AsTypeNode()->GetType(this), &extendsSet, it->Start());
    }
    type->AddObjectFlag(ETSObjectFlags::RESOLVED_INTERFACES);
}

void ETSChecker::GetInterfacesOfInterface(ETSObjectType *type)
{
    if (type->HasObjectFlag(ETSObjectFlags::RESOLVED_INTERFACES)) {
        return;
    }

    const auto *declNode = type->GetDeclNode()->AsTSInterfaceDeclaration();

    TypeStackElement tse(this, type, {"Cyclic inheritance involving ", type->Name(), "."}, declNode->Id()->Start());
    if (tse.HasTypeError()) {
        type->Interfaces().clear();
        return;
    }

    std::unordered_set<Type *> extendsSet;
    for (auto *it : declNode->Extends()) {
        ValidateImplementedInterface(type, it->Expr()->AsTypeNode()->GetType(this), &extendsSet, it->Start());
    }
    type->AddObjectFlag(ETSObjectFlags::RESOLVED_INTERFACES);
}

ArenaVector<ETSObjectType *> ETSChecker::GetInterfaces(ETSObjectType *type)
{
    ASSERT(type->GetDeclNode()->IsClassDefinition() || type->GetDeclNode()->IsTSInterfaceDeclaration());

    if (type->GetDeclNode()->IsClassDefinition()) {
        GetInterfacesOfClass(type);
    } else {
        GetInterfacesOfInterface(type);
    }

    return type->Interfaces();
}

std::pair<ArenaVector<Type *>, bool> ETSChecker::CreateUnconstrainedTypeParameters(
    ir::TSTypeParameterDeclaration const *typeParams)
{
    bool ok = true;
    ArenaVector<Type *> result {Allocator()->Adapter()};
    checker::ScopeContext scopeCtx(this, typeParams->Scope());

    // Note: we have to run pure check loop first to avoid endless loop because of possible circular dependencies
    Type2TypeMap extends {};
    TypeSet typeParameterDecls {};
    for (auto *const typeParam : typeParams->Params()) {
        ok &= CheckDefaultTypeParameter(typeParam, typeParameterDecls);
        if (auto *const constraint = typeParam->Constraint();
            constraint != nullptr && constraint->IsETSTypeReference() &&
            constraint->AsETSTypeReference()->Part()->Name()->IsIdentifier()) {
            ok &= CheckTypeParameterConstraint(typeParam, extends);
        }
    }

    for (auto *const typeParam : typeParams->Params()) {
        result.emplace_back(SetUpParameterType(typeParam));
    }

    return {result, ok};
}

void ETSChecker::AssignTypeParameterConstraints(ir::TSTypeParameterDeclaration const *typeParams)
{
    ConstraintCheckScope ctScope(this);
    // The type parameter might be used in the constraint, like 'K extend Comparable<K>',
    // so we need to create their type first, then set up the constraint
    for (auto *const param : typeParams->Params()) {
        SetUpTypeParameterConstraint(param);
    }
    ctScope.TryCheckConstraints();
}

bool ETSChecker::CheckDefaultTypeParameter(const ir::TSTypeParameter *param, TypeSet &typeParameterDecls)
{
    bool ok = true;
    const auto typeParamVar = param->Name()->Variable();
    if (typeParameterDecls.count(typeParamVar) != 0U) {
        LogTypeError({"Duplicate type parameter '", param->Name()->Name().Utf8(), "'."}, param->Start());
        return false;
    }

    std::function<void(ir::AstNode *)> checkDefault = [&typeParameterDecls, this, &checkDefault,
                                                       &ok](ir::AstNode *node) {
        if (node->IsETSTypeReferencePart()) {
            ir::ETSTypeReferencePart *defaultTypePart = node->AsETSTypeReferencePart();
            if (defaultTypePart->Name()->Variable()->TsType() == nullptr &&
                (defaultTypePart->Name()->Variable()->Flags() & varbinder::VariableFlags::TYPE_PARAMETER) != 0U &&
                typeParameterDecls.count(defaultTypePart->Name()->Variable()) == 0U) {
                LogTypeError({"Type Parameter ", defaultTypePart->Name()->AsIdentifier()->Name().Utf8(),
                              " should be defined before use."},
                             node->Start());
                ok = false;
            }
        }
        node->Iterate(checkDefault);
    };

    if (param->DefaultType() != nullptr) {
        param->DefaultType()->Iterate(checkDefault);
    }

    typeParameterDecls.emplace(typeParamVar);
    return ok;
}

bool ETSChecker::CheckTypeParameterConstraint(ir::TSTypeParameter *param, Type2TypeMap &extends)
{
    const auto typeParamVar = param->Name()->Variable();
    const auto constraintVar = param->Constraint()->AsETSTypeReference()->Part()->Name()->Variable();
    extends.emplace(typeParamVar, constraintVar);
    auto it = extends.find(constraintVar);
    while (it != extends.cend()) {
        if (it->second == typeParamVar) {
            LogTypeError({"Type parameter '", param->Name()->Name().Utf8(), "' has circular constraint dependency."},
                         param->Constraint()->Start());
            return false;
        }
        it = extends.find(it->second);
    }

    return true;
}

void ETSChecker::SetUpTypeParameterConstraint(ir::TSTypeParameter *const param)
{
    ETSTypeParameter *const paramType = param->Name()->Variable()->TsType()->AsETSTypeParameter();
    auto const traverseReferenced =
        [this, scope = param->Parent()->AsTSTypeParameterDeclaration()->Scope()](ir::TypeNode *typeNode) {
            if (!typeNode->IsETSTypeReference()) {
                return;
            }
            const auto typeName = typeNode->AsETSTypeReference()->Part()->Name()->AsIdentifier()->Name();
            auto *const found = scope->FindLocal(typeName, varbinder::ResolveBindingOptions::BINDINGS);
            if (found != nullptr) {
                SetUpTypeParameterConstraint(found->Declaration()->Node()->AsTSTypeParameter());
            }
        };

    if (param->Constraint() != nullptr) {
        traverseReferenced(param->Constraint());
        auto *const constraint = param->Constraint()->GetType(this);
        // invalid: T extends int[]
        if (!constraint->IsETSObjectType() && !constraint->IsETSTypeParameter() && !constraint->IsETSUnionType()) {
            LogTypeError("Extends constraint must be an object", param->Constraint()->Start());
        }
        paramType->SetConstraintType(constraint);
    } else {
        paramType->SetConstraintType(GlobalETSNullishObjectType());
    }

    if (param->DefaultType() != nullptr) {
        traverseReferenced(param->DefaultType());
        // NOTE: #14993 ensure default matches constraint
        paramType->SetDefaultType(MaybeBoxType(param->DefaultType()->GetType(this)));
    }
}

ETSTypeParameter *ETSChecker::SetUpParameterType(ir::TSTypeParameter *const param)
{
    if (param->Name()->Variable() != nullptr && param->Name()->Variable()->TsType() != nullptr) {
        ASSERT(param->Name()->Variable()->TsType()->IsETSTypeParameter());
        return param->Name()->Variable()->TsType()->AsETSTypeParameter();
    }

    auto *const paramType = CreateTypeParameter();

    paramType->AddTypeFlag(TypeFlag::GENERIC);
    paramType->SetDeclNode(param);
    paramType->SetVariable(param->Variable());
    // NOTE: #15026 recursive type parameter workaround
    paramType->SetConstraintType(GlobalETSNullishObjectType());

    param->Name()->Variable()->SetTsType(paramType);
    return paramType;
}

void ETSChecker::CreateTypeForClassOrInterfaceTypeParameters(ETSObjectType *type)
{
    if (type->HasObjectFlag(ETSObjectFlags::RESOLVED_TYPE_PARAMS)) {
        return;
    }

    ir::TSTypeParameterDeclaration *typeParams = type->GetDeclNode()->IsClassDefinition()
                                                     ? type->GetDeclNode()->AsClassDefinition()->TypeParams()
                                                     : type->GetDeclNode()->AsTSInterfaceDeclaration()->TypeParams();
    auto [typeParamTypes, ok] = CreateUnconstrainedTypeParameters(typeParams);
    type->SetTypeArguments(std::move(typeParamTypes));
    if (ok) {
        AssignTypeParameterConstraints(typeParams);
    }
    type->AddObjectFlag(ETSObjectFlags::RESOLVED_TYPE_PARAMS);
    type->AddObjectFlag(ETSObjectFlags::INCOMPLETE_INSTANTIATION);
}

ETSObjectType *ETSChecker::BuildBasicInterfaceProperties(ir::TSInterfaceDeclaration *interfaceDecl)
{
    auto *var = interfaceDecl->Id()->Variable();
    ASSERT(var);

    checker::ETSObjectType *interfaceType {};
    if (var->TsType() == nullptr) {
        interfaceType = CreateETSObjectType(var->Name(), interfaceDecl, checker::ETSObjectFlags::INTERFACE);
        interfaceType->SetVariable(var);
        var->SetTsType(interfaceType);
    } else {
        interfaceType = var->TsType()->AsETSObjectType();
    }

    ConstraintCheckScope ctScope(this);
    if (interfaceDecl->TypeParams() != nullptr) {
        interfaceType->AddTypeFlag(TypeFlag::GENERIC);
        CreateTypeForClassOrInterfaceTypeParameters(interfaceType);
    }

    GetInterfaces(interfaceType);
    interfaceType->SetSuperType(GlobalETSObjectType());
    ctScope.TryCheckConstraints();
    return interfaceType;
}

ETSObjectType *ETSChecker::BuildBasicClassProperties(ir::ClassDefinition *classDef)
{
    if (classDef->IsFinal() && classDef->IsAbstract()) {
        LogTypeError("Cannot use both 'final' and 'abstract' modifiers.", classDef->Start());
    }

    auto *var = classDef->Ident()->Variable();
    ASSERT(var);

    const util::StringView &className = classDef->Ident()->Name();

    checker::ETSObjectType *classType {};
    if (var->TsType() == nullptr) {
        classType = CreateETSObjectType(className, classDef, checker::ETSObjectFlags::CLASS);
        classType->SetVariable(var);
        var->SetTsType(classType);
        if (classDef->IsAbstract()) {
            classType->AddObjectFlag(checker::ETSObjectFlags::ABSTRACT);
        }
    } else {
        classType = var->TsType()->AsETSObjectType();
    }

    classDef->SetTsType(classType);

    ConstraintCheckScope ctScope(this);
    if (classDef->TypeParams() != nullptr) {
        classType->AddTypeFlag(TypeFlag::GENERIC);
        CreateTypeForClassOrInterfaceTypeParameters(classType);
    }

    auto *enclosingClass = Context().ContainingClass();
    classType->SetEnclosingType(enclosingClass);
    CheckerStatus newStatus = CheckerStatus::IN_CLASS;

    if (classDef->IsInner()) {
        newStatus |= CheckerStatus::INNER_CLASS;
        classType->AddObjectFlag(checker::ETSObjectFlags::INNER);
    }

    auto savedContext = checker::SavedCheckerContext(this, newStatus, classType);

    if (!classType->HasObjectFlag(ETSObjectFlags::RESOLVED_SUPER)) {
        GetSuperType(classType);
        GetInterfaces(classType);
    }
    ctScope.TryCheckConstraints();
    return classType;
}

ETSObjectType *ETSChecker::BuildAnonymousClassProperties(ir::ClassDefinition *classDef, ETSObjectType *superType)
{
    auto classType = CreateETSObjectType(classDef->Ident()->Name(), classDef, checker::ETSObjectFlags::CLASS);
    classDef->SetTsType(classType);
    classType->SetSuperType(superType);
    classType->AddObjectFlag(checker::ETSObjectFlags::RESOLVED_SUPER);

    return classType;
}

static void ResolveDeclaredFieldsOfObject(ETSChecker *checker, const ETSObjectType *type, varbinder::ClassScope *scope)
{
    for (auto &[_, it] : scope->InstanceFieldScope()->Bindings()) {
        (void)_;
        ASSERT(it->Declaration()->Node()->IsClassProperty());
        auto *classProp = it->Declaration()->Node()->AsClassProperty();
        it->AddFlag(checker->GetAccessFlagFromNode(classProp));
        type->AddProperty<PropertyType::INSTANCE_FIELD>(it->AsLocalVariable());
    }

    for (auto &[_, it] : scope->StaticFieldScope()->Bindings()) {
        (void)_;
        ASSERT(it->Declaration()->Node()->IsClassProperty());
        auto *classProp = it->Declaration()->Node()->AsClassProperty();
        it->AddFlag(checker->GetAccessFlagFromNode(classProp));
        type->AddProperty<PropertyType::STATIC_FIELD>(it->AsLocalVariable());
    }
}

static void ResolveDeclaredMethodsOfObject(ETSChecker *checker, const ETSObjectType *type, varbinder::ClassScope *scope)
{
    for (auto &[_, it] : scope->InstanceMethodScope()->Bindings()) {
        (void)_;
        auto *method = it->Declaration()->Node()->AsMethodDefinition();
        auto *function = method->Function();

        if (function->IsProxy()) {
            continue;
        }

        it->AddFlag(checker->GetAccessFlagFromNode(method));
        auto *funcType = checker->BuildMethodSignature(method);
        it->SetTsType(funcType);
        funcType->SetVariable(it);
        method->SetTsType(funcType);
        type->AddProperty<PropertyType::INSTANCE_METHOD>(it->AsLocalVariable());
    }

    for (auto &[_, it] : scope->StaticMethodScope()->Bindings()) {
        (void)_;
        if (!it->Declaration()->Node()->IsMethodDefinition()) {
            continue;
        }

        auto *method = it->Declaration()->Node()->AsMethodDefinition();
        auto *function = method->Function();

        if (function->IsProxy()) {
            continue;
        }

        it->AddFlag(checker->GetAccessFlagFromNode(method));
        auto *funcType = checker->BuildMethodSignature(method);
        it->SetTsType(funcType);
        funcType->SetVariable(it);
        method->SetTsType(funcType);

        if (method->IsConstructor()) {
            type->AddConstructSignature(funcType->CallSignatures());
            continue;
        }

        type->AddProperty<PropertyType::STATIC_METHOD>(it->AsLocalVariable());
    }
}

static void ResolveDeclaredDeclsOfObject(ETSChecker *checker, const ETSObjectType *type, varbinder::ClassScope *scope)
{
    for (auto &[_, it] : scope->InstanceDeclScope()->Bindings()) {
        (void)_;
        it->AddFlag(checker->GetAccessFlagFromNode(it->Declaration()->Node()));
        type->AddProperty<PropertyType::INSTANCE_DECL>(it->AsLocalVariable());
    }

    for (auto &[_, it] : scope->StaticDeclScope()->Bindings()) {
        (void)_;
        it->AddFlag(checker->GetAccessFlagFromNode(it->Declaration()->Node()));
        type->AddProperty<PropertyType::STATIC_DECL>(it->AsLocalVariable());
    }
}

void ETSChecker::ResolveDeclaredMembersOfObject(const ETSObjectType *type)
{
    if (type->IsPropertiesInstantiated()) {
        return;
    }

    auto *declNode = type->GetDeclNode();

    if (declNode == nullptr || !(declNode->IsClassDefinition() || declNode->IsTSInterfaceDeclaration())) {
        return;
    }

    if (type->IsGeneric() && type != type->GetOriginalBaseType()) {
        const auto *baseType = type->GetOriginalBaseType();
        auto *baseDeclNode = baseType->GetDeclNode();
        checker::CheckerStatus baseStatus = baseDeclNode->IsTSInterfaceDeclaration()
                                                ? checker::CheckerStatus::IN_INTERFACE
                                                : checker::CheckerStatus::IN_CLASS;
        auto baseScope = baseDeclNode->IsTSInterfaceDeclaration() ? baseDeclNode->AsTSInterfaceDeclaration()->Scope()
                                                                  : baseDeclNode->AsClassDefinition()->Scope();
        auto savedContext = checker::SavedCheckerContext(this, baseStatus, baseType);
        checker::ScopeContext scopeCtx(this, baseScope);
        ResolveDeclaredMembersOfObject(baseType);
        return;
    }

    checker::CheckerStatus status =
        declNode->IsTSInterfaceDeclaration() ? checker::CheckerStatus::IN_INTERFACE : checker::CheckerStatus::IN_CLASS;
    auto *scope = declNode->IsTSInterfaceDeclaration() ? declNode->AsTSInterfaceDeclaration()->Scope()
                                                       : declNode->AsClassDefinition()->Scope();
    auto savedContext = checker::SavedCheckerContext(this, status, type);
    checker::ScopeContext scopeCtx(this, scope);

    ResolveDeclaredFieldsOfObject(this, type, scope->AsClassScope());
    ResolveDeclaredMethodsOfObject(this, type, scope->AsClassScope());
    ResolveDeclaredDeclsOfObject(this, type, scope->AsClassScope());
}

bool ETSChecker::HasETSFunctionType(ir::TypeNode *typeAnnotation)
{
    if (typeAnnotation->IsETSFunctionType()) {
        return true;
    }
    std::unordered_set<ir::TypeNode *> childrenSet;

    if (!typeAnnotation->IsETSTypeReference()) {
        return false;
    }

    auto const addTypeAlias = [&childrenSet, &typeAnnotation](varbinder::Decl *typeDecl) {
        typeAnnotation = typeDecl->Node()->AsTSTypeAliasDeclaration()->TypeAnnotation();
        if (!typeAnnotation->IsETSUnionType()) {
            childrenSet.insert(typeAnnotation);
            return;
        }
        for (auto *type : typeAnnotation->AsETSUnionType()->Types()) {
            if (type->IsETSTypeReference()) {
                childrenSet.insert(type);
            }
        }
    };

    auto *typeDecl = typeAnnotation->AsETSTypeReference()->Part()->Name()->AsIdentifier()->Variable()->Declaration();
    if (typeDecl != nullptr && typeDecl->IsTypeAliasDecl()) {
        addTypeAlias(typeDecl);
    }

    for (auto *child : childrenSet) {
        if (HasETSFunctionType(child)) {
            return true;
        }
    }
    return false;
}

std::vector<Signature *> ETSChecker::CollectAbstractSignaturesFromObject(const ETSObjectType *objType)
{
    std::vector<Signature *> abstracts;
    for (const auto &prop : objType->Methods()) {
        GetTypeOfVariable(prop);

        if (!prop->TsType()->IsETSFunctionType()) {
            continue;
        }

        for (auto *sig : prop->TsType()->AsETSFunctionType()->CallSignatures()) {
            if (sig->HasSignatureFlag(SignatureFlags::ABSTRACT) && !sig->HasSignatureFlag(SignatureFlags::PRIVATE)) {
                abstracts.push_back(sig);
            }
        }
    }

    return abstracts;
}

void ETSChecker::CreateFunctionTypesFromAbstracts(const std::vector<Signature *> &abstracts,
                                                  ArenaVector<ETSFunctionType *> *target)
{
    for (auto *it : abstracts) {
        auto name = it->Function()->Id()->Name();
        auto *found = FindFunctionInVectorGivenByName(name, *target);
        if (found != nullptr) {
            found->AddCallSignature(it);
            continue;
        }

        auto *created = CreateETSFunctionType(it);
        target->push_back(created);
    }
}

void ETSChecker::ComputeAbstractsFromInterface(ETSObjectType *interfaceType)
{
    auto cached = cachedComputedAbstracts_.find(interfaceType);
    if (cached != cachedComputedAbstracts_.end()) {
        return;
    }

    for (auto *it : interfaceType->Interfaces()) {
        ComputeAbstractsFromInterface(it);
    }

    ArenaVector<ETSFunctionType *> merged(Allocator()->Adapter());
    CreateFunctionTypesFromAbstracts(CollectAbstractSignaturesFromObject(interfaceType), &merged);
    ArenaUnorderedSet<ETSObjectType *> abstractInheritanceTarget(Allocator()->Adapter());

    for (auto *interface : interfaceType->Interfaces()) {
        auto found = cachedComputedAbstracts_.find(interface);
        ASSERT(found != cachedComputedAbstracts_.end());

        if (!abstractInheritanceTarget.insert(found->first).second) {
            continue;
        }

        MergeComputedAbstracts(merged, found->second.first);

        for (auto *base : found->second.second) {
            abstractInheritanceTarget.insert(base);
        }
    }

    cachedComputedAbstracts_.insert({interfaceType, {merged, abstractInheritanceTarget}});
}

ArenaVector<ETSFunctionType *> &ETSChecker::GetAbstractsForClass(ETSObjectType *classType)
{
    ArenaVector<ETSFunctionType *> merged(Allocator()->Adapter());
    CreateFunctionTypesFromAbstracts(CollectAbstractSignaturesFromObject(classType), &merged);

    ArenaUnorderedSet<ETSObjectType *> abstractInheritanceTarget(Allocator()->Adapter());
    if (classType->SuperType() != nullptr) {
        auto base = cachedComputedAbstracts_.find(classType->SuperType());
        ASSERT(base != cachedComputedAbstracts_.end());
        MergeComputedAbstracts(merged, base->second.first);

        abstractInheritanceTarget.insert(base->first);
        for (auto *it : base->second.second) {
            abstractInheritanceTarget.insert(it);
        }
    }

    for (auto *it : classType->Interfaces()) {
        ComputeAbstractsFromInterface(it);
        auto found = cachedComputedAbstracts_.find(it);
        ASSERT(found != cachedComputedAbstracts_.end());

        if (!abstractInheritanceTarget.insert(found->first).second) {
            continue;
        }

        MergeComputedAbstracts(merged, found->second.first);

        for (auto *interface : found->second.second) {
            abstractInheritanceTarget.insert(interface);
        }
    }

    return cachedComputedAbstracts_.insert({classType, {merged, abstractInheritanceTarget}}).first->second.first;
}

static bool DoObjectImplementInterface(const ETSObjectType *interfaceType, const ETSObjectType *target)
{
    return std::any_of(interfaceType->Interfaces().begin(), interfaceType->Interfaces().end(),
                       [&target](auto *it) { return it == target || DoObjectImplementInterface(it, target); });
}

static bool CheckIfInterfaceCanBeFoundOnDifferentPaths(const ETSObjectType *classType,
                                                       const ETSObjectType *interfaceType)
{
    return std::count_if(classType->Interfaces().begin(), classType->Interfaces().end(),
                         [&interfaceType](auto *it) { return DoObjectImplementInterface(it, interfaceType); }) == 1;
}

static void GetInterfacesOfClass(ETSObjectType *type, ArenaVector<ETSObjectType *> &interfaces)
{
    for (auto &classInterface : type->Interfaces()) {
        if (std::find(interfaces.begin(), interfaces.end(), classInterface) == interfaces.end()) {
            interfaces.emplace_back(classInterface);
            GetInterfacesOfClass(classInterface, interfaces);
        }
    }
}

void ETSChecker::CheckIfOverrideIsValidInInterface(ETSObjectType *classType, Signature *sig, ir::ScriptFunction *func)
{
    if (AreOverrideEquivalent(func->Signature(), sig) && func->IsStatic() == sig->Function()->IsStatic()) {
        if (CheckIfInterfaceCanBeFoundOnDifferentPaths(classType, func->Signature()->Owner()) &&
            (Relation()->IsSupertypeOf(func->Signature()->Owner(), sig->Owner()) ||
             Relation()->IsSupertypeOf(sig->Owner(), func->Signature()->Owner()))) {
            return;
        }

        LogTypeError({"Method '", sig->Function()->Id()->Name(), "' is declared in ", sig->Owner()->Name(), " and ",
                      func->Signature()->Owner()->Name(), " interfaces."},
                     classType->GetDeclNode()->Start());
    }
}

void ETSChecker::CheckFunctionRedeclarationInInterface(ETSObjectType *classType,
                                                       ArenaVector<Signature *> &similarSignatures,
                                                       ir::ScriptFunction *func)
{
    for (auto *const sig : similarSignatures) {
        if (sig != func->Signature() && func->HasBody()) {
            if (classType == sig->Owner()) {
                return;
            }

            CheckIfOverrideIsValidInInterface(classType, sig, func);
        }
    }

    similarSignatures.push_back(func->Signature());
}

void ETSChecker::CheckInterfaceFunctions(ETSObjectType *classType)
{
    ArenaVector<ETSObjectType *> interfaces(Allocator()->Adapter());
    ArenaVector<Signature *> similarSignatures(Allocator()->Adapter());
    interfaces.emplace_back(classType);
    checker::GetInterfacesOfClass(classType, interfaces);

    for (auto *const &interface : interfaces) {
        for (auto *const &prop : interface->Methods()) {
            ir::AstNode *node = prop->Declaration()->Node();
            ir::ScriptFunction *func = node->AsMethodDefinition()->Function();
            if (func->Body() != nullptr) {
                CheckFunctionRedeclarationInInterface(classType, similarSignatures, func);
            }
        }
    }
}

/// Traverse the interface inheritance tree and collects implemented methods
void ETSChecker::CollectImplementedMethodsFromInterfaces(ETSObjectType *classType,
                                                         std::vector<Signature *> *implementedSignatures,
                                                         const ArenaVector<ETSFunctionType *> &abstractsToBeImplemented)
{
    std::vector<ETSObjectType *> collectedInterfaces;

    for (auto &classInterface : classType->Interfaces()) {
        collectedInterfaces.emplace_back(classInterface);
    }

    size_t index = 0;

    while (index < collectedInterfaces.size()) {
        for (auto &it : abstractsToBeImplemented) {
            for (const auto &prop : collectedInterfaces[index]->Methods()) {
                GetTypeOfVariable(prop);
                AddImplementedSignature(implementedSignatures, prop, it);
            }
        }

        for (auto &currentInterfaceChild : collectedInterfaces[index]->Interfaces()) {
            collectedInterfaces.emplace_back(currentInterfaceChild);
        }

        index++;
    }
}

void ETSChecker::ValidateAbstractSignature(ArenaVector<ETSFunctionType *>::iterator &it,
                                           ArenaVector<ETSFunctionType *> &abstractsToBeImplemented,
                                           const std::vector<Signature *> &implementedSignatures,
                                           bool &functionOverridden, Accessor &isGetSetExternal)
{
    for (auto abstractSignature = (*it)->CallSignatures().begin();
         abstractSignature != (*it)->CallSignatures().end();) {
        bool foundSignature = false;
        isGetSetExternal.isGetter = (*abstractSignature)->HasSignatureFlag(SignatureFlags::GETTER);
        isGetSetExternal.isSetter = (*abstractSignature)->HasSignatureFlag(SignatureFlags::SETTER);
        isGetSetExternal.isExternal = (*abstractSignature)->Function()->IsExternal();
        for (auto *const implemented : implementedSignatures) {
            if (implemented->HasSignatureFlag(SignatureFlags::NEED_RETURN_TYPE)) {
                implemented->OwnerVar()->Declaration()->Node()->Check(this);
            }
            Signature *substImplemented = AdjustForTypeParameters(*abstractSignature, implemented);

            if (substImplemented == nullptr) {
                continue;
            }

            if (!AreOverrideEquivalent(*abstractSignature, substImplemented) ||
                !IsReturnTypeSubstitutable(substImplemented, *abstractSignature)) {
                continue;
            }

            if ((*it)->CallSignatures().size() > 1) {
                abstractSignature = (*it)->CallSignatures().erase(abstractSignature);
                foundSignature = true;
            } else {
                it = abstractsToBeImplemented.erase(it);
                functionOverridden = true;
            }

            break;
        }

        if (functionOverridden) {
            break;
        }

        if (!foundSignature) {
            ++abstractSignature;
        }
    }
}

void ETSChecker::ValidateNonOverriddenFunction(ETSObjectType *classType, ArenaVector<ETSFunctionType *>::iterator &it,
                                               ArenaVector<ETSFunctionType *> &abstractsToBeImplemented,
                                               bool &functionOverridden, const Accessor &isGetSet)
{
    auto superClassType = classType->SuperType();
    while (!functionOverridden && superClassType != nullptr) {
        for (auto *field : superClassType->Fields()) {
            if (field->Name() == (*it)->Name()) {
                auto *newProp =
                    field->Declaration()->Node()->Clone(Allocator(), classType->GetDeclNode())->AsClassProperty();
                newProp->AddModifier(ir::ModifierFlags::SUPER_OWNER);
                newProp->AddModifier(isGetSet.isGetter && isGetSet.isSetter ? ir::ModifierFlags::GETTER_SETTER
                                     : isGetSet.isGetter                    ? ir::ModifierFlags::GETTER
                                                                            : ir::ModifierFlags::SETTER);
                auto *newFieldDecl = Allocator()->New<varbinder::LetDecl>(newProp->Key()->AsIdentifier()->Name());
                newFieldDecl->BindNode(newProp);

                auto newFieldVar = classType->GetDeclNode()
                                       ->Scope()
                                       ->AsClassScope()
                                       ->InstanceFieldScope()
                                       ->AddDecl(Allocator(), newFieldDecl, ScriptExtension::ETS)
                                       ->AsLocalVariable();
                newFieldVar->AddFlag(varbinder::VariableFlags::PROPERTY);
                newFieldVar->AddFlag(varbinder::VariableFlags::PUBLIC);
                classType->AddProperty<PropertyType::INSTANCE_FIELD>(newFieldVar);
                it = abstractsToBeImplemented.erase(it);
                functionOverridden = true;
                break;
            }
        }

        superClassType = superClassType->SuperType();
    }
}

void ETSChecker::ApplyModifiersAndRemoveImplementedAbstracts(ArenaVector<ETSFunctionType *>::iterator &it,
                                                             ArenaVector<ETSFunctionType *> &abstractsToBeImplemented,
                                                             ETSObjectType *classType, bool &functionOverridden,
                                                             const Accessor &isGetSetExternal)
{
    for (auto *field : classType->Fields()) {
        if (field->Name() == (*it)->Name()) {
            field->Declaration()->Node()->AddModifier(isGetSetExternal.isGetter && isGetSetExternal.isSetter
                                                          ? ir::ModifierFlags::GETTER_SETTER
                                                      : isGetSetExternal.isGetter ? ir::ModifierFlags::GETTER
                                                                                  : ir::ModifierFlags::SETTER);
            it = abstractsToBeImplemented.erase(it);
            functionOverridden = true;
            break;
        }
    }
}

void ETSChecker::ValidateAbstractMethodsToBeImplemented(ArenaVector<ETSFunctionType *> &abstractsToBeImplemented,
                                                        ETSObjectType *classType,
                                                        const std::vector<Signature *> &implementedSignatures)
{
    SavedTypeRelationFlagsContext savedFlagsCtx(Relation(), TypeRelationFlag::NO_RETURN_TYPE_CHECK);
    for (auto it = abstractsToBeImplemented.begin(); it != abstractsToBeImplemented.end();) {
        bool functionOverridden = false;
        Accessor isGetSetExternal;

        ValidateAbstractSignature(it, abstractsToBeImplemented, implementedSignatures, functionOverridden,
                                  isGetSetExternal);

        if (functionOverridden) {
            continue;
        }

        if (!isGetSetExternal.isGetter && !isGetSetExternal.isSetter) {
            it++;
            continue;
        }

        ApplyModifiersAndRemoveImplementedAbstracts(it, abstractsToBeImplemented, classType, functionOverridden,
                                                    isGetSetExternal);

        if (functionOverridden) {
            continue;
        }

        ValidateNonOverriddenFunction(classType, it, abstractsToBeImplemented, functionOverridden, isGetSetExternal);

        if (!functionOverridden) {
            it++;
        }
    }
}

void ETSChecker::MaybeReportErrorsForOverridingValidation(ArenaVector<ETSFunctionType *> &abstractsToBeImplemented,
                                                          ETSObjectType *classType, const lexer::SourcePosition &pos,
                                                          bool reportError)
{
    if (!abstractsToBeImplemented.empty() && reportError) {
        auto unimplementedSignature = abstractsToBeImplemented.front()->CallSignatures().front();
        auto containingObjectName = GetContainingObjectNameFromSignature(unimplementedSignature);
        if (unimplementedSignature->HasSignatureFlag(SignatureFlags::GETTER)) {
            LogTypeError({classType->Name(), " is not abstract and does not implement getter for ",
                          unimplementedSignature->Function()->Id()->Name(), " property in ", containingObjectName},
                         pos);
            return;
        }
        if (unimplementedSignature->HasSignatureFlag(SignatureFlags::SETTER)) {
            LogTypeError({classType->Name(), " is not abstract and does not implement setter for ",
                          unimplementedSignature->Function()->Id()->Name(), " property in ", containingObjectName},
                         pos);
            return;
        }
        LogTypeError({classType->Name(), " is not abstract and does not override abstract method ",
                      unimplementedSignature->Function()->Id()->Name(), unimplementedSignature, " in ",
                      containingObjectName},
                     pos);
    }
}

void ETSChecker::ValidateOverriding(ETSObjectType *classType, const lexer::SourcePosition &pos)
{
    if (classType->HasObjectFlag(ETSObjectFlags::CHECKED_COMPATIBLE_ABSTRACTS)) {
        return;
    }

    bool throwError = true;
    if (classType->HasObjectFlag(ETSObjectFlags::ABSTRACT)) {
        throwError = false;
    }

    if (classType->SuperType() != nullptr) {
        ValidateOverriding(classType->SuperType(), classType->SuperType()->GetDeclNode()->Start());
    }

    auto &abstractsToBeImplemented = GetAbstractsForClass(classType);
    std::vector<Signature *> implementedSignatures;

    // Since interfaces can define function bodies we have to collect the implemented ones first
    CollectImplementedMethodsFromInterfaces(classType, &implementedSignatures, abstractsToBeImplemented);
    CheckInterfaceFunctions(classType);

    auto *superIter = classType;
    do {
        for (auto &it : abstractsToBeImplemented) {
            for (const auto &prop : superIter->Methods()) {
                GetTypeOfVariable(prop);
                AddImplementedSignature(&implementedSignatures, prop, it);
            }
        }
        superIter = superIter->SuperType();
    } while (superIter != nullptr);
    ValidateAbstractMethodsToBeImplemented(abstractsToBeImplemented, classType, implementedSignatures);
    MaybeReportErrorsForOverridingValidation(abstractsToBeImplemented, classType, pos, throwError);

    classType->AddObjectFlag(ETSObjectFlags::CHECKED_COMPATIBLE_ABSTRACTS);
}

void ETSChecker::AddImplementedSignature(std::vector<Signature *> *implementedSignatures,
                                         varbinder::LocalVariable *function, ETSFunctionType *it)
{
    if (!function->TsType()->IsETSFunctionType()) {
        return;
    }

    for (auto signature : function->TsType()->AsETSFunctionType()->CallSignatures()) {
        if (signature->Function()->IsAbstract() || signature->Function()->IsStatic()) {
            continue;
        }

        if (signature->Function()->Id()->Name() == it->Name()) {
            implementedSignatures->emplace_back(signature);
        }
    }
}

void ETSChecker::CheckLocalClass(ir::ClassDefinition *classDef, CheckerStatus &checkerStatus)
{
    if (classDef->IsLocal()) {
        checkerStatus |= CheckerStatus::IN_LOCAL_CLASS;
        if (!classDef->Parent()->Parent()->IsBlockStatement()) {
            LogTypeError("Local classes must be defined between balanced braces", classDef->Start());
        }
    }
}

void ETSChecker::CheckClassDefinition(ir::ClassDefinition *classDef)
{
    classDef->SetClassDefinitionChecked();
    auto *classType = classDef->TsType()->AsETSObjectType();
    if (classType->SuperType() != nullptr) {
        classType->SuperType()->GetDeclNode()->Check(this);
    }

    auto newStatus = checker::CheckerStatus::IN_CLASS;
    if (Context().ContainingClass() != classType) {
        classType->SetEnclosingType(Context().ContainingClass());
    }

    if (classDef->IsInner()) {
        newStatus |= CheckerStatus::INNER_CLASS;
        classType->AddObjectFlag(checker::ETSObjectFlags::INNER);
    }

    classDef->IsGlobal() ? classType->AddObjectFlag(checker::ETSObjectFlags::GLOBAL)
                         : CheckLocalClass(classDef, newStatus);

    checker::ScopeContext scopeCtx(this, classDef->Scope());
    auto savedContext = SavedCheckerContext(this, newStatus, classType);

    ResolveDeclaredMembersOfObject(classType);

    if (classDef->IsAbstract()) {
        AddStatus(checker::CheckerStatus::IN_ABSTRACT);
        classType->AddObjectFlag(checker::ETSObjectFlags::ABSTRACT);
    }

    if (classDef->IsStatic() && !Context().ContainingClass()->HasObjectFlag(ETSObjectFlags::GLOBAL)) {
        AddStatus(checker::CheckerStatus::IN_STATIC_CONTEXT);
    }

    // NOTE(gogabr): temporary, until we have proper bridges, see #16485
    // Don't check overriding for synthetic functional classes.
    if ((static_cast<ir::AstNode *>(classDef)->Modifiers() & ir::ModifierFlags::FUNCTIONAL) == 0) {
        ValidateOverriding(classType, classDef->Start());
    }
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    TransformProperties(classType);

    CheckClassAnnotations(classDef);
    CheckClassMembers(classDef);

    if (classDef->IsGlobal() || classType->SuperType() == nullptr) {
        return;
    }

    CheckConstructors(classDef, classType);
    CheckValidInheritance(classType, classDef);
    CheckConstFields(classType);
    CheckGetterSetterProperties(classType);
    CheckInvokeMethodsLegitimacy(classType);
}

void ETSChecker::CheckClassAnnotations(ir::ClassDefinition *classDef)
{
    if (!CheckDuplicateAnnotations(classDef->Annotations())) {
        return;
    }
    for (auto *it : classDef->Annotations()) {
        if (!it->IsClassProperty()) {
            it->Check(this);
        }
    }
}

void ETSChecker::CheckClassMembers(ir::ClassDefinition *classDef)
{
    for (auto *it : classDef->Body()) {
        if (it->IsClassProperty()) {
            it->Check(this);
        }
    }

    for (auto *it : classDef->Body()) {
        if (!it->IsClassProperty()) {
            it->Check(this);
        }
    }
}

void ETSChecker::CheckConstructors(ir::ClassDefinition *classDef, ETSObjectType *classType)
{
    if (!classDef->IsDeclare()) {
        for (auto *it : classType->ConstructSignatures()) {
            CheckCyclicConstructorCall(it);
            CheckImplicitSuper(classType, it);
            CheckThisOrSuperCallInConstructor(classType, it);
        }
    }
}

bool IsAsyncMethod(ir::AstNode *node)
{
    if (!node->IsMethodDefinition()) {
        return false;
    }
    auto *method = node->AsMethodDefinition();
    return method->Function()->IsAsyncFunc() && !method->Function()->IsProxy();
}

void ETSChecker::CreateAsyncProxyMethods(ir::ClassDefinition *classDef)
{
    ArenaVector<ir::MethodDefinition *> asyncImpls(Allocator()->Adapter());

    for (auto *it : classDef->Body()) {
        if (!IsAsyncMethod(it)) {
            continue;
        }

        auto *asyncMethod = it->AsMethodDefinition();
        auto *proxy = CreateAsyncProxy(asyncMethod, classDef);
        asyncImpls.push_back(proxy);

        for (auto *overload : asyncMethod->Overloads()) {
            if (!IsAsyncMethod(overload)) {
                continue;
            }

            auto *impl = CreateAsyncProxy(overload, classDef, false);
            impl->Function()->Id()->SetVariable(proxy->Function()->Id()->Variable());
            proxy->AddOverload(impl);
        }
    }

    for (auto *it : asyncImpls) {
        it->SetParent(classDef);
        it->Check(this);
        classDef->Body().push_back(it);
    }
}

void ETSChecker::CheckImplicitSuper(ETSObjectType *classType, Signature *ctorSig)
{
    if (classType == GlobalETSObjectType()) {
        return;
    }

    auto &stmts = ctorSig->Function()->Body()->AsBlockStatement()->Statements();
    const auto thisCall = std::find_if(stmts.begin(), stmts.end(), [](const ir::Statement *stmt) {
        return stmt->IsExpressionStatement() && stmt->AsExpressionStatement()->GetExpression()->IsCallExpression() &&
               stmt->AsExpressionStatement()->GetExpression()->AsCallExpression()->Callee()->IsThisExpression();
    });
    // There is an alternate constructor invocation, no need for super constructor invocation
    if (thisCall != stmts.end()) {
        return;
    }

    const auto superExpr = std::find_if(stmts.begin(), stmts.end(), [](const ir::Statement *stmt) {
        return stmt->IsExpressionStatement() && stmt->AsExpressionStatement()->GetExpression()->IsCallExpression() &&
               stmt->AsExpressionStatement()->GetExpression()->AsCallExpression()->Callee()->IsSuperExpression();
    });
    // There is no super expression
    if (superExpr == stmts.end()) {
        const auto superTypeCtorSigs = classType->SuperType()->ConstructSignatures();
        const auto superTypeCtorSig = std::find_if(superTypeCtorSigs.begin(), superTypeCtorSigs.end(),
                                                   [](const Signature *sig) { return sig->Params().empty(); });
        // Super type has no parameterless ctor
        if (superTypeCtorSig == superTypeCtorSigs.end()) {
            LogTypeError("Must call super constructor", ctorSig->Function()->Start());
        }

        ctorSig->Function()->AddFlag(ir::ScriptFunctionFlags::IMPLICIT_SUPER_CALL_NEEDED);
    }
}

void ETSChecker::CheckThisOrSuperCallInConstructor(ETSObjectType *classType, Signature *ctorSig)
{
    if (classType == GlobalETSObjectType()) {
        return;
    }

    for (auto it : ctorSig->Function()->Body()->AsBlockStatement()->Statements()) {
        if (it->IsExpressionStatement() && it->AsExpressionStatement()->GetExpression()->IsCallExpression() &&
            (it->AsExpressionStatement()->GetExpression()->AsCallExpression()->Callee()->IsThisExpression() ||
             it->AsExpressionStatement()->GetExpression()->AsCallExpression()->Callee()->IsSuperExpression())) {
            ArenaVector<const ir::Expression *> expressions =
                ArenaVector<const ir::Expression *>(Allocator()->Adapter());
            expressions.insert(expressions.end(),
                               it->AsExpressionStatement()->GetExpression()->AsCallExpression()->Arguments().begin(),
                               it->AsExpressionStatement()->GetExpression()->AsCallExpression()->Arguments().end());
            CheckExpressionsInConstructor(expressions);
        }
    }
}

void ETSChecker::CheckExpressionsInConstructor(const ArenaVector<const ir::Expression *> &arguments)
{
    for (auto *arg : arguments) {
        auto expressions = CheckMemberOrCallOrObjectExpressionInConstructor(arg);

        if (arg->IsETSNewClassInstanceExpression()) {
            expressions.insert(expressions.end(), arg->AsETSNewClassInstanceExpression()->GetArguments().begin(),
                               arg->AsETSNewClassInstanceExpression()->GetArguments().end());
        } else if (arg->IsArrayExpression()) {
            expressions.insert(expressions.end(), arg->AsArrayExpression()->Elements().begin(),
                               arg->AsArrayExpression()->Elements().end());
        } else if (arg->IsBinaryExpression()) {
            expressions.push_back(arg->AsBinaryExpression()->Left());
            expressions.push_back(arg->AsBinaryExpression()->Right());
        } else if (arg->IsAssignmentExpression()) {
            expressions.push_back(arg->AsAssignmentExpression()->Left());
            expressions.push_back(arg->AsAssignmentExpression()->Right());
        } else if (arg->IsTSAsExpression()) {
            expressions.push_back(arg->AsTSAsExpression()->Expr());
        } else if (arg->IsConditionalExpression()) {
            expressions.push_back(arg->AsConditionalExpression()->Test());
            expressions.push_back(arg->AsConditionalExpression()->Consequent());
            expressions.push_back(arg->AsConditionalExpression()->Alternate());
        } else if (arg->IsTypeofExpression()) {
            expressions.push_back(arg->AsTypeofExpression()->Argument());
        } else if (arg->IsTSNonNullExpression()) {
            expressions.push_back(arg->AsTSNonNullExpression()->Expr());
        } else if (arg->IsUnaryExpression()) {
            expressions.push_back(arg->AsUnaryExpression()->Argument());
        } else if (arg->IsUpdateExpression()) {
            expressions.push_back(arg->AsUpdateExpression()->Argument());
        }

        if (!expressions.empty()) {
            CheckExpressionsInConstructor(expressions);
        }
    }
}

ArenaVector<const ir::Expression *> ETSChecker::CheckMemberOrCallOrObjectExpressionInConstructor(
    const ir::Expression *arg)
{
    ArenaVector<const ir::Expression *> expressions = ArenaVector<const ir::Expression *>(Allocator()->Adapter());

    if (arg->IsMemberExpression()) {
        if ((arg->AsMemberExpression()->Object()->IsSuperExpression() ||
             arg->AsMemberExpression()->Object()->IsThisExpression())) {
            std::stringstream ss;
            ss << "Using " << (arg->AsMemberExpression()->Object()->IsSuperExpression() ? "super" : "this")
               << " is not allowed in constructor";
            LogTypeError(ss.str(), arg->Start());
        }

        expressions.push_back(arg->AsMemberExpression()->Property());
        expressions.push_back(arg->AsMemberExpression()->Object());
    } else if (arg->IsCallExpression()) {
        expressions.insert(expressions.end(), arg->AsCallExpression()->Arguments().begin(),
                           arg->AsCallExpression()->Arguments().end());

        if (arg->AsCallExpression()->Callee()->IsMemberExpression() &&
            (arg->AsCallExpression()->Callee()->AsMemberExpression()->Object()->IsSuperExpression() ||
             arg->AsCallExpression()->Callee()->AsMemberExpression()->Object()->IsThisExpression()) &&
            !arg->AsCallExpression()->Callee()->AsMemberExpression()->Property()->IsStatic()) {
            std::stringstream ss;
            ss << "Using "
               << (arg->AsCallExpression()->Callee()->AsMemberExpression()->IsSuperExpression() ? "super" : "this")
               << " is not allowed in constructor";
            LogTypeError(ss.str(), arg->Start());
        }
    } else if (arg->IsObjectExpression()) {
        for (auto *prop : arg->AsObjectExpression()->Properties()) {
            expressions.push_back(prop->AsProperty()->Value());
        }
    }

    return expressions;
}

void ETSChecker::CheckConstFields(const ETSObjectType *classType)
{
    for (const auto &prop : classType->Fields()) {
        if (!(prop->Declaration()->IsConstDecl() || prop->Declaration()->IsReadonlyDecl()) ||
            !prop->HasFlag(varbinder::VariableFlags::EXPLICIT_INIT_REQUIRED)) {
            continue;
        }
        CheckConstFieldInitialized(classType, prop);
    }
}

void ETSChecker::CheckConstFieldInitialized(const ETSObjectType *classType, varbinder::LocalVariable *classVar)
{
    const bool classVarStatic = classVar->Declaration()->Node()->AsClassProperty()->IsStatic();
    for (const auto &prop : classType->Methods()) {
        if (!prop->TsType()->IsETSFunctionType()) {
            continue;
        }

        const auto &callSigs = prop->TsType()->AsETSFunctionType()->CallSignatures();
        for (const auto *signature : callSigs) {
            if ((signature->Function()->IsConstructor() && !classVarStatic) ||
                (signature->Function()->IsStaticBlock() && classVarStatic)) {
                CheckConstFieldInitialized(signature, classVar);
            }
        }
    }
}

void ETSChecker::FindAssignment(const ir::AstNode *node, const varbinder::LocalVariable *classVar, bool &initialized)
{
    if (node->IsAssignmentExpression() && node->AsAssignmentExpression()->Target() == classVar) {
        if (initialized) {
            LogTypeError({"Variable '", classVar->Declaration()->Name(), "' might already have been initialized"},
                         node->Start());
        }

        initialized = true;
        return;
    }

    FindAssignments(node, classVar, initialized);
}

void ETSChecker::FindAssignments(const ir::AstNode *node, const varbinder::LocalVariable *classVar, bool &initialized)
{
    node->Iterate(
        [this, classVar, &initialized](ir::AstNode *childNode) { FindAssignment(childNode, classVar, initialized); });
}

void ETSChecker::CheckConstFieldInitialized(const Signature *signature, varbinder::LocalVariable *classVar)
{
    bool initialized = false;
    const auto &stmts = signature->Function()->Body()->AsBlockStatement()->Statements();
    const auto it = stmts.begin();
    if (it != stmts.end()) {
        if (const auto *first = *it;
            first->IsExpressionStatement() && first->AsExpressionStatement()->GetExpression()->IsCallExpression() &&
            first->AsExpressionStatement()->GetExpression()->AsCallExpression()->Callee()->IsThisExpression()) {
            initialized = true;
        }
    }

    // NOTE: szd. control flow
    FindAssignments(signature->Function()->Body(), classVar, initialized);
    if (!initialized) {
        LogTypeError({"Variable '", classVar->Declaration()->Name(), "' might not have been initialized"},
                     signature->Function()->End());
    }

    classVar->RemoveFlag(varbinder::VariableFlags::EXPLICIT_INIT_REQUIRED);
}

void ETSChecker::CheckInnerClassMembers(const ETSObjectType *classType)
{
    for (const auto &[_, it] : classType->StaticMethods()) {
        (void)_;
        LogTypeError("Inner class cannot have static methods", it->Declaration()->Node()->Start());
    }

    for (const auto &[_, it] : classType->StaticFields()) {
        (void)_;
        if (!it->Declaration()->IsReadonlyDecl()) {
            LogTypeError("Inner class cannot have non-readonly static properties", it->Declaration()->Node()->Start());
        }
    }
}

bool ETSChecker::ValidateArrayIndex(ir::Expression *const expr, bool relaxed)
{
    auto *const expressionType = expr->Check(this);
    auto const *const unboxedExpressionType = MaybeUnboxInRelation(expressionType);

    Type const *const indexType = ApplyUnaryOperatorPromotion(expressionType);

    if (expressionType->IsETSObjectType() && (unboxedExpressionType != nullptr)) {
        expr->AddBoxingUnboxingFlags(GetUnboxingFlag(unboxedExpressionType));
    }

    if (relaxed && indexType != nullptr && indexType->HasTypeFlag(TypeFlag::ETS_FLOATING_POINT)) {
        if (!expr->IsNumberLiteral()) {
            return true;
        }

        auto num = expr->AsNumberLiteral()->Number();
        ASSERT(num.IsReal());
        double value = num.GetDouble();
        double intpart;
        if (std::modf(value, &intpart) != 0.0) {
            LogTypeError("Index fractional part should be zero.", expr->Start());
            return false;
        }
        return true;
    }

    if (indexType == nullptr || !indexType->HasTypeFlag(TypeFlag::ETS_ARRAY_INDEX)) {
        std::stringstream message("");
        expressionType->ToString(message);

        LogTypeError(
            "Type '" + message.str() +
                "' cannot be used as an index type. Only primitive or unboxable integral types can be used as index.",
            expr->Start());
        return false;
    }

    return true;
}

std::optional<int32_t> ETSChecker::GetTupleElementAccessValue(const Type *const type, const lexer::SourcePosition &pos)
{
    ASSERT(type->HasTypeFlag(TypeFlag::CONSTANT | TypeFlag::ETS_CONVERTIBLE_TO_NUMERIC));

    switch (ETSType(type)) {
        case TypeFlag::BYTE: {
            return type->AsByteType()->GetValue();
        }
        case TypeFlag::SHORT: {
            return type->AsShortType()->GetValue();
        }
        case TypeFlag::INT: {
            return type->AsIntType()->GetValue();
        }
        case TypeFlag::LONG: {
            if (auto val = type->AsLongType()->GetValue();
                val <= std::numeric_limits<int32_t>::max() && val >= std::numeric_limits<int32_t>::min()) {
                return static_cast<int32_t>(val);
            }

            LogTypeError("Element accessor value is out of tuple size bounds.", pos);
            return std::nullopt;
        }
        default: {
            UNREACHABLE();
        }
    }
}

bool ETSChecker::ValidateTupleIndex(const ETSTupleType *const tuple, ir::MemberExpression *const expr)
{
    auto *const expressionType = expr->Property()->Check(this);
    auto const *const unboxedExpressionType = MaybeUnboxInRelation(expressionType);

    if (expressionType->IsETSObjectType() && (unboxedExpressionType != nullptr)) {
        expr->AddBoxingUnboxingFlags(GetUnboxingFlag(unboxedExpressionType));
    }

    const auto *const exprType = expr->Property()->TsType();
    ASSERT(exprType != nullptr);

    if (!exprType->HasTypeFlag(TypeFlag::CONSTANT) && !tuple->HasSpreadType()) {
        LogTypeError("Only constant expression allowed for element access on tuples.", expr->Property()->Start());
        return false;
    }

    if (!exprType->HasTypeFlag(TypeFlag::ETS_ARRAY_INDEX | TypeFlag::LONG)) {
        LogTypeError("Only integer type allowed for element access on tuples.", expr->Property()->Start());
        return false;
    }

    auto exprValue = GetTupleElementAccessValue(exprType, expr->Property()->Start());
    if (!exprValue.has_value()) {
        return false;  // spread the error
    }
    if (((*exprValue >= tuple->GetTupleSize()) && !tuple->HasSpreadType()) || (*exprValue < 0)) {
        LogTypeError("Element accessor value is out of tuple size bounds.", expr->Property()->Start());
        return false;
    }

    return true;
}

ETSObjectType *ETSChecker::CheckThisOrSuperAccess(ir::Expression *node, ETSObjectType *classType, std::string_view msg)
{
    if ((Context().Status() & CheckerStatus::IGNORE_VISIBILITY) != 0U) {
        return classType;
    }

    if (node->Parent()->IsCallExpression() && (node->Parent()->AsCallExpression()->Callee() == node)) {
        if (Context().ContainingSignature() == nullptr) {
            LogTypeError({"Call to '", msg, "' must be first statement in constructor"}, node->Start());
            return classType;
        }

        auto *sig = Context().ContainingSignature();
        ASSERT(sig->Function()->Body() && sig->Function()->Body()->IsBlockStatement());

        if (!sig->HasSignatureFlag(checker::SignatureFlags::CONSTRUCT)) {
            LogTypeError({"Call to '", msg, "' must be first statement in constructor"}, node->Start());
            return classType;
        }

        if (sig->Function()->Body()->AsBlockStatement()->Statements().front() != node->Parent()->Parent()) {
            LogTypeError({"Call to '", msg, "' must be first statement in constructor"}, node->Start());
            return classType;
        }
    }

    if (HasStatus(checker::CheckerStatus::IN_STATIC_CONTEXT)) {
        LogTypeError({"'", msg, "' cannot be referenced from a static context"}, node->Start());
        return classType;
    }

    if (classType->GetDeclNode()->IsClassDefinition() && classType->GetDeclNode()->AsClassDefinition()->IsGlobal()) {
        LogTypeError({"Cannot reference '", msg, "' in this context."}, node->Start());
        return classType;
    }

    return classType;
}

void ETSChecker::CheckCyclicConstructorCall(Signature *signature)
{
    ASSERT(signature->Function());

    if (signature->Function()->Body() == nullptr || signature->Function()->IsExternal()) {
        return;
    }

    auto *funcBody = signature->Function()->Body()->AsBlockStatement();

    TypeStackElement tse(this, signature, "Recursive constructor invocation", signature->Function()->Start());
    if (tse.HasTypeError()) {
        return;
    }

    if (!funcBody->Statements().empty() && funcBody->Statements()[0]->IsExpressionStatement() &&
        funcBody->Statements()[0]->AsExpressionStatement()->GetExpression()->IsCallExpression() &&
        funcBody->Statements()[0]
            ->AsExpressionStatement()  // CC-OFF(G.FMT.06-CPP,G.FMT.02-CPP) project code style
            ->GetExpression()
            ->AsCallExpression()
            ->Callee()
            ->IsThisExpression()) {
        auto *constructorCall = funcBody->Statements()[0]->AsExpressionStatement()->GetExpression()->AsCallExpression();
        ASSERT(constructorCall->Signature());
        CheckCyclicConstructorCall(constructorCall->Signature());
    }
}

ETSObjectType *ETSChecker::CheckExceptionOrErrorType(checker::Type *type, const lexer::SourcePosition pos)
{
    if (!type->IsETSObjectType() || (!Relation()->IsAssignableTo(type, GlobalBuiltinExceptionType()) &&
                                     !Relation()->IsAssignableTo(type, GlobalBuiltinErrorType()))) {
        LogTypeError({"Argument must be an instance of '", compiler::Signatures::BUILTIN_EXCEPTION_CLASS, "' or '",
                      compiler::Signatures::BUILTIN_ERROR_CLASS, "'"},
                     pos);
        return GlobalETSObjectType();
    }

    return type->AsETSObjectType();
}

Type *ETSChecker::TryToInstantiate(Type *const type, ArenaAllocator *const allocator, TypeRelation *const relation,
                                   GlobalTypesHolder *const globalTypes)
{
    // NOTE: Handle generic functions
    auto *returnType = type;
    const bool isIncomplete =
        type->IsETSObjectType() && type->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::INCOMPLETE_INSTANTIATION);
    if (const bool isFunctionType = type->IsETSFunctionType(); isFunctionType || isIncomplete) {
        returnType = type->Instantiate(allocator, relation, globalTypes);
    }

    return returnType;
}

void ETSChecker::ValidateResolvedProperty(varbinder::LocalVariable **property, const ETSObjectType *const target,
                                          const ir::Identifier *const ident, const PropertySearchFlags flags)
{
    if (*property != nullptr) {
        return;
    }

    using Utype = std::underlying_type_t<PropertySearchFlags>;
    static constexpr uint32_t CORRECT_PROPERTY_SEARCH_ORDER_INSTANCE = 7U;
    static_assert(static_cast<Utype>(PropertySearchFlags::SEARCH_INSTANCE) == CORRECT_PROPERTY_SEARCH_ORDER_INSTANCE,
                  "PropertySearchFlags order changed");
    static constexpr uint32_t CORRECT_PROPERTY_SEARCH_ORDER_STATIC = 56U;
    static_assert(static_cast<Utype>(PropertySearchFlags::SEARCH_STATIC) == CORRECT_PROPERTY_SEARCH_ORDER_STATIC,
                  "PropertySearchFlags order changed");
    const auto flagsNum = static_cast<Utype>(flags);
    // This algorithm swaps the first 3 bits of a number with it's consecutive 3 bits, example: 0b110001 -> 0b001110
    // Effectively it changes PropertySearchFlags to search for the appropriate declarations
    const Utype x = (flagsNum ^ (flagsNum >> 3U)) & 7U;
    const auto newFlags = PropertySearchFlags {flagsNum ^ (x | (x << 3U))};

    auto *const newProp = target->GetProperty(ident->Name(), newFlags);
    if (newProp == nullptr) {
        LogTypeError({"Property '", ident->Name(), "' does not exist on type '", target->Name(), "'"}, ident->Start());
        return;
    }

    *property = newProp;  // trying to recover as much as possible; log the error but treat the property as legal

    if (IsVariableStatic(newProp)) {
        LogTypeError({"'", ident->Name(), "' is a static property of '", target->Name(), "'"}, ident->Start());
        return;
    }

    LogTypeError({"'", ident->Name(), "' is an instance property of '", target->Name(), "'"}, ident->Start());
}

varbinder::Variable *ETSChecker::ResolveInstanceExtension(const ir::MemberExpression *const memberExpr)
{
    // clang-format off
    auto *globalFunctionVar = Scope()
                                ->FindInGlobal(memberExpr->Property()->AsIdentifier()->Name(),
                                                // CC-OFFNXT(G.FMT.06-CPP) project code style
                                                varbinder::ResolveBindingOptions::STATIC_METHODS)
                                .variable;
    // clang-format on

    if (globalFunctionVar == nullptr || !ExtensionETSFunctionType(this->GetTypeOfVariable(globalFunctionVar))) {
        return nullptr;
    }

    return globalFunctionVar;
}

PropertySearchFlags ETSChecker::GetInitialSearchFlags(const ir::MemberExpression *const memberExpr)
{
    constexpr auto FUNCTIONAL_FLAGS =
        PropertySearchFlags::SEARCH_METHOD | PropertySearchFlags::IS_FUNCTIONAL | PropertySearchFlags::SEARCH_FIELD;
    constexpr auto GETTER_FLAGS = PropertySearchFlags::SEARCH_METHOD | PropertySearchFlags::IS_GETTER;
    constexpr auto SETTER_FLAGS = PropertySearchFlags::SEARCH_METHOD | PropertySearchFlags::IS_SETTER;

    switch (memberExpr->Parent()->Type()) {
        case ir::AstNodeType::CALL_EXPRESSION: {
            if (memberExpr->Parent()->AsCallExpression()->Callee() == memberExpr) {
                return FUNCTIONAL_FLAGS;
            }

            break;
        }
        case ir::AstNodeType::ETS_NEW_CLASS_INSTANCE_EXPRESSION: {
            if (memberExpr->Parent()->AsETSNewClassInstanceExpression()->GetTypeRef() == memberExpr) {
                return PropertySearchFlags::SEARCH_DECL;
            }
            break;
        }
        case ir::AstNodeType::MEMBER_EXPRESSION: {
            return PropertySearchFlags::SEARCH_FIELD | PropertySearchFlags::SEARCH_DECL | GETTER_FLAGS;
        }
        case ir::AstNodeType::UPDATE_EXPRESSION:
        case ir::AstNodeType::UNARY_EXPRESSION:
        case ir::AstNodeType::BINARY_EXPRESSION: {
            return PropertySearchFlags::SEARCH_FIELD | GETTER_FLAGS;
        }
        case ir::AstNodeType::ASSIGNMENT_EXPRESSION: {
            const auto *const assignmentExpr = memberExpr->Parent()->AsAssignmentExpression();

            if (assignmentExpr->Left() == memberExpr) {
                if (assignmentExpr->OperatorType() == lexer::TokenType::PUNCTUATOR_SUBSTITUTION) {
                    return PropertySearchFlags::SEARCH_FIELD | SETTER_FLAGS;
                }
                return PropertySearchFlags::SEARCH_FIELD | GETTER_FLAGS | SETTER_FLAGS;
            }

            auto const *targetType = assignmentExpr->Left()->TsType();
            if (targetType->IsETSObjectType() &&
                targetType->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::FUNCTIONAL)) {
                return FUNCTIONAL_FLAGS;
            }

            return PropertySearchFlags::SEARCH_FIELD | GETTER_FLAGS;
        }
        default: {
            break;
        }
    }

    return PropertySearchFlags::SEARCH_FIELD | FUNCTIONAL_FLAGS | GETTER_FLAGS;
}

PropertySearchFlags ETSChecker::GetSearchFlags(const ir::MemberExpression *const memberExpr,
                                               const varbinder::Variable *targetRef)
{
    auto searchFlag = GetInitialSearchFlags(memberExpr);
    searchFlag |= PropertySearchFlags::SEARCH_IN_BASE | PropertySearchFlags::SEARCH_IN_INTERFACES;
    if (targetRef != nullptr &&
        (targetRef->HasFlag(varbinder::VariableFlags::CLASS_OR_INTERFACE) ||
         (targetRef->HasFlag(varbinder::VariableFlags::TYPE_ALIAS) &&
          targetRef->TsType()->Variable()->HasFlag(varbinder::VariableFlags::CLASS_OR_INTERFACE)))) {
        searchFlag &= ~PropertySearchFlags::SEARCH_INSTANCE;
    } else if (memberExpr->Object()->IsThisExpression() ||
               (targetRef != nullptr && targetRef->Declaration() != nullptr &&
                targetRef->Declaration()->IsLetOrConstDecl()) ||
               (memberExpr->Object()->IsIdentifier() && memberExpr->ObjType()->GetDeclNode() != nullptr &&
                memberExpr->ObjType()->GetDeclNode()->IsTSInterfaceDeclaration())) {
        searchFlag &= ~PropertySearchFlags::SEARCH_STATIC;
    }
    return searchFlag;
}

const varbinder::Variable *ETSChecker::GetTargetRef(const ir::MemberExpression *const memberExpr)
{
    if (memberExpr->Object()->IsIdentifier()) {
        return memberExpr->Object()->AsIdentifier()->Variable();
    }
    if (memberExpr->Object()->IsMemberExpression()) {
        return memberExpr->Object()->AsMemberExpression()->PropVar();
    }
    return nullptr;
}

void ETSChecker::ValidateReadonlyProperty(const ir::MemberExpression *const memberExpr, const ETSFunctionType *propType,
                                          const lexer::SourcePosition sourcePos)
{
    ir::ClassProperty *classProp = nullptr;
    ETSObjectType *currentObj = memberExpr->ObjType();
    bool foundInThis = false;
    while (classProp == nullptr && currentObj != nullptr) {
        classProp = FindClassProperty(currentObj, propType);
        if (classProp != nullptr && currentObj == memberExpr->ObjType()) {
            foundInThis = true;
        }

        currentObj = currentObj->SuperType();
    }

    if (classProp != nullptr && this->Context().ContainingSignature() != nullptr && classProp->IsReadonly()) {
        if (!foundInThis || (!this->Context().ContainingSignature()->Function()->IsConstructor())) {
            LogTypeError("Cannot assign to this property because it is readonly.", sourcePos);
            return;
        }

        if (IsInitializedProperty(memberExpr->ObjType()->GetDeclNode()->AsClassDefinition(), classProp)) {
            LogTypeError("Readonly field already initialized at declaration.", sourcePos);
        }
    }
}

void ETSChecker::ValidateGetterSetter(const ir::MemberExpression *const memberExpr,
                                      const varbinder::LocalVariable *const prop, PropertySearchFlags searchFlag)
{
    auto *propType = prop->TsType()->AsETSFunctionType();
    ASSERT((propType->FindGetter() != nullptr) == propType->HasTypeFlag(TypeFlag::GETTER));
    ASSERT((propType->FindSetter() != nullptr) == propType->HasTypeFlag(TypeFlag::SETTER));

    auto const &sourcePos = memberExpr->Property()->Start();
    auto callExpr = memberExpr->Parent()->IsCallExpression() ? memberExpr->Parent()->AsCallExpression() : nullptr;

    if ((searchFlag & PropertySearchFlags::IS_GETTER) != 0) {
        if (!propType->HasTypeFlag(TypeFlag::GETTER)) {
            LogTypeError("Cannot read from this property because it is writeonly.", sourcePos);
            return;
        }
        ValidateSignatureAccessibility(memberExpr->ObjType(), callExpr, propType->FindGetter(), sourcePos);
    }

    if ((searchFlag & PropertySearchFlags::IS_SETTER) != 0) {
        ValidateReadonlyProperty(memberExpr, propType, sourcePos);
        ValidateSignatureAccessibility(memberExpr->ObjType(), callExpr, propType->FindSetter(), sourcePos);
    }
}

ir::ClassProperty *ETSChecker::FindClassProperty(const ETSObjectType *const objectType, const ETSFunctionType *propType)
{
    auto propName =
        util::UString(std::string(compiler::Signatures::PROPERTY) + propType->Name().Mutf8(), Allocator()).View();

    ir::ClassProperty *classProp = nullptr;
    if (objectType->GetDeclNode()->IsClassDefinition()) {
        auto body = objectType->GetDeclNode()->AsClassDefinition()->Body();
        auto foundValue = std::find_if(body.begin(), body.end(), [propName](ir::AstNode *node) {
            return node->IsClassProperty() && node->AsClassProperty()->Key()->AsIdentifier()->Name() == propName;
        });
        if (foundValue != body.end()) {
            classProp = (*foundValue)->AsClassProperty();
        }
    }

    return classProp;
}

bool ETSChecker::IsInitializedProperty(const ir::ClassDefinition *classDefinition, const ir::ClassProperty *prop)
{
    std::string targetName = prop->Key()->AsIdentifier()->Name().Mutf8();
    if (targetName.find(compiler::Signatures::PROPERTY) == 0) {
        targetName = targetName.substr(compiler::Signatures::PROPERTY.size());
    }

    for (auto *it : classDefinition->Body()) {
        if (it->IsClassProperty() && it->AsClassProperty()->Value() != nullptr) {
            return FindPropertyInAssignment(it, targetName);
        }
    }

    return false;
}

bool ETSChecker::FindPropertyInAssignment(const ir::AstNode *it, const std::string &targetName)
{
    return it->AsClassProperty()->Value()->FindChild([&targetName](ir::AstNode *node) {
        return node->IsIdentifier() && node->AsIdentifier()->Name().Is(targetName) && node->Parent() != nullptr &&
               node->Parent()->IsMemberExpression();
    }) != nullptr;
}

void ETSChecker::ValidateVarDeclaratorOrClassProperty(const ir::MemberExpression *const memberExpr,
                                                      varbinder::LocalVariable *const prop)
{
    const auto [target_ident,
                type_annotation] = [memberExpr]() -> std::pair<const ir::Identifier *, const ir::TypeNode *> {
        if (memberExpr->Parent()->IsVariableDeclarator()) {
            const auto *const ident = memberExpr->Parent()->AsVariableDeclarator()->Id()->AsIdentifier();
            return {ident, ident->TypeAnnotation()};
        }
        return {memberExpr->Parent()->AsClassProperty()->Key()->AsIdentifier(),
                memberExpr->Parent()->AsClassProperty()->TypeAnnotation()};
    }();

    GetTypeOfVariable(prop);

    if (prop->TsType()->IsETSFunctionType() && !IsVariableGetterSetter(prop)) {
        if (type_annotation == nullptr) {
            LogTypeError({"Cannot infer type for ", target_ident->Name(),
                          " because method reference needs an explicit target type"},
                         target_ident->Start());
            return;
        }

        auto *targetType = GetTypeOfVariable(target_ident->Variable());
        ASSERT(targetType != nullptr);

        if (!targetType->IsETSObjectType() ||
            !targetType->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::FUNCTIONAL)) {
            LogTypeError({"Method ", memberExpr->Property()->AsIdentifier()->Name(), " does not exist on this type."},
                         memberExpr->Property()->Start());
        }
    }
}

// NOLINTNEXTLINE(readability-function-size)
std::vector<ResolveResult *> ETSChecker::ResolveMemberReference(const ir::MemberExpression *const memberExpr,
                                                                const ETSObjectType *const target)
{
    std::vector<ResolveResult *> resolveRes {};

    if (target->IsETSDynamicType() && !target->AsETSDynamicType()->HasDecl()) {
        auto propName = memberExpr->Property()->AsIdentifier()->Name();
        varbinder::LocalVariable *propVar = target->AsETSDynamicType()->GetPropertyDynamic(propName, this);
        resolveRes.emplace_back(Allocator()->New<ResolveResult>(propVar, ResolvedKind::PROPERTY));
        return resolveRes;
    }

    varbinder::Variable *globalFunctionVar = nullptr;

    if (memberExpr->Parent()->IsCallExpression() && memberExpr->Parent()->AsCallExpression()->Callee() == memberExpr) {
        globalFunctionVar = ResolveInstanceExtension(memberExpr);
    } else if (target->GetDeclNode() != nullptr && target->GetDeclNode()->IsClassDefinition() &&
               !target->GetDeclNode()->AsClassDefinition()->IsClassDefinitionChecked()) {
        this->CheckClassDefinition(target->GetDeclNode()->AsClassDefinition());
    }
    const auto *const targetRef = GetTargetRef(memberExpr);
    auto searchFlag = GetSearchFlags(memberExpr, targetRef);
    if (target->HasTypeFlag(TypeFlag::GENERIC) && (searchFlag & PropertySearchFlags::SEARCH_STATIC) != 0) {
        searchFlag |= PropertySearchFlags::SEARCH_ALL;
    }

    auto searchName = target->GetReExportAliasValue(memberExpr->Property()->AsIdentifier()->Name());
    auto *prop = target->GetProperty(searchName, searchFlag);

    if (memberExpr->Parent()->IsCallExpression() && memberExpr->Parent()->AsCallExpression()->Callee() == memberExpr) {
        globalFunctionVar = ResolveInstanceExtension(memberExpr);
    }

    if (globalFunctionVar == nullptr ||
        (targetRef != nullptr && targetRef->HasFlag(varbinder::VariableFlags::CLASS_OR_INTERFACE))) {
        /*
            Instance extension function can only be called by class instance, if a property is accessed by
            CLASS or INTERFACE type, it couldn't be an instance extension function call

            Example code:
                class A {}
                static function A.xxx() {}
                function main() {
                    A.xxx()
                }

            !NB: When supporting static extension function, the above code case would be supported
        */
        ValidateResolvedProperty(&prop, target, memberExpr->Property()->AsIdentifier(), searchFlag);
        if (prop == nullptr) {
            return resolveRes;
        }
    } else {
        resolveRes.emplace_back(
            Allocator()->New<ResolveResult>(globalFunctionVar, ResolvedKind::INSTANCE_EXTENSION_FUNCTION));

        if (prop == nullptr) {
            // No matched property, but have possible matched global extension function
            return resolveRes;
        }
    }

    resolveRes.emplace_back(Allocator()->New<ResolveResult>(prop, ResolvedKind::PROPERTY));

    ResolveMemberReferenceValidate(prop, searchFlag, memberExpr);

    return resolveRes;
}

void ETSChecker::ResolveMemberReferenceValidate(varbinder::LocalVariable *const prop,
                                                PropertySearchFlags const searchFlag,
                                                const ir::MemberExpression *const memberExpr)
{
    if (prop->HasFlag(varbinder::VariableFlags::METHOD) && !IsVariableGetterSetter(prop) &&
        (searchFlag & PropertySearchFlags::IS_FUNCTIONAL) == 0) {
        LogTypeError("Method used in wrong context", memberExpr->Property()->Start());
        return;
    }

    if (IsVariableGetterSetter(prop)) {
        ValidateGetterSetter(memberExpr, prop, searchFlag);
    }

    // Before returning the computed property variable, we have to validate the special case where we are in a variable
    // declaration, and the properties type is a function type but the currently declared variable doesn't have a type
    // annotation
    if (memberExpr->Parent()->IsVariableDeclarator() || memberExpr->Parent()->IsClassProperty()) {
        ValidateVarDeclaratorOrClassProperty(memberExpr, prop);
    }
}

void ETSChecker::CheckValidInheritance(ETSObjectType *classType, ir::ClassDefinition *classDef)
{
    if (classType->SuperType() == nullptr) {
        return;
    }

    if (classDef->TypeParams() != nullptr &&
        (Relation()->IsAssignableTo(classType->SuperType(), GlobalBuiltinExceptionType()) ||
         Relation()->IsAssignableTo(classType->SuperType(), GlobalBuiltinErrorType()))) {
        LogTypeError({"Generics are not allowed as '", compiler::Signatures::BUILTIN_EXCEPTION_CLASS, "' or '",
                      compiler::Signatures::BUILTIN_ERROR_CLASS, "' subclasses."},
                     classDef->TypeParams()->Start());
    }

    const auto &allProps = classType->GetAllProperties();

    for (auto *it : allProps) {
        const auto searchFlag = PropertySearchFlags::SEARCH_ALL | PropertySearchFlags::SEARCH_IN_BASE |
                                PropertySearchFlags::SEARCH_IN_INTERFACES |
                                PropertySearchFlags::DISALLOW_SYNTHETIC_METHOD_CREATION;
        auto *foundInSuper = classType->SuperType()->GetProperty(it->Name(), searchFlag);

        ETSObjectType *interfaceFound = nullptr;
        if (foundInSuper != nullptr) {
            CheckProperties(classType, classDef, it, foundInSuper, interfaceFound);
        }

        auto interfaceList = GetInterfaces(classType);
        varbinder::LocalVariable *foundInInterface = nullptr;
        for (auto *interface : interfaceList) {
            auto *propertyFound = interface->GetProperty(it->Name(), searchFlag);
            if (propertyFound == nullptr) {
                continue;
            }
            foundInInterface = propertyFound;
            interfaceFound = interface;
            break;
        }
        if (foundInInterface == nullptr) {
            continue;
        }

        CheckProperties(classType, classDef, it, foundInInterface, interfaceFound);
    }
}

void ETSChecker::CheckProperties(ETSObjectType *classType, ir::ClassDefinition *classDef, varbinder::LocalVariable *it,
                                 varbinder::LocalVariable *found, ETSObjectType *interfaceFound)
{
    if (found->TsType() == nullptr) {
        GetTypeOfVariable(found);
    }

    if (!IsSameDeclarationType(it, found)) {
        if (IsVariableStatic(it) != IsVariableStatic(found)) {
            return;
        }

        if (it->Declaration()->Type() == varbinder::DeclType::LET &&
            found->Declaration()->Type() == varbinder::DeclType::READONLY) {
            return;
        }

        if (it->TsType()->IsETSFunctionType()) {
            auto getter = it->TsType()->AsETSFunctionType()->FindGetter();
            if (getter != nullptr && Relation()->IsIdenticalTo(getter->ReturnType(), found->TsType())) {
                return;
            }
            auto setter = it->TsType()->AsETSFunctionType()->FindSetter();
            if (setter != nullptr && Relation()->IsIdenticalTo(setter->ReturnType(), found->TsType())) {
                return;
            }
        }

        const char *targetType {};

        if (it->HasFlag(varbinder::VariableFlags::PROPERTY)) {
            targetType = "field";
        } else if (it->HasFlag(varbinder::VariableFlags::METHOD)) {
            targetType = "method";
        } else if (it->HasFlag(varbinder::VariableFlags::CLASS)) {
            targetType = "class";
        } else if (it->HasFlag(varbinder::VariableFlags::INTERFACE)) {
            targetType = "interface";
        } else {
            targetType = "enum";
        }

        if (interfaceFound != nullptr) {
            LogTypeError({"Cannot inherit from interface ", interfaceFound->Name(), " because ", targetType, " ",
                          it->Name(), " is inherited with a different declaration type"},
                         interfaceFound->GetDeclNode()->Start());
            return;
        }
        LogTypeError({"Cannot inherit from class ", classType->SuperType()->Name(), ", because ", targetType, " ",
                      it->Name(), " is inherited with a different declaration type"},
                     classDef->Super()->Start());
    }
}

void ETSChecker::TransformProperties(ETSObjectType *classType)
{
    auto propertyList = classType->Fields();
    auto *const classDef = classType->GetDeclNode()->AsClassDefinition();

    for (auto *const field : propertyList) {
        ASSERT(field->Declaration()->Node()->IsClassProperty());
        auto *const originalProp = field->Declaration()->Node()->AsClassProperty();

        if ((originalProp->Modifiers() & ir::ModifierFlags::GETTER_SETTER) == 0U) {
            continue;
        }

        if (!field->HasFlag(varbinder::VariableFlags::PUBLIC)) {
            LogTypeError("Interface property implementation cannot be generated as non-public",
                         field->Declaration()->Node()->Start());
        }
        classType->RemoveProperty<checker::PropertyType::INSTANCE_FIELD>(field);
        GenerateGetterSetterPropertyAndMethod(originalProp, classType);
    }

    for (auto it = classDef->Body().begin(); it != classDef->Body().end(); ++it) {
        if ((*it)->IsClassProperty() && ((*it)->Modifiers() & ir::ModifierFlags::GETTER_SETTER) != 0U) {
            classDef->Body().erase(it);
        }
    }
}

void ETSChecker::CheckGetterSetterProperties(ETSObjectType *classType)
{
    auto const checkGetterSetter = [this](varbinder::LocalVariable *var, util::StringView name) {
        auto const *type = var->TsType()->AsETSFunctionType();
        auto const *sigGetter = type->FindGetter();
        auto const *sigSetter = type->FindSetter();

        for (auto const *sig : type->CallSignatures()) {
            if (!sig->Function()->IsGetter() && !sig->Function()->IsSetter()) {
                LogTypeError({"Method cannot use the same name as ", name, " accessor property"},
                             sig->Function()->Start());
                return;
            }
            if (sig != sigGetter && sig != sigSetter) {
                LogTypeError("Duplicate accessor definition", sig->Function()->Start());
                return;
            }
        }
        if (sigSetter != nullptr && ((sigGetter->Function()->Modifiers() ^ sigSetter->Function()->Modifiers()) &
                                     ir::ModifierFlags::ACCESSOR_MODIFIERS) != 0) {
            LogTypeError("Getter and setter methods must have the same accessor modifiers",
                         sigGetter->Function()->Start());
        }
    };

    for (const auto &[name, var] : classType->InstanceMethods()) {
        if (IsVariableGetterSetter(var)) {
            checkGetterSetter(var, name);
        }
    }

    for (const auto &[name, var] : classType->StaticMethods()) {
        if (IsVariableGetterSetter(var)) {
            checkGetterSetter(var, name);
        }
    }
}

void ETSChecker::AddElementsToModuleObject(ETSObjectType *moduleObj, const util::StringView &str)
{
    for (const auto &[name, var] : VarBinder()->GetScope()->Bindings()) {
        if (name.Is(str.Mutf8()) || name.Is(compiler::Signatures::ETS_GLOBAL)) {
            continue;
        }

        if (var->HasFlag(varbinder::VariableFlags::METHOD)) {
            moduleObj->AddProperty<checker::PropertyType::STATIC_METHOD>(var->AsLocalVariable());
        } else if (var->HasFlag(varbinder::VariableFlags::PROPERTY)) {
            moduleObj->AddProperty<checker::PropertyType::STATIC_FIELD>(var->AsLocalVariable());
        } else {
            moduleObj->AddProperty<checker::PropertyType::STATIC_DECL>(var->AsLocalVariable());
        }
    }
}

// This function computes effective runtime view of type
Type *ETSChecker::GetApparentType(Type *type)
{
    if (auto it = apparentTypes_.find(type); LIKELY(it != apparentTypes_.end())) {
        return it->second;
    }
    auto cached = [this, type](Type *res) {
        if (type != res) {
            apparentTypes_.insert({type, res});
        }
        apparentTypes_.insert({res, res});
        return res;
    };

    if (type->IsETSTypeParameter()) {
        return cached(GetApparentType(type->AsETSTypeParameter()->GetConstraintType()));
    }
    if (type->IsETSNonNullishType()) {
        return cached(
            GetNonNullishType(GetApparentType(type->AsETSNonNullishType()->GetUnderlying()->GetConstraintType())));
    }
    if (type->IsETSPartialTypeParameter()) {
        return cached(CreatePartialType(
            GetApparentType(type->AsETSPartialTypeParameter()->GetUnderlying()->GetConstraintType())));
    }
    if (type->IsETSArrayType()) {
        return cached(type);
    }
    if (type->IsETSUnionType()) {
        bool differ = false;
        ArenaVector<checker::Type *> newConstituent(Allocator()->Adapter());
        for (auto const &ct : type->AsETSUnionType()->ConstituentTypes()) {
            newConstituent.push_back(GetApparentType(ct));
            differ |= (newConstituent.back() != ct);
        }
        return cached(differ ? CreateETSUnionType(std::move(newConstituent)) : type);
    }
    return cached(type);
}

Type const *ETSChecker::GetApparentType(Type const *type) const
{
    if (auto it = apparentTypes_.find(type); LIKELY(it != apparentTypes_.end())) {
        return it->second;
    }
    // Relaxed for some types
    if (type->IsETSTypeParameter()) {
        return GetApparentType(type->AsETSTypeParameter()->GetConstraintType());
    }
    if (type->IsETSArrayType()) {
        return type;
    }
    if (type->IsETSUnionType() || type->IsETSNonNullishType() || type->IsETSPartialTypeParameter()) {
        ASSERT_PRINT(false, std::string("Type ") + type->ToString() + " was not found in apparent_types_");
    }
    return type;
}

ETSObjectType *ETSChecker::GetClosestCommonAncestor(ETSObjectType *source, ETSObjectType *target)
{
    if (source->AsETSObjectType()->GetDeclNode() == target->AsETSObjectType()->GetDeclNode()) {
        return source;
    }
    if (target->SuperType() == nullptr) {
        return GlobalETSObjectType();
    }

    auto *targetBase = GetOriginalBaseType(target->SuperType());
    auto *targetType = targetBase == nullptr ? target->SuperType() : targetBase;

    auto *sourceBase = GetOriginalBaseType(source);
    auto *sourceType = sourceBase == nullptr ? source : sourceBase;

    if (Relation()->IsSupertypeOf(targetType, sourceType)) {
        // NOTE: TorokG. Extending the search to find intersection types
        return targetType;
    }

    return GetClosestCommonAncestor(sourceType, targetType);
}

void ETSChecker::CheckInvokeMethodsLegitimacy(ETSObjectType *const classType)
{
    if (classType->HasObjectFlag(ETSObjectFlags::CHECKED_INVOKE_LEGITIMACY)) {
        return;
    }

    auto searchFlag = PropertySearchFlags::SEARCH_IN_INTERFACES | PropertySearchFlags::SEARCH_IN_BASE |
                      PropertySearchFlags::SEARCH_STATIC_METHOD;

    auto *const invokeMethod = classType->GetProperty(compiler::Signatures::STATIC_INVOKE_METHOD, searchFlag);
    if (invokeMethod == nullptr) {
        classType->AddObjectFlag(ETSObjectFlags::CHECKED_INVOKE_LEGITIMACY);
        return;
    }

    auto *const instantiateMethod = classType->GetProperty(compiler::Signatures::STATIC_INSTANTIATE_METHOD, searchFlag);
    if (instantiateMethod != nullptr) {
        LogTypeError({"Static ", compiler::Signatures::STATIC_INVOKE_METHOD, " method and static ",
                      compiler::Signatures::STATIC_INSTANTIATE_METHOD, " method both exist in class/interface ",
                      classType->Name(), " is not allowed."},
                     classType->GetDeclNode()->Start());
    }
    classType->AddObjectFlag(ETSObjectFlags::CHECKED_INVOKE_LEGITIMACY);
}

}  // namespace ark::es2panda::checker
