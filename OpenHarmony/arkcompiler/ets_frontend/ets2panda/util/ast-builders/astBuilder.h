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

#ifndef ES2PANDA_UTIL_INCLUDE_AST_BUILDER
#define ES2PANDA_UTIL_INCLUDE_AST_BUILDER

#include "mem/arena_allocator.h"
#include "util/helpers.h"

namespace ark::es2panda::ir {

template <typename NodeType>
class AstBuilder {
public:
    explicit AstBuilder(ark::ArenaAllocator *allocator) : allocator_(allocator) {}
    NO_COPY_SEMANTIC(AstBuilder);
    NO_MOVE_SEMANTIC(AstBuilder);
    virtual ~AstBuilder() = default;

    ark::ArenaAllocator *Allocator()
    {
        return allocator_;
    }

    template <typename BuilderType>
    BuilderType &SetParent(AstNode *parent)
    {
        parent_ = parent;
        return *static_cast<BuilderType *>(this);
    }

    template <typename... Args>
    NodeType *AllocNode(Args &&...args)
    {
        auto node = util::NodeAllocator::ForceSetParent<NodeType>(allocator_, std::forward<Args>(args)...);
        if (parent_ != nullptr) {
            node->SetParent(parent_);
        }
        return node;
    }

private:
    ark::ArenaAllocator *allocator_;
    AstNode *parent_ {};
};

}  // namespace ark::es2panda::ir
#endif  // ES2PANDA_UTIL_INCLUDE_AST_BUILDER