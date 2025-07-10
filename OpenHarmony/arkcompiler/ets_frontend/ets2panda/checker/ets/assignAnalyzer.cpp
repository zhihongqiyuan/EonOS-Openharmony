/*
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

#include "assignAnalyzer.h"
#include <cstddef>

#include "ir/base/classDefinition.h"
#include "ir/base/classProperty.h"
#include "ir/base/classStaticBlock.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/scriptFunction.h"
#include "ir/statements/classDeclaration.h"
#include "ir/statements/variableDeclaration.h"
#include "ir/statements/doWhileStatement.h"
#include "ir/statements/expressionStatement.h"
#include "ir/statements/whileStatement.h"
#include "ir/statements/forUpdateStatement.h"
#include "ir/statements/labelledStatement.h"
#include "ir/statements/forOfStatement.h"
#include "ir/statements/blockStatement.h"
#include "ir/statements/ifStatement.h"
#include "ir/statements/switchStatement.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/statements/throwStatement.h"
#include "ir/statements/switchCaseStatement.h"
#include "ir/statements/breakStatement.h"
#include "ir/statements/continueStatement.h"
#include "ir/statements/returnStatement.h"
#include "ir/statements/tryStatement.h"
#include "ir/statements/assertStatement.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/arrowFunctionExpression.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/binaryExpression.h"
#include "ir/expressions/conditionalExpression.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/objectExpression.h"
#include "ir/expressions/unaryExpression.h"
#include "ir/expressions/updateExpression.h"
#include "ir/expressions/typeofExpression.h"
#include "ir/ets/etsNewClassInstanceExpression.h"
#include "ir/ets/etsStructDeclaration.h"
#include "ir/ts/tsInterfaceDeclaration.h"
#include "varbinder/ETSBinder.h"
#include "varbinder/variable.h"
#include "varbinder/scope.h"
#include "varbinder/declaration.h"
#include "checker/ETSchecker.h"
#include "ir/base/catchClause.h"
#include "parser/program/program.h"
#include "checker/types/ts/objectType.h"

namespace ark::es2panda::checker {

static constexpr NodeId INVALID_ID = -1;
static constexpr bool CHECK_ALL_PROPERTIES = true;
// NOTE(pantos) generic field initialization issue, skip them for now
static constexpr bool CHECK_GENERIC_NON_READONLY_PROPERTIES = false;
static constexpr bool WARN_NO_INIT_ONCE_PER_VARIABLE = false;
static constexpr int LOOP_PHASES = 2;

template <typename... Ts>
struct ScopeGuard {
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    std::tuple<Ts...> values;
    std::tuple<Ts &...> refs;
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    explicit ScopeGuard(Ts &...ts) : values(ts...), refs(ts...) {}
    ~ScopeGuard()
    {
        refs = values;
    }

    DEFAULT_COPY_SEMANTIC(ScopeGuard);
    DEFAULT_MOVE_SEMANTIC(ScopeGuard);
};

static std::string Capitalize(const util::StringView &str)
{
    if (str.Empty()) {
        return "";
    }
    std::string ret(str.Utf8());
    ret[0] = std::toupper(ret[0]);
    return ret;
}

void Set::Reset()
{
    reset_ = true;
}

bool Set::IsReset()
{
    return reset_;
}

void Set::Incl(const int id)
{
    nodes_.insert(id);
}

void Set::InclRange(const int start, const int limit)
{
    for (int x = start; x < limit; x++) {
        nodes_.insert(x);
    }
}

void Set::Excl(const int id)
{
    nodes_.erase(id);
}

void Set::ExcludeFrom(const int start)
{
    auto it = nodes_.lower_bound(start);
    nodes_.erase(nodes_.begin(), it);
}

bool Set::IsMember(const int id) const
{
    return nodes_.find(id) != nodes_.end();
}

Set &Set::AndSet(const Set &xs)
{
    std::set<int> res;
    std::set_intersection(nodes_.begin(), nodes_.end(), xs.nodes_.begin(), xs.nodes_.end(),
                          std::inserter(res, res.begin()));
    nodes_ = res;
    return *this;
}

Set &Set::OrSet(const Set &xs)
{
    std::set<int> res;
    std::set_union(nodes_.begin(), nodes_.end(), xs.nodes_.begin(), xs.nodes_.end(), std::inserter(res, res.begin()));
    nodes_ = res;
    return *this;
}

Set &Set::DiffSet(const Set &xs)
{
    std::set<int> res;
    std::set_difference(nodes_.begin(), nodes_.end(), xs.nodes_.begin(), xs.nodes_.end(),
                        std::inserter(res, res.begin()));
    nodes_ = res;
    return *this;
}

int Set::Next(const int id)
{
    auto it = nodes_.upper_bound(id);
    if (it != nodes_.end()) {
        return *it;
    }
    return -1;
}

AssignAnalyzer::AssignAnalyzer(ETSChecker *checker)
    : checker_(checker),
      varDecls_(checker->Allocator()->Adapter()),
      nodeIdMap_(checker->Allocator()->Adapter()),
      foundErrors_(checker->Allocator()->Adapter())
{
}

void AssignAnalyzer::Analyze(const ir::AstNode *node)
{
    const auto program = checker_->VarBinder()->Program();
    globalClass_ = program->GlobalClass();

    AnalyzeClassDef(globalClass_);
    globalClassIsVisited_ = true;

    firstNonGlobalAdr_ = nextAdr_;

    AnalyzeNodes(node);

    if (numErrors_ > 0) {
        checker_->LogTypeError("There were errors during assign analysis (" + std::to_string(numErrors_) + ")",
                               node->Start());
    }
}

void AssignAnalyzer::Warning(const std::string_view message, const lexer::SourcePosition &pos)
{
    ++numErrors_;
    checker_->Warning(message, pos);
}

void AssignAnalyzer::Warning(std::initializer_list<TypeErrorMessageElement> list, const lexer::SourcePosition &pos)
{
    ++numErrors_;
    checker_->ReportWarning(list, pos);
}

void AssignAnalyzer::AnalyzeNodes(const ir::AstNode *node)
{
    node->Iterate([this](auto *childNode) { AnalyzeNode(childNode); });
}

void AssignAnalyzer::AnalyzeNode(const ir::AstNode *node)
{
    if (node == nullptr) {
        return;
    }

    // NOTE(pantos) these are dummy methods to conform the CI's method size and complexity requirements
    if (AnalyzeStmtNode1(node) || AnalyzeStmtNode2(node) || AnalyzeExprNode1(node) || AnalyzeExprNode2(node)) {
        return;
    }

    switch (node->Type()) {
        case ir::AstNodeType::STRUCT_DECLARATION: {
            AnalyzeStructDecl(node->AsETSStructDeclaration());
            break;
        }
        case ir::AstNodeType::CLASS_DECLARATION: {
            AnalyzeClassDecl(node->AsClassDeclaration());
            break;
        }
        case ir::AstNodeType::CLASS_DEFINITION: {
            AnalyzeClassDef(node->AsClassDefinition());
            break;
        }
        case ir::AstNodeType::METHOD_DEFINITION: {
            AnalyzeMethodDef(node->AsMethodDefinition());
            break;
        }
        case ir::AstNodeType::VARIABLE_DECLARATION: {
            AnalyzeVarDef(node->AsVariableDeclaration());
            break;
        }
        default: {
            AnalyzeNodes(node);
            if (node->IsExpression()) {
                if (inits_.IsReset()) {
                    Merge();
                }
            }
            break;
        }
    }
}

bool AssignAnalyzer::AnalyzeStmtNode1(const ir::AstNode *node)
{
    switch (node->Type()) {
        case ir::AstNodeType::EXPRESSION_STATEMENT: {
            AnalyzeNode(node->AsExpressionStatement()->GetExpression());
            break;
        }
        case ir::AstNodeType::BLOCK_STATEMENT: {
            AnalyzeBlock(node->AsBlockStatement());
            break;
        }
        case ir::AstNodeType::DO_WHILE_STATEMENT: {
            AnalyzeDoLoop(node->AsDoWhileStatement());
            break;
        }
        case ir::AstNodeType::WHILE_STATEMENT: {
            AnalyzeWhileLoop(node->AsWhileStatement());
            break;
        }
        case ir::AstNodeType::FOR_UPDATE_STATEMENT: {
            AnalyzeForLoop(node->AsForUpdateStatement());
            break;
        }
        case ir::AstNodeType::FOR_OF_STATEMENT: {
            AnalyzeForOfLoop(node->AsForOfStatement());
            break;
        }
        case ir::AstNodeType::IF_STATEMENT: {
            AnalyzeIf(node->AsIfStatement());
            break;
        }
        default:
            return false;
    }

    return true;
}

bool AssignAnalyzer::AnalyzeStmtNode2(const ir::AstNode *node)
{
    switch (node->Type()) {
        case ir::AstNodeType::LABELLED_STATEMENT: {
            AnalyzeLabelled(node->AsLabelledStatement());
            break;
        }
        case ir::AstNodeType::SWITCH_STATEMENT: {
            AnalyzeSwitch(node->AsSwitchStatement());
            break;
        }
        case ir::AstNodeType::TRY_STATEMENT: {
            AnalyzeTry(node->AsTryStatement());
            break;
        }
        case ir::AstNodeType::BREAK_STATEMENT: {
            AnalyzeBreak(node->AsBreakStatement());
            break;
        }
        case ir::AstNodeType::CONTINUE_STATEMENT: {
            AnalyzeContinue(node->AsContinueStatement());
            break;
        }
        case ir::AstNodeType::RETURN_STATEMENT: {
            AnalyzeReturn(node->AsReturnStatement());
            break;
        }
        case ir::AstNodeType::THROW_STATEMENT: {
            AnalyzeThrow(node->AsThrowStatement());
            break;
        }
        case ir::AstNodeType::ASSERT_STATEMENT: {
            AnalyzeAssert(node->AsAssertStatement());
            break;
        }
        default:
            return false;
    }

    return true;
}

bool AssignAnalyzer::AnalyzeExprNode1(const ir::AstNode *node)
{
    switch (node->Type()) {
        case ir::AstNodeType::ETS_NEW_CLASS_INSTANCE_EXPRESSION: {
            AnalyzeNewClass(node->AsETSNewClassInstanceExpression());
            break;
        }
        case ir::AstNodeType::CALL_EXPRESSION: {
            AnalyzeCallExpr(node->AsCallExpression());
            break;
        }
        case ir::AstNodeType::IDENTIFIER: {
            AnalyzeId(node->AsIdentifier());
            break;
        }
        case ir::AstNodeType::ASSIGNMENT_EXPRESSION: {
            AnalyzeAssignExpr(node->AsAssignmentExpression());
            break;
        }
        case ir::AstNodeType::CONDITIONAL_EXPRESSION: {
            AnalyzeCondExpr(node->AsConditionalExpression());
            break;
        }
        case ir::AstNodeType::MEMBER_EXPRESSION: {
            AnalyzeMemberExpr(node->AsMemberExpression());
            break;
        }
        default:
            return false;
    }

    return true;
}

bool AssignAnalyzer::AnalyzeExprNode2(const ir::AstNode *node)
{
    switch (node->Type()) {
        case ir::AstNodeType::BINARY_EXPRESSION: {
            AnalyzeBinaryExpr(node->AsBinaryExpression());
            break;
        }
        case ir::AstNodeType::UNARY_EXPRESSION: {
            AnalyzeUnaryExpr(node->AsUnaryExpression());
            break;
        }
        case ir::AstNodeType::UPDATE_EXPRESSION: {
            AnalyzeUpdateExpr(node->AsUpdateExpression());
            break;
        }
        case ir::AstNodeType::ARROW_FUNCTION_EXPRESSION: {
            AnalyzeArrowFunctionExpr(node->AsArrowFunctionExpression());
            break;
        }
        default:
            return false;
    }

    return true;
}

void AssignAnalyzer::AnalyzeStat(const ir::AstNode *node)
{
    if (node == nullptr) {
        return;
    }

    AnalyzeNode(node);
}

void AssignAnalyzer::AnalyzeStats(const ArenaVector<ir::Statement *> &stats)
{
    for (const auto it : stats) {
        AnalyzeStat(it);
    }
}

void AssignAnalyzer::AnalyzeBlock(const ir::BlockStatement *blockStmt)
{
    ScopeGuard save(nextAdr_);

    AnalyzeStats(blockStmt->Statements());
}

void AssignAnalyzer::AnalyzeStructDecl(const ir::ETSStructDeclaration *structDecl)
{
    AnalyzeNode(structDecl->Definition());
}

void AssignAnalyzer::AnalyzeClassDecl(const ir::ClassDeclaration *classDecl)
{
    AnalyzeNode(classDecl->Definition());
}

void AssignAnalyzer::AnalyzeClassDef(const ir::ClassDefinition *classDef)
{
    if (classDef == globalClass_ && globalClassIsVisited_) {
        return;
    }

    SetOldPendingExits(PendingExits());

    ScopeGuard save(firstAdr_, nextAdr_, classDef_, classFirstAdr_);

    classDef_ = classDef;
    firstAdr_ = nextAdr_;
    classFirstAdr_ = nextAdr_;

    ProcessClassDefStaticFields(classDef_);

    // define all the instance fields
    for (const auto it : classDef->Body()) {
        if (it->IsClassProperty() && !it->IsStatic()) {
            const auto prop = it->AsClassProperty();
            NewVar(prop);
            if (prop->Value() != nullptr) {
                LetInit(prop);
            }
        }
    }

    CheckAnonymousClassCtor(classDef_);

    // process all the methods
    std::vector<const ir::AstNode *> methods;
    for (const auto it : classDef->Body()) {
        if (it->IsMethodDefinition()) {
            const auto methodDef = it->AsMethodDefinition();
            if (methodDef->Key()->AsIdentifier()->Name().Is(compiler::Signatures::INIT_METHOD)) {
                // skip the special init method as we have already checked it
                continue;
            }

            methods.push_back(methodDef);

            for (const auto it2 : methodDef->Overloads()) {
                methods.push_back(it2);
            }
        }
    }

    for (const auto it : methods) {
        AnalyzeNode(it);
    }

    SetPendingExits(OldPendingExits());
}

// NOTE (pantos) awkward methods to conform method length/complexity requirements of CI...
void AssignAnalyzer::ProcessClassDefStaticFields(const ir::ClassDefinition *classDef)
{
    // define all the static fields
    for (const auto it : classDef->Body()) {
        if (it->IsClassProperty() && it->IsStatic()) {
            const auto prop = it->AsClassProperty();
            NewVar(prop);
            if (prop->Value() != nullptr) {
                LetInit(prop);
            }
        }
    }

    // process all the static initializers
    for (const auto it : classDef->Body()) {
        if (it->IsClassStaticBlock() ||
            (it->IsStatic() && it->IsMethodDefinition() &&
             it->AsMethodDefinition()->Key()->AsIdentifier()->Name().Is(compiler::Signatures::INIT_METHOD))) {
            AnalyzeNodes(it);
            CheckPendingExits(false);
        }
    }

    // verify all static const fields got initailized
    if (classDef != globalClass_) {
        for (int i = firstAdr_; i < nextAdr_; i++) {
            const ir::AstNode *var = varDecls_[i];
            if (var->IsStatic() && (var->IsConst() || CHECK_ALL_PROPERTIES)) {
                CheckInit(var);
            }
        }
    }
}

void AssignAnalyzer::CheckAnonymousClassCtor(const ir::ClassDefinition *classDef)
{
    if (classDef == globalClass_) {
        return;
    }

    // NOTE(pantos) anonymous classes of new expressions has no default ctor right now
    // but this feature might be completely removed from the spec...
    bool hasCtor = false;
    for (const auto it : classDef->Body()) {
        if (it->IsMethodDefinition() && it->AsMethodDefinition()->IsConstructor()) {
            hasCtor = true;
            break;
        }
    }
    if (!hasCtor) {
        for (int i = firstAdr_; i < nextAdr_; i++) {
            const ir::AstNode *var = varDecls_[i];
            if (!var->IsStatic() && (var->IsConst() || CHECK_ALL_PROPERTIES)) {
                CheckInit(var);
            }
        }
    }
}

// NOTE(pantos) modified version of ETSChecker::CheckCyclicConstructorCall
static bool IsInitialConstructor(const ir::AstNode *node)
{
    if (!node->IsMethodDefinition() || !node->AsMethodDefinition()->IsConstructor()) {
        return false;
    }

    const auto methodDef = node->AsMethodDefinition();
    if (methodDef->Function()->Body() == nullptr || methodDef->Function()->IsExternal()) {
        return false;
    }

    const auto funcBody = node->AsMethodDefinition()->Function()->Body()->AsBlockStatement();

    return !(!funcBody->Statements().empty() && funcBody->Statements()[0]->IsExpressionStatement() &&
             funcBody->Statements()[0]->AsExpressionStatement()->GetExpression()->IsCallExpression() &&
             funcBody->Statements()[0]
                 ->AsExpressionStatement()
                 ->GetExpression()
                 ->AsCallExpression()
                 ->Callee()
                 ->IsThisExpression());
}

void AssignAnalyzer::AnalyzeMethodDef(const ir::MethodDefinition *methodDef)
{
    auto *func = methodDef->Function();

    if (func->Body() == nullptr || func->IsProxy()) {
        return;
    }

    Set initsPrev = inits_;
    Set uninitsPrev = uninits_;

    ScopeGuard save(firstAdr_, nextAdr_, returnAdr_, isInitialConstructor_);

    isInitialConstructor_ = IsInitialConstructor(methodDef);
    if (!isInitialConstructor_) {
        firstAdr_ = nextAdr_;
    }

    AnalyzeStat(func->Body());

    if (isInitialConstructor_) {
        for (int i = firstAdr_; i < nextAdr_; i++) {
            const ir::AstNode *var = varDecls_[i];
            if (!var->IsStatic() && (var->IsConst() || CHECK_ALL_PROPERTIES)) {
                CheckInit(var);
            }
        }
    }

    CheckPendingExits(true);

    inits_ = initsPrev;
    uninits_ = uninitsPrev;
}

void AssignAnalyzer::AnalyzeVarDef(const ir::VariableDeclaration *varDef)
{
    for (auto *var : varDef->Declarators()) {
        NewVar(var);

        if (var->Init() != nullptr) {
            AnalyzeNode(var->Init());
            LetInit(var);
        }
    }
}

void AssignAnalyzer::AnalyzeDoLoop(const ir::DoWhileStatement *doWhileStmt)
{
    SetOldPendingExits(PendingExits());

    Set initsSkip {};
    Set uninitsSkip {};
    int prevErrors = numErrors_;

    for (int phase = 1; phase <= LOOP_PHASES; phase++) {
        Set uninitsEntry = uninits_;
        uninitsEntry.ExcludeFrom(nextAdr_);

        AnalyzeStat(doWhileStmt->Body());

        ResolveContinues(doWhileStmt);

        AnalyzeCond(doWhileStmt->Test());

        if (phase == 1) {
            initsSkip = initsWhenFalse_;
            uninitsSkip = uninitsWhenFalse_;
        }

        if (prevErrors != numErrors_ || phase == LOOP_PHASES ||
            uninitsEntry.DiffSet(uninitsWhenTrue_).Next(firstAdr_) == -1) {
            break;
        }

        inits_ = initsWhenTrue_;
        uninits_ = uninitsEntry.AndSet(uninitsWhenTrue_);
    }

    inits_ = initsSkip;
    uninits_ = uninitsSkip;

    ResolveBreaks(doWhileStmt);
}

void AssignAnalyzer::AnalyzeWhileLoop(const ir::WhileStatement *whileStmt)
{
    SetOldPendingExits(PendingExits());

    Set initsSkip {};
    Set uninitsSkip {};
    int prevErrors = numErrors_;

    Set uninitsEntry = uninits_;
    uninitsEntry.ExcludeFrom(nextAdr_);

    for (int phase = 1; phase <= LOOP_PHASES; phase++) {
        AnalyzeCond(whileStmt->Test());

        if (phase == 1) {
            initsSkip = initsWhenFalse_;
            uninitsSkip = uninitsWhenFalse_;
        }

        inits_ = initsWhenTrue_;
        uninits_ = uninitsWhenTrue_;

        AnalyzeStat(whileStmt->Body());

        ResolveContinues(whileStmt);

        if (prevErrors != numErrors_ || phase == LOOP_PHASES || uninitsEntry.DiffSet(uninits_).Next(firstAdr_) == -1) {
            break;
        }

        uninits_ = uninitsEntry.AndSet(uninits_);
    }

    inits_ = initsSkip;
    uninits_ = uninitsSkip;

    ResolveBreaks(whileStmt);
}

void AssignAnalyzer::AnalyzeForLoop(const ir::ForUpdateStatement *forStmt)
{
    ScopeGuard save(nextAdr_);

    AnalyzeNode(forStmt->Init());

    Set initsSkip {};
    Set uninitsSkip {};
    int prevErrors = numErrors_;

    SetOldPendingExits(PendingExits());

    for (int phase = 1; phase <= LOOP_PHASES; phase++) {
        Set uninitsEntry = uninits_;
        uninitsEntry.ExcludeFrom(nextAdr_);

        if (forStmt->Test() != nullptr) {
            AnalyzeCond(forStmt->Test());

            if (phase == 1) {
                initsSkip = initsWhenFalse_;
                uninitsSkip = uninitsWhenFalse_;
            }

            inits_ = initsWhenTrue_;
            uninits_ = uninitsWhenTrue_;
        } else if (phase == 1) {
            initsSkip = inits_;
            initsSkip.InclRange(firstAdr_, nextAdr_);
            uninitsSkip = uninits_;
            uninitsSkip.InclRange(firstAdr_, nextAdr_);
        }

        AnalyzeStat(forStmt->Body());

        ResolveContinues(forStmt);

        AnalyzeNode(forStmt->Update());

        if (prevErrors != numErrors_ || phase == LOOP_PHASES || uninitsEntry.DiffSet(uninits_).Next(firstAdr_) == -1) {
            break;
        }

        uninits_ = uninitsEntry.AndSet(uninits_);
    }

    inits_ = initsSkip;
    uninits_ = uninitsSkip;

    ResolveBreaks(forStmt);
}

void AssignAnalyzer::AnalyzeForOfLoop(const ir::ForOfStatement *forOfStmt)
{
    ScopeGuard save(nextAdr_);

    if (forOfStmt->Left()->IsVariableDeclaration()) {
        AnalyzeVarDef(forOfStmt->Left()->AsVariableDeclaration());
        for (auto *var : forOfStmt->Left()->AsVariableDeclaration()->Declarators()) {
            LetInit(var);
        }
    } else {
        LetInit(forOfStmt->Left());
    }

    AnalyzeNode(forOfStmt->Right());

    Set initsStart = inits_;
    Set uninitsStart = uninits_;
    int prevErrors = numErrors_;

    SetOldPendingExits(PendingExits());

    for (int phase = 1; phase <= LOOP_PHASES; phase++) {
        Set uninitsEntry = uninits_;
        uninitsEntry.ExcludeFrom(nextAdr_);

        AnalyzeStat(forOfStmt->Body());

        ResolveContinues(forOfStmt);

        if (prevErrors != numErrors_ || phase == LOOP_PHASES || uninitsEntry.DiffSet(uninits_).Next(firstAdr_) == -1) {
            break;
        }

        uninits_ = uninitsEntry.AndSet(uninits_);
    }

    inits_ = initsStart;
    uninits_ = uninitsStart.AndSet(uninits_);

    ResolveBreaks(forOfStmt);
}

void AssignAnalyzer::AnalyzeIf(const ir::IfStatement *ifStmt)
{
    AnalyzeCond(ifStmt->Test());

    Set initsBeforeElse = initsWhenFalse_;
    Set uninitsBeforeElse = uninitsWhenFalse_;
    inits_ = initsWhenTrue_;
    uninits_ = uninitsWhenTrue_;

    AnalyzeStat(ifStmt->Consequent());

    if (ifStmt->Alternate() != nullptr) {
        Set initsAfterThen = inits_;
        Set uninitsAfterThen = uninits_;
        inits_ = initsBeforeElse;
        uninits_ = uninitsBeforeElse;

        AnalyzeStat(ifStmt->Alternate());

        inits_.AndSet(initsAfterThen);
        uninits_.AndSet(uninitsAfterThen);
    } else {
        inits_.AndSet(initsBeforeElse);
        uninits_.AndSet(uninitsBeforeElse);
    }
}

void AssignAnalyzer::AnalyzeLabelled(const ir::LabelledStatement *labelledStmt)
{
    SetOldPendingExits(PendingExits());

    AnalyzeStat(labelledStmt->Body());

    ResolveBreaks(labelledStmt);
}

void AssignAnalyzer::AnalyzeSwitch(const ir::SwitchStatement *switchStmt)
{
    SetOldPendingExits(PendingExits());

    ScopeGuard save(nextAdr_);

    AnalyzeNode(switchStmt->Discriminant());

    Set initsSwitch = inits_;
    Set uninitsSwitch = uninits_;

    bool hasDefault = false;

    for (const auto caseClause : switchStmt->Cases()) {
        inits_ = initsSwitch;
        uninits_ = uninits_.AndSet(uninitsSwitch);

        if (caseClause->Test() == nullptr) {
            hasDefault = true;
        } else {
            AnalyzeNode(caseClause->Test());
        }

        if (hasDefault) {
            inits_ = initsSwitch;
            uninits_ = uninits_.AndSet(uninitsSwitch);
        }

        AnalyzeStats(caseClause->Consequent());

        for (const auto stmt : caseClause->Consequent()) {
            if (!stmt->IsVariableDeclaration()) {
                continue;
            }
            for (auto *var : stmt->AsVariableDeclaration()->Declarators()) {
                NodeId adr = GetNodeId(var);
                ASSERT(adr >= 0);
                initsSwitch.Excl(adr);
                uninitsSwitch.Incl(adr);
            }
        }

        if (!hasDefault) {
            inits_ = initsSwitch;
            uninits_ = uninits_.AndSet(uninitsSwitch);
        }
    }

    if (!hasDefault) {
        inits_.AndSet(initsSwitch);
    }

    ResolveBreaks(switchStmt);
}

void AssignAnalyzer::AnalyzeTry(const ir::TryStatement *tryStmt)
{
    Set uninitsTryPrev = uninitsTry_;

    PendingExitsVector prevPendingExits = PendingExits();
    SetOldPendingExits(prevPendingExits);

    Set initsTry = inits_;
    uninitsTry_ = uninits_;

    AnalyzeNode(tryStmt->Block());

    uninitsTry_.AndSet(uninits_);

    Set initsEnd = inits_;
    Set uninitsEnd = uninits_;
    int nextAdrCatch = nextAdr_;

    Set initsCatchPrev = initsTry;  // NOLINT(performance-unnecessary-copy-initialization)
    Set uninitsCatchPrev = uninitsTry_;

    for (const auto catchClause : tryStmt->CatchClauses()) {
        inits_ = initsCatchPrev;
        uninits_ = uninitsCatchPrev;

        AnalyzeNode(catchClause->Body());

        initsEnd.AndSet(inits_);
        uninitsEnd.AndSet(uninits_);
        nextAdr_ = nextAdrCatch;
    }

    if (tryStmt->FinallyBlock() != nullptr) {
        inits_ = initsTry;
        uninits_ = uninitsTry_;

        PendingExitsVector exits = PendingExits();
        SetPendingExits(prevPendingExits);

        AnalyzeNode(tryStmt->FinallyBlock());

        if (tryStmt->FinallyCanCompleteNormally()) {
            uninits_.AndSet(uninitsEnd);
            for (auto exit : exits) {
                exit.exitInits_.OrSet(inits_);
                exit.exitUninits_.AndSet(uninits_);
                PendingExits().push_back(exit);
            }
            inits_.OrSet(initsEnd);
        }
    } else {
        inits_ = initsEnd;
        uninits_ = uninitsEnd;

        PendingExitsVector exits = PendingExits();
        SetPendingExits(prevPendingExits);

        for (const auto &exit : exits) {
            PendingExits().push_back(exit);
        }
    }

    uninitsTry_.AndSet(uninitsTryPrev).AndSet(uninits_);
}

void AssignAnalyzer::AnalyzeBreak(const ir::BreakStatement *breakStmt)
{
    RecordExit(AssignPendingExit(breakStmt, inits_, uninits_));
}

void AssignAnalyzer::AnalyzeContinue(const ir::ContinueStatement *contStmt)
{
    RecordExit(AssignPendingExit(contStmt, inits_, uninits_));
}

void AssignAnalyzer::AnalyzeReturn(const ir::ReturnStatement *retStmt)
{
    AnalyzeNode(retStmt->Argument());
    RecordExit(AssignPendingExit(retStmt, inits_, uninits_));
}

void AssignAnalyzer::AnalyzeThrow(const ir::ThrowStatement *throwStmt)
{
    AnalyzeNode(throwStmt->Argument());
    MarkDead();
}

void AssignAnalyzer::AnalyzeAssert(const ir::AssertStatement *assertStmt)
{
    Set initsExit = inits_;
    Set uninitsExit = uninits_;

    AnalyzeCond(assertStmt->Test());

    uninitsExit.AndSet(uninitsWhenTrue_);

    if (assertStmt->Second() != nullptr) {
        inits_ = initsWhenFalse_;
        uninits_ = uninitsWhenFalse_;
        AnalyzeExpr(assertStmt->Second());
    }

    inits_ = initsExit;
    uninits_ = uninitsExit;
}

void AssignAnalyzer::AnalyzeExpr(const ir::AstNode *node)
{
    if (node != nullptr) {
        AnalyzeNode(node);
        if (inits_.IsReset()) {
            Merge();
        }
    }
}

void AssignAnalyzer::AnalyzeExprs(const ArenaVector<ir::Expression *> &exprs)
{
    for (const auto it : exprs) {
        AnalyzeExpr(it);
    }
}

void AssignAnalyzer::AnalyzeCond(const ir::AstNode *node)
{
    ASSERT(node->IsExpression());
    const ir::Expression *expr = node->AsExpression();

    if (auto etype = expr->TsType();
        etype != nullptr && etype->IsETSBooleanType() && etype->HasTypeFlag(TypeFlag::CONSTANT)) {
        const ETSBooleanType *condType = etype->AsETSBooleanType();
        if (inits_.IsReset()) {
            Merge();
        }
        if (condType->GetValue()) {
            initsWhenFalse_ = inits_;
            initsWhenFalse_.InclRange(firstAdr_, nextAdr_);
            uninitsWhenFalse_ = uninits_;
            uninitsWhenFalse_.InclRange(firstAdr_, nextAdr_);
            initsWhenTrue_ = inits_;
            uninitsWhenTrue_ = uninits_;
        } else {
            initsWhenTrue_ = inits_;
            initsWhenTrue_.InclRange(firstAdr_, nextAdr_);
            uninitsWhenTrue_ = uninits_;
            uninitsWhenTrue_.InclRange(firstAdr_, nextAdr_);
            initsWhenFalse_ = inits_;
            uninitsWhenFalse_ = uninits_;
        }
    } else {
        AnalyzeNode(node);
        if (!inits_.IsReset()) {
            Split(true);
        }
    }

    inits_.Reset();
    uninits_.Reset();
}

void AssignAnalyzer::AnalyzeId(const ir::Identifier *id)
{
    if (id->Parent()->IsProperty() && id->Parent()->AsProperty()->Key() == id &&
        id->Parent()->Parent()->IsObjectExpression()) {
        return;  // inside ObjectExpression
    }

    if (id->Parent()->IsTypeofExpression() && id->Parent()->AsTypeofExpression()->Argument() == id) {
        return;  // according to the spec 'typeof' works on uninitialized variables too
    }

    if (id->Parent()->IsBinaryExpression()) {
        const ir::BinaryExpression *binExpr = id->Parent()->AsBinaryExpression();
        if ((binExpr->OperatorType() == lexer::TokenType::PUNCTUATOR_EQUAL ||
             binExpr->OperatorType() == lexer::TokenType::PUNCTUATOR_NOT_EQUAL) &&
            (binExpr->Left()->IsNullLiteral() || binExpr->Right()->IsNullLiteral() ||
             binExpr->Left()->IsUndefinedLiteral() || binExpr->Right()->IsUndefinedLiteral())) {
            return;  // null/undefined comparison with == or != operators (e.g. in assert statement)
        }
    }

    if (id->Parent()->IsMemberExpression()) {
        const ir::MemberExpression *membExpr = id->Parent()->AsMemberExpression();
        if (id == membExpr->Property() && !membExpr->Object()->IsThisExpression() &&
            membExpr->HasMemberKind(ir::MemberExpressionKind::PROPERTY_ACCESS)) {
            return;  // something.property
        }
    }

    if (id->Variable() != nullptr) {
        CheckInit(id);
    }
}

static bool IsIdentOrThisDotIdent(const ir::AstNode *node)
{
    return node->IsIdentifier() ||
           (node->IsMemberExpression() && node->AsMemberExpression()->Object()->IsThisExpression());
}

void AssignAnalyzer::AnalyzeAssignExpr(const ir::AssignmentExpression *assignExpr)
{
    if (!IsIdentOrThisDotIdent(assignExpr->Left())) {
        AnalyzeExpr(assignExpr->Left());
    }

    AnalyzeExpr(assignExpr->Right());

    if (assignExpr->OperatorType() == lexer::TokenType::PUNCTUATOR_SUBSTITUTION) {
        LetInit(assignExpr->Left());
    } else {
        CheckInit(assignExpr->Left());
    }
}

void AssignAnalyzer::AnalyzeCondExpr(const ir::ConditionalExpression *condExpr)
{
    AnalyzeCond(condExpr->Test());

    Set initsBeforeElse = initsWhenFalse_;
    Set uninitsBeforeElse = uninitsWhenFalse_;
    inits_ = initsWhenTrue_;
    uninits_ = uninitsWhenTrue_;

    ASSERT(condExpr->Consequent()->TsType() && condExpr->Alternate()->TsType());

    if (condExpr->Consequent()->TsType()->IsETSBooleanType() && condExpr->Alternate()->TsType()->IsETSBooleanType()) {
        AnalyzeCond(condExpr->Consequent());

        Set initsAfterThenWhenTrue = initsWhenTrue_;
        Set initsAfterThenWhenFalse = initsWhenFalse_;
        Set uninitsAfterThenWhenTrue = uninitsWhenTrue_;
        Set uninitsAfterThenWhenFalse = uninitsWhenFalse_;
        inits_ = initsBeforeElse;
        uninits_ = uninitsBeforeElse;

        AnalyzeCond(condExpr->Alternate());

        initsWhenTrue_.AndSet(initsAfterThenWhenTrue);
        initsWhenFalse_.AndSet(initsAfterThenWhenFalse);
        uninitsWhenTrue_.AndSet(uninitsAfterThenWhenTrue);
        uninitsWhenFalse_.AndSet(uninitsAfterThenWhenFalse);
    } else {
        AnalyzeExpr(condExpr->Consequent());

        Set initsAfterThen = inits_;
        Set uninitsAfterThen = uninits_;
        inits_ = initsBeforeElse;
        uninits_ = uninitsBeforeElse;

        AnalyzeExpr(condExpr->Alternate());

        inits_.AndSet(initsAfterThen);
        uninits_.AndSet(uninitsAfterThen);
    }
}

void AssignAnalyzer::AnalyzeCallExpr(const ir::CallExpression *callExpr)
{
    AnalyzeExpr(callExpr->Callee());
    AnalyzeExprs(callExpr->Arguments());
}

void AssignAnalyzer::AnalyzeMemberExpr(const ir::MemberExpression *membExpr)
{
    if (membExpr->Object()->IsThisExpression() && membExpr->HasMemberKind(ir::MemberExpressionKind::PROPERTY_ACCESS)) {
        CheckInit(membExpr);
    } else {
        AnalyzeNode(membExpr->Object());
        AnalyzeNode(membExpr->Property());
    }
}

void AssignAnalyzer::AnalyzeNewClass(const ir::ETSNewClassInstanceExpression *newClass)
{
    AnalyzeExpr(newClass->GetTypeRef());
    AnalyzeExprs(newClass->GetArguments());
    AnalyzeNode(newClass->ClassDefinition());
}

void AssignAnalyzer::AnalyzeUnaryExpr(const ir::UnaryExpression *unaryExpr)
{
    AnalyzeCond(unaryExpr->Argument());

    switch (unaryExpr->OperatorType()) {
        case lexer::TokenType::PUNCTUATOR_EXCLAMATION_MARK: {
            Set t = initsWhenFalse_;
            initsWhenFalse_ = initsWhenTrue_;
            initsWhenTrue_ = t;
            t = uninitsWhenFalse_;
            uninitsWhenFalse_ = uninitsWhenTrue_;
            uninitsWhenTrue_ = t;
            break;
        }
        default: {
            AnalyzeExpr(unaryExpr->Argument());
            break;
        }
    }
}

void AssignAnalyzer::AnalyzeBinaryExpr(const ir::BinaryExpression *binExpr)
{
    switch (binExpr->OperatorType()) {
        case lexer::TokenType::PUNCTUATOR_LOGICAL_AND: {
            AnalyzeCond(binExpr->Left());
            Set initsWhenFalseLeft = initsWhenFalse_;
            Set uninitsWhenFalseLeft = uninitsWhenFalse_;
            inits_ = initsWhenTrue_;
            uninits_ = uninitsWhenTrue_;
            AnalyzeCond(binExpr->Right());
            initsWhenFalse_.AndSet(initsWhenFalseLeft);
            uninitsWhenFalse_.AndSet(uninitsWhenFalseLeft);
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LOGICAL_OR: {
            AnalyzeCond(binExpr->Left());
            Set initsWhenTrueLeft = initsWhenTrue_;
            Set uninitsWhenTrueLeft = uninitsWhenTrue_;
            inits_ = initsWhenFalse_;
            uninits_ = uninitsWhenFalse_;
            AnalyzeCond(binExpr->Right());
            initsWhenTrue_.AndSet(initsWhenTrueLeft);
            uninitsWhenTrue_.AndSet(uninitsWhenTrueLeft);
            break;
        }
        default: {
            AnalyzeExpr(binExpr->Left());
            AnalyzeExpr(binExpr->Right());
            break;
        }
    }
}

void AssignAnalyzer::AnalyzeUpdateExpr(const ir::UpdateExpression *updateExpr)
{
    AnalyzeExpr(updateExpr->Argument());
    LetInit(updateExpr->Argument());
}

void AssignAnalyzer::AnalyzeArrowFunctionExpr(const ir::ArrowFunctionExpression *arrowFuncExpr)
{
    // NOTE (pantos) handle lamdas correctly
    (void)arrowFuncExpr;
}

util::StringView AssignAnalyzer::GetVariableType(const ir::AstNode *node) const
{
    switch (node->Type()) {
        case ir::AstNodeType::CLASS_PROPERTY:
            if (node->AsClassProperty()->Parent() == globalClass_) {
                return "variable";
            } else {
                return "property";
            }
        case ir::AstNodeType::VARIABLE_DECLARATOR:
            return "variable";
        default:
            UNREACHABLE();
    }
}

util::StringView AssignAnalyzer::GetVariableName(const ir::AstNode *node) const
{
    switch (node->Type()) {
        case ir::AstNodeType::CLASS_PROPERTY:
            return node->AsClassProperty()->Id()->Name();
        case ir::AstNodeType::VARIABLE_DECLARATOR:
            return node->AsVariableDeclarator()->Id()->AsIdentifier()->Name();
        default:
            UNREACHABLE();
    }
}

const lexer::SourcePosition &AssignAnalyzer::GetVariablePosition(const ir::AstNode *node) const
{
    switch (node->Type()) {
        case ir::AstNodeType::CLASS_PROPERTY:
            return node->AsClassProperty()->Key()->Start();
        case ir::AstNodeType::VARIABLE_DECLARATOR:
        default:
            return node->Start();
    }
}

NodeId AssignAnalyzer::GetNodeId(const ir::AstNode *node) const
{
    auto res = nodeIdMap_.find(node);
    if (res != nodeIdMap_.end()) {
        return res->second;
    }
    return INVALID_ID;
}

bool AssignAnalyzer::Trackable(const ir::AstNode *node) const
{
    switch (node->Type()) {
        case ir::AstNodeType::CLASS_PROPERTY:
        case ir::AstNodeType::VARIABLE_DECLARATOR:
            return true;
        default:
            return false;
    }
}

bool AssignAnalyzer::IsConstUninitializedField(const ir::AstNode *node) const
{
    return node->IsClassProperty() && node->IsConst();
}

bool AssignAnalyzer::IsConstUninitializedStaticField(const ir::AstNode *node) const
{
    return IsConstUninitializedField(node) && node->IsStatic();
}

void AssignAnalyzer::NewVar(const ir::AstNode *node)
{
    if (!Trackable(node)) {
        return;
    }

    if (GetNodeId(node) != INVALID_ID) {
        return;
    }

    nodeIdMap_[node] = nextAdr_;
    varDecls_.reserve(nextAdr_ + 1);
    varDecls_.insert(varDecls_.begin() + nextAdr_, node);
    inits_.Excl(nextAdr_);
    uninits_.Incl(nextAdr_);
    ++nextAdr_;
}

varbinder::Variable *AssignAnalyzer::GetBoundVariable(const ir::AstNode *node)
{
    varbinder::Variable *ret = nullptr;

    if (node->IsClassProperty()) {
        ret = node->AsClassProperty()->Id()->Variable();
    } else if (node->IsVariableDeclarator()) {
        ret = node->AsVariableDeclarator()->Id()->AsIdentifier()->Variable();
    } else {
        UNREACHABLE();
    }

    return ret;
}

const ir::AstNode *AssignAnalyzer::GetDeclaringNode(const ir::AstNode *node)
{
    if (node->IsClassProperty() || node->IsVariableDeclarator()) {
        return node;
    }

    const ir::AstNode *ret = nullptr;

    if (node->IsMemberExpression()) {
        const ir::MemberExpression *membExpr = node->AsMemberExpression();
        if (membExpr->PropVar() != nullptr) {
            if (membExpr->PropVar()->Declaration() != nullptr) {
                ret = membExpr->PropVar()->Declaration()->Node();
            }
        }
    } else if (node->IsIdentifier()) {
        const ir::Identifier *id = node->AsIdentifier();
        if (id->Variable() != nullptr) {
            if (id->Variable()->Declaration() != nullptr) {
                ret = id->Variable()->Declaration()->Node();
            }
        }
    }

    if (ret != nullptr) {
        if (ret->IsIdentifier() && ret->Parent()->IsVariableDeclarator() &&
            ret == ret->Parent()->AsVariableDeclarator()->Id()) {
            ret = ret->Parent();
        }
    }

    return ret;
}

bool AssignAnalyzer::VariableHasDefaultValue(const ir::AstNode *node)
{
    ASSERT(node != nullptr);

    const checker::Type *type = nullptr;
    bool isNonReadonlyField = false;

    if (node->IsClassProperty()) {
        type = node->AsClassProperty()->TsType();
        isNonReadonlyField = !node->IsReadonly();  // NOTE(pantos) readonly is true, const is not set?
    } else if (node->IsVariableDeclarator()) {
        varbinder::Variable *variable = GetBoundVariable(node);
        if (variable != nullptr) {
            type = variable->TsType();
        }
    } else {
        UNREACHABLE();
    }

    return type != nullptr &&
           (type->IsETSPrimitiveType() ||
            (type->PossiblyETSNullish() && (!type->HasTypeFlag(checker::TypeFlag::GENERIC) ||
                                            (isNonReadonlyField && !CHECK_GENERIC_NON_READONLY_PROPERTIES))));
}

void AssignAnalyzer::LetInit(const ir::AstNode *node)
{
    const ir::AstNode *declNode = GetDeclaringNode(node);

    if (declNode == nullptr || declNode->IsDeclare()) {
        return;
    }

    NodeId adr = GetNodeId(declNode);
    if (adr == INVALID_ID) {
        return;
    }

    if (node != declNode && declNode->IsConst()) {
        // check reassignment of readonly properties
        util::StringView type = GetVariableType(declNode);
        util::StringView name = GetVariableName(declNode);
        const lexer::SourcePosition &pos = GetVariablePosition(node);

        auto uninit = [this](NodeId a) {
            uninits_.Excl(a);
            if (!inits_.IsMember(a)) {
                uninitsTry_.Excl(a);
            }
        };

        if (classDef_ == globalClass_ || (adr < classFirstAdr_ || adr >= firstAdr_)) {
            if (declNode->IsClassProperty() && classDef_ != declNode->Parent()) {
                Warning({"Cannot assign to '", name, "' because it is a read-only property."}, pos);
            } else if (!uninits_.IsMember(adr)) {
                Warning({Capitalize(type).c_str(), " '", name, "' might already have been assigned."}, pos);
            } else {
                uninit(adr);
            }
        }
    }

    inits_.Incl(adr);
}

void AssignAnalyzer::CheckInit(const ir::AstNode *node)
{
    const ir::AstNode *declNode = GetDeclaringNode(node);

    if (declNode == nullptr || declNode->IsDeclare()) {
        return;
    }

    NodeId adr = GetNodeId(declNode);
    if (adr == INVALID_ID) {
        return;
    }

    if (VariableHasDefaultValue(declNode)) {
        // no explicit init is required (primitive, nullish)
        return;
    }

    if (declNode->IsClassProperty()) {
        if (!CHECK_ALL_PROPERTIES && !declNode->IsConst()) {
            // non readonly property
            return;
        }

        if (declNode->Parent() == globalClass_) {
            // NOTE(pantos) dont check global variable accesses
            return;
        }

        if (declNode->Parent() != classDef_) {
            // property of an other class
            return;
        }
    }

    if (classDef_ == globalClass_ || (adr < classFirstAdr_ || adr >= firstAdr_)) {
        if (!inits_.IsMember(adr)) {
            if (WARN_NO_INIT_ONCE_PER_VARIABLE && !foundErrors_.insert(declNode).second) {
                return;
            }

            util::StringView type = GetVariableType(declNode);
            util::StringView name = GetVariableName(declNode);
            const lexer::SourcePosition &pos = GetVariablePosition(node);

            std::stringstream ss;
            if (node->IsClassProperty()) {
                ss << "Property '" << name << "' might not have been initialized.";
            } else {
                ss << Capitalize(type) << " '" << name << "' is used before being assigned.";
            }

            Warning(ss.str(), pos);
        }
    }
}

void AssignAnalyzer::Split(const bool setToNull)
{
    initsWhenFalse_ = inits_;
    uninitsWhenFalse_ = uninits_;
    initsWhenTrue_ = inits_;
    uninitsWhenTrue_ = uninits_;
    if (setToNull) {
        inits_.Reset();
        uninits_.Reset();
    }
}

void AssignAnalyzer::Merge()
{
    inits_ = initsWhenFalse_.AndSet(initsWhenTrue_);
    uninits_ = uninitsWhenFalse_.AndSet(uninitsWhenTrue_);
}

void AssignAnalyzer::CheckPendingExits(bool inMethod)
{
    PendingExitsVector exits = PendingExits();

    for (auto &it : exits) {
        // NOTE(pantos) pending exits should be refactored, break/continue may stay in this
        if (inMethod && !it.Node()->IsReturnStatement()) {
            continue;
        }

        if (inMethod && isInitialConstructor_) {
            inits_ = it.exitInits_;

            for (int i = firstAdr_; i < nextAdr_; i++) {
                CheckInit(varDecls_[i]);
            }
        }
    }

    ClearPendingExits();
}

void AssignAnalyzer::MarkDead()
{
    if (!isInitialConstructor_) {
        inits_.InclRange(returnAdr_, nextAdr_);
    } else {
        for (int address = returnAdr_; address < nextAdr_; address++) {
            if (!IsConstUninitializedStaticField(varDecls_[address])) {
                inits_.Incl(address);
            }
        }
    }
    uninits_.InclRange(returnAdr_, nextAdr_);
}

}  // namespace ark::es2panda::checker
