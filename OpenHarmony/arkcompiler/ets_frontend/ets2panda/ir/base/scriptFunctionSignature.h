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

#ifndef ES2PANDA_COMPILER_CORE_SCRIPT_FUNCTION_SIGNATURE_H
#define ES2PANDA_COMPILER_CORE_SCRIPT_FUNCTION_SIGNATURE_H

#include "ir/astNode.h"
#include "ir/ets/etsParameterExpression.h"

namespace ark::es2panda::ir {
class TSTypeParameterDeclaration;
class TypeNode;

class FunctionSignature {
public:
    using FunctionParams = ArenaVector<Expression *>;

    FunctionSignature(TSTypeParameterDeclaration *typeParams, FunctionParams &&params, TypeNode *returnTypeAnnotation)
        : typeParams_(typeParams), params_(std::move(params)), returnTypeAnnotation_(returnTypeAnnotation)
    {
    }

    const FunctionParams &Params() const
    {
        return params_;
    }

    FunctionParams &Params()
    {
        return params_;
    }

    TSTypeParameterDeclaration *TypeParams()
    {
        return typeParams_;
    }

    const TSTypeParameterDeclaration *TypeParams() const
    {
        return typeParams_;
    }

    TypeNode *ReturnType()
    {
        return returnTypeAnnotation_;
    }

    void SetReturnType(TypeNode *type)
    {
        returnTypeAnnotation_ = type;
    }

    const TypeNode *ReturnType() const
    {
        return returnTypeAnnotation_;
    }

    [[nodiscard]] size_t DefaultParamIndex() const
    {
        for (size_t i = 0; i < params_.size(); i++) {
            if (params_[i]->AsETSParameterExpression()->IsDefault()) {
                return i;
            }
        }
        return params_.size();
    }

    void Iterate(const NodeTraverser &cb) const;

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName);

    [[nodiscard]] FunctionSignature Clone(ArenaAllocator *allocator);

private:
    TSTypeParameterDeclaration *typeParams_;
    ArenaVector<Expression *> params_;
    TypeNode *returnTypeAnnotation_;
};

}  // namespace ark::es2panda::ir

#endif
