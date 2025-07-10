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

#ifndef ES2PANDA_UTIL_INCLUDE_TS_SIGNATURE_DECLARATION_BUILDER
#define ES2PANDA_UTIL_INCLUDE_TS_SIGNATURE_DECLARATION_BUILDER

#include "ir/base/tsSignatureDeclaration.h"
#include "mem/arena_allocator.h"
#include "astBuilder.h"

namespace ark::es2panda::ir {

class TSSignatureDeclarationBuilder : public AstBuilder<TSSignatureDeclaration> {
public:
    explicit TSSignatureDeclarationBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    TSSignatureDeclarationBuilder &SetFunctionSignature(ir::FunctionSignature *signature)
    {
        signature_ = signature;
        return *this;
    }

    TSSignatureDeclarationBuilder &SetKind(TSSignatureDeclaration::TSSignatureDeclarationKind kind)
    {
        kind_ = kind;
        return *this;
    }

    TSSignatureDeclaration *Build()
    {
        auto node = AllocNode(kind_, std::move(*signature_));
        return node;
    }

private:
    ir::FunctionSignature *signature_ {};
    TSSignatureDeclaration::TSSignatureDeclarationKind kind_ =
        TSSignatureDeclaration::TSSignatureDeclarationKind::CALL_SIGNATURE;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_TS_SIGNATURE_DECLARATION_BUILDER