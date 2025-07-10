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

#include "classPrivateContext.h"

#include "ir/expression.h"
#include "ir/expressions/identifier.h"
#include "ir/base/classElement.h"
#include "ir/base/methodDefinition.h"

namespace ark::es2panda::parser {
bool ClassPrivateContext::AddElement(const ir::ClassElement *elem)
{
    bool newPropIsStatic = elem->IsStatic();
    util::StringView newPropName = elem->Id()->Name();
    ir::MethodDefinitionKind newPropMethodKind = ir::MethodDefinitionKind::METHOD;

    if (elem->IsMethodDefinition()) {
        newPropMethodKind = elem->AsMethodDefinition()->Kind();
    }

    for (const auto *prop : elements_) {
        const ir::Identifier *ident = prop->Id();
        ir::MethodDefinitionKind methodKind = ir::MethodDefinitionKind::METHOD;
        bool isStatic = prop->IsStatic();

        if (prop->IsMethodDefinition()) {
            methodKind = prop->AsMethodDefinition()->Kind();
        }

        if (ident == nullptr || !ident->IsPrivateIdent() || ident->Name() != newPropName ||
            isStatic != newPropIsStatic) {
            continue;
        }

        if ((newPropMethodKind == ir::MethodDefinitionKind::GET && methodKind == ir::MethodDefinitionKind::SET) ||
            (newPropMethodKind == ir::MethodDefinitionKind::SET && methodKind == ir::MethodDefinitionKind::GET)) {
            continue;
        }

        return false;
    }

    elements_.push_back(elem);

    return true;
}

bool ClassPrivateContext::FindElement(const ir::Identifier *elem) const
{
    for (const auto *it : elements_) {
        if (it->Id()->Name().Compare(elem->Name()) == 0) {
            return true;
        }
    }

    if (prev_ != nullptr) {
        return prev_->FindElement(elem);
    }

    return false;
}
}  // namespace ark::es2panda::parser
