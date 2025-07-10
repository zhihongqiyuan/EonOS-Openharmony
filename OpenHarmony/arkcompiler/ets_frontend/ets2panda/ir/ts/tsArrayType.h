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

#ifndef ES2PANDA_IR_TS_ARRAY_TYPE_H
#define ES2PANDA_IR_TS_ARRAY_TYPE_H

#include "ir/typeNode.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
class TSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class TSArrayType : public TypeNode {
public:
    explicit TSArrayType(TypeNode *elementType) : TypeNode(AstNodeType::TS_ARRAY_TYPE), elementType_(elementType) {}

    const TypeNode *ElementType() const
    {
        return elementType_;
    }

    // NOTE (vivienvoros): these friend relationships can be removed once there are getters for private fields
    friend class checker::TSAnalyzer;
    friend class checker::ETSAnalyzer;

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *GetType([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override;
    checker::Type *GetType([[maybe_unused]] checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

    [[nodiscard]] TSArrayType *Clone(ArenaAllocator *allocator, AstNode *parent) override;

private:
    TypeNode *elementType_;
};
}  // namespace ark::es2panda::ir

#endif
