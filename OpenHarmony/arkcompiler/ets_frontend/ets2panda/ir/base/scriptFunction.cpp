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

#include "scriptFunction.h"

#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {

ScriptFunction::ScriptFunction(ArenaAllocator *allocator, ScriptFunctionData &&data)
    : AstNode(AstNodeType::SCRIPT_FUNCTION, data.flags),
      irSignature_(std::move(data.signature)),
      body_(data.body),
      funcFlags_(data.funcFlags),
      lang_(data.lang),
      returnStatements_(allocator->Adapter()),
      annotations_(allocator->Adapter())
{
    for (auto *param : irSignature_.Params()) {
        param->SetParent(this);
    }

    if (auto *returnType = irSignature_.ReturnType(); returnType != nullptr) {
        returnType->SetParent(this);
    }

    if (auto *typeParams = irSignature_.TypeParams(); typeParams != nullptr) {
        typeParams->SetParent(this);
    }
}

std::size_t ScriptFunction::FormalParamsLength() const noexcept
{
    std::size_t length = 0U;

    for (const auto *param : irSignature_.Params()) {
        if (param->IsRestElement() || param->IsAssignmentPattern()) {
            break;
        }

        ++length;
    }

    return length;
}

void ScriptFunction::SetIdent(Identifier *id) noexcept
{
    id_ = id;
    id_->SetParent(this);
}

ScriptFunction *ScriptFunction::Clone(ArenaAllocator *allocator, AstNode *parent)
{
    ArenaVector<Expression *> params {allocator->Adapter()};
    ArenaVector<AnnotationUsage *> annotationUsages {allocator->Adapter()};
    for (auto *param : Params()) {
        params.push_back(param->Clone(allocator, nullptr)->AsExpression());
    }
    for (auto *annotationUsage : Annotations()) {
        annotationUsages.push_back(annotationUsage->Clone(allocator, nullptr)->AsAnnotationUsage());
    }
    auto *res = util::NodeAllocator::ForceSetParent<ScriptFunction>(
        allocator, allocator,
        ScriptFunctionData {
            body_ != nullptr ? body_->Clone(allocator, nullptr) : nullptr,
            FunctionSignature {
                TypeParams() != nullptr ? TypeParams()->Clone(allocator, nullptr)->AsTSTypeParameterDeclaration()
                                        : nullptr,
                std::move(params),
                ReturnTypeAnnotation() != nullptr ? ReturnTypeAnnotation()->Clone(allocator, nullptr)->AsTypeNode()
                                                  : nullptr},
            funcFlags_, flags_, lang_});
    res->SetParent(parent);
    res->SetAnnotations(std::move(annotationUsages));
    return res;
}

void ScriptFunction::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (id_ != nullptr) {
        if (auto *transformedNode = cb(id_); id_ != transformedNode) {
            id_->SetTransformedNode(transformationName, transformedNode);
            id_ = transformedNode->AsIdentifier();
        }
    }

    irSignature_.TransformChildren(cb, transformationName);

    if (body_ != nullptr) {
        if (auto *transformedNode = cb(body_); body_ != transformedNode) {
            body_->SetTransformedNode(transformationName, transformedNode);
            body_ = transformedNode;
        }
    }

    for (auto *&it : annotations_) {
        if (auto *transformedNode = cb(it); it != transformedNode) {
            it->SetTransformedNode(transformationName, transformedNode);
            it = transformedNode->AsAnnotationUsage();
        }
    }
}

void ScriptFunction::Iterate(const NodeTraverser &cb) const
{
    if (id_ != nullptr) {
        cb(id_);
    }
    irSignature_.Iterate(cb);
    if (body_ != nullptr) {
        cb(body_);
    }
    for (auto *it : annotations_) {
        cb(it);
    }
}

void ScriptFunction::SetReturnTypeAnnotation(TypeNode *node) noexcept
{
    irSignature_.SetReturnType(node);
    if (node != nullptr) {
        node->SetParent(this);
    }
}

void ScriptFunction::Dump(ir::AstDumper *dumper) const
{
    const char *throwMarker = nullptr;
    if (IsThrowing()) {
        throwMarker = "throws";
    } else if (IsRethrowing()) {
        throwMarker = "rethrows";
    }
    dumper->Add({{"type", "ScriptFunction"},
                 {"id", AstDumper::Nullish(id_)},
                 {"generator", IsGenerator()},
                 {"async", IsAsyncFunc()},
                 {"expression", ((funcFlags_ & ir::ScriptFunctionFlags::EXPRESSION) != 0)},
                 {"params", irSignature_.Params()},
                 {"returnType", AstDumper::Optional(irSignature_.ReturnType())},
                 {"typeParameters", AstDumper::Optional(irSignature_.TypeParams())},
                 {"declare", AstDumper::Optional(IsDeclare())},
                 {"body", AstDumper::Optional(body_)},
                 {"annotations", AstDumper::Optional(annotations_)},
                 {"throwMarker", AstDumper::Optional(throwMarker)}});
}

void ScriptFunction::Dump(ir::SrcDumper *dumper) const
{
    if (TypeParams() != nullptr) {
        dumper->Add("<");
        TypeParams()->Dump(dumper);
        dumper->Add(">");
    }
    dumper->Add("(");
    for (auto param : Params()) {
        if (param->IsETSParameterExpression() && param->AsETSParameterExpression()->Ident() != nullptr &&
            param->AsETSParameterExpression()->Ident()->Name() == varbinder::VarBinder::MANDATORY_PARAM_THIS) {
            continue;
        }
        param->Dump(dumper);
        if (param != Params().back()) {
            dumper->Add(", ");
        }
    }
    dumper->Add(")");
    if (ReturnTypeAnnotation() != nullptr) {
        dumper->Add(": ");
        ReturnTypeAnnotation()->Dump(dumper);
    }

    if (IsThrowing()) {
        dumper->Add(" throws");
    } else if (IsRethrowing()) {
        dumper->Add(" rethrows");
    }

    if (HasBody()) {
        if (IsArrow()) {
            dumper->Add(" =>");
        }
        if (body_->IsBlockStatement()) {
            dumper->Add(" {");
            if (!body_->AsBlockStatement()->Statements().empty()) {
                dumper->IncrIndent();
                dumper->Endl();
                body_->Dump(dumper);
                dumper->DecrIndent();
                dumper->Endl();
            }
            dumper->Add("}");
        } else {
            dumper->Add(" ");
            body_->Dump(dumper);
        }
    }
    if (!IsArrow()) {
        dumper->Endl();
    }
}

void ScriptFunction::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void ScriptFunction::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ScriptFunction::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ScriptFunction::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
