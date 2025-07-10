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

#include "ir/typeNode.h"
#include "ir/expressions/literals/stringLiteral.h"
#include "ir/expressions/literals/bigIntLiteral.h"
#include "ir/expressions/literals/numberLiteral.h"
#include "ir/expressions/arrayExpression.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/objectExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/property.h"
#include "ir/base/spreadElement.h"
#include "ir/statements/blockStatement.h"
#include "ir/statements/returnStatement.h"
#include "ir/statements/functionDeclaration.h"
#include "util/helpers.h"
#include "varbinder/variable.h"
#include "varbinder/scope.h"
#include "varbinder/declaration.h"

#include "checker/TSchecker.h"
#include "checker/ts/destructuringContext.h"
#include "checker/types/ts/objectDescriptor.h"
#include "checker/types/ts/objectType.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

namespace ark::es2panda::checker {
Type *TSChecker::HandleFunctionReturn(ir::ScriptFunction *func)
{
    if (func->ReturnTypeAnnotation() != nullptr) {
        func->ReturnTypeAnnotation()->Check(this);
        Type *returnType = func->ReturnTypeAnnotation()->GetType(this);

        if (func->IsArrow() && func->Body()->IsExpression()) {
            ElaborateElementwise(returnType, func->Body()->AsExpression(), func->Body()->Start());
        }

        if (returnType->IsNeverType()) {
            ThrowTypeError("A function returning 'never' cannot have a reachable end point.",
                           func->ReturnTypeAnnotation()->Start());
        }

        if (!MaybeTypeOfKind(returnType, TypeFlag::ANY_OR_VOID)) {
            CheckAllCodePathsInNonVoidFunctionReturnOrThrow(
                func, func->ReturnTypeAnnotation()->Start(),
                "A function whose declared type is neither 'void' nor 'any' must return a value.");
        }

        return returnType;
    }

    if (func->IsDeclare()) {
        return GlobalAnyType();
    }

    if (func->IsArrow() && func->Body()->IsExpression()) {
        return func->Body()->Check(this);
    }

    ArenaVector<Type *> returnTypes(Allocator()->Adapter());
    CollectTypesFromReturnStatements(func->Body(), &returnTypes);

    if (returnTypes.empty()) {
        return GlobalVoidType();
    }

    if (returnTypes.size() == 1 && returnTypes[0] == GlobalResolvingReturnType()) {
        ThrowReturnTypeCircularityError(func);
    }

    for (auto *it : returnTypes) {
        if (it == GlobalResolvingReturnType()) {
            ThrowReturnTypeCircularityError(func);
        }
    }

    return CreateUnionType(std::move(returnTypes));
}

void TSChecker::ThrowReturnTypeCircularityError(ir::ScriptFunction *func)
{
    if (func->ReturnTypeAnnotation() != nullptr) {
        ThrowTypeError("Return type annotation circularly reference itself", func->ReturnTypeAnnotation()->Start());
    }

    if (func->Id() != nullptr) {
        ThrowTypeError({func->Id()->AsIdentifier()->Name(),
                        " implicitly has return type 'any' because it does not have a return type annotation and is "
                        "referenced directly or indirectly in one of its return expressions."},
                       func->Id()->Start());
    }

    ThrowTypeError(
        "Function implicitly has return type 'any' because it does not have a return type annotation and is "
        "referenced directly or indirectly in one of its return expressions.",
        func->Start());
}

std::tuple<varbinder::LocalVariable *, varbinder::LocalVariable *, bool> TSChecker::CheckFunctionIdentifierParameter(
    ir::Identifier *param)
{
    ASSERT(param->Variable());
    varbinder::Variable *paramVar = param->Variable();
    bool isOptional = param->IsOptional();

    if (param->TypeAnnotation() == nullptr) {
        ThrowTypeError({"Parameter ", param->Name(), " implicitly has any type."}, param->Start());
    }

    if (isOptional) {
        paramVar->AddFlag(varbinder::VariableFlags::OPTIONAL);
    }

    param->TypeAnnotation()->Check(this);
    paramVar->SetTsType(param->TypeAnnotation()->GetType(this));
    return {paramVar->AsLocalVariable(), nullptr, isOptional};
}

Type *TSChecker::CreateParameterTypeForArrayAssignmentPattern(ir::ArrayExpression *arrayPattern, Type *inferredType)
{
    if (!inferredType->IsObjectType()) {
        return inferredType;
    }

    ASSERT(inferredType->AsObjectType()->IsTupleType());
    TupleType *inferredTuple = inferredType->AsObjectType()->AsTupleType();

    if (inferredTuple->FixedLength() > arrayPattern->Elements().size()) {
        return inferredType;
    }

    TupleType *newTuple =
        inferredTuple->Instantiate(Allocator(), Relation(), GetGlobalTypesHolder())->AsObjectType()->AsTupleType();

    for (uint32_t index = inferredTuple->FixedLength(); index < arrayPattern->Elements().size(); index++) {
        util::StringView memberIndex = util::Helpers::ToStringView(Allocator(), index);
        varbinder::LocalVariable *newMember = varbinder::Scope::CreateVar(
            Allocator(), memberIndex, varbinder::VariableFlags::PROPERTY | varbinder::VariableFlags::OPTIONAL, nullptr);
        newMember->SetTsType(GlobalAnyType());
        newTuple->AddProperty(newMember);
    }

    return newTuple;
}

Type *TSChecker::CreateParameterTypeForObjectAssignmentPattern(ir::ObjectExpression *objectPattern, Type *inferredType)
{
    if (!inferredType->IsObjectType()) {
        return inferredType;
    }

    ObjectType *newObject = inferredType->Instantiate(Allocator(), Relation(), GetGlobalTypesHolder())->AsObjectType();

    for (auto *it : objectPattern->Properties()) {
        if (it->IsRestElement()) {
            continue;
        }

        ir::Property *prop = it->AsProperty();
        varbinder::LocalVariable *foundVar = newObject->GetProperty(prop->Key()->AsIdentifier()->Name(), true);

        if (foundVar != nullptr) {
            if (prop->Value()->IsAssignmentPattern()) {
                foundVar->AddFlag(varbinder::VariableFlags::OPTIONAL);
            }

            continue;
        }

        ASSERT(prop->Value()->IsAssignmentPattern());
        ir::AssignmentExpression *assignmentPattern = prop->Value()->AsAssignmentPattern();

        varbinder::LocalVariable *newProp = varbinder::Scope::CreateVar(
            Allocator(), prop->Key()->AsIdentifier()->Name(),
            varbinder::VariableFlags::PROPERTY | varbinder::VariableFlags::OPTIONAL, nullptr);
        newProp->SetTsType(GetBaseTypeOfLiteralType(CheckTypeCached(assignmentPattern->Right())));
        newObject->AddProperty(newProp);
    }

    newObject->AddObjectFlag(ObjectFlags::RESOLVED_MEMBERS);
    return newObject;
}

using ReturnedVariable = std::tuple<varbinder::LocalVariable *, varbinder::LocalVariable *, bool>;
ReturnedVariable TSChecker::CheckFunctionAssignmentPatternParameter(ir::AssignmentExpression *param)
{
    if (param->Left()->IsIdentifier()) {
        ir::Identifier *paramIdent = param->Left()->AsIdentifier();
        varbinder::Variable *paramVar = paramIdent->Variable();
        ASSERT(paramVar);

        if (paramIdent->TypeAnnotation() != nullptr) {
            paramIdent->TypeAnnotation()->Check(this);
            Type *paramType = paramIdent->TypeAnnotation()->GetType(this);
            paramVar->SetTsType(paramType);
            ElaborateElementwise(paramType, param->Right(), paramIdent->Start());
            return {paramVar->AsLocalVariable(), nullptr, true};
        }

        paramVar->SetTsType(GetBaseTypeOfLiteralType(param->Right()->Check(this)));
        paramVar->AddFlag(varbinder::VariableFlags::OPTIONAL);
        return {paramVar->AsLocalVariable(), nullptr, true};
    }

    Type *paramType = nullptr;
    std::stringstream ss;

    auto savedContext = SavedCheckerContext(this, CheckerStatus::FORCE_TUPLE | CheckerStatus::IN_PARAMETER);

    if (param->Left()->IsArrayPattern()) {
        ir::ArrayExpression *arrayPattern = param->Left()->AsArrayPattern();
        auto context = ArrayDestructuringContext(
            {this, arrayPattern, false, true, arrayPattern->TypeAnnotation(), param->Right()});
        context.Start();
        paramType = CreateParameterTypeForArrayAssignmentPattern(arrayPattern, context.InferredType());
        CreatePatternParameterName(param->Left(), ss);
    } else {
        ir::ObjectExpression *objectPattern = param->Left()->AsObjectPattern();
        auto context = ObjectDestructuringContext(
            {this, objectPattern, false, true, objectPattern->TypeAnnotation(), param->Right()});
        context.Start();
        paramType = CreateParameterTypeForObjectAssignmentPattern(objectPattern, context.InferredType());
        CreatePatternParameterName(param->Left(), ss);
    }

    util::UString pn(ss.str(), Allocator());
    varbinder::LocalVariable *patternVar =
        varbinder::Scope::CreateVar(Allocator(), pn.View(), varbinder::VariableFlags::NONE, param);
    patternVar->SetTsType(paramType);
    patternVar->AddFlag(varbinder::VariableFlags::OPTIONAL);
    return {patternVar->AsLocalVariable(), nullptr, true};
}

std::tuple<varbinder::LocalVariable *, varbinder::LocalVariable *, bool> TSChecker::CheckFunctionRestParameter(
    ir::SpreadElement *param, SignatureInfo *signatureInfo)
{
    ir::TypeNode *typeAnnotation = nullptr;
    if (param->Argument() != nullptr) {
        typeAnnotation = param->Argument()->AsAnnotatedExpression()->TypeAnnotation();
    }

    Type *restType = Allocator()->New<ArrayType>(GlobalAnyType());

    if (typeAnnotation != nullptr) {
        typeAnnotation->Check(this);
        restType = typeAnnotation->GetType(this);
        if (!restType->IsArrayType()) {
            ThrowTypeError("A rest parameter must be of an array type", param->Start());
        }
    }

    switch (param->Argument()->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            ir::Identifier *restIdent = param->Argument()->AsIdentifier();
            ASSERT(restIdent->Variable());
            restIdent->Variable()->SetTsType(restType->AsArrayType()->ElementType());
            return {nullptr, restIdent->Variable()->AsLocalVariable(), false};
        }
        case ir::AstNodeType::OBJECT_PATTERN: {
            ASSERT(param->Argument()->IsObjectPattern());
            auto savedContext = SavedCheckerContext(this, CheckerStatus::FORCE_TUPLE);
            auto destructuringContext =
                ObjectDestructuringContext({this, param->Argument(), false, false, nullptr, nullptr});
            destructuringContext.SetInferredType(restType);
            destructuringContext.SetSignatureInfo(signatureInfo);
            destructuringContext.Start();
            return {nullptr, nullptr, false};
        }
        case ir::AstNodeType::ARRAY_PATTERN: {
            auto savedContext = SavedCheckerContext(this, CheckerStatus::FORCE_TUPLE);
            auto destructuringContext =
                ArrayDestructuringContext({this, param->Argument(), false, false, nullptr, nullptr});
            destructuringContext.SetInferredType(restType);
            destructuringContext.SetSignatureInfo(signatureInfo);
            destructuringContext.Start();
            return {nullptr, nullptr, false};
        }
        default: {
            UNREACHABLE();
        }
    }
}

std::tuple<varbinder::LocalVariable *, varbinder::LocalVariable *, bool> TSChecker::CheckFunctionArrayPatternParameter(
    ir::ArrayExpression *param)
{
    std::stringstream ss;
    CreatePatternParameterName(param, ss);
    util::UString pn(ss.str(), Allocator());
    varbinder::LocalVariable *patternVar =
        varbinder::Scope::CreateVar(Allocator(), pn.View(), varbinder::VariableFlags::NONE, param);

    if (param->TypeAnnotation() != nullptr) {
        auto savedContext = SavedCheckerContext(this, CheckerStatus::FORCE_TUPLE);
        auto destructuringContext =
            ArrayDestructuringContext({this, param->AsArrayPattern(), false, false, param->TypeAnnotation(), nullptr});
        destructuringContext.Start();
        patternVar->SetTsType(destructuringContext.InferredType());
        return {patternVar->AsLocalVariable(), nullptr, false};
    }

    patternVar->SetTsType(param->CheckPattern(this));
    return {patternVar->AsLocalVariable(), nullptr, false};
}

std::tuple<varbinder::LocalVariable *, varbinder::LocalVariable *, bool> TSChecker::CheckFunctionObjectPatternParameter(
    ir::ObjectExpression *param)
{
    std::stringstream ss;
    CreatePatternParameterName(param, ss);
    util::UString pn(ss.str(), Allocator());
    varbinder::LocalVariable *patternVar =
        varbinder::Scope::CreateVar(Allocator(), pn.View(), varbinder::VariableFlags::NONE, param);

    if (param->TypeAnnotation() != nullptr) {
        auto savedContext = SavedCheckerContext(this, CheckerStatus::FORCE_TUPLE);
        auto destructuringContext = ObjectDestructuringContext(
            {this, param->AsObjectPattern(), false, false, param->TypeAnnotation(), nullptr});
        destructuringContext.Start();
        patternVar->SetTsType(destructuringContext.InferredType());
        return {patternVar->AsLocalVariable(), nullptr, false};
    }

    patternVar->SetTsType(param->CheckPattern(this));
    return {patternVar->AsLocalVariable(), nullptr, false};
}

std::tuple<varbinder::LocalVariable *, varbinder::LocalVariable *, bool> TSChecker::CheckFunctionParameter(
    ir::Expression *param, SignatureInfo *signatureInfo)
{
    std::tuple<varbinder::LocalVariable *, varbinder::LocalVariable *, bool> result;
    if (param->TsType() != nullptr) {
        ASSERT(param->TsType()->Variable());
        varbinder::Variable *var = param->TsType()->Variable();
        result = {var->AsLocalVariable(), nullptr, var->HasFlag(varbinder::VariableFlags::OPTIONAL)};
        return result;
    }

    bool cache = true;
    switch (param->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            result = CheckFunctionIdentifierParameter(param->AsIdentifier());
            break;
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            result = CheckFunctionAssignmentPatternParameter(param->AsAssignmentPattern());
            break;
        }
        case ir::AstNodeType::REST_ELEMENT: {
            result = CheckFunctionRestParameter(param->AsRestElement(), signatureInfo);
            cache = false;
            break;
        }
        case ir::AstNodeType::ARRAY_PATTERN: {
            result = CheckFunctionArrayPatternParameter(param->AsArrayPattern());
            break;
        }
        case ir::AstNodeType::OBJECT_PATTERN: {
            result = CheckFunctionObjectPatternParameter(param->AsObjectPattern());
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    if (cache) {
        Type *placeholder = Allocator()->New<ArrayType>(GlobalAnyType());
        placeholder->SetVariable(std::get<0>(result));
        param->SetTsType(placeholder);
    }

    return result;
}

void TSChecker::CheckFunctionParameterDeclarations(const ArenaVector<ir::Expression *> &params,
                                                   SignatureInfo *signatureInfo)
{
    signatureInfo->restVar = nullptr;
    signatureInfo->minArgCount = 0;

    for (auto it = params.rbegin(); it != params.rend(); it++) {
        auto [paramVar, restVar, isOptional] = CheckFunctionParameter(*it, signatureInfo);

        if (restVar != nullptr) {
            signatureInfo->restVar = restVar;
            continue;
        }

        if (paramVar == nullptr) {
            continue;
        }

        signatureInfo->params.insert(signatureInfo->params.begin(), paramVar);

        if (!isOptional) {
            signatureInfo->minArgCount++;
        }
    }
}

bool ShouldCreatePropertyValueName(ir::Expression *propValue)
{
    return propValue->IsArrayPattern() || propValue->IsObjectPattern() ||
           (propValue->IsAssignmentPattern() && (propValue->AsAssignmentPattern()->Left()->IsArrayPattern() ||
                                                 propValue->AsAssignmentPattern()->Left()->IsObjectPattern()));
}

void TSChecker::HandlePropertyPatternParameterName(ir::Property *prop, std::stringstream &ss)
{
    util::StringView propName;
    if (prop->Key()->IsIdentifier()) {
        propName = prop->Key()->AsIdentifier()->Name();
    } else {
        switch (prop->Key()->Type()) {
            case ir::AstNodeType::NUMBER_LITERAL: {
                propName =
                    util::Helpers::ToStringView(Allocator(), prop->Key()->AsNumberLiteral()->Number().GetDouble());
                break;
            }
            case ir::AstNodeType::BIGINT_LITERAL: {
                propName = prop->Key()->AsBigIntLiteral()->Str();
                break;
            }
            case ir::AstNodeType::STRING_LITERAL: {
                propName = prop->Key()->AsStringLiteral()->Str();
                break;
            }
            default: {
                UNREACHABLE();
                break;
            }
        }
    }

    ss << propName;

    if (ShouldCreatePropertyValueName(prop->Value())) {
        ss << ": ";
        TSChecker::CreatePatternParameterName(prop->Value(), ss);
    }
}

void TSChecker::CreatePatternParameterName(ir::AstNode *node, std::stringstream &ss)
{
    switch (node->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            ss << node->AsIdentifier()->Name();
            break;
        }
        case ir::AstNodeType::ARRAY_PATTERN: {
            ss << "[";

            const auto &elements = node->AsArrayPattern()->Elements();
            for (auto it = elements.begin(); it != elements.end(); it++) {
                CreatePatternParameterName(*it, ss);
                if (std::next(it) != elements.end()) {
                    ss << ", ";
                }
            }

            ss << "]";
            break;
        }
        case ir::AstNodeType::OBJECT_PATTERN: {
            ss << "{ ";

            const auto &properties = node->AsObjectPattern()->Properties();
            for (auto it = properties.begin(); it != properties.end(); it++) {
                CreatePatternParameterName(*it, ss);
                if (std::next(it) != properties.end()) {
                    ss << ", ";
                }
            }

            ss << " }";
            break;
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            CreatePatternParameterName(node->AsAssignmentPattern()->Left(), ss);
            break;
        }
        case ir::AstNodeType::PROPERTY: {
            HandlePropertyPatternParameterName(node->AsProperty(), ss);
            break;
        }
        case ir::AstNodeType::REST_ELEMENT: {
            ss << "...";
            TSChecker::CreatePatternParameterName(node->AsRestElement()->Argument(), ss);
            break;
        }
        default:
            break;
    }
}

ir::Statement *FindSubsequentFunctionNode(ir::BlockStatement *block, ir::ScriptFunction *node)
{
    for (auto it = block->Statements().begin(); it != block->Statements().end(); it++) {
        if ((*it)->IsFunctionDeclaration() && (*it)->AsFunctionDeclaration()->Function() == node) {
            return *(++it);
        }
    }

    UNREACHABLE();
    return nullptr;
}

void TSChecker::ValidateSubsequentNode(const ir::Statement *const subsequentNode, const ir::ScriptFunction *const func)
{
    if (!subsequentNode->IsFunctionDeclaration()) {
        ThrowTypeError("Function implementation is missing or not immediately following the declaration.",
                       func->Id()->Start());
    }

    const ir::ScriptFunction *const subsequentFunc = subsequentNode->AsFunctionDeclaration()->Function();
    if (subsequentFunc->Id()->Name() != func->Id()->Name()) {
        ThrowTypeError("Function implementation is missing or not immediately following the declaration.",
                       func->Id()->Start());
    }

    if (subsequentFunc->IsDeclare() != func->IsDeclare()) {
        ThrowTypeError("Overload signatures must all be ambient or non-ambient.", func->Id()->Start());
    }
}

void TSChecker::CheckOverloadSignatureCompatibility(Signature *bodyCallSignature, Signature *signature)
{
    if (bodyCallSignature->ReturnType()->IsVoidType() ||
        IsTypeAssignableTo(bodyCallSignature->ReturnType(), signature->ReturnType()) ||
        IsTypeAssignableTo(signature->ReturnType(), bodyCallSignature->ReturnType())) {
        bodyCallSignature->AssignmentTarget(Relation(), signature);

        if (Relation()->IsTrue()) {
            return;
        }
    }

    ASSERT(signature->Function());
    ThrowTypeError("This overload signature is not compatible with its implementation signature",
                   signature->Function()->Id()->Start());
}

void TSChecker::InferFunctionDeclarationType(const varbinder::FunctionDecl *decl, varbinder::Variable *funcVar)
{
    ir::ScriptFunction *bodyDeclaration = decl->Decls().back();
    if (bodyDeclaration->IsOverload()) {
        ThrowTypeError("Function implementation is missing or not immediately following the declaration.",
                       bodyDeclaration->Id()->Start());
    }

    ObjectDescriptor *descWithOverload = Allocator()->New<ObjectDescriptor>(Allocator());
    for (auto it = decl->Decls().begin(); it != decl->Decls().end() - 1; it++) {
        ir::ScriptFunction *func = *it;
        ASSERT(func->IsOverload() && (*it)->Parent()->Parent()->IsBlockStatement());
        ir::Statement *subsequentNode = FindSubsequentFunctionNode((*it)->Parent()->Parent()->AsBlockStatement(), func);
        ASSERT(subsequentNode);
        ValidateSubsequentNode(subsequentNode, func);

        ScopeContext scopeCtx(this, func->Scope());

        auto *overloadSignatureInfo = Allocator()->New<checker::SignatureInfo>(Allocator());
        CheckFunctionParameterDeclarations(func->Params(), overloadSignatureInfo);

        Type *returnType = GlobalAnyType();

        if (func->ReturnTypeAnnotation() != nullptr) {
            func->ReturnTypeAnnotation()->Check(this);
            returnType = func->ReturnTypeAnnotation()->GetType(this);
        }

        Signature *overloadSignature = Allocator()->New<checker::Signature>(overloadSignatureInfo, returnType, func);
        descWithOverload->callSignatures.push_back(overloadSignature);
    }

    ScopeContext scopeCtx(this, bodyDeclaration->Scope());

    auto *signatureInfo = Allocator()->New<checker::SignatureInfo>(Allocator());
    CheckFunctionParameterDeclarations(bodyDeclaration->Params(), signatureInfo);
    auto *bodyCallSignature = Allocator()->New<checker::Signature>(signatureInfo, GlobalResolvingReturnType());

    if (descWithOverload->callSignatures.empty()) {
        Type *funcType = CreateFunctionTypeWithSignature(bodyCallSignature);
        funcType->SetVariable(funcVar);
        funcVar->SetTsType(funcType);
    }

    bodyCallSignature->SetReturnType(HandleFunctionReturn(bodyDeclaration));

    if (!descWithOverload->callSignatures.empty()) {
        Type *funcType = Allocator()->New<FunctionType>(descWithOverload);
        funcType->SetVariable(funcVar);
        funcVar->SetTsType(funcType);

        for (auto *iter : descWithOverload->callSignatures) {
            CheckOverloadSignatureCompatibility(bodyCallSignature, iter);
        }
    }
}

void TSChecker::CollectTypesFromReturnStatements(ir::AstNode *parent, ArenaVector<Type *> *returnTypes)
{
    parent->Iterate([this, returnTypes](ir::AstNode *childNode) -> void {
        if (childNode->IsScriptFunction()) {
            return;
        }

        if (childNode->IsReturnStatement()) {
            ir::ReturnStatement *returnStmt = childNode->AsReturnStatement();

            if (returnStmt->Argument() == nullptr) {
                return;
            }

            returnTypes->push_back(
                GetBaseTypeOfLiteralType(CheckTypeCached(childNode->AsReturnStatement()->Argument())));
        }

        CollectTypesFromReturnStatements(childNode, returnTypes);
    });
}

static bool SearchForReturnOrThrow(ir::AstNode *parent)
{
    bool found = false;

    parent->Iterate([&found](ir::AstNode *childNode) -> void {
        if (childNode->IsThrowStatement() || childNode->IsReturnStatement()) {
            found = true;
            return;
        }

        if (childNode->IsScriptFunction()) {
            return;
        }

        SearchForReturnOrThrow(childNode);
    });

    return found;
}

void TSChecker::CheckAllCodePathsInNonVoidFunctionReturnOrThrow(ir::ScriptFunction *func,
                                                                lexer::SourcePosition lineInfo, const char *errMsg)
{
    if (!SearchForReturnOrThrow(func->Body())) {
        ThrowTypeError(errMsg, lineInfo);
    }
    // NOTE: aszilagyi. this function is not fully implement the TSC one, in the future if we will have a
    // noImplicitReturn compiler option for TypeScript we should update this function
}

ArgRange TSChecker::GetArgRange(const ArenaVector<Signature *> &signatures,
                                ArenaVector<Signature *> *potentialSignatures, uint32_t callArgsSize,
                                bool *haveSignatureWithRest)
{
    uint32_t minArg = UINT32_MAX;
    uint32_t maxArg = 0;

    for (auto *it : signatures) {
        if (it->RestVar() != nullptr) {
            *haveSignatureWithRest = true;
        }

        if (it->MinArgCount() < minArg) {
            minArg = it->MinArgCount();
        }

        if (it->Params().size() > maxArg) {
            maxArg = it->Params().size();
        }

        if (callArgsSize >= it->MinArgCount() && (callArgsSize <= it->Params().size() || it->RestVar() != nullptr)) {
            potentialSignatures->push_back(it);
        }
    }

    return {minArg, maxArg};
}

bool TSChecker::CallMatchesSignature(const ArenaVector<ir::Expression *> &args, Signature *signature, bool throwError)
{
    for (size_t index = 0; index < args.size(); index++) {
        checker::Type *sigArgType = nullptr;
        bool validateRestArg = false;

        if (index >= signature->Params().size()) {
            ASSERT(signature->RestVar());
            validateRestArg = true;
            sigArgType = signature->RestVar()->TsType();
        } else {
            sigArgType = signature->Params()[index]->TsType();
        }

        if (validateRestArg || !throwError) {
            checker::Type *callArgType = GetBaseTypeOfLiteralType(args[index]->Check(this));
            if (IsTypeAssignableTo(callArgType, sigArgType)) {
                continue;
            }

            if (throwError) {
                ThrowTypeError(
                    {"Argument of type '", callArgType, "' is not assignable to parameter of type '", sigArgType, "'."},
                    args[index]->Start());
            }
            return false;
        }

        ElaborateElementwise(sigArgType, args[index], args[index]->Start());
    }

    return true;
}

Type *TSChecker::ResolveCallOrNewExpression(const ArenaVector<Signature *> &signatures,
                                            ArenaVector<ir::Expression *> arguments,
                                            const lexer::SourcePosition &errPos)
{
    if (signatures.empty()) {
        ThrowTypeError("This expression is not callable.", errPos);
    }

    ArenaVector<checker::Signature *> potentialSignatures(Allocator()->Adapter());
    bool haveSignatureWithRest = false;

    auto argRange = GetArgRange(signatures, &potentialSignatures, arguments.size(), &haveSignatureWithRest);

    if (potentialSignatures.empty()) {
        if (haveSignatureWithRest) {
            ThrowTypeError({"Expected at least ", argRange.first, " arguments, but got ", arguments.size(), "."},
                           errPos);
        }

        if (signatures.size() == 1 && argRange.first == argRange.second) {
            lexer::SourcePosition loc =
                (argRange.first > arguments.size()) ? errPos : arguments[argRange.second]->Start();
            ThrowTypeError({"Expected ", argRange.first, " arguments, but got ", arguments.size(), "."}, loc);
        }

        ThrowTypeError({"Expected ", argRange.first, "-", argRange.second, " arguments, but got ", arguments.size()},
                       errPos);
    }

    checker::Type *returnType = nullptr;
    for (auto *it : potentialSignatures) {
        if (CallMatchesSignature(arguments, it, potentialSignatures.size() == 1)) {
            returnType = it->ReturnType();
            break;
        }
    }

    if (returnType == nullptr) {
        ThrowTypeError("No overload matches this call.", errPos);
    }

    return returnType;
}
}  // namespace ark::es2panda::checker
