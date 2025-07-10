/*
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

#ifndef ES2PANDA_LEXER_TOKEN_NUMBER_H
#define ES2PANDA_LEXER_TOKEN_NUMBER_H

#include "util/ustring.h"
#include "util/enumbitops.h"

namespace ark::es2panda::lexer {

using ENUMBITOPS_OPERATORS;

enum class NumberFlags : uint32_t {
    NONE,
    BIGINT = 1U << 0U,
    DECIMAL_POINT = 1U << 1U,
    EXPONENT = 1U << 2U,
    ERROR = 1U << 3U,
};

}  // namespace ark::es2panda::lexer

template <>
struct enumbitops::IsAllowedType<ark::es2panda::lexer::NumberFlags> : std::true_type {
};

namespace ark::es2panda::lexer {

// NOLINTBEGIN(readability-identifier-naming)
// NOLINTBEGIN(fuchsia-multiple-inheritance)
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
// NOLINTEND(fuchsia-multiple-inheritance)

template <typename>
inline constexpr bool dependent_false_v = false;
// NOLINTEND(readability-identifier-naming)

// NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
class Number {
public:
    // NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
    explicit Number() noexcept : num_(static_cast<int32_t>(0)) {};
    explicit Number(util::StringView str) noexcept : str_(str) {}
    // NOLINTNEXTLINE(bugprone-exception-escape)
    explicit Number(util::StringView str, NumberFlags flags) noexcept;
    explicit Number(util::StringView str, double num) noexcept : str_(str), num_(num) {}
    explicit Number(uint32_t num) noexcept : Number(static_cast<int32_t>(num)) {}
    explicit Number(int32_t num) noexcept : num_(num) {}
    explicit Number(uint64_t num) noexcept : Number(static_cast<int64_t>(num)) {}
    explicit Number(int64_t num) noexcept : num_(num) {}
    explicit Number(float num) noexcept : num_(num) {}
    explicit Number(double num) noexcept : num_(num) {}
    DEFAULT_COPY_SEMANTIC(Number);
    DEFAULT_MOVE_SEMANTIC(Number);
    ~Number() = default;
    // NOLINTEND(cppcoreguidelines-pro-type-member-init)

    bool IsInt() const noexcept
    {
        return std::holds_alternative<int32_t>(num_);
    }

    bool IsLong() const noexcept
    {
        return std::holds_alternative<int64_t>(num_);
    }

    bool IsInteger() const noexcept
    {
        return IsInt() || IsLong();
    }

    bool IsFloat() const noexcept
    {
        return std::holds_alternative<float>(num_);
    }

    bool IsDouble() const noexcept
    {
        return std::holds_alternative<double>(num_);
    }

    bool IsReal() const noexcept
    {
        return IsFloat() || IsDouble();
    }

    bool ConversionError() const
    {
        return (flags_ & NumberFlags::ERROR) != 0;
    }

    int32_t GetInt() const
    {
        ASSERT(IsInt());
        return std::get<int32_t>(num_);
    }

    int64_t GetLong() const
    {
        return std::visit(overloaded {[](int64_t value) { return value; },
                                      [](int32_t value) { return static_cast<int64_t>(value); },
                                      []([[maybe_unused]] auto value) {
                                          ASSERT(false);
                                          return static_cast<int64_t>(0);
                                      }},
                          num_);
    }

    float GetFloat() const
    {
        ASSERT(IsFloat());
        return std::get<float>(num_);
    }

    double GetDouble() const
    {
        return std::visit(
            overloaded {[](double value) { return value; }, [](auto value) { return static_cast<double>(value); }},
            num_);
    }

    const util::StringView &Str() const
    {
        return str_;
    }

    void Negate()
    {
        std::visit(overloaded {[](auto &value) { value = -value; }}, num_);
        if (std::holds_alternative<int64_t>(num_)) {
            int64_t num = std::get<int64_t>(num_);
            if (num == INT32_MIN) {
                SetValue<int32_t>(num);
            }
        }
    }

    template <typename RT>
    bool CanGetValue() const noexcept
    {
        using T = typename std::remove_cv_t<typename std::remove_reference_t<RT>>;

        if constexpr (std::is_same_v<T, int64_t>) {
            return IsInteger();
        } else if constexpr (std::is_same_v<T, int32_t>) {
            return IsInt();
        } else if constexpr (std::is_same_v<T, double>) {
            return true;
        } else if constexpr (std::is_same_v<T, float>) {
            return IsFloat();
        } else {
            return false;
        }
    }

    template <typename RT>
    auto GetValue() const
    {
        using T = typename std::remove_cv_t<typename std::remove_reference_t<RT>>;

        if constexpr (std::is_same_v<T, int64_t>) {
            return GetLong();
        } else if constexpr (std::is_same_v<T, int32_t>) {
            return GetInt();
        } else if constexpr (std::is_same_v<T, double>) {
            return GetDouble();
        } else if constexpr (std::is_same_v<T, float>) {
            return GetFloat();
        } else {
            static_assert(dependent_false_v<T>, "Invalid value type was requested for Number.");
        }
    }

    template <typename RT>
    void SetValue(RT &&value)
    {
        using T = typename std::remove_cv_t<typename std::remove_reference_t<RT>>;

        if constexpr (std::is_same_v<T, int64_t> || std::is_same_v<T, int32_t> || std::is_same_v<T, double> ||
                      std::is_same_v<T, float>) {
            num_ = std::forward<RT>(value);
        } else {
            static_assert(dependent_false_v<T>, "Invalid value type was requested for Number.");
        }
    }

private:
    util::StringView str_ {};
    std::variant<int32_t, int64_t, float, double> num_;
    NumberFlags flags_ {NumberFlags::NONE};
};
}  // namespace ark::es2panda::lexer

#endif
