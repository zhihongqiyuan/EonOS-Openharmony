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

#ifndef ES2PANDA_IR_STATEMENT_SWITCH_STATEMENT_H
#define ES2PANDA_IR_STATEMENT_SWITCH_STATEMENT_H

#include "varbinder/scope.h"
#include "ir/statement.h"

namespace ark::es2panda::checker {
class TSAnalyzer;
class ETSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class Expression;
class SwitchCaseStatement;

class SwitchStatement : public Statement {
public:
    SwitchStatement() = delete;
    ~SwitchStatement() override = default;

    NO_COPY_SEMANTIC(SwitchStatement);
    NO_MOVE_SEMANTIC(SwitchStatement);

    explicit SwitchStatement(Expression *discriminant, ArenaVector<SwitchCaseStatement *> &&cases)
        : Statement(AstNodeType::SWITCH_STATEMENT), discriminant_(discriminant), cases_(std::move(cases))
    {
    }

    [[nodiscard]] const Expression *Discriminant() const noexcept
    {
        return discriminant_;
    }

    [[nodiscard]] Expression *Discriminant() noexcept
    {
        return discriminant_;
    }

    [[nodiscard]] const ArenaVector<SwitchCaseStatement *> &Cases() const noexcept
    {
        return cases_;
    }

    [[nodiscard]] ArenaVector<SwitchCaseStatement *> &Cases() noexcept
    {
        return cases_;
    }

    [[nodiscard]] bool IsScopeBearer() const noexcept override
    {
        return true;
    }

    [[nodiscard]] varbinder::LocalScope *Scope() const noexcept override
    {
        return scope_;
    }

    void SetScope(varbinder::LocalScope *scope) noexcept
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
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    varbinder::LocalScope *scope_ {nullptr};
    Expression *discriminant_;
    ArenaVector<SwitchCaseStatement *> cases_;
};
}  // namespace ark::es2panda::ir

#endif
