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

#ifndef ES2PANDA_COMPILER_CHECKER_TYPES_TYPE_FACTS_H
#define ES2PANDA_COMPILER_CHECKER_TYPES_TYPE_FACTS_H

#include <cstdint>

#include "util/enumbitops.h"

namespace ark::es2panda::checker {

using ENUMBITOPS_OPERATORS;

enum class TypeFacts : uint32_t {
    NONE = 0U,
    TYPEOF_EQ_STRING = 1U << 0U,        // typeof x === "string"
    TYPEOF_EQ_NUMBER = 1U << 1U,        // typeof x === "number"
    TYPEOF_EQ_BIGINT = 1U << 2U,        // typeof x === "bigint"
    TYPEOF_EQ_BOOLEAN = 1U << 3U,       // typeof x === "boolean"
    TYPEOF_EQ_SYMBOL = 1U << 4U,        // typeof x === "symbol"
    TYPEOF_EQ_OBJECT = 1U << 5U,        // typeof x === "object"
    TYPEOF_EQ_FUNCTION = 1U << 6U,      // typeof x === "function"
    TYPEOF_EQ_HOST_OBJECT = 1U << 7U,   // typeof x === "xxx"
    TYPEOF_NE_STRING = 1U << 8U,        // typeof x !== "string"
    TYPEOF_NE_NUMBER = 1U << 9U,        // typeof x !== "number"
    TYPEOF_NE_BIGINT = 1U << 10U,       // typeof x !== "bigint"
    TYPEOF_NE_BOOLEAN = 1U << 11U,      // typeof x !== "boolean"
    TYPEOF_NE_SYMBOL = 1U << 12U,       // typeof x !== "symbol"
    TYPEOF_NE_OBJECT = 1U << 13U,       // typeof x !== "object"
    TYPEOF_NE_FUNCTION = 1U << 14U,     // typeof x !== "function"
    TYPEOF_NE_HOST_OBJECT = 1U << 15U,  // typeof x !== "xxx"
    EQ_UNDEFINED = 1U << 16U,           // x === undefined
    EQ_NULL = 1U << 17U,                // x === null
    EQ_UNDEFINED_OR_NULL = 1U << 18U,   // x === undefined / x === null
    NE_UNDEFINED = 1U << 19U,           // x !== undefined
    NE_NULL = 1U << 20U,                // x !== null
    NE_UNDEFINED_OR_NULL = 1U << 21U,   // x != undefined / x != null
    TRUTHY = 1U << 22U,                 // x
    FALSY = 1U << 23U,                  // !x

    LAST = FALSY,
    ALL = (LAST << 1U) - 1U,

    // Number facts
    BASE_NUMBER_STRICT_FACTS = TYPEOF_EQ_NUMBER | TYPEOF_NE_STRING | TYPEOF_NE_BIGINT | TYPEOF_NE_BOOLEAN |
                               TYPEOF_NE_SYMBOL | TYPEOF_NE_OBJECT | TYPEOF_NE_FUNCTION | TYPEOF_NE_HOST_OBJECT |
                               NE_UNDEFINED | NE_NULL | NE_UNDEFINED_OR_NULL,
    BASE_NUMBER_FACTS = BASE_NUMBER_STRICT_FACTS | EQ_UNDEFINED | EQ_NULL | EQ_UNDEFINED_OR_NULL | FALSY,
    NUMBER_FACTS = BASE_NUMBER_FACTS | TRUTHY,
    ZERO_NUMBER_STRICT_FACTS = BASE_NUMBER_STRICT_FACTS | FALSY,
    ZERO_NUMBER_FACTS = BASE_NUMBER_FACTS,
    NON_ZERO_NUMBER_FACTS = BASE_NUMBER_FACTS | TRUTHY,

    // String facts
    BASE_STRING_STRICT_FACTS = TYPEOF_EQ_STRING | TYPEOF_NE_NUMBER | TYPEOF_NE_BIGINT | TYPEOF_NE_BOOLEAN |
                               TYPEOF_NE_SYMBOL | TYPEOF_NE_OBJECT | TYPEOF_NE_FUNCTION | TYPEOF_NE_HOST_OBJECT |
                               NE_UNDEFINED | NE_NULL | NE_UNDEFINED_OR_NULL,
    BASE_STRING_FACTS = BASE_STRING_STRICT_FACTS | EQ_UNDEFINED | EQ_NULL | EQ_UNDEFINED_OR_NULL | FALSY,
    STRING_FACTS = BASE_STRING_FACTS | TRUTHY,
    EMPTY_STRING_STRICT_FACTS = BASE_STRING_STRICT_FACTS | FALSY | TRUTHY,
    EMPTY_STRING_FACTS = BASE_STRING_FACTS,
    NON_EMPTY_STRING_FACTS = BASE_STRING_FACTS | TRUTHY,

    // Bigint facts
    BASE_BIGINT_STRICT_FACTS = TYPEOF_EQ_BIGINT | TYPEOF_NE_STRING | TYPEOF_NE_NUMBER | TYPEOF_NE_BOOLEAN |
                               TYPEOF_NE_SYMBOL | TYPEOF_NE_OBJECT | TYPEOF_NE_FUNCTION | TYPEOF_NE_HOST_OBJECT |
                               NE_UNDEFINED | NE_NULL | NE_UNDEFINED_OR_NULL,
    BASE_BIGINT_FACTS = BASE_BIGINT_STRICT_FACTS | EQ_UNDEFINED | EQ_NULL | EQ_UNDEFINED_OR_NULL | FALSY,
    BIGINT_FACTS = BASE_BIGINT_FACTS | TRUTHY,
    ZERO_BIGINT_STRICT_FACTS = BASE_BIGINT_STRICT_FACTS | FALSY,
    ZERO_BIGINT_FACTS = BASE_BIGINT_FACTS,
    NON_ZERO_BIGINT_FACTS = BASE_BIGINT_FACTS | TRUTHY,

    // Boolean facts
    BASE_BOOLEAN_STRICT_FACTS = TYPEOF_NE_BIGINT | TYPEOF_NE_STRING | TYPEOF_NE_NUMBER | TYPEOF_EQ_BOOLEAN |
                                TYPEOF_NE_SYMBOL | TYPEOF_NE_OBJECT | TYPEOF_NE_FUNCTION | TYPEOF_NE_HOST_OBJECT |
                                NE_UNDEFINED | NE_NULL | NE_UNDEFINED_OR_NULL,
    BASE_BOOLEAN_FACTS = BASE_BOOLEAN_STRICT_FACTS | EQ_UNDEFINED | EQ_NULL | EQ_UNDEFINED_OR_NULL | FALSY,
    BOOLEAN_FACTS = BASE_BOOLEAN_FACTS | TRUTHY,
    FALSE_STRICT_FACTS = BASE_BOOLEAN_STRICT_FACTS | FALSY,
    FALSE_FACTS = BASE_BOOLEAN_FACTS,
    TRUE_STRICT_FACTS = BASE_BOOLEAN_STRICT_FACTS | TRUTHY,
    TRUE_FACTS = BASE_BOOLEAN_FACTS | TRUTHY,

    // Object facts
    OBJECT_STRICT_FACTS = TYPEOF_NE_BIGINT | TYPEOF_NE_STRING | TYPEOF_NE_NUMBER | TYPEOF_NE_BOOLEAN |
                          TYPEOF_NE_SYMBOL | TYPEOF_EQ_OBJECT | TYPEOF_NE_FUNCTION | TYPEOF_EQ_HOST_OBJECT |
                          NE_UNDEFINED | NE_NULL | NE_UNDEFINED_OR_NULL | TRUTHY,
    OBJECT_FACTS = OBJECT_STRICT_FACTS | EQ_UNDEFINED | EQ_NULL | EQ_UNDEFINED_OR_NULL | FALSY,
    EMPTY_OBJECT_STRICT_FACTS = ALL & ~(EQ_UNDEFINED | EQ_NULL | EQ_UNDEFINED_OR_NULL),
    EMPTY_OBJECT_FACTS = ALL,

    // Function facts
    FUNCTION_STRICT_FACTS = TYPEOF_NE_BIGINT | TYPEOF_NE_STRING | TYPEOF_NE_NUMBER | TYPEOF_NE_BOOLEAN |
                            TYPEOF_NE_SYMBOL | TYPEOF_NE_OBJECT | TYPEOF_EQ_FUNCTION | TYPEOF_EQ_HOST_OBJECT |
                            NE_UNDEFINED | NE_NULL | NE_UNDEFINED_OR_NULL | TRUTHY,
    FUNCTION_FACTS = FUNCTION_STRICT_FACTS | EQ_UNDEFINED | EQ_NULL | EQ_UNDEFINED_OR_NULL | FALSY,

    // Undefined facts
    UNDEFINED_FACTS = TYPEOF_NE_BIGINT | TYPEOF_NE_STRING | TYPEOF_NE_NUMBER | TYPEOF_NE_BOOLEAN | TYPEOF_NE_SYMBOL |
                      TYPEOF_NE_OBJECT | TYPEOF_NE_FUNCTION | TYPEOF_NE_HOST_OBJECT | EQ_UNDEFINED | NE_NULL |
                      EQ_UNDEFINED_OR_NULL | FALSY,

    // Null facts
    NULL_FACTS = TYPEOF_NE_BIGINT | TYPEOF_NE_STRING | TYPEOF_NE_NUMBER | TYPEOF_NE_BOOLEAN | TYPEOF_NE_SYMBOL |
                 TYPEOF_EQ_OBJECT | TYPEOF_NE_FUNCTION | TYPEOF_NE_HOST_OBJECT | NE_UNDEFINED | EQ_NULL |
                 EQ_UNDEFINED_OR_NULL | FALSY,
};

}  // namespace ark::es2panda::checker

template <>
struct enumbitops::IsAllowedType<ark::es2panda::checker::TypeFacts> : std::true_type {
};

#endif /* TYPESCRIPT_TYPES_TYPE_FACTS_H */
