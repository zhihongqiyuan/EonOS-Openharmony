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

#include "perf_pipe.h"
#include "hiperf_client.h"
#include "ipc_utilities.h"
#include "utilities.h"
#if defined(is_ohos) && is_ohos
#include "hiperf_hilog.h"
#endif

using namespace std::chrono;
namespace OHOS {
namespace Developtools {
namespace HiPerf {
const std::string RECORD_CONTROL_FIFO_FILE_C2S = "/data/log/hiperflog/.hiperf_record_control_c2s";
const std::string RECORD_CONTROL_FIFO_FILE_S2C = "/data/log/hiperflog/.hiperf_record_control_s2c";
const std::string STAT_CONTROL_FIFO_FILE_C2S = "/data/log/hiperflog/.hiperf_stat_control_c2s";
const std::string STAT_CONTROL_FIFO_FILE_S2C = "/data/log/hiperflog/.hiperf_stat_control_s2c";
const std::chrono::milliseconds CONTROL_WAITREPY_TIMEOUT = 2000ms;
const std::chrono::milliseconds CONTROL_WAITREPY_TIMEOUT_CHECK = 1000ms;
static constexpr uint64_t CHECK_WAIT_TIME_MS = 200;
static constexpr uint32_t MAX_CLIENT_OUTPUT_WAIT_COUNT = 240;

void PerfPipe::SetFifoFileName(const CommandType& commandType, std::string& controlCmd,
    std::string& fifoFileC2S, std::string& fifoFileS2C)
{
    if (commandType == CommandType::RECORD) {
        fifoFileC2S_ = RECORD_CONTROL_FIFO_FILE_C2S;
        fifoFileS2C_ = RECORD_CONTROL_FIFO_FILE_S2C;
        perfCmd_ = "sampling";
    } else if (commandType == CommandType::STAT) {
        fifoFileC2S_ = STAT_CONTROL_FIFO_FILE_C2S;
        fifoFileS2C_ = STAT_CONTROL_FIFO_FILE_S2C;
        perfCmd_ = "counting";
    }
    fifoFileC2S = fifoFileC2S_;
    fifoFileS2C = fifoFileS2C_;
    controlCmd_ = controlCmd;
    HLOGD("C2S:%s, S2C:%s", fifoFileC2S.c_str(), fifoFileS2C.c_str());
}

void PerfPipe::RemoveFifoFile()
{
    char errInfo[ERRINFOLEN] = { 0 };
    if (remove(fifoFileC2S_.c_str()) != 0) {
        strerror_r(errno, errInfo, ERRINFOLEN);
        HLOGE("remove %s failed, errno:(%d:%s)", fifoFileC2S_.c_str(), errno, errInfo);
        HIPERF_HILOGE(MODULE_DEFAULT, "remove %{public}s failed, errno:(%{public}d:%{public}s)",
            fifoFileC2S_.c_str(), errno, errInfo);
    }
    if (remove(fifoFileS2C_.c_str()) != 0) {
        strerror_r(errno, errInfo, ERRINFOLEN);
        HLOGE("remove %s failed, errno:(%d:%s)", fifoFileS2C_.c_str(), errno, errInfo);
        HIPERF_HILOGE(MODULE_DEFAULT, "remove %{public}s failed, errno:(%{public}d:%{public}s)",
            fifoFileS2C_.c_str(), errno, errInfo);
    }
}

bool PerfPipe::CreateFifoFile()
{
    char errInfo[ERRINFOLEN] = { 0 };
    const mode_t fifoMode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    std::string tempPath("/data/log/hiperflog/");
    if (!IsDirectoryExists(tempPath)) {
        HIPERF_HILOGI(MODULE_DEFAULT, "%{public}s not exist.", tempPath.c_str());
        if (!CreateDirectory(tempPath, HIPERF_FILE_PERM_770)) {
            HIPERF_HILOGI(MODULE_DEFAULT, "create %{public}s failed.", tempPath.c_str());
        }
    }
    if (mkfifo(fifoFileS2C_.c_str(), fifoMode) != 0 ||
        mkfifo(fifoFileC2S_.c_str(), fifoMode) != 0) {
        if (errno == EEXIST) {
            printf("another %s service is running.\n", perfCmd_.c_str());
        } else {
            RemoveFifoFile();
        }
        strerror_r(errno, errInfo, ERRINFOLEN);
        HLOGE("create fifo file failed. %d:%s", errno, errInfo);
        return false;
    }
    return true;
}

bool PerfPipe::SendFifoAndWaitReply(const std::string &cmd, const std::chrono::milliseconds &timeOut)
{
    // need open for read first, because server maybe send reply before client wait to read
    int fdRead = open(fifoFileS2C_.c_str(), O_RDONLY | O_NONBLOCK);
    if (fdRead == -1) {
        HLOGE("can not open fifo file(%s)", fifoFileS2C_.c_str());
        HIPERF_HILOGE(MODULE_DEFAULT, "can not open fifo file: %{public}s.", fifoFileS2C_.c_str());
        return false;
    }
    int fdWrite = open(fifoFileC2S_.c_str(), O_WRONLY | O_NONBLOCK);
    if (fdWrite == -1) {
        HLOGE("can not open fifo file(%s)", fifoFileC2S_.c_str());
        HIPERF_HILOGE(MODULE_DEFAULT, "can not open fifo file: %{public}s.", fifoFileC2S_.c_str());
        close(fdRead);
        return false;
    }
    size_t size = write(fdWrite, cmd.c_str(), cmd.size());
    if (size != cmd.size()) {
        HLOGE("failed to write fifo file(%s) command(%s)", fifoFileC2S_.c_str(), cmd.c_str());
        HIPERF_HILOGE(MODULE_DEFAULT, "failed to write fifo file(%{public}s) command(%{public}s).",
                      fifoFileC2S_.c_str(), cmd.c_str());
        close(fdWrite);
        close(fdRead);
        return false;
    }
    close(fdWrite);

    bool ret = WaitFifoReply(fdRead, timeOut);
    close(fdRead);
    return ret;
}

bool PerfPipe::WaitFifoReply(int fd, const std::chrono::milliseconds &timeOut)
{
    std::string reply;
    WaitFifoReply(fd, timeOut, reply);
    return reply == HiperfClient::ReplyOK;
}

void PerfPipe::WaitFifoReply(int fd, const std::chrono::milliseconds &timeOut, std::string& reply)
{
    struct pollfd pollFd {
        fd, POLLIN, 0
    };
    int polled = poll(&pollFd, 1, timeOut.count());
    reply.clear();
    if (polled > 0) {
        bool exitLoop = false;
        while (!exitLoop) {
            char c;
            ssize_t result = TEMP_FAILURE_RETRY(read(fd, &c, 1));
            if (result <= 0) {
                HLOGE("read from fifo file(%s) failed", fifoFileS2C_.c_str());
                HIPERF_HILOGE(MODULE_DEFAULT, "read from fifo file(%{public}s) failed", fifoFileS2C_.c_str());
                exitLoop = true;
            }
            reply.push_back(c);
            if (c == '\n') {
                exitLoop = true;
            }
        }
    } else if (polled == 0) {
        HLOGD("wait fifo file(%s) timeout", fifoFileS2C_.c_str());
        HIPERF_HILOGD(MODULE_DEFAULT, "wait fifo file(%{public}s) timeout", fifoFileS2C_.c_str());
    } else {
        HLOGD("wait fifo file(%s) failed", fifoFileS2C_.c_str());
        HIPERF_HILOGD(MODULE_DEFAULT, "wait fifo file(%{public}s) failed", fifoFileS2C_.c_str());
    }
}

void PerfPipe::SetOutPutEnd(bool outputEnd)
{
    outputEnd_ = outputEnd;
}

void PerfPipe::ProcessStopCommand(bool ret)
{
    if (ret) {
        // wait sampling process exit really
        static constexpr uint64_t waitCheckSleepMs = 200;
        std::this_thread::sleep_for(milliseconds(waitCheckSleepMs));
        while (SendFifoAndWaitReply(HiperfClient::ReplyCheck, CONTROL_WAITREPY_TIMEOUT_CHECK)) {
            std::this_thread::sleep_for(milliseconds(waitCheckSleepMs));
        }
        HLOGI("wait reply check end.");
    }

    RemoveFifoFile();
}

void PerfPipe::ProcessOutputCommand(bool ret)
{
    if (!ret) {
        HLOGI("send fifo and wait repoy fail");
        HIPERF_HILOGI(MODULE_DEFAULT, "send fifo and wait repoy fail");
        return;
    }

    std::this_thread::sleep_for(milliseconds(CHECK_WAIT_TIME_MS));
    uint32_t outputFailCount = 0;
    while (!outputEnd_) {
        ret = SendFifoAndWaitReply(HiperfClient::ReplyOutputCheck, CONTROL_WAITREPY_TIMEOUT_CHECK);
        if (outputFailCount++ > MAX_CLIENT_OUTPUT_WAIT_COUNT || ret) {
            break;
        }
        std::this_thread::sleep_for(milliseconds(CHECK_WAIT_TIME_MS));
    }
}

bool PerfPipe::ProcessControlCmd()
{
    bool ret = false;
    if (controlCmd_ == CONTROL_CMD_START) {
        ret = SendFifoAndWaitReply(HiperfClient::ReplyStart, CONTROL_WAITREPY_TIMEOUT);
    } else if (controlCmd_ == CONTROL_CMD_RESUME) {
        ret = SendFifoAndWaitReply(HiperfClient::ReplyResume, CONTROL_WAITREPY_TIMEOUT);
    } else if (controlCmd_ == CONTROL_CMD_PAUSE) {
        ret = SendFifoAndWaitReply(HiperfClient::ReplyPause, CONTROL_WAITREPY_TIMEOUT);
    } else if (controlCmd_ == CONTROL_CMD_STOP) {
        ret = SendFifoAndWaitReply(HiperfClient::ReplyStop, CONTROL_WAITREPY_TIMEOUT);
        if (!ret) {
            ret = SendFifoAndWaitReply(HiperfClient::ReplyStop, CONTROL_WAITREPY_TIMEOUT);
        }
        ProcessStopCommand(ret);
    } else if (controlCmd_ == CONTROL_CMD_OUTPUT) {
        ret = SendFifoAndWaitReply(HiperfClient::ReplyOutput, CONTROL_WAITREPY_TIMEOUT);
        ProcessOutputCommand(ret);
    }
    if (ret) {
        printf("%s %s success.\n", controlCmd_.c_str(), perfCmd_.c_str());
        HIPERF_HILOGI(MODULE_DEFAULT, "%{public}s %{public}s success.", controlCmd_.c_str(), perfCmd_.c_str());
    } else {
        printf("%s %s failed.\n", controlCmd_.c_str(), perfCmd_.c_str());
        HIPERF_HILOGI(MODULE_DEFAULT, "%{public}s %{public}s failed.", controlCmd_.c_str(), perfCmd_.c_str());
    }
    return ret;
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
