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

#include "etsParameterExpression.h"

#include "checker/ETSchecker.h"
#include "checker/ets/typeRelationContext.h"
#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/typeNode.h"
#include "ir/expressions/identifier.h"
#include "ir/base/spreadElement.h"

namespace ark::es2panda::ir {

ETSParameterExpression::ETSParameterExpression(AnnotatedExpression *const identOrSpread, Expression *const initializer)
    : Expression(AstNodeType::ETS_PARAMETER_EXPRESSION), initializer_(initializer)
{
    ASSERT(identOrSpread != nullptr);
    identOrSpread->SetParent(this);

    if (identOrSpread->IsIdentifier()) {
        ident_ = identOrSpread->AsIdentifier();
    } else if (identOrSpread->IsRestElement()) {
        spread_ = identOrSpread->AsRestElement();
        ASSERT(spread_->Argument()->IsIdentifier());
        ident_ = spread_->Argument()->AsIdentifier();
        ident_->SetParent(spread_);
        initializer_ = nullptr;  // Just in case!
    } else {
        UNREACHABLE();
    }
}

const Identifier *ETSParameterExpression::Ident() const noexcept
{
    return ident_;
}

Identifier *ETSParameterExpression::Ident() noexcept
{
    return ident_;
}

const SpreadElement *ETSParameterExpression::RestParameter() const noexcept
{
    return spread_;
}

SpreadElement *ETSParameterExpression::RestParameter() noexcept
{
    return spread_;
}

const Expression *ETSParameterExpression::Initializer() const noexcept
{
    return initializer_;
}

Expression *ETSParameterExpression::Initializer() noexcept
{
    return initializer_;
}

varbinder::Variable *ETSParameterExpression::Variable() const noexcept
{
    return ident_->Variable();
}

TypeNode const *ETSParameterExpression::TypeAnnotation() const noexcept
{
    return !IsRestParameter() ? ident_->TypeAnnotation() : spread_->TypeAnnotation();
}

TypeNode *ETSParameterExpression::TypeAnnotation() noexcept
{
    return !IsRestParameter() ? ident_->TypeAnnotation() : spread_->TypeAnnotation();
}

void ETSParameterExpression::SetVariable(varbinder::Variable *const variable) noexcept
{
    ident_->SetVariable(variable);
}

void ETSParameterExpression::SetLexerSaved(util::StringView s) noexcept
{
    savedLexer_ = s;
}

util::StringView ETSParameterExpression::LexerSaved() const noexcept
{
    return savedLexer_;
}

void ETSParameterExpression::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (IsRestParameter()) {
        if (auto *transformedNode = cb(spread_); spread_ != transformedNode) {
            spread_->SetTransformedNode(transformationName, transformedNode);
            spread_ = transformedNode->AsRestElement();
        }
        ident_ = spread_->Argument()->AsIdentifier();
    } else {
        if (auto *transformedNode = cb(ident_); ident_ != transformedNode) {
            ident_->SetTransformedNode(transformationName, transformedNode);
            ident_ = transformedNode->AsIdentifier();
        }
    }

    if (IsDefault()) {
        if (auto *transformedNode = cb(initializer_); initializer_ != transformedNode) {
            initializer_->SetTransformedNode(transformationName, transformedNode);
            initializer_ = transformedNode->AsExpression();
        }
    }
}

void ETSParameterExpression::Iterate(const NodeTraverser &cb) const
{
    if (IsRestParameter()) {
        cb(spread_);
    } else {
        cb(ident_);
    }

    if (IsDefault()) {
        cb(initializer_);
    }
}

void ETSParameterExpression::Dump(ir::AstDumper *const dumper) const
{
    if (!IsRestParameter()) {
        dumper->Add(
            {{"type", "ETSParameterExpression"}, {"name", ident_}, {"initializer", AstDumper::Optional(initializer_)}});
    } else {
        dumper->Add({{"type", "ETSParameterExpression"}, {"rest parameter", spread_}});
    }
}

void ETSParameterExpression::Dump(ir::SrcDumper *const dumper) const
{
    if (IsRestParameter()) {
        spread_->Dump(dumper);
    } else {
        if (ident_ != nullptr) {
            ASSERT(ident_->IsAnnotatedExpression());
            ident_->Dump(dumper);
            if (initializer_ != nullptr && initializer_->IsUndefinedLiteral()) {
                dumper->Add("?");
            }
            auto typeAnnotation = ident_->AsAnnotatedExpression()->TypeAnnotation();
            if (typeAnnotation != nullptr) {
                dumper->Add(": ");
                typeAnnotation->Dump(dumper);
            }
        }
        if (initializer_ != nullptr && !initializer_->IsUndefinedLiteral()) {
            dumper->Add(" = ");
            initializer_->Dump(dumper);
        }
    }
}

void ETSParameterExpression::Compile(compiler::PandaGen *const pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ETSParameterExpression::Compile(compiler::ETSGen *const etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ETSParameterExpression::Check(checker::TSChecker *const checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ETSParameterExpression::Check(checker::ETSChecker *const checker)
{
    return checker->GetAnalyzer()->Check(this);
}

ETSParameterExpression *ETSParameterExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const identOrSpread = spread_ != nullptr ? spread_->Clone(allocator, nullptr)->AsAnnotatedExpression()
                                                   : ident_->Clone(allocator, nullptr)->AsAnnotatedExpression();
    auto *const initializer =
        initializer_ != nullptr ? initializer_->Clone(allocator, nullptr)->AsExpression() : nullptr;

    if (auto *const clone = allocator->New<ETSParameterExpression>(identOrSpread, initializer); clone != nullptr) {
        identOrSpread->SetParent(clone);

        if (initializer != nullptr) {
            initializer->SetParent(clone);
        }

        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRequiredParams(extraValue_);
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
