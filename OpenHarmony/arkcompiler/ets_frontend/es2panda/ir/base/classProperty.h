/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_CLASS_PROPERTY_H
#define ES2PANDA_PARSER_INCLUDE_AST_CLASS_PROPERTY_H

#include <ir/statement.h>
#include <ir/expression.h>

namespace panda::es2panda::compiler {
class PandaGen;
}  // namespace panda::es2panda::compiler

namespace panda::es2panda::checker {
class Checker;
class Type;
}  // namespace panda::es2panda::checker

namespace panda::es2panda::ir {

class Expression;

class ClassProperty : public Statement {
public:
    explicit ClassProperty(Expression *key, Expression *value, Expression *typeAnnotation, ModifierFlags modifiers,
                           ArenaVector<Decorator *> &&decorators, bool isComputed, bool definite)
        : Statement(AstNodeType::CLASS_PROPERTY),
          key_(key),
          value_(value),
          typeAnnotation_(typeAnnotation),
          modifiers_(modifiers),
          decorators_(std::move(decorators)),
          isComputed_(isComputed),
          definite_(definite)
    {
    }

    const Expression *Key() const
    {
        return key_;
    }

    Expression *Key()
    {
        return key_;
    }

    void SetKey(Expression *key)
    {
        key_ = key;
    }

    const Expression *Value() const
    {
        return value_;
    }

    Expression *Value()
    {
        return value_;
    }

    const Expression *TypeAnnotation() const
    {
        return typeAnnotation_;
    }

    Expression *TypeAnnotation()
    {
        return typeAnnotation_;
    }

    ModifierFlags Modifiers() const
    {
        return modifiers_;
    }

    bool IsStatic() const
    {
        return (modifiers_ & ModifierFlags::STATIC) != 0;
    }

    const ArenaVector<Decorator *> &Decorators() const
    {
        return decorators_;
    }

    bool HasDecorators() const
    {
        return !decorators_.empty();
    }

    bool IsComputed() const
    {
        return isComputed_;
    }

    bool IsPrivate() const
    {
        return key_->IsPrivateIdentifier();
    }

    void SetComputed(bool computed)
    {
        isComputed_ = computed;
    }

    bool IsAutoAccessor() const
    {
        return (modifiers_ & ModifierFlags::ACCESSOR) != 0;
    }

    bool Definite() const
    {
        return definite_;
    }

    bool NeedCompileKey() const
    {
        return !key_->IsLiteral();
    }

    void RemoveValue()
    {
        value_ = nullptr;
    }

    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    checker::Type *Check([[maybe_unused]] checker::Checker *checker) const override;
    void UpdateChildNodes(const NodeUpdater &cb);
    void UpdateSelf(const NodeUpdater &cb, binder::Binder *binder) override;

private:
    Expression *key_;
    Expression *value_;
    Expression *typeAnnotation_;
    ModifierFlags modifiers_;
    ArenaVector<Decorator *> decorators_;
    bool isComputed_;
    bool definite_;
};

}  // namespace panda::es2panda::ir

#endif
