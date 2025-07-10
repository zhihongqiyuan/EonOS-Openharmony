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

#include "destructuringContext.h"

#include "util/helpers.h"
#include "varbinder/scope.h"
#include "ir/typeNode.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/objectExpression.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/arrayExpression.h"
#include "ir/base/spreadElement.h"
#include "ir/base/property.h"
#include "ir/expression.h"

namespace ark::es2panda::checker {
void DestructuringContext::Prepare(ir::TypeNode *typeAnnotation, ir::Expression *initializer,
                                   const lexer::SourcePosition &loc)
{
    if (typeAnnotation != nullptr) {
        typeAnnotation->Check(checker_);
        Type *annotationType = typeAnnotation->GetType(checker_);

        if (initializer != nullptr) {
            checker_->ElaborateElementwise(annotationType, initializer, loc);
        }

        validateTypeAnnotation_ = true;
        inferredType_ = annotationType;
        return;
    }

    if (initializer != nullptr) {
        if (!initializer->IsObjectExpression()) {
            validateObjectPatternInitializer_ = false;
        }

        inferredType_ = initializer->Check(checker_);
    }
}

void DestructuringContext::HandleDestructuringAssignment(ir::Identifier *ident, Type *inferredType, Type *defaultType)
{
    if (ident->Variable() == nullptr) {
        checker_->ThrowTypeError({"Cannot find name '", ident->Name(), "'."}, ident->Start());
    }

    varbinder::Variable *variable = ident->Variable();
    ASSERT(variable->TsType());

    if (defaultType != nullptr && !checker_->IsTypeAssignableTo(defaultType, variable->TsType())) {
        checker_->ThrowAssignmentError(defaultType, variable->TsType(), ident->Start());
    }

    if (inferredType != nullptr && !checker_->IsTypeAssignableTo(inferredType, variable->TsType())) {
        checker_->ThrowAssignmentError(inferredType, variable->TsType(), ident->Start());
    }
}

void DestructuringContext::SetInferredTypeForVariable(varbinder::Variable *var, Type *inferredType,
                                                      const lexer::SourcePosition &loc)
{
    ASSERT(var);

    if (!checker_->HasStatus(CheckerStatus::IN_CONST_CONTEXT)) {
        inferredType = checker_->GetBaseTypeOfLiteralType(inferredType);
    }

    if (var->TsType() != nullptr) {
        checker_->IsTypeIdenticalTo(var->TsType(), inferredType,
                                    {"Subsequent variable declaration must have the same type. Variable '", var->Name(),
                                     "' must be of type '", var->TsType(), "', but here has type '", inferredType,
                                     "'."},
                                    loc);
        return;
    }

    if (signatureInfo_ != nullptr) {
        signatureInfo_->params.push_back(var->AsLocalVariable());
        signatureInfo_->minArgCount++;
    }

    var->SetTsType(inferredType);
}

void DestructuringContext::ValidateObjectLiteralType(ObjectType *objType, ir::ObjectExpression *objPattern)
{
    for (const auto *sourceProp : objType->Properties()) {
        const util::StringView &sourceName = sourceProp->Name();
        bool found = false;

        for (const auto *targetProp : objPattern->Properties()) {
            if (targetProp->IsRestElement()) {
                continue;
            }

            ASSERT(targetProp->IsProperty());
            const util::StringView &targetName = targetProp->AsProperty()->Key()->AsIdentifier()->Name();

            if (sourceName == targetName) {
                found = true;
                break;
            }
        }

        if (!found) {
            checker_->ThrowTypeError({"Object literal may only specify known properties, and property '", sourceName,
                                      "' does not exist in the pattern."},
                                     objPattern->Start());
        }
    }
}

// Helper function to reduce HandleAssignmentPattern and pass code checker
void DestructuringContext::HandleAssignmentPatternArrayPattern(ir::AssignmentExpression *assignmentPattern,
                                                               Type *inferredType)
{
    ArrayDestructuringContext nextContext = ArrayDestructuringContext(
        {checker_, assignmentPattern->Left(), inAssignment_, convertTupleToArray_, nullptr, nullptr});
    nextContext.SetInferredType(inferredType);
    nextContext.Start();
}

// Helper function to reduce HandleAssignmentPattern and pass code checker
void DestructuringContext::HandleAssignmentPatternIdentifier(ir::AssignmentExpression *assignmentPattern,
                                                             Type *defaultType, Type *inferredType)
{
    if (validateTypeAnnotation_ && !checker_->IsTypeAssignableTo(defaultType, inferredType)) {
        checker_->ThrowAssignmentError(defaultType, inferredType, assignmentPattern->Left()->Start());
    }

    SetInferredTypeForVariable(assignmentPattern->Left()->AsIdentifier()->Variable(), inferredType,
                               assignmentPattern->Start());
}

void DestructuringContext::HandleAssignmentPattern(ir::AssignmentExpression *assignmentPattern, Type *inferredType,
                                                   bool validateDefault)
{
    if (!assignmentPattern->Left()->IsArrayPattern()) {
        checker_->RemoveStatus(CheckerStatus::FORCE_TUPLE);
    }

    Type *defaultType = assignmentPattern->Right()->Check(checker_);
    if (!checker_->HasStatus(CheckerStatus::IN_CONST_CONTEXT)) {
        defaultType = checker_->GetBaseTypeOfLiteralType(defaultType);
    }

    if (validateDefault && assignmentPattern->Right()->IsObjectExpression() &&
        assignmentPattern->Left()->IsObjectPattern()) {
        ValidateObjectLiteralType(defaultType->AsObjectType(), assignmentPattern->Left()->AsObjectPattern());
    }

    Type *initType = inferredType;
    checker_->AddStatus(CheckerStatus::FORCE_TUPLE);

    if (validateTypeAnnotation_) {
        if (inferredType == nullptr) {
            inferredType = checker_->GlobalUndefinedType();
        }
    } else {
        if (inferredType == nullptr) {
            inferredType = defaultType;
        } else if (inferredType->IsUnionType()) {
            inferredType->AsUnionType()->AddConstituentType(defaultType, checker_->Relation());
        } else {
            inferredType = checker_->CreateUnionType({inferredType, defaultType});
        }
    }

    if (assignmentPattern->Left()->IsIdentifier()) {
        if (inAssignment_) {
            HandleDestructuringAssignment(assignmentPattern->Left()->AsIdentifier(), initType, defaultType);
            return;
        }

        HandleAssignmentPatternIdentifier(assignmentPattern, defaultType, inferredType);
        return;
    }

    if (assignmentPattern->Left()->IsArrayPattern()) {
        HandleAssignmentPatternArrayPattern(assignmentPattern, inferredType);
        return;
    }

    ASSERT(assignmentPattern->Left()->IsObjectPattern());
    ObjectDestructuringContext nextContext = ObjectDestructuringContext(
        {checker_, assignmentPattern->Left(), inAssignment_, convertTupleToArray_, nullptr, nullptr});
    nextContext.SetInferredType(inferredType);
    nextContext.Start();
}

void ArrayDestructuringContext::ValidateInferredType()
{
    if (!inferredType_->IsArrayType() && !inferredType_->IsUnionType() &&
        (!inferredType_->IsObjectType() || !inferredType_->AsObjectType()->IsTupleType())) {
        checker_->ThrowTypeError(
            {"Type ", inferredType_, " must have a '[Symbol.iterator]()' method that returns an iterator."},
            id_->Start());
    }

    if (inferredType_->IsUnionType()) {
        for (auto *it : inferredType_->AsUnionType()->ConstituentTypes()) {
            if (!it->IsArrayType() && (!it->IsObjectType() || !it->AsObjectType()->IsTupleType())) {
                checker_->ThrowTypeError(
                    {"Type ", inferredType_, " must have a '[Symbol.iterator]()' method that returns an iterator."},
                    id_->Start());
            }
        }
    }
}

Type *ArrayDestructuringContext::GetTypeFromTupleByIndex(TupleType *tuple)
{
    util::StringView memberIndex = util::Helpers::ToStringView(checker_->Allocator(), index_);
    varbinder::Variable *memberVar = tuple->GetProperty(memberIndex, false);

    if (memberVar == nullptr) {
        return nullptr;
    }

    return memberVar->TsType();
}

Type *ArrayDestructuringContext::NextInferredType([[maybe_unused]] const util::StringView &searchName, bool throwError)
{
    if (inferredType_->IsArrayType()) {
        return inferredType_->AsArrayType()->ElementType();
    }

    if (inferredType_->IsObjectType()) {
        ASSERT(inferredType_->AsObjectType()->IsTupleType());
        Type *returnType = GetTypeFromTupleByIndex(inferredType_->AsObjectType()->AsTupleType());

        if (returnType == nullptr && throwError) {
            if (!validateTypeAnnotation_ && checker_->HasStatus(CheckerStatus::IN_PARAMETER)) {
                return returnType;
            }

            checker_->ThrowTypeError({"Tuple type ", inferredType_, " of length ",
                                      inferredType_->AsObjectType()->AsTupleType()->FixedLength(),
                                      " has no element at index ", index_, "."},
                                     id_->Start());
        }

        return returnType;
    }

    ASSERT(inferredType_->IsUnionType());

    ArenaVector<Type *> unionTypes(checker_->Allocator()->Adapter());

    for (auto *type : inferredType_->AsUnionType()->ConstituentTypes()) {
        if (type->IsArrayType()) {
            unionTypes.push_back(type->AsArrayType()->ElementType());
            continue;
        }

        ASSERT(type->IsObjectType() && type->AsObjectType()->IsTupleType());
        Type *elementType = GetTypeFromTupleByIndex(type->AsObjectType()->AsTupleType());

        if (elementType == nullptr) {
            continue;
        }

        unionTypes.push_back(elementType);
    }

    if (unionTypes.empty()) {
        if (throwError) {
            checker_->ThrowTypeError({"Property ", index_, " does not exist on type ", inferredType_, "."},
                                     id_->Start());
        }

        return nullptr;
    }

    return checker_->CreateUnionType(std::move(unionTypes));
}

Type *ArrayDestructuringContext::CreateArrayTypeForRest(UnionType *inferredType)
{
    ArenaVector<Type *> unionTypes(checker_->Allocator()->Adapter());
    uint32_t savedIdx = index_;

    for (auto *it : inferredType->ConstituentTypes()) {
        if (it->IsArrayType()) {
            unionTypes.push_back(it->AsArrayType()->ElementType());
            continue;
        }

        ASSERT(it->IsObjectType() && it->AsObjectType()->IsTupleType());
        Type *tupleElementType = GetTypeFromTupleByIndex(it->AsObjectType()->AsTupleType());

        while (tupleElementType != nullptr) {
            unionTypes.push_back(tupleElementType);
            index_++;
            tupleElementType = GetTypeFromTupleByIndex(it->AsObjectType()->AsTupleType());
        }

        index_ = savedIdx;
    }

    Type *restArrayElementType = checker_->CreateUnionType(std::move(unionTypes));
    return checker_->Allocator()->New<ArrayType>(restArrayElementType);
}

Type *ArrayDestructuringContext::CreateTupleTypeForRest(TupleType *tuple)
{
    ObjectDescriptor *desc = checker_->Allocator()->New<ObjectDescriptor>(checker_->Allocator());
    ArenaVector<ElementFlags> elementFlags(checker_->Allocator()->Adapter());
    uint32_t savedIdx = index_;
    uint32_t iterIndex = 0;

    Type *tupleElementType = GetTypeFromTupleByIndex(tuple);

    while (tupleElementType != nullptr) {
        ElementFlags memberFlag = ElementFlags::REQUIRED;
        util::StringView memberIndex = util::Helpers::ToStringView(checker_->Allocator(), iterIndex);
        auto *memberVar = varbinder::Scope::CreateVar(checker_->Allocator(), memberIndex,
                                                      varbinder::VariableFlags::PROPERTY, nullptr);
        memberVar->SetTsType(tupleElementType);
        elementFlags.push_back(memberFlag);
        desc->properties.push_back(memberVar);

        index_++;
        iterIndex++;

        tupleElementType = GetTypeFromTupleByIndex(tuple);
    }

    index_ = savedIdx;
    const checker::TupleTypeInfo tupleTypeInfo = {ElementFlags::REQUIRED, iterIndex, iterIndex, false};
    return checker_->CreateTupleType(desc, std::move(elementFlags), tupleTypeInfo);
}

Type *ArrayDestructuringContext::GetRestType([[maybe_unused]] const lexer::SourcePosition &loc)
{
    if (inferredType_->IsArrayType()) {
        return inferredType_;
    }

    if (inferredType_->IsObjectType() && inferredType_->AsObjectType()->IsTupleType()) {
        return CreateTupleTypeForRest(inferredType_->AsObjectType()->AsTupleType());
    }

    ASSERT(inferredType_->IsUnionType());
    bool createArrayType = false;

    for (auto *it : inferredType_->AsUnionType()->ConstituentTypes()) {
        if (it->IsArrayType()) {
            createArrayType = true;
            break;
        }
    }

    if (createArrayType) {
        return CreateArrayTypeForRest(inferredType_->AsUnionType());
    }

    ArenaVector<Type *> tupleUnion(checker_->Allocator()->Adapter());

    for (auto *it : inferredType_->AsUnionType()->ConstituentTypes()) {
        ASSERT(it->IsObjectType() && it->AsObjectType()->IsTupleType());
        Type *newTuple = CreateTupleTypeForRest(it->AsObjectType()->AsTupleType());
        tupleUnion.push_back(newTuple);
    }

    return checker_->CreateUnionType(std::move(tupleUnion));
}

void ArrayDestructuringContext::HandleRest(ir::SpreadElement *rest)
{
    Type *inferredRestType = GetRestType(rest->Start());

    if (rest->Argument()->IsIdentifier()) {
        if (inAssignment_) {
            HandleDestructuringAssignment(rest->Argument()->AsIdentifier(), inferredRestType, nullptr);
            return;
        }

        SetInferredTypeForVariable(rest->Argument()->AsIdentifier()->Variable(), inferredRestType, rest->Start());
        return;
    }

    if (rest->Argument()->IsArrayPattern()) {
        ArrayDestructuringContext nextContext = ArrayDestructuringContext(
            {checker_, rest->Argument(), inAssignment_, convertTupleToArray_, nullptr, nullptr});
        nextContext.SetInferredType(inferredRestType);
        nextContext.Start();
        return;
    }

    ASSERT(rest->Argument()->IsObjectPattern());
    ObjectDestructuringContext nextContext =
        ObjectDestructuringContext({checker_, rest->Argument(), inAssignment_, convertTupleToArray_, nullptr, nullptr});
    nextContext.SetInferredType(inferredRestType);
    nextContext.Start();
}

Type *ArrayDestructuringContext::ConvertTupleTypeToArrayTypeIfNecessary(ir::AstNode *node, Type *type)
{
    if (!convertTupleToArray_) {
        return type;
    }

    if (type == nullptr) {
        return type;
    }

    if (node->IsArrayPattern() ||
        (node->IsAssignmentPattern() && node->AsAssignmentPattern()->Left()->IsArrayPattern())) {
        return type;
    }

    if (type->IsObjectType() && type->AsObjectType()->IsTupleType()) {
        return type->AsObjectType()->AsTupleType()->ConvertToArrayType(checker_);
    }

    return type;
}

static void SetParameterType(ir::AstNode *parent, Type *type)
{
    parent->Iterate([type](ir::AstNode *childNode) -> void {
        if (childNode->IsIdentifier() && childNode->AsIdentifier()->Variable() != nullptr) {
            childNode->AsIdentifier()->Variable()->SetTsType(type);
            return;
        }

        SetParameterType(childNode, type);
    });
}

void ArrayDestructuringContext::SetRemainingParameterTypes()
{
    do {
        auto *it = id_->AsArrayPattern()->Elements()[index_];
        ASSERT(it);
        SetParameterType(it, checker_->GlobalAnyType());
    } while (++index_ != id_->AsArrayPattern()->Elements().size());
}

void ArrayDestructuringContext::HandleElement(ir::Expression *element, Type *nextInferredType)
{
    switch (element->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            if (inAssignment_) {
                HandleDestructuringAssignment(element->AsIdentifier(), nextInferredType, nullptr);
                break;
            }

            SetInferredTypeForVariable(element->AsIdentifier()->Variable(), nextInferredType, element->Start());
            break;
        }
        case ir::AstNodeType::ARRAY_PATTERN: {
            ArrayDestructuringContext nextContext =
                ArrayDestructuringContext({checker_, element, inAssignment_, convertTupleToArray_, nullptr, nullptr});
            nextContext.SetInferredType(nextInferredType);
            nextContext.Start();
            break;
        }
        case ir::AstNodeType::OBJECT_PATTERN: {
            ObjectDestructuringContext nextContext =
                ObjectDestructuringContext({checker_, element, inAssignment_, convertTupleToArray_, nullptr, nullptr});
            nextContext.SetInferredType(nextInferredType);
            nextContext.Start();
            break;
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            HandleAssignmentPattern(element->AsAssignmentPattern(), nextInferredType, false);
            break;
        }
        case ir::AstNodeType::OMITTED_EXPRESSION: {
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void ArrayDestructuringContext::Start()
{
    ASSERT(id_->IsArrayPattern());

    ValidateInferredType();

    util::StringView name = util::Helpers::ToStringView(checker_->Allocator(), 0);

    for (auto *it : id_->AsArrayPattern()->Elements()) {
        if (it->IsRestElement()) {
            HandleRest(it->AsRestElement());
            break;
        }

        Type *nextInferredType =
            ConvertTupleTypeToArrayTypeIfNecessary(it, NextInferredType(name, !it->IsAssignmentPattern()));

        if (nextInferredType == nullptr && checker_->HasStatus(CheckerStatus::IN_PARAMETER)) {
            SetRemainingParameterTypes();
            return;
        }

        if (convertTupleToArray_ && nextInferredType != nullptr && inferredType_->IsObjectType()) {
            ASSERT(inferredType_->AsObjectType()->IsTupleType());

            varbinder::Variable *currentTupleElement = inferredType_->AsObjectType()->Properties()[index_];

            if (currentTupleElement != nullptr) {
                currentTupleElement->SetTsType(nextInferredType);
            }
        }

        HandleElement(it, nextInferredType);
        index_++;
    }
}

void ObjectDestructuringContext::ValidateInferredType()
{
    if (!inferredType_->IsObjectType()) {
        return;
    }

    ValidateObjectLiteralType(inferredType_->AsObjectType(), id_->AsObjectPattern());
}

void ObjectDestructuringContext::HandleRest(ir::SpreadElement *rest)
{
    Type *inferredRestType = GetRestType(rest->Start());
    ASSERT(rest->Argument()->IsIdentifier());

    if (inAssignment_) {
        HandleDestructuringAssignment(rest->Argument()->AsIdentifier(), inferredRestType, nullptr);
        return;
    }

    SetInferredTypeForVariable(rest->Argument()->AsIdentifier()->Variable(), inferredRestType, rest->Start());
}

Type *ObjectDestructuringContext::CreateObjectTypeForRest(ObjectType *objType)
{
    ObjectDescriptor *desc = checker_->Allocator()->New<ObjectDescriptor>(checker_->Allocator());

    for (auto *it : objType->AsObjectType()->Properties()) {
        if (!it->HasFlag(varbinder::VariableFlags::INFERRED_IN_PATTERN)) {
            auto *memberVar =
                varbinder::Scope::CreateVar(checker_->Allocator(), it->Name(), varbinder::VariableFlags::NONE, nullptr);
            memberVar->SetTsType(it->TsType());
            memberVar->AddFlag(it->Flags());
            desc->properties.push_back(memberVar);
        }
    }

    Type *returnType = checker_->Allocator()->New<ObjectLiteralType>(desc);
    returnType->AsObjectType()->AddObjectFlag(ObjectFlags::RESOLVED_MEMBERS);
    return returnType;
}

Type *ObjectDestructuringContext::GetRestType([[maybe_unused]] const lexer::SourcePosition &loc)
{
    if (inferredType_->IsUnionType()) {
        ArenaVector<Type *> unionTypes(checker_->Allocator()->Adapter());

        for (auto *it : inferredType_->AsUnionType()->ConstituentTypes()) {
            if (it->IsObjectType()) {
                unionTypes.push_back(CreateObjectTypeForRest(it->AsObjectType()));
                continue;
            }

            checker_->ThrowTypeError("Rest types may only be created from object types.", loc);
        }

        return checker_->CreateUnionType(std::move(unionTypes));
    }

    if (inferredType_->IsObjectType()) {
        return CreateObjectTypeForRest(inferredType_->AsObjectType());
    }

    checker_->ThrowTypeError("Rest types may only be created from object types.", loc);
}

Type *ObjectDestructuringContext::ConvertTupleTypeToArrayTypeIfNecessary(ir::AstNode *node, Type *type)
{
    if (!convertTupleToArray_) {
        return type;
    }

    if (type == nullptr) {
        return type;
    }

    ASSERT(node->IsProperty());

    ir::Property *property = node->AsProperty();

    if (property->Value()->IsArrayPattern()) {
        return type;
    }

    if (property->Value()->IsAssignmentPattern() &&
        property->Value()->AsAssignmentPattern()->Left()->IsArrayPattern()) {
        return type;
    }

    if (type->IsObjectType() && type->AsObjectType()->IsTupleType()) {
        return type->AsObjectType()->AsTupleType()->ConvertToArrayType(checker_);
    }

    return type;
}

Type *ObjectDestructuringContext::NextInferredType([[maybe_unused]] const util::StringView &searchName, bool throwError)
{
    varbinder::Variable *prop = checker_->GetPropertyOfType(inferredType_, searchName, !throwError,
                                                            varbinder::VariableFlags::INFERRED_IN_PATTERN);

    if (prop != nullptr) {
        prop->AddFlag(varbinder::VariableFlags::INFERRED_IN_PATTERN);
        return prop->TsType();
    }

    if (inferredType_->IsObjectType()) {
        checker::ObjectType *objType = inferredType_->AsObjectType();

        if (objType->StringIndexInfo() != nullptr) {
            return objType->StringIndexInfo()->GetType();
        }
    }

    if (throwError) {
        checker_->ThrowTypeError({"Property ", searchName, " does not exist on type ", inferredType_, "."},
                                 id_->Start());
    }

    return nullptr;
}

void ObjectDestructuringContext::StartPropertyHelper(ir::Expression *it)
{
    ir::Property *property = it->AsProperty();

    Type *nextInferredType = ConvertTupleTypeToArrayTypeIfNecessary(
        it->AsProperty(), NextInferredType(property->Key()->AsIdentifier()->Name(),
                                           (!property->Value()->IsAssignmentPattern() || validateTypeAnnotation_)));

    if (property->Value()->IsIdentifier()) {
        if (inAssignment_) {
            HandleDestructuringAssignment(property->Value()->AsIdentifier(), nextInferredType, nullptr);
            return;
        }

        SetInferredTypeForVariable(property->Value()->AsIdentifier()->Variable(), nextInferredType, it->Start());
        return;
    }

    if (property->Value()->IsArrayPattern()) {
        ArrayDestructuringContext nextContext = ArrayDestructuringContext(
            {checker_, property->Value()->AsArrayPattern(), inAssignment_, convertTupleToArray_, nullptr, nullptr});
        nextContext.SetInferredType(nextInferredType);
        nextContext.Start();
        return;
    }

    if (property->Value()->IsObjectPattern()) {
        ObjectDestructuringContext nextContext = ObjectDestructuringContext(
            {checker_, property->Value()->AsObjectPattern(), inAssignment_, convertTupleToArray_, nullptr, nullptr});
        nextContext.SetInferredType(nextInferredType);
        nextContext.Start();
        return;
    }

    ASSERT(property->Value()->IsAssignmentPattern());
    HandleAssignmentPattern(property->Value()->AsAssignmentPattern(), nextInferredType, true);
}

void ObjectDestructuringContext::Start()
{
    ASSERT(id_->IsObjectPattern());

    if (!id_->AsObjectPattern()->Properties().back()->IsRestElement() && validateObjectPatternInitializer_) {
        ValidateInferredType();
    }

    for (auto *it : id_->AsObjectPattern()->Properties()) {
        const auto itType = it->Type();
        if (itType == ir::AstNodeType::PROPERTY) {
            ir::Property *property = it->AsProperty();
            if (property->IsComputed()) {
                // NOTE: aszilagyi.
                return;
            }
            StartPropertyHelper(it);
        } else if (itType == ir::AstNodeType::REST_ELEMENT) {
            HandleRest(it->AsRestElement());
        } else {
            UNREACHABLE();
        }
    }
}
}  // namespace ark::es2panda::checker
