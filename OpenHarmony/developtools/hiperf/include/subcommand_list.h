/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#ifndef SUBCOMMAND_LIST_H_
#define SUBCOMMAND_LIST_H_

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "debug_logger.h"
#include "option.h"
#include "perf_events.h"
#include "subcommand.h"
#include "utilities.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
class SubCommandList : public SubCommand {
public:
    SubCommandList()
        // clang-format off
        : SubCommand("list", "List the supported event types.",
        "Usage: hiperf list [event type name]\n"
        "       List all supported event types on this devices.\n"
        "   To list the events of a specific type, specify the type name\n"
        "       hw          hardware events\n"
        "       sw          software events\n"
        "       tp          tracepoint events\n"
        "       cache       hardware cache events\n"
        "       raw         raw pmu events\n\n"
        )
    // clang-format on
    {
    }

    HiperfError OnSubCommand(std::vector<std::string>& args) override;

    static void RegisterSubCommandList(void);

    static SubCommand& GetInstance();
private:
    PerfEvents perfEvents_;

    const std::map<std::string, perf_type_id> SUPPORT_NAME_OPTIONS = {
        {"hw", PERF_TYPE_HARDWARE},    {"sw", PERF_TYPE_SOFTWARE}, {"tp", PERF_TYPE_TRACEPOINT},
        {"cache", PERF_TYPE_HW_CACHE}, {"raw", PERF_TYPE_RAW},
    };

    bool ShowSupportEventsTypes(std::vector<perf_type_id> &requestEventTypes);
    void SetHM();
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // SUBCOMMAND_LIST_H_
