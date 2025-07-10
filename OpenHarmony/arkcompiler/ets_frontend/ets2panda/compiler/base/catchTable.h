/*
 * Copyright (c) 2021 - 2023 Huawei Device Co., Ltd.
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

#ifndef ES2PANDA_COMPILER_BASE_CATCH_TABLE_H
#define ES2PANDA_COMPILER_BASE_CATCH_TABLE_H

#include "ir/irnode.h"
#include "compiler/core/labelPair.h"

namespace ark::es2panda::compiler {
class CodeGen;

class TryLabelSet {
public:
    explicit TryLabelSet(CodeGen *cg);
    explicit TryLabelSet(CodeGen *cg, LabelPair tryLabelPair);

    ~TryLabelSet() = default;
    DEFAULT_COPY_SEMANTIC(TryLabelSet);
    DEFAULT_MOVE_SEMANTIC(TryLabelSet);

    const LabelPair &TryLabelPair() const
    {
        return try_;
    }

    const LabelPair &CatchLabelPair() const
    {
        return catch_;
    }

    Label *TryBegin() const
    {
        return try_.Begin();
    }

    Label *TryEnd() const
    {
        return try_.End();
    }

    Label *CatchBegin() const
    {
        return catch_.Begin();
    }

    Label *CatchEnd() const
    {
        return catch_.End();
    }

private:
    LabelPair try_;
    LabelPair catch_;
};

class CatchTable {
public:
    CatchTable(CodeGen *cg, uint32_t depth, util::StringView exceptionType)
        : labelSet_(cg), depth_(depth), exceptionType_(exceptionType)
    {
    }
    CatchTable(CodeGen *cg, uint32_t depth, LabelPair tryLabelPair, util::StringView exceptionType)
        : labelSet_(cg, tryLabelPair), depth_(depth), exceptionType_(exceptionType)
    {
    }

    ~CatchTable() = default;
    NO_COPY_SEMANTIC(CatchTable);
    NO_MOVE_SEMANTIC(CatchTable);

    const TryLabelSet &LabelSet() const
    {
        return labelSet_;
    }

    uint32_t Depth() const
    {
        return depth_;
    }

    std::string Exception() const
    {
        return exceptionType_.Mutf8();
    }

private:
    TryLabelSet labelSet_;
    uint32_t depth_;
    util::StringView exceptionType_ {};
};
}  // namespace ark::es2panda::compiler

#endif
