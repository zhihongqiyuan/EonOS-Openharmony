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

#ifndef ES2PANDA_EVALUATE_CLASS_BUILDER_H
#define ES2PANDA_EVALUATE_CLASS_BUILDER_H

#include "ir/base/classDefinition.h"
#include "util/ustring.h"
#include "libpandabase/utils/arena_containers.h"

namespace ark::panda_file {
class ClassDataAccessor;
}  // namespace ark::panda_file

namespace ark::es2panda::parser {
class Program;
}  // namespace ark::es2panda::parser

namespace ark::es2panda::checker {
class ETSChecker;
}  // namespace ark::es2panda::checker

namespace ark::es2panda::ir {
class ClassDeclaration;
class Expression;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::ir {
class ClassDeclaration;
class AstNode;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::evaluate {

class ClassBuilder {
public:
    explicit ClassBuilder(checker::ETSChecker *checker, util::StringView name, panda_file::ClassDataAccessor &cda,
                          ir::Expression *superClass);

    ir::ClassDeclaration *Build(parser::Program *program) &&;

    [[nodiscard]] bool IsAbstract() const
    {
        return (modifierFlags_ & ir::ModifierFlags::ABSTRACT) != 0;
    }

private:
    void BuildFields(ArenaVector<ir::AstNode *> &classBody);
    void BuildMethods(ArenaVector<ir::AstNode *> &classBody);

private:
    checker::ETSChecker *checker_ {nullptr};

    util::StringView className_ {};
    panda_file::ClassDataAccessor &cda_;
    ir::Expression *superClass_ {nullptr};

    ir::ModifierFlags modifierFlags_ {ir::ModifierFlags::NONE};
    ir::ClassDefinitionModifiers classModifiers_ {ir::ClassDefinitionModifiers::NONE};
};

}  // namespace ark::es2panda::evaluate

#endif  // ES2PANDA_EVALUATE_CLASS_BUILDER_H
