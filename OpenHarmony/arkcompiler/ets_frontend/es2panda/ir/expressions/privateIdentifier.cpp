/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "privateIdentifier.h"

#include <ir/astDump.h>

namespace panda::es2panda::ir {

void PrivateIdentifier::Iterate(const NodeTraverser &cb) const
{
    if (typeAnnotation_) {
        cb(typeAnnotation_);
    }
}

void PrivateIdentifier::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "PrivateIdentifier"},
                 {"prefix", "#"},
                 {"name", name_},
                 {"typeAnnotation", AstDumper::Optional(typeAnnotation_)}});
}

void PrivateIdentifier::Compile(compiler::PandaGen *pg) const
{
}

checker::Type *PrivateIdentifier::Check(checker::Checker *checker) const
{
    return nullptr;
}

void PrivateIdentifier::UpdateSelf(const NodeUpdater &cb, [[maybe_unused]] binder::Binder *binder)
{
    if (typeAnnotation_) {
        typeAnnotation_ = std::get<ir::AstNode *>(cb(typeAnnotation_))->AsExpression();
    }
}

}  // namespace panda::es2panda::ir