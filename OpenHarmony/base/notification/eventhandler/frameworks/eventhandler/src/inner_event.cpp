/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include "inner_event.h"

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <vector>

#include "event_handler_utils.h"
#include "event_logger.h"
#include "singleton.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
static constexpr int DATETIME_STRING_LENGTH = 80;
static constexpr int MAX_MS_LENGTH = 3;
static constexpr int MS_PER_SECOND = 1000;
DEFINE_EH_HILOG_LABEL("InnerEvent");

class WaiterImp final : public InnerEvent::Waiter {
public:
    WaiterImp(){};
    ~WaiterImp() override{};
    DISALLOW_COPY_AND_MOVE(WaiterImp);

    void Wait() final
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!finished_) {
            ++waitingCount_;
            condition_.wait(lock);
            --waitingCount_;
        }
    }

    void Notify() final
    {
        std::lock_guard<std::mutex> lock(mutex_);
        finished_ = true;
        if (waitingCount_ > 0) {
            condition_.notify_all();
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable condition_;
    uint32_t waitingCount_ {0};
    bool finished_ {false};
};
}  // unnamed namespace

// Implementation for event pool.
class InnerEventPool : public DelayedRefSingleton<InnerEventPool> {
    DECLARE_DELAYED_REF_SINGLETON(InnerEventPool);

public:
    DISALLOW_COPY_AND_MOVE(InnerEventPool);

    InnerEvent::Pointer Get()
    {
        // Allocate new memory, while pool is empty.
        return InnerEvent::Pointer(new (std::nothrow) InnerEvent, Drop);
    }

private:
    static void Drop(InnerEvent *event)
    {
        if (event == nullptr) {
            return;
        }

        // Clear content of the event
        event->ClearEvent();
        if (event != nullptr) {
            delete event;
        }
    }
};

InnerEventPool::InnerEventPool()
{
    HILOGD("InnerEventPool enter");
}

InnerEventPool::~InnerEventPool()
{
    HILOGD("~InnerEventPool enter");
}

InnerEvent::Pointer InnerEvent::Get()
{
    auto event = InnerEventPool::GetInstance().Get();
    return event;
}

InnerEvent::Pointer InnerEvent::Get(uint32_t innerEventId, int64_t param, const Caller &caller)
{
    auto event = InnerEventPool::GetInstance().Get();
    if (event != nullptr) {
        event->innerEventId_ = innerEventId;
        event->param_ = param;
        event->caller_ = caller;
        HILOGD("innerEventId is %{public}u, caller is %{public}s", innerEventId, caller.ToString().c_str());
    }
    return event;
}

InnerEvent::Pointer InnerEvent::Get(const EventId &innerEventId, int64_t param, const Caller &caller)
{
    auto event = InnerEventPool::GetInstance().Get();
    if (event != nullptr) {
        event->innerEventId_ = innerEventId;
        event->param_ = param;
        event->caller_ = caller;
        if (innerEventId.index() == TYPE_U32_INDEX) {
            HILOGD("innerEventId is %{public}u, caller is %{public}s",
                std::get<uint32_t>(innerEventId),
                caller.ToString().c_str());
        } else {
            HILOGD("innerEventId is %{public}s, caller is %{public}s",
                std::get<std::string>(innerEventId).c_str(),
                caller.ToString().c_str());
        }
    }
    return event;
}

InnerEvent::Pointer InnerEvent::Get(const Callback &callback, const std::string &name, const Caller &caller)
{
    // Returns nullptr while callback is invalid.
    if (!callback) {
        HILOGW("Failed to create inner event with an invalid callback");
        return InnerEvent::Pointer(nullptr, nullptr);
    }

    auto event = InnerEventPool::GetInstance().Get();
    if (event != nullptr) {
        event->taskCallback_ = callback;
        event->taskName_ = name;
        event->caller_ = caller;
        HILOGD("event taskName is '%{public}s', caller is %{public}s", name.c_str(), caller.ToString().c_str());
    }
    return event;
}

void InnerEvent::ClearEvent()
{
    // Wake up all waiting threads.
    if (waiter_) {
        waiter_->Notify();
        waiter_.reset();
    }

    if (HasTask()) {
        // Clear members for task
        taskName_.clear();
        caller_.ClearCaller();
    } else {
        // Clear members for event
        if (smartPtrDtor_) {
            smartPtrDtor_(smartPtr_);
            smartPtrDtor_ = nullptr;
            smartPtr_ = nullptr;
            smartPtrTypeId_ = 0;
        }
    }

    if (hiTraceId_) {
        hiTraceId_.reset();
    }

    // Clear owner
    owner_.reset();
}

void InnerEvent::WarnSmartPtrCastMismatch()
{
    HILOGD("Type of the shared_ptr, weak_ptr or unique_ptr mismatched");
}

const std::shared_ptr<InnerEvent::Waiter> &InnerEvent::CreateWaiter()
{
    waiter_ = std::make_shared<WaiterImp>();
    return waiter_;
}

bool InnerEvent::HasWaiter() const
{
    return (waiter_ != nullptr);
}

const std::shared_ptr<HiTraceId> InnerEvent::GetOrCreateTraceId()
{
    if (hiTraceId_) {
        return hiTraceId_;
    }

    auto traceId = HiTraceChain::GetId();
    if (!traceId.IsValid()) {
        return nullptr;
    }

    hiTraceId_ = std::make_shared<HiTraceId>(HiTraceChain::CreateSpan());
    return hiTraceId_;
}

const std::shared_ptr<HiTraceId> InnerEvent::GetTraceId()
{
    return hiTraceId_;
}

std::string InnerEvent::DumpTimeToString(const std::chrono::system_clock::time_point &time)
{
    auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(time);
    auto tt = std::chrono::system_clock::to_time_t(time);
    auto ms = tp.time_since_epoch().count() % MS_PER_SECOND;
    auto msString = std::to_string(ms);
    if (msString.length() < MAX_MS_LENGTH) {
        msString = std::string(MAX_MS_LENGTH - msString.length(), '0') + msString;
    }
    struct tm curTime = {0};
    localtime_r(&tt, &curTime);
    char sysTime[DATETIME_STRING_LENGTH];
    std::strftime(sysTime, sizeof(char) * DATETIME_STRING_LENGTH, "%Y-%m-%d %I:%M:%S.", &curTime);
    return std::string(sysTime) + msString;
}

std::string InnerEvent::DumpTimeToString(const TimePoint &time)
{
    auto tp = std::chrono::system_clock::now() +
        std::chrono::duration_cast<std::chrono::milliseconds>(time - std::chrono::steady_clock::now());
    return DumpTimeToString(tp);
}

std::string InnerEvent::Dump()
{
    std::string content;

    content.append("Event { ");
    if (!owner_.expired()) {
        content.append("send thread = " + std::to_string(senderKernelThreadId_));
        content.append(", send time = " + DumpTimeToString(sendTime_));
        content.append(", handle time = " + DumpTimeToString(handleTime_));
        if (HasTask()) {
            content.append(", task name = " + taskName_);
        } else {
            if (innerEventId_.index() == TYPE_U32_INDEX) {
                content.append(", id = " + std::to_string(std::get<uint32_t>(innerEventId_)));
            } else {
                content.append(", id = " + std::get<std::string>(innerEventId_));
            }
        }
        if (param_ != 0) {
            content.append(", param = " + std::to_string(param_));
        }
        content.append(", caller = " + caller_.ToString());
    } else {
        content.append("No handler");
    }
    content.append(" }" + LINE_SEPARATOR);

    return content;
}

std::string InnerEvent::TraceInfo()
{
    std::string content;

    content.append("Et:");
    if (!owner_.expired()) {
        content.append(std::to_string(senderKernelThreadId_));
        content.append("," + std::to_string(sendTime_.time_since_epoch().count()));
        content.append("," + std::to_string(handleTime_.time_since_epoch().count()));
        if (HasTask()) {
            content.append("," + taskName_);
        } else {
            if (innerEventId_.index() == TYPE_U32_INDEX) {
                content.append("," + std::to_string(std::get<uint32_t>(innerEventId_)));
            } else {
                content.append("," + std::get<std::string>(innerEventId_));
            }
        }
        content.append("," + std::to_string(priority));
        content.append("," + caller_.ToString());
    } else {
        content.append("NA");
    }

    return content;
}

void InnerEvent::SetEventUniqueId()
{
    auto nowTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
    eventId = std::to_string(nowTime);
}

}  // namespace AppExecFwk
}  // namespace OHOS
