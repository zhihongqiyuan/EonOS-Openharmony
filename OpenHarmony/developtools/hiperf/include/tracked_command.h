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
#ifndef HIPERF_TRACKED_COMMAND_H_
#define HIPERF_TRACKED_COMMAND_H_

#include <memory>
#include <string>
#include <utilities.h>
namespace OHOS {
namespace Developtools {
namespace HiPerf {
class TrackedCommand : public Noncopyable {
public:
    enum class State {
        COMMAND_WAITING, // child process blocked to execute command
        COMMAND_STARTED, // child process executing command
        COMMAND_FAILURE, // command failed to start
        COMMAND_STOPPED  // no child process or command execution
    };

    static std::unique_ptr<TrackedCommand> CreateInstance(const std::vector<std::string> &args);

    ~TrackedCommand();

    bool CreateChildProcess();
    bool StartCommand();
    bool WaitCommand(int &wstatus);
    void Stop();

    inline std::string GetCommandName()
    {
        if (!command_.empty()) {
            return command_[0];
        }
        return EMPTY_STRING;
    }

    inline State GetState()
    {
        return state_;
    }

    inline pid_t GetChildPid()
    {
        return childPid_;
    }

private:
    explicit TrackedCommand(const std::vector<std::string> &args);

    bool InitSignalPipes(int &startFd, int &ackFd);
    void ExecuteCommand(const int &startFd, const int &ackFd);
    void MakeInvalid();

    std::vector<std::string> command_ {};
    int startFd_ {-1};
    int ackFd_ {-1};
    pid_t childPid_ {-1};
    State state_ {State::COMMAND_STOPPED};
};
} // namespace HiPerf
} // namespace Developtools
} // namespace OHOS
#endif // HIPERF_TRACKED_COMMAND_H_
