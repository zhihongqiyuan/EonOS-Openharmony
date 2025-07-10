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

#ifndef ES2PANDA_UTIL_INCLUDE_FOR_OF_STATEMENT_BUILDER
#define ES2PANDA_UTIL_INCLUDE_FOR_OF_STATEMENT_BUILDER

#include "mem/arena_allocator.h"
#include "astBuilder.h"
#include "ir/statements/forOfStatement.h"

namespace ark::es2panda::ir {

class ForOfStatementBuilder : public AstBuilder<ir::ForOfStatement> {
public:
    explicit ForOfStatementBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    ForOfStatementBuilder &SetLeft(AstNode *left)
    {
        left_ = left;
        return *this;
    }

    ForOfStatementBuilder &SetRight(Expression *right)
    {
        right_ = right;
        return *this;
    }

    ForOfStatementBuilder &SetBody(Statement *body)
    {
        body_ = body;
        return *this;
    }

    ForOfStatementBuilder &SetIsAwait(bool isAwait)
    {
        isAwait_ = isAwait;
        return *this;
    }

    ForOfStatement *Build()
    {
        auto *node = AllocNode(left_, right_, body_, isAwait_);
        return node;
    }

private:
    AstNode *left_ {};
    Expression *right_ {};
    Statement *body_ {};
    bool isAwait_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_FOR_OF_STATEMENT_BUILDER