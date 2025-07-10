/*
 * Copyright (c) 2021 - 2024 Huawei Device Co., Ltd.
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

#ifndef PANDA_TYPECHECKER_H
#define PANDA_TYPECHECKER_H

#include "ir/visitor/IterateAstVisitor.h"

namespace ark::es2panda::checker {

/*
 * This class should be launched after Checker pass.
 * Should contain all type checks, which are non-critical for `Checker` (`Checker` can keep working with such type
 * error).
 */
class TypeChecker : public ir::visitor::IterateAstVisitor {
public:
    // NOTE(aleksisch): after merging #14938 add `const-AstVisitor` and make this pass const
    bool Check(ir::AstNode *node)
    {
        node->Accept(this);
        return true;
    }

protected:
    // NOTE(aleksisch): checker should be replaced with some `ErrorHandler`
    explicit TypeChecker(Checker *checker) : checker_(checker) {}

    void LogTypeError(std::initializer_list<TypeErrorMessageElement> list, const lexer::SourcePosition &pos)
    {
        checker_->LogTypeError(list, pos);
    }

private:
    Checker *const checker_;
};

class ETSTypeChecker : public TypeChecker {
public:
    explicit ETSTypeChecker(Checker *checker) : TypeChecker(checker) {}

private:
    void VisitArrowFunctionExpression(ir::ArrowFunctionExpression *node) override;
};

class JSTypeChecker : public TypeChecker {
public:
    explicit JSTypeChecker(Checker *checker) : TypeChecker(checker) {}
};

class TSTypeChecker : public TypeChecker {
public:
    explicit TSTypeChecker(Checker *checker) : TypeChecker(checker) {}
};

class ASTypeChecker : public TypeChecker {
public:
    explicit ASTypeChecker(Checker *checker) : TypeChecker(checker) {}
};

bool RunTypeChecker(Checker *checker, ScriptExtension ext, ir::AstNode *node);

}  // namespace ark::es2panda::checker
#endif  // PANDA_TYPECHECKER_H
