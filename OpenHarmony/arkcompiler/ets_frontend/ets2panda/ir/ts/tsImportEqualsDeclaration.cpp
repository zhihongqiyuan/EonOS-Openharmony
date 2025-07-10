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

#include "tsImportEqualsDeclaration.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/expression.h"
#include "ir/expressions/identifier.h"

namespace ark::es2panda::ir {
void TSImportEqualsDeclaration::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(id_); id_ != transformedNode) {
        id_->SetTransformedNode(transformationName, transformedNode);
        id_ = transformedNode->AsIdentifier();
    }

    if (auto *transformedNode = cb(moduleReference_); moduleReference_ != transformedNode) {
        moduleReference_->SetTransformedNode(transformationName, transformedNode);
        moduleReference_ = transformedNode->AsExpression();
    }
}

void TSImportEqualsDeclaration::Iterate(const NodeTraverser &cb) const
{
    cb(id_);
    cb(moduleReference_);
}

void TSImportEqualsDeclaration::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSImportEqualsDeclaration"},
                 {"id", id_},
                 {"moduleReference", moduleReference_},
                 {"isExport", isExport_}});
}

void TSImportEqualsDeclaration::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TSImportEqualsDeclaration");
}

void TSImportEqualsDeclaration::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void TSImportEqualsDeclaration::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSImportEqualsDeclaration::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSImportEqualsDeclaration::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
