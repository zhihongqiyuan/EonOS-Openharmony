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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_ASSIGN_ANALYZER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_ASSIGN_ANALYZER_H

#include "checker/ETSchecker.h"
#include "checker/ets/baseAnalyzer.h"

#include "utils/arena_containers.h"

namespace ark::es2panda::ir {
class AstNode;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::checker {

class Set {
public:
    Set() = default;
    ~Set() = default;

    DEFAULT_COPY_SEMANTIC(Set);
    DEFAULT_NOEXCEPT_MOVE_SEMANTIC(Set);

    void Reset();
    bool IsReset();
    void Incl(const int id);
    void InclRange(const int start, const int limit);
    void ExcludeFrom(const int start);
    void Excl(const int id);
    bool IsMember(const int id) const;
    Set &AndSet(const Set &xs);
    Set &OrSet(const Set &xs);
    Set &DiffSet(const Set &xs);
    int Next(const int id);

protected:
    void InternalAndSet(const Set &xs);

private:
    bool reset_ {};
    std::set<int> nodes_ {};
};

class AssignPendingExit : public PendingExit {
public:
    explicit AssignPendingExit(const ir::AstNode *node, Set &inits, Set &uninits)
        : PendingExit(node), inits_(&inits), uninits_(&uninits)
    {
        exitInits_ = inits;
        exitUninits_ = uninits;
    }
    ~AssignPendingExit() override = default;

    DEFAULT_COPY_SEMANTIC(AssignPendingExit);
    DEFAULT_NOEXCEPT_MOVE_SEMANTIC(AssignPendingExit);

    void ResolveJump() override
    {
        inits_->AndSet(exitInits_);
        uninits_->AndSet(exitUninits_);
    }

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes,readability-identifier-naming)
    Set *inits_;
    Set *uninits_;
    Set exitInits_;
    Set exitUninits_;
    // NOLINTEND(misc-non-private-member-variables-in-classes,readability-identifier-naming)
};

using NodeId = int;
using NodeIdMap = ArenaMap<const ir::AstNode *, NodeId>;

class AssignAnalyzer : public BaseAnalyzer<AssignPendingExit> {
public:
    explicit AssignAnalyzer(ETSChecker *checker);
    void Analyze(const ir::AstNode *node);

    void MarkDead() override;

private:
    // node visitors
    void AnalyzeNodes(const ir::AstNode *node);
    void AnalyzeNode(const ir::AstNode *node);
    bool AnalyzeStmtNode1(const ir::AstNode *node);
    bool AnalyzeStmtNode2(const ir::AstNode *node);
    bool AnalyzeExprNode1(const ir::AstNode *node);
    bool AnalyzeExprNode2(const ir::AstNode *node);
    void AnalyzeStat(const ir::AstNode *node);
    void AnalyzeStats(const ArenaVector<ir::Statement *> &stats);
    void AnalyzeBlock(const ir::BlockStatement *blockStmt);
    void AnalyzeStructDecl(const ir::ETSStructDeclaration *structDecl);
    void AnalyzeClassDecl(const ir::ClassDeclaration *classDecl);
    void AnalyzeClassDef(const ir::ClassDefinition *classDef);
    void ProcessClassDefStaticFields(const ir::ClassDefinition *classDef);
    void CheckAnonymousClassCtor(const ir::ClassDefinition *classDef);
    void AnalyzeMethodDef(const ir::MethodDefinition *methodDef);
    void AnalyzeVarDef(const ir::VariableDeclaration *varDef);
    void AnalyzeDoLoop(const ir::DoWhileStatement *doWhileStmt);
    void AnalyzeWhileLoop(const ir::WhileStatement *whileStmt);
    void AnalyzeForLoop(const ir::ForUpdateStatement *forStmt);
    void AnalyzeForOfLoop(const ir::ForOfStatement *forOfStmt);
    void AnalyzeIf(const ir::IfStatement *ifStmt);
    void AnalyzeLabelled(const ir::LabelledStatement *labelledStmt);
    void AnalyzeSwitch(const ir::SwitchStatement *switchStmt);
    void AnalyzeTry(const ir::TryStatement *tryStmt);
    void AnalyzeBreak(const ir::BreakStatement *breakStmt);
    void AnalyzeContinue(const ir::ContinueStatement *contStmt);
    void AnalyzeReturn(const ir::ReturnStatement *retStmt);
    void AnalyzeThrow(const ir::ThrowStatement *throwStmt);
    void AnalyzeAssert(const ir::AssertStatement *assertStmt);
    void AnalyzeExpr(const ir::AstNode *node);
    void AnalyzeExprs(const ArenaVector<ir::Expression *> &exprs);
    void AnalyzeCond(const ir::AstNode *node);
    void AnalyzeAssignExpr(const ir::AssignmentExpression *assignExpr);
    void AnalyzeBinaryExpr(const ir::BinaryExpression *binExpr);
    void AnalyzeCallExpr(const ir::CallExpression *callExpr);
    void AnalyzeCondExpr(const ir::ConditionalExpression *condExpr);
    void AnalyzeId(const ir::Identifier *id);
    void AnalyzeMemberExpr(const ir::MemberExpression *membExpr);
    void AnalyzeNewClass(const ir::ETSNewClassInstanceExpression *newClass);
    void AnalyzeUnaryExpr(const ir::UnaryExpression *unaryExpr);
    void AnalyzeUpdateExpr(const ir::UpdateExpression *updateExpr);
    void AnalyzeArrowFunctionExpr(const ir::ArrowFunctionExpression *arrowFuncExpr);

    // utils
    void Warning(std::string_view message, const lexer::SourcePosition &pos);
    void Warning(std::initializer_list<TypeErrorMessageElement> list, const lexer::SourcePosition &pos);
    bool Trackable(const ir::AstNode *node) const;
    bool IsConstUninitializedField(const ir::AstNode *node) const;
    bool IsConstUninitializedStaticField(const ir::AstNode *node) const;
    void NewVar(const ir::AstNode *node);
    void LetInit(const ir::AstNode *node);
    void CheckInit(const ir::AstNode *node);
    void Split(bool setToNull);
    void Merge();
    void CheckPendingExits(bool inMethod);
    NodeId GetNodeId(const ir::AstNode *node) const;
    util::StringView GetVariableType(const ir::AstNode *node) const;
    util::StringView GetVariableName(const ir::AstNode *node) const;
    const lexer::SourcePosition &GetVariablePosition(const ir::AstNode *node) const;
    const ir::AstNode *GetDeclaringNode(const ir::AstNode *node);
    varbinder::Variable *GetBoundVariable(const ir::AstNode *node);
    bool VariableHasDefaultValue(const ir::AstNode *node);

    ETSChecker *checker_;
    Set inits_ {};
    Set uninits_ {};
    Set uninitsTry_ {};
    Set initsWhenTrue_ {};
    Set initsWhenFalse_ {};
    Set uninitsWhenTrue_ {};
    Set uninitsWhenFalse_ {};
    ArenaVector<const ir::AstNode *> varDecls_;
    const ir::ClassDefinition *globalClass_ {};
    const ir::ClassDefinition *classDef_ {};
    bool globalClassIsVisited_ {};
    int classFirstAdr_ {};
    int firstNonGlobalAdr_ {};
    int firstAdr_ {};
    int nextAdr_ {};
    int returnAdr_ {};
    bool isInitialConstructor_ {};
    NodeIdMap nodeIdMap_;
    int numErrors_ {};
    ArenaSet<const ir::AstNode *> foundErrors_;
};

}  // namespace ark::es2panda::checker

#endif
