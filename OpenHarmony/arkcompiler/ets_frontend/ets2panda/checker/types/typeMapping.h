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

#ifndef ES2PANDA_COMPILER_CHECKER_TYPES_TYPE_MAPPING_H
#define ES2PANDA_COMPILER_CHECKER_TYPES_TYPE_MAPPING_H

#include "typeFlag.h"

// CC-OFFNXT(G.PRE.06) solid logic
// NOLINTBEGIN(cppcoreguidelines-macro-usage)
#define TYPE_MAPPING(_)                                                \
    _(TypeFlag::ARRAY, ArrayType)                                      \
    _(TypeFlag::ANY, AnyType)                                          \
    _(TypeFlag::BIGINT_LITERAL, BigintLiteralType)                     \
    _(TypeFlag::NUMBER, NumberType)                                    \
    _(TypeFlag::STRING, StringType)                                    \
    _(TypeFlag::BOOLEAN, BooleanType)                                  \
    _(TypeFlag::VOID, VoidType)                                        \
    _(TypeFlag::NULL_TYPE, NullType)                                   \
    _(TypeFlag::UNDEFINED, UndefinedType)                              \
    _(TypeFlag::UNKNOWN, UnknownType)                                  \
    _(TypeFlag::NEVER, NeverType)                                      \
    _(TypeFlag::UNION, UnionType)                                      \
    _(TypeFlag::OBJECT, ObjectType)                                    \
    _(TypeFlag::BIGINT, BigintType)                                    \
    _(TypeFlag::BOOLEAN_LITERAL, BooleanLiteralType)                   \
    _(TypeFlag::NUMBER_LITERAL, NumberLiteralType)                     \
    _(TypeFlag::STRING_LITERAL, StringLiteralType)                     \
    _(TypeFlag::ENUM, EnumType)                                        \
    _(TypeFlag::ENUM_LITERAL, EnumLiteralType)                         \
    _(TypeFlag::TYPE_PARAMETER, TypeParameter)                         \
    _(TypeFlag::TYPE_REFERENCE, TypeReference)                         \
    _(TypeFlag::BYTE, ByteType)                                        \
    _(TypeFlag::SHORT, ShortType)                                      \
    _(TypeFlag::INT, IntType)                                          \
    _(TypeFlag::LONG, LongType)                                        \
    _(TypeFlag::FLOAT, FloatType)                                      \
    _(TypeFlag::DOUBLE, DoubleType)                                    \
    _(TypeFlag::CHAR, CharType)                                        \
    _(TypeFlag::ETS_BOOLEAN, ETSBooleanType)                           \
    _(TypeFlag::ETS_VOID, ETSVoidType)                                 \
    _(TypeFlag::ETS_NULL, ETSNullType)                                 \
    _(TypeFlag::ETS_UNDEFINED, ETSUndefinedType)                       \
    _(TypeFlag::ETS_NEVER, ETSNeverType)                               \
    _(TypeFlag::FUNCTION, ETSFunctionType)                             \
    _(TypeFlag::ETS_OBJECT, ETSObjectType)                             \
    _(TypeFlag::ETS_ARRAY, ETSArrayType)                               \
    _(TypeFlag::ETS_UNION, ETSUnionType)                               \
    _(TypeFlag::NON_PRIMITIVE, NonPrimitiveType)                       \
    _(TypeFlag::WILDCARD, WildcardType)                                \
    _(TypeFlag::ETS_TYPE_PARAMETER, ETSTypeParameter)                  \
    _(TypeFlag::ETS_NONNULLISH, ETSNonNullishType)                     \
    _(TypeFlag::ETS_READONLY, ETSReadonlyType)                         \
    _(TypeFlag::ETS_INT_ENUM, ETSIntEnumType)                          \
    _(TypeFlag::ETS_STRING_ENUM, ETSStringEnumType)                    \
    _(TypeFlag::ETS_ENUM, ETSEnumType)                                 \
    _(TypeFlag::ETS_EXTENSION_FUNC_HELPER, ETSExtensionFuncHelperType) \
    _(TypeFlag::ETS_TUPLE, ETSTupleType)                               \
    _(TypeFlag::ETS_PARTIAL_TYPE_PARAMETER, ETSPartialTypeParameter)   \
    _(TypeFlag::TYPE_ERROR, TypeError)                                 \
    _(TypeFlag::ETS_TYPE_ALIAS, ETSTypeAliasType)

#define OBJECT_TYPE_MAPPING(_)                                \
    _(ObjectType::ObjectTypeKind::FUNCTION, FunctionType)     \
    _(ObjectType::ObjectTypeKind::TUPLE, TupleType)           \
    _(ObjectType::ObjectTypeKind::LITERAL, ObjectLiteralType) \
    _(ObjectType::ObjectTypeKind::INTERFACE, InterfaceType)
// NOLINTEND(cppcoreguidelines-macro-usage)

#endif /* TYPE_MAPPING_H */
