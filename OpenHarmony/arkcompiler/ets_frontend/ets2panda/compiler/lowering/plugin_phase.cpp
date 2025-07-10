/**
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include "plugin_phase.h"

namespace ark::es2panda::compiler {

bool PluginPhase::Perform(public_lib::Context *ctx, [[maybe_unused]] parser::Program *program)
{
    ctx->state = contextState_;

    if (ctx->plugins == nullptr) {
        return true;
    }

    for (auto &plugin : *(ctx->plugins)) {
        (plugin.*methodCall_)(reinterpret_cast<es2panda_Context *>(ctx));
        if (ctx->state == ES2PANDA_STATE_ERROR) {
            ctx->checker->LogTypeError(ctx->errorMessage, ctx->errorPos);
            return false;
        }
    }

    return true;
}

}  // namespace ark::es2panda::compiler
