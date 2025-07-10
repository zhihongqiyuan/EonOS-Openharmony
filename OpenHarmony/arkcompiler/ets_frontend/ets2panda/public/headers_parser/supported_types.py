#!/usr/bin/env python3
# coding=utf-8
#
# Copyright (c) 2024 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


"""Types, supported by generator"""

ast_nodes_supported = [
    "AstNode",
    "ArrowFunctionExpression",
    "AssertStatement",
    "AwaitExpression",
    "BigIntLiteral",
    "BinaryExpression",
    "BlockStatement",
    "BooleanLiteral",
    "BreakStatement",
    "CallExpression",
    "CatchClause",
    "ChainExpression",
    "CharLiteral",
    "ClassDefinition",
    "ClassDeclaration",
    "ClassExpression",
    "ClassProperty",
    "ClassStaticBlock",
    "ConditionalExpression",
    "ContinueStatement",
    "DebuggerStatement",
    "Decorator",
    "DirectEvalExpression",
    "DoWhileStatement",
    "EmptyStatement",
    "ExportAllDeclaration",
    "ExportDefaultDeclaration",
    "ExportNamedDeclaration",
    "ExportSpecifier",
    "ExpressionStatement",
    "ForInStatement",
    "ForOfStatement",
    "ForUpdateStatement",
    "FunctionDeclaration",
    "FunctionExpression",
    "Identifier",
    "IfStatement",
    "ImportDeclaration",
    "ImportExpression",
    "ImportDefaultSpecifier",
    "ImportNamespaceSpecifier",
    "ImportSpecifier",
    "LabelledStatement",
    "MemberExpression",
    "MetaProperty",
    "MethodDefinition",
    "NamedType",
    "NewExpression",
    "NullLiteral",
    "UndefinedLiteral",
    "NumberLiteral",
    "OmittedExpression",
    "PrefixAssertionExpression",
    "Property",
    "RegExpLiteral",
    "ETSReExportDeclaration",
    "ReturnStatement",
    "ScriptFunction",
    "SequenceExpression",
    "StringLiteral",
    "ETSNullType",
    "ETSUndefinedType",
    "ETSFunctionType",
    "ETSWildcardType",
    "ETSPrimitiveType",
    "ETSPackageDeclaration",
    "ETSClassLiteral",
    "ETSTypeReference",
    "ETSTypeReferencePart",
    "ETSUnionType",
    "ETSLaunchExpression",
    "ETSNewArrayInstanceExpression",
    "ETSNewMultiDimArrayInstanceExpression",
    "ETSNewClassInstanceExpression",
    "ETSImportDeclaration",
    "ETSParameterExpression",
    "ETSTuple",
    "ETSScript",
    "SuperExpression",
    "ETSStructDeclaration",
    "SwitchCaseStatement",
    "SwitchStatement",
    "TSEnumDeclaration",
    "TSEnumMember",
    "TSExternalModuleReference",
    "TSNumberKeyword",
    "TSAnyKeyword",
    "TSStringKeyword",
    "TSBooleanKeyword",
    "TSVoidKeyword",
    "TSUndefinedKeyword",
    "TSUnknownKeyword",
    "TSObjectKeyword",
    "TSBigintKeyword",
    "TSNeverKeyword",
    "TSNonNullExpression",
    "TSNullKeyword",
    "TSArrayType",
    "TSUnionType",
    "TSTypeLiteral",
    "TSPropertySignature",
    "TSMethodSignature",
    "TSSignatureDeclaration",
    "TSParenthesizedType",
    "TSLiteralType",
    "TSInferType",
    "TSConditionalType",
    "TSImportType",
    "TSIntersectionType",
    "TSMappedType",
    "TSModuleBlock",
    "TSThisType",
    "TSTypeOperator",
    "TSTypeParameter",
    "TSTypeParameterDeclaration",
    "TSTypeParameterInstantiation",
    "TSTypePredicate",
    "TSParameterProperty",
    "TSModuleDeclaration",
    "TSImportEqualsDeclaration",
    "TSFunctionType",
    "TSConstructorType",
    "TSTypeAliasDeclaration",
    "TSTypeReference",
    "TSQualifiedName",
    "TSIndexedAccessType",
    "TSInterfaceDeclaration",
    "TSInterfaceBody",
    "TSInterfaceHeritage",
    "TSTupleType",
    "TSNamedTupleMember",
    "TSIndexSignature",
    "TSTypeQuery",
    "TSAsExpression",
    "TSClassImplements",
    "TSTypeAssertion",
    "TaggedTemplateExpression",
    "TemplateElement",
    "TemplateLiteral",
    "ThisExpression",
    "TypeofExpression",
    "ThrowStatement",
    "TryStatement",
    "UnaryExpression",
    "UpdateExpression",
    "VariableDeclaration",
    "VariableDeclarator",
    "WhileStatement",
    "YieldExpression",
    "OpaqueTypeNode",
    "BlockExpression",
    "Statement",
    "Expression",
]

all_types_supported = [

    # Cpp types
    "char",
    "short",
    "int",
    "long",
    "long long",
    "float",
    "double",
    "long double",
    "bool",
    "void",

    # enums
    "AstNodeFlags",
    "BoxingUnboxingFlags",
    "ModifierFlags",
    "ScriptFunctionFlags",
    "TSOperatorType",
    "MappedOption",
    "PrivateFieldKind",

    # astType
    "Type",
    "ArrayType",
    "AnyType",
    "BigintLiteralType",
    "NumberType",
    "StringType",
    "BooleanType",
    "VoidType",
    "NullType",
    "UndefinedType",
    "UnknownType",
    "NeverType",
    "UnionType",
    "ObjectType",
    "BigintType",
    "BooleanLiteralType",
    "NumberLiteralType",
    "StringLiteralType",
    "EnumType",
    "EnumLiteralType",
    "TypeParameter",
    "TypeReference",
    "ByteType",
    "ShortType",
    "IntType",
    "LongType",
    "FloatType",
    "DoubleType",
    "CharType",
    "ETSBooleanType",
    "ETSVoidType",
    "ETSNullType",
    "ETSUndefinedType",
    "ETSFunctionType",
    "ETSObjectType",
    "ETSArrayType",
    "ETSUnionType",
    "NonPrimitiveType",
    "WildcardType",
    "ETSTypeParameter",
    "ETSNonNullishType",
    "ETSEnumType",
    "ETSStringEnumType",
    "ETSExtensionFuncHelperType",
    "ETSTupleType",
    "FunctionType",
    "TupleType",
    "ObjectLiteralType",
    "InterfaceType",

    # Variable
    "Variable",
    "LocalVariable",
    "GlobalVariable",
    "ModuleVariable",
    "EnumVariable",
    "NamespaceVariable",
    "ImportEqualsVariable",
    "EnumLiteralVariable",

    # others
    "StringView",
    "ArenaAllocator",
    "Checker",
    "string",
]

all_types_supported.extend(ast_nodes_supported)

no_gen_keywords = {
    "postfix": [
        "= delete",
        "override",
    ],
    "name_starts_with": [
        "~",
    ],
    "return_type": [
        "ETSChecker",
        "ArenaAllocator",
        "Allocator",
    ],
}


def is_method_supported(function: dict) -> bool:
    if "args" in function:
        for arg in function["args"]:
            if "type" not in arg or "name" not in arg["type"]:
                raise RuntimeError(""" + arg + "",  "" + function + """)
            if arg["type"]["name"] not in all_types_supported:
                return False
    if "return_type" in function:
        if "name" in function["return_type"]:
            if function["return_type"]["name"] not in all_types_supported:
                return False

    return True


def need_to_gen(function: dict) -> bool:
    if "postfix" in function:
        for ban in no_gen_keywords["postfix"]: # CC-OFF(G.TYP.07) dict key exist
            if function["postfix"].find(ban) != -1:
                return False
    for name_start in no_gen_keywords["name_starts_with"]: # CC-OFF(G.TYP.07) dict key exist
        if function["name"].startswith(name_start):
            return False
    if "return_type" in function:
        for ban in no_gen_keywords["return_type"]: # CC-OFF(G.TYP.07) dict key exist
            if "name" in function["return_type"] and function["return_type"]["name"].find(ban) != -1:
                return False
    return True
