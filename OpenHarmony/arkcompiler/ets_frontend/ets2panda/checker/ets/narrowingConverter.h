/*
 * Copyright (c) 2021 - 2024 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_NARROWING_CONVERTER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_NARROWING_CONVERTER_H

#include "checker/ets/typeConverter.h"
#include "checker/ETSchecker.h"
#include "util/helpers.h"

namespace ark::es2panda::checker {
class NarrowingConverter : public TypeConverter {
public:
    static constexpr TypeFlag NARROWABLE_TO_FLOAT = TypeFlag::DOUBLE;
    static constexpr TypeFlag NARROWABLE_TO_LONG = TypeFlag::FLOAT | NARROWABLE_TO_FLOAT;
    static constexpr TypeFlag NARROWABLE_TO_INT = TypeFlag::LONG | NARROWABLE_TO_LONG;
    static constexpr TypeFlag NARROWABLE_TO_CHAR = TypeFlag::SHORT | TypeFlag::INT | NARROWABLE_TO_INT;
    static constexpr TypeFlag NARROWABLE_TO_SHORT = TypeFlag::CHAR | TypeFlag::INT | NARROWABLE_TO_INT;
    static constexpr TypeFlag NARROWABLE_TO_BYTE = TypeFlag::CHAR | NARROWABLE_TO_CHAR;

    explicit NarrowingConverter(ETSChecker *checker, TypeRelation *relation, Type *target, Type *source)
        : TypeConverter(checker, relation, target, source)
    {
        if (!relation->ApplyNarrowing()) {
            return;
        }

        ASSERT(relation->GetNode());

        switch (ETSChecker::ETSChecker::ETSType(target)) {
            case TypeFlag::BYTE: {
                ApplyNarrowing<ByteType>(NARROWABLE_TO_BYTE);
                break;
            }
            case TypeFlag::CHAR: {
                ApplyNarrowing<CharType>(NARROWABLE_TO_CHAR);
                break;
            }
            case TypeFlag::SHORT: {
                ApplyNarrowing<ShortType>(NARROWABLE_TO_SHORT);
                break;
            }
            case TypeFlag::INT: {
                ApplyNarrowing<IntType>(NARROWABLE_TO_INT);
                break;
            }
            case TypeFlag::LONG: {
                ApplyNarrowing<LongType>(NARROWABLE_TO_LONG);
                break;
            }
            case TypeFlag::FLOAT: {
                ApplyNarrowing<FloatType>(NARROWABLE_TO_FLOAT);
                break;
            }

            default: {
                break;
            }
        }
    }

private:
    template <typename TargetType>
    void ApplyNarrowing(TypeFlag flag)
    {
        if (!Source()->HasTypeFlag(flag)) {
            return;
        }

        switch (ETSChecker::ETSChecker::ETSType(Source())) {
            case TypeFlag::CHAR: {
                ApplyNarrowing<TargetType, CharType>();
                break;
            }
            case TypeFlag::SHORT: {
                ApplyNarrowing<TargetType, ShortType>();
                break;
            }
            case TypeFlag::INT: {
                ApplyNarrowing<TargetType, IntType>();
                break;
            }
            case TypeFlag::LONG: {
                ApplyNarrowing<TargetType, LongType>();
                break;
            }
            case TypeFlag::FLOAT: {
                ApplyNarrowing<TargetType, FloatType>();
                break;
            }
            case TypeFlag::DOUBLE: {
                ApplyNarrowing<TargetType, DoubleType>();
                break;
            }
            default: {
                break;
            }
        }
    }

    template <typename From, typename To>
    To CastFloatingPointToIntOrLong(From value)
    {
        if (std::isinf(value)) {
            if (std::signbit(value)) {
                return std::numeric_limits<To>::min();
            }
            return std::numeric_limits<To>::max();
        }
        ASSERT(std::is_floating_point_v<From>);
        ASSERT(std::is_integral_v<To>);
        To minInt = std::numeric_limits<To>::min();
        To maxInt = std::numeric_limits<To>::max();
        auto floatMinInt = static_cast<From>(minInt);
        auto floatMaxInt = static_cast<From>(maxInt);

        if (value > floatMinInt) {
            if (value < floatMaxInt) {
                return static_cast<To>(value);
            }
            return maxInt;
        }
        if (std::isnan(value)) {
            return 0;
        }
        return minInt;
    }

    template <typename TType, typename SType>
    TType CalculateNarrowedValue(Type *target, Type *source, SType value)
    {
        switch (ETSChecker::ETSChecker::ETSType(target)) {
            case TypeFlag::BYTE:
            case TypeFlag::CHAR:
            case TypeFlag::SHORT: {
                if (source->HasTypeFlag(checker::TypeFlag::DOUBLE) || source->HasTypeFlag(checker::TypeFlag::FLOAT)) {
                    return static_cast<TType>(CastFloatingPointToIntOrLong<SType, int32_t>(value));
                }
                return static_cast<TType>(value);
            }
            case TypeFlag::INT:
            case TypeFlag::LONG: {
                if (source->HasTypeFlag(checker::TypeFlag::DOUBLE) || source->HasTypeFlag(checker::TypeFlag::FLOAT)) {
                    return CastFloatingPointToIntOrLong<SType, TType>(value);
                }
                return static_cast<TType>(value);
            }
            case TypeFlag::FLOAT:
            case TypeFlag::DOUBLE: {
                return static_cast<TType>(value);
            }
            default: {
                UNREACHABLE();
            }
        }
    }

    template <typename TargetType, typename SourceType>
    void ApplyNarrowing()
    {
        using SType = typename SourceType::UType;
        using TType = typename TargetType::UType;

        if (Source()->HasTypeFlag(TypeFlag::CONSTANT)) {
            SType value = reinterpret_cast<SourceType *>(Source())->GetValue();
            if (!Relation()->InCastingContext() && Source()->HasTypeFlag(TypeFlag::ETS_FLOATING_POINT) &&
                Target()->HasTypeFlag(TypeFlag::ETS_INTEGRAL)) {
                auto narrowedValue = CalculateNarrowedValue<TType, SType>(Target(), Source(), value);
                if (narrowedValue != value) {
                    Relation()->Result(RelationResult::ERROR);
                    return;
                }
            }

            if (Relation()->InCastingContext() || util::Helpers::IsTargetFitInSourceRange<TType, SType>(value)) {
                auto narrowedValue = CalculateNarrowedValue<TType, SType>(Target(), Source(), value);
                TargetType *newType = Checker()->Allocator()->New<TargetType>(narrowedValue);
                Relation()->GetNode()->SetTsType(newType);
                Relation()->Result(true);
                return;
            }

            Relation()->Result(RelationResult::ERROR);
            return;
        }

        Relation()->Result(true);
    }
};
}  // namespace ark::es2panda::checker

#endif
