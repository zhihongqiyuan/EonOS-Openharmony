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

#ifndef ES2PANDA_IR_STATEMENT_LAUNCH_STATEMENT_H
#define ES2PANDA_IR_STATEMENT_LAUNCH_STATEMENT_H

#include "ir/expression.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::compiler {
class ETSCompiler;
}  // namespace ark::es2panda::compiler

namespace ark::es2panda::ir {

class CallExpression;

class ETSLaunchExpression : public Expression {
public:
    ETSLaunchExpression() = delete;
    ~ETSLaunchExpression() override = default;

    NO_COPY_SEMANTIC(ETSLaunchExpression);
    NO_MOVE_SEMANTIC(ETSLaunchExpression);

    explicit ETSLaunchExpression(CallExpression *expr);

    // NOTE (csabahurton): these friend relationships can be removed once there are getters for private fields
    friend class checker::ETSAnalyzer;
    friend class compiler::ETSCompiler;

    [[nodiscard]] ETSLaunchExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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

    bool IsStaticCall() const;

    [[nodiscard]] const CallExpression *Call() const noexcept
    {
        return expr_;
    }

private:
    CallExpression *expr_;
};
}  // namespace ark::es2panda::ir
#endif
