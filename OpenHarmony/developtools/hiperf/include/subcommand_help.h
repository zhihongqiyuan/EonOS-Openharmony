/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#ifndef HIPERF_SUBCOMMAND_HELP_H_
#define HIPERF_SUBCOMMAND_HELP_H_

#include "option.h"
#include "subcommand.h"
namespace OHOS {
namespace Developtools {
namespace HiPerf {
class SubCommandHelp : public SubCommand {
public:
    SubCommandHelp()
        // clang-format off
        : SubCommand("help", "Show more help information for hiperf",
        "Usage: hiperf help [subcommand]\n"
        "    By default, all options help information and subcommand brief information are output.\n"
        "    If you provide a subcommand, only the help information for this command will be output.\n\n"
        )
    // clang-format on
    {
    }

    HiperfError OnSubCommand(std::vector<std::string>& args) override;
    static void RegisterSubCommandHelp(void);
    static bool OnHelp(std::vector<std::string> &args);

    static SubCommand& GetInstance();
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_SUBCOMMAND_HELP_H_
