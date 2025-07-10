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

import { describe, beforeAll, beforeEach, afterEach, afterAll, it, expect } from '@ohos/hypium';
import data_preferences from '@ohos.data.preferences';
import contextConstant from '@ohos.app.ability.contextConstant';
import fs from '@ohos.file.fs';

const TAG = "[stagePreferencesSyncJsTest]"
let mPreferences;
let context;

function isFileExists(path: string) : boolean {
    if (!path) {
        console.error(TAG, 'path is undefined');
        return false;
    }
    try {
        if (fs.accessSync(path)) {
            return true;
        }
    } catch (err) {
        console.error(TAG, 'accessSync failed: ' + JSON.stringify(err));
    }
    return false;
}

export default function stagePreferencesSyncJsTest() {
  describe('stagePreferencesSyncJsTest', () =>{
    beforeAll(() => {
      console.log('stagePreferencesSyncJsTest TestAbility context' + globalThis.abilityContext);
      context = globalThis.abilityContext;
      console.info('beforeAll');
    })
    afterAll(() => {
      console.info('afterAll');
    })

    /**
     * @tc.number SUB_DDM_Preferences_GetPreferencesSync_0100
     * @tc.name getPreferencesSync interface test
     * @tc.desc getPreferencesSync interface test
     */
    it('SUB_DDM_Preferences_GetPreferencesSync_0100', 0, () => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: null,
      };
      try {
        mPreferences = data_preferences.getPreferencesSync(globalThis.abilityContext, Options)
        console.info(TAG, `getPreferences successfully.`);
        expect(mPreferences !== null).assertTrue();
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_GetPreferencesSync_0200
     * @tc.name getPreferencesSync interface test
     * @tc.desc getPreferencesSync interface test
     */
    it('SUB_DDM_Preferences_GetPreferencesSync_0200', 0, () => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: undefined,
      };
      try {
        mPreferences = data_preferences.getPreferencesSync(globalThis.abilityContext, Options)
        console.info(TAG, `getPreferences successfully.`);
        expect(mPreferences !== null).assertTrue();
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_GetPreferencesSync_0300
     * @tc.name getPreferencesSync interface test
     * @tc.desc getPreferencesSync error interface test
     */
    it('SUB_DDM_Preferences_GetPreferencesSync_0300', 0, () => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '123123',
      };
      try {
        mPreferences = data_preferences.getPreferencesSync(globalThis.abilityContext, Options)
        expect().assertFail()
      } catch (errInfo) {
        console.info(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect(errInfo.code).assertEqual('15501002');
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_GetPreferencesSync_0400
     * @tc.name getPreferencesSync interface test
     * @tc.desc getPreferencesSync interface test
     */
    it('SUB_DDM_Preferences_GetPreferencesSync_0400', 0, () => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '123456',
      };
      try {
        mPreferences = data_preferences.getPreferencesSync(globalThis.abilityContext, Options)
        console.info(TAG, `getPreferences successfully.`);
        expect(mPreferences !== null).assertTrue();
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail()
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_removePreferencesFromCacheSync_0100
     * @tc.name removePreferencesFromCacheSync interface test
     * @tc.desc removePreferencesFromCacheSync interface test
     */
    it('SUB_DDM_Preferences_removePreferencesFromCacheSync_0100', 0, () => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: null,
      };
      try {
        data_preferences.removePreferencesFromCacheSync(globalThis.abilityContext, Options)
        console.info(TAG, `removePreferencesFromCacheSync successfully.`);
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_removePreferencesFromCacheSync_0200
     * @tc.name removePreferencesFromCacheSync interface test
     * @tc.desc removePreferencesFromCacheSync interface test
     */
    it('SUB_DDM_Preferences_removePreferencesFromCacheSync_0200', 0, () => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: undefined,
      };
      try {
        data_preferences.removePreferencesFromCacheSync(globalThis.abilityContext, Options)
        console.info(TAG, `removePreferencesFromCacheSync successfully.`);
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_removePreferencesFromCacheSync_0300
     * @tc.name removePreferencesFromCacheSync interface test
     * @tc.desc removePreferencesFromCacheSync interface test
     */
    it('SUB_DDM_Preferences_removePreferencesFromCacheSync_0300', 0, () => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '123123',
      };
      try {
        data_preferences.removePreferencesFromCacheSync(globalThis.abilityContext, Options)
        expect().assertFail();
      } catch (errInfo) {
        console.info(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect(errInfo.code).assertEqual('15501002');
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_removePreferencesFromCacheSync_0400
     * @tc.name removePreferencesFromCacheSync interface test
     * @tc.desc removePreferencesFromCacheSync interface test
     */
    it('SUB_DDM_Preferences_removePreferencesFromCacheSync_0400', 0, () => {
      const Options = {
        name: "test_preferences_callback",
        dataGroupId: '123456',
      };
      try {
        data_preferences.removePreferencesFromCacheSync(globalThis.abilityContext, Options)
        console.info(TAG, `removePreferencesFromCacheSync successfully.`);
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
      * @tc.name isStorageTypeSupported
      * @tc.number SUB_DDM_AppDataFWK_JSPreferences_Preferences_StorageType_001
      * @tc.desc test StorageType
      */
    it('testPreferencesStorageType001', 0, () => {
      try {
        // normal storage type
        console.info(TAG + "enum value of xml: ${data_preferences.StorageType.XML}");
        console.info(TAG + "enum value of GSKV: ${data_preferences.StorageType.GSKV}");
        expect(0).assertEqual(data_preferences.StorageType.XML);
        expect(1).assertEqual(data_preferences.StorageType.GSKV);

        let isXmlSupported = data_preferences.isStorageTypeSupported(data_preferences.StorageType.XML);
        console.info(TAG + "isXmlSupported: ${isXmlSupported}");
        expect(isXmlSupported).assertTrue()

        let isGskvSupported = data_preferences.isStorageTypeSupported(data_preferences.StorageType.GSKV);
        console.info(TAG + "isGskvSupported: ${isGskvSupported}");
        expect(isGskvSupported).assertFalse()

        console.info("====>testPreferencesStorageType001 success: part1")
      } catch (err) {
          console.info("====>testPreferencesStorageType001 throw_err:" + JSON.stringify(err))
          expect().assertFail()
      }
      // invalid storage type
      try {
        let boolVar = data_preferences.isStorageTypeSupported(-1);
        expect().assertFail();
      } catch (err) {
        console.log("try catch err =" + err + ", code =" + err.code +", message =" + err.message);
        expect("401").assertEqual(err.code.toString());
        console.info("====>testPreferencesStorageType001 success: part2")
      }
      try {
        let boolVar = data_preferences.isStorageTypeSupported(2);
        expect().assertFail();
      } catch (err) {
        console.log("try catch err =" + err + ", code =" + err.code +", message =" + err.message);
        expect("401").assertEqual(err.code.toString());
        console.info("====>testPreferencesStorageType001 success: part3")
      }
    })

    /**
      * @tc.name GetPreferencesSync
      * @tc.number SUB_DDM_AppDataFWK_JSPreferences_Preferences_StorageType_002
      * @tc.desc test StorageType
      */
    it('testPreferencesStorageType002', 0, async() => {
      let OptionArray: data_preferences.Options[] = [
        { name: "default_storage_002_1" },
        { name: "default_storage_002_2", dataGroupId: null },
        { name: "default_storage_002_3", storageType: null },
        { name: "default_storage_002_4", storageType: 0 },
        { name: "default_storage_002_5", storageType: data_preferences.StorageType.XML }
      ]
      let spArray:data_preferences.Preferences[] = []
      for (let i = 0; i < OptionArray.length; i++) {
        try {
          spArray[i] = data_preferences.getPreferencesSync(context, OptionArray[i]);
          expect(spArray[i] != null).assertTrue();
          spArray[i].putSync("key", "value");
          await spArray[i].flush();
          data_preferences.removePreferencesFromCacheSync(context, OptionArray[i]);
        } catch (err) {
          console.error(TAG + "try put and flush catch err =" + err + ", code =" + err.code +
              ", message =" + err.message);
          expect().assertFail();
        }
      }
      for (let i = 0; i < OptionArray.length; i++) {
        if (!isFileExists(context.preferencesDir + '/' + OptionArray[i].name)) {
          console.error(TAG + 'check file failed: ' + context.preferencesDir + '/' +
              OptionArray[i].name);
          expect().assertFail();
        }
      }
    })

    /**
      * @tc.name GetPreferencesSync
      * @tc.number SUB_DDM_AppDataFWK_JSPreferences_Preferences_StorageType_004
      * @tc.desc test StorageType
      */
    it('testPreferencesStorageType004', 0, async () => {
      // not white list
      // when xml exists, open again, it should be ok and be xml
      // firstly create xml
      let sp = await data_preferences.getPreferences(context,
          {name: "storage_004_1", storageType: null});
      expect(sp != null).assertTrue();
      sp.putSync("key", "value");
      await sp.flush();
      data_preferences.removePreferencesFromCacheSync(context,
          {name: "storage_004_1", storageType: null});
      expect(isFileExists(context.preferencesDir + '/' + "storage_004_1")).assertTrue();

      // open again
      try {
        sp = await data_preferences.getPreferences(context,
            {name: "storage_004_1", storageType: null});
        expect(sp != null).assertTrue();
        data_preferences.removePreferencesFromCacheSync(context,
            {name: "storage_004_1", storageType: null});
      } catch (err) {
        console.log("try catch err =" + err + ", code =" + err.code +", message =" + err.message);
        expect().assertFail();
      }
    })

    /**
      * @tc.name GetPreferencesSync
      * @tc.number SUB_DDM_AppDataFWK_JSPreferences_Preferences_StorageType_005
      * @tc.desc test StorageType
      */
    it('testPreferencesStorageType005', 0, () => {
      // open with xml type but GSKV exists
      // it should return not supported
      // firstly create GSKV
      let isEnhance = data_preferences.isStorageTypeSupported(data_preferences.StorageType.GSKV);
      if (isEnhance) {
        const Options : data_preferences.Options = {
          name: "storage_005",
          storageType: data_preferences.StorageType.GSKV
        };
        let sp = data_preferences.getPreferencesSync(context, Options);
        expect(sp != null).assertTrue();
        data_preferences.removePreferencesFromCacheSync(context, Options);
        expect(isFileExists(context.preferencesDir + '/' + "storage_005.db")).assertTrue();
        const Options2 : data_preferences.Options = {
          name: "storage_005",
          storageType: data_preferences.StorageType.XML
        };

        // secondly, open with xml type
        try {
          sp = data_preferences.getPreferencesSync(context, Options2);
          expect(sp == null).assertTrue();
        } catch (err) {
          console.log("try catch err =" + err + ", code =" + err.code +", message =" + err.message);
          console.info("====>testPreferencesStorageType005 success")
        }
      }
    })

    /**
      * @tc.name GetPreferencesSync
      * @tc.number SUB_DDM_AppDataFWK_JSPreferences_Preferences_StorageType_006
      * @tc.desc test StorageType
      */
    it('testPreferencesStorageType006', 0, async () => {
      // open with GSKV type but xml exists
      // it should return not supported
      // firstly create xml
      const Options : data_preferences.Options = {
        name: "storage_006"
      }
      let sp = await data_preferences.getPreferences(context, Options);
      expect(sp != null).assertTrue();
      sp.putSync("key", "value");
      await sp.flush();
      data_preferences.removePreferencesFromCacheSync(context, Options);
      expect(isFileExists(context.preferencesDir + '/' + Options.name)).assertTrue();

      // open with GSKV
      const Options2 : data_preferences.Options = {
        name: "storage_006",
        storageType: data_preferences.StorageType.GSKV
      }
      try {
        sp = data_preferences.getPreferencesSync(context, Options2);
        expect(sp == null).assertTrue();
      } catch (err) {
        console.log("try catch err =" + err + ", code =" + err.code +", message =" + err.message);
        console.info("====>testPreferencesStorageType006 success")
      }
    })

    /**
      * @tc.name GetPreferencesSync
      * @tc.number SUB_DDM_AppDataFWK_JSPreferences_Preferences_StorageType_007
      * @tc.desc test StorageType
      */
    it('testPreferencesStorageType007', 0, () => {
      // open with GSKV type and open again
      // firstly create GSKV
      let isEnhance = data_preferences.isStorageTypeSupported(data_preferences.StorageType.GSKV);
      if (isEnhance) {
        try {
          const Options : data_preferences.Options = {
            name: "storage_007",
            storageType: data_preferences.StorageType.GSKV
          };
          let sp = data_preferences.getPreferencesSync(context, Options);
          expect(sp != null).assertTrue();
          data_preferences.removePreferencesFromCacheSync(context, Options);

          sp = data_preferences.getPreferencesSync(context, Options);
          expect(sp != null).assertTrue();
          data_preferences.removePreferencesFromCacheSync(context, Options);
        } catch (err) {
          console.log("try catch err =" + err + ", code =" + err.code +", message =" + err.message);
          expect().assertFail();
        }
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_PreferencesEl3_0100
     * @tc.name PreferencesSync interface test
     * @tc.desc PreferencesSync interface test
     */
    it('SUB_DDM_Preferences_PreferencesEl3_0100', 0, () => {
      console.info(TAG, 'SUB_DDM_Preferences_PreferencesEl3_0100 start.');
      const Options: data_preferences.Options = {
        name: "test_preferences_el3",
        dataGroupId: "123456",
      };
      try {
        let el3Ctx = context.createAreaModeContext(contextConstant.AreaMode.EL3);
        let pref = data_preferences.getPreferencesSync(el3Ctx, Options);
        pref.putSync("stringKey", "testValue");
        pref.putSync("intKey", 1);
        let stringVal = pref.getSync("stringKey", "defValue");
        expect("testValue").assertEqual(stringVal);
        pref.flushSync();
        data_preferences.removePreferencesFromCacheSync(el3Ctx, Options)
        pref = data_preferences.getPreferencesSync(el3Ctx, Options)
        let intVal = pref.getSync("intKey", 0);
        expect(1).assertEqual(intVal);
        pref.deleteSync("intKey");
        intVal = pref.getSync("intKey", 0);
        expect(0).assertEqual(intVal);
        pref.clearSync();
        expect(false).assertEqual(pref.hasSync("stringKey"));
        data_preferences.deletePreferences(el3Ctx, Options);
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_PreferencesEl4_0100
     * @tc.name PreferencesSync interface test
     * @tc.desc PreferencesSync interface test
     */
    it('SUB_DDM_Preferences_PreferencesEl4_0100', 0, () => {
      console.info(TAG, 'SUB_DDM_Preferences_PreferencesEl4_0100 start.');
      const Options: data_preferences.Options = {
        name: "test_preferences_el4",
        dataGroupId: "123456",
      };
      try {
        let el4Ctx = context.createAreaModeContext(contextConstant.AreaMode.EL4);
        let pref = data_preferences.getPreferencesSync(el4Ctx, Options);
        pref.putSync("stringKey", "testValue");
        pref.putSync("intKey", 1);
        let stringVal = pref.getSync("stringKey", "defValue");
        expect("testValue").assertEqual(stringVal);
        pref.flushSync();
        data_preferences.removePreferencesFromCacheSync(el4Ctx, Options)
        pref = data_preferences.getPreferencesSync(el4Ctx, Options)
        let intVal = pref.getSync("intKey", 0);
        expect(1).assertEqual(intVal);
        pref.deleteSync("intKey");
        intVal = pref.getSync("intKey", 0);
        expect(0).assertEqual(intVal);
        pref.clearSync();
        expect(false).assertEqual(pref.hasSync("stringKey"));
        data_preferences.deletePreferences(el4Ctx, Options);
      } catch (errInfo) {
        console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
        expect().assertFail();
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_PreferencesEl3_GSKV_0100
     * @tc.name PreferencesSync interface test
     * @tc.desc PreferencesSync interface test
     */
    it('SUB_DDM_Preferences_PreferencesEl3_GSKV_0100', 0, () => {
      console.info(TAG, 'SUB_DDM_Preferences_PreferencesEl3_GSKV_0100 start.');
      let isEnhance = data_preferences.isStorageTypeSupported(data_preferences.StorageType.GSKV);
      if (isEnhance) {
        const Options : data_preferences.Options = {
          name: "test_gskv_el3",
          dataGroupId: "123456",
          storageType: data_preferences.StorageType.GSKV
        };
        try {
          let el3Ctx = context.createAreaModeContext(contextConstant.AreaMode.EL3);
          let pref = data_preferences.getPreferencesSync(el3Ctx, Options);
          pref.putSync("stringKey", "testValue");
          pref.putSync("intKey", 1);
          let stringVal = pref.getSync("stringKey", "defValue");
          expect("testValue").assertEqual(stringVal);
          pref.flushSync();
          data_preferences.removePreferencesFromCacheSync(el3Ctx, Options)
          pref = data_preferences.getPreferencesSync(el3Ctx, Options)
          let intVal = pref.getSync("intKey", 0);
          expect(1).assertEqual(intVal);
          pref.deleteSync("intKey");
          intVal = pref.getSync("intKey", 0);
          expect(0).assertEqual(intVal);
          pref.clearSync();
          expect(false).assertEqual(pref.hasSync("stringKey"));
          data_preferences.deletePreferences(el3Ctx, Options);
        } catch (errInfo) {
          console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
          expect().assertFail();
        }
      }
    })

    /**
     * @tc.number SUB_DDM_Preferences_PreferencesEl4_GSKV_0100
     * @tc.name PreferencesSync interface test
     * @tc.desc PreferencesSync interface test
     */
    it('SUB_DDM_Preferences_PreferencesEl4_GSKV_0100', 0, () => {
      console.info(TAG, 'SUB_DDM_Preferences_PreferencesEl4_GSKV_0100 start.');
      let isEnhance = data_preferences.isStorageTypeSupported(data_preferences.StorageType.GSKV);
      if (isEnhance) {
        const Options : data_preferences.Options = {
          name: "test_gskv_el4",
          dataGroupId: "123456",
          storageType: data_preferences.StorageType.GSKV
        };
        try {
          let el4Ctx = context.createAreaModeContext(contextConstant.AreaMode.EL4);
          let pref = data_preferences.getPreferencesSync(el4Ctx, Options);
          pref.putSync("stringKey", "testValue");
          pref.putSync("intKey", 1);
          let stringVal = pref.getSync("stringKey", "defValue");
          expect("testValue").assertEqual(stringVal);
          pref.flushSync();
          data_preferences.removePreferencesFromCacheSync(el4Ctx, Options)
          pref = data_preferences.getPreferencesSync(el4Ctx, Options)
          let intVal = pref.getSync("intKey", 0);
          expect(1).assertEqual(intVal);
          pref.deleteSync("intKey");
          intVal = pref.getSync("intKey", 0);
          expect(0).assertEqual(intVal);
          pref.clearSync();
          expect(false).assertEqual(pref.hasSync("stringKey"));
          data_preferences.deletePreferences(el4Ctx, Options);
        } catch (errInfo) {
          console.error(TAG, `failed, code is ${errInfo.code}, message is ${errInfo.message}`);
          expect().assertFail();
        }
      }
    })
  })
}
