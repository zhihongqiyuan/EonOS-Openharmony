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

#include "interfaceType.h"

#include <typescript/checker.h>

namespace panda::es2panda::checker {

void InterfaceType::ToString(std::stringstream &ss) const
{
    ss << name_;

    if (!typeParamTypes_.empty()) {
        ss << "<";

        for (auto it = typeParamTypes_.begin(); it != typeParamTypes_.end(); it++) {
            (*it)->ToString(ss);

            if (std::next(it) != typeParamTypes_.end()) {
                ss << ", ";
            }
        }

        ss << ">";
    }
}

bool InterfaceType::IsPropertiesIdentical(TypeRelation *relation, InterfaceType *other)
{
    const ArenaVector<binder::LocalVariable *> &targetProperties = Properties();
    const ArenaVector<binder::LocalVariable *> &sourceProperties = other->Properties();
    if (targetProperties.size() != sourceProperties.size()) {
        relation->Result(false);
        return false;
    }

    for (auto *targetProp : targetProperties) {
        bool foundProp =
            std::any_of(sourceProperties.begin(), sourceProperties.end(),
                        [targetProp, relation](binder::LocalVariable *sourceProp) {
                            if (targetProp->Name() == sourceProp->Name()) {
                                Type *targetType = relation->GetChecker()->GetTypeOfVariable(targetProp);
                                Type *sourceType = relation->GetChecker()->GetTypeOfVariable(sourceProp);
                                return relation->IsIdenticalTo(targetType, sourceType);
                            }

                            return false;
                        });
        if (!foundProp) {
            relation->Result(false);
            return false;
        }
    }

    return true;
}

bool InterfaceType::IsIndexInfoIdentical(TypeRelation *relation, InterfaceType *other)
{
    IndexInfo *targetNumberInfo = NumberIndexInfo();
    IndexInfo *sourceNumberInfo = other->NumberIndexInfo();
    if ((targetNumberInfo && !sourceNumberInfo) || (!targetNumberInfo && sourceNumberInfo)) {
        relation->Result(false);
        return false;
    }

    relation->IsIdenticalTo(targetNumberInfo, sourceNumberInfo);

    if (relation->IsTrue()) {
        IndexInfo *targetStringInfo = StringIndexInfo();
        IndexInfo *sourceStringInfo = other->StringIndexInfo();

        if ((targetStringInfo && !sourceStringInfo) || (!targetStringInfo && sourceStringInfo)) {
            relation->Result(false);
            return false;
        }

        relation->IsIdenticalTo(targetStringInfo, sourceStringInfo);
    }

    return true;
}

void InterfaceType::Identical(TypeRelation *relation, Type *other)
{
    if (!other->IsObjectType() || !other->AsObjectType()->IsInterfaceType()) {
        return;
    }

    InterfaceType *otherInterface = other->AsObjectType()->AsInterfaceType();
    if (!IsPropertiesIdentical(relation, otherInterface)) {
        return;
    }

    const ArenaVector<Signature *> &targetCallSignatures = CallSignatures();
    const ArenaVector<Signature *> &sourceCallSignatures = otherInterface->CallSignatures();
    if (targetCallSignatures.size() != sourceCallSignatures.size()) {
        relation->Result(false);
        return;
    }

    if (!EachSignatureRelatedToSomeSignature(relation, targetCallSignatures, sourceCallSignatures) ||
        !EachSignatureRelatedToSomeSignature(relation, sourceCallSignatures, targetCallSignatures)) {
        return;
    }

    const ArenaVector<Signature *> &targetConstructSignatures = ConstructSignatures();
    const ArenaVector<Signature *> &sourceConstructSignatures = otherInterface->ConstructSignatures();
    if (targetConstructSignatures.size() != sourceConstructSignatures.size()) {
        relation->Result(false);
        return;
    }

    if (!EachSignatureRelatedToSomeSignature(relation, targetConstructSignatures, sourceConstructSignatures) ||
        !EachSignatureRelatedToSomeSignature(relation, sourceConstructSignatures, targetConstructSignatures)) {
        return;
    }

    if (!IsIndexInfoIdentical(relation, otherInterface)) {
        return;
    }
}

Type *InterfaceType::Instantiate(ArenaAllocator *allocator, TypeRelation *relation, GlobalTypesHolder *globalTypes)
{
    ObjectDescriptor *copiedDesc = allocator->New<ObjectDescriptor>(allocator);

    desc_->Copy(allocator, copiedDesc, relation, globalTypes);

    Type *newInterfaceType = allocator->New<InterfaceType>(allocator, name_, copiedDesc);
    CHECK_NOT_NULL(newInterfaceType);
    for (auto *it : bases_) {
        newInterfaceType->AsObjectType()->AsInterfaceType()->AddBase(
            it->Instantiate(allocator, relation, globalTypes)->AsObjectType());
    }

    return newInterfaceType;
}

void InterfaceType::CollectSignatures(ArenaVector<Signature *> *collectedSignatures, bool collectCallSignatures) const
{
    if (collectCallSignatures) {
        for (auto *it : desc_->callSignatures) {
            collectedSignatures->push_back(it);
        }
    } else {
        for (auto *it : desc_->constructSignatures) {
            collectedSignatures->push_back(it);
        }
    }

    for (auto *it : bases_) {
        it->AsInterfaceType()->CollectSignatures(collectedSignatures, collectCallSignatures);
    }
}

void InterfaceType::CollectProperties(ArenaVector<binder::LocalVariable *> *collectedPropeties) const
{
    for (auto *currentProp : desc_->properties) {
        bool propAlreadyCollected = false;
        for (auto *collectedProp : *collectedPropeties) {
            if (currentProp->Name() == collectedProp->Name()) {
                propAlreadyCollected = true;
                break;
            }
        }

        if (propAlreadyCollected) {
            continue;
        }

        collectedPropeties->push_back(currentProp);
    }

    for (auto *it : bases_) {
        it->AsInterfaceType()->CollectProperties(collectedPropeties);
    }
}

const IndexInfo *InterfaceType::FindIndexInfo(bool findNumberInfo) const
{
    const IndexInfo *foundInfo = nullptr;

    if (findNumberInfo && desc_->numberIndexInfo) {
        foundInfo = desc_->numberIndexInfo;
    } else if (!findNumberInfo && desc_->stringIndexInfo) {
        foundInfo = desc_->stringIndexInfo;
    }

    for (auto it = bases_.begin(); it != bases_.end() && !foundInfo; it++) {
        foundInfo = (*it)->AsInterfaceType()->FindIndexInfo(findNumberInfo);
    }

    return foundInfo;
}

IndexInfo *InterfaceType::FindIndexInfo(bool findNumberInfo)
{
    IndexInfo *foundInfo = nullptr;

    if (findNumberInfo && desc_->numberIndexInfo) {
        foundInfo = desc_->numberIndexInfo;
    } else if (!findNumberInfo && desc_->stringIndexInfo) {
        foundInfo = desc_->stringIndexInfo;
    }

    for (auto it = bases_.begin(); it != bases_.end() && !foundInfo; it++) {
        foundInfo = (*it)->AsInterfaceType()->FindIndexInfo(findNumberInfo);
    }

    return foundInfo;
}

TypeFacts InterfaceType::GetTypeFacts() const
{
    if (desc_->properties.empty() && desc_->callSignatures.empty() && desc_->constructSignatures.empty() &&
        !desc_->stringIndexInfo && !desc_->numberIndexInfo) {
        if (bases_.empty()) {
            return TypeFacts::EMPTY_OBJECT_FACTS;
        }

        bool isEmpty = true;
        for (auto it = bases_.begin(); isEmpty && it != bases_.end(); it++) {
            if (!(*it)->Properties().empty() || !(*it)->CallSignatures().empty() ||
                !(*it)->ConstructSignatures().empty() || (*it)->StringIndexInfo() || (*it)->NumberIndexInfo()) {
                isEmpty = false;
            }
        }

        if (isEmpty) {
            return TypeFacts::EMPTY_OBJECT_FACTS;
        }
    }

    return TypeFacts::OBJECT_FACTS;
}

}  // namespace panda::es2panda::checker
