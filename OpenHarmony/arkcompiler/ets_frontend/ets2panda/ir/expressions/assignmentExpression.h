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

#ifndef ES2PANDA_IR_EXPRESSION_ASSIGNMENT_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_ASSIGNMENT_EXPRESSION_H

#include "ir/expression.h"
#include "lexer/token/tokenType.h"
namespace ark::es2panda::checker {
class ETSAnalyzer;
}  // namespace ark::es2panda::checker
namespace ark::es2panda::ir {
class AssignmentExpression : public Expression {
private:
    struct Tag {};

public:
    AssignmentExpression() = delete;
    ~AssignmentExpression() override = default;

    AssignmentExpression &operator=(const AssignmentExpression &) = delete;
    NO_MOVE_SEMANTIC(AssignmentExpression);

    explicit AssignmentExpression(Expression *const left, Expression *const right,
                                  lexer::TokenType const assignmentOperator)
        : AssignmentExpression(AstNodeType::ASSIGNMENT_EXPRESSION, left, right, assignmentOperator)
    {
    }

    explicit AssignmentExpression(AstNodeType const type, Expression *const left, Expression *const right,
                                  lexer::TokenType const assignmentOperator)
        : Expression(type), left_(left), right_(right), operator_(assignmentOperator)
    {
    }

    explicit AssignmentExpression(Tag tag, AssignmentExpression const &other, Expression *left, Expression *right);

    // NOTE (vivienvoros): these friend relationships can be removed once there are getters for private fields
    friend class checker::ETSAnalyzer;

    [[nodiscard]] const Expression *Left() const noexcept
    {
        return left_;
    }

    [[nodiscard]] Expression *Left() noexcept
    {
        return left_;
    }

    [[nodiscard]] Expression *Right() noexcept
    {
        return right_;
    }

    [[nodiscard]] const Expression *Right() const noexcept
    {
        return right_;
    }

    void SetRight(Expression *const expr) noexcept
    {
        right_ = expr;

        if (right_ != nullptr) {
            right_->SetParent(this);
        }
    }

    [[nodiscard]] const Expression *Result() const noexcept
    {
        return result_;
    }

    [[nodiscard]] Expression *Result() noexcept
    {
        return result_;
    }

    [[nodiscard]] lexer::TokenType OperatorType() const noexcept
    {
        return operator_;
    }

    lexer::TokenType SetOperatorType(lexer::TokenType tokenType) noexcept
    {
        return operator_ = tokenType;
    }

    void SetResult(Expression *expr) noexcept
    {
        left_ = expr;
        SetStart(left_->Start());
    }

    [[nodiscard]] bool IsLogicalExtended() const noexcept
    {
        return operator_ == lexer::TokenType::PUNCTUATOR_LOGICAL_AND ||
               operator_ == lexer::TokenType::PUNCTUATOR_LOGICAL_OR;
    }

    [[nodiscard]] varbinder::Variable *Target() noexcept
    {
        return target_;
    }

    [[nodiscard]] varbinder::Variable *Target() const noexcept
    {
        return target_;
    }

    void SetIgnoreConstAssign()
    {
        ignoreConstAssign_ = true;
    }

    [[nodiscard]] bool IsIgnoreConstAssign() const
    {
        return ignoreConstAssign_;
    }

    [[nodiscard]] AssignmentExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    [[nodiscard]] bool ConvertibleToAssignmentPatternLeft(bool mustBePattern);
    [[nodiscard]] bool ConvertibleToAssignmentPatternRight();
    [[nodiscard]] bool ConvertibleToAssignmentPattern(bool mustBePattern = true);

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    void CompilePattern(compiler::PandaGen *pg) const;
    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

protected:
    AssignmentExpression(AssignmentExpression const &other) : Expression(static_cast<Expression const &>(other))
    {
        operator_ = other.operator_;
        target_ = other.target_;
        operationType_ = other.operationType_;
    }

private:
    Expression *left_ = nullptr;
    Expression *right_ = nullptr;
    Expression *result_ = nullptr;
    lexer::TokenType operator_;
    varbinder::Variable *target_ {};
    checker::Type *operationType_ {};
    bool ignoreConstAssign_ = false;
};
}  // namespace ark::es2panda::ir

#endif
