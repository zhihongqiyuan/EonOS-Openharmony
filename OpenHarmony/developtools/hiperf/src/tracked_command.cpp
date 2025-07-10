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
#include "tracked_command.h"
#include <cerrno>
#include <csignal>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "debug_logger.h"
#include "ipc_utilities.h"

namespace OHOS {
namespace Developtools {
namespace HiPerf {
static constexpr uint64_t MAX_LOOP_COUNT = 10000;

std::unique_ptr<TrackedCommand> TrackedCommand::CreateInstance(const std::vector<std::string> &args)
{
    std::unique_ptr<TrackedCommand> command {new (std::nothrow) TrackedCommand(args)};
    if (!command) {
        return nullptr;
    }
    if (!command->CreateChildProcess()) {
        return nullptr;
    }
    return command;
}

TrackedCommand::TrackedCommand(const std::vector<std::string> &args) : command_ {args} {}

TrackedCommand::~TrackedCommand()
{
    MakeInvalid();
}

void TrackedCommand::Stop()
{
    MakeInvalid();
}

bool TrackedCommand::InitSignalPipes(int &startFd, int &ackFd)
{
    int startSignalPipe[2] {-1, -1};
    if (pipe2(startSignalPipe, O_CLOEXEC) != 0) {
        HLOGE("pipe2() failed in TrackedCommand::InitSignalPipes()");
        return false;
    }
    startFd = startSignalPipe[0];
    startFd_ = startSignalPipe[1];

    int ackSignalPipe[2] {-1, -1};
    if (pipe2(ackSignalPipe, O_CLOEXEC) != 0) {
        HLOGE("pipe2() failed in TrackedCommand::InitSignalPipes()");
        close(startFd);
        close(startFd_);
        startFd = -1;
        startFd_ = -1;
        return false;
    }
    ackFd = ackSignalPipe[1];
    ackFd_ = ackSignalPipe[0];
    return true;
}

bool TrackedCommand::CreateChildProcess()
{
    int startFd {-1};
    int ackFd {-1};
    if (!InitSignalPipes(startFd, ackFd)) {
        return false;
    }
    CheckIpcBeforeFork();
    pid_t pid = fork();
    if (pid == -1) {
        HLOGE("fork() failed in TrackedCommand::CreateChildProcess()");
        MakeInvalid();
        return false;
    } else if (pid == 0) {
        close(startFd_);
        close(ackFd_);
        ExecuteCommand(startFd, ackFd);
        _exit(0);
    } else {
        close(startFd);
        close(ackFd);
        childPid_ = pid;
        state_ = State::COMMAND_WAITING;
        return true;
    }
}

bool TrackedCommand::StartCommand()
{
    // send start signal to start execution of command
    ssize_t nbyte {0};
    char startSignal {1};
    uint64_t loopCount = 0;
    while (true) {
        nbyte = write(startFd_, &startSignal, 1);
        if (nbyte == -1) {
            if (loopCount++ > MAX_LOOP_COUNT) {
                HLOGE("read failed.");
                break;
            }
            continue;
        }
        break;
    }
    HLOG_ASSERT(nbyte == 1);
    // check execution state of command
    // read acknowledgement signal
    char ackSignal {0};
    loopCount = 0;
    while (true) {
        nbyte = read(ackFd_, &ackSignal, 1);
        if (nbyte == -1 && (errno == EINTR || errno == EIO)) {
            if (loopCount++ > MAX_LOOP_COUNT) {
                HLOGE("read failed.");
                break;
            }
            continue;
        }
        HLOGE("*** nbyte: %zd, ackSignal: %d ***\n", nbyte, ackSignal);
        break;
    }
    if (nbyte == 0) {
        state_ = State::COMMAND_STARTED;
        return true;
    }
    HLOG_ASSERT(nbyte == 1);
    state_ = State::COMMAND_FAILURE;
    return false;
}

void TrackedCommand::ExecuteCommand(const int &startFd, const int &ackFd)
{
    HLOG_ASSERT(startFd != -1);
    HLOG_ASSERT(ackFd != -1);
    prctl(PR_SET_PDEATHSIG, SIGKILL, 0, 0, 0);
    // waiting start signal
    char startSignal {0};
    ssize_t nbyte {0};
    uint64_t loopCount = 0;
    while (true) {
        nbyte = read(startFd, &startSignal, 1);
        if (nbyte == -1) {
            if (loopCount++ > MAX_LOOP_COUNT) {
                HLOGE("read failed.");
                break;
            }
            continue;
        }
        break;
    }
    HLOG_ASSERT(nbyte == 1);
    // execute command
    char *argv[command_.size() + 1];
    for (size_t index = 0; index < command_.size(); ++index) {
        argv[index] = const_cast<char *>(command_[index].c_str());
    }
    argv[command_.size()] = nullptr;
    // On sucees, startFd and ackFd will be closed hence parent process reads EPIPE;
    if (IsPath(argv[0])) {
        execv(argv[0], argv);
    } else {
        execvp(argv[0], argv);
    }
    // execv() or execvp() failed, send failure signal
    char ackSignal {1};
    loopCount = 0;
    while (true) {
        nbyte = write(ackFd, &ackSignal, 1);
        if (nbyte == -1) {
            if (loopCount++ > MAX_LOOP_COUNT) {
                HLOGE("read failed.");
                break;
            }
            continue;
        }
        break;
    }
    HLOG_ASSERT(nbyte == 1);
    HLOGE("child process failed to execute command");
}

bool TrackedCommand::WaitCommand(int &wstatus)
{
    if (childPid_ != -1) {
        HLOG_ASSERT(state_ != State::COMMAND_STOPPED);
        pid_t pid = waitpid(childPid_, &wstatus, WNOHANG);
        if (pid == 0) {
            return false;
        } else { // pid == childPid_ || pid == -1
            childPid_ = -1;
            state_ = State::COMMAND_STOPPED;
            return true;
        }
    }
    return true;
}

void TrackedCommand::MakeInvalid()
{
    if (childPid_ != -1) {
        HLOG_ASSERT(state_ != State::COMMAND_STOPPED);
        int wstatus;
        pid_t pid = waitpid(childPid_, &wstatus, WNOHANG);
        if (pid != childPid_) {
            kill(childPid_, SIGKILL);
        }
        childPid_ = -1;
        state_ = State::COMMAND_STOPPED;
    }
    if (startFd_ != -1) {
        close(startFd_);
        startFd_ = -1;
    }
    if (ackFd_ != -1) {
        close(ackFd_);
        ackFd_ = -1;
    }
}
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
