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

#ifndef ES2PANDA_COMPILER_CHECKER_TYPES_GLOBAL_TYPES_HOLDER_H
#define ES2PANDA_COMPILER_CHECKER_TYPES_GLOBAL_TYPES_HOLDER_H

#include "ir/astNodeFlags.h"
#include "checker/types/type.h"

namespace ark::es2panda::checker {
enum class GlobalTypeId : std::size_t {
    NUMBER,
    ANY,
    STRING,
    BOOLEAN,
    VOID,
    NULL_ID,
    UNDEFINED,
    UNKNOWN,
    NEVER,
    NON_PRIMITIVE,
    BIGINT,
    FALSE_ID,
    TRUE_ID,
    NUMBER_OR_BIGINT,
    STRING_OR_NUMBER,
    ZERO,
    EMPTY_STRING,
    ZERO_BIGINT,
    PRIMITIVE,
    EMPTY_TUPLE,
    EMPTY_OBJECT,
    RESOLVING_RETURN_TYPE,
    ERROR_TYPE,
    BYTE,
    SHORT,
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    CHAR,
    ETS_BOOLEAN,
    ETS_STRING,
    ETS_VOID,
    ETS_OBJECT_BUILTIN,
    ETS_NULL,
    ETS_UNDEFINED,
    ETS_NULLISH_TYPE,
    ETS_NEVER,
    ETS_NULLISH_OBJECT,
    ETS_WILDCARD,
    ETS_BOOLEAN_BUILTIN,
    ETS_BYTE_BUILTIN,
    ETS_CHAR_BUILTIN,
    ETS_COMPARABLE_BUILTIN,
    ETS_CONSOLE_BUILTIN,
    ETS_DATE_BUILTIN,
    ETS_DOUBLE_BUILTIN,
    ETS_EXCEPTION_BUILTIN,
    ETS_FLOAT_BUILTIN,
    ETS_FLOATING_BUILTIN,
    ETS_INTEGER_BUILTIN,
    ETS_INTEGRAL_BUILTIN,
    ETS_LONG_BUILTIN,
    ETS_MAP_BUILTIN,
    ETS_ERROR_BUILTIN,
    ETS_RUNTIME_BUILTIN,
    ETS_RUNTIME_LINKER_BUILTIN,
    ETS_SET_BUILTIN,
    ETS_SHORT_BUILTIN,
    ETS_STACK_TRACE_ELEMENT_BUILTIN,
    ETS_STACK_TRACE_BUILTIN,
    ETS_NULL_POINTER_EXCEPTION_BUILTIN,
    ETS_ARRAY_INDEX_OUT_OF_BOUNDS_ERROR_BUILTIN,
    ETS_ARITHMETIC_ERROR_BUILTIN,
    ETS_CLASS_NOT_FOUND_EXCEPTION_BUILTIN,
    ETS_CLASS_CAST_ERROR_BUILTIN,
    ETS_OUT_OF_MEMORY_ERROR_BUILTIN,
    ETS_NO_SUCH_METHOD_ERROR_BUILTIN,
    ETS_ASSERTION_ERROR_BUILTIN,
    ETS_DIVIDE_BY_ZERO_ERROR_BUILTIN,
    ETS_NULL_POINTER_ERROR_BUILTIN,
    ETS_UNCAUGHT_EXCEPTION_ERROR_BUILTIN,
    ETS_STRING_BUILTIN,
    ETS_STRING_BUILDER_BUILTIN,
    ETS_TYPE_BUILTIN,
    ETS_TYPES_BUILTIN,
    ETS_PROMISE_BUILTIN,
    ETS_REGEXP_BUILTIN,
    ETS_ARRAY_BUILTIN,
    ETS_INTEROP_JSRUNTIME_BUILTIN,
    ETS_INTEROP_JSVALUE_BUILTIN,
    ETS_BOX_BUILTIN,
    ETS_BOOLEAN_BOX_BUILTIN,
    ETS_BYTE_BOX_BUILTIN,
    ETS_CHAR_BOX_BUILTIN,
    ETS_SHORT_BOX_BUILTIN,
    ETS_INT_BOX_BUILTIN,
    ETS_LONG_BOX_BUILTIN,
    ETS_FLOAT_BOX_BUILTIN,
    ETS_DOUBLE_BOX_BUILTIN,
    ETS_BIG_INT_BUILTIN,
    ETS_BIG_INT,

    ETS_FUNCTION0_CLASS,
    ETS_FUNCTION1_CLASS,
    ETS_FUNCTION2_CLASS,
    ETS_FUNCTION3_CLASS,
    ETS_FUNCTION4_CLASS,
    ETS_FUNCTION5_CLASS,
    ETS_FUNCTION6_CLASS,
    ETS_FUNCTION7_CLASS,
    ETS_FUNCTION8_CLASS,
    ETS_FUNCTION9_CLASS,
    ETS_FUNCTION10_CLASS,
    ETS_FUNCTION11_CLASS,
    ETS_FUNCTION12_CLASS,
    ETS_FUNCTION13_CLASS,
    ETS_FUNCTION14_CLASS,
    ETS_FUNCTION15_CLASS,
    ETS_FUNCTION16_CLASS,
    ETS_FUNCTIONN_CLASS,

    ETS_THROWING_FUNCTION0_CLASS,
    ETS_THROWING_FUNCTION1_CLASS,
    ETS_THROWING_FUNCTION2_CLASS,
    ETS_THROWING_FUNCTION3_CLASS,
    ETS_THROWING_FUNCTION4_CLASS,
    ETS_THROWING_FUNCTION5_CLASS,
    ETS_THROWING_FUNCTION6_CLASS,
    ETS_THROWING_FUNCTION7_CLASS,
    ETS_THROWING_FUNCTION8_CLASS,
    ETS_THROWING_FUNCTION9_CLASS,
    ETS_THROWING_FUNCTION10_CLASS,
    ETS_THROWING_FUNCTION11_CLASS,
    ETS_THROWING_FUNCTION12_CLASS,
    ETS_THROWING_FUNCTION13_CLASS,
    ETS_THROWING_FUNCTION14_CLASS,
    ETS_THROWING_FUNCTION15_CLASS,
    ETS_THROWING_FUNCTION16_CLASS,
    ETS_THROWING_FUNCTIONN_CLASS,

    ETS_RETHROWING_FUNCTION0_CLASS,
    ETS_RETHROWING_FUNCTION1_CLASS,
    ETS_RETHROWING_FUNCTION2_CLASS,
    ETS_RETHROWING_FUNCTION3_CLASS,
    ETS_RETHROWING_FUNCTION4_CLASS,
    ETS_RETHROWING_FUNCTION5_CLASS,
    ETS_RETHROWING_FUNCTION6_CLASS,
    ETS_RETHROWING_FUNCTION7_CLASS,
    ETS_RETHROWING_FUNCTION8_CLASS,
    ETS_RETHROWING_FUNCTION9_CLASS,
    ETS_RETHROWING_FUNCTION10_CLASS,
    ETS_RETHROWING_FUNCTION11_CLASS,
    ETS_RETHROWING_FUNCTION12_CLASS,
    ETS_RETHROWING_FUNCTION13_CLASS,
    ETS_RETHROWING_FUNCTION14_CLASS,
    ETS_RETHROWING_FUNCTION15_CLASS,
    ETS_RETHROWING_FUNCTION16_CLASS,
    ETS_RETHROWING_FUNCTIONN_CLASS,

    TYPE_ERROR,

    COUNT,
};

class GlobalTypesHolder {
public:
    explicit GlobalTypesHolder(ArenaAllocator *allocator);
    ~GlobalTypesHolder() = default;
    NO_COPY_SEMANTIC(GlobalTypesHolder);
    NO_MOVE_SEMANTIC(GlobalTypesHolder);

    void AddETSEscompatLayer();
    void AddEtsSpecificTypes(ArenaAllocator *allocator);
    void AddEtsSpecificBuiltinTypes();
    void AddTSSpecificTypes(ArenaAllocator *allocator);
    void AddFunctionTypes(ArenaAllocator *allocator);

    // TS specific types
    Type *GlobalNumberType();
    Type *GlobalAnyType();
    Type *GlobalStringType();
    Type *GlobalBooleanType();
    Type *GlobalVoidType();
    Type *GlobalNullType();
    Type *GlobalUndefinedType();
    Type *GlobalUnknownType();
    Type *GlobalNeverType();
    Type *GlobalNonPrimitiveType();
    Type *GlobalBigintType();
    Type *GlobalFalseType();
    Type *GlobalTrueType();
    Type *GlobalNumberOrBigintType();
    Type *GlobalStringOrNumberType();
    Type *GlobalZeroType();
    Type *GlobalEmptyStringType();
    Type *GlobalZeroBigintType();
    Type *GlobalPrimitiveType();
    Type *GlobalEmptyTupleType();
    Type *GlobalEmptyObjectType();
    Type *GlobalResolvingReturnType();
    Type *GlobalErrorType();

    // ETS specific types
    Type *GlobalByteType();
    Type *GlobalShortType();
    Type *GlobalIntType();
    Type *GlobalLongType();
    Type *GlobalFloatType();
    Type *GlobalDoubleType();
    Type *GlobalCharType();
    Type *GlobalETSBooleanType();
    Type *GlobalETSStringLiteralType();
    Type *GlobalETSVoidType();
    Type *GlobalETSObjectType();
    Type *GlobalETSNullType();
    Type *GlobalETSUndefinedType();
    Type *GlobalETSNeverType();
    Type *GlobalETSNullishType();
    Type *GlobalETSNullishObjectType();
    Type *GlobalWildcardType();
    Type *GlobalETSBooleanBuiltinType();
    Type *GlobalByteBuiltinType();
    Type *GlobalCharBuiltinType();
    Type *GlobalComparableBuiltinType();
    Type *GlobalConsoleBuiltinType();
    Type *GlobalDoubleBuiltinType();
    Type *GlobalExceptionBuiltinType();
    Type *GlobalFloatBuiltinType();
    Type *GlobalFloatingBuiltinType();
    Type *GlobalIntegerBuiltinType();
    Type *GlobalIntegralBuiltinType();
    Type *GlobalLongBuiltinType();
    Type *GlobalErrorBuiltinType();
    Type *GlobalRuntimeBuiltinType();
    Type *GlobalShortBuiltinType();
    Type *GlobalStackTraceElementBuiltinType();
    Type *GlobalStackTraceBuiltinType();
    Type *GlobalNullPointerExceptionBuiltinType();
    Type *GlobalArrayIndexOutOfBoundsErrorBuiltinType();
    Type *GlobalArithmeticErrorBuiltinType();
    Type *GlobalClassNotFoundExceptionBuiltinType();
    [[nodiscard]] Type *GlobalClassCastErrorBuiltinType() const noexcept;
    Type *GlobalETSStringBuiltinType();
    Type *GlobalETSBigIntBuiltinType();
    Type *GlobalETSBigIntLiteralType();
    Type *GlobalStringBuilderBuiltinType();
    Type *GlobalTypeBuiltinType();
    Type *GlobalTypesBuiltinType();
    Type *GlobalPromiseBuiltinType();
    Type *GlobalBoxBuiltinType();
    Type *GlobalBooleanBoxBuiltinType();
    Type *GlobalByteBoxBuiltinType();
    Type *GlobalCharBoxBuiltinType();
    Type *GlobalShortBoxBuiltinType();
    Type *GlobalIntBoxBuiltinType();
    Type *GlobalLongBoxBuiltinType();
    Type *GlobalFloatBoxBuiltinType();
    Type *GlobalDoubleBoxBuiltinType();

    // Functional types
    size_t VariadicFunctionTypeThreshold();
    Type *GlobalFunctionBuiltinType(size_t nargs, ir::ScriptFunctionFlags flags);

    // ETS escompat layer
    Type *GlobalArrayBuiltinType();
    Type *GlobalClassOutOfMemoryErrorBuiltinType();
    Type *GlobalNoSuchMethodErrorBuiltinType();
    Type *GlobalAssertionErrorBuiltinType();
    Type *GlobalDivideByZeroErrorBuiltinType();
    Type *GlobalNullPointerErrorBuiltinType();
    Type *GlobalUncaughtExceptionErrorBuiltinType();
    Type *GlobalMapBuiltinType();
    Type *GlobalRegExpBuiltinType();
    Type *GlobalSetBuiltinType();

    // JS specific types
    Type *GlobalJSRuntimeBuiltinType();
    Type *GlobalJSValueBuiltinType();

    Type *GlobalTypeError();

    void InitializeBuiltin(util::StringView name, Type *type);

    using Holder = std::array<Type *, static_cast<size_t>(GlobalTypeId::COUNT)>;

    Holder &GlobalTypes()
    {
        return globalTypes_;
    }

    const Holder &GlobalTypes() const
    {
        return globalTypes_;
    }

private:
    Holder globalTypes_ {};
    ArenaMap<util::StringView, GlobalTypeId> builtinNameMappings_;
};
}  // namespace ark::es2panda::checker

#endif /* ES2PANDA_COMPILER_CHECKER_TYPES_TS_GLOBAL_TYPES_HOLDER_H */
