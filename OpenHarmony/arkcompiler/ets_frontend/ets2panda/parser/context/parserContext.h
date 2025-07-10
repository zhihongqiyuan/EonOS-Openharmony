/**
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_PARSER_CORE_PARSER_PRIVATE_CONTEXT_H
#define ES2PANDA_PARSER_CORE_PARSER_PRIVATE_CONTEXT_H

#include "util/enumbitops.h"
#include "util/language.h"
#include "util/ustring.h"

namespace ark::es2panda::parser {
class Program;

using ENUMBITOPS_OPERATORS;

enum class ParserStatus : uint64_t {
    NO_OPTS = 0U,
    DIRECT_EVAL = 1U << 0U,

    FUNCTION = 1U << 1U,
    ARROW_FUNCTION = 1U << 2U,
    GENERATOR_FUNCTION = 1U << 3U,
    ASYNC_FUNCTION = 1U << 4U,
    CONSTRUCTOR_FUNCTION = 1U << 5U,
    FUNCTION_PARAM = 1U << 6U,
    IS_SPREAD = 1U << 7U,
    ACCESSOR_FUNCTION = 1U << 8U,
    FUNCTION_DECLARATION = 1U << 9U,

    ALLOW_SUPER = 1U << 10U,
    ALLOW_SUPER_CALL = 1U << 11U,

    IN_ITERATION = 1U << 14U,
    IN_LABELED = 1U << 15U,

    EXPORT_DEFAULT_REACHED = 1U << 16U,
    HAS_COMPLEX_PARAM = 1U << 17U,
    IN_SWITCH = 1U << 18U,

    MODULE = 1U << 19U,
    ALLOW_NEW_TARGET = 1U << 20U,

    IN_EXTENDS = 1U << 21U,
    ALLOW_THIS_TYPE = 1U << 22U,
    IN_METHOD_DEFINITION = 1U << 23U,
    IN_AMBIENT_CONTEXT = 1U << 24U,
    IN_CLASS_BODY = 1U << 25U,
    NEED_RETURN_TYPE = 1U << 26U,

    IN_DEFAULT_IMPORTS = 1U << 29U,
    IN_EXTENSION_FUNCTION = 1U << 30U,
    FUNCTION_HAS_RETURN_STATEMENT = 1U << 31U,
    IN_NAMESPACE = 1ULL << 32ULL,

    ALLOW_DEFAULT_VALUE = 1ULL << 33ULL,
    FUNCTION_HAS_THROW_STATEMENT = 1ULL << 34ULL,
};

}  // namespace ark::es2panda::parser

template <>
struct enumbitops::IsAllowedType<ark::es2panda::parser::ParserStatus> : std::true_type {
};

namespace ark::es2panda::parser {

class ParserContext {
public:
    // NOLINTNEXTLINE(modernize-avoid-c-arrays)
    inline static constexpr char const DEFAULT_SOURCE_FILE[] = "<auxiliary_tmp>.sts";

    explicit ParserContext(const Program *program, ParserStatus status);

    explicit ParserContext(ParserContext *current, ParserStatus newStatus, util::StringView label = "")
        : program_(current->program_), prev_(current), label_(label), lang_(current->lang_)
    {
        ParserStatus currentStatus = current->status_;
        currentStatus &= (ParserStatus::MODULE | ParserStatus::ALLOW_NEW_TARGET | ParserStatus::IN_EXTENDS |
                          ParserStatus::ALLOW_THIS_TYPE | ParserStatus::IN_CLASS_BODY | ParserStatus::FUNCTION |
                          ParserStatus::IN_AMBIENT_CONTEXT);
        status_ = currentStatus | newStatus;
    }

    DEFAULT_COPY_SEMANTIC(ParserContext);
    DEFAULT_MOVE_SEMANTIC(ParserContext);
    ~ParserContext() = default;
    ParserContext() = delete;

    [[nodiscard]] const Program *GetProgram() const noexcept
    {
        return program_;
    }

    void SetProgram(Program *program) noexcept
    {
        program_ = program;
    }

    [[nodiscard]] Language GetLanguage() const noexcept
    {
        return lang_;
    }

    Language SetLanguage(Language lang) noexcept
    {
        auto res = lang_;
        lang_ = lang;
        return res;
    }

    [[nodiscard]] ParserContext *Prev() const noexcept
    {
        return prev_;
    }

    [[nodiscard]] const ParserStatus &Status() const noexcept
    {
        return status_;
    }

    [[nodiscard]] ParserStatus &Status() noexcept
    {
        return status_;
    }

    [[nodiscard]] bool IsGenerator() const noexcept
    {
        return (status_ & ParserStatus::GENERATOR_FUNCTION) != 0;
    }

    [[nodiscard]] bool IsFunctionOrParam() const noexcept
    {
        return (status_ & (ParserStatus::FUNCTION | ParserStatus::FUNCTION_PARAM)) != 0;
    }

    [[nodiscard]] bool IsAsync() const noexcept
    {
        return (status_ & ParserStatus::ASYNC_FUNCTION) != 0;
    }

    [[nodiscard]] bool IsModule() const noexcept
    {
        return (status_ & ParserStatus::MODULE) != 0;
    }

    [[nodiscard]] bool IsDynamic() const noexcept
    {
        return lang_.IsDynamic();
    }

    const ParserContext *FindLabel(const util::StringView &label) const;

    [[nodiscard]] std::string_view FormattingFileName() const noexcept
    {
        return formattingFileName_;
    }

    template <typename T>
    void SetFormattingFileName(T &&fileName)
    {
        formattingFileName_ = std::string_view {std::forward<T>(fileName)};
    }

private:
    const Program *program_;
    ParserContext *prev_ {};
    ParserStatus status_ {};
    util::StringView label_ {};
    std::string_view formattingFileName_ {DEFAULT_SOURCE_FILE};
    Language lang_;
};
}  // namespace ark::es2panda::parser

#endif
