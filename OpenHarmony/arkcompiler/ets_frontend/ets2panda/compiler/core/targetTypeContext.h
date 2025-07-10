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

#ifndef ES2PANDA_COMPILER_CORE_TARGET_TYPE_CONTEXT_H
#define ES2PANDA_COMPILER_CORE_TARGET_TYPE_CONTEXT_H

#include "macros.h"

namespace ark::es2panda::checker {
class Type;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::compiler {
class ETSGen;

class TargetTypeContext {
public:
    explicit TargetTypeContext(ETSGen *etsg, const checker::Type *targetType);
    NO_COPY_SEMANTIC(TargetTypeContext);
    NO_MOVE_SEMANTIC(TargetTypeContext);

    ~TargetTypeContext();

    void *operator new(size_t) = delete;
    void *operator new[](size_t) = delete;

private:
    ETSGen *etsg_;
    const checker::Type *prev_;
};
}  // namespace ark::es2panda::compiler

#endif
