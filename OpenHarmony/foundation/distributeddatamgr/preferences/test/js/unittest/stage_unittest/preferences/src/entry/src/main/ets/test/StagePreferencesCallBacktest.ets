/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

export default function stagePreferencesCallbackJsTest() {
  describe('stagePreferencesCallbackJsTest', ():void =>{
    console.log(TAG + "*************Unit Test Begin*************");
    beforeAll(():void => {
      console.log('stagePreferencesCallbackJsTest TestAbility context' + globalThis.abilityContext);
      console.info('beforeAll')
    })
    afterAll(():void =>{
      console.info('afterAll')
    })
    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0100
     * @tc.name preferences dataGroupId null callback test
     * @tc.desc preferences getPreferences dataGroupId null callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0100', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: null,
      };
      try {
        dataPreferences.getPreferences(globalThis.abilityContext, Options, (err, preferences) =>{
          if (err) {
            console.error(TAG,`failed err, code is ${err.code}, message is ${err.message}`);
            done();
            expect().assertFail();
          }
          console.info(TAG, `getPreferences successfully.`);
          done();
          expect(preferences !== null).assertTrue();
        })
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0200
     * @tc.name preferences dataGroupId undefined callback test
     * @tc.desc preferences getPreferences dataGroupId undefined callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0200', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: undefined,
      };
      try {
        dataPreferences.getPreferences(globalThis.abilityContext, Options, (err, preferences) => {
          if (err) {
            console.error(TAG,`failed err, code is ${err.code}, message is ${err.message}`);
            done();
            expect().assertFail();
          }
          console.info(TAG, `getPreferences successfully.`);
          done();
          expect(preferences !== null).assertTrue();
        })
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect().assertFail();
        }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0300
     * @tc.name preferences dataGroupId callback test
     * @tc.desc preferences getPreferences error dataGroupId callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0300', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '112233',
      };
      try {
        dataPreferences.getPreferences(globalThis.abilityContext, Options, (err, preferences) => {
          console.info(TAG, `getPreferences successfully.`);
          done();
          expect().assertFail();
        })
      } catch (errInfo) {
        console.info(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect(errInfo.code).assertEqual('15501002');
      } 
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0400
     * @tc.name preferences dataGroupId null callback test
     * @tc.desc preferences removePreferencesFromCache dataGroupId null callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0400', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: null,
      };
      try {
        dataPreferences.removePreferencesFromCache(globalThis.abilityContext, Options, (err) => {
          if (err) {
            console.error(TAG,`failed err, code is ${err.code}, message is ${err.message}`);
            done();
            expect().assertFail();
          }
          console.info(TAG, `removePreferencesFromCache successfully.`);
          done();
        })
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0500
     * @tc.name preferences dataGroupId undefined callback test
     * @tc.desc preferences removePreferencesFromCache dataGroupId undefined callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0500', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: undefined,
      };
      try {
        dataPreferences.removePreferencesFromCache(globalThis.abilityContext, Options, (err) => {
          if (err) {
            console.error(TAG,`failed err, code is ${err.code}, message is ${err.message}`);
            done();
            expect().assertFail();
          }
          console.info(TAG, `removePreferencesFromCache successfully.`);
          done();
        })
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0600
     * @tc.name preferences dataGroupId callback test
     * @tc.desc preferences removePreferencesFromCache error dataGroupId callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0600', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '112233',
      };
      try {
        dataPreferences.removePreferencesFromCache(globalThis.abilityContext, Options, (err) => {
          console.info(TAG, `removePreferencesFromCache successfully.`);
          done();
          expect().assertFail();
        })
      } catch (errInfo) {
        console.info(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect(errInfo.code).assertEqual('15501002');
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0700
     * @tc.name preferences dataGroupId null callback test
     * @tc.desc preferences deletePreferences dataGroupId null callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0700', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: null,
      };
      try {
        dataPreferences.deletePreferences(globalThis.abilityContext, Options, (err) => {
          if (err) {
            console.error(TAG,`failed err, code is ${err.code}, message is ${err.message}`);
            done();
            expect().assertFail();
          }
          console.info(TAG, `deletePreferences successfully.`);
          done();
        })
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0800
     * @tc.name preferences dataGroupId undefined callback test
     * @tc.desc preferences deletePreferences dataGroupId undefined callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0800', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: undefined,
      };
      try {
        dataPreferences.deletePreferences(globalThis.abilityContext, Options, (err) =>{
          if (err) {
            console.error(TAG,`failed err, code is ${err.code}, message is ${err.message}`);
            done();
            expect().assertFail();
          }
          console.info(TAG, `deletePreferences successfully.`);
          done();
        })
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect().assertFail();
        }
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_0900
     * @tc.name preferences dataGroupId callback test
     * @tc.desc preferences deletePreferences error dataGroupId callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_0900', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '112233',
      };
      try {
        dataPreferences.deletePreferences(globalThis.abilityContext, Options, (err) =>{
          console.info(TAG, `deletePreferences successfully.`);
          done();
          expect().assertFail();
        })
      } catch (errInfo) {
        console.info(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect(errInfo.code).assertEqual('15501002');
      } 
    })

    /**
     * @tc.number SUB_DDM_Preferences_DataGroupId_Callback_1000
     * @tc.name preferences dataGroupId callback test
     * @tc.desc preferences getPreferences dataGroupId callback test
     */
    it('SUB_DDM_Preferences_DataGroupId_Callback_1000', 0, (done) => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '123456',
      };
      try {
        dataPreferences.getPreferences(globalThis.abilityContext, Options, (err, preferences) =>{
          if (err) {
            console.error(TAG,`failed err, code is ${err.code}, message is ${err.message}`);
            done();
            expect().assertFail();
          }
          console.info(TAG, `getPreferences successfully.`);
          done();
          expect(preferences !== null).assertTrue();
        })
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        done();
        expect().assertFail();
      } 
    })

    console.log(TAG + "*************Unit Test End*************");
  })
}