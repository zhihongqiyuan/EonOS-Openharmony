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

#include "etsTypeReference.h"

#include "checker/ETSchecker.h"
#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/ts/tsQualifiedName.h"
#include "ir/ets/etsTypeReferencePart.h"

namespace ark::es2panda::ir {
void ETSTypeReference::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(part_); part_ != transformedNode) {
        part_->SetTransformedNode(transformationName, transformedNode);
        part_ = transformedNode->AsETSTypeReferencePart();
    }
}

void ETSTypeReference::Iterate(const NodeTraverser &cb) const
{
    cb(part_);
}

ir::Identifier *ETSTypeReference::BaseName() const
{
    ir::ETSTypeReferencePart *partIter = part_;

    while (partIter->Previous() != nullptr) {
        partIter = partIter->Previous();
    }

    ir::Expression *baseName = partIter->Name();

    if (baseName->IsIdentifier()) {
        return baseName->AsIdentifier();
    }

    ir::TSQualifiedName *nameIter = baseName->AsTSQualifiedName();

    while (nameIter->Left()->IsTSQualifiedName()) {
        nameIter = nameIter->Left()->AsTSQualifiedName();
    }

    return nameIter->Left()->AsIdentifier();
}

void ETSTypeReference::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSTypeReference"}, {"part", part_}});
}

void ETSTypeReference::Dump(ir::SrcDumper *dumper) const
{
    ASSERT(part_ != nullptr);
    part_->Dump(dumper);
}

void ETSTypeReference::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void ETSTypeReference::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSTypeReference::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSTypeReference::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSTypeReference::GetType(checker::ETSChecker *checker)
{
    if (TsType() == nullptr) {
        SetTsType(part_->GetType(checker));
    }
    return TsType();
}

ETSTypeReference *ETSTypeReference::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const partClone = part_ != nullptr ? part_->Clone(allocator, nullptr)->AsETSTypeReferencePart() : nullptr;

    if (auto *const clone = allocator->New<ETSTypeReference>(partClone); clone != nullptr) {
        if (partClone != nullptr) {
            partClone->SetParent(clone);
        }

        clone->flags_ = flags_;

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
