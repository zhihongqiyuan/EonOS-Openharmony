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
import { FASlotName } from '../../../../../../../../common/src/main/ets/default/Constants';
import { StatusBarConfig
} from '../../../../../../../../features/statusbarcomponent/src/main/ets/com/ohos/common/Constants';

const statusbarConfig: StatusBarConfig = {
  LayoutGroups: [
    {
      id: 1,
      Components: [
        FASlotName.SIGNAL,
        FASlotName.WIFI,
        FASlotName.CAPSULE,
        FASlotName.NOTIFICATION,
      ],
    },
    {
      id: 2,
      Components: [],
    },
    {
      id: 3,
      Components: [
        FASlotName.LOCATION,
        FASlotName.BLUETOOTH,
        FASlotName.RING_MODE,
        FASlotName.BATTERY,
        FASlotName.CLOCK,
        'systemui_notificationpanel',
        'systemui_controlpanel',
      ],
    },
  ],
  LocalSlots: [
    FASlotName.BATTERY,
    FASlotName.BLUETOOTH,
    FASlotName.CAPSULE,
    FASlotName.CLOCK,
    FASlotName.LOCATION,
    FASlotName.NOTIFICATION,
    FASlotName.RING_MODE,
    FASlotName.SIGNAL,
    FASlotName.WIFI,
  ],
  MetaSlots: [
    {
      id: 'systemui_notificationpanel',
      bundleName: 'com.ohos.systemui',
      abilityName: 'com.ohos.systemui.notificationpanel.ServiceExtAbility',
    },
    {
      id: 'systemui_controlpanel',
      bundleName: 'com.ohos.systemui',
      abilityName: 'com.ohos.systemui.controlpanel.ServiceExtAbility',
    },
  ],
  emptyPosition: {
    x1: 0,
    y1: 0,
    x2: 0,
    y2: 0,
  },
};

export default statusbarConfig;