/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

import { describe, beforeAll, beforeEach, afterEach, afterAll, it, expect } from '@ohos/hypium'
import dataPreferences from '@ohos.data.preferences';

const TAG = "[preferencesStageTest]"

export default function stagePreferencesPromiseJsTest() {
  describe('stagePreferencesPromiseJsTest', function () {
    console.log(TAG + "*************Unit Test Begin*************");
    beforeAll(function() {
      console.log('stagePreferencesPromiseJsTest TestAbility context' + globalThis.abilityContext);
      console.info('beforeAll')
    })
    afterAll(async function () {
      console.info('afterAll');
    })
    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0100
     * @tc.name preferences dataGroupId null promise test
     * @tc.desc preferences getPreferences dataGroupId null promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0100', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: null,
      };
      try {
        let preferences = await dataPreferences.getPreferences(globalThis.abilityContext, Options);
        expect(preferences !== null).assertTrue();
      } catch(errInfo){
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0200
     * @tc.name preferences dataGroupId undefined promise test
     * @tc.desc preferences getPreferences dataGroupId undefined promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0200', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: undefined,
      };
      try {
        let preferences = await dataPreferences.getPreferences(globalThis.abilityContext, Options);
        expect(preferences !== null).assertTrue();
      } catch(errInfo){
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0300
     * @tc.name preferences dataGroupId promise test
     * @tc.desc preferences getPreferences error dataGroupId promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0300', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '112233',
      };
      try {
        let preferences = await dataPreferences.getPreferences(globalThis.abilityContext, Options)
        console.info(TAG, `getPreferences successfully.`);
        expect().assertFail();
      } catch(errInfo){
        console.info(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect(errInfo.code).assertEqual('15501002');
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0400
     * @tc.name preferences dataGroupId null promise test
     * @tc.desc preferences removePreferencesFromCache dataGroupId null promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0400', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: null,
      };
      try {
        let promise = dataPreferences.removePreferencesFromCache(globalThis.abilityContext, Options)
        await promise.then(() => {
          console.info(TAG, `removePreferencesFromCache successfully.`);
        }).catch((error) => {
          console.error(TAG, `failed, code is ${error.code}, message is ${error.message}`);
          expect().assertFail();
        })
      } catch(errInfo){
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0500
     * @tc.name preferences dataGroupId undefined promise test
     * @tc.desc preferences removePreferencesFromCache dataGroupId undefined promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0500', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: undefined,
      };
      try {
        let promise = dataPreferences.removePreferencesFromCache(globalThis.abilityContext, Options)
        await promise.then(() => {
          console.info(TAG, `removePreferencesFromCache successfully.`);
        }).catch((error) => {
          console.error(TAG, `failed, code is ${error.code}, message is ${error.message}`);
          expect().assertFail();
        })
      } catch(errInfo){
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0600
     * @tc.name preferences dataGroupId promise test
     * @tc.desc preferences removePreferencesFromCache error dataGroupId promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0600', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '112233',
      };
      try {
        await dataPreferences.removePreferencesFromCache(globalThis.abilityContext, Options)
        console.info(TAG, `removePreferencesFromCache successfully.`);
        expect().assertFail();
      } catch(errInfo){
        console.info(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect(errInfo.code).assertEqual('15501002');
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0700
     * @tc.name preferences dataGroupId null promise test
     * @tc.desc preferences deletePreferences dataGroupId null promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0700', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: null,
      };
      try {
        let promise = dataPreferences.deletePreferences(globalThis.abilityContext, Options)
        await promise.then(() => {
          console.info(TAG, `deletePreferences successfully.`);
        }).catch((error) => {
          console.error(TAG, `failed, code is ${error.code}, message is ${error.message}`);
          expect().assertFail();
        })
      } catch(errInfo){
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })
    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0800
     * @tc.name preferences dataGroupId undefined promise test
     * @tc.desc preferences deletePreferences dataGroupId undefined promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0800', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: undefined,
      };
      try {
        let promise = dataPreferences.deletePreferences(globalThis.abilityContext, Options)
        await promise.then(() => {
          console.info(TAG, `deletePreferences successfully.`);
        }).catch((error) => {
          console.error(TAG, `failed, code is ${error.code}, message is ${error.message}`);
          expect().assertFail();
        })
      } catch(errInfo){
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_0900
     * @tc.name preferences dataGroupId promise test
     * @tc.desc preferences deletePreferences error dataGroupId promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_0900', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '112233',
      };
      try {
        await dataPreferences.deletePreferences(globalThis.abilityContext, Options)
        console.info(TAG, `deletePreferences successfully.`);
        expect().assertFail();
      } catch(errInfo){
        console.info(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect(errInfo.code).assertEqual('15501002');
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Promise_1000
     * @tc.name preferences dataGroupId null promise test
     * @tc.desc preferences getPreferences dataGroupId null promise test
     */
    it('SUB_DDM_Preferences_DataGroupId_Promise_1000', 0, async function () {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '123456',
      };
      try {
        let preferences = await dataPreferences.getPreferences(globalThis.abilityContext, Options);
        expect(preferences !== null).assertTrue();
      } catch(errInfo){
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail()
      }
    })

    console.log(TAG + "*************Unit Test End*************");
  })
}