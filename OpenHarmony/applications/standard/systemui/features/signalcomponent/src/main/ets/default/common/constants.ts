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
  static readonly RADIO_TECHNOLOGY_UNKNOWN = 0;
  static readonly RADIO_TECHNOLOGY_GSM = 1;
  static readonly RADIO_TECHNOLOGY_1XRTT = 2;
  static readonly RADIO_TECHNOLOGY_WCDMA = 3;
  static readonly RADIO_TECHNOLOGY_HSPA = 4;
  static readonly RADIO_TECHNOLOGY_HSPAP = 5;
  static readonly RADIO_TECHNOLOGY_TD_SCDMA = 6;
  static readonly RADIO_TECHNOLOGY_EVDO = 7;
  static readonly RADIO_TECHNOLOGY_EHRPD = 8;
  static readonly RADIO_TECHNOLOGY_LTE = 9;
  static readonly RADIO_TECHNOLOGY_LTE_CA = 10;
  static readonly RADIO_TECHNOLOGY_IWLAN = 11;
  static readonly RADIO_TECHNOLOGY_NR = 12;

  static readonly REG_STATE_NO_SERVICE = 0;
  static readonly REG_STATE_IN_SERVICE = 1;
  static readonly REG_STATE_EMERGENCY_CALL_ONLY = 2;
  static readonly REG_STATE_POWER_OFF = 3;

  static readonly CELLULAR_SIGNAL_NO = 0;
  static readonly CELLULAR_SIGNAL_MIN = 1;
  static readonly CELLULAR_SIGNAL_LOW = 2;
  static readonly CELLULAR_SIGNAL_HALF = 3;
  static readonly CELLULAR_SIGNAL_HIGH = 4;
  static readonly CELLULAR_SIGNAL_FULL = 5;
  static readonly CELLULAR_NO_SIM_CARD = -1;

  static readonly NET_NULL = 'NULL';

  // Events for Subscribing to Notifications
  static readonly COMMON_EVENT_SPN_INFO_CHANGED = "usual.event.SPN_INFO_CHANGED";
}
