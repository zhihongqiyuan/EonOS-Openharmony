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

#ifndef ES2PANDA_IR_STATEMENT_BLOCK_STATEMENT_H
#define ES2PANDA_IR_STATEMENT_BLOCK_STATEMENT_H

#include "ir/statement.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class BlockStatement : public Statement {
public:
    explicit BlockStatement(ArenaAllocator *allocator, ArenaVector<Statement *> &&statementList)
        : Statement(AstNodeType::BLOCK_STATEMENT),
          statements_(std::move(statementList)),
          trailingBlocks_(allocator->Adapter())
    {
    }

    // NOTE (somas): this friend relationship can be removed once there are getters for private fields
    friend class checker::ETSAnalyzer;

    [[nodiscard]] bool IsScopeBearer() const noexcept override
    {
        return true;
    }

    [[nodiscard]] varbinder::Scope *Scope() const noexcept override
    {
        return scope_;
    }

    void SetScope(varbinder::Scope *scope) noexcept
    {
        scope_ = scope;
    }

    void ClearScope() noexcept override
    {
        scope_ = nullptr;
    }

    const ArenaVector<Statement *> &Statements() const
    {
        return statements_;
    }

    ArenaVector<Statement *> &Statements()
    {
        return statements_;
    }

    void SetStatements(ArenaVector<Statement *> &&statementList)
    {
        statements_ = std::move(statementList);

        for (auto *statement : statements_) {
            statement->SetParent(this);
        }
    }

    void AddTrailingBlock(AstNode *stmt, BlockStatement *trailingBlock)
    {
        trailingBlocks_.emplace(stmt, trailingBlock);
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;

    AstNode *Clone(ArenaAllocator *const allocator, AstNode *const parent) override;

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
    varbinder::Scope *scope_ {};
    ArenaVector<Statement *> statements_;
    ArenaUnorderedMap<AstNode *, BlockStatement *> trailingBlocks_;
};
}  // namespace ark::es2panda::ir

#endif
