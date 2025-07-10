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

#ifndef ES2PANDA_IR_TS_TYPE_PARAMETER_DECLARATION_H
#define ES2PANDA_IR_TS_TYPE_PARAMETER_DECLARATION_H

#include "varbinder/scope.h"
#include "ir/expression.h"
#include "ir/ts/tsTypeParameter.h"

namespace ark::es2panda::ir {

class TSTypeParameterDeclaration : public Expression {
public:
    explicit TSTypeParameterDeclaration(ArenaVector<TSTypeParameter *> &&params, size_t requiredParams)
        : Expression(AstNodeType::TS_TYPE_PARAMETER_DECLARATION),
          params_(std::move(params)),
          requiredParams_(requiredParams)
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
        scope_ = scope;
    }

    void ClearScope() noexcept override
    {
        scope_ = nullptr;
    }

    const ArenaVector<TSTypeParameter *> &Params() const
    {
        return params_;
    }

    void AddParam(TSTypeParameter *param)
    {
        if (requiredParams_ == params_.size() && param->DefaultType() == nullptr) {
            requiredParams_++;
        }
        params_.push_back(param);
    }

    size_t RequiredParams() const
    {
        return requiredParams_;
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
    ArenaVector<TSTypeParameter *> params_;
    varbinder::LocalScope *scope_ {nullptr};
    size_t requiredParams_;
};
}  // namespace ark::es2panda::ir

#endif
