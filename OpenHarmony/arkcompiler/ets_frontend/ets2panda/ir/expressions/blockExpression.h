/**
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_IR_EXPRESSION_BLOCK_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_BLOCK_EXPRESSION_H

#include "ir/astNode.h"
#include "ir/expression.h"
#include "ir/statement.h"

namespace ark::es2panda::ir {

class BlockExpression : public Expression {
private:
    struct Tag {};

public:
    BlockExpression() = delete;
    ~BlockExpression() override = default;

    NO_COPY_SEMANTIC(BlockExpression);
    NO_MOVE_SEMANTIC(BlockExpression);

    explicit BlockExpression(ArenaVector<ir::Statement *> &&statements);
    explicit BlockExpression(Tag tag, BlockExpression const &other, ArenaAllocator *allocator);

    [[nodiscard]] ArenaVector<ir::Statement *> const &Statements() const noexcept
    {
        return statements_;
    }

    [[nodiscard]] ArenaVector<ir::Statement *> &Statements() noexcept
    {
        return statements_;
    }

    void AddStatements(ArenaVector<ir::Statement *> const &statements)
    {
        std::copy_if(statements.begin(), statements.end(), std::back_inserter(statements_),
                     [this](ir::Statement *statement) {
                         if (statement != nullptr) {
                             statement->SetParent(this);
                             return true;
                         };
                         return false;  // CC-OFF(G.FMT.02) project code style
                     });
    }

    void AddStatement(ir::Statement *statement)
    {
        if (statement != nullptr) {
            statement->SetParent(this);
            statements_.emplace_back(statement);
        }
    }

    [[nodiscard]] BlockExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

    [[nodiscard]] bool IsScopeBearer() const noexcept override
    {
        return true;
    }

    [[nodiscard]] varbinder::Scope *Scope() const noexcept override
    {
        return scope_;
    }

    void SetScope(varbinder::Scope *scope)
    {
        ASSERT(scope_ == nullptr);
        scope_ = scope;
    }

    void ClearScope() noexcept override
    {
        scope_ = nullptr;
    }

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
    varbinder::Scope *scope_ = {};
    ArenaVector<ir::Statement *> statements_;
};
}  // namespace ark::es2panda::ir

#endif
