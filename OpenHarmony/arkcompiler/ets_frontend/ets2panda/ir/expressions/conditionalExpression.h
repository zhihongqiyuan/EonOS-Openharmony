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

#ifndef ES2PANDA_IR_EXPRESSION_CONDITIONAL_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_CONDITIONAL_EXPRESSION_H

#include "ir/expression.h"

namespace ark::es2panda::checker {
class TSAnalyzer;
class ETSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class ConditionalExpression : public Expression {
public:
    ConditionalExpression() = delete;
    ~ConditionalExpression() override = default;

    NO_COPY_SEMANTIC(ConditionalExpression);
    NO_MOVE_SEMANTIC(ConditionalExpression);

    explicit ConditionalExpression(Expression *test, Expression *consequent, Expression *alternate)
        : Expression(AstNodeType::CONDITIONAL_EXPRESSION), test_(test), consequent_(consequent), alternate_(alternate)
    {
    }

    [[nodiscard]] const Expression *Test() const noexcept
    {
        return test_;
    }

    [[nodiscard]] Expression *Test() noexcept
    {
        return test_;
    }

    void SetTest(Expression *expr) noexcept
    {
        test_ = expr;
        test_->SetParent(this);
    }

    [[nodiscard]] const Expression *Consequent() const noexcept
    {
        return consequent_;
    }

    [[nodiscard]] Expression *Consequent() noexcept
    {
        return consequent_;
    }

    void SetConsequent(Expression *expr) noexcept
    {
        consequent_ = expr;
        consequent_->SetParent(this);
    }

    [[nodiscard]] const Expression *Alternate() const noexcept
    {
        return alternate_;
    }

    [[nodiscard]] Expression *Alternate() noexcept
    {
        return alternate_;
    }

    void SetAlternate(Expression *expr) noexcept
    {
        alternate_ = expr;
        alternate_->SetParent(this);
    }

    [[nodiscard]] ConditionalExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;

    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    checker::Type *NumericConditionalCheck(checker::ETSChecker *checker, checker::Type *consequentType,
                                           checker::Type *alternateType);

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    Expression *test_;
    Expression *consequent_;
    Expression *alternate_;
};
}  // namespace ark::es2panda::ir

#endif
