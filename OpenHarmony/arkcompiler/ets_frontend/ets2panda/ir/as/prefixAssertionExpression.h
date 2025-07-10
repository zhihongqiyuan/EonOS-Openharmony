/*
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

#ifndef ES2PANDA_IR_PREFIX_ASSERTION_EXPRESSION_H
#define ES2PANDA_IR_PREFIX_ASSERTION_EXPRESSION_H

#include "ir/expression.h"

namespace ark::es2panda::ir {
class PrefixAssertionExpression : public Expression {
public:
    explicit PrefixAssertionExpression(Expression *expr, TypeNode *type)
        : Expression(AstNodeType::PREFIX_ASSERTION_EXPRESSION), expr_(expr), type_(type)
    {
    }

    const Expression *Expr() const
    {
        return expr_;
    }

    const TypeNode *Type() const
    {
        return type_;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(AstDumper *dumper) const override;
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
    TypeNode *type_;
};
}  // namespace ark::es2panda::ir

#endif
