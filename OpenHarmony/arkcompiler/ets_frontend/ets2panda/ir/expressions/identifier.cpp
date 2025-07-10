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

#include "identifier.h"

#include "checker/ETSchecker.h"
#include "checker/TSchecker.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
Identifier::Identifier([[maybe_unused]] Tag const tag, Identifier const &other, ArenaAllocator *const allocator)
    : AnnotatedExpression(static_cast<AnnotatedExpression const &>(other), allocator), decorators_(allocator->Adapter())
{
    name_ = other.name_;
    flags_ = other.flags_;

    for (auto *decorator : other.decorators_) {
        decorators_.emplace_back(decorator->Clone(allocator, this));
    }
}

Identifier *Identifier::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<Identifier>(Tag {}, *this, allocator); clone != nullptr) {
        clone->SetTsType(TsType());
        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());

        return clone;
    }
    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

Identifier *Identifier::CloneReference(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<Identifier>(Tag {}, *this, allocator); clone != nullptr) {
        clone->SetTsType(TsType());
        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());

        if (clone->IsReference(ScriptExtension::ETS) && (clone->TypeAnnotation() != nullptr)) {
            clone->SetTsTypeAnnotation(nullptr);
        }

        return clone;
    }
    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

void Identifier::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (auto *typeAnnotation = TypeAnnotation(); typeAnnotation != nullptr) {
        if (auto *transformedNode = cb(typeAnnotation); typeAnnotation != transformedNode) {
            typeAnnotation->SetTransformedNode(transformationName, transformedNode);
            SetTsTypeAnnotation(static_cast<TypeNode *>(transformedNode));
        }
    }

    for (auto *&it : decorators_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsDecorator();
        }
    }
}

void Identifier::Iterate(const NodeTraverser &cb) const
{
    if (TypeAnnotation() != nullptr) {
        cb(TypeAnnotation());
    }

    for (auto *it : decorators_) {
        cb(it);
    }
}

ValidationInfo Identifier::ValidateExpression()
{
    if ((flags_ & IdentifierFlags::OPTIONAL) != 0U) {
        return {"Unexpected token '?'.", Start()};
    }

    if (TypeAnnotation() != nullptr) {
        return {"Unexpected token.", TypeAnnotation()->Start()};
    }

    ValidationInfo info;
    return info;
}

void Identifier::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", IsPrivateIdent() ? "PrivateIdentifier" : "Identifier"},
                 {"name", name_},
                 {"typeAnnotation", AstDumper::Optional(TypeAnnotation())},
                 {"optional", AstDumper::Optional(IsOptional())},
                 {"decorators", decorators_}});
}

void Identifier::Dump(ir::SrcDumper *dumper) const
{
    if (IsPrivateIdent()) {
        dumper->Add("private ");
    }

    dumper->Add(std::string(name_));
    if (IsOptional()) {
        dumper->Add("?");
    }
}

void Identifier::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void Identifier::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *Identifier::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *Identifier::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

bool Identifier::IsDeclaration(ScriptExtension ext) const
{
    // GLOBAL class is not a reference
    if (Name() == "ETSGLOBAL") {
        return true;
    }

    // We can determine reference status from parent node
    if (Parent() == nullptr) {
        return false;
    }

    auto const *const parent = Parent();

    // We need two Parts because check is too long to fit in one function
    if (CheckDeclarationsPart1(parent, ext) || CheckDeclarationsPart2(parent, ext)) {
        return true;
    }

    // This will check other cases that are not related to declarations
    return CheckNotDeclarations(parent, ext);
}

bool Identifier::CheckNotDeclarations(const ir::AstNode *parent, [[maybe_unused]] ScriptExtension ext) const
{
    if (parent->IsTSMethodSignature() && ext == ScriptExtension::TS) {
        // NOTE(kkonkuznetsov): fix in TS
        // Example: c(a: (a: number, b: void) => string, b?: number[]): undefined;
        // a, b should not be references
        return false;
    }

    // Parameters in methods are not references
    // Example:
    // function foo(a: int)
    if (parent->IsETSParameterExpression()) {
        return true;
    }

    if (parent->IsTSPropertySignature() && ext == ScriptExtension::TS) {
        // Example:
        //
        // interface foo {
        //     a: number,
        // }
        auto *prop = parent->AsTSPropertySignature();
        if (prop->Key() == this) {
            return true;
        }
    }

    if (parent->IsClassProperty()) {
        // RHS is a reference
        // Example: const foo1: (self: Object, x: int) => int = foo;
        // foo here is a reference
        auto *prop = parent->AsClassProperty();
        return !(prop->Value() == this);
    }

    // Identifier in catch is not a reference
    // Example:
    //
    // _try_{
    //   _throw_new_Exception();}_catch_(e)_{}
    if (parent->IsCatchClause()) {
        return true;
    }

    // Type Parameter is not a reference
    // Example:
    // interface X <K> {}
    if (parent->IsTSTypeParameter()) {
        return true;
    }

    // Rest Parameter is not a reference
    // Example:
    // class A {
    //    constructor(... items :Object[]){}
    // }
    if (parent->IsRestElement()) {
        if (ext == ScriptExtension::TS) {
            // NOTE(kkonkuznetsov): fix in TS
            // Example: var func8: { (...c): boolean, (...c: number[]): string };
            // c is expected to be a reference
            return false;
        }

        if (ext == ScriptExtension::JS) {
            // NOTE(kkonkuznetsov): some JS tests have the following code:
            //
            // let x;
            // async function* fn() {
            //   for await ([...x] of [g()]) {
            //   }
            // }
            // Here in `[...x]` x is parsed as Rest Element,
            // however x is expected to be a reference.
            // Otherwise the tests fail.
            return !(parent->Parent() != nullptr && parent->Parent()->IsArrayPattern());
        }

        return true;
    }

    // Script function identifiers are not references
    // Example:
    // public static foo()
    if (parent->IsScriptFunction()) {
        if (ext == ScriptExtension::TS) {
            // NOTE(kkonkuznetsov): fix in TS
            // Example: let c = (a?: number) => { }
            return false;
        }

        if (ext == ScriptExtension::JS) {
            // In some JS tests the following code:
            //
            // let_f_=_()_=>_o;
            //
            // creates AST that has Identifier as Body
            return !(parent->AsScriptFunction()->Body() == this);
        }

        return true;
    }

    // Helper function to reduce function size
    return CheckDefinitions(parent, ext);
}

bool Identifier::CheckDefinitions(const ir::AstNode *parent, [[maybe_unused]] ScriptExtension ext) const
{
    // New methods are not references
    if (parent->IsMethodDefinition()) {
        return true;
    }

    // New classes are not references
    if (parent->IsClassDefinition()) {
        if (ext == ScriptExtension::JS) {
            // Example from JS tests:
            // inner Outer is a reference
            //
            // class Outer {
            //   innerclass() {
            //     return class extends Outer {};
            //   }
            // }
            auto *def = parent->AsClassDefinition();
            return !(def->Super() == this);
        }

        return true;
    }

    return false;
}

bool Identifier::CheckDeclarationsPart1(const ir::AstNode *parent, [[maybe_unused]] ScriptExtension ext) const
{
    // All declarations are not references
    if (parent->IsVariableDeclarator()) {
        if (ext == ScriptExtension::TS) {
            // NOTE(kkonkuznetsov): fix in TS
            // All variable declarations in TS are expected to be references for some reason
            return false;
        }

        auto *decl = parent->AsVariableDeclarator();

        // Example: let goo = foo;
        // RHS is a reference
        return !(decl->Init() == this);
    }

    // All declarations are not references
    if (parent->IsVariableDeclaration()) {
        return true;
    }

    if (parent->IsClassDeclaration()) {
        return true;
    }

    if (parent->IsETSPackageDeclaration()) {
        return true;
    }

    if (parent->IsFunctionDeclaration()) {
        return true;
    }

    if (parent->IsImportDeclaration()) {
        return true;
    }

    if (parent->IsETSImportDeclaration()) {
        return true;
    }

    if (parent->IsETSStructDeclaration()) {
        return true;
    }

    if (parent->IsExportAllDeclaration()) {
        return true;
    }

    if (parent->IsExportDefaultDeclaration()) {
        return true;
    }

    return false;
}

bool Identifier::CheckDeclarationsPart2(const ir::AstNode *parent, ScriptExtension ext) const
{
    // All declarations are not references
    if (parent->IsExportNamedDeclaration()) {
        return true;
    }

    if (parent->IsTSEnumDeclaration()) {
        // NOTE(kkonkuznetsov): fix in TS
        // Should not be a reference
        // However currently some TS tests depend on declaration ident being a reference
        return !(ext == ScriptExtension::TS);
    }

    if (parent->IsTSInterfaceDeclaration()) {
        // NOTE(kkonkuznetsov): This should not be a reference
        // Example:
        //
        // interface foo {
        //     a: number,
        // }
        //
        // However currently some TS tests depend on interface ident being a reference
        return !(ext == ScriptExtension::TS);
    }

    if (parent->IsTSModuleDeclaration()) {
        return true;
    }

    if (parent->IsTSSignatureDeclaration()) {
        // NOTE(kkonkuznetsov): fix in TS
        // Example: new(a: null, b?: string): { a: number, b: string, c?([a, b]): string }
        // a, b should not be references
        return !(ext == ScriptExtension::TS);
    }

    if (parent->IsETSReExportDeclaration()) {
        return true;
    }

    if (parent->IsTSImportEqualsDeclaration()) {
        return true;
    }

    if (parent->IsTSTypeAliasDeclaration()) {
        return true;
    }

    if (parent->IsTSTypeParameterDeclaration()) {
        return true;
    }

    if (parent->IsDeclare()) {
        return true;
    }

    if (parent->Parent() != nullptr) {
        if (parent->Parent()->IsTSEnumDeclaration()) {
            return true;
        }
    }

    return false;
}

}  // namespace ark::es2panda::ir
