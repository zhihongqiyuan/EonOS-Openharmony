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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_METHOD_SIGNATURE_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_METHOD_SIGNATURE_BUILDER

#include "ir/base/tsMethodSignature.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSMethodSignatureBuilder : public AstBuilder<TSMethodSignature> {
public:
    explicit TSMethodSignatureBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSMethodSignatureBuilder &SetFunctionSignature(ir::FunctionSignature *signature)
    {
        signature_ = signature;
        return *this;
    }

    TSMethodSignatureBuilder &SetKey(Expression *key)
    {
        key_ = key;
        return *this;
    }

    TSMethodSignatureBuilder &SetComputed(bool computed)
    {
        computed_ = computed;
        return *this;
    }

    TSMethodSignatureBuilder &SetOptional(bool optional)
    {
        optional_ = optional;
        return *this;
    }

    TSMethodSignature *Build()
    {
        auto node = AllocNode(key_, std::move(*signature_), computed_, optional_);
        return node;
    }

private:
    ir::FunctionSignature *signature_ {};
    Expression *key_ {};
    bool computed_ = false;
    bool optional_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_METHOD_SIGNATURE_BUILDER