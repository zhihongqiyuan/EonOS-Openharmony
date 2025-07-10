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

#include "typeConverter.h"

namespace ark::es2panda::checker {
TypeConverter::TypeConverter(ETSChecker *checker, TypeRelation *relation, Type *target, Type *source)
    : checker_(checker), relation_(relation), target_(target), source_(source)
{
}

Type *TypeConverter::Result() const
{
    return result_;
}

void TypeConverter::SetResult(Type *result)
{
    result_ = result;
}

Type *TypeConverter::Source() const
{
    return source_;
}

Type *TypeConverter::Target() const
{
    return target_;
}

TypeRelation *TypeConverter::Relation() const
{
    return relation_;
}

ETSChecker *TypeConverter::Checker() const
{
    return checker_;
}
}  // namespace ark::es2panda::checker
