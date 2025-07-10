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

#include "tsArrayType.h"

#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "macros.h"

namespace ark::es2panda::ir {
void TSArrayType::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(elementType_); elementType_ != transformedNode) {
        elementType_->SetTransformedNode(transformationName, transformedNode);
        elementType_ = static_cast<TypeNode *>(transformedNode);
    }
}

void TSArrayType::Iterate(const NodeTraverser &cb) const
{
    cb(elementType_);
}

void TSArrayType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSArrayType"}, {"elementType", elementType_}});
}

void TSArrayType::Dump(ir::SrcDumper *dumper) const
{
    ASSERT(elementType_);
    elementType_->Dump(dumper);
    dumper->Add("[]");
}

void TSArrayType::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TSArrayType::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSArrayType::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSArrayType::GetType([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->Allocator()->New<checker::ArrayType>(elementType_->GetType(checker));
}

checker::Type *TSArrayType::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSArrayType::GetType(checker::ETSChecker *checker)
{
    checker::Type *type = checker->CreateETSArrayType(elementType_->GetType(checker));
    if (IsReadonlyType()) {
        type = checker->GetReadonlyType(type);
    }
    SetTsType(type);
    return type;
}

TSArrayType *TSArrayType::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const elementTypeClone = elementType_ != nullptr ? elementType_->Clone(allocator, nullptr) : nullptr;

    if (auto *const clone = allocator->New<TSArrayType>(elementTypeClone); clone != nullptr) {
        clone->AddModifier(flags_);

        if (elementTypeClone != nullptr) {
            elementTypeClone->SetParent(clone);
        }

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

}  // namespace ark::es2panda::ir
