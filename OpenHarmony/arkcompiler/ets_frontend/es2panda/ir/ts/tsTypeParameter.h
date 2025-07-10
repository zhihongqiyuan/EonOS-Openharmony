/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_IR_TS_TYPE_PARAMETER_H
#define ES2PANDA_IR_TS_TYPE_PARAMETER_H

#include <ir/expression.h>

namespace panda::es2panda::compiler {
class PandaGen;
}  // namespace panda::es2panda::compiler

namespace panda::es2panda::checker {
class Checker;
class Type;
}  // namespace panda::es2panda::checker

namespace panda::es2panda::ir {

class Identifier;

class TSTypeParameter : public Expression {
public:
    explicit TSTypeParameter(Identifier *name, Expression *constraint, Expression *defaultType,
                             bool isTypeIn = false, bool isTypeOut = false)
        : Expression(AstNodeType::TS_TYPE_PARAMETER),
        name_(name),
        constraint_(constraint),
        defaultType_(defaultType),
        isTypeIn_(isTypeIn),
        isTypeOut_(isTypeOut)
    {
    }

    const Identifier *Name() const
    {
        return name_;
    }

    const Expression *Constraint() const
    {
        return constraint_;
    }

    const Expression *DefaultType() const
    {
        return defaultType_;
    }

    bool IsTypeIn() const
    {
        return isTypeIn_;
    }

    bool IsTypeOut() const
    {
        return isTypeOut_;
    }

    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    checker::Type *Check([[maybe_unused]] checker::Checker *checker) const override;
    void UpdateSelf(const NodeUpdater &cb, [[maybe_unused]] binder::Binder *binder) override;

private:
    Identifier *name_;
    Expression *constraint_;
    Expression *defaultType_;
    bool isTypeIn_;
    bool isTypeOut_;
};
}  // namespace panda::es2panda::ir

#endif
