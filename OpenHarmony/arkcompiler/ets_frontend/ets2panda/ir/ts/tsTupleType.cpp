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

#include "tsTupleType.h"

#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "util/helpers.h"
#include "varbinder/scope.h"
#include "checker/TSchecker.h"
#include "checker/types/ts/indexInfo.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/expressions/identifier.h"
#include "ir/ts/tsNamedTupleMember.h"

namespace ark::es2panda::ir {
void TSTupleType::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    for (auto *&it : elementTypes_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = static_cast<TypeNode *>(transformedNode);
        }
    }
}

void TSTupleType::Iterate(const NodeTraverser &cb) const
{
    for (auto *it : elementTypes_) {
        cb(it);
    }
}

void TSTupleType::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSTupleType"}, {"elementTypes", elementTypes_}});
}

void TSTupleType::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TSTupleType");
}

void TSTupleType::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TSTupleType::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *GetNumberIndexType(ArenaVector<checker::Type *> numberIndexTypes, checker::TSChecker *checker)
{
    checker::Type *numberIndexType = nullptr;
    if (numberIndexTypes.empty()) {
        numberIndexType = checker->GlobalNeverType();
    } else if (numberIndexTypes.size() == 1) {
        numberIndexType = numberIndexTypes[0];
    } else {
        numberIndexType = checker->CreateUnionType(std::move(numberIndexTypes));
    }
    return numberIndexType;
}

checker::Type *TSTupleType::GetType(checker::TSChecker *checker)
{
    if (TsType() != nullptr) {
        return TsType();
    }

    checker::ObjectDescriptor *desc = checker->Allocator()->New<checker::ObjectDescriptor>(checker->Allocator());
    checker::NamedTupleMemberPool namedMembers(checker->Allocator()->Adapter());
    ArenaVector<checker::ElementFlags> elementFlags(checker->Allocator()->Adapter());
    checker::ElementFlags combinedFlags = checker::ElementFlags::NO_OPTS;
    uint32_t minLength = 0;
    uint32_t index = 0;
    ArenaVector<checker::Type *> numberIndexTypes(checker->Allocator()->Adapter());
    for (auto *it : elementTypes_) {
        util::StringView memberIndex = util::Helpers::ToStringView(checker->Allocator(), index);

        auto *memberVar =
            varbinder::Scope::CreateVar(checker->Allocator(), memberIndex, varbinder::VariableFlags::PROPERTY, it);

        checker::ElementFlags memberFlag;
        if (it->IsTSNamedTupleMember()) {
            auto *namedMember = it->AsTSNamedTupleMember();
            checker::Type *memberType = namedMember->ElementType()->GetType(checker);

            if (namedMember->IsOptional()) {
                memberVar->AddFlag(varbinder::VariableFlags::OPTIONAL);
                memberFlag = checker::ElementFlags::OPTIONAL;
            } else {
                memberFlag = checker::ElementFlags::REQUIRED;
                minLength++;
            }

            memberType->SetVariable(memberVar);
            memberVar->SetTsType(memberType);
            numberIndexTypes.push_back(memberType);
            namedMembers.insert({memberVar, namedMember->Label()->AsIdentifier()->Name()});
        } else {
            checker::Type *memberType = it->GetType(checker);
            memberType->SetVariable(memberVar);
            memberVar->SetTsType(memberType);
            memberFlag = checker::ElementFlags::REQUIRED;
            numberIndexTypes.push_back(memberType);
            minLength++;
        }

        combinedFlags |= memberFlag;

        elementFlags.push_back(memberFlag);
        desc->properties.push_back(memberVar);
        index++;
    }

    uint32_t fixedLength = desc->properties.size();

    checker::Type *numberIndexType = GetNumberIndexType(numberIndexTypes, checker);
    desc->numberIndexInfo = checker->Allocator()->New<checker::IndexInfo>(numberIndexType, "x", false);

    const checker::TupleTypeInfo tupleTypeInfo = {combinedFlags, minLength, fixedLength, false};
    SetTsType(checker->CreateTupleType(desc, std::move(elementFlags), tupleTypeInfo, std::move(namedMembers)));
    return TsType();
}

checker::Type *TSTupleType::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSTupleType::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
