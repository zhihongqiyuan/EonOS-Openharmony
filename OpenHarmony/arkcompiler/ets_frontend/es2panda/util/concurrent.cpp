/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "concurrent.h"

#include <ir/base/scriptFunction.h>

namespace panda::es2panda::util {

void Concurrent::SetConcurrent(ir::ScriptFunction *func, const ir::AstNode * node, const lexer::LineIndex &lineIndex)
{
    // concurrent function should only be function declaration
    if (!func->CanBeConcurrent()) {
        ThrowInvalidConcurrentFunction(lineIndex, node, ConcurrentInvalidFlag::NOT_ORDINARY_FUNCTION);
    }

    func->AddFlag(ir::ScriptFunctionFlags::CONCURRENT);
}

void Concurrent::ThrowInvalidConcurrentFunction(const lexer::LineIndex &lineIndex, const ir::AstNode *expr,
                                                ConcurrentInvalidFlag errFlag, util::StringView varName)
{
    auto line = expr->Range().start.line;
    auto column = (const_cast<lexer::LineIndex &>(lineIndex)).GetLocation(expr->Range().start).col - 1;
    switch (errFlag) {
        case ConcurrentInvalidFlag::NOT_ORDINARY_FUNCTION: {
            throw Error {ErrorType::SYNTAX, "Concurrent function should only be function declaration", line,
                         column};
            break;
        }
        case ConcurrentInvalidFlag::NOT_IMPORT_VARIABLE: {
            std::stringstream ss;
            ss << "Concurrent function should only use import variable or local variable, '" << varName
               << "' is not one of them";
            throw Error {ErrorType::SYNTAX, ss.str(), line, column};
            break;
        }
        default:
            break;
    }
}

void Concurrent::ProcessConcurrent(const lexer::LineIndex &lineIndex, const ir::AstNode *node,
                                   const binder::ScopeFindResult &result, parser::Program *program)
{
    if (!result.concurrentFunc) {
        return;
    }

    if (result.variable->IsModuleVariable() && result.variable->Declaration()->IsImportDecl()) {
        CollectRelativeModule(result, program);
        return;
    }

    ThrowInvalidConcurrentFunction(lineIndex, node, ConcurrentInvalidFlag::NOT_IMPORT_VARIABLE,
                                   result.variable->Declaration()->Name());
}

void Concurrent::CollectRelativeModule(const binder::ScopeFindResult &result, parser::Program *program)
{
    int moduleRequestIdx = program->ModuleRecord()->GetModuleRequestIdx(result.variable->Name());
    if (moduleRequestIdx != parser::SourceTextModuleRecord::INVALID_MODULEREQUEST_ID) {
        result.concurrentFunc->AddConcurrentModuleRequest(moduleRequestIdx);
    }
}

} // namespace panda::es2panda::util