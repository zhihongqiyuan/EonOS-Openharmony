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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_PROPERTY_SIGNATURE_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_PROPERTY_SIGNATURE_BUILDER

#include "ir/base/tsPropertySignature.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSPropertySignatureBuilder : public AstBuilder<TSPropertySignature> {
public:
    explicit TSPropertySignatureBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSPropertySignatureBuilder &SetTypeAnnotation(TypeNode *typeAnnotation)
    {
        typeAnnotation_ = typeAnnotation;
        return *this;
    }

    TSPropertySignatureBuilder &SetKey(Expression *key)
    {
        key_ = key;
        return *this;
    }

    TSPropertySignatureBuilder &SetComputed(bool computed)
    {
        computed_ = computed;
        return *this;
    }

    TSPropertySignatureBuilder &SetOptional(bool optional)
    {
        optional_ = optional;
        return *this;
    }

    TSPropertySignatureBuilder &SetReadOnly(bool readonly)
    {
        readonly_ = readonly;
        return *this;
    }

    TSPropertySignature *Build()
    {
        auto node = AllocNode(key_, typeAnnotation_, computed_, optional_, readonly_);
        return node;
    }

private:
    TypeNode *typeAnnotation_ {};
    Expression *key_ {};
    bool computed_ = false;
    bool optional_ = false;
    bool readonly_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_PROPERTY_SIGNATURE_BUILDER