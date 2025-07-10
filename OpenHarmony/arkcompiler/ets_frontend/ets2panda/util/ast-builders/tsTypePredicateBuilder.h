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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_TYPE_PREDICATE_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_TYPE_PREDICATE_BUILDER

#include "ir/ts/tsTypePredicate.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSTypePredicateBuilder : public AstBuilder<TSTypePredicate> {
public:
    explicit TSTypePredicateBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSTypePredicateBuilder &SetParameterName(Expression *parameterName)
    {
        parameterName_ = parameterName;
        return *this;
    }

    TSTypePredicateBuilder &SetTypeAnnotation(TypeNode *typeAnnotation)
    {
        typeAnnotation_ = typeAnnotation;
        return *this;
    }

    TSTypePredicateBuilder &SetAsserts(bool asserts)
    {
        asserts_ = asserts;
        return *this;
    }

    TSTypePredicate *Build()
    {
        auto node = AllocNode(parameterName_, typeAnnotation_, asserts_);
        return node;
    }

private:
    Expression *parameterName_ {};
    TypeNode *typeAnnotation_ {};
    bool asserts_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_TYPE_PREDICATE_BUILDER