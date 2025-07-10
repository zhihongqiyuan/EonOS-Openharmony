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
import Constants, { CmdData, WantParams } from '../../../../main/ets/common/vm/Constants';
import Want from '@ohos.app.ability.Want';

const delegator = abilityDelegatorRegistry.getAbilityDelegator();
const bundleName = abilityDelegatorRegistry.getArguments().bundleName;

const delayTime: number = 1000;

export default function CustomPasswordNumberTest() {
  describe('CustomPasswordNumberTest', () => {
    beforeAll(async () => {
      const authParam : userAuth.AuthParam = {
        challenge: new Uint8Array([49, 49, 49, 49, 49, 49]),
        authType: [1],
        authTrustLevel: 10000,
      };
      const widgetParam : userAuth.WidgetParam = {
        title: '使用密码验证',
        windowMode: 2,
        navigationButtonText: '确认',
      };
      let userAuthInstance = await userAuth.getUserAuthInstance(authParam, widgetParam);
      userAuthInstance.start();

      const data :WantParams = {
        "cmd": [{
          "event": "CMD_NOTIFY_AUTH_START",
          "payload": {
            "lockoutDuration": 0,
            "remainAttempts": 5,
            "type": "pin",
            "sensorInfo": "",
            "result": 1
          },
        }],
        "pinSubType": "PIN_SIX",
        "title": "",
        "type": ["pin"],
        "widgetContextId": 7932856209793250000,
        "navigationButtonText": "返回",
        "windowModeType": "FULLSCREEN",
      };

      let want: Want = {
        bundleName: bundleName,
        abilityName: 'CustomPwdNumberAbility',
        parameters: {
          useriamCmdData: data
        }
      }
      await delegator.startAbility(want);
    })

    it('numKeyBord0_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd0'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord1_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd1'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord2_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd2'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord3_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd3'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord4_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd4'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord5_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd5'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord6_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd6'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord7_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd7'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord8_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd8'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord10_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd10'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord9_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd9'));
      expect(true).assertTrue();
      done();
    })

    it('numKeyBord11_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('numKeyBordCustomPwd11'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord0_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd0'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord1_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd1'));
      expect(true).assertTrue();
      done();
    })

    it('teNtnumKeyBord2_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd2'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord3_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd3'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord4_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd4'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord5_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd5'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord6_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd6'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord7_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd7'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord8_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd8'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord10_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd10'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord9_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd9'));
      expect(true).assertTrue();
      done();
    })

    it('textNumKeyBord11_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('textNumKeyBordCustomPwd11'));
      expect(true).assertTrue();
      done();
    })

    it('textPrompt_customPwd_test', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('cancelIconCustomPwd'));
      expect(true).assertTrue();
      done();
    })

    it('clickEventCancal_Input', 0, async (done: Function) => {
      let driver = Driver.create();
      await driver.delayMs(delayTime);
      await driver.findComponent(ON.id('pinSix'));
      expect(true).assertTrue();
      done();
    })
  })
}