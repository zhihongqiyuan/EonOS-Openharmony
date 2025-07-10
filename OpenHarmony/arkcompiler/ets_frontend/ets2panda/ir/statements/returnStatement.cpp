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

#include "returnStatement.h"

#include "checker/TSchecker.h"
#include "compiler/core/pandagen.h"
#include "compiler/core/ETSGen.h"
#include "ir/astDump.h"
#include "ir/srcDump.h"

namespace ark::es2panda::ir {
void ReturnStatement::TransformChildren(const NodeTransformer &cb, std::string_view transformationName)
{
    if (argument_ != nullptr) {
        if (auto *transformedNode = cb(argument_); argument_ != transformedNode) {
            argument_->SetTransformedNode(transformationName, transformedNode);
            argument_ = transformedNode->AsExpression();
        }
    }
}

void ReturnStatement::Iterate(const NodeTraverser &cb) const
{
    if (argument_ != nullptr) {
        cb(argument_);
    }
}

void ReturnStatement::Dump(ir::AstDumper *dumper) const
{
    dumper->Add({{"type", "ReturnStatement"}, {"argument", AstDumper::Nullish(argument_)}});
}

void ReturnStatement::Dump(ir::SrcDumper *dumper) const
{
    dumper->Add("return");
    if (argument_ != nullptr) {
        dumper->Add(" ");
        argument_->Dump(dumper);
    }
    dumper->Add(";");
}

void ReturnStatement::Compile([[maybe_unused]] compiler::PandaGen *pg) const
{
    pg->GetAstCompiler()->Compile(this);
}

void ReturnStatement::Compile([[maybe_unused]] compiler::ETSGen *etsg) const
{
    etsg->GetAstCompiler()->Compile(this);
}

checker::Type *ReturnStatement::Check([[maybe_unused]] checker::TSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

checker::Type *ReturnStatement::Check(checker::ETSChecker *checker)
{
    return checker->GetAnalyzer()->Check(this);
}

void ReturnStatement::SetReturnType(checker::ETSChecker *checker, checker::Type *type)
{
    returnType_ = type;
    if (argument_ != nullptr) {
        checker::Type *argumentType = argument_->Check(checker);
        if (type->IsETSReferenceType() && !argumentType->IsETSReferenceType()) {
            auto *const relation = checker->Relation();
            relation->SetNode(argument_);
            relation->SetFlags(checker::TypeRelationFlag::NONE);

            argumentType = checker->MaybeBoxInRelation(argumentType);
            if (argumentType == nullptr) {
                checker->LogTypeError("Invalid return statement expression", argument_->Start());
                return;
            }
            argument_->AddBoxingUnboxingFlags(checker->GetBoxingFlag(argumentType));

            relation->SetNode(nullptr);
        }
    }
}

void ReturnStatement::SetArgument(Expression *arg)
{
    argument_ = arg;
    if (argument_ != nullptr) {
        argument_->SetParent(this);
    }
}

ReturnStatement *ReturnStatement::Clone(ArenaAllocator *const allocator, AstNode *const parent)
{
    ir::ReturnStatement *clone = allocator->New<ir::ReturnStatement>();
    if (clone != nullptr) {
        clone->SetParent(parent);
        if (argument_ != nullptr) {
            clone->SetArgument(argument_->Clone(allocator, clone)->AsExpression());
        }
        clone->returnType_ = returnType_;
        clone->SetRange(Range());
    }
    return clone;
}
}  // namespace ark::es2panda::ir
