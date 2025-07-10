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
  static readonly DEFAULT_WIFI_OPEN_STATUS: boolean = false;
  static readonly DEFAULT_WIFI_NAME: string = 'WLAN';
  static readonly DEFAULT_WIFI_INFO: number = 0;
  static readonly DEFAULT_WIFI_STATUS: boolean = false;
  static readonly WIFI_SIGNAL_NO: number = 0;
  static readonly WIFI_SIGNAL_LOW: number = 1;
  static readonly WIFI_SIGNAL_MID: number = 2;
  static readonly WIFI_SIGNAL_HIGH: number = 3;
  static readonly WIFI_SIGNAL_FULL: number = 4;
}

export enum WifiState {
  STATE_OFF = 0,
  STATE_ON = 1,
  STATE_TURNING_ON = 2,
  STATE_TURNING_OFF = 3
}

export enum WifiConnectionState {
  DISCONNECTED = 0,
  CONNECTED = 1
}
