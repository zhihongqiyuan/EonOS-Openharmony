/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_IR_STATEMENT_ANNOTATION_DECLARATION_H
#define ES2PANDA_IR_STATEMENT_ANNOTATION_DECLARATION_H

#include "varbinder/scope.h"
#include "varbinder/variable.h"
#include "ir/statement.h"
#include "ir/astNode.h"
#include "ir/expressions/identifier.h"

namespace ark::es2panda::ir {
class AnnotationDeclaration : public Statement {
public:
    explicit AnnotationDeclaration(Expression *expr, ArenaAllocator *allocator)
        : Statement(AstNodeType::ANNOTATION_DECLARATION), expr_(expr), properties_(allocator->Adapter())
    {
    }
    explicit AnnotationDeclaration(Expression *expr, ArenaVector<AstNode *> &&properties)
        : Statement(AstNodeType::ANNOTATION_DECLARATION), expr_(expr), properties_(std::move(properties))
    {
    }

    const util::StringView &InternalName() const
    {
        return internalName_;
    }

    void SetInternalName(util::StringView internalName)
    {
        internalName_ = internalName;
    }

    [[nodiscard]] const Expression *Expr() const noexcept
    {
        return expr_;
    }

    [[nodiscard]] Expression *Expr() noexcept
    {
        return expr_;
    }

    [[nodiscard]] ArenaVector<AstNode *> &Properties() noexcept
    {
        return properties_;
    }

    [[nodiscard]] const ArenaVector<AstNode *> &Properties() const noexcept
    {
        return properties_;
    }

    [[nodiscard]] const ArenaVector<AstNode *> *PropertiesPtr() const
    {
        return &Properties();
    }

    void AddProperties(ArenaVector<AstNode *> &&properties)
    {
        properties_ = std::move(properties);
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

    [[nodiscard]] bool IsScopeBearer() const noexcept override
    {
        return true;
    }

    [[nodiscard]] varbinder::LocalScope *Scope() const noexcept override
    {
        return scope_;
    }

    void SetScope(varbinder::LocalScope *scope)
    {
        ASSERT(scope_ == nullptr);
        scope_ = scope;
    }

    void ClearScope() noexcept override
    {
        scope_ = nullptr;
    }

    Identifier *GetBaseName() const;

private:
    util::StringView internalName_ {};
    varbinder::LocalScope *scope_ {};
    Expression *expr_;
    ArenaVector<AstNode *> properties_;
};
}  // namespace ark::es2panda::ir

#endif