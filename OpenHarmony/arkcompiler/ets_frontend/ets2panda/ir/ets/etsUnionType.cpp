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

#include "etsUnionType.h"

#include "checker/ETSchecker.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "utils/arena_containers.h"

namespace ark::es2panda::ir {
void ETSUnionType::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    for (auto *&it : types_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = static_cast<TypeNode *>(transformedNode);
        }
    }
}

void ETSUnionType::Iterate(const NodeTraverser &cb) const
{
    for (auto *it : types_) {
        cb(it);
    }
}

void ETSUnionType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ETSUnionType"}, {"types", types_}});
}

void ETSUnionType::Dump(ir::SrcDumper *dumper) const
{
    for (auto type : types_) {
        type->Dump(dumper);
        if (type != types_.back()) {
            dumper->Add(" | ");
        }
    }
}

void ETSUnionType::Compile([[maybe_unused]] compiler::PandaGen *pg) const {}

checker::Type *ETSUnionType::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return nullptr;
}

checker::Type *ETSUnionType::Check(checker::ETSChecker *checker)
{
    for (auto *it : types_) {
        it->Check(checker);
    }

    return GetType(checker);
}

static bool CheckConstituentTypesValid(ArenaVector<checker::Type *> const &constituentTypes)
{
    for (auto &it : constituentTypes) {
        if (it->IsTypeError()) {
            return false;
        }
    }
    return true;
}

checker::Type *ETSUnionType::GetType(checker::ETSChecker *checker)
{
    if (TsType() != nullptr) {
        return TsType();
    }

    ArenaVector<checker::Type *> types(checker->Allocator()->Adapter());

    for (auto *it : types_) {
        types.push_back(it->GetType(checker));
    }

    checker->Relation()->SetNode(this);
    if (!CheckConstituentTypesValid(types)) {
        SetTsType(checker->GlobalTypeError());
    } else {
        SetTsType(checker->CreateETSUnionType(std::move(types)));
    }
    checker->Relation()->SetNode(nullptr);
    return TsType();
}

ETSUnionType *ETSUnionType::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    ArenaVector<ir::TypeNode *> types(allocator->Adapter());
    for (auto *it : types_) {
        auto *type = it->Clone(allocator, nullptr);
        types.push_back(type);
    }
    ETSUnionType *const clone = allocator->New<ir::ETSUnionType>(std::move(types));
    if (parent != nullptr) {
        clone->SetParent(parent);
    }
    for (auto *it : clone->types_) {
        it->SetParent(clone);
    }
    return clone;
}
}  // namespace ark::es2panda::ir
