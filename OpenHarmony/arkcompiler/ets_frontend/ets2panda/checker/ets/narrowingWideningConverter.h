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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_NARROWING_WIDENING_CONVERTER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_NARROWING_WIDENING_CONVERTER_H

#include "checker/ets/narrowingConverter.h"
#include "checker/ets/wideningConverter.h"

namespace ark::es2panda::checker {
class NarrowingWideningConverter : public NarrowingConverter {
public:
    explicit NarrowingWideningConverter(ETSChecker *checker, TypeRelation *relation, Type *target, Type *source)
        : NarrowingConverter(checker, relation, target, source)
    {
        if (Relation()->IsTrue() || Relation()->IsError()) {
            return;
        }

        WideningConverter(checker, relation, target, source);
    }
};
}  // namespace ark::es2panda::checker

#endif
