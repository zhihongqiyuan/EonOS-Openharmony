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

#include "command.h"
#include "debug_logger.h"
#include "option.h"
#include "subcommand.h"
#include "utilities.h"
namespace OHOS {
namespace Developtools {
namespace HiPerf {
std::string Command::fullArgument = "";
bool Command::DispatchCommands(std::vector<std::string> arguments)
{
    fullArgument.clear();
    for (std::string arg : arguments) {
        fullArgument.append(" ");
        fullArgument.append(arg);
    }
    CommandReporter reporter(fullArgument);
    HLOGD("args:%s", VectorToString(arguments).c_str());
    while (!arguments.empty()) {
        // we need found main command args first
        auto commandOption = Option::FindMainOption(arguments.front());
        if (commandOption != nullptr) {
            // this is a arg which we support

            // remove the arg name
            arguments.erase(arguments.begin());

            if (!commandOption->callBackFunction(arguments)) {
                printf("unknown options: %s\nUse the help command to view help.\n", arguments.front().c_str());
                return false;
            }
            // goto next args
            continue;
        } else {
            // if it is an sub command
            auto subCommand = SubCommand::FindSubCommand(arguments.front());
            if (subCommand != nullptr) {
                reporter.mainCommand_ = arguments.front();
                // this is an sub command which we support

                // remove the subcmd name
                arguments.erase(arguments.begin());

                // if we found the sub command , after it processed , we will exit
                HLOGD("OnSubCommandOptions -> %s", subCommand->Name().c_str());
                if (subCommand->OnSubCommandOptions(arguments)) {
                    subCommand->AddReportArgs(reporter);
                    // if some help cmd ?
                    if (subCommand->OnPreSubCommand()) {
                        return true;
                    }

                    HLOGD("OnSubCommand -> %s", subCommand->Name().c_str());
                    if (HiperfError err = subCommand->OnSubCommand(arguments); err != HiperfError::NO_ERR) {
                        printf("subcommand '%s' failed\n", subCommand->Name().c_str());
                        reporter.errorCode_ = err;
                        return false;
                    } else {
                        HLOGD("OnSubCommand successed");
                        return true;
                    }
                } else {
                    reporter.errorCode_ = HiperfError::SUBCOMMAND_OPTIONS_ERROR;
                    HLOGD("OnSubCommandOptions interrupt the process.");
                    return false;
                }
            } else {
                // we don't support this command
                printf("unknown args: %s\n", arguments.front().c_str());
                return false;
            }
        }
    }
    return false;
}

bool Command::DispatchCommand(std::string argument)
{
    return Command::DispatchCommands(StringSplit(argument, " "));
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
