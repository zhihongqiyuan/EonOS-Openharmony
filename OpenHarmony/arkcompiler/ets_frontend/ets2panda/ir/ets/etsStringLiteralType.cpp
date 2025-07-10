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

#include "etsStringLiteralType.h"

#include "checker/ETSchecker.h"
#include "ir/astDump.h"

namespace ark::es2panda::ir {
void ETSStringLiteralType::TransformChildren([[maybe_unused]] const NodeTransformer &cb,
                                             [[maybe_unused]] std::string_view const transformationName)
{
}

void ETSStringLiteralType::Iterate([[maybe_unused]] const NodeTraverser &cb) const {}

void ETSStringLiteralType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSStringLiteralType"}, {"value", value_}});
}

void ETSStringLiteralType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add(value_.Mutf8());
}

void ETSStringLiteralType::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    UNREACHABLE();
}

checker::Type *ETSStringLiteralType::Check([[maybe_unused]] checker::TSChecker *checker)
{
    UNREACHABLE();
}

checker::Type *ETSStringLiteralType::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSStringLiteralType::GetType([[maybe_unused]] checker::ETSChecker *checker)
{
    SetTsType(checker->CreateETSStringLiteralType(value_));
    return TsType();
}

ETSStringLiteralType *ETSStringLiteralType::Clone(ArenaAllocator *allocator, AstNode *parent)
{
    if (auto *const clone = allocator->New<ir::ETSStringLiteralType>(value_); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    return nullptr;
}
}  // namespace ark::es2panda::ir
