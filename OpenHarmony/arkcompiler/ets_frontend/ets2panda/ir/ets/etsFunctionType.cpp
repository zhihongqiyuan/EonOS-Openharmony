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

#include "etsFunctionType.h"

#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ETSFunctionType::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    signature_.TransformChildren(cb, transformationName);
}

void ETSFunctionType::Iterate(const NodeTraverser &cb) const
{
    signature_.Iterate(cb);
}

void ETSFunctionType::Dump(ir::AstDumper *dumper) const
{
    const char *throwMarker = nullptr;
    if (IsThrowing()) {
        throwMarker = "throws";
    } else if (IsRethrowing()) {
        throwMarker = "rethrows";
    }
    dumper->Add({{"type", "ETSFunctionType"},
                 {"params", signature_.Params()},
                 {"typeParameters", AstDumper::Optional(signature_.TypeParams())},
                 {"returnType", signature_.ReturnType()},
                 {"throwMarker", AstDumper::Optional(throwMarker)}});
}

void ETSFunctionType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("((");
    for (auto *param : Params()) {
        param->Dump(dumper);
        if (param != Params().back()) {
            dumper->Add(", ");
        }
    }
    dumper->Add(")");

    if (TypeParams() != nullptr) {
        TypeParams()->Dump(dumper);
    }

    if (ReturnType() != nullptr) {
        dumper->Add("=> ");
        ReturnType()->Dump(dumper);
    }

    if (IsThrowing()) {
        dumper->Add(" throws");
    } else if (IsRethrowing()) {
        dumper->Add(" rethrows");
    }

    dumper->Add(")");
}

void ETSFunctionType::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ETSFunctionType::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSFunctionType::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSFunctionType::GetType([[maybe_unused]] checker::TSChecker *checker)
{
    return nullptr;
}

checker::Type *ETSFunctionType::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSFunctionType::GetType(checker::ETSChecker *checker)
{
    return Check(checker);
}

ETSFunctionType *ETSFunctionType::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    ArenaVector<Expression *> paramsClone(allocator->Adapter());

    for (auto *const param : signature_.Params()) {
        paramsClone.emplace_back(param->Clone(allocator, nullptr)->AsExpression());
    }

    auto *const typeParamsClone =
        signature_.TypeParams() != nullptr
            ? signature_.TypeParams()->Clone(allocator, nullptr)->AsTSTypeParameterDeclaration()
            : nullptr;
    auto *const returnTypeClone =
        signature_.ReturnType() != nullptr ? signature_.ReturnType()->Clone(allocator, nullptr)->AsTypeNode() : nullptr;

    if (auto *const clone = allocator->New<ETSFunctionType>(
            FunctionSignature(typeParamsClone, std::move(paramsClone), returnTypeClone), funcFlags_);
        clone != nullptr) {
        if (typeParamsClone != nullptr) {
            typeParamsClone->SetParent(clone);
        }

        if (returnTypeClone != nullptr) {
            returnTypeClone->SetParent(clone);
        }

        for (auto *param : clone->Params()) {
            param->SetParent(clone);
        }

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        // Reset scope for clone
        // Using old scopes with clone may lead to incorrect ast-structure
        clone->SetScope(nullptr);

        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
