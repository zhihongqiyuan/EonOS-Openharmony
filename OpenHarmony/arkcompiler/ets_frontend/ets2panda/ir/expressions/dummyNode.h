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

#ifndef ES2PANDA_IR_EXPRESSION_DUMMYNODE_H
#define ES2PANDA_IR_EXPRESSION_DUMMYNODE_H

#include "ir/astNode.h"
#include "ir/visitor/AstVisitor.h"
#include "util/ustring.h"

namespace ark::es2panda::ir {

using ENUMBITOPS_OPERATORS;

enum class DummyNodeFlag : uint32_t { NONE = 0U, INDEXER = 1U << 0U };

}  // namespace ark::es2panda::ir

template <>
struct enumbitops::IsAllowedType<ark::es2panda::ir::DummyNodeFlag> : std::true_type {
};

namespace ark::es2panda::ir {

class DummyNode : public AstNode {
public:
    DummyNode() = delete;
    ~DummyNode() override = default;

    NO_COPY_SEMANTIC(DummyNode);
    NO_MOVE_SEMANTIC(DummyNode);

public:
    explicit DummyNode(util::StringView const name, util::StringView indexName, TypeNode *returnType,
                       DummyNodeFlag flag)
        : AstNode(AstNodeType::DUMMYNODE), name_(name), indexName_(indexName), returnType_(returnType), flag_(flag)
    {
    }

    [[nodiscard]] bool IsDeclareIndexer() const noexcept
    {
        return (flag_ & ir::DummyNodeFlag::INDEXER) != 0;
    }

    void SetAmbientIndexer(bool const ambientIndexer) noexcept
    {
        if (ambientIndexer) {
            flag_ |= DummyNodeFlag::INDEXER;
        } else {
            flag_ &= ~DummyNodeFlag::INDEXER;
        }
    }

    [[nodiscard]] util::StringView GetIndexName() const
    {
        return indexName_;
    }

    void SetIndexName(const util::StringView &indexName)
    {
        indexName_ = indexName;
    }

    [[nodiscard]] TypeNode *GetReturnTypeLiteral() const
    {
        return returnType_;
    }

    void SetReturnTypeLiteral(TypeNode *returnType)
    {
        returnType_ = returnType;
    }

    util::StringView Name() const
    {
        return name_;
    }

    bool operator==(const DummyNode &node) const
    {
        return name_.Is(std::string(node.Name().Bytes()));
    }

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
    util::StringView name_;
    util::StringView indexName_;
    TypeNode *returnType_;
    DummyNodeFlag flag_ {DummyNodeFlag::NONE};
};
}  // namespace ark::es2panda::ir

#endif
