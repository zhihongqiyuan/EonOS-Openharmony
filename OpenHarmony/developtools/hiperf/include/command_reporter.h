/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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
#ifndef COMMAND_REPORTER_H_
#define COMMAND_REPORTER_H_

#include <string>
namespace OHOS::Developtools::HiPerf {

#define MAKE_ERROR_ITEM(GEN_ITEM)                        \
    GEN_ITEM(NO_ERR),                           /*  0 */ \
    GEN_ITEM(PROCESS_CONTROL_FAIL),             /*  1 */ \
    GEN_ITEM(PREPARE_SYS_KERNEL_FAIL),          /*  2 */ \
    GEN_ITEM(PREPARE_PERF_EVENT_FAIL),          /*  3 */ \
    GEN_ITEM(PREPARE_TACKING_FAIL),             /*  4 */ \
    GEN_ITEM(CREATE_OUTPUT_FILE_FAIL),          /*  5 */ \
    GEN_ITEM(PREPARE_VIRTUAL_RUNTIME_FAIL),     /*  6 */ \
    GEN_ITEM(PREPARE_START_TRACKING_FAIL),      /*  7 */ \
    GEN_ITEM(START_TRACKING_FAIL),              /*  8 */ \
    GEN_ITEM(FINISH_WRITE_RECORD_FILE_FAIL),    /*  9 */ \
    GEN_ITEM(POST_PROCESS_RECORD_FILE),         /* 10 */ \
    GEN_ITEM(CHECK_RESTART_OPTION_FAIL),        /* 11 */ \
    GEN_ITEM(CHECK_SELECT_CPU_PID_FAIL),        /* 12 */ \
    GEN_ITEM(CHECK_STAT_OPTION_FAIL),           /* 13 */ \
    GEN_ITEM(CHECK_APP_RUNNING_FAIL),           /* 14 */ \
    GEN_ITEM(CHECK_OPTION_PID_FAIL),            /* 15 */ \
    GEN_ITEM(CHECK_OPTION_PID_APP_FAIL),        /* 16 */ \
    GEN_ITEM(PREPAIR_EVENTS_FAIL),              /* 17 */ \
    GEN_ITEM(PREPARE_OUTPUT_FAIL),              /* 18 */ \
    GEN_ITEM(LOAD_PERF_DATA_FAIL),              /* 19 */ \
    GEN_ITEM(LOAD_SECOND_PERF_DATA_FAIL),       /* 20 */ \
    GEN_ITEM(OUTPUT_REPORT_FAIL),               /* 21 */ \
    GEN_ITEM(PREPARE_DUMP_OUTPUT_FAIL),         /* 22 */ \
    GEN_ITEM(DUMP_ELF_FILE_ERROR),              /* 23 */ \
    GEN_ITEM(DUMP_PROTO_FILE_ERROR),            /* 24 */ \
    GEN_ITEM(ACCESS_DATA_FILE_FAIL),            /* 25 */ \
    GEN_ITEM(OPEN_DATA_FILE_FAIL),              /* 26 */ \
    GEN_ITEM(SET_SYMBOLS_PATH_FAIL),            /* 27 */ \
    GEN_ITEM(OPTION_NOT_SUPPORT),               /* 28 */ \
    GEN_ITEM(SUBCOMMAND_OPTIONS_ERROR),         /* 29 */ \
    GEN_ITEM(CHECK_OUT_PUT_ERROR),              /* 30 */ \
    GEN_ITEM(WRONG_CONTROL_CMD),                /* 31 */ \
    GEN_ITEM(CHECK_DEBUG_APP_FAIL)              /* 32 */

#define FOR_ERROR_ENUM(x) x

enum class HiperfError : int32_t {
    MAKE_ERROR_ITEM(FOR_ERROR_ENUM)
};

class CommandReporter {
public:
    explicit CommandReporter(const std::string& fullArgument);
    ~CommandReporter();

    void ReportCommand();

    std::string mainCommand_ = "";
    std::string subCommand_ = "";
    std::string caller_ = "";
    std::string targetProcess_ = "";
    HiperfError errorCode_ = HiperfError::NO_ERR;

private:
    bool isReported_ = false;

    CommandReporter(const CommandReporter&) = delete;
    CommandReporter& operator=(const CommandReporter&) = delete;
    CommandReporter(CommandReporter&&) = delete;
    CommandReporter& operator=(CommandReporter&&) = delete;
};

} // namespace OHOS::Developtools::HiPerf

#endif // COMMAND_REPORTER_H_