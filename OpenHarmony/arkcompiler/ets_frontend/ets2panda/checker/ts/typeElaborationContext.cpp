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

#include "typeElaborationContext.h"

#include "util/helpers.h"
#include "varbinder/scope.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/objectExpression.h"
#include "ir/expressions/arrayExpression.h"
#include "ir/expressions/literals/numberLiteral.h"
#include "ir/expressions/literals/stringLiteral.h"
#include "ir/base/spreadElement.h"
#include "ir/base/property.h"

namespace ark::es2panda::checker {
Type *ElaborationContext::GetBestMatchingType(Type *indexType, ir::Expression *sourceNode)
{
    ArenaVector<Type *> bestMatchingType(checker_->Allocator()->Adapter());
    Type *sourceType = sourceNode != nullptr ? checker_->CheckTypeCached(sourceNode) : checker_->GlobalAnyType();

    for (auto it = potentialTypes_.begin(); it != potentialTypes_.end();) {
        Type *currentType = checker_->GetPropertyTypeForIndexType(*it, indexType);

        if (currentType == nullptr) {
            it = potentialTypes_.erase(it);
            continue;
        }

        if (!checker_->IsTypeAssignableTo(sourceType, currentType)) {
            it = potentialTypes_.erase(it);
        } else {
            it++;
        }

        bestMatchingType.push_back(currentType);
    }

    return checker_->CreateUnionType(std::move(bestMatchingType));
}

void ArrayElaborationContext::Start()
{
    ASSERT(sourceNode_->IsArrayExpression());
    RemoveUnnecessaryTypes();

    for (auto *it : sourceNode_->AsArrayExpression()->Elements()) {
        if (it->IsOmittedExpression()) {
            index_++;
            continue;
        }

        util::StringView memberIndex = util::Helpers::ToStringView(checker_->Allocator(), index_);

        Type *targetElementType = nullptr;

        if (targetType_->IsUnionType()) {
            targetElementType = GetBestMatchingType(checker_->CreateStringLiteralType(memberIndex), it);
        } else {
            targetElementType =
                checker_->GetPropertyTypeForIndexType(targetType_, checker_->CreateStringLiteralType(memberIndex));
        }

        if (targetElementType == nullptr) {
            return;
        }

        checker_->ElaborateElementwise(targetElementType, it, it->Start());
        index_++;
    }
}

void ArrayElaborationContext::RemoveUnnecessaryTypes()
{
    if (!targetType_->IsUnionType()) {
        return;
    }

    for (auto *it : targetType_->AsUnionType()->ConstituentTypes()) {
        if (it->IsArrayType() || it->IsObjectType()) {
            potentialTypes_.push_back(it);
        }
    }
}

Type *ObjectElaborationContext::NonComputedPropKeyType(ir::Property *prop)
{
    switch (prop->Key()->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            return checker_->Allocator()->New<StringLiteralType>(prop->Key()->AsIdentifier()->Name());
        }
        case ir::AstNodeType::NUMBER_LITERAL: {
            return checker_->Allocator()->New<NumberLiteralType>(prop->Key()->AsNumberLiteral()->Number().GetDouble());
        }
        case ir::AstNodeType::STRING_LITERAL: {
            return checker_->Allocator()->New<StringLiteralType>(prop->Key()->AsStringLiteral()->Str());
        }
        default: {
            UNREACHABLE();
            break;
        }
    }
    return nullptr;
}

void ObjectElaborationContext::Start()
{
    ASSERT(sourceNode_->IsObjectExpression());
    RemoveUnnecessaryTypes();

    for (auto *it : sourceNode_->AsObjectExpression()->Properties()) {
        if (it->IsSpreadElement()) {
            continue;
        }

        ir::Property *prop = it->AsProperty();

        Type *propKeyType = nullptr;
        if (prop->IsComputed()) {
            propKeyType = checker_->CheckComputedPropertyName(prop->Key());
        } else {
            propKeyType = NonComputedPropKeyType(prop);
        }

        Type *targetElementType = nullptr;

        if (targetType_->IsUnionType()) {
            targetElementType = GetBestMatchingType(propKeyType, prop->IsShorthand() ? nullptr : prop->Value());
        } else {
            targetElementType = checker_->GetPropertyTypeForIndexType(targetType_, propKeyType);
        }

        if (targetElementType == nullptr) {
            if (propKeyType->HasTypeFlag(TypeFlag::LITERAL)) {
                checker_->ThrowTypeError({"Object literal may only specify known properties, and ", propKeyType,
                                          " does not exist in type '", targetType_, "'."},
                                         it->Start());
            }

            return;
        }

        if (prop->IsShorthand()) {
            continue;
        }

        checker_->ElaborateElementwise(targetElementType, prop->Value(), it->Start());
    }
}

void ObjectElaborationContext::RemoveUnnecessaryTypes()
{
    if (!targetType_->IsUnionType()) {
        return;
    }

    for (auto *it : targetType_->AsUnionType()->ConstituentTypes()) {
        if (it->IsObjectType()) {
            potentialTypes_.push_back(it);
        }
    }
}
}  // namespace ark::es2panda::checker
