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

#ifndef ES2PANDA_LEXER_TOKEN_SOURCE_LOCATION_H
#define ES2PANDA_LEXER_TOKEN_SOURCE_LOCATION_H

#include "macros.h"
#include "util/ustring.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace ark::es2panda::lexer {
class SourcePosition {
public:
    explicit SourcePosition() noexcept = default;
    explicit SourcePosition(size_t i, size_t l) noexcept : index(i), line(l) {}
    DEFAULT_COPY_SEMANTIC(SourcePosition);
    DEFAULT_MOVE_SEMANTIC(SourcePosition);
    ~SourcePosition() = default;

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    size_t index {};
    size_t line {};
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

class SourceRange {
public:
    explicit SourceRange() noexcept = default;
    SourceRange(SourcePosition s, SourcePosition e) noexcept : start(s), end(e) {}
    DEFAULT_COPY_SEMANTIC(SourceRange);
    DEFAULT_MOVE_SEMANTIC(SourceRange);
    ~SourceRange() = default;

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    SourcePosition start {};
    SourcePosition end {};
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

class SourceLocation {
public:
    explicit SourceLocation() noexcept = default;
    explicit SourceLocation(size_t l, size_t c) noexcept : line(l), col(c) {}
    DEFAULT_COPY_SEMANTIC(SourceLocation);
    DEFAULT_MOVE_SEMANTIC(SourceLocation);
    ~SourceLocation() = default;

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    size_t line {};
    size_t col {};
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

class Range {
public:
    explicit Range(size_t bS) noexcept : byteSize(bS) {}

    DEFAULT_COPY_SEMANTIC(Range);
    DEFAULT_MOVE_SEMANTIC(Range);
    ~Range() = default;

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    size_t byteSize {};
    size_t cnt {1};
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

class OffsetEntry {
public:
    explicit OffsetEntry(size_t l) : lineStart(l), offset_(l) {};

    DEFAULT_COPY_SEMANTIC(OffsetEntry);
    DEFAULT_MOVE_SEMANTIC(OffsetEntry);
    ~OffsetEntry() = default;

    void AddCol(size_t offset);

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    std::vector<Range> ranges {};
    size_t lineStart {};
    // NOLINTEND(misc-non-private-member-variables-in-classes)

private:
    size_t offset_ {};
};

class LineIndex {
public:
    explicit LineIndex(const util::StringView &source) noexcept;
    NO_COPY_SEMANTIC(LineIndex);
    NO_MOVE_SEMANTIC(LineIndex);
    ~LineIndex() = default;

    SourceLocation GetLocation(SourcePosition pos) const noexcept;

private:
    std::vector<OffsetEntry> entries_;
};
}  // namespace ark::es2panda::lexer

#endif
