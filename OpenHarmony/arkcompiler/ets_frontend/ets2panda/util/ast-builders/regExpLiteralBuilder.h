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

#ifndef ES2PANDA_UTIL_INCLUDE_REG_EXP_LITERAL_BUILDER
#define ES2PANDA_UTIL_INCLUDE_REG_EXP_LITERAL_BUILDER

#include "mem/arena_allocator.h"
#include "astBuilder.h"
#include "ir/expressions/literals/regExpLiteral.h"

namespace ark::es2panda::ir {

class RegExpLiteralBuilder : public AstBuilder<ir::RegExpLiteral> {
public:
    explicit RegExpLiteralBuilder(ark::ArenaAllocator *allocator) : AstBuilder(allocator) {}

    RegExpLiteralBuilder &SetPattern(util::StringView pattern)
    {
        pattern_ = pattern;
        return *this;
    }

    RegExpLiteralBuilder &SetFlags(lexer::RegExpFlags flags)
    {
        flags_ = flags;
        return *this;
    }

    RegExpLiteralBuilder &SetFlagsString(util::StringView flagsStr)
    {
        flagsStr_ = flagsStr;
        return *this;
    }

    ir::RegExpLiteral *Build()
    {
        auto *node = AllocNode(pattern_, flags_, flagsStr_);
        return node;
    }

private:
    util::StringView pattern_;
    lexer::RegExpFlags flags_ = lexer::RegExpFlags::EMPTY;
    util::StringView flagsStr_;
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_REG_EXP_LITERAL_BUILDER