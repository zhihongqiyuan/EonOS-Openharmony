
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

#ifndef ES2PANDA_COMPILER_CHECKER_ETS_PRIMITIVE_WRAPPERS_H
#define ES2PANDA_COMPILER_CHECKER_ETS_PRIMITIVE_WRAPPERS_H

#include "checker/types/ets/etsObjectType.h"

namespace ark::es2panda::checker {
class ETSObjectType;

using WrapperDesc = ArenaUnorderedMap<util::StringView, std::pair<ETSObjectType *, ETSObjectFlags>>;

class PrimitiveWrappers {
public:
    explicit PrimitiveWrappers(ArenaAllocator *allocator) : wrappers_(allocator->Adapter())
    {
        wrappers_.insert({"Boolean", {nullptr, ETSObjectFlags::BUILTIN_BOOLEAN}});
        wrappers_.insert({"Byte", {nullptr, ETSObjectFlags::BUILTIN_BYTE}});
        wrappers_.insert({"Char", {nullptr, ETSObjectFlags::BUILTIN_CHAR}});
        wrappers_.insert({"Short", {nullptr, ETSObjectFlags::BUILTIN_SHORT}});
        wrappers_.insert({"Int", {nullptr, ETSObjectFlags::BUILTIN_INT}});
        wrappers_.insert({"Long", {nullptr, ETSObjectFlags::BUILTIN_LONG}});
        wrappers_.insert({"Float", {nullptr, ETSObjectFlags::BUILTIN_FLOAT}});
        wrappers_.insert({"Double", {nullptr, ETSObjectFlags::BUILTIN_DOUBLE}});
    }
    NO_COPY_SEMANTIC(PrimitiveWrappers);
    NO_MOVE_SEMANTIC(PrimitiveWrappers);
    ~PrimitiveWrappers() = default;

    WrapperDesc &Wrappers()
    {
        return wrappers_;
    }

    const WrapperDesc &Wrappers() const
    {
        return wrappers_;
    }

private:
    WrapperDesc wrappers_;
};
}  // namespace ark::es2panda::checker
#endif
