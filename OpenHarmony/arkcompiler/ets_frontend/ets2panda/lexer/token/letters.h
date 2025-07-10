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

#ifndef ES2PANDA_PARSER_CORE_LETTERS_H
#define ES2PANDA_PARSER_CORE_LETTERS_H

#include <cstdint>
namespace ark::es2panda::lexer {
inline constexpr char32_t LEX_ASCII_MAX_BITS = 128;

inline constexpr char32_t LEX_CHAR_NULL = 0x00; /* \0 */

// Line Terminators
inline constexpr char32_t LEX_CHAR_LF = 0x0A;   /* line feed */
inline constexpr char32_t LEX_CHAR_CR = 0x0D;   /* carriage return */
inline constexpr char32_t LEX_CHAR_LS = 0x2028; /* line separator */
inline constexpr char32_t LEX_CHAR_PS = 0x2029; /* paragraph separator */

inline constexpr char32_t LEX_CHAR_LOWERCASE_A = 0x61; /* a */
inline constexpr char32_t LEX_CHAR_LOWERCASE_B = 0x62; /* b */
inline constexpr char32_t LEX_CHAR_LOWERCASE_C = 0x63; /* c */
inline constexpr char32_t LEX_CHAR_LOWERCASE_D = 0x64; /* d */
inline constexpr char32_t LEX_CHAR_LOWERCASE_E = 0X65; /* e */
inline constexpr char32_t LEX_CHAR_LOWERCASE_F = 0X66; /* f */
inline constexpr char32_t LEX_CHAR_LOWERCASE_G = 0X67; /* g */
inline constexpr char32_t LEX_CHAR_LOWERCASE_H = 0X68; /* h */
inline constexpr char32_t LEX_CHAR_LOWERCASE_I = 0X69; /* i */
inline constexpr char32_t LEX_CHAR_LOWERCASE_J = 0X6A; /* j */
inline constexpr char32_t LEX_CHAR_LOWERCASE_K = 0X6B; /* k */
inline constexpr char32_t LEX_CHAR_LOWERCASE_L = 0X6C; /* l */
inline constexpr char32_t LEX_CHAR_LOWERCASE_M = 0X6D; /* m */
inline constexpr char32_t LEX_CHAR_LOWERCASE_N = 0X6E; /* n */
inline constexpr char32_t LEX_CHAR_LOWERCASE_O = 0X6F; /* o */
inline constexpr char32_t LEX_CHAR_LOWERCASE_P = 0x70; /* p */
inline constexpr char32_t LEX_CHAR_LOWERCASE_Q = 0x71; /* q */
inline constexpr char32_t LEX_CHAR_LOWERCASE_R = 0x72; /* r */
inline constexpr char32_t LEX_CHAR_LOWERCASE_S = 0x73; /* s */
inline constexpr char32_t LEX_CHAR_LOWERCASE_T = 0x74; /* t */
inline constexpr char32_t LEX_CHAR_LOWERCASE_U = 0x75; /* u */
inline constexpr char32_t LEX_CHAR_LOWERCASE_V = 0x76; /* v */
inline constexpr char32_t LEX_CHAR_LOWERCASE_W = 0x77; /* w */
inline constexpr char32_t LEX_CHAR_LOWERCASE_X = 0x78; /* x */
inline constexpr char32_t LEX_CHAR_LOWERCASE_Y = 0x79; /* y */
inline constexpr char32_t LEX_CHAR_LOWERCASE_Z = 0x7A; /* z */

inline constexpr char32_t LEX_CHAR_UPPERCASE_A = 0X41; /* A */
inline constexpr char32_t LEX_CHAR_UPPERCASE_B = 0X42; /* B */
inline constexpr char32_t LEX_CHAR_UPPERCASE_C = 0X43; /* C */
inline constexpr char32_t LEX_CHAR_UPPERCASE_D = 0X44; /* D */
inline constexpr char32_t LEX_CHAR_UPPERCASE_E = 0X45; /* E */
inline constexpr char32_t LEX_CHAR_UPPERCASE_F = 0X46; /* F */
inline constexpr char32_t LEX_CHAR_UPPERCASE_G = 0X47; /* G */
inline constexpr char32_t LEX_CHAR_UPPERCASE_H = 0X48; /* H */
inline constexpr char32_t LEX_CHAR_UPPERCASE_I = 0X49; /* I */
inline constexpr char32_t LEX_CHAR_UPPERCASE_J = 0X4A; /* J */
inline constexpr char32_t LEX_CHAR_UPPERCASE_K = 0X4B; /* K */
inline constexpr char32_t LEX_CHAR_UPPERCASE_L = 0X4C; /* L */
inline constexpr char32_t LEX_CHAR_UPPERCASE_M = 0X4D; /* M */
inline constexpr char32_t LEX_CHAR_UPPERCASE_N = 0X4E; /* N */
inline constexpr char32_t LEX_CHAR_UPPERCASE_O = 0X4F; /* O */
inline constexpr char32_t LEX_CHAR_UPPERCASE_P = 0X50; /* P */
inline constexpr char32_t LEX_CHAR_UPPERCASE_Q = 0X51; /* Q */
inline constexpr char32_t LEX_CHAR_UPPERCASE_R = 0X52; /* R */
inline constexpr char32_t LEX_CHAR_UPPERCASE_S = 0X53; /* S */
inline constexpr char32_t LEX_CHAR_UPPERCASE_T = 0X54; /* T */
inline constexpr char32_t LEX_CHAR_UPPERCASE_U = 0X55; /* U */
inline constexpr char32_t LEX_CHAR_UPPERCASE_V = 0X56; /* V */
inline constexpr char32_t LEX_CHAR_UPPERCASE_W = 0X57; /* W */
inline constexpr char32_t LEX_CHAR_UPPERCASE_X = 0x58; /* X */
inline constexpr char32_t LEX_CHAR_UPPERCASE_Y = 0x59; /* Y */
inline constexpr char32_t LEX_CHAR_UPPERCASE_Z = 0x5A; /* Y */

inline constexpr char32_t LEX_CHAR_BS = 0x08;           /* backspace */
inline constexpr char32_t LEX_CHAR_TAB = 0x09;          /* character tabulation */
inline constexpr char32_t LEX_CHAR_VT = 0x0B;           /* liner tabulation */
inline constexpr char32_t LEX_CHAR_FF = 0x0C;           /* form feed */
inline constexpr char32_t LEX_CHAR_SP = 0x20;           /* space */
inline constexpr char32_t LEX_CHAR_NBSP = 0xA0;         /* no-break space */
inline constexpr char32_t LEX_CHAR_ZWNBSP = 0xFEFF;     /* zero width no-break space */
inline constexpr char32_t LEX_CHAR_MVS = 0x180e;        /* MONGOLIAN VOWEL SEPARATOR (U+180E) */
inline constexpr char32_t LEX_CHAR_DOUBLE_QUOTE = 0x22; /* " */
inline constexpr char32_t LEX_CHAR_DOLLAR_SIGN = 0x24;  /* $ */
inline constexpr char32_t LEX_CHAR_SINGLE_QUOTE = 0x27; /* ' */

inline constexpr char32_t LEX_CHAR_0 = 0x30; /* 0 */
inline constexpr char32_t LEX_CHAR_1 = 0x31; /* 1 */
inline constexpr char32_t LEX_CHAR_2 = 0x32; /* 2 */
inline constexpr char32_t LEX_CHAR_3 = 0x33; /* 3 */
inline constexpr char32_t LEX_CHAR_4 = 0x34; /* 4 */
inline constexpr char32_t LEX_CHAR_5 = 0x35; /* 5 */
inline constexpr char32_t LEX_CHAR_6 = 0x36; /* 6 */
inline constexpr char32_t LEX_CHAR_7 = 0x37; /* 7 */
inline constexpr char32_t LEX_CHAR_8 = 0x38; /* 8 */
inline constexpr char32_t LEX_CHAR_9 = 0x39; /* 9 */

inline constexpr char32_t LEX_CHAR_BACKSLASH = 0X5c;  /* \\ */
inline constexpr char32_t LEX_CHAR_UNDERSCORE = 0x5F; /* _ */

// Punctuator characters
inline constexpr char32_t LEX_CHAR_EXCLAMATION = 0x21;  /* exclamation mark */
inline constexpr char32_t LEX_CHAR_PERCENT = 0x25;      /* percent sign */
inline constexpr char32_t LEX_CHAR_AMPERSAND = 0x26;    /* ampersand */
inline constexpr char32_t LEX_CHAR_LEFT_PAREN = 0x28;   /* left parenthesis */
inline constexpr char32_t LEX_CHAR_RIGHT_PAREN = 0x29;  /* right parenthesis */
inline constexpr char32_t LEX_CHAR_ASTERISK = 0x2A;     /* asterisk */
inline constexpr char32_t LEX_CHAR_PLUS = 0x2B;         /* plus sign */
inline constexpr char32_t LEX_CHAR_COMMA = 0x2C;        /* comma */
inline constexpr char32_t LEX_CHAR_MINUS = 0x2D;        /* hyphen-minus */
inline constexpr char32_t LEX_CHAR_DOT = 0x2E;          /* dot */
inline constexpr char32_t LEX_CHAR_SLASH = 0x2F;        /* solidus */
inline constexpr char32_t LEX_CHAR_COLON = 0x3A;        /* colon */
inline constexpr char32_t LEX_CHAR_SEMICOLON = 0x3B;    /* semicolon */
inline constexpr char32_t LEX_CHAR_LESS_THAN = 0x3C;    /* less-than sign */
inline constexpr char32_t LEX_CHAR_EQUALS = 0x3D;       /* equals sign */
inline constexpr char32_t LEX_CHAR_GREATER_THAN = 0x3E; /* greater-than sign */
inline constexpr char32_t LEX_CHAR_QUESTION = 0x3F;     /* question mark */
inline constexpr char32_t LEX_CHAR_LEFT_SQUARE = 0x5B;  /* left square bracket */
inline constexpr char32_t LEX_CHAR_RIGHT_SQUARE = 0x5D; /* right square bracket */
inline constexpr char32_t LEX_CHAR_CIRCUMFLEX = 0x5E;   /* circumflex accent */
inline constexpr char32_t LEX_CHAR_LEFT_BRACE = 0x7B;   /* left curly bracket */
inline constexpr char32_t LEX_CHAR_VLINE = 0x7C;        /* vertical line */
inline constexpr char32_t LEX_CHAR_RIGHT_BRACE = 0x7D;  /* right curly bracket */
inline constexpr char32_t LEX_CHAR_TILDE = 0x7E;        /* tilde */
inline constexpr char32_t LEX_CHAR_BACK_TICK = 0x60;    /* back tick */
inline constexpr char32_t LEX_CHAR_HASH_MARK = 0x23;    /* hash mark */
inline constexpr char32_t LEX_CHAR_AT = 0x40;           /* at */

inline constexpr char32_t LEX_CHAR_ZWNJ = 0x200C; /* zero width non-joiner */
inline constexpr char32_t LEX_CHAR_ZWJ = 0x200D;  /* zero width joiner */

inline constexpr char32_t LEX_VERTICAL_TILDE = 0x2E2F; /* vertical tilde */

inline constexpr char32_t LEX_UTF8_2_BYTE_CODE_POINT_MIN = 0x80;
inline constexpr char32_t LEX_UTF8_EXTRA_BYTE_MASK = 0xC0;

inline constexpr char32_t LEX_CHAR_EOS = 0xFFFF;
inline constexpr char32_t UNICODE_CODE_POINT_MAX = 0x10FFFF;
inline constexpr char32_t UNICODE_INVALID_CP = UINT32_MAX;
}  // namespace ark::es2panda::lexer

#endif
