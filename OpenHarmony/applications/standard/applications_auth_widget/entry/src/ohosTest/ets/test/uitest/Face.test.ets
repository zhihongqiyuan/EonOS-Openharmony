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
import userAuth from '@ohos.userIAM.userAuth';
import { WantParams } from '../../../../main/ets/common/vm/Constants';
import Want from '@ohos.app.ability.Want';

const delegator = abilityDelegatorRegistry.getAbilityDelegator();
const bundleName = abilityDelegatorRegistry.getArguments().bundleName;

const delayTime: number = 1000;

export default function FaceTest() {
  describe('FaceTest', () => {
    beforeAll(async () => {
      const authParam : userAuth.AuthParam = {
        challenge: new Uint8Array([49, 49, 49, 49, 49, 49]),
        authType: [2],
        authTrustLevel: 10000,
      };
      const widgetParam : userAuth.WidgetParam = {
        title: '使用人脸验证',
        windowMode: 1,
        navigationButtonText: '确认',
      };
      let userAuthInstance = await userAuth.getUserAuthInstance(authParam, widgetParam);
      userAuthInstance.start();

      const data: WantParams = {
        "cmd": [{
          "event": "CMD_NOTIFY_AUTH_START",
          "payload": {
            "lockoutDuration": 0,
            "remainAttempts": 5,
            "type": "face",
            "result": 0
          },
        }],
        "title": "请输入密码",
        "type": ["face"],
        "widgetContextId": 13808827370146417000,
        "windowModeType": "DIALOG_BOX",
        "pinSubType": "PIN_SIX"
      }

      let want: Want = {
        bundleName: bundleName,
        abilityName: 'FaceAbility',
        parameters: {
          useriamCmdData: data
        }
      }
      await delegator.startAbility(want);
    })

    it('title_faceAuth_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('titleFaceAuth'));
      expect(true).assertTrue();
      done();
    })

    it('image_faceAuth_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('imageFaceAuth'));
      expect(true).assertTrue();
      done();
    })

    it('text_faceAuth_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textFaceAuth'));
      expect(true).assertTrue();
      done();
    })

    it('cancelBtn_allFace_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('cancelBtnAllFace'));
      expect(true).assertTrue();
      done();
    })
  })
}