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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_ARITHMETIC_H
#define ES2PANDA_COMPILER_CHECKER_ETS_ARITHMETIC_H

#include "checker/ETSchecker.h"

namespace ark::es2panda::checker {

template <typename TargetType>
typename TargetType::UType ETSChecker::GetOperand(Type *type)
{
    switch (ETSType(type)) {
        case TypeFlag::BYTE: {
            return type->AsByteType()->GetValue();
        }
        case TypeFlag::CHAR: {
            return type->AsCharType()->GetValue();
        }
        case TypeFlag::SHORT: {
            return type->AsShortType()->GetValue();
        }
        case TypeFlag::INT: {
            return type->AsIntType()->GetValue();
        }
        case TypeFlag::LONG: {
            return type->AsLongType()->GetValue();
        }
        case TypeFlag::FLOAT: {
            return type->AsFloatType()->GetValue();
        }
        case TypeFlag::DOUBLE: {
            return type->AsDoubleType()->GetValue();
        }
        case TypeFlag::ETS_BOOLEAN: {
            return type->AsETSBooleanType()->GetValue();
        }
        default: {
            UNREACHABLE();
        }
    }
}

template <typename TargetType>
Type *ETSChecker::PerformRelationOperationOnTypes(Type *left, Type *right, lexer::TokenType operationType)
{
    using UType = typename TargetType::UType;

    UType leftValue = GetOperand<TargetType>(left);
    UType rightValue = GetOperand<TargetType>(right);

    bool result {};
    switch (operationType) {
        case lexer::TokenType::PUNCTUATOR_LESS_THAN: {
            result = leftValue < rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LESS_THAN_EQUAL: {
            result = leftValue <= rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN: {
            result = leftValue > rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_GREATER_THAN_EQUAL: {
            result = leftValue >= rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_EQUAL: {
            result = leftValue == rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_NOT_EQUAL: {
            result = leftValue != rightValue;
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    return CreateETSBooleanType(result);
}

template <typename TargetType>
Type *ETSChecker::PerformArithmeticOperationOnTypes(Type *left, Type *right, lexer::TokenType operationType)
{
    using UType = typename TargetType::UType;

    UType leftValue = GetOperand<TargetType>(left);
    UType rightValue = GetOperand<TargetType>(right);
    auto result = leftValue;
    auto isForbiddenZeroDivision = [&rightValue]() { return std::is_integral<UType>::value && rightValue == 0; };

    switch (operationType) {
        case lexer::TokenType::PUNCTUATOR_PLUS:
        case lexer::TokenType::PUNCTUATOR_PLUS_EQUAL: {
            result = leftValue + rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MINUS:
        case lexer::TokenType::PUNCTUATOR_MINUS_EQUAL: {
            result = leftValue - rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_DIVIDE:
        case lexer::TokenType::PUNCTUATOR_DIVIDE_EQUAL: {
            if (isForbiddenZeroDivision()) {
                return nullptr;
            }
            result = leftValue / rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MULTIPLY:
        case lexer::TokenType::PUNCTUATOR_MULTIPLY_EQUAL: {
            result = leftValue * rightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_MOD:
        case lexer::TokenType::PUNCTUATOR_MOD_EQUAL: {
            if (isForbiddenZeroDivision()) {
                return nullptr;
            }
            result = HandleModulo<UType>(leftValue, rightValue);
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    return Allocator()->New<TargetType>(result);
}

template <>
inline IntType::UType ark::es2panda::checker::ETSChecker::HandleModulo<IntType::UType>(IntType::UType leftValue,
                                                                                       IntType::UType rightValue)
{
    ASSERT(rightValue != 0);
    return leftValue % rightValue;
}

template <>
inline LongType::UType ark::es2panda::checker::ETSChecker::HandleModulo<LongType::UType>(LongType::UType leftValue,
                                                                                         LongType::UType rightValue)
{
    ASSERT(rightValue != 0);
    return leftValue % rightValue;
}

template <>
inline FloatType::UType ark::es2panda::checker::ETSChecker::HandleModulo<FloatType::UType>(FloatType::UType leftValue,
                                                                                           FloatType::UType rightValue)
{
    return std::fmod(leftValue, rightValue);
}

template <>
inline DoubleType::UType ark::es2panda::checker::ETSChecker::HandleModulo<DoubleType::UType>(
    DoubleType::UType leftValue, DoubleType::UType rightValue)
{
    return std::fmod(leftValue, rightValue);
}

template <typename IntegerUType, typename FloatOrIntegerUType>
inline IntegerUType CastIfFloat(FloatOrIntegerUType num)
{
    if constexpr (std::is_floating_point_v<FloatOrIntegerUType>) {
        return CastFloatToInt<FloatOrIntegerUType, IntegerUType>(num);
    } else {
        return num;
    }
}

template <typename FloatOrIntegerType, typename IntegerType>
Type *ETSChecker::HandleBitWiseArithmetic(Type *left, Type *right, lexer::TokenType operationType)
{
    using IntegerUType = typename IntegerType::UType;
    using UnsignedUType = std::make_unsigned_t<IntegerUType>;

    UnsignedUType result = 0;
    UnsignedUType unsignedLeftValue = CastIfFloat<IntegerUType>(GetOperand<FloatOrIntegerType>(left));
    UnsignedUType unsignedRightValue = CastIfFloat<IntegerUType>(GetOperand<FloatOrIntegerType>(right));

    auto mask = std::numeric_limits<UnsignedUType>::digits - 1U;
    auto shift = unsignedRightValue & mask;

    switch (operationType) {
        case lexer::TokenType::PUNCTUATOR_BITWISE_AND:
        case lexer::TokenType::PUNCTUATOR_BITWISE_AND_EQUAL: {
            result = unsignedLeftValue & unsignedRightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_BITWISE_OR:
        case lexer::TokenType::PUNCTUATOR_BITWISE_OR_EQUAL: {
            result = unsignedLeftValue | unsignedRightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_BITWISE_XOR:
        case lexer::TokenType::PUNCTUATOR_BITWISE_XOR_EQUAL: {
            result = unsignedLeftValue ^ unsignedRightValue;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_LEFT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_LEFT_SHIFT_EQUAL: {
            static_assert(sizeof(UnsignedUType) == 4 || sizeof(UnsignedUType) == 8);
            result = unsignedLeftValue << shift;
            break;
        }
        case lexer::TokenType::PUNCTUATOR_RIGHT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_RIGHT_SHIFT_EQUAL: {
            static_assert(sizeof(IntegerUType) == 4 || sizeof(IntegerUType) == 8);
            result = static_cast<IntegerUType>(unsignedLeftValue) >> shift;  // NOLINT(hicpp-signed-bitwise)
            break;
        }
        case lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT:
        case lexer::TokenType::PUNCTUATOR_UNSIGNED_RIGHT_SHIFT_EQUAL: {
            static_assert(sizeof(UnsignedUType) == 4 || sizeof(UnsignedUType) == 8);
            result = unsignedLeftValue >> shift;
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    return Allocator()->New<IntegerType>(result);
}
}  // namespace ark::es2panda::checker

#endif
