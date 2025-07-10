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
import Constants from '../../../../main/ets/common/vm/Constants';

export default function ConstantsTest() {
  describe('ConstantsTest', () => {
    it('constants_test', 0, (done: Function) => {
      expect(Constants.pinSix).assertEqual('PIN_SIX');
      expect(Constants.pinNumber).assertEqual('PIN_NUMBER');
      expect(Constants.pinMixed).assertEqual('PIN_MIXED');
      expect(Constants.userAuthWidgetMgrVersion).assertEqual(1);
      expect(Constants.userAuthWidgetMgrSuccess).assertEqual(0);
      expect(Constants.noticeVersion).assertEqual('1');
      expect(Constants.noticeTypePin).assertEqual('pin');
      expect(Constants.noticeTypeFace).assertEqual('face');
      expect(Constants.noticeTypeFinger).assertEqual('fingerprint');
      done();
    })
  })
}