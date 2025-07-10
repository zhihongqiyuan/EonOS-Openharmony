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

#include "compileQueue.h"

#include "varbinder/varbinder.h"
#include "varbinder/scope.h"
#include "compiler/core/emitter.h"
#include "compiler/core/function.h"
#include "compiler/core/pandagen.h"
#include "public/public.h"

namespace ark::es2panda::compiler {
CompileQueue::CompileQueue(size_t threadCount)
{
    threads_.reserve(threadCount);

    for (size_t i = 0; i < threadCount; i++) {
        threads_.push_back(os::thread::ThreadStart(Worker, this));
    }
}

CompileQueue::~CompileQueue()
{
    void *retval = nullptr;

    std::unique_lock<std::mutex> lock(m_);
    terminate_ = true;
    lock.unlock();
    jobsAvailable_.notify_all();

    for (const auto handleId : threads_) {
        os::thread::ThreadJoin(handleId, &retval);
    }
}

void CompileQueue::Schedule(public_lib::Context *context)
{
    ASSERT(jobsCount_ == 0);
    std::unique_lock<std::mutex> lock(m_);
    const auto &functions = context->parserProgram->VarBinder()->Functions();
    jobs_ = new CompileJob[functions.size()]();

    for (auto *function : functions) {
        jobs_[jobsCount_++].SetContext(context, function);  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    totalJobsCount_ = jobsCount_;

    lock.unlock();
    jobsAvailable_.notify_all();
}

void CompileQueue::Worker(CompileQueue *queue)
{
    while (true) {
        std::unique_lock<std::mutex> lock(queue->m_);
        queue->jobsAvailable_.wait(lock, [queue]() { return queue->terminate_ || queue->jobsCount_ != 0; });

        if (queue->terminate_) {
            return;
        }

        lock.unlock();

        queue->Consume();
        queue->jobsFinished_.notify_one();
    }
}

void CompileQueue::Consume()
{
    std::unique_lock<std::mutex> lock(m_);
    activeWorkers_++;

    while (jobsCount_ > 0) {
        --jobsCount_;
        auto &job = jobs_[jobsCount_];  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

        lock.unlock();

        try {
            job.Run();
        } catch (const Error &e) {
            lock.lock();
            errors_.push_back(e);
            lock.unlock();
        }

        lock.lock();
    }

    activeWorkers_--;
}

void CompileQueue::Wait(const JobsFinishedCb &onFinishedCb)
{
    std::unique_lock<std::mutex> lock(m_);
    jobsFinished_.wait(lock, [this]() { return activeWorkers_ == 0 && jobsCount_ == 0; });

    if (!errors_.empty()) {
        delete[] jobs_;
        // NOLINTNEXTLINE
        throw errors_.front();
    }

    for (uint32_t i = 0; i < totalJobsCount_; i++) {
        onFinishedCb(jobs_ + i);  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    delete[] jobs_;
}
}  // namespace ark::es2panda::compiler
