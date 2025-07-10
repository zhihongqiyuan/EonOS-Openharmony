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

#ifndef ES2PANDA_COMPILER_CHECKER_TYPES_TYPE_RELATION_H
#define ES2PANDA_COMPILER_CHECKER_TYPES_TYPE_RELATION_H

#include "lexer/token/sourceLocation.h"
#include "lexer/token/tokenType.h"
#include "util/ustring.h"
#include "util/enumbitops.h"

namespace ark::es2panda::ir {
class Expression;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::checker {
class Signature;
class IndexInfo;
class Type;
class Checker;

using ENUMBITOPS_OPERATORS;

enum class TypeRelationFlag : uint32_t {
    NONE = 0U,
    NARROWING = 1U << 0U,
    WIDENING = 1U << 1U,
    BOXING = 1U << 2U,
    UNBOXING = 1U << 3U,
    CAPTURE = 1U << 4U,
    STRING = 1U << 5U,
    VALUE_SET = 1U << 6U,
    UNCHECKED = 1U << 7U,
    NO_THROW = 1U << 8U,
    SELF_REFERENCE = 1U << 9U,
    NO_RETURN_TYPE_CHECK = 1U << 10U,
    DIRECT_RETURN = 1U << 11U,
    NO_WIDENING = 1U << 12U,
    NO_BOXING = 1U << 13U,
    NO_UNBOXING = 1U << 14U,
    ONLY_CHECK_WIDENING = 1U << 15U,
    ONLY_CHECK_BOXING_UNBOXING = 1U << 16U,
    IN_ASSIGNMENT_CONTEXT = 1U << 17U,
    IN_CASTING_CONTEXT = 1U << 18U,
    UNCHECKED_CAST = 1U << 19U,
    IGNORE_TYPE_PARAMETERS = 1U << 20U,
    CHECK_PROXY = 1U << 21U,
    NO_CHECK_TRAILING_LAMBDA = 1U << 23U,
    NO_THROW_GENERIC_TYPEALIAS = 1U << 24U,
    OVERRIDING_CONTEXT = 1U << 25U,
    IGNORE_REST_PARAM = 1U << 26U,
    STRING_TO_CHAR = 1U << 27U,

    ASSIGNMENT_CONTEXT = WIDENING | BOXING | UNBOXING,
    BRIDGE_CHECK = OVERRIDING_CONTEXT | IGNORE_TYPE_PARAMETERS | NO_RETURN_TYPE_CHECK,
    CASTING_CONTEXT = NARROWING | WIDENING | BOXING | UNBOXING | UNCHECKED_CAST,
};

enum class RelationResult { TRUE, FALSE, UNKNOWN, MAYBE, CACHE_MISS, ERROR };

enum class RelationType { COMPARABLE, ASSIGNABLE, IDENTICAL, UNCHECKED_CASTABLE, SUPERTYPE };

}  // namespace ark::es2panda::checker

template <>
struct enumbitops::IsAllowedType<ark::es2panda::checker::TypeRelationFlag> : std::true_type {
};

namespace ark::es2panda::checker {

class RelationKey {
public:
    uint64_t sourceId;
    uint64_t targetId;
};

class RelationKeyHasher {
public:
    size_t operator()(const RelationKey &key) const noexcept
    {
        return static_cast<size_t>(key.sourceId ^ key.targetId);
    }
};

class RelationKeyComparator {
public:
    bool operator()(const RelationKey &lhs, const RelationKey &rhs) const
    {
        return lhs.sourceId == rhs.sourceId && lhs.targetId == rhs.targetId;
    }
};

class RelationEntry {
public:
    RelationResult result;
    RelationType type;
};

using RelationMap = std::unordered_map<RelationKey, RelationEntry, RelationKeyHasher, RelationKeyComparator>;

class RelationHolder {
public:
    RelationMap cached;
    RelationType type {};
};

class AsSrc {
public:
    explicit AsSrc(const Type *type) : type_(const_cast<Type *>(type)) {}

    const Type *GetType() const
    {
        return type_;
    }

private:
    Type *type_;
};

using TypeErrorMessageElement =
    std::variant<const Type *, AsSrc, char *, util::StringView, lexer::TokenType, size_t, const Signature *>;

class TypeRelation {
public:
    explicit TypeRelation(Checker *checker)
        : checker_(checker), result_(RelationResult::FALSE), instantiationRecursionMap_(Allocator()->Adapter())
    {
    }

    bool IsTrue() const
    {
        return result_ == RelationResult::TRUE;
    }

    bool IsError() const
    {
        return result_ == RelationResult::ERROR;
    }

    bool ApplyNarrowing() const
    {
        return (flags_ & TypeRelationFlag::NARROWING) != 0;
    }

    bool ApplyWidening() const
    {
        return (flags_ & TypeRelationFlag::WIDENING) != 0;
    }

    bool ApplyBoxing() const
    {
        return (flags_ & TypeRelationFlag::BOXING) != 0;
    }

    bool ApplyUnboxing() const
    {
        return (flags_ & TypeRelationFlag::UNBOXING) != 0;
    }

    bool ApplyStringToChar() const
    {
        return (flags_ & TypeRelationFlag::STRING_TO_CHAR) != 0;
    }

    bool NoReturnTypeCheck() const
    {
        return (flags_ & TypeRelationFlag::NO_RETURN_TYPE_CHECK) != 0;
    }

    bool DirectReturn() const
    {
        return (flags_ & TypeRelationFlag::DIRECT_RETURN) != 0;
    }

    bool InAssignmentContext() const
    {
        return (flags_ & TypeRelationFlag::IN_ASSIGNMENT_CONTEXT) != 0;
    }

    bool OnlyCheckWidening() const
    {
        return (flags_ & TypeRelationFlag::ONLY_CHECK_WIDENING) != 0;
    }

    bool OnlyCheckBoxingUnboxing() const
    {
        return (flags_ & TypeRelationFlag::ONLY_CHECK_BOXING_UNBOXING) != 0;
    }

    bool IgnoreTypeParameters() const
    {
        return (flags_ & TypeRelationFlag::IGNORE_TYPE_PARAMETERS) != 0;
    }

    [[nodiscard]] bool InCastingContext() const noexcept
    {
        return (flags_ & TypeRelationFlag::IN_CASTING_CONTEXT) != 0;
    }

    [[nodiscard]] bool UncheckedCast() const noexcept
    {
        return (flags_ & TypeRelationFlag::UNCHECKED_CAST) != 0;
    }

    [[nodiscard]] bool NoThrow() const noexcept
    {
        return (flags_ & TypeRelationFlag::NO_THROW) != 0;
    }

    [[nodiscard]] bool NoThrowGenericTypeAlias() const noexcept
    {
        return (flags_ & TypeRelationFlag::NO_THROW_GENERIC_TYPEALIAS) != 0;
    }

    [[nodiscard]] bool IsOverridingCheck() const noexcept
    {
        return (flags_ & TypeRelationFlag::OVERRIDING_CONTEXT) != 0;
    }

    [[nodiscard]] bool IsBridgeCheck() const noexcept
    {
        return (flags_ & TypeRelationFlag::BRIDGE_CHECK) == helpers::ToUnderlying(TypeRelationFlag::BRIDGE_CHECK);
    }

    [[nodiscard]] TypeRelationFlag GetTypeRelationFlags() const noexcept
    {
        return flags_;
    }

    const Checker *GetChecker() const
    {
        return checker_;
    }

    ir::Expression *GetNode() const
    {
        return node_;
    }

    Checker *GetChecker()
    {
        return checker_;
    }

    void IncreaseTypeRecursionCount(Type *const type)
    {
        if (const auto foundType = instantiationRecursionMap_.find(type);
            foundType != instantiationRecursionMap_.end()) {
            foundType->second += 1;
            return;
        }

        instantiationRecursionMap_.insert({type, 1});
    }

    bool IsAtTypeDepthLimit(Type *const type)
    {
        // This limitation makes sure that no type can be instantiated in infinite recursion. When declaring generic
        // classes with recursive types, so the generic class itself, we need to allow 2 depth of recursion, to make it
        // possible to reference the correct types of it's members and methods. 2 is possibly enough, because if we
        // chain expressions, every one of them will be rechecked separately, thus allowing another 2 recursion.
        constexpr auto MAX_RECURSIVE_TYPE_INST = 2;
        const auto foundType = instantiationRecursionMap_.find(type);
        return foundType == instantiationRecursionMap_.end() ? true : (foundType->second < MAX_RECURSIVE_TYPE_INST);
    }

    void DecreaseTypeRecursionCount(Type *const type)
    {
        const auto foundType = instantiationRecursionMap_.find(type);
        if (foundType == instantiationRecursionMap_.end()) {
            return;
        }

        if (foundType->second > 1) {
            foundType->second -= 1;
            return;
        }

        instantiationRecursionMap_.erase(type);
    }

    //  NOTE: special overloading to be used mainly in ETSCompiler where types and nodes are 'const'.
    //  Unfortunately now we cannot have only a single method with 'const Types *' because it affects
    //  a lot of non-const references... :(((
    bool IsIdenticalTo(Type const *source, Type const *target)
    {
        return IsIdenticalTo(const_cast<Type *>(source), const_cast<Type *>(target));
    }
    bool IsIdenticalTo(Type *source, Type *target);
    bool IsIdenticalTo(IndexInfo *source, IndexInfo *target);
    bool IsCompatibleTo(Signature *source, Signature *target);
    bool IsAssignableTo(Type *source, Type *target);
    bool IsComparableTo(Type *source, Type *target);
    bool IsCastableTo(Type *const source, Type *const target);
    bool IsSupertypeOf(Type const *super, Type const *sub)
    {
        return IsSupertypeOf(const_cast<Type *>(super), const_cast<Type *>(sub));
    }
    bool IsSupertypeOf(Type *super, Type *sub);
    void RaiseError(const std::string &errMsg, const lexer::SourcePosition &loc) const;
    void RaiseError(std::initializer_list<TypeErrorMessageElement> list, const lexer::SourcePosition &loc) const;
    void LogError(std::initializer_list<TypeErrorMessageElement> list, const lexer::SourcePosition &loc) const;

    bool Result(bool res)
    {
        result_ = res ? RelationResult::TRUE : RelationResult::FALSE;
        return res;
    }

    void Result(RelationResult res)
    {
        result_ = res;
    }

    void SetNode(ir::Expression *node)
    {
        node_ = node;
    }

    void SetFlags(TypeRelationFlag flags)
    {
        flags_ = flags;
    }

    void RemoveFlags(TypeRelationFlag flags)
    {
        flags_ &= ~flags;
    }

    ArenaAllocator *Allocator();

    friend class SavedTypeRelationFlagsContext;

private:
    RelationResult CacheLookup(const Type *source, const Type *target, const RelationHolder &holder,
                               RelationType type) const;

    Checker *checker_;
    RelationResult result_ {};
    TypeRelationFlag flags_ {};
    ir::Expression *node_ {};
    ArenaMap<checker::Type *, int8_t> instantiationRecursionMap_;
};
class SavedTypeRelationFlagsContext {
public:
    explicit SavedTypeRelationFlagsContext(TypeRelation *relation, TypeRelationFlag newFlag)
        : relation_(relation), prev_(relation->flags_)
    {
        relation_->flags_ = newFlag;
    }

    NO_COPY_SEMANTIC(SavedTypeRelationFlagsContext);
    DEFAULT_MOVE_SEMANTIC(SavedTypeRelationFlagsContext);

    ~SavedTypeRelationFlagsContext()
    {
        relation_->flags_ = prev_;
    }

private:
    TypeRelation *relation_;
    TypeRelationFlag prev_;
};
}  // namespace ark::es2panda::checker

#endif /* TYPESCRIPT_TYPES_TYPE_RELATION_H */
