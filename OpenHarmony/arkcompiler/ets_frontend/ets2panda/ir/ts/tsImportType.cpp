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

#include "tsImportType.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/ts/tsTypeParameter.h"
#include "ir/ts/tsTypeParameterInstantiation.h"

namespace ark::es2panda::ir {
void TSImportType::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(param_); param_ != transformedNode) {
        param_->SetTransformedNode(transformationName, transformedNode);
        param_ = transformedNode->AsExpression();
    }

    if (typeParams_ != nullptr) {
        if (auto *transformedNode = cb(typeParams_); typeParams_ != transformedNode) {
            typeParams_->SetTransformedNode(transformationName, transformedNode);
            typeParams_ = transformedNode->AsTSTypeParameterInstantiation();
        }
    }

    if (qualifier_ != nullptr) {
        if (auto *transformedNode = cb(qualifier_); qualifier_ != transformedNode) {
            qualifier_->SetTransformedNode(transformationName, transformedNode);
            qualifier_ = transformedNode->AsExpression();
        }
    }
}

void TSImportType::Iterate(const NodeTraverser &cb) const
{
    cb(param_);

    if (typeParams_ != nullptr) {
        cb(typeParams_);
    }

    if (qualifier_ != nullptr) {
        cb(qualifier_);
    }
}

void TSImportType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSImportType"},
                 {"parameter", param_},
                 {"qualifier", AstDumper::Optional(qualifier_)},
                 {"typeParameters", AstDumper::Optional(typeParams_)},
                 {"isTypeOf", isTypeof_}});
}

void TSImportType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TSImportType");
}

void TSImportType::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TSImportType::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSImportType::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSImportType::GetType([[maybe_unused]] checker::TSChecker *checker)
{
    return nullptr;
}

checker::Type *TSImportType::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
