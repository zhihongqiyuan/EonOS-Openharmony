/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
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

#ifndef COMMAND_HELPER_H
#define COMMAND_HELPER_H

#include <string>
#include <array>

class CommandHelper {
public:
    ~CommandHelper() = default;
    static inline CommandHelper& GetInstance()
    {
        static CommandHelper instance {};
        return instance;
    }

    int Start(int* argc, char*** argv, const std::string& help);
private:
    CommandHelper() = default;
    void DoHelp(const std::string& help);

    enum ArgNames:int {
        ARG_NONG = 0,
        ARG_HELP,
        ARG_OUTPUT_FILE,
        ARG_EXCLUDE_TRACER,
        ARG_MAX_STACK_DEPTH,
        ARG_DURATION,
        ARG_EVENTS,
        ARG_PIDS,
        ARG_DUMP_EVENTS,
        ARG_UNWIND_STACK,
        ARG_BPF_LOG_LEVEL,
        ARG_BPF_LOG_FILE,
        ARG_LIBBPF_LOG_LEVEL,
        ARG_LIBBPF_LOG_FILE,
        ARG_HHLOG_LEVEL,
        ARG_HHLOG_FILE,
        ARG_SERVER_START,
        ARG_SERVER_STOP,
        NR_SUPPORTED_ARGS,
    };

    const std::array<const std::string, NR_SUPPORTED_ARGS> SUPPORTED_ARGS {
        "none",
        "help",
        "output_file",
        "exclude_tracer",
        "max_stack_depth",
        "duration",
        "events",
        "pids",
        "dump_events",
        "unwind_stack",
        "bpf_log_level",
        "bpf_log_file",
        "libbpf_log_level",
        "libbpf_log_file",
        "hhlog_level",
        "hhlog_file",
        "start",
        "stop"
    };

    const std::string tipHelp_ {
        "Tip:\n"
        "    I suppose you don't know the command usage, ignore this tip if you do,\n"
        "    run 'hiebpf --help help' for help if you don't\n"};
    const std::string helpHelp_ {
        "command usage: hiebpf [--<arg> <val>]\n"
        "supported arguments:\n"
        "    --help <arg>:\n"
        "        show help message of the specified argument, '--help help' shows the\n"
        "        current message\n"
        "    --output_file <filepath>:\n"
        "        the file used to save hiebpf data. default: /data/local/tmp/hiebpf.data\n"
        "    --exclude_tracer <true|false>:\n"
        "        controlls exclude to trace the command itself. default: true\n"
        "    --dump_events <val>:\n"
        "        maxinum number of events to print to console. default:0\n"
        "    --unwind_stack <true|false>:\n"
        "        whether enable unwinding or not. default:true.\n"
        "    --max_stack_depth <val>:\n"
        "        max stack depth to unwind\n"
        "    --duration <secs>:\n"
        "        the trace will run for <sec> seconds. defalut: 0\n"
        "    --events <event1[<,event2>]>:\n"
        "        configure event category.\n"
        "    --pids <pid1[<,pid2>]>:\n"
        "        pids of target process to trace. if not be set, trace system.\n"
        "    --bpf_log_level <level>:\n"
        "        log level of BPF programs. default: NONE\n"
        "    --bpf_log_file <filepath>:\n"
        "        the file used to save bpf logs. default: /data/local/tmp/bpf.log\n"
        "    --libbpf_log_level <level>:\n"
        "        level of libbpf's internal log. default:NONE\n"
        "    --libbpf_log_file <filepath>:\n"
        "        the file used to save libbpf logs. default: /data/local/tmp/libbpf.log\n"
        "    --hhlog_level <level>:\n"
        "        lowest level of HHLog to print\n"
        "    --hhlog_file <filepath>:\n"
        "        the file used to save HHLog. default: /data/local/tmp/hhlog.txt\n"
        "    --start <true|false>:\n"
        "        whether start hiebpf server. default: false\n"
        "    --stop <true|false>:\n"
        "        whether stop hiebpf server. default: false\n"
    };
    const std::string outputFileHelp_ {
        "argument usage:\n"
        "    --output_file <filepath>: the file used to save hiebpf data. default:/data/local/tmp/hiebpf.data\n"};
    const std::string excludeTracerHelp_ {
        "argument usage:\n"
        "    --exclude_tracer <true|false>: exclude to trace the command itself. default: true\n"};
    const std::string maxStackDepthHelp_ {
        "argument usage:\n"
        "    --max_stack_depth <depth>: max unwinding stack depth. default:\n"};
    const std::string durationHelp_ {
        "argument usage:\n"
        "    --duration <secs>: the tracer will run for <duration> seconds. default:0s, maxinum number:3600s"};
    const std::string eventsHelp_ {
        "argument usage:\n"
        "    --events <EVENT>: target events to trace, multiple events are seprated by comma\n"
        "supported <EVENT>:\n"
        "    fs        - all events of file system\n"
        "    fs:open   - open events of file system\n"
        "    fs:close  - close events of file system\n"
        "    fs:read   - read events of file system\n"
        "    fs:write  - write events of file system\n"
        "    ptrace    - page fault events\n"
        "    bio       - all BIO events\n"};
    const std::string pidsHelp_ {
        "argument usage:\n"
        "    --pids <pid1[<,pid2>]>: pids of target process to trace. if not be set, trace system.\n"};
    const std::string dumpEventsHelp_ {
        "argument usage:\n"
        "    --dump_events <maxinum>: maxinum number of events to print to console. default:0\n"};
    const std::string unwindStackHelp_ {
        "argument usage:\n"
        "    --unwind_stack <true|false>: whether enable unwinding or not. default: true\n"};
    const std::string bpfLogLevelHelp_ {
        "argument usage: \n"
        "    --bpf_log_level <level>: log level of BPF programs. default:NONE\n"
        "supported <level>:\n"
        "    DEBUG: \n"
        "    INFO: \n"
        "    WARN: \n"
        "    ERROR: \n"
        "    FATAL: \n"};
    const std::string bpfLogFileHelp_ {
        "argument usage: \n"
        "    --bpf_log_file <filepath>: the file used to save bpf logs. default:/data/local/tmp/bpf.log\n"};
    const std::string libbpfLogLevelHelp_ {
        "argument usage: \n"
        "    --libbpf_log_level <level>: level of libbpf's internal log. default:NONE\n"
        "supported <level>:\n"
        "    DEBUG: \n"
        "    INFO: \n"
        "    WARN: \n"
        "    ERROR: \n"
        "    FATAL: \n"};
    const std::string libbpfFileHelp_ {
        "argument usage: \n"
        "    --libbpf_log_file <filepath>: the file used to save bpf logs. default:/data/local/tmp/libbpf.log\n"};
    const std::string hhLogLevelHelp_ {
        "argument usage: \n"
        "    --hhlog_level <level>: lowest level of HHLog to print. default:NONE\n"
        "supported <level>:\n"
        "    DEBUG: \n"
        "    INFO: \n"
        "    WARN: \n"
        "    ERROR: \n"
        "    FATAL: \n"};
    const std::string hhLogFileHelp_ {
        "argument usage: \n"
        "    --hhlog_file <filepath>: the file used to save HHLog. default: /data/local/tmp/hhlog.txt\n"};
    const std::string serverStartHelp_ {
        "argument usage:\n"
        "    --start <true|false>: whether start hiebpf server. default: false\n"};
    const std::string serverStopHelp_ {
        "argument usage:\n"
        "    --stop <true|false>: whether stop hiebpf server. default: false\n"};
};

#endif
