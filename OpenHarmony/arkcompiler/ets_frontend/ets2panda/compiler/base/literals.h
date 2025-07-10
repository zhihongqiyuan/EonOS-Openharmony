/*
 * Copyright (c) 2021 - 2023 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_COMPILER_BASE_LITERALS_H
#define ES2PANDA_COMPILER_BASE_LITERALS_H

#include "ir/expressions/literal.h"
#include "util/ustring.h"

#include <variant>

namespace ark::es2panda::ir {
class Literal;
class TaggedTemplateExpression;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::checker {
class TSChecker;
class Type;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::compiler {
constexpr auto INVALID_LITERAL_BUFFER_ID = std::numeric_limits<uint32_t>::max();

// must be kept in sync with ark::panda_file::LiteralTag
enum class LiteralTag {
    INVALID,
    TAGVALUE,
    BOOLEAN,
    INTEGER,
    FLOAT,
    DOUBLE,
    STRING,
    NULL_VALUE,
    UNDEFINED_VALUE,
    ACCESSOR,
    METHOD,
    GENERATOR_METHOD,
    ASYNC_METHOD,
    ASYNC_GENERATOR_METHOD,
};

class Literal {
public:
    explicit Literal() = default;

    explicit Literal(LiteralTag tag, const util::StringView &str) : tag_(tag), value_(str.Mutf8()) {}
    explicit Literal(const util::StringView &str) : tag_(LiteralTag::STRING), value_(str.Mutf8()) {}
    explicit Literal(uint32_t number) : tag_(LiteralTag::INTEGER), value_(number) {}
    explicit Literal(double number) : tag_(LiteralTag::DOUBLE), value_(number) {}
    explicit Literal(bool boolean) : tag_(LiteralTag::BOOLEAN), value_(static_cast<uint32_t>(boolean)) {}

    DEFAULT_COPY_SEMANTIC(Literal);
    DEFAULT_MOVE_SEMANTIC(Literal);
    ~Literal() = default;

    static Literal NullLiteral()
    {
        return Literal(LiteralTag::NULL_VALUE);
    }

    static Literal UndefinedLiteral()
    {
        return Literal(LiteralTag::UNDEFINED_VALUE);
    }

    static Literal AccessorLiteral()
    {
        return Literal(LiteralTag::ACCESSOR);
    }

    LiteralTag Tag() const
    {
        return tag_;
    }

    bool IsTagMethod() const
    {
        return tag_ >= LiteralTag::METHOD;
    }

    const std::string &GetString() const
    {
        return std::get<std::string>(value_);
    }

    uint32_t GetInteger() const
    {
        return std::get<uint32_t>(value_);
    }

    double GetDouble() const
    {
        return std::get<double>(value_);
    }

    bool GetBoolean() const
    {
        return static_cast<bool>(std::get<uint32_t>(value_));
    }

    std::string GetMethod() const
    {
        ASSERT(tag_ == LiteralTag::ACCESSOR || tag_ == LiteralTag::METHOD || tag_ == LiteralTag::GENERATOR_METHOD ||
               tag_ == LiteralTag::ASYNC_METHOD || tag_ == LiteralTag::ASYNC_GENERATOR_METHOD);

        return std::get<std::string>(value_);
    }

    bool IsInvalid() const
    {
        return tag_ == LiteralTag::INVALID;
    }

private:
    explicit Literal(LiteralTag tag) : tag_(tag) {}

    LiteralTag tag_ {LiteralTag::INVALID};
    std::variant<std::string, double, uint32_t> value_ {static_cast<uint32_t>(0)};
};

class PandaGen;

using LiteralBuffer = std::vector<Literal>;

class Literals {
public:
    Literals() = delete;

    static void GetTemplateObject(PandaGen *pg, const ir::TaggedTemplateExpression *lit);
};
}  // namespace ark::es2panda::compiler

#endif
