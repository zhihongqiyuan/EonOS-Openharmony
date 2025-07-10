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

#include "classExpression.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ClassExpression::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *transformedNode = cb(def_); def_ != transformedNode) {
        def_->SetTransformedNode(transformationName, transformedNode);
        def_ = transformedNode->AsClassDefinition();
    }
}

void ClassExpression::Iterate(const NodeTraverser &cb) const
{
    cb(def_);
}

void ClassExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ClassExpression"}, {"definition", def_}});
}

void ClassExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("ClassExpression");
}

void ClassExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ClassExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ClassExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ClassExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

ClassExpression *ClassExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const def = def_ != nullptr ? def_->Clone(allocator, nullptr)->AsClassDefinition() : nullptr;

    if (auto *const clone = allocator->New<ClassExpression>(def); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        if (def != nullptr) {
            def->SetParent(clone);
        }
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
