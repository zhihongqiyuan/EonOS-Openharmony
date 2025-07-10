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

#include "globalTypesHolder.h"

#include "checker/types/typeError.h"
#include "checker/types/ts/numberType.h"
#include "checker/types/ts/anyType.h"
#include "checker/types/ts/stringType.h"
#include "checker/types/ts/booleanType.h"
#include "checker/types/ts/voidType.h"
#include "checker/types/ts/nullType.h"
#include "checker/types/ts/undefinedType.h"
#include "checker/types/ts/unknownType.h"
#include "checker/types/ts/neverType.h"
#include "checker/types/ts/nonPrimitiveType.h"
#include "checker/types/ts/bigintType.h"
#include "checker/types/ts/booleanLiteralType.h"
#include "checker/types/ts/bigintLiteralType.h"
#include "checker/types/ts/numberLiteralType.h"
#include "checker/types/ts/stringLiteralType.h"
#include "checker/types/ts/tupleType.h"
#include "checker/types/ts/objectLiteralType.h"
#include "checker/types/ts/unionType.h"
#include "checker/types/ets/byteType.h"
#include "checker/types/ets/charType.h"
#include "checker/types/ets/doubleType.h"
#include "checker/types/ets/floatType.h"
#include "checker/types/ets/intType.h"
#include "checker/types/ets/longType.h"
#include "checker/types/ets/shortType.h"
#include "checker/types/ets/etsBooleanType.h"
#include "checker/types/ets/etsStringType.h"
#include "checker/types/ets/etsBigIntType.h"
#include "checker/types/ets/etsVoidType.h"
#include "checker/types/ets/etsNullishTypes.h"
#include "checker/types/ets/etsObjectType.h"
#include "checker/types/ets/wildcardType.h"
#include "checker/types/ets/etsNeverType.h"
#include "util/helpers.h"

namespace ark::es2panda::checker {

void GlobalTypesHolder::AddETSEscompatLayer()
{
    // ETS escompat layer
    builtinNameMappings_.emplace("Array", GlobalTypeId::ETS_ARRAY_BUILTIN);
    builtinNameMappings_.emplace("Date", GlobalTypeId::ETS_DATE_BUILTIN);
    builtinNameMappings_.emplace("Error", GlobalTypeId::ETS_ERROR_BUILTIN);
    builtinNameMappings_.emplace("OutOfMemoryError", GlobalTypeId::ETS_OUT_OF_MEMORY_ERROR_BUILTIN);
    builtinNameMappings_.emplace("NoSuchMethodError", GlobalTypeId::ETS_NO_SUCH_METHOD_ERROR_BUILTIN);
    builtinNameMappings_.emplace("DivideByZeroError", GlobalTypeId::ETS_DIVIDE_BY_ZERO_ERROR_BUILTIN);
    builtinNameMappings_.emplace("NullPointerError", GlobalTypeId::ETS_NULL_POINTER_ERROR_BUILTIN);
    builtinNameMappings_.emplace("UncaughtExceptionError", GlobalTypeId::ETS_UNCAUGHT_EXCEPTION_ERROR_BUILTIN);
    builtinNameMappings_.emplace("Map", GlobalTypeId::ETS_MAP_BUILTIN);
    builtinNameMappings_.emplace("RegExp", GlobalTypeId::ETS_REGEXP_BUILTIN);
    builtinNameMappings_.emplace("Set", GlobalTypeId::ETS_SET_BUILTIN);
}

void GlobalTypesHolder::AddFunctionTypes(ArenaAllocator *allocator)
{
    // ETS throwing functional types
    for (size_t id = static_cast<size_t>(GlobalTypeId::ETS_THROWING_FUNCTION0_CLASS), nargs = 0;
         id < static_cast<size_t>(GlobalTypeId::ETS_THROWING_FUNCTIONN_CLASS); id++, nargs++) {
        builtinNameMappings_.emplace(util::UString("ThrowingFunction" + std::to_string(nargs), allocator).View(),
                                     static_cast<GlobalTypeId>(id));
    }

    builtinNameMappings_.emplace("ThrowingFunctionN", GlobalTypeId::ETS_THROWING_FUNCTIONN_CLASS);

    // ETS rethrowing functional types
    for (size_t id = static_cast<size_t>(GlobalTypeId::ETS_RETHROWING_FUNCTION0_CLASS), nargs = 0;
         id < static_cast<size_t>(GlobalTypeId::ETS_RETHROWING_FUNCTIONN_CLASS); id++, nargs++) {
        builtinNameMappings_.emplace(util::UString("RethrowingFunction" + std::to_string(nargs), allocator).View(),
                                     static_cast<GlobalTypeId>(id));
    }

    builtinNameMappings_.emplace("RethrowingFunctionN", GlobalTypeId::ETS_RETHROWING_FUNCTIONN_CLASS);

    // ETS functional types
    for (size_t id = static_cast<size_t>(GlobalTypeId::ETS_FUNCTION0_CLASS), nargs = 0;
         id < static_cast<size_t>(GlobalTypeId::ETS_FUNCTIONN_CLASS); id++, nargs++) {
        builtinNameMappings_.emplace(util::UString("Function" + std::to_string(nargs), allocator).View(),
                                     static_cast<GlobalTypeId>(id));
    }

    builtinNameMappings_.emplace("FunctionN", GlobalTypeId::ETS_FUNCTIONN_CLASS);
}

void GlobalTypesHolder::AddTSSpecificTypes(ArenaAllocator *allocator)
{
    globalTypes_[static_cast<size_t>(GlobalTypeId::NUMBER)] = allocator->New<NumberType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::ANY)] = allocator->New<AnyType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::STRING)] = allocator->New<StringType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::BOOLEAN)] = allocator->New<BooleanType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::VOID)] = allocator->New<VoidType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::NULL_ID)] = allocator->New<NullType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::UNDEFINED)] = allocator->New<UndefinedType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::UNKNOWN)] = allocator->New<UnknownType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::NEVER)] = allocator->New<NeverType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::NON_PRIMITIVE)] = allocator->New<NonPrimitiveType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::BIGINT)] = allocator->New<BigintType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::FALSE_ID)] = allocator->New<BooleanLiteralType>(false);
    globalTypes_[static_cast<size_t>(GlobalTypeId::TRUE_ID)] = allocator->New<BooleanLiteralType>(true);
    globalTypes_[static_cast<size_t>(GlobalTypeId::NUMBER_OR_BIGINT)] =
        allocator->New<UnionType>(allocator, std::initializer_list<Type *> {GlobalNumberType(), GlobalBigintType()});
    globalTypes_[static_cast<size_t>(GlobalTypeId::STRING_OR_NUMBER)] =
        allocator->New<UnionType>(allocator, std::initializer_list<Type *> {GlobalStringType(), GlobalNumberType()});
    globalTypes_[static_cast<size_t>(GlobalTypeId::ZERO)] = allocator->New<NumberLiteralType>(0);
    globalTypes_[static_cast<size_t>(GlobalTypeId::EMPTY_STRING)] = allocator->New<StringLiteralType>("");
    globalTypes_[static_cast<size_t>(GlobalTypeId::ZERO_BIGINT)] = allocator->New<BigintLiteralType>("0n", false);
    globalTypes_[static_cast<size_t>(GlobalTypeId::PRIMITIVE)] = allocator->New<UnionType>(
        allocator,
        std::initializer_list<Type *> {GlobalNumberType(), GlobalStringType(), GlobalBigintType(), GlobalBooleanType(),
                                       GlobalVoidType(), GlobalUndefinedType(), GlobalNullType()});
    globalTypes_[static_cast<size_t>(GlobalTypeId::EMPTY_TUPLE)] = allocator->New<TupleType>(allocator);
    globalTypes_[static_cast<size_t>(GlobalTypeId::EMPTY_OBJECT)] = allocator->New<ObjectLiteralType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::RESOLVING_RETURN_TYPE)] = allocator->New<AnyType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::ERROR_TYPE)] = allocator->New<AnyType>();
}

void GlobalTypesHolder::AddEtsSpecificTypes(ArenaAllocator *allocator)
{
    // ETS specific types
    globalTypes_[static_cast<size_t>(GlobalTypeId::BYTE)] = allocator->New<ByteType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::SHORT)] = allocator->New<ShortType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::INT)] = allocator->New<IntType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::LONG)] = allocator->New<LongType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::FLOAT)] = allocator->New<FloatType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::DOUBLE)] = allocator->New<DoubleType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::CHAR)] = allocator->New<CharType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::ETS_BOOLEAN)] = allocator->New<ETSBooleanType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::ETS_VOID)] = allocator->New<ETSVoidType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::ETS_NULL)] = allocator->New<ETSNullType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::ETS_UNDEFINED)] = allocator->New<ETSUndefinedType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::ETS_WILDCARD)] = allocator->New<WildcardType>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::TYPE_ERROR)] = allocator->New<TypeError>();
    globalTypes_[static_cast<size_t>(GlobalTypeId::ETS_NEVER)] = allocator->New<ETSNeverType>();
}

void GlobalTypesHolder::AddEtsSpecificBuiltinTypes()
{
    builtinNameMappings_.emplace("Boolean", GlobalTypeId::ETS_BOOLEAN_BUILTIN);
    builtinNameMappings_.emplace("Byte", GlobalTypeId::ETS_BYTE_BUILTIN);
    builtinNameMappings_.emplace("Char", GlobalTypeId::ETS_CHAR_BUILTIN);
    builtinNameMappings_.emplace("Comparable", GlobalTypeId::ETS_COMPARABLE_BUILTIN);
    builtinNameMappings_.emplace("Console", GlobalTypeId::ETS_CONSOLE_BUILTIN);
    builtinNameMappings_.emplace("Double", GlobalTypeId::ETS_DOUBLE_BUILTIN);
    builtinNameMappings_.emplace("Exception", GlobalTypeId::ETS_EXCEPTION_BUILTIN);
    builtinNameMappings_.emplace("Float", GlobalTypeId::ETS_FLOAT_BUILTIN);
    builtinNameMappings_.emplace("Floating", GlobalTypeId::ETS_FLOATING_BUILTIN);
    builtinNameMappings_.emplace("Int", GlobalTypeId::ETS_INTEGER_BUILTIN);
    builtinNameMappings_.emplace("Integral", GlobalTypeId::ETS_INTEGRAL_BUILTIN);
    builtinNameMappings_.emplace("Long", GlobalTypeId::ETS_LONG_BUILTIN);
    builtinNameMappings_.emplace("Object", GlobalTypeId::ETS_OBJECT_BUILTIN);
    builtinNameMappings_.emplace("Runtime", GlobalTypeId::ETS_RUNTIME_BUILTIN);
    builtinNameMappings_.emplace("RuntimeLinker", GlobalTypeId::ETS_RUNTIME_LINKER_BUILTIN);
    builtinNameMappings_.emplace("Short", GlobalTypeId::ETS_SHORT_BUILTIN);
    builtinNameMappings_.emplace("StackTraceElement", GlobalTypeId::ETS_STACK_TRACE_ELEMENT_BUILTIN);
    builtinNameMappings_.emplace("StackTrace", GlobalTypeId::ETS_STACK_TRACE_BUILTIN);
    builtinNameMappings_.emplace("NullPointerException", GlobalTypeId::ETS_NULL_POINTER_EXCEPTION_BUILTIN);
    builtinNameMappings_.emplace("ArrayIndexOutOfBoundsError",
                                 GlobalTypeId::ETS_ARRAY_INDEX_OUT_OF_BOUNDS_ERROR_BUILTIN);
    builtinNameMappings_.emplace("ArithmeticError", GlobalTypeId::ETS_ARITHMETIC_ERROR_BUILTIN);
    builtinNameMappings_.emplace("ClassNotFoundException", GlobalTypeId::ETS_CLASS_NOT_FOUND_EXCEPTION_BUILTIN);
    builtinNameMappings_.emplace("ClassCastError", GlobalTypeId::ETS_CLASS_CAST_ERROR_BUILTIN);
    builtinNameMappings_.emplace("String", GlobalTypeId::ETS_STRING_BUILTIN);
    builtinNameMappings_.emplace("BigInt", GlobalTypeId::ETS_BIG_INT_BUILTIN);
    builtinNameMappings_.emplace("StringBuilder", GlobalTypeId::ETS_STRING_BUILDER_BUILTIN);
    builtinNameMappings_.emplace("Type", GlobalTypeId::ETS_TYPE_BUILTIN);
    builtinNameMappings_.emplace("Types", GlobalTypeId::ETS_TYPES_BUILTIN);
    builtinNameMappings_.emplace("Promise", GlobalTypeId::ETS_PROMISE_BUILTIN);
    builtinNameMappings_.emplace("Box", GlobalTypeId::ETS_BOX_BUILTIN);
    builtinNameMappings_.emplace("BooleanBox", GlobalTypeId::ETS_BOOLEAN_BOX_BUILTIN);
    builtinNameMappings_.emplace("ByteBox", GlobalTypeId::ETS_BYTE_BOX_BUILTIN);
    builtinNameMappings_.emplace("CharBox", GlobalTypeId::ETS_CHAR_BOX_BUILTIN);
    builtinNameMappings_.emplace("ShortBox", GlobalTypeId::ETS_SHORT_BOX_BUILTIN);
    builtinNameMappings_.emplace("IntBox", GlobalTypeId::ETS_INT_BOX_BUILTIN);
    builtinNameMappings_.emplace("LongBox", GlobalTypeId::ETS_LONG_BOX_BUILTIN);
    builtinNameMappings_.emplace("FloatBox", GlobalTypeId::ETS_FLOAT_BOX_BUILTIN);
    builtinNameMappings_.emplace("DoubleBox", GlobalTypeId::ETS_DOUBLE_BOX_BUILTIN);
}

GlobalTypesHolder::GlobalTypesHolder(ArenaAllocator *allocator) : builtinNameMappings_(allocator->Adapter())
{
    // TS specific types
    AddTSSpecificTypes(allocator);

    // ETS specific types
    AddEtsSpecificTypes(allocator);

    AddEtsSpecificBuiltinTypes();

    // ETS escompat layer
    AddETSEscompatLayer();

    builtinNameMappings_.emplace("TYPE ERROR", GlobalTypeId::TYPE_ERROR);

    // ETS functional types
    for (size_t id = static_cast<size_t>(GlobalTypeId::ETS_FUNCTION0_CLASS), nargs = 0;
         id < static_cast<size_t>(GlobalTypeId::ETS_FUNCTIONN_CLASS); id++, nargs++) {
        builtinNameMappings_.emplace(util::UString("Function" + std::to_string(nargs), allocator).View(),
                                     static_cast<GlobalTypeId>(id));
    }

    builtinNameMappings_.emplace("FunctionN", GlobalTypeId::ETS_FUNCTIONN_CLASS);

    // Function types
    AddFunctionTypes(allocator);

    // ETS interop js specific types
    builtinNameMappings_.emplace("JSRuntime", GlobalTypeId::ETS_INTEROP_JSRUNTIME_BUILTIN);
    builtinNameMappings_.emplace("JSValue", GlobalTypeId::ETS_INTEROP_JSVALUE_BUILTIN);
}

Type *GlobalTypesHolder::GlobalNumberType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::NUMBER));
}

Type *GlobalTypesHolder::GlobalAnyType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ANY));
}

Type *GlobalTypesHolder::GlobalStringType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::STRING));
}

Type *GlobalTypesHolder::GlobalBooleanType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::BOOLEAN));
}

Type *GlobalTypesHolder::GlobalVoidType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::VOID));
}

Type *GlobalTypesHolder::GlobalNullType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::NULL_ID));
}

Type *GlobalTypesHolder::GlobalUndefinedType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::UNDEFINED));
}

Type *GlobalTypesHolder::GlobalUnknownType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::UNKNOWN));
}

Type *GlobalTypesHolder::GlobalNeverType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::NEVER));
}

Type *GlobalTypesHolder::GlobalNonPrimitiveType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::NON_PRIMITIVE));
}

Type *GlobalTypesHolder::GlobalBigintType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::BIGINT));
}

Type *GlobalTypesHolder::GlobalFalseType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::FALSE_ID));
}

Type *GlobalTypesHolder::GlobalTrueType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::TRUE_ID));
}

Type *GlobalTypesHolder::GlobalNumberOrBigintType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::NUMBER_OR_BIGINT));
}

Type *GlobalTypesHolder::GlobalStringOrNumberType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::STRING_OR_NUMBER));
}

Type *GlobalTypesHolder::GlobalZeroType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ZERO));
}

Type *GlobalTypesHolder::GlobalEmptyStringType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::EMPTY_STRING));
}

Type *GlobalTypesHolder::GlobalZeroBigintType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ZERO_BIGINT));
}

Type *GlobalTypesHolder::GlobalPrimitiveType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::PRIMITIVE));
}

Type *GlobalTypesHolder::GlobalEmptyTupleType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::EMPTY_TUPLE));
}

Type *GlobalTypesHolder::GlobalEmptyObjectType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::EMPTY_OBJECT));
}

Type *GlobalTypesHolder::GlobalResolvingReturnType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::RESOLVING_RETURN_TYPE));
}

Type *GlobalTypesHolder::GlobalErrorType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ERROR_TYPE));
}

Type *GlobalTypesHolder::GlobalByteType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::BYTE));
}

Type *GlobalTypesHolder::GlobalShortType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::SHORT));
}

Type *GlobalTypesHolder::GlobalIntType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::INT));
}

Type *GlobalTypesHolder::GlobalLongType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::LONG));
}

Type *GlobalTypesHolder::GlobalFloatType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::FLOAT));
}

Type *GlobalTypesHolder::GlobalDoubleType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::DOUBLE));
}

Type *GlobalTypesHolder::GlobalCharType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::CHAR));
}

Type *GlobalTypesHolder::GlobalETSBooleanType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_BOOLEAN));
}

Type *GlobalTypesHolder::GlobalETSStringLiteralType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_STRING));
}

Type *GlobalTypesHolder::GlobalETSVoidType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_VOID));
}

Type *GlobalTypesHolder::GlobalETSObjectType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_OBJECT_BUILTIN));
}

Type *GlobalTypesHolder::GlobalETSNullType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_NULL));
}

Type *GlobalTypesHolder::GlobalETSUndefinedType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_UNDEFINED));
}

Type *GlobalTypesHolder::GlobalETSNeverType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_NEVER));
}

Type *GlobalTypesHolder::GlobalETSNullishObjectType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_NULLISH_OBJECT));
}

Type *GlobalTypesHolder::GlobalETSNullishType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_NULLISH_TYPE));
}

Type *GlobalTypesHolder::GlobalWildcardType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_WILDCARD));
}

Type *GlobalTypesHolder::GlobalETSBooleanBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_BOOLEAN_BUILTIN));
}

Type *GlobalTypesHolder::GlobalByteBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_BYTE_BUILTIN));
}

Type *GlobalTypesHolder::GlobalCharBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_CHAR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalComparableBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_COMPARABLE_BUILTIN));
}

Type *GlobalTypesHolder::GlobalConsoleBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_CONSOLE_BUILTIN));
}

Type *GlobalTypesHolder::GlobalDoubleBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_DOUBLE_BUILTIN));
}

Type *GlobalTypesHolder::GlobalExceptionBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_EXCEPTION_BUILTIN));
}

Type *GlobalTypesHolder::GlobalFloatBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_FLOAT_BUILTIN));
}

Type *GlobalTypesHolder::GlobalFloatingBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_FLOATING_BUILTIN));
}

Type *GlobalTypesHolder::GlobalIntegerBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_INTEGER_BUILTIN));
}

Type *GlobalTypesHolder::GlobalIntegralBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_INTEGRAL_BUILTIN));
}

Type *GlobalTypesHolder::GlobalLongBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_LONG_BUILTIN));
}

Type *GlobalTypesHolder::GlobalMapBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_MAP_BUILTIN));
}

Type *GlobalTypesHolder::GlobalErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalRuntimeBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_RUNTIME_BUILTIN));
}

Type *GlobalTypesHolder::GlobalSetBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_SET_BUILTIN));
}

Type *GlobalTypesHolder::GlobalShortBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_SHORT_BUILTIN));
}

Type *GlobalTypesHolder::GlobalStackTraceElementBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_STACK_TRACE_ELEMENT_BUILTIN));
}

Type *GlobalTypesHolder::GlobalStackTraceBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_STACK_TRACE_BUILTIN));
}

Type *GlobalTypesHolder::GlobalNullPointerExceptionBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_NULL_POINTER_EXCEPTION_BUILTIN));
}

Type *GlobalTypesHolder::GlobalArrayIndexOutOfBoundsErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_ARRAY_INDEX_OUT_OF_BOUNDS_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalArithmeticErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_ARITHMETIC_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalClassNotFoundExceptionBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_CLASS_NOT_FOUND_EXCEPTION_BUILTIN));
}

Type *GlobalTypesHolder::GlobalClassCastErrorBuiltinType() const noexcept
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_CLASS_CAST_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalClassOutOfMemoryErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_OUT_OF_MEMORY_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalNoSuchMethodErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_NO_SUCH_METHOD_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalAssertionErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_ASSERTION_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalDivideByZeroErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_DIVIDE_BY_ZERO_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalNullPointerErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_NULL_POINTER_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalUncaughtExceptionErrorBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_UNCAUGHT_EXCEPTION_ERROR_BUILTIN));
}

Type *GlobalTypesHolder::GlobalETSStringBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_STRING_BUILTIN));
}

Type *GlobalTypesHolder::GlobalETSBigIntBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_BIG_INT_BUILTIN));
}

Type *GlobalTypesHolder::GlobalETSBigIntLiteralType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_BIG_INT));
}

Type *GlobalTypesHolder::GlobalStringBuilderBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_STRING_BUILDER_BUILTIN));
}

Type *GlobalTypesHolder::GlobalTypeBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_TYPE_BUILTIN));
}

Type *GlobalTypesHolder::GlobalTypesBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_TYPES_BUILTIN));
}

Type *GlobalTypesHolder::GlobalPromiseBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_PROMISE_BUILTIN));
}

Type *GlobalTypesHolder::GlobalRegExpBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_REGEXP_BUILTIN));
}

Type *GlobalTypesHolder::GlobalArrayBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_ARRAY_BUILTIN));
}

Type *GlobalTypesHolder::GlobalBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_BOX_BUILTIN));
}

Type *GlobalTypesHolder::GlobalJSRuntimeBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_INTEROP_JSRUNTIME_BUILTIN));
}

Type *GlobalTypesHolder::GlobalJSValueBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_INTEROP_JSVALUE_BUILTIN));
}

Type *GlobalTypesHolder::GlobalBooleanBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_BOOLEAN_BOX_BUILTIN));
}

Type *GlobalTypesHolder::GlobalByteBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_BYTE_BOX_BUILTIN));
}

Type *GlobalTypesHolder::GlobalCharBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_CHAR_BOX_BUILTIN));
}

Type *GlobalTypesHolder::GlobalShortBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_SHORT_BOX_BUILTIN));
}

Type *GlobalTypesHolder::GlobalIntBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_INT_BOX_BUILTIN));
}

Type *GlobalTypesHolder::GlobalLongBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_LONG_BOX_BUILTIN));
}

Type *GlobalTypesHolder::GlobalFloatBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_FLOAT_BOX_BUILTIN));
}

Type *GlobalTypesHolder::GlobalDoubleBoxBuiltinType()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_DOUBLE_BOX_BUILTIN));
}

size_t GlobalTypesHolder::VariadicFunctionTypeThreshold()
{
    auto val =
        static_cast<size_t>(GlobalTypeId::ETS_FUNCTIONN_CLASS) - static_cast<size_t>(GlobalTypeId::ETS_FUNCTION0_CLASS);
    ASSERT(val == (static_cast<size_t>(GlobalTypeId::ETS_THROWING_FUNCTIONN_CLASS) -
                   static_cast<size_t>(GlobalTypeId::ETS_THROWING_FUNCTION0_CLASS)));
    ASSERT(val == (static_cast<size_t>(GlobalTypeId::ETS_RETHROWING_FUNCTIONN_CLASS) -
                   static_cast<size_t>(GlobalTypeId::ETS_RETHROWING_FUNCTION0_CLASS)));
    return val;
}

Type *GlobalTypesHolder::GlobalFunctionBuiltinType(size_t nargs, ir::ScriptFunctionFlags flags)
{
    Type *type = globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_FUNCTION0_CLASS) + nargs);

    if (nargs >= VariadicFunctionTypeThreshold()) {
        if ((flags & ir::ScriptFunctionFlags::THROWS) != 0U) {
            type = globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_THROWING_FUNCTIONN_CLASS));
        } else if ((flags & ir::ScriptFunctionFlags::RETHROWS) != 0U) {
            type = globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_RETHROWING_FUNCTIONN_CLASS));
        } else {
            type = globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_FUNCTIONN_CLASS));
        }
    } else if ((flags & ir::ScriptFunctionFlags::THROWS) != 0U) {
        type = globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_THROWING_FUNCTION0_CLASS) + nargs);
    } else if ((flags & ir::ScriptFunctionFlags::RETHROWS) != 0U) {
        type = globalTypes_.at(static_cast<size_t>(GlobalTypeId::ETS_RETHROWING_FUNCTION0_CLASS) + nargs);
    }

    return type;
}

Type *GlobalTypesHolder::GlobalTypeError()
{
    return globalTypes_.at(static_cast<size_t>(GlobalTypeId::TYPE_ERROR));
}

void GlobalTypesHolder::InitializeBuiltin(const util::StringView name, Type *type)
{
    const auto typeId = builtinNameMappings_.find(name);
    if (typeId == builtinNameMappings_.end()) {
        util::Helpers::LogDebug("Did not find '", name, "' builtin in GlobalTypesHolder, it should be added.");
        return;
    }
    globalTypes_.at(static_cast<size_t>(typeId->second)) = type;
}
}  // namespace ark::es2panda::checker
