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

#include "importSpecifier.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ImportSpecifier::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (local_ != nullptr) {
        if (auto *transformedNode = cb(local_); local_ != transformedNode) {
            local_->SetTransformedNode(transformationName, transformedNode);
            local_ = transformedNode->AsIdentifier();
        }
    }

    if (auto *transformedNode = cb(imported_); imported_ != transformedNode) {
        imported_->SetTransformedNode(transformationName, transformedNode);
        imported_ = transformedNode->AsIdentifier();
    }
}

void ImportSpecifier::Iterate(const NodeTraverser &cb) const
{
    if (local_ != nullptr) {
        cb(local_);
    }
    cb(imported_);
}

void ImportSpecifier::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ImportSpecifier"}, {"local", ir::AstDumper::Optional(local_)}, {"imported", imported_}});
}

void ImportSpecifier::Dump(ir::SrcDumper *dumper) const
{
    imported_->Dump(dumper);
    if (local_ != nullptr) {
        dumper->Add(" as ");
        local_->Dump(dumper);
    }
}

void ImportSpecifier::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void ImportSpecifier::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ImportSpecifier::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ImportSpecifier::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
