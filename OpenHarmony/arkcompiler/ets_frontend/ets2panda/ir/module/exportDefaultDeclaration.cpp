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

#include "exportDefaultDeclaration.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ExportDefaultDeclaration::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(decl_); decl_ != transformedNode) {
        decl_->SetTransformedNode(transformationName, transformedNode);
        decl_ = transformedNode;
    }
}

void ExportDefaultDeclaration::Iterate(const NodeTraverser &cb) const
{
    cb(decl_);
}

void ExportDefaultDeclaration::Dump(ir::AstDumper *dumper) const
{
    dumper->Add(
        {{"type", IsExportEquals() ? "TSExportAssignment" : "ExportDefaultDeclaration"}, {"declaration", decl_}});
}

void ExportDefaultDeclaration::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("ExportDefaultDeclaration");
}

void ExportDefaultDeclaration::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ExportDefaultDeclaration::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ExportDefaultDeclaration::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ExportDefaultDeclaration::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
