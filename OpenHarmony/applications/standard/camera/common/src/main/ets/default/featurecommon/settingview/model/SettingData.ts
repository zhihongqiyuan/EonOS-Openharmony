/**
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

export class SettingData {
  settingTitle?: string | Resource;
  imagePath?: string | Resource;
  settingName?: Resource;
  checkedName?: Resource;
  settingAlias?: Resource;
  selectType?: string;
  radio?: string;
  toggle?: string;
  description?: string;

  constructor() {
  }
}


export class RadioItem {
  itemValue?: any
}

export class SettingItem {
  imagePath?: Resource;
  settingName?: Resource;
  description?: any;
  checkedName?: any;
  settingAlias?: any;
  selectType?: any;
  radio?: RadioItem[];
  toggle?: boolean;
}

export class SettingGroupItem {
  settingTitle?: Resource;
  settingChildren?: SettingItem[];
}