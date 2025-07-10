/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef ETS2PANDA_TEST_UNIT_NODE_CREATOR_H
#define ETS2PANDA_TEST_UNIT_NODE_CREATOR_H

#include "ir/astNode.h"
#include "ir/statements/variableDeclarator.h"
#include "ir/statements/variableDeclaration.h"
#include "ir/expressions/identifier.h"
#include "ir/statements/forUpdateStatement.h"
#include "ir/statements/blockStatement.h"
#include "ir/statements/whileStatement.h"
#include "ir/expressions/literals/booleanLiteral.h"
#include "ir/expressions/literals/numberLiteral.h"
#include "ir/expressions/binaryExpression.h"
#include "ir/expressions/updateExpression.h"

namespace ark::es2panda::gtests {

class NodeGenerator {
public:
    explicit NodeGenerator(ArenaAllocator *alloc) : alloc_(alloc) {}
    // x = 1
    ir::VariableDeclaration *CreateVarDecl(util::StringView name = "x")
    {
        auto varDecl = alloc_->New<ir::VariableDeclarator>(ir::VariableDeclaratorFlag::LET, CreateId(name));
        ArenaVector<ir::VariableDeclarator *> tmp {alloc_->Adapter()};
        tmp.emplace_back(varDecl);
        return alloc_->New<ir::VariableDeclaration>(ir::VariableDeclaration::VariableDeclarationKind::LET, alloc_,
                                                    std::move(tmp));
    }

    ir::Identifier *CreateId(util::StringView x)
    {
        return alloc_->New<ir::Identifier>(x, alloc_);
    }

    // x = x + 1
    ir::UpdateExpression *CreateIncrement(util::StringView name = "x", bool isPrefix = false)
    {
        return alloc_->New<ir::UpdateExpression>(CreateId(name), lexer::TokenType::PUNCTUATOR_PLUS_PLUS, isPrefix);
    }

    // x < 10
    ir::BinaryExpression *CreateLessCmpExpr(util::StringView name = "x")
    {
        const int anyLoopLimit = 10;
        return alloc_->New<ir::BinaryExpression>(CreateId(name),
                                                 alloc_->New<ir::NumberLiteral>(lexer::Number(anyLoopLimit)),
                                                 lexer::TokenType::PUNCTUATOR_LESS_THAN);
    }

    ir::BlockStatement *CreateBlockWithDeclare(util::StringView name = "x")
    {
        auto varDecl = CreateVarDecl(name);
        ArenaVector<ir::Statement *> tmp {alloc_->Adapter()};
        tmp.emplace_back(varDecl);
        auto *newBlock = alloc_->New<ir::BlockStatement>(alloc_, std::move(tmp));
        varDecl->SetParent(newBlock);
        return newBlock;
    }

    ir::ForUpdateStatement *CreateForUpdate()
    {
        return alloc_->New<ir::ForUpdateStatement>(CreateVarDecl(), CreateLessCmpExpr(), CreateIncrement(),
                                                   CreateBlockWithDeclare());
    }

    ir::WhileStatement *CreateWhile()
    {
        return alloc_->New<ir::WhileStatement>(CreateLessCmpExpr(), CreateBlockWithDeclare());
    }

private:
    ArenaAllocator *const alloc_;
};
}  // namespace ark::es2panda::gtests
#endif  // ETS2PANDA_TEST_UNIT_NODE_CREATOR_H
