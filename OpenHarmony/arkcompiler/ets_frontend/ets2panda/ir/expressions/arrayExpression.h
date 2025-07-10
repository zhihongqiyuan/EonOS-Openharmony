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

#ifndef ES2PANDA_IR_EXPRESSION_ARRAY_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_ARRAY_EXPRESSION_H

#include "ir/expression.h"
#include "ir/validationInfo.h"
#include "checker/types/ets/etsArrayType.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
}  // namespace ark::es2panda::checker
namespace ark::es2panda::compiler {
class ETSCompiler;
}  // namespace ark::es2panda::compiler
namespace ark::es2panda::ir {
class ArrayExpression : public AnnotatedExpression {
private:
    struct Tag {};

public:
    ArrayExpression() = delete;
    ~ArrayExpression() override = default;

    NO_COPY_SEMANTIC(ArrayExpression);
    NO_MOVE_SEMANTIC(ArrayExpression);

public:
    explicit ArrayExpression(ArenaVector<Expression *> &&elements, ArenaAllocator *const allocator)
        : ArrayExpression(AstNodeType::ARRAY_EXPRESSION, std::move(elements), allocator, false)
    {
    }

    explicit ArrayExpression(AstNodeType nodeType, ArenaVector<Expression *> &&elements,
                             ArenaAllocator *const allocator, bool const trailingComma)
        : AnnotatedExpression(nodeType),
          decorators_(allocator->Adapter()),
          elements_(std::move(elements)),
          trailingComma_(trailingComma)
    {
    }

    explicit ArrayExpression(Tag tag, ArrayExpression const &other, ArenaAllocator *allocator);

    // NOTE (vivienvoros): these friend relationships can be removed once there are getters for private fields
    friend class checker::ETSAnalyzer;
    friend class compiler::ETSCompiler;

    [[nodiscard]] const ArenaVector<Expression *> &Elements() const noexcept
    {
        return elements_;
    }

    [[nodiscard]] ArenaVector<Expression *> &Elements() noexcept
    {
        return elements_;
    }

    void SetElements(ArenaVector<Expression *> &&elements) noexcept
    {
        elements_ = std::move(elements);
    }

    [[nodiscard]] bool IsDeclaration() const noexcept
    {
        return isDeclaration_;
    }

    [[nodiscard]] bool IsOptional() const noexcept
    {
        return optional_;
    }

    void SetDeclaration() noexcept
    {
        isDeclaration_ = true;
    }

    void SetOptional(bool optional) noexcept
    {
        optional_ = optional;
    }

    void SetPreferredType(checker::Type *preferredType) noexcept
    {
        preferredType_ = preferredType;
    }

    [[nodiscard]] checker::Type *GetPreferredType() noexcept
    {
        return preferredType_;
    }

    [[nodiscard]] checker::Type const *GetPreferredType() const noexcept
    {
        return preferredType_;
    }

    [[nodiscard]] const ArenaVector<Decorator *> &Decorators() const noexcept
    {
        return decorators_;
    }

    const ArenaVector<Decorator *> *DecoratorsPtr() const override
    {
        return &Decorators();
    }

    void AddDecorators([[maybe_unused]] ArenaVector<ir::Decorator *> &&decorators) override
    {
        decorators_ = std::move(decorators);
    }

    bool CanHaveDecorator([[maybe_unused]] bool inTs) const override
    {
        return true;
    }

    [[nodiscard]] ArrayExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    [[nodiscard]] bool ConvertibleToArrayPattern();
    [[nodiscard]] ValidationInfo ValidateExpression();
    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;
    checker::Type *CheckPattern(checker::TSChecker *checker);
    bool HandleNestedArrayExpression(checker::ETSChecker *checker, ArrayExpression *currentElement,
                                     bool isPreferredTuple, std::size_t idx);

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

    void GetPrefferedTypeFromFuncParam(checker::ETSChecker *checker, Expression *param,
                                       checker::TypeRelationFlag flags);

private:
    ArenaVector<Decorator *> decorators_;
    ArenaVector<Expression *> elements_;
    checker::Type *preferredType_ {};
    bool isDeclaration_ {};
    bool trailingComma_ {};
    bool optional_ {};
};
}  // namespace ark::es2panda::ir

#endif
