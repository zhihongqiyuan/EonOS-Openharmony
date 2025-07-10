/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include <sys/utsname.h>
#include "subcommand_list.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
HiperfError SubCommandList::OnSubCommand(std::vector<std::string>& args)
{
    std::vector<perf_type_id> requestEventTypes;

    SetHM();

    if (args.empty()) {
        // all the list
        for (auto it : PERF_TYPES) {
            requestEventTypes.push_back(it.first);
        }
    } else {
        std::string requestEventType = args.front();
        auto it = SUPPORT_NAME_OPTIONS.find(requestEventType);
        if (it == SUPPORT_NAME_OPTIONS.end()) {
            printf("not support option: '%s'\n", requestEventType.c_str());
            return HiperfError::OPTION_NOT_SUPPORT;
        } else {
            requestEventTypes.push_back(it->second);
        }
    }
    ShowSupportEventsTypes(requestEventTypes);
    return HiperfError::NO_ERR;
}

bool SubCommandList::ShowSupportEventsTypes(std::vector<perf_type_id> &requestEventTypes)
{
    // each type
    for (perf_type_id id : requestEventTypes) {
        // each config
        auto configs = perfEvents_.GetSupportEvents(id);
        printf("\nSupported events for %s:\n", PERF_TYPES.at(id).c_str());
        for (auto config : configs) {
            printf("\t%s\n", config.second.c_str());
        }
        if (configs.size() == 0) {
            return false; // support nothing
        }
    }
    std::cout << std::endl;
    return true;
};

void SubCommandList::RegisterSubCommandList()
{
    SubCommand::RegisterSubCommand("list", SubCommandList::GetInstance);
}

void SubCommandList::SetHM()
{
    utsname unameBuf;
    if ((uname(&unameBuf)) == 0) {
        std::string osrelease = unameBuf.release;
        isHM_ = osrelease.find(HMKERNEL) != std::string::npos;
    }
    perfEvents_.SetHM(isHM_);
    HLOGD("Set isHM_: %d", isHM_);
}

SubCommand& SubCommandList::GetInstance()
{
    static SubCommandList subCommand;
    return subCommand;
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
