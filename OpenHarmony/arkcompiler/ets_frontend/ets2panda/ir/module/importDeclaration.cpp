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

#include "importDeclaration.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {

void ImportDeclaration::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(source_); source_ != transformedNode) {
        source_->SetTransformedNode(transformationName, transformedNode);
        source_ = transformedNode->AsStringLiteral();
    }

    for (auto *&it : specifiers_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode;
        }
    }
}

void ImportDeclaration::Iterate(const NodeTraverser &cb) const
{
    cb(source_);

    for (auto *it : specifiers_) {
        cb(it);
    }
}

void ImportDeclaration::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ImportDeclaration"}, {"source", source_}, {"specifiers", specifiers_}});
}

void ImportDeclaration::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("import ");
    if (specifiers_.size() == 1 && specifiers_[0]->IsImportNamespaceSpecifier()) {
        specifiers_[0]->Dump(dumper);
    } else {
        dumper->Add("{ ");
        for (auto specifier : specifiers_) {
            specifier->Dump(dumper);
            if (specifier != specifiers_.back()) {
                dumper->Add(", ");
            }
        }
        dumper->Add(" }");
    }

    dumper->Add(" from ");
    source_->Dump(dumper);
    dumper->Add(";");
    dumper->Endl();
}

void ImportDeclaration::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ImportDeclaration::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ImportDeclaration::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ImportDeclaration::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
