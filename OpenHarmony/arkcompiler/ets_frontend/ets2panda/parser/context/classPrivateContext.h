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

#ifndef ES2PANDA_PARSER_CONTEXT_CLASS_PRIVATE_CONTEXT_H
#define ES2PANDA_PARSER_CONTEXT_CLASS_PRIVATE_CONTEXT_H

#include <macros.h>
#include "util/enumbitops.h"
#include "util/ustring.h"

#include <vector>

namespace ark::es2panda::ir {
class ClassElement;
class Identifier;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::parser {
class ClassPrivateContext {
public:
    explicit ClassPrivateContext() = default;
    explicit ClassPrivateContext(ClassPrivateContext *current) : prev_(current) {}
    DEFAULT_COPY_SEMANTIC(ClassPrivateContext);
    DEFAULT_MOVE_SEMANTIC(ClassPrivateContext);
    ~ClassPrivateContext() = default;

    bool AddElement(const ir::ClassElement *elem);
    bool FindElement(const ir::Identifier *elem) const;

    const std::vector<const ir::ClassElement *> &Elements()
    {
        return elements_;
    }

    ClassPrivateContext *Prev() const
    {
        return prev_;
    }

private:
    ClassPrivateContext *prev_ {};
    std::vector<const ir::ClassElement *> elements_;
};
}  // namespace ark::es2panda::parser

#endif
