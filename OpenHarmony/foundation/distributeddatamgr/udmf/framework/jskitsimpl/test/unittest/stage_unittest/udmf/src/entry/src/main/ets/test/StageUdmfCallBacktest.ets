/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
import unifiedDataChannel from '@ohos.data.unifiedDataChannel';
import uniformTypeDescriptor from '@ohos.data.uniformTypeDescriptor';

const TAG = "[udmfStageTest]"

export default function stageUdmfCallbackJsTest() {
  describe('stageUdmfCallbackJsTest', ():void =>{
    console.log(TAG + "*************Unit Test Begin*************");
    beforeAll(():void => {
      console.log('stageUdmfCallbackJsTest TestAbility context' + globalThis.abilityContext);
      console.info('beforeAll')
    })
    afterAll(():void =>{
      console.info('afterAll')
    })

    /**
     * @tc.number    : SUB_DistributedData_UDMF_SDK_UnifiedDataCallbackJsAPITest_0100
     * @tc.name      : unifiedDataChannel insertData callback interface test
     * @tc.desc      : unifiedDataChannel insertData callback interface test.
     * @tc.size      : MEDIUM
     * @tc.type      : Function
     * @tc.level     : Level 3
     */
    it('SUB_DistributedData_UDMF_SDK_UnifiedDataCallbackJsAPITest_0100', 0, (done) => {
      let options: unifiedDataChannel.Options = {}
      let plainText = new unifiedDataChannel.PlainText();
      plainText.textContent = 'this is textContent';
      let unifiedData = new unifiedDataChannel.UnifiedData(plainText);
      try {
        unifiedDataChannel.insertData(options, unifiedData, (err, data) => {
          if (err === undefined) {
            console.info(TAG + `data = ${data}, options = ${options}`);
            expect(err === undefined).assertFail();
            done();
          } else {
            console.error(TAG, `Failed, code is ${err.code},message is ${err.message} `);
            expect(err !== null).assertFail();
            done();
          }
        })
      } catch (error) {
        console.error(TAG, `Failed to insertData0100 code is ${error.code},message is ${error.message} `);
        expect(error.code).assertEqual('401');
        done();
      }
    })
  })

  console.log(TAG + "*************Unit Test End*************");
}