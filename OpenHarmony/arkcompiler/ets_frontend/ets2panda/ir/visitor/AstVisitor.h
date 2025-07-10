/**
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

#ifndef ES2PANDA_COMPILER_CORE_AST_VISITOR_H
#define ES2PANDA_COMPILER_CORE_AST_VISITOR_H

#include <ir/astNodeMapping.h>
#include "macros.h"

#include <tuple>

namespace ark::es2panda::ir {
// CC-OFFNXT(G.PRE.02,G.PRE.09) name part
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_CLASSES(nodeType, className) class className;

AST_NODE_MAPPING(DECLARE_CLASSES)

#undef DECLARE_CLASSES
// CC-OFFNXT(G.PRE.02,G.PRE.09) name part
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_AST_NODE_CHECK_METHOD(_, __, nodeType, ___) class nodeType;
AST_NODE_REINTERPRET_MAPPING(DECLARE_AST_NODE_CHECK_METHOD)
#undef DECLARE_AST_NODE_CHECK_METHOD

namespace visitor {
/**
 * All such Visitors should override VisitClassName(Node*) for all types
 */
class ASTAbstractVisitor {
public:
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_CLASSES(nodeType, className)            \
    virtual void Visit##className(className *node) = 0; \
    void Accept(className *node)                        \
    {                                                   \
        Visit##className(node);                         \
    }

    AST_NODE_MAPPING(DECLARE_CLASSES)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_AST_NODE_CHECK_METHOD(nodeType1, nodeType2, baseClass, reinterpretClass) \
    DECLARE_CLASSES(nodeType1, baseClass)

    AST_NODE_REINTERPRET_MAPPING(DECLARE_AST_NODE_CHECK_METHOD)
#undef DECLARE_AST_NODE_CHECK_METHOD

#undef DECLARE_CLASSES
};
}  // namespace visitor
}  // namespace ark::es2panda::ir

#endif
