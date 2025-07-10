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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_DECORATOR_H
#define ES2PANDA_PARSER_INCLUDE_AST_DECORATOR_H

#include "ir/statement.h"

namespace ark::es2panda::ir {
class Expression;

class Decorator : public Statement {
public:
    Decorator() = delete;
    ~Decorator() override = default;

    NO_COPY_SEMANTIC(Decorator);
    NO_MOVE_SEMANTIC(Decorator);

    explicit Decorator(Expression *expr) : Statement(AstNodeType::DECORATOR), expr_(expr) {}

    [[nodiscard]] const Expression *Expr() const noexcept
    {
        return expr_;
    }

    [[nodiscard]] Decorator *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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
    Expression *expr_;
};
}  // namespace ark::es2panda::ir

#endif
