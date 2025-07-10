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

#include "tsClassImplements.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/ts/tsTypeParameter.h"
#include "ir/ts/tsTypeParameterInstantiation.h"

namespace ark::es2panda::ir {
void TSClassImplements::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(expression_); expression_ != transformedNode) {
        expression_->SetTransformedNode(transformationName, transformedNode);
        expression_ = transformedNode->AsExpression();
    }
}

void TSClassImplements::Iterate(const NodeTraverser &cb) const
{
    cb(expression_);

    if (typeParameters_ != nullptr) {
        cb(typeParameters_);
    }
}

void TSClassImplements::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSClassImplements"},
                 {"expression", expression_},
                 {"typeParameters", AstDumper::Optional(typeParameters_)}});
}

void TSClassImplements::Dump(ir::SrcDumper *dumper) const
{
    expression_->Dump(dumper);
    ASSERT(typeParameters_ == nullptr);
}

void TSClassImplements::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TSClassImplements::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSClassImplements::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSClassImplements::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
