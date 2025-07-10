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

#include "etsStructDeclaration.h"

#include "checker/TSchecker.h"
#include "compiler/base/lreference.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/base/classDefinition.h"
#include "ir/base/decorator.h"

namespace ark::es2panda::ir {
void ETSStructDeclaration::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    for (auto *&it : decorators_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsDecorator();
        }
    }

    if (auto *transformedNode = cb(def_); def_ != transformedNode) {
        def_->SetTransformedNode(transformationName, transformedNode);
        def_ = transformedNode->AsClassDefinition();
    }
}

void ETSStructDeclaration::Iterate(const NodeTraverser &cb) const
{
    for (auto *it : decorators_) {
        cb(it);
    }

    cb(def_);
}

void ETSStructDeclaration::Dump(ir::AstDumper *dumper) const
{
    dumper->Add(
        {{"type", "ETSStructDeclaration"}, {"definition", def_}, {"decorators", AstDumper::Optional(decorators_)}});
}

void ETSStructDeclaration::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("ETSStructDeclaration");
}

void ETSStructDeclaration::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ETSStructDeclaration::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSStructDeclaration::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSStructDeclaration::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

ETSStructDeclaration *ETSStructDeclaration::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const def = def_ != nullptr ? def_->Clone(allocator, nullptr)->AsClassDefinition() : nullptr;

    if (auto *const clone = allocator->New<ETSStructDeclaration>(def, allocator); clone != nullptr) {
        for (auto *const decorator : decorators_) {
            clone->AddDecorator(decorator->Clone(allocator, clone));
        }

        if (def != nullptr) {
            def->SetParent(clone);
        }

        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
