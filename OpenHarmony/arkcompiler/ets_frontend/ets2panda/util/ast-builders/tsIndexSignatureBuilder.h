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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_INDEX_SIGNATURE_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_INDEX_SIGNATURE_BUILDER

#include "ir/base/tsIndexSignature.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSIndexSignatureBuilder : public AstBuilder<TSIndexSignature> {
public:
    explicit TSIndexSignatureBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSIndexSignatureBuilder &SetParam(Expression *param)
    {
        param_ = param;
        return *this;
    }

    TSIndexSignatureBuilder &SetTypeAnnotation(TypeNode *typeAnnotation)
    {
        typeAnnotation_ = typeAnnotation;
        return *this;
    }

    TSIndexSignatureBuilder &SetReadOnly(bool readonly)
    {
        readonly_ = readonly;
        return *this;
    }

    TSIndexSignature *Build()
    {
        auto node = AllocNode(param_, typeAnnotation_, readonly_);
        return node;
    }

private:
    Expression *param_ {};
    TypeNode *typeAnnotation_ {};
    bool readonly_ = false;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_INDEX_SIGNATURE_BUILDER