/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"),
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

/**
 * @file
 * @kit AVSessionKit
 */

import { AVCastPickerState } from './@ohos.multimedia.avCastPickerParam';

/**
 * A picker view to show availale input device list.
 * @struct { AVInputCastPicker }
 * @syscap SystemCapability.Multimedia.AVSession.AVInputCast
 * @atomicservice
 * @since 20
 */
@Component
export declare struct AVInputCastPicker {

  /**
   * Set the custom builder for the picker appearance.
   * If not set, system will show the default appearance for different device type.
   * @type { ? CustomBuilder }
   * @syscap SystemCapability.Multimedia.AVSession.AVInputCast
   * @atomicservice
   * @since 20
   */
  @Prop
  customPicker?: CustomBuilder;

  /**
   * Picker state change callback.
   * @type { ?OnPickerStateCallback }
   * @syscap SystemCapability.Multimedia.AVSession.AVInputCast
   * @atomicservice
   * @since 20
   */
  onStateChange?: OnPickerStateCallback;
}

/**
 * Callback for picker state
 *
 * @typedef { function } OnPickerStateCallback
 * @param { AVCastPickerState } state - the picker state.
 * @returns { void }
 * @syscap SystemCapability.Multimedia.AVSession.AVInputCast
 * @atomicservice
 * @since 20
 */
export type OnPickerStateCallback = (state: AVCastPickerState) => void;