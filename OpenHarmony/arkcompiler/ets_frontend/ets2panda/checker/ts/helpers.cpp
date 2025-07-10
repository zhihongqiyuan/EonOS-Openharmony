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
#include "ir/base/scriptFunction.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/binaryExpression.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/ts/tsQualifiedName.h"
#include "ir/base/tsPropertySignature.h"
#include "ir/ts/tsTypeAliasDeclaration.h"
#include "ir/ts/tsTypeReference.h"
#include "ir/ts/tsTypeParameterDeclaration.h"
#include "ir/ts/tsTypeParameter.h"
#include "varbinder/variable.h"
#include "varbinder/scope.h"
#include "util/helpers.h"

#include "checker/ts/typeElaborationContext.h"
#include "checker/TSchecker.h"

namespace ark::es2panda::checker {
void TSChecker::CheckTruthinessOfType(Type *type, lexer::SourcePosition lineInfo)
{
    if (type->IsVoidType()) {
        ThrowTypeError("An expression of type void cannot be tested for truthiness", lineInfo);
    }
}

Type *TSChecker::CheckNonNullType(Type *type, lexer::SourcePosition lineInfo)
{
    if (type->IsNullType()) {
        ThrowTypeError("Object is possibly 'null'.", lineInfo);
    }

    if (type->IsUndefinedType()) {
        ThrowTypeError("Object is possibly 'undefined'.", lineInfo);
    }

    return type;
}

Type *TSChecker::GetBaseTypeOfLiteralType(Type *type)
{
    if (HasStatus(CheckerStatus::KEEP_LITERAL_TYPE)) {
        return type;
    }

    if (type->IsStringLiteralType()) {
        return GlobalStringType();
    }

    if (type->IsNumberLiteralType()) {
        return GlobalNumberType();
    }

    if (type->IsBooleanLiteralType()) {
        return GlobalBooleanType();
    }

    if (type->IsBigintLiteralType()) {
        return GlobalBigintType();
    }

    if (type->IsUnionType()) {
        auto &constituentTypes = type->AsUnionType()->ConstituentTypes();
        ArenaVector<Type *> newConstituentTypes(Allocator()->Adapter());

        newConstituentTypes.reserve(constituentTypes.size());
        for (auto *it : constituentTypes) {
            newConstituentTypes.push_back(GetBaseTypeOfLiteralType(it));
        }

        return CreateUnionType(std::move(newConstituentTypes));
    }

    return type;
}

void TSChecker::CheckReferenceExpression(ir::Expression *expr, const char *invalidReferenceMsg,
                                         const char *invalidOptionalChainMsg)
{
    if (expr->IsIdentifier()) {
        const util::StringView &name = expr->AsIdentifier()->Name();
        auto result = Scope()->Find(name);
        ASSERT(result.variable);

        if (result.variable->HasFlag(varbinder::VariableFlags::ENUM_LITERAL)) {
            ThrowTypeError({"Cannot assign to '", name, "' because it is not a variable."}, expr->Start());
        }
    } else if (!expr->IsMemberExpression()) {
        if (expr->IsChainExpression()) {
            ThrowTypeError(invalidOptionalChainMsg, expr->Start());
        }

        ThrowTypeError(invalidReferenceMsg, expr->Start());
    }
}

void TSChecker::CheckTestingKnownTruthyCallableOrAwaitableType([[maybe_unused]] ir::Expression *condExpr,
                                                               [[maybe_unused]] Type *type,
                                                               [[maybe_unused]] ir::AstNode *body)
{
    // NOTE: aszilagyi. rework this
}

Type *TSChecker::ExtractDefinitelyFalsyTypes(Type *type)
{
    if (type->IsStringType()) {
        return GlobalEmptyStringType();
    }

    if (type->IsNumberType()) {
        return GlobalZeroType();
    }

    if (type->IsBigintType()) {
        return GlobalZeroBigintType();
    }

    if (type == GlobalFalseType() || type->DefinitelyETSNullish() || type->HasTypeFlag(TypeFlag::ANY_OR_UNKNOWN) ||
        type->HasTypeFlag(TypeFlag::VOID) ||
        (type->IsStringLiteralType() && IsTypeIdenticalTo(type, GlobalEmptyStringType())) ||
        (type->IsNumberLiteralType() && IsTypeIdenticalTo(type, GlobalZeroType())) ||
        (type->IsBigintLiteralType() && IsTypeIdenticalTo(type, GlobalZeroBigintType()))) {
        return type;
    }

    if (type->IsUnionType()) {
        auto &constituentTypes = type->AsUnionType()->ConstituentTypes();
        ArenaVector<Type *> newConstituentTypes(Allocator()->Adapter());

        newConstituentTypes.reserve(constituentTypes.size());
        for (auto &it : constituentTypes) {
            newConstituentTypes.push_back(ExtractDefinitelyFalsyTypes(it));
        }

        return CreateUnionType(std::move(newConstituentTypes));
    }

    return GlobalNeverType();
}

Type *TSChecker::RemoveDefinitelyFalsyTypes(Type *type)
{
    if ((static_cast<uint64_t>(GetFalsyFlags(type)) & static_cast<uint64_t>(TypeFlag::DEFINITELY_FALSY)) != 0U) {
        if (!type->IsUnionType()) {
            return GlobalNeverType();
        }

        auto &constituentTypes = type->AsUnionType()->ConstituentTypes();
        ArenaVector<Type *> newConstituentTypes(Allocator()->Adapter());

        for (auto &it : constituentTypes) {
            if ((static_cast<uint64_t>(GetFalsyFlags(it)) & static_cast<uint64_t>(TypeFlag::DEFINITELY_FALSY)) == 0U) {
                newConstituentTypes.push_back(it);
            }
        }

        if (newConstituentTypes.empty()) {
            return GlobalNeverType();
        }

        if (newConstituentTypes.size() == 1) {
            return newConstituentTypes[0];
        }

        return CreateUnionType(std::move(newConstituentTypes));
    }

    return type;
}

TypeFlag TSChecker::GetFalsyFlags(Type *type)
{
    if (type->IsStringLiteralType()) {
        return type->AsStringLiteralType()->Value().Empty() ? TypeFlag::STRING_LITERAL : TypeFlag::NONE;
    }

    if (type->IsNumberLiteralType()) {
        return type->AsNumberLiteralType()->Value() == 0 ? TypeFlag::NUMBER_LITERAL : TypeFlag::NONE;
    }

    if (type->IsBigintLiteralType()) {
        return type->AsBigintLiteralType()->Value() == "0n" ? TypeFlag::BIGINT_LITERAL : TypeFlag::NONE;
    }

    if (type->IsBooleanLiteralType()) {
        return type->AsBooleanLiteralType()->Value() ? TypeFlag::NONE : TypeFlag::BOOLEAN_LITERAL;
    }

    if (type->IsUnionType()) {
        auto &constituentTypes = type->AsUnionType()->ConstituentTypes();
        TypeFlag returnFlag = TypeFlag::NONE;

        for (auto &it : constituentTypes) {
            returnFlag |= GetFalsyFlags(it);
        }

        return returnFlag;
    }

    return static_cast<TypeFlag>(type->TypeFlags() & TypeFlag::POSSIBLY_FALSY);
}

bool TSChecker::IsVariableUsedInConditionBody(ir::AstNode *parent, varbinder::Variable *searchVar)
{
    bool found = false;

    parent->Iterate([this, searchVar, &found](ir::AstNode *childNode) -> void {
        varbinder::Variable *resultVar = nullptr;
        if (childNode->IsIdentifier()) {
            auto result = Scope()->Find(childNode->AsIdentifier()->Name());
            ASSERT(result.variable);
            resultVar = result.variable;
        }

        if (searchVar == resultVar) {
            found = true;
            return;
        }

        if (!childNode->IsMemberExpression()) {
            IsVariableUsedInConditionBody(childNode, searchVar);
        }
    });

    return found;
}

bool TSChecker::FindVariableInBinaryExpressionChain(ir::AstNode *parent, varbinder::Variable *searchVar)
{
    bool found = false;

    parent->Iterate([this, searchVar, &found](ir::AstNode *childNode) -> void {
        if (childNode->IsIdentifier()) {
            auto result = Scope()->Find(childNode->AsIdentifier()->Name());
            ASSERT(result.variable);
            if (result.variable == searchVar) {
                found = true;
                return;
            }
        }

        FindVariableInBinaryExpressionChain(childNode, searchVar);
    });

    return found;
}

bool TSChecker::IsVariableUsedInBinaryExpressionChain(ir::AstNode *parent, varbinder::Variable *searchVar)
{
    while (parent->IsBinaryExpression() &&
           parent->AsBinaryExpression()->OperatorType() == lexer::TokenType::PUNCTUATOR_LOGICAL_AND) {
        if (FindVariableInBinaryExpressionChain(parent, searchVar)) {
            return true;
        }

        parent = parent->Parent();
    }

    return false;
}

void TSChecker::ThrowTypeError(std::initializer_list<TypeErrorMessageElement> list, const lexer::SourcePosition &pos)
{
    ThrowTypeError(FormatMsg(list), pos);
}

void TSChecker::ThrowTypeError(std::string_view message, const lexer::SourcePosition &pos)
{
    lexer::LineIndex index(Program()->SourceCode());
    lexer::SourceLocation loc = index.GetLocation(pos);

    throw Error {ErrorType::TYPE, Program()->SourceFilePath().Utf8(), message, loc.line, loc.col};
}

void TSChecker::ThrowBinaryLikeError(lexer::TokenType op, Type *leftType, Type *rightType,
                                     lexer::SourcePosition lineInfo)
{
    if (!HasStatus(CheckerStatus::IN_CONST_CONTEXT)) {
        ThrowTypeError({"operator ", op, " cannot be applied to types ", leftType, " and ", AsSrc(rightType)},
                       lineInfo);
    }

    ThrowTypeError({"operator ", op, " cannot be applied to types ", leftType, " and ", rightType}, lineInfo);
}

void TSChecker::ThrowAssignmentError(Type *source, Type *target, lexer::SourcePosition lineInfo, bool isAsSrcLeftType)
{
    if (isAsSrcLeftType || !target->HasTypeFlag(TypeFlag::LITERAL)) {
        ThrowTypeError({"Type '", AsSrc(source), "' is not assignable to type '", target, "'."}, lineInfo);
    }

    ThrowTypeError({"Type '", source, "' is not assignable to type '", target, "'."}, lineInfo);
}

Type *TSChecker::GetUnaryResultType(Type *operandType)
{
    if (checker::TSChecker::MaybeTypeOfKind(operandType, checker::TypeFlag::BIGINT_LIKE)) {
        if (operandType->HasTypeFlag(checker::TypeFlag::UNION_OR_INTERSECTION) &&
            checker::TSChecker::MaybeTypeOfKind(operandType, checker::TypeFlag::NUMBER_LIKE)) {
            return GlobalNumberOrBigintType();
        }

        return GlobalBigintType();
    }

    return GlobalNumberType();
}

void TSChecker::ElaborateElementwise(Type *targetType, ir::Expression *sourceNode, const lexer::SourcePosition &pos)
{
    auto savedContext = SavedCheckerContext(this, CheckerStatus::FORCE_TUPLE | CheckerStatus::KEEP_LITERAL_TYPE);

    Type *sourceType = CheckTypeCached(sourceNode);

    if (IsTypeAssignableTo(sourceType, targetType)) {
        return;
    }

    if (targetType->IsArrayType() && sourceNode->IsArrayExpression()) {
        ArrayElaborationContext(this, targetType, sourceType, sourceNode, pos).Start();
    } else if (targetType->IsObjectType() || targetType->IsUnionType()) {
        if (sourceNode->IsObjectExpression()) {
            ObjectElaborationContext(this, targetType, sourceType, sourceNode, pos).Start();
        } else if (sourceNode->IsArrayExpression()) {
            ArrayElaborationContext(this, targetType, sourceType, sourceNode, pos).Start();
        }
    }

    ThrowAssignmentError(sourceType, targetType, pos);
}

void TSChecker::InferSimpleVariableDeclaratorType(ir::VariableDeclarator *declarator)
{
    ASSERT(declarator->Id()->IsIdentifier());

    varbinder::Variable *var = declarator->Id()->AsIdentifier()->Variable();
    ASSERT(var);

    if (declarator->Id()->AsIdentifier()->TypeAnnotation() != nullptr) {
        var->SetTsType(declarator->Id()->AsIdentifier()->TypeAnnotation()->GetType(this));
        return;
    }

    if (declarator->Init() != nullptr) {
        var->SetTsType(CheckTypeCached(declarator->Init()));
        return;
    }

    ThrowTypeError({"Variable ", declarator->Id()->AsIdentifier()->Name(), " implicitly has an any type."},
                   declarator->Id()->Start());
}

void TSChecker::GetTypeVar(varbinder::Decl *decl)
{
    ir::AstNode *declarator =
        util::Helpers::FindAncestorGivenByType(decl->Node(), ir::AstNodeType::VARIABLE_DECLARATOR);
    ASSERT(declarator);

    if (declarator->AsVariableDeclarator()->Id()->IsIdentifier()) {
        InferSimpleVariableDeclaratorType(declarator->AsVariableDeclarator());
        return;
    }

    declarator->Check(this);
}

void TSChecker::GetTypeParam(varbinder::Variable *var, varbinder::Decl *decl)
{
    ir::AstNode *declaration = FindAncestorUntilGivenType(decl->Node(), ir::AstNodeType::SCRIPT_FUNCTION);

    if (declaration->IsIdentifier()) {
        auto *ident = declaration->AsIdentifier();
        if (ident->TypeAnnotation() != nullptr) {
            ASSERT(ident->Variable() == var);
            var->SetTsType(ident->TypeAnnotation()->GetType(this));
            return;
        }

        ThrowTypeError({"Parameter ", ident->Name(), " implicitly has an 'any' type."}, ident->Start());
    }

    if (declaration->IsAssignmentPattern() && declaration->AsAssignmentPattern()->Left()->IsIdentifier()) {
        ir::Identifier *ident = declaration->AsAssignmentPattern()->Left()->AsIdentifier();

        if (ident->TypeAnnotation() != nullptr) {
            ASSERT(ident->Variable() == var);
            var->SetTsType(ident->TypeAnnotation()->GetType(this));
            return;
        }

        var->SetTsType(declaration->AsAssignmentPattern()->Right()->Check(this));
    }

    CheckFunctionParameter(declaration->AsExpression(), nullptr);
}

void TSChecker::GetTypeEnum(varbinder::Variable *var, varbinder::Decl *decl)
{
    ASSERT(var->IsEnumVariable());
    varbinder::EnumVariable *enumVar = var->AsEnumVariable();

    if (std::holds_alternative<bool>(enumVar->Value())) {
        ThrowTypeError(
            "A member initializer in a enum declaration cannot reference members declared after it, "
            "including "
            "members defined in other enums.",
            decl->Node()->Start());
    }

    var->SetTsType(std::holds_alternative<double>(enumVar->Value()) ? GlobalNumberType() : GlobalStringType());
}

Type *TSChecker::GetDeclTsType(varbinder::Variable *var, varbinder::Decl *decl)
{
    switch (decl->Type()) {
        case varbinder::DeclType::CONST:
        case varbinder::DeclType::LET: {
            if (!decl->Node()->Parent()->IsTSTypeQuery()) {
                ThrowTypeError({"Block-scoped variable '", var->Name(), "' used before its declaration"},
                               decl->Node()->Start());
                break;
            }

            [[fallthrough]];
        }
        case varbinder::DeclType::VAR: {
            GetTypeVar(decl);
            break;
        }
        case varbinder::DeclType::PROPERTY: {
            var->SetTsType(decl->Node()->AsTSPropertySignature()->TypeAnnotation()->GetType(this));
            break;
        }
        case varbinder::DeclType::METHOD: {
            auto *signatureInfo = Allocator()->New<checker::SignatureInfo>(Allocator());
            auto *callSignature = Allocator()->New<checker::Signature>(signatureInfo, GlobalAnyType());
            var->SetTsType(CreateFunctionTypeWithSignature(callSignature));
            break;
        }
        case varbinder::DeclType::FUNC: {
            checker::ScopeContext scopeCtx(this, decl->Node()->AsScriptFunction()->Scope());
            InferFunctionDeclarationType(decl->AsFunctionDecl(), var);
            break;
        }
        case varbinder::DeclType::PARAM: {
            GetTypeParam(var, decl);
            break;
        }
        case varbinder::DeclType::ENUM: {
            GetTypeEnum(var, decl);
            break;
        }
        case varbinder::DeclType::ENUM_LITERAL: {
            UNREACHABLE();  // NOTE: aszilagyi.
        }
        default: {
            break;
        }
    }

    return var->TsType();
}

Type *TSChecker::GetTypeOfVariable(varbinder::Variable *var)
{
    if (var->TsType() != nullptr) {
        return var->TsType();
    }

    varbinder::Decl *decl = var->Declaration();

    TypeStackElement tse(
        this, decl->Node(),
        std::initializer_list<TypeErrorMessageElement> {
            "'", var->Name(), "' is referenced directly or indirectly in its ", "own initializer ot type annotation."},
        decl->Node()->Start());
    if (tse.HasTypeError()) {
        return GlobalErrorType();
    }

    return GetDeclTsType(var, decl);
}

Type *TSChecker::GetTypeFromClassOrInterfaceReference([[maybe_unused]] ir::TSTypeReference *node,
                                                      varbinder::Variable *var)
{
    Type *resolvedType = var->TsType();

    if (resolvedType == nullptr) {
        ObjectDescriptor *desc = Allocator()->New<ObjectDescriptor>(Allocator());
        resolvedType = Allocator()->New<InterfaceType>(Allocator(), var->Name(), desc);
        resolvedType->SetVariable(var);
        var->SetTsType(resolvedType);
    }

    return resolvedType;
}

Type *TSChecker::GetTypeFromTypeAliasReference(ir::TSTypeReference *node, varbinder::Variable *var)
{
    Type *resolvedType = var->TsType();

    if (resolvedType != nullptr) {
        return resolvedType;
    }

    TypeStackElement tse(this, var, {"Type alias ", var->Name(), " circularly refences itself"}, node->Start());
    if (tse.HasTypeError()) {
        return GlobalErrorType();
    }

    ASSERT(var->Declaration()->Node() && var->Declaration()->Node()->IsTSTypeAliasDeclaration());
    ir::TSTypeAliasDeclaration *declaration = var->Declaration()->Node()->AsTSTypeAliasDeclaration();
    resolvedType = declaration->TypeAnnotation()->GetType(this);
    var->SetTsType(resolvedType);

    return resolvedType;
}

Type *TSChecker::GetTypeReferenceType(ir::TSTypeReference *node, varbinder::Variable *var)
{
    ASSERT(var->Declaration());
    varbinder::Decl *decl = var->Declaration();

    if (decl->IsInterfaceDecl()) {
        return GetTypeFromClassOrInterfaceReference(node, var);
    }

    if (decl->IsTypeAliasDecl()) {
        return GetTypeFromTypeAliasReference(node, var);
    }

    ThrowTypeError("This reference refers to a value, but is being used as a type here. Did you mean to use 'typeof'?",
                   node->Start());
    return nullptr;
}
}  // namespace ark::es2panda::checker
