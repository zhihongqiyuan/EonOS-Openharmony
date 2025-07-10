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

#include "property.h"

#include "es2panda.h"
#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
Property::Property([[maybe_unused]] Tag const tag, Property const &other, Expression *const key,
                   Expression *const value)
    : Property(other)
{
    key_ = key;
    value_ = value;
}

Property *Property::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const key = key_ != nullptr ? key_->Clone(allocator, nullptr)->AsExpression() : nullptr;
    auto *const value = value_ != nullptr ? value_->Clone(allocator, nullptr)->AsExpression() : nullptr;

    if (auto *const clone = allocator->New<Property>(Tag {}, *this, key, value); clone != nullptr) {
        if (key != nullptr) {
            key->SetParent(clone);
        }
        if (value != nullptr) {
            value->SetParent(clone);
        }
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}

bool Property::ConvertibleToPatternProperty()
{
    // Object pattern can't contain getter or setter
    if (IsAccessor() || isMethod_) {
        return false;
    }

    switch (value_->Type()) {
        case AstNodeType::OBJECT_EXPRESSION: {
            return value_->AsObjectExpression()->ConvertibleToObjectPattern();
        }
        case AstNodeType::ARRAY_EXPRESSION: {
            return value_->AsArrayExpression()->ConvertibleToArrayPattern();
        }
        case AstNodeType::ASSIGNMENT_EXPRESSION: {
            return value_->AsAssignmentExpression()->ConvertibleToAssignmentPattern();
        }
        case AstNodeType::IDENTIFIER:
        case AstNodeType::MEMBER_EXPRESSION:
        case AstNodeType::ARRAY_PATTERN:
        case AstNodeType::OBJECT_PATTERN:
        case AstNodeType::ASSIGNMENT_PATTERN: {
            break;
        }
        default: {
            if (isShorthand_) {
                break;
            }

            return false;
        }
    }

    return true;
}

ValidationInfo Property::ValidateExpression()
{
    ValidationInfo info;

    if (!IsComputed() && !IsMethod() && !IsAccessor() && !IsShorthand()) {
        bool currentIsProto = false;

        if (key_->IsIdentifier()) {
            currentIsProto = key_->AsIdentifier()->Name().Is("__proto__");
        } else if (key_->IsStringLiteral()) {
            currentIsProto = key_->AsStringLiteral()->Str().Is("__proto__");
        }

        if (currentIsProto) {
            kind_ = PropertyKind::PROTO;
        }
    }

    if (value_ != nullptr) {
        if (value_->IsAssignmentPattern()) {
            return {"Invalid shorthand property initializer.", value_->Start()};
        }

        if (value_->IsObjectExpression()) {
            info = value_->AsObjectExpression()->ValidateExpression();
        } else if (value_->IsArrayExpression()) {
            info = value_->AsArrayExpression()->ValidateExpression();
        }
    }

    return info;
}

void Property::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(key_); key_ != transformedNode) {
        key_->SetTransformedNode(transformationName, transformedNode);
        key_ = transformedNode->AsExpression();
    }

    if (auto *transformedNode = cb(value_); value_ != transformedNode) {
        value_->SetTransformedNode(transformationName, transformedNode);
        value_ = transformedNode->AsExpression();
    }
}

void Property::Iterate(const NodeTraverser &cb) const
{
    cb(key_);
    cb(value_);
}

void Property::Dump(ir::AstDumper *dumper) const
{
    const char *kind = nullptr;

    switch (kind_) {
        case PropertyKind::INIT: {
            kind = "init";
            break;
        }
        case PropertyKind::PROTO: {
            kind = "proto";
            break;
        }
        case PropertyKind::GET: {
            kind = "get";
            break;
        }
        case PropertyKind::SET: {
            kind = "set";
            break;
        }
        default: {
            UNREACHABLE();
        }
    }

    dumper->Add({{"type", "Property"},
                 {"method", isMethod_},
                 {"shorthand", isShorthand_},
                 {"computed", isComputed_},
                 {"key", key_},
                 {"value", value_},
                 {"kind", kind}});
}

void Property::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void Property::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

void Property::Dump(ir::SrcDumper *dumper) const
{
    if (kind_ == PropertyKind::INIT) {
        key_->Dump(dumper);
        dumper->Add(": ");
        value_->Dump(dumper);
    }
}

checker::Type *Property::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *Property::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}
}  // namespace ark::es2panda::ir
