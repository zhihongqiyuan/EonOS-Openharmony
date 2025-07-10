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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_SPREAD_ELEMENT_H
#define ES2PANDA_PARSER_INCLUDE_AST_SPREAD_ELEMENT_H

#include "ir/expression.h"
#include "ir/validationInfo.h"

namespace ark::es2panda::ir {
class SpreadElement : public AnnotatedExpression {
private:
    struct Tag {};

public:
    SpreadElement() = delete;
    ~SpreadElement() override = default;

    NO_COPY_SEMANTIC(SpreadElement);
    NO_MOVE_SEMANTIC(SpreadElement);

    explicit SpreadElement(AstNodeType const nodeType, ArenaAllocator *const allocator, Expression *const argument)
        : AnnotatedExpression(nodeType), decorators_(allocator->Adapter()), argument_(argument)
    {
    }

    explicit SpreadElement(Tag tag, SpreadElement const &other, ArenaAllocator *allocator);

    [[nodiscard]] const Expression *Argument() const noexcept
    {
        return argument_;
    }

    [[nodiscard]] Expression *Argument() noexcept
    {
        return argument_;
    }

    [[nodiscard]] bool IsOptional() const noexcept
    {
        return optional_;
    }

    [[nodiscard]] const ArenaVector<Decorator *> &Decorators() const noexcept
    {
        return decorators_;
    }

    const ArenaVector<Decorator *> *DecoratorsPtr() const override
    {
        return &Decorators();
    }

    void AddDecorators(ArenaVector<Decorator *> &&decorators) override
    {
        decorators_ = std::move(decorators);
    }

    bool CanHaveDecorator([[maybe_unused]] bool inTs) const override
    {
        return true;
    }

    void SetOptional(bool optional) noexcept
    {
        optional_ = optional;
    }

    [[nodiscard]] SpreadElement *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    ValidationInfo ValidateExpression();
    [[nodiscard]] bool ConvertibleToRest(bool isDeclaration, bool allowPattern = true);

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    void Compile([[maybe_unused]] compiler::ETSGen *etsg) const override;
    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    ArenaVector<Decorator *> decorators_;
    Expression *argument_ = nullptr;
    bool optional_ {false};
};
}  // namespace ark::es2panda::ir

#endif
