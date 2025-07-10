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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_TYPE_RELATION_CONTEXT_H
#define ES2PANDA_COMPILER_CHECKER_ETS_TYPE_RELATION_CONTEXT_H

#include "checker/ETSchecker.h"

namespace ark::es2panda::checker {
class ETSChecker;

class AssignmentContext {
public:
    // CC-OFFNXT(G.FUN.01-CPP) solid logic
    AssignmentContext(TypeRelation *relation, ir::Expression *node, Type *source, Type *target,
                      const lexer::SourcePosition &pos, std::initializer_list<TypeErrorMessageElement> list,
                      TypeRelationFlag flags = TypeRelationFlag::NONE)
    {
        flags_ |= ((flags & TypeRelationFlag::NO_BOXING) != 0) ? TypeRelationFlag::NONE : TypeRelationFlag::BOXING;
        flags_ |= ((flags & TypeRelationFlag::NO_UNBOXING) != 0) ? TypeRelationFlag::NONE : TypeRelationFlag::UNBOXING;
        flags_ |= ((flags & TypeRelationFlag::NO_WIDENING) != 0) ? TypeRelationFlag::NONE : TypeRelationFlag::WIDENING;

        auto *const etsChecker = relation->GetChecker()->AsETSChecker();

        if (target->IsETSArrayType() && node->IsArrayExpression()) {
            assignable_ =
                ValidateArrayTypeInitializerByElement(relation, node->AsArrayExpression(), target->AsETSArrayType());
            return;
        }
        // CC-OFFNXT(G.FMT.02) project code style
        flags_ |= flags;
        relation->SetNode(node);

        // NOTE (oeotvos) The narrowing flag will be applied here. It means, that the result of "let tmp: int = 1.5"
        // will be 1, which could cause problems.
        if (source->HasTypeFlag(TypeFlag::CONSTANT)) {
            flags_ |= TypeRelationFlag::NARROWING;
        }

        relation->SetFlags(flags_);

        if (!relation->IsAssignableTo(source, target)) {
            if (((flags_ & TypeRelationFlag::UNBOXING) != 0) && !relation->IsTrue() && source->IsETSObjectType() &&
                !target->IsETSObjectType()) {
                etsChecker->CheckUnboxedTypesAssignable(relation, source, target);
            }
            if (((flags_ & TypeRelationFlag::BOXING) != 0) && target->IsETSObjectType() && !relation->IsTrue()) {
                etsChecker->CheckBoxedSourceTypeAssignable(relation, source, target);
            }
        }

        if (!relation->IsTrue() && (flags_ & TypeRelationFlag::NO_THROW) == 0) {
            relation->RaiseError(list, pos);
        }

        relation->SetNode(nullptr);
        relation->SetFlags(TypeRelationFlag::NONE);
        assignable_ = relation->IsTrue();
    }

    bool IsAssignable() const
    {
        return assignable_;
    }

    bool ValidateArrayTypeInitializerByElement(TypeRelation *relation, ir::ArrayExpression *node, ETSArrayType *target);

private:
    TypeRelationFlag flags_ = TypeRelationFlag::IN_ASSIGNMENT_CONTEXT;
    bool assignable_ {false};
};

class InvocationContext {
public:
    // CC-OFFNXT(G.FUN.01-CPP) solid logic
    InvocationContext(TypeRelation *relation, ir::Expression *node, Type *source, Type *target,
                      const lexer::SourcePosition &pos, std::initializer_list<TypeErrorMessageElement> list,
                      TypeRelationFlag initialFlags = TypeRelationFlag::NONE)
    {
        flags_ |=
            ((initialFlags & TypeRelationFlag::NO_BOXING) != 0) ? TypeRelationFlag::NONE : TypeRelationFlag::BOXING;
        flags_ |=
            ((initialFlags & TypeRelationFlag::NO_UNBOXING) != 0) ? TypeRelationFlag::NONE : TypeRelationFlag::UNBOXING;

        auto *const etsChecker = relation->GetChecker()->AsETSChecker();

        relation->SetNode(node);
        relation->SetFlags(flags_ | initialFlags);

        if (!relation->IsAssignableTo(source, target)) {
            if (((flags_ & TypeRelationFlag::UNBOXING) != 0U) && !relation->IsTrue() && source->IsETSObjectType() &&
                !target->IsETSObjectType()) {
                etsChecker->CheckUnboxedSourceTypeWithWideningAssignable(relation, source, target);
            }
            if (((flags_ & TypeRelationFlag::BOXING) != 0) && target->IsETSObjectType() && !relation->IsTrue()) {
                etsChecker->CheckBoxedSourceTypeAssignable(relation, source, target);
            }
        }

        relation->SetNode(nullptr);
        relation->SetFlags(TypeRelationFlag::NONE);

        if (!relation->IsTrue()) {
            if ((initialFlags & TypeRelationFlag::NO_THROW) == 0) {
                relation->RaiseError(list, pos);
                relation->Result(RelationResult::ERROR);
                invocable_ = false;
            }
            return;
        }

        invocable_ = true;
    }

    bool IsInvocable() const
    {
        return invocable_;
    }

private:
    TypeRelationFlag flags_ = TypeRelationFlag::NONE;
    bool invocable_ {false};
};

class ConstraintCheckScope {
public:
    explicit ConstraintCheckScope(ETSChecker *checker) : checker_(checker), isheld_(true)
    {
        size_t &counter = checker_->ConstraintCheckScopesCount();
        ASSERT(counter != 0 || checker_->PendingConstraintCheckRecords().empty());
        counter++;
    }

    ~ConstraintCheckScope()
    {
        if (isheld_) {
            Unlock();
        }
    }

    void TryCheckConstraints();

    NO_COPY_SEMANTIC(ConstraintCheckScope);
    NO_MOVE_SEMANTIC(ConstraintCheckScope);

private:
    bool Unlock()
    {
        ASSERT(isheld_);
        isheld_ = false;
        return --checker_->ConstraintCheckScopesCount() == 0;
    }

    ETSChecker *checker_;
    bool isheld_ {};
};

class InstantiationContext {
public:
    InstantiationContext(ETSChecker *checker, ETSObjectType *type, ir::TSTypeParameterInstantiation *typeArgs,
                         const lexer::SourcePosition &pos)
        : checker_(checker)
    {
        if (ValidateTypeArguments(type, typeArgs, pos)) {
            return;
        }
        InstantiateType(type, typeArgs);
    }

    InstantiationContext(ETSChecker *checker, ETSObjectType *type, ArenaVector<Type *> &&typeArgs,
                         const lexer::SourcePosition &pos)
        : checker_(checker)
    {
        if (type->HasObjectFlag(ETSObjectFlags::ENUM)) {
            return;
        }
        InstantiateType(type, std::move(typeArgs), pos);
    }

    Type *Result()
    {
        return result_;
    }

private:
    bool ValidateTypeArguments(ETSObjectType *type, ir::TSTypeParameterInstantiation *typeArgs,
                               const lexer::SourcePosition &pos);

    void InstantiateType(ETSObjectType *type, ir::TSTypeParameterInstantiation *typeArgs);

    void InstantiateType(ETSObjectType *type, ArenaVector<Type *> &&typeArgTypes, const lexer::SourcePosition &pos);
    util::StringView GetHashFromTypeArguments(ArenaVector<Type *> &typeArgTypes);

    ETSChecker *checker_;
    Type *result_ {};
};

}  // namespace ark::es2panda::checker

#endif
