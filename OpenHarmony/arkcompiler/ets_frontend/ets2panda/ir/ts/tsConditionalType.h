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

#ifndef ES2PANDA_IR_TS_CONDITIONAL_TYPE_H
#define ES2PANDA_IR_TS_CONDITIONAL_TYPE_H

#include "ir/typeNode.h"

namespace ark::es2panda::ir {
class TSConditionalType : public TypeNode {
public:
    explicit TSConditionalType(Expression *checkType, Expression *extendsType, Expression *trueType,
                               Expression *falseType)
        : TypeNode(AstNodeType::TS_CONDITIONAL_TYPE),
          checkType_(checkType),
          extendsType_(extendsType),
          trueType_(trueType),
          falseType_(falseType)
    {
    }

    const Expression *CheckType() const
    {
        return checkType_;
    }

    const Expression *ExtendsType() const
    {
        return extendsType_;
    }

    const Expression *TrueType() const
    {
        return trueType_;
    }

    const Expression *FalseType() const
    {
        return falseType_;
    }

    void TransformChildren(const NodeTransformer &cb, std::string_view transformationName) override;
    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Dump(ir::SrcDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    void Compile(compiler::ETSGen *etsg) const override;
    checker::Type *Check([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *GetType([[maybe_unused]] checker::TSChecker *checker) override;
    checker::Type *Check([[maybe_unused]] checker::ETSChecker *checker) override;

    void Accept(ASTVisitorT *v) override
    {
        v->Accept(this);
    }

private:
    Expression *checkType_;
    Expression *extendsType_;
    Expression *trueType_;
    Expression *falseType_;
};
}  // namespace ark::es2panda::ir

#endif
