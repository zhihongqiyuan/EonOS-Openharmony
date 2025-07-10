/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "etsNeverType.h"

#include "checker/ETSchecker.h"
#include "ir/astDump.h"

namespace ark::es2panda::ir {
void ETSNeverType::TransformChildren([[maybe_unused]] const NodeTransformer &cb,
                                     [[maybe_unused]] std::string_view const transformationName)
{
}

void ETSNeverType::Iterate([[maybe_unused]] const NodeTraverser &cb) const {}

void ETSNeverType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSNeverType"}});
}

void ETSNeverType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("never");
}

void ETSNeverType::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    UNREACHABLE();
}

checker::Type *ETSNeverType::Check([[maybe_unused]] checker::TSChecker *checker)
{
    UNREACHABLE();
}

checker::Type *ETSNeverType::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSNeverType::GetType([[maybe_unused]] checker::ETSChecker *checker)
{
    SetTsType(checker->GlobalETSNeverType());
    return TsType();
}

ETSNeverType *ETSNeverType::Clone(ArenaAllocator *allocator, AstNode *parent)
{
    if (auto *const clone = allocator->New<ir::ETSNeverType>(); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    return nullptr;
}
}  // namespace ark::es2panda::ir
