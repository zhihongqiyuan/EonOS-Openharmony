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

#include "etsNullishTypes.h"

#include "checker/ETSchecker.h"
#include "ir/astDump.h"

namespace ark::es2panda::ir {
void ETSUndefinedType::TransformChildren([[maybe_unused]] const NodeTransformer &cb,
                                         [[maybe_unused]] std::string_view const transformationName)
{
}

void ETSUndefinedType::Iterate([[maybe_unused]] const NodeTraverser &cb) const {}

void ETSUndefinedType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSUndefinedType"}});
}

void ETSUndefinedType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("undefined");
}

void ETSUndefinedType::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    UNREACHABLE();
}

checker::Type *ETSUndefinedType::Check([[maybe_unused]] checker::TSChecker *checker)
{
    UNREACHABLE();
}

checker::Type *ETSUndefinedType::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSUndefinedType::GetType([[maybe_unused]] checker::ETSChecker *checker)
{
    SetTsType(checker->GlobalETSUndefinedType());
    return TsType();
}

ETSUndefinedType *ETSUndefinedType::Clone(ArenaAllocator *allocator, AstNode *parent)
{
    if (auto *const clone = allocator->New<ir::ETSUndefinedType>(); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    return nullptr;
}

void ETSNullType::TransformChildren([[maybe_unused]] const NodeTransformer &cb,
                                    [[maybe_unused]] std::string_view const transformationName)
{
}

void ETSNullType::Iterate([[maybe_unused]] const NodeTraverser &cb) const {}

void ETSNullType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSNullType"}});
}

void ETSNullType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("null");
}

void ETSNullType::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    UNREACHABLE();
}

checker::Type *ETSNullType::Check([[maybe_unused]] checker::TSChecker *checker)
{
    UNREACHABLE();
}

checker::Type *ETSNullType::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSNullType::GetType([[maybe_unused]] checker::ETSChecker *checker)
{
    SetTsType(checker->GlobalETSNullType());
    return TsType();
}

ETSNullType *ETSNullType::Clone(ArenaAllocator *allocator, AstNode *parent)
{
    if (auto *const clone = allocator->New<ir::ETSNullType>(); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    return nullptr;
}
}  // namespace ark::es2panda::ir
