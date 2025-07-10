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

#include "prefixAssertionExpression.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/typeNode.h"

namespace ark::es2panda::ir {
void PrefixAssertionExpression::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(type_); type_ != transformedNode) {
        type_->SetTransformedNode(transformationName, transformedNode);
        type_ = static_cast<TypeNode *>(transformedNode);
    }

    if (auto *transformedNode = cb(expr_); expr_ != transformedNode) {
        expr_->SetTransformedNode(transformationName, transformedNode);
        expr_ = transformedNode->AsExpression();
    }
}

void PrefixAssertionExpression::Iterate(const NodeTraverser &cb) const
{
    cb(type_);
    cb(expr_);
}

void PrefixAssertionExpression::Dump(AstDumper *dumper) const
{
    dumper->Add({{"type", "PrefixAssertionExpression"}, {"expression", expr_}, {"type", type_}});
}

void PrefixAssertionExpression::Dump(SrcDumper *dumper) const
{
    dumper->Add("PrefixAssertionExpression");
}

void PrefixAssertionExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void PrefixAssertionExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *PrefixAssertionExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *PrefixAssertionExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
