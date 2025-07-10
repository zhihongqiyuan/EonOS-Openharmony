/**
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

#include "declaration.h"

#include <ir/base/classDefinition.h>
#include <ir/base/scriptFunction.h>
#include <ir/ts/tsModuleDeclaration.h>

namespace panda::es2panda::binder {

bool Decl::IsSendableClassDecl() const
{
    return node_ && node_->IsClassDefinition() && node_->AsClassDefinition()->IsSendable();
}

bool Decl::IsSendableFunctionDecl() const
{
    return node_ && node_->IsScriptFunction() && node_->AsScriptFunction()->IsSendable();
}

bool Decl::NeedSetInSendableEnv(Scope *scope) const
{
    if (!scope->IsModuleScope()) {
        return false;
    }

    // sendable env will not be effective before api 12
    if (scope->AsModuleScope()->Program()->TargetApiVersion() < 12) {
        return false;
    }
    return IsSendableClassDecl() || IsSendableFunctionDecl();
}

bool NamespaceDecl::IsInstantiated() const
{
    auto tsModules = Decls();
    for (auto *it : tsModules) {
        if (it->IsInstantiated()) {
            return true;
        }
    }
    return false;
}

}  // namespace panda::es2panda::binder
