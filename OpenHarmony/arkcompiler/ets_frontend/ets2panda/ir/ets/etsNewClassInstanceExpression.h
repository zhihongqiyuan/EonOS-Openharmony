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

#ifndef ES2PANDA_IR_ETS_NEW_CLASS_INSTANCE_EXPRESSION_H
#define ES2PANDA_IR_ETS_NEW_CLASS_INSTANCE_EXPRESSION_H

#include "compiler/core/vReg.h"
#include "ir/expression.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
class Signature;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::compiler {
class ETSCompiler;
}  // namespace ark::es2panda::compiler

namespace ark::es2panda::ir {

class ClassDefinition;

class ETSNewClassInstanceExpression : public Expression {
public:
    ETSNewClassInstanceExpression() = delete;
    ~ETSNewClassInstanceExpression() override = default;

    NO_COPY_SEMANTIC(ETSNewClassInstanceExpression);
    NO_MOVE_SEMANTIC(ETSNewClassInstanceExpression);

    explicit ETSNewClassInstanceExpression(ir::Expression *const typeReference,
                                           ArenaVector<ir::Expression *> &&arguments,
                                           ir::ClassDefinition *const classDefinition)
        : Expression(AstNodeType::ETS_NEW_CLASS_INSTANCE_EXPRESSION),
          typeReference_(typeReference),
          arguments_(std::move(arguments)),
          classDef_(classDefinition)
    {
    }
    // NOTE (csabahurton): these friend relationships can be removed once there are getters for private fields
    friend class checker::ETSAnalyzer;
    friend class compiler::ETSCompiler;

    explicit ETSNewClassInstanceExpression(ETSNewClassInstanceExpression const &other, ArenaAllocator *allocator);

    [[nodiscard]] ir::ClassDefinition *ClassDefinition() noexcept
    {
        return classDef_;
    }

    [[nodiscard]] const ir::ClassDefinition *ClassDefinition() const noexcept
    {
        return classDef_;
    }

    [[nodiscard]] ir::Expression *GetTypeRef() const noexcept
    {
        return typeReference_;
    }

    [[nodiscard]] const ArenaVector<ir::Expression *> &GetArguments() const noexcept
    {
        return arguments_;
    }

    [[nodiscard]] checker::Signature *GetSignature() const noexcept
    {
        return signature_;
    }

    void SetSignature(checker::Signature *const signature) noexcept
    {
        signature_ = signature;
    }

    void AddToArgumentsFront(ir::Expression *expr)
    {
        arguments_.insert(arguments_.begin(), expr);
    }

    [[nodiscard]] ETSNewClassInstanceExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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
    ir::Expression *typeReference_;
    ArenaVector<ir::Expression *> arguments_;
    ir::ClassDefinition *classDef_;
    checker::Signature *signature_ {};
};
}  // namespace ark::es2panda::ir

#endif
