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

#ifndef ES2PANDA_IR_ETS_FUNCTION_TYPE_H
#define ES2PANDA_IR_ETS_FUNCTION_TYPE_H

#include "ir/typeNode.h"
#include "ir/base/scriptFunctionSignature.h"

namespace ark::es2panda::checker {
class ETSAnalyzer;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class TSTypeParameterDeclaration;

class ETSFunctionType : public TypeNode {
public:
    explicit ETSFunctionType(FunctionSignature &&signature, ir::ScriptFunctionFlags funcFlags)
        : TypeNode(AstNodeType::ETS_FUNCTION_TYPE), signature_(std::move(signature)), funcFlags_(funcFlags)
    {
    }

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
        scope_ = scope;
    }

    void ClearScope() noexcept override
    {
        scope_ = nullptr;
    }

    [[nodiscard]] FunctionSignature IrSignature() noexcept
    {
        return signature_;
    }

    const TSTypeParameterDeclaration *TypeParams() const
    {
        return signature_.TypeParams();
    }

    TSTypeParameterDeclaration *TypeParams()
    {
        return signature_.TypeParams();
    }

    const ArenaVector<Expression *> &Params() const
    {
        return signature_.Params();
    }

    const TypeNode *ReturnType() const
    {
        return signature_.ReturnType();
    }

    TypeNode *ReturnType()
    {
        return signature_.ReturnType();
    }

    ir::TSInterfaceDeclaration *FunctionalInterface()
    {
        return functionalInterface_;
    }

    const ir::TSInterfaceDeclaration *FunctionalInterface() const
    {
        return functionalInterface_;
    }

    void SetFunctionalInterface(ir::TSInterfaceDeclaration *functionalInterface)
    {
        functionalInterface_ = functionalInterface;
    }

    ir::ScriptFunctionFlags Flags()
    {
        return funcFlags_;
    }

    bool IsThrowing() const
    {
        return (funcFlags_ & ir::ScriptFunctionFlags::THROWS) != 0;
    }

    bool IsRethrowing() const
    {
        return (funcFlags_ & ir::ScriptFunctionFlags::RETHROWS) != 0;
    }

    size_t DefaultParamIndex() const
    {
        return signature_.DefaultParamIndex();
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *GetType([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;
    checker::Type *GetType([[maybe_unused]] checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

    [[nodiscard]] ETSFunctionType *Clone(ArenaAllocator *allocator, AstNode *parent) override;

private:
    varbinder::Scope *scope_ {};
    FunctionSignature signature_;
    ir::TSInterfaceDeclaration *functionalInterface_ {};
    ir::ScriptFunctionFlags funcFlags_;
};
}  // namespace ark::es2panda::ir

#endif
