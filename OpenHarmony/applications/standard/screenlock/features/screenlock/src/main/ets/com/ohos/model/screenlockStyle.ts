/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import {Log, ReadConfigFile} from '@ohos/common'

const SCREENLOCK_MODE_FILE_NAME = "screenlock.json";
const TAG = 'ScreenLock-ScreenlockStyle';


export enum LockStyleMode {
    SlideScreenLock = 1,
    JournalScreenLock = 2,
    CustomScreenLock = 3
}

class ScreenlockStyle {
    private screenMode: LockStyleMode = LockStyleMode.SlideScreenLock

    setMode(mode: LockStyleMode): number {
        Log.showDebug(TAG, `setMode:${mode}`);
        return this.screenMode = mode
    }

    getMode(): number {
        return this.screenMode
    }

    readMode(): number{
        Log.showInfo(TAG, `readMode`);
        let that = this;
        try {
            ReadConfigFile(SCREENLOCK_MODE_FILE_NAME, (modeJson)=>{
                Log.showInfo(TAG, `ReadConfigFile content:` + JSON.stringify(modeJson));
                that.screenMode = modeJson.mode;
            });
        } catch(error) {
            Log.showInfo(TAG, `ReadConfigFile content error: ${error}`);
            this.screenMode = LockStyleMode.SlideScreenLock;
        }
        return this.screenMode;
    }
}

let screenlockStyle = new ScreenlockStyle()

export default screenlockStyle as ScreenlockStyle