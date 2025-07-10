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

#include "tsParameterProperty.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/expression.h"

namespace ark::es2panda::ir {
void TSParameterProperty::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(parameter_); parameter_ != transformedNode) {
        parameter_->SetTransformedNode(transformationName, transformedNode);
        parameter_ = transformedNode->AsExpression();
    }
}

void TSParameterProperty::Iterate(const NodeTraverser &cb) const
{
    cb(parameter_);
}

void TSParameterProperty::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSParameterProperty"},
                 {"accessibility", accessibility_ == AccessibilityOption::PUBLIC      ? "public"
                                   : accessibility_ == AccessibilityOption::PRIVATE   ? "private"
                                   : accessibility_ == AccessibilityOption::PROTECTED ? "protected"
                                                                                      : "undefined"},
                 {"readonly", readonly_},
                 {"static", static_},
                 {"export", export_},
                 {"parameter", parameter_}});
}

void TSParameterProperty::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TSParameterProperty");
}

void TSParameterProperty::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void TSParameterProperty::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSParameterProperty::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSParameterProperty::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
