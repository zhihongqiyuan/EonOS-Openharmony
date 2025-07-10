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

#ifndef ES2PANDA_UTIL_INCLUDE_TRY_STATEMENT_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TRY_STATEMENT_BUILDER

#include "mem/arena_allocator.h"
#include "astBuilder.h"
#include "ir/statements/tryStatement.h"

namespace ark::es2panda::ir {

class TryStatementBuilder : public AstBuilder<ir::TryStatement> {
public:
    explicit TryStatementBuilder(ark::ArenaAllocator *allocator)
        : AstBuilder(allocator), catchClauses_(allocator->Adapter()), finalizerInsertions_(allocator->Adapter())
    {
    }

    TryStatementBuilder &SetBlock(BlockStatement *block)
    {
        block_ = block;
        return *this;
    }

    TryStatementBuilder &SetFinalizer(BlockStatement *finalizer)
    {
        finalizer_ = finalizer;
        return *this;
    }

    TryStatementBuilder &AddCatchClause(CatchClause *catchClause)
    {
        catchClauses_.emplace_back(catchClause);
        return *this;
    }

    TryStatementBuilder &AddCatchClause(std::pair<compiler::LabelPair, const Statement *> finalizerInsertion)
    {
        finalizerInsertions_.emplace_back(finalizerInsertion);
        return *this;
    }

    TryStatement *Build()
    {
        auto *node = AllocNode(block_, std::move(catchClauses_), finalizer_, finalizerInsertions_);
        return node;
    }

private:
    BlockStatement *block_ {};
    ArenaVector<CatchClause *> catchClauses_;
    BlockStatement *finalizer_ {};
    ArenaVector<std::pair<compiler::LabelPair, const Statement *>> finalizerInsertions_;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TRY_STATEMENT_BUILDER