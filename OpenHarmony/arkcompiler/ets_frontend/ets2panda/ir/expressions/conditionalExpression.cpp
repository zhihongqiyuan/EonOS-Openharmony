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

#include "conditionalExpression.h"

#include "checker/TSchecker.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ConditionalExpression::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(test_); test_ != transformedNode) {
        test_->SetTransformedNode(transformationName, transformedNode);
        test_ = transformedNode->AsExpression();
    }

    if (auto *transformedNode = cb(consequent_); consequent_ != transformedNode) {
        consequent_->SetTransformedNode(transformationName, transformedNode);
        consequent_ = transformedNode->AsExpression();
    }

    if (auto *transformedNode = cb(alternate_); alternate_ != transformedNode) {
        alternate_->SetTransformedNode(transformationName, transformedNode);
        alternate_ = transformedNode->AsExpression();
    }
}

void ConditionalExpression::Iterate(const NodeTraverser &cb) const
{
    cb(test_);
    cb(consequent_);
    cb(alternate_);
}

void ConditionalExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add(
        {{"type", "ConditionalExpression"}, {"test", test_}, {"consequent", consequent_}, {"alternate", alternate_}});
}

void ConditionalExpression::Dump(ir::SrcDumper *dumper) const
{
    ASSERT(test_ != nullptr);
    dumper->Add("(");
    test_->Dump(dumper);
    dumper->Add(" ? ");
    if (consequent_ != nullptr) {
        consequent_->Dump(dumper);
    }
    dumper->Add(" : ");
    if (alternate_ != nullptr) {
        alternate_->Dump(dumper);
    }
    dumper->Add(")");
    if ((parent_ != nullptr) && (parent_->IsBlockStatement() || parent_->IsBlockExpression())) {
        dumper->Add(";");
        dumper->Endl();
    }
}

void ConditionalExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ConditionalExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ConditionalExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ConditionalExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

ConditionalExpression *ConditionalExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const test = test_->Clone(allocator, nullptr)->AsExpression();
    auto *const consequent = consequent_->Clone(allocator, nullptr)->AsExpression();
    auto *const alternate = alternate_->Clone(allocator, nullptr)->AsExpression();

    if (auto *const clone = allocator->New<ConditionalExpression>(test, consequent, alternate); clone != nullptr) {
        test->SetParent(clone);
        consequent->SetParent(clone);
        alternate->SetParent(clone);

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
