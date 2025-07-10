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

#include "tsTypeLiteral.h"

#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

#include "varbinder/variable.h"
#include "varbinder/declaration.h"
#include "checker/TSchecker.h"
#include "checker/types/signature.h"

namespace ark::es2panda::ir {
void TSTypeLiteral::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    for (auto *&it : members_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode;
        }
    }
}

void TSTypeLiteral::Iterate(const NodeTraverser &cb) const
{
    for (auto *it : members_) {
        cb(it);
    }
}

void TSTypeLiteral::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSTypeLiteral"}, {"members", members_}});
}

void TSTypeLiteral::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TSTypeLiteral");
}

void TSTypeLiteral::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TSTypeLiteral::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSTypeLiteral::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSTypeLiteral::GetType(checker::TSChecker *checker)
{
    if (TsType() != nullptr) {
        return TsType();
    }

    checker::ObjectDescriptor *desc = checker->Allocator()->New<checker::ObjectDescriptor>(checker->Allocator());
    checker::Type *type = checker->Allocator()->New<checker::ObjectLiteralType>(desc);
    type->SetVariable(Variable());

    SetTsType(type);
    return TsType();
}

checker::Type *TSTypeLiteral::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
