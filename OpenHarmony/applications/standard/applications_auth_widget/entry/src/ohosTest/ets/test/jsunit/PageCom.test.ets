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

import { describe, expect, it } from '@ohos/hypium';
import userAuth from '@ohos.userIAM.userAuth';
import LogUtils from '../../../../main/ets/common/utils/LogUtils';

const TAG = 'PageComTest';

export default function PageComTest() {
  describe('PageComTest', () => {
    it('clickEventOk_Input', 0, async (done: Function) => {
      const authParam : userAuth.AuthParam = {
        challenge: new Uint8Array([49, 49, 49, 49, 49, 49]),
        authType: [1, 2, 4],
        authTrustLevel: 10000,
      };
      const widgetParam : userAuth.WidgetParam = {
        title: '使用密码验证',
        windowMode: 1,
        navigationButtonText: '确认',
      };
      let userAuthInstance = await userAuth.getUserAuthInstance(authParam, widgetParam);
      userAuthInstance.on('result', {
        onResult (result) {
          LogUtils.info(TAG, 'userAuthInstance onResult: ' + JSON.stringify(result));
        }
      });
      expect(userAuthInstance).not().assertNull();
      done();
    })
  })
}