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

#ifndef HIEBPF_COMMAND_H
#define HIEBPF_COMMAND_H

#include <csignal>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

#include "bpf_controller.h"
#include "elf_symbol_info.h"
#include "maps_info.h"
#include "ring_buffer.h"
#include "hiebpf_common.h"
class Command {
public:
    bool DispatchCommand(std::vector<std::string> args);
    Command();
    void DoWork();

private:
    bool ParseArgs(std::vector<std::string> arguments);
    int GetClockId(const std::string &str)
    {
        if (str == "realtime") {
            return CLOCK_TYPE::RealTime;
        } else if (str == "boottime") {
            return CLOCK_TYPE::BootTime;
        } else if (str == "monotonic") {
            return CLOCK_TYPE::Monotonic;
        } else if (str == "monotonicraw") {
            return CLOCK_TYPE::MonotonicRaw;
        }
        return -1;
    }

    void PrintHelp();
    static int EventReceiver(BPFEvent* bpfEvent);

    void WriteHeader();
    void WriteEventMaps(uint32_t pid);
    void WriteSymbolInfo(const std::string &fileName);
    void WriteEventFs(uint8_t* data);
    void WriteEventMem();
private:
    bool helpFlag_ = false;
    BPFConfig bpfConfig;
    std::unique_ptr<BPFController> bpfControllerPtr_;
    // event type
    std::vector<std::string> events_;
    int clockId_ = CLOCK_TYPE::BootTime;
    int duration_ = 1000;
    std::string symbolFilePath_;
    std::string symbolFileName_;
    std::string fileName_ = "/data/local/tmp/ebpf.data";
    std::vector<int> pids_;
    std::string help_;
    std::ofstream stream_ {};
    std::string cmdLine_;
    std::thread writeFileThread_;
    bool ThreadRunningFlag_ = true;

    OHOS::Developtools::Hiebpf::MapsInfo mapsInfo_;
    OHOS::Developtools::Hiebpf::ElfSymbolInfo elfSymbolInfo_;

    void GetArgsValues(const FSTraceEvent* bpfEvent, uint64_t* args, std::vector<ArgStr>& argStr);
    void EventLoop();
    bool CheckPids(std::vector<int>& pids);
    bool SetTargetPids();
    bool SetTargetEvents();
};
#endif
