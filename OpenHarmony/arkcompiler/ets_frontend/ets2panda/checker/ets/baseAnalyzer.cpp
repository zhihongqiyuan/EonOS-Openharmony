/*
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

#include "baseAnalyzer.h"
#include "assignAnalyzer.h"
#include "ir/astNode.h"
#include "ir/statements/breakStatement.h"
#include "ir/statements/continueStatement.h"

namespace ark::es2panda::checker {

template <typename T>
void BaseAnalyzer<T>::ClearPendingExits()
{
    pendingExits_.clear();
    oldPendingExits_.clear();
}

template <typename T>
typename BaseAnalyzer<T>::PendingExitsVector &BaseAnalyzer<T>::PendingExits()
{
    return pendingExits_;
}

template <typename T>
void BaseAnalyzer<T>::SetPendingExits(const PendingExitsVector &pendingExits)
{
    pendingExits_ = pendingExits;
}

template <typename T>
typename BaseAnalyzer<T>::PendingExitsVector &BaseAnalyzer<T>::OldPendingExits()
{
    return oldPendingExits_;
}

template <typename T>
void BaseAnalyzer<T>::SetOldPendingExits(const PendingExitsVector &oldPendingExits)
{
    oldPendingExits_ = oldPendingExits;
}

template <typename T>
const ir::AstNode *BaseAnalyzer<T>::GetJumpTarget(const ir::AstNode *node) const
{
    if (node->IsBreakStatement()) {
        return node->AsBreakStatement()->Target();
    }

    ASSERT(node->IsContinueStatement());
    return node->AsContinueStatement()->Target();
}

template <typename T>
LivenessStatus BaseAnalyzer<T>::ResolveJump(const ir::AstNode *node, ir::AstNodeType jumpKind)
{
    bool resolved = false;
    PendingExitsVector exits = pendingExits_;
    pendingExits_ = oldPendingExits_;

    for (auto &it : exits) {
        if (it.Node()->Type() == jumpKind && node == GetJumpTarget(it.Node())) {
            it.ResolveJump();
            resolved = true;
        } else {
            pendingExits_.push_back(it);
        }
    }

    return From(resolved);
}

template <typename T>
LivenessStatus BaseAnalyzer<T>::ResolveContinues(const ir::AstNode *node)
{
    oldPendingExits_.clear();
    return ResolveJump(node, ir::AstNodeType::CONTINUE_STATEMENT);
}

template <typename T>
LivenessStatus BaseAnalyzer<T>::ResolveBreaks(const ir::AstNode *node)
{
    return ResolveJump(node, ir::AstNodeType::BREAK_STATEMENT);
}

template class BaseAnalyzer<PendingExit>;
template class BaseAnalyzer<AssignPendingExit>;

}  // namespace ark::es2panda::checker
