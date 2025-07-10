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

#include "classStaticBlock.h"

#include "compiler/core/envScope.h"
#include "ir/astDump.h"
#include "ir/statements/blockStatement.h"

namespace panda::es2panda::ir {

void ClassStaticBlock::Iterate(const NodeTraverser &cb) const
{
    cb(blockStatement_);
}

void ClassStaticBlock::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ClassStaticBlock"},
                 {"body", blockStatement_}});
}

void ClassStaticBlock::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    compiler::LabelTarget target(pg);
    compiler::VariableEnvScope envScope(pg, scope_->AsVariableScope(), target);
    blockStatement_->Compile(pg);
}

checker::Type *ClassStaticBlock::Check([[maybe_unused]] checker::Checker *checker) const
{
    return nullptr;
}

void ClassStaticBlock::UpdateSelf(const NodeUpdater &cb, [[maybe_unused]] binder::Binder *binder)
{
    blockStatement_ = std::get<ir::AstNode *>(cb(blockStatement_))->AsBlockStatement();
}

}  // namespace panda::es2panda::ir

