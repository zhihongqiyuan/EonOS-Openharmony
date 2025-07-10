/*
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

#include "typeRelationContext.h"
#include "boxingConverter.h"
#include "macros.h"
#include "varbinder/scope.h"
#include "varbinder/variable.h"
#include "varbinder/declaration.h"
#include "checker/types/ets/etsUnionType.h"
#include "ir/expressions/arrayExpression.h"
#include "ir/ts/tsTypeParameter.h"

namespace ark::es2panda::checker {
bool AssignmentContext::ValidateArrayTypeInitializerByElement(TypeRelation *relation, ir::ArrayExpression *node,
                                                              ETSArrayType *target)
{
    bool ok = true;
    if (target->IsETSTupleType()) {
        return true;
    }

    for (uint32_t index = 0; index < node->Elements().size(); index++) {
        ir::Expression *currentArrayElem = node->Elements()[index];
        auto *const currentArrayElementType = currentArrayElem->Check(relation->GetChecker()->AsETSChecker());

        if (!AssignmentContext(relation, currentArrayElem,
                               currentArrayElem->Check(relation->GetChecker()->AsETSChecker()), target->ElementType(),
                               currentArrayElem->Start(), {}, TypeRelationFlag::NO_THROW)
                 // CC-OFFNXT(G.FMT.06-CPP,G.FMT.02-CPP) project code style
                 .IsAssignable()) {
            relation->GetChecker()->LogTypeError(
                {"Array element at index ", index, " with type '", currentArrayElementType,
                 "' is not compatible with the target array element type '", target->ElementType(), "'"},
                currentArrayElem->Start());
            ok = false;
        }
    }
    return ok;
}

bool InstantiationContext::ValidateTypeArguments(ETSObjectType *type, ir::TSTypeParameterInstantiation *typeArgs,
                                                 const lexer::SourcePosition &pos)
{
    if (checker_->HasStatus(CheckerStatus::IN_INSTANCEOF_CONTEXT)) {
        if (typeArgs != nullptr) {
            checker_->ReportWarning(
                {"Type parameter is erased from type '", type->Name(), "' when used in instanceof expression."}, pos);
        }
        result_ = type;
        return true;
    }
    if (!checker_->CheckNumberOfTypeArguments(type, typeArgs, pos)) {
        result_ = checker_->GlobalTypeError();
        return true;
        // the return value 'true' of this function let Instantiationcontext constructor return immediately.
    }
    if (type->TypeArguments().empty()) {
        result_ = type;
        return true;
    }
    return false;
}

void InstantiationContext::InstantiateType(ETSObjectType *type, ir::TSTypeParameterInstantiation *typeArgs)
{
    ArenaVector<Type *> typeArgTypes(checker_->Allocator()->Adapter());
    typeArgTypes.reserve(type->TypeArguments().size());

    if (typeArgs != nullptr) {
        for (auto *const it : typeArgs->Params()) {
            auto *paramType = it->GetType(checker_);
            if (paramType->IsTypeError()) {
                result_ = paramType;
                return;
            }

            if (paramType->IsETSPrimitiveType()) {
                checker_->Relation()->SetNode(it);

                auto *const boxedTypeArg = checker_->MaybeBoxInRelation(paramType);
                ASSERT(boxedTypeArg);
                paramType = boxedTypeArg->Instantiate(checker_->Allocator(), checker_->Relation(),
                                                      checker_->GetGlobalTypesHolder());
            }

            typeArgTypes.push_back(paramType);
        }
    }

    while (typeArgTypes.size() < type->TypeArguments().size()) {
        auto *defaultType = type->TypeArguments().at(typeArgTypes.size())->AsETSTypeParameter()->GetDefaultType();
        typeArgTypes.push_back(defaultType);
    }

    auto pos = (typeArgs == nullptr) ? lexer::SourcePosition() : typeArgs->Range().start;
    InstantiateType(type, std::move(typeArgTypes), pos);
    ASSERT(result_->IsETSObjectType());
    result_->AsETSObjectType()->AddObjectFlag(ETSObjectFlags::NO_OPTS);
}

static void CheckInstantiationConstraints(ETSChecker *checker, ArenaVector<Type *> const &typeParams,
                                          const Substitution *substitution, lexer::SourcePosition pos)
{
    auto relation = checker->Relation();

    for (auto type : typeParams) {
        if (!type->IsETSTypeParameter()) {
            continue;
        }
        auto typeParam = type->AsETSTypeParameter();
        auto typeArg = typeParam->Substitute(relation, substitution);
        if (typeArg->IsWildcardType()) {
            continue;
        }
        if (typeArg->IsTypeError()) {
            continue;
        }
        // NOTE(vpukhov): #19701 void refactoring
        ASSERT(typeArg->IsETSReferenceType() || typeArg->IsETSVoidType());
        auto constraint = typeParam->GetConstraintType()->Substitute(relation, substitution);
        if (!relation->IsAssignableTo(typeArg, constraint)) {
            checker->LogTypeError(  // NOTE(vpukhov): refine message
                {"Type ", typeArg, " is not assignable to", " constraint type ", constraint}, pos);
        }
    }
}

void ConstraintCheckScope::TryCheckConstraints()
{
    if (Unlock()) {
        auto &records = checker_->PendingConstraintCheckRecords();
        for (auto const &[typeParams, substitution, pos] : records) {
            CheckInstantiationConstraints(checker_, *typeParams, substitution, pos);
        }
        records.clear();
    }
}

void InstantiationContext::InstantiateType(ETSObjectType *type, ArenaVector<Type *> &&typeArgTypes,
                                           const lexer::SourcePosition &pos)
{
    util::StringView hash = checker_->GetHashFromTypeArguments(typeArgTypes);
    auto const &typeParams = type->TypeArguments();

    while (typeArgTypes.size() < typeParams.size()) {
        typeArgTypes.push_back(typeParams.at(typeArgTypes.size()));
    }

    auto *substitution = checker_->NewSubstitution();
    for (size_t idx = 0; idx < typeParams.size(); idx++) {
        if (!typeParams[idx]->IsETSTypeParameter()) {
            continue;
        }
        ETSChecker::EmplaceSubstituted(substitution, typeParams[idx]->AsETSTypeParameter(), typeArgTypes[idx]);
    }

    ConstraintCheckScope ctScope(checker_);
    if (!checker_->Relation()->NoThrowGenericTypeAlias()) {
        checker_->PendingConstraintCheckRecords().push_back({&typeParams, substitution, pos});
    }

    result_ = type->Substitute(checker_->Relation(), substitution)->AsETSObjectType();
    type->GetInstantiationMap().try_emplace(hash, result_->AsETSObjectType());
    result_->AddTypeFlag(TypeFlag::GENERIC);

    ctScope.TryCheckConstraints();
}

}  // namespace ark::es2panda::checker
