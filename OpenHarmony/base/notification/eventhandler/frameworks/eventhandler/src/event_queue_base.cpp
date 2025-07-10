
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

#include "event_queue_base.h"

#include <algorithm>
#include <chrono>
#include <iterator>
#include <mutex>

#include "deamon_io_waiter.h"
#include "epoll_io_waiter.h"
#include "event_handler.h"
#include "event_handler_utils.h"
#include "event_logger.h"
#include "inner_event.h"
#include "none_io_waiter.h"
#include "event_hitrace_meter_adapter.h"

namespace OHOS {
namespace AppExecFwk {
namespace {

DEFINE_EH_HILOG_LABEL("EventQueueBase");
constexpr uint32_t MAX_DUMP_SIZE = 500;
constexpr int64_t GC_TIME_OUT = 300;
constexpr std::string_view STAGE_BEFORE_WAITING = "BEFORE_WAITING";
constexpr std::string_view STAGE_AFTER_WAITING = "AFTER_WAITING";
constexpr std::string_view STAGE_VIP_EXISTED = "STAGE_VIP_EXISTED";
constexpr std::string_view STAGE_VIP_NONE = "STAGE_VIP_NONE";
// Help to insert events into the event queue sorted by handle time.
void InsertEventsLocked(std::list<InnerEvent::Pointer> &events, InnerEvent::Pointer &event,
    EventInsertType insertType)
{
    if (insertType == EventInsertType::AT_FRONT) {
        if (!events.empty()) {
            // Ensure that events queue is in ordered
            auto headEvent = events.begin();
            if ((*headEvent)->GetHandleTime() < event->GetHandleTime()) {
                event->SetHandleTime((*headEvent)->GetHandleTime());
            }
        }
        events.emplace_front(std::move(event));
        return;
    }

    auto it = events.end();
    auto eventTime = event->GetHandleTime();
    while (it != events.begin()) {
        auto prevIt = std::prev(it);
        if ((*prevIt) == nullptr) {
            continue;
        }
        if ((*prevIt)->GetHandleTime() <= eventTime) {
            break;
        }
        it = prevIt;
    }
    events.insert(it, std::move(event));
}

// Help to check whether there is a valid event in list and update wake up time.
inline bool CheckEventInListLocked(const std::list<InnerEvent::Pointer> &events, const InnerEvent::TimePoint &now,
    InnerEvent::TimePoint &nextWakeUpTime, bool isBarrierMode)
{
    if (events.empty()) return false;

    auto filter = [&now, &nextWakeUpTime, isBarrierMode](const InnerEvent::Pointer &p) {
        const auto &handleTime = p->GetHandleTime();
        if (handleTime < nextWakeUpTime) {
            nextWakeUpTime = handleTime;
            return handleTime <= now && (!isBarrierMode || p->IsBarrierTask());
        }
        return false;
    };
    return std::find_if(events.begin(), events.end(), filter) != events.end();
}

inline InnerEvent::Pointer PopFrontEventFromListLocked(std::list<InnerEvent::Pointer> &events)
{
    InnerEvent::Pointer event = std::move(events.front());
    events.pop_front();
    return event;
}

inline InnerEvent::Pointer PopFrontBarrierEventFromListLocked(std::list<InnerEvent::Pointer> &events)
{
    auto filter = [](const InnerEvent::Pointer &p) {
        return p->IsBarrierTask();
    };
    std::list<InnerEvent::Pointer>::iterator iter = std::find_if(events.begin(), events.end(), filter);
    if (iter != events.end()) {
        std::list<InnerEvent::Pointer> tempList;
        tempList.splice(tempList.begin(), events, iter);
        return PopFrontEventFromListLocked(tempList);
    }
    return InnerEvent::Pointer(nullptr, nullptr);
}

inline InnerEvent::Pointer PopFrontBarrierEventFromListWithTimeLocked(std::list<InnerEvent::Pointer> &events,
    const InnerEvent::TimePoint &sendTime, const InnerEvent::TimePoint &handleTime)
{
    auto filter = [&sendTime, &handleTime](const InnerEvent::Pointer &p) {
        return p->IsBarrierTask() && (p->GetSendTime() <= sendTime) && (p->GetHandleTime() <= handleTime);
    };
    std::list<InnerEvent::Pointer>::iterator iter = std::find_if(events.begin(), events.end(), filter);
    if (iter != events.end()) {
        std::list<InnerEvent::Pointer> tempList;
        tempList.splice(tempList.begin(), events, iter);
        return PopFrontEventFromListLocked(tempList);
    }
    return InnerEvent::Pointer(nullptr, nullptr);
}
}  // unnamed namespace

EventQueueBase::EventQueueBase() : EventQueue(), historyEvents_(std::vector<HistoryEvent>(HISTORY_EVENT_NUM_POWER))
{
    HILOGD("enter");
}

EventQueueBase::EventQueueBase(const std::shared_ptr<IoWaiter> &ioWaiter)
    : EventQueue(ioWaiter), historyEvents_(std::vector<HistoryEvent>(HISTORY_EVENT_NUM_POWER))
{
    HILOGD("enter");
}

EventQueueBase::~EventQueueBase()
{
    std::lock_guard<std::mutex> lock(queueLock_);
    usable_.store(false);
    ioWaiter_ = nullptr;
    ClearObserver();
    EH_LOGD_LIMIT("EventQueueBase is unavailable hence");
}

static inline void MarkBarrierTaskIfNeed(InnerEvent::Pointer &event)
{
    if (EventRunner::IsAppMainThread() && (event->GetHandleTime() == event->GetSendTime()) &&
        (EventRunner::GetMainEventRunner() == event->GetOwner()->GetEventRunner())) {
        event->MarkBarrierTask();
    }
}

bool EventQueueBase::Insert(InnerEvent::Pointer &event, Priority priority, EventInsertType insertType)
{
    if (!event) {
        HILOGE("Could not insert an invalid event");
        return false;
    }
    HILOGD("Insert task: %{public}s %{public}d.", (event->GetEventUniqueId()).c_str(), insertType);
    MarkBarrierTaskIfNeed(event);
    std::lock_guard<std::mutex> lock(queueLock_);
    if (!usable_.load()) {
        HILOGW("EventQueue is unavailable.");
        return false;
    }
    bool needNotify = false;
    event->SetEventPriority(static_cast<int32_t>(priority));
    switch (priority) {
        case Priority::VIP:
        case Priority::IMMEDIATE:
        case Priority::HIGH:
        case Priority::LOW: {
            needNotify = (event->GetHandleTime() < wakeUpTime_) || (wakeUpTime_ < InnerEvent::Clock::now());
            if (event->IsVsyncTask()) {
                needNotify = true;
                DispatchVsyncTaskNotify();
            }
            InsertEventsLocked(subEventQueues_[static_cast<uint32_t>(priority)].queue, event, insertType);
            subEventQueues_[static_cast<uint32_t>(priority)].frontEventHandleTime =
                static_cast<uint64_t>((*subEventQueues_[static_cast<uint32_t>(priority)].queue.begin())
                    ->GetHandleTime().time_since_epoch().count());
            break;
        }
        case Priority::IDLE: {
            // Never wake up thread if insert an idle event.
            InsertEventsLocked(idleEvents_, event, insertType);
            break;
        }
        default:
            break;
    }

    if (needNotify) {
        ioWaiter_->NotifyOne();
    }
#ifdef NOTIFICATIONG_SMART_GC
    if (priority == Priority::VIP && !isExistVipTask_) {
        isExistVipTask_ = true;
        InnerEvent::TimePoint time = InnerEvent::Clock::now();
        TryExecuteObserverCallback(time, EventRunnerStage::STAGE_VIP_EXISTED);
    }
#endif
    return true;
}

void EventQueueBase::RemoveOrphan()
{
    HILOGD("enter");
    // Remove all events which lost its owner.
    auto filter = [this](const InnerEvent::Pointer &p) {
        bool ret = p->GetWeakOwner().expired();
        if (ret && p->IsVsyncTask()) {
            HandleVsyncTaskNotify();
            SetBarrierMode(false);
        }
        return ret;
    };

    RemoveOrphan(filter);

    std::lock_guard<std::mutex> lock(queueLock_);
    if (!usable_.load()) {
        HILOGW("RemoveOrphan EventQueueBase is unavailable.");
        return;
    }
    RemoveInvalidFileDescriptor();
}


void EventQueueBase::RemoveAll()
{
    HILOGD("enter");
    std::lock_guard<std::mutex> lock(queueLock_);
    if (!usable_.load()) {
        HILOGW("RemoveAll EventQueueBase is unavailable.");
        return;
    }
    for (uint32_t i = 0; i < SUB_EVENT_QUEUE_NUM; ++i) {
        subEventQueues_[i].queue.clear();
        subEventQueues_[i].frontEventHandleTime = UINT64_MAX;
    }
    idleEvents_.clear();
}

void EventQueueBase::Remove(const std::shared_ptr<EventHandler> &owner)
{
    HILOGD("Remove owner enter");
    if (!owner) {
        HILOGE("Invalid owner");
        return;
    }

    auto filter = [&owner](const InnerEvent::Pointer &p) { return (p->GetOwner() == owner); };

    Remove(filter);
}

void EventQueueBase::Remove(const std::shared_ptr<EventHandler> &owner, uint32_t innerEventId)
{
    HILOGD("Remove innerEventId enter");
    if (!owner) {
        HILOGE("Invalid owner");
        return;
    }
    auto filter = [&owner, innerEventId](const InnerEvent::Pointer &p) {
        return (!p->HasTask()) && (p->GetOwner() == owner) && (p->GetInnerEventId() == innerEventId);
    };

    Remove(filter);
}

void EventQueueBase::Remove(const std::shared_ptr<EventHandler> &owner, uint32_t innerEventId, int64_t param)
{
    HILOGD("Remove param enter");
    if (!owner) {
        HILOGE("Invalid owner");
        return;
    }

    auto filter = [&owner, innerEventId, param](const InnerEvent::Pointer &p) {
        return (!p->HasTask()) && (p->GetOwner() == owner) && (p->GetInnerEventId() == innerEventId) &&
               (p->GetParam() == param);
    };

    Remove(filter);
}

bool EventQueueBase::Remove(const std::shared_ptr<EventHandler> &owner, const std::string &name)
{
    HILOGD("Remove name enter");
    if ((!owner) || (name.empty())) {
        HILOGE("Invalid owner or task name");
        return false;
    }

    bool removed = false;
    auto filter = [&owner, &name, &removed](const InnerEvent::Pointer &p) {
        if (p == nullptr) {
            return false;
        }
        bool ret = (p->HasTask()) && (p->GetOwner() == owner) && (p->GetTaskName() == name);
        if (!removed) {
            removed = ret;
        }
        return ret;
    };

    Remove(filter);
    return removed;
}

void EventQueueBase::Remove(const RemoveFilter &filter) __attribute__((no_sanitize("cfi")))
{
    HILOGD("Remove filter enter");
    std::lock_guard<std::mutex> lock(queueLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueBase is unavailable.");
        return;
    }
#ifdef NOTIFICATIONG_SMART_GC
    bool result = hasVipTask();
#endif
    for (uint32_t i = 0; i < SUB_EVENT_QUEUE_NUM; ++i) {
        subEventQueues_[i].queue.remove_if(filter);
        subEventQueues_[i].frontEventHandleTime = (subEventQueues_[i].queue.size() ?
            static_cast<uint64_t>((*subEventQueues_[i].queue.begin())
            ->GetHandleTime().time_since_epoch().count()) : UINT64_MAX);
    }
    idleEvents_.remove_if(filter);
#ifdef NOTIFICATIONG_SMART_GC
    if (result) {
        NotifyObserverVipDoneBase();
    }
#endif
}

void EventQueueBase::RemoveOrphan(const RemoveFilter &filter)
{
    std::list<InnerEvent::Pointer> releaseIdleEvents;
    std::array<SubEventQueue, SUB_EVENT_QUEUE_NUM> releaseEventsQueue;
    {
        std::lock_guard<std::mutex> lock(queueLock_);
        if (!usable_.load()) {
            HILOGW("EventQueueBase is unavailable.");
            return;
        }
#ifdef NOTIFICATIONG_SMART_GC
        bool result = hasVipTask();
#endif
        for (uint32_t i = 0; i < SUB_EVENT_QUEUE_NUM; ++i) {
            auto it = std::stable_partition(subEventQueues_[i].queue.begin(), subEventQueues_[i].queue.end(), filter);
            std::move(subEventQueues_[i].queue.begin(), it, std::back_inserter(releaseEventsQueue[i].queue));
            subEventQueues_[i].queue.erase(subEventQueues_[i].queue.begin(), it);
            subEventQueues_[i].frontEventHandleTime = (subEventQueues_[i].queue.size() ?
            static_cast<uint64_t>((*subEventQueues_[i].queue.begin())
            ->GetHandleTime().time_since_epoch().count()) : UINT64_MAX);
        }
        auto idleEventIt = std::stable_partition(idleEvents_.begin(), idleEvents_.end(), filter);
        std::move(idleEvents_.begin(), idleEventIt, std::back_inserter(releaseIdleEvents));
        idleEvents_.erase(idleEvents_.begin(), idleEventIt);
#ifdef NOTIFICATIONG_SMART_GC
        if (result) {
            NotifyObserverVipDoneBase();
        }
#endif
    }
}

bool EventQueueBase::HasInnerEvent(const std::shared_ptr<EventHandler> &owner, uint32_t innerEventId)
{
    if (!owner) {
        HILOGE("Invalid owner");
        return false;
    }
    auto filter = [&owner, innerEventId](const InnerEvent::Pointer &p) {
        return (!p->HasTask()) && (p->GetOwner() == owner) && (p->GetInnerEventId() == innerEventId);
    };
    return HasInnerEvent(filter);
}

bool EventQueueBase::HasInnerEvent(const std::shared_ptr<EventHandler> &owner, int64_t param)
{
    if (!owner) {
        HILOGE("Invalid owner");
        return false;
    }
    auto filter = [&owner, param](const InnerEvent::Pointer &p) {
        return (!p->HasTask()) && (p->GetOwner() == owner) && (p->GetParam() == param);
    };
    return HasInnerEvent(filter);
}

bool EventQueueBase::HasInnerEvent(const HasFilter &filter)
{
    std::lock_guard<std::mutex> lock(queueLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueBase is unavailable.");
        return false;
    }
    for (uint32_t i = 0; i < SUB_EVENT_QUEUE_NUM; ++i) {
        std::list<InnerEvent::Pointer>::iterator iter =
            std::find_if(subEventQueues_[i].queue.begin(), subEventQueues_[i].queue.end(), filter);
        if (iter != subEventQueues_[i].queue.end()) {
            return true;
        }
    }
    if (std::find_if(idleEvents_.begin(), idleEvents_.end(), filter) != idleEvents_.end()) {
        return true;
    }
    return false;
}

InnerEvent::Pointer EventQueueBase::PickFirstVsyncEventLocked()
{
    auto &events = subEventQueues_[static_cast<uint32_t>(vsyncPriority_)].queue;
    auto removeFilter = [](const InnerEvent::Pointer &p) {
        return !p->GetTaskName().compare("BarrierEvent");
    };
    std::list<InnerEvent::Pointer>::iterator iter = std::find_if(events.begin(), events.end(), removeFilter);
    if (iter != events.end()) {
        std::list<InnerEvent::Pointer> tempList;
        tempList.splice(tempList.begin(), events, iter);
    }

    auto filter = [](const InnerEvent::Pointer &p) {
        return p->IsVsyncTask();
    };
    iter = std::find_if(events.begin(), events.end(), filter);
    if (iter != events.end()) {
        std::list<InnerEvent::Pointer> tempList;
        tempList.splice(tempList.begin(), events, iter);
        return PopFrontEventFromListLocked(tempList);
    }
    return InnerEvent::Pointer(nullptr, nullptr);
}

InnerEvent::Pointer EventQueueBase::PickEventLocked(const InnerEvent::TimePoint &now,
    InnerEvent::TimePoint &nextWakeUpTime)
{
    bool isBarrierMode = isBarrierMode_;
    uint32_t priorityIndex = SUB_EVENT_QUEUE_NUM;
    for (uint32_t i = 0; i < SUB_EVENT_QUEUE_NUM; ++i) {
        // Check whether any event need to be distributed.
        if (!CheckEventInListLocked(subEventQueues_[i].queue, now, nextWakeUpTime, isBarrierMode)) {
            continue;
        }

        // Check whether any event in higher priority need to be distributed.
        if (priorityIndex < SUB_EVENT_QUEUE_NUM) {
            SubEventQueue &subQueue = subEventQueues_[priorityIndex];
            // Check whether enough events in higher priority queue are handled continuously.
            if (subQueue.handledEventsCount < subQueue.maxHandledEventsCount) {
                subQueue.handledEventsCount++;
                break;
            }
        }

        // Try to pick event from this queue.
        priorityIndex = i;
    }

    if ((priorityIndex >= static_cast<uint32_t>(Priority::HIGH)) &&
        sumOfPendingVsync_.load() && !needEpoll_.load()) {
        auto event = PickFirstVsyncEventLocked();
        if (event) {
            return event;
        }
    }
    if (priorityIndex >= SUB_EVENT_QUEUE_NUM) {
        // If not found any event to distribute, return nullptr.
        return InnerEvent::Pointer(nullptr, nullptr);
    }

    // Reset handled event count for sub event queues in higher priority.
    for (uint32_t i = 0; i < priorityIndex; ++i) {
        subEventQueues_[i].handledEventsCount = 0;
    }
    if (isBarrierMode) {
        return PopFrontBarrierEventFromListLocked(subEventQueues_[priorityIndex].queue);
    }
    return PopFrontEventFromListLocked(subEventQueues_[priorityIndex].queue);
}

InnerEvent::Pointer EventQueueBase::GetExpiredEventLocked(InnerEvent::TimePoint &nextExpiredTime)
{
    auto now = InnerEvent::Clock::now();
    wakeUpTime_ = InnerEvent::TimePoint::max();
    // Find an event which could be distributed right now.
    InnerEvent::Pointer event = PickEventLocked(now, wakeUpTime_);
    if (event) {
        int32_t prio = event->GetEventPriority();
        subEventQueues_[prio].frontEventHandleTime = subEventQueues_[prio].queue.empty() ? UINT64_MAX :
            static_cast<uint64_t>((*subEventQueues_[prio].queue.begin())->GetHandleTime().time_since_epoch().count());
        // Exit idle mode, if found an event to distribute.
        isIdle_ = false;
        currentRunningEvent_ = CurrentRunningEvent(now, event);
        return event;
    }

    // If found nothing, enter idle mode and make a time stamp.
    if (!isIdle_) {
        idleTimeStamp_ = now;
        isIdle_ = true;
    }

    if (!idleEvents_.empty()) {
        if (isBarrierMode_) {
            event = PopFrontBarrierEventFromListWithTimeLocked(idleEvents_, idleTimeStamp_, now);
            if (event) {
                currentRunningEvent_ = CurrentRunningEvent(now, event);
                return event;
            }
        } else {
            const auto &idleEvent = idleEvents_.front();

            // Return the idle event that has been sent before time stamp and reaches its handle time.
            if ((idleEvent->GetSendTime() <= idleTimeStamp_) && (idleEvent->GetHandleTime() <= now)) {
                event = PopFrontEventFromListLocked(idleEvents_);
                currentRunningEvent_ = CurrentRunningEvent(now, event);
                return event;
            }
        }
    }

    // Update wake up time.
    nextExpiredTime = sumOfPendingVsync_.load()? InnerEvent::Clock::now() : wakeUpTime_;
    currentRunningEvent_ = CurrentRunningEvent();
    return InnerEvent::Pointer(nullptr, nullptr);
}

InnerEvent::Pointer EventQueueBase::GetEvent()
{
    std::unique_lock<std::mutex> lock(queueLock_);
    while (!finished_) {
        if (isBarrierMode_ && !needEpoll_.load()) {
            auto event = PickFirstVsyncEventLocked();
            if (event) {
                return event;
            }
        }
        InnerEvent::TimePoint nextWakeUpTime = InnerEvent::TimePoint::max();
        InnerEvent::Pointer event = GetExpiredEventLocked(nextWakeUpTime);
        if (event) {
            return event;
        }
        TryExecuteObserverCallback(nextWakeUpTime, EventRunnerStage::STAGE_BEFORE_WAITING);
        WaitUntilLocked(nextWakeUpTime, lock);
        epollTimePoint_ = InnerEvent::Clock::now();
        needEpoll_.store(false);
        TryExecuteObserverCallback(nextWakeUpTime, EventRunnerStage::STAGE_AFTER_WAITING);
    }

    HILOGD("Break out");
    return InnerEvent::Pointer(nullptr, nullptr);
}

void EventQueueBase::TryExecuteObserverCallback(InnerEvent::TimePoint &nextExpiredTime, EventRunnerStage stage)
{
    uint32_t stageUint = static_cast<uint32_t>(stage);
    if ((stageUint & observer_.stages) != stageUint) {
        HILOGD("The observer does not subscribe to this type of notification");
        return;
    }
    if (observer_.notifyCb == nullptr) {
        HILOGD("notifyCb is nullptr");
        return;
    }
    int64_t consumer = 0;
    StageInfo info;
    ObserverTrace obs;
    obs.source = GetObserverTypeName(observer_.observer);
    switch (stage) {
        case EventRunnerStage::STAGE_BEFORE_WAITING:
            info.sleepTime = NanosecondsToTimeout(TimePointToTimeOut(nextExpiredTime));
            obs.stage = STAGE_BEFORE_WAITING.data();
            consumer = ExecuteObserverCallback(obs, stage, info);
            if (nextExpiredTime < InnerEvent::TimePoint::max()) {
                HILOGD("time consumer: %{public}lld", static_cast<long long>(consumer));
                nextExpiredTime = nextExpiredTime + std::chrono::milliseconds(consumer);
                wakeUpTime_ = nextExpiredTime;
            }
            break;
        case EventRunnerStage::STAGE_AFTER_WAITING:
            obs.stage = STAGE_AFTER_WAITING.data();
            consumer = ExecuteObserverCallback(obs, stage, info);
            break;
        case EventRunnerStage::STAGE_VIP_EXISTED:
            obs.stage = STAGE_VIP_EXISTED.data();
            consumer = ExecuteObserverCallback(obs, stage, info);
            break;
        case EventRunnerStage::STAGE_VIP_NONE:
            obs.stage = STAGE_VIP_NONE.data();
            consumer = ExecuteObserverCallback(obs, stage, info);
            break;
        default:
            HILOGE("this branch is unreachable");
            break;
    }
    if (consumer > GC_TIME_OUT) {
        HILOGI("execute observer callback task consumer: %{public}lld, stage: %{public}u, wakeTime: %{public}s",
            static_cast<long long>(consumer), stageUint, InnerEvent::DumpTimeToString(wakeUpTime_).c_str());
    }
}

int64_t EventQueueBase::ExecuteObserverCallback(ObserverTrace obsTrace, EventRunnerStage stage, StageInfo &info)
{
    auto start = std::chrono::high_resolution_clock::now();
    info.timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(start).time_since_epoch().count();

    StartTraceObserver(obsTrace);
    (observer_.notifyCb)(stage, &info);
    FinishTraceAdapter();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    return duration.count();
}

std::string EventQueueBase::GetObserverTypeName(Observer observerType)
{
    switch (observerType) {
        case Observer::ARKTS_GC:
            return "ARKTS_GC";
        default :
            return "UNKNOWN_TYPE";
    }
}


void EventQueueBase::ClearObserver()
{
    observer_.stages = static_cast<uint32_t>(EventRunnerStage::STAGE_INVAILD);
    observer_.notifyCb = nullptr;
}

InnerEvent::Pointer EventQueueBase::GetExpiredEvent(InnerEvent::TimePoint &nextExpiredTime)
{
    std::unique_lock<std::mutex> lock(queueLock_);
    return GetExpiredEventLocked(nextExpiredTime);
}

void EventQueueBase::DumpCurrentRunningEventId(const InnerEvent::EventId &innerEventId, std::string &content)
{
    if (innerEventId.index() == TYPE_U32_INDEX) {
        content.append(", id = " + std::to_string(std::get<uint32_t>(innerEventId)));
    } else {
        content.append(", id = " + std::get<std::string>(innerEventId));
    }
}

std::string EventQueueBase::DumpCurrentRunning()
{
    std::string content;
    if (currentRunningEvent_.beginTime_ == InnerEvent::TimePoint::max()) {
        content.append("{}");
    } else {
        content.append("start at " + InnerEvent::DumpTimeToString(currentRunningEvent_.beginTime_) + ", ");
        content.append("Event { ");
        if (!currentRunningEvent_.owner_.expired()) {
            content.append("send thread = " + std::to_string(currentRunningEvent_.senderKernelThreadId_));
            content.append(", send time = " + InnerEvent::DumpTimeToString(currentRunningEvent_.sendTime_));
            content.append(", handle time = " + InnerEvent::DumpTimeToString(currentRunningEvent_.handleTime_));
            content.append(", trigger time = " + InnerEvent::DumpTimeToString(currentRunningEvent_.triggerTime_));
            if (currentRunningEvent_.hasTask_) {
                content.append(", task name = " + currentRunningEvent_.taskName_);
            } else {
                DumpCurrentRunningEventId(currentRunningEvent_.innerEventId_, content);
            }
            if (currentRunningEvent_.param_ != 0) {
                content.append(", param = " + std::to_string(currentRunningEvent_.param_));
            }
            content.append(", caller = " + currentRunningEvent_.callerInfo_);
        } else {
            content.append("No handler");
        }
        content.append(" }");
    }

    return content;
}

void EventQueueBase::DumpCurentQueueInfo(Dumper &dumper, uint32_t dumpMaxSize)
{
    std::string priority[] = {"VIP", "Immediate", "High", "Low"};
    uint32_t total = 0;
    for (uint32_t i = 0; i < SUB_EVENT_QUEUE_NUM; ++i) {
        uint32_t n = 0;
        dumper.Dump(dumper.GetTag() + " " + priority[i] + " priority event queue information:" + LINE_SEPARATOR);
        for (auto it = subEventQueues_[i].queue.begin(); it != subEventQueues_[i].queue.end(); ++it) {
            ++n;
            if (total < dumpMaxSize) {
                dumper.Dump(dumper.GetTag() + " No." + std::to_string(n) + " : " + (*it)->Dump());
            }
            ++total;
        }
        dumper.Dump(
            dumper.GetTag() + " Total size of " + priority[i] + " events : " + std::to_string(n) + LINE_SEPARATOR);
    }
    dumper.Dump(dumper.GetTag() + " Idle priority event queue information:" + LINE_SEPARATOR);
    int n = 0;
    for (auto it = idleEvents_.begin(); it != idleEvents_.end(); ++it) {
        ++n;
        if (total < dumpMaxSize) {
            dumper.Dump(dumper.GetTag() + " No." + std::to_string(n) + " : " + (*it)->Dump());
        }
        ++total;
    }
    dumper.Dump(dumper.GetTag() + " Total size of Idle events : " + std::to_string(n) + LINE_SEPARATOR);
    dumper.Dump(dumper.GetTag() + " Total event size : " + std::to_string(total) + LINE_SEPARATOR);
}

void EventQueueBase::Dump(Dumper &dumper)
{
    std::lock_guard<std::mutex> lock(queueLock_);
    HILOGI("EventQueue start dump.");
    if (!usable_.load()) {
        HILOGW("EventQueue is unavailable.");
        return;
    }
    dumper.Dump(dumper.GetTag() + " Current Running: " + DumpCurrentRunning() + LINE_SEPARATOR);
    dumper.Dump(dumper.GetTag() + " History event queue information:" + LINE_SEPARATOR);
    uint32_t dumpMaxSize = MAX_DUMP_SIZE;
    for (uint8_t i = 0; i < HISTORY_EVENT_NUM_POWER; i++) {
        if (historyEvents_[i].senderKernelThreadId == 0) {
            continue;
        }
        --dumpMaxSize;
        dumper.Dump(dumper.GetTag() + " No. " + std::to_string(i) + " : " + HistoryQueueDump(historyEvents_[i]));
    }
    DumpCurentQueueInfo(dumper, dumpMaxSize);
}

void EventQueueBase::DumpQueueInfo(std::string& queueInfo)
{
    std::lock_guard<std::mutex> lock(queueLock_);
    if (!usable_.load()) {
        HILOGW("EventQueue is unavailable.");
        return;
    }
    std::string priority[] = {"VIP", "Immediate", "High", "Low"};
    uint32_t total = 0;
    for (uint32_t i = 0; i < SUB_EVENT_QUEUE_NUM; ++i) {
        uint32_t n = 0;
        queueInfo +=  "            " + priority[i] + " priority event queue:" + LINE_SEPARATOR;
        for (auto it = subEventQueues_[i].queue.begin(); it != subEventQueues_[i].queue.end(); ++it) {
            ++n;
            queueInfo +=  "            No." + std::to_string(n) + " : " + (*it)->Dump();
            ++total;
        }
        queueInfo +=  "              Total size of " + priority[i] + " events : " + std::to_string(n) + LINE_SEPARATOR;
    }

    queueInfo += "            Idle priority event queue:" + LINE_SEPARATOR;

    int n = 0;
    for (auto it = idleEvents_.begin(); it != idleEvents_.end(); ++it) {
        ++n;
        queueInfo += "            No." + std::to_string(n) + " : " + (*it)->Dump();
        ++total;
    }
    queueInfo += "              Total size of Idle events : " + std::to_string(n) + LINE_SEPARATOR;
    queueInfo += "            Total event size : " + std::to_string(total);
}

bool EventQueueBase::IsIdle()
{
    return isIdle_;
}

bool EventQueueBase::IsQueueEmpty()
{
    std::lock_guard<std::mutex> lock(queueLock_);
    if (!usable_.load()) {
        HILOGW("EventQueue is unavailable.");
        return false;
    }
    for (uint32_t i = 0; i < SUB_EVENT_QUEUE_NUM; ++i) {
        uint32_t queueSize = subEventQueues_[i].queue.size();
        if (queueSize != 0) {
            return false;
        }
    }

    return idleEvents_.size() == 0;
}

void EventQueueBase::PushHistoryQueueBeforeDistribute(const InnerEvent::Pointer &event)
{
    if (event == nullptr) {
        HILOGW("event is nullptr.");
        return;
    }
    historyEvents_[historyEventIndex_].senderKernelThreadId = event->GetSenderKernelThreadId();
    historyEvents_[historyEventIndex_].sendTime = event->GetSendTime();
    historyEvents_[historyEventIndex_].handleTime = event->GetHandleTime();
    historyEvents_[historyEventIndex_].triggerTime = InnerEvent::Clock::now();
    historyEvents_[historyEventIndex_].priority = event->GetEventPriority();
    historyEvents_[historyEventIndex_].completeTime = InnerEvent::TimePoint::max();
    currentRunningEvent_.triggerTime_ = InnerEvent::Clock::now();

    if (event->HasTask()) {
        historyEvents_[historyEventIndex_].hasTask = true;
        historyEvents_[historyEventIndex_].taskName = event->GetTaskName();
    } else {
        historyEvents_[historyEventIndex_].innerEventId = event->GetInnerEventIdEx();
    }
}

void EventQueueBase::PushHistoryQueueAfterDistribute()
{
    historyEvents_[historyEventIndex_].completeTime = InnerEvent::Clock::now();
    historyEventIndex_++;
    historyEventIndex_ = historyEventIndex_ & (HISTORY_EVENT_NUM_POWER - 1);
}

std::string EventQueueBase::HistoryQueueDump(const HistoryEvent &historyEvent)
{
    std::string content;
    std::vector<std::string> prioritys = {"VIP", "Immediate", "High", "Low"};
    content.append("Event { ");
    content.append("send thread = " + std::to_string(historyEvent.senderKernelThreadId));
    content.append(", send time = " + InnerEvent::DumpTimeToString(historyEvent.sendTime));
    content.append(", handle time = " + InnerEvent::DumpTimeToString(historyEvent.handleTime));
    content.append(", trigger time = " + InnerEvent::DumpTimeToString(historyEvent.triggerTime));
    if (historyEvent.completeTime == InnerEvent::TimePoint::max()) {
        content.append(", completeTime time = ");
    } else {
        content.append(", completeTime time = " + InnerEvent::DumpTimeToString(historyEvent.completeTime));
    }
    if (historyEvent.priority >= 0 && historyEvent.priority < prioritys.size()) {
        content.append(", priority = " + prioritys[historyEvent.priority]);
    } else {
        content.append(", priority = ");
    }

    if (historyEvent.hasTask) {
        content.append(", task name = " + historyEvent.taskName);
    } else {
        DumpCurrentRunningEventId(historyEvent.innerEventId, content);
    }
    content.append(" }" + LINE_SEPARATOR);

    return content;
}

std::string EventQueueBase::DumpCurrentQueueSize()
{
    return "Current queue size: IMMEDIATE = " +
    std::to_string(subEventQueues_[static_cast<int>(Priority::IMMEDIATE)].queue.size()) + ", HIGH = " +
    std::to_string(subEventQueues_[static_cast<int>(Priority::HIGH)].queue.size()) + ", LOW = " +
    std::to_string(subEventQueues_[static_cast<int>(Priority::LOW)].queue.size()) + ", IDLE = " +
    std::to_string(idleEvents_.size()) + " ; ";
}

bool EventQueueBase::HasPreferEvent(int basePrio)
{
    for (int prio = 0; prio < basePrio; prio++) {
        if (subEventQueues_[prio].queue.size() > 0) {
            return true;
        }
    }
    return false;
}

PendingTaskInfo EventQueueBase::QueryPendingTaskInfo(int32_t fileDescriptor)
{
    PendingTaskInfo pendingTaskInfo;
    std::shared_ptr<FileDescriptorInfo> fileDescriptorInfo = nullptr;
    if (useDeamonIoWaiter_) {
        fileDescriptorInfo = DeamonIoWaiter::GetInstance().GetFileDescriptorMap(fileDescriptor);
    } else if (ioWaiter_) {
        fileDescriptorInfo = ioWaiter_->GetFileDescriptorMap(fileDescriptor);
    }
    if (fileDescriptorInfo == nullptr) {
        HILOGW("QueryPendingTaskInfo fileDescriptorInfo is unavailable.");
        return pendingTaskInfo;
    }

    std::lock_guard<std::mutex> lock(queueLock_);
    if (!usable_.load()) {
        HILOGW("QueryPendingTaskInfo event queue is unavailable.");
        return pendingTaskInfo;
    }

    auto now = InnerEvent::Clock::now();
    for (auto it = subEventQueues_[0].queue.begin(); it != subEventQueues_[0].queue.end(); it++) {
        if ((*it)->GetTaskName() == fileDescriptorInfo->taskName_) {
            pendingTaskInfo.taskCount++;
            InnerEvent::TimePoint handlerTime = (*it)->GetHandleTime();
            if (handlerTime >= now) {
                continue;
            }
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - handlerTime).count();
            if (duration > pendingTaskInfo.MaxPendingTime) {
                pendingTaskInfo.MaxPendingTime = duration;
            }
        }
    }
    EH_LOGI_LIMIT("Pend task %{public}d %{public}d", pendingTaskInfo.taskCount, pendingTaskInfo.MaxPendingTime);
    return PendingTaskInfo();
}

void EventQueueBase::CancelAndWait()
{
    HILOGD("EventQueueBase CancelAndWait enter.");
}

CurrentRunningEvent::CurrentRunningEvent()
{
    beginTime_ = InnerEvent::TimePoint::max();
}

CurrentRunningEvent::CurrentRunningEvent(InnerEvent::TimePoint time, InnerEvent::Pointer &event)
{
    beginTime_ = time;
    owner_ = event->GetWeakOwner();
    senderKernelThreadId_ = event->GetSenderKernelThreadId();
    sendTime_ = event->GetSendTime();
    handleTime_ = event->GetHandleTime();
    param_ = event->GetParam();
    callerInfo_ = (event->GetCaller()).ToString();
    if (event->HasTask()) {
        hasTask_ = true;
        taskName_ = event->GetTaskName();
    } else {
        innerEventId_ = event->GetInnerEventIdEx();
    }
}

ErrCode EventQueueBase::AddFileDescriptorListener(int32_t fileDescriptor, uint32_t events,
    const std::shared_ptr<FileDescriptorListener> &listener, const std::string &taskName,
    Priority priority)
{
    if ((fileDescriptor < 0) || ((events & FILE_DESCRIPTOR_EVENTS_MASK) == 0) || (!listener)) {
        HILOGE("%{public}d, %{public}u, %{public}s: Invalid parameter",
               fileDescriptor, events, listener ? "valid" : "null");
        return EVENT_HANDLER_ERR_INVALID_PARAM;
    }

    std::lock_guard<std::mutex> lock(queueLock_);
    return AddFileDescriptorListenerBase(fileDescriptor, events, listener, taskName, priority);
}

void EventQueueBase::RemoveFileDescriptorListener(const std::shared_ptr<EventHandler> &owner)
{
    HILOGD("enter");
    if (!owner) {
        HILOGE("Invalid owner");
        return;
    }

    std::lock_guard<std::mutex> lock(queueLock_);
    RemoveListenerByOwner(owner);
}

void EventQueueBase::RemoveFileDescriptorListener(int32_t fileDescriptor)
{
    HILOGD("enter");
    if (fileDescriptor < 0) {
        HILOGE("%{public}d: Invalid file descriptor", fileDescriptor);
        return;
    }

    std::lock_guard<std::mutex> lock(queueLock_);
    RemoveListenerByFd(fileDescriptor);
}

void EventQueueBase::Prepare()
{
    HILOGD("enter");
    std::lock_guard<std::mutex> lock(queueLock_);
    PrepareBase();
}

void EventQueueBase::Finish()
{
    HILOGD("enter");
    std::lock_guard<std::mutex> lock(queueLock_);
    FinishBase();
}

void EventQueueBase::NotifyObserverVipDone(const InnerEvent::Pointer &event)
{
    if (event->GetEventPriority() != static_cast<int32_t>(Priority::VIP)) {
        return;
    }
    NotifyObserverVipDoneBase();
}

void EventQueueBase::NotifyObserverVipDoneBase()
{
    if (subEventQueues_[static_cast<uint32_t>(Priority::VIP)].queue.empty()) {
        InnerEvent::TimePoint time = InnerEvent::Clock::now();
        TryExecuteObserverCallback(time, EventRunnerStage::STAGE_VIP_NONE);
        isExistVipTask_ = false;
    }
}

bool EventQueueBase::hasVipTask()
{
    if (!subEventQueues_[static_cast<uint32_t>(Priority::VIP)].queue.empty()) {
        return true;
    }
    return false;
}

inline uint64_t EventQueueBase::GetQueueFirstEventHandleTime(int32_t priority)
{
    if (__builtin_expect(isBarrierMode_, 0)) {
        return UINT64_MAX;
    }
    return subEventQueues_[static_cast<uint32_t>(priority)].frontEventHandleTime;
}
}  // namespace AppExecFwk
}  // namespace OHOS
