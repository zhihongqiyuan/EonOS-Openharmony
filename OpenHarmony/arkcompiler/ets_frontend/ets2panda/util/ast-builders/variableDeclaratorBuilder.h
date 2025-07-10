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

#ifndef ES2PANDA_UTIL_INCLUDE_VARIABLE_DECLARATOR_BUILDER
#define ES2PANDA_UTIL_INCLUDE_VARIABLE_DECLARATOR_BUILDER

#include "ir/statements/variableDeclarator.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class VariableDeclaratorBuilder : public AstBuilder<ir::VariableDeclarator> {
public:
    explicit VariableDeclaratorBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    VariableDeclaratorBuilder &SetId(Expression *id)
    {
        id_ = id;
        return *this;
    }

    VariableDeclaratorBuilder &SetInit(Expression *init)
    {
        init_ = init;
        return *this;
    }

    VariableDeclarator *Build()
    {
        auto *node = AllocNode(flag_, id_, init_);
        return node;
    }

private:
    ark::es2panda::ir::VariableDeclaratorFlag flag_ = ark::es2panda::ir::VariableDeclaratorFlag::LET;
    Expression *id_ {};
    Expression *init_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_VARIABLE_DECLARATOR_BUILDER