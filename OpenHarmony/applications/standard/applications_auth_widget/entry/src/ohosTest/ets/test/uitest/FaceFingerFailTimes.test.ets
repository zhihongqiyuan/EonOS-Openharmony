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
import { WantParams } from '../../../../main/ets/common/vm/Constants';
import Want from '@ohos.app.ability.Want';

const delegator = abilityDelegatorRegistry.getAbilityDelegator();
const bundleName = abilityDelegatorRegistry.getArguments().bundleName;

const delayTime: number = 1000;

export default function FaceFingerFailTimesTest() {
  describe('FaceFingerFailTimesTest', () => {
    beforeAll(async () => {
      const data: WantParams = {
        "cmd": [{
          "event": "CMD_NOTIFY_AUTH_START",
          "payload": {
            "lockoutDuration": 0,
            "remainAttempts": 3,
            "type": "face",
            "result": 0
          },
        }, {
          "event": "CMD_NOTIFY_AUTH_START",
          "payload": {
            "lockoutDuration": 0,
            "remainAttempts": 4,
            "sensorInfo": "{\"sensorType\":\"SensorType1\",\"udSensorCenterXInThousandth\":500,\"udSensorCenterYInThousandth\":1280,\"udSensorRadiusInPx\":30}",
            "type": "fingerprint",
            "result": 0
          },
        }],
        "pinSubType": "",
        "title": "",
        "type": ["face", "fingerprint"],
        "widgetContextId": 7728484037972500000,
        "windowModeType": "DIALOG_BOX"
      }

      let want: Want = {
        bundleName: bundleName,
        abilityName: 'FaceFingerFailNonAbility',
        parameters: {
          useriamCmdData: data
        }
      }
      await delegator.startAbility(want);
    })

    it('faceFingerFailTimes_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('cancelImgAllFaceAuth'));
      expect(true).assertTrue();
      done();
    })
  })
}