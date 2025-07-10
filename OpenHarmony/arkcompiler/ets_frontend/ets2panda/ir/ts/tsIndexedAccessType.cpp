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

#include "tsIndexedAccessType.h"

#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

#include "checker/TSchecker.h"

namespace ark::es2panda::ir {
void TSIndexedAccessType::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(objectType_); objectType_ != transformedNode) {
        objectType_->SetTransformedNode(transformationName, transformedNode);
        objectType_ = static_cast<TypeNode *>(transformedNode);
    }

    if (auto *transformedNode = cb(indexType_); indexType_ != transformedNode) {
        indexType_->SetTransformedNode(transformationName, transformedNode);
        indexType_ = static_cast<TypeNode *>(transformedNode);
    }
}

void TSIndexedAccessType::Iterate(const NodeTraverser &cb) const
{
    cb(objectType_);
    cb(indexType_);
}

void TSIndexedAccessType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSIndexedAccessType"}, {"objectType", objectType_}, {"indexType", indexType_}});
}

void TSIndexedAccessType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TSIndexedAccessType");
}

void TSIndexedAccessType::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void TSIndexedAccessType::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSIndexedAccessType::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSIndexedAccessType::GetType([[maybe_unused]] checker::TSChecker *checker)
{
    if (TsType() != nullptr) {
        return TsType();
    }

    checker::Type *baseType = objectType_->GetType(checker);
    checker::Type *indexType = indexType_->GetType(checker);
    checker::Type *resolved = checker->GetPropertyTypeForIndexType(baseType, indexType);

    SetTsType(resolved);
    return TsType();
}

checker::Type *TSIndexedAccessType::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
