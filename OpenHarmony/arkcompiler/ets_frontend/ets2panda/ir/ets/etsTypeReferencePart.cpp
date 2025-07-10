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

#include "etsTypeReferencePart.h"

#include "checker/ETSchecker.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ETSTypeReferencePart::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(name_); name_ != transformedNode) {
        name_->SetTransformedNode(transformationName, transformedNode);
        name_ = transformedNode->AsExpression();
    }

    if (typeParams_ != nullptr) {
        if (auto *transformedNode = cb(typeParams_); typeParams_ != transformedNode) {
            typeParams_->SetTransformedNode(transformationName, transformedNode);
            typeParams_ = transformedNode->AsTSTypeParameterInstantiation();
        }
    }

    if (prev_ != nullptr) {
        if (auto *transformedNode = cb(prev_); prev_ != transformedNode) {
            prev_->SetTransformedNode(transformationName, transformedNode);
            prev_ = transformedNode->AsETSTypeReferencePart();
        }
    }
}

void ETSTypeReferencePart::Iterate(const NodeTraverser &cb) const
{
    cb(name_);

    if (typeParams_ != nullptr) {
        cb(typeParams_);
    }

    if (prev_ != nullptr) {
        cb(prev_);
    }
}

void ETSTypeReferencePart::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSTypeReferencePart"},
                 {"name", name_},
                 {"typeParams", AstDumper::Optional(typeParams_)},
                 {"previous", AstDumper::Optional(prev_)}});
}

void ETSTypeReferencePart::Dump(ir::SrcDumper *dumper) const
{
    ASSERT(name_ != nullptr);
    name_->Dump(dumper);
    if (typeParams_ != nullptr) {
        typeParams_->Dump(dumper);
    }
}

void ETSTypeReferencePart::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void ETSTypeReferencePart::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSTypeReferencePart::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSTypeReferencePart::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSTypeReferencePart::HandleInternalTypes(checker::ETSChecker *const checker,
                                                         const Identifier *const ident)
{
    if ((ident->Variable() != nullptr) && (ident->Variable()->Declaration()->IsTypeAliasDecl())) {
        return checker->HandleTypeAlias(name_, typeParams_);
    }

    if (ident->Name() == compiler::Signatures::UNDEFINED) {
        return checker->GlobalETSUndefinedType();
    }

    if (ident->Name() == compiler::Signatures::NULL_LITERAL) {
        return checker->GlobalETSNullType();
    }

    if (ident->Name() == compiler::Signatures::NEVER_TYPE_NAME) {
        return checker->GlobalETSNeverType();
    }

    if (ident->Name() == compiler::Signatures::READONLY_TYPE_NAME ||
        ident->Name() == compiler::Signatures::REQUIRED_TYPE_NAME) {
        return checker->HandleUtilityTypeParameterNode(typeParams_, ident->Name().Utf8());
    }

    if (ident->Name() == compiler::Signatures::PARTIAL_TYPE_NAME) {
        auto *baseType = checker->HandleUtilityTypeParameterNode(typeParams_, ident->Name().Utf8());
        if (baseType->IsETSObjectType() && !baseType->AsETSObjectType()->TypeArguments().empty()) {
            // we treat Partial<A<T,D>> class as a different copy from A<T,D> now,
            // but not a generic type param for Partial<>
            for (auto &typeRef : typeParams_->Params()) {
                checker::InstantiationContext ctx(checker, baseType->AsETSObjectType(),
                                                  typeRef->AsETSTypeReference()->Part()->typeParams_, Start());
                baseType = ctx.Result();
            }
        }
        return baseType;
    }

    return nullptr;
}

checker::Type *ETSTypeReferencePart::GetType(checker::ETSChecker *checker)
{
    if (prev_ == nullptr) {
        if (name_->IsIdentifier()) {
            SetTsType(HandleInternalTypes(checker, name_->AsIdentifier()));
        }

        if (TsType() == nullptr) {
            checker::Type *baseType = checker->GetReferencedTypeBase(name_);

            ASSERT(baseType != nullptr);
            if (baseType->IsETSObjectType()) {
                checker::InstantiationContext ctx(checker, baseType->AsETSObjectType(), typeParams_, Start());
                SetTsType(ctx.Result());
            } else {
                SetTsType(baseType);
            }
        }
    } else {
        checker::Type *baseType = prev_->GetType(checker);
        SetTsType(checker->GetReferencedTypeFromBase(baseType, name_));
    }
    return TsType();
}

ETSTypeReferencePart *ETSTypeReferencePart::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const nameClone = name_ != nullptr ? name_->Clone(allocator, nullptr)->AsExpression() : nullptr;
    auto *const typeParamsClone =
        typeParams_ != nullptr ? typeParams_->Clone(allocator, nullptr)->AsTSTypeParameterInstantiation() : nullptr;
    auto *const prevClone = prev_ != nullptr ? prev_->Clone(allocator, nullptr)->AsETSTypeReferencePart() : nullptr;
    if (auto *const clone = allocator->New<ETSTypeReferencePart>(nameClone, typeParamsClone, prevClone);
        clone != nullptr) {
        if (nameClone != nullptr) {
            nameClone->SetParent(clone);
        }

        if (typeParamsClone != nullptr) {
            typeParamsClone->SetParent(clone);
        }

        if (prevClone != nullptr) {
            prevClone->SetParent(clone);
        }

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
