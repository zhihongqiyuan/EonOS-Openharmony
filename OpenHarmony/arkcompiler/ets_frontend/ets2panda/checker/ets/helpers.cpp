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

#include "util/helpers.h"
#include "macros.h"
#include "varbinder/ETSBinder.h"
#include "parser/ETSparser.h"

#include "checker/types/ets/etsTupleType.h"
#include "checker/types/ets/etsReadonlyType.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/ETSchecker.h"
#include "checker/types/globalTypesHolder.h"
#include "evaluate/scopedDebugInfoPlugin.h"

#include "compiler/lowering/scopesInit/scopesInitPhase.h"
#include "generated/signatures.h"

namespace ark::es2panda::checker {
varbinder::Variable *ETSChecker::FindVariableInFunctionScope(const util::StringView name,
                                                             const varbinder::ResolveBindingOptions options)
{
    return Scope()->FindInFunctionScope(name, options).variable;
}

std::pair<varbinder::Variable *, const ETSObjectType *> ETSChecker::FindVariableInClassOrEnclosing(
    const util::StringView name, const ETSObjectType *classType)
{
    const auto searchFlags = PropertySearchFlags::SEARCH_ALL | PropertySearchFlags::SEARCH_IN_BASE |
                             PropertySearchFlags::SEARCH_IN_INTERFACES;
    auto *resolved = classType->GetProperty(name, searchFlags);
    while (classType->EnclosingType() != nullptr && resolved == nullptr) {
        classType = classType->EnclosingType();
        resolved = classType->GetProperty(name, searchFlags);
    }

    return {resolved, classType};
}

varbinder::Variable *ETSChecker::FindVariableInGlobal(const ir::Identifier *const identifier,
                                                      const varbinder::ResolveBindingOptions options)
{
    return Scope()->FindInGlobal(identifier->Name(), options).variable;
}

bool ETSChecker::IsVariableStatic(const varbinder::Variable *var)
{
    if (var->HasFlag(varbinder::VariableFlags::METHOD)) {
        return var->TsType()->AsETSFunctionType()->CallSignatures()[0]->HasSignatureFlag(SignatureFlags::STATIC);
    }
    return var->HasFlag(varbinder::VariableFlags::STATIC);
}

bool ETSChecker::IsVariableGetterSetter(const varbinder::Variable *var)
{
    return var->TsType() != nullptr && var->TsType()->HasTypeFlag(TypeFlag::GETTER_SETTER);
}

void ETSChecker::LogUnresolvedReferenceError(ir::Identifier *const ident)
{
    LogTypeError({"Unresolved reference ", ident->Name()}, ident->Start());
}

void ETSChecker::WrongContextErrorClassifyByType(ir::Identifier *ident)
{
    std::string identCategoryName;
    switch (static_cast<varbinder::VariableFlags>(
        ident->Variable()->Flags() &
        (varbinder::VariableFlags::CLASS_OR_INTERFACE_OR_ENUM | varbinder::VariableFlags::METHOD))) {
        case varbinder::VariableFlags::CLASS: {
            identCategoryName = "Class";
            break;
        }
        case varbinder::VariableFlags::METHOD: {
            identCategoryName = "Function";
            break;
        }
        case varbinder::VariableFlags::INTERFACE: {
            identCategoryName = "Interface";
            break;
        }
        case varbinder::VariableFlags::ENUM_LITERAL: {
            identCategoryName = "Enum";
            break;
        }
        default: {
            LogUnresolvedReferenceError(ident);
            return;
        }
    }
    LogTypeError({identCategoryName.c_str(), " name \"", ident->Name(), "\" used in the wrong context"},
                 ident->Start());
}

void ETSChecker::NotResolvedError(ir::Identifier *const ident, const varbinder::Variable *classVar,
                                  const ETSObjectType *classType)
{
    if (classVar == nullptr) {
        LogUnresolvedReferenceError(ident);
        return;
    }

    if (IsVariableStatic(classVar)) {
        LogTypeError(
            {"Static property '", ident->Name(), "' must be accessed through it's class '", classType->Name(), "'"},
            ident->Start());
    } else {
        LogTypeError({"Property '", ident->Name(), "' must be accessed through 'this'"}, ident->Start());
    }
}

std::pair<const ir::Identifier *, ir::TypeNode *> ETSChecker::GetTargetIdentifierAndType(ir::Identifier *const ident)
{
    if (ident->Parent()->IsClassProperty()) {
        const auto *const classProp = ident->Parent()->AsClassProperty();
        ASSERT(classProp->Value() && classProp->Value() == ident);
        return std::make_pair(classProp->Key()->AsIdentifier(), classProp->TypeAnnotation());
    }
    const auto *const variableDecl = ident->Parent()->AsVariableDeclarator();
    ASSERT(variableDecl->Init() && variableDecl->Init() == ident);
    return std::make_pair(variableDecl->Id()->AsIdentifier(), variableDecl->Id()->AsIdentifier()->TypeAnnotation());
}

varbinder::Variable *ETSChecker::ExtraCheckForResolvedError(ir::Identifier *const ident)
{
    const auto [class_var, class_type] = FindVariableInClassOrEnclosing(ident->Name(), Context().ContainingClass());
    auto *parentClass = FindAncestorGivenByType(ident, ir::AstNodeType::CLASS_DEFINITION);
    if (parentClass != nullptr && parentClass->AsClassDefinition()->IsLocal()) {
        if (parentClass != class_type->GetDeclNode()) {
            LogTypeError({"Property '", ident->Name(), "' of enclosing class '", class_type->Name(),
                          "' is not allowed to be captured from the local class '",
                          parentClass->AsClassDefinition()->Ident()->Name(), "'"},
                         ident->Start());
        }
    }
    NotResolvedError(ident, class_var, class_type);
    return class_var;
}

bool ETSChecker::SaveCapturedVariableInLocalClass(varbinder::Variable *const var, ir::Identifier *ident)
{
    const auto &pos = ident->Start();

    if (!HasStatus(CheckerStatus::IN_LOCAL_CLASS)) {
        return false;
    }

    if (!var->HasFlag(varbinder::VariableFlags::LOCAL)) {
        return false;
    }

    LOG(DEBUG, ES2PANDA) << "Checking variable (line:" << pos.line << "): " << var->Name();
    auto *scopeIter = Scope();
    bool inStaticMethod = false;

    auto captureVariable = [this, var, ident, &scopeIter, &inStaticMethod, &pos]() {
        if (inStaticMethod) {
            LogTypeError({"Not allowed to capture variable '", var->Name(), "' in static method"}, pos);
            return false;
        }
        if (scopeIter->Node()->AsClassDefinition()->CaptureVariable(var)) {
            LOG(DEBUG, ES2PANDA) << "  Captured in class:" << scopeIter->Node()->AsClassDefinition()->Ident()->Name();
        }

        auto *parent = ident->Parent();

        if (parent->IsVariableDeclarator()) {
            parent = parent->Parent()->Parent();
        }

        if (!(parent->IsUpdateExpression() ||
              (parent->IsAssignmentExpression() && parent->AsAssignmentExpression()->Left() == ident)) ||
            var->Declaration() == nullptr) {
            return false;
        }

        if (var->Declaration()->IsParameterDecl()) {
            LOG(DEBUG, ES2PANDA) << "    - Modified parameter ";
            scopeIter->Node()->AsClassDefinition()->AddToLocalVariableIsNeeded(var);
        }
        return true;
    };

    while (scopeIter != var->GetScope()) {
        if (scopeIter->Node() != nullptr) {
            if (scopeIter->Node()->IsScriptFunction() && scopeIter->Node()->AsScriptFunction()->IsStatic()) {
                inStaticMethod = true;
            }

            if (scopeIter->Node()->IsClassDefinition()) {
                captureVariable();
                return true;
            }
        }
        scopeIter = scopeIter->Parent();
    }

    return false;
}

void ETSChecker::SaveCapturedVariable(varbinder::Variable *const var, ir::Identifier *ident)
{
    const auto &pos = ident->Start();

    if (!HasStatus(CheckerStatus::IN_LAMBDA)) {
        return;
    }

    if (var->HasFlag(varbinder::VariableFlags::PROPERTY)) {
        Context().AddCapturedVar(var, pos);
        return;
    }

    if ((!var->HasFlag(varbinder::VariableFlags::LOCAL) && !var->HasFlag(varbinder::VariableFlags::METHOD)) ||
        Context().ContainingLambda()->IsVarFromSubscope(var)) {
        return;
    }

    const auto *scopeIter = Scope();
    while (scopeIter != var->GetScope()) {
        if (scopeIter->IsFunctionScope()) {
            Context().AddCapturedVar(var, pos);
            return;
        }
        scopeIter = scopeIter->Parent();
    }
}

Type *ETSChecker::ResolveIdentifier(ir::Identifier *ident)
{
    if (ident->Variable() != nullptr) {
        auto *const resolved = ident->Variable();
        SaveCapturedVariable(resolved, ident);
        return GetTypeOfVariable(resolved);
    }

    auto options = ident->Parent()->IsTSTypeAliasDeclaration() ? varbinder::ResolveBindingOptions::TYPE_ALIASES
                                                               : varbinder::ResolveBindingOptions::ALL_NON_TYPE;

    auto *resolved = FindVariableInFunctionScope(ident->Name(), options);
    if (resolved == nullptr) {
        // If the reference is not found already in the current class, then it is not bound to the class, so we have to
        // find the reference in the global class first, then in the global scope
        resolved = FindVariableInGlobal(ident, options);
        if (UNLIKELY(resolved == nullptr && debugInfoPlugin_ != nullptr)) {
            resolved = debugInfoPlugin_->FindIdentifier(ident);
        }
    }

    if (resolved == nullptr) {
        resolved = ExtraCheckForResolvedError(ident);
        ident->SetVariable(resolved);
        if (resolved == nullptr) {
            return GlobalTypeError();
        }
        return GetTypeOfVariable(resolved);
    }

    ident->SetVariable(resolved);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    ValidateResolvedIdentifier(ident);

    ValidatePropertyAccess(resolved, Context().ContainingClass(), ident->Start());
    SaveCapturedVariable(resolved, ident);

    return GetTypeOfVariable(resolved);
}

std::optional<checker::Type *> CheckLeftRightType(checker::ETSChecker *checker, checker::Type *unboxedL,
                                                  checker::Type *unboxedR)
{
    if (unboxedL->IsDoubleType() || unboxedR->IsDoubleType()) {
        return checker->GlobalDoubleType();
    }
    if (unboxedL->IsFloatType() || unboxedR->IsFloatType()) {
        return checker->GlobalFloatType();
    }
    if (unboxedL->IsLongType() || unboxedR->IsLongType()) {
        return checker->GlobalLongType();
    }
    if (unboxedL->IsIntType() || unboxedR->IsIntType() || unboxedL->IsCharType() || unboxedR->IsCharType()) {
        return checker->GlobalIntType();
    }
    if (unboxedL->IsShortType() || unboxedR->IsShortType()) {
        return checker->GlobalShortType();
    }
    if (unboxedL->IsByteType() || unboxedR->IsByteType()) {
        return checker->GlobalByteType();
    }
    return std::nullopt;
}
checker::Type *ETSChecker::ApplyConditionalOperatorPromotion(checker::ETSChecker *checker, checker::Type *unboxedL,
                                                             checker::Type *unboxedR)
{
    if ((unboxedL->HasTypeFlag(checker::TypeFlag::CONSTANT) && unboxedL->IsIntType()) ||
        (unboxedR->HasTypeFlag(checker::TypeFlag::CONSTANT) && unboxedR->IsIntType())) {
        int value = unboxedL->IsIntType() ? unboxedL->AsIntType()->GetValue() : unboxedR->AsIntType()->GetValue();
        checker::Type *otherType = !unboxedL->IsIntType() ? unboxedL : unboxedR;

        switch (checker::ETSChecker::ETSType(otherType)) {
            case checker::TypeFlag::BYTE:
            case checker::TypeFlag::CHAR: {
                if (value <= static_cast<int>(std::numeric_limits<char>::max()) &&
                    value >= static_cast<int>(std::numeric_limits<char>::min())) {
                    return checker->GetNonConstantType(otherType);
                }
                break;
            }
            case checker::TypeFlag::SHORT: {
                if (value <= std::numeric_limits<int16_t>::max() && value >= std::numeric_limits<int16_t>::min()) {
                    return checker->GlobalShortType();
                }
                break;
            }
            default: {
                return otherType;
            }
        }
        return checker->GlobalIntType();
    }

    auto checkLeftRight = CheckLeftRightType(checker, unboxedL, unboxedR);
    if (checkLeftRight.has_value()) {
        return checkLeftRight.value();
    }
    UNREACHABLE();
}

Type *ETSChecker::ApplyUnaryOperatorPromotion(Type *type, const bool createConst, const bool doPromotion,
                                              const bool isCondExpr)
{
    Type *unboxedType = isCondExpr ? MaybeUnboxConditionalInRelation(type) : MaybeUnboxInRelation(type);

    if (unboxedType == nullptr) {
        return nullptr;
    }
    if (doPromotion) {
        switch (ETSType(unboxedType)) {
            case TypeFlag::BYTE:
            case TypeFlag::SHORT:
            case TypeFlag::CHAR: {
                if (!createConst) {
                    return GlobalIntType();
                }

                return CreateIntTypeFromType(unboxedType);
            }
            default: {
                break;
            }
        }
    }
    return unboxedType;
}

bool ETSChecker::IsNullLikeOrVoidExpression(const ir::Expression *expr) const
{
    // NOTE(vpukhov): #19701 void refactoring
    return expr->TsType()->DefinitelyETSNullish() || expr->TsType()->IsETSVoidType();
}

std::tuple<bool, bool> ETSChecker::IsResolvedAndValue(const ir::Expression *expr, Type *type) const
{
    auto [isResolve, isValue] =
        IsNullLikeOrVoidExpression(expr) ? std::make_tuple(true, false) : type->ResolveConditionExpr();

    const Type *tsType = expr->TsType();
    if (tsType->DefinitelyNotETSNullish() && !type->IsETSPrimitiveType()) {
        isResolve = true;
        isValue = true;
    }
    return std::make_tuple(isResolve, isValue);
}

Type *ETSChecker::HandleBooleanLogicalOperatorsExtended(Type *leftType, Type *rightType, ir::BinaryExpression *expr)
{
    ASSERT(leftType->IsConditionalExprType() && rightType->IsConditionalExprType());

    auto [resolveLeft, leftValue] = IsResolvedAndValue(expr->Left(), leftType);
    auto [resolveRight, rightValue] = IsResolvedAndValue(expr->Right(), rightType);

    if (!resolveLeft && !resolveRight && IsTypeIdenticalTo(leftType, rightType)) {
        return leftType;
    }

    switch (expr->OperatorType()) {
        case lexer::TokenType::PUNCTUATOR_LOGICAL_OR: {
            if (leftValue) {
                expr->SetResult(expr->Left());
                return leftType->IsETSBooleanType() ? CreateETSBooleanType(true) : leftType;
            }

            expr->SetResult(expr->Right());
            return rightType->IsETSBooleanType() && resolveRight ? CreateETSBooleanType(rightValue) : rightType;
        }
        case lexer::TokenType::PUNCTUATOR_LOGICAL_AND: {
            if (leftValue) {
                expr->SetResult(expr->Right());
                return rightType->IsETSBooleanType() && resolveRight ? CreateETSBooleanType(rightValue) : rightType;
            }

            expr->SetResult(expr->Left());
            return leftType->IsETSBooleanType() ? CreateETSBooleanType(false) : leftType;
        }
        default: {
            break;
        }
    }

    UNREACHABLE();
}

Type *ETSChecker::HandleBooleanLogicalOperators(Type *leftType, Type *rightType, lexer::TokenType tokenType)
{
    using UType = typename ETSBooleanType::UType;
    ASSERT(leftType->IsETSBooleanType() && rightType->IsETSBooleanType());

    if (!leftType->HasTypeFlag(checker::TypeFlag::CONSTANT) || !rightType->HasTypeFlag(checker::TypeFlag::CONSTANT)) {
        return GlobalETSBooleanType();
    }

    UType leftValue = leftType->AsETSBooleanType()->GetValue();
    UType rightValue = rightType->AsETSBooleanType()->GetValue();

    switch (tokenType) {
        case lexer::TokenType::PUNCTUATOR_BITWISE_XOR: {
            return CreateETSBooleanType(leftValue ^ rightValue);
        }
        case lexer::TokenType::PUNCTUATOR_BITWISE_AND: {
            return CreateETSBooleanType((static_cast<uint8_t>(leftValue) & static_cast<uint8_t>(rightValue)) != 0);
        }
        case lexer::TokenType::PUNCTUATOR_BITWISE_OR: {
            return CreateETSBooleanType((static_cast<uint8_t>(leftValue) | static_cast<uint8_t>(rightValue)) != 0);
        }
        case lexer::TokenType::PUNCTUATOR_LOGICAL_OR: {
            return CreateETSBooleanType(leftValue || rightValue);
        }
        case lexer::TokenType::PUNCTUATOR_LOGICAL_AND: {
            return CreateETSBooleanType(leftValue && rightValue);
        }
        default: {
            break;
        }
    }

    UNREACHABLE();
    return nullptr;
}

void ETSChecker::ResolveReturnStatement(checker::Type *funcReturnType, checker::Type *argumentType,
                                        ir::ScriptFunction *containingFunc, ir::ReturnStatement *st)
{
    if (funcReturnType->IsETSReferenceType() || argumentType->IsETSReferenceType()) {
        // function return type should be of reference (object) type
        Relation()->SetFlags(checker::TypeRelationFlag::NONE);

        if (!argumentType->IsETSReferenceType()) {
            argumentType = MaybeBoxInRelation(argumentType);
            if (argumentType == nullptr) {
                LogTypeError("Invalid return statement expression", st->Argument()->Start());
            } else {
                st->Argument()->AddBoxingUnboxingFlags(GetBoxingFlag(argumentType));
            }
        }

        if (!funcReturnType->IsETSReferenceType()) {
            funcReturnType = MaybeBoxInRelation(funcReturnType);
            if (funcReturnType == nullptr) {
                LogTypeError("Invalid return function expression", st->Start());
            }
        }
        if (argumentType != nullptr && funcReturnType != nullptr) {
            funcReturnType = CreateETSUnionType({funcReturnType, argumentType});
            containingFunc->Signature()->SetReturnType(funcReturnType);
            containingFunc->Signature()->AddSignatureFlag(checker::SignatureFlags::INFERRED_RETURN_TYPE);
        }
    } else if (funcReturnType->IsETSPrimitiveType() && argumentType->IsETSPrimitiveType()) {
        // function return type is of primitive type (including enums):
        Relation()->SetFlags(checker::TypeRelationFlag::DIRECT_RETURN |
                             checker::TypeRelationFlag::IN_ASSIGNMENT_CONTEXT |
                             checker::TypeRelationFlag::ASSIGNMENT_CONTEXT);
        if (Relation()->IsAssignableTo(funcReturnType, argumentType)) {
            funcReturnType = argumentType;
            containingFunc->Signature()->SetReturnType(funcReturnType);
            containingFunc->Signature()->AddSignatureFlag(checker::SignatureFlags::INFERRED_RETURN_TYPE);
        } else if (!Relation()->IsAssignableTo(argumentType, funcReturnType)) {
            const Type *targetType = TryGettingFunctionTypeFromInvokeFunction(funcReturnType);
            const Type *sourceType = TryGettingFunctionTypeFromInvokeFunction(argumentType);

            LogTypeError({"Function cannot have different primitive return types, require '", targetType, "', found '",
                          sourceType, "'"},
                         st->Argument()->Start());
        }
    } else {
        // Should never in this branch.
        UNREACHABLE();
        return;
    }
}

checker::Type *ETSChecker::CheckArrayElements(ir::ArrayExpression *init)
{
    ArenaVector<checker::Type *> elementTypes(Allocator()->Adapter());
    for (auto e : init->AsArrayExpression()->Elements()) {
        Type *eType = e->Check(this);
        if (eType->HasTypeFlag(TypeFlag::TYPE_ERROR)) {
            return eType;
        }
        elementTypes.push_back(GetNonConstantType(eType));
    }

    if (elementTypes.empty()) {
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        return Allocator()->New<ETSArrayType>(GlobalETSObjectType());
    }
    auto const isNumeric = [](checker::Type *ct) { return ct->HasTypeFlag(TypeFlag::ETS_CONVERTIBLE_TO_NUMERIC); };
    auto const isChar = [](checker::Type *ct) { return ct->HasTypeFlag(TypeFlag::CHAR); };
    auto const elementType =
        std::all_of(elementTypes.begin(), elementTypes.end(), isNumeric)
            ? std::all_of(elementTypes.begin(), elementTypes.end(), isChar) ? GlobalCharType() : GlobalDoubleType()
            : CreateETSUnionType(std::move(elementTypes));

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    return Allocator()->New<ETSArrayType>(elementType);
}

void ETSChecker::InferAliasLambdaType(ir::TypeNode *localTypeAnnotation, ir::ArrowFunctionExpression *init)
{
    ASSERT(localTypeAnnotation != nullptr);

    if (localTypeAnnotation->IsETSTypeReference()) {
        bool isAnnotationTypeAlias = true;
        while (localTypeAnnotation->IsETSTypeReference() && isAnnotationTypeAlias) {
            auto *nodeVar = localTypeAnnotation->AsETSTypeReference()->Part()->Name()->AsIdentifier()->Variable();
            if (nodeVar == nullptr) {
                break;
            }

            auto *node = nodeVar->Declaration()->Node();

            isAnnotationTypeAlias = node->IsTSTypeAliasDeclaration();
            if (isAnnotationTypeAlias) {
                localTypeAnnotation = node->AsTSTypeAliasDeclaration()->TypeAnnotation();
            }
        }
    }

    if (localTypeAnnotation->IsETSFunctionType()) {
        auto *const arrowFuncExpr = init;
        ir::ScriptFunction *const lambda = arrowFuncExpr->Function();
        if (lambda->Params().size() == localTypeAnnotation->AsETSFunctionType()->Params().size() &&
            NeedTypeInference(lambda)) {
            InferTypesForLambda(lambda, localTypeAnnotation->AsETSFunctionType());
        }
    }
}

checker::Type *ETSChecker::FixOptionalVariableType(varbinder::Variable *const bindingVar, ir::ModifierFlags flags,
                                                   ir::Expression *init)
{
    if ((flags & ir::ModifierFlags::OPTIONAL) != 0) {
        if (init != nullptr && bindingVar->TsType()->IsETSPrimitiveType()) {
            init->SetBoxingUnboxingFlags(GetBoxingFlag(bindingVar->TsType()));
        }
        bindingVar->SetTsType(CreateETSUnionType({GlobalETSUndefinedType(), bindingVar->TsType()}));
    }
    return bindingVar->TsType();
}

checker::Type *PreferredObjectTypeFromAnnotation(checker::Type *annotationType)
{
    if (!annotationType->IsETSUnionType()) {
        return annotationType;
    }

    checker::Type *resolvedType = nullptr;
    for (auto constituentType : annotationType->AsETSUnionType()->ConstituentTypes()) {
        if (constituentType->IsETSObjectType()) {
            if (resolvedType != nullptr) {
                return nullptr;
            }
            resolvedType = constituentType;
        }
    }

    return resolvedType;
}

bool ETSChecker::CheckInit(ir::Identifier *ident, ir::TypeNode *typeAnnotation, ir::Expression *init,
                           checker::Type *annotationType, varbinder::Variable *const bindingVar)
{
    if (typeAnnotation == nullptr) {
        if (init->IsArrayExpression()) {
            annotationType = CheckArrayElements(init->AsArrayExpression());
            bindingVar->SetTsType(annotationType);
        }

        if (init->IsObjectExpression()) {
            LogTypeError(
                {"Cannot infer type for ", ident->Name(), " because class composite needs an explicit target type"},
                ident->Start());
            return false;
        }
    }

    if (init->IsMemberExpression() && init->AsMemberExpression()->Object()->IsObjectExpression()) {
        LogTypeError({"Class composite must be constructed separately before referring their members."},
                     ident->Start());
    }

    if (annotationType != nullptr && annotationType->HasTypeFlag(TypeFlag::TYPE_ERROR)) {
        return false;
    }

    if ((init->IsMemberExpression()) && (annotationType != nullptr)) {
        SetArrayPreferredTypeForNestedMemberExpressions(init->AsMemberExpression(), annotationType);
    }

    if (init->IsArrayExpression() && (annotationType != nullptr) && !annotationType->IsETSDynamicType()) {
        if (annotationType->IsETSTupleType()) {
            if (!ValidateTupleMinElementSize(init->AsArrayExpression(), annotationType->AsETSTupleType())) {
                return false;
            }
        }

        init->AsArrayExpression()->SetPreferredType(annotationType);
    }

    if (init->IsObjectExpression() && annotationType != nullptr) {
        init->AsObjectExpression()->SetPreferredType(PreferredObjectTypeFromAnnotation(annotationType));
    }

    if (typeAnnotation != nullptr && init->IsArrowFunctionExpression()) {
        InferAliasLambdaType(typeAnnotation, init->AsArrowFunctionExpression());
    }

    return true;
}
void ETSChecker::CheckEnumType(ir::Expression *init, checker::Type *initType, const util::StringView &varName)
{
    if (initType->IsETSObjectType() && initType->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::ENUM) &&
        !init->IsMemberExpression()) {
        LogTypeError({"Cannot assign type '", initType->AsETSObjectType()->Name(), "' for variable ", varName, "."},
                     init->Start());
    }
}

static bool NeedWideningBasedOnInitializerHeuristics(ir::Expression *e)
{
    // NOTE: need to be done by smart casts. Return true if we need to infer wider type.
    if (e->IsUnaryExpression()) {
        return NeedWideningBasedOnInitializerHeuristics(e->AsUnaryExpression()->Argument());
    }
    const bool isConstInit =
        e->IsIdentifier() && e->Variable() != nullptr && e->Variable()->Declaration()->IsConstDecl();

    return e->IsConditionalExpression() || e->IsLiteral() || isConstInit;
}

// Isolated until 'constant' types are tracked in some cases
static bool ShouldPreserveConstantTypeInVariableDeclaration(Type *annotation, Type *init)
{
    auto const isNumericWithConstTracking = [](Type *type) {
        return type->HasTypeFlag(TypeFlag::ETS_NUMERIC) || type->IsCharType();
    };

    return ((isNumericWithConstTracking(init) && isNumericWithConstTracking(annotation)) ||
            (init->IsETSStringType() && annotation->IsETSStringType()));
}

checker::Type *ETSChecker::CheckVariableDeclaration(ir::Identifier *ident, ir::TypeNode *typeAnnotation,
                                                    ir::Expression *init, ir::ModifierFlags const flags)
{
    const util::StringView &varName = ident->Name();
    ASSERT(ident->Variable());
    varbinder::Variable *const bindingVar = ident->Variable();
    checker::Type *annotationType = nullptr;

    const bool isConst = (flags & ir::ModifierFlags::CONST) != 0;
    const bool isReadonly = (flags & ir::ModifierFlags::READONLY) != 0;
    const bool isStatic = (flags & ir::ModifierFlags::STATIC) != 0;
    // Note(lujiahui): It should be checked if the readonly function parameter and readonly number[] parameters
    // are assigned with CONSTANT, which would not be correct. (After feature supported)
    const bool omitConstInit = isConst || (isReadonly && isStatic);

    if (typeAnnotation != nullptr) {
        annotationType = typeAnnotation->GetType(this);
        bindingVar->SetTsType(annotationType);
    }

    if (init == nullptr) {
        return FixOptionalVariableType(bindingVar, flags, init);
    }

    if (!CheckInit(ident, typeAnnotation, init, annotationType, bindingVar)) {
        return GlobalTypeError();
    }

    checker::Type *initType = init->Check(this);

    if (initType == nullptr) {
        LogTypeError("Cannot get the expression type", init->Start());
        return GlobalTypeError();
    }

    if (typeAnnotation == nullptr && initType->IsETSFunctionType()) {
        annotationType =
            initType->AsETSFunctionType()->FunctionalInterface() == nullptr
                ? FunctionTypeToFunctionalInterfaceType(initType->AsETSFunctionType()->CallSignatures().front())
                : initType->AsETSFunctionType()->FunctionalInterface();
        bindingVar->SetTsType(annotationType);
    }

    if (annotationType != nullptr) {
        CheckAnnotationTypeForVariableDeclaration(annotationType, annotationType->IsETSUnionType(), init, initType);

        if (omitConstInit && ShouldPreserveConstantTypeInVariableDeclaration(annotationType, initType)) {
            bindingVar->SetTsType(init->TsType());
        }
        return FixOptionalVariableType(bindingVar, flags, init);
    }

    CheckEnumType(init, initType, varName);

    // NOTE: need to be done by smart casts
    auto needWidening = !omitConstInit && typeAnnotation == nullptr && NeedWideningBasedOnInitializerHeuristics(init);
    bindingVar->SetTsType(needWidening ? GetNonConstantType(initType) : initType);

    return FixOptionalVariableType(bindingVar, flags, init);
}

void ETSChecker::CheckAnnotationTypeForVariableDeclaration(checker::Type *annotationType, bool isUnionFunction,
                                                           ir::Expression *init, checker::Type *initType)
{
    Type *sourceType = TryGettingFunctionTypeFromInvokeFunction(initType);

    if (!isUnionFunction && annotationType->IsETSUnionType()) {
        for (auto it : annotationType->AsETSUnionType()->ConstituentTypes()) {
            if (it->IsETSFunctionType() ||
                (it->IsETSObjectType() && it->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::FUNCTIONAL))) {
                isUnionFunction = true;
                break;
            }
        }
    }

    if (!AssignmentContext(Relation(), init, initType, annotationType, init->Start(), {}, TypeRelationFlag::NO_THROW)
             // CC-OFFNXT(G.FMT.02) project code style
             .IsAssignable()) {
        Type *targetType = isUnionFunction ? annotationType : TryGettingFunctionTypeFromInvokeFunction(annotationType);
        LogTypeError({"Type '", sourceType, "' cannot be assigned to type '", targetType, "'"}, init->Start());
    }
}

//==============================================================================//
// Smart cast support
//==============================================================================//

checker::Type *ETSChecker::ResolveSmartType(checker::Type *sourceType, checker::Type *targetType)
{
    //  For left-hand variable of primitive type leave it as is.
    if (targetType->IsETSPrimitiveType()) {
        return targetType;
    }

    //  For left-hand variable of tuple type leave it as is.
    if (targetType->IsETSTupleType()) {
        return targetType;
    }

    //  For left-hand variable of builtin type leave it as is.
    if (targetType->IsETSObjectType() && targetType->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::BUILTIN_TYPE)) {
        return targetType;
    }

    //  For the Function source or target types leave the target type as is
    //  until we will be able to create the functional interface type from the source.
    if (targetType->HasTypeFlag(TypeFlag::FUNCTION) || sourceType->HasTypeFlag(TypeFlag::FUNCTION)) {
        return targetType;
    }

    // Nothing to do with identical types:
    auto *nonConstSourceType = GetNonConstantType(sourceType);
    auto *nonConstTargetType = GetNonConstantType(targetType);

    if (Relation()->IsIdenticalTo(nonConstSourceType, nonConstTargetType) ||
        Relation()->IsIdenticalTo(GlobalBuiltinJSValueType(), nonConstTargetType)) {
        return targetType;
    }

    //  For type parameter, null or undefined source type return it as is.
    if (sourceType->IsETSTypeParameter() || sourceType->DefinitelyETSNullish()) {
        return sourceType;
    }

    //  In case of Union left-hand type we have to select the proper type from the Union
    //  Because now we have logging of errors we have to continue analyze incorrect program, for
    //  this case we change typeError to source type.
    if (targetType->IsETSUnionType()) {
        auto component = targetType->AsETSUnionType()->GetAssignableType(this, sourceType);
        return component->IsTypeError() ? MaybeBoxType(sourceType) : component;
    }

    //  If source is reference type, set it as the current and use it for identifier smart cast
    if (sourceType->IsETSReferenceType()) {
        return sourceType;
    }

    //  For right-hand variable of primitive type apply boxing conversion (case: 'let x: Object = 5', then x => Int).
    if (sourceType->IsETSPrimitiveType() && !sourceType->IsETSVoidType() && targetType->IsETSObjectType()) {
        return MaybeBoxInRelation(sourceType);
    }

    //  NOTE - it seems that all the other possible cases are assignments like:
    //  'Object = ObjectLiteral' or smth similar ???
    //  thus for such cases also leave the target type as is.
    //  Possible errors in tests should clarify this hypothesis sooner or later :)
    return targetType;
}

// Auxiliary method to reduce the size of common 'CheckTestSmartCastConditions' function.
std::pair<Type *, Type *> ETSChecker::CheckTestNullishCondition(Type *testedType, Type *actualType, bool const strict)
{
    if (!strict) {
        return RemoveNullishTypes(actualType);
    }

    if (testedType->IsETSNullType()) {
        return {GlobalETSNullType(), RemoveNullType(actualType)};
    }

    if (testedType->IsETSUndefinedType()) {
        return {GlobalETSUndefinedType(), RemoveUndefinedType(actualType)};
    }

    return {GlobalETSNullishType(), GetNonNullishType(actualType)};
}

// Auxiliary method to reduce the size of common 'CheckTestSmartCastConditions' function.
std::pair<Type *, Type *> ETSChecker::CheckTestObjectCondition(ETSArrayType *testedType, Type *actualType)
{
    if (actualType->IsETSUnionType()) {
        return actualType->AsETSUnionType()->GetComplimentaryType(this, testedType);
    }

    // Both testing and actual (smart) types are arrays. Set types according to their relation.
    // NOTE: probably the rules of type extraction should be modified later on!
    if (actualType->IsETSArrayType()) {
        auto *const arrayType = actualType->AsETSArrayType();

        if (Relation()->IsIdenticalTo(arrayType, testedType) ||
            arrayType->AssemblerName() == testedType->AssemblerName()) {
            return {testedType, GetGlobalTypesHolder()->GlobalNeverType()};
        }

        if (Relation()->IsSupertypeOf(arrayType, testedType)) {
            return {testedType, actualType};
        }

        if (Relation()->IsSupertypeOf(testedType, arrayType)) {
            return {testedType, actualType};
        }
    } else if (actualType->IsETSObjectType() && actualType->AsETSObjectType()->IsGlobalETSObjectType()) {
        return {testedType, actualType};
    }

    return {GetGlobalTypesHolder()->GlobalNeverType(), actualType};
}

// Auxiliary method to reduce the size of common 'CheckTestSmartCastConditions' function.
std::pair<Type *, Type *> ETSChecker::CheckTestObjectCondition(ETSObjectType *testedType, Type *actualType,
                                                               bool const strict)
{
    if (actualType->IsETSUnionType()) {
        return actualType->AsETSUnionType()->GetComplimentaryType(this, testedType);
    }

    // Both testing and actual (smart) types are objects. Set types according to their relation.
    // NOTE: probably the rules of type extraction should be modified later on!
    if (actualType->IsETSObjectType()) {
        auto *const objectType = actualType->AsETSObjectType();

        if (Relation()->IsIdenticalTo(objectType, testedType) ||
            objectType->AssemblerName() == testedType->AssemblerName()) {
            return {testedType, strict ? GetGlobalTypesHolder()->GlobalNeverType() : actualType};
        }

        if (Relation()->IsSupertypeOf(objectType, testedType)) {
            return {testedType, actualType};
        }

        if (Relation()->IsSupertypeOf(testedType, objectType)) {
            return {testedType, actualType};
        }

        return {GetGlobalTypesHolder()->GlobalNeverType(), actualType};
    }

    // NOTE: other cases (for example with functional types) will be implemented later on
    return {testedType, actualType};
}

static constexpr std::size_t const VARIABLE_POSITION = 0UL;
static constexpr std::size_t const CONSEQUENT_TYPE_POSITION = 1UL;
static constexpr std::size_t const ALTERNATE_TYPE_POSITION = 2UL;

void CheckerContext::CheckTestSmartCastCondition(lexer::TokenType operatorType)
{
    if (operatorType != lexer::TokenType::EOS && operatorType != lexer::TokenType::PUNCTUATOR_LOGICAL_AND &&
        operatorType != lexer::TokenType::PUNCTUATOR_LOGICAL_OR) {
        return;
    }

    auto types = ResolveSmartCastTypes();

    if (operatorType_ == lexer::TokenType::PUNCTUATOR_LOGICAL_AND) {
        if (types.has_value()) {
            auto const &variable = std::get<VARIABLE_POSITION>(*types);
            //  NOTE: now we support only cases like 'if (x != null && y == null)' but don't support different type
            //  checks for a single variable (like 'if (x != null && x instanceof string)'), because it seems that
            //  it doesn't make much sense.
            //  Can be implemented later on if the need arises.
            if (auto [_, inserted] =
                    testSmartCasts_.emplace(variable, std::make_pair(std::get<CONSEQUENT_TYPE_POSITION>(*types),
                                                                     std::get<ALTERNATE_TYPE_POSITION>(*types)));
                !inserted) {
                testSmartCasts_[variable] = {nullptr, nullptr};
            }
        }
        //  Clear alternate types, because now they become indefinite
        for (auto &smartCast : testSmartCasts_) {
            smartCast.second.second = nullptr;
        }
    } else if (operatorType_ == lexer::TokenType::PUNCTUATOR_LOGICAL_OR) {
        if (bool const cleanConsequent = types.has_value() ? CheckTestOrSmartCastCondition(*types) : true;
            cleanConsequent) {
            //  Clear consequent types, because now they become indefinite
            for (auto &smartCast : testSmartCasts_) {
                smartCast.second.first = nullptr;
            }
        }
    } else if (types.has_value()) {
        testSmartCasts_.emplace(
            std::get<VARIABLE_POSITION>(*types),
            std::make_pair(std::get<CONSEQUENT_TYPE_POSITION>(*types), std::get<ALTERNATE_TYPE_POSITION>(*types)));
    }

    testCondition_ = {};
    operatorType_ = operatorType;
}

std::optional<SmartCastTuple> CheckerContext::ResolveSmartCastTypes()
{
    if (testCondition_.variable == nullptr) {
        return std::nullopt;
    }

    // Exclude processing of global variables and those captured in lambdas and modified there
    auto const *const variableScope = testCondition_.variable->GetScope();
    auto const topLevelVariable =
        variableScope != nullptr ? variableScope->IsGlobalScope() ||
                                       (variableScope->Parent() != nullptr && variableScope->Parent()->IsGlobalScope())
                                 : false;
    if (topLevelVariable) {
        return std::nullopt;
    }

    ASSERT(testCondition_.testedType != nullptr);
    // NOTE: functional types are not supported now
    if (!testCondition_.testedType->IsETSReferenceType() ||
        testCondition_.testedType->HasTypeFlag(TypeFlag::FUNCTION)) {
        return std::nullopt;
    }

    auto *smartType = GetSmartCast(testCondition_.variable);
    if (smartType == nullptr) {
        smartType = testCondition_.variable->TsType();
    }

    auto *const checker = parent_->AsETSChecker();
    Type *consequentType = nullptr;
    Type *alternateType = nullptr;

    if (testCondition_.testedType->DefinitelyETSNullish()) {
        // In case of testing for 'null' and/or 'undefined' remove corresponding null-like types.
        std::tie(consequentType, alternateType) =
            checker->CheckTestNullishCondition(testCondition_.testedType, smartType, testCondition_.strict);
    } else {
        if (testCondition_.testedType->IsETSObjectType()) {
            auto *const testedType = testCondition_.testedType->AsETSObjectType();
            std::tie(consequentType, alternateType) =
                checker->CheckTestObjectCondition(testedType, smartType, testCondition_.strict);
        } else if (testCondition_.testedType->IsETSArrayType()) {
            auto *const testedType = testCondition_.testedType->AsETSArrayType();
            std::tie(consequentType, alternateType) = checker->CheckTestObjectCondition(testedType, smartType);
        } else if (testCondition_.testedType->IsETSUnionType()) {
            //  NOTE: now we don't support 'instanceof' operation for union types?
            UNREACHABLE();
        } else {
            // NOTE: it seems that no more cases are possible here! :)
            UNREACHABLE();
        }
    }

    return !testCondition_.negate
               ? std::make_optional(std::make_tuple(testCondition_.variable, consequentType, alternateType))
               : std::make_optional(std::make_tuple(testCondition_.variable, alternateType, consequentType));
}
bool ETSChecker::CheckVoidAnnotation(const ir::ETSPrimitiveType *typeAnnotation)
{
    // Void annotation is valid only when used as 'return type' , 'type parameter instantiation', 'default type'.
    if (typeAnnotation->GetPrimitiveType() != ir::PrimitiveType::VOID) {
        return true;
    }

    auto parent = typeAnnotation->Parent();
    if (parent->IsScriptFunction() && parent->AsScriptFunction()->ReturnTypeAnnotation() == typeAnnotation) {
        return true;
    }
    if (parent->IsETSFunctionType() && parent->AsETSFunctionType()->ReturnType() == typeAnnotation) {
        return true;
    }
    if (parent->IsTSTypeParameterInstantiation() || parent->IsTSTypeParameter()) {
        return true;
    }
    LogTypeError({"'void' used as type annotation."}, typeAnnotation->Start());
    return false;
}
void ETSChecker::ApplySmartCast(varbinder::Variable const *const variable, checker::Type *const smartType) noexcept
{
    ASSERT(variable != nullptr);
    if (smartType != nullptr) {
        auto *variableType = variable->TsType();

        if (Relation()->IsIdenticalTo(variableType, smartType)) {
            Context().RemoveSmartCast(variable);
        } else {
            Context().SetSmartCast(variable, smartType);
        }
    }
}

bool CheckerContext::CheckTestOrSmartCastCondition(SmartCastTuple const &types)
{
    auto *const &variable = std::get<VARIABLE_POSITION>(types);
    auto *const &consequentTypeNew = std::get<CONSEQUENT_TYPE_POSITION>(types);
    auto *const &alternateTypeNew = std::get<ALTERNATE_TYPE_POSITION>(types);

    if (auto const it = testSmartCasts_.find(variable); it != testSmartCasts_.end()) {
        auto *const consequentTypeOld = it->second.first;
        if (consequentTypeOld == nullptr) {
            return true;
        }

        if (consequentTypeNew != nullptr && !parent_->Relation()->IsIdenticalTo(consequentTypeOld, consequentTypeNew)) {
            it->second.first = parent_->AsETSChecker()->CreateETSUnionType({consequentTypeOld, consequentTypeNew});
        }

        if (auto *const alternateTypeOld = it->second.second; alternateTypeOld != nullptr) {
            if (alternateTypeNew != nullptr &&
                !parent_->Relation()->IsIdenticalTo(alternateTypeOld, alternateTypeNew)) {
                it->second.second = parent_->AsETSChecker()->CreateETSUnionType({alternateTypeOld, alternateTypeNew});
            }
        } else {
            it->second.second = alternateTypeNew;
        }

        return false;
    }

    //  NOTE: now we support only cases like 'if (x != null || y != null)' or 'if (x instanceof A || x instanceof B)'
    //  although it seems that the resulting variable type in the second case isn't used in subsequent code directly.
    //  More complex conditions can be implemented later on if the need arises.
    testSmartCasts_.emplace(variable, std::make_pair(consequentTypeNew, alternateTypeNew));
    return true;
}

//==============================================================================//

void ETSChecker::SetArrayPreferredTypeForNestedMemberExpressions(ir::MemberExpression *expr, Type *annotationType)
{
    if ((expr == nullptr) || (annotationType == nullptr)) {
        return;
    }

    if (expr->Kind() != ir::MemberExpressionKind::ELEMENT_ACCESS) {
        return;
    }

    // Expand all member expressions
    Type *elementType = annotationType;
    ir::Expression *object = expr->Object();
    while ((object != nullptr) && (object->IsMemberExpression())) {
        ir::MemberExpression *memberExpr = object->AsMemberExpression();
        if (memberExpr->Kind() != ir::MemberExpressionKind::ELEMENT_ACCESS) {
            return;
        }

        object = memberExpr->Object();
        elementType = CreateETSArrayType(elementType);
    }

    // Set explicit target type for array
    if ((object != nullptr) && (object->IsArrayExpression())) {
        ir::ArrayExpression *array = object->AsArrayExpression();
        array->SetPreferredType(CreateETSArrayType(elementType));
    }
}

static void CollectAliasParametersForBoxing(Type *expandedAliasType, std::set<Type *> &parametersNeedToBeBoxed,
                                            bool needToBeBoxed)
{
    if (expandedAliasType->IsETSTypeParameter() && needToBeBoxed) {
        parametersNeedToBeBoxed.insert(expandedAliasType);
    } else if (expandedAliasType->IsETSObjectType()) {
        auto objectType = expandedAliasType->AsETSObjectType();
        needToBeBoxed =
            objectType->GetDeclNode()->IsClassDefinition() || objectType->GetDeclNode()->IsTSInterfaceDeclaration();
        for (const auto typeArgument : objectType->TypeArguments()) {
            CollectAliasParametersForBoxing(typeArgument, parametersNeedToBeBoxed, needToBeBoxed);
        }
    } else if (expandedAliasType->IsETSTupleType()) {
        auto tupleType = expandedAliasType->AsETSTupleType();
        needToBeBoxed = false;
        for (auto type : tupleType->GetTupleTypesList()) {
            CollectAliasParametersForBoxing(type, parametersNeedToBeBoxed, needToBeBoxed);
        }
    } else if (expandedAliasType->IsETSArrayType()) {
        auto arrayType = expandedAliasType->AsETSArrayType();
        needToBeBoxed = false;
        auto elementType = arrayType->ElementType();
        CollectAliasParametersForBoxing(elementType, parametersNeedToBeBoxed, needToBeBoxed);
    } else if (expandedAliasType->IsETSUnionType()) {
        auto unionType = expandedAliasType->AsETSUnionType();
        needToBeBoxed = false;
        for (auto type : unionType->ConstituentTypes()) {
            CollectAliasParametersForBoxing(type, parametersNeedToBeBoxed, needToBeBoxed);
        }
    }
}

Type *ETSChecker::HandleTypeAlias(ir::Expression *const name, const ir::TSTypeParameterInstantiation *const typeParams)
{
    ASSERT(name->IsIdentifier() && name->AsIdentifier()->Variable() &&
           name->AsIdentifier()->Variable()->Declaration()->IsTypeAliasDecl());

    auto *const typeAliasNode =
        name->AsIdentifier()->Variable()->Declaration()->AsTypeAliasDecl()->Node()->AsTSTypeAliasDeclaration();

    // NOTE (mmartin): modify for default params
    if ((typeParams == nullptr) != (typeAliasNode->TypeParams() == nullptr)) {
        if (typeParams == nullptr) {
            LogTypeError("Type alias declaration is generic, but no type parameters were provided", name->Start());
            return GlobalTypeError();
        }

        LogTypeError("Type alias declaration is not generic, but type parameters were provided", typeParams->Start());
        return GlobalTypeError();
    }

    if (typeParams == nullptr) {
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        return GetReferencedTypeBase(name);
    }

    for (auto *const origTypeParam : typeParams->Params()) {
        origTypeParam->Check(this);
    }

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    Type *const aliasType = GetReferencedTypeBase(name);
    auto *aliasSub = NewSubstitution();
    if (typeAliasNode->TypeParams()->Params().size() != typeParams->Params().size()) {
        LogTypeError("Wrong number of type parameters for generic type alias", typeParams->Start());
        return GlobalTypeError();
    }

    std::set<Type *> parametersNeedToBeBoxed;
    auto expandedAliasType = aliasType->Substitute(Relation(), aliasSub);
    CollectAliasParametersForBoxing(expandedAliasType, parametersNeedToBeBoxed, false);

    for (std::size_t idx = 0; idx < typeAliasNode->TypeParams()->Params().size(); ++idx) {
        auto *typeAliasTypeName = typeAliasNode->TypeParams()->Params().at(idx)->Name();
        auto *typeAliasType = typeAliasTypeName->Variable()->TsType();
        if (!typeAliasType->IsETSTypeParameter()) {
            continue;
        }
        auto paramType = typeParams->Params().at(idx)->TsType();
        if (parametersNeedToBeBoxed.find(typeAliasType) != parametersNeedToBeBoxed.end()) {
            auto boxedType = MaybeBoxInRelation(typeParams->Params().at(idx)->GetType(this));
            if (boxedType != nullptr) {
                paramType = boxedType;
            }
        }
        aliasSub->insert({typeAliasType->AsETSTypeParameter(), paramType});
    }

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    ValidateGenericTypeAliasForClonedNode(typeAliasNode->AsTSTypeAliasDeclaration(), typeParams);

    return aliasType->Substitute(Relation(), aliasSub);
}

std::vector<util::StringView> ETSChecker::GetNameForSynteticObjectType(const util::StringView &source)
{
    const std::string str = source.Mutf8();
    std::istringstream ss {str};
    const char delimiter = '.';
    std::string token;

    std::vector<util::StringView> syntheticName {};

    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            util::UString sV(token, Allocator());
            syntheticName.emplace_back(sV.View());
        }
    }

    return syntheticName;
}

std::pair<bool, util::StringView> FindSpecifierForModuleObject(ir::ETSImportDeclaration *importDecl,
                                                               util::StringView const &name)
{
    if (importDecl == nullptr) {
        return std::make_pair(true, util::StringView());
    }

    for (auto item : importDecl->Specifiers()) {
        if (item->IsImportSpecifier() && item->AsImportSpecifier()->Imported()->Name().Is(name.Mutf8())) {
            if (!item->AsImportSpecifier()->Imported()->Name().Is(item->AsImportSpecifier()->Local()->Name().Mutf8())) {
                return std::make_pair(true, item->AsImportSpecifier()->Local()->Name());
            }
            return std::make_pair(true, util::StringView());
        }
    }
    return std::make_pair(false, util::StringView());
}

template <checker::PropertyType TYPE>
void ETSChecker::BindingsModuleObjectAddProperty(checker::ETSObjectType *moduleObjType,
                                                 ir::ETSImportDeclaration *importDecl,
                                                 const varbinder::Scope::VariableMap &bindings)
{
    for (auto [_, var] : bindings) {
        (void)_;
        auto [found, aliasedName] = FindSpecifierForModuleObject(importDecl, var->AsLocalVariable()->Name());
        if ((var->AsLocalVariable()->Declaration()->Node()->IsExported() ||
             var->AsLocalVariable()->Declaration()->Node()->IsExportedType()) &&
            found) {
            if (!aliasedName.Empty()) {
                moduleObjType->AddReExportAlias(var->Declaration()->Name(), aliasedName);
            }
            moduleObjType->AddProperty<TYPE>(var->AsLocalVariable(),
                                             FindPropNameForNamespaceImport(var->AsLocalVariable()->Name()));
        }
    }
}

util::StringView ETSChecker::FindPropNameForNamespaceImport(const util::StringView &originalName)
{
    if (auto relatedMapItem =
            VarBinder()->AsETSBinder()->GetSelectiveExportAliasMultimap().find(Program()->SourceFilePath());
        relatedMapItem != VarBinder()->AsETSBinder()->GetSelectiveExportAliasMultimap().end()) {
        if (auto result = std::find_if(relatedMapItem->second.begin(), relatedMapItem->second.end(),
                                       [originalName](const auto &item) { return item.second == originalName; });
            result != relatedMapItem->second.end()) {
            return result->first;
        }
    }

    return originalName;
}

// Helps to prevent searching for the imported file among external sources if it is the entry program
static parser::Program *SelectEntryOrExternalProgram(varbinder::ETSBinder *etsBinder,
                                                     const util::StringView &importPath)
{
    if (importPath.Is(etsBinder->GetGlobalRecordTable()->Program()->AbsoluteName().Mutf8())) {
        return etsBinder->GetGlobalRecordTable()->Program();
    }

    auto programList = etsBinder->GetProgramList(importPath);
    ASSERT(!programList.empty());
    return programList.front();
}

void ETSChecker::SetPropertiesForModuleObject(checker::ETSObjectType *moduleObjType, const util::StringView &importPath,
                                              ir::ETSImportDeclaration *importDecl)
{
    parser::Program *program =
        SelectEntryOrExternalProgram(static_cast<varbinder::ETSBinder *>(VarBinder()), importPath);
    // Check imported properties before assigning them to module object
    if (!program->IsASTChecked()) {
        // NOTE: helps to avoid endless loop in case of recursive imports that uses all bindings
        program->MarkASTAsChecked();
        program->Ast()->Check(this);
    }

    BindingsModuleObjectAddProperty<checker::PropertyType::STATIC_FIELD>(
        moduleObjType, importDecl, program->GlobalClassScope()->StaticFieldScope()->Bindings());

    BindingsModuleObjectAddProperty<checker::PropertyType::STATIC_METHOD>(
        moduleObjType, importDecl, program->GlobalClassScope()->StaticMethodScope()->Bindings());

    BindingsModuleObjectAddProperty<checker::PropertyType::STATIC_DECL>(
        moduleObjType, importDecl, program->GlobalClassScope()->StaticDeclScope()->Bindings());

    BindingsModuleObjectAddProperty<checker::PropertyType::STATIC_DECL>(
        moduleObjType, importDecl, program->GlobalClassScope()->InstanceDeclScope()->Bindings());

    BindingsModuleObjectAddProperty<checker::PropertyType::STATIC_DECL>(
        moduleObjType, importDecl, program->GlobalClassScope()->TypeAliasScope()->Bindings());
}

void ETSChecker::SetrModuleObjectTsType(ir::Identifier *local, checker::ETSObjectType *moduleObjType)
{
    auto *etsBinder = static_cast<varbinder::ETSBinder *>(VarBinder());

    for (auto [bindingName, var] : etsBinder->TopScope()->Bindings()) {
        if (bindingName.Is(local->Name().Mutf8())) {
            var->SetTsType(moduleObjType);
        }
    }
}

Type *ETSChecker::GetReferencedTypeFromBase([[maybe_unused]] Type *baseType, [[maybe_unused]] ir::Expression *name)
{
    return TypeError(name, "Invalid type reference.", name->Start());
}

Type *ETSChecker::GetReferencedTypeBase(ir::Expression *name)
{
    if (name->IsTSQualifiedName()) {
        return name->Check(this);
    }

    ASSERT(name->IsIdentifier() && name->AsIdentifier()->Variable() != nullptr);

    // NOTE: kbaladurin. forbid usage imported entities as types without declarations
    auto *importData = VarBinder()->AsETSBinder()->DynamicImportDataForVar(name->AsIdentifier()->Variable());
    if (importData != nullptr && importData->import->IsPureDynamic()) {
        name->SetTsType(GlobalBuiltinDynamicType(importData->import->Language()));
        return name->TsType();
    }

    auto *refVar = name->AsIdentifier()->Variable()->AsLocalVariable();

    checker::Type *tsType = nullptr;
    switch (refVar->Declaration()->Node()->Type()) {
        case ir::AstNodeType::TS_INTERFACE_DECLARATION: {
            tsType = GetTypeFromInterfaceReference(refVar);
            break;
        }
        case ir::AstNodeType::CLASS_DECLARATION:
        case ir::AstNodeType::STRUCT_DECLARATION:
        case ir::AstNodeType::CLASS_DEFINITION: {
            tsType = GetTypeFromClassReference(refVar);
            break;
        }
        case ir::AstNodeType::TS_ENUM_DECLARATION: {
            // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
            tsType = GetTypeFromEnumReference(refVar);
            break;
        }
        case ir::AstNodeType::TS_TYPE_PARAMETER: {
            tsType = GetTypeFromTypeParameterReference(refVar, name->Start());
            break;
        }
        case ir::AstNodeType::TS_TYPE_ALIAS_DECLARATION: {
            tsType = GetTypeFromTypeAliasReference(refVar);
            break;
        }
        case ir::AstNodeType::ANNOTATION_DECLARATION: {
            LogTypeError("Annotations cannot be used as a type.", name->Start());
            tsType = GlobalTypeError();
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
    name->SetTsType(tsType);
    return tsType;
}

void ETSChecker::ConcatConstantString(util::UString &target, Type *type)
{
    switch (ETSType(type)) {
        case TypeFlag::ETS_OBJECT: {
            ASSERT(type->IsETSStringType());
            target.Append(type->AsETSStringType()->GetValue());
            break;
        }
        case TypeFlag::ETS_BOOLEAN: {
            target.Append(type->AsETSBooleanType()->GetValue() ? "true" : "false");
            break;
        }
        case TypeFlag::BYTE: {
            target.Append(std::to_string(type->AsByteType()->GetValue()));
            break;
        }
        case TypeFlag::CHAR: {
            std::string s(1, type->AsCharType()->GetValue());
            target.Append(s);
            break;
        }
        case TypeFlag::SHORT: {
            target.Append(std::to_string(type->AsShortType()->GetValue()));
            break;
        }
        case TypeFlag::INT: {
            target.Append(std::to_string(type->AsIntType()->GetValue()));
            break;
        }
        case TypeFlag::LONG: {
            target.Append(std::to_string(type->AsLongType()->GetValue()));
            break;
        }
        case TypeFlag::FLOAT: {
            target.Append(std::to_string(type->AsFloatType()->GetValue()));
            break;
        }
        case TypeFlag::DOUBLE: {
            target.Append(std::to_string(type->AsDoubleType()->GetValue()));
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

Type *ETSChecker::HandleStringConcatenation(Type *leftType, Type *rightType)
{
    ASSERT(leftType->IsETSStringType() || rightType->IsETSStringType());

    if (!leftType->HasTypeFlag(checker::TypeFlag::CONSTANT) || !rightType->HasTypeFlag(checker::TypeFlag::CONSTANT) ||
        leftType->IsETSBigIntType() || rightType->IsETSBigIntType()) {
        return GlobalETSStringLiteralType();
    }

    util::UString concatenated(Allocator());
    ConcatConstantString(concatenated, leftType);
    ConcatConstantString(concatenated, rightType);

    return CreateETSStringLiteralType(concatenated.View());
}

checker::ETSFunctionType *ETSChecker::FindFunctionInVectorGivenByName(util::StringView name,
                                                                      ArenaVector<checker::ETSFunctionType *> &list)
{
    for (auto *it : list) {
        if (it->Name() == name) {
            return it;
        }
    }

    return nullptr;
}

bool ETSChecker::IsFunctionContainsSignature(checker::ETSFunctionType *funcType, Signature *signature)
{
    for (auto *it : funcType->CallSignatures()) {
        Relation()->IsCompatibleTo(it, signature);
        if (Relation()->IsTrue()) {
            return true;
        }
    }

    return false;
}

bool ETSChecker::CheckFunctionContainsClashingSignature(const checker::ETSFunctionType *funcType, Signature *signature)
{
    for (auto *it : funcType->CallSignatures()) {
        SavedTypeRelationFlagsContext strfCtx(Relation(), TypeRelationFlag::NONE);
        Relation()->IsCompatibleTo(it, signature);
        if (Relation()->IsTrue() && it->Function()->Id()->Name() == signature->Function()->Id()->Name()) {
            std::stringstream ss;
            it->ToString(ss, nullptr, true);
            auto sigStr1 = ss.str();
            ss.str(std::string {});  // Clear buffer
            signature->ToString(ss, nullptr, true);
            auto sigStr2 = ss.str();
            LogTypeError({"Function '", it->Function()->Id()->Name(), sigStr1.c_str(),
                          "' is redeclared with different signature '", signature->Function()->Id()->Name(),
                          sigStr2.c_str(), "'"},
                         signature->Function()->ReturnTypeAnnotation()->Start());
            return false;
        }
    }
    return true;
}

void ETSChecker::MergeSignatures(checker::ETSFunctionType *target, checker::ETSFunctionType *source)
{
    for (auto *s : source->CallSignatures()) {
        if (IsFunctionContainsSignature(target, s)) {
            continue;
        }

        if (!CheckFunctionContainsClashingSignature(target, s)) {
            continue;
        }
        target->AddCallSignature(s);
    }
}

void ETSChecker::MergeComputedAbstracts(ArenaVector<checker::ETSFunctionType *> &merged,
                                        ArenaVector<checker::ETSFunctionType *> &current)
{
    for (auto *curr : current) {
        auto name = curr->Name();
        auto *found = FindFunctionInVectorGivenByName(name, merged);
        if (found != nullptr) {
            MergeSignatures(found, curr);
            continue;
        }

        merged.push_back(curr);
    }
}

ir::AstNode *ETSChecker::FindAncestorGivenByType(ir::AstNode *node, ir::AstNodeType type, const ir::AstNode *endNode)
{
    auto *iter = node->Parent();

    while (iter != endNode) {
        if (iter->Type() == type) {
            return iter;
        }

        iter = iter->Parent();
    }

    return nullptr;
}

util::StringView ETSChecker::GetContainingObjectNameFromSignature(Signature *signature)
{
    ASSERT(signature->Function());
    auto *iter = signature->Function()->Parent();

    while (iter != nullptr) {
        if (iter->IsClassDefinition()) {
            return iter->AsClassDefinition()->Ident()->Name();
        }

        if (iter->IsTSInterfaceDeclaration()) {
            return iter->AsTSInterfaceDeclaration()->Id()->Name();
        }

        iter = iter->Parent();
    }

    UNREACHABLE();
    return {""};
}

std::optional<const ir::AstNode *> ETSChecker::FindJumpTarget(ir::AstNode *node)
{
    ASSERT(node->IsBreakStatement() || node->IsContinueStatement());

    bool const isContinue = node->IsContinueStatement();

    // Look for label
    auto label = isContinue ? node->AsContinueStatement()->Ident() : node->AsBreakStatement()->Ident();
    if (label != nullptr) {
        auto var = label->Variable();
        if (var != nullptr && var->Declaration()->IsLabelDecl()) {
            return var->Declaration()->Node();
        }

        // Failed to resolve variable for label
        LogUnresolvedReferenceError(label);
        return {};
    }

    // No label, find the nearest loop or switch statement
    const auto *iter = node->Parent();
    while (iter != nullptr) {
        switch (iter->Type()) {
            case ir::AstNodeType::DO_WHILE_STATEMENT:
            case ir::AstNodeType::WHILE_STATEMENT:
            case ir::AstNodeType::FOR_UPDATE_STATEMENT:
            case ir::AstNodeType::FOR_OF_STATEMENT:
            case ir::AstNodeType::SWITCH_STATEMENT: {
                return iter;
            }
            default: {
                break;
            }
        }

        iter = iter->Parent();
    }

    LogTypeError({"Control flow redirection statement can not be used out of loop or switch statement."},
                 node->Start());
    return nullptr;
}

varbinder::VariableFlags ETSChecker::GetAccessFlagFromNode(const ir::AstNode *node)
{
    if (node->IsPrivate()) {
        return varbinder::VariableFlags::PRIVATE;
    }

    if (node->IsProtected()) {
        return varbinder::VariableFlags::PROTECTED;
    }

    return varbinder::VariableFlags::PUBLIC;
}

Type *ETSChecker::CheckSwitchDiscriminant(ir::Expression *discriminant)
{
    discriminant->Check(this);
    auto *discriminantType = GetNonConstantType(MaybeUnboxExpression(discriminant));
    if (!discriminantType->HasTypeFlag(TypeFlag::VALID_SWITCH_TYPE)) {
        if (!(discriminantType->IsETSObjectType() &&
              discriminantType->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::BUILTIN_STRING |
                                                                 ETSObjectFlags::STRING | ETSObjectFlags::ENUM))) {
            LogTypeError({"Incompatible types. Found: ", discriminantType,
                          ", required: char , byte , short , int, long , Char , Byte , Short , Int, Long , String "
                          "or an enum type"},
                         discriminant->Start());
        }
    }

    return discriminantType;
}

void ETSChecker::AddBoxingUnboxingFlagsToNode(ir::AstNode *node, Type *boxingUnboxingType)
{
    if (boxingUnboxingType->IsETSObjectType()) {
        node->AddBoxingUnboxingFlags(GetBoxingFlag(boxingUnboxingType));
    } else if (!boxingUnboxingType->IsETSUnionType()) {
        node->AddBoxingUnboxingFlags(GetUnboxingFlag(boxingUnboxingType));
    }
}

Type *ETSChecker::MaybeBoxExpression(ir::Expression *expr)
{
    auto *promoted = MaybeBoxType(expr->TsType());
    if (promoted != expr->TsType()) {
        expr->AddBoxingUnboxingFlags(GetBoxingFlag(promoted));
    }
    return promoted;
}

Type *ETSChecker::MaybeUnboxExpression(ir::Expression *expr)
{
    auto *primitive = MaybeUnboxType(expr->TsType());
    if (primitive != expr->TsType()) {
        expr->AddBoxingUnboxingFlags(GetUnboxingFlag(primitive));
    }
    return primitive;
}

void ETSChecker::CheckForSameSwitchCases(ArenaVector<ir::SwitchCaseStatement *> const &cases)
{
    CheckItemCasesConstant(cases);
    CheckItemCasesDuplicate(cases);
}

std::string ETSChecker::GetStringFromIdentifierValue(checker::Type *caseType) const
{
    const auto identifierTypeKind = ETSChecker::TypeKind(caseType);
    switch (identifierTypeKind) {
        case TypeFlag::BYTE: {
            return std::to_string(caseType->AsByteType()->GetValue());
        }
        case TypeFlag::SHORT: {
            return std::to_string(caseType->AsShortType()->GetValue());
        }
        case TypeFlag::CHAR: {
            return std::to_string(caseType->AsCharType()->GetValue());
        }
        case TypeFlag::INT: {
            return std::to_string(caseType->AsIntType()->GetValue());
        }
        case TypeFlag::LONG: {
            return std::to_string(caseType->AsLongType()->GetValue());
        }
        case TypeFlag::ETS_OBJECT: {
            VarBinder()->ThrowError(caseType->AsETSObjectType()->Variable()->Declaration()->Node()->Start(),
                                    "not implemented");
        }
        default: {
            UNREACHABLE();
        }
    }
}

bool IsConstantMemberOrIdentifierExpression(ir::Expression *expression)
{
    if (expression->IsMemberExpression()) {
        auto *var = expression->AsMemberExpression()->PropVar();
        return var->Declaration()->IsConstDecl() ||
               (var->Declaration()->IsReadonlyDecl() && var->HasFlag(varbinder::VariableFlags::STATIC));
    }

    if (expression->IsIdentifier()) {
        auto *var = expression->AsIdentifier()->Variable();
        return var->Declaration()->IsConstDecl() ||
               (var->Declaration()->IsReadonlyDecl() && var->HasFlag(varbinder::VariableFlags::STATIC));
    }

    return false;
}

void ETSChecker::CheckItemCasesConstant(ArenaVector<ir::SwitchCaseStatement *> const &cases)
{
    for (auto &it : cases) {
        auto *caseTest = it->Test();
        if (caseTest == nullptr) {
            continue;
        }
        auto *caseType = caseTest->TsType();
        if (caseTest->TsType()->IsETSEnumType()) {
            continue;
        }

        if (caseTest->IsIdentifier() || caseTest->IsMemberExpression()) {
            if (!IsConstantMemberOrIdentifierExpression(caseTest)) {
                LogTypeError("Constant expression required", it->Start());
                continue;
            }

            if (!caseType->HasTypeFlag(checker::TypeFlag::VALID_SWITCH_TYPE)) {
                LogTypeError("Unexpected type " + caseType->ToString(), it->Start());
                continue;
            }
            continue;
        }
    }
}

void CheckItemEnumType(ir::Expression const *const caseTest, ETSChecker *checker, ETSIntEnumType const *const type,
                       bool &isDup)
{
    if (caseTest->TsType()->AsETSIntEnumType()->IsSameEnumLiteralType(type)) {
        isDup = true;
        checker->LogTypeError("Case duplicate", caseTest->Start());
    }
}

void CheckItemStringEnumType(ir::Expression const *const caseTest, ETSChecker *checker,
                             ETSStringEnumType const *const type, bool &isDup)
{
    if (caseTest->TsType()->AsETSStringEnumType()->IsSameEnumLiteralType(type)) {
        isDup = true;
        checker->LogTypeError("Case duplicate", caseTest->Start());
    }
}

void ETSChecker::CheckItemCasesDuplicate(ArenaVector<ir::SwitchCaseStatement *> const &cases)
{
    for (size_t caseNum = 0; caseNum < cases.size(); caseNum++) {
        bool isItemDuplicate = false;
        for (size_t compareCase = caseNum + 1; compareCase < cases.size(); compareCase++) {
            auto *caseTest = cases.at(caseNum)->Test();
            auto *compareCaseTest = cases.at(compareCase)->Test();

            if (caseTest == nullptr || compareCaseTest == nullptr) {
                continue;
            }

            if (caseTest->TsType()->IsETSIntEnumType()) {
                CheckItemEnumType(caseTest, this, compareCaseTest->TsType()->AsETSIntEnumType(), isItemDuplicate);
                continue;
            }

            if (caseTest->TsType()->IsETSStringEnumType()) {
                CheckItemStringEnumType(caseTest, this, compareCaseTest->TsType()->AsETSStringEnumType(),
                                        isItemDuplicate);
                continue;
            }

            if (caseTest->IsIdentifier() || caseTest->IsMemberExpression()) {
                CheckIdentifierSwitchCase(caseTest, compareCaseTest, cases.at(caseNum)->Start());
                continue;
            }

            if (compareCaseTest->IsIdentifier() || compareCaseTest->IsMemberExpression()) {
                CheckIdentifierSwitchCase(compareCaseTest, caseTest, cases.at(compareCase)->Start());
                continue;
            }

            if (caseTest->IsLiteral() && compareCaseTest->IsLiteral() &&
                GetStringFromLiteral(caseTest) != GetStringFromLiteral(compareCaseTest)) {
                continue;
            }

            if (!(IsConstantExpression(caseTest, caseTest->TsType()) || caseTest->IsLiteral()) ||
                !(IsConstantExpression(compareCaseTest, compareCaseTest->TsType()) || compareCaseTest->IsLiteral())) {
                continue;
            }

            if (!isItemDuplicate) {
                isItemDuplicate = true;
                LogTypeError("Case duplicate", cases.at(compareCase)->Start());
            }
        }
    }
}

bool ETSChecker::CompareIdentifiersValuesAreDifferent(ir::Expression *compareValue, const std::string &caseValue)
{
    if (IsConstantMemberOrIdentifierExpression(compareValue)) {
        checker::Type *compareCaseType = compareValue->TsType();

        const auto compareCaseValue = GetStringFromIdentifierValue(compareCaseType);
        return caseValue != compareCaseValue;
    }

    return caseValue != GetStringFromLiteral(compareValue);
}

void ETSChecker::CheckIdentifierSwitchCase(ir::Expression *currentCase, ir::Expression *compareCase,
                                           const lexer::SourcePosition &pos)
{
    currentCase->Check(this);

    if (!IsConstantMemberOrIdentifierExpression(currentCase)) {
        return;
    }

    checker::Type *caseType = currentCase->TsType();

    if (!caseType->HasTypeFlag(checker::TypeFlag::VALID_SWITCH_TYPE)) {
        return;
    }

    if (!CompareIdentifiersValuesAreDifferent(compareCase, GetStringFromIdentifierValue(caseType))) {
        LogTypeError("Variable has same value with another switch case", pos);
        return;
    }
}

std::string ETSChecker::GetStringFromLiteral(ir::Expression *caseTest) const
{
    switch (caseTest->Type()) {
        case ir::AstNodeType::CHAR_LITERAL: {
            return std::to_string(caseTest->AsCharLiteral()->Char());
        }
        case ir::AstNodeType::STRING_LITERAL:
        case ir::AstNodeType::NUMBER_LITERAL: {
            return util::Helpers::LiteralToPropName(caseTest).Mutf8();
        }
        default:
            UNREACHABLE();
    }
}

bool ETSChecker::IsSameDeclarationType(varbinder::LocalVariable *target, varbinder::LocalVariable *compare)
{
    return target->Declaration()->Type() == compare->Declaration()->Type();
}

bool ETSChecker::CheckRethrowingParams(const ir::AstNode *ancestorFunction, const ir::AstNode *node)
{
    for (const auto param : ancestorFunction->AsScriptFunction()->Signature()->Function()->Params()) {
        if (node->AsCallExpression()->Callee()->AsIdentifier()->Name().Is(
                // CC-OFFNXT(G.FMT.06-CPP) project code style
                param->AsETSParameterExpression()->Ident()->Name().Mutf8())) {
            return true;
        }
    }
    return false;
}

void ETSChecker::CheckThrowingStatements(ir::AstNode *node)
{
    ir::AstNode *ancestorFunction = FindAncestorGivenByType(node, ir::AstNodeType::SCRIPT_FUNCTION);

    if (ancestorFunction == nullptr) {
        LogTypeError(
            "This statement can cause an exception, therefore it must be enclosed in a try statement with a default "
            "catch clause",
            node->Start());
        return;
    }

    if (ancestorFunction->AsScriptFunction()->IsThrowing() ||
        (ancestorFunction->AsScriptFunction()->IsRethrowing() &&
         (!node->IsThrowStatement() && CheckRethrowingParams(ancestorFunction, node)))) {
        return;
    }

    if (!CheckThrowingPlacement(node, ancestorFunction)) {
        if (ancestorFunction->AsScriptFunction()->IsRethrowing() && !node->IsThrowStatement()) {
            LogTypeError(
                "This statement can cause an exception, re-throwing functions can throw exception only by their "
                "parameters.",
                node->Start());
            return;
        }

        if (auto interfaces =
                ancestorFunction->AsScriptFunction()->Signature()->Owner()->AsETSObjectType()->Interfaces();
            !(!interfaces.empty() &&
              interfaces[0]->AsETSObjectType()->HasObjectFlag(checker::ETSObjectFlags::FUNCTIONAL_INTERFACE))) {
            LogTypeError(
                "This statement can cause an exception, therefore it must be enclosed in a try statement with a "
                "default "
                "catch clause",
                node->Start());
            return;
        }
    }
}

bool ETSChecker::CheckThrowingPlacement(ir::AstNode *node, const ir::AstNode *ancestorFunction)
{
    ir::AstNode *startPoint = node;
    ir::AstNode *enclosingCatchClause = nullptr;
    ir::BlockStatement *enclosingFinallyBlock = nullptr;
    ir::AstNode *p = startPoint->Parent();

    bool isHandled = false;
    const auto predicateFunc = [&enclosingCatchClause](ir::CatchClause *clause) {
        return clause == enclosingCatchClause;
    };

    do {
        if (p->IsTryStatement() && p->AsTryStatement()->HasDefaultCatchClause()) {
            enclosingCatchClause = FindAncestorGivenByType(startPoint, ir::AstNodeType::CATCH_CLAUSE, p);
            enclosingFinallyBlock = FindFinalizerOfTryStatement(startPoint, p);
            const auto catches = p->AsTryStatement()->CatchClauses();
            if (std::any_of(catches.begin(), catches.end(), predicateFunc)) {
                startPoint = enclosingCatchClause;
            } else if (enclosingFinallyBlock != nullptr &&
                       enclosingFinallyBlock == p->AsTryStatement()->FinallyBlock()) {
                startPoint = enclosingFinallyBlock;
            } else {
                isHandled = true;
                break;
            }
        }

        p = p->Parent();
    } while (p != ancestorFunction);

    return isHandled;
}

ir::BlockStatement *ETSChecker::FindFinalizerOfTryStatement(ir::AstNode *startFrom, const ir::AstNode *p)
{
    auto *iter = startFrom->Parent();

    do {
        if (iter->IsBlockStatement()) {
            ir::BlockStatement *finallyBlock = iter->AsBlockStatement();

            if (finallyBlock == p->AsTryStatement()->FinallyBlock()) {
                return finallyBlock;
            }
        }

        iter = iter->Parent();
    } while (iter != p);

    return nullptr;
}

void ETSChecker::CheckRethrowingFunction(ir::ScriptFunction *func)
{
    if (func->Signature()->Owner()->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::FUNCTIONAL_INTERFACE)) {
        return;
    }

    bool foundThrowingParam = false;

    // It doesn't support lambdas yet.
    for (auto item : func->Params()) {
        auto const *type = item->AsETSParameterExpression()->Ident()->TypeAnnotation();

        if (type->IsETSTypeReference()) {
            auto *typeDecl = type->AsETSTypeReference()->Part()->Name()->AsIdentifier()->Variable()->Declaration();
            if (typeDecl->IsTypeAliasDecl()) {
                type = typeDecl->Node()->AsTSTypeAliasDeclaration()->TypeAnnotation();
            }
        }

        if (type->IsETSFunctionType() && type->AsETSFunctionType()->IsThrowing()) {
            foundThrowingParam = true;
            break;
        }
    }

    if (!foundThrowingParam) {
        LogTypeError("A rethrowing function must have a throwing function parameter", func->Start());
    }
}

ETSObjectType *ETSChecker::GetRelevantArgumentedTypeFromChild(ETSObjectType *const child, ETSObjectType *const target)
{
    if (child->GetDeclNode() == target->GetDeclNode()) {
        auto *relevantType = CreateNewETSObjectType(child->Name(), child->GetDeclNode(), child->ObjectFlags());

        ArenaVector<Type *> params = child->TypeArguments();

        relevantType->SetTypeArguments(std::move(params));
        relevantType->SetEnclosingType(child->EnclosingType());
        relevantType->SetSuperType(child->SuperType());

        return relevantType;
    }

    ASSERT(child->SuperType() != nullptr);

    return GetRelevantArgumentedTypeFromChild(child->SuperType(), target);
}

void ETSChecker::EmplaceSubstituted(Substitution *substitution, ETSTypeParameter *tparam, Type *typeArg)
{
    // *only* reference type may be substituted, no exceptions
    ASSERT(typeArg->IsETSReferenceType());
    substitution->emplace(tparam, typeArg);
}

util::StringView ETSChecker::GetHashFromTypeArguments(const ArenaVector<Type *> &typeArgTypes)
{
    std::stringstream ss;

    for (auto *it : typeArgTypes) {
        it->ToString(ss, true);
        ss << compiler::Signatures::MANGLE_SEPARATOR;

        // In case of ETSTypeParameters storing the name might not be sufficient as there can
        // be multiple different type parameters with the same name. For those we test identity based
        // on their memory address equality, so we store them in the hash to keep it unique.
        // To make it consistent we store it for every type.
        // NOTE (mmartin): change bare address to something more appropriate unique representation
        ss << it << compiler::Signatures::MANGLE_SEPARATOR;
    }

    return util::UString(ss.str(), Allocator()).View();
}

util::StringView ETSChecker::GetHashFromSubstitution(const Substitution *substitution)
{
    std::vector<std::string> fields;
    for (auto [k, v] : *substitution) {
        std::stringstream ss;
        k->ToString(ss, true);
        ss << ":";
        v->ToString(ss, true);
        // NOTE (mmartin): change bare address to something more appropriate unique representation
        ss << ":" << k << ":" << v;
        fields.push_back(ss.str());
    }
    std::sort(fields.begin(), fields.end());

    std::stringstream ss;
    for (auto &fstr : fields) {
        ss << fstr;
        ss << ";";
    }
    return util::UString(ss.str(), Allocator()).View();
}

util::StringView ETSChecker::GetHashFromFunctionType(ir::ETSFunctionType *type)
{
    std::stringstream ss;
    for (auto *p : type->Params()) {
        auto *const param = p->AsETSParameterExpression();
        param->TypeAnnotation()->GetType(this)->ToString(ss, true);
        ss << ";";
    }

    type->ReturnType()->GetType(this)->ToString(ss, true);
    ss << ";";

    if (type->IsThrowing()) {
        ss << "throws;";
    }

    if (type->IsRethrowing()) {
        ss << "rethrows;";
    }

    return util::UString(ss.str(), Allocator()).View();
}

ETSObjectType *ETSChecker::GetOriginalBaseType(Type *const object)
{
    if (object == nullptr || !object->IsETSObjectType()) {
        return nullptr;
    }

    return object->AsETSObjectType()->GetOriginalBaseType();
}

void ETSChecker::CheckValidGenericTypeParameter(Type *const argType, const lexer::SourcePosition &pos)
{
    std::stringstream ss;
    argType->ToString(ss);
    LogTypeError("Type '" + ss.str() + "' is not valid for generic type arguments", pos);
}

bool ETSChecker::CheckNumberOfTypeArguments(ETSObjectType *const type, ir::TSTypeParameterInstantiation *const typeArgs,
                                            const lexer::SourcePosition &pos)
{
    auto const &typeParams = type->TypeArguments();
    if (typeParams.empty()) {
        if (typeArgs != nullptr) {
            LogTypeError({"Type '", type, "' is not generic."}, pos);
            return false;
        }
        return true;
    }

    size_t minimumTypeArgs = std::count_if(typeParams.begin(), typeParams.end(), [](Type *param) {
        return param->AsETSTypeParameter()->GetDefaultType() == nullptr;
    });
    if (typeArgs == nullptr && minimumTypeArgs > 0) {
        LogTypeError({"Type '", type, "' is generic but type argument were not provided."}, pos);
        return false;
    }

    if (typeArgs != nullptr &&
        ((minimumTypeArgs > typeArgs->Params().size()) || (typeParams.size() < typeArgs->Params().size()))) {
        LogTypeError({"Type '", type, "' has ", minimumTypeArgs, " number of type parameters, but ",
                      typeArgs->Params().size(), " type arguments were provided."},
                     pos);
        return false;
    }
    return true;
}

bool ETSChecker::NeedTypeInference(const ir::ScriptFunction *lambda)
{
    if (lambda->ReturnTypeAnnotation() == nullptr) {
        return true;
    }
    for (auto *const param : lambda->Params()) {
        const auto *const lambdaParam = param->AsETSParameterExpression()->Ident();
        if (lambdaParam->TypeAnnotation() == nullptr) {
            return true;
        }
    }
    return false;
}

std::vector<bool> ETSChecker::FindTypeInferenceArguments(const ArenaVector<ir::Expression *> &arguments)
{
    std::vector<bool> argTypeInferenceRequired(arguments.size());
    size_t index = 0;
    for (ir::Expression *arg : arguments) {
        if (arg->IsArrowFunctionExpression()) {
            ir::ScriptFunction *const lambda = arg->AsArrowFunctionExpression()->Function();
            if (NeedTypeInference(lambda)) {
                argTypeInferenceRequired[index] = true;
            }
        }
        ++index;
    }
    return argTypeInferenceRequired;
}

bool ETSChecker::CheckLambdaAssignableUnion(ir::AstNode *typeAnn, ir::ScriptFunction *lambda)
{
    for (auto *type : typeAnn->AsETSUnionType()->Types()) {
        if (type->IsETSFunctionType()) {
            return lambda->Params().size() == type->AsETSFunctionType()->Params().size();
        }
    }

    return false;
}

void ETSChecker::InferTypesForLambda(ir::ScriptFunction *lambda, ir::ETSFunctionType *calleeType,
                                     Signature *maybeSubstitutedFunctionSig)
{
    for (size_t i = 0; i < calleeType->Params().size(); ++i) {
        const auto *const calleeParam = calleeType->Params()[i]->AsETSParameterExpression()->Ident();
        auto *const lambdaParam = lambda->Params()[i]->AsETSParameterExpression()->Ident();
        if (lambdaParam->TypeAnnotation() == nullptr) {
            auto *const typeAnnotation = calleeParam->TypeAnnotation()->Clone(Allocator(), lambdaParam);
            if (maybeSubstitutedFunctionSig != nullptr) {
                ASSERT(maybeSubstitutedFunctionSig->Params().size() == calleeType->Params().size());
                typeAnnotation->SetTsType(maybeSubstitutedFunctionSig->Params()[i]->TsType());
            }
            lambdaParam->SetTsTypeAnnotation(typeAnnotation);
            typeAnnotation->SetParent(lambdaParam);
        }
    }
    if (lambda->ReturnTypeAnnotation() == nullptr) {
        auto *const returnTypeAnnotation = calleeType->ReturnType()->Clone(Allocator(), lambda);
        if (maybeSubstitutedFunctionSig != nullptr) {
            returnTypeAnnotation->SetTsType(maybeSubstitutedFunctionSig->ReturnType());
        }

        // Return type can be ETSFunctionType
        // Run varbinder to set scopes for cloned node
        compiler::InitScopesPhaseETS::RunExternalNode(returnTypeAnnotation, VarBinder());
        lambda->SetReturnTypeAnnotation(returnTypeAnnotation);
    }
}

void ETSChecker::ModifyPreferredType(ir::ArrayExpression *const arrayExpr, Type *const newPreferredType)
{
    // After modifying the preferred type of an array expression, it needs to be rechecked at the call site
    arrayExpr->SetPreferredType(newPreferredType);
    arrayExpr->SetTsType(nullptr);

    for (auto *const element : arrayExpr->Elements()) {
        if (element->IsArrayExpression()) {
            ModifyPreferredType(element->AsArrayExpression(), nullptr);
        }
    }
}

bool ETSChecker::IsInLocalClass(const ir::AstNode *node) const
{
    while (node != nullptr) {
        if (node->Type() == ir::AstNodeType::CLASS_DEFINITION) {
            return node->AsClassDefinition()->IsLocal();
        }
        node = node->Parent();
    }

    return false;
}

ir::Expression *ETSChecker::GenerateImplicitInstantiateArg(varbinder::LocalVariable *instantiateMethod,
                                                           const std::string &className)
{
    auto callSignatures = instantiateMethod->TsType()->AsETSFunctionType()->CallSignatures();
    ASSERT(!callSignatures.empty());
    auto methodOwner = std::string(callSignatures[0]->Owner()->Name());
    std::string implicitInstantiateArgument = "()=>{return new " + className + "()";
    if (methodOwner != className) {
        implicitInstantiateArgument.append(" as " + methodOwner);
    }
    implicitInstantiateArgument.append("}");

    parser::Program program(Allocator(), VarBinder());
    es2panda::CompilerOptions options;
    auto parser = parser::ETSParser(&program, options, parser::ParserStatus::NO_OPTS);
    auto *argExpr = parser.CreateExpression(implicitInstantiateArgument);
    compiler::InitScopesPhaseETS::RunExternalNode(argExpr, &program);

    return argExpr;
}

ir::ClassProperty *ETSChecker::ClassPropToImplementationProp(ir::ClassProperty *classProp, varbinder::ClassScope *scope)
{
    classProp->Key()->AsIdentifier()->SetName(
        util::UString(std::string(compiler::Signatures::PROPERTY) + classProp->Key()->AsIdentifier()->Name().Mutf8(),
                      Allocator())
            .View());
    classProp->AddModifier(ir::ModifierFlags::PRIVATE);

    auto *fieldDecl = Allocator()->New<varbinder::LetDecl>(classProp->Key()->AsIdentifier()->Name());
    fieldDecl->BindNode(classProp);

    auto fieldVar = scope->InstanceFieldScope()->AddDecl(Allocator(), fieldDecl, ScriptExtension::ETS);
    fieldVar->AddFlag(varbinder::VariableFlags::PROPERTY);

    classProp->Key()->SetVariable(fieldVar);
    classProp->Key()->AsIdentifier()->SetVariable(fieldVar);
    fieldVar->SetTsType(classProp->TsType());

    return classProp;
}

void ETSChecker::GenerateGetterSetterBody(ArenaVector<ir::Statement *> &stmts, ArenaVector<ir::Expression *> &params,
                                          ir::ClassProperty *const field, varbinder::FunctionParamScope *paramScope,
                                          bool isSetter)
{
    auto *classDef = field->Parent()->AsClassDefinition();

    ir::Expression *baseExpression;
    if ((field->Modifiers() & ir::ModifierFlags::SUPER_OWNER) != 0U) {
        baseExpression = Allocator()->New<ir::SuperExpression>();
    } else {
        baseExpression = Allocator()->New<ir::ThisExpression>();
    }
    baseExpression->SetParent(classDef);
    baseExpression->SetTsType(classDef->TsType());

    auto *memberExpression =
        AllocNode<ir::MemberExpression>(baseExpression, field->Key()->AsIdentifier()->Clone(Allocator(), nullptr),
                                        ir::MemberExpressionKind::PROPERTY_ACCESS, false, false);
    memberExpression->SetTsType(field->TsType());
    memberExpression->SetPropVar(field->Key()->Variable()->AsLocalVariable());
    memberExpression->SetRange(classDef->Range());
    if (memberExpression->ObjType() == nullptr && classDef->TsType() != nullptr) {
        memberExpression->SetObjectType(classDef->TsType()->AsETSObjectType());
    }

    if (!isSetter) {
        stmts.push_back(AllocNode<ir::ReturnStatement>(memberExpression));
        return;
    }

    auto *paramIdent = field->Key()->AsIdentifier()->Clone(Allocator(), nullptr);
    if (field->TypeAnnotation() != nullptr) {
        auto *const typeAnnotation = field->TypeAnnotation()->Clone(Allocator(), paramIdent);
        paramIdent->SetTsTypeAnnotation(typeAnnotation);
    } else {
        paramIdent->SetTsType(field->TsType());
    }

    auto *paramExpression = AllocNode<ir::ETSParameterExpression>(paramIdent, nullptr);
    paramExpression->SetRange(paramIdent->Range());
    auto *const paramVar = std::get<2>(paramScope->AddParamDecl(Allocator(), paramExpression));
    paramExpression->SetVariable(paramVar);

    params.push_back(paramExpression);

    auto *assignmentExpression = AllocNode<ir::AssignmentExpression>(
        memberExpression, paramExpression->Clone(Allocator(), nullptr), lexer::TokenType::PUNCTUATOR_SUBSTITUTION);
    assignmentExpression->SetTsType(paramVar->TsType());

    assignmentExpression->SetRange({field->Start(), field->End()});
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    stmts.push_back(AllocNode<ir::ExpressionStatement>(assignmentExpression));
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    stmts.push_back(Allocator()->New<ir::ReturnStatement>(nullptr));
}

ir::MethodDefinition *ETSChecker::GenerateDefaultGetterSetter(ir::ClassProperty *const property,
                                                              ir::ClassProperty *const field,
                                                              varbinder::ClassScope *classScope, bool isSetter,
                                                              ETSChecker *checker)
{
    auto *paramScope = checker->Allocator()->New<varbinder::FunctionParamScope>(checker->Allocator(), classScope);
    auto *functionScope = checker->Allocator()->New<varbinder::FunctionScope>(checker->Allocator(), paramScope);

    functionScope->BindParamScope(paramScope);
    paramScope->BindFunctionScope(functionScope);

    ArenaVector<ir::Expression *> params(checker->Allocator()->Adapter());
    ArenaVector<ir::Statement *> stmts(checker->Allocator()->Adapter());
    checker->GenerateGetterSetterBody(stmts, params, field, paramScope, isSetter);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *body = checker->AllocNode<ir::BlockStatement>(checker->Allocator(), std::move(stmts));
    auto funcFlags = isSetter ? ir::ScriptFunctionFlags::SETTER : ir::ScriptFunctionFlags::GETTER;
    auto *const returnTypeAnn = isSetter || field->TypeAnnotation() == nullptr
                                    ? nullptr
                                    : field->TypeAnnotation()->Clone(checker->Allocator(), nullptr);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *func = checker->AllocNode<ir::ScriptFunction>(
        checker->Allocator(),
        ir::ScriptFunction::ScriptFunctionData {body, ir::FunctionSignature(nullptr, std::move(params), returnTypeAnn),
                                                funcFlags, ir::ModifierFlags::PUBLIC});

    if (!isSetter) {
        func->AddFlag(ir::ScriptFunctionFlags::HAS_RETURN);
    }
    func->SetRange(field->Range());
    func->SetScope(functionScope);
    body->SetScope(functionScope);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *methodIdent = property->Key()->AsIdentifier()->Clone(checker->Allocator(), nullptr);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *funcExpr = checker->AllocNode<ir::FunctionExpression>(func);
    funcExpr->SetRange(func->Range());
    func->AddFlag(ir::ScriptFunctionFlags::METHOD);
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *method = checker->AllocNode<ir::MethodDefinition>(ir::MethodDefinitionKind::METHOD, methodIdent, funcExpr,
                                                            ir::ModifierFlags::PUBLIC, checker->Allocator(), false);

    auto *decl = checker->Allocator()->New<varbinder::FunctionDecl>(checker->Allocator(),
                                                                    property->Key()->AsIdentifier()->Name(), method);
    auto *var = checker->Allocator()->New<varbinder::LocalVariable>(decl, varbinder::VariableFlags::VAR);
    var->AddFlag(varbinder::VariableFlags::METHOD);

    methodIdent->SetVariable(var);

    method->Id()->SetMutator();
    method->SetRange(field->Range());
    method->Function()->SetIdent(method->Id()->Clone(checker->Allocator(), nullptr));
    method->Function()->AddModifier(method->Modifiers());
    method->SetVariable(var);
    method->SetParent(field->Parent());

    paramScope->BindNode(func);
    functionScope->BindNode(func);

    auto classCtx = varbinder::LexicalScope<varbinder::ClassScope>::Enter(checker->VarBinder(), classScope);
    checker->VarBinder()->AsETSBinder()->ResolveMethodDefinition(method);

    functionScope->BindName(classScope->Node()->AsClassDefinition()->InternalName());
    method->Check(checker);

    return method;
}

ir::ClassProperty *GetImplementationClassProp(ETSChecker *checker, ir::ClassProperty *interfaceProp,
                                              ir::ClassProperty *originalProp, ETSObjectType *classType)
{
    bool isSuperOwner = ((originalProp->Modifiers() & ir::ModifierFlags::SUPER_OWNER) != 0U);
    if (!isSuperOwner) {
        auto *const classDef = classType->GetDeclNode()->AsClassDefinition();
        auto *const scope = checker->Scope()->AsClassScope();
        auto *const classProp = checker->ClassPropToImplementationProp(
            interfaceProp->Clone(checker->Allocator(), originalProp->Parent()), scope);
        classType->AddProperty<PropertyType::INSTANCE_FIELD>(classProp->Key()->Variable()->AsLocalVariable());
        classDef->Body().push_back(classProp);
        return classProp;
    }

    auto *const classProp = classType
                                ->GetProperty(interfaceProp->Key()->AsIdentifier()->Name(),
                                              PropertySearchFlags::SEARCH_ALL | PropertySearchFlags::SEARCH_IN_BASE)
                                ->Declaration()
                                ->Node()
                                ->AsClassProperty();
    classProp->AddModifier(ir::ModifierFlags::SUPER_OWNER);
    return classProp;
}

static void SetupGetterSetterFlags(ir::ClassProperty *originalProp, ETSObjectType *classType,
                                   ir::MethodDefinition *getter, ir::MethodDefinition *setter, const bool inExternal)
{
    auto *const classDef = classType->GetDeclNode()->AsClassDefinition();
    for (auto &method : {getter, setter}) {
        if (method == nullptr) {
            continue;
        }

        const auto mflag = method == getter ? ir::ModifierFlags::GETTER : ir::ModifierFlags::SETTER;
        const auto tflag = method == getter ? TypeFlag::GETTER : TypeFlag::SETTER;

        method->TsType()->AddTypeFlag(tflag);
        method->Variable()->SetTsType(method->TsType());
        if (((originalProp->Modifiers() & mflag) != 0U)) {
            method->Function()->AddModifier(ir::ModifierFlags::OVERRIDE);
        }

        if (inExternal) {
            method->Function()->AddFlag(ir::ScriptFunctionFlags::EXTERNAL);
        }
        method->SetParent(classDef);
        classType->AddProperty<checker::PropertyType::INSTANCE_METHOD>(method->Variable()->AsLocalVariable());
    }
}

void ETSChecker::GenerateGetterSetterPropertyAndMethod(ir::ClassProperty *originalProp, ETSObjectType *classType)
{
    auto *const classDef = classType->GetDeclNode()->AsClassDefinition();
    auto *interfaceProp = originalProp->Clone(Allocator(), originalProp->Parent());
    interfaceProp->ClearModifier(ir::ModifierFlags::GETTER_SETTER);

    auto *const scope = Scope()->AsClassScope();
    scope->InstanceFieldScope()->EraseBinding(interfaceProp->Key()->AsIdentifier()->Name());
    interfaceProp->SetRange(originalProp->Range());

    auto *const classProp = GetImplementationClassProp(this, interfaceProp, originalProp, classType);

    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    ir::MethodDefinition *getter = GenerateDefaultGetterSetter(interfaceProp, classProp, scope, false, this);
    classDef->Body().push_back(getter);

    const auto &name = getter->Key()->AsIdentifier()->Name();

    ir::MethodDefinition *setter =
        !classProp->IsConst()
            ? GenerateDefaultGetterSetter(interfaceProp, classProp, Scope()->AsClassScope(), true, this)
            : nullptr;

    auto *const methodScope = scope->InstanceMethodScope();
    auto *const decl = Allocator()->New<varbinder::FunctionDecl>(Allocator(), name, getter);
    auto *var = methodScope->AddDecl(Allocator(), decl, ScriptExtension::ETS);

    if (var == nullptr) {
        auto *const prevDecl = methodScope->FindDecl(name);
        for (const auto &method : {getter, setter}) {
            if (method != nullptr) {
                prevDecl->Node()->AsMethodDefinition()->AddOverload(method);
            }
        }
        var = methodScope->FindLocal(name, varbinder::ResolveBindingOptions::BINDINGS);
    }
    var->AddFlag(varbinder::VariableFlags::METHOD);

    SetupGetterSetterFlags(originalProp, classType, getter, setter, HasStatus(CheckerStatus::IN_EXTERNAL));

    if (setter != nullptr) {
        getter->Variable()->TsType()->AsETSFunctionType()->AddCallSignature(
            setter->TsType()->AsETSFunctionType()->CallSignatures()[0]);
    }

    getter->Function()->Id()->SetVariable(var);
    if (setter != nullptr) {
        getter->AddOverload(setter);
    }
}

Type *ETSChecker::TryGettingFunctionTypeFromInvokeFunction(Type *type)
{
    if (type->IsETSObjectType() && type->AsETSObjectType()->HasObjectFlag(ETSObjectFlags::FUNCTIONAL)) {
        auto const propInvoke = type->AsETSObjectType()->GetProperty(FUNCTIONAL_INTERFACE_INVOKE_METHOD_NAME,
                                                                     PropertySearchFlags::SEARCH_INSTANCE_METHOD);
        ASSERT(propInvoke != nullptr);

        return propInvoke->TsType();
    }

    return type;
}

bool ETSChecker::TryTransformingToStaticInvoke(ir::Identifier *const ident, const Type *resolvedType)
{
    ASSERT(ident->Parent()->IsCallExpression());
    ASSERT(ident->Parent()->AsCallExpression()->Callee() == ident);

    if (!resolvedType->IsETSObjectType()) {
        return false;
    }

    auto className = ident->Name();
    std::string_view propertyName;

    PropertySearchFlags searchFlag = PropertySearchFlags::SEARCH_IN_INTERFACES | PropertySearchFlags::SEARCH_IN_BASE |
                                     PropertySearchFlags::SEARCH_STATIC_METHOD;
    auto *instantiateMethod =
        resolvedType->AsETSObjectType()->GetProperty(compiler::Signatures::STATIC_INSTANTIATE_METHOD, searchFlag);
    auto *invokeMethod =
        resolvedType->AsETSObjectType()->GetProperty(compiler::Signatures::STATIC_INVOKE_METHOD, searchFlag);
    if (instantiateMethod != nullptr) {
        propertyName = compiler::Signatures::STATIC_INSTANTIATE_METHOD;
    } else if (invokeMethod != nullptr) {
        propertyName = compiler::Signatures::STATIC_INVOKE_METHOD;
    } else {
        LogTypeError({"No static ", compiler::Signatures::STATIC_INVOKE_METHOD, " method and static ",
                      compiler::Signatures::STATIC_INSTANTIATE_METHOD, " method in ", className, ". ", className,
                      "() is not allowed."},
                     ident->Start());
        return true;
    }
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *classId = AllocNode<ir::Identifier>(className, Allocator());
    // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
    auto *methodId = AllocNode<ir::Identifier>(propertyName, Allocator());
    if (propertyName == compiler::Signatures::STATIC_INSTANTIATE_METHOD) {
        methodId->SetVariable(instantiateMethod);
    } else if (propertyName == compiler::Signatures::STATIC_INVOKE_METHOD) {
        methodId->SetVariable(invokeMethod);
    }

    auto *transformedCallee =
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        AllocNode<ir::MemberExpression>(classId, methodId, ir::MemberExpressionKind::PROPERTY_ACCESS, false, false);

    classId->SetRange(ident->Range());
    methodId->SetRange(ident->Range());
    transformedCallee->SetRange(ident->Range());

    auto *callExpr = ident->Parent()->AsCallExpression();
    transformedCallee->SetParent(callExpr);
    callExpr->SetCallee(transformedCallee);

    if (instantiateMethod != nullptr) {
        // SUPPRESS_CSA_NEXTLINE(alpha.core.AllocatorETSCheckerHint)
        auto *argExpr = GenerateImplicitInstantiateArg(instantiateMethod, std::string(className));

        argExpr->SetParent(callExpr);
        argExpr->SetRange(ident->Range());

        VarBinder()->AsETSBinder()->HandleCustomNodes(argExpr);

        auto &arguments = callExpr->Arguments();
        arguments.insert(arguments.begin(), argExpr);
    }

    return true;
}

checker::ETSObjectType *ETSChecker::CreateSyntheticType(util::StringView const &syntheticName,
                                                        checker::ETSObjectType *lastObjectType, ir::Identifier *id)
{
    auto *syntheticObjType = Allocator()->New<checker::ETSObjectType>(Allocator(), syntheticName, syntheticName, id,
                                                                      checker::ETSObjectFlags::NO_OPTS);

    auto *classDecl = Allocator()->New<varbinder::ClassDecl>(syntheticName);
    varbinder::LocalVariable *var =
        Allocator()->New<varbinder::LocalVariable>(classDecl, varbinder::VariableFlags::CLASS);
    var->SetTsType(syntheticObjType);
    lastObjectType->AddProperty<checker::PropertyType::STATIC_FIELD>(var);
    syntheticObjType->SetEnclosingType(lastObjectType);
    return syntheticObjType;
}

void ETSChecker::ImportNamespaceObjectTypeAddReExportType(ir::ETSImportDeclaration *importDecl,
                                                          checker::ETSObjectType *lastObjectType, ir::Identifier *ident)
{
    for (auto item : VarBinder()->AsETSBinder()->ReExportImports()) {
        if (!importDecl->ResolvedSource()->Str().Is(item->GetProgramPath().Mutf8())) {
            continue;
        }
        auto *reExportType = GetImportSpecifierObjectType(item->GetETSImportDeclarations(), ident);
        lastObjectType->AddReExports(reExportType);
        for (auto node : importDecl->Specifiers()) {
            if (node->IsImportSpecifier()) {
                auto specifier = node->AsImportSpecifier();
                lastObjectType->AddReExportAlias(specifier->Imported()->Name(), specifier->Local()->Name());
            }
        }
    }
}

ETSObjectType *ETSChecker::GetImportSpecifierObjectType(ir::ETSImportDeclaration *importDecl, ir::Identifier *ident)
{
    auto importPath = importDecl->ResolvedSource()->Str();
    parser::Program *program =
        SelectEntryOrExternalProgram(static_cast<varbinder::ETSBinder *>(VarBinder()), importPath);
    std::vector<util::StringView> syntheticNames = GetNameForSynteticObjectType(program->ModuleName());
    ASSERT(!syntheticNames.empty());
    auto assemblerName = syntheticNames[0];

    if (!program->OmitModuleName()) {
        assemblerName = util::UString(assemblerName.Mutf8()
                                          .append(compiler::Signatures::METHOD_SEPARATOR)
                                          .append(compiler::Signatures::ETS_GLOBAL),
                                      Allocator())
                            .View();
    }

    auto *moduleObjectType =
        Allocator()->New<checker::ETSObjectType>(Allocator(), syntheticNames[0], assemblerName,
                                                 std::make_tuple(ident, checker::ETSObjectFlags::CLASS, Relation()));

    auto *rootDecl = Allocator()->New<varbinder::ClassDecl>(syntheticNames[0]);
    varbinder::LocalVariable *rootVar =
        Allocator()->New<varbinder::LocalVariable>(rootDecl, varbinder::VariableFlags::NONE);
    rootVar->SetTsType(moduleObjectType);

    syntheticNames.erase(syntheticNames.begin());
    checker::ETSObjectType *lastObjectType(moduleObjectType);

    for (const auto &syntheticName : syntheticNames) {
        lastObjectType = CreateSyntheticType(syntheticName, lastObjectType, ident);
    }

    ImportNamespaceObjectTypeAddReExportType(importDecl, lastObjectType, ident);
    SetPropertiesForModuleObject(lastObjectType, importPath,
                                 importDecl->Specifiers()[0]->IsImportNamespaceSpecifier() ? nullptr : importDecl);
    SetrModuleObjectTsType(ident, lastObjectType);

    return moduleObjectType;
}
}  // namespace ark::es2panda::checker
