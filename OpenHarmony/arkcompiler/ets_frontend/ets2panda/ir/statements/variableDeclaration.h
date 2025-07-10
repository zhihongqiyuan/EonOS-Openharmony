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

#ifndef ES2PANDA_IR_STATEMENT_VARIABLE_DECLARATION_H
#define ES2PANDA_IR_STATEMENT_VARIABLE_DECLARATION_H

#include "ir/statement.h"

namespace ark::es2panda::ir {
class VariableDeclarator;

class VariableDeclaration : public Statement {
private:
    struct Tag {};

public:
    enum class VariableDeclarationKind { CONST, LET, VAR };

    explicit VariableDeclaration(VariableDeclarationKind kind, ArenaAllocator *allocator,
                                 ArenaVector<VariableDeclarator *> &&declarators)
        : Statement(AstNodeType::VARIABLE_DECLARATION),
          kind_(kind),
          decorators_(allocator->Adapter()),
          declarators_(std::move(declarators))
    {
    }

    explicit VariableDeclaration(Tag tag, VariableDeclaration const &other, ArenaAllocator *allocator);

    const ArenaVector<VariableDeclarator *> &Declarators() const
    {
        return declarators_;
    }

    VariableDeclarationKind Kind() const
    {
        return kind_;
    }

    const ArenaVector<Decorator *> &Decorators() const
    {
        return decorators_;
    }

    const ArenaVector<Decorator *> *DecoratorsPtr() const override
    {
        return &Decorators();
    }

    void AddDecorators([[maybe_unused]] ArenaVector<ir::Decorator *> &&decorators) override
    {
        decorators_ = std::move(decorators);
    }

    bool CanHaveDecorator([[maybe_unused]] bool inTs) const override
    {
        return true;
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

    [[nodiscard]] VariableDeclaration *Clone(ArenaAllocator *allocator, AstNode *parent) override;

private:
    VariableDeclarationKind kind_;
    ArenaVector<Decorator *> decorators_;
    ArenaVector<VariableDeclarator *> declarators_;
};
}  // namespace ark::es2panda::ir

#endif
