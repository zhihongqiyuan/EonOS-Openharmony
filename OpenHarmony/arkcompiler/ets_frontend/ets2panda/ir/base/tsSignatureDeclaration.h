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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_TS_SIGNATURE_DECLARATION_H
#define ES2PANDA_PARSER_INCLUDE_AST_TS_SIGNATURE_DECLARATION_H

#include "ir/astNode.h"
#include "ir/base/scriptFunctionSignature.h"
#include "ir/typed.h"

namespace ark::es2panda::checker {
class TSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class TSTypeParameterDeclaration;

class TSSignatureDeclaration : public TypedAstNode {
public:
    enum class TSSignatureDeclarationKind { CALL_SIGNATURE, CONSTRUCT_SIGNATURE };

    TSSignatureDeclaration() = delete;
    ~TSSignatureDeclaration() override = default;

    NO_COPY_SEMANTIC(TSSignatureDeclaration);
    NO_MOVE_SEMANTIC(TSSignatureDeclaration);

    explicit TSSignatureDeclaration(TSSignatureDeclarationKind const kind, FunctionSignature &&signature)
        : TypedAstNode(AstNodeType::TS_SIGNATURE_DECLARATION), kind_(kind), signature_(std::move(signature))
    {
    }
    // NOTE (csabahurton): friend relationship can be removed once there are getters for private fields
    friend class checker::TSAnalyzer;

    [[nodiscard]] bool IsScopeBearer() const noexcept override
    {
        return true;
    }

    [[nodiscard]] varbinder::Scope *Scope() const noexcept override
    {
        return scope_;
    }

    void SetScope(varbinder::Scope *scope)
    {
        ASSERT(scope_ == nullptr);
        scope_ = scope;
    }

    void ClearScope() noexcept override
    {
        scope_ = nullptr;
    }

    [[nodiscard]] const TSTypeParameterDeclaration *TypeParams() const noexcept
    {
        return signature_.TypeParams();
    }

    [[nodiscard]] TSTypeParameterDeclaration *TypeParams()
    {
        return signature_.TypeParams();
    }

    [[nodiscard]] const ArenaVector<Expression *> &Params() const noexcept
    {
        return signature_.Params();
    }

    [[nodiscard]] const TypeNode *ReturnTypeAnnotation() const noexcept
    {
        return signature_.ReturnType();
    }

    TypeNode *ReturnTypeAnnotation() noexcept
    {
        return signature_.ReturnType();
    }

    [[nodiscard]] TSSignatureDeclarationKind Kind() const noexcept
    {
        return kind_;
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
    varbinder::Scope *scope_ {nullptr};
    TSSignatureDeclarationKind kind_;
    ir::FunctionSignature signature_;
};
}  // namespace ark::es2panda::ir

#endif
