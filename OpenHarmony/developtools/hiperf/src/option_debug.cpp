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

#include "option_debug.h"
#include "debug_logger.h"
#include "option.h"
namespace OHOS {
namespace Developtools {
namespace HiPerf {
static bool OnVerboseLevel(const std::vector<std::string> &debugLevel)
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        if (debugLevel.size() == 0) {
            return false;
        }
        DebugLogger::GetInstance()->SetLogLevel(LEVEL_VERBOSE);
        DebugLogger::GetInstance()->Disable(false);
    }
#endif
    return true;
}

static bool OnMuchLevel(const std::vector<std::string> &debugLevel)
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        if (debugLevel.size() == 0) {
            return false;
        }
        DebugLogger::GetInstance()->SetLogLevel(LEVEL_MUCH);
        DebugLogger::GetInstance()->Disable(false);
    }
#endif
    return true;
}

static bool OnDebugLevel(const std::vector<std::string> &debugLevel)
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        if (debugLevel.size() == 0) {
            return false;
        }
        DebugLogger::GetInstance()->SetLogLevel(LEVEL_DEBUG);
        DebugLogger::GetInstance()->Disable(false);
    }
#endif
    return true;
}

static bool OnNoDebug(const std::vector<std::string> &debugLevel)
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        if (debugLevel.size() == 0) {
            return false;
        }
        DebugLogger::GetInstance()->Disable();
    }
#endif
    return true;
}

static bool OnMixLogOutput(const std::vector<std::string> &debugLevel)
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        if (debugLevel.size() == 0) {
            return false;
        }
        DebugLogger::GetInstance()->SetMixLogOutput(true);
    }
#endif
    return true;
}

static bool OnLogPath(std::vector<std::string> &args)
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        if (args.size() > 0) {
            DebugLogger::GetInstance()->SetLogPath(args[0]);
            args.erase(args.begin());
        } else {
            return false;
        }
    }
#endif
    return true;
}

static bool OnLogTag(std::vector<std::string> &args)
{
#if defined(is_ohos) && is_ohos
    if (IsRoot()) {
        if (args.size() > 0) {
            DebugLogger::GetInstance()->SetLogTags(args[0]);
            args.erase(args.begin());
        } else {
            return false;
        }
    }
#endif
    return true;
}
#if is_ohos && !is_double_framework
static bool OnHiLog(const std::vector<std::string> &args)
{
    DebugLogger::GetInstance()->EnableHiLog();
    return true;
}
#endif
void RegisterMainCommandDebug()
{
    Option::RegisterMainOption("--nodebug", "disable debug log, usage format: --nodebug [command] [args]",
                               OnNoDebug);
    Option::RegisterMainOption("--debug", "show debug log, usage format: --debug [command] [args]",
                               OnDebugLevel);
    Option::RegisterMainOption("--verbose", "show debug log, usage format: --verbose [command] [args]",
                               OnVerboseLevel);
    Option::RegisterMainOption("--much", "show extremely much debug log, usage format: --much [command] [args]",
                               OnMuchLevel);
    Option::RegisterMainOption("--mixlog", "mix the log in output, usage format: --much [command] [args]",
                               OnMixLogOutput);
    Option::RegisterMainOption("--logpath",
                               "log file name full path, usage format: --logpath [filepath] [command] [args]",
                               OnLogPath);
    std::string tagUsage = StringPrintf("%s\t%-20s\t%s\t%-20s\t%s",
        "enable log level for HILOG_TAG, usage format: --logtag <tag>[:level][,<tag>[:level]] [command] [args]\n", " ",
        "tag: Dump, Report, Record, Stat... level: D, V, M...\n", " ",
        "example: hiperf --verbose --logtag Record:D [command] [args]");
    Option::RegisterMainOption("--logtag", tagUsage.c_str(), OnLogTag);
#if is_ohos && !is_double_framework
    Option::RegisterMainOption("--hilog", "use hilog not file to record log", OnHiLog);
#endif
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
