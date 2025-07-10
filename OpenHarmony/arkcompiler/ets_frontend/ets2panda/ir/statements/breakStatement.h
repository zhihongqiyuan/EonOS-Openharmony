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

#ifndef ES2PANDA_IR_STATEMENT_BREAK_STATEMENT_H
#define ES2PANDA_IR_STATEMENT_BREAK_STATEMENT_H

#include "ir/statement.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::compiler {
class ETSCompiler;
}  // namespace ark::es2panda::compiler

namespace ark::es2panda::ir {
class Identifier;

class BreakStatement : public Statement {
public:
    ~BreakStatement() override = default;

    NO_COPY_SEMANTIC(BreakStatement);
    NO_MOVE_SEMANTIC(BreakStatement);

    explicit BreakStatement() : Statement(AstNodeType::BREAK_STATEMENT) {}
    explicit BreakStatement(Identifier *ident) : Statement(AstNodeType::BREAK_STATEMENT), ident_(ident) {}

    [[nodiscard]] const Identifier *Ident() const noexcept
    {
        return ident_;
    }

    Identifier *Ident() noexcept
    {
        return ident_;
    }

    [[nodiscard]] const ir::AstNode *Target() const noexcept
    {
        return target_;
    }

    void SetTarget(ir::AstNode const *target) noexcept
    {
        target_ = target;
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
    Identifier *ident_ {};
    const ir::AstNode *target_ {};
};
}  // namespace ark::es2panda::ir

#endif
