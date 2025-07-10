/**
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

import util from '@ohos.util';
import userAuth from '@ohos.userIAM.userAuth';
import { DialogType } from '../module/DialogType';
import LogUtils from './LogUtils';
import window from '@ohos.window';
import UIExtensionContentSession from '@ohos.app.ability.UIExtensionContentSession';

const TAG = 'FuncUtils';

export class FuncUtils {
  getUint8PW(value: string): Uint8Array {
    let textEncoder = new util.TextEncoder();
    return textEncoder.encode(value);
  }

  getDialogType(type: Array<userAuth.UserAuthType>): DialogType {
    if (type) {
      if (type.includes(userAuth.UserAuthType.PIN)) {
        if (type.includes(userAuth.UserAuthType.FACE)) {
          if (type.includes(userAuth.UserAuthType.FINGERPRINT)) {
            return DialogType.ALL;
          }
          return DialogType.PIN_FACE;
        }
        if (type.includes(userAuth.UserAuthType.FINGERPRINT)) {
          return DialogType.PIN_FINGER;
        }
        return DialogType.PIN;
      }
      if (type.includes(userAuth.UserAuthType.FACE) && type.includes(userAuth.UserAuthType.FINGERPRINT)) {
        return DialogType.FACE_FINGER_NAVIGATION;
      }
      if (type.includes(userAuth.UserAuthType.FINGERPRINT)) {
        return DialogType.FINGER;
      }
      if (type.includes(userAuth.UserAuthType.FACE)) {
        return DialogType.FACE;
      }
    }
    return DialogType.PIN;
  }

  getWindowHeight(): void {
    LogUtils.info(TAG, 'getWindowHeight');
    try {
      window.on('systemBarTintChange', (data) => {
        LogUtils.debug(TAG, 'Succeeded in enabling the listener for window stage event changes. Data: ' +
        JSON.stringify(data));
        for (let i = 0; i < data.regionTint.length; i++) {
          let regionData = data.regionTint[i];
          if (regionData.region === undefined) {
            continue;
          } else if (regionData.type === window.WindowType.TYPE_STATUS_BAR) {
            AppStorage.SetOrCreate('SYSTEM_STATUS_BAR_HEIGHT', px2vp(regionData.region.height));
            continue;
          } else if (regionData.type === window.WindowType.TYPE_NAVIGATION_BAR) {
            AppStorage.SetOrCreate('SYSTEM_NAVIGATION_BAR_HEIGHT', px2vp(regionData.region.height));
            continue;
          }
        }
      });
    } catch (error) {
      LogUtils.error(TAG, 'Failed to enable the listener for window stage event changes. error: ' + error?.code);
    }
  }

  judgmentOverflow(value: number): void {
    if (value === Number.POSITIVE_INFINITY || value === Number.NEGATIVE_INFINITY) {
      LogUtils.error(TAG, 'judgmentOverflow spill code value: ' + value);
      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
    }
  }
}

let funcUtils = new FuncUtils();

export default funcUtils as FuncUtils;
