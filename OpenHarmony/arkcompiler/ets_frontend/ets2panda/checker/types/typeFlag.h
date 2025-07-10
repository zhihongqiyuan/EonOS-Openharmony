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

#ifndef ES2PANDA_COMPILER_CHECKER_TYPES_TYPE_FLAG_H
#define ES2PANDA_COMPILER_CHECKER_TYPES_TYPE_FLAG_H

#include "util/enumbitops.h"

#include <cinttypes>

namespace ark::es2panda::checker {

using ENUMBITOPS_OPERATORS;

enum class TypeFlag : uint64_t {
    NONE = 0,
    NUMBER = 1ULL << 0ULL,               // x: number
    STRING = 1ULL << 1ULL,               // x: string
    BOOLEAN = 1ULL << 2ULL,              // x: boolean
    VOID = 1ULL << 3ULL,                 // x: void
    NULL_TYPE = 1ULL << 4ULL,            // x: null
    UNDEFINED = 1ULL << 5ULL,            // x: undefined
    UNKNOWN = 1ULL << 6ULL,              // x: unknown
    NEVER = 1ULL << 7ULL,                // x: never
    UNION = 1ULL << 8ULL,                // x: a | b
    OBJECT = 1ULL << 9ULL,               // x: object
    BIGINT = 1ULL << 10ULL,              // x: bigint
    BOOLEAN_LITERAL = 1ULL << 11ULL,     // x: true
    NUMBER_LITERAL = 1ULL << 12ULL,      // x: 10
    STRING_LITERAL = 1ULL << 13ULL,      // x: "foo"
    BIGINT_LITERAL = 1ULL << 14ULL,      // x: 10n
    ENUM = 1ULL << 15ULL,                // enum x
    ENUM_LITERAL = 1ULL << 16ULL,        // member of enum
    SYMBOL = 1ULL << 17ULL,              // x: symbol
    TYPE_PARAMETER = 1ULL << 18ULL,      // function<x>
    INTERSECTION = 1ULL << 19ULL,        // x: a & b
    INDEX = 1ULL << 20ULL,               // keyof x
    INDEX_ACCESS = 1ULL << 21ULL,        // x[a]
    CONDITIONAL = 1ULL << 22ULL,         // x extends a ? b : c
    TEMPLATE_LITERAL = 1ULL << 23ULL,    // x: `hello ${World}`
    ANY = 1ULL << 24ULL,                 // x: any
    ARRAY = 1ULL << 25ULL,               // x: number[]
    FUNCTION = 1ULL << 26ULL,            // x: (a) => b
    NON_PRIMITIVE = 1ULL << 27ULL,       // x: object
    TYPE_REFERENCE = 1ULL << 28ULL,      // x: A
    READONLY = 1ULL << 29ULL,            // type assigned to a readonly property
    CONSTANT = 1ULL << 30ULL,            // type for constant expressions containing the associated constant value
    BYTE = 1ULL << 31ULL,                // x: byte
    SHORT = 1ULL << 32ULL,               // x: short
    INT = 1ULL << 33ULL,                 // x: int
    LONG = 1ULL << 34ULL,                // x: long
    FLOAT = 1ULL << 35ULL,               // x: float
    DOUBLE = 1ULL << 36ULL,              // x: double
    CHAR = 1ULL << 37ULL,                // x: char
    ETS_BOOLEAN = 1ULL << 38ULL,         // ETS boolean type
    ETS_VOID = 1ULL << 39ULL,            // ETS void type
    ETS_OBJECT = 1ULL << 40ULL,          // ETS class or interface type
    ETS_ARRAY = 1ULL << 41ULL,           // ETS array type
    WILDCARD = 1ULL << 42ULL,            // new A<?>()
    ETS_TYPE_PARAMETER = 1ULL << 43ULL,  // ETS type parameter
    ETS_TYPE_REFERENCE = 1ULL << 44ULL,  // ETS type reference
    GENERIC = 1ULL << 45ULL,             // ETS Generic
    ETS_INT_ENUM = 1ULL << 46ULL,        // ETS Enum
    ETS_STRING_ENUM = 1ULL << 47ULL,     // ETS string-type Enumeration
    ETS_DYNAMIC_FLAG = 1ULL << 48ULL,    // ETS Dynamic flag
    GETTER = 1ULL << 49ULL,              // ETS Getter
    SETTER = 1ULL << 50ULL,              // ETS Setter
    ETS_EXTENSION_FUNC_HELPER = 1ULL << 51ULL,    // ETS Extension Function Helper
    ETS_UNION = 1ULL << 52ULL,                    // ETS union
    ETS_TUPLE = 1ULL << 53ULL,                    // ETS tuple type
    ETS_NULL = 1ULL << 54ULL,                     // ETS null
    ETS_UNDEFINED = 1ULL << 55ULL,                // ETS undefined
    ETS_NONNULLISH = 1ULL << 56ULL,               // ETS nonnullish type parameter
    ETS_READONLY = 1ULL << 57ULL,                 // ETS readonly type parameter
    ETS_REQUIRED_TYPE_PARAMETER = 1ULL << 58ULL,  // ETS Required type parameter
    ETS_PARTIAL_TYPE_PARAMETER = 1ULL << 59ULL,   // ETS Partial type parameter
    TYPE_ERROR = 1ULL << 60ULL,                   // type error
    ETS_TYPE_ALIAS = 1ULL << 61ULL,               // ETS Type alias
    ETS_NEVER = 1ULL << 62ULL,                    // ETS never
    ETS_DYNAMIC_TYPE = ETS_OBJECT | ETS_DYNAMIC_FLAG,
    ETS_DYNAMIC_FUNCTION_TYPE = FUNCTION | ETS_DYNAMIC_FLAG,
    ETS_TYPE = BYTE | SHORT | INT | LONG | FLOAT | DOUBLE | CHAR | ETS_BOOLEAN | ETS_VOID | ETS_OBJECT | ETS_ARRAY |
               WILDCARD | ETS_TYPE_PARAMETER | ETS_INT_ENUM | ETS_STRING_ENUM | ETS_DYNAMIC_TYPE | ETS_UNION |
               ETS_NULL | ETS_UNDEFINED | ETS_NONNULLISH | ETS_READONLY | ETS_REQUIRED_TYPE_PARAMETER |
               ETS_PARTIAL_TYPE_PARAMETER | ETS_NEVER,
    ETS_INTEGRAL_NUMERIC = BYTE | SHORT | INT | LONG,
    ETS_FLOATING_POINT = FLOAT | DOUBLE,
    ETS_NUMERIC = ETS_INTEGRAL_NUMERIC | ETS_FLOATING_POINT,
    ETS_INTEGRAL = ETS_INTEGRAL_NUMERIC | CHAR,
    ETS_ENUM = ETS_INT_ENUM | ETS_STRING_ENUM,
    ETS_ARRAY_INDEX = BYTE | SHORT | INT,
    ETS_CONVERTIBLE_TO_NUMERIC = ETS_NUMERIC | CHAR | ETS_INT_ENUM,
    VALID_SWITCH_TYPE = ETS_INTEGRAL | ETS_ENUM,
    ETS_ARRAY_OR_OBJECT = ETS_ARRAY | ETS_OBJECT,
    ANY_OR_UNKNOWN = ANY | UNKNOWN,
    ANY_OR_VOID = ANY | VOID,
    LITERAL = NUMBER_LITERAL | BOOLEAN_LITERAL | STRING_LITERAL | BIGINT_LITERAL,
    NUMBER_LIKE = NUMBER | NUMBER_LITERAL,
    STRING_LIKE = STRING | STRING_LITERAL,
    BOOLEAN_LIKE = BOOLEAN | BOOLEAN_LITERAL,
    BIGINT_LIKE = BIGINT | BIGINT_LITERAL,
    VOID_LIKE = VOID | UNDEFINED,
    PRIMITIVE =
        STRING | NUMBER | BIGINT | BOOLEAN | ENUM | ENUM_LITERAL | SYMBOL | VOID | UNDEFINED | NULL_TYPE | LITERAL,
    PRIMITIVE_OR_ANY = PRIMITIVE | ANY,
    UNION_OR_INTERSECTION = UNION | INTERSECTION,
    DEFINITELY_FALSY =
        STRING_LITERAL | NUMBER_LITERAL | BOOLEAN_LITERAL | BIGINT_LITERAL | VOID | UNDEFINED | NULL_TYPE,
    POSSIBLY_FALSY = DEFINITELY_FALSY | STRING | NUMBER | BOOLEAN | BIGINT,
    VALID_ARITHMETIC_TYPE = ANY | NUMBER_LIKE | BIGINT_LIKE | ENUM,
    UNIT = LITERAL | UNDEFINED | NULL_TYPE,
    GETTER_SETTER = GETTER | SETTER,
    CONDITION_EXPRESSION_TYPE = ETS_NULL | ETS_UNDEFINED | ETS_OBJECT | ETS_ARRAY | ETS_UNION | CONSTANT | BYTE | CHAR |
                                SHORT | INT | LONG | FLOAT | DOUBLE | ETS_BOOLEAN | ETS_INT_ENUM | ETS_STRING_ENUM,
};

}  // namespace ark::es2panda::checker

template <>
struct enumbitops::IsAllowedType<ark::es2panda::checker::TypeFlag> : std::true_type {
};

#endif /* TYPESCRIPT_TYPES_TYPE_FLAG_H */
