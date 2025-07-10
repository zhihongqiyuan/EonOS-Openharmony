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

#include "parserContext.h"
#include "parser/program/program.h"

namespace ark::es2panda::parser {

ParserContext::ParserContext(const Program *program, ParserStatus status)
    : program_(program), status_(status), lang_(ToLanguage(program->Extension()))
{
}

const ParserContext *ParserContext::FindLabel(const util::StringView &label) const
{
    const auto *iter = this;
    while (iter != nullptr && !iter->label_.Empty()) {
        if (iter->label_ == label) {
            return iter;
        }

        iter = iter->prev_;
    }

    return nullptr;
}
}  // namespace ark::es2panda::parser
