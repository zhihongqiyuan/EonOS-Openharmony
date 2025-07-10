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

#include "tsQualifiedName.h"

#include "checker/ETSchecker.h"
#include "checker/TSchecker.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/expressions/identifier.h"

namespace ark::es2panda::ir {
TSQualifiedName::TSQualifiedName([[maybe_unused]] Tag const tag, TSQualifiedName const &other,
                                 ArenaAllocator *allocator)
    : Expression(static_cast<Expression const &>(other))
{
    left_ = other.left_ != nullptr ? other.left_->Clone(allocator, this)->AsExpression() : nullptr;
    right_ = other.right_ != nullptr ? other.right_->Clone(allocator, this)->AsIdentifier() : nullptr;
}

void TSQualifiedName::Iterate(const NodeTraverser &cb) const
{
    cb(left_);
    cb(right_);
}

void TSQualifiedName::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (auto *transformedNode = cb(left_); left_ != transformedNode) {
        left_->SetTransformedNode(transformationName, transformedNode);
        left_ = transformedNode->AsExpression();
    }

    if (auto *transformedNode = cb(right_); right_ != transformedNode) {
        right_->SetTransformedNode(transformationName, transformedNode);
        right_ = transformedNode->AsIdentifier();
    }
}

void TSQualifiedName::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TSQualifiedName"}, {"left", left_}, {"right", right_}});
}

void TSQualifiedName::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TSQualifiedName");
}

void TSQualifiedName::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}
void TSQualifiedName::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TSQualifiedName::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TSQualifiedName::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

util::StringView TSQualifiedName::ToString(ArenaAllocator *allocator) const
{
    util::UString packageName(allocator);

    const auto *iter = this;

    while (iter->Left()->IsTSQualifiedName()) {
        iter = iter->Left()->AsTSQualifiedName();
    }

    packageName.Append(iter->Left()->AsIdentifier()->Name());

    const ir::AstNode *parent = iter;

    while (parent != nullptr && parent->IsTSQualifiedName()) {
        packageName.Append('.');
        packageName.Append(parent->AsTSQualifiedName()->Right()->AsIdentifier()->Name());
        parent = parent->Parent();
    }

    return packageName.View();
}

util::StringView TSQualifiedName::BaseToString(ArenaAllocator *allocator) const
{
    util::UString packageName(allocator);

    const auto *iter = this;

    while (iter->Left()->IsTSQualifiedName()) {
        iter = iter->Left()->AsTSQualifiedName();
    }

    packageName.Append(iter->Left()->AsIdentifier()->Name());

    const ir::AstNode *parent = iter->Parent();

    while (parent != nullptr && parent->IsTSQualifiedName()) {
        packageName.Append('.');
        packageName.Append(parent->AsTSQualifiedName()->Right()->AsIdentifier()->Name());
        parent = parent->Parent();
    }

    return packageName.View();
}

template <typename T>
static T ResolveLeftMostQualifiedNameImpl(T self)
{
    auto *iter = self;

    while (iter->Left()->IsTSQualifiedName()) {
        iter = iter->Left()->AsTSQualifiedName();
    }

    return iter;
}

ir::TSQualifiedName *TSQualifiedName::ResolveLeftMostQualifiedName()
{
    return ResolveLeftMostQualifiedNameImpl(this);
}

const ir::TSQualifiedName *TSQualifiedName::ResolveLeftMostQualifiedName() const
{
    return ResolveLeftMostQualifiedNameImpl(this);
}

TSQualifiedName *TSQualifiedName::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    if (auto *const clone = allocator->New<TSQualifiedName>(Tag {}, *this, allocator); clone != nullptr) {
        if (parent != nullptr) {
            clone->SetParent(parent);
        }

        clone->SetRange(Range());
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
