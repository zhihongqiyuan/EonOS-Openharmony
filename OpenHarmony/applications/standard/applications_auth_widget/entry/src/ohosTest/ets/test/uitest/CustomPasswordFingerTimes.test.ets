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

const delegator = abilityDelegatorRegistry.getAbilityDelegator();
const bundleName = abilityDelegatorRegistry.getArguments().bundleName;

const delayTime: number = 1000;

export default function CustomPasswordFingerTimesTest() {
  describe('CustomPasswordFingerTimesTest', () => {
    beforeAll(async () => {
      const data = {
        "cmd": [{
          "event": "CMD_NOTIFY_AUTH_START", "payload": {
            "lockoutDuration": 0, "remainAttempts": 2, "type": "pin", "sensorInfo": ""
          },
        }, {
          "event": "CMD_NOTIFY_AUTH_START", "payload": {
            "lockoutDuration": 0, "remainAttempts": 1, "type": "face", "sensorInfo": ""
          },
        }, {
          "event": "CMD_NOTIFY_AUTH_START", "payload": {
            "lockoutDuration": 0,
            "remainAttempts": 1,
            "sensorInfo": "{\"sensorType\":\"SensorType1\",\"udSensorCenterXInThousandth\":500,\"udSensorCenterYInThousandth\":500,\"udSensorRadiusInPx\":30}",
            "type": "fingerprint"
          },
        }],
        "pinSubType": "PIN_MIXED",
        "title": "",
        "type": ["pin", "face", "fingerprint"],
        "widgetContextId": 6711153399710207000,
        "windowModeType": "FULLSCREEN"
      };

      let want: Want = {
        bundleName: bundleName,
        abilityName: 'FingerprintAbility',
        parameters: {
          useriamCmdData: data
        }
      }
      await delegator.startAbility(want);
    })

    it('pinInput_full_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('pinInput'));
      expect(true).assertTrue();
      done();
    })
  })
}