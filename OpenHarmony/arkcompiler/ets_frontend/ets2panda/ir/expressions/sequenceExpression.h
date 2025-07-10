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

#ifndef ES2PANDA_IR_EXPRESSION_SEQUENCE_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_SEQUENCE_EXPRESSION_H

#include "ir/expression.h"

namespace ark::es2panda::ir {
class SequenceExpression : public Expression {
private:
    struct Tag {};

public:
    SequenceExpression() = delete;
    ~SequenceExpression() override = default;

    NO_COPY_SEMANTIC(SequenceExpression);
    NO_MOVE_SEMANTIC(SequenceExpression);

    explicit SequenceExpression(ArenaVector<Expression *> &&sequence)
        : Expression(AstNodeType::SEQUENCE_EXPRESSION), sequence_(std::move(sequence))
    {
    }

    explicit SequenceExpression(Tag tag, SequenceExpression const &other, ArenaAllocator *allocator);

    [[nodiscard]] const ArenaVector<Expression *> &Sequence() const noexcept
    {
        return sequence_;
    }

    [[nodiscard]] ArenaVector<Expression *> &Sequence() noexcept
    {
        return sequence_;
    }

    [[nodiscard]] SequenceExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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
    ArenaVector<Expression *> sequence_;
};
}  // namespace ark::es2panda::ir

#endif
