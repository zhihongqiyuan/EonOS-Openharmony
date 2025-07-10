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

#ifndef ES2PANDA_PARSER_CORE_REGEXP_H
#define ES2PANDA_PARSER_CORE_REGEXP_H

#include "util/enumbitops.h"
#include "util/ustring.h"

#include "../../parser/parserImpl.h"

#include <unordered_set>

namespace ark::es2panda::lexer {

using ENUMBITOPS_OPERATORS;

enum class RegExpFlags : uint32_t {
    EMPTY = 0U,
    GLOBAL = 1U << 0U,
    IGNORE_CASE = 1U << 1U,
    MULTILINE = 1U << 2U,
    DOTALL = 1U << 3U,
    UNICODE = 1U << 4U,
    STICKY = 1U << 5U,
};

struct RegExp {
    RegExp(util::StringView p, util::StringView f, RegExpFlags reFlags);

    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    util::StringView patternStr;
    util::StringView flagsStr;
    RegExpFlags flags;
    // NOLINTEND(misc-non-private-member-variables-in-classes)
};

class RegExpParser {
public:
    explicit RegExpParser(const RegExp &re, ArenaAllocator *allocator, const parser::ParserImpl &parser);
    void ParsePattern();

private:
    void ParseDisjunction();
    void ParseAlternatives();
    void ParseAlternative();
    bool ParseAlternativeCharLeftParen();

    void ParseNonCapturingGroup();
    void ParseNamedCapturingGroup();
    void ParseCapturingGroup();

    void ParseAssertion();
    char32_t ParseClassAtomHelper(char32_t cp);
    char32_t ParseClassAtom();
    void ParseCharacterClass();
    void ParseAtomEscape();
    void ParseAtomEscapeSwitch(char32_t cp);

    uint32_t ParseControlEscape();
    uint32_t ParseDecimalEscape();
    uint32_t ParseLegacyOctalEscape();
    uint32_t ParseHexEscape();
    uint32_t ParseUnicodeDigits();
    uint32_t ParseUnicodeEscape();

    void ParseUnicodePropertyEscape();
    void ValidateNamedBackreference(bool isUnicode);
    void ValidateGroupNameElement(char32_t cp);
    void ParseNamedBackreference();

    void ParseQuantifier();
    bool ParseBracedQuantifier();

    bool IsSyntaxCharacter(char32_t cp) const;
    bool ParsePatternCharacter();

    util::StringView ParseIdent();

    bool Unicode() const;

    char32_t Peek() const;
    char32_t Next();
    void Advance();
    bool Eos() const;

    RegExp re_;
    ArenaAllocator *allocator_ {};
    util::StringView::Iterator iter_;
    uint32_t capturingGroupCount_ {};
    std::unordered_set<util::StringView> groupNames_;
    std::unordered_set<util::StringView> backReferences_;
    const es2panda::parser::ParserImpl &parser_;
};
}  // namespace ark::es2panda::lexer

template <>
struct enumbitops::IsAllowedType<ark::es2panda::lexer::RegExpFlags> : std::true_type {
};

#endif
