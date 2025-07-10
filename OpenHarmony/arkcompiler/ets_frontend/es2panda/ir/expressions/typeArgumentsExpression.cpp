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

#include "typeArgumentsExpression.h"

#include <ir/astDump.h>
#include <ir/ts/tsTypeParameterInstantiation.h>

namespace panda::es2panda::ir {

void TypeArgumentsExpression::Iterate([[maybe_unused]] const NodeTraverser &cb) const
{
    cb(leftHandExpression_);
    cb(typeArguments_);
}

void TypeArgumentsExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TypeArgumentsExpression"}, {"leftHandExpression", leftHandExpression_},
                 {"typeArguments", typeArguments_}});
}

void TypeArgumentsExpression::Compile(compiler::PandaGen *pg) const
{
    leftHandExpression_->Compile(pg);
}

checker::Type *TypeArgumentsExpression::Check(checker::Checker *checker) const
{
    return nullptr;
}

void TypeArgumentsExpression::UpdateSelf(const NodeUpdater &cb, [[maybe_unused]] binder::Binder *binder)
{
    leftHandExpression_ = std::get<ir::AstNode *>(cb(leftHandExpression_))->AsExpression();
    typeArguments_ = std::get<ir::AstNode *>(cb(typeArguments_))->AsTSTypeParameterInstantiation();
}

}  // namespace panda::es2panda::ir
