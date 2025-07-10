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

#ifndef ES2PANDA_COMPILER_PLUGIN_PHASE_H
#define ES2PANDA_COMPILER_PLUGIN_PHASE_H

#include "compiler/lowering/phase.h"
#include "util/plugin.h"

namespace ark::es2panda::compiler {

class PluginPhase : public Phase {
public:
    constexpr PluginPhase(char const *name, es2panda_ContextState contextState,
                          void (util::Plugin::*methodCall)(es2panda_Context *) const) noexcept
        : name_(name), contextState_(contextState), methodCall_(methodCall)
    {
    }

    std::string_view Name() const override
    {
        return name_;
    }

    bool Perform(public_lib::Context *ctx, parser::Program *program) override;

private:
    char const *name_;
    es2panda_ContextState contextState_;
    void (util::Plugin::*methodCall_)(es2panda_Context *) const;
};

}  // namespace ark::es2panda::compiler

#endif
