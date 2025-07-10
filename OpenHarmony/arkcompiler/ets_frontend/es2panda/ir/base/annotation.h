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

#ifndef ES2PANDA_PARSER_INCLUDE_AST_ANNOTATION_H
#define ES2PANDA_PARSER_INCLUDE_AST_ANNOTATION_H

#include <ir/expressions/callExpression.h>
#include <ir/expressions/identifier.h>
#include <ir/expressions/memberExpression.h>
#include <ir/statement.h>
#include <ustring.h>

namespace panda::es2panda::compiler {
class PandaGen;
}  // namespace panda::es2panda::compiler

namespace panda::es2panda::checker {
class Checker;
class Type;
}  // namespace panda::es2panda::checker

namespace panda::es2panda::ir {
class Expression;

class Annotation : public Statement {
public:
    explicit Annotation(Expression *expr) : Statement(AstNodeType::ANNOTATION), expr_(expr)
    {
        if (expr->IsCallExpression()) {
            expr = expr->AsCallExpression()->Callee();
        }

        while (expr->IsMemberExpression()) {
            name_.insert(0, expr->AsMemberExpression()->Property()->AsIdentifier()->Name().Utf8());
            name_.insert(0, ".");
            expr = expr->AsMemberExpression()->Object();
        }
        name_.insert(0, expr->AsIdentifier()->Name().Utf8());
        nameView_ = util::StringView(name_);
    }

    const Expression *Expr() const
    {
        return expr_;
    }

    Expression *Expr()
    {
        return expr_;
    }

    const util::StringView &Name() const
    {
        return nameView_;
    }

    bool IsImported() const
    {
        return isImported_;
    }

    void SetIsImported()
    {
        isImported_ = true;
    }

    void Iterate(const NodeTraverser &cb) const override;
    void Dump(ir::AstDumper *dumper) const override;
    void Compile([[maybe_unused]] compiler::PandaGen *pg) const override;
    checker::Type *Check([[maybe_unused]] checker::Checker *checker) const override;
    void UpdateSelf(const NodeUpdater &cb, [[maybe_unused]] binder::Binder *binder) override;
    static constexpr char interfaceString[] = "interface";
    static constexpr char stringClassName[] = "panda.String";
    static constexpr char annotationPrefix[] = "__$$ETS_ANNOTATION$$__";

    // Conventions with TSC part
    static constexpr uint8_t EMPTY_LITERAL_ARRAY_WITH_NUMBER_TYPE = 0;
    static constexpr uint8_t EMPTY_LITERAL_ARRAY_WITH_BOOLEAN_TYPE = 1;
    static constexpr uint8_t EMPTY_LITERAL_ARRAY_WITH_STRING_TYPE = 2;

    static constexpr uint8_t ENUM_LITERAL_ARRAY_WITHOUT_INITIALIZER_NUMBER_TYPE = 0;
    static constexpr uint8_t ENUM_LITERAL_ARRAY_WITH_EMPTY_INITIALIZER_NUMBER_TYPE = 1;
    static constexpr uint8_t ENUM_LITERAL_ARRAY_WITHOUT_INITIALIZER_STRING_TYPE = 2;
    static constexpr uint8_t ENUM_LITERAL_ARRAY_WITH_EMPTY_INITIALIZER_STRING_TYPE = 3;

private:
    Expression *expr_ = nullptr;
    std::string name_ = "";
    util::StringView nameView_;
    bool isImported_ = false;
};

}  // namespace panda::es2panda::ir

#endif
