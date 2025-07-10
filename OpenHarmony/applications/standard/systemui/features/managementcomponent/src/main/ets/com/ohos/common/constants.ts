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

export default class Constants {
  static readonly WH_45_100 = '45%';
  static readonly WH_50_100 = '50%';
  static readonly WH_60_100 = '60%';
  static readonly WH_70_100 = '70%';
  static readonly WH_90_100 = '90%';
  static readonly WH_100_100 = '100%';
  static readonly MAX_LINES_1 = 1;
  static readonly MAX_LINES_2 = 2;
  static readonly MAX_LINES_3 = 3;
  static readonly TIME_EMPTY = '00:00';
  static readonly DATE_YEAR_IDX = 0;
  static readonly DATE_MONTH_IDX = 1;
  static readonly DATE_DAY_IDX = 2;
  static readonly DATE_HOUR_IDX = 3;
  static readonly DATE_MINUTE_IDX = 4;
}

/**
   * The type of the Do Not Disturb.
   *
   * @since 8
   * @systemapi Hide this for inner system use.
   */
export enum DoNotDisturbType {
    /**
     * Non do not disturb type notification
     */
    TYPE_NONE = 0,

    /**
     * Execute do not disturb once in the set time period (only watch hours and minutes)
     */
    TYPE_ONCE = 1,

    /**
     * Execute do not disturb every day with a set time period (only watch hours and minutes)
     */
    TYPE_DAILY = 2,

    /**
     * Execute in the set time period (specify the time, month, day and hour)
     */
    TYPE_CLEARLY = 3,
}