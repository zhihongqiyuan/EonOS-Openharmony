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

#include "namespaceDefinition.h"

#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/base/classStaticBlock.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/scriptFunction.h"
#include "ir/expressions/identifier.h"
#include "ir/ts/tsClassImplements.h"

namespace ark::es2panda::ir {
const FunctionExpression *NamespaceDefinition::Ctor() const
{
    return ctor_ != nullptr ? ctor_->Value()->AsFunctionExpression() : nullptr;
}

void NamespaceDefinition::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (ident_ != nullptr) {
        if (auto *transformedNode = cb(ident_); ident_ != transformedNode) {
            ident_->SetTransformedNode(transformationName, transformedNode);
            ident_ = transformedNode->AsIdentifier();
        }
    }

    for (auto *&it : body_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode;
        }
    }
}

void NamespaceDefinition::Iterate(const NodeTraverser &cb) const
{
    if (ident_ != nullptr) {
        cb(ident_);
    }

    // NOLINTNEXTLINE(modernize-loop-convert)
    for (size_t ix = 0; ix < body_.size(); ix++) {
        cb(body_[ix]);
    }
}

void NamespaceDefinition::SetIdent(ir::Identifier *ident) noexcept
{
    ident_ = ident;
    if (ident_ != nullptr) {
        ident_->SetParent(this);
    }
}

void NamespaceDefinition::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"id", AstDumper::Nullish(ident_)}, {"body", body_}});
}

void NamespaceDefinition::Dump([[maybe_unused]] ir::SrcDumper *dumper) const
{
    ASSERT(ident_ != nullptr);

    if (IsExported()) {
        dumper->Add("export ");
    }

    dumper->Add(" {");
    if (!body_.empty()) {
        dumper->IncrIndent();
        dumper->Endl();
        for (auto elem : body_) {
            elem->Dump(dumper);
            if (elem == body_.back()) {
                dumper->DecrIndent();
            }
            dumper->Endl();
        }
    }
    dumper->Add("}");
    dumper->Endl();
}

void NamespaceDefinition::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void NamespaceDefinition::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *NamespaceDefinition::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *NamespaceDefinition::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

}  // namespace ark::es2panda::ir
