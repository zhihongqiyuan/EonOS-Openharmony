/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef HIPERF_PERF_PIPE
#define HIPERF_PERF_PIPE

#include <fstream>
#include <stdint.h>
#include "utilities.h"
#include "perf_events.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
enum class CommandType {
    RECORD = 0,
    STAT,
};
const std::string CONTROL_CMD_PREPARE = "prepare";
const std::string CONTROL_CMD_START = "start";
const std::string CONTROL_CMD_PAUSE = "pause";
const std::string CONTROL_CMD_RESUME = "resume";
const std::string CONTROL_CMD_OUTPUT = "output";
const std::string CONTROL_CMD_STOP = "stop";
class PerfPipe {
private:
    std::string fifoFileC2S_;
    std::string fifoFileS2C_;
    std::string controlCmd_;
    std::string perfCmd_;
    bool outputEnd_ = false;

public:
    void SetFifoFileName(const CommandType& commandType, std::string& controlCmd,
        std::string& fifoFileC2S, std::string& fifoFileS2C);
    void RemoveFifoFile();
    bool CreateFifoFile();
    bool SendFifoAndWaitReply(const std::string &cmd, const std::chrono::milliseconds &timeOut);
    bool WaitFifoReply(int fd, const std::chrono::milliseconds &timeOut);
    void WaitFifoReply(int fd, const std::chrono::milliseconds &timeOut, std::string& reply);
    void ProcessStopCommand(bool ret);
    void ProcessOutputCommand(bool ret);
    bool ProcessControlCmd();
    void SetOutPutEnd(bool outputEnd);
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_PERF_PIPE
