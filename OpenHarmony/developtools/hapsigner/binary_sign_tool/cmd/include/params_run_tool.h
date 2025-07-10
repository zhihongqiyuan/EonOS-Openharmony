/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIGNATRUETOOLS_HAP_SIGN_TOOL_H
#define SIGNATRUETOOLS_HAP_SIGN_TOOL_H

#include <unordered_map>
#include <string>
#include <vector>

#include "cmd_util.h"
#include "sign_tool_service_impl.h"
#include "signature_tools_log.h"
#include "params_trust_list.h"
#include "string_utils.h"

namespace OHOS {
namespace SignatureTools {
class ParamsRunTool final {
public:
    ParamsRunTool() = delete;
    static bool ProcessCmd(char** args, size_t size);
    static bool DispatchParams(const ParamsSharedPtr& params, SignToolServiceImpl& api);
    static bool RunSignApp(Options* params, SignToolServiceImpl& api);
    static bool CheckProfile(Options& params);
    static void PrintHelp();
    static void Version();

private:
    static const std::string VERSION;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_HAP_SIGN_TOOL_H
