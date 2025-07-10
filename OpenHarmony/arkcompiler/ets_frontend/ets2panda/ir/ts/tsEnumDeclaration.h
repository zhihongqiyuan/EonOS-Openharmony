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

#ifndef ES2PANDA_IR_TS_ENUM_DECLARATION_H
#define ES2PANDA_IR_TS_ENUM_DECLARATION_H

#include "varbinder/scope.h"
#include "ir/statement.h"

namespace ark::es2panda::varbinder {
class EnumVariable;
}  // namespace ark::es2panda::varbinder

namespace ark::es2panda::ir {
class Identifier;
class TSEnumMember;

class TSEnumDeclaration : public TypedStatement {
public:
    // NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
    struct ConstructorFlags {
        bool isConst;
        bool isStatic = false;
        bool isDeclare = false;
    };
    // NOLINTEND(cppcoreguidelines-pro-type-member-init)

    explicit TSEnumDeclaration(ArenaAllocator *allocator, Identifier *key, ArenaVector<AstNode *> &&members,
                               ConstructorFlags &&flags)
        : TypedStatement(AstNodeType::TS_ENUM_DECLARATION),
          decorators_(allocator->Adapter()),
          key_(key),
          members_(std::move(members)),
          isConst_(flags.isConst)
    {
        if (flags.isStatic) {
            AddModifier(ModifierFlags::STATIC);
        }
        if (flags.isDeclare) {
            AddModifier(ModifierFlags::DECLARE);
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

    const Identifier *Key() const
    {
        return key_;
    }

    Identifier *Key()
    {
        return key_;
    }

    const ArenaVector<AstNode *> &Members() const
    {
        return members_;
    }

    const util::StringView &InternalName() const
    {
        return internalName_;
    }

    void SetInternalName(util::StringView internalName)
    {
        internalName_ = internalName;
    }

    ir::ClassDefinition *BoxedClass() const
    {
        return boxedClass_;
    }

    void SetBoxedClass(ir::ClassDefinition *const wrapperClass)
    {
        boxedClass_ = wrapperClass;
    }

    bool IsConst() const
    {
        return isConst_;
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

    static varbinder::EnumMemberResult EvaluateEnumMember(checker::TSChecker *checker, varbinder::EnumVariable *enumVar,
                                                          const ir::AstNode *expr);
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
    ArenaVector<ir::Decorator *> decorators_;
    Identifier *key_;
    ArenaVector<AstNode *> members_;
    util::StringView internalName_;
    ir::ClassDefinition *boxedClass_ {nullptr};
    bool isConst_;
};
}  // namespace ark::es2panda::ir

#endif
