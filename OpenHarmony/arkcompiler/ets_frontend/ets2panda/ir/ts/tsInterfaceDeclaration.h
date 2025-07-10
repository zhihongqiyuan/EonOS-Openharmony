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

#ifndef ES2PANDA_IR_TS_INTERFACE_DECLARATION_H
#define ES2PANDA_IR_TS_INTERFACE_DECLARATION_H

#include "varbinder/scope.h"
#include "ir/statement.h"

namespace ark::es2panda::varbinder {
class Variable;
}  // namespace ark::es2panda::varbinder

namespace ark::es2panda::ir {
class Identifier;
class TSInterfaceBody;
class TSInterfaceHeritage;
class TSTypeParameterDeclaration;

class TSInterfaceDeclaration : public TypedStatement {
public:
    // NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
    struct ConstructorData {
        Identifier *id;
        TSTypeParameterDeclaration *typeParams;
        TSInterfaceBody *body;
        bool isStatic;
        bool isExternal;
        es2panda::Language lang;
    };
    // NOLINTEND(cppcoreguidelines-pro-type-member-init)

    explicit TSInterfaceDeclaration(ArenaAllocator *allocator, ArenaVector<TSInterfaceHeritage *> &&extends,
                                    ConstructorData &&data)
        : TypedStatement(AstNodeType::TS_INTERFACE_DECLARATION),
          decorators_(allocator->Adapter()),
          id_(data.id),
          typeParams_(data.typeParams),
          body_(data.body),
          extends_(std::move(extends)),
          isStatic_(data.isStatic),
          isExternal_(data.isExternal),
          lang_(data.lang)
    {
        if (isStatic_) {
            AddModifier(ir::ModifierFlags::STATIC);
        }
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

    TSInterfaceBody *Body()
    {
        return body_;
    }

    const TSInterfaceBody *Body() const
    {
        return body_;
    }

    Identifier *Id()
    {
        return id_;
    }

    const Identifier *Id() const
    {
        return id_;
    }

    const util::StringView &InternalName() const
    {
        return internalName_;
    }

    void SetInternalName(util::StringView internalName)
    {
        internalName_ = internalName;
    }

    bool IsStatic() const
    {
        return isStatic_;
    }

    bool IsFromExternal() const
    {
        return isExternal_;
    }

    const TSTypeParameterDeclaration *TypeParams() const
    {
        return typeParams_;
    }

    TSTypeParameterDeclaration *TypeParams()
    {
        return typeParams_;
    }

    ArenaVector<TSInterfaceHeritage *> &Extends()
    {
        return extends_;
    }

    const ArenaVector<TSInterfaceHeritage *> &Extends() const
    {
        return extends_;
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
        return !inTs;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;

    es2panda::Language Language() const
    {
        return lang_;
    }

    ClassDeclaration *GetAnonClass() noexcept
    {
        return anonClass_;
    }

    ClassDeclaration *GetAnonClass() const noexcept
    {
        return anonClass_;
    }

    void SetAnonClass(ClassDeclaration *anonClass) noexcept
    {
        anonClass_ = anonClass;
    }

    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override;
    checker::Type *InferType(checker::TSChecker *checker, varbinder::Variable *bindingVar) const;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    ArenaVector<Decorator *> decorators_;
    varbinder::LocalScope *scope_ {nullptr};
    Identifier *id_;
    TSTypeParameterDeclaration *typeParams_;
    TSInterfaceBody *body_;
    ArenaVector<TSInterfaceHeritage *> extends_;
    util::StringView internalName_ {};
    bool isStatic_;
    bool isExternal_;
    es2panda::Language lang_;
    ClassDeclaration *anonClass_ {nullptr};
};
}  // namespace ark::es2panda::ir

#endif
