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

#ifndef ES2PANDA_IR_STATEMENT_LOOP_STATEMENT_H
#define ES2PANDA_IR_STATEMENT_LOOP_STATEMENT_H

#include "varbinder/scope.h"
#include "ir/statement.h"

namespace ark::es2panda::ir {
class LoopStatement : public Statement {
public:
    LoopStatement() = delete;
    ~LoopStatement() override = default;

    NO_COPY_SEMANTIC(LoopStatement);
    NO_MOVE_SEMANTIC(LoopStatement);

    [[nodiscard]] bool IsScopeBearer() const noexcept override
    {
        return true;
    }

    [[nodiscard]] varbinder::LoopScope *Scope() const noexcept final
    {
        return scope_;
    }

    void SetScope(varbinder::LoopScope *scope)
    {
        ASSERT(scope_ == nullptr);
        scope_ = scope;
    }

    void ClearScope() noexcept override
    {
        scope_ = nullptr;
    }

    void TransformChildren([[maybe_unused]] const NodeTransformer &cb,
                           [[maybe_unused]] std::string_view const transformationName) override
    {
        UNREACHABLE();
    }

    void Iterate([[maybe_unused]] const NodeTraverser &cb) const override
    {
        UNREACHABLE();
    }

    void Dump([[maybe_unused]] AstDumper *dumper) const override
    {
        UNREACHABLE();
    }

    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override
    {
        UNREACHABLE();
    }

    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override
    {
        UNREACHABLE();
        return nullptr;
    }

    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override
    {
        UNREACHABLE();
        return nullptr;
    }

protected:
    explicit LoopStatement(AstNodeType type) : Statement(type) {}

private:
    varbinder::LoopScope *scope_ {nullptr};
};
}  // namespace ark::es2panda::ir

#endif
