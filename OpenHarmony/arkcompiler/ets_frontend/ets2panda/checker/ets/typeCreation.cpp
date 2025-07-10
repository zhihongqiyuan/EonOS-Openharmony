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

#include <functional>
#include "checker/ETSchecker.h"

#include "checker/types/globalTypesHolder.h"
#include "checker/types/ets/byteType.h"
#include "checker/types/ets/charType.h"
#include "checker/types/ets/etsDynamicFunctionType.h"
#include "checker/types/ets/etsDynamicType.h"
#include "checker/types/ets/etsStringType.h"
#include "checker/types/ets/etsUnionType.h"
#include "checker/types/ets/shortType.h"
#include "compiler/lowering/ets/enumLowering.h"
#include "generated/signatures.h"
#include "ir/base/classDefinition.h"
#include "ir/statements/classDeclaration.h"
#include "ir/base/scriptFunction.h"
#include "ir/ets/etsScript.h"
#include "ir/expressions/identifier.h"
#include "ir/ts/tsEnumDeclaration.h"
#include "ir/ts/tsEnumMember.h"
#include "ir/ts/tsInterfaceDeclaration.h"
#include "checker/ets/boxingConverter.h"
#include "util/helpers.h"
#include "checker/types/ts/bigintType.h"

namespace ark::es2panda::checker {
ByteType *ETSChecker::CreateByteType(int8_t value)
{
    return Allocator()->New<ByteType>(value);
}

ETSBooleanType *ETSChecker::CreateETSBooleanType(bool value)
{
    return Allocator()->New<ETSBooleanType>(value);
}

DoubleType *ETSChecker::CreateDoubleType(double value)
{
    return Allocator()->New<DoubleType>(value);
}

FloatType *ETSChecker::CreateFloatType(float value)
{
    return Allocator()->New<FloatType>(value);
}

IntType *ETSChecker::CreateIntType(int32_t value)
{
    return Allocator()->New<IntType>(value);
}

IntType *ETSChecker::CreateIntTypeFromType(Type *type)
{
    if (!type->HasTypeFlag(TypeFlag::CONSTANT)) {
        return GlobalIntType()->AsIntType();
    }

    if (type->IsIntType()) {
        return type->AsIntType();
    }

    switch (ETSType(type)) {
        case TypeFlag::CHAR: {
            return CreateIntType(static_cast<int32_t>(type->AsCharType()->GetValue()));
        }
        case TypeFlag::BYTE: {
            return CreateIntType(static_cast<int32_t>(type->AsByteType()->GetValue()));
        }
        case TypeFlag::SHORT: {
            return CreateIntType(static_cast<int32_t>(type->AsShortType()->GetValue()));
        }
        default: {
            return nullptr;
        }
    }
}

LongType *ETSChecker::CreateLongType(int64_t value)
{
    return Allocator()->New<LongType>(value);
}

ShortType *ETSChecker::CreateShortType(int16_t value)
{
    return Allocator()->New<ShortType>(value);
}

CharType *ETSChecker::CreateCharType(char16_t value)
{
    return Allocator()->New<CharType>(value);
}

ETSBigIntType *ETSChecker::CreateETSBigIntLiteralType(util::StringView value)
{
    return Allocator()->New<ETSBigIntType>(Allocator(), GlobalBuiltinETSBigIntType(), Relation(), value);
}

ETSStringType *ETSChecker::CreateETSStringLiteralType(util::StringView value)
{
    return Allocator()->New<ETSStringType>(Allocator(), GlobalBuiltinETSStringType(), Relation(), value);
}

ETSArrayType *ETSChecker::CreateETSArrayType(Type *elementType)
{
    auto res = arrayTypes_.find(elementType);
    if (res != arrayTypes_.end()) {
        return res->second;
    }

    auto *arrayType = Allocator()->New<ETSArrayType>(elementType);

    std::stringstream ss;
    arrayType->ToAssemblerTypeWithRank(ss);
    arrayType->SetAssemblerName(util::UString(ss.str(), Allocator()).View());

    auto it = arrayTypes_.insert({elementType, arrayType});
    if (it.second && (!elementType->IsTypeParameter() || !elementType->IsETSTypeParameter())) {
        CreateBuiltinArraySignature(arrayType, arrayType->Rank());
    }

    return arrayType;
}

namespace {
[[nodiscard]] checker::ETSFunctionType *MakeProxyFunctionType(
    checker::ETSChecker *const checker, const util::StringView &name,
    const std::initializer_list<varbinder::LocalVariable *> &params, ir::ScriptFunction *const globalFunction,
    checker::Type *const returnType)
{
    auto *const signatureInfo = checker->CreateSignatureInfo();
    signatureInfo->params.insert(signatureInfo->params.end(), params);
    signatureInfo->minArgCount = signatureInfo->params.size();

    auto *const signature = checker->CreateSignature(signatureInfo, returnType, name);
    signature->SetFunction(globalFunction);
    signature->AddSignatureFlag(checker::SignatureFlags::PROXY);

    return checker->CreateETSFunctionType(signature, name);
}

[[nodiscard]] checker::Signature *MakeGlobalSignature(checker::ETSChecker *const checker,
                                                      ir::ScriptFunction *const function,
                                                      checker::Type *const returnType)
{
    auto *const signatureInfo = checker->CreateSignatureInfo();
    signatureInfo->params.reserve(function->Params().size());
    for (const auto *const param : function->Params()) {
        signatureInfo->params.push_back(param->AsETSParameterExpression()->Variable()->AsLocalVariable());
    }
    signatureInfo->minArgCount = signatureInfo->params.size();

    auto *const signature = checker->CreateSignature(signatureInfo, returnType, function);
    signature->AddSignatureFlag(checker::SignatureFlags::PUBLIC | checker::SignatureFlags::STATIC);
    function->SetSignature(signature);

    return signature;
}

void SetTypesForScriptFunction(checker::ETSChecker *const checker, ir::ScriptFunction *function)
{
    for (auto param : function->Params()) {
        ASSERT(param->IsETSParameterExpression());
        auto paramType = param->AsETSParameterExpression()->TypeAnnotation()->Check(checker);
        param->AsETSParameterExpression()->Ident()->SetTsType(paramType);
        param->AsETSParameterExpression()->Ident()->Variable()->SetTsType(paramType);
        param->SetTsType(paramType);
    }
}

}  // namespace

ETSEnumType::Method ETSChecker::MakeMethod(ir::TSEnumDeclaration const *const enumDecl, const std::string_view &name,
                                           bool buildPorxyParam, Type *returnType, bool buildProxy)
{
    auto function = FindFunction(enumDecl, name);
    if (function == nullptr) {
        return {};
    }

    SetTypesForScriptFunction(this, function);

    if (buildPorxyParam) {
        return {MakeGlobalSignature(this, function, returnType),
                MakeProxyFunctionType(
                    this, name, {function->Params()[0]->AsETSParameterExpression()->Variable()->AsLocalVariable()},
                    function, returnType)};
    }
    return {MakeGlobalSignature(this, function, returnType),
            buildProxy ? MakeProxyFunctionType(this, name, {}, function, returnType) : nullptr};
}

[[nodiscard]] ir::ScriptFunction *ETSChecker::FindFunction(ir::TSEnumDeclaration const *const enumDecl,
                                                           const std::string_view &name)
{
    if (enumDecl->BoxedClass() == nullptr) {
        return nullptr;
    }

    for (auto m : enumDecl->BoxedClass()->Body()) {
        if (m->IsMethodDefinition()) {
            if (m->AsMethodDefinition()->Id()->Name() == name) {
                return m->AsMethodDefinition()->Function();
            }
        }
    }
    return nullptr;
}

template <typename EnumType>
EnumType *ETSChecker::CreateEnumTypeFromEnumDeclaration(ir::TSEnumDeclaration const *const enumDecl)
{
    static_assert(std::is_same_v<EnumType, ETSIntEnumType> || std::is_same_v<EnumType, ETSStringEnumType>);
    SavedCheckerContext savedContext(this, Context().Status(), Context().ContainingClass(),
                                     Context().ContainingSignature());

    varbinder::Variable *enumVar = enumDecl->Key()->Variable();
    ASSERT(enumVar != nullptr);

    checker::ETSEnumType::UType ordinal = -1;
    auto *const enumType = Allocator()->New<EnumType>(enumDecl, ordinal++);
    auto *const boxedEnumType = enumDecl->BoxedClass()->TsType();

    enumType->SetVariable(enumVar);
    enumVar->SetTsType(enumType);

    auto const getNameMethod =
        MakeMethod(enumDecl, ETSEnumType::GET_NAME_METHOD_NAME, false, GlobalETSStringLiteralType());
    enumType->SetGetNameMethod(getNameMethod);

    auto getValueOfMethod = MakeMethod(enumDecl, ETSEnumType::GET_VALUE_OF_METHOD_NAME, true, enumType);
    enumType->SetGetValueOfMethod(getValueOfMethod);

    auto const fromIntMethod = MakeMethod(enumDecl, ETSEnumType::FROM_INT_METHOD_NAME, false, enumType, false);
    enumType->SetFromIntMethod(fromIntMethod);

    auto const boxedFromIntMethod =
        MakeMethod(enumDecl, ETSEnumType::BOXED_FROM_INT_METHOD_NAME, false, boxedEnumType, false);
    enumType->SetBoxedFromIntMethod(boxedFromIntMethod);

    auto const unboxMethod = MakeMethod(enumDecl, ETSEnumType::UNBOX_METHOD_NAME, false, enumType);
    enumType->SetUnboxMethod(unboxMethod);

    auto const toStringMethod =
        MakeMethod(enumDecl, ETSEnumType::TO_STRING_METHOD_NAME, false, GlobalETSStringLiteralType());
    enumType->SetToStringMethod(toStringMethod);

    ETSEnumType::Method valueOfMethod = toStringMethod;
    if (std::is_same_v<EnumType, ETSIntEnumType>) {
        valueOfMethod = MakeMethod(enumDecl, ETSEnumType::VALUE_OF_METHOD_NAME, false, GlobalIntType());
    }
    enumType->SetValueOfMethod(valueOfMethod);

    auto const valuesMethod =
        MakeMethod(enumDecl, ETSEnumType::VALUES_METHOD_NAME, false, CreateETSArrayType(enumType));
    enumType->SetValuesMethod(valuesMethod);

    for (auto *const member : enumType->GetMembers()) {
        auto *const memberVar = member->AsTSEnumMember()->Key()->AsIdentifier()->Variable();
        auto *const enumLiteralType = Allocator()->New<EnumType>(enumDecl, ordinal++, member->AsTSEnumMember());
        enumLiteralType->SetVariable(memberVar);
        memberVar->SetTsType(enumLiteralType);

        enumLiteralType->SetGetNameMethod(getNameMethod);
        enumLiteralType->SetGetValueOfMethod(getValueOfMethod);
        enumLiteralType->SetFromIntMethod(fromIntMethod);
        enumLiteralType->SetBoxedFromIntMethod(boxedFromIntMethod);
        enumLiteralType->SetUnboxMethod(unboxMethod);
        enumLiteralType->SetValueOfMethod(valueOfMethod);
        enumLiteralType->SetToStringMethod(toStringMethod);
        enumLiteralType->SetValuesMethod(valuesMethod);
    }
    return enumType;
}

ETSIntEnumType *ETSChecker::CreateEnumIntTypeFromEnumDeclaration(ir::TSEnumDeclaration *const enumDecl)
{
    auto etsEnumType = CreateEnumTypeFromEnumDeclaration<ETSIntEnumType>(enumDecl);
    enumDecl->SetTsType(etsEnumType);
    return etsEnumType;
}

ETSStringEnumType *ETSChecker::CreateEnumStringTypeFromEnumDeclaration(ir::TSEnumDeclaration *const enumDecl)
{
    auto etsEnumType = CreateEnumTypeFromEnumDeclaration<ETSStringEnumType>(enumDecl);
    enumDecl->SetTsType(etsEnumType);
    return etsEnumType;
}

Type *ETSChecker::CreateETSUnionType(Span<Type *const> constituentTypes)
{
    if (constituentTypes.empty()) {
        return nullptr;
    }

    ArenaVector<Type *> newConstituentTypes(Allocator()->Adapter());
    newConstituentTypes.assign(constituentTypes.begin(), constituentTypes.end());

    ETSUnionType::NormalizeTypes(Relation(), newConstituentTypes);
    if (newConstituentTypes.size() == 1) {
        return newConstituentTypes[0];
    }
    return Allocator()->New<ETSUnionType>(this, std::move(newConstituentTypes));
}

ETSTypeAliasType *ETSChecker::CreateETSTypeAliasType(util::StringView name, const ir::AstNode *declNode,
                                                     bool isRecursive)
{
    return Allocator()->New<ETSTypeAliasType>(this, name, declNode, isRecursive);
}

ETSFunctionType *ETSChecker::CreateETSFunctionType(ArenaVector<Signature *> &signatures)
{
    auto *funcType = Allocator()->New<ETSFunctionType>(signatures[0]->Function()->Id()->Name(), Allocator());

    for (auto *it : signatures) {
        funcType->AddCallSignature(it);
    }

    return funcType;
}

ETSFunctionType *ETSChecker::CreateETSFunctionType(Signature *signature)
{
    return Allocator()->New<ETSFunctionType>(signature->Function()->Id()->Name(), signature, Allocator());
}

ETSFunctionType *ETSChecker::CreateETSFunctionType(Signature *signature, util::StringView name)
{
    return Allocator()->New<ETSFunctionType>(name, signature, Allocator());
}

ETSFunctionType *ETSChecker::CreateETSFunctionType(ir::ScriptFunction *func, Signature *signature,
                                                   util::StringView name)
{
    if (func->IsDynamic()) {
        return Allocator()->New<ETSDynamicFunctionType>(name, signature, Allocator(), func->Language());
    }

    return Allocator()->New<ETSFunctionType>(name, signature, Allocator());
}

ETSFunctionType *ETSChecker::CreateETSFunctionType(ir::ScriptFunction *func, ArenaVector<Signature *> &&signatures,
                                                   util::StringView name)
{
    if (func->IsDynamic()) {
        return Allocator()->New<ETSDynamicFunctionType>(this, name, std::move(signatures), func->Language());
    }

    return Allocator()->New<ETSFunctionType>(this, name, std::move(signatures));
}

Signature *ETSChecker::CreateSignature(SignatureInfo *info, Type *returnType, ir::ScriptFunction *func)
{
    return Allocator()->New<Signature>(info, returnType, func);
}

Signature *ETSChecker::CreateSignature(SignatureInfo *info, Type *returnType, util::StringView internalName)
{
    return Allocator()->New<Signature>(info, returnType, internalName);
}

SignatureInfo *ETSChecker::CreateSignatureInfo()
{
    return Allocator()->New<SignatureInfo>(Allocator());
}

ETSTypeParameter *ETSChecker::CreateTypeParameter()
{
    return Allocator()->New<ETSTypeParameter>();
}

ETSFunctionType *ETSChecker::CreateETSFunctionType(util::StringView name)
{
    return Allocator()->New<ETSFunctionType>(name, Allocator());
}

ETSExtensionFuncHelperType *ETSChecker::CreateETSExtensionFuncHelperType(ETSFunctionType *classMethodType,
                                                                         ETSFunctionType *extensionFunctionType)
{
    return Allocator()->New<ETSExtensionFuncHelperType>(classMethodType, extensionFunctionType);
}

std::map<util::StringView, GlobalTypeId> &GetNameToTypeIdMap()
{
    static std::map<util::StringView, GlobalTypeId> nameToTypeId = {
        {compiler::Signatures::BUILTIN_BIGINT_CLASS, GlobalTypeId::ETS_BIG_INT_BUILTIN},
        {compiler::Signatures::BUILTIN_STRING_CLASS, GlobalTypeId::ETS_STRING_BUILTIN},
        {compiler::Signatures::BUILTIN_OBJECT_CLASS, GlobalTypeId::ETS_OBJECT_BUILTIN},
        {compiler::Signatures::BUILTIN_EXCEPTION_CLASS, GlobalTypeId::ETS_EXCEPTION_BUILTIN},
        {compiler::Signatures::BUILTIN_ERROR_CLASS, GlobalTypeId::ETS_ERROR_BUILTIN},
        {compiler::Signatures::BUILTIN_TYPE_CLASS, GlobalTypeId::ETS_TYPE_BUILTIN},
        {compiler::Signatures::BUILTIN_PROMISE_CLASS, GlobalTypeId::ETS_PROMISE_BUILTIN},
        {compiler::Signatures::BUILTIN_BOX_CLASS, GlobalTypeId::ETS_BOX_BUILTIN},
        {compiler::Signatures::BUILTIN_BOOLEAN_BOX_CLASS, GlobalTypeId::ETS_BOOLEAN_BOX_BUILTIN},
        {compiler::Signatures::BUILTIN_BYTE_BOX_CLASS, GlobalTypeId::ETS_BYTE_BOX_BUILTIN},
        {compiler::Signatures::BUILTIN_CHAR_BOX_CLASS, GlobalTypeId::ETS_CHAR_BOX_BUILTIN},
        {compiler::Signatures::BUILTIN_SHORT_BOX_CLASS, GlobalTypeId::ETS_SHORT_BOX_BUILTIN},
        {compiler::Signatures::BUILTIN_INT_BOX_CLASS, GlobalTypeId::ETS_INT_BOX_BUILTIN},
        {compiler::Signatures::BUILTIN_LONG_BOX_CLASS, GlobalTypeId::ETS_LONG_BOX_BUILTIN},
        {compiler::Signatures::BUILTIN_FLOAT_BOX_CLASS, GlobalTypeId::ETS_FLOAT_BOX_BUILTIN},
        {compiler::Signatures::BUILTIN_DOUBLE_BOX_CLASS, GlobalTypeId::ETS_DOUBLE_BOX_BUILTIN},
    };

    return nameToTypeId;
}

std::map<util::StringView, std::function<ETSObjectType *(const ETSChecker *)>> &GetNameToGlobalTypeMap()
{
    static std::map<util::StringView, std::function<ETSObjectType *(const ETSChecker *)>> nameToGlobalType = {
        {compiler::Signatures::BUILTIN_BIGINT_CLASS, &ETSChecker::GlobalBuiltinETSBigIntType},
        {compiler::Signatures::BUILTIN_STRING_CLASS, &ETSChecker::GlobalBuiltinETSStringType},
        {compiler::Signatures::BUILTIN_OBJECT_CLASS, &ETSChecker::GlobalETSObjectType},
        {compiler::Signatures::BUILTIN_EXCEPTION_CLASS, &ETSChecker::GlobalBuiltinExceptionType},
        {compiler::Signatures::BUILTIN_ERROR_CLASS, &ETSChecker::GlobalBuiltinErrorType},
        {compiler::Signatures::BUILTIN_TYPE_CLASS, &ETSChecker::GlobalBuiltinTypeType},
        {compiler::Signatures::BUILTIN_PROMISE_CLASS, &ETSChecker::GlobalBuiltinPromiseType},
    };

    return nameToGlobalType;
}

std::map<util::StringView, std::function<Type *(const ETSChecker *)>> &GetNameToGlobalBoxTypeMap()
{
    static std::map<util::StringView, std::function<Type *(const ETSChecker *)>> nameToGlobalBoxType = {
        {compiler::Signatures::BUILTIN_BOX_CLASS, &ETSChecker::GlobalETSObjectType},
        {compiler::Signatures::BUILTIN_BOOLEAN_BOX_CLASS, &ETSChecker::GlobalETSBooleanType},
        {compiler::Signatures::BUILTIN_BYTE_BOX_CLASS, &ETSChecker::GlobalByteType},
        {compiler::Signatures::BUILTIN_CHAR_BOX_CLASS, &ETSChecker::GlobalCharType},
        {compiler::Signatures::BUILTIN_SHORT_BOX_CLASS, &ETSChecker::GlobalShortType},
        {compiler::Signatures::BUILTIN_INT_BOX_CLASS, &ETSChecker::GlobalIntType},
        {compiler::Signatures::BUILTIN_LONG_BOX_CLASS, &ETSChecker::GlobalLongType},
        {compiler::Signatures::BUILTIN_FLOAT_BOX_CLASS, &ETSChecker::GlobalFloatType},
        {compiler::Signatures::BUILTIN_DOUBLE_BOX_CLASS, &ETSChecker::GlobalDoubleType},
    };

    return nameToGlobalBoxType;
}

ETSObjectType *ETSChecker::UpdateBoxedGlobalType(ETSObjectType *objType, util::StringView name)
{
    auto nameToGlobalBoxType = GetNameToGlobalBoxTypeMap();
    auto nameToTypeId = GetNameToTypeIdMap();

    if (nameToGlobalBoxType.find(name) != nameToGlobalBoxType.end()) {
        std::function<Type *(const ETSChecker *)> globalType = nameToGlobalBoxType[name];
        if (GlobalBuiltinBoxType(globalType(this)) != nullptr) {
            return GlobalBuiltinBoxType(globalType(this));
        }

        auto id = nameToTypeId.find(name);
        if (id != nameToTypeId.end()) {
            GetGlobalTypesHolder()->GlobalTypes()[static_cast<size_t>(id->second)] = objType;
        }
    }

    return objType;
}

ETSObjectType *ETSChecker::UpdateGlobalType(ETSObjectType *objType, util::StringView name)
{
    auto nameToGlobalType = GetNameToGlobalTypeMap();
    auto nameToTypeId = GetNameToTypeIdMap();

    if (nameToGlobalType.find(name) != nameToGlobalType.end()) {
        std::function<ETSObjectType *(const ETSChecker *)> globalType = nameToGlobalType[name];
        if (globalType(this) != nullptr) {
            return globalType(this);
        }

        auto id = nameToTypeId.find(name);
        if (id != nameToTypeId.end()) {
            GetGlobalTypesHolder()->GlobalTypes()[static_cast<size_t>(id->second)] = objType;
        }

        if (name == compiler::Signatures::BUILTIN_OBJECT_CLASS) {
            auto *nullish = CreateETSUnionType({objType, GlobalETSNullType(), GlobalETSUndefinedType()});
            GetGlobalTypesHolder()->GlobalTypes()[static_cast<size_t>(GlobalTypeId::ETS_NULLISH_OBJECT)] = nullish;
            nullish = CreateETSUnionType({GlobalETSNullType(), GlobalETSUndefinedType()});
            GetGlobalTypesHolder()->GlobalTypes()[static_cast<size_t>(GlobalTypeId::ETS_NULLISH_TYPE)] = nullish;
        }
    }

    return objType;
}

ETSObjectType *ETSChecker::CreateETSObjectTypeCheckBuiltins(util::StringView name, ir::AstNode *declNode,
                                                            ETSObjectFlags flags)
{
    if (name == compiler::Signatures::BUILTIN_BIGINT_CLASS) {
        if (GlobalBuiltinETSBigIntType() != nullptr) {
            return GlobalBuiltinETSBigIntType();
        }

        GetGlobalTypesHolder()->GlobalTypes()[static_cast<size_t>(GlobalTypeId::ETS_BIG_INT_BUILTIN)] =
            CreateNewETSObjectType(name, declNode, flags | ETSObjectFlags::BUILTIN_BIGINT);
        GetGlobalTypesHolder()->GlobalTypes()[static_cast<size_t>(GlobalTypeId::ETS_BIG_INT)] =
            Allocator()->New<ETSBigIntType>(Allocator(), GlobalBuiltinETSBigIntType());

        return GlobalBuiltinETSBigIntType();
    }

    if (name == compiler::Signatures::BUILTIN_STRING_CLASS) {
        if (GlobalBuiltinETSStringType() != nullptr) {
            return GlobalBuiltinETSStringType();
        }
        GetGlobalTypesHolder()->GlobalTypes()[static_cast<size_t>(GlobalTypeId::ETS_STRING_BUILTIN)] =
            CreateNewETSObjectType(name, declNode, flags | ETSObjectFlags::BUILTIN_STRING | ETSObjectFlags::STRING);

        GetGlobalTypesHolder()->GlobalTypes()[static_cast<size_t>(GlobalTypeId::ETS_STRING)] =
            Allocator()->New<ETSStringType>(Allocator(), GlobalBuiltinETSStringType(), Relation());
        return GlobalBuiltinETSStringType();
    }

    auto *objType = CreateNewETSObjectType(name, declNode, flags);
    auto nameToGlobalBoxType = GetNameToGlobalBoxTypeMap();

    return UpdateGlobalType(objType, name);
}

ETSObjectType *ETSChecker::CreateETSObjectType(util::StringView name, ir::AstNode *declNode, ETSObjectFlags flags)
{
    auto res = primitiveWrappers_.Wrappers().find(name);
    if (res == primitiveWrappers_.Wrappers().end()) {
        return CreateETSObjectTypeCheckBuiltins(name, declNode, flags);
    }

    if (res->second.first != nullptr) {
        return res->second.first;
    }

    auto *objType = CreateNewETSObjectType(name, declNode, flags | res->second.second);
    primitiveWrappers_.Wrappers().at(name).first = objType;
    return objType;
}

std::tuple<Language, bool> ETSChecker::CheckForDynamicLang(ir::AstNode *declNode, util::StringView assemblerName)
{
    Language lang(Language::Id::ETS);
    bool hasDecl = false;

    if (declNode->IsClassDefinition()) {
        auto *clsDef = declNode->AsClassDefinition();
        lang = clsDef->Language();
        hasDecl = clsDef->IsDeclare();
    }

    if (declNode->IsTSInterfaceDeclaration()) {
        auto *ifaceDecl = declNode->AsTSInterfaceDeclaration();
        lang = ifaceDecl->Language();
        hasDecl = ifaceDecl->IsDeclare();
    }

    auto res = compiler::Signatures::Dynamic::LanguageFromType(assemblerName.Utf8());
    if (res) {
        lang = *res;
    }

    return std::make_tuple(lang, hasDecl);
}

ETSObjectType *ETSChecker::CreateNewETSObjectType(util::StringView name, ir::AstNode *declNode, ETSObjectFlags flags)
{
    util::StringView assemblerName = name;
    util::StringView prefix {};

    auto *containingObjType = util::Helpers::GetContainingObjectType(declNode->Parent());

    if (declNode->IsClassDefinition()) {
        if (declNode->AsClassDefinition()->IsLocal()) {
            util::UString localName(declNode->AsClassDefinition()->LocalPrefix(), Allocator());
            localName.Append(name);
            assemblerName = localName.View();
        }
        if (declNode->AsClassDefinition()->OrigEnumDecl() != nullptr) {
            flags |= ETSObjectFlags::BOXED_ENUM;
        }
    }

    if (containingObjType != nullptr) {
        prefix = containingObjType->AssemblerName();
    } else if (const auto *topStatement = declNode->GetTopStatement();
               topStatement->Type() !=
               ir::AstNodeType::ETS_SCRIPT) {  // NOTE: should not occur, fix for TS_INTERFACE_DECLARATION
        ASSERT(declNode->IsTSInterfaceDeclaration());
        assemblerName = declNode->AsTSInterfaceDeclaration()->InternalName();
    } else {
        auto program = static_cast<ir::ETSScript *>(declNode->GetTopStatement())->Program();
        prefix = program->OmitModuleName() ? util::StringView() : program->ModuleName();
    }

    if (!prefix.Empty()) {
        assemblerName =
            util::UString(prefix.Mutf8() + compiler::Signatures::METHOD_SEPARATOR.data() + assemblerName.Mutf8(),
                          Allocator())
                .View();
    }

    auto [lang, hasDecl] = CheckForDynamicLang(declNode, assemblerName);
    if (lang.IsDynamic()) {
        return Allocator()->New<ETSDynamicType>(Allocator(), std::make_tuple(name, assemblerName, lang),
                                                std::make_tuple(declNode, flags, Relation()), hasDecl);
    }

    return Allocator()->New<ETSObjectType>(Allocator(), name, assemblerName,
                                           std::make_tuple(declNode, flags, Relation()));
}

std::tuple<util::StringView, SignatureInfo *> ETSChecker::CreateBuiltinArraySignatureInfo(ETSArrayType *arrayType,
                                                                                          size_t dim)
{
    std::stringstream ss;
    arrayType->ToAssemblerTypeWithRank(ss);
    ss << compiler::Signatures::METHOD_SEPARATOR << compiler::Signatures::CTOR << compiler::Signatures::MANGLE_BEGIN;
    arrayType->ToAssemblerTypeWithRank(ss);

    auto *info = CreateSignatureInfo();
    info->minArgCount = dim;

    for (size_t i = 0; i < dim; i++) {
        util::UString param(std::to_string(i), Allocator());
        auto *paramVar =
            varbinder::Scope::CreateVar(Allocator(), param.View(), varbinder::VariableFlags::NONE, nullptr);
        paramVar->SetTsType(GlobalIntType());

        info->params.push_back(paramVar);

        ss << compiler::Signatures::MANGLE_SEPARATOR << compiler::Signatures::PRIMITIVE_INT;
    }

    ss << compiler::Signatures::MANGLE_SEPARATOR << compiler::Signatures::PRIMITIVE_VOID
       << compiler::Signatures::MANGLE_SEPARATOR;
    auto internalName = util::UString(ss.str(), Allocator()).View();

    return {internalName, info};
}

Signature *ETSChecker::CreateBuiltinArraySignature(ETSArrayType *arrayType, size_t dim)
{
    auto res = globalArraySignatures_.find(arrayType);
    if (res != globalArraySignatures_.end()) {
        return res->second;
    }

    auto [internalName, info] = CreateBuiltinArraySignatureInfo(arrayType, dim);
    auto *signature = CreateSignature(info, GlobalVoidType(), internalName);
    globalArraySignatures_.insert({arrayType, signature});

    return signature;
}

ETSObjectType *ETSChecker::FunctionTypeToFunctionalInterfaceType(Signature *signature)
{
    auto *retType = signature->ReturnType();
    if (signature->RestVar() != nullptr) {
        auto *functionN =
            GlobalBuiltinFunctionType(GlobalBuiltinFunctionTypeVariadicThreshold(), signature->Function()->Flags())
                ->AsETSObjectType();
        auto *substitution = NewSubstitution();
        substitution->emplace(functionN->TypeArguments()[0]->AsETSTypeParameter(), MaybeBoxType(retType));
        return functionN->Substitute(Relation(), substitution);
    }

    // Note: FunctionN is not supported yet
    if (signature->Params().size() >= GetGlobalTypesHolder()->VariadicFunctionTypeThreshold()) {
        return nullptr;
    }

    auto *funcIface =
        GlobalBuiltinFunctionType(signature->Params().size(), signature->Function()->Flags())->AsETSObjectType();
    auto *substitution = NewSubstitution();

    for (size_t i = 0; i < signature->Params().size(); i++) {
        substitution->emplace(funcIface->TypeArguments()[i]->AsETSTypeParameter(),
                              MaybeBoxType(signature->Params()[i]->TsType()));
    }
    substitution->emplace(funcIface->TypeArguments()[signature->Params().size()]->AsETSTypeParameter(),
                          MaybeBoxType(signature->ReturnType()));
    return funcIface->Substitute(Relation(), substitution);
}

Type *ETSChecker::ResolveFunctionalInterfaces(ArenaVector<Signature *> &signatures)
{
    ArenaVector<Type *> types(Allocator()->Adapter());
    for (auto *signature : signatures) {
        types.push_back(FunctionTypeToFunctionalInterfaceType(signature));
    }
    return CreateETSUnionType(std::move(types));
}

ETSObjectType *ETSChecker::CreatePromiseOf(Type *type)
{
    ETSObjectType *const promiseType = GlobalBuiltinPromiseType();
    ASSERT(promiseType->TypeArguments().size() == 1);
    Substitution *substitution = NewSubstitution();
    ETSChecker::EmplaceSubstituted(substitution, promiseType->TypeArguments()[0]->AsETSTypeParameter()->GetOriginal(),
                                   type);

    return promiseType->Substitute(Relation(), substitution);
}

}  // namespace ark::es2panda::checker
