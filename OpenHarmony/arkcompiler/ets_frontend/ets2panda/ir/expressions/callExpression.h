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

#ifndef ES2PANDA_IR_EXPRESSION_CALL_EXPRESSION_H
#define ES2PANDA_IR_EXPRESSION_CALL_EXPRESSION_H

#include "varbinder/variable.h"
#include "ir/expression.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
class TSAnalyzer;
class Signature;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::compiler {
class JSCompiler;
class ETSCompiler;
}  // namespace ark::es2panda::compiler

namespace ark::es2panda::ir {
class TSTypeParameterInstantiation;

class CallExpression : public MaybeOptionalExpression {
public:
    CallExpression() = delete;
    ~CallExpression() override = default;

    NO_COPY_SEMANTIC(CallExpression);
    NO_MOVE_SEMANTIC(CallExpression);

    explicit CallExpression(Expression *const callee, ArenaVector<Expression *> &&arguments,
                            TSTypeParameterInstantiation *const typeParams, bool const optional,
                            bool const trailingComma = false)
        : MaybeOptionalExpression(AstNodeType::CALL_EXPRESSION, optional),
          callee_(callee),
          arguments_(std::move(arguments)),
          typeParams_(typeParams),
          trailingComma_(trailingComma)
    {
    }

    explicit CallExpression(CallExpression const &other, ArenaAllocator *allocator);

    // NOTE (csabahurton): these friend relationships can be removed once there are getters for private fields
    friend class checker::TSAnalyzer;
    friend class checker::ETSAnalyzer;
    friend class compiler::JSCompiler;
    friend class compiler::ETSCompiler;

    const Expression *Callee() const
    {
        return callee_;
    }

    [[nodiscard]] Expression *Callee() noexcept
    {
        return callee_;
    }

    void SetCallee(Expression *callee) noexcept
    {
        callee_ = callee;
        callee_->SetParent(this);
    }

    [[nodiscard]] const TSTypeParameterInstantiation *TypeParams() const noexcept
    {
        return typeParams_;
    }

    [[nodiscard]] TSTypeParameterInstantiation *TypeParams() noexcept
    {
        return typeParams_;
    }

    [[nodiscard]] const ArenaVector<Expression *> &Arguments() const noexcept
    {
        return arguments_;
    }

    [[nodiscard]] ArenaVector<Expression *> &Arguments() noexcept
    {
        return arguments_;
    }

    [[nodiscard]] bool HasTrailingComma() const noexcept
    {
        return trailingComma_;
    }

    [[nodiscard]] checker::Signature *Signature() noexcept
    {
        return signature_;
    }

    [[nodiscard]] checker::Signature *Signature() const noexcept
    {
        return signature_;
    }

    void SetSignature(checker::Signature *const signature) noexcept
    {
        signature_ = signature;
    }

    void SetTypeParams(TSTypeParameterInstantiation *typeParams) noexcept;

    [[nodiscard]] checker::Type *UncheckedType() const noexcept
    {
        return uncheckedType_;
    }

    void SetUncheckedType(checker::Type *type) noexcept
    {
        uncheckedType_ = type;
    }

    void SetTrailingBlock(ir::BlockStatement *const block) noexcept;

    [[nodiscard]] ir::BlockStatement *TrailingBlock() const noexcept
    {
        return trailingBlock_;
    }

    void SetIsTrailingBlockInNewLine(bool const isNewLine) noexcept
    {
        isTrailingBlockInNewLine_ = isNewLine;
    }

    [[nodiscard]] bool IsTrailingBlockInNewLine() const noexcept
    {
        return isTrailingBlockInNewLine_;
    }

    [[nodiscard]] CallExpression *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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

protected:
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    Expression *callee_;
    ArenaVector<Expression *> arguments_;
    TSTypeParameterInstantiation *typeParams_;
    checker::Signature *signature_ {};
    bool trailingComma_;
    // for trailing lambda feature in ets
    ir::BlockStatement *trailingBlock_ {};
    bool isTrailingBlockInNewLine_ {false};
    checker::Type *uncheckedType_ {};
    // NOLINTEND(misc-non-private-member-variables-in-classes)

private:
    bool IsETSConstructorCall() const;
    checker::Type *InitAnonymousLambdaCallee(checker::ETSChecker *checker, Expression *callee,
                                             checker::Type *calleeType);
};
}  // namespace ark::es2panda::ir

#endif
