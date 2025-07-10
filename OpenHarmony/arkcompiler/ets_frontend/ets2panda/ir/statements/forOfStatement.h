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

#ifndef ES2PANDA_IR_STATEMENT_FOR_OF_STATEMENT_H
#define ES2PANDA_IR_STATEMENT_FOR_OF_STATEMENT_H

#include <checker/types/signature.h>
#include "ir/statements/loopStatement.h"

namespace ark::es2panda::varbinder {
class LoopScope;
}  // namespace ark::es2panda::varbinder

namespace ark::es2panda::checker {
class ETSAnalyzer;
class ETSObjectType;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class Expression;

// NOLINTBEGIN(modernize-avoid-c-arrays)
inline constexpr char const ITERATOR_INTERFACE_NAME[] = "Iterator";
inline constexpr char const ITERATOR_INTERFACE_METHOD[] = "next";
inline constexpr char const ITERATOR_RESULT_NAME[] = "IteratorResult";
// NOLINTEND(modernize-avoid-c-arrays)

class ForOfStatement final : public LoopStatement {
    friend class checker::ETSAnalyzer;

public:
    ForOfStatement() = delete;
    ~ForOfStatement() override = default;

    NO_COPY_SEMANTIC(ForOfStatement);
    NO_MOVE_SEMANTIC(ForOfStatement);

    explicit ForOfStatement(AstNode *left, Expression *right, Statement *body, bool const isAwait)
        : LoopStatement(AstNodeType::FOR_OF_STATEMENT), left_(left), right_(right), body_(body), isAwait_(isAwait)
    {
    }

    [[nodiscard]] AstNode *Left() noexcept
    {
        return left_;
    }

    [[nodiscard]] const AstNode *Left() const noexcept
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

    [[nodiscard]] Statement *Body() noexcept
    {
        return body_;
    }

    [[nodiscard]] const Statement *Body() const noexcept
    {
        return body_;
    }

    [[nodiscard]] bool IsAwait() const noexcept
    {
        return isAwait_;
    }

    [[nodiscard]] ForOfStatement *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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

protected:
    [[nodiscard]] checker::Type *CheckIteratorMethod(checker::ETSChecker *checker);
    [[nodiscard]] checker::Type *CheckIteratorMethodForObject(checker::ETSChecker *checker,
                                                              checker::ETSObjectType *sourceType);
    static bool CheckReturnTypeOfIteratorMethod(checker::ETSChecker *checker, checker::ETSObjectType *sourceType,
                                                checker::Signature *signature, const lexer::SourcePosition &position);
    static bool CheckIteratorInterfaceForObject(checker::ETSChecker *checker, checker::ETSObjectType *obj);

private:
    checker::Type *CreateUnionIteratorTypes(checker::ETSChecker *checker, checker::Type *exprType);

    AstNode *left_;
    Expression *right_;
    Statement *body_;
    bool isAwait_;
};
}  // namespace ark::es2panda::ir

#endif
