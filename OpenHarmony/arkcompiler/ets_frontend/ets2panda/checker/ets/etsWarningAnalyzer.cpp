
/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "etsWarningAnalyzer.h"

#include "parser/program/program.h"
#include "util/options.h"
#include "ir/expressions/binaryExpression.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/scriptFunction.h"
#include "ir/statements/classDeclaration.h"
#include "ir/statements/expressionStatement.h"
#include "ir/statements/blockStatement.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/memberExpression.h"
#include "ir/ets/etsTypeReferencePart.h"
#include "ir/ets/etsTypeReference.h"
#include "ir/base/classDefinition.h"
#include "ir/statements/forOfStatement.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/statements/variableDeclaration.h"
#include "ir/expressions/updateExpression.h"

namespace ark::es2panda::checker {

void ETSWarningAnalyzer::AnalyzeClassDefForFinalModifier(const ir::ClassDefinition *classDef)
{
    ASSERT(classDef != nullptr);

    if (program_ == nullptr || classDef->IsFinal() || classDef->IsAbstract() || classDef->IsStatic() ||
        classDef->IsGlobal() || classDef->IsExported()) {
        return;
    }

    const auto statements = program_->Ast()->Statements();
    for (const auto *it : statements) {
        if (!it->IsClassDeclaration() ||
            classDef->Ident()->Name() == it->AsClassDeclaration()->Definition()->Ident()->Name()) {
            continue;
        }

        const auto *itAsClassDef = it->AsClassDeclaration()->Definition();

        if (!itAsClassDef->IsGlobal()) {
            const auto *superClass = itAsClassDef->Super();

            if (superClass == nullptr) {
                continue;
            }

            if (superClass->IsETSTypeReference() && superClass->AsETSTypeReference()->Part()->Name()->IsIdentifier() &&
                superClass->AsETSTypeReference()->Part()->Name()->AsIdentifier()->Name() == classDef->Ident()->Name()) {
                return;
            }
        }
    }

    ETSThrowWarning("Suggest 'final' modifier for class", classDef->Ident()->Start());
}

void ETSWarningAnalyzer::AnalyzeClassMethodForFinalModifier(const ir::MethodDefinition *methodDef,
                                                            const ir::ClassDefinition *classDef)
{
    ASSERT(methodDef != nullptr && classDef != nullptr);

    if (methodDef->IsAbstract() || methodDef->IsStatic() || classDef->IsFinal() || program_ == nullptr ||
        methodDef->IsFinal() || methodDef->IsConstructor() || classDef->IsGlobal()) {
        return;
    }

    bool suggestFinal = true;

    const auto statements = program_->Ast()->Statements();
    for (const auto *it : statements) {
        if (!it->IsClassDeclaration() || it->AsClassDeclaration()->Definition()->IsGlobal() ||
            classDef->Ident()->Name() == it->AsClassDeclaration()->Definition()->Ident()->Name()) {
            continue;
        }

        const auto *statementDef = it->AsClassDeclaration()->Definition();
        for (const auto *bodyPart : statementDef->Body()) {
            if (!bodyPart->IsMethodDefinition()) {
                continue;
            }
            static auto classAsETSObject = classDef->TsType()->AsETSObjectType();
            static auto potentialDescendant = statementDef->TsType()->AsETSObjectType();
            if (!potentialDescendant->IsDescendantOf(classAsETSObject)) {
                continue;
            }
            const util::StringView bodyMethodName =
                ETSChecker::GetSignatureFromMethodDefinition(bodyPart->AsMethodDefinition())->Function()->Id()->Name();
            if (bodyPart->IsOverride() && bodyMethodName != compiler::Signatures::CTOR &&
                bodyMethodName == methodDef->Function()->Id()->Name()) {
                suggestFinal = false;
                break;
            }
        }
    }

    if (suggestFinal) {
        ETSThrowWarning("Suggest 'final' modifier for method", methodDef->Function()->Start());
    }
}

void ETSWarningAnalyzer::ETSWarningSuggestFinal(const ir::AstNode *node)
{
    if (node->IsClassDeclaration() && !program_->NodeContainsETSNolint(node, ETSWarnings::SUGGEST_FINAL)) {
        if (node->AsClassDeclaration()->Definition()->IsClassDefinition()) {
            AnalyzeClassDefForFinalModifier(node->AsClassDeclaration()->Definition());
        }

        const auto classBody = node->AsClassDeclaration()->Definition()->Body();
        for (const auto *it : classBody) {
            if (it->IsMethodDefinition()) {
                AnalyzeClassMethodForFinalModifier(it->AsMethodDefinition(), node->AsClassDeclaration()->Definition());
            }
        }
    }
    node->Iterate([&](auto *childNode) { ETSWarningSuggestFinal(childNode); });
}

void ETSWarningAnalyzer::CheckTopLevelExpressions(const ir::Expression *expression)
{
    if (expression->IsCallExpression()) {
        const auto exprCallee = expression->AsCallExpression()->Callee();
        lexer::SourcePosition pos = exprCallee->Start();
        if (exprCallee->IsMemberExpression()) {
            pos = exprCallee->AsMemberExpression()->Object()->Start();
            ETSThrowWarning("Prohibit top-level statements", pos);
        }
    } else if (expression->IsAssignmentExpression()) {
        const auto assignmentExpr = expression->AsAssignmentExpression();
        ETSThrowWarning("Prohibit top-level statements", assignmentExpr->Left()->Start());
    }
}

void ETSWarningAnalyzer::CheckProhibitedTopLevelStatements(const ir::Statement *statement)
{
    switch (statement->Type()) {
        case ir::AstNodeType::ARROW_FUNCTION_EXPRESSION:
        case ir::AstNodeType::FUNCTION_DECLARATION:
        case ir::AstNodeType::SCRIPT_FUNCTION:
        case ir::AstNodeType::ETS_FUNCTION_TYPE:
        case ir::AstNodeType::IMPORT_NAMESPACE_SPECIFIER:
        case ir::AstNodeType::CLASS_DECLARATION:
        case ir::AstNodeType::CLASS_EXPRESSION:
        case ir::AstNodeType::VARIABLE_DECLARATION:
        case ir::AstNodeType::CLASS_DEFINITION:
        case ir::AstNodeType::CLASS_PROPERTY:
            break;
        default:
            ETSThrowWarning("Prohibit top-level statements", statement->Start());
            break;
    }
}

void ETSWarningAnalyzer::ETSWarningsProhibitTopLevelStatements(const ir::AstNode *node)
{
    if (!node->IsClassDeclaration() ||
        program_->NodeContainsETSNolint(node, ETSWarnings::PROHIBIT_TOP_LEVEL_STATEMENTS)) {
        node->Iterate([&](auto *childNode) { ETSWarningsProhibitTopLevelStatements(childNode); });
        return;
    }

    const auto *classDef = node->AsClassDeclaration()->Definition();
    if (!classDef->IsGlobal()) {
        node->Iterate([&](auto *childNode) { ETSWarningsProhibitTopLevelStatements(childNode); });
        return;
    }

    for (const auto *itBody : classDef->Body()) {
        if (!itBody->IsMethodDefinition() ||
            itBody->AsMethodDefinition()->Id()->Name() != compiler::Signatures::INIT_METHOD) {
            continue;
        }

        for (const auto *statement :
             itBody->AsMethodDefinition()->Function()->Body()->AsBlockStatement()->Statements()) {
            if (program_->NodeContainsETSNolint(statement, ETSWarnings::PROHIBIT_TOP_LEVEL_STATEMENTS)) {
                continue;
            }

            if (!statement->IsExpressionStatement()) {
                CheckProhibitedTopLevelStatements(statement);
                continue;
            }

            // Rewrite this part after fixing AST issue about tiop-level
            CheckTopLevelExpressions(statement->AsExpressionStatement()->GetExpression());
        }
    }
}

void ETSWarningAnalyzer::ETSWarningBoostEqualityStatement(const ir::AstNode *node)
{
    ASSERT(node != nullptr);

    if (node->IsBinaryExpression() && !program_->NodeContainsETSNolint(node, ETSWarnings::BOOST_EQUALITY_STATEMENT)) {
        const auto binExpr = node->AsBinaryExpression();
        if (binExpr->OperatorType() == lexer::TokenType::PUNCTUATOR_EQUAL ||
            binExpr->OperatorType() == lexer::TokenType::PUNCTUATOR_NOT_EQUAL) {
            if (binExpr->Right()->IsNullLiteral() && !binExpr->Left()->IsNullLiteral()) {
                ETSThrowWarning("Boost Equality Statement. Change sides of binary expression", node->Start());
            }
        }
    }
    node->Iterate([&](auto *childNode) { ETSWarningBoostEqualityStatement(childNode); });
}

void ETSWarningAnalyzer::ETSWarningRemoveAsync(const ir::AstNode *node)
{
    if (node->IsMethodDefinition() && !program_->NodeContainsETSNolint(node, ETSWarnings::REMOVE_ASYNC_FUNCTIONS)) {
        const auto methodDefinition = node->AsMethodDefinition();
        if (methodDefinition->IsAsync()) {
            ETSThrowWarning("Replace asynchronous function with coroutine", methodDefinition->Start());
        }
    }
    node->Iterate([&](auto *childNode) { ETSWarningRemoveAsync(childNode); });
}

void ETSWarningAnalyzer::ETSWarningRemoveLambda(const ir::AstNode *node)
{
    ASSERT(node != nullptr);

    if (node->IsArrowFunctionExpression() && !program_->NodeContainsETSNolint(node, ETSWarnings::REMOVE_LAMBDA)) {
        ETSThrowWarning("Replace the lambda function with a regular function", node->Start());
    }
    node->Iterate([&](auto *childNode) { ETSWarningRemoveLambda(childNode); });
}

void ETSWarningAnalyzer::CheckTypeOfBoxing(const ir::AstNode *node)
{
    ASSERT(node != nullptr);
    const auto flags = node->GetBoxingUnboxingFlags();
    if ((flags & ir::BoxingUnboxingFlags::BOXING_FLAG) != 0) {
        switch (static_cast<ir::BoxingUnboxingFlags>(flags & ir::BoxingUnboxingFlags::BOXING_FLAG)) {
            case ir::BoxingUnboxingFlags::BOX_TO_INT:
                ETSThrowWarning("Implicit Boxing to Int" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::BOX_TO_BOOLEAN:
                ETSThrowWarning("Implicit Boxing to Boolean" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::BOX_TO_BYTE:
                ETSThrowWarning("Implicit Boxing to Byte" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::BOX_TO_CHAR:
                ETSThrowWarning("Implicit Boxing to Char" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::BOX_TO_DOUBLE:
                ETSThrowWarning("Implicit Boxing to Double" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::BOX_TO_FLOAT:
                ETSThrowWarning("Implicit Boxing to Float" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::BOX_TO_LONG:
                ETSThrowWarning("Implicit Boxing to Long" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::BOX_TO_SHORT:
                ETSThrowWarning("Implicit Boxing to Short" + GetBoxingUnboxingType(node), node->Start());
                break;
            default:
                break;
        }
    }
}

void ETSWarningAnalyzer::CheckTypeOfUnboxing(const ir::AstNode *node)
{
    ASSERT(node != nullptr);
    const auto flags = node->GetBoxingUnboxingFlags();
    if ((flags & ir::BoxingUnboxingFlags::UNBOXING_FLAG) != 0) {
        switch (static_cast<ir::BoxingUnboxingFlags>(flags & ir::BoxingUnboxingFlags::UNBOXING_FLAG)) {
            case ir::BoxingUnboxingFlags::UNBOX_TO_INT:
                ETSThrowWarning("Implicit Unboxing to int" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::UNBOX_TO_BOOLEAN:
                ETSThrowWarning("Implicit Unboxing to boolean" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::UNBOX_TO_BYTE:
                ETSThrowWarning("Implicit Unboxing to byte" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::UNBOX_TO_CHAR:
                ETSThrowWarning("Implicit Unboxing to char" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::UNBOX_TO_DOUBLE:
                ETSThrowWarning("Implicit Unboxing to double" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::UNBOX_TO_FLOAT:
                ETSThrowWarning("Implicit Unboxing to float" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::UNBOX_TO_LONG:
                ETSThrowWarning("Implicit Unboxing to long" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::UNBOX_TO_SHORT:
                ETSThrowWarning("Implicit Unboxing to short" + GetBoxingUnboxingType(node), node->Start());
                break;
            case ir::BoxingUnboxingFlags::UNBOX_TO_ENUM:
                ETSThrowWarning("Implicit Unboxing to enum" + GetBoxingUnboxingType(node), node->Start());
                break;
            default:
                break;
        }
    }
}

void ETSWarningAnalyzer::CheckTypeOfBoxingUnboxing(const ir::AstNode *node)
{
    ASSERT(node != nullptr);

    CheckTypeOfBoxing(node);
    CheckTypeOfUnboxing(node);
}

std::string ETSWarningAnalyzer::GetBoxingUnboxingType(const ir::AstNode *node)
{
    ASSERT(node->Parent() != nullptr);
    switch (node->Parent()->Type()) {
        case ir::AstNodeType::VARIABLE_DECLARATOR: {
            return " in Variable Declaration";
        }
        case ir::AstNodeType::CALL_EXPRESSION: {
            return " in Call Method/Function";
        }
        case ir::AstNodeType::SWITCH_STATEMENT: {
            return " in Switch-case Statement";
        }
        case ir::AstNodeType::ASSIGNMENT_EXPRESSION: {
            return " in Assignment Expression";
        }
        case ir::AstNodeType::BINARY_EXPRESSION: {
            return " in Binary Expression";
        }
        case ir::AstNodeType::UNARY_EXPRESSION: {
            return " in Unary Expression";
        }
        case ir::AstNodeType::UPDATE_EXPRESSION: {
            return " in Update Expression";
        }
        case ir::AstNodeType::MEMBER_EXPRESSION: {
            return " in Member Expression";
        }
        default:
            return "";
    }
}

void ETSWarningAnalyzer::ETSWarningImplicitBoxingUnboxing(const ir::AstNode *node)
{
    ASSERT(node != nullptr);

    switch (node->Type()) {
        case ir::AstNodeType::VARIABLE_DECLARATOR:
        case ir::AstNodeType::SWITCH_STATEMENT:
        case ir::AstNodeType::CALL_EXPRESSION:
        case ir::AstNodeType::BINARY_EXPRESSION:
        case ir::AstNodeType::ASSIGNMENT_EXPRESSION:
        case ir::AstNodeType::UNARY_EXPRESSION:
        case ir::AstNodeType::UPDATE_EXPRESSION:
        case ir::AstNodeType::MEMBER_EXPRESSION: {
            if (!program_->NodeContainsETSNolint(node, ETSWarnings::IMPLICIT_BOXING_UNBOXING)) {
                node->Iterate([this](auto *childNode) { CheckTypeOfBoxingUnboxing(childNode); });
            }
            break;
        }
        default: {
            break;
        }
    }

    node->Iterate([&](auto *childNode) { ETSWarningImplicitBoxingUnboxing(childNode); });
}

void ETSWarningAnalyzer::ETSThrowWarning(const std::string &message, const lexer::SourcePosition &pos)
{
    lexer::LineIndex index(program_->SourceCode());
    lexer::SourceLocation location = index.GetLocation(pos);

    if (etsWerror_) {
        throw Error(ErrorType::ETS_WARNING, ark::es2panda::util::BaseName(program_->SourceFilePath().Utf8()), message,
                    location.line, location.col);
    }

    std::cout << "ETS Warning: " << message << "."
              << " [" << ark::es2panda::util::BaseName(program_->SourceFilePath().Utf8()) << ":" << location.line << ":"
              << location.col << "]" << std::endl;
}

}  // namespace ark::es2panda::checker
