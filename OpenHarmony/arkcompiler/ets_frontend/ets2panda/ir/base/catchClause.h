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

#ifndef ES2PANDA_IR_BASE_CATCH_CLAUSE_H
#define ES2PANDA_IR_BASE_CATCH_CLAUSE_H

#include "varbinder/scope.h"
#include "ir/statement.h"

namespace ark::es2panda::ir {
class BlockStatement;
class Expression;

class CatchClause : public TypedStatement {
public:
    explicit CatchClause(Expression *param, BlockStatement *body)
        : TypedStatement(AstNodeType::CATCH_CLAUSE), param_(param), body_(body)
    {
    }

    Expression *Param()
    {
        return param_;
    }

    const Expression *Param() const
    {
        return param_;
    }

    BlockStatement *Body()
    {
        return body_;
    }

    const BlockStatement *Body() const
    {
        return body_;
    }

    [[nodiscard]] bool IsScopeBearer() const noexcept override
    {
        return true;
    }

    [[nodiscard]] varbinder::CatchScope *Scope() const noexcept override
    {
        return scope_;
    }

    void SetScope(varbinder::CatchScope *scope)
    {
        ASSERT(scope_ == nullptr);
        scope_ = scope;
    }

    void ClearScope() noexcept override
    {
        scope_ = nullptr;
    }

    bool IsDefaultCatchClause() const;
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
    varbinder::CatchScope *scope_ {nullptr};
    Expression *param_;
    BlockStatement *body_;
};
}  // namespace ark::es2panda::ir

#endif
