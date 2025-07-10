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

#ifndef ES2PANDA_IR_EXPRESSION_NEW_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_NEW_EXPRESSION_H

#include "ir/expression.h"

namespace ark::es2panda::checker {
class TSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class NewExpression : public Expression {
private:
    struct Tag {};

public:
    NewExpression() = delete;
    ~NewExpression() override = default;

    NO_COPY_SEMANTIC(NewExpression);
    NO_MOVE_SEMANTIC(NewExpression);

    explicit NewExpression(Expression *const callee, ArenaVector<Expression *> &&arguments)
        : Expression(AstNodeType::NEW_EXPRESSION), callee_(callee), arguments_(std::move(arguments))
    {
    }
    explicit NewExpression(Tag tag, NewExpression const &other, ArenaAllocator *allocator);

    // NOTE (csabahurton): friend relationship can be removed once there are getters for private fields
    friend class checker::TSAnalyzer;

    [[nodiscard]] const Expression *Callee() const noexcept
    {
        return callee_;
    }

    [[nodiscard]] const ArenaVector<Expression *> &Arguments() const noexcept
    {
        return arguments_;
    }

    [[nodiscard]] NewExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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
    Expression *callee_ = nullptr;
    ArenaVector<Expression *> arguments_;
};
}  // namespace ark::es2panda::ir

#endif
