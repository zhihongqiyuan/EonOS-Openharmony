/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "dummyNode.h"

#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void DummyNode::TransformChildren([[maybe_unused]] const NodeTransformer &cb,
                                  [[maybe_unused]] std::string_view transformationName)
{
}

void DummyNode::Iterate([[maybe_unused]] const NodeTraverser &cb) const {}

void DummyNode::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "DummyNode"},
                 {"name", name_},
                 {"indexName", indexName_},
                 {"returnType", returnType_->AsETSTypeReferencePart()->Name()->AsIdentifier()->Name()}});
}

void DummyNode::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add(std::string(name_));
}

void DummyNode::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void DummyNode::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *DummyNode::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *DummyNode::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
