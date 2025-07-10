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

#include "ir/expressions/literals/bigIntLiteral.h"
#include "ir/expressions/literals/numberLiteral.h"
#include "ir/expressions/literals/stringLiteral.h"
#include "ir/expressions/functionExpression.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/base/property.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/spreadElement.h"
#include "ir/base/tsIndexSignature.h"
#include "ir/base/tsMethodSignature.h"
#include "ir/base/tsPropertySignature.h"
#include "ir/base/tsSignatureDeclaration.h"
#include "ir/ts/tsTypeLiteral.h"
#include "ir/ts/tsInterfaceDeclaration.h"
#include "ir/ts/tsInterfaceHeritage.h"
#include "ir/ts/tsInterfaceBody.h"
#include "util/helpers.h"
#include "varbinder/variable.h"
#include "varbinder/scope.h"

#include "checker/TSchecker.h"
#include "checker/types/ts/indexInfo.h"

namespace ark::es2panda::checker {
void TSChecker::CheckIndexConstraints(Type *type)
{
    if (!type->IsObjectType()) {
        return;
    }

    ObjectType *objType = type->AsObjectType();
    ResolveStructuredTypeMembers(objType);

    IndexInfo *numberInfo = objType->NumberIndexInfo();
    IndexInfo *stringInfo = objType->StringIndexInfo();
    const ArenaVector<varbinder::LocalVariable *> &properties = objType->Properties();

    if (numberInfo != nullptr) {
        for (auto *it : properties) {
            if (it->HasFlag(varbinder::VariableFlags::NUMERIC_NAME)) {
                Type *propType = GetTypeOfVariable(it);
                IsTypeAssignableTo(propType, numberInfo->GetType(),
                                   {"Property '", it->Name(), "' of type '", propType,
                                    "' is not assignable to numeric index type '", numberInfo->GetType(), "'."},
                                   it->Declaration()->Node()->Start());
            }
        }
    }

    if (stringInfo != nullptr) {
        for (auto *it : properties) {
            Type *propType = GetTypeOfVariable(it);
            IsTypeAssignableTo(propType, stringInfo->GetType(),
                               {"Property '", it->Name(), "' of type '", propType,
                                "' is not assignable to string index type '", stringInfo->GetType(), "'."},
                               it->Declaration()->Node()->Start());
        }

        if (numberInfo != nullptr && !IsTypeAssignableTo(numberInfo->GetType(), stringInfo->GetType())) {
            ThrowTypeError({"Number index info type ", numberInfo->GetType(),
                            " is not assignable to string index info type ", stringInfo->GetType(), "."},
                           numberInfo->Pos());
        }
    }
}

void TSChecker::ResolveStructuredTypeMembers(Type *type)
{
    if (type->IsObjectType()) {
        ObjectType *objType = type->AsObjectType();

        if (objType->IsObjectLiteralType()) {
            ResolveObjectTypeMembers(objType);
            return;
        }

        if (objType->IsInterfaceType()) {
            ResolveInterfaceOrClassTypeMembers(objType->AsInterfaceType());
            return;
        }
    }

    if (type->IsUnionType()) {
        ResolveUnionTypeMembers(type->AsUnionType());
        return;
    }
}

void TSChecker::ResolveUnionTypeMembers(UnionType *type)
{
    if (type->MergedObjectType() != nullptr) {
        return;
    }

    ObjectDescriptor *desc = Allocator()->New<ObjectDescriptor>(Allocator());
    ArenaVector<Type *> stringInfoTypes(Allocator()->Adapter());
    ArenaVector<Type *> numberInfoTypes(Allocator()->Adapter());
    ArenaVector<Signature *> callSignatures(Allocator()->Adapter());
    ArenaVector<Signature *> constructSignatures(Allocator()->Adapter());

    for (auto *it : type->AsUnionType()->ConstituentTypes()) {
        if (!it->IsObjectType()) {
            continue;
        }

        ObjectType *objType = it->AsObjectType();
        ResolveObjectTypeMembers(objType);

        if (!objType->CallSignatures().empty()) {
            for (auto *signature : objType->CallSignatures()) {
                callSignatures.push_back(signature);
            }
        }

        if (!objType->ConstructSignatures().empty()) {
            for (auto *signature : objType->ConstructSignatures()) {
                constructSignatures.push_back(signature);
            }
        }

        if (objType->StringIndexInfo() != nullptr) {
            stringInfoTypes.push_back(objType->StringIndexInfo()->GetType());
        }

        if (objType->NumberIndexInfo() != nullptr) {
            numberInfoTypes.push_back(objType->NumberIndexInfo()->GetType());
        }
    }

    desc->callSignatures = callSignatures;
    desc->constructSignatures = constructSignatures;

    if (!stringInfoTypes.empty()) {
        desc->stringIndexInfo = Allocator()->New<IndexInfo>(CreateUnionType(std::move(stringInfoTypes)), "x", false);
    }

    if (!numberInfoTypes.empty()) {
        desc->numberIndexInfo = Allocator()->New<IndexInfo>(CreateUnionType(std::move(numberInfoTypes)), "x", false);
    }

    ObjectType *mergedType = Allocator()->New<ObjectLiteralType>(desc);
    mergedType->AddObjectFlag(ObjectFlags::RESOLVED_MEMBERS);
    type->SetMergedObjectType(mergedType);
}

void TSChecker::ResolveInterfaceOrClassTypeMembers(InterfaceType *type)
{
    if (type->HasObjectFlag(ObjectFlags::RESOLVED_MEMBERS)) {
        return;
    }

    ResolveDeclaredMembers(type);
    GetBaseTypes(type);

    type->AddObjectFlag(ObjectFlags::RESOLVED_MEMBERS);
}

void TSChecker::ResolveObjectTypeMembers(ObjectType *type)
{
    if (!type->IsObjectLiteralType() || type->HasObjectFlag(ObjectFlags::RESOLVED_MEMBERS)) {
        return;
    }

    ASSERT(type->Variable() && type->Variable()->Declaration()->Node()->IsTSTypeLiteral());
    auto *typeLiteral = type->Variable()->Declaration()->Node()->AsTSTypeLiteral();
    ArenaVector<ir::TSSignatureDeclaration *> signatureDeclarations(Allocator()->Adapter());
    ArenaVector<ir::TSIndexSignature *> indexDeclarations(Allocator()->Adapter());

    for (auto *it : typeLiteral->Members()) {
        ResolvePropertiesOfObjectType(type, it, signatureDeclarations, indexDeclarations, false);
    }

    type->AddObjectFlag(ObjectFlags::RESOLVED_MEMBERS);

    ResolveSignaturesOfObjectType(type, signatureDeclarations);
    ResolveIndexInfosOfObjectType(type, indexDeclarations);
}

void TSChecker::ResolvePropertiesOfObjectType(ObjectType *type, ir::AstNode *member,
                                              ArenaVector<ir::TSSignatureDeclaration *> &signatureDeclarations,
                                              ArenaVector<ir::TSIndexSignature *> &indexDeclarations, bool isInterface)
{
    if (member->IsTSPropertySignature()) {
        varbinder::Variable *prop = member->AsTSPropertySignature()->Variable();

        if (!isInterface ||
            ValidateInterfaceMemberRedeclaration(type, prop, member->AsTSPropertySignature()->Key()->Start())) {
            type->AddProperty(prop->AsLocalVariable());
        }

        return;
    }

    if (member->IsTSMethodSignature()) {
        varbinder::Variable *method = member->AsTSMethodSignature()->Variable();

        if (!isInterface ||
            ValidateInterfaceMemberRedeclaration(type, method, member->AsTSMethodSignature()->Key()->Start())) {
            type->AddProperty(method->AsLocalVariable());
        }

        return;
    }

    if (member->IsTSSignatureDeclaration()) {
        signatureDeclarations.push_back(member->AsTSSignatureDeclaration());
        return;
    }

    ASSERT(member->IsTSIndexSignature());
    indexDeclarations.push_back(member->AsTSIndexSignature());
}

void TSChecker::ResolveSignaturesOfObjectType(ObjectType *type,
                                              ArenaVector<ir::TSSignatureDeclaration *> &signatureDeclarations)
{
    for (auto *it : signatureDeclarations) {
        Type *placeholderObj = it->Check(this);

        if (it->AsTSSignatureDeclaration()->Kind() ==
            ir::TSSignatureDeclaration::TSSignatureDeclarationKind::CALL_SIGNATURE) {
            type->AddCallSignature(placeholderObj->AsObjectType()->CallSignatures()[0]);
            continue;
        }

        type->AddConstructSignature(placeholderObj->AsObjectType()->ConstructSignatures()[0]);
    }
}
void TSChecker::ResolveIndexInfosOfObjectType(ObjectType *type, ArenaVector<ir::TSIndexSignature *> &indexDeclarations)
{
    for (auto *it : indexDeclarations) {
        Type *placeholderObj = it->Check(this);

        if (it->AsTSIndexSignature()->Kind() == ir::TSIndexSignature::TSIndexSignatureKind::NUMBER) {
            IndexInfo *numberInfo = placeholderObj->AsObjectType()->NumberIndexInfo();

            if (type->NumberIndexInfo() != nullptr) {
                ThrowTypeError("Duplicated index signature for type 'number'", it->Start());
            }

            type->Desc()->numberIndexInfo = numberInfo;
            continue;
        }

        IndexInfo *stringInfo = placeholderObj->AsObjectType()->StringIndexInfo();

        if (type->StringIndexInfo() != nullptr) {
            ThrowTypeError("Duplicated index signature for type 'string'", it->Start());
        }

        type->Desc()->stringIndexInfo = stringInfo;
    }
}

varbinder::Variable *TSChecker::GetPropertyOfType(Type *type, const util::StringView &name, bool getPartial,
                                                  varbinder::VariableFlags propagateFlags)
{
    if (type->IsObjectType()) {
        ResolveObjectTypeMembers(type->AsObjectType());
        return type->AsObjectType()->GetProperty(name, true);
    }

    if (type->IsUnionType()) {
        return GetPropertyOfUnionType(type->AsUnionType(), name, getPartial, propagateFlags);
    }

    return nullptr;
}

varbinder::Variable *TSChecker::GetPropertyOfUnionType(UnionType *type, const util::StringView &name, bool getPartial,
                                                       varbinder::VariableFlags propagateFlags)
{
    auto found = type->CachedSyntheticProperties().find(name);
    if (found != type->CachedSyntheticProperties().end()) {
        return found->second;
    }

    varbinder::VariableFlags flags = varbinder::VariableFlags::PROPERTY;
    ArenaVector<Type *> collectedTypes(Allocator()->Adapter());

    for (auto *it : type->ConstituentTypes()) {
        varbinder::Variable *prop = GetPropertyOfType(it, name);

        if (prop == nullptr) {
            if (it->IsArrayType()) {
                collectedTypes.push_back(it->AsArrayType()->ElementType());
                continue;
            }

            if (!it->IsObjectType() && getPartial) {
                continue;
            }
            if (!it->IsObjectType() && !getPartial) {
                return nullptr;
            }

            ObjectType *objType = it->AsObjectType();

            if (objType->StringIndexInfo() == nullptr && getPartial) {
                continue;
            }
            if (objType->StringIndexInfo() == nullptr && !getPartial) {
                return nullptr;
            }

            collectedTypes.push_back(objType->StringIndexInfo()->GetType());
            continue;
        }

        prop->AddFlag(propagateFlags);

        if (prop->HasFlag(varbinder::VariableFlags::OPTIONAL)) {
            flags |= varbinder::VariableFlags::OPTIONAL;
        }

        collectedTypes.push_back(GetTypeOfVariable(prop));
    }

    if (collectedTypes.empty()) {
        return nullptr;
    }

    varbinder::Variable *syntheticProp = varbinder::Scope::CreateVar(Allocator(), name, flags, nullptr);
    syntheticProp->SetTsType(CreateUnionType(std::move(collectedTypes)));
    type->CachedSyntheticProperties().insert({name, syntheticProp});
    return syntheticProp;
}

Type *TSChecker::CheckComputedPropertyName(ir::Expression *key)
{
    if (key->TsType() != nullptr) {
        return key->TsType();
    }

    Type *keyType = key->Check(this);

    if (!keyType->HasTypeFlag(TypeFlag::STRING_LIKE | TypeFlag::NUMBER_LIKE)) {
        ThrowTypeError(
            "A computed property name in a type literal must refer to an expression whose type is a literal "
            "type "
            "or a 'unique symbol' type",
            key->Start());
    }

    key->SetTsType(keyType);
    return keyType;
}

IndexInfo *TSChecker::GetApplicableIndexInfo(Type *type, Type *indexType)
{
    ResolveStructuredTypeMembers(type);
    bool getNumberInfo = indexType->HasTypeFlag(TypeFlag::NUMBER_LIKE);

    if (type->IsObjectType()) {
        if (getNumberInfo) {
            return type->AsObjectType()->NumberIndexInfo();
        }

        return type->AsObjectType()->StringIndexInfo();
    }

    if (type->IsUnionType()) {
        ASSERT(type->AsUnionType()->MergedObjectType());

        if (getNumberInfo) {
            return type->AsUnionType()->MergedObjectType()->NumberIndexInfo();
        }

        return type->AsUnionType()->MergedObjectType()->StringIndexInfo();
    }

    return nullptr;
}

Type *TSChecker::GetPropertyTypeForIndexType(Type *type, Type *indexType)
{
    if (type->IsArrayType()) {
        return type->AsArrayType()->ElementType();
    }

    if (indexType->HasTypeFlag(TypeFlag::STRING_LITERAL | TypeFlag::NUMBER_LITERAL)) {
        varbinder::Variable *prop = nullptr;

        if (indexType->IsStringLiteralType()) {
            prop = GetPropertyOfType(type, indexType->AsStringLiteralType()->Value());
        } else {
            util::StringView propName =
                util::Helpers::ToStringView(Allocator(), indexType->AsNumberLiteralType()->Value());
            prop = GetPropertyOfType(type, propName);
        }

        if (prop != nullptr) {
            Type *propType = GetTypeOfVariable(prop);

            if (prop->HasFlag(varbinder::VariableFlags::READONLY)) {
                propType->AddTypeFlag(TypeFlag::READONLY);
            }

            return propType;
        }
    }

    if (indexType->HasTypeFlag(TypeFlag::STRING_LIKE | TypeFlag::NUMBER_LIKE)) {
        IndexInfo *indexInfo = GetApplicableIndexInfo(type, indexType);

        if (indexInfo != nullptr) {
            Type *indexInfoType = indexInfo->GetType();

            if (indexInfo->Readonly()) {
                indexInfoType->AddTypeFlag(TypeFlag::READONLY);
            }

            return indexInfoType;
        }
    }

    return nullptr;
}

ArenaVector<ObjectType *> TSChecker::GetBaseTypes(InterfaceType *type)
{
    if (type->HasObjectFlag(ObjectFlags::RESOLVED_BASE_TYPES)) {
        return type->Bases();
    }

    ASSERT(type->Variable() && type->Variable()->Declaration()->IsInterfaceDecl());
    varbinder::InterfaceDecl *decl = type->Variable()->Declaration()->AsInterfaceDecl();

    TypeStackElement tse(this, type, {"Type ", type->Name(), " recursively references itself as a base type."},
                         decl->Node()->AsTSInterfaceDeclaration()->Id()->Start());
    if (tse.HasTypeError()) {
        type->Bases().clear();
        return type->Bases();
    }

    for (const auto *declaration : decl->Decls()) {
        if (declaration->Extends().empty()) {
            continue;
        }

        for (auto *extends : declaration->Extends()) {
            Type *baseType = extends->Expr()->GetType(this);

            if (!baseType->HasTypeFlag(TypeFlag::OBJECT | TypeFlag::NON_PRIMITIVE | TypeFlag::ANY)) {
                ThrowTypeError(
                    "An interface can only extend an object type or intersection of object types with statically "
                    "known "
                    "members",
                    extends->Start());
            }

            if (!baseType->IsObjectType()) {
                continue;
            }

            ObjectType *baseObj = baseType->AsObjectType();

            if (baseType == type) {
                ThrowTypeError({"Type ", type->Name(), " recursively references itself as a base type."},
                               decl->Node()->AsTSInterfaceDeclaration()->Id()->Start());
            }

            type->AddBase(baseObj);

            if (!baseObj->IsInterfaceType()) {
                continue;
            }

            CheckExtendsBases(baseObj, type, decl);
        }
    }

    type->AddObjectFlag(ObjectFlags::RESOLVED_BASE_TYPES);
    return type->Bases();
}

void TSChecker::CheckExtendsBases(ObjectType *&baseObj, InterfaceType *&type, varbinder::InterfaceDecl *&decl)
{
    ArenaVector<ObjectType *> extendsBases = GetBaseTypes(baseObj->AsInterfaceType());
    for (auto *extendBase : extendsBases) {
        if (extendBase == type) {
            ThrowTypeError({"Type ", type->Name(), " recursively references itself as a base type."},
                           decl->Node()->AsTSInterfaceDeclaration()->Id()->Start());
        }
    }
}

void TSChecker::ResolveDeclaredMembers(InterfaceType *type)
{
    if (type->HasObjectFlag(ObjectFlags::RESOLVED_DECLARED_MEMBERS)) {
        return;
    }

    ASSERT(type->Variable() && type->Variable()->Declaration()->IsInterfaceDecl());
    varbinder::InterfaceDecl *decl = type->Variable()->Declaration()->AsInterfaceDecl();

    ArenaVector<ir::TSSignatureDeclaration *> signatureDeclarations(Allocator()->Adapter());
    ArenaVector<ir::TSIndexSignature *> indexDeclarations(Allocator()->Adapter());

    for (const auto *declaration : decl->Decls()) {
        for (auto *member : declaration->Body()->Body()) {
            ResolvePropertiesOfObjectType(type, member, signatureDeclarations, indexDeclarations, true);
        }

        type->AddObjectFlag(ObjectFlags::RESOLVED_DECLARED_MEMBERS);

        ResolveSignaturesOfObjectType(type, signatureDeclarations);
        ResolveIndexInfosOfObjectType(type, indexDeclarations);
    }
}

bool TSChecker::ValidateInterfaceMemberRedeclaration(ObjectType *type, varbinder::Variable *prop,
                                                     const lexer::SourcePosition &locInfo)
{
    if (prop->HasFlag(varbinder::VariableFlags::COMPUTED)) {
        return true;
    }

    varbinder::Variable *found = type->GetProperty(prop->Name(), false);

    if (found == nullptr) {
        return true;
    }

    Type *targetType = GetTypeOfVariable(prop);
    Type *sourceType = GetTypeOfVariable(found);
    IsTypeIdenticalTo(targetType, sourceType,
                      {"Subsequent property declarations must have the same type.  Property ", prop->Name(),
                       " must be of type ", sourceType, ", but here has type ", targetType, "."},
                      locInfo);
    return false;
}
}  // namespace ark::es2panda::checker
