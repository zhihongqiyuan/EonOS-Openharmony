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

#include "etsWildcardType.h"

#include "checker/ETSchecker.h"
#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/ets/etsTypeReference.h"

namespace ark::es2panda::ir {
void ETSWildcardType::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (typeReference_ != nullptr) {
        if (auto *transformedNode = cb(typeReference_); typeReference_ != transformedNode) {
            typeReference_->SetTransformedNode(transformationName, transformedNode);
            typeReference_ = transformedNode->AsETSTypeReference();
        }
    }
}

void ETSWildcardType::Iterate(const NodeTraverser &cb) const
{
    if (typeReference_ != nullptr) {
        cb(typeReference_);
    }
}

void ETSWildcardType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSWildcardType"},
                 {"typeReference", AstDumper::Optional(typeReference_)},
                 {"in", AstDumper::Optional(IsIn())},
                 {"out", AstDumper::Optional(IsOut())}});
}

void ETSWildcardType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("ETSWildcardType");
}

void ETSWildcardType::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ETSWildcardType::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSWildcardType::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSWildcardType::GetType([[maybe_unused]] checker::TSChecker *checker)
{
    return nullptr;
}

checker::Type *ETSWildcardType::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSWildcardType::GetType([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GlobalWildcardType();
}
}  // namespace ark::es2panda::ir
