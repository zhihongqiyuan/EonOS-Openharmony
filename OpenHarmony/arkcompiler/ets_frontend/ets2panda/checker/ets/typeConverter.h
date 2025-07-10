/*
 * Copyright (c) 2021 - 2023 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_TYPE_CONVERTER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_TYPE_CONVERTER_H

#include "checker/types/type.h"

namespace ark::es2panda::checker {
class ETSChecker;

class TypeConverter {
public:
    TypeConverter(ETSChecker *checker, TypeRelation *relation, Type *target, Type *source);
    Type *Result() const;
    void SetResult(Type *result);
    Type *Source() const;
    Type *Target() const;
    TypeRelation *Relation() const;
    ETSChecker *Checker() const;

private:
    ETSChecker *checker_;
    TypeRelation *relation_;
    Type *target_;
    Type *source_;
    Type *result_ {};
};
}  // namespace ark::es2panda::checker

#endif
