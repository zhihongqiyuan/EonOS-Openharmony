/**
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

#ifndef ES2PANDA_EVALUATE_METHOD_BUILDER_H
#define ES2PANDA_EVALUATE_METHOD_BUILDER_H

#include "util/ustring.h"
#include "ir/astNodeFlags.h"
#include "generated/signatures.h"
#include "libpandabase/utils/arena_containers.h"

namespace ark::panda_file {
class ClassDataAccessor;
}  // namespace ark::panda_file

namespace ark::panda_file {
class MethodDataAccessor;
}  // namespace ark::panda_file

namespace ark::es2panda::ir {
class AstNode;
class Expression;
class TypeNode;
class ExpressionStatement;
class BlockStatement;
class MethodDefinition;
class FunctionExpression;
class Identifier;
class Statement;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::checker {
class ETSChecker;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::evaluate {

class MethodBuilder {
public:
    explicit MethodBuilder(checker::ETSChecker *checker, panda_file::MethodDataAccessor &mda,
                           ir::ModifierFlags classModifierFlags);

    ir::AstNode *Build() &&;

    [[nodiscard]] bool IsConstructor() const
    {
        return methodName_ == compiler::Signatures::CTOR;
    }

    [[nodiscard]] bool IsStaticConstructor() const
    {
        return methodName_ == compiler::Signatures::CCTOR;
    }

    [[nodiscard]] bool IsAbstractMethod() const
    {
        return (modifierFlags_ & ir::ModifierFlags::ABSTRACT) != 0;
    }

private:
    void CollectParametersAndReturnType();
    ir::ExpressionStatement *CreateSuperConstructorExpressionCall();
    ir::BlockStatement *CreateBody(ArenaVector<ir::Statement *> statements);

    template <bool IS_STATIC>
    ir::AstNode *CreateIrConstructor(ir::Identifier *id, ir::BlockStatement *body);

    ir::MethodDefinition *CreateIrMethod(ir::Identifier *id, ir::BlockStatement *body);
    ir::FunctionExpression *CreateFunctionExpression(ir::Identifier *id, ir::BlockStatement *body,
                                                     ir::ScriptFunctionFlags scriptFuncFlags);

private:
    checker::ETSChecker *checker_ {nullptr};

    panda_file::MethodDataAccessor &mda_;

    util::StringView methodName_ {};
    ir::ModifierFlags modifierFlags_ {ir::ModifierFlags::NONE};
    ArenaVector<ir::Expression *> params_;
    ir::TypeNode *returnType_ {nullptr};

    // Modifier flags of the class that owns the method that we want to build.
    ir::ModifierFlags classModifierFlags_ {ir::ModifierFlags::NONE};
};

}  // namespace ark::es2panda::evaluate

#endif  // ES2PANDA_EVALUATE_METHOD_BUILDER_H
