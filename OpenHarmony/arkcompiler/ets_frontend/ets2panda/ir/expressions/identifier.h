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

#ifndef ES2PANDA_IR_EXPRESSION_IDENTIFIER_H
#define ES2PANDA_IR_EXPRESSION_IDENTIFIER_H

#include "checker/checkerContext.h"
#include "ir/expression.h"
#include "ir/validationInfo.h"

namespace ark::es2panda::varbinder {
class Variable;
}  // namespace ark::es2panda::varbinder

namespace ark::es2panda::ir {

using ENUMBITOPS_OPERATORS;

enum class IdentifierFlags : uint32_t {
    NONE = 0U,
    OPTIONAL = 1U << 0U,
    TDZ = 1U << 1U,
    PRIVATE = 1U << 2U,
    GET = 1U << 3U,
    SET = 1U << 4U,
    IGNORE_BOX = 1U << 5U,
    ANNOTATIONDECL = 1U << 6U,
    ANNOTATIONUSAGE = 1U << 7U,
};

}  // namespace ark::es2panda::ir

template <>
struct enumbitops::IsAllowedType<ark::es2panda::ir::IdentifierFlags> : std::true_type {
};

namespace ark::es2panda::ir {

class Identifier : public AnnotatedExpression {
private:
    struct Tag {};

public:
    Identifier() = delete;
    ~Identifier() override = default;

    NO_COPY_SEMANTIC(Identifier);
    NO_MOVE_SEMANTIC(Identifier);

public:
    explicit Identifier(ArenaAllocator *const allocator) : Identifier("", allocator) {}
    explicit Identifier(util::StringView const name, ArenaAllocator *const allocator)
        : AnnotatedExpression(AstNodeType::IDENTIFIER), name_(name), decorators_(allocator->Adapter())
    {
    }

    explicit Identifier(util::StringView const name, TypeNode *const typeAnnotation, ArenaAllocator *const allocator)
        : AnnotatedExpression(AstNodeType::IDENTIFIER, typeAnnotation), name_(name), decorators_(allocator->Adapter())
    {
    }

    explicit Identifier(Tag tag, Identifier const &other, ArenaAllocator *allocator);

    [[nodiscard]] const util::StringView &Name() const noexcept
    {
        return name_;
    }

    [[nodiscard]] util::StringView &Name() noexcept
    {
        return name_;
    }

    void SetName(const util::StringView &newName) noexcept
    {
        name_ = newName;
    }

    [[nodiscard]] const ArenaVector<Decorator *> &Decorators() const noexcept
    {
        return decorators_;
    }

    const ArenaVector<Decorator *> *DecoratorsPtr() const override
    {
        return &Decorators();
    }

    [[nodiscard]] bool IsOptional() const noexcept
    {
        return (flags_ & IdentifierFlags::OPTIONAL) != 0;
    }

    void SetOptional(bool const optional) noexcept
    {
        if (optional) {
            flags_ |= IdentifierFlags::OPTIONAL;
        } else {
            flags_ &= ~IdentifierFlags::OPTIONAL;
        }
    }

    [[nodiscard]] bool IsReference(ScriptExtension ext) const noexcept
    {
        return !IsDeclaration(ext);
    }

    [[nodiscard]] bool IsTdz() const noexcept
    {
        return (flags_ & IdentifierFlags::TDZ) != 0;
    }

    void SetTdz() noexcept
    {
        flags_ |= IdentifierFlags::TDZ;
    }

    void SetAccessor() noexcept
    {
        flags_ |= IdentifierFlags::GET;
    }

    [[nodiscard]] bool IsAccessor() const noexcept
    {
        return (flags_ & IdentifierFlags::GET) != 0;
    }

    void SetMutator() noexcept
    {
        flags_ |= IdentifierFlags::SET;
    }

    [[nodiscard]] bool IsMutator() const noexcept
    {
        return (flags_ & IdentifierFlags::SET) != 0;
    }

    [[nodiscard]] bool IsPrivateIdent() const noexcept
    {
        return (flags_ & IdentifierFlags::PRIVATE) != 0;
    }

    void SetPrivate(bool const isPrivate) noexcept
    {
        if (isPrivate) {
            flags_ |= IdentifierFlags::PRIVATE;
        } else {
            flags_ &= ~IdentifierFlags::PRIVATE;
        }
    }

    [[nodiscard]] bool IsIgnoreBox() const noexcept
    {
        return (flags_ & IdentifierFlags::IGNORE_BOX) != 0;
    }

    void SetIgnoreBox() noexcept
    {
        flags_ |= IdentifierFlags::IGNORE_BOX;
    }

    [[nodiscard]] bool IsAnnotationDecl() const noexcept
    {
        return (flags_ & IdentifierFlags::ANNOTATIONDECL) != 0;
    }

    void SetAnnotationDecl() noexcept
    {
        flags_ |= IdentifierFlags::ANNOTATIONDECL;
    }

    [[nodiscard]] bool IsAnnotationUsage() const noexcept
    {
        return (flags_ & IdentifierFlags::ANNOTATIONUSAGE) != 0;
    }

    void SetAnnotationUsage() noexcept
    {
        flags_ |= IdentifierFlags::ANNOTATIONUSAGE;
    }

    void AddDecorators([[maybe_unused]] ArenaVector<ir::Decorator *> &&decorators) override
    {
        decorators_ = std::move(decorators);
    }

    [[nodiscard]] Identifier *Clone(ArenaAllocator *allocator, AstNode *parent) override;
    [[nodiscard]] Identifier *CloneReference(ArenaAllocator *allocator, AstNode *parent);

    bool CanHaveDecorator([[maybe_unused]] bool inTs) const override
    {
        return true;
    }

    [[nodiscard]] ValidationInfo ValidateExpression();

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    bool CheckDeclarationsPart2(const ir::AstNode *parent, ScriptExtension ext) const;
    bool CheckDeclarationsPart1(const ir::AstNode *parent, ScriptExtension ext) const;
    bool CheckNotDeclarations(const ir::AstNode *parent, ScriptExtension ext) const;
    bool CheckDefinitions(const ir::AstNode *parent, ScriptExtension ext) const;
    bool IsDeclaration(ScriptExtension ext) const;

    util::StringView name_;
    IdentifierFlags flags_ {IdentifierFlags::NONE};
    ArenaVector<Decorator *> decorators_;
};
}  // namespace ark::es2panda::ir

#endif
