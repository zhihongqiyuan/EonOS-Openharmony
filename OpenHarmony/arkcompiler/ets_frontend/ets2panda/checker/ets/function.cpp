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

#include "varbinder/ETSBinder.h"
#include "checker/ETSchecker.h"
#include "checker/ets/function_helpers.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/types/ets/etsAsyncFuncReturnType.h"
#include "checker/types/ets/etsObjectType.h"
#include "ir/base/catchClause.h"
#include "ir/base/classDefinition.h"
#include "ir/base/classProperty.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/spreadElement.h"
#include "ir/ets/etsFunctionType.h"
#include "ir/ets/etsParameterExpression.h"
#include "ir/ets/etsTypeReference.h"
#include "ir/ets/etsTypeReferencePart.h"
#include "ir/ets/etsUnionType.h"
#include "ir/expressions/arrowFunctionExpression.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/functionExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/objectExpression.h"
#include "ir/statements/blockStatement.h"
#include "ir/statements/doWhileStatement.h"
#include "ir/statements/expressionStatement.h"
#include "ir/statements/forInStatement.h"
#include "ir/statements/forOfStatement.h"
#include "ir/statements/forUpdateStatement.h"
#include "ir/statements/returnStatement.h"
#include "ir/statements/switchStatement.h"
#include "ir/statements/whileStatement.h"
#include "ir/ts/tsTypeAliasDeclaration.h"
#include "ir/ts/tsTypeParameter.h"
#include "ir/ts/tsTypeParameterInstantiation.h"
#include "parser/program/program.h"
#include "util/helpers.h"

namespace ark::es2panda::checker {

// NOTE: #14993 merge with InstantiationContext::ValidateTypeArg
bool ETSChecker::IsCompatibleTypeArgument(ETSTypeParameter *typeParam, Type *typeArgument,
                                          const Substitution *substitution)
{
    if (typeArgument->IsWildcardType()) {
        return true;
    }
    if (typeArgument->IsTypeError()) {
        return true;
    }
    // NOTE(vpukhov): #19701 void refactoring
    ASSERT(IsReferenceType(typeArgument) || typeArgument->IsETSVoidType());
    auto *constraint = typeParam->GetConstraintType()->Substitute(Relation(), substitution);
    bool retVal = false;
    // NOTE(vpukhov): #19701 void refactoring
    if (typeArgument->IsETSVoidType()) {
        retVal = Relation()->IsSupertypeOf(constraint, GlobalETSUndefinedType());
    } else if (typeArgument->IsETSFunctionType()) {
        retVal = Relation()->IsSupertypeOf(
            constraint,
            this->FunctionTypeToFunctionalInterfaceType(typeArgument->AsETSFunctionType()->CallSignatures().front()));
    } else {
        retVal = Relation()->IsSupertypeOf(constraint, typeArgument);
    }

    return retVal;
}

bool ETSChecker::HasTypeArgsOfObject(Type *argType, Type *paramType)
{
    return paramType->IsETSObjectType() && argType->IsETSObjectType() &&
           !argType->AsETSObjectType()->TypeArguments().empty() &&
           !paramType->AsETSObjectType()->TypeArguments().empty();
}

bool ETSChecker::InsertTypeIntoSubstitution(const ArenaVector<Type *> &typeParams, const Type *typeParam,
                                            const size_t index, Substitution *substitution, Type *objectParam)
{
    // Check if the type parameter is in the signature, and the type argument is not already in the return vector
    if (typeParams.size() > index &&
        IsCompatibleTypeArgument(typeParams[index]->AsETSTypeParameter(), objectParam, substitution) &&
        std::find(typeParams.begin(), typeParams.end(), typeParam) != typeParams.end()) {
        substitution->emplace(typeParams[index]->AsETSTypeParameter(), objectParam);
        return true;
    }

    return false;
}

bool ETSChecker::EnhanceSubstitutionForGenericType(const ArenaVector<Type *> &typeParams, const Type *argType,
                                                   const Type *paramType, Substitution *substitution)
{
    ArenaVector<Type *> objectParams(Allocator()->Adapter());

    if (!argType->AsETSObjectType()->GetDeclNode()->IsClassDefinition()) {
        return false;
    }

    const auto paramTypeArgs = paramType->AsETSObjectType()->TypeArguments();

    for (const auto it : typeParams) {
        bool found = false;

        for (size_t i = 0; i < paramTypeArgs.size() && !found; i++) {
            if (paramTypeArgs[i] == it) {
                objectParams.push_back(argType->AsETSObjectType()->TypeArguments()[i]);
                found = true;
            }
        }

        if (!found) {
            objectParams.push_back(nullptr);
        }
    }

    if (objectParams.size() < paramTypeArgs.size()) {
        return false;
    }

    bool res = true;
    for (size_t j = 0; j < paramTypeArgs.size() && res; ++j) {
        if (objectParams[j] != nullptr) {
            res = InsertTypeIntoSubstitution(typeParams, paramTypeArgs[j], j, substitution, objectParams[j]);
        } else {
            res = EnhanceSubstitutionForType(typeParams, paramTypeArgs[j],
                                             argType->AsETSObjectType()->TypeArguments()[j], substitution);
        }
    }

    return res;
}

bool ETSChecker::EnhanceSubstitutionForReadonly(const ArenaVector<Type *> &typeParams, ETSReadonlyType *paramType,
                                                Type *argumentType, Substitution *substitution)
{
    return EnhanceSubstitutionForType(typeParams, paramType->GetUnderlying(), GetReadonlyType(argumentType),
                                      substitution);
}

/* A very rough and imprecise partial type inference */
bool ETSChecker::EnhanceSubstitutionForType(const ArenaVector<Type *> &typeParams, Type *paramType, Type *argumentType,
                                            Substitution *substitution)
{
    if (argumentType->IsETSPrimitiveType()) {
        argumentType = MaybeBoxInRelation(argumentType);
    }
    if (paramType->IsETSTypeParameter()) {
        auto *const tparam = paramType->AsETSTypeParameter();
        auto *const originalTparam = tparam->GetOriginal();
        if (std::find(typeParams.begin(), typeParams.end(), originalTparam) != typeParams.end() &&
            substitution->count(originalTparam) == 0) {
            if (!IsReferenceType(argumentType)) {
                LogTypeError({argumentType, " is not compatible with type ", tparam}, tparam->GetDeclNode()->Start());
                return false;
            }

            ETSChecker::EmplaceSubstituted(substitution, originalTparam, argumentType);
            return IsCompatibleTypeArgument(tparam, argumentType, substitution);
        }
    }

    if (paramType->IsETSReadonlyType()) {
        return EnhanceSubstitutionForReadonly(typeParams, paramType->AsETSReadonlyType(), argumentType, substitution);
    }
    if (paramType->IsETSUnionType()) {
        return EnhanceSubstitutionForUnion(typeParams, paramType->AsETSUnionType(), argumentType, substitution);
    }
    if (paramType->IsETSObjectType()) {
        if (HasTypeArgsOfObject(argumentType, paramType) &&
            EnhanceSubstitutionForGenericType(typeParams, argumentType, paramType, substitution)) {
            return true;
        }
        return EnhanceSubstitutionForObject(typeParams, paramType->AsETSObjectType(), argumentType, substitution);
    }
    if (paramType->IsETSArrayType()) {
        return EnhanceSubstitutionForArray(typeParams, paramType->AsETSArrayType(), argumentType, substitution);
    }

    return true;
}

bool ETSChecker::EnhanceSubstitutionForUnion(const ArenaVector<Type *> &typeParams, ETSUnionType *paramUn,
                                             Type *argumentType, Substitution *substitution)
{
    if (!argumentType->IsETSUnionType()) {
        return std::any_of(
            paramUn->ConstituentTypes().begin(), paramUn->ConstituentTypes().end(),
            [this, typeParams, argumentType, substitution](Type *ctype) {
                return EnhanceSubstitutionForType(typeParams, ctype, argumentType, substitution) &&
                       (!ctype->IsETSTypeParameter() ||
                        (substitution->find(ctype->AsETSTypeParameter()) != substitution->end() &&
                         Relation()->IsAssignableTo(argumentType, substitution->at(ctype->AsETSTypeParameter()))));
            });
    }
    auto *const argUn = argumentType->AsETSUnionType();

    ArenaVector<Type *> paramWlist(Allocator()->Adapter());
    ArenaVector<Type *> argWlist(Allocator()->Adapter());

    for (auto *pc : paramUn->ConstituentTypes()) {
        for (auto *ac : argUn->ConstituentTypes()) {
            if (ETSChecker::GetOriginalBaseType(pc) != ETSChecker::GetOriginalBaseType(ac)) {
                paramWlist.push_back(pc);
                argWlist.push_back(ac);
                continue;
            }
            if (!EnhanceSubstitutionForType(typeParams, pc, ac, substitution)) {
                return false;
            }
        }
    }
    auto *const newArg = CreateETSUnionType(std::move(argWlist));

    for (auto *pc : paramWlist) {
        if (!EnhanceSubstitutionForType(typeParams, pc, newArg, substitution)) {
            return false;
        }
    }
    return true;
}

bool ETSChecker::EnhanceSubstitutionForObject(const ArenaVector<Type *> &typeParams, ETSObjectType *paramType,
                                              Type *argumentType, Substitution *substitution)
{
    auto *paramObjType = paramType->AsETSObjectType();

    auto const enhance = [this, typeParams, substitution](Type *ptype, Type *atype) {
        return EnhanceSubstitutionForType(typeParams, ptype, atype, substitution);
    };

    if (argumentType->IsETSObjectType()) {
        auto *argObjType = argumentType->AsETSObjectType();
        if (GetOriginalBaseType(argObjType) != GetOriginalBaseType(paramObjType)) {
            return true;  // don't attempt anything fancy for now
        }
        bool res = true;
        for (size_t i = 0; i < argObjType->TypeArguments().size(); i++) {
            res &= enhance(paramObjType->TypeArguments()[i], argObjType->TypeArguments()[i]);
        }
        return res;
    }

    if (argumentType->IsETSFunctionType() && paramObjType->HasObjectFlag(ETSObjectFlags::FUNCTIONAL)) {
        auto &parameterSignatures =
            paramObjType
                ->GetOwnProperty<checker::PropertyType::INSTANCE_METHOD>(FUNCTIONAL_INTERFACE_INVOKE_METHOD_NAME)
                ->TsType()
                ->AsETSFunctionType()
                ->CallSignatures();
        auto &argumentSignatures = argumentType->AsETSFunctionType()->CallSignatures();
        ASSERT(argumentSignatures.size() == 1);
        ASSERT(parameterSignatures.size() == 1);
        auto *argumentSignature = argumentSignatures[0];
        auto *parameterSignature = parameterSignatures[0];
        // NOTE(gogabr): handle rest parameter for argumentSignature
        if (parameterSignature->GetSignatureInfo()->params.size() !=
            argumentSignature->GetSignatureInfo()->params.size()) {
            return false;
        }
        bool res = true;
        for (size_t idx = 0; idx < argumentSignature->GetSignatureInfo()->params.size(); idx++) {
            res &= enhance(parameterSignature->GetSignatureInfo()->params[idx]->TsType(),
                           argumentSignature->GetSignatureInfo()->params[idx]->TsType());
        }
        res &= enhance(parameterSignature->ReturnType(), argumentSignature->ReturnType());
        return res;
    }

    return true;
}

bool ETSChecker::EnhanceSubstitutionForArray(const ArenaVector<Type *> &typeParams, ETSArrayType *const paramType,
                                             Type *const argumentType, Substitution *const substitution)
{
    auto *const elementType =
        argumentType->IsETSArrayType() ? argumentType->AsETSArrayType()->ElementType() : argumentType;

    return EnhanceSubstitutionForType(typeParams, paramType->ElementType(), elementType, substitution);
}

Signature *ETSChecker::ValidateParameterlessConstructor(Signature *signature, const lexer::SourcePosition &pos,
                                                        TypeRelationFlag flags)
{
    std::size_t const parameterCount = signature->MinArgCount();
    auto const reportError = (flags & TypeRelationFlag::NO_THROW) == 0;

    if (parameterCount != 0) {
        if (reportError) {
            LogTypeError({"No Matching Parameterless Constructor, parameter count ", parameterCount}, pos);
        }
        return nullptr;
    }
    return signature;
}

bool ETSChecker::CheckOptionalLambdaFunction(ir::Expression *argument, Signature *substitutedSig, std::size_t index)
{
    if (argument->IsArrowFunctionExpression()) {
        auto *const arrowFuncExpr = argument->AsArrowFunctionExpression();

        if (ir::ScriptFunction *const lambda = arrowFuncExpr->Function();
            CheckLambdaAssignable(substitutedSig->Function()->Params()[index], lambda)) {
            if (arrowFuncExpr->TsType() != nullptr) {
                arrowFuncExpr->Check(this);
                return true;
            }
        }
    }

    return false;
}

bool ETSChecker::ValidateArgumentAsIdentifier(const ir::Identifier *identifier)
{
    auto result = Scope()->Find(identifier->Name());
    return result.variable != nullptr && (result.variable->HasFlag(varbinder::VariableFlags::CLASS_OR_INTERFACE));
}

bool ETSChecker::ValidateSignatureRequiredParams(Signature *substitutedSig,
                                                 const ArenaVector<ir::Expression *> &arguments, TypeRelationFlag flags,
                                                 const std::vector<bool> &argTypeInferenceRequired, bool reportError)
{
    std::size_t const argumentCount = arguments.size();
    std::size_t const parameterCount = substitutedSig->MinArgCount();
    auto count = std::min(parameterCount, argumentCount);
    for (std::size_t index = 0; index < count; ++index) {
        auto &argument = arguments[index];

        if (argument->IsObjectExpression()) {
            if (substitutedSig->Params()[index]->TsType()->IsETSObjectType()) {
                // No chance to check the argument at this point
                continue;
            }
            return false;
        }

        if (argument->IsMemberExpression()) {
            SetArrayPreferredTypeForNestedMemberExpressions(arguments[index]->AsMemberExpression(),
                                                            substitutedSig->Params()[index]->TsType());
        } else if (argument->IsSpreadElement()) {
            if (reportError) {
                LogTypeError("Spread argument cannot be passed for ordinary parameter.", argument->Start());
            }
            return false;
        }

        if (argTypeInferenceRequired[index]) {
            ASSERT(argument->IsArrowFunctionExpression());
            auto *const arrowFuncExpr = argument->AsArrowFunctionExpression();
            ir::ScriptFunction *const lambda = arrowFuncExpr->Function();
            if (CheckLambdaAssignable(substitutedSig->Function()->Params()[index], lambda)) {
                continue;
            }
            return false;
        }

        if (argument->IsArrayExpression()) {
            argument->AsArrayExpression()->GetPrefferedTypeFromFuncParam(
                this, substitutedSig->Function()->Params()[index], flags);
        }

        if (!CheckInvokable(substitutedSig, argument, index, flags)) {
            return false;
        }

        if (argument->IsIdentifier() && ValidateArgumentAsIdentifier(argument->AsIdentifier())) {
            LogTypeError("Class name can't be the argument of function or method.", argument->Start());
            return false;
        }

        // clang-format off
        if (!ValidateSignatureInvocationContext(
            substitutedSig, argument,
            TryGettingFunctionTypeFromInvokeFunction(substitutedSig->Params()[index]->TsType()), index, flags)) {
            // clang-format on
            return false;
        }
    }

    return true;
}

bool ETSChecker::CheckInvokable(Signature *substitutedSig, ir::Expression *argument, std::size_t index,
                                TypeRelationFlag flags)
{
    auto *argumentType = argument->Check(this);
    if (argumentType->IsTypeError()) {
        return true;
    }

    auto *targetType = substitutedSig->Params()[index]->TsType();
    flags |= TypeRelationFlag::ONLY_CHECK_WIDENING;

    auto const invocationCtx =
        checker::InvocationContext(Relation(), argument, argumentType, targetType, argument->Start(),
                                   {"Type '", argumentType, "' is not compatible with type '",
                                    TryGettingFunctionTypeFromInvokeFunction(targetType), "' at index ", index + 1},
                                   flags);
    if (Relation()->IsError()) {
        return false;
    }
    return invocationCtx.IsInvocable() || CheckOptionalLambdaFunction(argument, substitutedSig, index);
}

bool ETSChecker::ValidateSignatureInvocationContext(Signature *substitutedSig, ir::Expression *argument,
                                                    const Type *targetType, std::size_t index, TypeRelationFlag flags)
{
    Type *argumentType = argument->Check(this);
    flags |= TypeRelationFlag::ONLY_CHECK_WIDENING;

    auto const invocationCtx = checker::InvocationContext(
        Relation(), argument, argumentType, substitutedSig->Params()[index]->TsType(), argument->Start(),
        {"Type '", argumentType, "' is not compatible with type '", targetType, "' at index ", index + 1}, flags);
    if (!invocationCtx.IsInvocable()) {
        return CheckOptionalLambdaFunction(argument, substitutedSig, index);
    }

    return true;
}

bool ETSChecker::ValidateSignatureRestParams(Signature *substitutedSig, const ArenaVector<ir::Expression *> &arguments,
                                             TypeRelationFlag flags, bool reportError)
{
    std::size_t const argumentCount = arguments.size();
    std::size_t const parameterCount = substitutedSig->MinArgCount();
    auto count = std::min(parameterCount, argumentCount);
    auto const restCount = argumentCount - count;

    for (std::size_t index = count; index < argumentCount; ++index) {
        auto &argument = arguments[index];

        if (!argument->IsSpreadElement()) {
            auto *const argumentType = argument->Check(this);
            const Type *targetType = TryGettingFunctionTypeFromInvokeFunction(
                substitutedSig->RestVar()->TsType()->AsETSArrayType()->ElementType());
            const Type *sourceType = TryGettingFunctionTypeFromInvokeFunction(argumentType);
            auto const invocationCtx = checker::InvocationContext(
                Relation(), argument, argumentType,
                substitutedSig->RestVar()->TsType()->AsETSArrayType()->ElementType(), argument->Start(),
                {"Type '", sourceType, "' is not compatible with rest parameter type '", targetType, "' at index ",
                 index + 1},
                flags);
            if (!invocationCtx.IsInvocable()) {
                return false;
            }
            continue;
        }

        if (restCount > 1U) {
            if (reportError) {
                LogTypeError("Spread argument for the rest parameter can be only one.", argument->Start());
            }
            return false;
        }

        auto *const restArgument = argument->AsSpreadElement()->Argument();
        auto *const argumentType = restArgument->Check(this);
        const Type *targetType = TryGettingFunctionTypeFromInvokeFunction(substitutedSig->RestVar()->TsType());
        const Type *sourceType = TryGettingFunctionTypeFromInvokeFunction(argumentType);

        auto const invocationCtx = checker::InvocationContext(
            Relation(), restArgument, argumentType, substitutedSig->RestVar()->TsType(), argument->Start(),
            {"Type '", sourceType, "' is not compatible with rest parameter type '", targetType, "' at index ",
             index + 1},
            flags);
        if (!invocationCtx.IsInvocable()) {
            return false;
        }
    }

    return true;
}

void ETSChecker::MaybeSubstituteLambdaArgumentsInFunctionCall(ir::CallExpression *callExpr)
{
    ir::AstNode *expr = callExpr;

    while (!expr->IsFunctionExpression()) {
        if (expr->Parent() == nullptr || expr->Parent()->IsClassDefinition()) {
            return;
        }
        expr = expr->Parent();
    }

    for (const auto it : expr->AsFunctionExpression()->Function()->Params()) {
        if (const auto ident = it->AsETSParameterExpression()->Ident();
            callExpr->Callee()->IsIdentifier() && ident->Name() == callExpr->Callee()->AsIdentifier()->Name() &&
            ident->IsAnnotatedExpression()) {
            if (ident->AsAnnotatedExpression()->TypeAnnotation()->IsETSFunctionType()) {
                MaybeSubstituteLambdaArguments(
                    ident->AsAnnotatedExpression()->TypeAnnotation()->AsETSFunctionType()->Params(), callExpr);
            }
        }
    }
}

void ETSChecker::MaybeSubstituteLambdaArgumentsInFunctionCallHelper(ir::CallExpression *callExpr, ir::Identifier *ident)
{
    ir::ETSFunctionType *funcType = nullptr;
    ir::TypeNode *typeAnnotation = ident->TypeAnnotation();

    if (typeAnnotation->IsETSTypeReference()) {
        auto typeAnnotationIdentifier = ident->TypeAnnotation()->AsETSTypeReference()->Part()->Name()->Variable();
        typeAnnotation = typeAnnotationIdentifier->Declaration()->Node()->AsTSTypeAliasDeclaration()->TypeAnnotation();
    }

    if (typeAnnotation->IsETSFunctionType()) {
        funcType = typeAnnotation->AsETSFunctionType();
    } else if (typeAnnotation->IsETSUnionType()) {
        auto found = std::find_if(typeAnnotation->AsETSUnionType()->Types().begin(),
                                  typeAnnotation->AsETSUnionType()->Types().end(),
                                  [](ir::TypeNode *const type) { return type->IsETSFunctionType(); });
        if (found != typeAnnotation->AsETSUnionType()->Types().end()) {
            funcType = (*found)->AsETSFunctionType();
        }
    }

    if (funcType == nullptr) {
        return;
    }

    MaybeSubstituteLambdaArguments(funcType->AsETSFunctionType()->Params(), callExpr);
}

void ETSChecker::MaybeSubstituteLambdaArguments(const ArenaVector<ir::Expression *> &params,
                                                ir::CallExpression *callExpr)
{
    for (size_t i = 0; i < params.size(); i++) {
        if (params[i]->AsETSParameterExpression()->IsDefault() && callExpr->Arguments().size() <= i &&
            params[i]->AsETSParameterExpression()->Initializer() != nullptr) {
            callExpr->Arguments().push_back(
                params[i]->AsETSParameterExpression()->Initializer()->Clone(Allocator(), callExpr)->AsExpression());
        }
    }
}

Signature *ETSChecker::ValidateSignature(
    std::tuple<Signature *, const ir::TSTypeParameterInstantiation *, TypeRelationFlag> info,
    const ArenaVector<ir::Expression *> &arguments, const lexer::SourcePosition &pos,
    const std::vector<bool> &argTypeInferenceRequired)
{
    auto [signature, typeArguments, flags] = info;
    Signature *substitutedSig = MaybeSubstituteTypeParameters(this, signature, typeArguments, arguments, pos, flags);
    if (substitutedSig == nullptr) {
        return nullptr;
    }

    fflush(stdout);

    auto const hasRestParameter = substitutedSig->RestVar() != nullptr;
    std::size_t const argumentCount = arguments.size();
    std::size_t const parameterCount = substitutedSig->MinArgCount();
    auto const reportError = (flags & TypeRelationFlag::NO_THROW) == 0;

    if (argumentCount < parameterCount || (argumentCount > parameterCount && !hasRestParameter)) {
        if (reportError) {
            LogTypeError({"Expected ", parameterCount, " arguments, got ", argumentCount, "."}, pos);
        }
        return nullptr;
    }

    if (argumentCount > parameterCount && hasRestParameter && (flags & TypeRelationFlag::IGNORE_REST_PARAM) != 0) {
        return nullptr;
    }

    auto count = std::min(parameterCount, argumentCount);
    // Check all required formal parameter(s) first
    if (!ValidateSignatureRequiredParams(substitutedSig, arguments, flags, argTypeInferenceRequired, reportError)) {
        return nullptr;
    }

    // Check rest parameter(s) if any exists
    if (!hasRestParameter || count >= argumentCount) {
        return substitutedSig;
    }
    if (!ValidateSignatureRestParams(substitutedSig, arguments, flags, reportError)) {
        return nullptr;
    }

    return substitutedSig;
}

Signature *ETSChecker::CollectParameterlessConstructor(ArenaVector<Signature *> &signatures,
                                                       const lexer::SourcePosition &pos, TypeRelationFlag resolveFlags)
{
    Signature *compatibleSignature = nullptr;

    auto collectSignatures = [&](TypeRelationFlag relationFlags) {
        for (auto *sig : signatures) {
            if (auto *concreteSig = ValidateParameterlessConstructor(sig, pos, relationFlags); concreteSig != nullptr) {
                compatibleSignature = concreteSig;
                break;
            }
        }
    };

    // We are able to provide more specific error messages.
    if (signatures.size() == 1) {
        collectSignatures(resolveFlags);
    } else {
        collectSignatures(resolveFlags | TypeRelationFlag::NO_THROW);
    }

    if (compatibleSignature == nullptr) {
        if ((resolveFlags & TypeRelationFlag::NO_THROW) == 0) {
            LogTypeError({"No matching parameterless constructor"}, pos);
        }
    }
    return compatibleSignature;
}

bool IsSignatureAccessible(Signature *sig, ETSObjectType *containingClass, TypeRelation *relation)
{
    // NOTE(vivienvoros): this check can be removed if signature is implicitly declared as public according to the spec.
    if (!sig->HasSignatureFlag(SignatureFlags::PUBLIC | SignatureFlags::PROTECTED | SignatureFlags::PRIVATE |
                               SignatureFlags::INTERNAL)) {
        return true;
    }

    // NOTE(vivienvoros): take care of SignatureFlags::INTERNAL and SignatureFlags::INTERNAL_PROTECTED
    if (sig->HasSignatureFlag(SignatureFlags::INTERNAL) && !sig->HasSignatureFlag(SignatureFlags::PROTECTED)) {
        return true;
    }

    if (sig->HasSignatureFlag(SignatureFlags::PUBLIC) || sig->Owner() == containingClass ||
        (sig->HasSignatureFlag(SignatureFlags::PROTECTED) && relation->IsSupertypeOf(sig->Owner(), containingClass))) {
        return true;
    }

    return false;
}

// NOLINTNEXTLINE(readability-magic-numbers)
std::array<TypeRelationFlag, 9U> GetFlagVariants()
{
    // NOTE(boglarkahaag): Not in sync with specification, but solves the issues with rest params for now (#17483)
    return {
        TypeRelationFlag::NO_THROW | TypeRelationFlag::NO_UNBOXING | TypeRelationFlag::NO_BOXING |
            TypeRelationFlag::IGNORE_REST_PARAM,
        TypeRelationFlag::NO_THROW | TypeRelationFlag::NO_UNBOXING | TypeRelationFlag::NO_BOXING,
        TypeRelationFlag::NO_THROW | TypeRelationFlag::IGNORE_REST_PARAM,
        TypeRelationFlag::NO_THROW,
        TypeRelationFlag::NO_THROW | TypeRelationFlag::WIDENING | TypeRelationFlag::NO_UNBOXING |
            TypeRelationFlag::NO_BOXING | TypeRelationFlag::IGNORE_REST_PARAM,
        TypeRelationFlag::NO_THROW | TypeRelationFlag::WIDENING | TypeRelationFlag::NO_UNBOXING |
            TypeRelationFlag::NO_BOXING,
        TypeRelationFlag::NO_THROW | TypeRelationFlag::WIDENING | TypeRelationFlag::IGNORE_REST_PARAM,
        TypeRelationFlag::NO_THROW | TypeRelationFlag::WIDENING,
        TypeRelationFlag::NO_THROW | TypeRelationFlag::STRING_TO_CHAR,
    };
}
ArenaVector<Signature *> ETSChecker::CollectSignatures(ArenaVector<Signature *> &signatures,
                                                       const ir::TSTypeParameterInstantiation *typeArguments,
                                                       const ArenaVector<ir::Expression *> &arguments,
                                                       const lexer::SourcePosition &pos, TypeRelationFlag resolveFlags)
{
    ArenaVector<Signature *> compatibleSignatures(Allocator()->Adapter());
    std::vector<bool> argTypeInferenceRequired = FindTypeInferenceArguments(arguments);
    Signature *notVisibleSignature = nullptr;

    auto collectSignatures = [&](TypeRelationFlag relationFlags) {
        for (auto *sig : signatures) {
            if (notVisibleSignature != nullptr &&
                !IsSignatureAccessible(sig, Context().ContainingClass(), Relation())) {
                continue;
            }
            auto *concreteSig = ValidateSignature(std::make_tuple(sig, typeArguments, relationFlags), arguments, pos,
                                                  argTypeInferenceRequired);
            if (concreteSig == nullptr) {
                continue;
            }
            if (notVisibleSignature == nullptr &&
                !IsSignatureAccessible(sig, Context().ContainingClass(), Relation())) {
                notVisibleSignature = concreteSig;
            } else {
                compatibleSignatures.push_back(concreteSig);
            }
        }
    };

    // If there's only one signature, we don't need special checks for boxing/unboxing/widening.
    // We are also able to provide more specific error messages.
    if (signatures.size() == 1) {
        TypeRelationFlag flags = TypeRelationFlag::WIDENING | TypeRelationFlag::STRING_TO_CHAR | resolveFlags;
        collectSignatures(flags);
    } else {
        for (auto flags : GetFlagVariants()) {
            flags = flags | resolveFlags;
            collectSignatures(flags);
            if (!compatibleSignatures.empty()) {
                break;
            }
        }
    }

    if (compatibleSignatures.empty() && notVisibleSignature != nullptr) {
        LogTypeError(
            {"Signature ", notVisibleSignature->Function()->Id()->Name(), notVisibleSignature, " is not visible here."},
            pos);
    }
    return compatibleSignatures;
}

Signature *ETSChecker::GetMostSpecificSignature(ArenaVector<Signature *> &compatibleSignatures,
                                                const ArenaVector<ir::Expression *> &arguments,
                                                const lexer::SourcePosition &pos, TypeRelationFlag resolveFlags)
{
    std::vector<bool> argTypeInferenceRequired = FindTypeInferenceArguments(arguments);
    Signature *mostSpecificSignature = ChooseMostSpecificSignature(compatibleSignatures, argTypeInferenceRequired, pos);

    if (mostSpecificSignature == nullptr) {
        LogTypeError({"Reference to ", compatibleSignatures.front()->Function()->Id()->Name(), " is ambiguous"}, pos);
        return nullptr;
    }

    if (!TypeInference(mostSpecificSignature, arguments, resolveFlags)) {
        return nullptr;
    }

    return mostSpecificSignature;
}

Signature *ETSChecker::ValidateSignatures(ArenaVector<Signature *> &signatures,
                                          const ir::TSTypeParameterInstantiation *typeArguments,
                                          const ArenaVector<ir::Expression *> &arguments,
                                          const lexer::SourcePosition &pos, std::string_view signatureKind,
                                          TypeRelationFlag resolveFlags)
{
    auto compatibleSignatures = CollectSignatures(signatures, typeArguments, arguments, pos, resolveFlags);
    if (!compatibleSignatures.empty()) {
        return GetMostSpecificSignature(compatibleSignatures, arguments, pos, resolveFlags);
    }

    if ((resolveFlags & TypeRelationFlag::NO_THROW) == 0 && !arguments.empty() && !signatures.empty()) {
        std::stringstream ss;

        if (signatures[0]->Function()->IsConstructor()) {
            ss << util::Helpers::GetClassDefiniton(signatures[0]->Function())->PrivateId().Mutf8();
        } else {
            ss << signatures[0]->Function()->Id()->Name().Mutf8();
        }

        ss << "(";

        for (uint32_t index = 0; index < arguments.size(); ++index) {
            if (arguments[index]->IsArrowFunctionExpression()) {
                // NOTE(peterseres): Refactor this case and add test case
                break;
            }

            arguments[index]->Check(this);
            arguments[index]->TsType()->ToString(ss);

            if (index == arguments.size() - 1) {
                ss << ")";
                LogTypeError({"No matching ", signatureKind, " signature for ", ss.str().c_str()}, pos);
                return nullptr;
            }

            ss << ", ";
        }
    }

    if ((resolveFlags & TypeRelationFlag::NO_THROW) == 0) {
        LogTypeError({"No matching ", signatureKind, " signature"}, pos);
    }

    return nullptr;
}

Signature *ETSChecker::FindMostSpecificSignature(const ArenaVector<Signature *> &signatures,
                                                 const ArenaMultiMap<size_t, Signature *> &bestSignaturesForParameter,
                                                 size_t paramCount)
{
    Signature *mostSpecificSignature = nullptr;

    for (auto *sig : signatures) {
        bool mostSpecific = true;

        for (size_t paramIdx = 0; paramIdx < paramCount; ++paramIdx) {
            const auto range = bestSignaturesForParameter.equal_range(paramIdx);
            // Check if signature is most specific for i. parameter type.
            mostSpecific = std::any_of(range.first, range.second, [&sig](auto entry) { return entry.second == sig; });
            if (!mostSpecific) {
                break;
            }
        }

        if (!mostSpecific) {
            continue;
        }
        if (mostSpecificSignature == nullptr) {
            mostSpecificSignature = sig;
            continue;
        }
        if (mostSpecificSignature->Owner() == sig->Owner()) {
            // NOTE: audovichenko. Remove this 'if' when #12443 gets resolved
            if (mostSpecificSignature->Function() == sig->Function()) {
                // The same signature
                continue;
            }
            return nullptr;
        }
    }
    return mostSpecificSignature;
}

static void InitMostSpecificType(const ArenaVector<Signature *> &signatures, [[maybe_unused]] Type *&mostSpecificType,
                                 [[maybe_unused]] Signature *&prevSig, const size_t idx)
{
    for (auto *sig : signatures) {
        if (Type *sigType = sig->Params().at(idx)->TsType();
            sigType->IsETSObjectType() && !sigType->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::INTERFACE)) {
            mostSpecificType = sigType;
            prevSig = sig;
            return;
        }
    }
}

void ETSChecker::SearchAmongMostSpecificTypes(
    Type *&mostSpecificType, Signature *&prevSig,
    std::tuple<const lexer::SourcePosition &, size_t, size_t, size_t, Signature *> info, bool lookForClassType)
{
    auto [pos, argumentsSize, paramCount, idx, sig] = info;
    if (lookForClassType && argumentsSize == ULONG_MAX) {
        [[maybe_unused]] const bool equalParamSize = sig->Params().size() == paramCount;
        ASSERT(equalParamSize);
    }
    Type *sigType = sig->Params().at(idx)->TsType();
    const bool isClassType =
        sigType->IsETSObjectType() && !sigType->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::INTERFACE);
    if (isClassType == lookForClassType) {
        if (Relation()->IsIdenticalTo(sigType, mostSpecificType)) {
            return;
        }
        if (Relation()->IsAssignableTo(sigType, mostSpecificType)) {
            mostSpecificType = sigType;
            prevSig = sig;
        } else if (sigType->IsETSObjectType() && mostSpecificType->IsETSObjectType() &&
                   !Relation()->IsAssignableTo(mostSpecificType, sigType)) {
            auto funcName = sig->Function()->Id()->Name();
            LogTypeError({"Call to `", funcName, "` is ambiguous as `2` versions of `", funcName, "` are available: `",
                          funcName, prevSig, "` and `", funcName, sig, "`"},
                         pos);
        }
    }
}

ArenaMultiMap<size_t, Signature *> ETSChecker::GetSuitableSignaturesForParameter(
    const std::vector<bool> &argTypeInferenceRequired, size_t paramCount, ArenaVector<Signature *> &signatures,
    const lexer::SourcePosition &pos, size_t argumentsSize)
{
    // Collect which signatures are most specific for each parameter.
    ArenaMultiMap<size_t /* parameter index */, Signature *> bestSignaturesForParameter(Allocator()->Adapter());

    const checker::SavedTypeRelationFlagsContext savedTypeRelationFlagCtx(Relation(),
                                                                          TypeRelationFlag::ONLY_CHECK_WIDENING);

    for (size_t i = 0; i < paramCount; ++i) {
        if (argTypeInferenceRequired[i]) {
            for (auto *sig : signatures) {
                bestSignaturesForParameter.insert({i, sig});
            }
            continue;
        }
        // 1st step: check which is the most specific parameter type for i. parameter.
        Type *mostSpecificType = signatures.front()->Params().at(i)->TsType();
        Signature *prevSig = signatures.front();

        InitMostSpecificType(signatures, mostSpecificType, prevSig, i);
        for (auto *sig : signatures) {
            SearchAmongMostSpecificTypes(mostSpecificType, prevSig,
                                         std::make_tuple(pos, argumentsSize, paramCount, i, sig), true);
        }
        for (auto *sig : signatures) {
            SearchAmongMostSpecificTypes(mostSpecificType, prevSig,
                                         std::make_tuple(pos, argumentsSize, paramCount, i, sig), false);
        }

        for (auto *sig : signatures) {
            Type *sigType = sig->Params().at(i)->TsType();
            if (Relation()->IsIdenticalTo(sigType, mostSpecificType)) {
                bestSignaturesForParameter.insert({i, sig});
            }
        }
    }
    return bestSignaturesForParameter;
}

Signature *ETSChecker::ChooseMostSpecificSignature(ArenaVector<Signature *> &signatures,
                                                   const std::vector<bool> &argTypeInferenceRequired,
                                                   const lexer::SourcePosition &pos, size_t argumentsSize)
{
    ASSERT(signatures.empty() == false);

    if (signatures.size() == 1) {
        return signatures.front();
    }

    size_t paramCount = signatures.front()->Params().size();
    if (argumentsSize != ULONG_MAX) {
        paramCount = argumentsSize;
    }
    // Multiple signatures with zero parameter because of inheritance.
    // Return the closest one in inheritance chain that is defined at the beginning of the vector.
    if (paramCount == 0) {
        auto zeroParamSignature = std::find_if(signatures.begin(), signatures.end(),
                                               [](auto *signature) { return signature->RestVar() == nullptr; });
        // If there is a zero parameter signature, return that
        if (zeroParamSignature != signatures.end()) {
            return *zeroParamSignature;
        }
        // If there are multiple rest parameter signatures with different argument types, throw error
        if (signatures.size() > 1 && std::any_of(signatures.begin(), signatures.end(), [signatures](const auto *param) {
                return param->RestVar()->TsType() != signatures.front()->RestVar()->TsType();
            })) {
            LogTypeError({"Call to `", signatures.front()->Function()->Id()->Name(), "` is ambiguous "}, pos);
            return nullptr;
        }
        // Else return the signature with the rest parameter
        auto restParamSignature = std::find_if(signatures.begin(), signatures.end(),
                                               [](auto *signature) { return signature->RestVar() != nullptr; });
        return *restParamSignature;
    }

    ArenaMultiMap<size_t /* parameter index */, Signature *> bestSignaturesForParameter =
        GetSuitableSignaturesForParameter(argTypeInferenceRequired, paramCount, signatures, pos, argumentsSize);
    // Find the signature that are most specific for all parameters.
    Signature *mostSpecificSignature = FindMostSpecificSignature(signatures, bestSignaturesForParameter, paramCount);

    return mostSpecificSignature;
}

Signature *ETSChecker::ResolveCallExpressionAndTrailingLambda(ArenaVector<Signature *> &signatures,
                                                              ir::CallExpression *callExpr,
                                                              const lexer::SourcePosition &pos,
                                                              const TypeRelationFlag reportFlag)
{
    Signature *sig = nullptr;
    if (callExpr->TrailingBlock() == nullptr) {
        for (auto it : signatures) {
            MaybeSubstituteLambdaArguments(it->Function()->Params(), callExpr);

            if (callExpr->Arguments().size() != it->Function()->Params().size()) {
                MaybeSubstituteLambdaArgumentsInFunctionCall(callExpr);
            }
        }

        sig = ValidateSignatures(signatures, callExpr->TypeParams(), callExpr->Arguments(), pos, "call", reportFlag);
        return sig;
    }

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto arguments = ExtendArgumentsWithFakeLamda(callExpr);
    sig = ValidateSignatures(signatures, callExpr->TypeParams(), arguments, pos, "call",
                             TypeRelationFlag::NO_THROW | TypeRelationFlag::NO_CHECK_TRAILING_LAMBDA);
    if (sig != nullptr) {
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        TransformTraillingLambda(callExpr);
        TypeInference(sig, callExpr->Arguments());
        return sig;
    }

    sig = ValidateSignatures(signatures, callExpr->TypeParams(), callExpr->Arguments(), pos, "call", reportFlag);
    if (sig != nullptr) {
        EnsureValidCurlyBrace(callExpr);
    }

    return sig;
}

Signature *ETSChecker::ResolveConstructExpression(ETSObjectType *type, const ArenaVector<ir::Expression *> &arguments,
                                                  const lexer::SourcePosition &pos)
{
    return ValidateSignatures(type->ConstructSignatures(), nullptr, arguments, pos, "construct");
}

/*
 * Object literals do not get checked in the process of call resolution; we need to check them separately
 * afterwards.
 */
void ETSChecker::CheckObjectLiteralArguments(Signature *signature, ArenaVector<ir::Expression *> const &arguments)
{
    for (uint32_t index = 0; index < arguments.size(); index++) {
        if (!arguments[index]->IsObjectExpression()) {
            continue;
        }

        Type *tp;
        if (index >= signature->MinArgCount()) {
            ASSERT(signature->RestVar());
            tp = signature->RestVar()->TsType();
        } else {
            tp = signature->Params()[index]->TsType();
        }

        arguments[index]->AsObjectExpression()->SetPreferredType(tp);
        arguments[index]->Check(this);
    }
}

checker::ETSFunctionType *ETSChecker::BuildMethodSignature(ir::MethodDefinition *method)
{
    if (method->TsType() != nullptr) {
        return method->TsType()->AsETSFunctionType();
    }

    bool isConstructSig = method->IsConstructor();

    method->Function()->Id()->SetVariable(method->Id()->Variable());
    BuildFunctionSignature(method->Function(), isConstructSig);
    if (method->Function()->Signature() == nullptr) {
        return nullptr;
    }
    auto *funcType = BuildNamedFunctionType(method->Function());
    std::vector<checker::ETSFunctionType *> overloads;

    for (ir::MethodDefinition *const currentFunc : method->Overloads()) {
        currentFunc->Function()->Id()->SetVariable(currentFunc->Id()->Variable());
        BuildFunctionSignature(currentFunc->Function(), isConstructSig);
        if (currentFunc->Function()->Signature() == nullptr) {
            return nullptr;
        }
        auto *const overloadType = BuildNamedFunctionType(currentFunc->Function());
        CheckIdenticalOverloads(funcType, overloadType, currentFunc);
        currentFunc->SetTsType(overloadType);
        funcType->AddCallSignature(currentFunc->Function()->Signature());
        overloads.push_back(overloadType);
    }
    for (size_t baseFuncCounter = 0; baseFuncCounter < overloads.size(); ++baseFuncCounter) {
        auto *overloadType = overloads.at(baseFuncCounter);
        for (size_t compareFuncCounter = baseFuncCounter + 1; compareFuncCounter < overloads.size();
             compareFuncCounter++) {
            auto *compareOverloadType = overloads.at(compareFuncCounter);
            CheckIdenticalOverloads(overloadType, compareOverloadType, method->Overloads()[compareFuncCounter]);
        }
    }

    method->Id()->Variable()->SetTsType(funcType);
    return funcType;
}

void ETSChecker::CheckIdenticalOverloads(ETSFunctionType *func, ETSFunctionType *overload,
                                         const ir::MethodDefinition *const currentFunc)
{
    SavedTypeRelationFlagsContext savedFlagsCtx(Relation(), TypeRelationFlag::NO_RETURN_TYPE_CHECK);

    Relation()->IsIdenticalTo(func, overload);
    if (Relation()->IsTrue() && func->CallSignatures()[0]->GetSignatureInfo()->restVar ==
                                    overload->CallSignatures()[0]->GetSignatureInfo()->restVar) {
        LogTypeError("Function " + func->Name().Mutf8() + " is already declared.", currentFunc->Start());
        return;
    }
    if (HasSameAssemblySignatures(func, overload)) {
        LogTypeError("Function " + func->Name().Mutf8() + " with this assembly signature already declared.",
                     currentFunc->Start());
    }
}

Signature *ETSChecker::ComposeSignature(ir::ScriptFunction *func, SignatureInfo *signatureInfo, Type *returnType,
                                        varbinder::Variable *nameVar)
{
    auto *signature = CreateSignature(signatureInfo, returnType, func);
    signature->SetOwner(Context().ContainingClass());
    signature->SetOwnerVar(nameVar);

    const auto *returnTypeAnnotation = func->ReturnTypeAnnotation();
    if (returnTypeAnnotation == nullptr && ((func->Flags() & ir::ScriptFunctionFlags::HAS_RETURN) != 0)) {
        signature->AddSignatureFlag(SignatureFlags::NEED_RETURN_TYPE);
    }

    if (returnTypeAnnotation != nullptr && returnTypeAnnotation->IsTSThisType()) {
        signature->AddSignatureFlag(SignatureFlags::THIS_RETURN_TYPE);
    }

    if (func->IsAbstract()) {
        signature->AddSignatureFlag(SignatureFlags::ABSTRACT);
        signature->AddSignatureFlag(SignatureFlags::VIRTUAL);
    }

    if (func->IsStatic()) {
        signature->AddSignatureFlag(SignatureFlags::STATIC);
    }

    if (func->IsConstructor()) {
        signature->AddSignatureFlag(SignatureFlags::CONSTRUCTOR);
    }

    if ((signature->Owner() != nullptr && signature->Owner()->GetDeclNode()->IsFinal()) || func->IsFinal()) {
        signature->AddSignatureFlag(SignatureFlags::FINAL);
    }

    if (func->IsPublic()) {
        signature->AddSignatureFlag(SignatureFlags::PUBLIC);
    } else if (func->IsInternal()) {
        if (func->IsProtected()) {
            signature->AddSignatureFlag(SignatureFlags::INTERNAL_PROTECTED);
        } else {
            signature->AddSignatureFlag(SignatureFlags::INTERNAL);
        }
    } else if (func->IsProtected()) {
        signature->AddSignatureFlag(SignatureFlags::PROTECTED);
    } else if (func->IsPrivate()) {
        signature->AddSignatureFlag(SignatureFlags::PRIVATE);
    }

    return signature;
}

Type *ETSChecker::ComposeReturnType(ir::ScriptFunction *func)
{
    if (auto typeAnnotation = func->ReturnTypeAnnotation(); typeAnnotation != nullptr) {
        return typeAnnotation->GetType(this);
    }
    if (func->IsAsyncFunc()) {
        return CreatePromiseOf(GlobalVoidType());
    }
    return GlobalVoidType();
}

SignatureInfo *ETSChecker::ComposeSignatureInfo(ir::ScriptFunction *func)
{
    auto *signatureInfo = CreateSignatureInfo();
    signatureInfo->restVar = nullptr;
    signatureInfo->minArgCount = 0;

    if ((func->IsConstructor() || !func->IsStatic()) && !func->IsArrow()) {
        auto *thisVar = func->Scope()->ParamScope()->Params().front();
        thisVar->SetTsType(Context().ContainingClass());
    }

    if (func->TypeParams() != nullptr) {
        auto [typeParamTypes, ok] = CreateUnconstrainedTypeParameters(func->TypeParams());
        signatureInfo->typeParams = std::move(typeParamTypes);
        if (ok) {
            AssignTypeParameterConstraints(func->TypeParams());
        }
    }

    for (auto *const it : func->Params()) {
        auto *const param = it->AsETSParameterExpression();

        if (param->IsRestParameter()) {
            auto const *const restIdent = param->Ident();

            ASSERT(restIdent->Variable());
            signatureInfo->restVar = restIdent->Variable()->AsLocalVariable();

            auto *const restParamTypeAnnotation = param->TypeAnnotation();
            ASSERT(restParamTypeAnnotation);

            signatureInfo->restVar->SetTsType(restParamTypeAnnotation->GetType(this));
            auto arrayType = signatureInfo->restVar->TsType()->AsETSArrayType();
            CreateBuiltinArraySignature(arrayType, arrayType->Rank());
        } else {
            auto *const paramIdent = param->Ident();

            varbinder::Variable *const paramVar = paramIdent->Variable();
            ASSERT(paramVar);

            auto *const paramTypeAnnotation = param->TypeAnnotation();
            if (paramIdent->TsType() == nullptr && paramTypeAnnotation == nullptr) {
                LogTypeError({"The type of parameter '", paramIdent->Name(), "' cannot be determined"}, param->Start());
                return nullptr;
            }

            if (paramIdent->TsType() == nullptr) {
                ASSERT(paramTypeAnnotation);

                paramVar->SetTsType(paramTypeAnnotation->GetType(this));
            } else {
                paramVar->SetTsType(paramIdent->TsType());
            }
            signatureInfo->params.push_back(paramVar->AsLocalVariable());
            ++signatureInfo->minArgCount;
        }
    }

    return signatureInfo;
}

ArenaVector<SignatureInfo *> ETSChecker::ComposeSignatureInfosForArrowFunction(
    ir::ArrowFunctionExpression *arrowFuncExpr)
{
    ArenaVector<SignatureInfo *> signatureInfos(Allocator()->Adapter());

    for (size_t i = arrowFuncExpr->Function()->DefaultParamIndex(); i < arrowFuncExpr->Function()->Params().size();
         i++) {
        auto *signatureInfo = CreateSignatureInfo();
        signatureInfo->restVar = nullptr;
        signatureInfo->minArgCount = 0;

        if (arrowFuncExpr->Function()->TypeParams() != nullptr) {
            signatureInfo->typeParams =
                CreateUnconstrainedTypeParameters(arrowFuncExpr->Function()->TypeParams()).first;
        }

        for (size_t j = 0; j < i; j++) {
            SetParamForSignatureInfoOfArrowFunction(signatureInfo,
                                                    arrowFuncExpr->Function()->Params()[j]->AsETSParameterExpression());
        }

        signatureInfos.push_back(signatureInfo);
    }

    return signatureInfos;
}

void ETSChecker::SetParamForSignatureInfoOfArrowFunction(SignatureInfo *signatureInfo,
                                                         ir::ETSParameterExpression *param)
{
    if (param->IsRestParameter()) {
        auto const *const restIdent = param->Ident();

        ASSERT(restIdent->Variable());
        signatureInfo->restVar = restIdent->Variable()->AsLocalVariable();

        auto *const restParamTypeAnnotation = param->TypeAnnotation();
        ASSERT(restParamTypeAnnotation);

        signatureInfo->restVar->SetTsType(restParamTypeAnnotation->GetType(this));
        auto arrayType = signatureInfo->restVar->TsType()->AsETSArrayType();
        CreateBuiltinArraySignature(arrayType, arrayType->Rank());
    } else {
        auto *const paramIdent = param->Ident();

        varbinder::Variable *const paramVar = paramIdent->Variable();
        ASSERT(paramVar);

        auto *const paramTypeAnnotation = param->TypeAnnotation();
        if (paramIdent->TsType() == nullptr) {
            ASSERT(paramTypeAnnotation);

            paramVar->SetTsType(paramTypeAnnotation->GetType(this));
        } else {
            paramVar->SetTsType(paramIdent->TsType());
        }
        signatureInfo->params.push_back(paramVar->AsLocalVariable());
        ++signatureInfo->minArgCount;
    }
}

void ETSChecker::ValidateMainSignature(ir::ScriptFunction *func)
{
    if (func->Params().size() >= 2U) {
        LogTypeError("0 or 1 argument are allowed", func->Start());
        return;
    }

    if (func->Params().size() == 1) {
        auto const *const param = func->Params()[0]->AsETSParameterExpression();

        if (param->IsRestParameter()) {
            LogTypeError("Rest parameter is not allowed in the 'main' function.", param->Start());
        }

        const auto paramType = param->Variable()->TsType();
        if (!paramType->IsETSArrayType() || !paramType->AsETSArrayType()->ElementType()->IsETSStringType()) {
            LogTypeError("Only 'string[]' type argument is allowed.", param->Start());
        }
    }
}

static void AddSignatureFlags(const ir::ScriptFunction *const func, Signature *const signature)
{
    if (func->IsAbstract()) {
        signature->AddSignatureFlag(SignatureFlags::ABSTRACT);
        signature->AddSignatureFlag(SignatureFlags::VIRTUAL);
    }

    if (func->IsStatic()) {
        signature->AddSignatureFlag(SignatureFlags::STATIC);
    }

    if (func->IsConstructor()) {
        signature->AddSignatureFlag(SignatureFlags::CONSTRUCTOR);
    }

    if ((func->Signature()->Owner() != nullptr && func->Signature()->Owner()->GetDeclNode()->IsFinal()) ||
        func->IsFinal()) {
        signature->AddSignatureFlag(SignatureFlags::FINAL);
    }

    if (func->IsPublic()) {
        signature->AddSignatureFlag(SignatureFlags::PUBLIC);
    } else if (func->IsInternal()) {
        if (func->IsProtected()) {
            signature->AddSignatureFlag(SignatureFlags::INTERNAL_PROTECTED);
        } else {
            signature->AddSignatureFlag(SignatureFlags::INTERNAL);
        }
    } else if (func->IsProtected()) {
        signature->AddSignatureFlag(SignatureFlags::PROTECTED);
    } else if (func->IsPrivate()) {
        signature->AddSignatureFlag(SignatureFlags::PRIVATE);
    }

    if (func->IsSetter()) {
        signature->AddSignatureFlag(SignatureFlags::SETTER);
    } else if (func->IsGetter()) {
        signature->AddSignatureFlag(SignatureFlags::GETTER);
    }
}

void ETSChecker::BuildFunctionSignature(ir::ScriptFunction *func, bool isConstructSig)
{
    bool isArrow = func->IsArrow();
    auto *nameVar = isArrow ? nullptr : func->Id()->Variable();
    auto funcName = nameVar == nullptr ? util::StringView() : nameVar->Name();

    auto *signatureInfo = ComposeSignatureInfo(func);
    if (signatureInfo == nullptr) {
        return;
    }

    if (funcName.Is(compiler::Signatures::MAIN) &&
        func->Scope()->Name().Utf8().find(compiler::Signatures::ETS_GLOBAL) != std::string::npos) {
        func->AddFlag(ir::ScriptFunctionFlags::ENTRY_POINT);
    }
    if (func->IsEntryPoint()) {
        ValidateMainSignature(func);
    }

    auto *returnType = ComposeReturnType(func);
    auto *signature = ComposeSignature(func, signatureInfo, returnType, nameVar);
    if (isConstructSig) {
        signature->AddSignatureFlag(SignatureFlags::CONSTRUCT);
    } else {
        signature->AddSignatureFlag(SignatureFlags::CALL);
    }
    func->SetSignature(signature);
    AddSignatureFlags(func, signature);
    VarBinder()->AsETSBinder()->BuildFunctionName(func);
}

checker::ETSFunctionType *ETSChecker::BuildNamedFunctionType(ir::ScriptFunction *func)
{
    ASSERT(!func->IsArrow());
    auto *nameVar = func->Id()->Variable();
    auto *funcType = CreateETSFunctionType(func, func->Signature(), nameVar->Name());
    funcType->SetVariable(nameVar);
    return funcType;
}

Signature *ETSChecker::CheckEveryAbstractSignatureIsOverridden(ETSFunctionType *target, ETSFunctionType *source)
{
    for (auto targetSig = target->CallSignatures().begin(); targetSig != target->CallSignatures().end();) {
        if (!(*targetSig)->HasSignatureFlag(SignatureFlags::ABSTRACT)) {
            continue;
        }

        bool isOverridden = false;
        for (auto sourceSig : source->CallSignatures()) {
            if ((*targetSig)->Function()->Id()->Name() == sourceSig->Function()->Id()->Name() &&
                Relation()->IsCompatibleTo(*targetSig, sourceSig)) {
                target->CallSignatures().erase(targetSig);
                isOverridden = true;
                break;
            }
            sourceSig++;
        }

        if (!isOverridden) {
            return *targetSig;
        }
    }

    return nullptr;
}

bool ETSChecker::IsOverridableIn(Signature *signature)
{
    if (signature->HasSignatureFlag(SignatureFlags::PRIVATE)) {
        return false;
    }

    // NOTE: #15095 workaround, separate internal visibility check
    if (signature->HasSignatureFlag(SignatureFlags::PUBLIC | SignatureFlags::INTERNAL)) {
        return true;
    }

    return signature->HasSignatureFlag(SignatureFlags::PROTECTED);
}

bool ETSChecker::IsMethodOverridesOther(Signature *base, Signature *derived)
{
    if (derived->Function()->IsConstructor()) {
        return false;
    }

    if (base == derived) {
        return true;
    }

    if (derived->HasSignatureFlag(SignatureFlags::STATIC) != base->HasSignatureFlag(SignatureFlags::STATIC)) {
        return false;
    }

    if (IsOverridableIn(base)) {
        SavedTypeRelationFlagsContext savedFlagsCtx(Relation(), TypeRelationFlag::NO_RETURN_TYPE_CHECK |
                                                                    TypeRelationFlag::OVERRIDING_CONTEXT);
        if (Relation()->IsCompatibleTo(base, derived)) {
            CheckThrowMarkers(base, derived);

            if (derived->HasSignatureFlag(SignatureFlags::STATIC)) {
                return false;
            }

            derived->Function()->SetOverride();
            return true;
        }
    }

    return false;
}

bool ETSChecker::CheckThrowMarkers(Signature *source, Signature *target)
{
    ir::ScriptFunctionFlags throwMarkers = ir::ScriptFunctionFlags::THROWS | ir::ScriptFunctionFlags::RETHROWS;
    if ((source->Function()->Flags() & throwMarkers) == (target->Function()->Flags() & throwMarkers)) {
        return true;
    }

    if ((source->Function()->IsRethrowing() && target->Function()->IsThrowing()) ||
        (!source->Function()->IsThrowing() &&
         (target->Function()->IsRethrowing() || target->Function()->IsThrowing()))) {
        LogTypeError(
            "A method that overrides or hides another method cannot change throw or rethrow clauses of the "
            "overridden "
            "or hidden method.",
            target->Function()->Body() == nullptr ? target->Function()->Start() : target->Function()->Body()->Start());
        return false;
    }
    return true;
}

OverrideErrorCode ETSChecker::CheckOverride(Signature *signature, Signature *other)
{
    if (other->HasSignatureFlag(SignatureFlags::STATIC)) {
        ASSERT(signature->HasSignatureFlag(SignatureFlags::STATIC));
        return OverrideErrorCode::NO_ERROR;
    }

    if (other->IsFinal()) {
        return OverrideErrorCode::OVERRIDDEN_FINAL;
    }

    if (!other->ReturnType()->IsETSTypeParameter()) {
        if (!IsReturnTypeSubstitutable(signature, other)) {
            return OverrideErrorCode::INCOMPATIBLE_RETURN;
        }
    } else {
        // We need to have this branch to allow generic overriding of the form:
        // foo<T>(x: T): T -> foo<someClass>(x: someClass): someClass
        if (!signature->ReturnType()->IsETSReferenceType()) {
            return OverrideErrorCode::INCOMPATIBLE_RETURN;
        }
    }

    if (signature->ProtectionFlag() > other->ProtectionFlag()) {
        return OverrideErrorCode::OVERRIDDEN_WEAKER;
    }

    return OverrideErrorCode::NO_ERROR;
}

Signature *ETSChecker::AdjustForTypeParameters(Signature *source, Signature *target)
{
    auto &sourceTypeParams = source->GetSignatureInfo()->typeParams;
    auto &targetTypeParams = target->GetSignatureInfo()->typeParams;
    if (sourceTypeParams.size() != targetTypeParams.size()) {
        return nullptr;
    }
    if (sourceTypeParams.empty()) {
        return target;
    }
    auto *substitution = NewSubstitution();
    for (size_t ix = 0; ix < sourceTypeParams.size(); ix++) {
        if (!targetTypeParams[ix]->IsETSTypeParameter()) {
            continue;
        }
        ETSChecker::EmplaceSubstituted(substitution, targetTypeParams[ix]->AsETSTypeParameter(), sourceTypeParams[ix]);
    }
    return target->Substitute(Relation(), substitution);
}

void ETSChecker::ReportOverrideError(Signature *signature, Signature *overriddenSignature,
                                     const OverrideErrorCode &errorCode)
{
    const char *reason {};
    switch (errorCode) {
        case OverrideErrorCode::OVERRIDDEN_FINAL: {
            reason = "overridden method is final.";
            break;
        }
        case OverrideErrorCode::INCOMPATIBLE_RETURN: {
            reason = "overriding return type is not compatible with the other return type.";
            break;
        }
        case OverrideErrorCode::OVERRIDDEN_WEAKER: {
            reason = "overridden method has weaker access privilege.";
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    LogTypeError({signature->Function()->Id()->Name(), signature, " in ", signature->Owner(), " cannot override ",
                  overriddenSignature->Function()->Id()->Name(), overriddenSignature, " in ",
                  overriddenSignature->Owner(), " because ", reason},
                 signature->Function()->Start());
}

bool ETSChecker::CheckOverride(Signature *signature, ETSObjectType *site)
{
    auto *target = site->GetProperty(signature->Function()->Id()->Name(), PropertySearchFlags::SEARCH_METHOD);
    bool isOverridingAnySignature = false;

    if (target == nullptr) {
        return isOverridingAnySignature;
    }

    for (auto *it : target->TsType()->AsETSFunctionType()->CallSignatures()) {
        auto *itSubst = AdjustForTypeParameters(signature, it);

        if (itSubst == nullptr) {
            continue;
        }

        if (itSubst->HasSignatureFlag(SignatureFlags::ABSTRACT) || site->HasObjectFlag(ETSObjectFlags::INTERFACE)) {
            if (site->HasObjectFlag(ETSObjectFlags::INTERFACE) && !CheckThrowMarkers(itSubst, signature)) {
                return false;
            }

            if ((itSubst->Function()->IsSetter() && !signature->Function()->IsSetter()) ||
                (itSubst->Function()->IsGetter() && !signature->Function()->IsGetter())) {
                continue;
            }
        }

        if (!IsMethodOverridesOther(itSubst, signature)) {
            continue;
        }

        if (auto err = CheckOverride(signature, itSubst); err != OverrideErrorCode::NO_ERROR) {
            ReportOverrideError(signature, it, err);
            return false;
        }

        if (signature->Owner()->HasObjectFlag(ETSObjectFlags::INTERFACE) &&
            Relation()->IsIdenticalTo(itSubst->Owner(), GlobalETSObjectType()) &&
            !itSubst->HasSignatureFlag(SignatureFlags::PRIVATE)) {
            LogTypeError("Cannot override non-private method of the class Object from an interface.",
                         signature->Function()->Start());
            return false;
        }

        isOverridingAnySignature = true;
        it->AddSignatureFlag(SignatureFlags::VIRTUAL);
    }

    return isOverridingAnySignature;
}

void ETSChecker::CheckOverride(Signature *signature)
{
    auto *owner = signature->Owner();
    bool isOverriding = false;

    if (!owner->HasObjectFlag(ETSObjectFlags::CLASS | ETSObjectFlags::INTERFACE)) {
        return;
    }

    for (auto *const interface : owner->Interfaces()) {
        isOverriding |= CheckInterfaceOverride(this, interface, signature);
    }

    ETSObjectType *iter = owner->SuperType();
    while (iter != nullptr) {
        isOverriding |= CheckOverride(signature, iter);

        for (auto *const interface : iter->Interfaces()) {
            isOverriding |= CheckInterfaceOverride(this, interface, signature);
        }

        iter = iter->SuperType();
    }

    if (!isOverriding && signature->Function()->IsOverride()) {
        LogTypeError({"Method ", signature->Function()->Id()->Name(), signature, " in ", signature->Owner(),
                      " not overriding any method"},
                     signature->Function()->Start());
    }
}

Signature *ETSChecker::GetSignatureFromMethodDefinition(const ir::MethodDefinition *methodDef)
{
    if (methodDef->TsType()->IsTypeError()) {
        return nullptr;
    }
    ASSERT(methodDef->TsType() && methodDef->TsType()->IsETSFunctionType());
    for (auto *it : methodDef->TsType()->AsETSFunctionType()->CallSignatures()) {
        if (it->Function() == methodDef->Function()) {
            return it;
        }
    }

    return nullptr;
}

void ETSChecker::ValidateSignatureAccessibility(ETSObjectType *callee, const ir::CallExpression *callExpr,
                                                Signature *signature, const lexer::SourcePosition &pos,
                                                char const *errorMessage)
{
    if ((Context().Status() & CheckerStatus::IGNORE_VISIBILITY) != 0U ||
        (!signature->HasSignatureFlag(SignatureFlags::PRIVATE) &&
         !signature->HasSignatureFlag(SignatureFlags::PROTECTED))) {
        return;
    }
    const auto *declNode = callee->GetDeclNode();
    auto *containingClass = Context().ContainingClass();
    bool isContainingSignatureInherited = containingClass->IsSignatureInherited(signature);
    ASSERT(declNode && (declNode->IsClassDefinition() || declNode->IsTSInterfaceDeclaration()));

    if (declNode->IsTSInterfaceDeclaration()) {
        const auto *enclosingFunc =
            util::Helpers::FindAncestorGivenByType(callExpr, ir::AstNodeType::SCRIPT_FUNCTION)->AsScriptFunction();
        if (callExpr->Callee()->IsMemberExpression() &&
            callExpr->Callee()->AsMemberExpression()->Object()->IsThisExpression() &&
            signature->Function()->IsPrivate() && !enclosingFunc->IsPrivate()) {
            LogTypeError({"Cannot reference 'this' in this context."}, enclosingFunc->Start());
        }

        if (containingClass == declNode->AsTSInterfaceDeclaration()->TsType() && isContainingSignatureInherited) {
            return;
        }
    }
    if (containingClass == declNode->AsClassDefinition()->TsType() && isContainingSignatureInherited) {
        return;
    }

    bool isSignatureInherited = callee->IsSignatureInherited(signature);
    const auto *currentOutermost = containingClass->OutermostClass();
    if (!signature->HasSignatureFlag(SignatureFlags::PRIVATE) &&
        ((signature->HasSignatureFlag(SignatureFlags::PROTECTED) && containingClass->IsDescendantOf(callee)) ||
         (currentOutermost != nullptr && currentOutermost == callee->OutermostClass())) &&
        isSignatureInherited) {
        return;
    }

    if (errorMessage == nullptr) {
        LogTypeError({"Signature ", signature->Function()->Id()->Name(), signature, " is not visible here."}, pos);
        return;
    }
    LogTypeError(errorMessage, pos);
}

void ETSChecker::CheckCapturedVariable(ir::AstNode *const node, varbinder::Variable *const var)
{
    if (node->IsIdentifier()) {
        const auto *const parent = node->Parent();

        if (parent->IsUpdateExpression() ||
            (parent->IsAssignmentExpression() && parent->AsAssignmentExpression()->Left() == node)) {
            const auto *const identNode = node->AsIdentifier();

            const auto *resolved = identNode->Variable();

            if (resolved == nullptr) {
                resolved =
                    FindVariableInFunctionScope(identNode->Name(), varbinder::ResolveBindingOptions::ALL_NON_TYPE);
            }

            if (resolved == nullptr) {
                resolved = FindVariableInGlobal(identNode, varbinder::ResolveBindingOptions::ALL_NON_TYPE);
            }

            if (resolved == var) {
                // For mutable captured variable [possible] smart-cast is senseless (or even erroneous)
                Context().RemoveSmartCast(var);
            }
        }
    }

    CheckCapturedVariableInSubnodes(node, var);
}

void ETSChecker::CheckCapturedVariableInSubnodes(ir::AstNode *node, varbinder::Variable *var)
{
    if (!node->IsClassDefinition()) {
        node->Iterate([this, var](ir::AstNode *childNode) { CheckCapturedVariable(childNode, var); });
    }
}

void ETSChecker::CheckCapturedVariables()
{
    // If we want to capture non constant local variables, we should wrap them in a generic reference class
    for (auto [var, _] : Context().CapturedVars()) {
        (void)_;
        if ((var->Declaration() == nullptr) || var->Declaration()->IsConstDecl() ||
            !var->HasFlag(varbinder::VariableFlags::LOCAL) || var->GetScope()->Node()->IsArrowFunctionExpression()) {
            continue;
        }

        auto *searchNode = var->Declaration()->Node()->Parent();

        if (searchNode->IsVariableDeclarator()) {
            searchNode = searchNode->Parent()->Parent();
        }

        CheckCapturedVariableInSubnodes(searchNode, var);
    }
}

bool ETSChecker::AreOverrideEquivalent(Signature *const s1, Signature *const s2)
{
    // Two functions, methods or constructors M and N have the same signature if
    // their names and type parameters (if any) are the same, and their formal parameter
    // types are also the same (after the formal parameter types of N are adapted to the type parameters of M).
    // Signatures s1 and s2 are override-equivalent only if s1 and s2 are the same.
    if (s1->Function()->Id()->Name() != s2->Function()->Id()->Name()) {
        return false;
    }

    SavedTypeRelationFlagsContext savedFlagsCtx(Relation(), TypeRelationFlag::OVERRIDING_CONTEXT);
    return Relation()->IsCompatibleTo(s1, s2);
}

bool ETSChecker::IsReturnTypeSubstitutable(Signature *const s1, Signature *const s2)
{
    auto *const r1 = s1->ReturnType();
    auto *const r2 = s2->ReturnType();

    // A method declaration d1 with return type R1 is return-type-substitutable for another method d2 with return
    // type R2 if any of the following is true:

    // NOTE(vpukhov): void type leaks into type arguments, so we have to check the original signature if the return type
    // is parametrized or not to use a proper subtyping check. To be replaced with IsETSPrimitiveType after #19701.
    auto const hasPrimitiveReturnType = [](Signature *s) {
        bool origIsRef = s->Function()->Signature()->ReturnType()->IsETSReferenceType();
        ASSERT(origIsRef == s->ReturnType()->IsETSReferenceType());
        return !origIsRef;
    };
    // - If R1 is a primitive type then R2 is identical to R1.
    if (hasPrimitiveReturnType(s1) || hasPrimitiveReturnType(s2)) {
        return Relation()->IsIdenticalTo(r2, r1);
    }

    // - If R1 is a reference type then R1, adapted to the type parameters of d2 (link to generic methods), is a
    // subtype of R2.
    ASSERT(IsReferenceType(r1));

    // Starting from this line, everything should be be restored to IsSupertypeOf check, to be reverted in #18866
    if (Relation()->IsSupertypeOf(r2, r1)) {
        return true;
    }

    return s2->Function()->ReturnTypeAnnotation()->IsETSTypeReference() &&
           s2->Function()->ReturnTypeAnnotation()->GetType(this)->IsETSTypeParameter() &&
           Relation()->IsSupertypeOf(
               s2->Function()->ReturnTypeAnnotation()->GetType(this)->AsETSTypeParameter()->GetConstraintType(), r1);
}

std::string ETSChecker::GetAsyncImplName(const util::StringView &name)
{
    std::string implName(name);
    implName += "$asyncimpl";
    return implName;
}

std::string ETSChecker::GetAsyncImplName(ir::MethodDefinition *asyncMethod)
{
    ir::Identifier *asyncName = asyncMethod->Function()->Id();
    ASSERT(asyncName != nullptr);
    return GetAsyncImplName(asyncName->Name());
}

bool ETSChecker::IsAsyncImplMethod(ir::MethodDefinition const *method)
{
    auto methodName = method->Key()->AsIdentifier()->Name().Utf8();
    std::string_view asyncSuffix = "$asyncimpl";
    if (methodName.size() < asyncSuffix.size()) {
        return false;
    }
    return methodName.substr(methodName.size() - asyncSuffix.size()) == asyncSuffix;
}

ir::MethodDefinition *ETSChecker::CreateAsyncImplMethod(ir::MethodDefinition *asyncMethod,
                                                        ir::ClassDefinition *classDef)
{
    util::UString implName(GetAsyncImplName(asyncMethod), Allocator());
    ir::ModifierFlags modifiers = asyncMethod->Modifiers();
    // clear ASYNC flag for implementation
    modifiers &= ~ir::ModifierFlags::ASYNC;
    ir::ScriptFunction *asyncFunc = asyncMethod->Function();
    ir::ScriptFunctionFlags flags = ir::ScriptFunctionFlags::METHOD;

    if (asyncFunc->IsProxy()) {
        flags |= ir::ScriptFunctionFlags::PROXY;
    }

    if (asyncFunc->HasReturnStatement()) {
        flags |= ir::ScriptFunctionFlags::HAS_RETURN;
    }

    asyncMethod->AddModifier(ir::ModifierFlags::NATIVE);
    asyncFunc->AddModifier(ir::ModifierFlags::NATIVE);
    // Create async_impl method copied from CreateInvokeFunction
    auto scopeCtx =
        varbinder::LexicalScope<varbinder::ClassScope>::Enter(VarBinder(), classDef->Scope()->AsClassScope());
    auto *body = asyncFunc->Body();
    ArenaVector<ir::Expression *> params(Allocator()->Adapter());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    varbinder::FunctionParamScope *paramScope = CopyParams(asyncFunc->Params(), params);

    ir::ETSTypeReference *returnTypeAnn = nullptr;

    if (!asyncFunc->Signature()->HasSignatureFlag(SignatureFlags::NEED_RETURN_TYPE)) {
        // Set impl method return type "Object" because it may return Promise as well as Promise parameter's type
        auto *objectId = AllocNode<ir::Identifier>(compiler::Signatures::BUILTIN_OBJECT_CLASS, Allocator());
        VarBinder()->AsETSBinder()->LookupTypeReference(objectId, false);
        returnTypeAnn =
            AllocNode<ir::ETSTypeReference>(AllocNode<ir::ETSTypeReferencePart>(objectId, nullptr, nullptr));
        objectId->SetParent(returnTypeAnn->Part());
        returnTypeAnn->Part()->SetParent(returnTypeAnn);
        auto *asyncFuncRetTypeAnn = asyncFunc->ReturnTypeAnnotation();
        auto *promiseType = [this](ir::TypeNode *type) {
            if (type != nullptr) {
                return type->GetType(this)->AsETSObjectType();
            }

            return GlobalBuiltinPromiseType()->AsETSObjectType();
        }(asyncFuncRetTypeAnn);
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *retType = Allocator()->New<ETSAsyncFuncReturnType>(Allocator(), Relation(), promiseType);
        returnTypeAnn->SetTsType(retType);
    }  // NOTE(vpukhov): #19874 - returnTypeAnn is not set

    ir::MethodDefinition *implMethod =
        CreateMethod(implName.View(), modifiers, flags, std::move(params), paramScope, returnTypeAnn, body);
    asyncFunc->SetBody(nullptr);

    if (returnTypeAnn != nullptr) {
        returnTypeAnn->SetParent(implMethod->Function());
    }

    implMethod->Function()->AddFlag(ir::ScriptFunctionFlags::ASYNC_IMPL);
    implMethod->SetParent(asyncMethod->Parent());
    return implMethod;
}

static void CreateFuncDecl(ETSChecker *checker, ir::MethodDefinition *func, varbinder::LocalScope *scope)
{
    auto *allocator = checker->Allocator();
    auto *varBinder = checker->VarBinder();
    // Add the function declarations to the lambda class scope
    auto ctx = varbinder::LexicalScope<varbinder::LocalScope>::Enter(varBinder, scope);
    varbinder::Variable *var = scope->FindLocal(func->Id()->Name(), varbinder::ResolveBindingOptions::ALL_DECLARATION);
    if (var == nullptr) {
        var = std::get<1>(
            varBinder->NewVarDecl<varbinder::FunctionDecl>(func->Start(), allocator, func->Id()->Name(), func));
    }
    var->AddFlag(varbinder::VariableFlags::METHOD);
    var->SetScope(ctx.GetScope());
    func->Function()->Id()->SetVariable(var);
}

ir::MethodDefinition *ETSChecker::CreateAsyncProxy(ir::MethodDefinition *asyncMethod, ir::ClassDefinition *classDef,
                                                   bool createDecl)
{
    ir::ScriptFunction *asyncFunc = asyncMethod->Function();
    if (!asyncFunc->IsExternal()) {
        VarBinder()->AsETSBinder()->GetRecordTable()->Signatures().push_back(asyncFunc->Scope());
    }

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    ir::MethodDefinition *implMethod = CreateAsyncImplMethod(asyncMethod, classDef);
    varbinder::FunctionScope *implFuncScope = implMethod->Function()->Scope();
    for (auto *decl : asyncFunc->Scope()->Decls()) {
        auto res = asyncFunc->Scope()->Bindings().find(decl->Name());
        ASSERT(res != asyncFunc->Scope()->Bindings().end());
        auto *const var = std::get<1>(*res);
        var->SetScope(implFuncScope);
        implFuncScope->Decls().push_back(decl);
        implFuncScope->InsertBinding(decl->Name(), var);
    }

    ReplaceScope(implMethod->Function()->Body(), asyncFunc, implFuncScope);

    ArenaVector<varbinder::Variable *> captured(Allocator()->Adapter());

    bool isStatic = asyncMethod->IsStatic();
    if (createDecl) {
        if (isStatic) {
            CreateFuncDecl(this, implMethod, classDef->Scope()->AsClassScope()->StaticMethodScope());
        } else {
            CreateFuncDecl(this, implMethod, classDef->Scope()->AsClassScope()->InstanceMethodScope());
        }
        implMethod->Id()->SetVariable(implMethod->Function()->Id()->Variable());
    }
    VarBinder()->AsETSBinder()->BuildProxyMethod(implMethod->Function(), classDef->InternalName(),
                                                 asyncFunc->IsExternal());
    implMethod->SetParent(asyncMethod->Parent());

    return implMethod;
}

ir::MethodDefinition *ETSChecker::CreateMethod(const util::StringView &name, ir::ModifierFlags modifiers,
                                               ir::ScriptFunctionFlags flags, ArenaVector<ir::Expression *> &&params,
                                               varbinder::FunctionParamScope *paramScope, ir::TypeNode *returnType,
                                               ir::AstNode *body)
{
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *nameId = AllocNode<ir::Identifier>(name, Allocator());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *scope = VarBinder()->Allocator()->New<varbinder::FunctionScope>(Allocator(), paramScope);
    // clang-format off
    auto *const func = AllocNode<ir::ScriptFunction>(
        Allocator(), ir::ScriptFunction::ScriptFunctionData {
            // CC-OFFNXT(G.FMT.05-CPP) project codestyle clang format off
            body, ir::FunctionSignature(nullptr, std::move(params), returnType), flags, modifiers});
    // clang-format on
    func->SetScope(scope);
    func->SetIdent(nameId);
    if (body != nullptr && body->IsBlockStatement()) {
        body->AsBlockStatement()->SetScope(scope);
    }
    scope->BindNode(func);
    paramScope->BindNode(func);
    scope->BindParamScope(paramScope);
    paramScope->BindFunctionScope(scope);

    if (!func->IsStatic()) {
        auto classDef = VarBinder()->GetScope()->AsClassScope()->Node()->AsClassDefinition();
        VarBinder()->AsETSBinder()->AddFunctionThisParam(func);
        func->Scope()->Find(varbinder::VarBinder::MANDATORY_PARAM_THIS).variable->SetTsType(classDef->TsType());
    }

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *funcExpr = AllocNode<ir::FunctionExpression>(func);
    auto *nameClone = nameId->Clone(Allocator(), nullptr);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *method = AllocNode<ir::MethodDefinition>(ir::MethodDefinitionKind::METHOD, nameClone, funcExpr, modifiers,
                                                   Allocator(), false);
    return method;
}

varbinder::FunctionParamScope *ETSChecker::CopyParams(const ArenaVector<ir::Expression *> &params,
                                                      ArenaVector<ir::Expression *> &outParams)
{
    auto paramCtx = varbinder::LexicalScope<varbinder::FunctionParamScope>(VarBinder());

    for (auto *const it : params) {
        auto *const paramOld = it->AsETSParameterExpression();
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *const paramNew = paramOld->Clone(Allocator(), paramOld->Parent())->AsETSParameterExpression();

        auto *const var = std::get<1>(VarBinder()->AddParamDecl(paramNew));

        var->SetTsType(paramOld->Ident()->Variable()->TsType());
        var->SetScope(paramCtx.GetScope());
        paramNew->SetVariable(var);

        paramNew->SetTsType(paramOld->Ident()->Variable()->TsType());

        outParams.emplace_back(paramNew);
    }

    return paramCtx.GetScope();
}

void ETSChecker::ReplaceScope(ir::AstNode *root, ir::AstNode *oldNode, varbinder::Scope *newScope)
{
    if (root == nullptr) {
        return;
    }

    root->Iterate([this, oldNode, newScope](ir::AstNode *child) {
        auto *scope = NodeScope(child);
        if (scope != nullptr) {
            while (scope->Parent() != nullptr && scope->Parent()->Node() != oldNode) {
                scope = scope->Parent();
            }
            scope->SetParent(newScope);
        } else {
            ReplaceScope(child, oldNode, newScope);
        }
    });
}

void ETSChecker::MoveTrailingBlockToEnclosingBlockStatement(ir::CallExpression *callExpr)
{
    if (callExpr == nullptr) {
        return;
    }

    ir::AstNode *parent = callExpr->Parent();
    ir::AstNode *current = callExpr;
    while (parent != nullptr) {
        if (!parent->IsBlockStatement()) {
            current = parent;
            parent = parent->Parent();
        } else {
            // Collect trailing block, insert it only when block statements traversal ends to avoid order mismatch.
            parent->AsBlockStatement()->AddTrailingBlock(current, callExpr->TrailingBlock());
            callExpr->TrailingBlock()->SetParent(parent);
            callExpr->SetTrailingBlock(nullptr);
            break;
        }
    }
}

void ETSChecker::TransformTraillingLambda(ir::CallExpression *callExpr)
{
    auto *trailingBlock = callExpr->TrailingBlock();
    ASSERT(trailingBlock != nullptr);

    auto *funcParamScope = varbinder::LexicalScope<varbinder::FunctionParamScope>(VarBinder()).GetScope();
    auto paramCtx = varbinder::LexicalScope<varbinder::FunctionParamScope>::Enter(VarBinder(), funcParamScope, false);

    auto funcCtx = varbinder::LexicalScope<varbinder::FunctionScope>(VarBinder());
    auto *funcScope = funcCtx.GetScope();
    funcScope->BindParamScope(funcParamScope);
    funcParamScope->BindFunctionScope(funcScope);
    funcParamScope->SetParent(trailingBlock->Scope()->Parent());

    for (auto [_, var] : trailingBlock->Scope()->Bindings()) {
        (void)_;
        if (var->GetScope() == trailingBlock->Scope()) {
            var->SetScope(funcScope);
            funcScope->InsertBinding(var->Name(), var);
        }
    }

    ArenaVector<ir::Expression *> params(Allocator()->Adapter());
    auto *funcNode = AllocNode<ir::ScriptFunction>(
        Allocator(), ir::ScriptFunction::ScriptFunctionData {trailingBlock,
                                                             ir::FunctionSignature(nullptr, std::move(params), nullptr),
                                                             ir::ScriptFunctionFlags::ARROW});
    funcNode->SetScope(funcScope);
    funcScope->BindNode(funcNode);
    funcParamScope->BindNode(funcNode);

    trailingBlock->SetScope(funcScope);
    ReplaceScope(funcNode->Body(), trailingBlock, funcScope);
    callExpr->SetTrailingBlock(nullptr);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *arrowFuncNode = AllocNode<ir::ArrowFunctionExpression>(funcNode);
    arrowFuncNode->SetRange(trailingBlock->Range());
    arrowFuncNode->SetParent(callExpr);

    callExpr->Arguments().push_back(arrowFuncNode);
}

ArenaVector<ir::Expression *> ETSChecker::ExtendArgumentsWithFakeLamda(ir::CallExpression *callExpr)
{
    auto funcCtx = varbinder::LexicalScope<varbinder::FunctionScope>(VarBinder());
    auto *funcScope = funcCtx.GetScope();
    ArenaVector<ir::Expression *> params(Allocator()->Adapter());

    ArenaVector<ir::Statement *> statements(Allocator()->Adapter());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *body = AllocNode<ir::BlockStatement>(Allocator(), std::move(statements));
    body->SetScope(funcScope);

    auto *funcNode = AllocNode<ir::ScriptFunction>(
        Allocator(),
        ir::ScriptFunction::ScriptFunctionData {body, ir::FunctionSignature(nullptr, std::move(params), nullptr),
                                                ir::ScriptFunctionFlags::ARROW});
    funcNode->SetScope(funcScope);
    funcScope->BindNode(funcNode);
    auto *arrowFuncNode = AllocNode<ir::ArrowFunctionExpression>(funcNode);
    arrowFuncNode->SetParent(callExpr);

    ArenaVector<ir::Expression *> fakeArguments = callExpr->Arguments();
    fakeArguments.push_back(arrowFuncNode);
    return fakeArguments;
}

void ETSChecker::EnsureValidCurlyBrace(ir::CallExpression *callExpr)
{
    if (callExpr->TrailingBlock() == nullptr) {
        return;
    }

    if (callExpr->IsTrailingBlockInNewLine()) {
        MoveTrailingBlockToEnclosingBlockStatement(callExpr);
        return;
    }

    LogTypeError({"No matching call signature with trailing lambda"}, callExpr->Start());
}

ETSObjectType *ETSChecker::GetCachedFunctionalInterface(ir::ETSFunctionType *type)
{
    auto hash = GetHashFromFunctionType(type);
    auto it = functionalInterfaceCache_.find(hash);
    if (it == functionalInterfaceCache_.cend()) {
        return nullptr;
    }
    return it->second;
}

void ETSChecker::CacheFunctionalInterface(ir::ETSFunctionType *type, ETSObjectType *ifaceType)
{
    auto hash = GetHashFromFunctionType(type);
    ASSERT(functionalInterfaceCache_.find(hash) == functionalInterfaceCache_.cend());
    functionalInterfaceCache_.emplace(hash, ifaceType);
}

void ETSChecker::CollectReturnStatements(ir::AstNode *parent)
{
    parent->Iterate([this](ir::AstNode *childNode) -> void {
        if (childNode->IsScriptFunction()) {
            return;
        }

        if (childNode->IsReturnStatement()) {
            ir::ReturnStatement *returnStmt = childNode->AsReturnStatement();
            returnStmt->Check(this);
        }

        CollectReturnStatements(childNode);
    });
}

ArenaVector<ConstraintCheckRecord> &ETSChecker::PendingConstraintCheckRecords()
{
    return pendingConstraintCheckRecords_;
}

size_t &ETSChecker::ConstraintCheckScopesCount()
{
    return constraintCheckScopesCount_;
}

bool ETSChecker::CmpAssemblerTypesWithRank(Signature const *const sig1, Signature const *const sig2) noexcept
{
    for (size_t ix = 0; ix < sig1->MinArgCount(); ix++) {
        std::stringstream s1;
        std::stringstream s2;
        sig1->Params()[ix]->TsType()->ToAssemblerTypeWithRank(s1);
        sig2->Params()[ix]->TsType()->ToAssemblerTypeWithRank(s2);
        if (s1.str() != s2.str()) {
            return false;
            break;
        }
    }
    return true;
}

bool ETSChecker::HasSameAssemblySignature(Signature const *const sig1, Signature const *const sig2) noexcept
{
    if (sig1->MinArgCount() != sig2->MinArgCount()) {
        return false;
    }

    if (!CmpAssemblerTypesWithRank(sig1, sig2)) {
        return false;
    }
    auto *rv1 = sig1->RestVar();
    auto *rv2 = sig2->RestVar();
    if (rv1 == nullptr && rv2 == nullptr) {
        return true;
    }
    if (rv1 == nullptr || rv2 == nullptr) {  // exactly one of them is null
        return false;
    }
    std::stringstream s1;
    std::stringstream s2;
    rv1->TsType()->ToAssemblerTypeWithRank(s1);
    rv2->TsType()->ToAssemblerTypeWithRank(s2);
    return s1.str() == s2.str();
}

bool ETSChecker::HasSameAssemblySignatures(ETSFunctionType const *const func1,
                                           ETSFunctionType const *const func2) noexcept
{
    for (auto const *sig1 : func1->CallSignatures()) {
        for (auto const *sig2 : func2->CallSignatures()) {
            if (HasSameAssemblySignature(sig1, sig2)) {
                return true;
            }
        }
    }
    return false;
}
}  // namespace ark::es2panda::checker
