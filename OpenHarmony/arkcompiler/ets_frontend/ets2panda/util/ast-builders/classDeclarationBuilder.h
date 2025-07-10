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

#ifndef ES2PANDA_UTIL_INCLUDE_CLASS_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_CLASS_DECLARATION_BUILDER

#include "ir/statements/classDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class ClassDeclarationBuilder : public AstBuilder<ir::ClassDeclaration> {
public:
    explicit ClassDeclarationBuilder(ark::ArenaAllocator *allocator)
        : AstBuilder(allocator), decorators_(Allocator()->Adapter())
    {
    }

    ClassDeclarationBuilder &SetDefinition(ClassDefinition *def)
    {
        def_ = def;
        return *this;
    }

    ClassDeclarationBuilder &SetDecorators(ArenaVector<Decorator *> &&decorators)
    {
        decorators_ = std::move(decorators);
        return *this;
    }

    ClassDeclaration *Build()
    {
        auto *node = AllocNode(def_, Allocator());
        node->AddDecorators(std::move(decorators_));
        return node;
    }

private:
    ClassDefinition *def_ = nullptr;
    ArenaVector<Decorator *> decorators_;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_CLASS_DECLARATION_BUILDER