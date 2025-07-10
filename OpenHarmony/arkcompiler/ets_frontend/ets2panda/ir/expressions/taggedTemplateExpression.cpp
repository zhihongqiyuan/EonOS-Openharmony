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

#include "taggedTemplateExpression.h"

#include "varbinder/variable.h"
#include "compiler/base/literals.h"
#include "compiler/core/ETSGen.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/regScope.h"
#include "checker/TSchecker.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"
#include "ir/expressions/templateLiteral.h"
#include "ir/ts/tsTypeParameterInstantiation.h"

namespace ark::es2panda::ir {
void TaggedTemplateExpression::TransformChildren(const NodeTransformer &cb, std::string_view const transformationName)
{
    if (typeParams_ != nullptr) {
        if (auto *transformedNode = cb(typeParams_); typeParams_ != transformedNode) {
            typeParams_->SetTransformedNode(transformationName, transformedNode);
            typeParams_ = transformedNode->AsTSTypeParameterInstantiation();
        }
    }

    if (auto *transformedNode = cb(tag_); tag_ != transformedNode) {
        tag_->SetTransformedNode(transformationName, transformedNode);
        tag_ = transformedNode->AsExpression();
    }

    if (auto *transformedNode = cb(quasi_); quasi_ != transformedNode) {
        quasi_->SetTransformedNode(transformationName, transformedNode);
        quasi_ = transformedNode->AsTemplateLiteral();
    }
}

void TaggedTemplateExpression::Iterate(const NodeTraverser &cb) const
{
    if (typeParams_ != nullptr) {
        cb(typeParams_);
    }

    cb(tag_);
    cb(quasi_);
}

void TaggedTemplateExpression::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "TaggedTemplateExpression"},
                 {"tag", tag_},
                 {"quasi", quasi_},
                 {"typeParameters", AstDumper::Optional(typeParams_)}});
}

void TaggedTemplateExpression::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("TaggedTemplateExpression");
}

void TaggedTemplateExpression::Compile(compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void TaggedTemplateExpression::Compile(compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *TaggedTemplateExpression::Check(checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *TaggedTemplateExpression::Check([[maybe_unused]] checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

TaggedTemplateExpression *TaggedTemplateExpression::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    auto *const tag = tag_ != nullptr ? tag_->Clone(allocator, nullptr)->AsExpression() : nullptr;
    auto *const quasi = quasi_ != nullptr ? quasi_->Clone(allocator, nullptr) : nullptr;
    auto *const typeParams = typeParams_ != nullptr ? typeParams_->Clone(allocator, nullptr) : nullptr;

    if (auto *const clone = allocator->New<TaggedTemplateExpression>(tag, quasi, typeParams); clone != nullptr) {
        if (tag != nullptr) {
            tag->SetParent(clone);
        }
        if (quasi != nullptr) {
            quasi->SetParent(clone);
        }
        if (typeParams != nullptr) {
            typeParams->SetParent(clone);
        }
        if (parent != nullptr) {
            clone->SetParent(parent);
        }
        return clone;
    }

    throw Error(ErrorType::GENERIC, "", CLONE_ALLOCATION_ERROR);
}
}  // namespace ark::es2panda::ir
