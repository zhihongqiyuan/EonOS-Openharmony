/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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

export default function stageUdmfPromiseJsTest() {
  describe('stageUdmfPromiseJsTest', function () {
    console.log(TAG + "*************Unit Test Begin*************");
    beforeAll(function() {
      console.log('stageUdmfPromiseJsTest TestAbility context' + globalThis.abilityContext);
      console.info('beforeAll')
    })
    afterAll(async function () {
      console.info('afterAll');
    })

    /**
      * @tc.number    : SUB_DistributedData_UDMF_SDK_UnifiedDataPromiseJsAPITest_0100
      * @tc.name      : unifiedDataChannel insertData promise interface test
      * @tc.desc      : unifiedDataChannel insertData promise interface test.
      * @tc.size      : MEDIUM
      * @tc.type      : Function
      * @tc.level     : Level 3
      */
    it('SUB_DistributedData_UDMF_SDK_UnifiedDataPromiseJsAPITest_0100', 0, (done) => {
      let options: unifiedDataChannel.Options = {}
      let plainText = new unifiedDataChannel.PlainText();
      plainText.textContent = 'this is textContent';
      let unifiedData = new unifiedDataChannel.UnifiedData(plainText);
      try {
        unifiedDataChannel.insertData(options, unifiedData).then((data) => {
          console.info(TAG + `data = ${data}, options = ${options}`);
          expect().assertFail();
          done();
        }).catch((err:BusinessError) => {
          console.error(TAG, `Failed, code is ${err.code},message is ${err.message} `);
          expect(err !== null).assertFail();
          done();
        })
      } catch (error) {
        console.error(TAG, `Failed to insertData0100 code is ${error.code},message is ${error.message} `);
        expect(error.code).assertEqual('401');
        done();
      }
    })
    console.log(TAG + "*************Unit Test End*************");
  })
}