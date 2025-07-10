/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "assertEntry.h"

#include <ir/astDump.h>
#include <ir/expressions/literals/stringLiteral.h>

namespace panda::es2panda::ir {

void AssertEntry::Iterate(const NodeTraverser &cb) const
{
    cb(key_);
    cb(value_);
}

void AssertEntry::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "AssertEntry"},
                 {"key", key_},
                 {"value", value_}});
}

void AssertEntry::Compile([[maybe_unused]] compiler::PandaGen *pg) const {}

checker::Type *AssertEntry::Check([[maybe_unused]] checker::Checker *checker) const
{
    return nullptr;
}

void AssertEntry::UpdateSelf(const NodeUpdater &cb, [[maybe_unused]] binder::Binder *binder)
{
    key_ = std::get<ir::AstNode *>(cb(key_))->AsExpression();
    value_ = std::get<ir::AstNode *>(cb(value_))->AsStringLiteral();
}

}  // namespace panda::es2panda::ir
