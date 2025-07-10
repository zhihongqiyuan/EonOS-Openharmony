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

#include "ir/base/scriptFunctionSignature.h"
#include "ir/ts/tsTypeParameterDeclaration.h"
#include "ir/typeNode.h"

namespace ark::es2panda::ir {

void FunctionSignature::Iterate(const NodeTraverser &cb) const
{
    if (typeParams_ != nullptr) {
        cb(typeParams_);
    }

    for (auto *it : Params()) {
        cb(it);
    }

    if (returnTypeAnnotation_ != nullptr) {
        cb(returnTypeAnnotation_);
    }
}

void FunctionSignature::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (typeParams_ != nullptr) {
        if (auto *transformedNode = cb(typeParams_); typeParams_ != transformedNode) {
            typeParams_->SetTransformedNode(transformationName, transformedNode);
            typeParams_ = transformedNode->AsTSTypeParameterDeclaration();
        }
    }

    for (auto *&it : params_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsExpression();
        }
    }

    if (returnTypeAnnotation_ != nullptr) {
        if (auto *transformedNode = cb(returnTypeAnnotation_); returnTypeAnnotation_ != transformedNode) {
            returnTypeAnnotation_->SetTransformedNode(transformationName, transformedNode);
            returnTypeAnnotation_ = static_cast<TypeNode *>(transformedNode);
        }
    }
}

FunctionSignature FunctionSignature::Clone(ArenaAllocator *allocator)
{
    ArenaVector<Expression *> clonedParams(allocator->Adapter());

    for (auto *const param : params_) {
        clonedParams.emplace_back(param->Clone(allocator, param->Parent())->AsExpression());
    }

    auto *const typeParamClone =
        typeParams_ != nullptr ? typeParams_->Clone(allocator, nullptr)->AsTSTypeParameterDeclaration() : nullptr;
    auto *const returnTypeAnnotationClone =
        returnTypeAnnotation_ != nullptr ? returnTypeAnnotation_->Clone(allocator, nullptr) : nullptr;

    return {typeParamClone, std::move(clonedParams), returnTypeAnnotationClone};
}
}  // namespace ark::es2panda::ir
