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

#ifndef ES2PANDA_IR_EXPRESSION_BINARY_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_BINARY_EXPRESSION_H

#include "checker/checkerContext.h"
#include "compiler/core/vReg.h"
#include "ir/expression.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class BinaryExpression : public Expression {
public:
    BinaryExpression() = delete;
    ~BinaryExpression() override = default;

    NO_COPY_SEMANTIC(BinaryExpression);
    NO_MOVE_SEMANTIC(BinaryExpression);

    explicit BinaryExpression(Expression *const left, Expression *const right, lexer::TokenType const operatorType)
        : Expression(AstNodeType::BINARY_EXPRESSION), left_(left), right_(right), operator_(operatorType)
    {
    }

    // NOTE (csabahurton): friend relationship can be removed once there are getters for private fields
    friend class checker::ETSAnalyzer;

    [[nodiscard]] const Expression *Left() const noexcept
    {
        return left_;
    }

    [[nodiscard]] Expression *Left() noexcept
    {
        return left_;
    }

    [[nodiscard]] const Expression *Right() const noexcept
    {
        return right_;
    }

    [[nodiscard]] Expression *Right() noexcept
    {
        return right_;
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

    [[nodiscard]] bool IsLogical() const noexcept
    {
        return operator_ <= lexer::TokenType::PUNCTUATOR_LOGICAL_AND;
    }

    [[nodiscard]] bool IsLogicalExtended() const noexcept
    {
        return operator_ == lexer::TokenType::PUNCTUATOR_LOGICAL_AND ||
               operator_ == lexer::TokenType::PUNCTUATOR_LOGICAL_OR;
    }

    [[nodiscard]] bool IsBitwise() const noexcept
    {
        return operator_ == lexer::TokenType::PUNCTUATOR_BITWISE_OR ||
               operator_ == lexer::TokenType::PUNCTUATOR_BITWISE_XOR ||
               operator_ == lexer::TokenType::PUNCTUATOR_BITWISE_AND ||
               operator_ == lexer::TokenType::PUNCTUATOR_BITWISE_AND_EQUAL ||
               operator_ == lexer::TokenType::PUNCTUATOR_BITWISE_OR_EQUAL ||
               operator_ == lexer::TokenType::PUNCTUATOR_BITWISE_XOR_EQUAL;
    }

    [[nodiscard]] bool IsArithmetic() const noexcept
    {
        return operator_ == lexer::TokenType::PUNCTUATOR_PLUS || operator_ == lexer::TokenType::PUNCTUATOR_MINUS ||
               operator_ == lexer::TokenType::PUNCTUATOR_MULTIPLY || operator_ == lexer::TokenType::PUNCTUATOR_DIVIDE ||
               operator_ == lexer::TokenType::PUNCTUATOR_MOD || operator_ == lexer::TokenType::PUNCTUATOR_PLUS_EQUAL ||
               operator_ == lexer::TokenType::PUNCTUATOR_MINUS_EQUAL ||
               operator_ == lexer::TokenType::PUNCTUATOR_MULTIPLY_EQUAL ||
               operator_ == lexer::TokenType::PUNCTUATOR_DIVIDE_EQUAL ||
               operator_ == lexer::TokenType::PUNCTUATOR_MOD_EQUAL || IsBitwise();
    }

    void SetLeft(Expression *expr) noexcept
    {
        left_ = expr;
        left_->SetParent(this);
        SetStart(left_->Start());
    }

    void SetRight(Expression *expr) noexcept
    {
        right_ = expr;
        right_->SetParent(this);
        SetEnd(right_->End());
    }

    void SetResult(Expression *expr) noexcept
    {
        result_ = expr;
        result_->SetParent(this);
        SetStart(result_->Start());
    }

    void SetOperator(lexer::TokenType operatorType) noexcept
    {
        operator_ = operatorType;
        type_ = AstNodeType::BINARY_EXPRESSION;
    }

    [[nodiscard]] checker::Type *OperationType() noexcept
    {
        return operationType_;
    }

    void SetOperationType(checker::Type *const operationType) noexcept
    {
        operationType_ = operationType;
    }

    [[nodiscard]] const checker::Type *OperationType() const noexcept
    {
        return operationType_;
    }

    [[nodiscard]] BinaryExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    void CompileOperands(compiler::ETSGen *etsg, compiler::VReg lhs) const;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    Expression *left_ = nullptr;
    Expression *right_ = nullptr;
    Expression *result_ = nullptr;
    lexer::TokenType operator_;
    checker::Type *operationType_ {};
};
}  // namespace ark::es2panda::ir

#endif
