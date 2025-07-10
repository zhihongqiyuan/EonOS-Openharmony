
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

#ifndef ES2PANDA_IR_ETS_IMPORT_SOURCE_H
#define ES2PANDA_IR_ETS_IMPORT_SOURCE_H

#include "checker/types/type.h"
#include "ir/astNode.h"
#include "ir/expressions/literals/stringLiteral.h"
#include "util/language.h"

namespace ark::es2panda::ir {

class ImportSource {
public:
    explicit ImportSource(ir::StringLiteral *source, ir::StringLiteral *resolvedSource, Language lang, bool hasDecl)
        : source_(source), resolvedSource_(resolvedSource), lang_(lang), hasDecl_(hasDecl)
    {
    }
    NO_COPY_SEMANTIC(ImportSource);
    NO_MOVE_SEMANTIC(ImportSource);
    ~ImportSource() = default;

    const ir::StringLiteral *Source() const
    {
        return source_;
    }

    ir::StringLiteral *Source()
    {
        return source_;
    }

    const ir::StringLiteral *ResolvedSource() const
    {
        return resolvedSource_;
    }

    ir::StringLiteral *ResolvedSource()
    {
        return resolvedSource_;
    }

    es2panda::Language Language() const
    {
        return lang_;
    }

    bool HasDecl() const
    {
        return hasDecl_;
    }

private:
    ir::StringLiteral *source_ {};
    ir::StringLiteral *resolvedSource_ {};
    es2panda::Language lang_;
    bool hasDecl_;
};

}  // namespace ark::es2panda::ir
#endif