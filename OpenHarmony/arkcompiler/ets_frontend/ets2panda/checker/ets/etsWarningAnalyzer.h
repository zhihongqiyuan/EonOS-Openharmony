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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_ETS_WARNING_ANALYZER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_ETS_WARNING_ANALYZER_H

#include "checker/ETSchecker.h"

namespace ark::es2panda::checker {
class ETSWarningAnalyzer {
public:
    ETSWarningAnalyzer(const ir::AstNode *node, parser::Program *program, const ETSWarnings warning, bool etsWerror)
        : program_(program), etsWerror_(etsWerror)
    {
        if (node == nullptr) {
            return;
        }

        switch (warning) {
            case ETSWarnings::SUGGEST_FINAL:
                ETSWarningSuggestFinal(node);
                break;
            case ETSWarnings::PROHIBIT_TOP_LEVEL_STATEMENTS:
                ETSWarningsProhibitTopLevelStatements(node);
                break;
            case ETSWarnings::BOOST_EQUALITY_STATEMENT:
                ETSWarningBoostEqualityStatement(node);
                break;
            case ETSWarnings::REMOVE_ASYNC_FUNCTIONS:
                ETSWarningRemoveAsync(node);
                break;
            case ETSWarnings::REMOVE_LAMBDA:
                ETSWarningRemoveLambda(node);
                break;
            case ETSWarnings::IMPLICIT_BOXING_UNBOXING:
                ETSWarningImplicitBoxingUnboxing(node);
                break;
            default:
                break;
        }
    }

private:
    void ETSThrowWarning(const std::string &message, const lexer::SourcePosition &position);

    void AnalyzeClassDefForFinalModifier(const ir::ClassDefinition *classDef);
    void AnalyzeClassMethodForFinalModifier(const ir::MethodDefinition *methodDef, const ir::ClassDefinition *classDef);
    void CheckTypeOfBoxing(const ir::AstNode *node);
    void CheckTypeOfUnboxing(const ir::AstNode *node);
    void CheckTopLevelExpressions(const ir::Expression *expression);
    void CheckProhibitedTopLevelStatements(const ir::Statement *statement);
    std::string GetBoxingUnboxingType(const ir::AstNode *node);
    void CheckTypeOfBoxingUnboxing(const ir::AstNode *node);

    void ETSWarningSuggestFinal(const ir::AstNode *node);
    void ETSWarningsProhibitTopLevelStatements(const ir::AstNode *node);
    void ETSWarningBoostEqualityStatement(const ir::AstNode *node);
    void ETSWarningRemoveAsync(const ir::AstNode *node);
    void ETSWarningRemoveLambda(const ir::AstNode *node);
    void ETSWarningImplicitBoxingUnboxing(const ir::AstNode *node);

    parser::Program *program_;
    bool etsWerror_;
};
}  // namespace ark::es2panda::checker

#endif
