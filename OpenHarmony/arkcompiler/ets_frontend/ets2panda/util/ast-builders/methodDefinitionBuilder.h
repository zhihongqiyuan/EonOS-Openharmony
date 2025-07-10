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

#ifndef ES2PANDA_UTIL_INCLUDE_METHOD_DEFINITION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_METHOD_DEFINITION_BUILDER

#include "mem/arena_allocator.h"
#include "astBuilder.h"
#include "ir/base/methodDefinition.h"

namespace ark::es2panda::ir {

class MethodDefinitionBuilder : public AstBuilder<ir::MethodDefinition> {
public:
    explicit MethodDefinitionBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    MethodDefinitionBuilder &SetMethodDefinitionKind(MethodDefinitionKind const kind)
    {
        kind_ = kind;
        return *this;
    }

    MethodDefinitionBuilder &SetKey(Expression *const key)
    {
        key_ = key;
        return *this;
    }

    MethodDefinitionBuilder &SetValue(Expression *const value)
    {
        value_ = value;
        return *this;
    }

    MethodDefinitionBuilder &SetModifierFlags(ModifierFlags const flags)
    {
        flags_ = flags;
        return *this;
    }

    MethodDefinition *Build()
    {
        auto *node = AllocNode(kind_, key_, value_, flags_, Allocator(), false);
        return node;
    }

private:
    MethodDefinitionKind kind_ = MethodDefinitionKind::NONE;
    Expression *key_ {};
    Expression *value_ {};
    ModifierFlags flags_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_METHOD_DEFINITION_BUILDER