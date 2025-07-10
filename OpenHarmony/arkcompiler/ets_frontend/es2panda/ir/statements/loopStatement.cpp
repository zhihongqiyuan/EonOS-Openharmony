/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "loopStatement.h"

#include "ir/statements/blockStatement.h"

namespace panda::es2panda::ir {

Statement *LoopStatement::UpdateChildStatement(const NodeUpdater &cb,
                                               const binder::Binder *binder,
                                               Statement *statement) const
{
    auto newStatement = cb(statement);
    if (std::holds_alternative<ir::AstNode *>(newStatement)) {
        return std::get<ir::AstNode *>(newStatement)->AsStatement();
    }

    ASSERT(std::holds_alternative<std::vector<ir::AstNode *>>(newStatement));
    ArenaVector<ir::Statement *> statements(binder->Allocator()->Adapter());
    auto newStatements = std::get<std::vector<ir::AstNode *>>(newStatement);
    for (auto *it : newStatements) {
        statements.push_back(it->AsStatement());
    }
    return binder->Allocator()->New<ir::BlockStatement>(nullptr, std::move(statements));
}

}  // namespace panda::es2panda::ir
