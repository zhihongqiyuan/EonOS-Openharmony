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

#ifndef ES2PANDA_IR_EXPRESSION_MEMBER_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_MEMBER_EXPRESSION_H

#include "checker/types/ets/etsObjectType.h"
#include "ir/expression.h"

namespace ark::es2panda::compiler {
class JSCompiler;
class ETSCompiler;
}  // namespace ark::es2panda::compiler

namespace ark::es2panda::checker {
class ETSObjectType;
class ETSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {

// NOLINTBEGIN(modernize-avoid-c-arrays)
inline constexpr char const PREDEFINED_METHOD[] = "The special predefined method '";
// NOLINTEND(modernize-avoid-c-arrays)

using ENUMBITOPS_OPERATORS;

enum class MemberExpressionKind : uint32_t {
    NONE = 0,
    ELEMENT_ACCESS = 1U << 0U,
    PROPERTY_ACCESS = 1U << 1U,
    GETTER = 1U << 2U,
    SETTER = 1U << 3U,
};

}  // namespace ark::es2panda::ir

template <>
struct enumbitops::IsAllowedType<ark::es2panda::ir::MemberExpressionKind> : std::true_type {
};

namespace ark::es2panda::ir {

class MemberExpression : public MaybeOptionalExpression {
    friend class checker::ETSAnalyzer;

private:
    struct Tag {};

public:
    MemberExpression() = delete;
    ~MemberExpression() override = default;

    MemberExpression &operator=(const MemberExpression &) = delete;
    NO_MOVE_SEMANTIC(MemberExpression);

    explicit MemberExpression(Expression *object, Expression *property, MemberExpressionKind kind, bool computed,
                              bool optional)
        : MaybeOptionalExpression(AstNodeType::MEMBER_EXPRESSION, optional),
          object_(object),
          property_(property),
          kind_(kind),
          computed_(computed)
    {
    }

    explicit MemberExpression(Tag tag, MemberExpression const &other, ArenaAllocator *allocator);

    // NOTE (csabahurton): these friend relationships can be removed once there are getters for private fields
    friend class compiler::JSCompiler;
    friend class compiler::ETSCompiler;

    [[nodiscard]] Expression *Object() noexcept
    {
        return object_;
    }

    [[nodiscard]] const Expression *Object() const noexcept
    {
        return object_;
    }

    void SetObject(Expression *object) noexcept
    {
        object_ = object;
        object_->SetParent(this);
    }

    [[nodiscard]] Expression *Property() noexcept
    {
        return property_;
    }

    [[nodiscard]] const Expression *Property() const noexcept
    {
        return property_;
    }

    [[nodiscard]] varbinder::LocalVariable *PropVar() noexcept
    {
        if (Kind() == MemberExpressionKind::ELEMENT_ACCESS) {
            return nullptr;
        }
        return Property()->Variable() != nullptr ? Property()->Variable()->AsLocalVariable() : nullptr;
    }

    [[nodiscard]] const varbinder::LocalVariable *PropVar() const noexcept
    {
        if (Kind() == MemberExpressionKind::ELEMENT_ACCESS) {
            return nullptr;
        }
        return Property()->Variable() != nullptr ? Property()->Variable()->AsLocalVariable() : nullptr;
    }

    [[nodiscard]] bool IsComputed() const noexcept
    {
        return computed_;
    }

    [[nodiscard]] MemberExpressionKind Kind() const noexcept
    {
        return kind_;
    }

    void AddMemberKind(MemberExpressionKind kind) noexcept
    {
        kind_ |= kind;
    }

    [[nodiscard]] bool HasMemberKind(MemberExpressionKind kind) const noexcept
    {
        return (kind_ & kind) != 0;
    }

    void RemoveMemberKind(MemberExpressionKind const kind) noexcept
    {
        kind_ &= ~kind;
    }

    [[nodiscard]] checker::ETSObjectType *ObjType() const noexcept
    {
        return objType_;
    }

    void SetPropVar(varbinder::LocalVariable *propVar) noexcept
    {
        ASSERT(Property());
        Property()->SetVariable(propVar);
    }

    void SetObjectType(checker::ETSObjectType *objType) noexcept
    {
        objType_ = objType;
    }

    [[nodiscard]] bool IsIgnoreBox() const noexcept
    {
        return ignoreBox_;
    }

    void SetIgnoreBox() noexcept
    {
        ignoreBox_ = true;
    }

    [[nodiscard]] checker::Type *UncheckedType() const noexcept
    {
        return uncheckedType_;
    }

    [[nodiscard]] bool IsPrivateReference() const noexcept;

    [[nodiscard]] MemberExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    bool CompileComputed(compiler::ETSGen *etsg) const;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    void CompileToReg(compiler::PandaGen *pg, compiler::VReg objReg) const;
    void CompileToRegs(compiler::PandaGen *pg, compiler::VReg object, compiler::VReg property) const;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

protected:
    MemberExpression(MemberExpression const &other) : MaybeOptionalExpression(other)
    {
        kind_ = other.kind_;
        computed_ = other.computed_;
        ignoreBox_ = other.ignoreBox_;
        // Note! Probably, we need to do 'Instantiate(...)' but we haven't access to 'Relation()' here...
        uncheckedType_ = other.uncheckedType_;
        objType_ = other.objType_;
    }

private:
    std::pair<checker::Type *, varbinder::LocalVariable *> ResolveEnumMember(checker::ETSChecker *checker,
                                                                             checker::ETSEnumType *type) const;
    std::pair<checker::Type *, varbinder::LocalVariable *> ResolveObjectMember(checker::ETSChecker *checker) const;

    checker::Type *AdjustType(checker::ETSChecker *checker, checker::Type *type);
    checker::Type *SetAndAdjustType(checker::ETSChecker *checker, checker::ETSObjectType *objectType);
    checker::Type *CheckComputed(checker::ETSChecker *checker, checker::Type *baseType);
    checker::Type *CheckUnionMember(checker::ETSChecker *checker, checker::Type *baseType);
    checker::Type *TraverseUnionMember(checker::ETSChecker *checker, checker::ETSUnionType *unionType,
                                       checker::Type *commonPropType);

    bool CheckArrayIndexValue(checker::ETSChecker *checker) const;
    checker::Type *CheckIndexAccessMethod(checker::ETSChecker *checker);
    checker::Type *CheckTupleAccessMethod(checker::ETSChecker *checker, checker::Type *baseType);

    void LoadRhs(compiler::PandaGen *pg) const;

    Expression *object_ = nullptr;
    Expression *property_ = nullptr;
    MemberExpressionKind kind_;
    bool computed_;
    bool ignoreBox_ {false};
    checker::Type *uncheckedType_ {};
    checker::ETSObjectType *objType_ {};
};
}  // namespace ark::es2panda::ir

#endif
