/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
const calendarManager = requireInternal('calendarManager');

class JsCalendarManager {
  nativeCalendarManager;

  constructor(calendarManager) {
    this.nativeCalendarManager = calendarManager;
  }

  async createCalendar(calendarAccount, callback) {
    if (callback) {
      this.nativeCalendarManager.createCalendar(calendarAccount, callback);
    } else {
      return await this.nativeCalendarManager.createCalendar(calendarAccount);
    }
  }

  async deleteCalendar(calendar, callback) {
    if (callback) {
      this.nativeCalendarManager.deleteCalendar(calendar, callback);
    } else {
      return await this.nativeCalendarManager.deleteCalendar(calendar);
    }
  }

  async getCalendar(calendarAccount, callback) {
    let calendar;
    if (calendarAccount) {
      if (callback) {
        this.nativeCalendarManager.getCalendar(calendarAccount, callback);
      } else {
        return await this.nativeCalendarManager.getCalendar(calendarAccount);
      }
    } else {
      if (callback) {
        this.nativeCalendarManager.getCalendar(callback);
      } else {
        return await this.nativeCalendarManager.getCalendar();
      }
    }
  }

  async getAllCalendars(callback) {
    if (callback) {
      this.nativeCalendarManager.getAllCalendars(callback);
    } else {
      return await this.nativeCalendarManager.getAllCalendars();
    }
  }

  async editEvent(event) {
    console.log('JsCalendarManager editEvent called');
    let context = getContext(this);
    let eventStr = JSON.stringify(event);
    let callerPkg = context.applicationInfo?.name;
    return await this.nativeCalendarManager.editEvent(context, eventStr, callerPkg);
  }
}

let mJsCalendarManager;

function getCalendarManager(context) {
  let nativeCalendarManager = calendarManager.getCalendarManager(context);
  if (mJsCalendarManager === null || mJsCalendarManager === undefined ||
    nativeCalendarManager !== mJsCalendarManager.nativeCalendarManager) {
    // 如果native层拿到的manager不一样了,就也重新创建一个JSCalendarManager包装对象
    mJsCalendarManager = new JsCalendarManager(nativeCalendarManager);
  }
  return mJsCalendarManager;
}

export default {
  getCalendarManager: getCalendarManager,
  CalendarManager: JsCalendarManager,
  Calendar: calendarManager.Calendar,
  CalendarAccount: calendarManager.CalendarAccount,
  CalendarConfig: calendarManager.CalendarConfig,
  Event: calendarManager.Event,
  CalendarType: calendarManager.CalendarType,
  Location: calendarManager.Location,
  EventFilter: calendarManager.EventFilter,
  EventType: calendarManager.EventType,
  RecurrenceRule: calendarManager.RecurrenceRule,
  RecurrenceFrequency: calendarManager.RecurrenceFrequency,
  Attendee: calendarManager.Attendee,
  EventService: calendarManager.EventService,
  ServiceType: calendarManager.ServiceType,
  AttendeeRole: calendarManager.AttendeeRole,
  AttendeeStatus: calendarManager.AttendeeStatus,
  AttendeeType: calendarManager.AttendeeType,
};