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
import { ControlCenterConfig
} from '../../../../../../../../features/controlcentercomponent/src/main/ets/com/ohos/common/Constants';
import {isNfcAvailable} from '../../../../../../../../common/src/main/ets/default/Constants';

const controlCenterConfig: ControlCenterConfig = {
  ComplexToggleLayout: [FASlotName.WIFI, FASlotName.BLUETOOTH],
  DefaultSimpleToggleLayout: [
    "screenshot",
    FASlotName.RING_MODE,
    FASlotName.LOCATION,
    'demosystemuicplmeta',
    isNfcAvailable()&&FASlotName.NFC,
    FASlotName.AUTO_ROTATE,
    FASlotName.AIR_PLANE
  ],
  LocalToggles: {
    ComplexToggles: [FASlotName.BLUETOOTH, FASlotName.WIFI],
    SimpleToggles: [
      FASlotName.LOCATION,
      FASlotName.RING_MODE,
      isNfcAvailable()&&FASlotName.NFC,
      FASlotName.AUTO_ROTATE,
      FASlotName.AIR_PLANE
    ],
  },
  MetaToggles: [
    {
      id: 'screenshot',
      bundleName: 'com.ohos.screenshot',
      abilityName: 'com.ohos.screenshot.ServiceExtAbility',
    },
    {
      id: 'demosystemuicplmeta',
      bundleName: 'com.ohos.demosystemuicplmeta',
      abilityName: 'com.ohos.demosystemuicplmeta.MainAbility',
    },
  ],
};

export default controlCenterConfig;
