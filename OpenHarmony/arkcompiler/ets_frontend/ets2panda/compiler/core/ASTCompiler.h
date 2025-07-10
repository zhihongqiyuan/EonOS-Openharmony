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

#ifndef ES2PANDA_COMPILER_CORE_ASTCOMPILER_H
#define ES2PANDA_COMPILER_CORE_ASTCOMPILER_H

#include "compiler/core/dynamicContext.h"
#include "ir/opaqueTypeNode.h"
#include "ir/as/namedType.h"
#include "ir/as/prefixAssertionExpression.h"
#include "ir/base/catchClause.h"
#include "ir/base/classDefinition.h"
#include "ir/base/classProperty.h"
#include "ir/base/classStaticBlock.h"
#include "ir/base/decorator.h"
#include "ir/base/metaProperty.h"
#include "ir/base/methodDefinition.h"
#include "ir/base/property.h"
#include "ir/base/scriptFunction.h"
#include "ir/base/spreadElement.h"
#include "ir/base/templateElement.h"
#include "ir/base/tsIndexSignature.h"
#include "ir/base/tsMethodSignature.h"
#include "ir/base/tsPropertySignature.h"
#include "ir/base/tsSignatureDeclaration.h"
#include "ir/ets/etsClassLiteral.h"
#include "ir/ets/etsFunctionType.h"
#include "ir/ets/etsImportDeclaration.h"
#include "ir/ets/etsLaunchExpression.h"
#include "ir/ets/etsNewArrayInstanceExpression.h"
#include "ir/ets/etsNewClassInstanceExpression.h"
#include "ir/ets/etsNewMultiDimArrayInstanceExpression.h"
#include "ir/ets/etsPackageDeclaration.h"
#include "ir/ets/etsParameterExpression.h"
#include "ir/ets/etsPrimitiveType.h"
#include "ir/ets/etsScript.h"
#include "ir/ets/etsStructDeclaration.h"
#include "ir/ets/etsTypeReference.h"
#include "ir/ets/etsTypeReferencePart.h"
#include "ir/ets/etsWildcardType.h"
#include "ir/expressions/arrayExpression.h"
#include "ir/expressions/arrowFunctionExpression.h"
#include "ir/expressions/assignmentExpression.h"
#include "ir/expressions/awaitExpression.h"
#include "ir/expressions/binaryExpression.h"
#include "ir/expressions/blockExpression.h"
#include "ir/expressions/callExpression.h"
#include "ir/expressions/chainExpression.h"
#include "ir/expressions/classExpression.h"
#include "ir/expressions/conditionalExpression.h"
#include "ir/expressions/directEvalExpression.h"
#include "ir/expressions/functionExpression.h"
#include "ir/expressions/identifier.h"
#include "ir/expressions/importExpression.h"
#include "ir/expressions/memberExpression.h"
#include "ir/expressions/newExpression.h"
#include "ir/expressions/objectExpression.h"
#include "ir/expressions/omittedExpression.h"
#include "ir/expressions/sequenceExpression.h"
#include "ir/expressions/superExpression.h"
#include "ir/expressions/taggedTemplateExpression.h"
#include "ir/expressions/templateLiteral.h"
#include "ir/expressions/thisExpression.h"
#include "ir/expressions/unaryExpression.h"
#include "ir/expressions/updateExpression.h"
#include "ir/expressions/yieldExpression.h"
#include "ir/expressions/literals/bigIntLiteral.h"
#include "ir/expressions/literals/booleanLiteral.h"
#include "ir/expressions/literals/charLiteral.h"
#include "ir/expressions/literals/nullLiteral.h"
#include "ir/expressions/literals/numberLiteral.h"
#include "ir/expressions/literals/regExpLiteral.h"
#include "ir/expressions/literals/stringLiteral.h"
#include "ir/module/exportAllDeclaration.h"
#include "ir/module/exportDefaultDeclaration.h"
#include "ir/module/exportNamedDeclaration.h"
#include "ir/module/exportSpecifier.h"
#include "ir/module/importDeclaration.h"
#include "ir/module/importDefaultSpecifier.h"
#include "ir/module/importNamespaceSpecifier.h"
#include "ir/module/importSpecifier.h"
#include "ir/statements/assertStatement.h"
#include "ir/statements/blockStatement.h"
#include "ir/statements/breakStatement.h"
#include "ir/statements/classDeclaration.h"
#include "ir/statements/continueStatement.h"
#include "ir/statements/debuggerStatement.h"
#include "ir/statements/doWhileStatement.h"
#include "ir/statements/emptyStatement.h"
#include "ir/statements/expressionStatement.h"
#include "ir/statements/forInStatement.h"
#include "ir/statements/forOfStatement.h"
#include "ir/statements/forUpdateStatement.h"
#include "ir/statements/functionDeclaration.h"
#include "ir/statements/ifStatement.h"
#include "ir/statements/labelledStatement.h"
#include "ir/statements/returnStatement.h"
#include "ir/statements/switchCaseStatement.h"
#include "ir/statements/switchStatement.h"
#include "ir/statements/throwStatement.h"
#include "ir/statements/tryStatement.h"
#include "ir/statements/variableDeclaration.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/statements/whileStatement.h"
#include "ir/ts/tsAnyKeyword.h"
#include "ir/ts/tsArrayType.h"
#include "ir/ts/tsAsExpression.h"
#include "ir/ts/tsBigintKeyword.h"
#include "ir/ts/tsBooleanKeyword.h"
#include "ir/ts/tsClassImplements.h"
#include "ir/ts/tsConditionalType.h"
#include "ir/ts/tsConstructorType.h"
#include "ir/ts/tsEnumDeclaration.h"
#include "ir/ts/tsEnumMember.h"
#include "ir/ts/tsExternalModuleReference.h"
#include "ir/ts/tsFunctionType.h"
#include "ir/ts/tsImportEqualsDeclaration.h"
#include "ir/ts/tsImportType.h"
#include "ir/ts/tsIndexedAccessType.h"
#include "ir/ts/tsInferType.h"
#include "ir/ts/tsInterfaceBody.h"
#include "ir/ts/tsInterfaceDeclaration.h"
#include "ir/ts/tsInterfaceHeritage.h"
#include "ir/ts/tsIntersectionType.h"
#include "ir/ts/tsLiteralType.h"
#include "ir/ts/tsMappedType.h"
#include "ir/ts/tsModuleBlock.h"
#include "ir/ts/tsModuleDeclaration.h"
#include "ir/ts/tsNamedTupleMember.h"
#include "ir/ts/tsNeverKeyword.h"
#include "ir/ts/tsNonNullExpression.h"
#include "ir/ts/tsNullKeyword.h"
#include "ir/ts/tsNumberKeyword.h"
#include "ir/ts/tsObjectKeyword.h"
#include "ir/ts/tsParameterProperty.h"
#include "ir/ts/tsParenthesizedType.h"
#include "ir/ts/tsQualifiedName.h"
#include "ir/ts/tsStringKeyword.h"
#include "ir/ts/tsThisType.h"
#include "ir/ts/tsTupleType.h"
#include "ir/ts/tsTypeAliasDeclaration.h"
#include "ir/ts/tsTypeAssertion.h"
#include "ir/ts/tsTypeLiteral.h"
#include "ir/ts/tsTypeOperator.h"
#include "ir/ts/tsTypeParameterDeclaration.h"
#include "ir/ts/tsTypeParameter.h"
#include "ir/ts/tsTypeParameterInstantiation.h"
#include "ir/ts/tsTypePredicate.h"
#include "ir/ts/tsTypeQuery.h"
#include "ir/ts/tsTypeReference.h"
#include "ir/ts/tsUndefinedKeyword.h"
#include "ir/ts/tsUnionType.h"
#include "ir/ts/tsUnknownKeyword.h"
#include "ir/ts/tsVoidKeyword.h"

namespace ark::es2panda::compiler {
class CodeGen;

class AstCompiler {
public:
    AstCompiler()
    {
        cg_ = nullptr;
    }
    virtual ~AstCompiler() = default;
    NO_COPY_SEMANTIC(AstCompiler);
    NO_MOVE_SEMANTIC(AstCompiler);
/* CC-OFFNXT(G.PRE.02,G.PRE.09) name part*/
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_AST_NODE_COMPILE_METHOD(_, nodeType) virtual void Compile(const ir::nodeType *node) const = 0;
    AST_NODE_MAPPING(DECLARE_AST_NODE_COMPILE_METHOD)
#undef DECLARE_AST_NODE_COMPILE_METHOD
/* CC-OFFNXT(G.PRE.02,G.PRE.09) name part*/
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define DECLARE_AST_NODE_COMPILE_METHOD(_, __, nodeType, ___) virtual void Compile(const ir::nodeType *node) const = 0;
    AST_NODE_REINTERPRET_MAPPING(DECLARE_AST_NODE_COMPILE_METHOD)
#undef DECLARE_AST_NODE_COMPILE_METHOD

    void SetCodeGen(CodeGen *cg)
    {
        cg_ = cg;
    }

protected:
    CodeGen *GetCodeGen() const
    {
        return cg_;
    }

private:
    CodeGen *cg_;
};

}  // namespace ark::es2panda::compiler

#endif  // ES2PANDA_COMPILER_CORE_ASTCOMPILER_H
