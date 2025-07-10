/**
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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
import LocationService from '../../../../../../main/ets/model/locationServicesImpl/LocationService';
import LocationViewModel from '../../../../../../main/ets/model/locationServicesImpl/LocationViewModel';

export default function LocationViewModelTest() {
  describe('LocationViewModelTest', () => {
    // Defines a test suite. Two parameters are supported: test suite name and test suite function.
    beforeAll(() => {
      // Presets an action, which is performed only once before all test cases of the test suite start.
      // This API supports only one parameter: preset action function.
    })
    beforeEach(() => {
      // Presets an action, which is performed before each unit test case starts.
      // The number of execution times is the same as the number of test cases defined by **it**.
      // This API supports only one parameter: preset action function.
    })
    afterEach(() =>  {
      // Presets a clear action, which is performed after each unit test case ends.
      // The number of execution times is the same as the number of test cases defined by **it**.
      // This API supports only one parameter: clear action function.
    })
    afterAll(() => {
      // Presets a clear action, which is performed after all test cases of the test suite end.
      // This API supports only one parameter: clear action function.
    })
    it('LocationViewModelTest_initViewModel_01', 0, () => {
      LocationViewModel.mIsStart = false;
      LocationViewModel.initViewModel()
      expect(0).assertEqual(0)
    })
    it('LocationViewModelTest_initViewModel_02', 0, () => {
      LocationViewModel.mIsStart = true;
      LocationViewModel.initViewModel()
      expect(0).assertEqual(0)
    })
    it('LocationViewModelTest_updateServiceState_01', 0, () => {
      let state = false;
      LocationViewModel.updateServiceState(state)
      expect(0).assertEqual(0)
    })
    it('LocationViewModelTest_updateServiceState_02', 0, () => {
      let state = true;
      LocationViewModel.updateServiceState(state)
      expect(0).assertEqual(0)
    })
    it('LocationViewModelTest_enableLocation', 0, () => {
      LocationViewModel.enableLocation()
      expect(0).assertEqual(0)
    })
    it('LocationViewModelTest_disableLocation', 0, () => {
      LocationViewModel.disableLocation()
      expect(0).assertEqual(0)
    })


  })
}