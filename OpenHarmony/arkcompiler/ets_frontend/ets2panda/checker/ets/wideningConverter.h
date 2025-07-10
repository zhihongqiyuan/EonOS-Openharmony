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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_WIDENING_CONVERTER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_WIDENING_CONVERTER_H

#include "checker/ets/typeConverter.h"
#include "checker/ETSchecker.h"

namespace ark::es2panda::checker {

class WideningConverter : public TypeConverter {
public:
    explicit WideningConverter(ETSChecker *checker, TypeRelation *relation, Type *target, Type *source)
        : TypeConverter(checker, relation, target, source)
    {
        if (!Relation()->ApplyWidening()) {
            return;
        }

        if (!Source()->HasTypeFlag(TypeFlag::CONSTANT)) {
            ApplyGlobalWidening();
        } else {
            ApplyConstWidening();
        }
    }

private:
    static constexpr TypeFlag WIDENABLE_TO_SHORT = TypeFlag::BYTE;
    static constexpr TypeFlag WIDENABLE_TO_CHAR = TypeFlag::BYTE;
    static constexpr TypeFlag WIDENABLE_TO_INT = TypeFlag::CHAR | TypeFlag::SHORT | WIDENABLE_TO_SHORT;
    static constexpr TypeFlag WIDENABLE_TO_LONG = TypeFlag::INT | WIDENABLE_TO_INT;
    static constexpr TypeFlag WIDENABLE_TO_FLOAT = TypeFlag::LONG | WIDENABLE_TO_LONG;
    static constexpr TypeFlag WIDENABLE_TO_DOUBLE = TypeFlag::FLOAT | WIDENABLE_TO_FLOAT;

    void ApplyConstWidening()
    {
        switch (ETSChecker::ETSChecker::ETSType(Target())) {
            case TypeFlag::SHORT: {
                ApplyWidening<ShortType>(WIDENABLE_TO_SHORT);
                break;
            }
            case TypeFlag::INT: {
                ApplyWidening<IntType>(WIDENABLE_TO_INT);
                break;
            }
            case TypeFlag::LONG: {
                ApplyWidening<LongType>(WIDENABLE_TO_LONG);
                break;
            }
            case TypeFlag::FLOAT: {
                ApplyWidening<FloatType>(WIDENABLE_TO_FLOAT);
                break;
            }
            case TypeFlag::DOUBLE: {
                ApplyWidening<DoubleType>(WIDENABLE_TO_DOUBLE);
                break;
            }
            default: {
                break;
            }
        }
    }

    void ApplyGlobalWidening()
    {
        switch (ETSChecker::ETSChecker::ETSType(Target())) {
            case TypeFlag::CHAR: {
                ApplyGlobalWidening(WIDENABLE_TO_CHAR);
                break;
            }
            case TypeFlag::SHORT: {
                ApplyGlobalWidening(WIDENABLE_TO_SHORT);
                break;
            }
            case TypeFlag::INT: {
                ApplyGlobalWidening(WIDENABLE_TO_INT);
                break;
            }
            case TypeFlag::LONG: {
                ApplyGlobalWidening(WIDENABLE_TO_LONG);
                break;
            }
            case TypeFlag::FLOAT: {
                ApplyGlobalWidening(WIDENABLE_TO_FLOAT);
                break;
            }
            case TypeFlag::DOUBLE: {
                ApplyGlobalWidening(WIDENABLE_TO_DOUBLE);
                break;
            }
            default: {
                break;
            }
        }
    }

    void ApplyGlobalWidening(TypeFlag flag)
    {
        if (!Source()->HasTypeFlag(flag)) {
            return;
        }

        if (!Relation()->OnlyCheckWidening()) {
            ASSERT(Relation()->GetNode());
            switch (ETSChecker::ETSChecker::ETSType(Source())) {
                case TypeFlag::BYTE: {
                    Relation()->GetNode()->SetTsType(Checker()->GlobalByteType());
                    break;
                }
                case TypeFlag::SHORT: {
                    Relation()->GetNode()->SetTsType(Checker()->GlobalShortType());
                    break;
                }
                case TypeFlag::CHAR: {
                    Relation()->GetNode()->SetTsType(Checker()->GlobalCharType());
                    break;
                }
                case TypeFlag::INT: {
                    Relation()->GetNode()->SetTsType(Checker()->GlobalIntType());
                    break;
                }
                case TypeFlag::LONG: {
                    Relation()->GetNode()->SetTsType(Checker()->GlobalLongType());
                    break;
                }
                case TypeFlag::FLOAT: {
                    Relation()->GetNode()->SetTsType(Checker()->GlobalFloatType());
                    break;
                }
                case TypeFlag::DOUBLE: {
                    Relation()->GetNode()->SetTsType(Checker()->GlobalDoubleType());
                    break;
                }
                default: {
                    return;
                }
            }
        }

        Relation()->Result(true);
    }

    template <typename TargetType>
    void ApplyWidening(TypeFlag flag)
    {
        if (!Source()->HasTypeFlag(flag)) {
            return;
        }

        switch (ETSChecker::ETSChecker::ETSType(Source())) {
            case TypeFlag::BYTE: {
                ApplyWidening<TargetType, ByteType>();
                break;
            }
            case TypeFlag::CHAR: {
                ApplyWidening<TargetType, CharType>();
                break;
            }
            case TypeFlag::SHORT: {
                ApplyWidening<TargetType, ShortType>();
                break;
            }
            case TypeFlag::INT: {
                ApplyWidening<TargetType, IntType>();
                break;
            }
            case TypeFlag::LONG: {
                ApplyWidening<TargetType, LongType>();
                break;
            }
            case TypeFlag::FLOAT: {
                ApplyWidening<TargetType, FloatType>();
                break;
            }
            case TypeFlag::DOUBLE: {
                ApplyWidening<TargetType, DoubleType>();
                break;
            }
            default: {
                return;
            }
        }
        Relation()->Result(true);
    }

    template <typename TargetType, typename SourceType>
    void ApplyWidening()
    {
        using SType = typename SourceType::UType;
        using TType = typename TargetType::UType;
        SType value = reinterpret_cast<SourceType *>(Source())->GetValue();

        if (!Relation()->OnlyCheckWidening()) {
            ASSERT(Relation()->GetNode());
            Relation()->GetNode()->SetTsType(Checker()->Allocator()->New<TargetType>(static_cast<TType>(value)));
        }
    }
};
}  // namespace ark::es2panda::checker

#endif
