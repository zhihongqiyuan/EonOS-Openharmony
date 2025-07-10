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

#include "arrayExpression.h"

#include "checker/ETSchecker.h"
#include "checker/TSchecker.h"
#include "checker/types/ets/etsTupleType.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/ts/destructuringContext.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/base/decorator.h"
#include "ir/srcDump.h"
#include "ir/typeNode.h"
#include "ir/base/spreadElement.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/objectExpression.h"
#include "util/helpers.h"

namespace ark::es2panda::ir {
ArrayExpression::ArrayExpression([[maybe_unused]] Tag const tag, ArrayExpression const &other,
                                 ArenaAllocator *const allocator)
    : AnnotatedExpression(static_cast<AnnotatedExpression const &>(other), allocator),
      decorators_(allocator->Adapter()),
      elements_(allocator->Adapter())
{
    preferredType_ = other.preferredType_;
    isDeclaration_ = other.isDeclaration_;
    trailingComma_ = other.trailingComma_;
    optional_ = other.optional_;

    for (auto *element : other.elements_) {
        elements_.emplace_back(element->Clone(allocator, this)->AsExpression());
    }

    for (auto *decorator : other.decorators_) {
        decorators_.emplace_back(decorator->Clone(allocator, this));
    }
}

ArrayExpression *ArrayExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<ArrayExpression>(Tag {}, *this, allocator); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

bool ArrayExpression::ConvertibleToArrayPattern()
{
    bool restFound = false;
    bool convResult = true;
    for (auto *it : elements_) {
        switch (it->Type()) {
            case AstNodeType::ARRAY_EXPRESSION: {
                convResult = it->AsArrayExpression()->ConvertibleToArrayPattern();
                break;
            }
            case AstNodeType::SPREAD_ELEMENT: {
                if (!restFound && it == elements_.back() && !trailingComma_) {
                    convResult = it->AsSpreadElement()->ConvertibleToRest(isDeclaration_);
                } else {
                    convResult = false;
                }
                restFound = true;
                break;
            }
            case AstNodeType::OBJECT_EXPRESSION: {
                convResult = it->AsObjectExpression()->ConvertibleToObjectPattern();
                break;
            }
            case AstNodeType::ASSIGNMENT_EXPRESSION: {
                convResult = it->AsAssignmentExpression()->ConvertibleToAssignmentPattern();
                break;
            }
            case AstNodeType::MEMBER_EXPRESSION:
            case AstNodeType::OMITTED_EXPRESSION:
            case AstNodeType::IDENTIFIER:
            case AstNodeType::ARRAY_PATTERN:
            case AstNodeType::OBJECT_PATTERN:
            case AstNodeType::ASSIGNMENT_PATTERN:
            case AstNodeType::REST_ELEMENT: {
                break;
            }
            default: {
                convResult = false;
                break;
            }
        }

        if (!convResult) {
            break;
        }
    }

    SetType(AstNodeType::ARRAY_PATTERN);
    return convResult;
}

ValidationInfo ArrayExpression::ValidateExpression()
{
    if (optional_) {
        return {"Unexpected token '?'.", Start()};
    }

    if (TypeAnnotation() != nullptr) {
        return {"Unexpected token.", TypeAnnotation()->Start()};
    }

    ValidationInfo info;

    for (auto *it : elements_) {
        switch (it->Type()) {
            case AstNodeType::OBJECT_EXPRESSION: {
                info = it->AsObjectExpression()->ValidateExpression();
                break;
            }
            case AstNodeType::ARRAY_EXPRESSION: {
                info = it->AsArrayExpression()->ValidateExpression();
                break;
            }
            case AstNodeType::ASSIGNMENT_EXPRESSION: {
                auto *assignmentExpr = it->AsAssignmentExpression();

                if (assignmentExpr->Left()->IsArrayExpression()) {
                    info = assignmentExpr->Left()->AsArrayExpression()->ValidateExpression();
                } else if (assignmentExpr->Left()->IsObjectExpression()) {
                    info = assignmentExpr->Left()->AsObjectExpression()->ValidateExpression();
                }

                break;
            }
            case AstNodeType::SPREAD_ELEMENT: {
                info = it->AsSpreadElement()->ValidateExpression();
                break;
            }
            default: {
                break;
            }
        }

        if (info.Fail()) {
            break;
        }
    }

    return info;
}

void ArrayExpression::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    for (auto *&it : decorators_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsDecorator();
        }
    }

    for (auto *&it : elements_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsExpression();
        }
    }

    if (auto *typeAnnotation = TypeAnnotation(); typeAnnotation != nullptr) {
        if (auto *transformedNode = cb(typeAnnotation); typeAnnotation != transformedNode) {
            typeAnnotation->SetTransformedNode(transformationName, transformedNode);
            SetTsTypeAnnotation(static_cast<TypeNode *>(transformedNode));
        }
    }
}

void ArrayExpression::Iterate(const NodeTraverser &cb) const
{
    for (auto *it : decorators_) {
        cb(it);
    }

    for (auto *it : elements_) {
        cb(it);
    }

    if (TypeAnnotation() != nullptr) {
        cb(TypeAnnotation());
    }
}

void ArrayExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", type_ == AstNodeType::ARRAY_EXPRESSION ? "ArrayExpression" : "ArrayPattern"},
                 {"decorators", AstDumper::Optional(decorators_)},
                 {"elements", elements_},
                 {"typeAnnotation", AstDumper::Optional(TypeAnnotation())},
                 {"optional", AstDumper::Optional(optional_)}});
}

void ArrayExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("[");
    for (auto element : elements_) {
        element->Dump(dumper);
        if (element != elements_.back()) {
            dumper->Add(", ");
        }
    }
    dumper->Add("]");
}

void ArrayExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ArrayExpression::Compile(compiler::ETSGen *const etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ArrayExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *CheckAssignmentPattern(Expression *it, checker::TSChecker *checker, checker::Type *elementType,
                                      bool &addOptional, checker::ElementFlags &memberFlag)
{
    auto *assignmentPattern = it->AsAssignmentPattern();
    if (assignmentPattern->Left()->IsIdentifier()) {
        const ir::Identifier *ident = assignmentPattern->Left()->AsIdentifier();
        ASSERT(ident->Variable());
        varbinder::Variable *bindingVar = ident->Variable();
        checker::Type *initializerType = checker->GetBaseTypeOfLiteralType(assignmentPattern->Right()->Check(checker));
        bindingVar->SetTsType(initializerType);
        elementType = initializerType;
    } else if (assignmentPattern->Left()->IsArrayPattern()) {
        auto savedContext = checker::SavedCheckerContext(checker, checker::CheckerStatus::FORCE_TUPLE);
        auto destructuringContext = checker::ArrayDestructuringContext(
            {checker, assignmentPattern->Left()->AsArrayPattern(), false, true, nullptr, assignmentPattern->Right()});
        destructuringContext.Start();
        elementType = destructuringContext.InferredType();
    } else {
        ASSERT(assignmentPattern->Left()->IsObjectPattern());
        auto savedContext = checker::SavedCheckerContext(checker, checker::CheckerStatus::FORCE_TUPLE);
        auto destructuringContext = checker::ObjectDestructuringContext(
            {checker, assignmentPattern->Left()->AsObjectPattern(), false, true, nullptr, assignmentPattern->Right()});
        destructuringContext.Start();
        elementType = destructuringContext.InferredType();
    }

    if (addOptional) {
        memberFlag = checker::ElementFlags::OPTIONAL;
    } else {
        memberFlag = checker::ElementFlags::REQUIRED;
    }
    return elementType;
}

checker::Type *CheckElementPattern(Expression *it, checker::Type *elementType, checker::TSChecker *checker,
                                   bool &addOptional, checker::ElementFlags &memberFlag)
{
    switch (it->Type()) {
        case ir::AstNodeType::REST_ELEMENT: {
            elementType = checker->Allocator()->New<checker::ArrayType>(checker->GlobalAnyType());
            memberFlag = checker::ElementFlags::REST;
            addOptional = false;
            return elementType;
        }
        case ir::AstNodeType::OBJECT_PATTERN: {
            elementType = it->AsObjectPattern()->CheckPattern(checker);
            memberFlag = checker::ElementFlags::REQUIRED;
            addOptional = false;
            return elementType;
        }
        case ir::AstNodeType::ARRAY_PATTERN: {
            elementType = it->AsArrayPattern()->CheckPattern(checker);
            memberFlag = checker::ElementFlags::REQUIRED;
            addOptional = false;
            return elementType;
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            return CheckAssignmentPattern(it, checker, elementType, addOptional, memberFlag);
        }
        case ir::AstNodeType::OMITTED_EXPRESSION: {
            elementType = checker->GlobalAnyType();
            memberFlag = checker::ElementFlags::REQUIRED;
            addOptional = false;
            return elementType;
        }
        case ir::AstNodeType::IDENTIFIER: {
            const ir::Identifier *ident = it->AsIdentifier();
            ASSERT(ident->Variable());
            elementType = checker->GlobalAnyType();
            ident->Variable()->SetTsType(elementType);
            memberFlag = checker::ElementFlags::REQUIRED;
            addOptional = false;
            return elementType;
        }
        default: {
            UNREACHABLE();
        }
    }
}

checker::Type *ArrayExpression::CheckPattern(checker::TSChecker *checker)
{
    checker::ObjectDescriptor *desc = checker->Allocator()->New<checker::ObjectDescriptor>(checker->Allocator());
    ArenaVector<checker::ElementFlags> elementFlags(checker->Allocator()->Adapter());
    checker::ElementFlags combinedFlags = checker::ElementFlags::NO_OPTS;
    uint32_t minLength = 0;
    uint32_t index = elements_.size();
    bool addOptional = true;

    for (auto it = elements_.rbegin(); it != elements_.rend(); it++) {
        checker::Type *elementType = nullptr;
        checker::ElementFlags memberFlag = checker::ElementFlags::NO_OPTS;

        elementType = CheckElementPattern(*it, elementType, checker, addOptional, memberFlag);

        util::StringView memberIndex = util::Helpers::ToStringView(checker->Allocator(), index - 1);

        auto *memberVar =
            varbinder::Scope::CreateVar(checker->Allocator(), memberIndex, varbinder::VariableFlags::PROPERTY, *it);

        if (memberFlag == checker::ElementFlags::OPTIONAL) {
            memberVar->AddFlag(varbinder::VariableFlags::OPTIONAL);
        } else {
            minLength++;
        }

        memberVar->SetTsType(elementType);
        elementFlags.push_back(memberFlag);
        desc->properties.insert(desc->properties.begin(), memberVar);

        combinedFlags |= memberFlag;
        index--;
    }

    const checker::TupleTypeInfo tupleTypeInfo = {combinedFlags, minLength,
                                                  static_cast<uint32_t>(desc->properties.size()), false};
    return checker->CreateTupleType(desc, std::move(elementFlags), tupleTypeInfo);
}

bool ArrayExpression::HandleNestedArrayExpression(checker::ETSChecker *const checker,
                                                  ArrayExpression *const currentElement, const bool isPreferredTuple,
                                                  const std::size_t idx)
{
    if (isPreferredTuple) {
        currentElement->SetPreferredType(preferredType_->AsETSTupleType()->GetTypeAtIndex(idx));

        if (currentElement->GetPreferredType()->IsETSTupleType()) {
            if (!checker->ValidateTupleMinElementSize(currentElement,
                                                      currentElement->GetPreferredType()->AsETSTupleType())) {
                return false;
            }
        }

        return true;
    }

    if (preferredType_->IsETSArrayType()) {
        if (preferredType_->AsETSArrayType()->ElementType()->IsETSTupleType()) {
            if (!checker->ValidateTupleMinElementSize(
                    // CC-OFFNXT(G.FMT.06-CPP) project code style
                    currentElement, preferredType_->AsETSArrayType()->ElementType()->AsETSTupleType())) {
                return false;
            }
        }

        currentElement->SetPreferredType(preferredType_->AsETSArrayType()->ElementType());
        return true;
    }

    if (currentElement->GetPreferredType() == nullptr) {
        currentElement->SetPreferredType(preferredType_);
    }
    return true;
}

checker::Type *ArrayExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

void ArrayExpression::GetPrefferedTypeFromFuncParam(checker::ETSChecker *checker, Expression *param,
                                                    checker::TypeRelationFlag flags)
{
    if (preferredType_ != nullptr) {
        return;
    }
    auto paramType = param->Check(checker);
    if (paramType->IsETSArrayType()) {
        auto *elementType = paramType->AsETSArrayType()->ElementType();
        bool isAssignable = true;
        for (auto elem : elements_) {
            auto assignCtx =
                checker::AssignmentContext(checker->Relation(), elem, elem->Check(checker), elementType, elem->Start(),
                                           {""}, checker::TypeRelationFlag::NO_THROW | flags);
            isAssignable &= assignCtx.IsAssignable();
        }
        if (isAssignable) {
            preferredType_ = paramType;
        }
    }
}

}  // namespace ark::es2panda::ir
