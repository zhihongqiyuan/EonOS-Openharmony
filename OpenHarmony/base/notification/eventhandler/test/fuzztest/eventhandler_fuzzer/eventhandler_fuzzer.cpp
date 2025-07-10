/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#define private public
#define protected public
#include "event_handler.h"
#undef private
#undef protected

#include "eventhandler_fuzzer.h"
#include "securec.h"
#include "deamon_io_waiter.h"

namespace OHOS {
namespace {
    constexpr size_t U32_AT_SIZE = 4;
}

class DumperTest : public AppExecFwk::Dumper {
public:
    DumperTest() = default;
    virtual ~DumperTest()
    {};
    void Dump(const std::string &message) override
    {}
    std::string GetTag() override
    {
        return {};
    }
};

class MyFileDescriptorListener : public AppExecFwk::FileDescriptorListener {
public:
    MyFileDescriptorListener()
    {}
    ~MyFileDescriptorListener()
    {}

    /* @param int32_t fileDescriptor */
    void OnReadable(int32_t)
    {}

    /* @param int32_t fileDescriptor */
    void OnWritable(int32_t)
    {}

    /* @param int32_t fileDescriptor */
    void OnException(int32_t)
    {}

    MyFileDescriptorListener(const MyFileDescriptorListener &) = delete;
    MyFileDescriptorListener &operator=(const MyFileDescriptorListener &) = delete;
    MyFileDescriptorListener(MyFileDescriptorListener &&) = delete;
    MyFileDescriptorListener &operator=(MyFileDescriptorListener &&) = delete;
};

bool DoSomethingInterestingWithMyAPI(const char* data, size_t size)
{
    std::shared_ptr<AppExecFwk::EventRunner> runner = nullptr;
    AppExecFwk::EventHandler eventHandler(runner);
    uint32_t innerEventId = *data;
    std::list<AppExecFwk::InnerEvent::Pointer> events;
    AppExecFwk::InnerEvent::Pointer event = std::move(events.front());
    int64_t taskTime = U32_AT(reinterpret_cast<const uint8_t*>(data));
    AppExecFwk::EventQueue::Priority priority = AppExecFwk::EventQueue::Priority::LOW;
    int32_t fileDescriptor = U32_AT(reinterpret_cast<const uint8_t*>(data));
    DumperTest dumper;
    eventHandler.Dump(dumper);
    eventHandler.GetEventName(event);
    eventHandler.ProcessEvent(event);
    eventHandler.RemoveAllFileDescriptorListeners();
    eventHandler.SendTimingEvent(event, taskTime, priority);
    eventHandler.RemoveFileDescriptorListener(fileDescriptor);
    eventHandler.SendEvent(event, taskTime, priority);
    eventHandler.SendSyncEvent(event, priority);
    eventHandler.RemoveAllEvents();
    eventHandler.RemoveEvent(innerEventId, taskTime);
    eventHandler.RemoveEvent(innerEventId);
    std::string stringData(data);
    eventHandler.RemoveTask(stringData);
    auto fileDescriptorListener = std::make_shared<MyFileDescriptorListener>();
    eventHandler.AddFileDescriptorListener(fileDescriptor, innerEventId, fileDescriptorListener,
        "DoSomethingInterestingWithMyAPI");
    eventHandler.SetEventRunner(runner);
    eventHandler.GetEventRunner();
    eventHandler.DistributeEvent(event);
    eventHandler.HasInnerEvent(taskTime);
    AppExecFwk::DeamonIoWaiter::GetInstance().Init();
    AppExecFwk::DeamonIoWaiter::GetInstance().StartEpollIoWaiter();
    AppExecFwk::DeamonIoWaiter::GetInstance().StopEpollIoWaiter();
    AppExecFwk::DeamonIoWaiter::GetInstance().NotifyOne();
    AppExecFwk::DeamonIoWaiter::GetInstance().NotifyAll();
    AppExecFwk::DeamonIoWaiter::GetInstance().SupportListeningFileDescriptor();
    return eventHandler.HasInnerEvent(innerEventId);
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    if (data == nullptr) {
        return 0;
    }

    if (size < OHOS::U32_AT_SIZE) {
        return 0;
    }

    char* ch = reinterpret_cast<char *>(malloc(size + 1));
    if (ch == nullptr) {
        return 0;
    }

    (void)memset_s(ch, size + 1, 0x00, size + 1);
    if (memcpy_s(ch, size, data, size) != EOK) {
        free(ch);
        ch = nullptr;
        return 0;
    }

    OHOS::DoSomethingInterestingWithMyAPI(ch, size);
    free(ch);
    ch = nullptr;
    return 0;
}
