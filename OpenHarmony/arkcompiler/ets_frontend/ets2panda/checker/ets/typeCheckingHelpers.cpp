/**
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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

#include "checker/checker.h"
#include "checker/ets/narrowingWideningConverter.h"
#include "checker/types/globalTypesHolder.h"
#include "checker/types/ets/etsObjectType.h"
#include "checker/types/ets/etsPartialTypeParameter.h"
#include "checker/types/typeFlag.h"
#include "ir/astNode.h"
#include "ir/base/catchClause.h"
#include "ir/expression.h"
#include "ir/typeNode.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/classProperty.h"
#include "ir/base/methodDefinition.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/statements/switchCaseStatement.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/arrowFunctionExpression.h"
#include "ir/statements/labelledStatement.h"
#include "ir/statements/tryStatement.h"
#include "ir/ets/etsNewClassInstanceExpression.h"
#include "ir/ets/etsParameterExpression.h"
#include "ir/ts/tsTypeAliasDeclaration.h"
#include "ir/ts/tsEnumMember.h"
#include "ir/ts/tsTypeParameter.h"
#include "ir/ets/etsUnionType.h"
#include "ir/ets/etsTypeReference.h"
#include "ir/ets/etsTypeReferencePart.h"
#include "utils/arena_containers.h"
#include "varbinder/variable.h"
#include "varbinder/scope.h"
#include "varbinder/declaration.h"
#include "parser/program/program.h"
#include "checker/ETSchecker.h"
#include "varbinder/ETSBinder.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/ets/boxingConverter.h"
#include "checker/ets/unboxingConverter.h"
#include "util/helpers.h"

namespace ark::es2panda::checker {
void ETSChecker::CheckTruthinessOfType(ir::Expression *expr)
{
    auto *const testType = expr->Check(this);
    auto *const conditionType = MaybeUnboxConditionalInRelation(testType);

    expr->SetTsType(conditionType);

    if (conditionType == nullptr || (!conditionType->IsTypeError() && !conditionType->IsConditionalExprType())) {
        LogTypeError("Condition must be of possible condition type", expr->Start());
        return;
    }

    if (conditionType->IsETSVoidType()) {
        LogTypeError("An expression of type 'void' cannot be tested for truthiness", expr->Start());
        return;
    }

    if (conditionType->IsETSPrimitiveType()) {
        FlagExpressionWithUnboxing(testType, conditionType, expr);
    }
}

bool ETSChecker::CheckNonNullish(ir::Expression const *expr)
{
    if (expr->TsType()->PossiblyETSNullish()) {
        LogTypeError("Value is possibly nullish.", expr->Start());
        return false;
    }
    return true;
}

Type *ETSChecker::GetNonNullishType(Type *type)
{
    if (type->DefinitelyNotETSNullish()) {
        return type;
    }
    if (type->IsETSTypeParameter()) {
        return Allocator()->New<ETSNonNullishType>(type->AsETSTypeParameter());
    }

    if (type->IsETSNullType() || type->IsETSUndefinedType()) {
        return GetGlobalTypesHolder()->GlobalETSNeverType();
    }

    ArenaVector<Type *> copied(Allocator()->Adapter());
    for (auto const &t : type->AsETSUnionType()->ConstituentTypes()) {
        if (t->IsETSNullType() || t->IsETSUndefinedType()) {
            continue;
        }
        copied.push_back(GetNonNullishType(t));
    }
    return copied.empty() ? GetGlobalTypesHolder()->GlobalETSNeverType() : CreateETSUnionType(std::move(copied));
}

Type *ETSChecker::RemoveNullType(Type *const type)
{
    if (type->DefinitelyNotETSNullish() || type->IsETSUndefinedType()) {
        return type;
    }

    if (type->IsETSTypeParameter()) {
        return Allocator()->New<ETSNonNullishType>(type->AsETSTypeParameter());
    }

    if (type->IsETSNullType()) {
        return GetGlobalTypesHolder()->GlobalETSNeverType();
    }

    ASSERT(type->IsETSUnionType());
    ArenaVector<Type *> copiedTypes(Allocator()->Adapter());

    for (auto *constituentType : type->AsETSUnionType()->ConstituentTypes()) {
        if (!constituentType->IsETSNullType()) {
            copiedTypes.push_back(RemoveNullType(constituentType));
        }
    }

    return copiedTypes.empty() ? GetGlobalTypesHolder()->GlobalETSNeverType()
                               : CreateETSUnionType(std::move(copiedTypes));
}

Type *ETSChecker::RemoveUndefinedType(Type *const type)
{
    if (type->DefinitelyNotETSNullish() || type->IsETSNullType()) {
        return type;
    }

    if (type->IsETSTypeParameter()) {
        return Allocator()->New<ETSNonNullishType>(type->AsETSTypeParameter());
    }

    if (type->IsETSUndefinedType()) {
        return GetGlobalTypesHolder()->GlobalETSNeverType();
    }

    ASSERT(type->IsETSUnionType());
    ArenaVector<Type *> copiedTypes(Allocator()->Adapter());

    for (auto *constituentType : type->AsETSUnionType()->ConstituentTypes()) {
        if (!constituentType->IsETSUndefinedType()) {
            copiedTypes.push_back(RemoveUndefinedType(constituentType));
        }
    }

    return copiedTypes.empty() ? GetGlobalTypesHolder()->GlobalETSNeverType()
                               : CreateETSUnionType(std::move(copiedTypes));
}

std::pair<Type *, Type *> ETSChecker::RemoveNullishTypes(Type *type)
{
    if (type->DefinitelyNotETSNullish()) {
        return {GetGlobalTypesHolder()->GlobalETSNeverType(), type};
    }

    if (type->IsETSTypeParameter()) {
        return {GetGlobalTypesHolder()->GlobalETSNullishType(),
                Allocator()->New<ETSNonNullishType>(type->AsETSTypeParameter())};
    }

    if (type->IsETSUndefinedType() || type->IsETSNullType()) {
        return {type, GetGlobalTypesHolder()->GlobalETSNeverType()};
    }

    ASSERT(type->IsETSUnionType());
    ArenaVector<Type *> nullishTypes(Allocator()->Adapter());
    ArenaVector<Type *> notNullishTypes(Allocator()->Adapter());

    for (auto *constituentType : type->AsETSUnionType()->ConstituentTypes()) {
        if (constituentType->IsETSUndefinedType() || constituentType->IsETSNullType()) {
            nullishTypes.push_back(constituentType);
        } else {
            notNullishTypes.push_back(!constituentType->IsETSTypeParameter()
                                          ? constituentType
                                          : Allocator()->New<ETSNonNullishType>(constituentType->AsETSTypeParameter()));
        }
    }

    Type *nullishType = nullishTypes.empty() ? GetGlobalTypesHolder()->GlobalETSNeverType()
                                             : CreateETSUnionType(std::move(nullishTypes));
    Type *notNullishType = notNullishTypes.empty() ? GetGlobalTypesHolder()->GlobalETSNeverType()
                                                   : CreateETSUnionType(std::move(notNullishTypes));
    return {nullishType, notNullishType};
}

// NOTE(vpukhov): can be implemented with relation if etscompiler will support it
template <typename Pred, typename Trv>
static bool MatchConstituentOrConstraint(const Type *type, Pred const &pred, Trv const &trv)
{
    auto const traverse = [&pred, &trv](const Type *ttype) {
        return MatchConstituentOrConstraint<Pred, Trv>(ttype, pred, trv);
    };
    if (pred(type)) {
        return true;
    }
    if (!trv(type)) {
        return false;
    }
    if (type->IsETSUnionType()) {
        for (auto const &ctype : type->AsETSUnionType()->ConstituentTypes()) {
            if (traverse(ctype)) {
                return true;
            }
        }
        return false;
    }
    if (type->IsETSTypeParameter()) {
        return traverse(type->AsETSTypeParameter()->GetConstraintType());
    }
    if (type->IsETSNonNullishType()) {
        auto tparam = type->AsETSNonNullishType()->GetUnderlying();
        return traverse(tparam->GetConstraintType());
    }
    if (type->IsETSPartialTypeParameter()) {
        auto tparam = type->AsETSPartialTypeParameter()->GetUnderlying();
        return traverse(tparam->GetConstraintType());
    }
    return false;
}

template <typename Pred>
static bool MatchConstituentOrConstraint(const Type *type, Pred const &pred)
{
    return MatchConstituentOrConstraint(type, pred, []([[maybe_unused]] const Type *t) { return true; });
}

bool Type::PossiblyETSNull() const
{
    return MatchConstituentOrConstraint(
        this, [](const Type *t) { return t->IsETSNullType(); },
        [](const Type *t) { return !t->IsETSNonNullishType(); });
}

bool Type::PossiblyETSUndefined() const
{
    return MatchConstituentOrConstraint(
        this, [](const Type *t) { return t->IsETSUndefinedType(); },
        [](const Type *t) { return !t->IsETSNonNullishType(); });
}

bool Type::PossiblyETSNullish() const
{
    return MatchConstituentOrConstraint(
        this, [](const Type *t) { return t->IsETSNullType() || t->IsETSUndefinedType(); },
        [](const Type *t) { return !t->IsETSNonNullishType(); });
}

bool Type::DefinitelyETSNullish() const
{
    return !MatchConstituentOrConstraint(
        this,
        [](const Type *t) {
            return !(t->IsTypeParameter() || t->IsETSUnionType() || t->IsETSNullType() || t->IsETSUndefinedType());
        },
        [](const Type *t) { return !t->IsETSNonNullishType(); });
}

bool Type::DefinitelyNotETSNullish() const
{
    return !PossiblyETSNullish();
}

bool Type::PossiblyETSString() const
{
    return MatchConstituentOrConstraint(this, [](const Type *t) {
        return t->IsETSStringType() || (t->IsETSObjectType() && t->AsETSObjectType()->IsGlobalETSObjectType());
    });
}

static bool IsValueTypedObjectType(ETSObjectType const *t)
{
    return t->IsGlobalETSObjectType() || t->HasObjectFlag(ETSObjectFlags::VALUE_TYPED);
}

bool Type::PossiblyETSValueTyped() const
{
    return MatchConstituentOrConstraint(this, [](const Type *t) {
        return t->IsNullType() || t->IsUndefinedType() ||
               (t->IsETSObjectType() && IsValueTypedObjectType(t->AsETSObjectType()));
    });
}

bool Type::PossiblyETSValueTypedExceptNullish() const
{
    return MatchConstituentOrConstraint(
        this, [](const Type *t) { return t->IsETSObjectType() && IsValueTypedObjectType(t->AsETSObjectType()); });
}

bool Type::IsETSArrowType() const
{
    return IsETSFunctionType() && AsETSFunctionType()->CallSignatures().size() == 1;
}

[[maybe_unused]] static bool IsSaneETSReferenceType(Type const *type)
{
    static constexpr TypeFlag ETS_SANE_REFERENCE_TYPE =
        TypeFlag::TYPE_ERROR | TypeFlag::ETS_NULL | TypeFlag::ETS_UNDEFINED | TypeFlag::ETS_OBJECT |
        TypeFlag::ETS_TYPE_PARAMETER | TypeFlag::WILDCARD | TypeFlag::ETS_NONNULLISH |
        TypeFlag::ETS_REQUIRED_TYPE_PARAMETER | TypeFlag::ETS_NEVER | TypeFlag::ETS_UNION | TypeFlag::ETS_ARRAY |
        TypeFlag::FUNCTION | TypeFlag::ETS_PARTIAL_TYPE_PARAMETER;

    // Issues
    if (type->IsETSVoidType()) {  // NOTE(vpukhov): #19701 void refactoring
        return true;
    }
    if (type->IsETSTypeAliasType()) {  // NOTE(vpukhov): #20561
        return true;
    }
    if (type->IsNeverType()) {  // NOTE(vpukhov): #20562 We use ets/never and ts/never simultaneously
        return true;
    }
    return type->HasTypeFlag(ETS_SANE_REFERENCE_TYPE);
}

bool Type::IsETSPrimitiveType() const
{
    static constexpr TypeFlag ETS_PRIMITIVE =
        TypeFlag::ETS_NUMERIC | TypeFlag::CHAR | TypeFlag::ETS_BOOLEAN | TypeFlag::ETS_ENUM;

    // Do not modify
    ASSERT(!HasTypeFlag(ETS_PRIMITIVE) == IsSaneETSReferenceType(this));
    return HasTypeFlag(ETS_PRIMITIVE);
}

bool Type::IsETSReferenceType() const
{
    // Do not modify
    return !IsETSPrimitiveType();
}

bool Type::IsETSUnboxableObject() const
{
    return IsETSObjectType() && AsETSObjectType()->HasObjectFlag(ETSObjectFlags::UNBOXABLE_TYPE);
}

bool ETSChecker::IsConstantExpression(ir::Expression *expr, Type *type)
{
    return (type->HasTypeFlag(TypeFlag::CONSTANT) && (expr->IsIdentifier() || expr->IsMemberExpression()));
}

Type *ETSChecker::GetNonConstantType(Type *type)
{
    if (type->IsETSStringType()) {
        return GlobalBuiltinETSStringType();
    }

    if (type->IsETSBigIntType()) {
        return GlobalETSBigIntType();
    }

    if (type->IsETSUnionType()) {
        return CreateETSUnionType(ETSUnionType::GetNonConstantTypes(this, type->AsETSUnionType()->ConstituentTypes()));
    }

    if (!type->IsETSPrimitiveType()) {
        return type;
    }

    if (type->HasTypeFlag(TypeFlag::LONG)) {
        return GlobalLongType();
    }

    if (type->HasTypeFlag(TypeFlag::BYTE)) {
        return GlobalByteType();
    }

    if (type->HasTypeFlag(TypeFlag::SHORT)) {
        return GlobalShortType();
    }

    if (type->HasTypeFlag(TypeFlag::CHAR)) {
        return GlobalCharType();
    }

    if (type->HasTypeFlag(TypeFlag::INT)) {
        return GlobalIntType();
    }

    if (type->HasTypeFlag(TypeFlag::FLOAT)) {
        return GlobalFloatType();
    }

    if (type->HasTypeFlag(TypeFlag::DOUBLE)) {
        return GlobalDoubleType();
    }

    if (type->IsETSBooleanType()) {
        return GlobalETSBooleanType();
    }
    return type;
}

Type *ETSChecker::GetTypeOfSetterGetter(varbinder::Variable *const var)
{
    auto *propType = var->TsType()->AsETSFunctionType();
    if (propType->HasTypeFlag(checker::TypeFlag::GETTER)) {
        return propType->FindGetter()->ReturnType();
    }
    return propType->FindSetter()->Params()[0]->TsType();
}

void ETSChecker::IterateInVariableContext(varbinder::Variable *const var)
{
    // Before computing the given variables type, we have to make a new checker context frame so that the checking is
    // done in the proper context, and have to enter the scope where the given variable is declared, so reference
    // resolution works properly
    auto *iter = var->Declaration()->Node()->Parent();
    while (iter != nullptr) {
        if (iter->IsMethodDefinition()) {
            auto *methodDef = iter->AsMethodDefinition();
            ASSERT(methodDef->TsType());
            Context().SetContainingSignature(methodDef->Function()->Signature());
        }

        if (iter->IsClassDefinition()) {
            auto *classDef = iter->AsClassDefinition();
            ETSObjectType *containingClass {};

            if (classDef->TsType() == nullptr) {
                containingClass = BuildBasicClassProperties(classDef);
                ResolveDeclaredMembersOfObject(containingClass);
            } else {
                containingClass = classDef->TsType()->AsETSObjectType();
            }

            ASSERT(classDef->TsType());
            Context().SetContainingClass(containingClass);
        }

        iter = iter->Parent();
    }
}

Type *ETSChecker::GetTypeFromVariableDeclaration(varbinder::Variable *const var)
{
    switch (var->Declaration()->Type()) {
        case varbinder::DeclType::CLASS: {
            auto *classDef = var->Declaration()->Node()->AsClassDefinition();
            BuildBasicClassProperties(classDef);
            return classDef->TsType();
        }
        case varbinder::DeclType::ENUM_LITERAL:
        case varbinder::DeclType::CONST:
        case varbinder::DeclType::READONLY:
        case varbinder::DeclType::LET:
        case varbinder::DeclType::VAR: {
            auto *declNode = var->Declaration()->Node();
            if (var->Declaration()->Node()->IsIdentifier()) {
                declNode = declNode->Parent();
            }
            return declNode->Check(this);
        }
        case varbinder::DeclType::FUNC:
        case varbinder::DeclType::IMPORT: {
            return var->Declaration()->Node()->Check(this);
        }
        case varbinder::DeclType::TYPE_ALIAS: {
            return GetTypeFromTypeAliasReference(var);
        }
        case varbinder::DeclType::INTERFACE: {
            return BuildBasicInterfaceProperties(var->Declaration()->Node()->AsTSInterfaceDeclaration());
        }
        case varbinder::DeclType::ANNOTATIONUSAGE: {
            return GlobalTypeError();
        }
        case varbinder::DeclType::ANNOTATIONDECL: {
            return GlobalTypeError();
        }
        default: {
            UNREACHABLE();
        }
    }
}

Type *ETSChecker::GetTypeOfVariable(varbinder::Variable *const var)
{
    if (IsVariableGetterSetter(var)) {
        return GetTypeOfSetterGetter(var);
    }

    if (var->TsType() != nullptr) {
        return var->TsType();
    }

    // NOTE: kbaladurin. forbid usage of imported entities as types without declarations
    if (VarBinder()->AsETSBinder()->IsDynamicModuleVariable(var)) {
        auto *importData = VarBinder()->AsETSBinder()->DynamicImportDataForVar(var);
        if (importData->import->IsPureDynamic()) {
            return GlobalBuiltinDynamicType(importData->import->Language());
        }
    }

    checker::SavedCheckerContext savedContext(this, CheckerStatus::NO_OPTS);
    checker::ScopeContext scopeCtx(this, var->GetScope());
    IterateInVariableContext(var);

    return GetTypeFromVariableDeclaration(var);
}

// Determine if unchecked cast is needed and yield guaranteed source type
Type *ETSChecker::GuaranteedTypeForUncheckedCast(Type *base, Type *substituted)
{
    // Apparent type acts as effective representation for type.
    //  For T extends SomeClass|undefined
    //  Apparent(Int|T|null) is Int|SomeClass|undefined|null
    auto *appBase = GetApparentType(base);
    auto *appSubst = GetApparentType(substituted);
    // Base is supertype of Substituted AND Substituted is supertype of Base
    return Relation()->IsIdenticalTo(appSubst, appBase) ? nullptr : appBase;
}

// Determine if substituted property access requires cast from erased type
Type *ETSChecker::GuaranteedTypeForUncheckedPropertyAccess(varbinder::Variable *const prop)
{
    if (IsVariableStatic(prop)) {
        return nullptr;
    }
    if (IsVariableGetterSetter(prop)) {
        auto *method = prop->TsType()->AsETSFunctionType();
        if (!method->HasTypeFlag(checker::TypeFlag::GETTER)) {
            return nullptr;
        }
        return GuaranteedTypeForUncheckedCallReturn(method->FindGetter());
    }
    // NOTE(vpukhov): mark ETSDynamicType properties
    if (prop->Declaration() == nullptr || prop->Declaration()->Node() == nullptr) {
        return nullptr;
    }

    varbinder::Variable *baseProp = nullptr;
    switch (auto node = prop->Declaration()->Node(); node->Type()) {
        case ir::AstNodeType::CLASS_PROPERTY:
            baseProp = node->AsClassProperty()->Id()->Variable();
            break;
        case ir::AstNodeType::METHOD_DEFINITION:
            baseProp = node->AsMethodDefinition()->Variable();
            break;
            // NOTE(vpukhov): should not be a case of unchecked access
        case ir::AstNodeType::CLASS_DEFINITION:
            baseProp = node->AsClassDefinition()->Ident()->Variable();
            break;
        default:
            UNREACHABLE();
    }

    if (baseProp == prop) {
        return nullptr;
    }
    return GuaranteedTypeForUncheckedCast(GetTypeOfVariable(baseProp), GetTypeOfVariable(prop));
}

// Determine if substituted method cast requires cast from erased type
Type *ETSChecker::GuaranteedTypeForUncheckedCallReturn(Signature *sig)
{
    if (sig->HasSignatureFlag(checker::SignatureFlags::THIS_RETURN_TYPE)) {
        return sig->ReturnType();
    }
    auto *baseSig = sig->Function()->Signature();
    if (baseSig == sig) {
        return nullptr;
    }
    return GuaranteedTypeForUncheckedCast(baseSig->ReturnType(), sig->ReturnType());
}

void ETSChecker::CheckEtsFunctionType(ir::Identifier *const ident, ir::Identifier const *const id)
{
    const auto *const targetType = GetTypeOfVariable(id->Variable());
    ASSERT(targetType != nullptr);

    if (!targetType->IsETSObjectType() && !targetType->IsETSUnionType()) {
        LogTypeError("Initializers type is not assignable to the target type", ident->Start());
    }
}

bool ETSChecker::IsAllowedTypeAliasRecursion(const ir::TSTypeAliasDeclaration *typeAliasNode,
                                             std::unordered_set<const ir::TSTypeAliasDeclaration *> &typeAliases)
{
    bool isAllowedRerursiveType = true;

    RecursionPreserver<const ir::TSTypeAliasDeclaration> recursionPreserver(typeAliases, typeAliasNode);

    if (*recursionPreserver) {
        return false;
    }

    auto typeAliasDeclarationCheck = [this, &typeAliases](ir::ETSTypeReferencePart *part) {
        if (!part->Name()->IsIdentifier()) {
            return false;
        }
        if (part->Name()->AsIdentifier()->Variable() != nullptr &&
            part->Name()->AsIdentifier()->Variable()->Declaration() != nullptr &&
            part->Name()->AsIdentifier()->Variable()->Declaration()->Node() != nullptr &&
            part->Name()->AsIdentifier()->Variable()->Declaration()->Node()->IsTSTypeAliasDeclaration()) {
            auto *aliasTypeNode =
                part->Name()->AsIdentifier()->Variable()->Declaration()->Node()->AsTSTypeAliasDeclaration();
            return IsAllowedTypeAliasRecursion(aliasTypeNode, typeAliases);
        }

        return true;
    };

    if (typeAliasNode->TypeAnnotation()->IsETSTypeReference()) {
        isAllowedRerursiveType &=
            typeAliasDeclarationCheck(typeAliasNode->TypeAnnotation()->AsETSTypeReference()->Part());
    }

    if (isAllowedRerursiveType && typeAliasNode->TypeAnnotation()->IsETSUnionType()) {
        for (auto &type : typeAliasNode->TypeAnnotation()->AsETSUnionType()->Types()) {
            if (type->IsETSTypeReference()) {
                isAllowedRerursiveType &= typeAliasDeclarationCheck(type->AsETSTypeReference()->Part());
            }
        }
    }

    return isAllowedRerursiveType;
}

Type *ETSChecker::GetTypeFromTypeAliasReference(varbinder::Variable *var)
{
    if (var->TsType() != nullptr) {
        return var->TsType();
    }

    auto *const aliasTypeNode = var->Declaration()->Node()->AsTSTypeAliasDeclaration();
    std::unordered_set<const ir::TSTypeAliasDeclaration *> typeAliases;
    auto isAllowedRecursion = IsAllowedTypeAliasRecursion(aliasTypeNode, typeAliases);

    TypeStackElement tse(this, aliasTypeNode, "Circular type alias reference", aliasTypeNode->Start(),
                         isAllowedRecursion);

    if (tse.HasTypeError()) {
        var->SetTsType(GlobalTypeError());
        return GlobalTypeError();
    }

    auto *typeAliasType = tse.GetElementType();

    if (typeAliasType != nullptr) {
        typeAliasType->AsETSTypeAliasType()->SetRecursive();
        return typeAliasType;
    }

    typeAliasType = CreateETSTypeAliasType(aliasTypeNode->Id()->Name(), aliasTypeNode);
    if (aliasTypeNode->TypeParams() != nullptr) {
        auto [typeParamTypes, ok] = CreateUnconstrainedTypeParameters(aliasTypeNode->TypeParams());
        typeAliasType->AsETSTypeAliasType()->SetTypeArguments(std::move(typeParamTypes));
        if (ok) {
            AssignTypeParameterConstraints(aliasTypeNode->TypeParams());
        }
    }
    tse.SetElementType(typeAliasType);

    aliasTypeNode->Check(this);
    Type *targetType = aliasTypeNode->TypeAnnotation()->GetType(this);
    typeAliasType->AsETSTypeAliasType()->SetTargetType(targetType);
    typeAliasType->AsETSTypeAliasType()->ApplySubstitution(Relation());

    var->SetTsType(targetType);
    return targetType;
}

Type *ETSChecker::GetTypeFromInterfaceReference(varbinder::Variable *var)
{
    if (var->TsType() != nullptr) {
        return var->TsType();
    }

    auto *interfaceType = BuildBasicInterfaceProperties(var->Declaration()->Node()->AsTSInterfaceDeclaration());
    var->SetTsType(interfaceType);
    return interfaceType;
}

Type *ETSChecker::GetTypeFromClassReference(varbinder::Variable *var)
{
    if (var->TsType() != nullptr) {
        return var->TsType();
    }

    auto *classType = BuildBasicClassProperties(var->Declaration()->Node()->AsClassDefinition());
    var->SetTsType(classType);
    return classType;
}

Type *ETSChecker::GetTypeFromEnumReference([[maybe_unused]] varbinder::Variable *var)
{
    if (var->TsType() != nullptr) {
        return var->TsType();
    }

    auto *const enumDecl = var->Declaration()->Node()->AsTSEnumDeclaration();
    if (enumDecl->BoxedClass()->TsType() == nullptr) {
        BuildBasicClassProperties(enumDecl->BoxedClass());
    }
    if (auto *const itemInit = enumDecl->Members().front()->AsTSEnumMember()->Init(); itemInit->IsNumberLiteral()) {
        return CreateEnumIntTypeFromEnumDeclaration(enumDecl);
    } else if (itemInit->IsStringLiteral()) {  // NOLINT(readability-else-after-return)
        return CreateEnumStringTypeFromEnumDeclaration(enumDecl);
    } else {  // NOLINT(readability-else-after-return)
        return TypeError(var, "Invalid enumeration value type.", enumDecl->Start());
    }
}

Type *ETSChecker::GetTypeFromTypeParameterReference(varbinder::LocalVariable *var, const lexer::SourcePosition &pos)
{
    ASSERT(var->Declaration()->Node()->IsTSTypeParameter());
    if ((var->Declaration()->Node()->AsTSTypeParameter()->Parent()->Parent()->IsClassDefinition() ||
         var->Declaration()->Node()->AsTSTypeParameter()->Parent()->Parent()->IsTSInterfaceDeclaration()) &&
        HasStatus(CheckerStatus::IN_STATIC_CONTEXT)) {
        return TypeError(var, FormatMsg({"Cannot make a static reference to the non-static type ", var->Name()}), pos);
    }

    return var->TsType();
}

bool ETSChecker::CheckAmbientAnnotationFieldInitializer(ir::Expression *init, ir::Expression *expected)
{
    if (init->Type() != expected->Type()) {
        return false;
    }

    switch (init->Type()) {
        case ir::AstNodeType::NUMBER_LITERAL:
        case ir::AstNodeType::BOOLEAN_LITERAL:
        case ir::AstNodeType::STRING_LITERAL:
        case ir::AstNodeType::ARRAY_EXPRESSION:
        case ir::AstNodeType::MEMBER_EXPRESSION:
        case ir::AstNodeType::UNARY_EXPRESSION: {
            if (CheckAmbientAnnotationFieldInitializerValue(init, expected)) {
                break;
            }
            LogTypeError({"The initial value does not match the expected value."}, init->Start());
            return false;
        }
        default:
            UNREACHABLE();
    }

    return true;
}

static bool IsValidateUnaryExpression(lexer::TokenType operatorType)
{
    return operatorType == lexer::TokenType::PUNCTUATOR_PLUS || operatorType == lexer::TokenType::PUNCTUATOR_MINUS;
}

bool ETSChecker::CheckAmbientAnnotationFieldInitializerValue(ir::Expression *init, ir::Expression *expected)
{
    switch (init->Type()) {
        case ir::AstNodeType::NUMBER_LITERAL: {
            return init->AsNumberLiteral()->Number().GetDouble() == expected->AsNumberLiteral()->Number().GetDouble();
        }
        case ir::AstNodeType::BOOLEAN_LITERAL: {
            return init->AsBooleanLiteral()->Value() == expected->AsBooleanLiteral()->Value();
        }
        case ir::AstNodeType::STRING_LITERAL: {
            return init->AsStringLiteral()->Str() == expected->AsStringLiteral()->Str();
        }
        case ir::AstNodeType::ARRAY_EXPRESSION: {
            const auto &elements = init->AsArrayExpression()->Elements();
            const auto &expectedElements = expected->AsArrayExpression()->Elements();
            if (elements.size() != expectedElements.size()) {
                return false;
            }
            for (size_t i = 0; i < elements.size(); ++i) {
                if (!CheckAmbientAnnotationFieldInitializer(elements[i], expectedElements[i])) {
                    return false;
                }
            }
            return true;
        }
        case ir::AstNodeType::MEMBER_EXPRESSION: {
            if (!Relation()->IsIdenticalTo(init->TsType(), expected->TsType())) {
                return false;
            }
            auto elem = init->AsMemberExpression()->Property()->AsIdentifier()->Name();
            auto expectedElem = expected->AsMemberExpression()->Property()->AsIdentifier()->Name();
            return elem == expectedElem;
        }
        case ir::AstNodeType::UNARY_EXPRESSION: {
            if (!IsValidateUnaryExpression(init->AsUnaryExpression()->OperatorType()) ||
                !IsValidateUnaryExpression(expected->AsUnaryExpression()->OperatorType())) {
                LogTypeError("Illegal unary operator.", init->Start());
                return false;
            }
            if (init->AsUnaryExpression()->OperatorType() != expected->AsUnaryExpression()->OperatorType()) {
                return false;
            }
            return CheckAmbientAnnotationFieldInitializer(init->AsUnaryExpression()->Argument(),
                                                          expected->AsUnaryExpression()->Argument());
        }
        default:
            UNREACHABLE();
    }
}

void ETSChecker::CheckAmbientAnnotation(ir::AnnotationDeclaration *annoImpl, ir::AnnotationDeclaration *annoDecl)
{
    std::unordered_map<util::StringView, ir::ClassProperty *> fieldMap;

    for (auto *prop : annoDecl->Properties()) {
        auto *field = prop->AsClassProperty();
        fieldMap[field->Id()->Name()] = field;
    }

    for (auto *prop : annoImpl->Properties()) {
        auto *field = prop->AsClassProperty();
        auto fieldName = field->Id()->Name();
        auto fieldDeclIter = fieldMap.find(fieldName);
        if (fieldDeclIter == fieldMap.end()) {
            LogTypeError({"Field '", fieldName, "' is not defined in the ambient annotation '",
                          annoDecl->GetBaseName()->Name(), "'."},
                         field->Start());
            continue;
        }

        auto *fieldDecl = fieldDeclIter->second;
        if (field->TsType() != fieldDecl->TsType()) {
            LogTypeError({"Field '", fieldName, "' has a type mismatch with the ambient annotation '",
                          annoDecl->GetBaseName()->Name(), "'."},
                         field->TypeAnnotation()->Start());
        }

        bool hasValueMismatch = (field->Value() == nullptr) != (fieldDecl->Value() == nullptr);
        bool initializerInvalid = field->Value() != nullptr && fieldDecl->Value() != nullptr &&
                                  !CheckAmbientAnnotationFieldInitializer(field->Value(), fieldDecl->Value());
        if (hasValueMismatch || initializerInvalid) {
            LogTypeError({"Initializer for field '", fieldName,
                          "' does not match the expected definition in the ambient annotation '",
                          annoDecl->GetBaseName()->Name(), "'."},
                         field->Start());
        }
        fieldMap.erase(fieldDeclIter);
    }

    for (auto it : fieldMap) {
        LogTypeError({"Field '", it.second->Key()->AsIdentifier()->Name(), "' in annotation '",
                      annoDecl->GetBaseName()->Name(),
                      "' is declared in the ambient declaration but missing in the implementation."},
                     annoImpl->Start());
    }
}

bool ETSChecker::CheckDuplicateAnnotations(const ArenaVector<ir::AnnotationUsage *> &annotations)
{
    std::unordered_set<util::StringView> seenAnnotations;
    for (const auto &anno : annotations) {
        auto annoName = anno->GetBaseName()->Name();
        if (seenAnnotations.find(annoName) != seenAnnotations.end()) {
            LogTypeError({"Duplicate annotations are not allowed. The annotation '", annoName,
                          "' has already been applied to this element."},
                         anno->Start());
            return false;
        }
        seenAnnotations.insert(annoName);
    }
    return true;
}

void ETSChecker::CheckAnnotationPropertyType(ir::ClassProperty *property)
{
    // typeAnnotation check
    if (!ValidateAnnotationPropertyType(property->TsType())) {
        LogTypeError({"Invalid annotation field type. Only numeric, boolean, string, enum, or "
                      "arrays of these types are permitted for annotation fields."},
                     property->Start());
    }

    // The type of the Initializer has been check in the parser,
    // except for the enumeration type, because it is a member expression,
    // so here is an additional check to the enumeration type.
    if (property->Value() != nullptr && property->Value()->IsMemberExpression() &&
        !property->TsType()->IsETSEnumType()) {
        LogTypeError("Invalid value for annotation field, expected a constant literal.", property->Value()->Start());
    }
}

void ETSChecker::CheckSinglePropertyAnnotation(ir::AnnotationUsage *st, ir::AnnotationDeclaration *annoDecl)
{
    auto *param = st->Properties().at(0)->AsClassProperty();
    if (annoDecl->Properties().size() > 1) {
        LogTypeError({"Annotation '", st->GetBaseName()->Name(), "' requires multiple fields to be specified."},
                     st->Start());
    }
    auto singleField = annoDecl->Properties().at(0)->AsClassProperty();
    auto ctx = checker::AssignmentContext(Relation(), param->Value(), param->TsType(), singleField->TsType(),
                                          param->Start(), {}, TypeRelationFlag::NO_THROW);
    if (!ctx.IsAssignable()) {
        LogTypeError({"The value provided for annotation '", st->GetBaseName()->Name(), "' field '",
                      param->Id()->Name(), "' is of type '", param->TsType(), "', but expected type is '",
                      singleField->TsType(), "'."},
                     param->Start());
    }
}

void ETSChecker::ProcessRequiredFields(ArenaUnorderedMap<util::StringView, ir::ClassProperty *> &fieldMap,
                                       ir::AnnotationUsage *st, ETSChecker *checker) const
{
    for (const auto &entry : fieldMap) {
        if (entry.second->Value() == nullptr) {
            checker->LogTypeError({"The required field '", entry.first,
                                   "' must be specified. Fields without default values cannot be omitted."},
                                  st->Start());
            continue;
        }
        auto *clone = entry.second->Clone(checker->Allocator(), st);
        clone->Check(checker);
        st->AddProperty(clone);
    }
}

void ETSChecker::CheckMultiplePropertiesAnnotation(ir::AnnotationUsage *st, ir::AnnotationDeclaration *annoDecl,
                                                   ArenaUnorderedMap<util::StringView, ir::ClassProperty *> &fieldMap)
{
    for (auto *it : st->Properties()) {
        auto *param = it->AsClassProperty();
        auto result = fieldMap.find(param->Id()->Name());
        if (result == fieldMap.end()) {
            LogTypeError({"The parameter '", param->Id()->Name(),
                          "' does not match any declared property in the annotation '", annoDecl->GetBaseName()->Name(),
                          "'."},
                         param->Start());
            continue;
        }
        auto ctx = checker::AssignmentContext(Relation(), param->Value(), param->TsType(), result->second->TsType(),
                                              param->Start(), {}, TypeRelationFlag::NO_THROW);
        if (!ctx.IsAssignable()) {
            LogTypeError({"The value provided for annotation '", st->GetBaseName()->Name(), "' field '",
                          param->Id()->Name(), "' is of type '", param->TsType(), "', but expected type is '",
                          result->second->TsType(), "'."},
                         param->Start());
        }
        fieldMap.erase(result);
    }
}

Type *ETSChecker::MaybeUnboxInRelation(Type *type)
{
    if (type == nullptr) {
        return nullptr;
    }

    if (type->IsETSPrimitiveType()) {
        return type;
    }

    if (!type->IsETSUnboxableObject()) {
        return nullptr;
    }

    auto savedResult = Relation()->IsTrue();
    Relation()->Result(false);

    UnboxingConverter converter = UnboxingConverter(AsETSChecker(), Relation(), type, type);
    Relation()->Result(savedResult);
    return converter.Result();
}

Type *ETSChecker::MaybeUnboxConditionalInRelation(Type *const objectType)
{
    if (objectType->IsTypeError()) {
        return objectType;
    }

    if ((objectType == nullptr) || !objectType->IsConditionalExprType()) {
        return nullptr;
    }

    if (auto *unboxed = MaybeUnboxInRelation(objectType); unboxed != nullptr) {
        return unboxed;
    }

    return objectType;
}

Type *ETSChecker::MaybeBoxInRelation(Type *objectType)
{
    if (objectType == nullptr) {
        return nullptr;
    }

    if (objectType->IsETSUnboxableObject()) {
        return objectType;
    }

    if (!objectType->IsETSPrimitiveType()) {
        return nullptr;
    }

    auto savedResult = Relation()->IsTrue();
    Relation()->Result(false);

    BoxingConverter converter = BoxingConverter(AsETSChecker(), Relation(), objectType,
                                                Checker::GetGlobalTypesHolder()->GlobalIntegerBuiltinType());
    Relation()->Result(savedResult);
    return converter.Result();
}

Type *ETSChecker::MaybeBoxType(Type *type) const
{
    return type->IsETSPrimitiveType() ? BoxingConverter::Convert(this, type) : type;
}

Type *ETSChecker::MaybeUnboxType(Type *type) const
{
    return type->IsETSUnboxableObject() ? UnboxingConverter::Convert(this, type->AsETSObjectType()) : type;
}

Type const *ETSChecker::MaybeBoxType(Type const *type) const
{
    return MaybeBoxType(const_cast<Type *>(type));
}

Type const *ETSChecker::MaybeUnboxType(Type const *type) const
{
    return MaybeUnboxType(const_cast<Type *>(type));
}

ir::BoxingUnboxingFlags ETSChecker::GetBoxingFlag(Type *const boxingType)
{
    auto typeKind = TypeKind(MaybeUnboxInRelation(boxingType));
    switch (typeKind) {
        case TypeFlag::ETS_BOOLEAN:
            return ir::BoxingUnboxingFlags::BOX_TO_BOOLEAN;
        case TypeFlag::BYTE:
            return ir::BoxingUnboxingFlags::BOX_TO_BYTE;
        case TypeFlag::CHAR:
            return ir::BoxingUnboxingFlags::BOX_TO_CHAR;
        case TypeFlag::SHORT:
            return ir::BoxingUnboxingFlags::BOX_TO_SHORT;
        case TypeFlag::INT:
            return ir::BoxingUnboxingFlags::BOX_TO_INT;
        case TypeFlag::LONG:
            return ir::BoxingUnboxingFlags::BOX_TO_LONG;
        case TypeFlag::FLOAT:
            return ir::BoxingUnboxingFlags::BOX_TO_FLOAT;
        case TypeFlag::DOUBLE:
            return ir::BoxingUnboxingFlags::BOX_TO_DOUBLE;
        case TypeFlag::ETS_INT_ENUM:
        case TypeFlag::ETS_STRING_ENUM:
            return ir::BoxingUnboxingFlags::BOX_TO_ENUM;
        default:
            UNREACHABLE();
    }
}

ir::BoxingUnboxingFlags ETSChecker::GetUnboxingFlag(Type const *const unboxingType) const
{
    auto typeKind = TypeKind(unboxingType);
    switch (typeKind) {
        case TypeFlag::ETS_BOOLEAN:
            return ir::BoxingUnboxingFlags::UNBOX_TO_BOOLEAN;
        case TypeFlag::BYTE:
            return ir::BoxingUnboxingFlags::UNBOX_TO_BYTE;
        case TypeFlag::CHAR:
            return ir::BoxingUnboxingFlags::UNBOX_TO_CHAR;
        case TypeFlag::SHORT:
            return ir::BoxingUnboxingFlags::UNBOX_TO_SHORT;
        case TypeFlag::INT:
            return ir::BoxingUnboxingFlags::UNBOX_TO_INT;
        case TypeFlag::LONG:
            return ir::BoxingUnboxingFlags::UNBOX_TO_LONG;
        case TypeFlag::FLOAT:
            return ir::BoxingUnboxingFlags::UNBOX_TO_FLOAT;
        case TypeFlag::DOUBLE:
            return ir::BoxingUnboxingFlags::UNBOX_TO_DOUBLE;
        case TypeFlag::ETS_INT_ENUM:
        case TypeFlag::ETS_STRING_ENUM:
            return ir::BoxingUnboxingFlags::UNBOX_TO_ENUM;
        default:
            UNREACHABLE();
    }
}

void ETSChecker::MaybeAddBoxingFlagInRelation(TypeRelation *relation, Type *target)
{
    auto boxingResult = MaybeBoxInRelation(target);
    if ((boxingResult != nullptr) && !relation->OnlyCheckBoxingUnboxing()) {
        relation->GetNode()->AddBoxingUnboxingFlags(GetBoxingFlag(boxingResult));
        relation->Result(true);
    }
}

void ETSChecker::MaybeAddUnboxingFlagInRelation(TypeRelation *relation, Type *source, Type *self)
{
    auto unboxingResult = UnboxingConverter(this, relation, source, self).Result();
    if ((unboxingResult != nullptr) && relation->IsTrue() && !relation->OnlyCheckBoxingUnboxing()) {
        relation->GetNode()->AddBoxingUnboxingFlags(GetUnboxingFlag(unboxingResult));
    }
}

void ETSChecker::CheckUnboxedTypeWidenable(TypeRelation *relation, Type *target, Type *self)
{
    checker::SavedTypeRelationFlagsContext savedTypeRelationFlagCtx(
        relation, TypeRelationFlag::ONLY_CHECK_WIDENING |
                      (relation->ApplyNarrowing() ? TypeRelationFlag::NARROWING : TypeRelationFlag::NONE));
    // NOTE: vpukhov. handle union type
    auto unboxedType = MaybeUnboxInRelation(target);
    if (unboxedType == nullptr) {
        return;
    }
    NarrowingWideningConverter(this, relation, unboxedType, self);
    if (!relation->IsTrue()) {
        relation->Result(relation->IsAssignableTo(self, unboxedType));
    }
}

void ETSChecker::CheckUnboxedTypesAssignable(TypeRelation *relation, Type *source, Type *target)
{
    auto *unboxedSourceType = relation->GetChecker()->AsETSChecker()->MaybeUnboxInRelation(source);
    auto *unboxedTargetType = relation->GetChecker()->AsETSChecker()->MaybeUnboxInRelation(target);
    if (unboxedSourceType == nullptr || unboxedTargetType == nullptr) {
        return;
    }
    relation->IsAssignableTo(unboxedSourceType, unboxedTargetType);
    if (relation->IsTrue()) {
        relation->GetNode()->AddBoxingUnboxingFlags(
            relation->GetChecker()->AsETSChecker()->GetUnboxingFlag(unboxedSourceType));
    }
}

void ETSChecker::CheckBoxedSourceTypeAssignable(TypeRelation *relation, Type *source, Type *target)
{
    ASSERT(relation != nullptr);
    checker::SavedTypeRelationFlagsContext savedTypeRelationFlagCtx(
        relation, (relation->ApplyWidening() ? TypeRelationFlag::WIDENING : TypeRelationFlag::NONE) |
                      (relation->ApplyNarrowing() ? TypeRelationFlag::NARROWING : TypeRelationFlag::NONE) |
                      (relation->OnlyCheckBoxingUnboxing() ? TypeRelationFlag::ONLY_CHECK_BOXING_UNBOXING
                                                           : TypeRelationFlag::NONE));

    auto *boxedSourceType = relation->GetChecker()->AsETSChecker()->MaybeBoxInRelation(source);
    if (boxedSourceType == nullptr) {
        return;
    }
    ASSERT(target != nullptr);
    // Do not box primitive in case of cast to dynamic types
    if (target->IsETSDynamicType()) {
        return;
    }
    relation->IsAssignableTo(boxedSourceType, target);
    if (relation->IsTrue()) {
        MaybeAddBoxingFlagInRelation(relation, boxedSourceType);
    } else {
        auto unboxedTargetType = MaybeUnboxInRelation(target);
        if (unboxedTargetType == nullptr) {
            return;
        }
        NarrowingWideningConverter(this, relation, unboxedTargetType, source);
        if (relation->IsTrue()) {
            MaybeAddBoxingFlagInRelation(relation, target);
        }
    }
}

void ETSChecker::CheckUnboxedSourceTypeWithWideningAssignable(TypeRelation *relation, Type *source, Type *target)
{
    auto *unboxedSourceType = relation->GetChecker()->AsETSChecker()->MaybeUnboxInRelation(source);
    if (unboxedSourceType == nullptr) {
        return;
    }
    relation->IsAssignableTo(unboxedSourceType, target);
    if (!relation->IsTrue() && relation->ApplyWidening()) {
        relation->GetChecker()->AsETSChecker()->CheckUnboxedTypeWidenable(relation, target, unboxedSourceType);
    }
    if (!relation->OnlyCheckBoxingUnboxing()) {
        relation->GetNode()->AddBoxingUnboxingFlags(
            relation->GetChecker()->AsETSChecker()->GetUnboxingFlag(unboxedSourceType));
    }
}

static ir::AstNode *DerefETSTypeReference(ir::AstNode *node)
{
    ASSERT(node->IsETSTypeReference());
    do {
        auto *name = node->AsETSTypeReference()->Part()->Name();
        ASSERT(name->IsIdentifier());
        auto *var = name->AsIdentifier()->Variable();
        ASSERT(var != nullptr);
        auto *declNode = var->Declaration()->Node();
        if (!declNode->IsTSTypeAliasDeclaration()) {
            return declNode;
        }
        node = declNode->AsTSTypeAliasDeclaration()->TypeAnnotation();
    } while (node->IsETSTypeReference());
    return node;
}

bool ETSChecker::CheckLambdaAssignable(ir::Expression *param, ir::ScriptFunction *lambda)
{
    ASSERT(param->IsETSParameterExpression());
    ir::AstNode *typeAnn = param->AsETSParameterExpression()->Ident()->TypeAnnotation();
    if (typeAnn->IsETSTypeReference()) {
        typeAnn = DerefETSTypeReference(typeAnn);
    }
    if (!typeAnn->IsETSFunctionType()) {
        if (typeAnn->IsETSUnionType()) {
            return CheckLambdaAssignableUnion(typeAnn, lambda);
        }

        return false;
    }
    ir::ETSFunctionType *calleeType = typeAnn->AsETSFunctionType();
    return lambda->Params().size() == calleeType->Params().size();
}

bool ETSChecker::CheckLambdaInfer(ir::AstNode *typeAnnotation, ir::ArrowFunctionExpression *const arrowFuncExpr,
                                  Type *const subParameterType)
{
    if (typeAnnotation->IsETSTypeReference()) {
        typeAnnotation = DerefETSTypeReference(typeAnnotation);
    }

    if (!typeAnnotation->IsETSFunctionType()) {
        return false;
    }

    ir::ScriptFunction *const lambda = arrowFuncExpr->Function();
    auto calleeType = typeAnnotation->AsETSFunctionType();
    // Lambda function will only have exactly one signature.
    auto functionSignature =
        TryGettingFunctionTypeFromInvokeFunction(subParameterType)->AsETSFunctionType()->CallSignatures()[0];
    InferTypesForLambda(lambda, calleeType, functionSignature);

    return true;
}

bool ETSChecker::CheckLambdaTypeAnnotation(ir::AstNode *typeAnnotation,
                                           ir::ArrowFunctionExpression *const arrowFuncExpr, Type *const parameterType,
                                           TypeRelationFlag flags)
{
    auto checkInvocable = [&arrowFuncExpr, &parameterType, this](TypeRelationFlag functionFlags) {
        Type *const argumentType = arrowFuncExpr->Check(this);
        functionFlags |= TypeRelationFlag::NO_THROW;

        checker::InvocationContext invocationCtx(Relation(), arrowFuncExpr, argumentType, parameterType,
                                                 arrowFuncExpr->Start(), {}, functionFlags);
        return invocationCtx.IsInvocable();
    };

    //  process `single` type as usual.
    if (!typeAnnotation->IsETSUnionType()) {
        ASSERT(!parameterType->IsETSUnionType());
        return CheckLambdaInfer(typeAnnotation, arrowFuncExpr, parameterType) && checkInvocable(flags);
    }

    // Preserve actual lambda types
    ir::ScriptFunction *const lambda = arrowFuncExpr->Function();
    ArenaVector<ir::TypeNode *> lambdaParamTypes {Allocator()->Adapter()};
    for (auto *const lambdaParam : lambda->Params()) {
        lambdaParamTypes.emplace_back(lambdaParam->AsETSParameterExpression()->Ident()->TypeAnnotation());
    }
    auto *const lambdaReturnTypeAnnotation = lambda->ReturnTypeAnnotation();

    ASSERT(parameterType->AsETSUnionType()->ConstituentTypes().size() ==
           typeAnnotation->AsETSUnionType()->Types().size());
    const auto typeAnnsOfUnion = typeAnnotation->AsETSUnionType()->Types();
    const auto typeParamOfUnion = parameterType->AsETSUnionType()->ConstituentTypes();
    for (size_t ix = 0; ix < typeAnnsOfUnion.size(); ++ix) {
        auto *typeNode = typeAnnsOfUnion[ix];
        auto *paramNode = typeParamOfUnion[ix];
        if (CheckLambdaInfer(typeNode, arrowFuncExpr, paramNode) && checkInvocable(flags)) {
            return true;
        }

        //  Restore inferring lambda types:
        for (std::size_t i = 0U; i < lambda->Params().size(); ++i) {
            auto *const lambdaParamTypeAnnotation = lambdaParamTypes[i];
            if (lambdaParamTypeAnnotation == nullptr) {
                lambda->Params()[i]->AsETSParameterExpression()->Ident()->SetTsTypeAnnotation(nullptr);
            }
        }
        if (lambdaReturnTypeAnnotation == nullptr) {
            lambda->SetReturnTypeAnnotation(nullptr);
        }
    }

    return false;
}

bool ETSChecker::TypeInference(Signature *signature, const ArenaVector<ir::Expression *> &arguments,
                               TypeRelationFlag flags)
{
    bool invocable = true;
    auto const argumentCount = arguments.size();
    auto const parameterCount = signature->Params().size();
    auto const count = std::min(parameterCount, argumentCount);

    for (size_t index = 0U; index < count; ++index) {
        auto const &argument = arguments[index];
        if (!argument->IsArrowFunctionExpression()) {
            continue;
        }

        if (index == arguments.size() - 1 && (flags & TypeRelationFlag::NO_CHECK_TRAILING_LAMBDA) != 0) {
            continue;
        }

        auto *const arrowFuncExpr = argument->AsArrowFunctionExpression();
        ir::ScriptFunction *const lambda = arrowFuncExpr->Function();
        if (!NeedTypeInference(lambda)) {
            continue;
        }

        auto const *const param = signature->Function()->Params()[index]->AsETSParameterExpression()->Ident();
        ir::AstNode *typeAnn = param->TypeAnnotation();
        Type *const parameterType = signature->Params()[index]->TsType();

        bool const rc = CheckLambdaTypeAnnotation(typeAnn, arrowFuncExpr, parameterType, flags);
        if (!rc && (flags & TypeRelationFlag::NO_THROW) == 0) {
            Type *const argumentType = arrowFuncExpr->Check(this);
            const Type *targetType = TryGettingFunctionTypeFromInvokeFunction(parameterType);
            const std::initializer_list<TypeErrorMessageElement> list = {
                "Type '", argumentType, "' is not compatible with type '", targetType, "' at index ", index + 1};
            LogTypeError(list, arrowFuncExpr->Start());
            return false;
        }

        invocable &= rc;
    }

    return invocable;
}

bool ETSChecker::ExtensionETSFunctionType(checker::Type *type)
{
    if (!type->IsETSFunctionType()) {
        return false;
    }

    for (auto *signature : type->AsETSFunctionType()->CallSignatures()) {
        if (signature->Function()->IsExtensionMethod()) {
            return true;
        }
    }

    return false;
}

void ETSChecker::CheckExceptionClauseType(const std::vector<checker::ETSObjectType *> &exceptions,
                                          ir::CatchClause *catchClause, checker::Type *clauseType)
{
    for (auto *exception : exceptions) {
        this->Relation()->IsIdenticalTo(clauseType, exception);
        if (this->Relation()->IsTrue()) {
            LogTypeError("Redeclaration of exception type", catchClause->Start());
        }
    }
}
}  // namespace ark::es2panda::checker
