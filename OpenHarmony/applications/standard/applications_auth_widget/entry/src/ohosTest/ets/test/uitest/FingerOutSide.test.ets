/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License")
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

import { Driver, ON } from '@ohos.UiTest';
import { beforeAll, describe, expect, it } from '@ohos/hypium';
import abilityDelegatorRegistry from '@ohos.app.ability.abilityDelegatorRegistry';
import Want from '@ohos.app.ability.Want';
import { WantParams } from '../../../../main/ets/common/vm/Constants';

const delegator = abilityDelegatorRegistry.getAbilityDelegator();
const bundleName = abilityDelegatorRegistry.getArguments().bundleName;

const delayTime: number = 1000;

export default function FingerOutSideTest() {
  describe('FingerOutSideTest', () => {

    let sleep: (ms: number) => Promise<Function> = (ms: number): Promise<Function> => {
      return new Promise(resolve => setTimeout(resolve, ms));
    }

    beforeAll(async () => {
      const data: WantParams = {
        "cmd": [{
          "event": "CMD_NOTIFY_AUTH_START",
          "payload": {
            "lockoutDuration": 0,
            "remainAttempts": 5,
            "sensorInfo": "{\"sensorType\":\"OUT_OF_SCREEN_SENSOR\",\"outOfScreenSensorType\":\"SIDE\"}",
            "type": "fingerprint",
            "result": 0
          },
        }],
        "pinSubType": "PIN_SIX",
        "title": "请输入密码",
        "type": ["fingerprint"],
        "navigationButtonText": "按钮",
        "widgetContextId": 10234972519197718021,
        "windowModeType": "DIALOG_BOX"
      }
      sleep(delayTime);

      let want: Want = {
        bundleName: bundleName,
        abilityName: 'FingerOutSideAbility',
        parameters: {
          useriamCmdData: data
        }
      }
      await delegator.startAbility(want);
      sleep(delayTime);
    })

    it('cancel_fingerSide_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('outFingerImage'));
      expect(true).assertTrue();
      done();
    })
  })
}