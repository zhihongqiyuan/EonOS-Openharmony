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

#ifndef ES2PANDA_IR_TS_QUALIFIED_NAME_H
#define ES2PANDA_IR_TS_QUALIFIED_NAME_H

#include "ir/expression.h"

namespace ark::es2panda::ir {
class TSQualifiedName : public Expression {
    struct Tag {};

public:
    TSQualifiedName() = delete;
    ~TSQualifiedName() override = default;

    NO_COPY_SEMANTIC(TSQualifiedName);
    NO_MOVE_SEMANTIC(TSQualifiedName);

    explicit TSQualifiedName(Expression *left, Identifier *right)
        : Expression(AstNodeType::TS_QUALIFIED_NAME), left_(left), right_(right)
    {
    }

    explicit TSQualifiedName(Tag tag, TSQualifiedName const &other, ArenaAllocator *allocator);

    [[nodiscard]] const Expression *Left() const noexcept
    {
        return left_;
    }

    [[nodiscard]] Expression *Left() noexcept
    {
        return left_;
    }

    [[nodiscard]] const Identifier *Right() const noexcept
    {
        return right_;
    }

    [[nodiscard]] Identifier *Right() noexcept
    {
        return right_;
    }

    util::StringView ToString(ArenaAllocator *allocator) const;
    util::StringView BaseToString(ArenaAllocator *allocator) const;
    ir::TSQualifiedName *ResolveLeftMostQualifiedName();
    const ir::TSQualifiedName *ResolveLeftMostQualifiedName() const;

    [[nodiscard]] TSQualifiedName *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    Expression *left_;
    Identifier *right_;
};
}  // namespace ark::es2panda::ir

#endif
