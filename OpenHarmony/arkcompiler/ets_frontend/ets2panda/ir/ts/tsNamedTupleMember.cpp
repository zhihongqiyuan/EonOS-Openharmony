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

#include "tsNamedTupleMember.h"

#include "checker/ETSchecker.h"
#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void TSNamedTupleMember::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(label_); label_ != transformedNode) {
        label_->SetTransformedNode(transformationName, transformedNode);
        label_ = transformedNode->AsExpression();
    }

    if (auto *transformedNode = cb(elementType_); elementType_ != transformedNode) {
        elementType_->SetTransformedNode(transformationName, transformedNode);
        elementType_ = static_cast<TypeNode *>(transformedNode);
    }
}

void TSNamedTupleMember::Iterate(const NodeTraverser &cb) const
{
    cb(label_);
    cb(elementType_);
}

void TSNamedTupleMember::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSNamedTupleMember"},
                 {"elementType", elementType_},
                 {"label", label_},
                 {"optional", AstDumper::Optional(optional_)}});
}

void TSNamedTupleMember::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TSNamedTupleMember");
}

void TSNamedTupleMember::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TSNamedTupleMember::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSNamedTupleMember::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSNamedTupleMember::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
