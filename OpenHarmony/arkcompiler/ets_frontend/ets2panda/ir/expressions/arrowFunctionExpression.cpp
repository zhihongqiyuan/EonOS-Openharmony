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

#include "arrowFunctionExpression.h"

#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "checker/ETSchecker.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/TSchecker.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/base/scriptFunction.h"
#include "ir/ets/etsTypeReference.h"
#include "ir/ets/etsTypeReferencePart.h"
#include "ir/expressions/identifier.h"
#include "ir/statements/variableDeclarator.h"

namespace ark::es2panda::ir {
void ArrowFunctionExpression::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(func_); func_ != transformedNode) {
        func_->SetTransformedNode(transformationName, transformedNode);
        func_ = transformedNode->AsScriptFunction();
    }
}

void ArrowFunctionExpression::Iterate(const NodeTraverser &cb) const
{
    cb(func_);
}

void ArrowFunctionExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ArrowFunctionExpression"}, {"function", func_}});
}

void ArrowFunctionExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("(");
    if (func_->IsScriptFunction() && func_->AsScriptFunction()->IsAsyncFunc()) {
        dumper->Add("async ");
    }
    func_->Dump(dumper);
    dumper->Add(")");
}

void ArrowFunctionExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ArrowFunctionExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ArrowFunctionExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ArrowFunctionExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

ArrowFunctionExpression::ArrowFunctionExpression(ArrowFunctionExpression const &other, ArenaAllocator *const allocator)
    : Expression(static_cast<Expression const &>(other))
{
    func_ = other.func_->Clone(allocator, this)->AsScriptFunction();
}

ArrowFunctionExpression *ArrowFunctionExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<ArrowFunctionExpression>(*this, allocator); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

ir::TypeNode *ArrowFunctionExpression::CreateReturnNodeFromType(checker::ETSChecker *checker, checker::Type *returnType)
{
    /*
    Construct a synthetic Node with the correct ts_type_.
    */
    ASSERT(returnType != nullptr);
    auto *ident = checker->AllocNode<ir::Identifier>(util::StringView(""), checker->Allocator());
    auto *const part = checker->AllocNode<ir::ETSTypeReferencePart>(ident);
    auto *returnNode = checker->AllocNode<ir::ETSTypeReference>(part);
    returnNode->SetTsType(returnType);
    return returnNode;
}

ir::TypeNode *ArrowFunctionExpression::CreateTypeAnnotation(checker::ETSChecker *checker)
{
    ir::TypeNode *returnNode = nullptr;
    /*
    There are two scenarios for lambda type inference: defined or undefined return type.
    example code:
    ```
    enum Color { Red, Blue}
    // has Return Type Color
    let x  = () : Color => {return Color.Red}
    // No Return Type Color
    let y  = () => {return Color.Red}
    ```
    */
    if (Function()->ReturnTypeAnnotation() == nullptr) {
        /*
        When lambda expression does not declare a return type, we need to construct the
        declaration node of lambda according to the Function()->Signature()->ReturnType().
        */
        returnNode = CreateReturnNodeFromType(checker, Function()->Signature()->ReturnType());
    } else {
        returnNode = Function()->ReturnTypeAnnotation()->Clone(checker->Allocator(), nullptr);
        returnNode->SetTsType(Function()->ReturnTypeAnnotation()->TsType());
    }

    ArenaVector<ir::Expression *> params {checker->Allocator()->Adapter()};
    checker->CopyParams(Function()->Params(), params);

    auto signature = ir::FunctionSignature(nullptr, std::move(params), returnNode);
    auto *funcType = checker->AllocNode<ir::ETSFunctionType>(std::move(signature), ir::ScriptFunctionFlags::NONE);
    return funcType;
}

bool ArrowFunctionExpression::IsVarFromSubscope(const varbinder::Variable *var) const
{
    // The parameter scope's and the function scope's common ancestor lives outside the function, so we have to check
    // them separetely.
    return Function()->Scope()->IsSuperscopeOf(var->GetScope()) ||
           Function()->Scope()->ParamScope()->IsSuperscopeOf(var->GetScope());
}

}  // namespace ark::es2panda::ir
