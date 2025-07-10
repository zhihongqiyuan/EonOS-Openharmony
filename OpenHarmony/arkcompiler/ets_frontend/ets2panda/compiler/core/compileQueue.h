/*
 * Copyright (c) 2021 - 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ES2PANDA_COMPILER_CORE_COMPILE_QUEUE_H
#define ES2PANDA_COMPILER_CORE_COMPILE_QUEUE_H

#include "macros.h"
#include "os/thread.h"
#include "es2panda.h"
#include "compiler/core/compileJob.h"

#include <condition_variable>
#include <functional>
#include <mutex>

namespace ark::es2panda::varbinder {
class FunctionScope;
}  // namespace ark::es2panda::varbinder

namespace ark::es2panda::public_lib {
struct Context;
}  // namespace ark::es2panda::public_lib

namespace ark::es2panda::compiler {

class CompileQueue {
public:
    using JobsFinishedCb = std::function<void(CompileJob *)>;

    explicit CompileQueue(size_t threadCount);
    NO_COPY_SEMANTIC(CompileQueue);
    NO_MOVE_SEMANTIC(CompileQueue);
    ~CompileQueue();

    void Schedule(public_lib::Context *context);
    void Consume();
    void Wait(const JobsFinishedCb &onFinishedCb);

private:
    static void Worker(CompileQueue *queue);

    std::vector<os::thread::NativeHandleType> threads_;
    std::vector<Error> errors_;
    std::mutex m_;
    std::condition_variable jobsAvailable_;
    std::condition_variable jobsFinished_;
    CompileJob *jobs_ {};
    size_t jobsCount_ {0};
    size_t totalJobsCount_ {0};
    size_t activeWorkers_ {0};
    bool terminate_ {};
};
}  // namespace ark::es2panda::compiler

#endif
