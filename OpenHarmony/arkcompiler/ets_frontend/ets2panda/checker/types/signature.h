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

#ifndef ES2PANDA_COMPILER_CHECKER_TYPES_SIGNATURE_H
#define ES2PANDA_COMPILER_CHECKER_TYPES_SIGNATURE_H

#include "type.h"

#include "varbinder/variable.h"

namespace ark::es2panda::public_lib {
struct Context;
}  // namespace ark::es2panda::public_lib

namespace ark::es2panda::checker {

class SignatureInfo {
public:
    explicit SignatureInfo(ArenaAllocator *allocator) : typeParams {allocator->Adapter()}, params {allocator->Adapter()}
    {
    }

    SignatureInfo(const SignatureInfo *other, ArenaAllocator *allocator)
        : typeParams(allocator->Adapter()), params(allocator->Adapter())
    {
        for (auto *it : other->typeParams) {
            typeParams.push_back(it);
        }
        for (auto *it : other->params) {
            params.push_back(it->Copy(allocator, it->Declaration()));
            params.back()->SetTsType(it->TsType());
        }

        minArgCount = other->minArgCount;

        if (other->restVar != nullptr) {
            restVar = other->restVar->Copy(allocator, other->restVar->Declaration());
            restVar->SetTsType(other->restVar->TsType());
        }
    }

    ~SignatureInfo() = default;
    NO_COPY_SEMANTIC(SignatureInfo);
    NO_MOVE_SEMANTIC(SignatureInfo);

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    ArenaVector<Type *> typeParams;
    uint32_t minArgCount {};
    varbinder::LocalVariable *restVar {};
    ArenaVector<varbinder::LocalVariable *> params;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

using ENUMBITOPS_OPERATORS;

enum class SignatureFlags : uint32_t {
    NO_OPTS = 0U,
    VIRTUAL = 1U << 0U,
    ABSTRACT = 1U << 1U,
    CALL = 1U << 2U,
    CONSTRUCT = 1U << 3U,
    PUBLIC = 1U << 4U,
    PROTECTED = 1U << 5U,
    PRIVATE = 1U << 6U,
    STATIC = 1U << 7U,
    FINAL = 1U << 8U,
    CONSTRUCTOR = 1U << 9U,
    TYPE = 1U << 10U,
    PROXY = 1U << 11U,
    INTERNAL = 1U << 12U,
    NEED_RETURN_TYPE = 1U << 13U,
    INFERRED_RETURN_TYPE = 1U << 14U,
    THIS_RETURN_TYPE = 1U << 15U,
    GETTER = 1U << 16U,
    SETTER = 1U << 17U,
    THROWS = 1U << 18U,
    RETHROWS = 1U << 19U,

    INTERNAL_PROTECTED = INTERNAL | PROTECTED,
    GETTER_OR_SETTER = GETTER | SETTER,
    FUNCTIONAL_INTERFACE_SIGNATURE = VIRTUAL | ABSTRACT | CALL | PUBLIC | TYPE
};

}  // namespace ark::es2panda::checker

template <>
struct enumbitops::IsAllowedType<ark::es2panda::checker::SignatureFlags> : std::true_type {
};

namespace ark::es2panda::checker {

class Signature {
public:
    Signature(SignatureInfo *signatureInfo, Type *returnType) : signatureInfo_(signatureInfo), returnType_(returnType)
    {
    }

    Signature(SignatureInfo *signatureInfo, Type *returnType, util::StringView internalName)
        : signatureInfo_(signatureInfo), returnType_(returnType), internalName_(internalName)
    {
    }

    Signature(SignatureInfo *signatureInfo, Type *returnType, ir::ScriptFunction *func)
        : signatureInfo_(signatureInfo), returnType_(returnType), func_(func)
    {
    }

    ~Signature() = default;
    NO_COPY_SEMANTIC(Signature);
    NO_MOVE_SEMANTIC(Signature);

    const SignatureInfo *GetSignatureInfo() const
    {
        return signatureInfo_;
    }

    SignatureInfo *GetSignatureInfo()
    {
        return signatureInfo_;
    }

    const ArenaVector<Type *> &TypeParams() const
    {
        return signatureInfo_->typeParams;
    }

    ArenaVector<Type *> &TypeParams()
    {
        return signatureInfo_->typeParams;
    }

    const ArenaVector<varbinder::LocalVariable *> &Params() const
    {
        return signatureInfo_->params;
    }

    ArenaVector<varbinder::LocalVariable *> &Params()
    {
        return signatureInfo_->params;
    }

    const Type *ReturnType() const
    {
        return returnType_;
    }

    Type *ReturnType()
    {
        return returnType_;
    }

    uint32_t MinArgCount() const
    {
        return signatureInfo_->minArgCount;
    }

    uint32_t OptionalArgCount() const
    {
        return signatureInfo_->params.size() - signatureInfo_->minArgCount;
    }

    void SetReturnType(Type *type)
    {
        returnType_ = type;
    }

    void SetOwner(ETSObjectType *owner)
    {
        ownerObj_ = owner;
    }

    void SetOwnerVar(varbinder::Variable *owner)
    {
        ownerVar_ = owner;
    }

    void SetFunction(ir::ScriptFunction *const function) noexcept
    {
        func_ = function;
    }

    ir::ScriptFunction *Function()
    {
        return func_;
    }

    ETSObjectType *Owner()
    {
        return ownerObj_;
    }

    const ETSObjectType *Owner() const
    {
        return ownerObj_;
    }

    varbinder::Variable *OwnerVar()
    {
        return ownerVar_;
    }

    const ir::ScriptFunction *Function() const
    {
        return func_;
    }

    const varbinder::LocalVariable *RestVar() const
    {
        return signatureInfo_->restVar;
    }

    uint8_t ProtectionFlag() const
    {
        if ((flags_ & SignatureFlags::PRIVATE) != 0) {
            return 2U;
        }

        if ((flags_ & SignatureFlags::PROTECTED) != 0) {
            return 1U;
        }

        return 0;
    }

    void AddSignatureFlag(SignatureFlags const flag) noexcept
    {
        flags_ |= flag;
    }

    void RemoveSignatureFlag(SignatureFlags const flag) noexcept
    {
        flags_ &= ~flag;
    }

    bool HasSignatureFlag(SignatureFlags const flag) const noexcept
    {
        return (flags_ & flag) != 0U;
    }

    [[nodiscard]] SignatureFlags GetFlags() const noexcept
    {
        return flags_;
    }

    bool IsFinal() const noexcept
    {
        return HasSignatureFlag(SignatureFlags::FINAL);
    }

    void ToAssemblerType(std::stringstream &ss) const;

    util::StringView InternalName() const;

    Signature *Copy(ArenaAllocator *allocator, TypeRelation *relation, GlobalTypesHolder *globalTypes);
    Signature *Substitute(TypeRelation *relation, const Substitution *substitution);

    void ToString(std::stringstream &ss, const varbinder::Variable *variable, bool printAsMethod = false,
                  bool precise = false) const;
    std::string ToString() const;
    void Compatible(TypeRelation *relation, Signature *other);
    bool CheckFunctionalInterfaces(TypeRelation *relation, Type *source, Type *target);
    void AssignmentTarget(TypeRelation *relation, Signature *source);
    Signature *BoxPrimitives(ETSChecker *checker);

private:
    bool CheckParameter(TypeRelation *relation, Type *type1, Type *type2);
    bool CheckReturnType(TypeRelation *relation, Type *type1, Type *type2);

    checker::SignatureInfo *signatureInfo_;
    Type *returnType_;
    ir::ScriptFunction *func_ {};
    SignatureFlags flags_ {SignatureFlags::NO_OPTS};
    util::StringView internalName_ {};
    ETSObjectType *ownerObj_ {};
    varbinder::Variable *ownerVar_ {};
};
}  // namespace ark::es2panda::checker

#endif /* TYPESCRIPT_TYPES_SIGNATURE_H */
