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

#include "forUpdateStatement.h"

#include "compiler/base/condition.h"
#include "compiler/base/lreference.h"
#include "compiler/core/labelTarget.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "checker/TSchecker.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ForUpdateStatement::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (init_ != nullptr) {
        if (auto *transformedNode = cb(init_); init_ != transformedNode) {
            init_->SetTransformedNode(transformationName, transformedNode);
            init_ = transformedNode;
        }
    }

    if (test_ != nullptr) {
        if (auto *transformedNode = cb(test_); test_ != transformedNode) {
            test_->SetTransformedNode(transformationName, transformedNode);
            test_ = transformedNode->AsExpression();
        }
    }

    if (update_ != nullptr) {
        if (auto *transformedNode = cb(update_); update_ != transformedNode) {
            update_->SetTransformedNode(transformationName, transformedNode);
            update_ = transformedNode->AsExpression();
        }
    }

    if (auto *transformedNode = cb(body_); body_ != transformedNode) {
        body_->SetTransformedNode(transformationName, transformedNode);
        body_ = transformedNode->AsStatement();
    }
}

void ForUpdateStatement::Iterate(const NodeTraverser &cb) const
{
    if (init_ != nullptr) {
        cb(init_);
    }
    if (test_ != nullptr) {
        cb(test_);
    }
    if (update_ != nullptr) {
        cb(update_);
    }

    cb(body_);
}

void ForUpdateStatement::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ForUpdateStatement"},
                 {"init", AstDumper::Nullish(init_)},
                 {"test", AstDumper::Nullish(test_)},
                 {"update", AstDumper::Nullish(update_)},
                 {"body", body_}});
}

void ForUpdateStatement::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("for ");
    dumper->Add("(");
    if (init_ != nullptr) {
        init_->Dump(dumper);
    }
    dumper->Add(";");
    if (test_ != nullptr) {
        test_->Dump(dumper);
    }
    dumper->Add(";");
    if (update_ != nullptr) {
        update_->Dump(dumper);
    }
    dumper->Add(") ");
    dumper->Add("{");
    if (body_ != nullptr) {
        dumper->IncrIndent();
        dumper->Endl();
        body_->Dump(dumper);
        dumper->DecrIndent();
        dumper->Endl();
    }
    dumper->Add("}");
}

void ForUpdateStatement::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ForUpdateStatement::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ForUpdateStatement::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ForUpdateStatement::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
