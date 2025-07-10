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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_CLASS_ELEMENT_H
#define ES2PANDA_PARSER_INCLUDE_AST_CLASS_ELEMENT_H

#include "ir/statement.h"
#include "ir/typed.h"

namespace ark::es2panda::ir {
class Expression;

class ClassElement : public TypedStatement {
public:
    ClassElement() = delete;
    ~ClassElement() override = default;

    NO_COPY_SEMANTIC(ClassElement);
    NO_MOVE_SEMANTIC(ClassElement);
    // CC-OFFNXT(G.FUN.01-CPP) solid logic
    explicit ClassElement(AstNodeType const elementType, Expression *const key, Expression *const value,
                          ModifierFlags const modifiers, ArenaAllocator *const allocator, bool const isComputed)
        : TypedStatement(elementType, modifiers),
          key_(key),
          value_(value),
          decorators_(allocator->Adapter()),
          isComputed_(isComputed)
    {
    }

    [[nodiscard]] Identifier *Id() noexcept;

    [[nodiscard]] const Identifier *Id() const noexcept;

    [[nodiscard]] Expression *Key() noexcept
    {
        return key_;
    }

    [[nodiscard]] const Expression *Key() const noexcept
    {
        return key_;
    }

    [[nodiscard]] Expression *Value() noexcept
    {
        return value_;
    }

    void SetValue(Expression *value) noexcept;

    [[nodiscard]] const Expression *Value() const noexcept
    {
        return value_;
    }

    [[nodiscard]] bool IsPrivateElement() const noexcept;

    [[nodiscard]] const ArenaVector<Decorator *> &Decorators() const noexcept
    {
        return decorators_;
    }

    const ArenaVector<Decorator *> *DecoratorsPtr() const override
    {
        return &Decorators();
    }

    [[nodiscard]] bool IsComputed() const noexcept
    {
        return isComputed_;
    }

    void AddDecorators([[maybe_unused]] ArenaVector<ir::Decorator *> &&decorators) override
    {
        decorators_ = std::move(decorators);
    }

    void AddDecorator(ir::Decorator *const decorator)
    {
        if (decorator != nullptr) {
            decorators_.emplace_back(decorator);
        }
    }

    bool CanHaveDecorator([[maybe_unused]] bool inTs) const override
    {
        return true;
    }

    [[nodiscard]] virtual PrivateFieldKind ToPrivateFieldKind(bool isStatic) const = 0;

protected:
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    Expression *key_;
    Expression *value_;
    ArenaVector<Decorator *> decorators_;
    bool isComputed_;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};
}  // namespace ark::es2panda::ir

#endif
