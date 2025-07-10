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

#include "subcommand_help.h"
#include "debug_logger.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
HiperfError SubCommandHelp::OnSubCommand(std::vector<std::string>& args)
{
    HLOGV("enter");
    OnHelp(args);
    return HiperfError::NO_ERR;
}

void SubCommandHelp::RegisterSubCommandHelp()
{
    HLOGV("enter");
    Option::RegisterMainOption("--help", "show help", OnHelp);
    Option::RegisterMainOption("-h", "show help", OnHelp);
    SubCommand::RegisterSubCommand("help", SubCommandHelp::GetInstance);
}

bool SubCommandHelp::OnHelp(std::vector<std::string> &args)
{
    if (args.empty()) {
        const auto &mainOptions = Option::GetMainOptions();
        HLOGD("%zu options found:", mainOptions.size());
        printf("Usage: hiperf [options] command [args for command]\n");

        printf("options:\n");
        for (const auto &commandOption : mainOptions) {
            if (commandOption.second != nullptr) {
                printf("\t%-20s\t%s\n", commandOption.first.c_str(),
                    commandOption.second->help.c_str());
            }
        }

        auto &commands = SubCommand::GetSubCommands();
        HLOGD("%zu cmds found:", commands.size());
        printf("command:\n");
        for (const auto &command : commands) {
            if (command.second == nullptr) {
                continue;
            }
            printf("\t%s:\t%s\n", command.second().Name().c_str(), command.second().Brief().c_str());
        }
        printf("\nSee 'hiperf help [command]' for more information on a specific command.\n\n");
    } else {
        auto command = SubCommand::FindSubCommand(args.front());
        if (command != nullptr) {
            args.clear();
            printf("%s\n", command->Help().c_str());
        } else {
            printf("Unknown command: '%s'\n", args.front().c_str());
            return false;
        }
    }

    return true;
}

SubCommand& SubCommandHelp::GetInstance()
{
    static SubCommandHelp subCommand;
    return subCommand;
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
