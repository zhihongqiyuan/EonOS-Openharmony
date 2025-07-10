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

#ifndef ES2PANDA_IR_TS_TYPE_PARAMETER_H
#define ES2PANDA_IR_TS_TYPE_PARAMETER_H

#include "ir/expression.h"

namespace ark::es2panda::ir {
class Identifier;

class TSTypeParameter : public Expression {
public:
    explicit TSTypeParameter(Identifier *name, TypeNode *constraint, TypeNode *defaultType)
        : Expression(AstNodeType::TS_TYPE_PARAMETER), name_(name), constraint_(constraint), defaultType_(defaultType)
    {
    }

    explicit TSTypeParameter(Identifier *name, TypeNode *constraint, TypeNode *defaultType, ModifierFlags flags)
        : Expression(AstNodeType::TS_TYPE_PARAMETER, flags),
          name_(name),
          constraint_(constraint),
          defaultType_(defaultType)
    {
        ASSERT(flags == ModifierFlags::NONE || flags == ModifierFlags::IN || flags == ModifierFlags::OUT);
    }

    const Identifier *Name() const
    {
        return name_;
    }

    Identifier *Name()
    {
        return name_;
    }

    TypeNode *Constraint()
    {
        return constraint_;
    }

    const TypeNode *Constraint() const
    {
        return constraint_;
    }

    void SetConstraint(TypeNode *constraint)
    {
        constraint_ = constraint;
    }

    TypeNode *DefaultType() const
    {
        return defaultType_;
    }

    void SetDefaultType(TypeNode *defaultType)
    {
        defaultType_ = defaultType;
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
    Identifier *name_;
    TypeNode *constraint_;
    TypeNode *defaultType_;
};
}  // namespace ark::es2panda::ir

#endif
