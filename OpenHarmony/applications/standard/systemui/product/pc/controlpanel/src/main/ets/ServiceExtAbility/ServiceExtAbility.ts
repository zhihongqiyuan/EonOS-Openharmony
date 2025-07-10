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

import ServiceExtension from '@ohos.app.ability.ServiceExtensionAbility';
import display from '@ohos.display';
import Log from '../../../../../../../common/src/main/ets/default/Log';
import WindowManager, { WindowType } from '../../../../../../../common/src/main/ets/default/WindowManager';
import AbilityManager from '../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import { Want } from 'ability/want';
import ControlCenterConstants from '../../../../../../../features/controlcentercomponent/src/main/ets/com/ohos/common/Constants';

const TAG = 'ControlPanel_ServiceExtAbility';

class ServiceExtAbility extends ServiceExtension {
  async onCreate(want: Want): Promise<void> {
    Log.showInfo(TAG, `onCreate, want: ${JSON.stringify(want)}`);
    AbilityManager.setContext(AbilityManager.ABILITY_NAME_CONTROL_PANEL, this.context);
    globalThis[ControlCenterConstants.PLUGIN_COMPONENT_OWNER_WANT_KEY] = want;

    let dis = await display.getDefaultDisplay();
    Log.showDebug(TAG, `onCreate, dis: ${JSON.stringify(dis)}`);
    let rect = {
      left: (872 * dis.width) / 1280,
      top: (44 * dis.width) / 1280,
      width: (402 * dis.width) / 1280,
      height: (345 * dis.width) / 1280,
    };

    AbilityManager.setAbilityData(AbilityManager.ABILITY_NAME_CONTROL_PANEL, 'rect', rect);
    AbilityManager.setAbilityData(AbilityManager.ABILITY_NAME_CONTROL_PANEL, 'dis', {
      width: dis.width,
      height: dis.height,
    });
    WindowManager.createWindow(this.context, WindowType.CONTROL_PANEL, rect, 'pages/index').then(() => {
      Log.showDebug(TAG, 'createWindow success.');
    }).then(() => {
    }).catch((err) => {
    });
  }

  onDestroy(): void {
    Log.showInfo(TAG, 'onDestroy');
  }
}

export default ServiceExtAbility;
