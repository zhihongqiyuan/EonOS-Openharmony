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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_BASE_ANALYZER_H
#define ES2PANDA_COMPILER_CHECKER_ETS_BASE_ANALYZER_H

#include "utils/arena_containers.h"
#include "util/enumbitops.h"

namespace ark::es2panda::ir {
class AstNode;
enum class AstNodeType;
}  // namespace ark::es2panda::ir

namespace ark::es2panda::checker {
class ETSChecker;

using ENUMBITOPS_OPERATORS;

enum class LivenessStatus { DEAD, ALIVE };

class PendingExit {
public:
    using JumpResolver = std::function<void()>;

    explicit PendingExit(
        const ir::AstNode *node, JumpResolver jumpResolver = [] {})
        : node_(node), jumpResolver_(std::move(jumpResolver))
    {
    }
    virtual ~PendingExit() = default;

    DEFAULT_COPY_SEMANTIC(PendingExit);
    DEFAULT_NOEXCEPT_MOVE_SEMANTIC(PendingExit);

    virtual void ResolveJump()
    {
        jumpResolver_();
    }

    const ir::AstNode *Node() const
    {
        return node_;
    }

private:
    const ir::AstNode *node_;
    JumpResolver jumpResolver_;
};

template <typename T>
class BaseAnalyzer {
public:
    using PendingExitsVector = std::vector<T>;

    explicit BaseAnalyzer() = default;

    virtual void MarkDead() = 0;

    void RecordExit(const T &pe)
    {
        pendingExits_.push_back(pe);
        MarkDead();
    }

    LivenessStatus From(bool value)
    {
        return value ? LivenessStatus::ALIVE : LivenessStatus::DEAD;
    }

    LivenessStatus ResolveJump(const ir::AstNode *node, ir::AstNodeType jumpKind);
    LivenessStatus ResolveContinues(const ir::AstNode *node);
    LivenessStatus ResolveBreaks(const ir::AstNode *node);
    const ir::AstNode *GetJumpTarget(const ir::AstNode *node) const;

protected:
    void ClearPendingExits();
    PendingExitsVector &PendingExits();
    void SetPendingExits(const PendingExitsVector &pendingExits);
    PendingExitsVector &OldPendingExits();
    void SetOldPendingExits(const PendingExitsVector &oldPendingExits);

private:
    PendingExitsVector pendingExits_;
    PendingExitsVector oldPendingExits_;
};
}  // namespace ark::es2panda::checker

template <>
struct enumbitops::IsAllowedType<ark::es2panda::checker::LivenessStatus> : std::true_type {
};

#endif
