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

#ifndef ES2PANDA_IR_TS_MODULE_DECLARATION_H
#define ES2PANDA_IR_TS_MODULE_DECLARATION_H

#include "varbinder/scope.h"
#include "ir/statement.h"

namespace ark::es2panda::ir {
class Expression;

class TSModuleDeclaration : public Statement {
public:
    // NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
    struct ConstructorFlags {
        bool global;
        bool isExternalAmbient;
    };
    // NOLINTEND(cppcoreguidelines-pro-type-member-init)

    explicit TSModuleDeclaration(ArenaAllocator *allocator, Expression *name, Statement *body, ConstructorFlags &&flags)
        : Statement(AstNodeType::TS_MODULE_DECLARATION),
          decorators_(allocator->Adapter()),
          name_(name),
          body_(body),
          global_(flags.global),
          isExternalAmbient_(flags.isExternalAmbient)
    {
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

    const Expression *Name() const
    {
        return name_;
    }

    const Statement *Body() const
    {
        return body_;
    }

    bool Global() const
    {
        return global_;
    }

    bool IsExternalOrAmbient() const
    {
        return isExternalAmbient_;
    }

    void AddDecorators([[maybe_unused]] ArenaVector<ir::Decorator *> &&decorators) override
    {
        decorators_ = std::move(decorators);
    }

    bool CanHaveDecorator([[maybe_unused]] bool inTs) const override
    {
        return !inTs;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    ArenaVector<Decorator *> decorators_;
    varbinder::LocalScope *scope_ {nullptr};
    Expression *name_;
    Statement *body_;
    bool global_;
    bool isExternalAmbient_;
};
}  // namespace ark::es2panda::ir

#endif
