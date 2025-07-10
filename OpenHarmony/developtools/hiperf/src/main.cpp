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

#include <cstdio>
#include <iostream>

#include "command.h"
#include "debug_logger.h"
#if defined(is_ohos) && is_ohos
#include "hiperf_hilog.h"
#endif
#include "option_debug.h"
#include "subcommand.h"
#include "subcommand_help.h"
#include "utilities.h"
#if SUPPORT_PERF_EVENT
#include "subcommand_list.h"
#include "subcommand_record.h"
#include "subcommand_stat.h"
#endif
#include "subcommand_dump.h"
#include "subcommand_report.h"

using namespace OHOS::Developtools::HiPerf;

#ifdef FUZZER_TEST
#define main HiperfFuzzerMain
#endif

int main(const int argc, const char *argv[])
{
    if (!GetDeveloperMode() && !IsAllowProfilingUid()) {
        printf("error: not in developermode, exit.\n");
        return -1;
    }

    if (argc > 128) { // 128 : max input argument counts
        printf("The number of input arguments exceeds the upper limit.\n");
        return -1;
    }
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

#if defined(is_ohos) && is_ohos
    WriteStringToFile("/proc/self/oom_score_adj", "0");
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        HIPERF_HILOGI(MODULE_DEFAULT, "ignore SIGPIPE failed.");
    }
    HIPERF_HILOGI(MODULE_DEFAULT, "hiperf start.");
#endif

    // pass the argv to next
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }

    if (args.empty()) {
        printf("no command input.\n");
        args.push_back("help"); // no cmd like help cmd
    }

// register all the main command
#ifdef HIPERF_DEBUG
    RegisterMainCommandDebug();
#endif

    // register all the sub command
    SubCommandHelp::RegisterSubCommandHelp();

#if SUPPORT_PERF_EVENT
    RegisterSubCommandStat();
    SubCommandList::RegisterSubCommandList();
    SubCommandRecord::RegisterSubCommandRecord();
#endif

    SubCommandDump::RegisterSubCommandDump();
    SubCommandReport::RegisterSubCommandReport();
#ifdef FUZZER_TEST
    bool isRecordCmd = false;
    if (args[0] == "record") {
        isRecordCmd = true;
    }
#endif
    // tell sub cmd to do the process
    Command::DispatchCommands(args);

#ifdef FUZZER_TEST
    SubCommand::ClearSubCommands();
    Option::ClearMainOptions();

    if (isRecordCmd) {
        const uint64_t msWaitSysReleaseThread = 100;
        std::this_thread::sleep_for(std::chrono::milliseconds(msWaitSysReleaseThread));
    }
#endif

    HLOGD("normal exit.");
    return 0;
}
