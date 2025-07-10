
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

#include "event_queue_ffrt.h"

#include "ffrt_inner.h"
#include "deamon_io_waiter.h"
#include "epoll_io_waiter.h"
#include "event_logger.h"
#include "event_handler.h"
#include "none_io_waiter.h"

namespace OHOS {
namespace AppExecFwk {
namespace {

DEFINE_EH_HILOG_LABEL("EventQueueFFRT");
constexpr static uint32_t MAX_DUMP_INFO_LENGTH = 120000;
constexpr static uint32_t MILLI_TO_MICRO = 1000;
static constexpr int FFRT_REMOVE_SUCC = 0;
ffrt_inner_queue_priority_t TransferInnerPriority(EventQueue::Priority priority)
{
    ffrt_inner_queue_priority_t innerPriority = ffrt_inner_queue_priority_t::ffrt_inner_queue_priority_low;
    switch (priority) {
        case EventQueue::Priority::VIP:
            innerPriority = ffrt_inner_queue_priority_t::ffrt_inner_queue_priority_vip;
            break;
        case EventQueue::Priority::IMMEDIATE:
            innerPriority = ffrt_inner_queue_priority_t::ffrt_inner_queue_priority_immediate;
            break;
        case EventQueue::Priority::HIGH:
            innerPriority = ffrt_inner_queue_priority_t::ffrt_inner_queue_priority_high;
            break;
        case EventQueue::Priority::LOW:
            innerPriority = ffrt_inner_queue_priority_t::ffrt_inner_queue_priority_low;
            break;
        case EventQueue::Priority::IDLE:
            innerPriority = ffrt_inner_queue_priority_t::ffrt_inner_queue_priority_idle;
            break;
        default:
            break;
    }
    return innerPriority;
}

inline ffrt_queue_t* TransferQueuePtr(std::shared_ptr<ffrt::queue> queue)
{
    if (queue) {
        return reinterpret_cast<ffrt_queue_t*>(queue.get());
    }
    return nullptr;
}


}  // unnamed namespace

EventQueueFFRT::EventQueueFFRT() : EventQueue()
{
    // Destructed queue in the ffrt task needs to be switched to asynchronous to avoid parsing deadlocks
    ffrtQueue_ = std::shared_ptr<ffrt::queue>(new ffrt::queue(static_cast<ffrt::queue_type>(
        ffrt_inner_queue_type_t::ffrt_queue_eventhandler_adapter), "EventHandler_QUEUE"), [](ffrt::queue* ptr) {
        if (ffrt_this_task_get_id()) {
            ffrt::submit([ptr]() { delete ptr; });
        } else {
            delete ptr;
        }
    });
    HILOGD("Event queue ffrt");
}

EventQueueFFRT::EventQueueFFRT(const std::shared_ptr<IoWaiter> &ioWaiter): EventQueue(ioWaiter)
{
    // Destructed queue in the ffrt task needs to be switched to asynchronous to avoid parsing deadlocks
    ffrtQueue_ = std::shared_ptr<ffrt::queue>(new ffrt::queue(static_cast<ffrt::queue_type>(
        ffrt_inner_queue_type_t::ffrt_queue_eventhandler_adapter), "EventHandler_QUEUE"), [](ffrt::queue* ptr) {
        if (ffrt_this_task_get_id()) {
            ffrt::submit([ptr]() { delete ptr; });
        } else {
            delete ptr;
        }
    });
    HILOGD("Event queue ffrt");
}

EventQueueFFRT::~EventQueueFFRT()
{
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    usable_.store(false);
    ioWaiter_ = nullptr;
    EH_LOGD_LIMIT("EventQueueFFRT is unavailable hence");
}

bool EventQueueFFRT::Insert(InnerEvent::Pointer &event, Priority priority, EventInsertType insertType)
{
    return InsertEvent(event, priority, false, insertType);
}

void EventQueueFFRT::RemoveOrphanByHandlerId(const std::string& handlerId)
{
    // taskname: handler Id | has task | inner event id | param | task name
    std::string regular = handlerId + "\\|.*";
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("Remove is unavailable.");
        return;
    }
    ffrt_queue_cancel_by_name(*queue, regular.c_str());
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return;
    }
    RemoveInvalidFileDescriptor();
}


void EventQueueFFRT::RemoveAll()
{
    HILOGD("RemoveAll");
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return;
    }
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("RemoveAll is unavailable.");
        return;
    }
    ffrt_queue_cancel_all(*queue);
}

void EventQueueFFRT::Remove(const std::shared_ptr<EventHandler> &owner)
{
    HILOGD("Remove");
    if (!owner) {
        HILOGE("Invalid owner");
        return;
    }

    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return;
    }

    // taskname: handler Id | has task | inner event id | param | task name
    std::string regular = owner->GetHandlerId() + "\\|.*";
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("Remove is unavailable.");
        return;
    }
    ffrt_queue_cancel_by_name(*queue, regular.c_str());
}

void EventQueueFFRT::Remove(const std::shared_ptr<EventHandler> &owner, uint32_t innerEventId)
{
    HILOGD("Remove");
    if (!owner) {
        HILOGE("Invalid owner");
        return;
    }

    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return;
    }

    // taskname: handler Id | has task | inner event id | param | task name
    std::string regular = owner->GetHandlerId() + "\\|0\\|" + std::to_string(innerEventId) + "\\|.*";
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("Remove is unavailable.");
        return;
    }
    ffrt_queue_cancel_by_name(*queue, regular.c_str());
}

void EventQueueFFRT::Remove(const std::shared_ptr<EventHandler> &owner, uint32_t innerEventId, int64_t param)
{
    HILOGD("Remove");
    if (!owner) {
        HILOGE("Invalid owner");
        return;
    }

    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return;
    }

    // taskname: handler Id | has task | inner event id | param | task name
    std::string regular = owner->GetHandlerId() + "\\|0\\|" + std::to_string(innerEventId) + "\\|" +
        std::to_string(param) + "\\|.*";
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("Remove is unavailable.");
        return;
    }
    ffrt_queue_cancel_by_name(*queue, regular.c_str());
}

bool EventQueueFFRT::Remove(const std::shared_ptr<EventHandler> &owner, const std::string &name)
{
    HILOGD("Remove");
    if (!owner) {
        HILOGE("Invalid owner");
        return false;
    }

    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return false;
    }

    // taskname: handler Id | has task | inner event id | param | task name
    std::string regular = owner->GetHandlerId() + "\\|1\\|" + ".*\\|" + name;
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("Remove is unavailable.");
        return false;
    }
    int ret = ffrt_queue_cancel_by_name(*queue, regular.c_str());
    return ret == FFRT_REMOVE_SUCC ? true : false;
}

bool EventQueueFFRT::HasInnerEvent(const std::shared_ptr<EventHandler> &owner, uint32_t innerEventId)
{
    if (!owner) {
        HILOGE("Invalid owner");
        return false;
    }
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return false;
    }

    // taskname: handler Id | has task | inner event id | param | task name
    std::string regular = owner->GetHandlerId() + "\\|0\\|" + std::to_string(innerEventId) + "\\|.*";
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("Remove is unavailable.");
        return false;
    }
    return ffrt_queue_has_task(*queue, regular.c_str());
}

bool EventQueueFFRT::HasInnerEvent(const std::shared_ptr<EventHandler> &owner, int64_t param)
{
    if (!owner) {
        HILOGE("Invalid owner");
        return false;
    }
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return false;
    }

    // taskname: handler Id | has task | inner event id | param | task name
    std::string regular = owner->GetHandlerId() + "\\|0\\|.*" + std::to_string(param) + "\\|.*";
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("Remove is unavailable.");
        return false;
    }
    return ffrt_queue_has_task(*queue, regular.c_str());
}

void EventQueueFFRT::Dump(Dumper &dumper)
{
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueue is unavailable.");
        return;
    }

    std::unique_ptr<char[]> chars = std::make_unique<char[]>(MAX_DUMP_INFO_LENGTH);
    if (chars == nullptr) {
        return;
    }
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("Dump is unavailable.");
        return;
    }
    int ret = ffrt_queue_dump(*queue, dumper.GetTag().c_str(), chars.get(), MAX_DUMP_INFO_LENGTH, true);
    if (ret > 0) {
        dumper.Dump(chars.get());
    }
}

void EventQueueFFRT::DumpQueueInfo(std::string& queueInfo)
{
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueue is unavailable.");
        return;
    }

    std::unique_ptr<char[]> chars = std::make_unique<char[]>(MAX_DUMP_INFO_LENGTH);
    if (chars == nullptr) {
        return;
    }
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("DumpQueueInfo is unavailable.");
        return;
    }
    int ret = ffrt_queue_dump(*queue, "", chars.get(), MAX_DUMP_INFO_LENGTH, false);
    if (ret > 0) {
        queueInfo.append(chars.get());
    }
}

bool EventQueueFFRT::IsIdle()
{
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueue is unavailable.");
        return false;
    }

    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("IsIdle is unavailable.");
        return false;
    }
    return ffrt_queue_is_idle(*queue);
}

bool EventQueueFFRT::IsQueueEmpty()
{
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueue is unavailable.");
        return false;
    }

    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("IsIdle is unavailable.");
        return false;
    }
    uint32_t queueNum = static_cast<uint32_t>(Priority::IDLE);
    for (uint32_t i = 0; i < queueNum; i++) {
        Priority priority = static_cast<Priority>(i);
        ffrt_inner_queue_priority_t innerPriority = TransferInnerPriority(priority);
        int size = ffrt_queue_size_dump(*queue, innerPriority);
        if (size > 0) {
            return false;
        }
    }
    return true;
}

std::string EventQueueFFRT::DumpCurrentQueueSize()
{
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("DumpCurrentQueueSize is unavailable.");
        return "";
    }

    std::string dumpInfo = "Current queue size: ";
    std::string prioritys[] = {"VIP = ", ", IMMEDIATE = ", ", HIGH =", ", LOW = ", ", IDLE = "};
    uint32_t queueNum = static_cast<uint32_t>(Priority::IDLE);
    for (uint32_t i = 0; i < queueNum; i++) {
        dumpInfo += prioritys[i];
        Priority priority = static_cast<Priority>(i);
        ffrt_inner_queue_priority_t innerPriority = TransferInnerPriority(priority);
        dumpInfo += std::to_string(ffrt_queue_size_dump(*queue, innerPriority));
    }
    dumpInfo += " ; ";
    return dumpInfo;
}

bool EventQueueFFRT::HasPreferEvent(int basePrio)
{
    return false;
}

PendingTaskInfo EventQueueFFRT::QueryPendingTaskInfo(int32_t fileDescriptor)
{
    HILOGW("FFRT queue is not support.");
    return PendingTaskInfo();
}

void EventQueueFFRT::CancelAndWait()
{
    HILOGD("FFRT CancelAndWait enter.");
    if (!usable_.load()) {
        HILOGW("CancelAndWait - EventQueue is unavailable.");
        return;
    }
    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("CancelAndWait - queue is unavailable.");
        return;
    }
    ffrt_queue_cancel_and_wait(*queue);
}

void* EventQueueFFRT::GetFfrtQueue()
{
    if (ffrtQueue_) {
        return reinterpret_cast<void*>(ffrtQueue_.get());
    }
    return nullptr;
}

bool EventQueueFFRT::InsertSyncEvent(InnerEvent::Pointer &event, Priority priority, EventInsertType insertType)
{
    return InsertEvent(event, priority, true, insertType);
}

bool EventQueueFFRT::InsertEvent(InnerEvent::Pointer &event, Priority priority, bool syncWait,
    EventInsertType insertType)
{
    if (!event) {
        HILOGE("Could not insert an invalid event");
        return false;
    }
    std::unique_lock<ffrt::mutex> lock(ffrtLock_);
    if (!usable_.load()) {
        HILOGW("EventQueueFFRT is unavailable.");
        return false;
    }

    // taskname: handler Id | has task | inner event id | param | task name
    std::string taskName = event->GetOwnerId() + "|" + (event->HasTask() ? "1" : "0") + "|" +
        std::to_string(event->GetInnerEventId()) + "|" + std::to_string(event->GetParam()) +
        "|" + event->GetTaskName();
    HILOGD("Submit task %{public}s, %{public}d, %{public}d, %{public}d.", taskName.c_str(), priority,
        insertType, syncWait);
    if (insertType == EventInsertType::AT_FRONT) {
        return SubmitEventAtFront(event, priority, syncWait, taskName, lock);
    } else {
        return SubmitEventAtEnd(event, priority, syncWait, taskName, lock);
    }
}

// helper for move unique_ptr from lambda object to std::function object
template<class F>
auto MakeCopyableFunction(F&& f)
{
    using FType = std::decay_t<F>;
    auto wrapper = std::make_shared<FType>(std::forward<F>(f));
    return [wrapper]() { (*wrapper)(); };
}

bool EventQueueFFRT::SubmitEventAtEnd(InnerEvent::Pointer &event, Priority priority, bool syncWait,
    const std::string &taskName, std::unique_lock<ffrt::mutex> &lock)
{
    uint64_t time = event->GetDelayTime();
    ffrt_queue_priority_t queuePriority = static_cast<ffrt_queue_priority_t>(TransferInnerPriority(priority));
    std::function<void()> task = MakeCopyableFunction([ffrtEvent = std::move(event)]() {
        auto handler = new (std::nothrow) std::shared_ptr<EventHandler>(ffrtEvent->GetOwner());
        if (handler && (*handler)) {
            ffrt_queue_t* queue = reinterpret_cast<ffrt_queue_t*>(
                (*handler)->GetEventRunner()->GetEventQueue()->GetFfrtQueue());
            if (queue != nullptr) {
                ffrt_queue_set_eventhandler(*queue, (void*)handler);
            }
            (*handler)->DistributeEvent(ffrtEvent);
            if (queue != nullptr) {
                ffrt_queue_set_eventhandler(*queue, nullptr);
            }
        }
        delete handler;
    });

    if (syncWait) {
        ffrt::task_handle handle = ffrtQueue_->submit_h(task, ffrt::task_attr().name(taskName.c_str())
            .delay(time * MILLI_TO_MICRO).priority(queuePriority));
        lock.unlock();
        ffrtQueue_->wait(handle);
    } else {
        ffrtQueue_->submit(task, ffrt::task_attr().name(taskName.c_str()).delay(time * MILLI_TO_MICRO).
            priority(queuePriority));
    }
    return true;
}

bool EventQueueFFRT::SubmitEventAtFront(InnerEvent::Pointer &event, Priority priority, bool syncWait,
    const std::string &taskName, std::unique_lock<ffrt::mutex> &lock)
{
    uint64_t time = event->GetDelayTime();
    ffrt_queue_priority_t queuePriority = static_cast<ffrt_queue_priority_t>(TransferInnerPriority(priority));
    ffrt_task_attr_t attribute;
    (void)ffrt_task_attr_init(&attribute);
    ffrt_task_attr_set_name(&attribute, taskName.c_str());
    ffrt_task_attr_set_delay(&attribute, time * MILLI_TO_MICRO);
    ffrt_task_attr_set_queue_priority(&attribute, queuePriority);

    std::function<void()> task = MakeCopyableFunction([ffrtEvent = std::move(event)]() {
        auto handler = new (std::nothrow) std::shared_ptr<EventHandler>(ffrtEvent->GetOwner());
        if (handler && (*handler)) {
            ffrt_queue_t* queue = reinterpret_cast<ffrt_queue_t*>(
                (*handler)->GetEventRunner()->GetEventQueue()->GetFfrtQueue());
            if (queue != nullptr) {
                ffrt_queue_set_eventhandler(*queue, (void*)handler);
            }
            (*handler)->DistributeEvent(ffrtEvent);
            if (queue != nullptr) {
                ffrt_queue_set_eventhandler(*queue, nullptr);
            }
        }
        delete handler;
    });

    ffrt_queue_t* queue = TransferQueuePtr(ffrtQueue_);
    if (queue == nullptr) {
        HILOGW("SubmitEventAtFront is unavailable.");
        return false;
    }
    ffrt_function_header_t* header = ffrt::create_function_wrapper((task));
    if (syncWait) {
        ffrt::task_handle handle = ffrt_queue_submit_head_h(*queue, header, &attribute);
        lock.unlock();
        ffrtQueue_->wait(handle);
    } else {
        ffrt_queue_submit_head(*queue, header, &attribute);
    }
    return true;
}

ErrCode EventQueueFFRT::AddFileDescriptorListener(int32_t fileDescriptor, uint32_t events,
    const std::shared_ptr<FileDescriptorListener> &listener, const std::string &taskName,
    Priority priority)
{
    if ((fileDescriptor < 0) || ((events & FILE_DESCRIPTOR_EVENTS_MASK) == 0) || (!listener)) {
        HILOGE("%{public}d, %{public}u, %{public}s: Invalid parameter",
               fileDescriptor, events, listener ? "valid" : "null");
        return EVENT_HANDLER_ERR_INVALID_PARAM;
    }

    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    return AddFileDescriptorListenerBase(fileDescriptor, events, listener, taskName, priority);
}

void EventQueueFFRT::RemoveFileDescriptorListener(const std::shared_ptr<EventHandler> &owner)
{
    HILOGD("enter");
    if (!owner) {
        HILOGE("Invalid owner");
        return;
    }

    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    RemoveListenerByOwner(owner);
}

void EventQueueFFRT::RemoveFileDescriptorListener(int32_t fileDescriptor)
{
    HILOGD("enter");
    if (fileDescriptor < 0) {
        HILOGE("%{public}d: Invalid file descriptor", fileDescriptor);
        return;
    }

    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    RemoveListenerByFd(fileDescriptor);
}

inline uint64_t EventQueueFFRT::GetQueueFirstEventHandleTime(int32_t priority)
{
    return UINT64_MAX;
}

void EventQueueFFRT::Prepare()
{
    HILOGD("enter");
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    PrepareBase();
}

void EventQueueFFRT::Finish()
{
    HILOGD("enter");
    std::lock_guard<ffrt::mutex> lock(ffrtLock_);
    FinishBase();
}
}  // namespace AppExecFwk
}  // namespace OHOS
