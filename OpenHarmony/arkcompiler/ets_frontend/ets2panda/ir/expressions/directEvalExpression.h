/*
 * Copyright (c) 2021 - 2023 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_IR_EXPRESSION_DIRECT_EVAL_H
#define ES2PANDA_IR_EXPRESSION_DIRECT_EVAL_H

namespace ark::es2panda::compiler {
class JSCompiler;
}  // namespace ark::es2panda::compiler

#include "ir/expressions/callExpression.h"

namespace ark::es2panda::ir {
class DirectEvalExpression : public CallExpression {
public:
    explicit DirectEvalExpression(Expression *callee, ArenaVector<Expression *> &&arguments,
                                  TSTypeParameterInstantiation *typeParams, bool optional, uint32_t parserStatus)
        : CallExpression(callee, std::move(arguments), typeParams, optional), parserStatus_(parserStatus)
    {
        type_ = AstNodeType::DIRECT_EVAL;
    }

    // NOTE (csabahurton): friend relationship can be removed once there are getters for private fields
    friend class compiler::JSCompiler;

    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    uint32_t parserStatus_ {};
};
}  // namespace ark::es2panda::ir

#endif
