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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_META_PROPERTY_H
#define ES2PANDA_PARSER_INCLUDE_AST_META_PROPERTY_H

#include "ir/expression.h"

namespace ark::es2panda::ir {
class MetaProperty : public Expression {
private:
    struct Tag {};

public:
    MetaProperty() = delete;
    ~MetaProperty() override = default;

    NO_COPY_SEMANTIC(MetaProperty);
    NO_MOVE_SEMANTIC(MetaProperty);

    enum class MetaPropertyKind { NEW_TARGET, IMPORT_META };

    explicit MetaProperty(MetaPropertyKind const kind) : Expression(AstNodeType::META_PROPERTY_EXPRESSION), kind_(kind)
    {
    }

    [[nodiscard]] MetaPropertyKind Kind() const noexcept
    {
        return kind_;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;

    [[nodiscard]] MetaProperty *Clone(ArenaAllocator *allocator, AstNode *parent) override;

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
    MetaPropertyKind kind_;
};
}  // namespace ark::es2panda::ir

#endif
