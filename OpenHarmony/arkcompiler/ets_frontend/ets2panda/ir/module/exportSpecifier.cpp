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

#include "exportSpecifier.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ExportSpecifier::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(local_); local_ != transformedNode) {
        local_->SetTransformedNode(transformationName, transformedNode);
        local_ = transformedNode->AsIdentifier();
    }

    if (auto *transformedNode = cb(exported_); exported_ != transformedNode) {
        exported_->SetTransformedNode(transformationName, transformedNode);
        exported_ = transformedNode->AsIdentifier();
    }
}

void ExportSpecifier::Iterate(const NodeTraverser &cb) const
{
    cb(local_);
    cb(exported_);
}

void ExportSpecifier::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ExportSpecifier"}, {"local", local_}, {"exported", exported_}});
}

void ExportSpecifier::Dump(ir::SrcDumper *dumper) const
{
    exported_->Dump(dumper);

    if (local_ != nullptr) {
        dumper->Add(" as ");
        local_->Dump(dumper);
    }
}

void ExportSpecifier::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ExportSpecifier::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ExportSpecifier::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ExportSpecifier::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
