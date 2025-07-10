/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "JSCompiler.h"

namespace ark::es2panda::compiler {

// from as folder
void JSCompiler::Compile([[maybe_unused]] const ir::NamedType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::PrefixAssertionExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ClassProperty *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ClassStaticBlock *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::Decorator *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::MethodDefinition *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::Property *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ScriptFunction *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::SpreadElement *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TemplateElement *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSIndexSignature *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSMethodSignature *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSPropertySignature *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSSignatureDeclaration *node) const
{
    UNREACHABLE();
}
// from ets folder
void JSCompiler::Compile([[maybe_unused]] const ir::ETSScript *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSClassLiteral *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSFunctionType *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSTuple *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSImportDeclaration *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSReExportDeclaration *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSLaunchExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSNewArrayInstanceExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSNewClassInstanceExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSNewMultiDimArrayInstanceExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSPackageDeclaration *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSParameterExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSPrimitiveType *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSStructDeclaration *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSTypeReference *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSTypeReferencePart *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSNullType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSUndefinedType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSNeverType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSStringLiteralType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSUnionType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ETSWildcardType *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::BlockExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::OpaqueTypeNode *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::OmittedExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::CharLiteral *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::UndefinedLiteral *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ExportSpecifier *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ImportDefaultSpecifier *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ImportNamespaceSpecifier *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::ImportSpecifier *st) const
{
    UNREACHABLE();
}
// Compile methods for STATEMENTS in alphabetical order
void JSCompiler::Compile([[maybe_unused]] const ir::AssertStatement *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::SwitchCaseStatement *st) const
{
    UNREACHABLE();
}

// from ts folder
void JSCompiler::Compile([[maybe_unused]] const ir::TSAnyKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSArrayType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSAsExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSBigintKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSBooleanKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSClassImplements *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSConditionalType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSConstructorType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSEnumDeclaration *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSEnumMember *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSExternalModuleReference *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSFunctionType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSImportEqualsDeclaration *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSImportType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSIndexedAccessType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSInferType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSInterfaceBody *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSInterfaceDeclaration *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSInterfaceHeritage *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSIntersectionType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSLiteralType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSMappedType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSModuleBlock *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSModuleDeclaration *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSNamedTupleMember *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSNeverKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSNonNullExpression *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSNullKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSNumberKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSObjectKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSParameterProperty *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSParenthesizedType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSQualifiedName *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSStringKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSThisType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTupleType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeAliasDeclaration *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeAssertion *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeLiteral *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeOperator *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeParameter *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeParameterDeclaration *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeParameterInstantiation *expr) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypePredicate *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeQuery *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSTypeReference *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSUndefinedKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSUnionType *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSUnknownKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::TSVoidKeyword *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::DummyNode *node) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::NamespaceDeclaration *st) const
{
    UNREACHABLE();
}

void JSCompiler::Compile([[maybe_unused]] const ir::NamespaceDefinition *st) const
{
    UNREACHABLE();
}
}  // namespace ark::es2panda::compiler
