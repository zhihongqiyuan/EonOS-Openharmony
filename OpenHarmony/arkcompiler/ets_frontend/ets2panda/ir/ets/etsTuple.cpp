/**
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include "etsTuple.h"

#include "checker/types/ets/etsTupleType.h"
#include "ir/astDump.h"

namespace ark::es2panda::ir {

void ETSTuple::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    for (auto *&it : GetTupleTypeAnnotationsList()) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = static_cast<TypeNode *>(transformedNode);
        }
    }

    if (HasSpreadType()) {
        if (auto *transformedNode = cb(spreadType_); spreadType_ != transformedNode) {
            spreadType_->SetTransformedNode(transformationName, transformedNode);
            spreadType_ = static_cast<TypeNode *>(transformedNode);
        }
    }
}

void ETSTuple::Iterate(const NodeTraverser &cb) const
{
    for (auto *const it : GetTupleTypeAnnotationsList()) {
        cb(it);
    }

    if (HasSpreadType()) {
        cb(spreadType_);
    }
}

void ETSTuple::Dump(ir::AstDumper *const dumper) const
{
    dumper->Add({{"type", "ETSTuple"},
                 {"types", AstDumper::Optional(typeAnnotationList_)},
                 {"spreadType", AstDumper::Nullish(spreadType_)}});
}

void ETSTuple::Dump(ir::SrcDumper *const dumper) const
{
    dumper->Add("[");
    for (const auto *const typeAnnot : typeAnnotationList_) {
        typeAnnot->Dump(dumper);
        if ((typeAnnot != typeAnnotationList_.back()) || (spreadType_ != nullptr)) {
            dumper->Add(", ");
        }
    }
    if (spreadType_ != nullptr) {
        dumper->Add("...");
        spreadType_->Dump(dumper);
    }
    dumper->Add("]");
}

void ETSTuple::Compile([[maybe_unused]] compiler::PandaGen *const pg) const {}
void ETSTuple::Compile([[maybe_unused]] compiler::ETSGen *const etsg) const {}

checker::Type *ETSTuple::Check([[maybe_unused]] checker::TSChecker *const checker)
{
    return nullptr;
}

checker::Type *ETSTuple::Check([[maybe_unused]] checker::ETSChecker *const checker)
{
    return GetType(checker);
}

checker::Type *ETSTuple::CalculateLUBForTuple(checker::ETSChecker *const checker,
                                              ArenaVector<checker::Type *> &typeList, checker::Type **spreadTypePtr)
{
    auto &spreadType = *spreadTypePtr;
    if (typeList.empty()) {
        return spreadType == nullptr ? checker->GlobalETSObjectType() : spreadType;
    }

    bool allElementsAreSame = std::all_of(typeList.begin(), typeList.end(), [&checker, &typeList](auto *element) {
        return checker->Relation()->IsIdenticalTo(typeList[0], element);
    });

    if (spreadType != nullptr) {
        allElementsAreSame = allElementsAreSame && checker->Relation()->IsIdenticalTo(typeList[0], spreadType);
    }

    // If only one type present in the tuple, that will be the holder array type. If any two not identical types
    // present, primitives will be boxed, and LUB is calculated for all of them.
    // That makes it possible to assign eg. `[int, int, ...int[]]` tuple type to `int[]` array type. Because a
    // `short[]` array already isn't assignable to `int[]` array, that preserve that the `[int, short, ...int[]]`
    // tuple type's element type will be calculated to `Object[]`, which is not assignable to `int[]` array either.
    if (allElementsAreSame) {
        return typeList[0];
    }
    // Other case - promote element types
    // NOTE(vpukhov): #15570 normalization happens or not?
    std::for_each(typeList.begin(), typeList.end(), [checker](auto &t) { t = checker->MaybeBoxType(t); });

    auto ctypes = typeList;
    if (spreadType != nullptr) {
        spreadType = checker->MaybeBoxType(spreadType);
        ctypes.push_back(spreadType);
    }
    return checker->CreateETSUnionType(std::move(ctypes));
}

checker::Type *ETSTuple::GetType(checker::ETSChecker *const checker)
{
    if (TsType() != nullptr) {
        return TsType();
    }

    ArenaVector<checker::Type *> typeList(checker->Allocator()->Adapter());

    for (auto *const typeAnnotation : GetTupleTypeAnnotationsList()) {
        auto *const checkedType = typeAnnotation->GetType(checker);
        typeList.emplace_back(checkedType);
    }

    if (HasSpreadType()) {
        ASSERT(spreadType_->IsTSArrayType());
        auto *const arrayType = spreadType_->GetType(checker);
        ASSERT(arrayType->IsETSArrayType());
        spreadType_->SetTsType(arrayType->AsETSArrayType()->ElementType());
    }

    auto *spreadElementType = spreadType_ != nullptr ? spreadType_->TsType() : nullptr;

    checker::Type *tupleType = checker->Allocator()->New<checker::ETSTupleType>(
        typeList, CalculateLUBForTuple(checker, typeList, &spreadElementType), spreadElementType);

    if (IsReadonlyType()) {
        tupleType = checker->GetReadonlyType(tupleType);
    }
    SetTsType(tupleType);
    return TsType();
}

ETSTuple *ETSTuple::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<ETSTuple>(allocator, size_); clone != nullptr) {
        clone->AddModifier(flags_);

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        if (spreadType_ != nullptr) {
            auto *const spreadType = spreadType_->Clone(allocator, clone)->AsTypeNode();
            clone->SetSpreadType(spreadType);
        }

        ArenaVector<TypeNode *> typeList(allocator->Adapter());
        for (auto *const type : typeAnnotationList_) {
            auto *const t = type->Clone(allocator, clone);
            typeList.push_back(t);
        }

        clone->SetTypeAnnotationsList(std::move(typeList));
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

}  // namespace ark::es2panda::ir
