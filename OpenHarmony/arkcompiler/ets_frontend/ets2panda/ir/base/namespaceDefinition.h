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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_NAMESPACE_DEFINITION_H
#define ES2PANDA_PARSER_INCLUDE_AST_NAMESPACE_DEFINITION_H

#include "varbinder/scope.h"
#include "varbinder/variable.h"
#include "ir/astNode.h"
#include "ir/expressions/identifier.h"
#include "util/language.h"

namespace ark::es2panda::ir {
class ClassElement;
class Identifier;
class MethodDefinition;
class TSTypeParameterDeclaration;
class TSTypeParameterInstantiation;
class TSClassImplements;
class TSIndexSignature;

class NamespaceDefinition : public TypedAstNode {
public:
    NamespaceDefinition() = delete;
    ~NamespaceDefinition() override = default;

    NO_COPY_SEMANTIC(NamespaceDefinition);
    NO_MOVE_SEMANTIC(NamespaceDefinition);

    explicit NamespaceDefinition(Identifier *ident, ArenaVector<AstNode *> &&body, MethodDefinition *ctor,
                                 ModifierFlags flags, Language lang)
        : TypedAstNode(AstNodeType::NAMESPACE_DEFINITION, flags),

          ident_(ident),
          ctor_(ctor),
          body_(std::move(body)),
          lang_(lang)
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

    [[nodiscard]] const Identifier *Ident() const noexcept
    {
        return ident_;
    }

    [[nodiscard]] Identifier *Ident() noexcept
    {
        return ident_;
    }

    void SetIdent(ir::Identifier *ident) noexcept;

    [[nodiscard]] const util::StringView &PrivateId() const noexcept
    {
        return privateId_;
    }

    [[nodiscard]] const util::StringView &InternalName() const noexcept
    {
        return privateId_;
    }

    void SetInternalName(util::StringView internalName) noexcept
    {
        privateId_ = internalName;
    }

    [[nodiscard]] es2panda::Language Language() const noexcept
    {
        return lang_;
    }

    [[nodiscard]] MethodDefinition *Ctor() noexcept
    {
        return ctor_;
    }

    void SetCtor(MethodDefinition *ctor)
    {
        ctor_ = ctor;
    }

    void AddProperties(ArenaVector<AstNode *> &&body)
    {
        for (auto *prop : body) {
            prop->SetParent(this);
        }

        body_.insert(body_.end(), body.begin(), body.end());
    }

    [[nodiscard]] ArenaVector<AstNode *> &Body() noexcept
    {
        return body_;
    }

    [[nodiscard]] const ArenaVector<AstNode *> &Body() const noexcept
    {
        return body_;
    }

    const FunctionExpression *Ctor() const;
    bool HasPrivateMethod() const;
    bool HasComputedInstanceField() const;
    bool HasMatchingPrivateKey(const util::StringView &name) const;

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
    varbinder::LocalScope *scope_ {nullptr};
    util::StringView privateId_ {};
    Identifier *ident_ {};
    MethodDefinition *ctor_ {};
    ArenaVector<AstNode *> body_;
    es2panda::Language lang_;
};
}  // namespace ark::es2panda::ir

#endif
