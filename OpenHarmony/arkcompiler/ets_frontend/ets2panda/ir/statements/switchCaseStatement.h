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

#ifndef ES2PANDA_IR_STATEMENT_SWITCH_CASE_STATEMENT_H
#define ES2PANDA_IR_STATEMENT_SWITCH_CASE_STATEMENT_H

#include "ir/statement.h"

namespace ark::es2panda::ir {
class Expression;

class SwitchCaseStatement : public Statement {
public:
    SwitchCaseStatement() = delete;
    ~SwitchCaseStatement() override = default;

    NO_COPY_SEMANTIC(SwitchCaseStatement);
    NO_MOVE_SEMANTIC(SwitchCaseStatement);

    explicit SwitchCaseStatement(Expression *test, ArenaVector<Statement *> &&consequent)
        : Statement(AstNodeType::SWITCH_CASE_STATEMENT), test_(test), consequent_(std::move(consequent))
    {
    }

    [[nodiscard]] Expression *Test() noexcept
    {
        return test_;
    }

    [[nodiscard]] const Expression *Test() const noexcept
    {
        return test_;
    }

    [[nodiscard]] const ArenaVector<Statement *> &Consequent() const noexcept
    {
        return consequent_;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;

    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    void CheckAndTestCase(checker::ETSChecker *checker, checker::Type *comparedExprType, checker::Type *unboxedDiscType,
                          ir::Expression *node, bool &isDefaultCase);

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    Expression *test_;
    ArenaVector<Statement *> consequent_;
};
}  // namespace ark::es2panda::ir

#endif
