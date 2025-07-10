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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_PROPERTY_H
#define ES2PANDA_PARSER_INCLUDE_AST_PROPERTY_H

#include "ir/expression.h"
#include "ir/validationInfo.h"

namespace ark::es2panda::ir {
enum class PropertyKind { INIT, GET, SET, PROTO };

class Property : public Expression {
private:
    struct Tag {};

public:
    Property() = delete;
    ~Property() override = default;

    Property &operator=(const Property &) = delete;
    NO_MOVE_SEMANTIC(Property);

    explicit Property(Expression *const key, Expression *const value)
        : Expression(AstNodeType::PROPERTY), kind_(PropertyKind::INIT), key_(key), value_(value)
    {
    }

    explicit Property(PropertyKind const kind, Expression *const key, Expression *const value, bool const isMethod,
                      bool const isComputed)
        : Expression(AstNodeType::PROPERTY),
          kind_(kind),
          key_(key),
          value_(value),
          isMethod_(isMethod),
          isShorthand_(false),
          isComputed_(isComputed)
    {
    }

    explicit Property(Tag tag, Property const &other, Expression *key, Expression *value);

    [[nodiscard]] Expression *Key() noexcept
    {
        return key_;
    }

    [[nodiscard]] const Expression *Key() const noexcept
    {
        return key_;
    }

    [[nodiscard]] const Expression *Value() const noexcept
    {
        return value_;
    }

    [[nodiscard]] Expression *Value() noexcept
    {
        return value_;
    }

    [[nodiscard]] PropertyKind Kind() const noexcept
    {
        return kind_;
    }

    [[nodiscard]] bool IsMethod() const noexcept
    {
        return isMethod_;
    }

    [[nodiscard]] bool IsShorthand() const noexcept
    {
        return isShorthand_;
    }

    [[nodiscard]] bool IsComputed() const noexcept
    {
        return isComputed_;
    }

    [[nodiscard]] bool IsAccessor() const noexcept
    {
        return IsAccessorKind(kind_);
    }

    [[nodiscard]] static bool IsAccessorKind(PropertyKind kind) noexcept
    {
        return kind == PropertyKind::GET || kind == PropertyKind::SET;
    }

    [[nodiscard]] Property *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    bool ConvertibleToPatternProperty();
    ValidationInfo ValidateExpression();

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

protected:
    Property(Property const &other) : Expression(static_cast<Expression const &>(other))
    {
        kind_ = other.kind_;
        isMethod_ = other.isMethod_;
        isShorthand_ = other.isShorthand_;
        isComputed_ = other.isComputed_;
    }

private:
    PropertyKind kind_;
    Expression *key_ = nullptr;
    Expression *value_ = nullptr;
    bool isMethod_ = false;
    bool isShorthand_ = true;
    bool isComputed_ = false;
};
}  // namespace ark::es2panda::ir

#endif
