/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "lreference.h"

#include <compiler/base/destructuring.h>
#include <compiler/core/pandagen.h>
#include <ir/base/classDefinition.h>
#include <ir/base/spreadElement.h>
#include <ir/expressions/assignmentExpression.h>
#include <ir/expressions/identifier.h>
#include <ir/expressions/memberExpression.h>
#include <ir/statements/variableDeclaration.h>
#include <ir/statements/variableDeclarator.h>
#include <ir/ts/tsAsExpression.h>
#include <ir/ts/tsSatisfiesExpression.h>
#include <ir/ts/tsTypeAssertion.h>
#include <ir/ts/tsNonNullExpression.h>

namespace panda::es2panda::compiler {

// LReference

LReference::LReference(const ir::AstNode *node, PandaGen *pg, bool isDeclaration, ReferenceKind refKind,
                       binder::ScopeFindResult res)
    : node_(node), pg_(pg), refKind_(refKind), res_(res), isDeclaration_(isDeclaration)
{
    if (refKind == ReferenceKind::MEMBER) {
        obj_ = pg_->AllocReg();

        node_->AsMemberExpression()->CompileObject(pg_, obj_);
        if (!node_->AsMemberExpression()->AccessPrivateProperty()) {
            prop_ = node->AsMemberExpression()->CompileKey(pg_);
        }
    }
}

void LReference::GetValue()
{
    switch (refKind_) {
        case ReferenceKind::VAR_OR_GLOBAL: {
            pg_->LoadVar(node_->AsIdentifier(), res_);
            break;
        }
        case ReferenceKind::MEMBER: {
            if (node_->AsMemberExpression()->AccessPrivateProperty()) {
                auto name = node_->AsMemberExpression()->Property()->AsPrivateIdentifier()->Name();
                auto result = pg_->Scope()->FindPrivateName(name);
                if (!result.result.isMethod) {
                    pg_->LoadAccumulator(node_, obj_);
                    pg_->LoadPrivateProperty(node_, result.lexLevel, result.result.slot);
                    break;
                }

                if (result.result.isStatic) {
                    pg_->LoadLexicalVar(node_, result.lexLevel, result.result.validateMethodSlot);
                    pg_->Equal(node_, obj_);
                    pg_->ThrowTypeErrorIfFalse(node_, "Object does not have private property");
                } else {
                    pg_->LoadAccumulator(node_, obj_);
                    pg_->LoadPrivateProperty(node_, result.lexLevel, result.result.validateMethodSlot);
                }
                if (result.result.isSetter) {
                    pg_->ThrowTypeError(node_, "Property is not defined with Getter");
                }
                if (result.result.isGetter) {
                    pg_->LoadAccumulator(node_, obj_);
                    pg_->LoadPrivateProperty(node_, result.lexLevel, result.result.slot);
                    break;
                }
                pg_->LoadLexicalVar(node_, result.lexLevel, result.result.slot);
            } else {
                pg_->LoadObjProperty(node_, obj_, prop_);
            }
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

void LReference::SetValue()
{
    switch (refKind_) {
        case ReferenceKind::VAR_OR_GLOBAL: {
            pg_->StoreVar(node_, res_, isDeclaration_);
            break;
        }
        case ReferenceKind::MEMBER: {
            if (node_->AsMemberExpression()->Object()->IsSuperExpression()) {
                pg_->StoreSuperProperty(node_, obj_, prop_);
            } else if (node_->AsMemberExpression()->AccessPrivateProperty()) {
                compiler::RegScope rs(pg_);
                VReg valueReg =  pg_->AllocReg();
                
                auto name = node_->AsMemberExpression()->Property()->AsPrivateIdentifier()->Name();
                auto result = pg_->Scope()->FindPrivateName(name, true);
                if (!result.result.isMethod) {
                    pg_->StorePrivateProperty(node_, result.lexLevel, result.result.slot, obj_);
                    break;
                }
                if (!result.result.isSetter) {
                    pg_->ThrowTypeError(node_, "Method is not writable");
                }
                // store value
                pg_->StoreAccumulator(node_, valueReg);

                if (result.result.isStatic) {
                    pg_->LoadLexicalVar(node_, result.lexLevel, result.result.validateMethodSlot);
                    pg_->Equal(node_, obj_);
                    pg_->ThrowTypeErrorIfFalse(node_, "Object does not have private property");
                } else {
                    pg_->LoadAccumulator(node_, obj_);
                    pg_->LoadPrivateProperty(node_, result.lexLevel, result.result.validateMethodSlot);
                }
                pg_->LoadAccumulator(node_, valueReg);
                pg_->StorePrivateProperty(node_, result.lexLevel, result.result.slot, obj_);
            } else {
                pg_->StoreObjProperty(node_, obj_, prop_);
            }

            break;
        }
        case ReferenceKind::DESTRUCTURING: {
            Destructuring::Compile(pg_, node_->AsExpression());
            break;
        }
        default: {
            UNREACHABLE();
        }
    }
}

ReferenceKind LReference::Kind() const
{
    return refKind_;
}

binder::Variable *LReference::Variable() const
{
    return res_.variable;
}

LReference LReference::CreateLRef(PandaGen *pg, const ir::AstNode *node, bool isDeclaration)
{
    switch (node->Type()) {
        case ir::AstNodeType::IDENTIFIER: {
            const util::StringView &name = node->AsIdentifier()->Name();
            binder::ScopeFindResult res = pg->Scope()->Find(name);

            return {node, pg, isDeclaration, ReferenceKind::VAR_OR_GLOBAL, res};
        }
        case ir::AstNodeType::MEMBER_EXPRESSION: {
            return {node, pg, false, ReferenceKind::MEMBER, {}};
        }
        case ir::AstNodeType::VARIABLE_DECLARATION: {
            ASSERT(node->AsVariableDeclaration()->Declarators().size() == 1);
            return LReference::CreateLRef(pg, node->AsVariableDeclaration()->Declarators()[0]->Id(), true);
        }
        case ir::AstNodeType::VARIABLE_DECLARATOR: {
            return LReference::CreateLRef(pg, node->AsVariableDeclarator()->Id(), true);
        }
        case ir::AstNodeType::ARRAY_PATTERN:
        case ir::AstNodeType::OBJECT_PATTERN:
        case ir::AstNodeType::ARRAY_EXPRESSION:
        case ir::AstNodeType::OBJECT_EXPRESSION: {
            return {node, pg, isDeclaration, ReferenceKind::DESTRUCTURING, {}};
        }
        case ir::AstNodeType::ASSIGNMENT_PATTERN: {
            return LReference::CreateLRef(pg, node->AsAssignmentPattern()->Left(), true);
        }
        case ir::AstNodeType::REST_ELEMENT: {
            return LReference::CreateLRef(pg, node->AsRestElement()->Argument(), isDeclaration);
        }
        case ir::AstNodeType::EXPORT_DEFAULT_DECLARATION: {
            // export default [anonymous class decl]
            util::StringView name = parser::SourceTextModuleRecord::DEFAULT_LOCAL_NAME;
            binder::ScopeFindResult res = pg->Scope()->Find(name);

            return {node, pg, isDeclaration, ReferenceKind::VAR_OR_GLOBAL, res};
        }
        case ir::AstNodeType::TS_AS_EXPRESSION: {
            return LReference::CreateLRef(pg, node->AsTSAsExpression()->Expr(), isDeclaration);
        }
        case ir::AstNodeType::TS_SATISFIES_EXPRESSION: {
            return LReference::CreateLRef(pg, node->AsTSSatisfiesExpression()->Expr(), isDeclaration);
        }
        case ir::AstNodeType::TS_TYPE_ASSERTION: {
            return LReference::CreateLRef(pg, node->AsTSTypeAssertion()->GetExpression(), isDeclaration);
        }
        case ir::AstNodeType::TS_NON_NULL_EXPRESSION: {
            return LReference::CreateLRef(pg, node->AsTSNonNullExpression()->Expr(), isDeclaration);
        }
        default: {
            UNREACHABLE();
        }
    }
}

}  // namespace panda::es2panda::compiler
