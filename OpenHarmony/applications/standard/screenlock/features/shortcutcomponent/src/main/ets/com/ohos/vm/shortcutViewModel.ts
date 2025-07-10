/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import {Log} from '@ohos/common'
import Constants from '../common/constants'
import power from '@ohos.power'

const TAG = 'ScreenLock-ShortcutViewModel'

export default class ShortcutViewModel {
    onShortcutClick(shortcutType: string) {
        Log.showInfo(TAG, `onShortcutClick ${shortcutType}`)
        switch (shortcutType) {
            case Constants.CLICK_TYPE_SHUTDOWN:
                power.shutdown("shutdown_device")
                break;
            case Constants.CLICK_TYPE_REBOOT:
                power.rebootDevice("reboot_device")
                break;
            default:
                Log.showError(TAG, `${shortcutType} is not support`)
        }
    }
}