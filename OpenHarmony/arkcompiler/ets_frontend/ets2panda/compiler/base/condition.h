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

#ifndef ES2PANDA_COMPILER_BASE_CONDITION_H
#define ES2PANDA_COMPILER_BASE_CONDITION_H

#include "ir/expression.h"

namespace ark::es2panda::compiler {
class PandaGen;
class ETSGen;
class Label;

class Condition {
public:
    Condition() = delete;

    enum class Result {
        CONST_TRUE,
        CONST_FALSE,
        UNKNOWN,
    };

    static void Compile(PandaGen *pg, const ir::Expression *expr, Label *falseLabel);
    static void Compile(ETSGen *etsg, const ir::Expression *expr, Label *falseLabel);
    static Result CheckConstantExpr(ETSGen *etsg, const ir::Expression *expr);

private:
    static bool CompileBinaryExpr(PandaGen *pg, const ir::BinaryExpression *binExpr, Label *falseLabel);
    static bool CompileBinaryExpr(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel);
    static void CompileLogicalAndExpr(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel);
    static void CompileLogicalOrExpr(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel);
    static bool CompileBinaryExprForBigInt(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel);
    static void CompileInstanceofExpr(ETSGen *etsg, const ir::BinaryExpression *binExpr, Label *falseLabel);
};
}  // namespace ark::es2panda::compiler

#endif
