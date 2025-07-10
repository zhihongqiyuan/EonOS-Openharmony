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

#ifndef ES2PANDA_IR_NAMED_TYPE_H
#define ES2PANDA_IR_NAMED_TYPE_H

#include "ir/typeNode.h"

namespace ark::es2panda::ir {
class Identifier;
class TSTypeParameterInstantiation;

class NamedType : public TypeNode {
public:
    explicit NamedType(Identifier *name) : TypeNode(AstNodeType::NAMED_TYPE), name_(name) {}

    const Identifier *Name() const
    {
        return name_;
    }

    const TSTypeParameterInstantiation *TypeParams() const
    {
        return typeParams_;
    }

    bool IsNullable() const
    {
        return nullable_;
    }

    void SetNullable(bool nullable)
    {
        nullable_ = nullable;
    }

    void SetNext(NamedType *next)
    {
        next_ = next;
    }

    void SetTypeParams(TSTypeParameterInstantiation *typeParams)
    {
        typeParams_ = typeParams;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(AstDumper *dumper) const override;
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
    Identifier *name_;
    TSTypeParameterInstantiation *typeParams_ {};
    NamedType *next_ {};
    bool nullable_ {};
};
}  // namespace ark::es2panda::ir

#endif
