/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef NATIVE_CALENDAR_H
#define NATIVE_CALENDAR_H
#include <vector>
#include <set>
#include "calendar_define.h"
#include "event_filter.h"
#include "uri.h"
using Uri = OHOS::Uri;
namespace OHOS::CalendarApi::Native {
class Calendar final {
public:
    Calendar(int id);
    Calendar(CalendarAccount account, int id);
    Calendar(Calendar&& other) : m_account(std::move(other.m_account)), m_id(other.m_id)
    {
    }
    Calendar(const Calendar& other) : m_account(other.m_account), m_id(other.m_id)
    {
    }

    Calendar() = delete;
    ~Calendar() = default;
    int AddEvent(const Event& event);
    int AddEvents(const std::vector<Event>& events);
    bool DeleteEvent(int id);
    int DeleteEvents(const std::vector<int>& id);
    void DeleteAllEvents();
    bool UpdateEvent(const Event& event);
    int UpdateEvents(const std::vector<Event>& events);
    CalendarConfig GetConfig();
    bool SetConfig(const CalendarConfig& config);
    std::vector<Event> GetEvents(std::shared_ptr<EventFilter> filter, const std::vector<string>& eventKey);
    std::vector<Attendee> GetAttendeesByEventId(int id);
    std::optional<std::vector<int>> GetRemindersByEventId(int id);
    void InsertReminders(int eventId, vector<int> reminders);
    int AddEventInfo(const Event& event, int channelId);
    std::vector<Event> QueryEventInstances(int64_t start, int64_t end, const std::vector<int> &ids,
     const std::vector<string>& eventKey);
    void FillEventsInfo(std::vector<Event> &events, const std::set<std::string>& resultSetField);
    CalendarAccount GetAccount() const
    {
        return m_account;
    }
    int GetId() const
    {
        return m_id;
    }
private:
    CalendarAccount m_account;
    CalendarConfig m_config;
    int m_id = 0;
    std::unique_ptr<Uri> m_eventUri;
    std::unique_ptr<Uri> m_attendeeUri;
    std::unique_ptr<Uri> m_calendarUri;
    std::unique_ptr<Uri> m_reminderUrl;
    std::unique_ptr<Uri> m_instanceUrl;
};
}  // namespace OHOS::Calendar
#endif  //  NATIVE_CALENDAR_H