/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"),
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

/**
 * @file
 * @kit BackgroundTasksKit
 */

import { AsyncCallback } from './@ohos.base';
import notification from './@ohos.notificationManager';
import { NotificationSlot } from './notification/notificationSlot';
import { ValuesBucket } from './@ohos.data.ValuesBucket';

/**
 * Providers static methods for managing reminders, including publishing or canceling a reminder.
 * Add or remove a notification slot, and obtain or cancel all reminders of the current application.
 *
 * @namespace reminderAgentManager
 * @syscap SystemCapability.Notification.ReminderAgent
 * @since 9
 */
declare namespace reminderAgentManager {
  /**
   * Publishes a scheduled reminder.
   * 
   * @permission ohos.permission.PUBLISH_AGENT_REMINDER
   * @param { ReminderRequest } reminderReq - Indicates the reminder instance to publish.
   * @param { AsyncCallback<number> } callback - Indicates the callback function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700001 - Notification is not enabled.
   * @throws { BusinessError } 1700002 - The number of reminders exceeds the limit.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function publishReminder(reminderReq: ReminderRequest, callback: AsyncCallback<number>): void;

  /**
   * Publishes a scheduled reminder.
   *
   * @permission ohos.permission.PUBLISH_AGENT_REMINDER
   * @param { ReminderRequest } reminderReq - Indicates the reminder instance to publish.
   * @returns { Promise<number> } The reminder id.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700001 - Notification is not enabled.
   * @throws { BusinessError } 1700002 - The number of reminders exceeds the limit.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function publishReminder(reminderReq: ReminderRequest): Promise<number>;

  /**
   * Cancel a reminder.
   *
   * @param { number } reminderId - Indicates the reminder id.
   * @param { AsyncCallback<void> } callback - Indicates the callback function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700003 - The reminder does not exist.
   * @throws { BusinessError } 1700004 - The bundle name does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function cancelReminder(reminderId: number, callback: AsyncCallback<void>): void;

  /**
   * Cancel a reminder.
   *
   * @param { number } reminderId - Indicates the reminder id.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700003 - The reminder does not exist.
   * @throws { BusinessError } 1700004 - The bundle name does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function cancelReminder(reminderId: number): Promise<void>;

  /**
   * Obtains all the valid reminders of current application.
   *
   * @param { AsyncCallback<Array<ReminderRequest>> } callback - Indicates the callback function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700004 - The bundle name does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function getValidReminders(callback: AsyncCallback<Array<ReminderRequest>>): void;

  /**
   * Obtains all the valid reminders of current application.
   *
   * @returns { Promise<Array<ReminderRequest>> } The promise returned by the function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700004 - The bundle name does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function getValidReminders(): Promise<Array<ReminderRequest>>;

  /**
   * Cancel all the reminders of current application.
   *
   * @param { AsyncCallback<void> } callback - Indicates the callback function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700004 - The bundle name does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function cancelAllReminders(callback: AsyncCallback<void>): void;

  /**
   * Cancel all the reminders of current application.
   *
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700004 - The bundle name does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function cancelAllReminders(): Promise<void>;

  /**
   * Add notification slot.
   *
   * @param { NotificationSlot } slot - Indicates the slot.
   * @param { AsyncCallback<void> } callback - Indicates the callback function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function addNotificationSlot(slot: NotificationSlot, callback: AsyncCallback<void>): void;

  /**
   * Add notification slot.
   *
   * @param { NotificationSlot } slot - Indicates the slot.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function addNotificationSlot(slot: NotificationSlot): Promise<void>;

  /**
   * Deletes a created notification slot based on the slot type.
   *
   * @param { notification.SlotType } slotType Indicates the type of the slot.
   * @param { AsyncCallback<void> } callback - Indicates the callback function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function removeNotificationSlot(slotType: notification.SlotType, callback: AsyncCallback<void>): void;

  /**
   * Deletes a created notification slot based on the slot type.
   *
   * @param { notification.SlotType } slotType Indicates the type of the slot.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  function removeNotificationSlot(slotType: notification.SlotType): Promise<void>;
  
  /**
   * add exclude date for calendar reminder request.
   *
   * @param { number } reminderId reminder id
   * @param { Date } date date time
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 401 - If the input parameter is not valid parameter.
   * @throws { BusinessError } 1700003 - The reminder does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 12
   */
  function addExcludeDate(reminderId: number, date: Date): Promise<void>;
  
  /**
   * delete exclude date for calendar reminder request.
   *
   * @param { number } reminderId reminder id
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 1700003 - The reminder does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 12
   */
  function deleteExcludeDates(reminderId: number): Promise<void>;
  
  /**
   * get exclude dates
   *
   * @param { number } reminderId reminder id
   * @returns { Promise<Array<Date>> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 1700003 - The reminder does not exist.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 12
   */
  function getExcludeDates(reminderId: number): Promise<Array<Date>>;

  /**
   * Obtains all the valid reminders of current application, include reminderId.
   *
   * @returns { Promise<Array<ReminderInfo>> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 12
   */
  function getAllValidReminders(): Promise<Array<ReminderInfo>>;

  /**
   * Updates a scheduled reminder.
   *
   * @permission ohos.permission.PUBLISH_AGENT_REMINDER
   * @param { reminderId } number - Indicates the reminder id.
   * @param { ReminderRequest } reminderReq - Indicates the reminder.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 1700003 - The reminder does not exist.
   * @throws { BusinessError } 1700007 - If the input parameter is not valid parameter.
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 20
   */
  function updateReminder(reminderId: number, reminderReq: ReminderRequest): Promise<void>;

  /**
   * Declares action button type.
   *
   * @enum { number }
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  export enum ActionButtonType {
    /**
     * Button for closing the reminder.
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    ACTION_BUTTON_TYPE_CLOSE = 0,

    /**
     * Button for snoozing the reminder.
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    ACTION_BUTTON_TYPE_SNOOZE = 1,

    /**
     * The custom button.
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    ACTION_BUTTON_TYPE_CUSTOM = 2
  }

  /**
   * Declares reminder type.
   *
   * @enum { number }
   *
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  export enum ReminderType {
    /**
     * Countdown reminder.
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    REMINDER_TYPE_TIMER = 0,

    /**
     * Calendar reminder.
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    REMINDER_TYPE_CALENDAR = 1,

    /**
     * Alarm reminder.
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    REMINDER_TYPE_ALARM = 2
  }

  /**
   * Action button information. The button will show on displayed reminder.
   *
   * @interface ActionButton
   *
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  interface ActionButton {
    /**
     * Text on the button.
     *
     * @type { string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    title: string;

    /**
     * Resource ID for button title.
     * 
     * @type { ?string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 11
     */
    titleResource?: string;

    /**
     * Button type.
     *
     * @type { ActionButtonType }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    type: ActionButtonType;

    /**
     * Information about the ability that is redirected to when the button is clicked.
     *
     * @type { ?WantAgent }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    wantAgent?: WantAgent;

    /**
     * It will update the database when the button is clicked.
     *
     * @type { ?DataShareUpdate }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    dataShareUpdate?: DataShareUpdate;
  }

  /**
   * Want agent information.
   * It will switch to target ability when you click the displayed reminder.
   *
   * @interface WantAgent
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  interface WantAgent {
    /**
     * Name of the package redirected to when the reminder notification is clicked.
     *
     * @type { string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    pkgName: string;

    /**
     * Name of the ability that is redirected to when the reminder notification is clicked.
     *
     * @type { string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    abilityName: string;

    /**
     * The Uniform Resource Identifier (URI) that will be redirected to.
     *
     * @type { ?string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 12
     */
    uri?: string;
	
    /**
     * The description of the WantParams object in an Want
     *
     * @type { ?Record<string, Object> }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 12
     */
    parameters?: Record<string, Object>;
  }

  /**
   * DataShareUpdate information.
   * It will update the database when the button is clicked.
   *
   * @interface DataShareUpdate
   * @syscap SystemCapability.Notification.ReminderAgent
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  interface DataShareUpdate {
    /**
     * Indicates the path of data to update.
     *
     * @type { string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    uri: string;

    /**
     * Indicates filter criteria.
     *
     * @type { Record<string, number | string | boolean> }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    equalTo: Record<string, number | string | boolean>;

    /**
     * Indicates the data to update. This parameter can be null.
     *
     * @type { ValuesBucket }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    value: ValuesBucket;
  }

  /**
   * Max screen want agent information.
   *
   * @interface MaxScreenWantAgent
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  interface MaxScreenWantAgent {
    /**
     * Name of the package that is automatically started when the reminder arrives and the device is not in use.
     *
     * @type { string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    pkgName: string;

    /**
     * Name of the ability that is automatically started when the reminder arrives and the device is not in use.
     *
     * @type { string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    abilityName: string;
  }

  /**
   * Declares ring channel.
   *
   * @enum { number }
   *
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 20
   */
  export enum RingChannel {
    /**
     * Ring channel alarm.
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 20
     */
    RING_CHANNEL_ALARM = 0,

    /**
     * Ring channel alarm.
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 20
     */
    RING_CHANNEL_MEDIA = 1,
  }

  /**
   * Reminder Common information.
   *
   * @interface ReminderRequest
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  interface ReminderRequest {
    /**
     * Type of the reminder.
     *
     * @type { ReminderType }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    reminderType: ReminderType;

    /**
     * Action button displayed on the reminder notification.
     * (The parameter is optional. Up to two buttons are supported).
     *
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    /**
     * Action button displayed on the reminder notification.
     * (The parameter is optional. Up to three buttons are supported).
     *
     * @type { ?[ActionButton?, ActionButton?, ActionButton?] }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 10
     */
    actionButton?: [ActionButton?, ActionButton?, ActionButton?];

    /**
     * Information about the ability that is redirected to when the notification is clicked.
     *
     * @type { ?WantAgent }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    wantAgent?: WantAgent;

    /**
     * Information about the ability that is automatically started when the reminder arrives.
     * If the device is in use, a notification will be displayed.
     *
     * @type { ?MaxScreenWantAgent }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    maxScreenWantAgent?: MaxScreenWantAgent;

    /**
     * Ringing duration.
     *
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    ringDuration?: number;

    /**
     * Type of the ring channel.
     *
     * @type { ?RingChannel }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 20
     */
    ringChannel?: RingChannel;

    /**
     * Number of reminder snooze times.
     *
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    snoozeTimes?: number;

    /**
     * Reminder snooze interval.
     *
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    timeInterval?: number;

    /**
     * Reminder title.
     *
     * @type { ?string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    title?: string;

    /**
     * Resource ID for title.
     * 
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 18
     */
    titleResourceId?: number;
  
    /**
     * Reminder content.
     *
     * @type { ?string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    content?: string;

    /**
     * Resource ID for content.
     * 
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 18
     */
    contentResourceId?: number;
  
    /**
     * Content to be displayed when the reminder is expired.
     *
     * @type { ?string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    expiredContent?: string;

    /**
     * Resource ID for expired content.
     * 
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 18
     */
    expiredContentResourceId?: number;
  
    /**
     * Content to be displayed when the reminder is snoozing.
     *
     * @type { ?string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    snoozeContent?: string;

    /**
     * Resource ID for snooze content.
     * 
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 18
     */
    snoozeContentResourceId?: number;
  
    /**
     * Notification id. If there are reminders with the same ID, the later one will overwrite the earlier one.
     *
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    notificationId?: number;

    /**
     * If the same group ID is set for reminders, these reminders are canceled together.
     *
     * @type { ?string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 11
     */
    groupId?: string;

    /**
     * Type of the slot used by the reminder.
     *
     * @type { ?notification.SlotType }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    slotType?: notification.SlotType;

    /**
     * Whether the notification is automatically cleared.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 10
     */
    tapDismissed?: boolean;

    /**
     * Time when the notification is automatically cleared.
     *
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 10
     */
    autoDeletedTime?: number;

    /**
     * Type of the snoozeSlot used by the reminder.
     *
     * @type { ?notification.SlotType }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 11
     */
    snoozeSlotType?: notification.SlotType;

    /**
     * The directory of storing reminder announcements.
     *
     * @type { ?string }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 11
     */
    customRingUri?: string;
  }

  /**
   * Calendar reminder information.
   *
   * @extends ReminderRequest
   * @typedef ReminderRequestCalendar
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  interface ReminderRequestCalendar extends ReminderRequest {
    /**
     * Reminder time.
     *
     * @type { LocalDateTime }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    dateTime: LocalDateTime;

    /**
     * Month in which the reminder repeats.
     *
     * @type { ?Array<number> }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    repeatMonths?: Array<number>;

    /**
     * Date on which the reminder repeats.
     *
     * @type { ?Array<number> }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    repeatDays?: Array<number>;

    /**
     * Days of a week when the reminder repeats.
     *
     * @type { ?Array<number> }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 11
     */
    daysOfWeek?: Array<number>;

    /**
     * calendar end date time
     *
     * @type { ?LocalDateTime }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 12
     */
    endDateTime?: LocalDateTime;

    /**
     * For custom repeat calendar(RRule), the service extension will pull up when
     * param set and the notification arrived.
     *
     * @type { ?WantAgent }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    rruleWantAgent?: WantAgent;
  }

  /**
   * Alarm reminder information.
   *
   * @extends ReminderRequest
   * @typedef ReminderRequestAlarm
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  interface ReminderRequestAlarm extends ReminderRequest {
    /**
     * Hour portion of the reminder time.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    hour: number;

    /**
     * Minute portion of the reminder time.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    minute: number;

    /**
     * Days of a week when the reminder repeats.
     *
     * @type { ?Array<number> }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    daysOfWeek?: Array<number>;
  }

  /**
   * CountDown reminder information.
   *
   * @extends ReminderRequest
   * @typedef ReminderRequestTimer
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  interface ReminderRequestTimer extends ReminderRequest {
    /**
     * The number of seconds to count down.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    triggerTimeInSeconds: number;
  }

  /**
   * Reminder information, include reminderId.
   *
   * @interface ReminderInfo
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 12
   */
  interface ReminderInfo {
    /**
     * Reminder id.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 12
     */
    reminderId: number;

    /**
     * Reminder request.
     *
     * @type { ReminderRequest }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 12
     */
    reminderReq: ReminderRequest;
  }

  /**
   * Used for ReminderRequestCalendar when set the time.
   *
   * @interface LocalDateTime
   * @syscap SystemCapability.Notification.ReminderAgent
   * @since 9
   */
  interface LocalDateTime {
    /**
     * Value of year.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    year: number;

    /**
     * Value of month.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    month: number;

    /**
     * Value of day.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    day: number;

    /**
     * Value of hour.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    hour: number;

    /**
     * Value of minute.
     *
     * @type { number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    minute: number;

    /**
     * Value of second.
     *
     * @type { ?number }
     * @syscap SystemCapability.Notification.ReminderAgent
     * @since 9
     */
    second?: number;
  }
}
export default reminderAgentManager;
