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

#ifndef ES2PANDA_UTIL_INCLUDE_SWITCH_STATEMENT_BUILDER
#define ES2PANDA_UTIL_INCLUDE_SWITCH_STATEMENT_BUILDER

#include "ir/statements/switchStatement.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class SwitchStatementBuilder : public AstBuilder<ir::SwitchStatement> {
public:
    explicit SwitchStatementBuilder(ark::ArenaAllocator *allocator)
        : AstBuilder(allocator), cases_(allocator->Adapter())
    {
    }

    SwitchStatementBuilder &SetDiscriminant(Expression *discr)
    {
        discriminant_ = discr;
        return *this;
    }

    SwitchStatementBuilder &SetCases(ArenaVector<SwitchCaseStatement *> cases)
    {
        cases_ = std::move(cases);
        return *this;
    }

    SwitchStatementBuilder &AddCases(SwitchCaseStatement *cases)
    {
        cases_.emplace_back(cases);
        return *this;
    }

    SwitchStatement *Build()
    {
        auto node = AllocNode(discriminant_, std::move(cases_));
        return node;
    }

private:
    Expression *discriminant_ {};
    ArenaVector<SwitchCaseStatement *> cases_;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_SWITCH_STATEMENT_BUILDER