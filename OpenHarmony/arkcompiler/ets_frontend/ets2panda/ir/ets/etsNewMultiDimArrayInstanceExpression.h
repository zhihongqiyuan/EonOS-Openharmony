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

#ifndef ES2PANDA_IR_ETS_NEW_MULTI_DIM_ARRAY_INSTANCE_EXPRESSION_H
#define ES2PANDA_IR_ETS_NEW_MULTI_DIM_ARRAY_INSTANCE_EXPRESSION_H

#include "ir/expression.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
class Signature;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::compiler {
class ETSCompiler;
}  // namespace ark::es2panda::compiler

namespace ark::es2panda::ir {

class ETSNewMultiDimArrayInstanceExpression : public Expression {
public:
    ETSNewMultiDimArrayInstanceExpression() = delete;
    ~ETSNewMultiDimArrayInstanceExpression() override = default;

    NO_COPY_SEMANTIC(ETSNewMultiDimArrayInstanceExpression);
    NO_MOVE_SEMANTIC(ETSNewMultiDimArrayInstanceExpression);

    explicit ETSNewMultiDimArrayInstanceExpression(ir::TypeNode *const typeReference,
                                                   ArenaVector<ir::Expression *> &&dimensions)
        : Expression(AstNodeType::ETS_NEW_MULTI_DIM_ARRAY_INSTANCE_EXPRESSION),
          typeReference_(typeReference),
          dimensions_(std::move(dimensions))
    {
    }

    explicit ETSNewMultiDimArrayInstanceExpression(ETSNewMultiDimArrayInstanceExpression const &other,
                                                   ArenaAllocator *allocator);

    [[nodiscard]] ir::TypeNode *TypeReference() noexcept
    {
        return typeReference_;
    }

    [[nodiscard]] ir::TypeNode const *TypeReference() const noexcept
    {
        return typeReference_;
    }

    [[nodiscard]] ArenaVector<ir::Expression *> &Dimensions() noexcept
    {
        return dimensions_;
    }

    [[nodiscard]] ArenaVector<ir::Expression *> const &Dimensions() const noexcept
    {
        return dimensions_;
    }

    [[nodiscard]] checker::Signature *Signature() noexcept
    {
        return signature_;
    }

    [[nodiscard]] const checker::Signature *Signature() const noexcept
    {
        return signature_;
    }

    void SetSignature(checker::Signature *signature) noexcept
    {
        signature_ = signature;
    }

    [[nodiscard]] ETSNewMultiDimArrayInstanceExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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
    ir::TypeNode *typeReference_;
    ArenaVector<ir::Expression *> dimensions_;
    checker::Signature *signature_ {};
};
}  // namespace ark::es2panda::ir

#endif
