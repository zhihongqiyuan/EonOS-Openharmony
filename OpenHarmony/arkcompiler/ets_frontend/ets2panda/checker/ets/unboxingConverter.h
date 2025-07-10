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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_UNBOXING_CONVERTER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_UNBOXING_CONVERTER_H

#include "checker/ets/typeConverter.h"
#include "checker/types/ets/etsObjectType.h"

namespace ark::es2panda::checker {

class UnboxingConverter : public TypeConverter {
public:
    UnboxingConverter(ETSChecker *checker, TypeRelation *relation, Type *source)
        : TypeConverter(checker, relation, nullptr, source)
    {
        if (!source->IsETSUnboxableObject()) {
            relation->Result(false);
            return;
        }

        SetResult(Convert(checker, source->AsETSObjectType()));
        relation->Result(source != Result());
    }

    UnboxingConverter(ETSChecker *checker, TypeRelation *relation, Type *source, Type *target)
        : TypeConverter(checker, relation, target, source)
    {
        SetResult(Source());

        if (!Source()->IsETSUnboxableObject() || relation->IsTrue()) {
            return;
        }

        SetResult(Convert(checker, source->AsETSObjectType()));
        Relation()->Result(Result()->TypeFlags() == target->TypeFlags());
    }

    static checker::Type *Convert(checker::ETSChecker const *checker, ETSObjectType *type);
};
}  // namespace ark::es2panda::checker

#endif
