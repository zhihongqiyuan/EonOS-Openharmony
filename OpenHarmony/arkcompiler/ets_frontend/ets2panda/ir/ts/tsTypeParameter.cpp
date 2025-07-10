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

#include "tsTypeParameter.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/typeNode.h"
#include "ir/expressions/identifier.h"

namespace ark::es2panda::ir {
void TSTypeParameter::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(name_); name_ != transformedNode) {
        name_->SetTransformedNode(transformationName, transformedNode);
        name_ = transformedNode->AsIdentifier();
    }

    if (constraint_ != nullptr) {
        if (auto *transformedNode = cb(constraint_); constraint_ != transformedNode) {
            constraint_->SetTransformedNode(transformationName, transformedNode);
            constraint_ = static_cast<TypeNode *>(transformedNode);
        }
    }

    if (defaultType_ != nullptr) {
        if (auto *transformedNode = cb(defaultType_); defaultType_ != transformedNode) {
            defaultType_->SetTransformedNode(transformationName, transformedNode);
            defaultType_ = static_cast<TypeNode *>(transformedNode);
        }
    }
}

void TSTypeParameter::Iterate(const NodeTraverser &cb) const
{
    cb(name_);

    if (constraint_ != nullptr) {
        cb(constraint_);
    }

    if (defaultType_ != nullptr) {
        cb(defaultType_);
    }
}

void TSTypeParameter::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({
        {"type", "TSTypeParameter"},
        {"name", name_},
        {"constraint", AstDumper::Optional(constraint_)},
        {"default", AstDumper::Optional(defaultType_)},
        {"in", AstDumper::Optional(IsIn())},
        {"out", AstDumper::Optional(IsOut())},
    });
}

void TSTypeParameter::Dump(ir::SrcDumper *dumper) const
{
    if (IsIn()) {
        dumper->Add("in ");
    }
    if (IsOut()) {
        dumper->Add("out ");
    }

    name_->Dump(dumper);

    if (defaultType_ != nullptr) {
        dumper->Add(" = ");
        defaultType_->Dump(dumper);
    }
    if (constraint_ != nullptr) {
        dumper->Add(" extends ");
        constraint_->Dump(dumper);
    }
}

void TSTypeParameter::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void TSTypeParameter::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSTypeParameter::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSTypeParameter::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
