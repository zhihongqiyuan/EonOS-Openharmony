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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_TS_PROPERTY_SIGNATURE_H
#define ES2PANDA_PARSER_INCLUDE_AST_TS_PROPERTY_SIGNATURE_H

#include "ir/expression.h"

namespace ark::es2panda::ir {
class TypeNode;

class TSPropertySignature : public AnnotatedAstNode {
public:
    TSPropertySignature() = delete;
    ~TSPropertySignature() override = default;

    NO_COPY_SEMANTIC(TSPropertySignature);
    NO_MOVE_SEMANTIC(TSPropertySignature);

    explicit TSPropertySignature(Expression *key, TypeNode *typeAnnotation, bool computed, bool optional, bool readonly)
        : AnnotatedAstNode(AstNodeType::TS_PROPERTY_SIGNATURE, typeAnnotation),
          key_(key),
          computed_(computed),
          optional_(optional),
          readonly_(readonly)
    {
    }

    [[nodiscard]] const Expression *Key() const noexcept
    {
        return key_;
    }

    [[nodiscard]] Expression *Key() noexcept
    {
        return key_;
    }

    [[nodiscard]] bool Computed() const noexcept
    {
        return computed_;
    }

    [[nodiscard]] bool Optional() const noexcept
    {
        return optional_;
    }

    [[nodiscard]] bool Readonly() const noexcept
    {
        return readonly_;
    }

    [[nodiscard]] TSPropertySignature *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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
    Expression *key_;
    bool computed_;
    bool optional_;
    bool readonly_;
};
}  // namespace ark::es2panda::ir

#endif
