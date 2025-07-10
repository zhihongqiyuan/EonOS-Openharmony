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

import AbilityManager from '../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import ControlCenterComponent from '../../../../../../../features/controlcentercomponent/src/main/ets/com/ohos/pages/ControlCenterComponent'
import Log from '../../../../../../../common/src/main/ets/default/Log'
import Constants from './common/constants'
import ControlCenterConfig from './common/ControlCenterConfig';

const TAG = 'ControlPanel-Control';

@Component
export default struct Control {
  private modeChangeCallback: Function = () => {}

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start`)
    AbilityManager.setContextName(AbilityManager.ABILITY_NAME_CONTROL_PANEL,AbilityManager.ABILITY_NAME_CONTROL_PANEL);
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisAppear`)
  }

  build() {
    Column() {
      ControlCenterComponent({
        mControlCenterComponentConfig: ControlCenterConfig,
        mSimpleToggleColumnCount: Constants.DEFAULT_SIMPLE_TOGGLE_COLUMN_COUNT,
        modeChangeCallback: (isEdit: boolean): void => this.modeChangeCallback(isEdit),
        titleDisplayInside: true,
        moduleName: 'pc_controlpanel',
      })
    }
    .width('100%')
  }
}