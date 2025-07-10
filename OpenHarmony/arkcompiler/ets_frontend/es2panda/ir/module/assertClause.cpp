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

#include "assertClause.h"

#include <ir/astDump.h>

namespace panda::es2panda::ir {

void AssertClause::Iterate(const NodeTraverser &cb) const
{
    for (auto *it : elements_) {
        cb(it);
    }
}

void AssertClause::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "AssertClause"},
                 {"elements", elements_}});
}

void AssertClause::Compile([[maybe_unused]] compiler::PandaGen *pg) const {}

checker::Type *AssertClause::Check([[maybe_unused]] checker::Checker *checker) const
{
    return nullptr;
}

void AssertClause::UpdateSelf(const NodeUpdater &cb, [[maybe_unused]] binder::Binder *binder)
{
    for (auto iter = elements_.begin(); iter != elements_.end(); iter++) {
        *iter = std::get<ir::AstNode *>(cb(*iter))->AsAssertEntry();
    }
}

}  // namespace panda::es2panda::ir
