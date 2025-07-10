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

#ifndef ES2PANDA_IR_STATEMENT_FUNCTION_DECLARATION_H
#define ES2PANDA_IR_STATEMENT_FUNCTION_DECLARATION_H

#include "ir/statement.h"
#include "ir/statements/annotationUsage.h"

namespace ark::es2panda::ir {
class ScriptFunction;
class AnnotationUsage;

class FunctionDeclaration : public Statement {
public:
    explicit FunctionDeclaration(ArenaAllocator *allocator, ScriptFunction *func,
                                 ArenaVector<AnnotationUsage *> &&annotations, bool isAnonymous = false)
        : Statement(AstNodeType::FUNCTION_DECLARATION),
          decorators_(allocator->Adapter()),
          func_(func),
          isAnonymous_(isAnonymous),
          annotations_(std::move(annotations))
    {
    }

    explicit FunctionDeclaration(ArenaAllocator *allocator, ScriptFunction *func, bool isAnonymous = false)
        : Statement(AstNodeType::FUNCTION_DECLARATION),
          decorators_(allocator->Adapter()),
          func_(func),
          isAnonymous_(isAnonymous),
          annotations_(allocator->Adapter())
    {
    }

    ScriptFunction *Function()
    {
        return func_;
    }

    bool IsAnonymous() const
    {
        return isAnonymous_;
    }

    const ScriptFunction *Function() const
    {
        return func_;
    }

    void AddDecorators([[maybe_unused]] ArenaVector<ir::Decorator *> &&decorators) override
    {
        decorators_ = std::move(decorators);
    }

    bool CanHaveDecorator([[maybe_unused]] bool inTs) const override
    {
        return !inTs;
    }

    [[nodiscard]] ArenaVector<ir::AnnotationUsage *> &Annotations() noexcept
    {
        return annotations_;
    }

    [[nodiscard]] const ArenaVector<ir::AnnotationUsage *> &Annotations() const noexcept
    {
        return annotations_;
    }

    void SetAnnotations(ArenaVector<ir::AnnotationUsage *> &&annotations)
    {
        annotations_ = std::move(annotations);
        for (auto anno : annotations_) {
            anno->SetParent(this);
        }
    }

    void AddAnnotations(AnnotationUsage *const annotations)
    {
        annotations_.emplace_back(annotations);
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile(compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check(checker::TSChecker *checker) override;
    checker::Type *Check(checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    ArenaVector<Decorator *> decorators_;
    ScriptFunction *func_;
    const bool isAnonymous_;
    ArenaVector<AnnotationUsage *> annotations_;
};
}  // namespace ark::es2panda::ir

#endif
