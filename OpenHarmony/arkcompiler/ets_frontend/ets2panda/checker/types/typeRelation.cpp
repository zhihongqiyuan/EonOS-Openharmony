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

#include "typeRelation.h"

#include "checker/checker.h"
#include "checker/types/ts/indexInfo.h"
#include "checker/types/signature.h"

namespace ark::es2panda::checker {
ArenaAllocator *TypeRelation::Allocator()
{
    return checker_->Allocator();
}

RelationResult TypeRelation::CacheLookup(const Type *source, const Type *target, const RelationHolder &holder,
                                         RelationType type) const
{
    if (result_ == RelationResult::CACHE_MISS) {
        return result_;
    }

    RelationKey relationKey {source->Id(), target->Id()};
    auto res = holder.cached.find(relationKey);
    if (res == holder.cached.end()) {
        return RelationResult::CACHE_MISS;
    }

    if (res->second.type >= type && res->second.result == RelationResult::TRUE) {
        return RelationResult::TRUE;
    }

    if (res->second.type <= type && res->second.result == RelationResult::FALSE) {
        return RelationResult::FALSE;
    }

    return RelationResult::CACHE_MISS;
}

bool TypeRelation::IsIdenticalTo(Type *source, Type *target)
{
    if (source == nullptr || target == nullptr) {
        return Result(false);
    }

    if (source == target) {
        return Result(true);
    }

    result_ = CacheLookup(source, target, checker_->IdenticalResults(), RelationType::IDENTICAL);
    if (result_ == RelationResult::CACHE_MISS) {
        checker_->ResolveStructuredTypeMembers(source);
        checker_->ResolveStructuredTypeMembers(target);
        result_ = RelationResult::FALSE;
        target->Identical(this, source);
        checker_->IdenticalResults().cached.insert({{source->Id(), target->Id()}, {result_, RelationType::IDENTICAL}});
    }

    return IsTrue();
}

bool TypeRelation::IsCompatibleTo(Signature *source, Signature *target)
{
    if (source == target) {
        return Result(true);
    }

    result_ = RelationResult::FALSE;
    target->Compatible(this, source);

    return result_ == RelationResult::TRUE;
}

bool TypeRelation::IsIdenticalTo(IndexInfo *source, IndexInfo *target)
{
    if (source == target) {
        Result(true);
        return true;
    }

    result_ = RelationResult::FALSE;
    target->Identical(this, source);

    return result_ == RelationResult::TRUE;
}

// NOTE: applyNarrowing -> flag
bool TypeRelation::IsAssignableTo(Type *source, Type *target)
{
    result_ = CacheLookup(source, target, checker_->AssignableResults(), RelationType::ASSIGNABLE);
    if (result_ == RelationResult::CACHE_MISS) {
        // NOTE: we support assigning T to Readonly<T>, but do not support assigning Readonly<T> to T
        // more details in spec
        if (source->HasTypeFlag(TypeFlag::READONLY) && !target->HasTypeFlag(TypeFlag::READONLY)) {
            result_ = RelationResult::FALSE;
        }

        if (result_ != RelationResult::FALSE && IsIdenticalTo(source, target)) {
            return true;
        }

        if (result_ == RelationResult::ERROR) {
            // Return early to prevent logging same errors again
            return false;
        }

        result_ = RelationResult::FALSE;

        if (!source->AssignmentSource(this, target)) {
            target->AssignmentTarget(this, source);
        }

        if (flags_ == TypeRelationFlag::NONE) {
            checker_->AssignableResults().cached.insert(
                {{source->Id(), target->Id()}, {result_, RelationType::ASSIGNABLE}});
        }
    }

    return result_ == RelationResult::TRUE;
}

bool TypeRelation::IsComparableTo(Type *source, Type *target)
{
    result_ = CacheLookup(source, target, checker_->ComparableResults(), RelationType::COMPARABLE);

    // NOTE: vpukhov. reimplement dynamic comparison and remove this check
    if (source->IsETSDynamicType() || target->IsETSDynamicType()) {
        if (!(source->IsETSDynamicType() && target->IsETSDynamicType())) {
            return false;
        }
    }

    if (result_ == RelationResult::CACHE_MISS) {
        if (IsAssignableTo(source, target)) {
            return true;
        }

        result_ = RelationResult::FALSE;
        target->Compare(this, source);
        checker_->ComparableResults().cached.insert(
            {{source->Id(), target->Id()}, {result_, RelationType::COMPARABLE}});
    }

    return result_ == RelationResult::TRUE;
}

bool TypeRelation::IsCastableTo(Type *const source, Type *const target)
{
    result_ = CacheLookup(source, target, checker_->UncheckedCastableResult(), RelationType::UNCHECKED_CASTABLE);
    if (result_ == RelationResult::CACHE_MISS) {
        result_ = RelationResult::FALSE;
        flags_ |= TypeRelationFlag::UNCHECKED_CAST;

        source->Cast(this, target);
        if (!IsTrue()) {
            target->CastTarget(this, source);
        }

        if (!IsTrue()) {
            return false;
        }

        // NOTE: Can't cache if the node has BoxingUnboxingFlags. These flags should be stored and restored on the node
        // on cache hit.
        if (UncheckedCast() && node_->GetBoxingUnboxingFlags() == ir::BoxingUnboxingFlags::NONE) {
            checker_->UncheckedCastableResult().cached.insert(
                {{source->Id(), target->Id()}, {result_, RelationType::UNCHECKED_CASTABLE}});
        }

        return true;
    }

    return result_ == RelationResult::TRUE;
}

bool TypeRelation::IsSupertypeOf(Type *super, Type *sub)
{
    result_ = CacheLookup(super, sub, checker_->SupertypeResults(), RelationType::SUPERTYPE);
    if (result_ == RelationResult::CACHE_MISS) {
        if (IsIdenticalTo(super, sub)) {
            return true;
        }

        result_ = RelationResult::FALSE;

        if (super->IsSupertypeOf(this, sub), !IsTrue()) {
            sub->IsSubtypeOf(this, super);
        }

        if (flags_ == TypeRelationFlag::NONE) {
            checker_->SupertypeResults().cached.insert({{super->Id(), sub->Id()}, {result_, RelationType::SUPERTYPE}});
        }
    }

    return result_ == RelationResult::TRUE;
}

void TypeRelation::RaiseError(const std::string &errMsg, const lexer::SourcePosition &loc) const
{
    checker_->LogTypeError(errMsg, loc);
}

void TypeRelation::RaiseError(std::initializer_list<TypeErrorMessageElement> list,
                              const lexer::SourcePosition &loc) const
{
    checker_->LogTypeError(list, loc);
}
}  // namespace ark::es2panda::checker
