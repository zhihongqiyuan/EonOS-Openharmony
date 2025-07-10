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

#ifndef ES2PANDA_COMPILER_CORE_VREG_H
#define ES2PANDA_COMPILER_CORE_VREG_H

#include "macros.h"

#include <limits>

namespace ark::es2panda::compiler {
class VReg final {
public:
    using Index = uint32_t;
    explicit constexpr VReg() noexcept : VReg(INVALID_IDX) {}
    explicit constexpr VReg(const uint32_t idx) noexcept : idx_(idx) {}

    [[nodiscard]] constexpr Index GetIndex() const noexcept
    {
        return idx_;
    }

    constexpr void SetIndex(const Index idx) noexcept
    {
        idx_ = idx;
    }

    [[nodiscard]] static constexpr VReg Invalid() noexcept
    {
        return VReg(INVALID_IDX);
    }

    [[nodiscard]] static constexpr VReg RegStart() noexcept
    {
        return VReg(REG_START);
    }

    [[nodiscard]] constexpr bool IsInvalid() const noexcept
    {
        return idx_ == INVALID_IDX;
    }

    [[nodiscard]] constexpr bool IsValid(uint32_t limit) const noexcept
    {
        return (idx_ >= REG_MAX - limit) && (limit == REG_MAX || !IsParameter());
    }

    [[nodiscard]] constexpr bool IsParameter() const noexcept
    {
        return idx_ >= PARAM_START;
    }

    constexpr VReg &operator++() noexcept
    {
        idx_++;
        return *this;
    }

    // NOLINTNEXTLINE(readability-const-return-type)
    constexpr const VReg operator++(int) noexcept
    {
        const auto temp = *this;
        ++*this;
        return temp;
    }

    constexpr VReg &operator--() noexcept
    {
        idx_--;
        return *this;
    }

    // NOLINTNEXTLINE(readability-const-return-type)
    constexpr const VReg operator--(int) noexcept
    {
        const auto temp = *this;
        --*this;
        return temp;
    }

    [[nodiscard]] constexpr bool operator!=(const VReg other) const noexcept
    {
        return !(*this == other);
    }

    [[nodiscard]] constexpr bool operator==(const VReg other) const noexcept
    {
        return idx_ == other.idx_;
    }

    [[nodiscard]] constexpr bool operator!=(const Index idx) const noexcept
    {
        return !(*this == idx);
    }

    [[nodiscard]] constexpr bool operator==(const Index idx) const noexcept
    {
        return idx_ == idx;
    }

    [[nodiscard]] constexpr VReg operator+(const Index idx) const noexcept
    {
        return VReg(idx_ + idx);
    }

    [[nodiscard]] constexpr VReg operator+(const VReg other) const noexcept
    {
        return VReg(idx_ + other.idx_);
    }

    static constexpr auto REG_START = std::numeric_limits<std::uint16_t>::max();
    static constexpr auto REG_MAX = REG_START + 1U;
    static constexpr auto PARAM_START = REG_MAX;
    static constexpr auto MANDATORY_PARAM_NUM = 3U;

private:
    static constexpr auto INVALID_IDX = std::numeric_limits<Index>::max();

    Index idx_;
};
}  // namespace ark::es2panda::compiler

template <>
struct std::hash<ark::es2panda::compiler::VReg> {
    std::size_t operator()(const ark::es2panda::compiler::VReg &vreg) const
    {
        return std::hash<ark::es2panda::compiler::VReg::Index> {}(vreg.GetIndex());
    }
};

#endif
