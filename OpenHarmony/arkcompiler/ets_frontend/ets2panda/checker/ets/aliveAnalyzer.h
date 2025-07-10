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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_ALIVE_ANALYZER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_ALIVE_ANALYZER_H

#include "checker/ETSchecker.h"
#include "checker/ets/baseAnalyzer.h"

#include "utils/arena_containers.h"

namespace ark::es2panda::ir {
class AstNode;
class Statement;
class ClassDefinition;
class MethodDefinition;
class DoWhileStatement;
class VariableDeclaration;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::checker {
class AliveAnalyzer : public BaseAnalyzer<PendingExit> {
public:
    // NOLINTNEXTLINE(readability-redundant-member-init)
    AliveAnalyzer(const ir::AstNode *node, ETSChecker *checker) : BaseAnalyzer<PendingExit>(), checker_(checker)
    {
        AnalyzeNodes(node);
    }

    void MarkDead() override
    {
        status_ = LivenessStatus::DEAD;
    }

    LivenessStatus Or(LivenessStatus left, LivenessStatus right)
    {
        return static_cast<LivenessStatus>(left | right);
    }

    LivenessStatus And(LivenessStatus left, LivenessStatus right)
    {
        return static_cast<LivenessStatus>(left & right);
    }

private:
    void AnalyzeNodes(const ir::AstNode *node);
    void AnalyzeNode(const ir::AstNode *node);
    void AnalyzeNodeHelper1(const ir::AstNode *node);
    void AnalyzeNodeHelper2(const ir::AstNode *node);
    void AnalyzeDef(const ir::AstNode *node);
    void AnalyzeStat(const ir::AstNode *node);
    void AnalyzeStats(const ArenaVector<ir::Statement *> &stats);
    void AnalyzeStructDecl(const ir::ETSStructDeclaration *structDecl);
    void AnalyzeClassDecl(const ir::ClassDeclaration *classDecl);
    void AnalyzeMethodDef(const ir::MethodDefinition *methodDef);
    void AnalyzeVarDef(const ir::VariableDeclaration *varDef);
    void AnalyzeDoLoop(const ir::DoWhileStatement *doWhile);
    void AnalyzeWhileLoop(const ir::WhileStatement *whileStmt);
    void AnalyzeForLoop(const ir::ForUpdateStatement *forStmt);
    void AnalyzeForOfLoop(const ir::ForOfStatement *forOfStmt);
    void AnalyzeIf(const ir::IfStatement *ifStmt);
    void AnalyzeLabelled(const ir::LabelledStatement *labelledStmt);
    void AnalyzeNewClass(const ir::ETSNewClassInstanceExpression *newClass);
    void AnalyzeCall(const ir::CallExpression *callExpr);
    void AnalyzeThrow(const ir::ThrowStatement *throwStmt);
    void AnalyzeSwitch(const ir::SwitchStatement *switchStmt);
    void AnalyzeTry(const ir::TryStatement *tryStmt);
    void AnalyzeBreak(const ir::BreakStatement *breakStmt);
    void AnalyzeContinue(const ir::ContinueStatement *contStmt);
    void AnalyzeReturn(const ir::ReturnStatement *retStmt);

    ETSChecker *checker_;
    LivenessStatus status_ {LivenessStatus::ALIVE};
};
}  // namespace ark::es2panda::checker

#endif
