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

#include "exportAllDeclaration.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ExportAllDeclaration::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(source_); source_ != transformedNode) {
        source_->SetTransformedNode(transformationName, transformedNode);
        source_ = transformedNode->AsStringLiteral();
    }

    if (exported_ != nullptr) {
        if (auto *transformedNode = cb(exported_); exported_ != transformedNode) {
            exported_->SetTransformedNode(transformationName, transformedNode);
            exported_ = transformedNode->AsIdentifier();
        }
    }
}

void ExportAllDeclaration::Iterate(const NodeTraverser &cb) const
{
    cb(source_);

    if (exported_ != nullptr) {
        cb(exported_);
    }
}

void ExportAllDeclaration::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ExportAllDeclaration"}, {"source", source_}, {"exported", AstDumper::Nullish(exported_)}});
}

void ExportAllDeclaration::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("ExportAllDeclaration");
}

void ExportAllDeclaration::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ExportAllDeclaration::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ExportAllDeclaration::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ExportAllDeclaration::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
