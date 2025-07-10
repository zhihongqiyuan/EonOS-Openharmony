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

#include "objectExpression.h"

#include "ir/base/decorator.h"
#include "util/helpers.h"
#include "compiler/base/literals.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "checker/TSchecker.h"
#include "checker/ETSchecker.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/ts/destructuringContext.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/typeNode.h"
#include "ir/base/property.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/spreadElement.h"
#include "ir/expressions/arrayExpression.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/validationInfo.h"

namespace ark::es2panda::ir {
ObjectExpression::ObjectExpression([[maybe_unused]] Tag const tag, ObjectExpression const &other,
                                   ArenaAllocator *const allocator)
    : AnnotatedExpression(static_cast<AnnotatedExpression const &>(other), allocator),
      decorators_(allocator->Adapter()),
      properties_(allocator->Adapter())
{
    preferredType_ = other.preferredType_;
    isDeclaration_ = other.isDeclaration_;
    trailingComma_ = other.trailingComma_;
    optional_ = other.optional_;

    for (auto *property : other.properties_) {
        properties_.emplace_back(property->Clone(allocator, this)->AsExpression());
    }

    for (auto *decorator : other.decorators_) {
        decorators_.emplace_back(decorator->Clone(allocator, this));
    }
}

ObjectExpression *ObjectExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<ObjectExpression>(Tag {}, *this, allocator); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }
    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

static std::pair<ValidationInfo, bool> ValidateProperty(Property *prop, bool &foundProto)
{
    ValidationInfo info = prop->ValidateExpression();
    if (prop->Kind() == PropertyKind::PROTO) {
        if (foundProto) {
            return {{"Duplicate __proto__ fields are not allowed in object literals", prop->Key()->Start()}, true};
        }

        foundProto = true;
    }

    return {info, false};
}

ValidationInfo ObjectExpression::ValidateExpression()
{
    if (optional_) {
        return {"Unexpected token '?'.", Start()};
    }

    if (TypeAnnotation() != nullptr) {
        return {"Unexpected token.", TypeAnnotation()->Start()};
    }

    ValidationInfo info;
    bool foundProto = false;

    for (auto *it : properties_) {
        switch (it->Type()) {
            case AstNodeType::OBJECT_EXPRESSION:
            case AstNodeType::ARRAY_EXPRESSION: {
                return {"Unexpected token.", it->Start()};
            }
            case AstNodeType::SPREAD_ELEMENT: {
                info = it->AsSpreadElement()->ValidateExpression();
                break;
            }
            case AstNodeType::PROPERTY: {
                auto *prop = it->AsProperty();
                bool ret = false;
                std::tie(info, ret) = ValidateProperty(prop, foundProto);
                if (ret) {
                    return info;
                }
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

bool ObjectExpression::ConvertibleToObjectPattern()
{
    // NOTE: rsipka. throw more precise messages in case of false results
    bool restFound = false;
    bool convResult = true;

    for (auto *it : properties_) {
        switch (it->Type()) {
            case AstNodeType::ARRAY_EXPRESSION: {
                convResult = it->AsArrayExpression()->ConvertibleToArrayPattern();
                break;
            }
            case AstNodeType::SPREAD_ELEMENT: {
                if (!restFound && it == properties_.back() && !trailingComma_) {
                    convResult = it->AsSpreadElement()->ConvertibleToRest(isDeclaration_, false);
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
            case AstNodeType::META_PROPERTY_EXPRESSION:
            case AstNodeType::CHAIN_EXPRESSION:
            case AstNodeType::SEQUENCE_EXPRESSION: {
                convResult = false;
                break;
            }
            case AstNodeType::PROPERTY: {
                convResult = it->AsProperty()->ConvertibleToPatternProperty();
                break;
            }
            default: {
                break;
            }
        }

        if (!convResult) {
            break;
        }
    }

    SetType(AstNodeType::OBJECT_PATTERN);
    return convResult;
}

void ObjectExpression::SetDeclaration()
{
    isDeclaration_ = true;
}

void ObjectExpression::SetOptional(bool optional)
{
    optional_ = optional;
}

void ObjectExpression::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    for (auto *&it : decorators_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsDecorator();
        }
    }

    for (auto *&it : properties_) {
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

void ObjectExpression::Iterate(const NodeTraverser &cb) const
{
    for (auto *it : decorators_) {
        cb(it);
    }

    for (auto *it : properties_) {
        cb(it);
    }

    if (TypeAnnotation() != nullptr) {
        cb(TypeAnnotation());
    }
}

void ObjectExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", (type_ == AstNodeType::OBJECT_EXPRESSION) ? "ObjectExpression" : "ObjectPattern"},
                 {"decorators", AstDumper::Optional(decorators_)},
                 {"properties", properties_},
                 {"typeAnnotation", AstDumper::Optional(TypeAnnotation())},
                 {"optional", AstDumper::Optional(optional_)}});
}

void ObjectExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("{");
    if (!properties_.empty()) {
        dumper->IncrIndent();
        dumper->Endl();
        for (auto property : properties_) {
            property->Dump(dumper);
            dumper->Add(",");
            if (property == properties_.back()) {
                dumper->DecrIndent();
            }
            dumper->Endl();
        }
    }
    dumper->Add("}");
}

void ObjectExpression::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

// CC-OFFNXT(G.FMT.10-CPP) project code style
std::tuple<bool, varbinder::Variable *, checker::Type *, varbinder::LocalVariable *>
ObjectExpression::CheckPatternIsShorthand(CheckPatternIsShorthandArgs *args)
{
    if (args->prop->IsShorthand()) {
        switch (args->prop->Value()->Type()) {
            case ir::AstNodeType::IDENTIFIER: {
                const ir::Identifier *ident = args->prop->Value()->AsIdentifier();
                ASSERT(ident->Variable());
                args->bindingVar = ident->Variable();
                break;
            }
            case ir::AstNodeType::ASSIGNMENT_PATTERN: {
                auto *assignmentPattern = args->prop->Value()->AsAssignmentPattern();
                args->patternParamType = assignmentPattern->Right()->Check(args->checker);
                ASSERT(assignmentPattern->Left()->AsIdentifier()->Variable());
                args->bindingVar = assignmentPattern->Left()->AsIdentifier()->Variable();
                args->isOptional = true;
                break;
            }
            default: {
                UNREACHABLE();
            }
        }
        return {args->isOptional, args->bindingVar, args->patternParamType, args->foundVar};
    }

    switch (args->prop->Value()->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            args->bindingVar = args->prop->Value()->AsIdentifier()->Variable();
            break;
        }
        case ir::AstNodeType::ARRAY_PATTERN: {
            args->patternParamType = args->prop->Value()->AsArrayPattern()->CheckPattern(args->checker);
            break;
        }
        case ir::AstNodeType::OBJECT_PATTERN: {
            args->patternParamType = args->prop->Value()->AsObjectPattern()->CheckPattern(args->checker);
            break;
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            args->isOptional = CheckAssignmentPattern(args->prop, args->bindingVar, args->patternParamType,
                                                      args->checker, args->foundVar);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
    return {args->isOptional, args->bindingVar, args->patternParamType, args->foundVar};
}

checker::Type *ObjectExpression::CheckPattern(checker::TSChecker *checker)
{
    checker::ObjectDescriptor *desc = checker->Allocator()->New<checker::ObjectDescriptor>(checker->Allocator());

    bool isOptional = false;

    for (auto it = properties_.rbegin(); it != properties_.rend(); it++) {
        if ((*it)->IsRestElement()) {
            ASSERT((*it)->AsRestElement()->Argument()->IsIdentifier());
            util::StringView indexInfoName("x");
            auto *newIndexInfo =
                checker->Allocator()->New<checker::IndexInfo>(checker->GlobalAnyType(), indexInfoName, false);
            desc->stringIndexInfo = newIndexInfo;
            continue;
        }

        ASSERT((*it)->IsProperty());
        auto *prop = (*it)->AsProperty();

        if (prop->IsComputed()) {
            continue;
        }

        varbinder::LocalVariable *foundVar = desc->FindProperty(prop->Key()->AsIdentifier()->Name());
        checker::Type *patternParamType = checker->GlobalAnyType();
        varbinder::Variable *bindingVar = nullptr;
        auto args = CheckPatternIsShorthandArgs {checker, patternParamType, bindingVar, prop, foundVar, isOptional};

        std::tie(isOptional, bindingVar, patternParamType, foundVar) = CheckPatternIsShorthand(&args);

        if (bindingVar != nullptr) {
            bindingVar->SetTsType(patternParamType);
        }

        if (foundVar != nullptr) {
            continue;
        }

        varbinder::LocalVariable *patternVar = varbinder::Scope::CreateVar(
            checker->Allocator(), prop->Key()->AsIdentifier()->Name(), varbinder::VariableFlags::PROPERTY, *it);
        patternVar->SetTsType(patternParamType);

        if (isOptional) {
            patternVar->AddFlag(varbinder::VariableFlags::OPTIONAL);
        }

        desc->properties.insert(desc->properties.begin(), patternVar);
    }

    checker::Type *returnType = checker->Allocator()->New<checker::ObjectLiteralType>(desc);
    returnType->AsObjectType()->AddObjectFlag(checker::ObjectFlags::RESOLVED_MEMBERS);
    return returnType;
}

bool ObjectExpression::CheckAssignmentPattern(Property *prop, varbinder::Variable *&bindingVar,
                                              checker::Type *&patternParamType, checker::TSChecker *checker,
                                              varbinder::LocalVariable *foundVar)
{
    auto *assignmentPattern = prop->Value()->AsAssignmentPattern();

    if (assignmentPattern->Left()->IsIdentifier()) {
        bindingVar = assignmentPattern->Left()->AsIdentifier()->Variable();
        patternParamType = checker->GetBaseTypeOfLiteralType(assignmentPattern->Right()->Check(checker));
        return true;
    }

    if (assignmentPattern->Left()->IsArrayPattern()) {
        auto savedContext = checker::SavedCheckerContext(checker, checker::CheckerStatus::FORCE_TUPLE);
        auto destructuringContext = checker::ArrayDestructuringContext(
            {checker, assignmentPattern->Left()->AsArrayPattern(), false, true, nullptr, assignmentPattern->Right()});

        if (foundVar != nullptr) {
            destructuringContext.SetInferredType(
                checker->CreateUnionType({foundVar->TsType(), destructuringContext.InferredType()}));
        }

        destructuringContext.Start();
        patternParamType = destructuringContext.InferredType();
        return true;
    }

    ASSERT(assignmentPattern->Left()->IsObjectPattern());
    auto savedContext = checker::SavedCheckerContext(checker, checker::CheckerStatus::FORCE_TUPLE);
    auto destructuringContext = checker::ObjectDestructuringContext(
        {checker, assignmentPattern->Left()->AsObjectPattern(), false, true, nullptr, assignmentPattern->Right()});

    if (foundVar != nullptr) {
        destructuringContext.SetInferredType(
            checker->CreateUnionType({foundVar->TsType(), destructuringContext.InferredType()}));
    }

    destructuringContext.Start();
    patternParamType = destructuringContext.InferredType();
    return true;
}

checker::Type *ObjectExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

void ObjectExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ObjectExpression::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
