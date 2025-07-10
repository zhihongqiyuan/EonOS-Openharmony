/*
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

#ifndef ES2PANDA_COMPILER_CORE_AST_VERIFIER_CHECKCONTEXT_H
#define ES2PANDA_COMPILER_CORE_AST_VERIFIER_CHECKCONTEXT_H

#include "ir/astNode.h"
#include "utils/json_builder.h"

namespace ark::es2panda::compiler::ast_verifier {

enum class CheckDecision { CORRECT, INCORRECT };
enum class CheckAction { CONTINUE, SKIP_SUBTREE };
using CheckResult = std::tuple<CheckDecision, CheckAction>;

enum class CheckSeverity { ERROR, WARNING, UNKNOWN };
inline std::string CheckSeverityString(CheckSeverity value)
{
    switch (value) {
        case CheckSeverity::ERROR:
            return "error";
        case CheckSeverity::WARNING:
            return "warning";
        default:
            UNREACHABLE();
    }
}

class CheckMessage {
public:
    explicit CheckMessage(util::StringView name, util::StringView cause, util::StringView message, size_t line)
        : invariantName_ {name}, cause_ {cause}, message_ {message}, line_ {line}
    {
    }

    std::string Invariant() const
    {
        return invariantName_;
    }

    std::string Cause() const
    {
        return cause_;
    }

    std::function<void(JsonObjectBuilder &)> DumpJSON(CheckSeverity severity, const std::string &sourceName,
                                                      const std::string &phaseName) const
    {
        return [sourceName, phaseName, severity, this](JsonObjectBuilder &body) {
            body.AddProperty("severity", CheckSeverityString(severity));
            body.AddProperty("invariant", invariantName_);
            body.AddProperty("cause", cause_);
            body.AddProperty("ast", message_);
            body.AddProperty("line", line_ + 1);
            body.AddProperty("source", sourceName);
            body.AddProperty("phase", phaseName);
        };
    }

private:
    std::string invariantName_;
    std::string cause_;
    std::string message_;
    size_t line_;
};

using Messages = std::vector<CheckMessage>;

class CheckContext {
public:
    explicit CheckContext() : checkName_ {"Invalid"} {}

    void AddCheckMessage(const std::string &cause, const ir::AstNode &node, const lexer::SourcePosition &from);
    void SetCheckName(util::StringView checkName);
    Messages GetMessages();

private:
    Messages messages_;
    util::StringView checkName_;
};

}  // namespace ark::es2panda::compiler::ast_verifier

#endif  // ES2PANDA_COMPILER_CORE_AST_VERIFIER_CHECKCONTEXT_H
