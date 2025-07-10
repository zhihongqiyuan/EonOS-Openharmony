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

#ifndef ES2PANDA_COMPILER_CORE_ITERATE_AST_VISITOR_H
#define ES2PANDA_COMPILER_CORE_ITERATE_AST_VISITOR_H

#include "AstVisitor.h"
#include "ir/expressions/literals/undefinedLiteral.h"
#include "ir/expressions/blockExpression.h"
#include "ir/ets/etsUnionType.h"
#include "ir/ets/etsStringLiteralType.h"
#include "ir/ets/etsTuple.h"
#include "ir/ets/etsNeverType.h"
#include "ir/ets/etsNullishTypes.h"
#include "ir/statements/functionDeclaration.h"
#include "ir/expressions/functionExpression.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/classProperty.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/dummyNode.h"
#include "ir/ets/etsReExportDeclaration.h"
#include "ir/statements/variableDeclaration.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/statements/namespaceDeclaration.h"

namespace ark::es2panda::ir::visitor {

namespace detail {
class DefaultBehaviourAstVisitor : public ASTAbstractVisitor {
public:
    DefaultBehaviourAstVisitor() = default;
    virtual ~DefaultBehaviourAstVisitor() = 0;
    NO_COPY_SEMANTIC(DefaultBehaviourAstVisitor);
    NO_MOVE_SEMANTIC(DefaultBehaviourAstVisitor);

    virtual void HandleNode(ir::AstNode *node) = 0;

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_CLASSES(nodeType, className)          \
    void Visit##className(className *node) override   \
    {                                                 \
        HandleNode(static_cast<ir::AstNode *>(node)); \
    }

    AST_NODE_MAPPING(DECLARE_CLASSES)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_AST_NODE_CHECK_METHOD(nodeType1, nodeType2, baseClass, reinterpretClass) \
    DECLARE_CLASSES(nodeType1, baseClass);  // CC-OFF(G.PRE.09) code gen

    AST_NODE_REINTERPRET_MAPPING(DECLARE_AST_NODE_CHECK_METHOD)
#undef DECLARE_AST_NODE_CHECK_METHOD

#undef DECLARE_CLASSES
};
inline DefaultBehaviourAstVisitor::~DefaultBehaviourAstVisitor() = default;
}  // namespace detail

/**
 * Children should noImpl VisitNode methods (might be virtual might be not)
 * for all classes or provide default behaviour using
 * template <T> VisitNode(T *t) {}
 */
class IterateAstVisitor : public detail::DefaultBehaviourAstVisitor {
public:
    IterateAstVisitor() = default;

    void Iterate(ir::AstNode *node)
    {
        if (node != nullptr) {
            node->Iterate([this](ir::AstNode *child) { child->Accept(this); });
        }
    }

    void HandleNode(ir::AstNode *node) final
    {
        Iterate(node);
    }
};

class EmptyAstVisitor : public detail::DefaultBehaviourAstVisitor {
public:
    EmptyAstVisitor() = default;

    void HandleNode(ir::AstNode * /*node*/) final {}
};

class AbortAstVisitor : public detail::DefaultBehaviourAstVisitor {
public:
    AbortAstVisitor() = default;

    void HandleNode(ir::AstNode * /*node*/) final
    {
        UNREACHABLE();
    }
};

using CustomAstVisitor = detail::DefaultBehaviourAstVisitor;

}  // namespace ark::es2panda::ir::visitor

#endif
