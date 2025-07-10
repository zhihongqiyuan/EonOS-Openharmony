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

#include "checkContext.h"

namespace ark::es2panda::compiler::ast_verifier {

void CheckContext::AddCheckMessage(const std::string &cause, const ir::AstNode &node, const lexer::SourcePosition &from)
{
    const auto loc = from.line;
    const auto &&dump = node.DumpJSON();
    messages_.emplace_back(checkName_, cause.data(), dump.data(), loc);
}

void CheckContext::SetCheckName(util::StringView checkName)
{
    checkName_ = checkName;
}

Messages CheckContext::GetMessages()
{
    return messages_;
}

}  // namespace ark::es2panda::compiler::ast_verifier
