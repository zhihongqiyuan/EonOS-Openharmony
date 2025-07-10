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

#ifndef ES2PANDA_CHECKER_TYPES_ERROR_TYPE_ERROR_H
#define ES2PANDA_CHECKER_TYPES_ERROR_TYPE_ERROR_H

#include "es2panda.h"
#include "checker/types/type.h"

namespace ark::es2panda::checker {

class TypeError : public Type {
public:
    explicit TypeError() : Type(TypeFlag::TYPE_ERROR) {}

    bool AssignmentSource(TypeRelation *relation, [[maybe_unused]] Type *target) override
    {
        // Don't spread the error
        relation->Result(RelationResult::TRUE);
        return true;
    }

    void AssignmentTarget(TypeRelation *relation, [[maybe_unused]] Type *source) override
    {
        // Don't spread the error
        relation->Result(RelationResult::TRUE);
    }

    void ToString(std::stringstream &ss, [[maybe_unused]] bool precise) const override
    {
        ss << "*TYPE_ERROR*";
    }
};

}  // namespace ark::es2panda::checker

#endif
